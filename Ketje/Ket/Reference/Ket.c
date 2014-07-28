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

#include "Ketje.h"
#include "Ket.h"

#define	Ket_Minimum( a, b ) (((a) < (b)) ? (a) : (b))

/* Permutation state management functions	*/

unsigned char Ket_StateExtractByte( void *state, unsigned int offset )
{
	unsigned char data[1];

	SnP_ExtractBytes(state, data, offset, 1);
	return data[0];
}

void Ket_StateOverwrite( void *state, unsigned int offset, const unsigned char *data, unsigned int length )
{
    SnP_OverwriteBytes(state, data, offset, length);
}

void Ket_StateXORByte( void *state, unsigned int offset, unsigned char data )
{
   unsigned char localData[1];

   localData[0] = data;
   SnP_XORBytes(state, localData, offset, 1 );
}

/* Ketje low level functions	*/

void Ket_Step( void *state, unsigned int size, unsigned char frameAndPaddingBits)
{

	Ket_StateXORByte(state, size, frameAndPaddingBits);
	Ket_StateXORByte(state, Ketje_BlockSize, 0x08 );
    SnP_PermuteRounds(state, Ket_StepRounds );
}

void Ket_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks )
{

	do
	{
		Ket_StateXORByte( state, 0, *(data++) );
		Ket_StateXORByte( state, 1, *(data++) );
		#if (KeccakF_width == 400 )
		Ket_StateXORByte( state, 2, *(data++) );
		Ket_StateXORByte( state, 3, *(data++) );
		#endif
		Ket_Step( state, Ketje_BlockSize, FRAMEBITS00 ); 
	}
	while ( --nBlocks != 0 );
}

void Ket_UnwrapBlocks( void *state, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks )
{
	unsigned char tempBlock[Ketje_BlockSize];
    unsigned char frameAndPaddingBits[1];
    frameAndPaddingBits[0] = 0x08 | FRAMEBITS11;
	
	while ( nBlocks-- != 0 ) 
	{
		SnP_ExtractBytes(state, tempBlock, 0, Ketje_BlockSize);
		tempBlock[0] = *(plaintext++) = *(ciphertext++) ^ tempBlock[0];
		tempBlock[1] = *(plaintext++) = *(ciphertext++) ^ tempBlock[1];
		#if (KeccakF_width == 400 )
		tempBlock[2] = *(plaintext++) = *(ciphertext++) ^ tempBlock[2];
		tempBlock[3] = *(plaintext++) = *(ciphertext++) ^ tempBlock[3];
		#endif
		SnP_XORBytes(state, tempBlock, 0, Ketje_BlockSize);
		SnP_XORBytes(state, frameAndPaddingBits, Ketje_BlockSize, 1);
		SnP_PermuteRounds(state, Ket_StepRounds);
	}
}

void Ket_WrapBlocks( void *state, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks )
{
	unsigned char keystream[Ketje_BlockSize];
	unsigned char plaintemp[Ketje_BlockSize];
    unsigned char frameAndPaddingBits[1];
    frameAndPaddingBits[0] = 0x08 | FRAMEBITS11;

	while ( nBlocks-- != 0 ) 
	{
		SnP_ExtractBytes(state, keystream, 0, Ketje_BlockSize);
		plaintemp[0] = plaintext[0];
		plaintemp[1] = plaintext[1];
		#if (KeccakF_width == 400 )
		plaintemp[2] = plaintext[2];
		plaintemp[3] = plaintext[3];
		#endif
		*(ciphertext++) = *(plaintext++) ^ keystream[0];
		*(ciphertext++) = *(plaintext++) ^ keystream[1];
		#if (KeccakF_width == 400 )
		*(ciphertext++) = *(plaintext++) ^ keystream[2];
		*(ciphertext++) = *(plaintext++) ^ keystream[3];
		#endif
		SnP_XORBytes(state, plaintemp, 0, Ketje_BlockSize);
		SnP_XORBytes(state, frameAndPaddingBits, Ketje_BlockSize, 1);
		SnP_PermuteRounds(state, Ket_StepRounds);
	}
}
