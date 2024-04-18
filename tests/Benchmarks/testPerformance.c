/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#ifdef XKCP_has_KangarooTwelve
#include "KangarooTwelve.h"
#endif
#ifdef XKCP_has_Sponge_Keccak
#include "KeccakSponge.h"
#endif
#ifdef XKCP_has_Kravatte
#include "Kravatte.h"
#include "KravatteModes.h"
#endif
#ifdef XKCP_has_SP800_185
#include "SP800-185.h"
#endif
#include "timing.h"
#include "testPerformance.h"

ALIGN_DEFAULT uint8_t bigBuffer1[BIG_BUFFER_SIZE];
ALIGN_DEFAULT uint8_t bigBuffer2[BIG_BUFFER_SIZE];

void displayMeasurements1101001000(cycles_t *measurements, uint32_t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes);

#define xstr(s) str(s)
#define str(s) #s

#ifdef XKCP_has_KeccakP200
    #include "KeccakP-200-SnP.h"

    #define prefix KeccakP200
    #define SnP KeccakP200
    #define SnP_width 200
    #define SnP_Permute KeccakP200_Permute_18rounds
    #define SnP_Permute_Nrounds KeccakP200_Permute_Nrounds
    #if defined(KeccakF200_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF200_FastLoop_Absorb
    #endif
        #include "timingSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_Nrounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP400
    #include "KeccakP-400-SnP.h"

    #define prefix KeccakP400
    #define SnP KeccakP400
    #define SnP_width 400
    #define SnP_Permute KeccakP400_Permute_20rounds
    #define SnP_Permute_Nrounds KeccakP400_Permute_Nrounds
    #if defined(KeccakF400_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF400_FastLoop_Absorb
    #endif
        #include "timingSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_Nrounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"

    #define prefix KeccakP800
    #define SnP KeccakP800
    #define SnP_width 800
    #define SnP_Permute KeccakP800_Permute_22rounds
    #define SnP_Permute_12rounds KeccakP800_Permute_12rounds
    #if defined(KeccakF800_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF800_FastLoop_Absorb
    #endif
        #include "timingSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakP1600
    #define SnP KeccakP1600
    #define SnP_state KeccakP1600_state
    #define SnP_width 1600
    #define SnP_Permute KeccakP1600_Permute_24rounds
    #define SnP_Permute_12rounds KeccakP1600_Permute_12rounds
    #if defined(KeccakF1600_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF1600_FastLoop_Absorb
    #endif
        #include "timingSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_state
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef KeccakF800_FastLoop_supported
cycles_t KeccakP800_measureSnP_FastLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount);

void KeccakP800_gatherSnP_FastLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts)
{
    measurements[ 0] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 16, 1);
    measurements[ 1] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 16, 10);
    measurements[ 2] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 16, 100);
    measurements[ 3] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 16, 1000);
    measurements[ 4] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 17, 1);
    measurements[ 5] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 17, 10);
    measurements[ 6] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 17, 100);
    measurements[ 7] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 17, 1000);
    measurements[ 8] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 19, 1);
    measurements[ 9] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 19, 10);
    measurements[10] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 19, 100);
    measurements[11] = KeccakP800_measureSnP_FastLoop_Absorb(dtMin, 19, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 19;
}
#endif

#ifdef KeccakF1600_FastLoop_supported
cycles_t KeccakP1600_measureSnP_FastLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount);

void KeccakP1600_gatherSnP_FastLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts)
{
    measurements[ 0] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 16, 1);
    measurements[ 1] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 16, 10);
    measurements[ 2] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 16, 100);
    measurements[ 3] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 16, 1000);
    measurements[ 4] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 17, 1);
    measurements[ 5] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 17, 10);
    measurements[ 6] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 17, 100);
    measurements[ 7] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 17, 1000);
    measurements[ 8] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 21, 1);
    measurements[ 9] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 21, 10);
    measurements[10] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 21, 100);
    measurements[11] = KeccakP1600_measureSnP_FastLoop_Absorb(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
}
#endif

#ifdef XKCP_has_KeccakP200
cycles_t KeccakP200_measureSnP_GenericLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP200_gatherSnP_GenericLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts)
{
    measurements[ 0] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 1);
    measurements[ 1] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 10);
    measurements[ 2] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 100);
    measurements[ 3] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 1000);
    laneCounts[0] =  5;
    return 1;
}
#endif

