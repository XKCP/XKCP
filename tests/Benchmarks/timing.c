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

#include "timing.h"

const char * getTimerUnit()
{
#if defined(__aarch64__)
    return "ns";
#else
    return "cycles";
#endif
}

double timerCorrectionFactor = 1.0;

static double getTimerCorrectionFactor()
{
#if defined(__aarch64__)
    int64_t virtual_timer_freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(virtual_timer_freq));
    return (double)1.0e9 / (double)virtual_timer_freq;
#else
    return 1.0;
#endif
}

cycles_t CalibrateTimer()
{
    cycles_t dtMin = CYCLES_MAX;        /* big number to start */
    cycles_t t0,t1,i;

    timerCorrectionFactor = getTimerCorrectionFactor();

    for (i=0;i < TIMER_SAMPLE_CNT;i++)  /* calibrate the overhead for measuring time */
        {
        t0 = CycleTimer();
        t1 = CycleTimer();
        if (dtMin > t1-t0)              /* keep only the minimum time */
            dtMin = t1-t0;
        }
    return dtMin;
}
