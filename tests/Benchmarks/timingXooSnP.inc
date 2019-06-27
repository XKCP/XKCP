/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
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

#define SnP_stateAlignment              JOIN(SnP, _stateAlignment)
#define SnP_stateSizeInBytes            JOIN(SnP, _stateSizeInBytes)
#define SnP_laneLengthInBytes           ((SnP_width)/(3*4))
#define SnP_laneCount                   (3*4)
#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                  JOIN(SnP, _Initialize)
#define SnP_AddBytes                    JOIN(SnP, _AddBytes)

uint_32t measureSnP_Permute_6rounds(uint_32t dtMin)
{
    ALIGN(SnP_stateAlignment) unsigned char state[SnP_stateSizeInBytes];

    measureTimingBegin
    SnP_Permute_6rounds(state);
    measureTimingEnd
}

uint_32t measureSnP_Permute_12rounds(uint_32t dtMin)
{
    ALIGN(SnP_stateAlignment) unsigned char state[SnP_stateSizeInBytes];

    measureTimingBegin
    SnP_Permute_12rounds(state);
    measureTimingEnd
}

void doTimingSnP(const char *module, const char *implementation)
{
    uint_32t calibration;
    uint_32t measurement;
    //!!uint_32t measurements[16];
    //uint_32t laneCounts[4];
//    unsigned int numberOfColumns;

    printf("*** %s ***\n", module);
    printf("Implementation: %s\n\n", implementation);
    calibration = calibrate();

    measurement = measureSnP_Permute_6rounds(calibration);
    printf("Cycles for " xstr(SnP_Permute_6rounds) "(state): %d\n\n", measurement);

    measurement = measureSnP_Permute_12rounds(calibration);
    printf("Cycles for " xstr(SnP_Permute_12rounds) "(state): %d\n\n", measurement);

    printf("\n");
}

#undef measureSnP_Permute_6rounds
#undef measureSnP_Permute_12rounds
#undef doTimingSnP
#undef SnP_stateAlignment
#undef SnP_stateSizeInBytes
#undef SnP_laneLengthInBytes
#undef SnP_laneCount
#undef SnP_StaticInitialize
#undef SnP_Initialize
