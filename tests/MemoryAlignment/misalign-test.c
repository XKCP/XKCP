/*
Misaligned-buffer test for XKCP.

Feeds deliberately misaligned input/output/key buffers (offsets 1..7 from
an aligned arena) through the public API and checks that the results match
the same computation on aligned buffers.

The long-message cases also reach the parallel implementations:
- KangarooTwelve and ParallelHash on 15 blocks of 8192 bytes plus 1000
  bytes, so that each of Keccak-p[1600]x8, x4 and x2 that the build has
  processes at least one group of blocks;
- Kravatte and Xoofff on 3004 bytes of input and output, for the same
  reason (Keccak-p[1600]x8, x4 and x2; Xoodoo x16, x8 and x4);
- keyed Xoodyak, for the full-block absorb, encrypt and decrypt paths.
On targets with runtime CPU dispatch, the XKCP --disable* options select the
code path, e.g. "misalign-test --disableAVX512 --disableAVX2". The test
prints the implementations that it uses.

Two failure modes it detects in unpatched code:
- Built with -fsanitize=alignment (GCC or Clang), a load or store through a
  misaligned uint32_t * or uint64_t * in the library is reported as a
  runtime error, on any architecture -- including x86-64 and AArch64,
  whose hardware would otherwise tolerate the access silently. Accesses
  made inside intrinsics (gathers, unaligned vector loads, and vld1_u64()
  with Clang) are not checked, so on those paths the test shows only that
  the code gives the right results on misaligned buffers.
- On strict-alignment hardware (e.g. 32-bit ARM), the unpatched library
  crashes with a bus error outright.
The test compares the library with itself, so it does not detect an error
that gives the same wrong result for aligned and misaligned buffers. The
unit tests check the results against known values.

Exit code 0 = all aligned/misaligned result pairs match.
*/

#include <stdio.h>
#include <string.h>
#include "config.h"
#include "align.h"
#include "SimpleFIPS202.h"
#include "KeccakHash.h"
#include "SP800-185.h"
#include "SnP-implementations.h"
#ifdef XKCP_has_Xoodyak
#include "Xoodyak.h"
#endif
#ifdef XKCP_has_KangarooTwelve
#include "KangarooTwelve.h"
#endif
#ifdef XKCP_has_Kravatte
#include "Kravatte.h"
#endif
#ifdef XKCP_has_Xoofff
/* In misalign-xoofff.c, as Xoofff.h and Kravatte.h cannot be included together. */
int misalignXoofff(const unsigned char *in, size_t inLen, const unsigned char *key, size_t keyLen, unsigned char *out, size_t outLen);
#endif
#ifdef XKCP_has_x86_64_CPU_detection
#include "x86-64-dispatch.h"
#define XKCP_has_CPU_detection
#endif
#ifdef XKCP_has_aarch64_CPU_detection
#include "aarch64-dispatch.h"
#define XKCP_has_CPU_detection
#endif

#define INLEN  1000
#define OUTLEN 136     /* > one rate block of SHAKE256, exercises ExtractLanes */
#define KEYLEN 32
#define KECCAKLEN (15*8192 + 1000)  /* KangarooTwelve and ParallelHash input: see above */
#define FARFALLELEN 3004            /* Kravatte and Xoofff input and output: see above */
#define LONGLEN KECCAKLEN
#define LONGOUT 3008                /* >= FARFALLELEN, a multiple of 64 so that each refLong row is aligned */
#define XOODYAK_KEYLEN 16
#define XOODYAK_ADLEN 200           /* > 2 blocks of Rkin = 44 bytes */
#define XOODYAK_PLEN 240            /* > 2 blocks of Rkout = 24 bytes */

ALIGN(64) static unsigned char arena_in[INLEN + 64];
ALIGN(64) static unsigned char arena_out[OUTLEN + 64];
ALIGN(64) static unsigned char arena_key[KEYLEN + 64];
ALIGN(64) static unsigned char arena_long[LONGLEN + 64];
ALIGN(64) static unsigned char arena_longout[LONGOUT + 64];

static int nFailed = 0;

static void check(const char *what, unsigned int offset,
                  const unsigned char *ref, const unsigned char *got, size_t len)
{
    if (memcmp(ref, got, len) != 0) {
        printf("MISMATCH: %s at offset %u\n", what, offset);
        nFailed++;
    }
}