#ifdef XKCP_has_KeccakP400
cycles_t KeccakP400_measureSnP_GenericLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP400_gatherSnP_GenericLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts)
{
    measurements[ 0] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin,  9, 1);
    measurements[ 1] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin,  9, 10);
    measurements[ 2] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin,  9, 100);
    measurements[ 3] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin,  9, 1000);
    measurements[ 4] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 13, 1);
    measurements[ 5] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 13, 10);
    measurements[ 6] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 13, 100);
    measurements[ 7] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 13, 1000);
    measurements[ 8] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 15, 1);
    measurements[ 9] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 15, 10);
    measurements[10] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 15, 100);
    measurements[11] = KeccakP400_measureSnP_GenericLoop_Absorb(dtMin, 15, 1000);
    laneCounts[0] =  9;
    laneCounts[1] = 13;
    laneCounts[2] = 15;
    return 3;
}
#endif

#ifdef XKCP_has_KeccakP800
cycles_t KeccakP800_measureSnP_GenericLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP800_gatherSnP_GenericLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts)
{
    measurements[ 0] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 16, 1);
    measurements[ 1] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 16, 10);
    measurements[ 2] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 16, 100);
    measurements[ 3] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 16, 1000);
    measurements[ 4] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 17, 1);
    measurements[ 5] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 17, 10);
    measurements[ 6] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 17, 100);
    measurements[ 7] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 17, 1000);
    measurements[ 8] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 19, 1);
    measurements[ 9] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 19, 10);
    measurements[10] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 19, 100);
    measurements[11] = KeccakP800_measureSnP_GenericLoop_Absorb(dtMin, 19, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 19;
    return 3;
}
#endif

#ifdef XKCP_has_KeccakP1600
cycles_t KeccakP1600_measureSnP_GenericLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP1600_gatherSnP_GenericLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts)
{
    measurements[ 0] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 16, 1);
    measurements[ 1] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 16, 10);
    measurements[ 2] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 16, 100);
    measurements[ 3] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 16, 1000);
    measurements[ 4] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 17, 1);
    measurements[ 5] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 17, 10);
    measurements[ 6] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 17, 100);
    measurements[ 7] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 17, 1000);
    measurements[ 8] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 21, 1);
    measurements[ 9] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 21, 10);
    measurements[10] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 21, 100);
    measurements[11] = KeccakP1600_measureSnP_GenericLoop_Absorb(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
    return 3;
}
#endif

#ifdef XKCP_has_KeccakP1600times2
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeccakP1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define SnP_width                   1600
    #define PlSnP_PermuteAll            KeccakP1600times2_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times2_PermuteAll_12rounds
    #if defined(KeccakF1600times2_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF1600times2_FastLoop_Absorb
    #endif
        #include "timingPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP1600times4
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      KeccakP1600times4
    #define PlSnP                       KeccakP1600times4
    #define PlSnP_parallelism           4
    #define SnP_width                   1600
    #define PlSnP_PermuteAll            KeccakP1600times4_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times4_PermuteAll_12rounds
    #if defined(KeccakF1600times4_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF1600times4_FastLoop_Absorb
    #endif
        #include "timingPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP1600times8
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      KeccakP1600times8
    #define PlSnP                       KeccakP1600times8
    #define PlSnP_parallelism           8
    #define SnP_width                   1600
    #define PlSnP_PermuteAll            KeccakP1600times8_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times8_PermuteAll_12rounds
    #if defined(KeccakF1600times8_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF1600times8_FastLoop_Absorb
    #endif
        #include "timingPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_Sponge_Keccak_width800
    #define prefix      KeccakWidth800
        #include "timingSponge.inc"
    #undef prefix

void KeccakWidth800_timing()
{
    cycles_t calibartion = KeccakWidth800_start("Keccak sponge functions using Keccak-f[800]", KeccakP800_implementation);
    KeccakWidth800_timingRC(calibartion, 544, 256);
    KeccakWidth800_timingRC(calibartion, 608, 192);
    printf("\n\n");
}
#endif

#ifdef XKCP_has_Sponge_Keccak_width1600
    #define prefix      KeccakWidth1600
        #include "timingSponge.inc"
    #undef prefix

void KeccakWidth1600_timing()
{
    cycles_t calibartion = KeccakWidth1600_start("Keccak sponge functions using Keccak-f[1600]", KeccakP1600_implementation);
    KeccakWidth1600_timingRC(calibartion, 576, 1024);
    KeccakWidth1600_timingRC(calibartion, 832,  768);
    KeccakWidth1600_timingRC(calibartion, 1088, 512);
    KeccakWidth1600_timingRC(calibartion, 1152, 448);
    KeccakWidth1600_timingRC(calibartion, 1344, 256);
    printf("\n\n");
}
#endif

