/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "align.h"

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define measurePlSnP_PermuteAll                     JOIN(prefix, _measureSnP_Permute)
#define measurePlSnP_PermuteAll_12rounds            JOIN(prefix, _measureSnP_Permute_12rounds)
#define measurePlSnP_FastLoop_Absorb_interleaved    JOIN(prefix, _measurePlSnP_FastLoop_Absorb_interleaved)
#define gatherPlSnP_FastLoop_Absorb_interleaved     JOIN(prefix, _gatherPlSnP_FastLoop_Absorb_interleaved)
#define measurePlSnP_FastLoop_Absorb_segmented      JOIN(prefix, _measurePlSnP_FastLoop_Absorb_segmented)
#define gatherPlSnP_FastLoop_Absorb_segmented       JOIN(prefix, _gatherPlSnP_FastLoop_Absorb_segmented)
#define measurePlSnP_GenericLoop_Absorb_interleaved JOIN(prefix, _measurePlSnP_GenericLoop_Absorb_interleaved)
#define gatherPlSnP_GenericLoop_Absorb_interleaved  JOIN(prefix, _gatherPlSnP_GenericLoop_Absorb_interleaved)
#define doTimingPlSnP                               JOIN(prefix, _timingPlSnP)

#define PlSnP_statesSizeInBytes                     JOIN(PlSnP, _statesSizeInBytes)
#define PlSnP_statesAlignment                       JOIN(PlSnP, _statesAlignment)
#define PlSnP_P                                     PlSnP_parallelism
#define SnP_laneLengthInBytes                       ((SnP_width)/200)
#define SnP_laneCount                               25
#define PlSnP_StaticInitialize                      JOIN(PlSnP, _StaticInitialize)
#define PlSnP_InitializeAll                         JOIN(PlSnP, _InitializeAll)
#define PlSnP_AddLanesAll                           JOIN(PlSnP, _AddLanesAll)

uint_32t measurePlSnP_PermuteAll(uint_32t dtMin)
{
    ALIGN(PlSnP_statesAlignment) unsigned char states[PlSnP_statesSizeInBytes];

    measureTimingBegin
    PlSnP_PermuteAll(states);
    measureTimingEnd
}

#ifdef PlSnP_PermuteAll_12rounds
uint_32t measurePlSnP_PermuteAll_12rounds(uint_32t dtMin)
{
    ALIGN(PlSnP_statesAlignment) unsigned char states[PlSnP_statesSizeInBytes];

    measureTimingBegin
    PlSnP_PermuteAll_12rounds(states);
    measureTimingEnd
}
#endif

#ifdef PlSnP_FastLoop_Absorb
uint_32t measurePlSnP_FastLoop_Absorb_interleaved(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN(PlSnP_statesAlignment) unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN(PlSnP_statesAlignment) unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    measureTimingDeclare
    assert(dataSize <= sizeof(data));

    measureTimingBeginDeclared
    PlSnP_FastLoop_Absorb(&states, laneCount, laneCount, laneCount*PlSnP_P, data, dataSize);
    measureTimingEnd
}

void gatherPlSnP_FastLoop_Absorb_interleaved(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 16, 1);
    measurements[ 1] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 16, 10);
    measurements[ 2] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 16, 100);
    measurements[ 3] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 16, 1000);
    measurements[ 4] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 21, 10);
    measurements[10] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 21, 100);
    measurements[11] = measurePlSnP_FastLoop_Absorb_interleaved(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
}

uint_32t measurePlSnP_FastLoop_Absorb_segmented(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN(PlSnP_statesAlignment) unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN(PlSnP_statesAlignment) unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    measureTimingDeclare
    assert(dataSize <= sizeof(data));

    measureTimingBeginDeclared
    PlSnP_FastLoop_Absorb(&states, laneCount, laneCount*blockCount, laneCount, data, dataSize);
    measureTimingEnd
}

