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

#define accumulateBuffer                JOIN(prefix, _accumulateBuffer)
#define accumulateState                 JOIN(prefix, _accumulateState)
#define dumpState                       JOIN(prefix, _dumpState)
#define testSnP                         JOIN(prefix, _testSnP)
#define checkBufferPadding              JOIN(prefix, _checkBufferPadding)
#define checkStatePadding               JOIN(prefix, _checkStatePadding)
#define elementarySnPtest               JOIN(prefix, _elementarySnPtest)
#define nominalSnPtest                  JOIN(prefix, _nominalSnPtest)

#define SnP_state                       JOIN(SnP, _state)
#define SnP_stateSizeInBytes            sizeof(SnP_state)
#define SnP_laneLengthInBytes           ((SnP_width)/(8*SnP_laneCount))
#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                  JOIN(SnP, _Initialize)
#define SnP_AddByte                     JOIN(SnP, _AddByte)
#define SnP_AddBytes                    JOIN(SnP, _AddBytes)
#define SnP_OverwriteBytes              JOIN(SnP, _OverwriteBytes)
#define SnP_OverwriteWithZeroes         JOIN(SnP, _OverwriteWithZeroes)
#define SnP_ExtractBytes                JOIN(SnP, _ExtractBytes)
#define SnP_ExtractAndAddBytes          JOIN(SnP, _ExtractAndAddBytes)

#ifdef UT_VERBOSE
    #define DUMP( argS, argT )  dumpState( argS, argT )

void dumpState(const unsigned char *state, char *message)
{
    unsigned int i;
    unsigned char buffer[SnP_width/8];

    SnP_ExtractBytes(state, buffer, 0, SnP_width/8 );
    printf( "\n%s\n", message );
    state = buffer;
    for ( i = 0; i < SnP_laneCount; ++i, state += SnP_laneLengthInBytes )
    {
        #if defined(DUMP_BYTES)

        unsigned int j;

        for ( j = 0; j < SnP_laneLengthInBytes; ++j )
        {
            printf("0x%02X, ", state[j] );
        }
        printf("\n" );

        #else

        #if (SnP_laneLengthInBytes == 8)
        printf("%08X%08X ", ((uint32_t*)state)[1], ((uint32_t*)state)[0] );
        #elif (SnP_laneLengthInBytes == 4)
        printf("%08X ", ((uint32_t*)state)[0] );
        #elif (SnP_laneLengthInBytes == 2)
        printf("%04X ", ((uint16_t*)state)[0] );
        #elif (SnP_laneLengthInBytes == 1)
        printf("%02X ", ((uint8_t*)state)[0] );
        #endif

        #if (SnP_laneCount == 25)
        if ( (i % 5) == 4 )
            printf("\n" );
        #else
        if ( (i % 4) == 3 )
            printf("\n" );
        #endif
        #endif
    }
}
#else
    #define DUMP( argS, argT )
#endif

