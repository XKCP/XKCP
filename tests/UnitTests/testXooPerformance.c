/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Xoofff.h"
#include "XoofffModes.h"
#include "timing.h"
#include "testXooPerformance.h"


void displayMeasurements1101001000(uint_32t *measurements, uint_32t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes);

#define xstr(s) str(s)
#define str(s) #s


#ifndef Xoodoo_excluded

#include "Xoodoo-SnP.h"

    #define prefix Xoodoo
    #define SnP Xoodoo
    #define SnP_width 384
    #define SnP_Permute_6rounds  Xoodoo_Permute_6rounds
    #define SnP_Permute_12rounds Xoodoo_Permute_12rounds
        #include "timingXooSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #endif

#include "Xoodoo-times4-SnP.h"
#if !defined(Xoodootimes4_isFallback)

    #define prefix                      Xoodootimes4
    #define PlSnP                       Xoodootimes4
    #define PlSnP_parallelism           4
    #define SnP_width                   384
    #define PlSnP_PermuteAll_6rounds    Xoodootimes4_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes4_PermuteAll_12rounds
        #include "timingXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
#endif

#include "Xoodoo-times8-SnP.h"
#if !defined(Xoodootimes8_isFallback)

    #define prefix                      Xoodootimes8
    #define PlSnP                       Xoodootimes8
    #define PlSnP_parallelism           8
    #define SnP_width                   384
    #define PlSnP_PermuteAll_6rounds    Xoodootimes8_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes8_PermuteAll_12rounds
        #include "timingXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
#endif

#include "Xoodoo-times16-SnP.h"
#if !defined(Xoodootimes16_isFallback)

    #define prefix                      Xoodootimes16
    #define PlSnP                       Xoodootimes16
    #define PlSnP_parallelism           16
    #define SnP_width                   384
    #define PlSnP_PermuteAll_6rounds    Xoodootimes16_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes16_PermuteAll_12rounds
        #include "timingXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
#endif

#ifndef Xoodoo_excluded

#define Xoofff_KeyLen 16
#define Xoofff_rate   384

uint_32t measureXoofff_MaskDerivation(uint_32t dtMin)
{
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;
 
    memset(key, 0xA5, Xoofff_KeyLen);

    {
        measureTimingBegin
        Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
        measureTimingEnd
    }
}

uint_32t measureXoofff_Compress(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(64) unsigned char input[256*Xoofff_rate];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;
 
    assert(inputLen <= 256*Xoofff_rate);

    memset(key, 0xA5, Xoofff_KeyLen);
    Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
    memset(input, 0xA5, inputLen/8);

    {
        measureTimingBegin
        Xoofff_Compress(&xp, input, inputLen, Xoofff_FlagLastPart);
        measureTimingEnd
    }
}

uint_32t measureXoofff_Expand(uint_32t dtMin, unsigned int outputLen)
{
    ALIGN(64) unsigned char output[256*Xoofff_rate];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;

    assert(outputLen <= 256*Xoofff_rate);

    memset(key, 0xA5, Xoofff_KeyLen);
    Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
    output[0] = 0;
    Xoofff_Compress(&xp, output, 0, Xoofff_FlagLastPart);

    {
        measureTimingBegin
        xp.phase = COMPRESSING; /* avoid error when calling multiple times with last flag set */
        Xoofff_Expand(&xp, output, outputLen, Xoofff_FlagLastPart);
        measureTimingEnd
    }
}

uint_32t measureXoofffSANE_Wrap(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(64) unsigned char input[256*Xoofff_rate];
    ALIGN(64) unsigned char output[256*Xoofff_rate];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char AD[16];
    ALIGN(64) unsigned char tag[XoofffSANE_TagLength];
    XoofffSANE_Instance xp;

    assert(inputLen <= 256*Xoofff_rate);

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    XoofffSANE_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    {
        measureTimingBegin
        XoofffSANE_Wrap(&xp, input, output, inputLen, AD, 0, tag);
        measureTimingEnd
    }
}

uint_32t measureXoofffSANE_MAC(uint_32t dtMin, unsigned int ADLen)
{
    ALIGN(64) unsigned char input[1];
    ALIGN(64) unsigned char output[1];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char AD[256*Xoofff_rate];
    ALIGN(64) unsigned char tag[XoofffSANE_TagLength];
    XoofffSANE_Instance xp;

    assert(ADLen <= 256*Xoofff_rate);

    memset(key, 0xA5, Xoofff_KeyLen);
    memset(nonce, 0x55, sizeof(nonce));
    XoofffSANE_Initialize(&xp, key, Xoofff_KeyLen*8, nonce, sizeof(nonce)*8, tag);
    memset(input, 0xA5, sizeof(input));
    memset(AD, 0x5A, ADLen/8);

    {
        measureTimingBegin
        XoofffSANE_Wrap(&xp, input, output, 0, AD, ADLen, tag);
        measureTimingEnd
    }
}

uint_32t measureXoofffSANSE(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(64) unsigned char input[256*Xoofff_rate];
    ALIGN(64) unsigned char output[256*Xoofff_rate];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char AD[16];
    ALIGN(64) unsigned char tag[XoofffSANSE_TagLength];
    XoofffSANSE_Instance xp;

    assert(inputLen <= 256*Xoofff_rate);

    memset(key, 0xA5, Xoofff_KeyLen);
    XoofffSANSE_Initialize(&xp, key, Xoofff_KeyLen*8);
    memset(input, 0xA5, inputLen/8);
    memset(AD, 0x5A, sizeof(AD));

    {
        measureTimingBegin
        XoofffSANSE_Wrap(&xp, input, output, inputLen, AD, sizeof(AD)*8, tag);
        measureTimingEnd
    }
}