#ifdef XKCP_has_Xoodyak
/* Keyed Xoodyak: absorb, encrypt and squeeze a tag, then decrypt the ciphertext
   (out of place) and squeeze the tag again. All of it is written to out. */
static int xoodyakKeyed(const unsigned char *in, const unsigned char *key, unsigned char *out)
{
    Xoodyak_Instance x;
    unsigned char *C = out, *tag = out + XOODYAK_PLEN;
    unsigned char *P = out + XOODYAK_PLEN + 16, *tag2 = P + XOODYAK_PLEN;

    if ((Xoodyak_Initialize(&x, key, XOODYAK_KEYLEN, NULL, 0, NULL, 0) != 0)
        || (Xoodyak_Absorb(&x, in, XOODYAK_ADLEN) != 0)
        || (Xoodyak_Encrypt(&x, in + XOODYAK_ADLEN, C, XOODYAK_PLEN) != 0)
        || (Xoodyak_Squeeze(&x, tag, 16) != 0))
        return 1;
    if ((Xoodyak_Initialize(&x, key, XOODYAK_KEYLEN, NULL, 0, NULL, 0) != 0)
        || (Xoodyak_Absorb(&x, in, XOODYAK_ADLEN) != 0)
        || (Xoodyak_Decrypt(&x, C, P, XOODYAK_PLEN) != 0)
        || (Xoodyak_Squeeze(&x, tag2, 16) != 0))
        return 1;
    return (memcmp(P, in + XOODYAK_ADLEN, XOODYAK_PLEN) != 0) || (memcmp(tag, tag2, 16) != 0);
}
#endif

/* The long-message cases: compute into out (LONGOUT bytes) from in (LONGLEN bytes). */
/* Returns 0 on success, -1 when case n is not compiled in. */
static int longCases(const unsigned char *in, const unsigned char *key, unsigned char *out, unsigned int n)
{
    memset(out, 0, LONGOUT);
    (void)in; (void)key;
    switch (n) {
#ifdef XKCP_has_KangarooTwelve
    case 0: return KT128(in, KECCAKLEN, out, 64, NULL, 0);
    case 1: return KT256(in, KECCAKLEN, out, 64, NULL, 0);
#endif
#ifdef XKCP_has_SP800_185
    case 2: return ParallelHash128(in, (BitLength)KECCAKLEN*8, 8192, out, 256, NULL, 0);
    case 3: return ParallelHash256(in, (BitLength)KECCAKLEN*8, 8192, out, 512, NULL, 0);
#endif
#ifdef XKCP_has_Kravatte
    case 4: {
        Kravatte_Instance kv;
        if (Kravatte_MaskDerivation(&kv, key, KEYLEN*8) != 0)
            return 1;
        return Kravatte(&kv, in, (BitLength)FARFALLELEN*8, out, (BitLength)FARFALLELEN*8, KRAVATTE_FLAG_INIT);
    }
#endif
#ifdef XKCP_has_Xoofff
    case 5: return misalignXoofff(in, FARFALLELEN, key, KEYLEN, out, FARFALLELEN);
#endif
#ifdef XKCP_has_Xoodyak
    case 6: return xoodyakKeyed(in, key, out);
#endif
    default: return -1;
    }
}

static const char *longCaseNames[] = { "KT128", "KT256", "ParallelHash128", "ParallelHash256", "Kravatte", "Xoofff", "Xoodyak keyed" };
#define NLONGCASES 7

