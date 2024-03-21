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

#include "align.h"

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define measurePlSnP_PermuteAll_6rounds             JOIN(prefix, _measureSnP_Permute_6rounds)
#define measurePlSnP_PermuteAll_12rounds            JOIN(prefix, _measureSnP_Permute_12rounds)
#define doTimingPlSnP                               JOIN(prefix, _timingPlSnP)

#define PlSnP_P                                     PlSnP_parallelism
#define PlSnP_states                                JOIN(PlSnP, _states)
#define PlSnP_StaticInitialize                      JOIN(PlSnP, _StaticInitialize)
#define PlSnP_InitializeAll                         JOIN(PlSnP, _InitializeAll)
#define PlSnP_AddLanesAll                           JOIN(PlSnP, _AddLanesAll)

cycles_t measurePlSnP_PermuteAll_6rounds(cycles_t dtMin)
{
    PlSnP_states states;

    measureTimingBegin
    PlSnP_PermuteAll_6rounds(&states);
    measureTimingEnd
}

cycles_t measurePlSnP_PermuteAll_12rounds(cycles_t dtMin)
{
    PlSnP_states states;

    measureTimingBegin
    PlSnP_PermuteAll_12rounds(&states);
    measureTimingEnd
}


void doTimingPlSnP(const char *module, const char *implementation)
{
    cycles_t calibration;
    cycles_t measurement;

    printf("*** %s ***\n", module);
    printf("Implementation: %s\n\n", implementation);
    calibration = CalibrateTimer();

    measurement = measurePlSnP_PermuteAll_6rounds(calibration);
    printf("Time for " xstr(PlSnP_PermuteAll_6rounds) "(state): %" PRId64 "\n\n", measurement);

    measurement = measurePlSnP_PermuteAll_12rounds(calibration);
    printf("Time for " xstr(PlSnP_PermuteAll_12rounds) "(state): %" PRId64 "\n\n", measurement);

    printf("\n");
}

#undef measurePlSnP_PermuteAll_6rounds
#undef measurePlSnP_PermuteAll_12rounds
#undef doTimingPlSnP
#undef PlSnP_P
#undef PlSnP_states
#undef PlSnP_StaticInitialize
#undef PlSnP_InitializeAll
#undef PlSnP_AddLanesAll
