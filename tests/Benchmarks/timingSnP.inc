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

#include "align.h"

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define measureSnP_Permute              JOIN(prefix, _measureSnP_Permute)
#define measureSnP_Permute_12rounds     JOIN(prefix, _measureSnP_Permute_12rounds)
#define measureSnP_FastLoop_Absorb      JOIN(prefix, _measureSnP_FastLoop_Absorb)
#define gatherSnP_FastLoop_Absorb       JOIN(prefix, _gatherSnP_FastLoop_Absorb)
#define measureSnP_GenericLoop_Absorb      JOIN(prefix, _measureSnP_GenericLoop_Absorb)
#define gatherSnP_GenericLoop_Absorb       JOIN(prefix, _gatherSnP_GenericLoop_Absorb)
#define doTimingSnP                     JOIN(prefix, _timingSnP)

#define SnP_laneLengthInBytes           ((SnP_width)/200)
#define SnP_laneCount                   25
#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                  JOIN(SnP, _Initialize)
#define SnP_AddBytes                    JOIN(SnP, _AddBytes)

cycles_t measureSnP_Permute(cycles_t dtMin)
{
    SnP_state state;

    measureTimingBegin
    SnP_Permute(&state);
    measureTimingEnd
}

#ifdef SnP_Permute_12rounds
cycles_t measureSnP_Permute_12rounds(cycles_t dtMin)
{
    SnP_state state;

    measureTimingBegin
    SnP_Permute_12rounds(&state);
    measureTimingEnd
}
#endif

#ifdef SnP_FastLoop_Absorb
cycles_t measureSnP_FastLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    SnP_state state;
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes;
    measureTimingDeclare
    assert(dataSize <= sizeof(bigBuffer1));

    measureTimingBeginDeclared
    SnP_FastLoop_Absorb(&state, laneCount, bigBuffer1, dataSize);
    measureTimingEnd
}

void gatherSnP_FastLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts);
#endif

cycles_t measureSnP_GenericLoop_Absorb(cycles_t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    SnP_state state;
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes;
    measureTimingDeclare
    assert(dataSize <= sizeof(bigBuffer1));

    measureTimingBeginDeclared
    {
        const unsigned char *curData = bigBuffer1;
        unsigned int rateInBytes = laneCount*SnP_laneLengthInBytes;
        unsigned int i;

        for(i=0; i<blockCount; i++) {
            SnP_AddBytes(&state, curData, 0, rateInBytes);
            SnP_Permute(&state);
            curData += rateInBytes;
        }
    }
    measureTimingEnd
}

unsigned int gatherSnP_GenericLoop_Absorb(cycles_t dtMin, cycles_t *measurements, uint32_t *laneCounts);

void doTimingSnP(const char *module, const char *implementation)
{
    cycles_t calibration;
    cycles_t measurement;
    cycles_t measurements[16];
    uint32_t laneCounts[4];
    unsigned int numberOfColumns;

    printf("*** %s ***\n", module);
    printf("Implementation: %s\n\n", implementation);
#ifdef SnP_FastLoop_Absorb
    measureSnP_FastLoop_Absorb(0, 16, 1000);
#endif
    calibration = CalibrateTimer();

    measurement = measureSnP_Permute(calibration);
    printf("Time for " xstr(SnP_Permute) "(state): %" PRId64 "\n\n", measurement);

#ifdef SnP_Permute_12rounds
    measurement = measureSnP_Permute_12rounds(calibration);
    printf("Time for " xstr(SnP_Permute_12rounds) "(state): %" PRId64 "\n\n", measurement);
#endif

    numberOfColumns = gatherSnP_GenericLoop_Absorb(calibration, measurements, laneCounts);
    printf("Time for loop of " xstr(SnP_AddBytes) "() and " xstr(SnP_Permute) "(): \n");
    displayMeasurements1101001000(measurements, laneCounts, numberOfColumns, SnP_laneLengthInBytes);

#ifdef SnP_FastLoop_Absorb
    gatherSnP_FastLoop_Absorb(calibration, measurements, laneCounts);
    printf("Time for " xstr(SnP_FastLoop_Absorb) "(state, laneCount): \n");
    displayMeasurements1101001000(measurements, laneCounts, 3, SnP_laneLengthInBytes);
#else
    printf("SnP_FastLoop_Absorb() is not supported.\n");
#endif

    printf("\n");
}

#undef measureSnP_Permute
#undef measureSnP_Permute_12rounds
#undef measureSnP_FastLoop_Absorb
#undef gatherSnP_FastLoop_Absorb
#undef doTimingSnP
#undef SnP_laneLengthInBytes
#undef SnP_laneCount
#undef SnP_StaticInitialize
#undef SnP_Initialize
