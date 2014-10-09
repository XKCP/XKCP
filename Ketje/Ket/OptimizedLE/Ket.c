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

/* Permutation state management functions	*/

void Ket_StateOverwrite( void *stateArg, unsigned int offset, const unsigned char *data, unsigned int length )
{
	unsigned char *state = (unsigned char*)stateArg + offset;

	while ( length-- != 0 )
	{
		*(state++) = *(data++);
	}
}

/* Ketje low level functions	*/

void Ket_Step( void *state, unsigned int size, unsigned char framing )
{

	((unsigned char*)state)[size] ^= framing;
	((unsigned char*)state)[Ketje_BlockSize] ^= 0x08;
    SnP_PermuteRounds(state, Ket_StepRounds);
}

void Ket_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks )
{
	unsigned long t;

	do
	{
		#if (KeccakF_width == 400 )
		t = ((unsigned long*)state)[0];
		t ^= *(data++);
		t ^= *(data++) << 8;
		t ^= *(data++) << 16;
		t ^= *(data++) << 24;
		((unsigned long*)state)[0] = t;
		((unsigned char*)state)[Ketje_BlockSize] ^= 0x08 | FRAMEBITS00;
		#else
		t = ((unsigned long*)state)[0];
		t ^= *(data++);
		t ^= *(data++) << 8;
		t ^= (0x08 | FRAMEBITS00) << 16;
		((unsigned long*)state)[0] = t;
		#endif
		SnP_PermuteRounds(state, Ket_StepRounds);
	}
	while ( --nBlocks != 0 );
}

void Ket_UnwrapBlocks( void *state, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks )
{
	unsigned long t;

	do
	{
		#if (KeccakF_width == 400 )
		t = ((unsigned long*)state)[0];
		t ^= *(ciphertext++);
		t ^= *(ciphertext++) << 8;
		t ^= *(ciphertext++) << 16;
		t ^= *(ciphertext++) << 24;
		*(plaintext++) = (unsigned char)t;
		*(plaintext++) = (unsigned char)(t >> 8);
		*(plaintext++) = (unsigned char)(t >> 16);
		*(plaintext++) = (unsigned char)(t >> 24);
		((unsigned long*)state)[0] ^= t;
		((unsigned char*)state)[Ketje_BlockSize] ^= 0x08 | FRAMEBITS11;
		#else
		t = ((unsigned short*)state)[0];
		t ^= *(ciphertext++);
		t ^= *(ciphertext++) << 8;
		t ^= (0x08 | FRAMEBITS11) << 16;
		*(plaintext++) = (unsigned char)t;
		*(plaintext++) = (unsigned char)(t >> 8);
		((unsigned long*)state)[0] ^= t;
		#endif
		SnP_PermuteRounds(state, Ket_StepRounds);
	}
	while ( --nBlocks != 0 );
}

void Ket_WrapBlocks( void *state, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks )
{
	unsigned long t;

	do
	{
		#if (KeccakF_width == 400 )
		t = ((unsigned long*)state)[0];
		t ^= *(plaintext++);
		t ^= *(plaintext++) << 8;
		t ^= *(plaintext++) << 16;
		t ^= *(plaintext++) << 24;
		((unsigned long*)state)[0] = t;
		*(ciphertext++) = (unsigned char)t;
		*(ciphertext++) = (unsigned char)(t >> 8);
		*(ciphertext++) = (unsigned char)(t >> 16);
		*(ciphertext++) = (unsigned char)(t >> 24);
		((unsigned char*)state)[Ketje_BlockSize] ^= 0x08 | FRAMEBITS11;
		#else
		t = ((unsigned long*)state)[0];
		t ^= *(plaintext++);
		t ^= *(plaintext++) << 8;
		t ^= (0x08 | FRAMEBITS11) << 16;
		((unsigned long*)state)[0] = t;
		*(ciphertext++) = (unsigned char)t;
		*(ciphertext++) = (unsigned char)(t >> 8);
		#endif
		SnP_PermuteRounds(state, Ket_StepRounds);
	}
	while ( --nBlocks != 0 );
}