void gatherPlSnP_FastLoop_Absorb_segmented(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 16, 1);
    measurements[ 1] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 16, 10);
    measurements[ 2] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 16, 100);
    measurements[ 3] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 16, 1000);
    measurements[ 4] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 21, 10);
    measurements[10] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 21, 100);
    measurements[11] = measurePlSnP_FastLoop_Absorb_segmented(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
}
#endif

uint_32t measurePlSnP_GenericLoop_Absorb_interleaved(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN(PlSnP_statesAlignment) unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN(PlSnP_statesAlignment) unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    measureTimingDeclare
    assert(dataSize <= sizeof(data));

    measureTimingBeginDeclared
    {
        const unsigned char *curData = data;
        unsigned int rateInBytes = laneCount*SnP_laneLengthInBytes*PlSnP_P;
        unsigned int i;

        for(i=0; i<blockCount; i++) {
            PlSnP_AddLanesAll(states, curData, laneCount, laneCount);
            PlSnP_PermuteAll(states);
            curData += rateInBytes;
        }
    }
    measureTimingEnd
}

void gatherPlSnP_GenericLoop_Absorb_interleaved(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 16, 1);
    measurements[ 1] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 16, 10);
    measurements[ 2] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 16, 100);
    measurements[ 3] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 16, 1000);
    measurements[ 4] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 21, 10);
    measurements[10] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 21, 100);
    measurements[11] = measurePlSnP_GenericLoop_Absorb_interleaved(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
}

void doTimingPlSnP(const char *module, const char *implementation)
{
    uint_32t calibration;
    uint_32t measurement;
    uint_32t measurements[16];
    uint_32t laneCounts[4];

    printf("*** %s ***\n", module);
    printf("Implementation: %s\n\n", implementation);
    calibration = calibrate();

    measurement = measurePlSnP_PermuteAll(calibration);
    printf("Cycles for " xstr(PlSnP_PermuteAll) "(state): %d\n\n", measurement);

#ifdef PlSnP_PermuteAll_12rounds
    measurement = measurePlSnP_PermuteAll_12rounds(calibration);
    printf("Cycles for " xstr(PlSnP_PermuteAll_12rounds) "(state): %d\n\n", measurement);
#endif

    gatherPlSnP_GenericLoop_Absorb_interleaved(calibration, measurements, laneCounts);
    printf("Cycles for loop of " xstr(PlSnP_AddLanesAll) "() and " xstr(PlSnP_PermuteAll) "(): \n");
    displayMeasurements1101001000(measurements, laneCounts, 3, SnP_laneLengthInBytes*PlSnP_P);

#ifdef PlSnP_FastLoop_Absorb
    gatherPlSnP_FastLoop_Absorb_interleaved(calibration, measurements, laneCounts);
    printf("Cycles for PlSnP_FastLoop_Absorb(state, 16, 17 and 21 lanes) interleaved: \n");
    displayMeasurements1101001000(measurements, laneCounts, 3, SnP_laneLengthInBytes*PlSnP_P);

    gatherPlSnP_FastLoop_Absorb_segmented(calibration, measurements, laneCounts);
    printf("Cycles for PlSnP_FastLoop_Absorb(state, 16, 17 and 21 lanes) segmented: \n");
    displayMeasurements1101001000(measurements, laneCounts, 3, SnP_laneLengthInBytes*PlSnP_P);
#endif

    printf("\n");
}

#undef measurePlSnP_PermuteAll
#undef measurePlSnP_PermuteAll_12rounds
#undef measurePlSnP_FastLoop_Absorb_interleaved
#undef gatherPlSnP_FastLoop_Absorb_interleaved
#undef measurePlSnP_FastLoop_Absorb_segmented
#undef gatherPlSnP_FastLoop_Absorb_segmented
#undef measurePlSnP_GenericLoop_Absorb_interleaved
#undef gatherPlSnP_GenericLoop_Absorb_interleaved
#undef doTimingPlSnP
#undef PlSnP_statesSizeInBytes
#undef PlSnP_statesAlignment
#undef PlSnP_P
#undef SnP_laneLengthInBytes
#undef SnP_laneCount
#undef PlSnP_StaticInitialize
#undef PlSnP_InitializeAll
#undef PlSnP_AddLanesAll
