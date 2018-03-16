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

#define accumulateBufferParallel        JOIN(prefix, _accumulateBufferParallel)
#define accumulateStateParallel         JOIN(prefix, _accumulateStateParallel)
#define dumpBuffer                      JOIN(prefix, _dumpBuffer)
#define dumpState                       JOIN(prefix, _dumpState)
#define testPlSnP                       JOIN(prefix, _testPlSnP)

#define PlSnP_statesSizeInBytes         JOIN(PlSnP, _statesSizeInBytes)
#define PlSnP_statesAlignment           JOIN(PlSnP, _statesAlignment)
#define PlSnP_P                         PlSnP_parallelism

#define SnP_laneLengthInBytes           ((SnP_width)/200)
#define SnP_laneCount                   25
#define PlSnP_StaticInitialize          JOIN(PlSnP, _StaticInitialize)
#define PlSnP_InitializeAll             JOIN(PlSnP, _InitializeAll)
#define PlSnP_AddByte                   JOIN(PlSnP, _AddByte)
#define PlSnP_AddBytes                  JOIN(PlSnP, _AddBytes)
#define PlSnP_AddLanesAll               JOIN(PlSnP, _AddLanesAll)
#define PlSnP_OverwriteBytes            JOIN(PlSnP, _OverwriteBytes)
#define PlSnP_OverwriteLanesAll         JOIN(PlSnP, _OverwriteLanesAll)
#define PlSnP_OverwriteWithZeroes       JOIN(PlSnP, _OverwriteWithZeroes)
#define PlSnP_ExtractBytes              JOIN(PlSnP, _ExtractBytes)
#define PlSnP_ExtractLanesAll           JOIN(PlSnP, _ExtractLanesAll)
#define PlSnP_ExtractAndAddBytes        JOIN(PlSnP, _ExtractAndAddBytes)
#define PlSnP_ExtractAndAddLanesAll     JOIN(PlSnP, _ExtractAndAddLanesAll)

#ifdef VERBOSE
    #define DUMP( argS, argT )  dumpState( argS, argT )

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

void dumpBuffer( const unsigned char * buffer );
void dumpBuffer( const unsigned char * buffer )
{
    unsigned int i;

    for ( i = 0; i < 25; ++i, buffer += SnP_laneLengthInBytes )
    {
        #if defined(DUMP_BYTES)

        unsigned int j;

        for ( j = 0; j < SnP_laneLengthInBytes; ++j )
        {
            printf("0x%02X, ", buffer[j] );
        }
        printf("\n" );

        #else

        #if (SnP_width == 1600)
        printf("%08X%08X ", ((UINT32*)buffer)[1], ((UINT32*)buffer)[0] );
        #elif (SnP_width == 800)
        printf("%08X ", ((UINT32*)buffer)[0] );
        #elif (SnP_width == 400)
        printf("%04X ", ((UINT16*)buffer)[0] );
        #elif (SnP_width == 200)
        printf("%02X ", ((UINT8*)buffer)[0] );
        #endif
        if ( (i % 5) == 4 )
            printf("\n" );

        #endif
    }
}

void dumpState( const unsigned char * state, char * message )
{
    unsigned int n;
    unsigned char buffer[(SnP_width/8) * PlSnP_parallelism];

    PlSnP_ExtractLanesAll(state, buffer, SnP_laneCount, SnP_laneCount);
    for ( n = 0; n < PlSnP_parallelism; ++n )
    {
        printf( "\n%s %u\n", message, n );
        dumpBuffer( buffer + n * SnP_width/8 );
    }
}
#else
    #define DUMP( argS, argT )
#endif

#ifndef PlSnP_FastLoop_Absorb
#define PlSnP_FastLoop_Absorb JOIN(PlSnP, _FastLoop_Absorb)
size_t PlSnP_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen)
{
    const unsigned char *dataStart = data;

    while(dataByteLen >= (laneOffsetParallel*PlSnP_P - laneOffsetParallel + laneCount)*SnP_laneLengthInBytes) {
        PlSnP_AddLanesAll(states, data, laneCount, laneOffsetParallel);
        PlSnP_PermuteAll(states);
        data += laneOffsetSerial*SnP_laneLengthInBytes;
        dataByteLen -= laneOffsetSerial*SnP_laneLengthInBytes;
    }
    return data - dataStart;
}
#endif

