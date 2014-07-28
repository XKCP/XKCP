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

#ifndef Ketje
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
#endif
#if defined(Ketje)
    // Testing KeccakP_StatePermute()
	{
        unsigned int nr;

		for(nr=1; nr <= SnP_maxRounds; nr++) {
			SnP_PermuteRounds(stateTest, nr);
            accumulateState(stateAccumulated, stateTest);
        }
	}
#endif

#ifdef OUTPUT
    // Outputting the result
    {
        unsigned char buffer[SnP_width/8];
        unsigned int i;
        FILE *f;
        char fileName[100];
    
        SnP_ExtractBytes(stateAccumulated, buffer, 0, SnP_width/8);
#if defined(Ketje)
        sprintf(fileName, "SnP/KeccakP-%d.txt", SnP_width);
#else
        sprintf(fileName, "SnP/KeccakF-%d.txt", SnP_width);
#endif
        f = fopen(fileName, "w");
        assert(f != NULL);
#if defined(Ketje)
        fprintf(f, "Testing SnP with Keccak-p[%d, nr 1 to %d]: ", SnP_width, MaxRound);
#else
        fprintf(f, "Testing SnP with Keccak-f[%d]: ", SnP_width);
#endif
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
#ifdef KeccakF_800
            "\xc7\xe9\x85\xdc\x83\x57\x68\xcb\xdd\x92\x83\x81\xb8\xc2\x56\x71\xa3\xec\x11\x2d\x62\xe9\x95\xd1\x6d\x2e\xfd\xc7\x82\x54\xf0\x46\xd2\x96\xe9\x5b\x6d\x66\xcf\xa3\x47\x30\x4e\x9a\x21\x2c\x7f\x5c\x5d\x80\x1e\xe4\x5e\x0b\x75\x9d\x5d\x99\x74\x5c\xb7\x47\x51\xab\x65\xbb\x1a\x72\xf8\x93\x20\x01\x20\x91\x73\xd7\x8f\x25\x5a\xbb\x0b\x9a\x85\x73\xe4\x0e\x10\xd0\xb3\x6b\x48\x04\xc8\xed\xed\xb3\xde\xc0\xb4\x94";
#endif
#ifdef KeccakF_400
            "\x73\x98\xe7\xd2\xbc\x10\x82\x84\x3e\xc3\x1e\x16\xe2\xae\x5c\xad\xd0\x76\xba\x52\x0f\x9e\x57\xf8\xc7\xa6\xaa\xd4\x32\x06\xb6\x56\xf0\xa5\x33\xc4\x6d\x1f\x22\xfc\xd4\x7b\x6d\x30\x68\xcb\x0f\x66\x73\xce";
#endif
#ifdef KeccakF_200
            "\x5b\x6c\x69\xea\x3a\xea\xeb\x71\xb4\xc7\xaa\x66\xa9\x31\x65\xc8\x30\xb2\xa8\x86\x6c\x1e\xd2\x53\x32";
#endif
#ifdef KeccakP_400
            "\xf8\x5e\xae\x44\xcd\x42\x09\xec\x2f\xb0\x32\x29\xcf\xd7\x23\x12\x04\x76\xb2\x7b\x7b\xe2\xc5\x38\x4d\xcf\x57\xe8\x84\x62\xbb\x56\xc2\x74\x6e\x08\x17\x3c\x16\x31\xb9\x95\x32\xa4\x8d\xeb\x08\x8e\x93\xd5";
#endif
#ifdef KeccakP_200
            "\xa4\x79\x05\xc3\xa1\xcf\x6a\x0f\xd0\x8a\xef\xcb\x74\x85\x63\x5e\xb0\x1a\x79\xde\xb1\xda\xe3\xad\x73";
#endif
        unsigned char buffer[SnP_width/8];
        SnP_ExtractBytes(stateAccumulated, buffer, 0, SnP_width/8);
        assert(memcmp(expected, buffer, sizeof(buffer)) == 0);
    }
}
