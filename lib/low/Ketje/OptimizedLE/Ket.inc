/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                         a ## b
#define JOIN(a, b)                          JOIN0(a, b)

#define Ket_StateTwistIndexes               JOIN(prefix, _StateTwistIndexes)
#define Ket_StateAddByte                    JOIN(prefix, _StateAddByte)
#define Ket_StateExtractByte                JOIN(prefix, _StateExtractByte)
#define Ket_StateOverwrite                  JOIN(prefix, _StateOverwrite)
#define Ket_Step                            JOIN(prefix, _Step)
#define Ket_FeedAssociatedDataBlocks        JOIN(prefix, _FeedAssociatedDataBlocks)
#define Ket_UnwrapBlocks                    JOIN(prefix, _UnwrapBlocks)
#define Ket_WrapBlocks                      JOIN(prefix, _WrapBlocks)

#define Ket_StateTwistIndexBlock0			0
#define Ket_StateTwistIndexBlock1			6
#if (SnP_width <= 400 )
#define Ket_StateTwistIndexBlockSize		12
#else
#define Ket_StateTwistIndexBlock2			12
#define Ket_StateTwistIndexBlock3			18
#define Ket_StateTwistIndexBlockSize		24
#endif

const unsigned char Ket_StateTwistIndexes[] = {  
    0,  6, 12, 18, 24,
    3,  9, 10, 16, 22,
    1,  7, 13, 19, 20,
    4,  5, 11, 17, 23,
    2,  8, 14, 15, 21
};

/* Permutation state management functions   */

void Ket_StateAddByte( void *state, unsigned char value, unsigned int offset )
{
    ((unsigned char *)state)[Ket_StateTwistIndexes[offset / Ketje_LaneSize] * Ketje_LaneSize + offset % Ketje_LaneSize] ^= value;
}

unsigned char Ket_StateExtractByte( void *state, unsigned int offset )
{
    return ((unsigned char *)state)[Ket_StateTwistIndexes[offset / Ketje_LaneSize] * Ketje_LaneSize + offset % Ketje_LaneSize];
}

void Ket_StateOverwrite( void *stateArg, unsigned int offset, const unsigned char *data, unsigned int length )
{
    unsigned char *state = (unsigned char*)stateArg;

    while ( length-- != 0 )
    {
        state[Ket_StateTwistIndexes[offset / Ketje_LaneSize] * Ketje_LaneSize + offset % Ketje_LaneSize] = *(data++);
		offset++;
    }
}

/* Ketje low level functions    */

void Ket_Step( void *state, unsigned int size, unsigned char framing )
{

    ((unsigned char*)state)[Ket_StateTwistIndexes[size / Ketje_LaneSize] * Ketje_LaneSize + size % Ketje_LaneSize] ^= framing;
    ((tKeccakLane*)state)[Ket_StateTwistIndexBlockSize] ^= 0x08;
    SnP_PermuteRounds(state, Ket_StepRounds);
}

