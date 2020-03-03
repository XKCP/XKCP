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

    SnP_StaticInitialize();

    SnP_Initialize(stateAccumulated);

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

#ifdef SnP_Permute_Nrounds
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

#ifndef SnP_NoFastLoopAbsorb
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
