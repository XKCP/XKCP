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

int Ketje_Initialize(Ketje_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce, unsigned int nonceSizeInBits)
{
	unsigned char smallData[2];
    unsigned int keyPackSizeInBits;

    keyPackSizeInBits = 8*((keySizeInBits+16)/8);
    if ( (keyPackSizeInBits + nonceSizeInBits + 2) > KeccakF_width)
		return 1;

	instance->phase = Ketje_Phase_FeedingAssociatedData;
	instance->dataRemainderSize = 0;

    SnP_StaticInitialize();
    SnP_Initialize(instance->state);

    // Key pack
    smallData[0] = keySizeInBits / 8 + 2;
	Ket_StateOverwrite( instance->state, 0, smallData, 1 );
	Ket_StateOverwrite( instance->state, 1, key, keySizeInBits/8 );
    if ((keySizeInBits % 8) == 0)
        smallData[0] = 0x01;
    else {
        unsigned char padding = (unsigned char)1 << (keySizeInBits%8);
        unsigned char mask = padding-1;
        smallData[0] = (key[keySizeInBits/8] & mask) | padding;
    }
	Ket_StateOverwrite( instance->state, 1+keySizeInBits/8, smallData, 1 );

    // Nonce
	Ket_StateOverwrite( instance->state, 1+keySizeInBits/8+1, nonce, nonceSizeInBits / 8 );
    if ((nonceSizeInBits % 8) == 0)
        smallData[0] = 0x01;
    else {
        unsigned char padding = (unsigned char)1 << (nonceSizeInBits%8);
        unsigned char mask = padding-1;
        smallData[0] = (nonce[nonceSizeInBits/8] & mask) | padding;
    }
	Ket_StateOverwrite( instance->state, 1+keySizeInBits/8+1+nonceSizeInBits/8, smallData, 1 );

	Ket_StateXORByte(instance->state, KeccakF_width / 8 - 1, 0x80 );
    SnP_PermuteRounds(instance->state, Ket_StartRounds );

    return 0;
}

int Ketje_FeedAssociatedData(Ketje_Instance *instance, const unsigned char *data, unsigned int dataSizeInBytes)
{
	unsigned int size;

    if ((instance->phase & Ketje_Phase_FeedingAssociatedData) == 0)
        return 1;

	if ( (instance->dataRemainderSize + dataSizeInBytes) > Ketje_BlockSize )
	{
		if (instance->dataRemainderSize != 0)
		{
			dataSizeInBytes -= Ketje_BlockSize - instance->dataRemainderSize;
			while ( instance->dataRemainderSize != Ketje_BlockSize )
				Ket_StateXORByte( instance->state, instance->dataRemainderSize++, *(data++) );
			Ket_Step( instance->state, Ketje_BlockSize, FRAMEBITS00 );
			instance->dataRemainderSize = 0;
		}

		if ( dataSizeInBytes > Ketje_BlockSize )
		{
			size = ((dataSizeInBytes + (Ketje_BlockSize - 1)) & ~(Ketje_BlockSize - 1)) - Ketje_BlockSize;
			Ket_FeedAssociatedDataBlocks( instance->state, data, size / Ketje_BlockSize);
			dataSizeInBytes -= size;
			data += size;
		}
	}

	while ( dataSizeInBytes-- != 0 )
		Ket_StateXORByte( instance->state, instance->dataRemainderSize++, *(data++) );
    return 0;
}

int Ketje_WrapPlaintext(Ketje_Instance *instance, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int dataSizeInBytes )
{
	unsigned int size;
	unsigned char temp;

	if ( (instance->phase & Ketje_Phase_FeedingAssociatedData) != 0)
	{
		Ket_Step( instance->state, instance->dataRemainderSize, FRAMEBITS01 );
		instance->dataRemainderSize = 0;
		instance->phase = Ketje_Phase_Wrapping;
	}

	if ( (instance->phase & Ketje_Phase_Wrapping) == 0)
		return 1;

	if ( (instance->dataRemainderSize + dataSizeInBytes) > Ketje_BlockSize )
	{
		// More than a block
		if (instance->dataRemainderSize != 0)
		{
			// Process data remainder
			while ( instance->dataRemainderSize < Ketje_BlockSize )
			{
				temp = *(plaintext++);
				*(ciphertext++) = temp ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
				Ket_StateXORByte( instance->state, instance->dataRemainderSize++, temp );
				--dataSizeInBytes;
			}
			Ket_Step( instance->state, Ketje_BlockSize, FRAMEBITS11 );
			instance->dataRemainderSize = 0;
		}

		//	Wrap multiple blocks except last.
		if ( dataSizeInBytes > Ketje_BlockSize )
		{
			size = ((dataSizeInBytes + (Ketje_BlockSize - 1)) & ~(Ketje_BlockSize - 1)) - Ketje_BlockSize;
			Ket_WrapBlocks( instance->state, plaintext, ciphertext, size / Ketje_BlockSize );
			dataSizeInBytes -= size;
			plaintext += size;
			ciphertext += size;
		}
	}

	//	XOR and buffer remaining data
	while ( dataSizeInBytes-- != 0 )
	{
		temp = *(plaintext++);
		*(ciphertext++) = temp ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
		Ket_StateXORByte( instance->state, instance->dataRemainderSize++, temp );
	}

    return 0;
}

