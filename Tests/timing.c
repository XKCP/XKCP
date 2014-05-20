/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/
*/


#include <stdio.h>

/************** Timing routine (for performance measurements) ***********/
/* By Doug Whiting */
/* unfortunately, this is generally assembly code and not very portable */
#if defined(_M_IX86) || defined(__i386) || defined(_i386) || defined(__i386__) || defined(i386) || \
    defined(_X86_)   || defined(__x86_64__) || defined(_M_X64) || defined(__x86_64)
#define _Is_X86_    1
#endif

#if  defined(_Is_X86_) && (!defined(__STRICT_ANSI__)) && (defined(__GNUC__) || !defined(__STDC__)) && \
    (defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW_H) || defined(__GNUC__))
#define HI_RES_CLK_OK         1          /* it's ok to use RDTSC opcode */

#if defined(_MSC_VER) // && defined(_M_X64)
#include <intrin.h>
#pragma intrinsic(__rdtsc)         /* use MSVC rdtsc call where defined */
#endif

#endif

typedef unsigned int uint_32t;

uint_32t HiResTime(void)           /* return the current value of time stamp counter */
    {
#if defined(HI_RES_CLK_OK)
    uint_32t x[2];
#if   defined(__BORLANDC__)
#define COMPILER_ID "BCC"
    __emit__(0x0F,0x31);           /* RDTSC instruction */
    _asm { mov x[0],eax };
#elif defined(_MSC_VER)
#define COMPILER_ID "MSC"
#if defined(_MSC_VER) // && defined(_M_X64)
    x[0] = (uint_32t) __rdtsc();
#else
    _asm { _emit 0fh }; _asm { _emit 031h };
    _asm { mov x[0],eax };
#endif
#elif defined(__MINGW_H) || defined(__GNUC__)
#define COMPILER_ID "GCC"
    asm volatile("rdtsc" : "=a"(x[0]), "=d"(x[1]));
#else
#error  "HI_RES_CLK_OK -- but no assembler code for this platform (?)"
#endif
    return x[0];
#else
    /* avoid annoying MSVC 9.0 compiler warning #4720 in ANSI mode! */
#if (!defined(_MSC_VER)) || (!defined(__STDC__)) || (_MSC_VER < 1300)
    FatalError("No support for RDTSC on this CPU platform\n");
#endif
    return 0;
#endif /* defined(HI_RES_CLK_OK) */
    }

#define TIMER_SAMPLE_CNT (100)

uint_32t calibrate()
{
    uint_32t dtMin = 0xFFFFFFFF;        /* big number to start */
    uint_32t t0,t1,i;

    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = HiResTime();
        t1 = HiResTime();
        if (dtMin > t1-t0)              /* keep only the minimum time */
            dtMin = t1-t0;
        }
    return dtMin;
}

#include "KeccakF-1600-interface.h"
#include "KeccakDuplex.h"
#include "KeccakSponge.h"

#define measureTimingBegin \
    uint_32t tMin = 0xFFFFFFFF; \
    uint_32t t0,t1,i; \
    for (i=0;i < TIMER_SAMPLE_CNT;i++) \
        { \
        t0 = HiResTime();

#define measureTimingEnd \
        t1 = HiResTime(); \
        if (tMin > t1-t0 - dtMin) \
            tMin = t1-t0 - dtMin; \
        } \
    return tMin;

uint_32t measureKeccakF1600_StatePermute(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];

    measureTimingBegin
    KeccakF1600_StatePermute(state);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_0_0(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, 0, 0, 0, 0);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_16_0(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];
    ALIGN unsigned char data[200];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, data, 16, 0, 0);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_17_0(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];
    ALIGN unsigned char data[200];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, data, 17, 0, 0);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_21_0(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];
    ALIGN unsigned char data[200];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, data, 21, 0, 0);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_0_16(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];
    ALIGN unsigned char data[200];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, 0, 0, data, 16);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_0_17(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];
    ALIGN unsigned char data[200];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, 0, 0, data, 17);
    measureTimingEnd
}

