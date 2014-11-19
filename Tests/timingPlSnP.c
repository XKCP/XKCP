/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "PlSnP-interface.h"
#include "timing.h"
#include "timingPlSnP.h"

// on Mac OS-X and possibly others, ALIGN(x) is defined in param.h, and -Werror chokes on the redef.
#ifdef ALIGN
#undef ALIGN
#endif

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(32))
#else
#define ALIGN
#endif

uint_32t measurePlSnP_PermuteAll(uint_32t dtMin)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];

    measureTimingBegin
    PlSnP_PermuteAll(states);
    measureTimingEnd
}

uint_32t measurePlSnP_FBWL_Absorb_interleaved(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    assert(dataSize <= sizeof(data));

    measureTimingBegin
    PlSnP_FBWL_Absorb(&states, laneCount, laneCount, laneCount*PlSnP_P, data, dataSize, 0x12);
    measureTimingEnd
}

void gatherPlSnP_FBWL_Absorb_interleaved(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 16, 1);
    measurements[ 1] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 16, 10);
    measurements[ 2] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 16, 100);
    measurements[ 3] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 16, 1000);
    measurements[ 4] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 21, 10);
    measurements[10] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 21, 100);
    measurements[11] = measurePlSnP_FBWL_Absorb_interleaved(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
}

uint_32t measurePlSnP_FBWL_Absorb_segmented(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    assert(dataSize <= sizeof(data));

    measureTimingBegin
    PlSnP_FBWL_Absorb(&states, laneCount, laneCount*blockCount, laneCount, data, dataSize, 0x13);
    measureTimingEnd
}

void gatherPlSnP_FBWL_Absorb_segmented(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 16, 1);
    measurements[ 1] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 16, 10);
    measurements[ 2] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 16, 100);
    measurements[ 3] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 16, 1000);
    measurements[ 4] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 21, 10);
    measurements[10] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 21, 100);
    measurements[11] = measurePlSnP_FBWL_Absorb_segmented(dtMin, 21, 1000);
    laneCounts[0] = 16;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
}

uint_32t measurePlSnP_FBWL_Wrap_1buffer(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    assert(dataSize <= sizeof(data));

    measureTimingBegin
    PlSnP_FBWL_Wrap(&states, laneCount, laneCount, laneCount*PlSnP_P, data, data, dataSize, 0x01);
    measureTimingEnd
}

uint_32t measurePlSnP_FBWL_Wrap_2buffers(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN unsigned char data1[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    ALIGN unsigned char data2[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    assert(dataSize <= sizeof(data1));

    measureTimingBegin
    PlSnP_FBWL_Wrap(&states, laneCount, laneCount, laneCount*PlSnP_P, data1, data2, dataSize, 0x01);
    measureTimingEnd
}

void gatherPlSnP_FBWL_Wrap(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 17, 1);
    measurements[ 1] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 17, 10);
    measurements[ 2] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 17, 100);
    measurements[ 3] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 17, 1000);
    measurements[ 4] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 21, 10);
    measurements[10] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 21, 100);
    measurements[11] = measurePlSnP_FBWL_Wrap_1buffer(dtMin, 21, 1000);
    measurements[12] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 21, 1);
    measurements[13] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 21, 10);
    measurements[14] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 21, 100);
    measurements[15] = measurePlSnP_FBWL_Wrap_2buffers(dtMin, 21, 1000);
    laneCounts[0] = 17;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
    laneCounts[3] = 21;
}

uint_32t measurePlSnP_FBWL_Unwrap_1buffer(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN unsigned char data[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    assert(dataSize <= sizeof(data));

    measureTimingBegin
    PlSnP_FBWL_Unwrap(&states, laneCount, laneCount, laneCount*PlSnP_P, data, data, dataSize, 0x01);
    measureTimingEnd
}

uint_32t measurePlSnP_FBWL_Unwrap_2buffers(uint_32t dtMin, unsigned int laneCount, unsigned int blockCount)
{
    ALIGN unsigned char states[PlSnP_statesSizeInBytes];
    ALIGN unsigned char data1[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    ALIGN unsigned char data2[1000*25*SnP_laneLengthInBytes*PlSnP_P];
    unsigned int dataSize = laneCount*blockCount*SnP_laneLengthInBytes*PlSnP_P;
    assert(dataSize <= sizeof(data1));

    measureTimingBegin
    PlSnP_FBWL_Unwrap(&states, laneCount, laneCount, laneCount*PlSnP_P, data1, data2, dataSize, 0x01);
    measureTimingEnd
}

void gatherPlSnP_FBWL_Unwrap(uint_32t dtMin, uint_32t *measurements, uint_32t *laneCounts)
{
    measurements[ 0] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 17, 1);
    measurements[ 1] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 17, 10);
    measurements[ 2] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 17, 100);
    measurements[ 3] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 17, 1000);
    measurements[ 4] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 17, 1);
    measurements[ 5] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 17, 10);
    measurements[ 6] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 17, 100);
    measurements[ 7] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 17, 1000);
    measurements[ 8] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 21, 1);
    measurements[ 9] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 21, 10);
    measurements[10] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 21, 100);
    measurements[11] = measurePlSnP_FBWL_Unwrap_1buffer(dtMin, 21, 1000);
    measurements[12] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 21, 1);
    measurements[13] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 21, 10);
    measurements[14] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 21, 100);
    measurements[15] = measurePlSnP_FBWL_Unwrap_2buffers(dtMin, 21, 1000);
    laneCounts[0] = 17;
    laneCounts[1] = 17;
    laneCounts[2] = 21;
    laneCounts[3] = 21;
}

