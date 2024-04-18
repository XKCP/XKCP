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

#define measureSnP_Permute_6rounds      JOIN(prefix, _measureSnP_Permute_6rounds)
#define measureSnP_Permute_12rounds     JOIN(prefix, _measureSnP_Permute_12rounds)
#define doTimingSnP                     JOIN(prefix, _timingSnP)

#define SnP_laneLengthInBytes           ((SnP_width)/(3*4))
#define SnP_laneCount                   (3*4)
#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                  JOIN(SnP, _Initialize)
#define SnP_AddBytes                    JOIN(SnP, _AddBytes)

cycles_t measureSnP_Permute_6rounds(cycles_t dtMin)
{
    SnP_state state;

    measureTimingBegin
    SnP_Permute_6rounds(&state);
    measureTimingEnd
}

cycles_t measureSnP_Permute_12rounds(cycles_t dtMin)
{
    SnP_state state;

    measureTimingBegin
    SnP_Permute_12rounds(&state);
    measureTimingEnd
}

void doTimingSnP(const char *module, const char *implementation)
{
    cycles_t calibration;
    cycles_t measurement;

    printf("*** %s ***\n", module);
    printf("Implementation: %s\n\n", implementation);
    calibration = CalibrateTimer();

    measurement = measureSnP_Permute_6rounds(calibration);
    printf("Time for " xstr(SnP_Permute_6rounds) "(state): %" PRId64 "\n\n", measurement);

    measurement = measureSnP_Permute_12rounds(calibration);
    printf("Time for " xstr(SnP_Permute_12rounds) "(state): %" PRId64 "\n\n", measurement);

    printf("\n");
}

#undef measureSnP_Permute_6rounds
#undef measureSnP_Permute_12rounds
#undef doTimingSnP
#undef SnP_laneLengthInBytes
#undef SnP_laneCount
#undef SnP_StaticInitialize
#undef SnP_Initialize
