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

//#define OUTPUT

#include <assert.h>
#ifdef OUTPUT
#include <stdio.h>
#endif
#include <string.h>
#include "PlSnP-interface.h"

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

void accumulateBufferParallel(void *stateAccumulated, const unsigned char *buffer)
{
    PlSnP_XORLanesAll(stateAccumulated, buffer, SnP_laneCount, SnP_laneCount);
    PlSnP_PermuteAll(stateAccumulated);
}

void accumulateStateParallel(void *stateAccumulated, const void *stateTest)
{
    unsigned char buffer[PlSnP_P*(SnP_width/8)];
    PlSnP_ExtractLanesAll(stateTest, buffer, SnP_laneCount, SnP_laneCount);
    accumulateBufferParallel(stateAccumulated, buffer);
}

void testPlSnP(void)
{
    ALIGN unsigned char stateAccumulated[PlSnP_statesSizeInBytes];
    ALIGN unsigned char stateTest[PlSnP_statesSizeInBytes];

    PlSnP_StaticInitialize();

    PlSnP_InitializeAll(stateAccumulated);

    memset(stateTest, 0xAA, sizeof(stateTest));

    // Testing PlSnP_InitializeAll()
    {
        PlSnP_InitializeAll(stateTest);
        accumulateStateParallel(stateAccumulated, stateTest);
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_XORBytes()
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(i=0; i<sizeof(buffer); i++)
                buffer[i] = 0xF3 + 5*i + 3*instanceIndex;

            for(offset=0; offset<(SnP_width/8); offset += (offset < 10) ? 1 : (7+4*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 20) ? 1 : (5+2*instanceIndex)) {
                alignment = (offset+length+instanceIndex+1)%8;
                PlSnP_XORBytes(stateTest, instanceIndex, buffer+alignment, offset, length);
                accumulateStateParallel(stateAccumulated, stateTest);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_XORLanesAll()
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned i, laneCount, laneOffset, alignment;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+1)%4;
            alignment = (laneCount+2)%8;
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0x75 + 3*i - 8*alignment;
            PlSnP_XORLanesAll(stateTest, buffer+alignment, laneCount, laneOffset);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_OverwriteBytes()
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

    // Testing PlSnP_OverwriteLanesAll()
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

    // Testing PlSnP_OverwriteWithZeroes()
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

    // Testing PlSnP_ComplementBit()
    {
        unsigned bitPosition, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++)
        for(bitPosition=0; bitPosition<SnP_width; bitPosition += (bitPosition < 128) ? (3+instanceIndex) : (47+instanceIndex)) {
            PlSnP_ComplementBit(stateTest, instanceIndex, bitPosition);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_ComplementBitAll()
    {
        unsigned bitPosition;

        for(bitPosition=0; bitPosition<SnP_width; bitPosition += (bitPosition < 128) ? 1 : 19) {
            PlSnP_ComplementBitAll(stateTest, bitPosition);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_Permute()
    {
        unsigned int instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            PlSnP_Permute(stateTest, instanceIndex);
            accumulateStateParallel(stateAccumulated, stateTest);
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_ExtractBytes()
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, offset, length, alignment, instanceIndex;

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

    // Testing PlSnP_ExtractLanesAll()
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

    // Testing PlSnP_ExtractAndXORBytes()
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, offset, length, alignment, instanceIndex;

        for(instanceIndex=0; instanceIndex<PlSnP_P; instanceIndex++) {
            for(offset=0; offset<(SnP_width/8); offset += (offset < 13) ? 1 : (5+2*instanceIndex))
            for(length=(offset == instanceIndex) ? 0 : ((SnP_width/8)-offset); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 23) ? 1 : (9+4*instanceIndex)) {
                alignment = (offset+length+instanceIndex+7)%8;
                memset(buffer, 0x3C+offset+length+instanceIndex, sizeof(buffer));
                PlSnP_ExtractAndXORBytes(stateTest, instanceIndex, buffer+alignment, offset, length);
                accumulateBufferParallel(stateAccumulated, buffer+alignment);
            }
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_ExtractAndXORLanesAll()
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8+4*SnP_laneLengthInBytes)+8];
        unsigned laneCount, laneOffset, alignment;

        for(laneCount=0; laneCount<=SnP_laneCount; laneCount++) {
            laneOffset = laneCount + (laneCount+0)%4;
            alignment = (laneCount+1)%8;
            memset(buffer, 0xD2-laneCount-32*alignment, sizeof(buffer));
            PlSnP_ExtractAndXORLanesAll(stateTest, buffer+alignment, laneCount, laneOffset);
            accumulateBufferParallel(stateAccumulated, buffer+alignment);
            accumulateBufferParallel(stateAccumulated, buffer+alignment+PlSnP_P*4*SnP_laneLengthInBytes);
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_FBWL_Absorb()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Absorb(stateTest, SnP_laneCount, SnP_laneCount, PlSnP_P*SnP_laneCount, buffer, (SnP_laneCount-1)*PlSnP_P, 0) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            blocks = 100/laneCount;
            if (blocks > 20) blocks = 20;
            extra = (laneCount-1)*SnP_laneLengthInBytes + blocks%SnP_laneLengthInBytes;
            alignment = (laneCount+5+extra)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xA0 - 17*i + 2*alignment - 4*laneCount;
            assert(PlSnP_FBWL_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            assert(PlSnP_FBWL_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

    // Testing PlSnP_FBWL_Squeeze()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Squeeze(stateTest, SnP_laneCount, SnP_laneCount, PlSnP_P*SnP_laneCount, buffer, (SnP_laneCount-1)*PlSnP_P) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            blocks = 100/laneCount;
            if (blocks > 20) blocks = 20;
            extra = (laneCount-1)*SnP_laneLengthInBytes + blocks%SnP_laneLengthInBytes;
            alignment = (laneCount+6+extra)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            memset(buffer, 0xBF+laneCount*2+16*alignment, sizeof(buffer));
            assert(PlSnP_FBWL_Squeeze(stateTest, laneCount, laneCount, PlSnP_P*laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            assert(PlSnP_FBWL_Squeeze(stateTest, laneCount, blocks*laneCount, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x02);
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

    // Testing PlSnP_FBWL_Wrap()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned char bufferOut[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Wrap(stateTest, SnP_laneCount, SnP_laneCount, PlSnP_P*SnP_laneCount, buffer, bufferOut, (SnP_laneCount-1)*PlSnP_P, 0) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            blocks = 100/laneCount;
            if (blocks > 20) blocks = 20;
            extra = (laneCount-1)*SnP_laneLengthInBytes + blocks%SnP_laneLengthInBytes;
            alignment = (laneCount+7+extra)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xCE - 33*i + 4*alignment - 8*laneCount;
            memset(bufferOut, 0xCE, sizeof(bufferOut));
            // in != out
            assert(PlSnP_FBWL_Wrap(stateTest, laneCount, laneCount, PlSnP_P*laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            assert(PlSnP_FBWL_Wrap(stateTest, laneCount, blocks*laneCount, laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x02);
            // in == out
            assert(PlSnP_FBWL_Wrap(stateTest, laneCount, laneCount, PlSnP_P*laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x03);
            assert(PlSnP_FBWL_Wrap(stateTest, laneCount, blocks*laneCount, laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x04);
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

    // Testing PlSnP_FBWL_Unwrap()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned char bufferOut[PlSnP_P*SnP_laneLengthInBytes*100+SnP_laneCount*SnP_laneLengthInBytes+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Unwrap(stateTest, SnP_laneCount, SnP_laneCount, PlSnP_P*SnP_laneCount, buffer, bufferOut, (SnP_laneCount-1)*PlSnP_P, 0) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            blocks = 100/laneCount;
            if (blocks > 20) blocks = 20;
            extra = (laneCount-1)*SnP_laneLengthInBytes + blocks%SnP_laneLengthInBytes;
            alignment = (laneCount+0+extra)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xDD - 65*i + 8*alignment - 16*laneCount;
            memset(bufferOut, 0xDD, sizeof(bufferOut));
            // in != out
            assert(PlSnP_FBWL_Unwrap(stateTest, laneCount, laneCount, PlSnP_P*laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x10);
            assert(PlSnP_FBWL_Unwrap(stateTest, laneCount, blocks*laneCount, laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x20);
            // in == out
            assert(PlSnP_FBWL_Unwrap(stateTest, laneCount, laneCount, PlSnP_P*laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, laneCount, PlSnP_P*laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x30);
            assert(PlSnP_FBWL_Unwrap(stateTest, laneCount, blocks*laneCount, laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, blocks*laneCount, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x40);
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

#ifdef OUTPUT
    // Outputting the result
    {
        unsigned char buffer[PlSnP_P*(SnP_width/8)];
        unsigned int i;
        FILE *f;
        char fileName[100];

        PlSnP_ExtractLanesAll(stateAccumulated, buffer, SnP_laneCount, SnP_laneCount);
#ifdef Keyak
        sprintf(fileName, "PlSnP/Parallel%dKeccakP-%d-12.txt", PlSnP_P, SnP_width);
#else
        sprintf(fileName, "PlSnP/Parallel%dKeccakF-%d.txt", PlSnP_P, SnP_width);
#endif
        f = fopen(fileName, "w");
        assert(f != NULL);
#ifdef Keyak
        fprintf(f, "Testing PlSnP with %d-way parallel Keccak-p[%d, nr=12]: ", PlSnP_P, SnP_width);
#else
        fprintf(f, "Testing PlSnP with %d-way parallel Keccak-f[%d]: ", PlSnP_P, SnP_width);
#endif
        fprintf(f, "\"");
        for(i=0; i<PlSnP_P*(SnP_width/8); i++)
            fprintf(f, "\\x%02x", buffer[i]);
        fprintf(f, "\"\n");
        fclose(f);
    }
#endif

    {
        const unsigned char *expected = (const unsigned char*)
#ifdef KeccakF_1600
#if (PlSnP_P == 2)
        "\x8e\x11\xbd\x0f\x8b\x68\xa6\xa6\xe3\xd4\x9a\x1a\x99\x9e\xf9\xbe\x00\x67\x26\xb5\xb2\xff\x3e\xe0\x92\x57\x97\x9d\x8b\x93\x43\x65\xa8\xa9\xd7\xf7\x5e\xe1\xb2\x9c\x16\x3f\x22\x6b\x40\xc8\x29\x28\x4c\xed\xab\x16\x91\x48\x2f\x07\x5b\xa2\x23\xf5\xff\x74\x18\xd2\x4d\xe4\x57\xbc\x07\x1d\x03\x78\x71\xab\x9d\xcc\x53\x4c\xb9\x35\xce\x5d\x68\x70\xad\xfd\xbe\x2f\x6e\xda\xbd\x4b\x85\x38\xd0\x1b\xa3\xa0\xc0\xfa\x4f\xfe\xfa\x4d\xaa\xc5\x90\xa7\xca\xac\x12\x88\x99\x6d\xc4\xc4\x33\xee\x21\xf6\x25\x56\xc1\xbe\xdb\x34\x96\xf4\xa6\xf1\x99\x58\x10\xb2\x3f\xd9\x70\xf4\x46\x21\x14\x54\x86\x27\x5a\xd2\x69\xfb\x13\xaa\xef\x7a\x8d\x1e\xc0\xeb\x4d\x73\x5f\x60\x52\x3f\x46\x15\x6d\x14\x06\x6b\xb7\x1e\xee\xfb\xc5\x25\xa9\x8e\x1a\x84\xf3\x64\x79\xcf\xf8\x06\x66\xc8\xf2\xd0\x45\x20\xcc\xcc\xd6\xfb\x47\x6c\x0c\xb0\x59\x73\xb8\x4a\xb8\x55\xcf\x04\x6e\x31\xb7\xc4\xa6\xd9\x05\x6c\xcc\xbd\xb1\x94\xa4\x60\xe6\x37\xf5\xe2\x29\x01\x71\x2f\x9a\x25\x33\xef\x79\x47\x4e\xa7\x7b\x76\x90\x05\x3c\x27\x38\x90\x56\x55\x9d\x0b\x9f\xa4\x4b\x11\x61\x26\xe0\xee\xa2\x94\xbc\xb8\x02\xde\xfc\x93\xa8\x13\x78\x29\xc0\x0e\xd6\x0e\x63\xd1\x72\x0f\xe5\x81\x4b\x6d\xa6\xb1\x6e\x39\x0c\xd7\x98\xf3\x12\xa6\x56\xff\xe5\x26\x1e\x57\x31\x41\x64\x72\x01\x13\xd1\xb6\x9d\xc6\x4e\x64\x60\xf7\x7b\xe5\xfa\x45\x39\xa0\x1b\x51\x02\xa9\x72\xe7\xa4\xb3\x8e\x01\x25\x02\x96\x8b\x26\x2f\x46\xb0\x56\xbb\xb4\xeb\xcb\xfb\xe4\x6e\x27\xec\x6f\xb0\xdd\x1e\xe3\xca\xbc\xae\xd2\x52\xab\xb2\xaa\x6f\xb6\xbe\x2a\xd0\x2e\x3c\x18\x40\xb1\x9c\x7e\x3d\xee\x95\x86\x00\x13\xf2\xa4\xa6\x88\xfc\xcb\x55\x14\x5d\xee\xdb\x73\x04\x77\x01\xad\x86\xb2\x37\x6d\x48\x9f\x8c\x9c\x52";
#endif
#if (PlSnP_P == 4)
        "\xce\x9b\x18\x0a\x37\x5d\xcd\xd8\x78\xf6\x96\x08\x5f\x75\xa7\x81\x03\x5a\x7b\xf4\xab\xcb\x2f\x63\x52\xc7\xe1\xd3\xfd\xf9\x42\x9a\xab\xd7\xdc\xf5\x6a\x1f\x5d\x9f\x82\x56\x10\xac\xdb\x39\xc8\x9b\xea\xa3\xc2\xb2\x40\x68\xbc\xd8\x80\xf0\x0d\x71\x96\xaa\x7e\x0c\x9d\xf5\x58\xba\xf6\x7e\x25\x62\x0f\x53\x5c\x02\x04\xf3\x89\xe6\x88\x83\x05\xee\xba\x33\x64\x22\x0e\xd1\xb3\x12\x06\x59\xae\x69\x86\x20\x01\xc2\xe4\xd8\x47\x05\xbf\x0d\x44\xd1\xa3\x1b\xea\x12\x9b\x0e\x07\xf7\x23\x24\xb9\x5d\x2b\x37\xac\xb1\xb6\xba\x8c\x1f\x63\xd4\x0a\x73\xad\xbf\xb3\x0b\x52\xc0\x3a\x8d\xfa\x53\x04\x76\x34\x7a\xfd\x51\x33\xb7\x17\xd4\xc7\x4a\xb1\x50\x62\x60\x91\x03\xb1\xc5\x9a\x1e\xbe\xd5\xd6\x17\x00\x3a\x28\x3e\x9f\xe9\xf9\xdd\x5c\xe6\x18\xda\x60\xb3\x5b\x36\xe0\xdf\x92\xcf\x55\xdf\x57\xaa\xe1\x77\x1c\xae\xb5\x7e\x92\x74\x63\x3f\xd0\x07\x8c\x9d\xf2\x49\x8d\xe6\xa5\xef\xca\x8e\xa1\x9f\x32\x1c\x1b\xce\x7a\xab\x92\x79\xc4\x28\x99\x35\x32\xad\xd1\xe4\x3e\x69\x4d\xe1\xfe\xdc\x67\x9b\x4e\x52\xa9\x09\x8c\x35\x41\x4a\x63\x0d\x01\x70\xcf\x3c\xa2\xca\xc0\xbb\xc1\xab\x2c\xde\x94\x49\x09\xa5\x23\x0c\x24\x86\x44\x36\x9b\x46\xd1\x77\x27\x53\x52\x19\x36\x2c\x51\x51\x68\x10\x91\xe9\x29\xc8\x08\x04\xbb\x41\x6b\x15\xee\xf8\x70\x06\x06\xa0\xd7\x2b\xba\x80\xf7\x90\x42\x9c\xb8\xd2\x93\x67\x1d\xb9\x1d\xd8\x64\x66\x08\x52\xb5\x17\xbf\x23\x13\xa4\xc7\x08\xf5\x78\x69\xd3\x30\x44\xf1\xd4\x3e\x55\xc6\x94\x11\x4b\x74\xea\xfd\xd1\x9c\x47\xb7\x4c\x56\x0b\xdb\x29\x71\xeb\x86\xd7\xb3\xb6\x58\xe0\x0d\xe4\x7b\xcd\x6f\x3c\xea\x79\x5f\x21\xde\x8f\x79\xab\x5e\x28\x6f\x11\xf4\xc1\xb0\x57\x6e\xf8\x02\x4c\x3a\xf7\x2a\xe9\x02\xc5\xe7\x9e\x82\x06\x48\xa9\x6d\xdf\x49\x37\x2a\xa1\x32\x49\x3e\x73\x9c\x94\x03\x7a\xe3\xb9\x3b\xef\x4b\x58\x93\x2f\x75\x96\xd5\x28\x43\x10\xa4\xc9\x37\xd2\x1b\x4f\x95\x45\x32\x58\x46\xe2\xe9\xd8\x0c\xa9\x2b\x55\xf6\x3d\x13\xd4\xc3\x0e\x65\x2c\xb9\xeb\x25\xed\x0c\x87\x81\x8d\x31\x79\xc8\x70\xd4\xb7\x3c\x4d\x29\x57\x5d\x8d\xa7\x73\xb9\xa5\xef\x59\x1f\x01\x62\x93\x27\xac\x2a\x99\x66\xe4\x8d\x58\xa9\x52\x29\x98\xd1\x58\x0f\xfe\xb1\x2c\x10\xb8\x41\x6d\xd6\x11\x09\x3d\x1e\x9d\xef\x39\x79\x60\x12\x7d\xaa\x22\xa8\x92\xbd\x92\xf3\x4d\x6d\x30\x72\xd0\x5a\xd3\xb6\x3a\x9d\x70\x90\x2c\x1f\x4a\x5e\x74\x61\x25\xd6\x7b\x83\x43\x53\xd6\x00\x0c\x97\x5e\xe7\xe5\xc5\xb4\x92\xc8\x60\xb0\x8a\x7e\x6e\x65\x13\x3c\x1b\x61\x92\xed\xe0\x19\x43\x15\xa1\xf6\xea\xc9\xd1\x2f\x11\x0f\x0b\x0e\xe4\x1d\x75\xac\x3e\xd8\xb2\x21\xf8\x64\x63\xb0\x12\x55\x1a\xa2\xae\xc3\xd1\x5d\x6d\xef\x5d\x8b\x0b\x87\x44\x9d\x74\x80\x79\x89\x16\xc4\xb2\xbe\xe4\x4e\x83\xb5\x68\x91\xdd\xa6\x83\x49\xc7\x20\xc7\x8e\xf0\xb2\x55\x57\xea\x3d\x51\x6e\x57\x3d\x68\xbd\x47\x62\x63\x22\x87\x25\x5d\x9c\x20\xa4\x68\x85\x3c\x8f\xf7\x80\xf5\xda\xdb\x1b\xa5\xe6\x41\x8b\x79\x8c\xe0\xa9\x28\x9a\xe2\xbc\x81\x06\x05\x09\x35\xcc\x37\x09\x07\xd7\x72\x41\x26\x68\xad\x6c\x5e\xba\xdc\x4b\xc1\xb7\xc0\xbc\x85\xff\x1e\x26\xcf\x65\xe6\x3c\x14\x62\x77\x08\x4a\x40\x58\xeb\x9a\x9c\x39\x0c\x05\x1c\x46\xe8\xe0\x68\xba\xe9\x60\x10\x68\x40\x2b\x2b\x61\xe7\x64\xd8\x47\x9a\x67\xd4\x42\xe3\x7a\x6a\x4a\x36\x2f\xf1\x23\x2d\x92\xa4\xf8\xb8\xe2\xc9\xd1\xc2\x7c\x6d\x5c\x54\xed\xf8\xcb\x0b\xb5\x75\xb0\x70\x79\xc7\x2d\x3f\x4e\x57\x2c\xa4\x5d\x85\xe6\xf1\xad\x1c\xd9\xb8\x46\xca\x75\xb7\xd2\x88";
#endif
#endif
#ifdef KeccakP_1600_12
#if (PlSnP_P == 2)
        "\xdc\x76\xcd\x4f\xa9\x27\x69\xac\xfa\xb8\xac\xd2\x64\xbe\x98\xce\x3e\x48\xed\x53\xdb\x21\xba\xc8\x70\x9b\xa3\xfe\x6f\xe9\x45\xe7\x7d\xa7\x48\x89\x78\x7c\xb5\x92\x33\x50\x9b\xa3\xd7\x8c\x41\x66\x24\x7e\x11\xca\x5d\xd9\x42\x3d\x8d\xd7\xc0\x6e\xaf\xcb\x9c\xae\x79\x33\x04\xf2\xf9\x2f\xca\xfb\xdf\x65\xee\x54\x03\xbb\x3a\xc8\x1c\xe0\x73\x76\x17\xf0\xdc\xd6\xe0\x9d\x17\x08\x7a\x54\x41\x19\xf6\xe3\xcc\x14\x38\xd7\x75\xf4\x5f\xe6\x6c\x86\xa1\xe2\xb7\xcc\xcf\x0e\x3d\x15\x06\x06\xc0\x58\x3d\x01\x98\x73\x9a\x8d\x11\x1f\x71\x48\x72\xa8\xb2\xed\xfd\x6d\x93\x5f\xb8\x8c\x1d\x92\xf0\x0b\x4f\xd9\xf8\x3f\x7d\x3c\xc6\xdc\x38\xb9\xda\x07\xc7\xe0\xb0\xda\xd6\x3d\xd6\xae\x7c\x5b\x7e\xbc\xdd\x5c\x45\xc2\x6e\x77\xbe\xe3\xdd\x79\x0e\x0d\x18\x3c\xaa\x8f\x4e\x7c\x5e\xb2\x77\x93\xa3\xe3\xff\x50\x32\x7e\xda\x34\xcc\xc9\x26\x97\x63\x3b\x6d\xd4\x35\xbc\x31\x15\xc9\x2b\xd2\x79\xb9\x8a\xc9\x9a\x36\x61\x41\x5e\x6b\xd0\x95\x09\x61\x66\x1c\x02\x23\x9c\x3c\x4f\xbc\xb8\x25\x72\x4f\xc6\x11\xe7\xbd\x30\x5d\xd5\xe2\xf4\x1c\x22\xdb\x49\xa6\xe5\xe5\x42\xa8\x3e\x60\xa9\x29\x39\x3c\x21\xbb\xc4\x82\xe0\x6c\x0d\xdc\x6e\x1a\x30\x18\x60\xdf\x36\x40\x0a\x43\x37\x57\x3e\xed\x09\x78\x7f\x1c\xb0\x39\xaa\x60\xac\x9a\x6e\xb4\xe4\x69\x57\x47\x8c\x3f\x93\x46\x63\xf0\xf4\xcf\x92\xad\x37\x1c\x2d\x13\x87\x23\xe7\xd1\x4e\x2a\xed\x34\x4f\xd3\x41\x2f\x8f\xe4\x34\xad\x1b\xed\xbe\xed\xf1\x0b\x35\xb9\x90\xa6\xd9\x10\xbb\xe1\x14\x19\x5f\x07\x7a\x24\xd8\x71\x0b\xf1\x95\xc0\x86\xa7\x79\x0d\x1d\xf0\x33\xa4\xf2\x29\xf5\xaa\x69\x0f\x9c\xd8\x63\xb7\xe3\x92\x95\xf4\x8b\xce\xf8\xdc\x1b\x7e\x72\x31\x45\xea\xd0\x2e\xfd\x70\x1e\xa9\x3d\xd6\x1f\xcc\x6c";
#endif
#if (PlSnP_P == 4)
        "\xdc\xad\x6f\x9a\xce\x68\x80\x43\xde\x1d\xaf\xae\x67\xdd\xb7\xcd\x4b\xa2\x56\xe2\x9b\xfd\x97\x5b\x45\x7f\x03\x98\x09\x52\xb8\x01\x0d\xce\x4d\x1e\x4c\xe7\x2f\xe1\x82\x97\x7f\x5e\x3b\x98\xb5\x97\xf0\x1c\x44\x38\xe3\x78\xf8\x7e\xe4\x54\x4b\x20\x43\xd0\x6a\xa4\x1b\x58\x1e\x4e\x3a\xc7\xb8\x85\x5c\xbd\xb7\x70\x59\x69\x8e\x95\x64\x5f\x4f\x2b\x34\x11\x14\x30\xa9\x51\x32\xd3\x20\xfa\xfd\xf8\xf8\xfa\x23\xfe\xc0\xeb\x15\xcf\x11\x5d\xd4\x9b\x3e\xdd\xb1\x3b\x22\x7f\xd5\xb3\x3c\x3d\x07\x72\xa2\x36\x8b\x6b\x28\x19\x0e\x1d\x46\x0c\x28\x39\x39\x62\x0b\xe0\x32\x6e\xdd\x94\x3a\xd1\x8f\x56\x91\x4c\xe3\x21\xbe\x5f\x83\x07\x85\x7c\x49\xe6\xe3\x09\x83\x17\xa5\x18\x9b\x55\x2f\x22\x20\x53\x70\x78\x66\xa4\xf4\x8b\xd0\x3e\xb9\xe9\x26\x53\x34\x27\xf6\x65\xa3\x0b\x18\xa2\x87\x16\x89\xb1\xeb\xd4\x43\x87\xf3\x8e\xd5\xdc\x74\x3e\xf5\x97\xb4\xd4\xbe\x07\xbb\xce\xee\xdd\x08\xab\x9d\xe4\xbb\xa7\xa3\x5c\xd5\x75\xa0\x5e\x5c\x60\x6a\xed\x17\xa9\x36\x74\x14\x91\x89\x7b\xf2\xdf\x3b\xfc\x69\x32\x75\x4c\x89\xa3\xf8\x04\x0e\x4a\xe3\x73\xd6\x65\x9c\x5d\xde\xba\xd2\x52\x04\xe9\xfd\x44\xb1\xf7\xfe\xfa\x66\x4a\x96\x9e\x38\x50\xeb\x6a\x2e\x83\x5f\xbc\x47\x07\xfc\x6b\xde\x2d\xe7\xcf\xdc\x76\xc6\xdd\xd5\x34\xb4\x00\xdc\x2a\xb7\x4a\xc8\xec\x71\xee\x5c\x40\xfe\x86\xe8\x12\x17\x50\x55\xf9\xf5\x18\xd6\x7e\x45\xf0\x35\xb1\x78\xad\xdc\x76\xeb\x71\x30\xb8\x5e\x1b\x91\x76\x1c\xfb\xbd\x9e\x7d\xb0\x5e\x1d\xb8\x10\x87\x72\x17\xd6\x72\xd8\x91\xd2\x39\x46\x2f\xbc\xd9\x7e\x63\xbf\x24\xa5\x96\x3c\xfc\x2e\xac\xc6\xa9\xd9\x02\x02\xd3\x49\xc8\xbc\xe4\xb3\x20\xa7\x48\x57\xea\x0e\x29\xdd\x0b\x9d\xd4\x11\xa4\x03\xf1\xf7\x36\xd6\xc8\x48\x7d\x4b\x7d\x91\xdc\x19\xbb\x07\x85\x45\xba\x2a\xe4\x92\x68\xb4\xa2\xc1\xa1\xd6\x48\xa3\x17\xc9\x73\x63\x73\xee\xa5\x97\xc2\xde\xfe\x32\xb3\x9c\x8a\x47\x8a\xb9\x8b\x4e\xe3\xe4\x7c\x0e\x8e\x97\xca\x4e\x20\xd7\x32\x02\x18\x33\xd2\x98\x29\x40\x14\xcd\xa8\xaa\x76\xc2\x92\x34\x51\xc4\x47\x61\x40\x51\xe9\x44\xc5\x65\x72\x49\x93\x45\x47\x5d\xab\x76\x53\x8b\x25\x49\xe3\xc7\x9c\xe8\x01\xef\x77\xf0\x12\x96\xea\x31\xa0\xa6\xfd\x6b\xa8\x9c\x50\xb7\x4c\x67\xa5\x9d\xbe\x83\x16\x7d\x6a\x59\x84\x44\x1d\x64\xa6\x6a\x1a\x1d\x32\x1f\x61\xeb\x4f\xee\xc3\x57\x9d\xbf\x17\x81\x30\x2e\xb8\x7f\x76\xb4\xf9\xae\x4b\xcb\xdf\x87\x67\x40\x4a\x81\x0c\x5f\x3d\xbc\xcc\xd1\xcc\xf9\xe1\x00\x9e\xf9\xf3\x3e\x18\x17\x96\x0a\xde\xa7\x6b\x38\x6b\x85\x3a\xe7\x4a\x53\x9c\x64\xeb\x2b\x1a\x38\x66\xca\x05\xba\xe4\x43\x11\x9f\x8e\xfb\x5e\xd9\x2f\x97\x15\xd6\xb9\x59\x84\xf4\x05\x4e\xd9\xb7\x28\xe8\x3f\xeb\x23\x7f\xd6\xec\x10\x2c\xf8\x7e\xa7\x66\x81\x2c\x45\xda\x76\x40\xb0\x7c\xbc\x18\x97\x1f\x96\x89\xf9\x2a\xf9\x76\xdc\x41\xd8\x06\x27\x79\x04\x0b\xba\x69\x96\xb4\xa3\x6b\x4a\x11\x0d\x23\x21\xc0\xcb\x80\xfd\x42\x7a\xf9\xe9\x6f\xcd\x82\x09\x46\x14\x4b\x79\x6d\x0a\x5e\xba\xa1\xdf\xdf\x7b\xad\x0d\x6b\x70\xa7\x25\xdb\x36\x89\xc8\x44\xc4\x1e\x9e\x74\xca\xaa\x3a\x6a\x29\x7b\xb6\xe4\xd5\x9c\xd5\xfb\xe2\x96\xe1\xba\x39\xd3\xe4\x76\xff\x48\x20\xcf\x89\x93\x27\x1f\xd7\x9c\x6b\xd9\x6f\x7c\x4b\x6a\x21\x8a\x33\xcb\xe6\x50\x82\xbd\x7a\x8a\xb2\x8d\x8c\x90\xe5\x2b\xe5\x0b\xc5\xa3\xab\x02\x1c\x02\x06\x89\x2f\x61\xe7\x59\xdb\xf6\x0b\x07\x97\x34\xcb\xab\xf5\x47\x7a\x7b\x11\x8f\x23\x74\x59\x56\xbe\x9c\xd9\x8b\x88\x85\x21\xb2\x8c\xf8\xac\x69\x3d\xc3";
#endif
#endif
        unsigned char buffer[PlSnP_P*(SnP_width/8)];
        PlSnP_ExtractLanesAll(stateAccumulated, buffer, SnP_laneCount, SnP_laneCount);
        assert(memcmp(expected, buffer, sizeof(buffer)) == 0);
    }
}
