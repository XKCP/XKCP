/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Gilles Van Assche, hereby denoted as "the implementer".
Ported to ARMv8A NEON by [contributor].
Optimized for ARMv8A NEON by avoiding GPR-to-SIMD transfers.

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements helper functions and FastLoop_Absorb for Keccak-p[1600]×2
using ARMv8A NEON intrinsics. The permutation itself is in assembly.
Please refer to PlSnP-documentation.h for more details.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arm_neon.h>
#include "align.h"
#include "KeccakP-1600-times2-SnP.h"

#include "brg_endian.h"
#include "load-store.h"
#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
#error Expecting a little-endian platform
#endif

/* Type alias for 128-bit vector */
typedef uint64x2_t V128;

#define laneIndex(instanceIndex, lanePosition) ((lanePosition)*2 + instanceIndex)

/* ---------------------------------------------------------------- */
/* NEON intrinsic mappings - optimized for ARMv8A */

#define ANDnu128(a, b)      vbicq_u64(b, a)           /* b AND NOT a */
#define LOAD128(a)          vld1q_u64((const uint64_t *)&(a))
#define STORE128(a, b)      vst1q_u64((uint64_t *)&(a), b)
#define XOR128(a, b)        veorq_u64(a, b)
#define XOReq128(a, b)      a = veorq_u64(a, b)
#define ZERO128()           vdupq_n_u64(0)

/* 
 * OPTIMIZED: Load two 64-bit values from different addresses into one 128-bit vector.
 * The pointers need not be aligned. With GCC, XKCP_load64() + vcreate_u64() gives
 * the same code as vld1_u64(); Clang loads the high lane directly (ld1 {v.d}[1]).
 */
static inline __attribute__((always_inline)) V128 LOAD6464(const unsigned char *hi_ptr, const unsigned char *lo_ptr) {
    uint64x1_t lo = vcreate_u64(XKCP_load64(lo_ptr));
    uint64x1_t hi = vcreate_u64(XKCP_load64(hi_ptr));
    return vcombine_u64(lo, hi);
}

/* Store low 64 bits */
static inline void STORE64L(uint64_t *a, V128 b) {
    vst1_u64(a, vget_low_u64(b));
}

/* Store high 64 bits */
static inline void STORE64H(uint64_t *a, V128 b) {
    vst1_u64(a, vget_high_u64(b));
}

/* 64-bit rotate left within 128-bit vector (both lanes) */
#define ROL64in128(a, o)    vorrq_u64(vshlq_n_u64(a, o), vshrq_n_u64(a, 64-(o)))

/* 
 * Per-lane 64-bit rotations by byte-aligned amounts using TBL (table lookup).
 * This is the correct NEON equivalent of SSSE3's _mm_shuffle_epi8.
 *
 * NOTE: vextq_u8 CANNOT be used here because it rotates the entire 128-bit
 * vector as a single unit, which mixes bytes across the two independent
 * 64-bit lanes in our parallel x2 state. TBL performs per-byte indexing
 * within the full 128-bit source, allowing independent per-lane operations.
 */
static const uint8_t ROL64in128_8_indices[16] __attribute__((aligned(16))) =
    {7,0,1,2,3,4,5,6, 15,8,9,10,11,12,13,14};
static const uint8_t ROL64in128_56_indices[16] __attribute__((aligned(16))) =
    {1,2,3,4,5,6,7,0, 9,10,11,12,13,14,15,8};

static inline __attribute__((always_inline)) V128 ROL64in128_8(V128 a) {
    /* Rotate left by 8 bits independently in each 64-bit lane */
    return vreinterpretq_u64_u8(vqtbl1q_u8(vreinterpretq_u8_u64(a),
        vld1q_u8(ROL64in128_8_indices)));
}

static inline __attribute__((always_inline)) V128 ROL64in128_56(V128 a) {
    /* Rotate left by 56 bits independently in each 64-bit lane */
    return vreinterpretq_u64_u8(vqtbl1q_u8(vreinterpretq_u8_u64(a),
        vld1q_u8(ROL64in128_56_indices)));
}

#define SnP_laneLengthInBytes 8

/* ---------------------------------------------------------------- */
/* 
 * OPTIMIZED: Pre-vectorized round constants.
 * Instead of using vdupq_n_u64() which broadcasts a scalar (requiring GPR load + dup),
 * we store constants as 128-bit vectors and load them directly with vld1q.
 * For parallel x2 states, both lanes get the same constant.
 */

/* Round constants as 128-bit vectors (same value in both lanes) */
static const uint64_t KeccakF1600RoundConstants_vec[24][2] __attribute__((aligned(16))) = {
    {0x0000000000000001ULL, 0x0000000000000001ULL},
    {0x0000000000008082ULL, 0x0000000000008082ULL},
    {0x800000000000808aULL, 0x800000000000808aULL},
    {0x8000000080008000ULL, 0x8000000080008000ULL},
    {0x000000000000808bULL, 0x000000000000808bULL},
    {0x0000000080000001ULL, 0x0000000080000001ULL},
    {0x8000000080008081ULL, 0x8000000080008081ULL},
    {0x8000000000008009ULL, 0x8000000000008009ULL},
    {0x000000000000008aULL, 0x000000000000008aULL},
    {0x0000000000000088ULL, 0x0000000000000088ULL},
    {0x0000000080008009ULL, 0x0000000080008009ULL},
    {0x000000008000000aULL, 0x000000008000000aULL},
    {0x000000008000808bULL, 0x000000008000808bULL},
    {0x800000000000008bULL, 0x800000000000008bULL},
    {0x8000000000008089ULL, 0x8000000000008089ULL},
    {0x8000000000008003ULL, 0x8000000000008003ULL},
    {0x8000000000008002ULL, 0x8000000000008002ULL},
    {0x8000000000000080ULL, 0x8000000000000080ULL},
    {0x000000000000800aULL, 0x000000000000800aULL},
    {0x800000008000000aULL, 0x800000008000000aULL},
    {0x8000000080008081ULL, 0x8000000080008081ULL},
    {0x8000000000008080ULL, 0x8000000000008080ULL},
    {0x0000000080000001ULL, 0x0000000080000001ULL},
    {0x8000000080008008ULL, 0x8000000080008008ULL}
};

