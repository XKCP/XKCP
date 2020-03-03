/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <cassert>
#include <string.h>
#include "Exception.h"
#include "SnP.h"
#include "Xoodoo.h"

bool UT_VERBOSE = false;

using namespace std;

void accumulateBuffer(SnP& stateAccumulated, const uint8_t *buffer)
{
    stateAccumulated.AddBytes(buffer, 0, stateAccumulated.getWidth()/8);
    stateAccumulated.Permute();
}

void accumulateState(SnP& stateAccumulated, const SnP& stateTest)
{
    uint8_t buffer[stateTest.getWidth()/8];
    stateTest.ExtractBytes(buffer, 0, stateTest.getWidth()/8);
    accumulateBuffer(stateAccumulated, buffer);
}

void displayAccumulated(const string& synopsis, const SnP& stateAccumulated)
{
    const unsigned int SnP_width = stateAccumulated.getWidth();
    unsigned char buffer[SnP_width/8];
    unsigned int i;

    cout << synopsis << ": ";
    stateAccumulated.ExtractBytes(buffer, 0, SnP_width/8);
    for(i=0; i<SnP_width/8; i++)
        printf("\\x%02x", buffer[i]);
    cout << endl;
}

void dumpState(const SnP& state, const string& message)
{
    const unsigned int SnP_width = state.getWidth();
    const unsigned int SnP_laneLengthInBytes = state.getLaneSizeInBytes();
    const unsigned int SnP_laneCount = (SnP_width/8) / SnP_laneLengthInBytes;
    unsigned int i;
    unsigned char buffer[SnP_width/8];

    state.ExtractBytes(buffer, 0, SnP_width/8 );
    cout << endl << message << endl;
    {
        unsigned char *state = buffer;
        for ( i = 0; i < SnP_laneCount; ++i, state += SnP_laneLengthInBytes )
        {

            if (SnP_laneLengthInBytes == 8)
                printf("%08X%08X ", ((uint32_t*)state)[1], ((uint32_t*)state)[0] );
            else if (SnP_laneLengthInBytes == 4)
                printf("%08X ", ((uint32_t*)state)[0] );
            else if (SnP_laneLengthInBytes == 2)
                printf("%04X ", ((uint16_t*)state)[0] );
            else if (SnP_laneLengthInBytes == 1)
                printf("%02X ", ((uint8_t*)state)[0] );

            if (SnP_laneCount == 25) {
                if ( (i % 5) == 4 )
                    printf("\n" );
            }
            else {
                if ( (i % 4) == 3 )
                    printf("\n" );
            }
        }
    }
}

void nominalTest_KeccakP_wide(unsigned int width, const string& synopsis)
{
    try {
        SnP_KeccakP stateAccumulated(width);
        SnP_KeccakP stateTest(width);
        const unsigned int SnP_Permute_maxRounds = stateTest.getDefaultRoundCount();
        const unsigned int SnP_width = stateTest.getWidth();
        const unsigned int SnP_laneLengthInBytes = stateTest.getLaneSizeInBytes();
        const unsigned int SnP_laneCount = (SnP_width/8) / SnP_laneLengthInBytes;

#define SnP_StaticInitialize()
#define SnP_Initialize(snp)                                         snp.Initialize()
#define SnP_AddByte(snp, data, offset)                              snp.AddByte(data, offset)
#define SnP_AddBytes(snp, data, offset, length)                     snp.AddBytes(data, offset, length)
#define SnP_OverwriteBytes(snp, data, offset, length)               snp.OverwriteBytes(data, offset, length)
#define SnP_OverwriteWithZeroes(snp, byteCount)                     snp.OverwriteWithZeroes(byteCount)
#define SnP_Permute(snp)                                            snp.Permute()
#define SnP_Permute_12rounds(snp)                                   snp.Permute_Nrounds(12)
#define SnP_Permute_Nrounds(snp, nr)                                snp.Permute_Nrounds(nr)
#define SnP_ExtractBytes(snp, data, offset, length)                 snp.ExtractBytes(data, offset, length)
#define SnP_ExtractAndAddBytes(snp, input, output, offset, length)  snp.ExtractAndAddBytes(input, output, offset, length)
#define SnP_FastLoop_Absorb(snp, laneCount, data, dataByteLen)      snp.FastLoop_Absorb(laneCount, data, dataByteLen)
#define DUMP(state, message)                                        if (UT_VERBOSE) dumpState(state, message)
#include "testSnPnominal.inc"
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_Permute
#undef SnP_Permute_12rounds
#undef SnP_Permute_Nrounds
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes
#undef SnP_FastLoop_Absorb
#undef DUMP

        displayAccumulated(synopsis, stateAccumulated);
    }
    catch(Exception e) {
        cout << e.reason << endl;
    }
}

