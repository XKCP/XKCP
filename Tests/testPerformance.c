/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "KangarooTwelve.h"
#include "Keyakv2.h"
#include "Kravatte.h"
#include "KravatteModes.h"
#include "SP800-185.h"
#include "timing.h"
#include "testPerformance.h"

void displayMeasurements1101001000(uint_32t *measurements, uint_32t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes);

#define xstr(s) str(s)
#define str(s) #s

#ifndef KeccakP200_excluded
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

#ifndef KeccakP400_excluded
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

#ifndef KeccakP800_excluded
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

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakP1600
    #define SnP KeccakP1600
    #define SnP_width 1600
    #define SnP_Permute KeccakP1600_Permute_24rounds
    #define SnP_Permute_12rounds KeccakP1600_Permute_12rounds
    #if defined(KeccakF1600_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF1600_FastLoop_Absorb
    #endif
        #include "timingSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef KeccakF800_FastLoop_supported
uint_32t KeccakP800_measureSnP_FastLoop_Absorb(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount);

void KeccakP800_gatherSnP_FastLoop_Absorb(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
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
uint_32t KeccakP1600_measureSnP_FastLoop_Absorb(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount);

void KeccakP1600_gatherSnP_FastLoop_Absorb(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
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

#ifndef KeccakP200_excluded
uint_32t KeccakP200_measureSnP_GenericLoop_Absorb(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP200_gatherSnP_GenericLoop_Absorb(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 1);
    measurements[ 1] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 10);
    measurements[ 2] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 100);
    measurements[ 3] = KeccakP200_measureSnP_GenericLoop_Absorb(dtMin,  5, 1000);
    laneCounts[0] =  5;
    return 1;
}
#endif