/* Load round constant as 128-bit vector directly */
#define CONST128_RC(i)      vld1q_u64(KeccakF1600RoundConstants_vec[i])

/* ---------------------------------------------------------------- */
/* State variable declarations */

#define declareABCDE \
    V128 Aba, Abe, Abi, Abo, Abu; \
    V128 Aga, Age, Agi, Ago, Agu; \
    V128 Aka, Ake, Aki, Ako, Aku; \
    V128 Ama, Ame, Ami, Amo, Amu; \
    V128 Asa, Ase, Asi, Aso, Asu; \
    V128 Bba, Bbe, Bbi, Bbo, Bbu; \
    V128 Bga, Bge, Bgi, Bgo, Bgu; \
    V128 Bka, Bke, Bki, Bko, Bku; \
    V128 Bma, Bme, Bmi, Bmo, Bmu; \
    V128 Bsa, Bse, Bsi, Bso, Bsu; \
    V128 Ca, Ce, Ci, Co, Cu; \
    V128 Da, De, Di, Do, Du; \
    V128 Eba, Ebe, Ebi, Ebo, Ebu; \
    V128 Ega, Ege, Egi, Ego, Egu; \
    V128 Eka, Eke, Eki, Eko, Eku; \
    V128 Ema, Eme, Emi, Emo, Emu; \
    V128 Esa, Ese, Esi, Eso, Esu;

#define prepareTheta \
    Ca = XOR128(Aba, XOR128(Aga, XOR128(Aka, XOR128(Ama, Asa)))); \
    Ce = XOR128(Abe, XOR128(Age, XOR128(Ake, XOR128(Ame, Ase)))); \
    Ci = XOR128(Abi, XOR128(Agi, XOR128(Aki, XOR128(Ami, Asi)))); \
    Co = XOR128(Abo, XOR128(Ago, XOR128(Ako, XOR128(Amo, Aso)))); \
    Cu = XOR128(Abu, XOR128(Agu, XOR128(Aku, XOR128(Amu, Asu))));

#define copyFromState(X, state) \
    X##ba = LOAD128(state[ 0]); \
    X##be = LOAD128(state[ 1]); \
    X##bi = LOAD128(state[ 2]); \
    X##bo = LOAD128(state[ 3]); \
    X##bu = LOAD128(state[ 4]); \
    X##ga = LOAD128(state[ 5]); \
    X##ge = LOAD128(state[ 6]); \
    X##gi = LOAD128(state[ 7]); \
    X##go = LOAD128(state[ 8]); \
    X##gu = LOAD128(state[ 9]); \
    X##ka = LOAD128(state[10]); \
    X##ke = LOAD128(state[11]); \
    X##ki = LOAD128(state[12]); \
    X##ko = LOAD128(state[13]); \
    X##ku = LOAD128(state[14]); \
    X##ma = LOAD128(state[15]); \
    X##me = LOAD128(state[16]); \
    X##mi = LOAD128(state[17]); \
    X##mo = LOAD128(state[18]); \
    X##mu = LOAD128(state[19]); \
    X##sa = LOAD128(state[20]); \
    X##se = LOAD128(state[21]); \
    X##si = LOAD128(state[22]); \
    X##so = LOAD128(state[23]); \
    X##su = LOAD128(state[24]);

