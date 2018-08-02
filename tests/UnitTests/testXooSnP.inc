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

#define accumulateBuffer                JOIN(prefix, _accumulateBuffer)
#define accumulateState                 JOIN(prefix, _accumulateState)
#define dumpState                       JOIN(prefix, _dumpState)
#define testSnP                         JOIN(prefix, _testSnP)
#define checkBufferPadding              JOIN(prefix, _checkBufferPadding)
#define checkStatePadding               JOIN(prefix, _checkStatePadding)
#define elementarySnPtest               JOIN(prefix, _elementarySnPtest)
#define nominalSnPtest                  JOIN(prefix, _nominalSnPtest)

#define SnP_stateAlignment              JOIN(SnP, _stateAlignment)
#define SnP_stateSizeInBytes            JOIN(SnP, _stateSizeInBytes)
#define SnP_laneLengthInBytes           ((SnP_width)/(3*4*8))
#define SnP_laneCount                   (3*4)
#define SnP_StaticInitialize            JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                  JOIN(SnP, _Initialize)
#define SnP_AddByte                     JOIN(SnP, _AddByte)
#define SnP_AddBytes                    JOIN(SnP, _AddBytes)
#define SnP_OverwriteBytes              JOIN(SnP, _OverwriteBytes)
#define SnP_OverwriteWithZeroes         JOIN(SnP, _OverwriteWithZeroes)
#define SnP_ExtractBytes                JOIN(SnP, _ExtractBytes)
#define SnP_ExtractAndAddBytes          JOIN(SnP, _ExtractAndAddBytes)

#ifdef VERBOSE
    #define DUMP( argS, argT )  dumpState( argS, argT )

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
void dumpState(const unsigned char *state, char *message)
{
    unsigned int i;
    unsigned char buffer[SnP_width/8];

    SnP_ExtractBytes(state, buffer, 0, SnP_width/8 );
    printf( "\n%s @%p\n", message, state );
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

        #if (SnP_width == 384)
        printf("%08X ", ((UINT32*)state)[0] );
        #endif

        if ( (i % 4) == 3 )
            printf("\n" );
        #endif
    }
}
#else
    #define DUMP( argS, argT )
#endif

#ifndef SnP_FastLoop_Absorb
#define SnP_FastLoop_Absorb JOIN(SnP, _FastLoop_Absorb)
size_t SnP_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
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

void accumulateBuffer(void *stateAccumulated, const unsigned char *buffer)
{
    SnP_AddBytes(stateAccumulated, buffer, 0, SnP_width/8);
    SnP_Permute(stateAccumulated);
}

void accumulateState(void *stateAccumulated, const void *stateTest)
{
    unsigned char buffer[SnP_width/8];
    SnP_ExtractBytes(stateTest, buffer, 0, SnP_width/8);
    accumulateBuffer(stateAccumulated, buffer);
}

void nominalSnPtest(const char *fileName, const char *synopsis, const unsigned char *expected);
void elementarySnPtest(void);

void testSnP(const char *fileName, const char *synopsis, const unsigned char *expected)
{
#if !defined(EMBEDDED)
    printf("Testing %s: %s\n", synopsis, JOIN(SnP, _implementation));
#endif

    elementarySnPtest();

    nominalSnPtest(fileName, synopsis, expected);
}

#define padding 32
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
    ALIGN(SnP_stateAlignment) unsigned char paddedState[padding+SnP_stateSizeInBytes+padding];
    ALIGN(SnP_stateAlignment) unsigned char paddedBuffer[padding+SnP_width/8+padding];
    unsigned char *state = paddedState+padding;
    unsigned char *buffer = paddedBuffer+padding;
    unsigned int i, j;

    memset(paddedState, 0x5A, sizeof(paddedState));
    memset(paddedBuffer, 0x96, sizeof(paddedBuffer));

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
#undef padding