#ifndef KeccakP400_excluded
uint_32t KeccakP400_measureSnP_GenericLoop_Absorb(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP400_gatherSnP_GenericLoop_Absorb(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
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

#ifndef KeccakP800_excluded
uint_32t KeccakP800_measureSnP_GenericLoop_Absorb(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP800_gatherSnP_GenericLoop_Absorb(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
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

#ifndef KeccakP1600_excluded
uint_32t KeccakP1600_measureSnP_GenericLoop_Absorb(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount);

unsigned int KeccakP1600_gatherSnP_GenericLoop_Absorb(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
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

#ifndef KeccakP800timesN_excluded
    #include "KeccakP-800-times2-SnP.h"

    #define prefix                      KeccakP800times2
    #define PlSnP                       KeccakP800times2
    #define PlSnP_parallelism           2
    #define SnP_width                   800
    #define PlSnP_PermuteAll            KeccakP800times2_PermuteAll_22rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP800times2_PermuteAll_12rounds
    #if defined(KeccakF800times2_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF800times2_FastLoop_Absorb
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

#ifndef KeccakP800timesN_excluded
    #include "KeccakP-800-times4-SnP.h"

    #define prefix                      KeccakP800times4
    #define PlSnP                       KeccakP800times4
    #define PlSnP_parallelism           4
    #define SnP_width                   800
    #define PlSnP_PermuteAll            KeccakP800times4_PermuteAll_22rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP800times4_PermuteAll_12rounds
    #if defined(KeccakF800times4_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF800times4_FastLoop_Absorb
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

#ifndef KeccakP800timesN_excluded
    #include "KeccakP-800-times8-SnP.h"

    #define prefix                      KeccakP800times8
    #define PlSnP                       KeccakP800times8
    #define PlSnP_parallelism           8
    #define SnP_width                   800
    #define PlSnP_PermuteAll            KeccakP800times8_PermuteAll_22rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP800times8_PermuteAll_12rounds
    #if defined(KeccakF800times8_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF800times8_FastLoop_Absorb
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

#ifndef KeccakP1600timesN_excluded
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

#ifndef KeccakP1600timesN_excluded
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

#ifndef KeccakP1600timesN_excluded
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

#ifndef KeccakP800_excluded
    #define prefix      KeccakWidth800
        #include "timingSponge.inc"
    #undef prefix

void KeccakWidth800_timing()
{
    uint_32t calibartion = KeccakWidth800_start("Keccak sponge functions using Keccak-f[800]", KeccakP800_implementation);
    KeccakWidth800_timingRC(calibartion, 544, 256);
    KeccakWidth800_timingRC(calibartion, 608, 192);
    printf("\n\n");
}
#endif

#ifndef KeccakP1600_excluded
    #define prefix      KeccakWidth1600
        #include "timingSponge.inc"
    #undef prefix

void KeccakWidth1600_timing()
{
    uint_32t calibartion = KeccakWidth1600_start("Keccak sponge functions using Keccak-f[1600]", KeccakP1600_implementation);
    KeccakWidth1600_timingRC(calibartion, 1088, 512);
    KeccakWidth1600_timingRC(calibartion, 1344, 256);
    printf("\n\n");
}
#endif

#ifndef KeccakP800_excluded
    #define prefix      RiverKeyak
    #define Rs          68
    #define Ra          96
    #define P           1
        #include "timingKeyak.inc"
    #undef prefix
    #undef Rs
    #undef Ra
    #undef P
#endif

#ifndef KeccakP1600_excluded
    #define prefix      LakeKeyak
    #define Rs          168
    #define Ra          192
    #define P           1
        #include "timingKeyak.inc"
    #undef prefix
    #undef Rs
    #undef Ra
    #undef P
#endif

#ifndef KeccakP1600timesN_excluded
    #define prefix      SeaKeyak
    #define Rs          168
    #define Ra          192
    #define P           2
        #include "timingKeyak.inc"
    #undef prefix
    #undef Rs
    #undef Ra
    #undef P
#endif

#ifndef KeccakP1600timesN_excluded
    #define prefix      OceanKeyak
    #define Rs          168
    #define Ra          192
    #define P           4
        #include "timingKeyak.inc"
    #undef prefix
    #undef Rs
    #undef Ra
    #undef P
#endif

#ifndef KeccakP1600timesN_excluded
    #define prefix      LunarKeyak
    #define Rs          168
    #define Ra          192
    #define P           8
        #include "timingKeyak.inc"
    #undef prefix
    #undef Rs
    #undef Ra
    #undef P
#endif

#ifndef KeccakP1600_excluded
uint_32t measureParallelHash(uint_32t dtMin, unsigned int securityStrength, unsigned int blockByteLen, unsigned int inputLen)
{
    ALIGN(32) unsigned char input[1024*1024];
    ALIGN(32) unsigned char output[32];

    assert(inputLen <= 1024*1024);

    memset(input, 0xA5, 16);

    if(securityStrength == 128) {
        measureTimingBegin
        ParallelHash128(input, inputLen*8, blockByteLen, output, 256, "", 0);
        measureTimingEnd
    }
    else if(securityStrength == 256) {
        measureTimingBegin
        ParallelHash256(input, inputLen*8, blockByteLen, output, 256, "", 0);
        measureTimingEnd
    }
    else
        return 0;
}

void printParallelHashPerformanceHeader(unsigned int securityStrength)
{
    printf("*** ParallelHash%d ***\n", securityStrength);
    printf("Using Keccak-f[1600] implementations:\n");
    printf("- \303\2271: " KeccakP1600_implementation "\n");
#ifndef KeccakP1600timesN_excluded
    printf("- \303\2272: " KeccakP1600times2_implementation "\n");
    printf("- \303\2274: " KeccakP1600times4_implementation "\n");
    printf("- \303\2278: " KeccakP1600times8_implementation "\n");
#endif
    printf("\n");
}

void testParallelHashPerformanceOne(unsigned int securityStrength, unsigned int blockByteLen)
{
    unsigned halfTones;
    uint_32t calibration = calibrate();
    unsigned int blockByteLenLog = (unsigned int)floor(log(blockByteLen)/log(2.0)+0.5);
    int displaySlope = 1;

    printf("Block size: %d bytes\n", blockByteLen);
    for(halfTones=blockByteLenLog*12-28; halfTones<=19*12; halfTones+=4) {
        double I = pow(2.0, halfTones/12.0);
        unsigned int i  = (unsigned int)floor(I+0.5);
        uint_32t time, timePlus1Block, timePlus2Blocks, timePlus4Blocks, timePlus8Blocks;
        time = measureParallelHash(calibration, securityStrength, blockByteLen, i);
        if (displaySlope) {
            timePlus1Block = measureParallelHash(calibration, securityStrength, blockByteLen, i+1*blockByteLen);
            timePlus2Blocks = measureParallelHash(calibration, securityStrength, blockByteLen, i+2*blockByteLen);
            timePlus4Blocks = measureParallelHash(calibration, securityStrength, blockByteLen, i+4*blockByteLen);
            timePlus8Blocks = measureParallelHash(calibration, securityStrength, blockByteLen, i+8*blockByteLen);
        }
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", i, time, time*1.0/i);
        if (displaySlope) {
            printf("     +1 block:  %9d cycles, %6.3f cycles/byte (slope)\n", timePlus1Block, (timePlus1Block-(double)(time))*1.0/blockByteLen/1.0);
            printf("     +2 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus2Blocks, (timePlus2Blocks-(double)(time))*1.0/blockByteLen/2.0);
            printf("     +4 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus4Blocks, (timePlus4Blocks-(double)(time))*1.0/blockByteLen/4.0);
            printf("     +8 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus8Blocks, (timePlus8Blocks-(double)(time))*1.0/blockByteLen/8.0);
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

#ifndef KeccakP1600_excluded
uint_32t measureKangarooTwelve(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(32) unsigned char input[1024*1024];
    ALIGN(32) unsigned char output[32];

    assert(inputLen <= 1024*1024);

    memset(input, 0xA5, 16);

    measureTimingBegin
    KangarooTwelve(input, inputLen, output, 32, "", 0);
    measureTimingEnd
}

void printKangarooTwelvePerformanceHeader( void )
{
    printf("*** KangarooTwelve ***\n");
    printf("Using Keccak-p[1600,12] implementations:\n");
    printf("- \303\2271: " KeccakP1600_implementation "\n");
#ifndef KeccakP1600timesN_excluded
    printf("- \303\2272: " KeccakP1600times2_implementation "\n");
    printf("- \303\2274: " KeccakP1600times4_implementation "\n");
    printf("- \303\2278: " KeccakP1600times8_implementation "\n");
#endif
    printf("\n");
}

void testKangarooTwelvePerformanceOne( void )
{
    const unsigned int chunkSize = 8192;
    unsigned halfTones;
    uint_32t calibration = calibrate();
    unsigned int chunkSizeLog = (unsigned int)floor(log(chunkSize)/log(2.0)+0.5);
    int displaySlope = 0;

    for(halfTones=chunkSizeLog*12-28; halfTones<=13*12; halfTones+=4) {
        double I = pow(2.0, halfTones/12.0);
        unsigned int i  = (unsigned int)floor(I+0.5);
        uint_32t time, timePlus1Block, timePlus2Blocks, timePlus4Blocks, timePlus8Blocks;
        uint_32t timePlus84Blocks;
        time = measureKangarooTwelve(calibration, i);
        if (i == chunkSize) {
            displaySlope = 1;
            timePlus1Block  = measureKangarooTwelve(calibration, i+1*chunkSize);
            timePlus2Blocks = measureKangarooTwelve(calibration, i+2*chunkSize);
            timePlus4Blocks = measureKangarooTwelve(calibration, i+4*chunkSize);
            timePlus8Blocks = measureKangarooTwelve(calibration, i+8*chunkSize);
            timePlus84Blocks = measureKangarooTwelve(calibration, i+84*chunkSize);
        }
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", i, time, time*1.0/i);
        if (displaySlope) {
            printf("     +1 block:  %9d cycles, %6.3f cycles/byte (slope)\n", timePlus1Block, (timePlus1Block-(double)(time))*1.0/chunkSize/1.0);
            printf("     +2 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus2Blocks, (timePlus2Blocks-(double)(time))*1.0/chunkSize/2.0);
            printf("     +4 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus4Blocks, (timePlus4Blocks-(double)(time))*1.0/chunkSize/4.0);
            printf("     +8 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus8Blocks, (timePlus8Blocks-(double)(time))*1.0/chunkSize/8.0);
            printf("    +84 blocks: %9d cycles, %6.3f cycles/byte (slope)\n", timePlus84Blocks, (timePlus84Blocks-(double)(time))*1.0/chunkSize/84.0);
            displaySlope = 0;
        }
    }
    for(halfTones=12*12; halfTones<=19*12; halfTones+=4) {
        double I = chunkSize + pow(2.0, halfTones/12.0);
        unsigned int i  = (unsigned int)floor(I+0.5);
        uint_32t time;
        time = measureKangarooTwelve(calibration, i);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", i, time, time*1.0/i);
    }
    printf("\n\n");
}

void testKangarooTwelvePerformance()
{
    printKangarooTwelvePerformanceHeader();
    testKangarooTwelvePerformanceOne();
}
#endif

#ifndef KeccakP1600_excluded

#define Kravatte_KeyLen 16
#define Kravatte_rate   1600

uint_32t measureKravatte_MaskDerivation(uint_32t dtMin)
{
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    Kravatte_Instance kv;
 
    memset(key, 0xA5, Kravatte_KeyLen);

    measureTimingBegin
    Kravatte_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    measureTimingEnd
}

uint_32t measureKra(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(32) unsigned char input[1024*1024];
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    Kravatte_Instance kv;
 
    assert(inputLen <= 1024*1024);

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    memset(input, 0xA5, inputLen/8);

    measureTimingBegin
    Kra(&kv, input, inputLen, KRAVATTE_FLAG_LAST_PART);
    measureTimingEnd
}

uint_32t measureVatte(uint_32t dtMin, unsigned int outputLen)
{
    ALIGN(32) unsigned char output[1024*1024];
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    Kravatte_Instance kv;

    assert(outputLen <= 1024*1024);

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    output[0] = 0;
    Kra(&kv, output, 0, KRAVATTE_FLAG_LAST_PART);

    measureTimingBegin
    kv.phase = COMPRESSING; /* avoid error when calling multiple times with last flag set */
    Vatte(&kv, output, outputLen, KRAVATTE_FLAG_LAST_PART);
    measureTimingEnd
}

uint_32t measureKravatte_SIV(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(32) unsigned char input[1024*1024];
    ALIGN(32) unsigned char output[1024*1024];
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    ALIGN(32) unsigned char AD[16];
    ALIGN(32) unsigned char tag[Kravatte_SIV_TagLength];
    Kravatte_Instance kv;

    assert(inputLen <= 1024*1024);

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_SIV_MaskDerivation(&kv, key, Kravatte_KeyLen*8);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    measureTimingBegin
    Kravatte_SIV_Wrap(&kv, input, output, inputLen, AD, sizeof(AD)*8, tag);
    measureTimingEnd
}

uint_32t measureKravatte_SAE_Wrap(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(32) unsigned char input[1024*1024];
    ALIGN(32) unsigned char output[1024*1024];
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    ALIGN(32) unsigned char nonce[16];
    ALIGN(32) unsigned char AD[16];
    ALIGN(32) unsigned char tag[Kravatte_SAE_TagLength];
    Kravatte_Instance kv;

    assert(inputLen <= 1024*1024);

    memset(key, 0xA5, Kravatte_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Kravatte_SAE_Initialize(&kv, key, Kravatte_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    measureTimingBegin
    Kravatte_SAE_Wrap(&kv, input, output, inputLen, AD, 0, tag);
    measureTimingEnd
}

uint_32t measureKravatte_SAE_MAC(uint_32t dtMin, unsigned int ADLen)
{
    ALIGN(32) unsigned char input[1];
    ALIGN(32) unsigned char output[1];
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    ALIGN(32) unsigned char nonce[16];
    ALIGN(32) unsigned char AD[1024*1024];
    ALIGN(32) unsigned char tag[Kravatte_SAE_TagLength];
    Kravatte_Instance kv;

    assert(ADLen <= 1024*1024);

    memset(key, 0xA5, Kravatte_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    Kravatte_SAE_Initialize(&kv, key, Kravatte_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, sizeof(input));
    memset(AD, 0x5A, ADLen/8);

    measureTimingBegin
    Kravatte_SAE_Wrap(&kv, input, output, 0, AD, ADLen, tag);
    measureTimingEnd
}

uint_32t measureKravatte_WBC(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(32) unsigned char input[1024*1024];
    ALIGN(32) unsigned char output[1024*1024];
    ALIGN(32) unsigned char key[Kravatte_KeyLen];
    ALIGN(32) unsigned char W[16];
    Kravatte_Instance kvw;

    assert(inputLen <= 1024*1024);

    memset(key, 0xA5, Kravatte_KeyLen);
    Kravatte_WBC_Initialize(&kvw, key, Kravatte_KeyLen*8);
    memset(input, 0xA5, inputLen/8);
    memset(W, 0x55, sizeof(W));

    measureTimingBegin
    Kravatte_WBC_Encipher(&kvw, input, output, inputLen, W, sizeof(W)*8);
    measureTimingEnd
}

void printKravattePerformanceHeader( void )
{
    printf("*** Kravatte ***\n");
    printf("Using Keccak-p[1600,6] implementations:\n");
    printf("- \303\2271: " KeccakP1600_implementation "\n");
#ifndef KeccakP1600timesN_excluded
    printf("- \303\2272: " KeccakP1600times2_implementation "\n");
    printf("- \303\2274: " KeccakP1600times4_implementation "\n");
    printf("- \303\2278: " KeccakP1600times8_implementation "\n");
#endif
    printf("\n");
}


void testKravattePerformanceOne( void )
{
    uint_32t calibration = calibrate();
    uint_32t len;
    uint_32t time;

    time = measureKravatte_MaskDerivation(calibration);
    printf("Kravatte Mask Derivation %9d cycles\n\n", time);

    printf("Kra\n");
    for(len=8; len <= 256*Kravatte_rate; /* empty */ ) {
        time = measureKra(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
        if (len < Kravatte_rate) {
            len <<= 1;
            if (len > Kravatte_rate)
                len = Kravatte_rate;
        }
        else
            len <<= 1;
    }

    printf("\nVatte\n");
    for(len=8; len <= 256*Kravatte_rate; /* empty */ ) {
        time = measureVatte(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
        if (len < Kravatte_rate) {
            len <<= 1;
            if (len > Kravatte_rate)
                len = Kravatte_rate;
        }
        else
            len <<= 1;
    }

    printf("\nKravatte_SIV\n");
    for(len=8; len <= 256*Kravatte_rate; /* empty */ ) {
        time = measureKravatte_SIV(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
        if (len < Kravatte_rate) {
            len <<= 1;
            if (len > Kravatte_rate)
                len = Kravatte_rate;
        }
        else
            len <<= 1;
    }

    printf("\nKravatte_SAE Wrap (only plaintext input, no AD)\n");
    for(len=8; len <= 256*Kravatte_rate; /* empty */ ) {
        time = measureKravatte_SAE_Wrap(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
        if (len < Kravatte_rate) {
            len <<= 1;
            if (len > Kravatte_rate)
                len = Kravatte_rate;
        }
        else
            len <<= 1;
    }

    printf("\nKravatte_SAE MAC (only AD input, no plaintext)\n");
    for(len=8; len <= 256*Kravatte_rate; /* empty */ ) {
        time = measureKravatte_SAE_MAC(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
        if (len < Kravatte_rate) {
            len <<= 1;
            if (len > Kravatte_rate)
                len = Kravatte_rate;
        }
        else
            len <<= 1;
    }

    printf("\nKravatte_WBC (Tweak 128 bits)\n");
    for(len=8; len <= 256*Kravatte_rate; /* empty */ ) {
        time = measureKravatte_WBC(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
        if (len < Kravatte_rate) {
            len <<= 1;
            if (len > Kravatte_rate)
                len = Kravatte_rate;
        }
        else
            len <<= 1;
    }
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
#ifndef KeccakP200_excluded
    KeccakP200_timingSnP("Keccak-p[200]", KeccakP200_implementation);
#endif

#ifndef KeccakP400_excluded
    KeccakP400_timingSnP("Keccak-p[400]", KeccakP400_implementation);
#endif

#ifndef KeccakP800_excluded
    KeccakP800_timingSnP("Keccak-p[800]", KeccakP800_implementation);
#endif
#ifndef KeccakP800timesN_excluded
    KeccakP800times2_timingPlSnP("Keccak-p[800]\303\2272", KeccakP800times2_implementation);
    KeccakP800times4_timingPlSnP("Keccak-p[800]\303\2274", KeccakP800times4_implementation);
    KeccakP800times8_timingPlSnP("Keccak-p[800]\303\2278", KeccakP800times8_implementation);
#endif

#ifndef KeccakP1600_excluded
    KeccakP1600_timingSnP("Keccak-p[1600]", KeccakP1600_implementation);
#endif
#ifndef KeccakP1600timesN_excluded
    KeccakP1600times2_timingPlSnP("Keccak-p[1600]\303\2272", KeccakP1600times2_implementation);
    KeccakP1600times4_timingPlSnP("Keccak-p[1600]\303\2274", KeccakP1600times4_implementation);
    KeccakP1600times8_timingPlSnP("Keccak-p[1600]\303\2278", KeccakP1600times8_implementation);
#endif

#ifndef KeccakP800_excluded
    KeccakWidth800_timing();
#endif
#ifndef KeccakP1600_excluded
    KeccakWidth1600_timing();
#endif

#ifndef KeccakP800_excluded
    RiverKeyak_timing("River Keyak", KeccakP800_implementation);
#endif
#ifndef KeccakP1600_excluded
    LakeKeyak_timing("Lake Keyak", KeccakP1600_implementation);
#endif
#ifndef KeccakP1600timesN_excluded
    SeaKeyak_timing("Sea Keyak", KeccakP1600times2_implementation);
    OceanKeyak_timing("Ocean Keyak", KeccakP1600times4_implementation);
    LunarKeyak_timing("Lunar Keyak", KeccakP1600times8_implementation);
#endif

#ifndef KeccakP1600_excluded
    testParallelHashPerformance();
#endif

#ifndef KeccakP1600_excluded
    testKangarooTwelvePerformance();
#endif

#ifndef KeccakP1600_excluded
    testKravattePerformance();
#endif
}

void bubbleSort(double *list, unsigned int size)
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

double med4(double x0, double x1, double x2, double x3)
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

void displayMeasurements1101001000(uint_32t *measurements, uint_32t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes)
{
    double cpb[4];
    unsigned int i;

    for(i=0; i<numberOfColumns; i++) {
        uint_32t bytes = laneCounts[i]*laneLengthInBytes;
        double x = med4(measurements[i*4+0]*1.0, measurements[i*4+1]/10.0, measurements[i*4+2]/100.0, measurements[i*4+3]/1000.0);
        cpb[i] = x/bytes;
    }
    if (numberOfColumns == 1) {
        printf("     laneCount:  %5d\n", laneCounts[0]);
        printf("       1 block:  %5d\n", measurements[0]);
        printf("      10 blocks: %6d\n", measurements[1]);
        printf("     100 blocks: %7d\n", measurements[2]);
        printf("    1000 blocks: %8d\n", measurements[3]);
        printf("    cycles/byte: %7.2f\n", cpb[0]);
    }
    else if (numberOfColumns == 2) {
        printf("     laneCount:  %5d       %5d\n", laneCounts[0], laneCounts[1]);
        printf("       1 block:  %5d       %5d\n", measurements[0], measurements[4]);
        printf("      10 blocks: %6d      %6d\n", measurements[1], measurements[5]);
        printf("     100 blocks: %7d     %7d\n", measurements[2], measurements[6]);
        printf("    1000 blocks: %8d    %8d\n", measurements[3], measurements[7]);
        printf("    cycles/byte: %7.2f     %7.2f\n", cpb[0], cpb[1]);
    }
    else if (numberOfColumns == 3) {
        printf("     laneCount:  %5d       %5d       %5d\n", laneCounts[0], laneCounts[1], laneCounts[2]);
        printf("       1 block:  %5d       %5d       %5d\n", measurements[0], measurements[4], measurements[8]);
        printf("      10 blocks: %6d      %6d      %6d\n", measurements[1], measurements[5], measurements[9]);
        printf("     100 blocks: %7d     %7d     %7d\n", measurements[2], measurements[6], measurements[10]);
        printf("    1000 blocks: %8d    %8d    %8d\n", measurements[3], measurements[7], measurements[11]);
        printf("    cycles/byte: %7.2f     %7.2f     %7.2f\n", cpb[0], cpb[1], cpb[2]);
    }
    else if (numberOfColumns == 4) {
        printf("     laneCount:  %5d       %5d       %5d       %5d\n", laneCounts[0], laneCounts[1], laneCounts[2], laneCounts[3]);
        printf("       1 block:  %5d       %5d       %5d       %5d\n", measurements[0], measurements[4], measurements[8], measurements[12]);
        printf("      10 blocks: %6d      %6d      %6d      %6d\n", measurements[1], measurements[5], measurements[9], measurements[13]);
        printf("     100 blocks: %7d     %7d     %7d     %7d\n", measurements[2], measurements[6], measurements[10], measurements[14]);
        printf("    1000 blocks: %8d    %8d    %8d    %8d\n", measurements[3], measurements[7], measurements[11], measurements[15]);
        printf("    cycles/byte: %7.2f     %7.2f     %7.2f     %7.2f\n", cpb[0], cpb[1], cpb[2], cpb[3]);
    }
    printf("\n");
}
