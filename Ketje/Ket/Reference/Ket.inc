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

#define JOIN0(a, b)                         a ## b
#define JOIN(a, b)                          JOIN0(a, b)

#define SnP_AddBytes                        JOIN(SnP, _AddBytes)
#define SnP_AddByte                         JOIN(SnP, _AddByte)
#define SnP_OverwriteBytes                  JOIN(SnP, _OverwriteBytes)
#define SnP_ExtractBytes                    JOIN(SnP, _ExtractBytes)

#define Ket_StateExtractByte                JOIN(prefix, _StateExtractByte)
#define Ket_StateOverwrite                  JOIN(prefix, _StateOverwrite)
#define Ket_Step                            JOIN(prefix, _Step)
#define Ket_FeedAssociatedDataBlocks        JOIN(prefix, _FeedAssociatedDataBlocks)
#define Ket_UnwrapBlocks                    JOIN(prefix, _UnwrapBlocks)
#define Ket_WrapBlocks                      JOIN(prefix, _WrapBlocks)

/* Permutation state management functions   */

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

/* Ketje low level functions    */

void Ket_Step( void *state, unsigned int size, unsigned char frameAndPaddingBits)
{

    SnP_AddByte(state, frameAndPaddingBits, size);
    SnP_AddByte(state, 0x08, Ketje_BlockSize );
    SnP_PermuteRounds(state, Ket_StepRounds );
}

void Ket_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks )
{

    do
    {
        SnP_AddByte( state, *(data++), 0 );
        SnP_AddByte( state, *(data++), 1 );
        #if (SnP_width == 400 )
        SnP_AddByte( state, *(data++), 2 );
        SnP_AddByte( state, *(data++), 3 );
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
        #if (SnP_width == 400 )
        tempBlock[2] = *(plaintext++) = *(ciphertext++) ^ tempBlock[2];
        tempBlock[3] = *(plaintext++) = *(ciphertext++) ^ tempBlock[3];
        #endif
        SnP_AddBytes(state, tempBlock, 0, Ketje_BlockSize);
        SnP_AddBytes(state, frameAndPaddingBits, Ketje_BlockSize, 1);
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
        #if (SnP_width == 400 )
        plaintemp[2] = plaintext[2];
        plaintemp[3] = plaintext[3];
        #endif
        *(ciphertext++) = *(plaintext++) ^ keystream[0];
        *(ciphertext++) = *(plaintext++) ^ keystream[1];
        #if (SnP_width == 400 )
        *(ciphertext++) = *(plaintext++) ^ keystream[2];
        *(ciphertext++) = *(plaintext++) ^ keystream[3];
        #endif
        SnP_AddBytes(state, plaintemp, 0, Ketje_BlockSize);
        SnP_AddBytes(state, frameAndPaddingBits, Ketje_BlockSize, 1);
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
}

#undef  SnP_AddBytes
#undef  SnP_AddByte
#undef  SnP_OverwriteBytes
#undef  SnP_ExtractBytes

#undef Ket_StateExtractByte
#undef Ket_StateOverwrite
#undef Ket_Step
#undef Ket_FeedAssociatedDataBlocks
#undef Ket_UnwrapBlocks
#undef Ket_WrapBlocks
