/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "config.h"
#ifdef XKCP_has_OD_Keccak
// #define UT_OUTPUT

#include "KeccakOD.h"
#include "UT.h"
#include <inttypes.h>
#include <time.h>
#include <assert.h>

static int is_all_zeroes(const void *buf, size_t len)
{
    const unsigned char *p = (const unsigned char *)buf;

    for (size_t i = 0; i < len; ++i)
        if (p[i] != 0)
            return 0;
    return 1;
}

#ifdef XKCP_has_OD_Keccak_width1600
    #define inst KeccakWidth1600

        #define prefix SHAKE
        #include "testOD.inc"
        #undef prefix

        #define prefix TurboSHAKE
        #include "testOD.inc"
        #undef prefix

    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times2
        #define pinst KeccakWidth1600Times2
        #define parallelism 2

            #define pprefix SHAKETimes2
            #define prefix SHAKE
            #include "testParallelOD.inc"
            #undef pprefix
            #undef prefix

            #define pprefix TurboSHAKETimes2
            #define prefix TurboSHAKE
            #include "testParallelOD.inc"
            #undef pprefix
            #undef prefix

        #undef pinst
        #undef parallelism
    #endif

    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times4
        #define pinst KeccakWidth1600Times4
        #define parallelism 4

            #define pprefix SHAKETimes4
            #define prefix SHAKE
            #include "testParallelOD.inc"
            #undef pprefix
            #undef prefix

            #define pprefix TurboSHAKETimes4
            #define prefix TurboSHAKE
            #include "testParallelOD.inc"
            #undef pprefix
            #undef prefix

        #undef pinst
        #undef parallelism
    #endif

    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times8
        #define pinst KeccakWidth1600Times8
        #define parallelism 8

            #define pprefix SHAKETimes8
            #define prefix SHAKE
            #include "testParallelOD.inc"
            #undef pprefix
            #undef prefix

            #define pprefix TurboSHAKETimes8
            #define prefix TurboSHAKE
            #include "testParallelOD.inc"
            #undef pprefix
            #undef prefix

        #undef pinst
        #undef parallelism
    #endif

    #undef inst
#endif

#ifdef UT_OUTPUT
void writeTestOD(void)
{
    FILE *f;
    unsigned int rate;

    f = fopen("TestOD.txt", "w");
    assert(f != NULL);
    #ifdef XKCP_has_OD_Keccak_width1600
    SHAKE_writeTestOD(f, 160, 256);
    SHAKE_writeTestOD(f, 128, 512);
    TurboSHAKE_writeTestOD(f, 160, 256);
    TurboSHAKE_writeTestOD(f, 128, 512);
    #endif
    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times2
    if (KeccakP1600times2_GetFeatures()) {
        SHAKETimes2_writeTestParallelOD(f, 160, 256);
        SHAKETimes2_writeTestParallelOD(f, 128, 512);
        TurboSHAKETimes2_writeTestParallelOD(f, 160, 256);
        TurboSHAKETimes2_writeTestParallelOD(f, 128, 512);
    }
    #endif
    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times4
    if (KeccakP1600times4_GetFeatures()) {
        SHAKETimes4_writeTestParallelOD(f, 160, 256);
        SHAKETimes4_writeTestParallelOD(f, 128, 512);
        TurboSHAKETimes4_writeTestParallelOD(f, 160, 256);
        TurboSHAKETimes4_writeTestParallelOD(f, 128, 512);
    }
    #endif
    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times8
    if (KeccakP1600times8_GetFeatures()) {
        SHAKETimes8_writeTestParallelOD(f, 160, 256);
        SHAKETimes8_writeTestParallelOD(f, 128, 512);
        TurboSHAKETimes8_writeTestParallelOD(f, 160, 256);
        TurboSHAKETimes8_writeTestParallelOD(f, 128, 512);
    }
    #endif
    fclose(f);
}

static void displayByteStringWithTrailer(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length, unsigned char E)
{
    unsigned int i;

    fprintf(f, "%s (%d) ", synopsis, length);
    for(i=0; i<length; i++)
        fprintf(f, "%02x ", (unsigned int)data[i]);
    if (E > 0)
        fprintf(f, "--%02x \n", (unsigned int)E);
    else
        fprintf(f, "\n");
}