void Ket_FeedAssociatedDataBlocks( void *stateArg, const unsigned char *data, unsigned int nBlocks )
{
	tKeccakLane *state = (tKeccakLane*)stateArg;

    do
    {
		state[Ket_StateTwistIndexBlock0] ^= ReadUnalignedLane( data + 0 * sizeof(tKeccakLane) );
		state[Ket_StateTwistIndexBlock1] ^= ReadUnalignedLane( data + 1 * sizeof(tKeccakLane) );
        #if (SnP_width >= 800 )
		state[Ket_StateTwistIndexBlock2] ^= ReadUnalignedLane( data + 2 * sizeof(tKeccakLane) );
		state[Ket_StateTwistIndexBlock3] ^= ReadUnalignedLane( data + 3 * sizeof(tKeccakLane) );
		#endif
		data += Ketje_BlockSize;
        state[Ket_StateTwistIndexBlockSize] ^= 0x08 | FRAMEBITS00;
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
    while ( --nBlocks != 0 );
}

void Ket_UnwrapBlocks( void *stateArg, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks )
{
	tKeccakLane *state = (tKeccakLane*)stateArg;
    tKeccakLane t;

    do
    {
		t = ReadUnalignedLane( ciphertext + 0 * sizeof(tKeccakLane) );
		WriteUnalignedLane( plaintext + 0 * sizeof(tKeccakLane), state[Ket_StateTwistIndexBlock0] ^ t );
		state[Ket_StateTwistIndexBlock0] = t;
		t = ReadUnalignedLane( ciphertext + 1 * sizeof(tKeccakLane) );
		WriteUnalignedLane( plaintext + 1 * sizeof(tKeccakLane), state[Ket_StateTwistIndexBlock1] ^ t );
		state[Ket_StateTwistIndexBlock1] = t;

        #if (SnP_width >= 800 )
		t = ReadUnalignedLane( ciphertext + 2 * sizeof(tKeccakLane) );
		WriteUnalignedLane( plaintext + 2 * sizeof(tKeccakLane), state[Ket_StateTwistIndexBlock2] ^ t );
		state[Ket_StateTwistIndexBlock2] = t;
		t = ReadUnalignedLane( ciphertext + 3 * sizeof(tKeccakLane) );
		WriteUnalignedLane( plaintext + 3 * sizeof(tKeccakLane), state[Ket_StateTwistIndexBlock3] ^ t );
		state[Ket_StateTwistIndexBlock3] = t;
		#endif

		ciphertext += Ketje_BlockSize;
		plaintext += Ketje_BlockSize;
        state[Ket_StateTwistIndexBlockSize] ^= 0x08 | FRAMEBITS11;
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
    while ( --nBlocks != 0 );
}

void Ket_WrapBlocks( void *stateArg, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks )
{
	tKeccakLane *state = (tKeccakLane*)stateArg;
    tKeccakLane t;

    do
    {
		t = state[Ket_StateTwistIndexBlock0] ^ ReadUnalignedLane( plaintext + 0 * sizeof(tKeccakLane) );
		state[Ket_StateTwistIndexBlock0] = t;
		WriteUnalignedLane( ciphertext + 0 * sizeof(tKeccakLane), t );
		t = state[Ket_StateTwistIndexBlock1] ^ ReadUnalignedLane( plaintext + 1 * sizeof(tKeccakLane) );
		state[Ket_StateTwistIndexBlock1] = t;
		WriteUnalignedLane( ciphertext + 1 * sizeof(tKeccakLane), t );

        #if (SnP_width >= 800 )
		t = state[Ket_StateTwistIndexBlock2] ^ ReadUnalignedLane( plaintext + 2 * sizeof(tKeccakLane) );
		state[Ket_StateTwistIndexBlock2] = t;
		WriteUnalignedLane( ciphertext + 2 * sizeof(tKeccakLane), t );
		t = state[Ket_StateTwistIndexBlock3] ^ ReadUnalignedLane( plaintext + 3 * sizeof(tKeccakLane) );
		state[Ket_StateTwistIndexBlock3] = t;
		WriteUnalignedLane( ciphertext + 3 * sizeof(tKeccakLane), t );
		#endif

		ciphertext += Ketje_BlockSize;
		plaintext += Ketje_BlockSize;
        state[Ket_StateTwistIndexBlockSize] ^= 0x08 | FRAMEBITS11;
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
    while ( --nBlocks != 0 );
}

#undef Ket_StateTwistIndexes
#undef Ket_StateAddByte
#undef Ket_StateExtractByte
#undef Ket_StateOverwrite
#undef Ket_Step
#undef Ket_FeedAssociatedDataBlocks
#undef Ket_UnwrapBlocks
#undef Ket_WrapBlocks

#undef Ket_StateTwistIndexBlock0
#undef Ket_StateTwistIndexBlock1
#undef Ket_StateTwistIndexBlock2
#undef Ket_StateTwistIndexBlock3
#undef Ket_StateTwistIndexBlockSize