uint_32t measureXoofffWBC(uint_32t dtMin, unsigned int inputLen)
{
    ALIGN(64) unsigned char input[256*Xoofff_rate];
    ALIGN(64) unsigned char output[256*Xoofff_rate];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char W[16];
    Xoofff_Instance xp;

    assert(inputLen <= 256*Xoofff_rate);

    memset(key, 0xA5, Xoofff_KeyLen);
    XoofffWBC_Initialize(&xp, key, Xoofff_KeyLen*8);
    memset(input, 0xA5, inputLen/8);
    memset(W, 0x55, sizeof(W));

    {
        measureTimingBegin
        XoofffWBC_Encipher(&xp, input, output, inputLen, W, sizeof(W)*8);
        measureTimingEnd
    }
}

void printXoofffPerformanceHeader( void )
{
    printf("*** Xoofff ***\n");
    printf("Using Xoodoo implementations:\n");
    printf("- \303\227\x31: " Xoodoo_implementation "\n");
    #if !defined(Xoodootimes4_isFallback)
    printf("- \303\2274: " Xoodootimes4_implementation "\n");
    #endif
    #if !defined(Xoodootimes8_isFallback)
    printf("- \303\2278: " Xoodootimes8_implementation "\n");
    #endif
    #if !defined(Xoodootimes16_isFallback)
    printf("- \303\22716: " Xoodootimes16_implementation "\n");
    #endif
    printf("\n");
}

uint_32t testXoofffNextLen( uint_32t len )
{
    if (len < Xoofff_rate) {
        len <<= 1;
        if (len > Xoofff_rate)
            len = Xoofff_rate;
    }
    else
        len <<= 1;
    return len;
}

uint_32t testXoofffAdaptLen( uint_32t len )
{
    return (len < Xoofff_rate) ? len : (len-8);
}

typedef uint_32t (* measurePerf)(uint_32t, unsigned int);

static void testXooPerfSlope( measurePerf pFunc, uint_32t calibration )
{
    uint_32t len;
    uint_32t count;
    uint_32t time;
    uint_32t time128;
    uint_32t time256;

    time128 = 0xFFFFFFFF;
    len = 128*Xoofff_rate;
    count = 100;
    do {
        time = pFunc(calibration, len);
        if (time < time128) {
            time128 = time;
            count = 100;
        }
    } while( --count != 0);
    time256 = 0xFFFFFFFF;
    len = 256*Xoofff_rate;
    count = 100;
    do {
        time = pFunc(calibration, len);
        if (time < time256) {
            time256 = time;
            count = 100;
        }
    } while( --count != 0);

    time = time256-time128;
    len = 128*Xoofff_rate;
    /*printf("%8u %8u\n", time128, time256);*/
    printf("Slope %8d bytes (%u blocks): %9d cycles, %6.3f cycles/byte\n", len/8, len/Xoofff_rate, time, time*1.0/(len/8));
}

void testXoofffPerformanceOne( void )
{
    uint_32t calibration = calibrate();
    uint_32t len;
    uint_32t time;

    time = measureXoofff_MaskDerivation(calibration);
    printf("Xoodoo Mask Derivation %9d cycles\n\n", time);

    printf("Xoofff_Compress\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofff_Compress(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", testXoofffAdaptLen(len)/8, time, time*1.0/(len/8));
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofff_Compress(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
    }
    testXooPerfSlope(measureXoofff_Compress, calibration);
    
    printf("\nXoofff_Expand\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofff_Expand(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", testXoofffAdaptLen(len)/8, time, time*1.0/(len/8));
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofff_Expand(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
    }
    testXooPerfSlope(measureXoofff_Expand, calibration);

    printf("\nXoofff-SANE Wrap (only plaintext input, no AD)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANE_Wrap(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", testXoofffAdaptLen(len)/8, time, time*1.0/(len/8));
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANE_Wrap(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
    }
    testXooPerfSlope(measureXoofffSANE_Wrap, calibration);

    printf("\nXoofff-SANE MAC (only AD input, no plaintext)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANE_MAC(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", testXoofffAdaptLen(len)/8, time, time*1.0/(len/8));
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANE_MAC(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
    }
    testXooPerfSlope(measureXoofffSANE_MAC, calibration);

    printf("\nXoofff-SANSE\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANSE(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", testXoofffAdaptLen(len)/8, time, time*1.0/(len/8));
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANSE(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
    }
    testXooPerfSlope(measureXoofffSANSE, calibration);

    printf("\nXoofff-WBC (Tweak 128 bits)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffWBC(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", testXoofffAdaptLen(len)/8, time, time*1.0/(len/8));
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffWBC(calibration, len);
        printf("%8d bytes: %9d cycles, %6.3f cycles/byte\n", len/8, time, time*1.0/(len/8));
    }
    testXooPerfSlope(measureXoofffWBC, calibration);

    printf("\n\n");
}

void testXoofffPerformance(void)
{
    printXoofffPerformanceHeader();
    testXoofffPerformanceOne();
}
#endif

void testXooPerformance(void)
{

#ifndef Xoodoo_excluded
    Xoodoo_timingSnP("Xoodoo", Xoodoo_implementation);
#endif
    #if !defined(Xoodootimes4_isFallback)
    Xoodootimes4_timingPlSnP("Xoodoo\303\2274", Xoodootimes4_implementation);
    #endif
    #if !defined(Xoodootimes8_isFallback)
    Xoodootimes8_timingPlSnP("Xoodoo\303\2278", Xoodootimes8_implementation);
    #endif
    #if !defined(Xoodootimes16_isFallback)
    Xoodootimes16_timingPlSnP("Xoodoo\303\22716", Xoodootimes16_implementation);
    #endif

#ifndef Xoodoo_excluded
    testXoofffPerformance();
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
