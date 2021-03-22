// Adapted from Google Benchmark (https://github.com/google/benchmark).
//
// Copyright 2020 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _XKCP_timing_h_
#define _XKCP_timing_h_

#include <stdint.h>

#if defined(__GNUC__)
#define BENCHMARK_ALWAYS_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER) && !defined(__clang__)
#define BENCHMARK_ALWAYS_INLINE __forceinline
#if _MSC_VER >= 1900
#else
#endif
#define __func__ __FUNCTION__
#else
#define BENCHMARK_ALWAYS_INLINE
#endif

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#if defined(__clang__)
  #if defined(__ibmxl__)
    #if !defined(COMPILER_IBMXL)
      #define COMPILER_IBMXL
    #endif
  #elif !defined(COMPILER_CLANG)
    #define COMPILER_CLANG
  #endif
#elif defined(_MSC_VER)
  #if !defined(COMPILER_MSVC)
    #define COMPILER_MSVC
  #endif
#elif defined(__GNUC__)
  #if !defined(COMPILER_GCC)
    #define COMPILER_GCC
  #endif
#endif

#if defined(__CYGWIN__)
  #define BENCHMARK_OS_CYGWIN 1
#elif defined(_WIN32)
  #define BENCHMARK_OS_WINDOWS 1
  #if defined(__MINGW32__)
    #define BENCHMARK_OS_MINGW 1
  #endif
#elif defined(__APPLE__)
  #define BENCHMARK_OS_APPLE 1
  #include "TargetConditionals.h"
  #if defined(TARGET_OS_MAC)
    #define BENCHMARK_OS_MACOSX 1
    #if defined(TARGET_OS_IPHONE)
      #define BENCHMARK_OS_IOS 1
    #endif
  #endif
#elif defined(__FreeBSD__)
  #define BENCHMARK_OS_FREEBSD 1
#elif defined(__NetBSD__)
  #define BENCHMARK_OS_NETBSD 1
#elif defined(__OpenBSD__)
  #define BENCHMARK_OS_OPENBSD 1
#elif defined(__DragonFly__)
  #define BENCHMARK_OS_DRAGONFLY 1
#elif defined(__linux__)
  #define BENCHMARK_OS_LINUX 1
#elif defined(__native_client__)
  #define BENCHMARK_OS_NACL 1
#elif defined(__EMSCRIPTEN__)
  #define BENCHMARK_OS_EMSCRIPTEN 1
#elif defined(__rtems__)
  #define BENCHMARK_OS_RTEMS 1
#elif defined(__Fuchsia__)
#define BENCHMARK_OS_FUCHSIA 1
#elif defined (__SVR4) && defined (__sun)
#define BENCHMARK_OS_SOLARIS 1
#elif defined(__QNX__)
#define BENCHMARK_OS_QNX 1
#elif defined(__MVS__)
#define BENCHMARK_OS_ZOS 1
#endif

#if defined(BENCHMARK_OS_MACOSX)
#include <mach/mach_time.h>
#endif
// For MSVC, we want to use '_asm rdtsc' when possible (since it works
// with even ancient MSVC compilers), and when not possible the
// __rdtsc intrinsic, declared in <intrin.h>.  Unfortunately, in some
// environments, <windows.h> and <intrin.h> have conflicting
// declarations of some other intrinsics, breaking compilation.
// Therefore, we simply declare __rdtsc ourselves. See also
// http://connect.microsoft.com/VisualStudio/feedback/details/262047
#if defined(COMPILER_MSVC) && !defined(_M_IX86)
uint64_t __rdtsc();
#pragma intrinsic(__rdtsc)
#endif

#if !defined(BENCHMARK_OS_WINDOWS) || defined(BENCHMARK_OS_MINGW)
#include <sys/time.h>
#include <time.h>
#endif

#ifdef BENCHMARK_OS_EMSCRIPTEN
#include <emscripten.h>
#endif