static void writeTestVector(uint32_t seed, KeccakWidth1600_ODInstance *od, size_t ilen, size_t olen)
{
    unsigned char input[200];
    unsigned char output[200];
    unsigned char E = ((ilen + olen) % 31) + 1;
    UT_generateArbitraryMaterial(input, ilen, seed);
    displayByteStringWithTrailer(stdout, "←", input, ilen, E);
    TurboSHAKE_OD_Duplexing(od, output, olen, input, ilen, E, 0);
    displayByteStringWithTrailer(stdout, "→", output, olen, 0);
}

static void writeTestVectors(unsigned int rho, unsigned int capacity)
{
    KeccakWidth1600_ODInstance od;
    uint32_t seed = 0x23A5BA9F;
    unsigned char key[16];
    const size_t maximum = rho;
    UT_generateArbitraryMaterial(key, 16, seed++);

    TurboSHAKE_OD_Initialize(&od, rho, capacity, key, 16);
    displayByteStringWithTrailer(stdout, "Key:", key, 16, 0);
    for(size_t ilen = 0; ilen <= maximum; ilen = (ilen < 200) ? ilen+1 : ((ilen < 1000) ? ilen+17 : ilen+97))
    {
        const size_t olen = 8;
        writeTestVector(seed++, &od, ilen, olen);
    }
    for(size_t olen = 0; olen <= maximum; olen = (olen < 200) ? olen+1 : ((olen < 1000) ? olen+19 : olen+87))
    {
        const size_t ilen = 0;
        writeTestVector(seed++, &od, ilen, olen);
    }

}
#endif

void testKeccakOD()
{
    #ifdef UT_OUTPUT
    writeTestOD();
    if (0) {
        printf("TurboSHAKE128-OD\n");
        writeTestVectors(160, 256);
        printf("TurboSHAKE256-OD\n");
        writeTestVectors(128, 512);
    }
    #endif

    #ifdef XKCP_has_OD_Keccak_width1600
    UT_startTest("(Turbo)SHAKE-OD", "");
    SHAKE_selfTestOD(160, 256, 0xe41a6ac7);
    SHAKE_selfTestOD(128, 512, 0x8abc181d);
    TurboSHAKE_selfTestOD(160, 256, 0x9ad3360d);
    TurboSHAKE_selfTestOD(128, 512, 0xbb3caa23);
    UT_endTest();
    #endif
    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times2
    if (KeccakP1600times2_GetFeatures()) {
        UT_startTest("(Turbo)SHAKE-OD", "2-way parallel OD");
        SHAKETimes2_selfTestParallelOD(160, 256, 0x86f54f95);
        SHAKETimes2_selfTestParallelOD(128, 512, 0x2301ce9f);
        TurboSHAKETimes2_selfTestParallelOD(160, 256, 0xea32a40b);
        TurboSHAKETimes2_selfTestParallelOD(128, 512, 0x21e8d340);
        UT_endTest();
    }
    #endif
    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times4
    if (KeccakP1600times4_GetFeatures()) {
        UT_startTest("(Turbo)SHAKE-OD", "4-way parallel OD");
        SHAKETimes4_selfTestParallelOD(160, 256, 0xd3ba7fd6);
        SHAKETimes4_selfTestParallelOD(128, 512, 0xc9087659);
        TurboSHAKETimes4_selfTestParallelOD(160, 256, 0xa45a0e02);
        TurboSHAKETimes4_selfTestParallelOD(128, 512, 0xc4f7861e);
        UT_endTest();
    }
    #endif
    #ifdef XKCP_has_ParallelOD_Keccak_width1600_times8
    if (KeccakP1600times8_GetFeatures()) {
        UT_startTest("(Turbo)SHAKE-OD", "8-way parallel OD");
        SHAKETimes8_selfTestParallelOD(160, 256, 0xdb996b4b);
        SHAKETimes8_selfTestParallelOD(128, 512, 0x62d09979);
        TurboSHAKETimes8_selfTestParallelOD(160, 256, 0x359ea412);
        TurboSHAKETimes8_selfTestParallelOD(128, 512, 0xb5997319);
        UT_endTest();
    }
    #endif
}

#endif /* XKCP_has_OD_Keccak */
