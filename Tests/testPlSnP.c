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

void accumulateBufferParallel(void *stateAccumulated, const unsigned char *buffer)
{
    PlSnP_XORLanesAll(stateAccumulated, buffer, 25);
    PlSnP_PermuteAll(stateAccumulated);
}

void accumulateStateParallel(void *stateAccumulated, const void *stateTest)
{
    unsigned char buffer[PlSnP_P*(SnP_width/8)];
    PlSnP_ExtractLanesAll(stateTest, buffer, 25);
    accumulateBufferParallel(stateAccumulated, buffer);
}

void testPlSnP()
{
    unsigned char stateAccumulated[PlSnP_statesSizeInBytes];
    unsigned char stateTest[PlSnP_statesSizeInBytes];

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
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, laneCount, alignment;

        for(laneCount=0; laneCount<=25; laneCount++) {
            alignment = (laneCount+2)%8;
            for(i=0; i<PlSnP_P*(SnP_width/8); i++)
                buffer[i+alignment] = 0x75 + 3*i - 8*alignment;
            PlSnP_XORLanesAll(stateTest, buffer+alignment, laneCount);
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
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned i, laneCount, alignment;

        for(laneCount=0; laneCount<=25; laneCount++) {
            alignment = (laneCount+4)%8;
            for(i=0; i<PlSnP_P*(SnP_width/8); i++)
                buffer[i+alignment] = 0x75 + 3*i - 8*alignment - laneCount;
            PlSnP_OverwriteLanesAll(stateTest, buffer+alignment, laneCount);
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
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned laneCount, alignment;

        for(laneCount=0; laneCount<=25; laneCount++) {
            alignment = (laneCount+6)%8;
            memset(buffer, 0xD2-laneCount-32*alignment, sizeof(buffer));
            PlSnP_ExtractLanesAll(stateTest, buffer+alignment, laneCount);
            accumulateBufferParallel(stateAccumulated, buffer+alignment);
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
        unsigned char buffer[PlSnP_P*(SnP_width/8)+8];
        unsigned laneCount, alignment;

        for(laneCount=0; laneCount<=25; laneCount++) {
            alignment = (laneCount+1)%8;
            memset(buffer, 0xD2-laneCount-32*alignment, sizeof(buffer));
            PlSnP_ExtractAndXORLanesAll(stateTest, buffer+alignment, laneCount);
            accumulateBufferParallel(stateAccumulated, buffer+alignment);
        }
    }
    PlSnP_PermuteAll(stateTest);

    // Testing PlSnP_FBWL_Absorb()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Absorb(stateTest, 25, buffer, 24*PlSnP_P, 0) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<25; laneCount+=4) {
            alignment = (laneCount+5+extra)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xA0 - 17*i + 2*alignment - 4*laneCount;
            assert(PlSnP_FBWL_Absorb(stateTest, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            blocks--;
            extra++;
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

    // Testing PlSnP_FBWL_Squeeze()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Squeeze(stateTest, 25, buffer, 24*PlSnP_P) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=3; laneCount<25; laneCount+=6) {
            alignment = (laneCount+6)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            memset(buffer, 0xBF+laneCount*2+16*alignment, sizeof(buffer));
            assert(PlSnP_FBWL_Squeeze(stateTest, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, buffer+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            blocks--;
            extra++;
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

    // Testing PlSnP_FBWL_Wrap()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+10+8];
        unsigned char bufferOut[PlSnP_P*SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Wrap(stateTest, 25, buffer, bufferOut, 24*PlSnP_P, 0) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=2; laneCount<25; laneCount+=6) {
            alignment = (laneCount+7)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xCE - 33*i + 4*alignment - 8*laneCount;
            memset(bufferOut, 0xCE, sizeof(bufferOut));
            // in != out
            assert(PlSnP_FBWL_Wrap(stateTest, laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            // in == out
            assert(PlSnP_FBWL_Wrap(stateTest, laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            blocks--;
            extra++;
        }
        accumulateStateParallel(stateAccumulated, stateTest);
    }

    // Testing PlSnP_FBWL_Unwrap()
    {
        unsigned char buffer[PlSnP_P*SnP_laneLengthInBytes*100+10+8];
        unsigned char bufferOut[PlSnP_P*SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(PlSnP_FBWL_Unwrap(stateTest, 25, buffer, bufferOut, 24*PlSnP_P, 0) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<25; laneCount+=4) {
            alignment = (laneCount+0)%8;
            assert(blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xDD - 65*i + 8*alignment - 16*laneCount;
            memset(bufferOut, 0xDD, sizeof(bufferOut));
            // in != out
            assert(PlSnP_FBWL_Unwrap(stateTest, laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            // in == out
            assert(PlSnP_FBWL_Unwrap(stateTest, laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes);
            PlSnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*PlSnP_P*SnP_laneLengthInBytes, 0x01);
            blocks--;
            extra++;
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
    
        PlSnP_ExtractLanesAll(stateAccumulated, buffer, 25);
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
        for(i=0; i<PlSnP_P*(SnP_width/8); i++)
            fprintf(f, "%02x ", buffer[i]);
        fprintf(f, "\n");
        fclose(f);
    }
#endif

    {
        const unsigned char *expected = (const unsigned char*)
#ifdef KeccakP_1600_12
#if (PlSnP_P == 2)
        "\xe0\xdd\xe4\x36\x9d\x69\x4a\xe3\xb4\x08\x57\x38\x35\x0f\xbb\x35\x2d\x7f\x7d\xc8\x69\x84\x3b\x82\xa8\x0e\x59\x7b\x08\x3f\xd6\x99\x37\x70\xfd\xad\xea\xda\x41\x93\xdc\xdc\xd1\x75\x23\x1b\x4b\x6d\xda\xb0\x83\x3d\x91\x77\x03\xd1\x5a\x36\xe7\x48\xf3\xdd\xef\x3e\x2d\xe6\xc1\x82\x72\xac\x3d\x0e\x98\xf4\xa6\xa3\x91\xa5\x4e\x1c\x13\xce\x7c\xcb\x6c\xd0\xd8\x71\xec\x65\x70\xa7\xc2\xff\xeb\xeb\xbf\x15\x0e\xc9\x7a\x50\xe9\x1a\xb9\x00\xce\x5c\x79\xc4\x55\xbf\xf4\x3c\x27\xa2\x22\xe4\x6f\x8a\xa1\x2f\xca\x8c\x6a\xd7\x3f\x6a\x8d\xcb\xd4\x3f\x0d\xa1\x3b\x2c\xad\xbb\x84\x36\x5e\x02\x0d\x76\xc7\x07\x6c\xf0\x63\xb7\xf6\xf4\x0e\x06\xef\xa7\x63\xd2\xb8\xc2\x46\x29\x59\xf9\x41\x6d\x7c\x9e\xa5\xc4\xec\x2f\x68\x3f\x0a\x53\xdf\x19\x39\xac\x2d\xf8\xea\xc2\xe6\xa0\xd7\xcb\x2e\x77\x3e\x1d\x47\xf1\x84\xa0\x07\x56\x71\xac\x92\x4e\xab\x23\x18\xf2\xfa\x42\xd8\x13\x8b\x72\x19\xef\xf2\x47\x9f\xe0\x90\x31\x84\xc1\x29\x15\xe7\x83\x04\x96\xb9\xe6\x06\xae\xfb\x89\x55\x46\xdc\x37\x80\xcf\x7a\x32\xdd\x2e\x9a\x41\x8d\x51\x40\x2c\x4e\x9c\xb1\x9c\xd8\x27\xec\x3c\xfc\x57\x93\x63\xcc\xf1\x94\xe3\xef\x49\x5d\x48\x47\xe1\x1d\x9d\x4d\xd6\x4c\x2f\x43\xaf\xcf\xc1\x68\x4a\x13\x50\x9e\x24\xda\x04\x5e\x08\x78\x62\x3e\xf5\xf4\x93\x4d\xbc\x87\xb1\x21\x7f\x8d\xb4\x2f\xdc\x54\xb4\x35\xf7\x80\x5c\x66\x2b\x91\x75\x68\xb4\x04\xb2\xa6\xbe\xfd\xaa\x07\xdf\x5d\x8d\x81\x47\xda\x18\x4b\x5f\x2c\x72\xd1\x98\xd0\x42\xe7\x17\xdf\x0d\xef\x15\xf6\x3f\x00\xf2\x37\xf5\x20\x65\x84\x53\xbb\x93\x5b\x63\xc3\x05\x1e\xc4\x5d\x7a\x71\x3d\xdd\x6a\x3e\xcf\x3b\xf5\x27\x1c\x36\x42\x1c\xb8\x00\xcf\xae\xa3\x91\x2f\x3d\x7c\xd0\x2f\x4b\xd4\x6b\x53\x13\x53\x22\x0a";
#endif
#if (PlSnP_P == 4)
        "\xfe\x6a\x61\xea\xd5\x80\x84\xb7\x37\x5f\x26\xc6\x45\x32\x98\x78\x96\xc3\x66\x77\x00\x60\xc4\xcb\xfc\x16\xa1\x6f\x26\xc1\x2a\x5a\x32\x78\xdf\x2f\xb4\xe4\xb4\x4d\x35\x22\x39\x6d\xcc\x30\xcc\xe9\x5d\x26\xf2\x07\x9d\xef\x4d\x41\xd8\x92\x22\xbc\xed\xee\x6e\x29\x58\x4f\x45\xda\xf0\x08\x23\x64\xbf\xe8\x45\x07\x61\x95\x23\xd6\x8c\xf7\xd2\x6e\x83\x99\xa5\x81\x56\x20\x6f\x66\x6b\xc0\x37\x14\x27\x2e\xf0\xcc\xe7\x07\x3c\xe7\xc7\x84\xaf\xf0\xce\x5d\xdb\x26\x86\xd9\x91\x43\x8a\x4f\x1c\x79\x76\xb5\x49\xda\x3d\x8e\x52\xd8\x42\x31\x86\x26\x10\x0a\x8b\xac\x0a\x1c\xa4\xa6\x90\xdd\x68\x6e\xc9\x00\x2b\x8b\x80\x6f\x5d\xed\xac\x9b\xe2\xd6\xb7\x0e\xef\x1b\x5c\xfd\xc8\x68\xe4\xe1\x1d\xef\xb4\x78\xfb\xdc\x05\x80\x02\x65\x88\x6a\xce\xea\xd8\x8c\x83\x6a\x3c\x20\x01\xed\xca\x0d\x7f\xdd\xc6\x98\x54\xaf\xbe\xee\x74\xae\xd5\x3d\x32\x3f\x83\x59\xb0\xf6\x37\x3e\xb9\x17\x8d\x12\xe7\x22\xe3\xbe\x6f\xe8\x92\xe2\x16\xd7\x96\x2f\x43\xc1\x2e\x61\x6e\x13\x33\x65\xcb\x2d\xe3\x8b\x50\x6a\x6f\xd4\xa1\xf3\x01\xb2\x3d\x13\xe5\xd2\x63\x50\xfc\x03\x99\x42\x1b\x29\xa2\x1d\x70\xda\xa7\xee\xf8\xc1\x1b\xca\x51\x66\xfc\x63\x4b\x77\x9c\x7d\x8b\x49\xa7\x8a\xff\xeb\x89\x79\xe3\x18\xae\xca\xa8\x2b\xd6\xb2\xe6\xc8\x74\x30\xa2\xf4\x3e\x5d\xe8\xa0\xf5\x6f\x17\x97\x4b\xc3\xd9\x33\x32\xca\x52\x96\xb7\x5d\x39\x89\xb8\x15\x31\x2c\xea\xa2\x47\x51\x3d\xec\xb5\x29\xec\x5b\x8a\x5e\x55\x13\x7d\x27\x91\x4f\x23\xc7\x5f\x80\x04\x69\x95\xe2\x59\x7b\x25\x09\xa8\xee\xdf\xdd\x62\xc2\xcc\xa4\x8c\x74\xa4\x11\x5a\xfa\x7a\xbd\x0f\x93\xfd\x74\xd8\xe8\x19\x08\xe1\x93\x31\xad\x82\x4d\xa3\x2f\xb0\x0d\xd5\x11\x20\x79\xe9\x1c\xad\x19\x32\x57\x90\xee\xd8\x08\x0c\xf6\xd9\xf4\x29\x6c\x11\x85\x2b\xb8\xe5\x0e\xa9\x75\x04\xac\x18\xd4\x3e\xaa\x7f\x19\x9c\x43\xf0\x94\xa9\x0d\xa0\xba\xa0\x34\x42\x77\x37\xb9\xd8\x52\xea\xa3\x34\x0b\x63\x0c\x86\x8f\x26\x8c\x59\x5d\xee\x1d\xe0\xac\x08\x91\x80\xe8\x12\x3f\xef\xd9\xc9\x45\x2d\x51\x9b\xa2\xef\x6e\xab\xfa\xf8\x71\x38\x58\xeb\x2c\xe4\x83\x69\x75\x2d\xe6\x6e\xbe\x16\x70\x4f\xb3\x3f\x3d\x1e\x08\x14\x78\x2a\x85\xe6\x6a\xf9\x72\x3a\xa6\xad\x26\x68\x56\x3b\x32\x3a\x41\xc1\xd3\x51\x35\x74\x8f\xe9\x42\x64\x00\x30\x80\x2b\xdc\x3f\xa8\xb3\xf7\x26\xc1\x13\x06\x4e\x86\x3a\x47\xdf\xd3\x69\x8e\x07\x9c\xfe\x2a\xfa\x7c\xc6\x21\x95\x58\x37\x09\x5f\x68\xee\x2d\x97\x38\xc1\xd2\xf5\x0f\x4f\xc1\x94\x97\x93\x52\xd8\x8c\xb2\xd3\xbe\x42\xbf\x69\x8d\x63\x09\x2e\x90\x6f\x5f\xd5\xfa\x23\x3c\xd0\x2d\x47\x64\x53\x77\xed\x09\x3b\xf6\x08\x63\xbb\xe5\xf4\x49\x26\xe8\xfb\x7a\xf8\xee\x6f\xbe\x70\x1c\x44\x87\x07\x67\xec\x84\x6a\x70\x0b\x7c\xca\xb7\xb8\x57\xd1\x0f\x31\xaf\xfe\x11\x2b\x7c\x12\x28\xcb\xa2\xb8\x33\x77\x69\x09\x9a\xa9\x52\x73\x34\xcd\xcc\x46\x98\x2b\xaf\x9a\x49\x34\x6a\x9c\xff\x3b\x25\x89\xb0\xfd\xb0\x81\xe7\x45\xc0\xdc\x62\x99\x96\xfe\x94\x38\x9c\x4e\x43\xe9\xa8\x50\x4c\xd6\xee\x46\xe2\x3b\x8e\x81\xab\xc1\x10\xd4\x10\x41\x6f\x30\x08\x41\x5d\x34\xa4\xb1\x47\xe4\xd1\x37\x3f\x73\xab\xbc\xcd\x61\x4b\xb3\xed\x1b\x7e\x4c\x65\x2e\xd7\x7c\x14\xf9\x31\x6e\xaf\x68\x44\x53\xcb\x68\xb7\x29\x20\xb6\x6f\xb6\xf6\x3a\xdd\x84\xcb\xba\x31\xab\x6f\x9d\x22\xa1\x64\xb5\x1d\x62\xd2\x76\xe2\x2a\x52\xa6\x79\xb3\x95\xa2\x7b\x56\xf4\x98\x38\x19\x7c\x3b\x0d\x5e\xbf\x9e\x24\x03\x71\xd1\x74\x73\x39\x8e\x28\x17\x03\xa0\x4f\x4d\x72\x09\xdd\x3d";
#endif
#endif
        unsigned char buffer[PlSnP_P*(SnP_width/8)];
        PlSnP_ExtractLanesAll(stateAccumulated, buffer, 25);
        assert(memcmp(expected, buffer, sizeof(buffer)) == 0);
    }
}