int Ketje_UnwrapCiphertext(Ketje_Instance *instance, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int dataSizeInBytes)
{
	unsigned int size;
	unsigned char temp;

	if ( (instance->phase & Ketje_Phase_FeedingAssociatedData) != 0)
	{
		Ket_Step( instance->state, instance->dataRemainderSize, FRAMEBITS01 );
		instance->dataRemainderSize = 0;
		instance->phase = Ketje_Phase_Unwrapping;
	}

	if ( (instance->phase & Ketje_Phase_Unwrapping) == 0)
		return 1;

	if ( (instance->dataRemainderSize + dataSizeInBytes) > Ketje_BlockSize )
	{
		// More than a block
		if (instance->dataRemainderSize != 0)
		{
			// Process data remainder
			while ( instance->dataRemainderSize < Ketje_BlockSize )
			{
				temp = *(ciphertext++) ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
				*(plaintext++) = temp;
				Ket_StateXORByte( instance->state, instance->dataRemainderSize++, temp );
				--dataSizeInBytes;
			}
			Ket_Step( instance->state, Ketje_BlockSize, FRAMEBITS11 );
			instance->dataRemainderSize = 0;
		}

		//	Unwrap multiple blocks except last.
		if ( dataSizeInBytes > Ketje_BlockSize )
		{
			size = ((dataSizeInBytes + (Ketje_BlockSize - 1)) & ~(Ketje_BlockSize - 1)) - Ketje_BlockSize;
			Ket_UnwrapBlocks( instance->state, ciphertext, plaintext, size / Ketje_BlockSize );
			dataSizeInBytes -= size;
			plaintext += size;
			ciphertext += size;
		}
	}

	//	XOR and buffer remaining data
	while ( dataSizeInBytes-- != 0 )
	{
		temp = *(ciphertext++) ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
		*(plaintext++) = temp;
		Ket_StateXORByte( instance->state, instance->dataRemainderSize++, temp );
	}

    return 0;
}

int Ketje_GetTag(Ketje_Instance *instance, unsigned char *tag, unsigned int tagSizeInBytes)
{
	unsigned int tagSizePart;
	unsigned int i;

	if ((instance->phase & (Ketje_Phase_Wrapping | Ketje_Phase_Unwrapping)) == 0)
        return 1;

	Ket_StateXORByte(instance->state, instance->dataRemainderSize, FRAMEBITS10);
	Ket_StateXORByte(instance->state, Ketje_BlockSize, 0x08);	//padding
    SnP_PermuteRounds(instance->state, Ket_StrideRounds );
	instance->dataRemainderSize = 0;
	tagSizePart = Ketje_BlockSize;
	if ( tagSizeInBytes < Ketje_BlockSize )
		tagSizePart = tagSizeInBytes;
	for ( i = 0; i < tagSizePart; ++i )
		*(tag++) = Ket_StateExtractByte( instance->state, i );
	tagSizeInBytes -= tagSizePart;

    while(tagSizeInBytes > 0)
	{
		Ket_Step( instance->state, 0, FRAMEBITS0 );
		tagSizePart = Ketje_BlockSize;
		if ( tagSizeInBytes < Ketje_BlockSize )
			tagSizePart = tagSizeInBytes;
		for ( i = 0; i < tagSizePart; ++i )
			*(tag++) = Ket_StateExtractByte( instance->state, i );
		tagSizeInBytes -= tagSizePart;
    }

    instance->phase = Ketje_Phase_FeedingAssociatedData;

    return 0;
}