#ifndef SnP_FastLoop_Absorb
#define SnP_FastLoop_Absorb JOIN(SnP, _FastLoop_Absorb)
size_t SnP_FastLoop_Absorb(SnP_state *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
{
    size_t processed = 0;

    while(dataByteLen >= laneCount*SnP_laneLengthInBytes) {
        SnP_AddBytes(state, data, 0, laneCount*SnP_laneLengthInBytes);
        SnP_Permute(state);
        data += laneCount*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*SnP_laneLengthInBytes;
        processed += laneCount*SnP_laneLengthInBytes;
    }
    return processed;
}
#endif

void accumulateBuffer(SnP_state *stateAccumulated, const unsigned char *buffer)
{
    SnP_AddBytes(stateAccumulated, buffer, 0, SnP_width/8);
    SnP_Permute(stateAccumulated);
}

void accumulateState(SnP_state *stateAccumulated, const SnP_state *stateTest)
{
    unsigned char buffer[SnP_width/8];
    SnP_ExtractBytes(stateTest, buffer, 0, SnP_width/8);
    accumulateBuffer(stateAccumulated, buffer);
}

void nominalSnPtest(const char *fileName, const char *synopsis, const unsigned char *expected);
void elementarySnPtest(void);

void testSnP(const char *fileName, const char *synopsis, const unsigned char *expected)
{
    UT_startTest(synopsis, JOIN(SnP, _implementation));
    elementarySnPtest();
    nominalSnPtest(fileName, synopsis, expected);
    UT_endTest();
}

#if (SnP_stateAlignment > 32)
#define padding SnP_stateAlignment
#else
#define padding 32
#endif
void checkStatePadding(const unsigned char *paddedState)
{
    unsigned int i;
    for(i=0; i<padding; i++) {
        assert(paddedState[i] == 0x5A);
        assert(paddedState[padding+SnP_stateSizeInBytes+i] == 0x5A);
    }
}

void checkBufferPadding(const unsigned char *paddedBuffer)
{
    unsigned int i;
    for(i=0; i<padding; i++) {
        assert(paddedBuffer[i] == 0x96);
        assert(paddedBuffer[padding+SnP_width/8+i] == 0x96);
    }
}

void elementarySnPtest()
{
    ALIGN(SnP_stateAlignment) unsigned char prePaddedState[padding + SnP_stateSizeInBytes + SnP_stateAlignment + padding];
    ALIGN(SnP_stateAlignment) unsigned char prePaddedBuffer[padding + SnP_width/8 + SnP_stateAlignment + padding];
    unsigned int a;
    for(a = 0; a <= SnP_stateAlignment; a += SnP_stateAlignment) {
        unsigned char *paddedState = prePaddedState + a;
        unsigned char *paddedBuffer = prePaddedBuffer + a;
        SnP_state *state = (SnP_state*)(paddedState + padding);
        unsigned char *buffer = paddedBuffer + padding;
        unsigned int i, j;

        memset(prePaddedState, 0x5A, sizeof(prePaddedState));
        memset(prePaddedBuffer, 0x96, sizeof(prePaddedBuffer));

        /* Testing SnP_Initialize() */
        {
            SnP_Initialize(state);
            SnP_ExtractBytes(state, buffer, 0, SnP_width/8);
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
            for(i=0; i<SnP_width/8; i++)
                assert(buffer[i] == 0);
        }

        /* Testing SnP_AddByte() */
        {
            for(i=0; i<SnP_width/8; i++)
                SnP_AddByte(state, (unsigned char)i, (unsigned char)i);
            SnP_ExtractBytes(state, buffer, 0, SnP_width/8);
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
            for(i=0; i<SnP_width/8; i++)
                assert(buffer[i] == i);
        }

        /* Testing SnP_ExtractBytes() */
        {
            for(i=0; i<7; i++) {
                memset(buffer, 0x3C, SnP_width/8);
                SnP_ExtractBytes(state, buffer, i, SnP_width/8-4*i);
                for(j=0; j<SnP_width/8-4*i; j++)
                    assert(buffer[j] == (i+j));
                for(j=i+SnP_width/8-4*i; j<SnP_width/8; j++)
                    assert(buffer[j] == 0x3C);
            }
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
        }

        /* Testing SnP_ExtractAndAddBytes() (input = output) */
        {
            for(i=0; i<7; i++) {
                memset(buffer, 0xC3, SnP_width/8);
                SnP_ExtractAndAddBytes(state, buffer, buffer, i, SnP_width/8-4*i);
                for(j=0; j<SnP_width/8-4*i; j++)
                    assert(buffer[j] == (0xC3 ^ (i+j)));
                for(j=i+SnP_width/8-4*i; j<SnP_width/8; j++)
                    assert(buffer[j] == 0xC3);
            }
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
        }

        /* Testing SnP_AddBytes() */
        {
            for(i=0; i<8; i++) {
                memset(buffer, 1 << i, SnP_width/8);
                SnP_AddBytes(state, buffer, i, SnP_width/8-2*i);
            }
            SnP_ExtractBytes(state, buffer, 0, SnP_width/8);
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
            for(i=0; i<7; i++)
                assert(buffer[i] == (i ^ ((1 << (i+1))-1)));
            for(i=7; i<SnP_width/8-7; i++)
                assert(buffer[i] == (i ^ 0xFF));
            for(i=SnP_width/8-7; i<SnP_width/8; i++)
                assert(buffer[i] == (i ^ ((1 << (SnP_width/8-i))-1)));
        }

        /* Testing SnP_OverwriteBytes() */
        {
            for(i=0; i<8; i++) {
                memset(buffer, i+1, SnP_width/8);
                SnP_OverwriteBytes(state, buffer, i, SnP_width/8-3*i);
            }
            SnP_ExtractBytes(state, buffer, 0, SnP_width/8);
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
            for(i=0; i<8; i++)
                assert(buffer[i] == i+1);
            for(i=8; i<SnP_width/8-16; i++)
                assert(buffer[i] == 8);
            for(i=SnP_width/8-16; i<SnP_width/8; i++)
                assert(buffer[i] == (SnP_width/8-i-1)/2+1);
        }

        /* Testing SnP_OverwriteWithZeroes() */
        {
            for(i=0; i<SnP_width/8; i+=7) {
                SnP_OverwriteWithZeroes(state, i);
                SnP_ExtractBytes(state, buffer, i, 1);
                assert(buffer[0] != 0);
                if (i > 0) {
                    SnP_ExtractBytes(state, buffer, i-1, 1);
                    assert(buffer[0] == 0);
                }
            }
            SnP_OverwriteWithZeroes(state, SnP_width/8);
            SnP_ExtractBytes(state, buffer, 0, SnP_width/8);
            checkStatePadding(paddedState);
            checkBufferPadding(paddedBuffer);
            for(i=0; i<SnP_width/8; i++)
                assert(buffer[i] == 0);
        }
    }
}
#undef padding

void nominalSnPtest(const char *fileName, const char *synopsis, const unsigned char *expected)
{
    ALIGN(SnP_stateAlignment) unsigned char preStateAccumulated[SnP_stateSizeInBytes + SnP_stateAlignment];
    ALIGN(SnP_stateAlignment) unsigned char preStateTest[SnP_stateSizeInBytes + SnP_stateAlignment];

    unsigned int a;
    for(a = 0; a <= SnP_stateAlignment; a += SnP_stateAlignment) {

        SnP_state *stateAccumulated = (SnP_state*)(preStateAccumulated + a);
        SnP_state *stateTest = (SnP_state*)(preStateTest + a);
        memset(preStateTest, 0xAA, sizeof(preStateTest));

#ifdef UT_VERBOSE
        printf( "stateAccumulated @%p ", stateAccumulated );
        printf( "stateTest @%p\n", stateTest );
#endif

#include "testSnPnominal.inc"

#ifdef UT_OUTPUT
        /* Outputting the result */
        {
            unsigned char buffer[SnP_width/8];
            unsigned int i;
            FILE *f;

            SnP_ExtractBytes(stateAccumulated, buffer, 0, SnP_width/8);
            f = fopen(fileName, "w");
            assert(f != NULL);
            fprintf(f, "%s", synopsis);
            fprintf(f, "\"");
            for(i=0; i<SnP_width/8; i++)
                fprintf(f, "\\x%02x", buffer[i]);
            fprintf(f, "\"\n");
            fclose(f);
        }
#endif

        {
            unsigned char buffer[SnP_width/8];
            SnP_ExtractBytes(stateAccumulated, buffer, 0, SnP_width/8);
            assert(memcmp(expected, buffer, sizeof(buffer)) == 0);
        }
    }
}

#undef accumulateBuffer
#undef accumulateState
#undef dumpState
#undef testSnP
#undef checkBufferPadding
#undef checkStatePadding
#undef elementarySnPtest
#undef nominalSnPtest

#undef SnP_state
#undef SnP_stateSizeInBytes
#undef SnP_laneLengthInBytes
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes
#undef SnP_FastLoop_Absorb
