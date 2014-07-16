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
#include "SnP-interface.h"

void accumulateBuffer(void *stateAccumulated, const unsigned char *buffer)
{
    SnP_XORBytes(stateAccumulated, buffer, 0, SnP_width/8);
    SnP_Permute(stateAccumulated);
}

void accumulateState(void *stateAccumulated, const void *stateTest)
{
    unsigned char buffer[SnP_width/8];
    SnP_ExtractBytes(stateTest, buffer, 0, SnP_width/8);
    accumulateBuffer(stateAccumulated, buffer);
}

void testSnP(void)
{
    unsigned char stateAccumulated[SnP_stateSizeInBytes];
    unsigned char stateTest[SnP_stateSizeInBytes];

    SnP_StaticInitialize();

    SnP_Initialize(stateAccumulated);

    memset(stateTest, 0xAA, sizeof(stateTest));

    // Testing SnP_Initialize()
    {
        SnP_Initialize(stateTest);
        accumulateState(stateAccumulated, stateTest);
    }
    SnP_Permute(stateTest);

    // Testing StateXORBytes()
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned i, offset, length, alignment;

        for(i=0; i<sizeof(buffer); i++)
            buffer[i] = 0xF3 + 5*i;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 10) ? 1 : 7)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 10) ? 1 : (5+offset)) {
            alignment = (offset+length+1)%8;
            SnP_XORBytes(stateTest, buffer+alignment, offset, length);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);

    // Testing SnP_OverwriteBytes()
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned i, offset, length, alignment;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 11) ? 1 : 5)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 11) ? 1 : (9+4*offset)) {
            alignment = (offset+length+3)%8;
            for(i=0; i<sizeof(buffer); i++)
                buffer[i] = 0xF3 + 5*i + alignment + offset + length;
            SnP_OverwriteBytes(stateTest, buffer+alignment, offset, length);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);

    // Testing SnP_OverwriteWithZeroes()
    {
        unsigned byteCount;
        
        for(byteCount=0; byteCount<=SnP_width/8; byteCount++) {
            SnP_Permute(stateTest);
            SnP_OverwriteWithZeroes(stateTest, byteCount);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);

    // Testing SnP_ComplementBit()
    {
        unsigned bitPosition;
        
        for(bitPosition=0; bitPosition<SnP_width; bitPosition += (bitPosition < 128) ? 1 : 19) {
            SnP_ComplementBit(stateTest, bitPosition);
            accumulateState(stateAccumulated, stateTest);
        }
    }
    SnP_Permute(stateTest);

    // Testing SnP_ExtractBytes()
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

    // Testing SnP_ExtractAndXORBytes()
    {
        unsigned char buffer[SnP_width/8+8];
        unsigned offset, length, alignment;

        for(offset=0; offset<(SnP_width/8); offset += (offset < 13) ? 1 : 9)
        for(length=(offset <= 1) ? 0 : ((SnP_width/8)-offset-2); length<=(SnP_width/8)-offset; length += ((SnP_width/8)-offset-length < 13) ? 1 : (5+2*offset)) {
            alignment = (offset+length+7)%8;
            memset(buffer, 0x3C+offset+length, sizeof(buffer));
            SnP_ExtractAndXORBytes(stateTest, buffer+alignment, offset, length);
            accumulateBuffer(stateAccumulated, buffer+alignment);
        }
    }
    SnP_Permute(stateTest);

    // Testing SnP_FBWL_Absorb()
    {
        unsigned char buffer[SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(SnP_FBWL_Absorb(stateTest, 25, buffer, 24, 0) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<25; laneCount+=4) {
            alignment = (laneCount+5+extra)%8;
            assert(blocks*laneCount*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xA0 - 17*i + 2*alignment - 4*laneCount;
            assert(SnP_FBWL_Absorb(stateTest, laneCount, buffer+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            blocks--;
            extra++;
        }
        accumulateState(stateAccumulated, stateTest);
    }

    // Testing SnP_FBWL_Squeeze()
    {
        unsigned char buffer[SnP_laneLengthInBytes*100+10+8];
        unsigned blocks, extra, laneCount, alignment;

        assert(SnP_FBWL_Squeeze(stateTest, 25, buffer, 24) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=3; laneCount<25; laneCount+=6) {
            alignment = (laneCount+6)%8;
            assert(blocks*laneCount*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            memset(buffer, 0xBF+laneCount*2+16*alignment, sizeof(buffer));
            assert(SnP_FBWL_Squeeze(stateTest, laneCount, buffer+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra)
                == blocks*laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, buffer+alignment, blocks*laneCount*SnP_laneLengthInBytes, 0x01);
            blocks--;
            extra++;
        }
        accumulateState(stateAccumulated, stateTest);
    }

    // Testing SnP_FBWL_Wrap()
    {
        unsigned char buffer[SnP_laneLengthInBytes*100+10+8];
        unsigned char bufferOut[SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(SnP_FBWL_Wrap(stateTest, 25, buffer, bufferOut, 24, 0) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=2; laneCount<25; laneCount+=6) {
            alignment = (laneCount+7)%8;
            assert(blocks*laneCount*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xCE - 33*i + 4*alignment - 8*laneCount;
            memset(bufferOut, 0xCE, sizeof(bufferOut));
            // in != out
            assert(SnP_FBWL_Wrap(stateTest, laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes, 0x01);
            // in == out
            assert(SnP_FBWL_Wrap(stateTest, laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes, 0x01);
            blocks--;
            extra++;
        }
        accumulateState(stateAccumulated, stateTest);
    }

    // Testing SnP_FBWL_Unwrap()
    {
        unsigned char buffer[SnP_laneLengthInBytes*100+10+8];
        unsigned char bufferOut[SnP_laneLengthInBytes*100+10+8];
        unsigned i, blocks, extra, laneCount, alignment;

        assert(SnP_FBWL_Unwrap(stateTest, 25, buffer, bufferOut, 24, 0) == 0);
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<25; laneCount+=4) {
            alignment = (laneCount+0)%8;
            assert(blocks*laneCount*SnP_laneLengthInBytes+extra+alignment <= sizeof(buffer));
            for(i=0; i<sizeof(buffer)-8; i++)
                buffer[i+alignment] = 0xDD - 65*i + 8*alignment - 16*laneCount;
            memset(bufferOut, 0xDD, sizeof(bufferOut));
            // in != out
            assert(SnP_FBWL_Unwrap(stateTest, laneCount, buffer+alignment, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes, 0x01);
            // in == out
            assert(SnP_FBWL_Unwrap(stateTest, laneCount, bufferOut+alignment, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes+extra, laneCount)
                == blocks*laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, blocks*laneCount*SnP_laneLengthInBytes, 0x01);
            blocks--;
            extra++;
        }
        accumulateState(stateAccumulated, stateTest);
    }

#ifdef OUTPUT
    // Outputting the result
    {
        unsigned char buffer[SnP_width/8];
        unsigned int i;
        FILE *f;
        char fileName[100];
    
        SnP_ExtractBytes(stateAccumulated, buffer, 0, SnP_width/8);
        sprintf(fileName, "SnP/KeccakF-%d.txt", SnP_width);
        f = fopen(fileName, "w");
        assert(f != NULL);
        fprintf(f, "Testing SnP with Keccak-f[%d]: ", SnP_width);
        fprintf(f, "\"");
        for(i=0; i<SnP_width/8; i++)
            fprintf(f, "\\x%02x", buffer[i]);
        fprintf(f, "\"\n");
        fclose(f);
    }
#endif

    {
        const unsigned char *expected = (const unsigned char*)
#ifdef KeccakF_1600
            "\xc4\xc9\x6f\x1a\x93\x8b\xd0\x08\x22\x97\x96\x52\x5d\xc5\xdd\x82\x49\x52\x4a\xcb\xbb\x3a\x7e\x7a\xd9\xbd\x3e\xfe\xdb\x8c\x77\x64\x5b\xc2\x2e\xb4\xad\x43\xf6\x80\x9a\xf8\xc8\xe8\x07\x10\x74\x82\x6f\x26\x68\x41\x2e\x37\x09\x13\x3f\xb1\x29\xa0\x8c\x65\xa3\xd8\xed\xd4\x3d\x3b\x17\x02\x5f\x80\x6e\x10\x3c\x6f\x6a\xe6\xce\x64\x72\xe4\x32\x6a\x67\x60\x63\x85\xa6\x34\xa2\x0a\x8c\xcb\x36\xd4\x04\x4c\x76\x60\x49\xc5\x96\x28\x2f\xbd\xe8\x9a\xbe\x7f\xc7\xc4\x2f\xa5\xab\xf3\xcb\x89\x2c\x67\x08\x1a\x5e\x07\x4d\x78\x39\x8a\x10\x4e\xc5\x7a\xb6\x89\x66\x75\xd3\x67\x40\x57\xd6\x40\x00\xd6\x59\x42\xcb\xde\x9e\x50\x5e\xe6\xc6\xcd\x51\xa9\x7e\x5b\xf0\xa9\x02\xe8\xbc\x35\x68\x00\x0c\x0f\xf1\x55\x0a\x96\xe3\x59\xeb\x06\x31\x44\x8b\x63\x30\xe9\x3a\x14\x8f\x7c\x15\xb8\x60\x6c\xc5\xb0\xd3\x24\xb7\xe8\x17\x84\xe6\x5b";
#endif
        unsigned char buffer[SnP_width/8];
        SnP_ExtractBytes(stateAccumulated, buffer, 0, SnP_width/8);
        assert(memcmp(expected, buffer, sizeof(buffer)) == 0);
    }
}
