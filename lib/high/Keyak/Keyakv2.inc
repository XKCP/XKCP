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

#define Keyak_Instance                  JOIN(prefix, Keyak_Instance)
#define Keyak_Initialize                JOIN(prefix, Keyak_Initialize)
#define Keyak_Wrap                      JOIN(prefix, Keyak_Wrap)
#define Keyak_WrapPartial               JOIN(prefix, Keyak_WrapPartial)
#define Lk                              JOIN(prefix, Keyak_Lk)
#define MaxNoncelength                  JOIN(prefix, Keyak_MaxNoncelength)

#define Motorist_Instance               JOIN(prefixMotorist, _Motorist_Instance)
#define Motorist_Initialize             JOIN(prefixMotorist, _Motorist_Initialize)
#define Motorist_StartEngine            JOIN(prefixMotorist, _Motorist_StartEngine)
#define Motorist_Wrap                   JOIN(prefixMotorist, _Motorist_Wrap)


int Keyak_Initialize(Keyak_Instance *instance, 
    const unsigned char *key, unsigned int keySizeInBytes, 
    const unsigned char *nonce, unsigned int nonceSizeInBytes,
    int tagFlag, unsigned char * tag, 
    int unwrapFlag, int forgetFlag)
{
    unsigned char suv[Lk+MaxNoncelength];

    if ( (keySizeInBytes > (Lk-2)) || ((keySizeInBytes + nonceSizeInBytes) > (Lk-2+MaxNoncelength)) )
        return ( -1 );
    if ( Motorist_Initialize( &instance->motorist ) != 0 )
        return ( -1 );

    /*  Compose SUV */
    suv[0] = Lk;
    memcpy( &suv[1], key, keySizeInBytes );
    suv[1+keySizeInBytes] = 1;
    memset( &suv[1+keySizeInBytes+1], 0, Lk - (1+keySizeInBytes+1) );
    memcpy( &suv[Lk], nonce, nonceSizeInBytes );

    return ( Motorist_StartEngine( &instance->motorist, suv, Lk + nonceSizeInBytes, tagFlag, tag, unwrapFlag, forgetFlag) );
}

int Keyak_Wrap(Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes,
        const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag )
{
    size_t  processedI, processedMD;

    return ( Motorist_Wrap( &instance->motorist, input, dataSizeInBytes, output, AD, ADlen, tag, unwrapFlag, forgetFlag, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD ) );
}

int Keyak_WrapPartial( Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes,
        const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag,
        int lastFlags, size_t *processedIlen, size_t *processedAlen)
{
    return ( Motorist_Wrap( &instance->motorist, input, dataSizeInBytes, output, AD, ADlen, tag, unwrapFlag, forgetFlag, lastFlags, processedIlen, processedAlen ) );
}

#undef Keyak_Instance
#undef Keyak_Initialize
#undef Keyak_Wrap
#undef Keyak_WrapPartial
#undef Lk
#undef MaxNoncelength

#undef Motorist_Instance
#undef Motorist_Initialize
#undef Motorist_StartEngine
#undef Motorist_Wrap
