/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

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
#ifdef XKCP_has_Xoofff
#include "Xoofff.h"
#include "XoofffModes.h"
#endif
#ifdef XKCP_has_Xoodyak
#include "Xoodyak.h"
#endif
#include "timing.h"
#include "testPerformance.h"
#include "testXooPerformance.h"

typedef cycles_t (* measurePerf)(cycles_t, unsigned int);

void displayMeasurements1101001000(cycles_t *measurements, uint32_t *laneCounts, unsigned int numberOfColumns, unsigned int laneLengthInBytes);

#define xstr(s) str(s)
#define str(s) #s


#ifdef XKCP_has_Xoodoo
#include "Xoodoo-SnP.h"

    #define prefix Xoodoo
    #define SnP Xoodoo
    #define SnP_state Xoodoo_state
    #define SnP_width 384
    #define SnP_Permute_6rounds  Xoodoo_Permute_6rounds
    #define SnP_Permute_12rounds Xoodoo_Permute_12rounds
        #include "timingXooSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_state
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
#endif

#ifdef XKCP_has_Xoodootimes4
#include "Xoodoo-times4-SnP.h"

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

#ifdef XKCP_has_Xoodootimes8
#include "Xoodoo-times8-SnP.h"

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

#ifdef XKCP_has_Xoodootimes16
#include "Xoodoo-times16-SnP.h"

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

#ifdef XKCP_has_Xoofff

#define Xoofff_KeyLen 16
#define Xoofff_rate   384

cycles_t measureXoofff_MaskDerivation(cycles_t dtMin)
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

cycles_t measureXoofff_Compress(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;
 
    assert(inputLen <= sizeof(bigBuffer1));

    memset(key, 0xA5, Xoofff_KeyLen);
    Xoofff_MaskDerivation(&xp, key, Xoofff_KeyLen*8);
    memset(input, 0xA5, inputLen/8);

    {
        measureTimingBegin
        Xoofff_Compress(&xp, input, inputLen, Xoofff_FlagLastPart);
        measureTimingEnd
    }
}