void bubbleSort(double *list, unsigned int size)
{
    unsigned int n = size;

    do {
       unsigned int newn = 0;
       unsigned int i;

       for(i=1; i<n; i++) {
          if (list[i-1] > list[i]) {
              double temp = list[i-1];
              list[i-1] = list[i];
              list[i] = temp;
              newn = i;
          }
       }
       n = newn;
    }
    while(n > 0);
}

double med4(double x0, double x1, double x2, double x3)
{
    double list[4];
    list[0] = x0;
    list[1] = x1;
    list[2] = x2;
    list[3] = x3;
    bubbleSort(list, 4);
    if (fabs(list[2]-list[0]) < fabs(list[3]-list[1]))
        return 0.25*list[0]+0.375*list[1]+0.25*list[2]+0.125*list[3];
    else
        return 0.125*list[0]+0.25*list[1]+0.375*list[2]+0.25*list[3];
}

void displayMeasurements1101001000(uint_32t *measurements, uint_32t *laneCounts, unsigned int numberOfColumns)
{
    double cpb[4];
    unsigned int i;

    for(i=0; i<numberOfColumns; i++) {
        uint_32t bytes = laneCounts[i]*SnP_laneLengthInBytes*PlSnP_P;
        double x = med4(measurements[i*4+0]*1.0, measurements[i*4+1]/10.0, measurements[i*4+2]/100.0, measurements[i*4+3]/1000.0);
        cpb[i] = x/bytes;
    }
    if (numberOfColumns == 3) {
        printf("       1 block:  %5d       %5d       %5d\n", measurements[0], measurements[4], measurements[8]);
        printf("      10 blocks: %6d      %6d      %6d\n", measurements[1], measurements[5], measurements[9]);
        printf("     100 blocks: %7d     %7d     %7d\n", measurements[2], measurements[6], measurements[10]);
        printf("    1000 blocks: %8d    %8d    %8d\n", measurements[3], measurements[7], measurements[11]);
        printf("    cycles/byte: %7.2f     %7.2f     %7.2f\n", cpb[0], cpb[1], cpb[2]);
    }
    else if (numberOfColumns == 4) {
        printf("       1 block:  %5d       %5d       %5d       %5d\n", measurements[0], measurements[4], measurements[8], measurements[12]);
        printf("      10 blocks: %6d      %6d      %6d      %6d\n", measurements[1], measurements[5], measurements[9], measurements[13]);
        printf("     100 blocks: %7d     %7d     %7d     %7d\n", measurements[2], measurements[6], measurements[10], measurements[14]);
        printf("    1000 blocks: %8d    %8d    %8d    %8d\n", measurements[3], measurements[7], measurements[11], measurements[15]);
        printf("    cycles/byte: %7.2f     %7.2f     %7.2f     %7.2f\n", cpb[0], cpb[1], cpb[2], cpb[3]);
    }
    printf("\n");
}

void doTimingPlSnP(void)
{
    uint_32t calibration;
    uint_32t measurement;
    uint_32t measurements[16];
    uint_32t laneCounts[4];

    measurePlSnP_FBWL_Absorb_interleaved(0, 16, 1000);
    calibration = calibrate();

    measurement = measurePlSnP_PermuteAll(calibration);
    printf("Cycles for PlSnP_PermuteAll(state): %d\n\n", measurement);

    gatherPlSnP_FBWL_Absorb_interleaved(calibration, measurements, laneCounts);
    printf("Cycles for PlSnP_FBWL_Absorb(state, 16, 17 and 21 lanes) interleaved: \n");
    displayMeasurements1101001000(measurements, laneCounts, 3);

    gatherPlSnP_FBWL_Absorb_segmented(calibration, measurements, laneCounts);
    printf("Cycles for PlSnP_FBWL_Absorb(state, 16, 17 and 21 lanes) segmented: \n");
    displayMeasurements1101001000(measurements, laneCounts, 3);

    gatherPlSnP_FBWL_Wrap(calibration, measurements, laneCounts);
    printf("Cycles for PlSnP_FBWL_Wrap(state, 17 lanes 1 and 2 buffers, 21 lanes 1 and 2 buffers): \n");
    displayMeasurements1101001000(measurements, laneCounts, 4);

    gatherPlSnP_FBWL_Unwrap(calibration, measurements, laneCounts);
    printf("Cycles for PlSnP_FBWL_Unwrap(state, 17 lanes 1 and 2 buffers, 21 lanes 1 and 2 buffers): \n");
    displayMeasurements1101001000(measurements, laneCounts, 4);
}