void accumulateBufferParallel(void *stateAccumulated, const unsigned char *buffer)
{
    PlSnP_AddLanesAll(stateAccumulated, buffer, SnP_laneCount, SnP_laneCount);
    PlSnP_PermuteAll(stateAccumulated);
}

void accumulateStateParallel(void *stateAccumulated, const void *stateTest)
{
    unsigned char buffer[PlSnP_P*(SnP_width/8)];
    PlSnP_ExtractLanesAll(stateTest, buffer, SnP_laneCount, SnP_laneCount);
    accumulateBufferParallel(stateAccumulated, buffer);
}

void testPlSnP(const char *fileName, const char *synopsis, const unsigned char *expected)
{
    ALIGN(PlSnP_statesAlignment) unsigned char stateAccumulated[PlSnP_statesSizeInBytes];
    ALIGN(PlSnP_statesAlignment) unsigned char stateTest[PlSnP_statesSizeInBytes];

#if !defined(EMBEDDED)
    printf("Testing %s: %s\n", synopsis, JOIN(PlSnP, _implementation));
#endif

    PlSnP_StaticInitialize();

    PlSnP_InitializeAll(stateAccumulated);

    memset(stateTest, 0xAA, sizeof(stateTest));

    /* Testing PlSnP_InitializeAll() */
    {
        PlSnP_InitializeAll(stateTest);
        accumulateStateParallel(stateAccumulated, stateTest);
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_InitializeAll()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_InitializeAll()" );

    /* Testing PlSnP_AddByte() */
    {
        unsigned offset, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(offset=0; offset<(SnP_width/8); offset += (offset < 6) ? 1 : (9+2*instanceIndex)) {
                unsigned char byte = 0xCD - offset*4 + instanceIndex*16;
                PlSnP_AddByte(stateTest, instanceIndex, byte, offset);
                accumulateStateParallel(stateAccumulated, stateTest);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_AddByte()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_AddByte()" );

    /* Testing PlSnP_AddBytes() */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(i=0; i<sizeof(buffer); i++)
                buffer[i] = 0xF3 + 5*i + 3*instanceIndex;

            for(offset=0; offset<(SnP_width/8); offset += (offset < 10) ? 1 : (7+4*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 20) ? 1 : (5+2*instanceIndex)) {
                alignment = (offset+length+instanceIndex+1)%8;
                PlSnP_AddBytes(stateTest, instanceIndex, buffer+alignment, offset, length);
                accumulateStateParallel(stateAccumulated, stateTest);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_AddBytes()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_AddBytes()" );

    /* Testing PlSnP_AddLanesAll() */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned i, laneCount, laneOffset, alignment;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+1)%4;
            alignment = (laneCount+2)%8;
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0x75 + 3*i - 8*alignment;
            PlSnP_AddLanesAll(stateTest, buffer+alignment, laneCount, laneOffset);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_AddLanesAll()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_AddLanesAll()" );

    /* Testing PlSnP_OverwriteBytes() */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(offset=0; offset<(SnP_width/8); offset += (offset < 11) ? 1 : (5+4*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 21) ? 1 : (9+2*instanceIndex)) {
                alignment = (offset+length+instanceIndex+3)%8;
                for(i=0; i<sizeof(buffer); i++)
                    buffer[i] = 0xF3 + 5*i + alignment + offset + length + instanceIndex;
                PlSnP_OverwriteBytes(stateTest, instanceIndex, buffer+alignment, offset, length);
                accumulateStateParallel(stateAccumulated, stateTest);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_OverwriteBytes()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_OverwriteBytes()" );

    /* Testing PlSnP_OverwriteLanesAll() */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned i, laneCount, laneOffset, alignment;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+2)%4;
            alignment = (laneCount+4)%8;
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0x75 + 3*i - 8*alignment - laneCount;
            PlSnP_OverwriteLanesAll(stateTest, buffer+alignment, laneCount, laneOffset);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_OverwriteLanesAll()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_OverwriteLanesAll()" );

    /* Testing PlSnP_OverwriteWithZeroes() */
    {
        unsigned byteCount, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++)
        for(byteCount=0; byteCount<=SnP_width/8; byteCount++) {
            PlSnP_PermuteAll(stateTest);
            PlSnP_OverwriteWithZeroes(stateTest, instanceIndex, byteCount);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_OverwriteWithZeroes()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_OverwriteWithZeroes()" );

#ifdef PlSnP_PermuteAll_12rounds
    /* Testing PlSnP_PermuteAll_12rounds() */
    {
        unsigned int i;

        for(i=0; i<3; i++) {
            PlSnP_PermuteAll_12rounds(stateTest);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_PermuteAll_12rounds()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_PermuteAll_12rounds()" );
#endif

#ifdef PlSnP_PermuteAll_6rounds
    /* Testing PlSnP_PermuteAll_6rounds() */
    {
        unsigned int i;

        for(i=0; i<3; i++) {
            PlSnP_PermuteAll_6rounds(stateTest);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_PermuteAll_6rounds()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_PermuteAll_6rounds()" );
#endif

#ifdef PlSnP_PermuteAll_4rounds
    /* Testing PlSnP_PermuteAll_4rounds() */
    {
        unsigned int i;

        for(i=0; i<3; i++) {
            PlSnP_PermuteAll_4rounds(stateTest);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_PermuteAll_4rounds()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_PermuteAll_4rounds()" );
#endif

    /* Testing PlSnP_ExtractBytes() */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(offset=0; offset<(SnP_width/8); offset += (offset < 12) ? 1 : (7+2*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 22) ? 1 : (5+4*instanceIndex)) {
                alignment = (offset+length+instanceIndex+5)%8;
                memset(buffer, 0x3C+offset+length+instanceIndex, sizeof(buffer));
                PlSnP_ExtractBytes(stateTest, instanceIndex, buffer+alignment, offset, length);
                accumulateBufferParallel(stateAccumulated, buffer+alignment);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_ExtractBytes()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_ExtractBytes()" );

    /* Testing PlSnP_ExtractLanesAll() */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned laneCount, laneOffset, alignment;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+3)%4;
            alignment = (laneCount+6)%8;
            memset(buffer, 0xD2-laneCount-32*alignment, sizeof(buffer));
            PlSnP_ExtractLanesAll(stateTest, buffer+alignment, laneCount, laneOffset);
            accumulateBufferParallel(stateAccumulated, buffer+alignment);
            accumulateBufferParallel(stateAccumulated, buffer+alignment+PlSnP_P*4*SnP_laneLengthInBytes);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_ExtractLanesAll()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_ExtractLanesAll()" );

    /* Testing PlSnP_ExtractAndAddBytes() (input = output) */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(offset=0; offset<(SnP_width/8); offset += (offset < 13) ? 1 : (5+2*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 23) ? 1 : (9+4*instanceIndex)) {
                alignment = (offset+length+instanceIndex+7)%8;
                memset(buffer, 0x3C+offset+length+instanceIndex, sizeof(buffer));
                PlSnP_ExtractAndAddBytes(stateTest, instanceIndex, buffer+alignment, buffer+alignment, offset, length);
                accumulateBufferParallel(stateAccumulated, buffer+alignment);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_ExtractAndAddBytes() (input = output)" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_ExtractAndAddBytes() (input = output)" );

    /* Testing PlSnP_ExtractAndAddBytes() (input ≠ output) */
    {
        unsigned char buffer1[PlSnP_P*(SnP_width/8)+8];
        unsigned char buffer2[PlSnP_P*(SnP_width/8)+8];
        unsigned offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(offset=0; offset<(SnP_width/8); offset += (offset < 9) ? 1 : (6+3*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 13) ? 1 : (11+2*instanceIndex)) {
                alignment = (offset+length+instanceIndex)%8;
                memset(buffer1, 0x3C+offset+length+instanceIndex, sizeof(buffer1));
                memset(buffer2, 0x4D+offset+length+instanceIndex, sizeof(buffer2));
                PlSnP_ExtractAndAddBytes(stateTest, instanceIndex, buffer1+alignment, buffer2+alignment, offset, length);
                accumulateBufferParallel(stateAccumulated, buffer2+alignment);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_ExtractAndAddBytes() (input != output)" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_ExtractAndAddBytes() (input != output)" );

    /* Testing PlSnP_ExtractAndAddLanesAll() (input = output) */
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned laneCount, laneOffset, alignment;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+0)%4;
            alignment = (laneCount+1)%8;
            memset(buffer, 0xD2-laneCount-32*alignment, sizeof(buffer));
            PlSnP_ExtractAndAddLanesAll(stateTest, buffer+alignment, buffer+alignment, laneCount, laneOffset);
            accumulateBufferParallel(stateAccumulated, buffer+alignment);
            accumulateBufferParallel(stateAccumulated, buffer+alignment+PlSnP_P*4*SnP_laneLengthInBytes);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_ExtractAndAddLanesAll() (input = output)" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_ExtractAndAddLanesAll() (input = output)" );

    /* Testing PlSnP_ExtractAndAddLanesAll() (input ≠ output) */
    {
        unsigned char buffer1[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned char buffer2[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned laneCount, laneOffset, alignment1, alignment2;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+2)%4;
            alignment1 = (laneCount+3)%8;
            alignment2 = (2*laneCount+5)%8;
            memset(buffer1, 0xE3-laneCount-16*alignment1, sizeof(buffer1));
            memset(buffer2, 0xF4-laneCount-8*alignment1, sizeof(buffer2));
            PlSnP_ExtractAndAddLanesAll(stateTest, buffer1+alignment1, buffer2+alignment2, laneCount, laneOffset);
            accumulateBufferParallel(stateAccumulated, buffer2+alignment2);
            accumulateBufferParallel(stateAccumulated, buffer2+alignment2+PlSnP_P*4*SnP_laneLengthInBytes);
        }
    }
    PlSnP_PermuteAll(stateTest);
    DUMP( stateTest, "stateTest after testing PlSnP_ExtractAndAddLanesAll() (input != output)" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_ExtractAndAddLanesAll() (input != output)" );

    /* Testing PlSnP_FastLoop_Absorb() */
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FastLoop_Absorb(stateTest, SnP_laneCount, SnP_laneCount, PlSnP_P*SnP_laneCount, buffer, (SnP_laneCount-1)*PlSnP_P) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            blocks = 100/laneCount;
            if (blocks > 20) blocks = 20;
            extra = (laneCount-1)*SnP_laneLengthInBytes + blocks%SnP_laneLengthInBytes;
            alignment = (laneCount+5+extra)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xA0 - 17*i + 2*alignment - 4*laneCount;
            assert(PlSnP_FastLoop_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            assert(PlSnP_FastLoop_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*SnP_laneLengthInBytes);
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }
    DUMP( stateTest, "stateTest after testing PlSnP_FastLoop_Absorb()" );
    DUMP( stateAccumulated, "stateAccumulated after testing PlSnP_FastLoop_Absorb()" );

    {
        unsigned char buffer[SnP_width/8];
        unsigned int i;

        memset(buffer, 0, sizeof(buffer));
        for(i=0; i<PlSnP_P; i++) {
            PlSnP_ExtractAndAddBytes(stateAccumulated, i, buffer, buffer, 0, SnP_width/8);
            PlSnP_PermuteAll(stateAccumulated);
        }
#ifdef OUTPUT
        /* Outputting the result */
        {
            FILE *f;

            f = fopen(fileName, "w");
            assert(f != NULL);
            fprintf(f, "%s", synopsis);
            fprintf(f, "\"");
            for(i=0; i<sizeof(buffer); i++)
                fprintf(f, "\\x%02x", buffer[i]);
            fprintf(f, "\"\n");
            fclose(f);
        }
#endif
        assert(memcmp(expected, buffer, sizeof(buffer)) == 0);
    }
}

#undef accumulateBufferParallel
#undef accumulateStateParallel
#undef dumpBuffer
#undef dumpState
#undef testPlSnP
#undef PlSnP_statesSizeInBytes
#undef PlSnP_statesAlignment
#undef PlSnP_P
#undef SnP_laneLengthInBytes
#undef SnP_laneCount
#undef PlSnP_StaticInitialize
#undef PlSnP_InitializeAll
#undef PlSnP_AddByte
#undef PlSnP_AddBytes
#undef PlSnP_AddLanesAll
#undef PlSnP_OverwriteBytes
#undef PlSnP_OverwriteLanesAll
#undef PlSnP_OverwriteWithZeroes
#undef PlSnP_ExtractBytes
#undef PlSnP_ExtractLanesAll
#undef PlSnP_ExtractAndAddBytes
#undef PlSnP_ExtractAndAddLanesAll
#undef PlSnP_FastLoop_Absorb