#define copyToState(state, X) \
    STORE128(state[ 0], X##ba); \
    STORE128(state[ 1], X##be); \
    STORE128(state[ 2], X##bi); \
    STORE128(state[ 3], X##bo); \
    STORE128(state[ 4], X##bu); \
    STORE128(state[ 5], X##ga); \
    STORE128(state[ 6], X##ge); \
    STORE128(state[ 7], X##gi); \
    STORE128(state[ 8], X##go); \
    STORE128(state[ 9], X##gu); \
    STORE128(state[10], X##ka); \
    STORE128(state[11], X##ke); \
    STORE128(state[12], X##ki); \
    STORE128(state[13], X##ko); \
    STORE128(state[14], X##ku); \
    STORE128(state[15], X##ma); \
    STORE128(state[16], X##me); \
    STORE128(state[17], X##mi); \
    STORE128(state[18], X##mo); \
    STORE128(state[19], X##mu); \
    STORE128(state[20], X##sa); \
    STORE128(state[21], X##se); \
    STORE128(state[22], X##si); \
    STORE128(state[23], X##so); \
    STORE128(state[24], X##su);

/* --- Theta Rho Pi Chi Iota Prepare-theta --- */
/* OPTIMIZED: Uses CONST128_RC for direct vector load of round constants */
#define thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    Da = XOR128(Cu, ROL64in128(Ce, 1)); \
    De = XOR128(Ca, ROL64in128(Ci, 1)); \
    Di = XOR128(Ce, ROL64in128(Co, 1)); \
    Do = XOR128(Ci, ROL64in128(Cu, 1)); \
    Du = XOR128(Co, ROL64in128(Ca, 1)); \
\
    XOReq128(A##ba, Da); \
    Bba = A##ba; \
    XOReq128(A##ge, De); \
    Bbe = ROL64in128(A##ge, 44); \
    XOReq128(A##ki, Di); \
    Bbi = ROL64in128(A##ki, 43); \
    E##ba = XOR128(Bba, ANDnu128(Bbe, Bbi)); \
    XOReq128(E##ba, CONST128_RC(i)); \
    Ca = E##ba; \
    XOReq128(A##mo, Do); \
    Bbo = ROL64in128(A##mo, 21); \
    E##be = XOR128(Bbe, ANDnu128(Bbi, Bbo)); \
    Ce = E##be; \
    XOReq128(A##su, Du); \
    Bbu = ROL64in128(A##su, 14); \
    E##bi = XOR128(Bbi, ANDnu128(Bbo, Bbu)); \
    Ci = E##bi; \
    E##bo = XOR128(Bbo, ANDnu128(Bbu, Bba)); \
    Co = E##bo; \
    E##bu = XOR128(Bbu, ANDnu128(Bba, Bbe)); \
    Cu = E##bu; \
\
    XOReq128(A##bo, Do); \
    Bga = ROL64in128(A##bo, 28); \
    XOReq128(A##gu, Du); \
    Bge = ROL64in128(A##gu, 20); \
    XOReq128(A##ka, Da); \
    Bgi = ROL64in128(A##ka, 3); \
    E##ga = XOR128(Bga, ANDnu128(Bge, Bgi)); \
    XOReq128(Ca, E##ga); \
    XOReq128(A##me, De); \
    Bgo = ROL64in128(A##me, 45); \
    E##ge = XOR128(Bge, ANDnu128(Bgi, Bgo)); \
    XOReq128(Ce, E##ge); \
    XOReq128(A##si, Di); \
    Bgu = ROL64in128(A##si, 61); \
    E##gi = XOR128(Bgi, ANDnu128(Bgo, Bgu)); \
    XOReq128(Ci, E##gi); \
    E##go = XOR128(Bgo, ANDnu128(Bgu, Bga)); \
    XOReq128(Co, E##go); \
    E##gu = XOR128(Bgu, ANDnu128(Bga, Bge)); \
    XOReq128(Cu, E##gu); \
\
    XOReq128(A##be, De); \
    Bka = ROL64in128(A##be, 1); \
    XOReq128(A##gi, Di); \
    Bke = ROL64in128(A##gi, 6); \
    XOReq128(A##ko, Do); \
    Bki = ROL64in128(A##ko, 25); \
    E##ka = XOR128(Bka, ANDnu128(Bke, Bki)); \
    XOReq128(Ca, E##ka); \
    XOReq128(A##mu, Du); \
    Bko = ROL64in128_8(A##mu); \
    E##ke = XOR128(Bke, ANDnu128(Bki, Bko)); \
    XOReq128(Ce, E##ke); \
    XOReq128(A##sa, Da); \
    Bku = ROL64in128(A##sa, 18); \
    E##ki = XOR128(Bki, ANDnu128(Bko, Bku)); \
    XOReq128(Ci, E##ki); \
    E##ko = XOR128(Bko, ANDnu128(Bku, Bka)); \
    XOReq128(Co, E##ko); \
    E##ku = XOR128(Bku, ANDnu128(Bka, Bke)); \
    XOReq128(Cu, E##ku); \
\
    XOReq128(A##bu, Du); \
    Bma = ROL64in128(A##bu, 27); \
    XOReq128(A##ga, Da); \
    Bme = ROL64in128(A##ga, 36); \
    XOReq128(A##ke, De); \
    Bmi = ROL64in128(A##ke, 10); \
    E##ma = XOR128(Bma, ANDnu128(Bme, Bmi)); \
    XOReq128(Ca, E##ma); \
    XOReq128(A##mi, Di); \
    Bmo = ROL64in128(A##mi, 15); \
    E##me = XOR128(Bme, ANDnu128(Bmi, Bmo)); \
    XOReq128(Ce, E##me); \
    XOReq128(A##so, Do); \
    Bmu = ROL64in128_56(A##so); \
    E##mi = XOR128(Bmi, ANDnu128(Bmo, Bmu)); \
    XOReq128(Ci, E##mi); \
    E##mo = XOR128(Bmo, ANDnu128(Bmu, Bma)); \
    XOReq128(Co, E##mo); \
    E##mu = XOR128(Bmu, ANDnu128(Bma, Bme)); \
    XOReq128(Cu, E##mu); \
\
    XOReq128(A##bi, Di); \
    Bsa = ROL64in128(A##bi, 62); \
    XOReq128(A##go, Do); \
    Bse = ROL64in128(A##go, 55); \
    XOReq128(A##ku, Du); \
    Bsi = ROL64in128(A##ku, 39); \
    E##sa = XOR128(Bsa, ANDnu128(Bse, Bsi)); \
    XOReq128(Ca, E##sa); \
    XOReq128(A##ma, Da); \
    Bso = ROL64in128(A##ma, 41); \
    E##se = XOR128(Bse, ANDnu128(Bsi, Bso)); \
    XOReq128(Ce, E##se); \
    XOReq128(A##se, De); \
    Bsu = ROL64in128(A##se, 2); \
    E##si = XOR128(Bsi, ANDnu128(Bso, Bsu)); \
    XOReq128(Ci, E##si); \
    E##so = XOR128(Bso, ANDnu128(Bsu, Bsa)); \
    XOReq128(Co, E##so); \
    E##su = XOR128(Bsu, ANDnu128(Bsa, Bse)); \
    XOReq128(Cu, E##su);

/* Round macros - two rounds at a time, alternating A and E state */
/* NOTE: 12-round permutation uses the LAST 12 round constants (12-23) */
#define rounds12 \
    thetaRhoPiChiIotaPrepareTheta(12, A, E) \
    thetaRhoPiChiIotaPrepareTheta(13, E, A) \
    thetaRhoPiChiIotaPrepareTheta(14, A, E) \
    thetaRhoPiChiIotaPrepareTheta(15, E, A) \
    thetaRhoPiChiIotaPrepareTheta(16, A, E) \
    thetaRhoPiChiIotaPrepareTheta(17, E, A) \
    thetaRhoPiChiIotaPrepareTheta(18, A, E) \
    thetaRhoPiChiIotaPrepareTheta(19, E, A) \
    thetaRhoPiChiIotaPrepareTheta(20, A, E) \
    thetaRhoPiChiIotaPrepareTheta(21, E, A) \
    thetaRhoPiChiIotaPrepareTheta(22, A, E) \
    thetaRhoPiChiIotaPrepareTheta(23, E, A)

#define rounds24 \
    thetaRhoPiChiIotaPrepareTheta( 0, A, E) \
    thetaRhoPiChiIotaPrepareTheta( 1, E, A) \
    thetaRhoPiChiIotaPrepareTheta( 2, A, E) \
    thetaRhoPiChiIotaPrepareTheta( 3, E, A) \
    thetaRhoPiChiIotaPrepareTheta( 4, A, E) \
    thetaRhoPiChiIotaPrepareTheta( 5, E, A) \
    thetaRhoPiChiIotaPrepareTheta( 6, A, E) \
    thetaRhoPiChiIotaPrepareTheta( 7, E, A) \
    thetaRhoPiChiIotaPrepareTheta( 8, A, E) \
    thetaRhoPiChiIotaPrepareTheta( 9, E, A) \
    thetaRhoPiChiIotaPrepareTheta(10, A, E) \
    thetaRhoPiChiIotaPrepareTheta(11, E, A) \
    thetaRhoPiChiIotaPrepareTheta(12, A, E) \
    thetaRhoPiChiIotaPrepareTheta(13, E, A) \
    thetaRhoPiChiIotaPrepareTheta(14, A, E) \
    thetaRhoPiChiIotaPrepareTheta(15, E, A) \
    thetaRhoPiChiIotaPrepareTheta(16, A, E) \
    thetaRhoPiChiIotaPrepareTheta(17, E, A) \
    thetaRhoPiChiIotaPrepareTheta(18, A, E) \
    thetaRhoPiChiIotaPrepareTheta(19, E, A) \
    thetaRhoPiChiIotaPrepareTheta(20, A, E) \
    thetaRhoPiChiIotaPrepareTheta(21, E, A) \
    thetaRhoPiChiIotaPrepareTheta(22, A, E) \
    thetaRhoPiChiIotaPrepareTheta(23, E, A)

/* ---------------------------------------------------------------- */
/* 
 * OPTIMIZED XOR_In macro using direct SIMD loads.
 * This is the key optimization - avoiding GPR-to-SIMD transfers.
 */
#define XOR_In_Fast(Xxx, curData0, curData1, argIndex) \
    XOReq128(Xxx, LOAD6464((curData1) + 8*(argIndex), (curData0) + 8*(argIndex)))

#if defined(__ARM_FEATURE_SHA3)
/* ---------------------------------------------------------------- */
/*
 * One Keccak-p[1600]x2 round using the ARMv8.4-A SHA3 instructions, expressed
 * with NEON intrinsics. This is a faithful, statement-for-statement transcription
 * of the verified mlkem-native x2 round loop (keccak_f1600_x2_v84a_aarch64_asm.S,
 * Becker-Kannwischer): s[] holds the 25 interleaved state lanes (s[i] = lane i of
 * both instances), t25..t31 are the kernel's scratch registers, and the exact
 * instruction order is preserved so the in-place register reuse stays correct.
 * Intrinsic semantics: veor3q(a,b,c)=a^b^c; vrax1q(a,b)=a^ROL(b,1);
 * vxarq(a,b,n)=ROR(a^b,n); vbcaxq(a,b,c)=a^(b&~c).
 *
 * Used by the absorb fast-loop so the whole sponge step stays register-resident
 * (no per-block state load/store), recovering the throughput the standalone
 * permute achieves.
 */
#define KeccakP1600times2_SHA3_Round(s, rcvec)                       \
do {                                                                 \
    V128 t25, t26, t27, t28, t29, t30, t31;                          \
    t30 = veor3q_u64(s[0], s[5], s[10]);                             \
    t29 = veor3q_u64(s[1], s[6], s[11]);                             \
    t28 = veor3q_u64(s[2], s[7], s[12]);                             \
    t27 = veor3q_u64(s[3], s[8], s[13]);                             \
    t26 = veor3q_u64(s[4], s[9], s[14]);                             \
    t30 = veor3q_u64(t30, s[15], s[20]);                             \
    t29 = veor3q_u64(t29, s[16], s[21]);                             \
    t28 = veor3q_u64(t28, s[17], s[22]);                             \
    t27 = veor3q_u64(t27, s[18], s[23]);                             \
    t26 = veor3q_u64(t26, s[19], s[24]);                             \
    t25 = vrax1q_u64(t30, t28);                                      \
    t28 = vrax1q_u64(t28, t26);                                      \
    t26 = vrax1q_u64(t26, t29);                                      \
    t29 = vrax1q_u64(t29, t27);                                      \
    t27 = vrax1q_u64(t27, t30);                                      \
    t30 = veorq_u64(s[0], t26);                                      \
    s[0]  = vxarq_u64(s[2],  t29, 2);                                \
    s[2]  = vxarq_u64(s[12], t29, 21);                               \
    s[12] = vxarq_u64(s[13], t28, 39);                               \
    s[13] = vxarq_u64(s[19], t27, 56);                               \
    s[19] = vxarq_u64(s[23], t28, 8);                                \
    s[23] = vxarq_u64(s[15], t26, 23);                               \
    s[15] = vxarq_u64(s[1],  t25, 63);                               \
    s[1]  = vxarq_u64(s[8],  t28, 9);                                \
    s[8]  = vxarq_u64(s[16], t25, 19);                               \
    s[16] = vxarq_u64(s[7],  t29, 58);                               \
    s[7]  = vxarq_u64(s[10], t26, 61);                               \
    s[10] = vxarq_u64(s[3],  t28, 36);                               \
    s[3]  = vxarq_u64(s[18], t28, 43);                               \
    s[18] = vxarq_u64(s[17], t29, 49);                               \
    s[17] = vxarq_u64(s[11], t25, 54);                               \
    s[11] = vxarq_u64(s[9],  t27, 44);                               \
    s[9]  = vxarq_u64(s[22], t29, 3);                                \
    s[22] = vxarq_u64(s[14], t27, 25);                               \
    s[14] = vxarq_u64(s[20], t26, 46);                               \
    s[20] = vxarq_u64(s[4],  t27, 37);                               \
    s[4]  = vxarq_u64(s[24], t27, 50);                               \
    s[24] = vxarq_u64(s[21], t25, 62);                               \
    s[21] = vxarq_u64(s[5],  t26, 28);                               \
    t27   = vxarq_u64(s[6],  t25, 20);                               \
    t31 = (rcvec);                                                   \
    s[5]  = vbcaxq_u64(s[10], s[7],  s[11]);                         \
    s[6]  = vbcaxq_u64(s[11], s[8],  s[7]);                          \
    s[7]  = vbcaxq_u64(s[7],  s[9],  s[8]);                          \
    s[8]  = vbcaxq_u64(s[8],  s[10], s[9]);                          \
    s[9]  = vbcaxq_u64(s[9],  s[11], s[10]);                         \
    s[10] = vbcaxq_u64(s[15], s[12], s[16]);                         \
    s[11] = vbcaxq_u64(s[16], s[13], s[12]);                         \
    s[12] = vbcaxq_u64(s[12], s[14], s[13]);                         \
    s[13] = vbcaxq_u64(s[13], s[15], s[14]);                         \
    s[14] = vbcaxq_u64(s[14], s[16], s[15]);                         \
    s[15] = vbcaxq_u64(s[20], s[17], s[21]);                         \
    s[16] = vbcaxq_u64(s[21], s[18], s[17]);                         \
    s[17] = vbcaxq_u64(s[17], s[19], s[18]);                         \
    s[18] = vbcaxq_u64(s[18], s[20], s[19]);                         \
    s[19] = vbcaxq_u64(s[19], s[21], s[20]);                         \
    s[20] = vbcaxq_u64(s[0],  s[22], s[1]);                          \
    s[21] = vbcaxq_u64(s[1],  s[23], s[22]);                         \
    s[22] = vbcaxq_u64(s[22], s[24], s[23]);                         \
    s[23] = vbcaxq_u64(s[23], s[0],  s[24]);                         \
    s[24] = vbcaxq_u64(s[24], s[1],  s[0]);                          \
    s[0]  = vbcaxq_u64(t30, s[2],  t27);                             \
    s[1]  = vbcaxq_u64(t27, s[3],  s[2]);                            \
    s[2]  = vbcaxq_u64(s[2],  s[4],  s[3]);                          \
    s[3]  = vbcaxq_u64(s[3],  t30,  s[4]);                           \
    s[4]  = vbcaxq_u64(s[4],  t27,  t30);                            \
    s[0]  = veorq_u64(s[0], t31);                                    \
} while (0)

/* 24 SHA3 rounds over a register-resident state s[25]. */
#define KeccakP1600times2_SHA3_Rounds24(s)             \
do {                                                   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(0));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(1));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(2));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(3));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(4));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(5));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(6));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(7));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(8));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(9));   \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(10));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(11));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(12));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(13));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(14));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(15));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(16));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(17));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(18));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(19));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(20));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(21));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(22));  \
    KeccakP1600times2_SHA3_Round(s, CONST128_RC(23));  \
} while (0)

/* Constant-index load/store so the compiler keeps s[] in NEON registers. */
#define KeccakP1600times2_LoadState(s, p) \
do { \
    s[ 0]=LOAD128((p)[ 0]); s[ 1]=LOAD128((p)[ 1]); s[ 2]=LOAD128((p)[ 2]); s[ 3]=LOAD128((p)[ 3]); s[ 4]=LOAD128((p)[ 4]); \
    s[ 5]=LOAD128((p)[ 5]); s[ 6]=LOAD128((p)[ 6]); s[ 7]=LOAD128((p)[ 7]); s[ 8]=LOAD128((p)[ 8]); s[ 9]=LOAD128((p)[ 9]); \
    s[10]=LOAD128((p)[10]); s[11]=LOAD128((p)[11]); s[12]=LOAD128((p)[12]); s[13]=LOAD128((p)[13]); s[14]=LOAD128((p)[14]); \
    s[15]=LOAD128((p)[15]); s[16]=LOAD128((p)[16]); s[17]=LOAD128((p)[17]); s[18]=LOAD128((p)[18]); s[19]=LOAD128((p)[19]); \
    s[20]=LOAD128((p)[20]); s[21]=LOAD128((p)[21]); s[22]=LOAD128((p)[22]); s[23]=LOAD128((p)[23]); s[24]=LOAD128((p)[24]); \
} while (0)
#define KeccakP1600times2_StoreState(p, s) \
do { \
    STORE128((p)[ 0],s[ 0]); STORE128((p)[ 1],s[ 1]); STORE128((p)[ 2],s[ 2]); STORE128((p)[ 3],s[ 3]); STORE128((p)[ 4],s[ 4]); \
    STORE128((p)[ 5],s[ 5]); STORE128((p)[ 6],s[ 6]); STORE128((p)[ 7],s[ 7]); STORE128((p)[ 8],s[ 8]); STORE128((p)[ 9],s[ 9]); \
    STORE128((p)[10],s[10]); STORE128((p)[11],s[11]); STORE128((p)[12],s[12]); STORE128((p)[13],s[13]); STORE128((p)[14],s[14]); \
    STORE128((p)[15],s[15]); STORE128((p)[16],s[16]); STORE128((p)[17],s[17]); STORE128((p)[18],s[18]); STORE128((p)[19],s[19]); \
    STORE128((p)[20],s[20]); STORE128((p)[21],s[21]); STORE128((p)[22],s[22]); STORE128((p)[23],s[23]); STORE128((p)[24],s[24]); \
} while (0)
#define KP1600t2_XorIn(s, c0, c1, j) \
    s[j] = veorq_u64(s[j], LOAD6464((c1) + 8*(j), (c0) + 8*(j)))
#endif /* __ARM_FEATURE_SHA3 */

/* ---------------------------------------------------------------- */
/* FastLoop_Absorb - 24 rounds */

size_t KeccakF1600times2_FastLoop_Absorb(
    KeccakP1600times2_states *states,
    unsigned int laneCount,
    unsigned int laneOffsetParallel,
    unsigned int laneOffsetSerial,
    const unsigned char *data,
    size_t dataByteLen)
{
#if defined(__ARM_FEATURE_SHA3)
    /* SHA3-instruction core. For the common rates (21 lanes = SHAKE128/ParallelHash128,
       17 lanes = SHA3-256/SHAKE256) the whole sponge step stays register-resident:
       the state is loaded once, then each block is XORed in and run through 24 SHA3
       rounds without touching memory, and stored once at the end. */
    if (laneCount == 21 || laneCount == 17) {
        const unsigned char *dataStart = data;
        const unsigned char *curData0 = data;
        const unsigned char *curData1 = data + laneOffsetParallel * SnP_laneLengthInBytes;
        V128 *statesAsLanes = (V128 *)states->A;
        V128 s[25];

        KeccakP1600times2_LoadState(s, statesAsLanes);
        if (laneCount == 21) {
            while (dataByteLen >= (laneOffsetParallel + 21) * SnP_laneLengthInBytes) {
                KP1600t2_XorIn(s, curData0, curData1,  0); KP1600t2_XorIn(s, curData0, curData1,  1);
                KP1600t2_XorIn(s, curData0, curData1,  2); KP1600t2_XorIn(s, curData0, curData1,  3);
                KP1600t2_XorIn(s, curData0, curData1,  4); KP1600t2_XorIn(s, curData0, curData1,  5);
                KP1600t2_XorIn(s, curData0, curData1,  6); KP1600t2_XorIn(s, curData0, curData1,  7);
                KP1600t2_XorIn(s, curData0, curData1,  8); KP1600t2_XorIn(s, curData0, curData1,  9);
                KP1600t2_XorIn(s, curData0, curData1, 10); KP1600t2_XorIn(s, curData0, curData1, 11);
                KP1600t2_XorIn(s, curData0, curData1, 12); KP1600t2_XorIn(s, curData0, curData1, 13);
                KP1600t2_XorIn(s, curData0, curData1, 14); KP1600t2_XorIn(s, curData0, curData1, 15);
                KP1600t2_XorIn(s, curData0, curData1, 16); KP1600t2_XorIn(s, curData0, curData1, 17);
                KP1600t2_XorIn(s, curData0, curData1, 18); KP1600t2_XorIn(s, curData0, curData1, 19);
                KP1600t2_XorIn(s, curData0, curData1, 20);
                KeccakP1600times2_SHA3_Rounds24(s);
                curData0 += laneOffsetSerial*8;
                curData1 += laneOffsetSerial*8;
                dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
            }
        } else { /* laneCount == 17 */
            while (dataByteLen >= (laneOffsetParallel + 17) * SnP_laneLengthInBytes) {
                KP1600t2_XorIn(s, curData0, curData1,  0); KP1600t2_XorIn(s, curData0, curData1,  1);
                KP1600t2_XorIn(s, curData0, curData1,  2); KP1600t2_XorIn(s, curData0, curData1,  3);
                KP1600t2_XorIn(s, curData0, curData1,  4); KP1600t2_XorIn(s, curData0, curData1,  5);
                KP1600t2_XorIn(s, curData0, curData1,  6); KP1600t2_XorIn(s, curData0, curData1,  7);
                KP1600t2_XorIn(s, curData0, curData1,  8); KP1600t2_XorIn(s, curData0, curData1,  9);
                KP1600t2_XorIn(s, curData0, curData1, 10); KP1600t2_XorIn(s, curData0, curData1, 11);
                KP1600t2_XorIn(s, curData0, curData1, 12); KP1600t2_XorIn(s, curData0, curData1, 13);
                KP1600t2_XorIn(s, curData0, curData1, 14); KP1600t2_XorIn(s, curData0, curData1, 15);
                KP1600t2_XorIn(s, curData0, curData1, 16);
                KeccakP1600times2_SHA3_Rounds24(s);
                curData0 += laneOffsetSerial*8;
                curData1 += laneOffsetSerial*8;
                dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
            }
        }
        KeccakP1600times2_StoreState(statesAsLanes, s);
        return curData0 - dataStart;
    } else {
        /* Other rates: AddLanesAll + the verified asm permute. */
        const unsigned char *dataStart = data;
        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            KeccakP1600times2_AddLanesAll(states, data, laneCount, laneOffsetParallel);
            KeccakP1600times2_PermuteAll_24rounds(states);
            data += laneOffsetSerial * SnP_laneLengthInBytes;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        return data - dataStart;
    }
#else
    if (laneCount == 21) {
        /* Optimized path for rate 168 bytes (21 lanes) - SHAKE128/ParallelHash128 */
        const unsigned char *dataStart = data;
        const unsigned char *curData0 = data;
        const unsigned char *curData1 = data + laneOffsetParallel * SnP_laneLengthInBytes;
        V128 *statesAsLanes = (V128 *)states->A;
        declareABCDE

        copyFromState(A, statesAsLanes)
        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            XOR_In_Fast(Aba, curData0, curData1,  0);
            XOR_In_Fast(Abe, curData0, curData1,  1);
            XOR_In_Fast(Abi, curData0, curData1,  2);
            XOR_In_Fast(Abo, curData0, curData1,  3);
            XOR_In_Fast(Abu, curData0, curData1,  4);
            XOR_In_Fast(Aga, curData0, curData1,  5);
            XOR_In_Fast(Age, curData0, curData1,  6);
            XOR_In_Fast(Agi, curData0, curData1,  7);
            XOR_In_Fast(Ago, curData0, curData1,  8);
            XOR_In_Fast(Agu, curData0, curData1,  9);
            XOR_In_Fast(Aka, curData0, curData1, 10);
            XOR_In_Fast(Ake, curData0, curData1, 11);
            XOR_In_Fast(Aki, curData0, curData1, 12);
            XOR_In_Fast(Ako, curData0, curData1, 13);
            XOR_In_Fast(Aku, curData0, curData1, 14);
            XOR_In_Fast(Ama, curData0, curData1, 15);
            XOR_In_Fast(Ame, curData0, curData1, 16);
            XOR_In_Fast(Ami, curData0, curData1, 17);
            XOR_In_Fast(Amo, curData0, curData1, 18);
            XOR_In_Fast(Amu, curData0, curData1, 19);
            XOR_In_Fast(Asa, curData0, curData1, 20);
            prepareTheta
            rounds24
            curData0 += laneOffsetSerial*8;
            curData1 += laneOffsetSerial*8;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        copyToState(statesAsLanes, A)
        return curData0 - dataStart;
    }
    else if (laneCount == 17) {
        /* Optimized path for rate 136 bytes (17 lanes) - SHA3-256/SHAKE256 */
        const unsigned char *dataStart = data;
        const unsigned char *curData0 = data;
        const unsigned char *curData1 = data + laneOffsetParallel * SnP_laneLengthInBytes;
        V128 *statesAsLanes = (V128 *)states->A;
        declareABCDE

        copyFromState(A, statesAsLanes)
        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            XOR_In_Fast(Aba, curData0, curData1,  0);
            XOR_In_Fast(Abe, curData0, curData1,  1);
            XOR_In_Fast(Abi, curData0, curData1,  2);
            XOR_In_Fast(Abo, curData0, curData1,  3);
            XOR_In_Fast(Abu, curData0, curData1,  4);
            XOR_In_Fast(Aga, curData0, curData1,  5);
            XOR_In_Fast(Age, curData0, curData1,  6);
            XOR_In_Fast(Agi, curData0, curData1,  7);
            XOR_In_Fast(Ago, curData0, curData1,  8);
            XOR_In_Fast(Agu, curData0, curData1,  9);
            XOR_In_Fast(Aka, curData0, curData1, 10);
            XOR_In_Fast(Ake, curData0, curData1, 11);
            XOR_In_Fast(Aki, curData0, curData1, 12);
            XOR_In_Fast(Ako, curData0, curData1, 13);
            XOR_In_Fast(Aku, curData0, curData1, 14);
            XOR_In_Fast(Ama, curData0, curData1, 15);
            XOR_In_Fast(Ame, curData0, curData1, 16);
            prepareTheta
            rounds24
            curData0 += laneOffsetSerial*8;
            curData1 += laneOffsetSerial*8;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        copyToState(statesAsLanes, A)
        return curData0 - dataStart;
    }
    else {
        /* General case - use assembly permutation */
        const unsigned char *dataStart = data;

        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            KeccakP1600times2_AddLanesAll(states, data, laneCount, laneOffsetParallel);
            KeccakP1600times2_PermuteAll_24rounds(states);
            data += laneOffsetSerial * SnP_laneLengthInBytes;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        return data - dataStart;
    }
#endif /* __ARM_FEATURE_SHA3 */
}

/* ---------------------------------------------------------------- */
/* FastLoop_Absorb - 12 rounds (for TurboSHAKE/KangarooTwelve) */

size_t KeccakP1600times2_12rounds_FastLoop_Absorb(
    KeccakP1600times2_states *states,
    unsigned int laneCount,
    unsigned int laneOffsetParallel,
    unsigned int laneOffsetSerial,
    const unsigned char *data,
    size_t dataByteLen)
{
    if (laneCount == 21) {
        /* Optimized path for rate 168 bytes (21 lanes) - K12/TurboSHAKE128 */
        const unsigned char *dataStart = data;
        const unsigned char *curData0 = data;
        const unsigned char *curData1 = data + laneOffsetParallel * SnP_laneLengthInBytes;
        V128 *statesAsLanes = (V128 *)states->A;
        declareABCDE

        copyFromState(A, statesAsLanes)
        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            XOR_In_Fast(Aba, curData0, curData1,  0);
            XOR_In_Fast(Abe, curData0, curData1,  1);
            XOR_In_Fast(Abi, curData0, curData1,  2);
            XOR_In_Fast(Abo, curData0, curData1,  3);
            XOR_In_Fast(Abu, curData0, curData1,  4);
            XOR_In_Fast(Aga, curData0, curData1,  5);
            XOR_In_Fast(Age, curData0, curData1,  6);
            XOR_In_Fast(Agi, curData0, curData1,  7);
            XOR_In_Fast(Ago, curData0, curData1,  8);
            XOR_In_Fast(Agu, curData0, curData1,  9);
            XOR_In_Fast(Aka, curData0, curData1, 10);
            XOR_In_Fast(Ake, curData0, curData1, 11);
            XOR_In_Fast(Aki, curData0, curData1, 12);
            XOR_In_Fast(Ako, curData0, curData1, 13);
            XOR_In_Fast(Aku, curData0, curData1, 14);
            XOR_In_Fast(Ama, curData0, curData1, 15);
            XOR_In_Fast(Ame, curData0, curData1, 16);
            XOR_In_Fast(Ami, curData0, curData1, 17);
            XOR_In_Fast(Amo, curData0, curData1, 18);
            XOR_In_Fast(Amu, curData0, curData1, 19);
            XOR_In_Fast(Asa, curData0, curData1, 20);
            prepareTheta
            rounds12
            curData0 += laneOffsetSerial*8;
            curData1 += laneOffsetSerial*8;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        copyToState(statesAsLanes, A)
        return curData0 - dataStart;
    }
    else if (laneCount == 17) {
        /* Optimized path for rate 136 bytes (17 lanes) - TurboSHAKE256 */
        const unsigned char *dataStart = data;
        const unsigned char *curData0 = data;
        const unsigned char *curData1 = data + laneOffsetParallel * SnP_laneLengthInBytes;
        V128 *statesAsLanes = (V128 *)states->A;
        declareABCDE

        copyFromState(A, statesAsLanes)
        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            XOR_In_Fast(Aba, curData0, curData1,  0);
            XOR_In_Fast(Abe, curData0, curData1,  1);
            XOR_In_Fast(Abi, curData0, curData1,  2);
            XOR_In_Fast(Abo, curData0, curData1,  3);
            XOR_In_Fast(Abu, curData0, curData1,  4);
            XOR_In_Fast(Aga, curData0, curData1,  5);
            XOR_In_Fast(Age, curData0, curData1,  6);
            XOR_In_Fast(Agi, curData0, curData1,  7);
            XOR_In_Fast(Ago, curData0, curData1,  8);
            XOR_In_Fast(Agu, curData0, curData1,  9);
            XOR_In_Fast(Aka, curData0, curData1, 10);
            XOR_In_Fast(Ake, curData0, curData1, 11);
            XOR_In_Fast(Aki, curData0, curData1, 12);
            XOR_In_Fast(Ako, curData0, curData1, 13);
            XOR_In_Fast(Aku, curData0, curData1, 14);
            XOR_In_Fast(Ama, curData0, curData1, 15);
            XOR_In_Fast(Ame, curData0, curData1, 16);
            prepareTheta
            rounds12
            curData0 += laneOffsetSerial*8;
            curData1 += laneOffsetSerial*8;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        copyToState(statesAsLanes, A)
        return curData0 - dataStart;
    }
    else {
        /* General case - use assembly permutation */
        const unsigned char *dataStart = data;

        while (dataByteLen >= (laneOffsetParallel + laneCount) * SnP_laneLengthInBytes) {
            KeccakP1600times2_AddLanesAll(states, data, laneCount, laneOffsetParallel);
            KeccakP1600times2_PermuteAll_12rounds(states);
            data += laneOffsetSerial * SnP_laneLengthInBytes;
            dataByteLen -= laneOffsetSerial * SnP_laneLengthInBytes;
        }
        return data - dataStart;
    }
}