void nominalTest_KeccakP_narrow(unsigned int width, const string& synopsis)
{
    try {
        SnP_KeccakP stateAccumulated(width);
        SnP_KeccakP stateTest(width);
        const unsigned int SnP_Permute_maxRounds = stateTest.getDefaultRoundCount();
        const unsigned int SnP_width = stateTest.getWidth();
        const unsigned int SnP_laneCount = 25;
        const unsigned int SnP_laneLengthInBytes = (SnP_width/8) / SnP_laneCount;

#define SnP_StaticInitialize()
#define SnP_Initialize(snp)                                         snp.Initialize()
#define SnP_AddByte(snp, data, offset)                              snp.AddByte(data, offset)
#define SnP_AddBytes(snp, data, offset, length)                     snp.AddBytes(data, offset, length)
#define SnP_OverwriteBytes(snp, data, offset, length)               snp.OverwriteBytes(data, offset, length)
#define SnP_OverwriteWithZeroes(snp, byteCount)                     snp.OverwriteWithZeroes(byteCount)
#define SnP_Permute(snp)                                            snp.Permute()
#define SnP_Permute_Nrounds(snp, nr)                                snp.Permute_Nrounds(nr)
#define SnP_ExtractBytes(snp, data, offset, length)                 snp.ExtractBytes(data, offset, length)
#define SnP_ExtractAndAddBytes(snp, input, output, offset, length)  snp.ExtractAndAddBytes(input, output, offset, length)
#define SnP_FastLoop_Absorb(snp, laneCount, data, dataByteLen)      snp.FastLoop_Absorb(laneCount, data, dataByteLen)
#define DUMP(state, message)                                        if (UT_VERBOSE) dumpState(state, message)
#include "testSnPnominal.inc"
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_Permute
#undef SnP_Permute_Nrounds
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes
#undef SnP_FastLoop_Absorb
#undef DUMP

        displayAccumulated(synopsis, stateAccumulated);
    }
    catch(Exception e) {
        cout << e.reason << endl;
    }
}

void nominalTest_Xoodoo(const string& synopsis)
{
    try {
        const unsigned int width = 384;
        SnP_Xoodoo stateAccumulated;
        SnP_Xoodoo stateTest;
        const unsigned int SnP_Permute_maxRounds = 12;
        const unsigned int SnP_width = width;

#define SnP_StaticInitialize()
#define SnP_Initialize(snp)                                         snp.Initialize()
#define SnP_AddByte(snp, data, offset)                              snp.AddByte(data, offset)
#define SnP_AddBytes(snp, data, offset, length)                     snp.AddBytes(data, offset, length)
#define SnP_OverwriteBytes(snp, data, offset, length)               snp.OverwriteBytes(data, offset, length)
#define SnP_OverwriteWithZeroes(snp, byteCount)                     snp.OverwriteWithZeroes(byteCount)
#define SnP_Permute(snp)                                            snp.Permute()
#define SnP_Permute_6rounds(snp)                                    snp.Permute_Nrounds(6)
#define SnP_Permute_12rounds(snp)                                   snp.Permute_Nrounds(12)
#define SnP_Permute_Nrounds(snp, nr)                                snp.Permute_Nrounds(nr)
#define SnP_ExtractBytes(snp, data, offset, length)                 snp.ExtractBytes(data, offset, length)
#define SnP_ExtractAndAddBytes(snp, input, output, offset, length)  snp.ExtractAndAddBytes(input, output, offset, length)
#define SnP_NoFastLoopAbsorb
#define DUMP(state, message)                                        if (UT_VERBOSE) dumpState(state, message)
#include "testSnPnominal.inc"
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_Permute
#undef SnP_Permute_6rounds
#undef SnP_Permute_12rounds
#undef SnP_Permute_Nrounds
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes
#undef SnP_NoFastLoopAbsorb
#undef DUMP

        displayAccumulated(synopsis, stateAccumulated);
    }
    catch(Exception e) {
        cout << e.reason << endl;
    }
}

void nominalTest_Xoodoo_noNrounds(const string& synopsis)
{
    try {
        const unsigned int width = 384;
        SnP_Xoodoo stateAccumulated;
        SnP_Xoodoo stateTest;
        const unsigned int SnP_width = width;

#define SnP_StaticInitialize()
#define SnP_Initialize(snp)                                         snp.Initialize()
#define SnP_AddByte(snp, data, offset)                              snp.AddByte(data, offset)
#define SnP_AddBytes(snp, data, offset, length)                     snp.AddBytes(data, offset, length)
#define SnP_OverwriteBytes(snp, data, offset, length)               snp.OverwriteBytes(data, offset, length)
#define SnP_OverwriteWithZeroes(snp, byteCount)                     snp.OverwriteWithZeroes(byteCount)
#define SnP_Permute(snp)                                            snp.Permute()
#define SnP_Permute_6rounds(snp)                                    snp.Permute_Nrounds(6)
#define SnP_Permute_12rounds(snp)                                   snp.Permute_Nrounds(12)
#define SnP_ExtractBytes(snp, data, offset, length)                 snp.ExtractBytes(data, offset, length)
#define SnP_ExtractAndAddBytes(snp, input, output, offset, length)  snp.ExtractAndAddBytes(input, output, offset, length)
#define SnP_NoFastLoopAbsorb
#define DUMP(state, message)                                        if (UT_VERBOSE) dumpState(state, message)
#include "testSnPnominal.inc"
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_Permute
#undef SnP_Permute_6rounds
#undef SnP_Permute_12rounds
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes
#undef SnP_NoFastLoopAbsorb
#undef DUMP

        displayAccumulated(synopsis, stateAccumulated);
    }
    catch(Exception e) {
        cout << e.reason << endl;
    }
}

int test()
{
    nominalTest_Xoodoo("Xoodoo");
    nominalTest_Xoodoo_noNrounds("Xoodoo (without Nrounds)");
    nominalTest_KeccakP_narrow( 200, "Keccak-p[200]");
    nominalTest_KeccakP_narrow( 400, "Keccak-p[400]");
    nominalTest_KeccakP_wide( 800, "Keccak-p[800]");
    nominalTest_KeccakP_wide(1600, "Keccak-p[1600]");
    return 0;
}

int main()
{
    return test();
}
