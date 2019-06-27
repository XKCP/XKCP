/*
-------- Forwarded Message --------
Subject:    RE: Time Trouble
Date:   Mon, 28 Jul 2008 08:07:47 -0400
From:   Doug Whiting <DWhiting at hifn.com>
Reply-To:   hash-forum at nist.gov
To:     Multiple recipients of list <hash-forum at nist.gov>

Sorry for the earlier empty email. I pushed send by mistake while starting my message.

Yes, it's a real shame that C doesn't have a standard way to do this. Below is some code that you are free to copy if you wish I have used variants of this function for years, all the way back to AES days, and the code is entirely mine, so I hereby release it to the public domain. If you keep reading below, I also give some concrete suggestions on how to use it.

This code works on x86 family CPUs (32-big and 64-bit), under MSVC, gcc, and BorlandC, including older compiler versions where the __rdtsc() function is not defined. It also checks for ANSI compiles (i.e., -ansi using gcc, /Za using MSVC, and -A using Borland) and disables the call, to avoid compile-time warnings/errors. The function HiResTime() currently returns only 32 bits, mostly for historical reasons. However, that's enough to do most timing measurements, and you could easily enhance it to return 64 bits if desired. I normally compile with multiple compilers -- e.g., three versions of MSVC (v4.2, v6.0 and v9.0), at least two versions of gcc, plus Borland -- and take performance measurements on all of them.

[…]

*/

/************** Timing routine (for performance measurements) ***********/
/* By Doug Whiting */
/* unfortunately, this is generally assembly code and not very portable */

/* 2019-01-01 Bruno Pairault : ARM CYCLE COUNT IS BASED on PMU assuming Linux Kernel allow User Access to the PMU.
   The method checks on GCC if the Architecture is Aarch64 for PMU Aarch64 or ARM version 7-A to implement PMU 32.
   It's very likely that you have to check all the parameters depending on your OS, GCC native parameters.
   Example : 2019/01/01 : Raspbian OS 9- Kernel 4.14.79 is an Aarch32 OS and declared as ARMv7l.
                          it uses gcc version 6.3.0 20170516 (Raspbian 6.3.0-18+rpi1+deb9u1) with -arch=armv6 as target=native.
                          There's only few OS that support Aarch64 as OpenSuse on RPI3B+.
                          it will then provide performance and Aarch64 PMU.
*/


#if defined(_M_IX86) || defined(__i386) || defined(_i386) || defined(__i386__) || defined(i386) || \
    defined(_X86_)   || defined(__x86_64__) || defined(_M_X64) || defined(__x86_64)
#define _Is_X86_    1
#endif

#if  defined(_Is_X86_) && (!defined(__STRICT_ANSI__)) && (defined(__GNUC__) || !defined(__STDC__)) && \
    (defined(__BORLANDC__) || defined(_MSC_VER) || defined(__MINGW_H) || defined(__GNUC__))
#define HI_RES_CLK_OK         1          /* it's ok to use RDTSC opcode */

#if defined(_MSC_VER) /* && defined(_M_X64) */
#include <intrin.h>
#pragma intrinsic(__rdtsc)         /* use MSVC rdtsc call where defined */
#endif

#endif

typedef unsigned int uint_32t;

static uint_32t HiResTime(void)           /* return the current value of time stamp counter */
    {
#if defined(HI_RES_CLK_OK)
    uint_32t x[2];
#if   defined(__BORLANDC__)
#define COMPILER_ID "BCC"
    __emit__(0x0F,0x31);           /* RDTSC instruction */
    _asm { mov x[0],eax };
#elif defined(_MSC_VER)
#define COMPILER_ID "MSC"
#if defined(_MSC_VER) /* && defined(_M_X64) */
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
    /* Detecting ARM ARCHITECTURE on GCC */
    #ifdef __aarch64__
    /* Implement Aarch64 bits on PMU : Tested on Cortex a-53 */
        uint32_t cycle_count;
        asm volatile("MRS %0, pmevcntr0_el0" : "=r" (cycle_count));
        return cycle_count;
    #elif  defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A_)
    /* Implement Aarch32 bits on PMU : Tested on Cortex a-53 */
        uint32_t cycle_count;
        asm volatile("MRC p15, 0, %0, c9, c13, 0 \t\n" : "=r"(cycle_count));
        return cycle_count;
    #elif defined(__ARM_ARCH_6__)
        #error "Unsupported ARM. Check the option -march and your CPU. Alternatively you can return 0. "
        #error "RPI3 B+ running 32 bits OS will support to force ARCH_7A until these OS updates with there native Arch 6 GCC implementation."
    #endif
#endif
    return 0;
#endif /* defined(HI_RES_CLK_OK) */
    }

#define TIMER_SAMPLE_CNT (100)

static uint_32t calibrate()
{
    uint_32t dtMin = 0xFFFFFFFF;        /* big number to start */
    uint_32t t0,t1,i;
    /* if GCC and ARM then Initialize PMU. Theoretically applied once but choosen to be set for Code Simplication */
    #if (!defined(_MSC_VER)) || (!defined(__STDC__)) || (_MSC_VER < 1300)
        #ifdef __aarch64__
        /* Implement Aarch64 bits on PMU : Tested on Cortex a-53 */
            uint32_t evtCount;
            evtCount= 0x008;
            #define ARMV8_PMEVTYPER_EVTCOUNT_MASK  0x3ff
            evtCount &= ARMV8_PMEVTYPER_EVTCOUNT_MASK;
            asm volatile("isb");
            /* Just use counter 0 */
            asm volatile("msr pmevtyper0_el0, %0" : : "r" (evtCount));
            /*   Performance Monitors Count Enable Set register bit 30:1 disable, 31,1 enable */
            uint32_t r = 0;
            asm volatile("mrs %0, pmcntenset_el0" : "=r" (r));
            asm volatile("msr pmcntenset_el0, %0" : : "r" (r|1));
        #elif  defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A_)
        /* Implement Aarch32 bits on PMU : Tested on Cortex a-53 */
            /* Enable counters in Control Register and reset cycle count and event count */
            printf("PMU32 Enable.. \n");
            asm volatile("MCR   p15, 0, %0, c9, c12, 0" : : "r"(0x00000007));
            /* Event counter selection register, which counter to access */
            asm volatile("MCR   p15, 0, %0, c9, c12, 5" : : "r"(0x0));
            /* selected event type to record, instructions executed */
            asm volatile("MCR   p15, 0, %0, c9, c13, 1" : : "r"(0x00000008));
            /* count enable set register, bit 31 enables the cycle counter,and bit 0 enables the first counter */
            asm volatile("MCR   p15, 0, %0, c9, c12, 1" : : "r"(0x8000000f));
        #elif defined(__ARM_ARCH_6__)
            printf("Do nothing.. Check the configuration. Unsupported ARM\n");
        #endif
    #endif
    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = HiResTime();
        t1 = HiResTime();
        if (dtMin > t1-t0)              /* keep only the minimum time */
            dtMin = t1-t0;
        }
    return dtMin;
}

#define measureTimingDeclare \
    uint_32t tMin = 0xFFFFFFFF; \
    uint_32t t0,t1,i;

#define measureTimingBeginDeclared \
    for (i=0;i < TIMER_SAMPLE_CNT;i++) \
        { \
        t0 = HiResTime();

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