#ifdef XKCP_has_TurboSHAKE
XKCP_DeclareSpongeFunctions(TurboSHAKE)

    #define prefix      TurboSHAKE
        #include "timingSponge.inc"
    #undef prefix

void TurboSHAKE_timing()
{
    cycles_t calibartion = TurboSHAKE_start("TurboSHAKE", KeccakP1600_implementation);
    TurboSHAKE_timingRCx(calibartion, 1088, 512, "TurboSHAKE256      ");
    TurboSHAKE_timingRCx(calibartion, 1344, 256, "TurboSHAKE128      ");
    printf("\n\n");
}
#endif

void printParallelImplementations(
    int useKeccakF1600timesN_FastLoop_Absorb,
    int useKeccakP1600timesN_12rounds_FastLoop_Absorb,
    int useKeccakF1600timesN_FastKravatte
)
{
    #ifdef XKCP_has_KeccakP1600
        printf("- \303\2271: " KeccakP1600_implementation "\n");
    #if defined(KeccakF1600_FastLoop_supported)
    if (useKeccakF1600timesN_FastLoop_Absorb)
        printf("      + KeccakF1600_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakP1600_12rounds_FastLoop_supported)
    if (useKeccakP1600timesN_12rounds_FastLoop_Absorb)
        printf("      + KeccakP1600_12rounds_FastLoop_Absorb()\n");
    #endif
    #else
    printf("- \303\2271: not used\n");
    #endif

    #if defined(XKCP_has_KeccakP1600times2)
    printf("- \303\2272: " KeccakP1600times2_implementation "\n");
    #if defined(KeccakP1600times2_12rounds_FastLoop_supported)
    if (useKeccakP1600timesN_12rounds_FastLoop_Absorb)
        printf("      + KeccakP1600times2_12rounds_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakF1600times2_FastLoop_supported)
    if (useKeccakF1600timesN_FastLoop_Absorb)
        printf("      + KeccakF1600times2_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakF1600times2_FastKravatte_supported)
    if (useKeccakF1600timesN_FastKravatte) {
        printf("      + KeccakP1600times2_KravatteCompress()\n");
        printf("      + KeccakP1600times2_KravatteExpand()\n");
    }
    #endif
    #else
    printf("- \303\2272: not used\n");
    #endif

    #if defined(XKCP_has_KeccakP1600times4)
    printf("- \303\2274: " KeccakP1600times4_implementation "\n");
    #if defined(KeccakP1600times4_12rounds_FastLoop_supported)
    if (useKeccakP1600timesN_12rounds_FastLoop_Absorb)
        printf("      + KeccakP1600times4_12rounds_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakF1600times4_FastLoop_supported)
    if (useKeccakF1600timesN_FastLoop_Absorb)
        printf("      + KeccakF1600times4_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakF1600times4_FastKravatte_supported)
    if (useKeccakF1600timesN_FastKravatte) {
        printf("      + KeccakP1600times4_KravatteCompress()\n");
        printf("      + KeccakP1600times4_KravatteExpand()\n");
    }
    #endif
    #else
    printf("- \303\2274: not used\n");
    #endif

    #if defined(XKCP_has_KeccakP1600times8)
    printf("- \303\2278: " KeccakP1600times8_implementation "\n");
    #if defined(KeccakP1600times8_12rounds_FastLoop_supported)
    if (useKeccakP1600timesN_12rounds_FastLoop_Absorb)
        printf("      + KeccakP1600times8_12rounds_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakF1600times8_FastLoop_supported)
    if (useKeccakF1600timesN_FastLoop_Absorb)
        printf("      + KeccakF1600times8_FastLoop_Absorb()\n");
    #endif
    #if defined(KeccakF1600times8_FastKravatte_supported)
    if (useKeccakF1600timesN_FastKravatte) {
        printf("      + KeccakP1600times8_KravatteCompress()\n");
        printf("      + KeccakP1600times8_KravatteExpand()\n");
    }
    #endif
    #else
    printf("- \303\2278: not used\n");
    #endif
}

#ifdef XKCP_has_SP800_185
cycles_t measureParallelHash(cycles_t dtMin, unsigned int securityStrength, unsigned int blockByteLen, unsigned int inputLen)
{
    unsigned char *input = bigBuffer1;
    ALIGN_DEFAULT unsigned char output[32];
    measureTimingDeclare

    assert(inputLen <= sizeof(bigBuffer1));

    memset(input, 0xA5, 16);

    if(securityStrength == 128) {
        measureTimingBeginDeclared
        ParallelHash128(input, inputLen*8, blockByteLen, output, 256, (const BitSequence *)"", 0);
        measureTimingEnd
    }
    else if(securityStrength == 256) {
        measureTimingBeginDeclared
        ParallelHash256(input, inputLen*8, blockByteLen, output, 256, (const BitSequence *)"", 0);
        measureTimingEnd
    }
    else
        return 0;
}

void printParallelHashPerformanceHeader(unsigned int securityStrength)
{
    printf("*** ParallelHash%d ***\n", securityStrength);
    printf("Using Keccak-f[1600] implementations:\n");
    printParallelImplementations(1, 0, 0);
    printf("\n");
}

void testParallelHashPerformanceOne(unsigned int securityStrength, unsigned int blockByteLen)
{
    unsigned halfTones;
    cycles_t calibration = CalibrateTimer();
    unsigned int blockByteLenLog = (unsigned int)floor(log(blockByteLen)/log(2.0)+0.5);
    int displaySlope = 1;

    printf("Block size: %d bytes\n", blockByteLen);
    for(halfTones=blockByteLenLog*12-28; halfTones<=19*12; halfTones+=4) {
        double I = pow(2.0, halfTones/12.0);
        unsigned int i  = (unsigned int)floor(I+0.5);
        cycles_t time, timePlus1Block, timePlus2Blocks, timePlus4Blocks, timePlus8Blocks;
        time = measureParallelHash(calibration, securityStrength, blockByteLen, i);
        if (displaySlope) {
            timePlus1Block = measureParallelHash(calibration, securityStrength, blockByteLen, i+1*blockByteLen);
            timePlus2Blocks = measureParallelHash(calibration, securityStrength, blockByteLen, i+2*blockByteLen);
            timePlus4Blocks = measureParallelHash(calibration, securityStrength, blockByteLen, i+4*blockByteLen);
            timePlus8Blocks = measureParallelHash(calibration, securityStrength, blockByteLen, i+8*blockByteLen);
        }
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", i, time, getTimerUnit(), time*1.0/i, getTimerUnit());
        if (displaySlope) {
            printf("     +1 block:  %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus1Block, getTimerUnit(), (timePlus1Block-(double)(time))*1.0/blockByteLen/1.0, getTimerUnit());
            printf("     +2 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus2Blocks, getTimerUnit(), (timePlus2Blocks-(double)(time))*1.0/blockByteLen/2.0, getTimerUnit());
            printf("     +4 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus4Blocks, getTimerUnit(), (timePlus4Blocks-(double)(time))*1.0/blockByteLen/4.0, getTimerUnit());
            printf("     +8 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus8Blocks, getTimerUnit(), (timePlus8Blocks-(double)(time))*1.0/blockByteLen/8.0, getTimerUnit());
        }
        displaySlope = 0;
    }
    printf("\n\n");
}

void testParallelHashPerformance()
{
    printParallelHashPerformanceHeader(128);
    testParallelHashPerformanceOne(128, 8192);
    printParallelHashPerformanceHeader(256);
    testParallelHashPerformanceOne(256, 8192);
}
#endif

#ifdef XKCP_has_KangarooTwelve
cycles_t measureKangarooTwelve(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char *input = bigBuffer1;
    ALIGN_DEFAULT unsigned char output[32];
    measureTimingDeclare

    assert(inputLen <= sizeof(bigBuffer1));

    memset(input, 0xA5, 16);

    measureTimingBeginDeclared
    KangarooTwelve(input, inputLen, output, 32, (const BitSequence *)"", 0);
    measureTimingEnd
}

void printKangarooTwelvePerformanceHeader( void )
{
    printf("*** KangarooTwelve ***\n");
    printf("Using Keccak-p[1600,12] implementations:\n");
    printParallelImplementations(0, 1, 0);
    printf("\n");
}

void testKangarooTwelvePerformanceOne( void )
{
    const unsigned int chunkSize = 8192;
    unsigned halfTones;
    cycles_t calibration = CalibrateTimer();
    unsigned int chunkSizeLog = (unsigned int)floor(log(chunkSize)/log(2.0)+0.5);
    int displaySlope = 0;

    measureKangarooTwelve(calibration, 500000);
    for(halfTones=chunkSizeLog*12-28; halfTones<=13*12; halfTones+=4) {
        double I = pow(2.0, halfTones/12.0);
        unsigned int i  = (unsigned int)floor(I+0.5);
        cycles_t time, timePlus1Block, timePlus2Blocks, timePlus4Blocks, timePlus8Blocks;
        cycles_t timePlus168Blocks;
        time = measureKangarooTwelve(calibration, i);
        if (i == chunkSize) {
            displaySlope = 1;
            timePlus1Block  = measureKangarooTwelve(calibration, i+1*chunkSize);
            timePlus2Blocks = measureKangarooTwelve(calibration, i+2*chunkSize);
            timePlus4Blocks = measureKangarooTwelve(calibration, i+4*chunkSize);
            timePlus8Blocks = measureKangarooTwelve(calibration, i+8*chunkSize);
            timePlus168Blocks = measureKangarooTwelve(calibration, i+168*chunkSize);
        }
        printf("%8u bytes: %9" PRId64 " %s, %6.3f %s/byte\n", i, time, getTimerUnit(), time*1.0/i, getTimerUnit());
        if (displaySlope) {
            printf("     +1 block:  %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus1Block, getTimerUnit(), (timePlus1Block-(double)(time))*1.0/chunkSize/1.0, getTimerUnit());
            printf("     +2 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus2Blocks, getTimerUnit(), (timePlus2Blocks-(double)(time))*1.0/chunkSize/2.0, getTimerUnit());
            printf("     +4 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus4Blocks, getTimerUnit(), (timePlus4Blocks-(double)(time))*1.0/chunkSize/4.0, getTimerUnit());
            printf("     +8 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus8Blocks, getTimerUnit(), (timePlus8Blocks-(double)(time))*1.0/chunkSize/8.0, getTimerUnit());
            printf("   +168 blocks: %9" PRId64 " %s, %6.3f %s/byte (slope)\n", timePlus168Blocks, getTimerUnit(), (timePlus168Blocks-(double)(time))*1.0/chunkSize/168.0, getTimerUnit());
            displaySlope = 0;
        }
    }
    for(halfTones=12*12; halfTones<=20*12; halfTones+=4) {
        double I = chunkSize + pow(2.0, halfTones/12.0);
        unsigned int i  = (unsigned int)floor(I+0.5);
        cycles_t time;
        time = measureKangarooTwelve(calibration, i);
        printf("%8u bytes: %9" PRId64 " %s, %6.3f %s/byte\n", i, time, getTimerUnit(), time*1.0/i, getTimerUnit());
    }
    printf("\n\n");
}

void testKangarooTwelvePerformance()
{
    printKangarooTwelvePerformanceHeader();
    testKangarooTwelvePerformanceOne();
}
#endif

#ifdef XKCP_has_Kravatte

#define Kravatte_KeyLen 16
#define Kravatte_rate   1600

cycles_t measureKravatte_MaskDerivation(cycles_t dtMin)
{
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    Kravatte_Instance kv;
    measureTimingDeclare
 
    memset(key, 0xA5, Kravatte_KeyLen);

    measureTimingBeginDeclared
    Kravatte_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    measureTimingEnd
}

cycles_t measureKra(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    Kravatte_Instance kv;
    measureTimingDeclare
 
    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    memset(input, 0xA5, inputLen/8);

    measureTimingBeginDeclared
    Kra(&kv, input, inputLen, KRAVATTE_FLAG_LAST_PART);
    measureTimingEnd
}

cycles_t measureVatte(cycles_t dtMin, unsigned int outputLen)
{
    unsigned char* output = bigBuffer1;
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    Kravatte_Instance kv;
    measureTimingDeclare

    assert(outputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    output[0] = 0;
    Kra(&kv, output, 0, KRAVATTE_FLAG_LAST_PART);

    measureTimingBeginDeclared
    kv.phase = COMPRESSING; /* avoid error when calling multiple times with last flag set */
    Vatte(&kv, output, outputLen, KRAVATTE_FLAG_LAST_PART);
    measureTimingEnd
}

cycles_t measureKravatte_SANSE(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    ALIGN_DEFAULT unsigned char AD[16];
    ALIGN_DEFAULT unsigned char tag[Kravatte_SANSE_TagLength];
    Kravatte_SANSE_Instance kv;
    measureTimingDeclare

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_SANSE_Initialize(&kv, key, Kravatte_KeyLen*8);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    measureTimingBeginDeclared
    Kravatte_SANSE_Wrap(&kv, input, output, inputLen, AD, sizeof(AD)*8, tag);
    measureTimingEnd
}

cycles_t measureKravatte_SANE_Wrap(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    ALIGN_DEFAULT unsigned char nonce[16];
    ALIGN_DEFAULT unsigned char AD[16];
    ALIGN_DEFAULT unsigned char tag[Kravatte_SANE_TagLength];
    Kravatte_SANE_Instance kv;
    measureTimingDeclare

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Kravatte_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Kravatte_SANE_Initialize(&kv, key, Kravatte_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    measureTimingBeginDeclared
    Kravatte_SANE_Wrap(&kv, input, output, inputLen, AD, 0, tag);
    measureTimingEnd
}

cycles_t measureKravatte_SANE_MAC(cycles_t dtMin, unsigned int ADLen)
{
    ALIGN_DEFAULT unsigned char input[1];
    ALIGN_DEFAULT unsigned char output[1];
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    ALIGN_DEFAULT unsigned char nonce[16];
    unsigned char* AD = bigBuffer1;
    ALIGN_DEFAULT unsigned char tag[Kravatte_SANE_TagLength];
    Kravatte_SANE_Instance kv;
    measureTimingDeclare

    assert(ADLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Kravatte_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Kravatte_SANE_Initialize(&kv, key, Kravatte_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, sizeof(input));
    memset(AD, 0x5A, ADLen/8);

    measureTimingBeginDeclared
    Kravatte_SANE_Wrap(&kv, input, output, 0, AD, ADLen, tag);
    measureTimingEnd
}

cycles_t measureKravatte_WBC(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN_DEFAULT unsigned char key[Kravatte_KeyLen];
    ALIGN_DEFAULT unsigned char W[16];
    Kravatte_Instance kvw;
    measureTimingDeclare

    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_WBC_Initialize(&kvw, key, Kravatte_KeyLen*8);
    memset(input, 0xA5, inputLen/8);
    memset(W, 0x55, sizeof(W));

    measureTimingBeginDeclared
    Kravatte_WBC_Encipher(&kvw, input, output, inputLen, W, sizeof(W)*8);
    measureTimingEnd
}

void printKravattePerformanceHeader( void )
{
    printf("*** Kravatte ***\n");
    printf("Using Keccak-p[1600,6] implementations:\n");
    printParallelImplementations(0, 0, 1);
    printf("\n");
}

uint32_t testKravatteNextLen( uint32_t len )
{
    if (len < Kravatte_rate) {
        len <<= 1;
        if (len > Kravatte_rate)
            len = Kravatte_rate;
    }
    else if (len < 16*Kravatte_rate) {
        len += Kravatte_rate;
    }
    else
        len <<= 1;
	return len;
}

uint32_t testKravatteAdaptLen( uint32_t len )
{
    return (len < Kravatte_rate) ? len : (len-8);
}

uint32_t testKravatteWBCAdaptLen( uint32_t len )
{
    return (len < Kravatte_rate) ? len : (len-16);
}

typedef cycles_t (* measurePerf)(cycles_t, unsigned int);

static void testKravattePerfSlope(measurePerf pFunc, cycles_t calibration)
{
	uint32_t len;
    uint32_t count;
    cycles_t time;
    cycles_t time128;
    cycles_t time256;
    const uint32_t stabilityCount = 10;

    time128 = CYCLES_MAX;
    len = 128*Kravatte_rate;
	count = stabilityCount;
    do {
        time = pFunc(calibration, len);
		if (time < time128) {
			time128 = time;
			count = stabilityCount;
		}
    } while( --count != 0);
    time256 = CYCLES_MAX;
    len = 256*Kravatte_rate;
	count = stabilityCount;
    do {
        time = pFunc(calibration, len);
		if (time < time256) {
			time256 = time;
			count = stabilityCount;
		}
    } while( --count != 0);

    time = time256-time128;
	len = 128*Kravatte_rate;
    printf("Slope %8d bytes (%u blocks): %9" PRId64 " %s, %6.3f %s/byte\n", len/8, len/Kravatte_rate, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
}

void testKravattePerformanceOne( void )
{
    cycles_t calibration = CalibrateTimer();
    uint32_t len;
    cycles_t time;

    time = measureKravatte_MaskDerivation(calibration);
    printf("Kravatte Mask Derivation %9" PRId64 " %s\n\n", time, getTimerUnit());

    printf("Kra\n");
    {
        len = 4096*8;
        time = measureKra(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=8; len <= 256*Kravatte_rate; len = testKravatteNextLen(len)) {
        time = measureKra(calibration, testKravatteAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testKravatteAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
	testKravattePerfSlope(measureKra, calibration);

    printf("\nVatte\n");
    {
        len = 4096*8;
        time = measureVatte(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=8; len <= 256*Kravatte_rate; len = testKravatteNextLen(len)) {
        time = measureVatte(calibration, testKravatteAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testKravatteAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
	testKravattePerfSlope(measureVatte, calibration);

    printf("\nKravatte_SANE Wrap (only plaintext input, no AD)\n");
    {
        len = 4096*8;
        time = measureKravatte_SANE_Wrap(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=8; len <= 256*Kravatte_rate; len = testKravatteNextLen(len)) {
        time = measureKravatte_SANE_Wrap(calibration, testKravatteAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testKravatteAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
	testKravattePerfSlope(measureKravatte_SANE_Wrap, calibration);

    printf("\nKravatte_SANE MAC (only AD input, no plaintext)\n");
    {
        len = 4096*8;
        time = measureKravatte_SANE_MAC(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=8; len <= 256*Kravatte_rate; len = testKravatteNextLen(len)) {
        time = measureKravatte_SANE_MAC(calibration, testKravatteAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testKravatteAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
	testKravattePerfSlope(measureKravatte_SANE_MAC, calibration);

    printf("\nKravatte_SANSE\n");
    {
        len = 4096*8;
        time = measureKravatte_SANSE(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=8; len <= 256*Kravatte_rate; len = testKravatteNextLen(len)) {
        time = measureKravatte_SANSE(calibration, testKravatteAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testKravatteAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
	testKravattePerfSlope(measureKravatte_SANSE, calibration);

    printf("\nKravatte_WBC (Tweak 128 bits)\n");
    for(len=2048*8; len<=16384*8; len*=2) {
        time = measureKravatte_WBC(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=8; len <= 256*Kravatte_rate; len = testKravatteNextLen(len)) {
        time = measureKravatte_WBC(calibration, testKravatteWBCAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testKravatteWBCAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
	testKravattePerfSlope(measureKravatte_WBC, calibration);

    printf("\n\n");
}

void testKravattePerformance(void)
{
    printKravattePerformanceHeader();
    testKravattePerformanceOne();
}
#endif

void testPerformance()
{
#ifdef XKCP_has_KeccakP200
    KeccakP200_timingSnP("Keccak-p[200]", KeccakP200_implementation);
#endif

#ifdef XKCP_has_KeccakP400
    KeccakP400_timingSnP("Keccak-p[400]", KeccakP400_implementation);
#endif

#ifdef XKCP_has_KeccakP800
    KeccakP800_timingSnP("Keccak-p[800]", KeccakP800_implementation);
#endif

#ifdef XKCP_has_KeccakP1600
    KeccakP1600_timingSnP("Keccak-p[1600]", KeccakP1600_implementation);
#endif
#ifdef XKCP_has_KeccakP1600times2
    KeccakP1600times2_timingPlSnP("Keccak-p[1600]\303\2272", KeccakP1600times2_implementation);
#endif
#ifdef XKCP_has_KeccakP1600times4
    KeccakP1600times4_timingPlSnP("Keccak-p[1600]\303\2274", KeccakP1600times4_implementation);
#endif
#ifdef XKCP_has_KeccakP1600times8
    KeccakP1600times8_timingPlSnP("Keccak-p[1600]\303\2278", KeccakP1600times8_implementation);
#endif

#ifdef XKCP_has_Sponge_Keccak_width800
    KeccakWidth800_timing();
#endif
#ifdef XKCP_has_Sponge_Keccak_width1600
    KeccakWidth1600_timing();
#endif
#ifdef XKCP_has_TurboSHAKE
    TurboSHAKE_timing();
#endif

#ifdef XKCP_has_SP800_185
    testParallelHashPerformance();
#endif

#ifdef XKCP_has_KangarooTwelve
    testKangarooTwelvePerformance();
#endif

#ifdef XKCP_has_Kravatte
    testKravattePerformance();
#endif
}

static void bubbleSort(double *list, unsigned int size)
{
    unsigned int n = size;

    do {
       unsigned int newn = 0;
       unsigned int i;

       for(i=1; i<n; i++) {
          if (list[i-1] > list[i]) {
              double temp = list[i-1];
              list[i-1] = list[i];
              list[i] = temp;
              newn = i;
          }
       }
       n = newn;
    }
    while(n > 0);
}

static double med4(double x0, double x1, double x2, double x3)
{
    double list[4];
    list[0] = x0;
    list[1] = x1;
    list[2] = x2;
    list[3] = x3;
    bubbleSort(list, 4);
    if (fabs(list[2]-list[0]) < fabs(list[3]-list[1]))
        return 0.25*list[0]+0.375*list[1]+0.25*list[2]+0.125*list[3];
    else
        return 0.125*list[0]+0.25*list[1]+0.375*list[2]+0.25*list[3];
}

void displayMeasurements1101001000(cycles_t *measurements, uint32_t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes)
{
    double cpb[4];
    unsigned int i;

    for(i=0; i<numberOfColumns; i++) {
        uint32_t bytes = laneCounts[i]*laneLengthInBytes;
        double x = med4(measurements[i*4+0]*1.0, measurements[i*4+1]/10.0, measurements[i*4+2]/100.0, measurements[i*4+3]/1000.0);
        cpb[i] = x/bytes;
    }
    if (numberOfColumns == 1) {
        printf("     laneCount:  %5u\n", laneCounts[0]);
        printf("       1 block:  %5" PRId64 "\n", measurements[0]);
        printf("      10 blocks: %6" PRId64 "\n", measurements[1]);
        printf("     100 blocks: %7" PRId64 "\n", measurements[2]);
        printf("    1000 blocks: %8" PRId64 "\n", measurements[3]);
        printf("    %s/byte: %7.2f\n", getTimerUnit(), cpb[0]);
    }
    else if (numberOfColumns == 2) {
        printf("     laneCount:  %5u       %5u\n", laneCounts[0], laneCounts[1]);
        printf("       1 block:  %5" PRId64 "       %5" PRId64 "\n", measurements[0], measurements[4]);
        printf("      10 blocks: %6" PRId64 "      %6" PRId64 "\n", measurements[1], measurements[5]);
        printf("     100 blocks: %7" PRId64 "     %7" PRId64 "\n", measurements[2], measurements[6]);
        printf("    1000 blocks: %8" PRId64 "    %8" PRId64 "\n", measurements[3], measurements[7]);
        printf("    %s/byte: %7.2f     %7.2f\n", getTimerUnit(), cpb[0], cpb[1]);
    }
    else if (numberOfColumns == 3) {
        printf("     laneCount:  %5u       %5u       %5u\n", laneCounts[0], laneCounts[1], laneCounts[2]);
        printf("       1 block:  %5" PRId64 "       %5" PRId64 "       %5" PRId64 "\n", measurements[0], measurements[4], measurements[8]);
        printf("      10 blocks: %6" PRId64 "      %6" PRId64 "      %6" PRId64 "\n", measurements[1], measurements[5], measurements[9]);
        printf("     100 blocks: %7" PRId64 "     %7" PRId64 "     %7" PRId64 "\n", measurements[2], measurements[6], measurements[10]);
        printf("    1000 blocks: %8" PRId64 "    %8" PRId64 "    %8" PRId64 "\n", measurements[3], measurements[7], measurements[11]);
        printf("    %s/byte: %7.2f     %7.2f     %7.2f\n", getTimerUnit(), cpb[0], cpb[1], cpb[2]);
    }
    else if (numberOfColumns == 4) {
        printf("     laneCount:  %5u       %5u       %5u       %5u\n", laneCounts[0], laneCounts[1], laneCounts[2], laneCounts[3]);
        printf("       1 block:  %5" PRId64 "       %5" PRId64 "       %5" PRId64 "       %5" PRId64 "\n", measurements[0], measurements[4], measurements[8], measurements[12]);
        printf("      10 blocks: %6" PRId64 "      %6" PRId64 "      %6" PRId64 "      %6" PRId64 "\n", measurements[1], measurements[5], measurements[9], measurements[13]);
        printf("     100 blocks: %7" PRId64 "     %7" PRId64 "     %7" PRId64 "     %7" PRId64 "\n", measurements[2], measurements[6], measurements[10], measurements[14]);
        printf("    1000 blocks: %8" PRId64 "    %8" PRId64 "    %8" PRId64 "    %8" PRId64 "\n", measurements[3], measurements[7], measurements[11], measurements[15]);
        printf("    %s/byte: %7.2f     %7.2f     %7.2f     %7.2f\n", getTimerUnit(), cpb[0], cpb[1], cpb[2], cpb[3]);
    }
    printf("\n");
}
