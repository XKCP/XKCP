/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

“Shaking up authenticated encryption”: Keccak-based duplex ciphers, deck ciphers and authenticated encryption schemes designed by Joan Daemen, Seth Hoffert, Silvia Mella, Gilles Van Assche and Ronny Van Keer

Implementation by Ronny Van Keer and Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define OD_Initialize                   JOIN(prefix, _OD_Initialize)
#define OD_Clone                        JOIN(prefix, _OD_Clone)
#define OD_CloneCompact                 JOIN(prefix, _OD_CloneCompact)
#define OD_Duplexing                    JOIN(prefix, _OD_Duplexing)
#define OD_DuplexingFastOnlyOut         JOIN(prefix, _OD_DuplexingFastOnlyOut)
#define OD_DuplexingFastOnlyIn          JOIN(prefix, _OD_DuplexingFastOnlyIn)
#define OD_Squeezing                    JOIN(prefix, _OD_Squeezing)
#define UpperDeck_Initialize            JOIN(prefix, _UpperDeck_Initialize)
#define UpperDeck_Initialize128         JOIN(prefix, 128_UpperDeck_Initialize)
#define UpperDeck_Initialize256         JOIN(prefix, 256_UpperDeck_Initialize)
#define UpperDeck_Clone                 JOIN(prefix, _UpperDeck_Clone)
#define UpperDeck_CloneCompact          JOIN(prefix, _UpperDeck_CloneCompact)
#define UpperDeck_Duplexing             JOIN(prefix, _UpperDeck_Duplexing)
#define UpperDeck_Squeezing             JOIN(prefix, _UpperDeck_Squeezing)

int UpperDeck_Initialize( KeccakWidth1600_UpperDeckInstance *ud, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen )
{
    if ((rho > RHO_LEN_MAX)
        || (keyByteLen > rho))
        return 1;
    if (OD_Initialize( &ud->D, rho, c, key, keyByteLen ) != 0)
        return 1;
    memset( &ud->Dsqueeze, 0u, sizeof(ud->Dsqueeze) );
    ud->o = (size_t)-1;
    return 0;
}

int UpperDeck_Initialize128( KeccakWidth1600_UpperDeckInstance *ud, const unsigned char *key, unsigned int keyByteLen )
{
    return UpperDeck_Initialize(ud, 160, 256, key, keyByteLen);
}

int UpperDeck_Initialize256( KeccakWidth1600_UpperDeckInstance *ud, const unsigned char *key, unsigned int keyByteLen )
{
    return UpperDeck_Initialize(ud, 128, 512, key, keyByteLen);
}

int UpperDeck_Clone( KeccakWidth1600_UpperDeckInstance *udnew, const KeccakWidth1600_UpperDeckInstance *ud )
{
    udnew->o = ud->o;
    if (OD_Clone( &udnew->D,        &ud->D ) != 0)
        return 1;
    if (OD_Clone( &udnew->Dsqueeze, &ud->D ) != 0)
        return 1;
    return 0;
}

int UpperDeck_CloneCompact( KeccakWidth1600_UpperDeckInstance *udnew, const KeccakWidth1600_UpperDeckInstance *ud )
{
    udnew->o = (size_t)-1;
    if (OD_CloneCompact( &udnew->D, &ud->D ) != 0)
        return 1;
    memset( &udnew->Dsqueeze, 0u, sizeof(udnew->Dsqueeze) );
    return 0;
}

int UpperDeck_Duplexing( KeccakWidth1600_UpperDeckInstance *ud, unsigned char *output, size_t outputLen, const unsigned char *input, size_t inputLen, unsigned int E, const unsigned char *toAdd )
{
    if ( inputLen > ud->D.rho ) {
        size_t bytesProcessed = OD_DuplexingFastOnlyIn( &ud->D, input, inputLen, 2u );
        input       += bytesProcessed;
        inputLen    -= bytesProcessed;
        assert( inputLen != 0u );
    }
    assert( inputLen <= ud->D.rho );
    if (OD_Duplexing( &ud->D, NULL, 0, input, inputLen, OD_Concat( E, 1u ), NULL ) != 0)
        return 1;
    ud->o = 0u;
    memset( &ud->Dsqueeze, 0u, sizeof(ud->Dsqueeze) );
    return UpperDeck_Squeezing( ud, output, outputLen, toAdd );
}

int UpperDeck_Squeezing( KeccakWidth1600_UpperDeckInstance *ud, unsigned char *output, size_t outputLen, const unsigned char *toAdd )
{
    if ( ud->o <= ud->D.rho ) {
        unsigned int x = MYMIN( ud->D.rho - ud->o, outputLen );
        if (OD_Squeezing( &ud->D, output, x, toAdd ) != 0)
            return 1;
        if ( (ud->o + outputLen) > ud->D.rho ) {
            if (OD_CloneCompact( &ud->Dsqueeze, &ud->D ) != 0)
                return 1;
        }
        outputLen -= x;
        output    += x;
        if ( toAdd != NULL ) {
            toAdd += x;
        }
        ud->o += x;
    }
    while ( outputLen > 0u ) {
        if ( (ud->o % ud->Dsqueeze.rho) == 0u ) {
            if ( outputLen > ud->Dsqueeze.rho ) {
                size_t bytesProcessed = OD_DuplexingFastOnlyOut( &ud->Dsqueeze, 2u, output, outputLen, toAdd );
                outputLen -= bytesProcessed;
                output    += bytesProcessed;
                if ( toAdd != NULL ) {
                    toAdd += bytesProcessed;
                }
                ud->o += bytesProcessed;
            }
            assert( outputLen <= ud->D.rho );
            if ( outputLen > 0u ) {
                if (OD_Duplexing( &ud->Dsqueeze, output, outputLen, NULL, 0u, 2u, toAdd ) != 0)
                    return 1;
                ud->o += outputLen;
                outputLen = 0;
            }
        }
        else {
            unsigned int x = MYMIN( ud->Dsqueeze.rho - ud->o % ud->Dsqueeze.rho, outputLen );
            if (OD_Squeezing( &ud->Dsqueeze, output, x, toAdd ) != 0)
                return 1;
            outputLen -= x;
            output    += x;
            if ( toAdd != NULL ) {
                toAdd += x;
            }
            ud->o += x;
        }
    }
    return 0;
}

#undef OD_Initialize
#undef OD_Clone
#undef OD_CloneCompact
#undef OD_Duplexing
#undef OD_DuplexingFastOnlyOut
#undef OD_DuplexingFastOnlyIn
#undef OD_Squeezing
#undef UpperDeck_Initialize
#undef UpperDeck_Initialize128
#undef UpperDeck_Initialize256
#undef UpperDeck_Clone
#undef UpperDeck_CloneCompact
#undef UpperDeck_Duplexing
#undef UpperDeck_Squeezing
#undef JOIN
#undef JOIN0
