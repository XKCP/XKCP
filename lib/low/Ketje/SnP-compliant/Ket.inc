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

#define SnP_AddBytes                        JOIN(SnP, _AddBytes)
#define SnP_AddByte                         JOIN(SnP, _AddByte)
#define SnP_OverwriteBytes                  JOIN(SnP, _OverwriteBytes)
#define SnP_ExtractBytes                    JOIN(SnP, _ExtractBytes)
#define SnP_ExtractAndAddBytes              JOIN(SnP, _ExtractAndAddBytes)

#define Ket_StateTwistIndexes               JOIN(prefix, _StateTwistIndexes)
#define Ket_StateAddByte                    JOIN(prefix, _StateAddByte)
#define Ket_StateExtractByte                JOIN(prefix, _StateExtractByte)
#define Ket_StateOverwrite                  JOIN(prefix, _StateOverwrite)
#define Ket_Step                            JOIN(prefix, _Step)
#define Ket_FeedAssociatedDataBlocks        JOIN(prefix, _FeedAssociatedDataBlocks)
#define Ket_UnwrapBlocks                    JOIN(prefix, _UnwrapBlocks)
#define Ket_WrapBlocks                      JOIN(prefix, _WrapBlocks)

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
    SnP_AddByte(state, value, Ket_StateTwistIndexes[offset / Ketje_LaneSize] * Ketje_LaneSize + offset % Ketje_LaneSize);
}

unsigned char Ket_StateExtractByte( void *state, unsigned int offset )
{
    unsigned char data[1];

    SnP_ExtractBytes(state, data, Ket_StateTwistIndexes[offset / Ketje_LaneSize] * Ketje_LaneSize + offset % Ketje_LaneSize, 1);
    return data[0];
}

void Ket_StateOverwrite( void *state, unsigned int offset, const unsigned char *data, unsigned int length )
{
	while ( length-- != 0 )
	{
	    SnP_OverwriteBytes(state, data, Ket_StateTwistIndexes[offset / Ketje_LaneSize] * Ketje_LaneSize + offset % Ketje_LaneSize, 1);
		++data;
		++offset;
	}
}

/* Ketje low level functions    */

void Ket_Step( void *state, unsigned int size, unsigned char frameAndPaddingBits)
{

    SnP_AddByte(state, frameAndPaddingBits, Ket_StateTwistIndexes[size / Ketje_LaneSize] * Ketje_LaneSize + size % Ketje_LaneSize);
    SnP_AddByte(state, 0x08, Ket_StateTwistIndexes[Ketje_BlockSize / Ketje_LaneSize] * Ketje_LaneSize);
    SnP_PermuteRounds(state, Ket_StepRounds);
}

void Ket_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks )
{
	unsigned int laneIndex;

    do
    {
		for ( laneIndex = 0; laneIndex < (Ketje_BlockSize / Ketje_LaneSize); ++laneIndex )
		{
	        SnP_AddBytes( state, data, Ket_StateTwistIndexes[laneIndex] * Ketje_LaneSize, Ketje_LaneSize );
			data += Ketje_LaneSize;
		}
        Ket_Step( state, Ketje_BlockSize, FRAMEBITS00 );
    }
    while ( --nBlocks != 0 );
}

void Ket_UnwrapBlocks( void *state, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks )
{
	unsigned int laneIndex;

    while ( nBlocks-- != 0 )
    {
		for ( laneIndex = 0; laneIndex < (Ketje_BlockSize / Ketje_LaneSize); ++laneIndex )
		{
	        SnP_ExtractAndAddBytes( state, ciphertext, plaintext, Ket_StateTwistIndexes[laneIndex] * Ketje_LaneSize, Ketje_LaneSize );
	        SnP_AddBytes(state, plaintext, Ket_StateTwistIndexes[laneIndex] * Ketje_LaneSize, Ketje_LaneSize);
			plaintext += Ketje_LaneSize;
			ciphertext += Ketje_LaneSize;
		}
        SnP_AddByte(state, 0x08 | FRAMEBITS11, Ket_StateTwistIndexes[Ketje_BlockSize / Ketje_LaneSize] * Ketje_LaneSize);
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
}

void Ket_WrapBlocks( void *state, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks )
{
	unsigned int laneIndex;

    while ( nBlocks-- != 0 )
    {
		for ( laneIndex = 0; laneIndex < (Ketje_BlockSize / Ketje_LaneSize); ++laneIndex )
		{
	        SnP_AddBytes(state, plaintext, Ket_StateTwistIndexes[laneIndex] * Ketje_LaneSize, Ketje_LaneSize);
	        SnP_ExtractBytes( state, ciphertext, Ket_StateTwistIndexes[laneIndex] * Ketje_LaneSize, Ketje_LaneSize );
			plaintext += Ketje_LaneSize;
			ciphertext += Ketje_LaneSize;
		}
        SnP_AddByte(state, 0x08 | FRAMEBITS11, Ket_StateTwistIndexes[Ketje_BlockSize / Ketje_LaneSize] * Ketje_LaneSize);
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
}

#undef SnP_AddBytes
#undef SnP_AddByte
#undef SnP_OverwriteBytes
#undef SnP_ExtractBytes
#undef SnP_ExtractAndAddBytes

#undef Ket_StateExtractByte
#undef Ket_StateOverwrite
#undef Ket_Step
#undef Ket_FeedAssociatedDataBlocks
#undef Ket_UnwrapBlocks
#undef Ket_WrapBlocks