// NOTE: only i386 and x86_64 have been well tested.
// PPC, sparc, alpha, and ia64 are based on
//    http://peter.kuscsik.com/wordpress/?p=14
// with modifications by m3b.  See also
//    https://setisvn.ssl.berkeley.edu/svn/lib/fftw-3.0.1/kernel/cycle.h

// This should return the number of cycles since power-on.  Thread-safe.
inline BENCHMARK_ALWAYS_INLINE int64_t CycleTimer() {
#if defined(BENCHMARK_OS_EMSCRIPTEN)
  // this goes above x86-specific code because old versions of Emscripten
  // define __x86_64__, although they have nothing to do with it.
  return (int64_t)(emscripten_get_now() * 1e+6);
#elif defined(__i386__)
  int64_t ret;
  __asm__ volatile("rdtsc" : "=A"(ret));
  return ret;
#elif defined(__x86_64__) || defined(__amd64__)
  uint64_t low, high;
  __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
  return (high << 32) | low;
#elif defined(BENCHMARK_OS_MACOSX)
  // this goes at the top because we need ALL Macs, regardless of
  // architecture, to return the number of "mach time units" that
  // have passed since startup.  See sysinfo.cc where
  // InitializeSystemInfo() sets the supposed cpu clock frequency of
  // macs to the number of mach time units per second, not actual
  // CPU clock frequency (which can change in the face of CPU
  // frequency scaling).  Also note that when the Mac sleeps, this
  // counter pauses; it does not continue counting, nor does it
  // reset to zero.
  // XKCP-specific: moved this below i386 and x86_64 tests to favor real CPU cycles when available
  return mach_absolute_time();
#elif defined(__powerpc__) || defined(__ppc__)
  // This returns a time-base, which is not always precisely a cycle-count.
#if defined(__powerpc64__) || defined(__ppc64__)
  int64_t tb;
  asm volatile("mfspr %0, 268" : "=r"(tb));
  return tb;
#else
  uint32_t tbl, tbu0, tbu1;
  asm volatile(
      "mftbu %0\n"
      "mftb %1\n"
      "mftbu %2"
      : "=r"(tbu0), "=r"(tbl), "=r"(tbu1));
  tbl &= -(int32_t)(tbu0 == tbu1);
  // high 32 bits in tbu1; low 32 bits in tbl  (tbu0 is no longer needed)
  return ((uint64_t)(tbu1) << 32) | tbl;
#endif
#elif defined(__sparc__)
  int64_t tick;
  asm(".byte 0x83, 0x41, 0x00, 0x00");
  asm("mov   %%g1, %0" : "=r"(tick));
  return tick;
#elif defined(__ia64__)
  int64_t itc;
  asm("mov %0 = ar.itc" : "=r"(itc));
  return itc;
#elif defined(COMPILER_MSVC) && defined(_M_IX86)
  // Older MSVC compilers (like 7.x) don't seem to support the
  // __rdtsc intrinsic properly, so I prefer to use _asm instead
  // when I know it will work.  Otherwise, I'll use __rdtsc and hope
  // the code is being compiled with a non-ancient compiler.
  _asm rdtsc
#elif defined(COMPILER_MSVC)
  return __rdtsc();
#elif defined(BENCHMARK_OS_NACL)
  // Native Client validator on x86/x86-64 allows RDTSC instructions,
  // and this case is handled above. Native Client validator on ARM
  // rejects MRC instructions (used in the ARM-specific sequence below),
  // so we handle it here. Portable Native Client compiles to
  // architecture-agnostic bytecode, which doesn't provide any
  // cycle counter access mnemonics.

  // Native Client does not provide any API to access cycle counter.
  // Use clock_gettime(CLOCK_MONOTONIC, ...) instead of gettimeofday
  // because is provides nanosecond resolution (which is noticable at
  // least for PNaCl modules running on x86 Mac & Linux).
  // Initialize to always return 0 if clock_gettime fails.
  struct timespec ts = {0, 0};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (int64_t)(ts.tv_sec) * 1000000000 + ts.tv_nsec;
#elif defined(__aarch64__)
  // System timer of ARMv8 runs at a different frequency than the CPU's.
  // The frequency is fixed, typically in the range 1-50MHz.  It can be
  // read at CNTFRQ special register.  We assume the OS has set up
  // the virtual timer properly.
  int64_t virtual_timer_value;
  asm volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
  return virtual_timer_value;
#elif defined(__ARM_ARCH)
  // V6 is the earliest arch that has a standard cyclecount
  // Native Client validator doesn't allow MRC instructions.
#if (__ARM_ARCH >= 6)
  uint32_t pmccntr;
  uint32_t pmuseren;
  uint32_t pmcntenset;
  // Read the user mode perf monitor counter access permissions.
  asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
  if (pmuseren & 1) {  // Allows reading perfmon counters for user mode code.
    asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
    if (pmcntenset & 0x80000000ul) {  // Is it counting?
      asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
      // The counter is set up to count every 64th cycle
      return (int64_t)(pmccntr) * 64;  // Should optimize to << 6
    }
  }
#endif
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)(tv.tv_sec) * 1000000 + tv.tv_usec;
#elif defined(__mips__) || defined(__m68k__)
  // mips apparently only allows rdtsc for superusers, so we fall
  // back to gettimeofday.  It's possible clock_gettime would be better.
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)(tv.tv_sec) * 1000000 + tv.tv_usec;
#elif defined(__s390__)  // Covers both s390 and s390x.
  // Return the CPU clock.
  uint64_t tsc;
