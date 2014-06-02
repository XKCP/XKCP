/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <stdio.h>
#include <string.h>
#include "KeccakF-1600-interface.h"


void accumulateBuffer(void *stateAccumulated, const unsigned char *buffer)
{
    KeccakF1600_StateXORLanes(stateAccumulated, buffer, 25);
    KeccakF1600_StatePermute(stateAccumulated);
}

void accumulateState(void *stateAccumulated, const void *stateTest)
{
    unsigned char buffer[KeccakF_width/8];
    KeccakF1600_StateExtractLanes(stateTest, buffer, 25);
    accumulateBuffer(stateAccumulated, buffer);
}

void testPermutationAndStateMgt(void)
{
    unsigned char stateAccumulated[KeccakF_width/8];
    unsigned char stateTest[KeccakF_width/8];

    KeccakF1600_Initialize();

    KeccakF1600_StateInitialize(stateAccumulated);

    memset(stateTest, 0xAA, sizeof(stateTest));

    // Testing KeccakF1600_StateInitialize()
    {
        KeccakF1600_StateInitialize(stateTest);
        accumulateState(stateAccumulated, stateTest);
    }
    KeccakF1600_StatePermute(stateTest);

    // Testing KeccakF1600_StateXORBytesInLane()
    {
        unsigned char buffer[KeccakF_laneInBytes+8];
        unsigned i, lanePosition, offset, length, alignment;
        for(i=0; i<sizeof(buffer); i++)
            buffer[i] = 0xF3 + 5*i;

        for(alignment=0; alignment<8; alignment++)
        for(lanePosition=0; lanePosition<25; lanePosition++)
        for(offset=0; offset<KeccakF_laneInBytes; offset++)
        for(length=0; length<=KeccakF_laneInBytes-offset; length++) {
            KeccakF1600_StateXORBytesInLane(stateTest, lanePosition, buffer+alignment, offset, length);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    KeccakF1600_StatePermute(stateTest);

    // Testing KeccakF1600_StateXORLanes()
    {
        unsigned char buffer[KeccakF_width/8+8];
        unsigned i, laneCount, alignment;

        for(alignment=0; alignment<8; alignment++) {
            for(i=0; i<KeccakF_width/8; i++)
                buffer[i+alignment] = 0x74 - 3*i + 8*alignment;

            for(laneCount=0; laneCount<=25; laneCount++) {
                KeccakF1600_StateXORLanes(stateTest, buffer+alignment, laneCount);
                accumulateState(stateAccumulated, stateTest);
            }
        }
    }
    KeccakF1600_StatePermute(stateTest);

    // Testing KeccakF1600_StateComplementBit()
    {
        unsigned bitPosition;

        for(bitPosition=0; bitPosition<KeccakF_width; bitPosition++) {
            KeccakF1600_StateComplementBit(stateTest, bitPosition);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    KeccakF1600_StatePermute(stateTest);

    // Testing KeccakF1600_StateExtractBytesInLane()
    {
        unsigned char buffer[KeccakF_width/8+8];
        unsigned lanePosition, offset, length, alignment;

        for(alignment=0; alignment<8; alignment++)
        for(lanePosition=0; lanePosition<25; lanePosition++)
        for(offset=0; offset<KeccakF_laneInBytes; offset++)
        for(length=0; length<=KeccakF_laneInBytes-offset; length++) {
            memset(buffer, 0x3C+lanePosition+offset+length, sizeof(buffer));
            KeccakF1600_StateExtractBytesInLane(stateTest, lanePosition, buffer+alignment, offset, length);
            accumulateBuffer(stateAccumulated, buffer+alignment);
        }
    }
    KeccakF1600_StatePermute(stateTest);

    // Testing KeccakF1600_StateExtractLanes()
    {
        unsigned char buffer[KeccakF_width/8+8];
        unsigned laneCount, alignment;

        for(alignment=0; alignment<8; alignment++)
        for(laneCount=0; laneCount<=25; laneCount++) {
            memset(buffer, 0xD1+laneCount+32*alignment, sizeof(buffer));
            KeccakF1600_StateExtractLanes(stateTest, buffer+alignment, laneCount);
            accumulateBuffer(stateAccumulated, buffer+alignment);
        }
    }
    KeccakF1600_StatePermute(stateTest);

    // Testing KeccakF1600_StateXORPermuteExtract()
    {
        unsigned char buffer[KeccakF_width/8+8];
        unsigned int i, inLaneCount, outLaneCount, alignment;

        for(alignment=0; alignment<8; alignment++)
        for(inLaneCount=0; inLaneCount<=25; inLaneCount++)
        for(outLaneCount=0; outLaneCount<=25; outLaneCount++) {
            for(i=0; i<KeccakF_width/8; i++)
                buffer[i+alignment] = 0xEB +3*i + 4*inLaneCount - outLaneCount + 16*alignment;
            KeccakF1600_StateXORPermuteExtract(stateTest, buffer+alignment, inLaneCount, buffer+alignment, outLaneCount);
            accumulateState(stateAccumulated, stateTest);
            accumulateBuffer(stateAccumulated, buffer+alignment);
        }
    }

    // Outputting the result
    {
        unsigned char buffer[KeccakF_width/8];
        unsigned int i;
        FILE *f;

        KeccakF1600_StateExtractLanes(stateAccumulated, buffer, 25);
        f = fopen("TestKeccakF-1600AndStateMgt.txt", "w");
        fprintf(f, "Testing Keccak-f[1600] permutation and state management: ");
        for(i=0; i<KeccakF_width/8; i++)
            fprintf(f, "%02x ", buffer[i]);
        fprintf(f, "\n");
        fclose(f);
    }
}
