/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define SnP_StaticInitialize                JOIN(SnP, _StaticInitialize)
#define SnP_Initialize                      JOIN(SnP, _Initialize)

#define Ketje_Instance                      JOIN(prefix, _Instance)
#define Ketje_Initialize                    JOIN(prefix, _Initialize)
#define Ketje_FeedAssociatedData            JOIN(prefix, _FeedAssociatedData)
#define Ketje_WrapPlaintext                 JOIN(prefix, _WrapPlaintext)
#define Ketje_UnwrapCiphertext              JOIN(prefix, _UnwrapCiphertext)
#define Ketje_GetTag                        JOIN(prefix, _GetTag)

#define Ket_StateAddByte                    JOIN(prefixKet, _StateAddByte)
#define Ket_StateExtractByte                JOIN(prefixKet, _StateExtractByte)
#define Ket_StateOverwrite                  JOIN(prefixKet, _StateOverwrite)
#define Ket_Step                            JOIN(prefixKet, _Step)
#define Ket_FeedAssociatedDataBlocks        JOIN(prefixKet, _FeedAssociatedDataBlocks)
#define Ket_UnwrapBlocks                    JOIN(prefixKet, _UnwrapBlocks)
#define Ket_WrapBlocks                      JOIN(prefixKet, _WrapBlocks)

int Ketje_Initialize(Ketje_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce, unsigned int nonceSizeInBits)
{
    unsigned char smallData[1];
    unsigned int keyPackSizeInBits;

    keyPackSizeInBits = 8*((keySizeInBits+16)/8);
    if ( (keyPackSizeInBits + nonceSizeInBits + 2) > SnP_width)
        return 1;

    instance->phase = Ketje_Phase_FeedingAssociatedData;
    instance->dataRemainderSize = 0;

    SnP_StaticInitialize();
    SnP_Initialize(instance->state);

    /* Key pack */
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

    /* Nonce */
    Ket_StateOverwrite( instance->state, 1+keySizeInBits/8+1, nonce, nonceSizeInBits / 8 );
    if ((nonceSizeInBits % 8) == 0)
        smallData[0] = 0x01;
    else {
        unsigned char padding = (unsigned char)1 << (nonceSizeInBits%8);
        unsigned char mask = padding-1;
        smallData[0] = (nonce[nonceSizeInBits/8] & mask) | padding;
    }
    Ket_StateOverwrite( instance->state, 1+keySizeInBits/8+1+nonceSizeInBits/8, smallData, 1 );

    Ket_StateAddByte(instance->state, 0x80, SnP_width / 8 - 1 );
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
                Ket_StateAddByte( instance->state, *(data++), instance->dataRemainderSize++ );
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
        Ket_StateAddByte( instance->state, *(data++), instance->dataRemainderSize++ );
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
        /* More than a block */
        if (instance->dataRemainderSize != 0)
        {
            /* Process data remainder */
            while ( instance->dataRemainderSize < Ketje_BlockSize )
            {
                temp = *(plaintext++);
                *(ciphertext++) = temp ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
                Ket_StateAddByte( instance->state, temp, instance->dataRemainderSize++ );
                --dataSizeInBytes;
            }
            Ket_Step( instance->state, Ketje_BlockSize, FRAMEBITS11 );
            instance->dataRemainderSize = 0;
        }

        /*  Wrap multiple blocks except last. */
        if ( dataSizeInBytes > Ketje_BlockSize )
        {
            size = ((dataSizeInBytes + (Ketje_BlockSize - 1)) & ~(Ketje_BlockSize - 1)) - Ketje_BlockSize;
            Ket_WrapBlocks( instance->state, plaintext, ciphertext, size / Ketje_BlockSize );
            dataSizeInBytes -= size;
            plaintext += size;
            ciphertext += size;
        }
    }

    /*  Add remaining data */
    while ( dataSizeInBytes-- != 0 )
    {
        temp = *(plaintext++);
        *(ciphertext++) = temp ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
        Ket_StateAddByte( instance->state, temp, instance->dataRemainderSize++ );
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
        /* More than a block */
        if (instance->dataRemainderSize != 0)
        {
            /* Process data remainder */
            while ( instance->dataRemainderSize < Ketje_BlockSize )
            {
                temp = *(ciphertext++) ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
                *(plaintext++) = temp;
                Ket_StateAddByte( instance->state, temp, instance->dataRemainderSize++ );
                --dataSizeInBytes;
            }
            Ket_Step( instance->state, Ketje_BlockSize, FRAMEBITS11 );
            instance->dataRemainderSize = 0;
        }

        /*  Unwrap multiple blocks except last. */
        if ( dataSizeInBytes > Ketje_BlockSize )
        {
            size = ((dataSizeInBytes + (Ketje_BlockSize - 1)) & ~(Ketje_BlockSize - 1)) - Ketje_BlockSize;
            Ket_UnwrapBlocks( instance->state, ciphertext, plaintext, size / Ketje_BlockSize );
            dataSizeInBytes -= size;
            plaintext += size;
            ciphertext += size;
        }
    }

    /*  Add remaining data */
    while ( dataSizeInBytes-- != 0 )
    {
        temp = *(ciphertext++) ^ Ket_StateExtractByte( instance->state, instance->dataRemainderSize );
        *(plaintext++) = temp;
        Ket_StateAddByte( instance->state, temp, instance->dataRemainderSize++ );
    }

    return 0;
}

int Ketje_GetTag(Ketje_Instance *instance, unsigned char *tag, unsigned int tagSizeInBytes)
{
    unsigned int tagSizePart;
    unsigned int i;

    if ((instance->phase & (Ketje_Phase_Wrapping | Ketje_Phase_Unwrapping)) == 0)
        return 1;

    Ket_StateAddByte(instance->state, FRAMEBITS10, instance->dataRemainderSize);
    Ket_StateAddByte(instance->state, 0x08, Ketje_BlockSize);    /* padding */
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

#undef  SnP_StaticInitialize
#undef  SnP_Initialize

#undef Ketje_Instance
#undef Ketje_Initialize
#undef Ketje_FeedAssociatedData
#undef Ketje_WrapPlaintext
#undef Ketje_UnwrapCiphertext
#undef Ketje_GetTag

#undef Ket_StateAddByte
#undef Ket_StateExtractByte
#undef Ket_StateOverwrite
#undef Ket_Step
#undef Ket_FeedAssociatedDataBlocks
#undef Ket_UnwrapBlocks
#undef Ket_WrapBlocks