void nominalSnPtest(const char *fileName, const char *synopsis, const unsigned char *expected)
{
    ALIGN(SnP_stateAlignment) unsigned char stateAccumulated[SnP_stateSizeInBytes];
    ALIGN(SnP_stateAlignment) unsigned char stateTest[SnP_stateSizeInBytes];

    SnP_StaticInitialize();

    SnP_Initialize(stateAccumulated);

    memset(stateTest, 0xAA, sizeof(stateTest));

    /* Testing SnP_Initialize() */
    {
        SnP_Initialize(stateTest);
        accumulateState(stateAccumulated, stateTest);
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_Initialize()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_Initialize()");

    /* Testing SnP_AddByte() */
    {
        unsigned offset;

        for(offset=0; offset<(SnP_width/8); offset += 13) {
            unsigned char byte = 0xAB - (unsigned char)(offset*4);
            SnP_AddByte(stateTest, byte, offset);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_AddByte()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_AddByte()");

    /* Testing SnP_AddBytes() */
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned i, offset, length, alignment;

        for(i=0; i<sizeof(buffer); i++)
            buffer[i] = 0xF3 + (unsigned char)(5*i);

        for(offset=0; offset<(SnP_width/8); offset += (offset < 10) ? 1 : 7)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 10) ? 1 : (5+offset)) {
            alignment = (offset+length+1)%8;
            SnP_AddBytes(stateTest, buffer+alignment, offset, length);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_AddBytes()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_AddBytes()");

    /* Testing SnP_OverwriteBytes() */
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned i, offset, length, alignment;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 11) ? 1 : 5)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 11) ? 1 : (9+4*offset)) {
            alignment = (offset+length+3)%8;
            for(i=0; i<sizeof(buffer); i++)
                buffer[i] = (unsigned char)(0xF3 + 5*i + alignment + offset + length);
            SnP_OverwriteBytes(stateTest, buffer+alignment, offset, length);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_OverwriteBytes()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_OverwriteBytes()");

    /* Testing SnP_OverwriteWithZeroes() */
    {
        unsigned byteCount;

        for(byteCount=0; byteCount<=SnP_width/8; byteCount++) {
            SnP_Permute(stateTest);
            SnP_OverwriteWithZeroes(stateTest, byteCount);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_OverwriteWithZeroes()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_OverwriteWithZeroes()");

    /* Testing SnP_ExtractBytes() */
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned offset, length, alignment;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 12) ? 1 : 7)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 12) ? 1 : (6+3*offset)) {
            alignment = (offset+length+5)%8;
            memset(buffer, 0x3C+offset+length, sizeof(buffer));
            SnP_ExtractBytes(stateTest, buffer+alignment, offset, length);
            accumulateBuffer(stateAccumulated, buffer+alignment);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_ExtractBytes()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_ExtractBytes()");

    /* Testing SnP_ExtractAndAddBytes() (input = output) */
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned offset, length, alignment;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 13) ? 1 : 9)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 13) ? 1 : (5+2*offset)) {
            alignment = (offset+length+7)%8;
            memset(buffer, 0x3C+offset+length, sizeof(buffer));
            SnP_ExtractAndAddBytes(stateTest, buffer+alignment, buffer+alignment, offset, length);
            accumulateBuffer(stateAccumulated, buffer+alignment);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_ExtractAndAddBytes() (input = output)");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_ExtractAndAddBytes() (input = output)");

    /* Testing SnP_ExtractAndAddBytes() (input ≠ output) */
    {
        unsigned char buffer1[SnP_width/8+8];
        unsigned char buffer2[SnP_width/8+8];
        unsigned offset, length, alignment1, alignment2;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 9) ? 1 : 11)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 11) ? 1 : (7+2*offset)) {
            alignment1 = (offset+length)%8;
            alignment2 = (offset+2*length)%8;
            memset(buffer1, 0x4B+offset+length, sizeof(buffer1));
            memset(buffer2, 0x5A+offset+length, sizeof(buffer2));
            SnP_ExtractAndAddBytes(stateTest, buffer1+alignment1, buffer2+alignment2, offset, length);
            accumulateBuffer(stateAccumulated, buffer2+alignment2);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_ExtractAndAddBytes() (input != output)");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_ExtractAndAddBytes() (input != output)");

#ifdef SnP_Permute_12rounds
    /* Testing SnP_Permute_12rounds() */
    {
        unsigned int i;

        for(i=0; i<3; i++) {
            SnP_Permute_12rounds(stateTest);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_Permute_12rounds()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_Permute_12rounds()");
#endif

#if defined(Xoodoo_HasNround)
    /* Testing SnP_Permute_Nrounds() */
    {
        unsigned int nr;

        for(nr=1; nr<= SnP_Permute_maxRounds; nr++) {
            SnP_Permute_Nrounds(stateTest, nr);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);
    DUMP(stateTest, "stateTest after testing SnP_Permute_Nrounds()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_Permute_Nrounds()");
#endif

#if 0
    /* Testing SnP_FastLoop_Absorb() */
    {
        unsigned char buffer[SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(SnP_FastLoop_Absorb(stateTest, SnP_laneCount, buffer, SnP_laneCount-1) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            alignment = (laneCount+1)%8;
            for(i=0; i<laneCount*SnP_laneLengthInBytes; i++)
                buffer[i+alignment] = 0x11+2*laneCount+4*i;
            assert(SnP_FastLoop_Absorb(stateTest, laneCount, buffer+alignment, laneCount*SnP_laneLengthInBytes)
                == laneCount*SnP_laneLengthInBytes);
        }
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<SnP_laneCount; laneCount+=4) {
            alignment = (laneCount+5+extra)%8;
            assert(blocks*laneCount*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xA0 - 17*i + 2*alignment - 4*laneCount;
            assert(SnP_FastLoop_Absorb(stateTest, laneCount, buffer+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*SnP_laneLengthInBytes);
            blocks--;
            extra++;
        }
        accumulateState(stateAccumulated, stateTest);
    }
    DUMP(stateTest, "stateTest after testing SnP_FastLoop_Absorb()");
    DUMP(stateAccumulated, "stateAccumulated after testing SnP_FastLoop_Absorb()");
#endif

#ifdef OUTPUT
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

#undef accumulateBuffer
#undef accumulateState
#undef dumpState
#undef testSnP
#undef checkBufferPadding
#undef checkStatePadding
#undef elementarySnPtest
#undef nominalSnPtest

#undef SnP_stateAlignment
#undef SnP_stateSizeInBytes
#undef SnP_laneLengthInBytes
#undef SnP_laneCount
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes
#undef SnP_FastLoop_Absorb