cycles_t measureXoofff_Expand(cycles_t dtMin, unsigned int outputLen)
{
    unsigned char* output = bigBuffer1;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    Xoofff_Instance xp;

    assert(outputLen <= sizeof(bigBuffer1));

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

cycles_t measureXoofffSANE_Wrap(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    ALIGN(64) unsigned char AD[16];
    ALIGN(64) unsigned char tag[XoofffSANE_TagLength];
    XoofffSANE_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

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

cycles_t measureXoofffSANE_MAC(cycles_t dtMin, unsigned int ADLen)
{
    ALIGN(64) unsigned char input[1];
    ALIGN(64) unsigned char output[1];
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char nonce[16];
    unsigned char* AD = bigBuffer1;
    ALIGN(64) unsigned char tag[XoofffSANE_TagLength];
    XoofffSANE_Instance xp;

    assert(ADLen <= sizeof(bigBuffer1));

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

cycles_t measureXoofffSANSE(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char AD[16];
    ALIGN(64) unsigned char tag[XoofffSANSE_TagLength];
    XoofffSANSE_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

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


cycles_t measureXoofffWBC(cycles_t dtMin, unsigned int inputLen)
{
    unsigned char* input = bigBuffer1;
    unsigned char* output = bigBuffer2;
    ALIGN(64) unsigned char key[Xoofff_KeyLen];
    ALIGN(64) unsigned char W[16];
    Xoofff_Instance xp;

    assert(inputLen <= sizeof(bigBuffer1));

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
    #if defined(XKCP_has_Xoodootimes4)
    printf("- \303\2274: " Xoodootimes4_implementation "\n");
    #endif
    #if defined(XKCP_has_Xoodootimes8)
    printf("- \303\2278: " Xoodootimes8_implementation "\n");
    #endif
    #if defined(XKCP_has_Xoodootimes16)
    printf("- \303\22716: " Xoodootimes16_implementation "\n");
    #endif
    printf("\n");
}

uint32_t testXoofffNextLen( uint32_t len )
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

uint32_t testXoofffAdaptLen( uint32_t len )
{
    return (len < Xoofff_rate) ? len : (len-8);
}

static void testXooPerfSlope( measurePerf pFunc, cycles_t calibration )
{
    uint32_t len;
    uint32_t count;
    cycles_t time;
    cycles_t time128;
    cycles_t time256;
    const uint32_t stabilityCount = 10;

    time128 = CYCLES_MAX;
    len = 128*Xoofff_rate;
    count = stabilityCount;
    do {
        time = pFunc(calibration, len);
        if (time < time128) {
            time128 = time;
            count = stabilityCount;
        }
    } while( --count != 0);
    time256 = CYCLES_MAX;
    len = 256*Xoofff_rate;
    count = stabilityCount;
    do {
        time = pFunc(calibration, len);
        if (time < time256) {
            time256 = time;
            count = stabilityCount;
        }
    } while( --count != 0);

    time = time256-time128;
    len = 128*Xoofff_rate;
    printf("Slope %8d bytes (%u blocks): %9" PRId64 " %s, %6.3f %s/byte\n", len/8, len/Xoofff_rate, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
}

void testXoofffPerformanceOne( void )
{
    cycles_t calibration = CalibrateTimer();
    uint32_t len;
    cycles_t time;

    time = measureXoofff_MaskDerivation(calibration);
    printf("Xoodoo Mask Derivation %9" PRId64 " %s\n\n", time, getTimerUnit());

    printf("Xoofff_Compress\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofff_Compress(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofff_Compress(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofff_Compress, calibration);
    
    printf("\nXoofff_Expand\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofff_Expand(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofff_Expand(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofff_Expand, calibration);

    printf("\nXoofff-SANE Wrap (only plaintext input, no AD)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANE_Wrap(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANE_Wrap(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofffSANE_Wrap, calibration);

    printf("\nXoofff-SANE MAC (only AD input, no plaintext)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANE_MAC(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANE_MAC(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofffSANE_MAC, calibration);

    printf("\nXoofff-SANSE\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffSANSE(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffSANSE(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    testXooPerfSlope(measureXoofffSANSE, calibration);

    printf("\nXoofff-WBC (Tweak 128 bits)\n");
    for(len=8; len <= 256*Xoofff_rate; len = testXoofffNextLen(len)) {
        time = measureXoofffWBC(calibration, testXoofffAdaptLen(len));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoofffAdaptLen(len)/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
    }
    for(len=128*8; len <= 8192*8; len = len*2) {
        time = measureXoofffWBC(calibration, len);
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", len/8, time, getTimerUnit(), time*1.0/(len/8), getTimerUnit());
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

/* Xoodyak ------------------------------------------- */

#ifdef XKCP_has_Xoodyak

#define	Xoodyak_TagLength	16

static cycles_t measureXoodyak_MAC(cycles_t dtMin, unsigned int ADLen)
{
    ALIGN(64) uint8_t key[16];
    ALIGN(64) uint8_t AD[32*Xoodyak_Rkin];
    ALIGN(64) uint8_t tag[Xoodyak_TagLength];
    Xoodyak_Instance xd;

    assert(ADLen <= sizeof(AD));

    memset(key, 0xA5, sizeof(key));
    memset(AD, 0x5A, ADLen/8);
    Xoodyak_Initialize(&xd, key, sizeof(key), NULL, 0, NULL, 0);
    {
        measureTimingBegin
		Xoodyak_Absorb(&xd, AD, (size_t)ADLen);
	    Xoodyak_Squeeze(&xd, tag, Xoodyak_TagLength);
        measureTimingEnd
    }
}

static cycles_t measureXoodyak_Wrap(cycles_t dtMin, unsigned int inputLen)
{
    ALIGN(64) uint8_t input[32*Xoodyak_Rkout];
    ALIGN(64) uint8_t output[32*Xoodyak_Rkout];
    ALIGN(64) uint8_t key[16];
    ALIGN(64) uint8_t AD[16];
    ALIGN(64) uint8_t tag[Xoodyak_TagLength];
    Xoodyak_Instance xd;

    assert(inputLen <= sizeof(input));

    memset(key, 0xA5, sizeof(key));
    memset(input, 0xA5, sizeof(input));
    memset(AD, 0x5A, sizeof(AD));
    Xoodyak_Initialize(&xd, key, sizeof(key), NULL, 0, NULL, 0);
    {
        measureTimingBegin
		Xoodyak_Absorb(&xd, AD, sizeof(AD));
	    Xoodyak_Encrypt(&xd, input, output, (size_t)inputLen);
	    Xoodyak_Squeeze(&xd, tag, Xoodyak_TagLength);
        measureTimingEnd
    }
}

static cycles_t measureXoodyak_Hash(cycles_t dtMin, unsigned int messageLen)
{
    ALIGN(64) uint8_t message[32*Xoodyak_Rhash];
    ALIGN(64) uint8_t hash[32];
    Xoodyak_Instance xd;

    assert(messageLen <= sizeof(message));

    memset(message, 0xA5, sizeof(message));
    Xoodyak_Initialize(&xd, NULL, 0, NULL, 0, NULL, 0);
    {
        measureTimingBegin
		Xoodyak_Absorb(&xd, message, messageLen);
	    Xoodyak_Squeeze(&xd, hash, sizeof(hash));
        measureTimingEnd
    }
}

uint32_t testXoodyakNextLen(uint32_t len, uint32_t rateInBytes)
{
    if (len < rateInBytes) {
        len <<= 1;
        if (len > rateInBytes)
            len = rateInBytes;
    }
    else
        len <<= 1;
    return len;
}

uint32_t testXoodyakAdaptLen(uint32_t len, uint32_t rateInBytes)
{
    return (len < rateInBytes) ? len : (len-8);
}

static void testXoodyakPerfSlope( measurePerf pFunc, cycles_t calibration, uint32_t rateInBytes )
{
    uint32_t len;
    uint32_t count;
    cycles_t time;
    cycles_t time16;
    cycles_t time32;
    const uint32_t stabilityCount = 10;

    time16 = CYCLES_MAX;
    len = 16*rateInBytes;
    count = stabilityCount;
    do {
        time = pFunc(calibration, len);
        if (time < time16) {
            time16 = time;
            count = stabilityCount;
        }
    } while( --count != 0);
    time32 = CYCLES_MAX;
    len = 32*rateInBytes;
    count = stabilityCount;
    do {
        time = pFunc(calibration, len);
        if (time < time32) {
            time32 = time;
            count = stabilityCount;
        }
    } while( --count != 0);

    time = time32-time16;
    len = 16*rateInBytes;
    printf("Slope %8d bytes (%u blocks): %9" PRId64 " %s, %6.3f %s/byte\n", len, len/rateInBytes, time, getTimerUnit(), time*1.0/len, getTimerUnit());
}

void testXoodyakPerformanceOne( void )
{
    cycles_t calibration = CalibrateTimer();
    uint32_t len;
    cycles_t time;

    printf("\nXoodyak Hash\n");
    for(len=1; len <= 32*Xoodyak_Rhash; len = testXoodyakNextLen(len, Xoodyak_Rhash)) {
        time = measureXoodyak_Hash(calibration, testXoodyakAdaptLen(len, Xoodyak_Rhash));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoodyakAdaptLen(len, Xoodyak_Rhash), time, getTimerUnit(), time*1.0/len, getTimerUnit());
    }
    testXoodyakPerfSlope(measureXoodyak_Hash, calibration, Xoodyak_Rhash);

    printf("\nXoodyak Wrap (plaintext + 16 bytes AD)\n");
    for(len=1; len <= 32*Xoodyak_Rkout; len = testXoodyakNextLen(len, Xoodyak_Rkout)) {
        time = measureXoodyak_Wrap(calibration, testXoodyakAdaptLen(len, Xoodyak_Rkout));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoodyakAdaptLen(len, Xoodyak_Rkout), time, getTimerUnit(), time*1.0/len, getTimerUnit());
    }
    testXoodyakPerfSlope(measureXoodyak_Wrap, calibration, Xoodyak_Rkout);

    printf("\nXoodyak MAC (only AD)\n");
    for(len=1; len <= 32*Xoodyak_Rkin; len = testXoodyakNextLen(len, Xoodyak_Rkin)) {
        time = measureXoodyak_MAC(calibration, testXoodyakAdaptLen(len, Xoodyak_Rkin));
        printf("%8d bytes: %9" PRId64 " %s, %6.3f %s/byte\n", testXoodyakAdaptLen(len, Xoodyak_Rkin), time, getTimerUnit(), time*1.0/len, getTimerUnit());
    }
    testXoodyakPerfSlope(measureXoodyak_MAC, calibration, Xoodyak_Rkin);

    printf("\n\n");
}

void printXoodyakPerformanceHeader( void )
{
    printf("*** Xoodyak ***\n");
    printf("Using Xoodoo implementations:\n");
    printf("- \303\227\x31: " Xoodoo_implementation "\n");
    printf("\n");
}

void testXoodyakPerformance(void)
{
    printXoodyakPerformanceHeader();
    testXoodyakPerformanceOne();
}
#endif

void testXooPerformance(void)
{

#ifdef XKCP_has_Xoodoo
    Xoodoo_timingSnP("Xoodoo", Xoodoo_implementation);
#endif
#if defined(XKCP_has_Xoodootimes4)
    Xoodootimes4_timingPlSnP("Xoodoo\303\2274", Xoodootimes4_implementation);
#endif
#if defined(XKCP_has_Xoodootimes8)
    Xoodootimes8_timingPlSnP("Xoodoo\303\2278", Xoodootimes8_implementation);
#endif
#if defined(XKCP_has_Xoodootimes16)
    Xoodootimes16_timingPlSnP("Xoodoo\303\22716", Xoodootimes16_implementation);
#endif

#ifdef XKCP_has_Xoofff
    testXoofffPerformance();
#endif
#ifdef XKCP_has_Xoodyak
    testXoodyakPerformance();
#endif
}