#if defined(BENCHMARK_OS_ZOS) && defined(COMPILER_IBMXL)
  // z/OS XL compiler HLASM syntax.
  asm(" stck %0" : "=m"(tsc) : : "cc");
#else
  asm("stck %0" : "=Q"(tsc) : : "cc");
#endif
  return tsc;
#elif defined(__riscv) // RISC-V
  // Use RDCYCLE (and RDCYCLEH on riscv32)
#if __riscv_xlen == 32
  uint32_t cycles_lo, cycles_hi0, cycles_hi1;
  // This asm also includes the PowerPC overflow handling strategy, as above.
  // Implemented in assembly because Clang insisted on branching.
  asm volatile(
      "rdcycleh %0\n"
      "rdcycle %1\n"
      "rdcycleh %2\n"
      "sub %0, %0, %2\n"
      "seqz %0, %0\n"
      "sub %0, zero, %0\n"
      "and %1, %1, %0\n"
      : "=r"(cycles_hi0), "=r"(cycles_lo), "=r"(cycles_hi1));
  return ((uint64_t)(cycles_hi1) << 32) | cycles_lo;
#else
  uint64_t cycles;
  asm volatile("rdcycle %0" : "=r"(cycles));
  return cycles;
#endif
#else
// The soft failover to a generic implementation is automatic only for ARM.
// For other platforms the developer is expected to make an attempt to create
// a fast implementation and use generic version if nothing better is available.
#error You need to define CycleTimer for your OS and CPU
#endif
}

/* ---------------------------------------------------------------- */
/*           XKCP-specific definitions follow.                      */
/* ---------------------------------------------------------------- */


typedef int64_t cycles_t;
#define CYCLES_MAX INT64_MAX

#define TIMER_SAMPLE_CNT (100)

const char * getTimerUnit();
extern double timerCorrectionFactor;
cycles_t CalibrateTimer();

#define measureTimingDeclare \
    cycles_t tMin = CYCLES_MAX; \
    cycles_t t0,t1,i;

#define measureTimingBeginDeclared \
    for (i=0;i < TIMER_SAMPLE_CNT;i++) \
        { \
        t0 = CycleTimer();

#define measureTimingBegin \
    cycles_t tMin = CYCLES_MAX; \
    cycles_t t0,t1,i; \
    for (i=0;i < TIMER_SAMPLE_CNT;i++) \
        { \
        t0 = CycleTimer();

#define measureTimingEnd \
        t1 = CycleTimer(); \
        if (tMin > t1-t0 - dtMin) \
            tMin = t1-t0 - dtMin; \
        } \
    return (cycles_t)(tMin * timerCorrectionFactor + 0.5);

#endif  // _XKCP_timing_h_
