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
    ALIGN unsigned char stateAccumulated[SnP_stateSizeInBytes];
    ALIGN unsigned char stateTest[SnP_stateSizeInBytes];

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

        assert(SnP_FBWL_Absorb(stateTest, SnP_laneCount, buffer, SnP_laneCount-1, 0) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            alignment = (laneCount+1)%8;
            for(i=0; i<laneCount*SnP_laneLengthInBytes; i++)
                buffer[i+alignment] = 0x11+2*laneCount+4*i;
            assert(SnP_FBWL_Absorb(stateTest, laneCount, buffer+alignment, laneCount*SnP_laneLengthInBytes, 0xFF)
                == laneCount*SnP_laneLengthInBytes);
        }
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<SnP_laneCount; laneCount+=4) {
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

        assert(SnP_FBWL_Squeeze(stateTest, SnP_laneCount, buffer, SnP_laneCount-1) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            alignment = (laneCount+2)%8;
            assert(SnP_FBWL_Squeeze(stateTest, laneCount, buffer+alignment, laneCount*SnP_laneLengthInBytes)
                == laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, buffer+alignment, laneCount*SnP_laneLengthInBytes, 0x7F);
        }
        blocks = 7;
        extra = 1;
        for(laneCount=3; laneCount<SnP_laneCount; laneCount+=6) {
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

        assert(SnP_FBWL_Wrap(stateTest, SnP_laneCount, buffer, bufferOut, SnP_laneCount-1, 0) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            alignment = (laneCount+2)%8;
            for(i=0; i<laneCount*SnP_laneLengthInBytes; i++)
                buffer[i+alignment] = 0x22+4*laneCount+8*i;
            memset(bufferOut, 0x22, sizeof(bufferOut));
            // in != out
            assert(SnP_FBWL_Wrap(stateTest, laneCount, buffer+alignment, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x3F)
                == laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x3F);
            // in == out
            assert(SnP_FBWL_Wrap(stateTest, laneCount, bufferOut+alignment, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x1F)
                == laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x1F);
        }
        blocks = 7;
        extra = 1;
        for(laneCount=2; laneCount<SnP_laneCount; laneCount+=6) {
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

        assert(SnP_FBWL_Unwrap(stateTest, SnP_laneCount, buffer, bufferOut, SnP_laneCount-1, 0) == 0);
        for(laneCount=1; laneCount<SnP_laneCount; laneCount++) {
            alignment = (laneCount+3)%8;
            for(i=0; i<laneCount*SnP_laneLengthInBytes; i++)
                buffer[i+alignment] = 0x33+8*laneCount+16*i;
            memset(bufferOut, 0x33, sizeof(bufferOut));
            // in != out
            assert(SnP_FBWL_Unwrap(stateTest, laneCount, buffer+alignment, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x0F)
                == laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x0F);
            // in == out
            assert(SnP_FBWL_Unwrap(stateTest, laneCount, bufferOut+alignment, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x07)
                == laneCount*SnP_laneLengthInBytes);
            SnP_FBWL_Absorb(stateTest, laneCount, bufferOut+alignment, laneCount*SnP_laneLengthInBytes, 0x07);
        }
        blocks = 7;
        extra = 1;
        for(laneCount=5; laneCount<SnP_laneCount; laneCount+=4) {
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
#ifdef Keyak
        sprintf(fileName, "SnP/KeccakP-%d-12.txt", SnP_width);
#elif defined(Ketje)
        sprintf(fileName, "SnP/KeccakP-%d.txt", SnP_width);
#else
        sprintf(fileName, "SnP/KeccakF-%d.txt", SnP_width);
#endif
        f = fopen(fileName, "w");
        assert(f != NULL);
#ifdef Keyak
        fprintf(f, "Testing SnP with Keccak-p[%d, nr=12]: ", SnP_width);
#elif defined(Ketje)
        fprintf(f, "Testing SnP with Keccak-p[%d, nr 1 to %d]: ", SnP_width, SnP_maxRounds);
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
            "\x10\x4b\x38\x31\xbf\xd8\xab\x7f\xb0\xb8\x0b\x6a\x69\x59\x16\x86\x86\x23\x72\x6e\x77\x76\x54\x2e\x21\xf0\xf0\x2a\x1b\x01\x06\xdc\xb9\xbb\x7d\x78\x8f\x8d\xcd\xbd\x4d\x78\x55\x63\x1c\x20\xf3\x97\x72\x3e\xa1\xcb\x60\x20\xee\x6b\xba\xa4\x8c\x51\x5e\xcb\xe1\xeb\x59\xd2\x94\x0c\xc4\xaa\x13\xf4\x7b\xe2\x78\x48\xa3\xc1\x5b\x59\x87\xab\x2b\x45\xb3\x38\x2f\x65\x98\xfd\x2d\xc5\xe9\x50\x8a\x16\xce\x4d\xbf\xf6\xfc\x60\x75\x05\x1c\x7a\x96\x28\x5b\xbe\x9f\xf1\x85\xef\x24\xfd\x7c\xcd\xc4\xd6\xeb\xee\xcd\x46\x6f\x83\x22\x3b\x2f\x9a\xbc\xf2\xe2\x76\x2c\x54\x69\xc3\x00\x0c\x8f\x6d\xd4\x2b\x0c\x32\x53\xc8\xcf\x8e\x4a\x26\x42\xab\x49\x19\x8e\xfe\x42\xfd\x08\x72\x50\x30\xa5\x75\x4a\x49\x0e\xa0\x48\x93\x72\x7e\x74\xfd\x38\x13\x58\x2c\x50\xb6\x4d\x96\xe7\xe9\x5c\xc7\xa9\x96\x1a\xa3\xdf\xc9\xe7\x31\x8c\xaa\x2d\x3d";
#endif
#ifdef KeccakF_800
            "\x50\x88\xa3\xa8\xc3\x19\x08\x4e\x32\xf6\x30\xd7\x41\x5e\xfc\x9a\xf0\xbf\x95\x88\x8e\xa2\x5b\xb8\xf2\xba\x35\x21\x26\xc3\x94\x5d\x75\xf2\xdc\x7c\x66\xde\x57\xc8\xd1\xa6\xb1\xc9\x03\x2d\x74\x14\xd6\x43\x0a\xb2\x3c\xda\x17\x21\xf5\x73\x42\xba\xff\x80\x14\xee\x98\x19\xe9\x69\x97\xc5\x80\xb8\x1a\xdb\x6c\xd2\x26\xdb\x33\xde\xaf\xc5\x8e\x7a\x19\x01\xb8\xa0\x47\xf9\x9f\x83\xc2\x9f\xaa\x50\x18\x54\x97\xd4";
#endif
#ifdef KeccakF_400
            "\x1a\x35\xbe\x05\xec\xb1\xd3\x8d\xdc\x46\x34\xc5\x60\xbf\xc1\xb0\x8f\x28\x03\x1b\x74\xc1\x04\xe2\xd2\xf8\xef\x66\x09\x90\xb1\x45\x2f\x0d\xa6\x09\xfc\x32\x4c\xf7\x2f\x85\xdc\x62\x4e\x78\xab\x49\x89\x3e";
#endif
#ifdef KeccakF_200
            "\x3b\x24\x3b\xbd\x10\x47\xa2\x28\xc7\x83\xc6\xe2\x4b\x71\x0b\x3b\x9c\x78\x2d\x88\xe9\xf7\x8e\x8d\x7b";
#endif
#ifdef KeccakP_1600_12
            "\x04\x92\xda\x72\x79\x1b\xc2\xee\x07\x9a\x38\x97\x20\xc1\x12\x0f\xa3\x4d\xca\xbf\x0c\x4f\xe7\x80\xde\x6e\xdc\xe1\x68\x67\x21\x4a\x8a\x17\x4e\xf5\xf2\x8c\x2f\xa2\x30\xc8\x8b\x95\x01\xae\x7e\x38\x5e\x3a\xfd\xc8\xeb\x4a\x61\xb4\x7f\x22\x2f\xe1\x6b\x86\xd1\x37\xf9\x1b\x2c\xea\xd0\xdf\x03\x0d\xbc\x2d\x14\x88\x88\x7c\x12\x88\x33\x3e\x9f\xf3\x4d\xc0\x04\x13\xdb\xce\x6c\xb4\xec\xd1\xc2\x44\xa6\x13\x1d\x26\x01\xd4\x26\x16\x14\x4d\xd8\xfc\xd1\x68\x7f\x07\x33\x77\x90\x84\x58\x04\xc3\xb4\x02\x26\x6a\x46\x61\xb6\xfe\x1c\xaa\x3f\x94\xa2\xba\xa6\xea\x3c\x43\x5f\x2a\xf4\x94\xc0\x73\x2d\x0f\x94\x33\x27\x84\xc4\xab\xd8\xe5\x5c\xd6\xc8\x9b\x09\x5d\x34\x49\x8a\x04\x89\x23\x41\x53\xd5\x6a\xd1\xa0\x9b\x97\x4a\xb5\x6b\x77\x50\xc3\xf8\x15\x9f\x00\xf0\xc1\x64\xee\xc7\x6a\x1b\x69\xce\xa2\x6f\xab\x3f\xa3\xdd\x35\xae";
#endif
#ifdef KeccakP_800_12
            "\xd3\x95\x03\x00\xfc\x40\x7f\x62\xb8\xce\x6e\xe3\x79\x95\xd3\xee\x3a\xf1\x36\xcf\x71\xc3\x28\x4a\xe2\xe6\x12\x85\x3f\x58\xc9\x1c\xe1\x03\x96\x58\x95\x9a\xa1\x64\x82\xe1\xb0\xc9\x1a\x22\x76\x79\x02\x4c\xf4\x37\x07\xf8\x22\x2d\xce\x5e\x75\xf6\x62\xe8\xa3\xdf\x47\x0d\x14\xea\x42\xdb\x5b\xde\xf8\x49\x8a\x37\xb2\x37\xfe\xb5\x9a\x02\xa2\x7f\xde\xaa\x27\xc8\x21\xd4\xef\xe7\xae\x6b\xa7\xf5\xa0\xe7\x84\x5a";
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