int main(int argc, char *argv[])
{
    unsigned char refSHA3[32], refSHAKE[OUTLEN], refKMAC[32], refInc[32];
#ifdef XKCP_has_Xoodyak
    unsigned char refXoodyak[32];
#endif
    ALIGN(64) static unsigned char refLong[NLONGCASES][LONGOUT];
    int skipped[NLONGCASES];
    unsigned int offset, i, n;

    /* Line-buffered, so that the log keeps the output up to a crash. */
    setvbuf(stdout, NULL, _IOLBF, 0);
    for (i = 1; i < (unsigned int)argc; i++) {
#ifdef XKCP_has_CPU_detection
        if (XKCP_ProcessCpuFeatureCommandLineOption(argv[i]))
            continue;
#endif
        printf("Unrecognized option: %s\n", argv[i]);
        return 2;
    }
    XKCP_PrintImplementations();

    for (i = 0; i < INLEN; i++)
        arena_in[i] = (unsigned char)(i * 7 + 1);
    for (i = 0; i < KEYLEN; i++)
        arena_key[i] = (unsigned char)(0x40 + i);
    for (i = 0; i < LONGLEN; i++)
        arena_long[i] = (unsigned char)(i * 13 + 5);

    /* Reference results on aligned buffers (offset 0). */
    SHA3_256(refSHA3, arena_in, INLEN);
    SHAKE256(refSHAKE, OUTLEN, arena_in, INLEN);
    KMAC128(arena_key, KEYLEN*8, arena_in, INLEN*8, refKMAC, 256, NULL, 0);
    {
        Keccak_HashInstance h;
        Keccak_HashInitialize_SHA3_256(&h);
        Keccak_HashUpdate(&h, arena_in, 7*8);
        Keccak_HashUpdate(&h, arena_in + 7, (INLEN - 7)*8);
        Keccak_HashFinal(&h, refInc);
    }
#ifdef XKCP_has_Xoodyak
    {
        Xoodyak_Instance x;
        Xoodyak_Initialize(&x, NULL, 0, NULL, 0, NULL, 0);
        Xoodyak_Absorb(&x, arena_in, INLEN);
        Xoodyak_Squeeze(&x, refXoodyak, 32);
    }
#endif
    for (n = 0; n < NLONGCASES; n++) {
        int rc = longCases(arena_long, arena_key, refLong[n], n);
        skipped[n] = (rc < 0);
        if (skipped[n])
            printf("SKIPPED: %s is not in this build\n", longCaseNames[n]);
        else if (rc != 0) {
            printf("ERROR: %s failed on aligned buffers\n", longCaseNames[n]);
            nFailed++;
        }
    }

    for (offset = 1; offset < 8; offset++) {
        unsigned char *in  = arena_in  + offset;   /* misaligned input  */
        unsigned char *out = arena_out + offset;   /* misaligned output */
        unsigned char *key = arena_key + offset;   /* misaligned key    */
        unsigned char aref[64];

        memmove(in, arena_in, INLEN);              /* same content, shifted */
        memmove(key, arena_key, KEYLEN);

        /* one-shot hash, misaligned input and output */
        SHA3_256(out, in, INLEN);
        check("SHA3-256", offset, refSHA3, out, 32);

        /* XOF with long misaligned output (ExtractLanes path) */
        SHAKE256(out, OUTLEN, in, INLEN);
        check("SHAKE256", offset, refSHAKE, out, OUTLEN);

        /* MAC, all three buffers misaligned */
        KMAC128(key, KEYLEN*8, in, INLEN*8, out, 256, NULL, 0);
        check("KMAC128", offset, refKMAC, out, 32);

        /* incremental hashing with a non-lane-multiple first chunk,
           so the second chunk enters AddLanes/FastLoop misaligned */
        {
            Keccak_HashInstance h;
            Keccak_HashInitialize_SHA3_256(&h);
            Keccak_HashUpdate(&h, in, 7*8);
            Keccak_HashUpdate(&h, in + 7, (INLEN - 7)*8);
            Keccak_HashFinal(&h, aref);
            check("SHA3-256 incremental", offset, refInc, aref, 32);
        }

#ifdef XKCP_has_Xoodyak
        {
            Xoodyak_Instance x;
            Xoodyak_Initialize(&x, NULL, 0, NULL, 0, NULL, 0);
            Xoodyak_Absorb(&x, in, INLEN);
            Xoodyak_Squeeze(&x, out, 32);
            check("Xoodyak", offset, refXoodyak, out, 32);
        }
#endif

        /* long messages: misaligned input, key and output */
        {
            unsigned char *longIn  = arena_long    + offset;
            unsigned char *longOut = arena_longout + offset;

            memmove(longIn, arena_long, LONGLEN);
            for (n = 0; n < NLONGCASES; n++) {
                if (skipped[n])
                    continue;
                if (longCases(longIn, key, longOut, n) != 0) {
                    printf("ERROR: %s failed at offset %u\n", longCaseNames[n], offset);
                    nFailed++;
                }
                check(longCaseNames[n], offset, refLong[n], longOut, LONGOUT);
            }
            memmove(arena_long, longIn, LONGLEN);
        }

        memmove(arena_in, in, INLEN);              /* restore for next round */
        memmove(arena_key, key, KEYLEN);
    }

    if (nFailed == 0)
        printf("All misaligned-buffer results match the aligned references.\n");
    return nFailed != 0;
}