uint_32t measureKeccakF1600_StateXORPermuteExtract_0_21(uint_32t dtMin)
{
    ALIGN unsigned char state[KeccakF_width/8];
    ALIGN unsigned char data[200];

    measureTimingBegin
    KeccakF1600_StateXORPermuteExtract(state, 0, 0, data, 21);
    measureTimingEnd
}

uint_32t measureKeccakAbsorb1000blocks(uint_32t dtMin)
{
    Keccak_SpongeInstance sponge;
    ALIGN unsigned char data[1000*200];

    measureTimingBegin
    Keccak_SpongeInitialize(&sponge, 1344, 256);
    Keccak_SpongeAbsorb(&sponge, data, 999*1344/8+1);
    Keccak_SpongeAbsorbLastFewBits(&sponge, 0x01);
    measureTimingEnd
}

uint_32t measureKeccakSqueeze1000blocks(uint_32t dtMin)
{
    Keccak_SpongeInstance sponge;
    ALIGN unsigned char data[1000*200];

    measureTimingBegin
    Keccak_SpongeInitialize(&sponge, 1344, 256);
    Keccak_SpongeSqueeze(&sponge, data, 1000*1344/8);
    measureTimingEnd
}

uint_32t measureKeccakDuplexing1000blocks(uint_32t dtMin)
{
    Keccak_DuplexInstance duplex;
    int j;
    ALIGN unsigned char dataIn[200];
    ALIGN unsigned char dataOut[200];

    measureTimingBegin
    Keccak_DuplexInitialize(&duplex, 1344+3, 256-3);
    for(j=0; j<1000; j++)
        Keccak_Duplexing(&duplex, dataIn, 1344/8, dataOut, 1344/8, 0x03);
    measureTimingEnd
}

void doTiming(void)
{
    uint_32t calibration;
    uint_32t measurement;

    measureKeccakAbsorb1000blocks(0);
    calibration = calibrate();

    measurement = measureKeccakF1600_StatePermute(calibration);
    printf("Cycles for KeccakF1600_StatePermute(state): %d\n\n", measurement);

    measurement = measureKeccakF1600_StateXORPermuteExtract_0_0(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, 0, 0, 0, 0): %d\n\n", measurement);

    measurement = measureKeccakF1600_StateXORPermuteExtract_16_0(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, data, 16, 0, 0): %d\n", measurement);
    printf("Cycles per byte for rate 1024: %f\n\n", measurement/128.0);

    measurement = measureKeccakF1600_StateXORPermuteExtract_17_0(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, data, 17, 0, 0): %d\n", measurement);
    printf("Cycles per byte for rate 1088: %f\n\n", measurement/136.0);

    measurement = measureKeccakF1600_StateXORPermuteExtract_21_0(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, data, 21, 0, 0): %d\n", measurement);
    printf("Cycles per byte for rate 1344: %f\n\n", measurement/168.0);

    measurement = measureKeccakF1600_StateXORPermuteExtract_0_16(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, 0, 0, data, 16): %d\n", measurement);
    printf("Cycles per byte for rate 1024: %f\n\n", measurement/128.0);

    measurement = measureKeccakF1600_StateXORPermuteExtract_0_17(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, 0, 0, data, 17): %d\n", measurement);
    printf("Cycles per byte for rate 1088: %f\n\n", measurement/136.0);

    measurement = measureKeccakF1600_StateXORPermuteExtract_0_21(calibration);
    printf("Cycles for KeccakF1600_StateXORPermuteExtract(state, 0, 0, data, 21): %d\n", measurement);
    printf("Cycles per byte for rate 1344: %f\n\n", measurement/168.0);

    measurement = measureKeccakAbsorb1000blocks(calibration);
    printf("Cycles for Keccak_SpongeInitialize, Absorb (1000 blocks) and AbsorbLastFewBits: %d\n\n", measurement);

    measurement = measureKeccakSqueeze1000blocks(calibration);
    printf("Cycles for Keccak_SpongeInitialize and Squeeze (1000 blocks): %d\n\n", measurement);

    measurement = measureKeccakDuplexing1000blocks(calibration);
    printf("Cycles for Keccak_DuplexInitialize and Duplexing (1000 blocks): %d\n\n", measurement);
}
