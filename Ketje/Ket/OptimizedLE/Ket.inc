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

#define Ket_StateOverwrite                  JOIN(prefix, _StateOverwrite)
#define Ket_Step                            JOIN(prefix, _Step)
#define Ket_FeedAssociatedDataBlocks        JOIN(prefix, _FeedAssociatedDataBlocks)
#define Ket_UnwrapBlocks                    JOIN(prefix, _UnwrapBlocks)
#define Ket_WrapBlocks                      JOIN(prefix, _WrapBlocks)

/* Permutation state management functions   */

void Ket_StateOverwrite( void *stateArg, unsigned int offset, const unsigned char *data, unsigned int length )
{
    unsigned char *state = (unsigned char*)stateArg + offset;

    while ( length-- != 0 )
    {
        *(state++) = *(data++);
    }
}

/* Ketje low level functions    */

void Ket_Step( void *state, unsigned int size, unsigned char framing )
{

    ((unsigned char*)state)[size] ^= framing;
    ((unsigned char*)state)[Ketje_BlockSize] ^= 0x08;
    SnP_PermuteRounds(state, Ket_StepRounds);
}

void Ket_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks )
{
    uint32_t t;

    do
    {
        #if (SnP_width == 400 )
        t = ((uint32_t*)state)[0];
        t ^= (uint32_t)*(data++);
        t ^= (uint32_t)*(data++) << 8;
        t ^= (uint32_t)*(data++) << 16;
        t ^= (uint32_t)*(data++) << 24;
        ((uint32_t*)state)[0] = t;
        ((unsigned char*)state)[Ketje_BlockSize] ^= 0x08 | FRAMEBITS00;
        #else
        t = ((uint32_t*)state)[0];
        t ^= (uint32_t)*(data++);
        t ^= (uint32_t)*(data++) << 8;
        t ^= (uint32_t)(0x08 | FRAMEBITS00) << 16;
        ((uint32_t*)state)[0] = t;
        #endif
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
    while ( --nBlocks != 0 );
}

void Ket_UnwrapBlocks( void *state, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks )
{
    uint32_t t;

    do
    {
        #if (SnP_width == 400 )
        t = ((uint32_t*)state)[0];
        t ^= (uint32_t)*(ciphertext++);
        t ^= (uint32_t)*(ciphertext++) << 8;
        t ^= (uint32_t)*(ciphertext++) << 16;
        t ^= (uint32_t)*(ciphertext++) << 24;
        *(plaintext++) = (unsigned char)t;
        *(plaintext++) = (unsigned char)(t >> 8);
        *(plaintext++) = (unsigned char)(t >> 16);
        *(plaintext++) = (unsigned char)(t >> 24);
        ((uint32_t*)state)[0] ^= t;
        ((unsigned char*)state)[Ketje_BlockSize] ^= 0x08 | FRAMEBITS11;
        #else
        t = ((uint16_t*)state)[0];
        t ^= (uint32_t)*(ciphertext++);
        t ^= (uint32_t)*(ciphertext++) << 8;
        t ^= (uint32_t)(0x08 | FRAMEBITS11) << 16;
        *(plaintext++) = (unsigned char)t;
        *(plaintext++) = (unsigned char)(t >> 8);
        ((uint32_t*)state)[0] ^= t;
        #endif
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
    while ( --nBlocks != 0 );
}

void Ket_WrapBlocks( void *state, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks )
{
    uint32_t t;

    do
    {
        #if (SnP_width == 400 )
        t = ((uint32_t*)state)[0];
        t ^= (uint32_t)*(plaintext++);
        t ^= (uint32_t)*(plaintext++) << 8;
        t ^= (uint32_t)*(plaintext++) << 16;
        t ^= (uint32_t)*(plaintext++) << 24;
        ((uint32_t*)state)[0] = t;
        *(ciphertext++) = (unsigned char)t;
        *(ciphertext++) = (unsigned char)(t >> 8);
        *(ciphertext++) = (unsigned char)(t >> 16);
        *(ciphertext++) = (unsigned char)(t >> 24);
        ((unsigned char*)state)[Ketje_BlockSize] ^= 0x08 | FRAMEBITS11;
        #else
        t = ((uint32_t*)state)[0];
        t ^= (uint32_t)*(plaintext++);
        t ^= (uint32_t)*(plaintext++) << 8;
        t ^= (uint32_t)(0x08 | FRAMEBITS11) << 16;
        ((uint32_t*)state)[0] = t;
        *(ciphertext++) = (unsigned char)t;
        *(ciphertext++) = (unsigned char)(t >> 8);
        #endif
        SnP_PermuteRounds(state, Ket_StepRounds);
    }
    while ( --nBlocks != 0 );
}

#undef  SnP_AddBytes
#undef  SnP_AddByte
#undef  SnP_OverwriteBytes
#undef  SnP_ExtractBytes

#undef Ket_StateOverwrite
#undef Ket_Step
#undef Ket_FeedAssociatedDataBlocks
#undef Ket_UnwrapBlocks
#undef Ket_WrapBlocks
