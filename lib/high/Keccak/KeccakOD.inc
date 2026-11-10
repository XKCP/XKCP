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

#include "align.h"
#include "load-store.h"

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define OD_TrailencAndXor               JOIN(prefix, _OD_TrailencAndXor)
#define OD_Initialize                   JOIN(prefix, _OD_Initialize)
#define OD_Clone                        JOIN(prefix, _OD_Clone)
#define OD_CloneCompact                 JOIN(prefix, _OD_CloneCompact)
#define OD_Duplexing                    JOIN(prefix, _OD_Duplexing)
#define OD_DuplexingFast                JOIN(prefix, _OD_DuplexingFast)
#define OD_DuplexingFastOnlyOut         JOIN(prefix, _OD_DuplexingFastOnlyOut)
#define OD_DuplexingFastOnlyIn          JOIN(prefix, _OD_DuplexingFastOnlyIn)
#define OD_Squeezing                    JOIN(prefix, _OD_Squeezing)

// OD -------------------------------------------------------------------------

static void OD_TrailencAndXor( KeccakWidth1600_ODInstance *od, unsigned char D )
{
    ALIGN(8) const unsigned char trailenc[8] = { D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 + DOM_SEP };
    SnP_AddBytes( &od->s, trailenc, od->rho, sizeof(trailenc) );
}

int OD_Initialize( KeccakWidth1600_ODInstance *od, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen )
{

    od->o       = rho;
    od->c       = c;
    od->rho     = rho;
    SnP_StaticInitialize();
    SnP_Initialize( &od->s );
    OD_Duplexing( od, NULL, 0, key, keyByteLen, 1, NULL );
    return 0;
}

int OD_Clone( KeccakWidth1600_ODInstance *odnew, const KeccakWidth1600_ODInstance *od )
{
    *odnew = *od;
    return 0;
}

int OD_CloneCompact( KeccakWidth1600_ODInstance *odnew, const KeccakWidth1600_ODInstance *od )
{
    *odnew = *od;
    odnew->o = odnew->rho;
    SnP_OverwriteWithZeroes( &odnew->s, odnew->rho );
    return 0;
}

size_t OD_DuplexingFast( KeccakWidth1600_ODInstance *od, const unsigned char *input, size_t len, unsigned int E, unsigned char *output, const unsigned char *toAdd )
{
    if ((E < 1u) || (E > 63u)
        || (od->rho == 0u)
        || (len < od->rho)
        || (toAdd == NULL))
        return 0;

    ALIGN(8) const unsigned char trailenc[8] = { OD_Concat( E, 1u ), 0u, 0u, 0u, 0u, 0u, 0u, 0x80u + DOM_SEP };
    unsigned int            r               = od->rho;

    od->o = r;

    if (SnP_GetFeatures() & SnP_Feature_OD)
        return SnP_ODDuplexingFastInOut(&od->s, r/8, input, len, output, toAdd, XKCP_load64(trailenc));

    size_t  initialLen = len;
    do {
        SnP_OverwriteBytes( &od->s, input, 0, r );
        SnP_AddBytes( &od->s, trailenc, r, sizeof(trailenc) );
        SnP_Permute( &od->s );
        SnP_ExtractAndAddBytes( &od->s, toAdd, output, 0, r );
        toAdd += r;
        output += r;
        input += r;
        len -= r;
    }
    while ( len >= od->rho );

    return initialLen - len; // bytes processed
}

size_t OD_DuplexingFastOnlyOut( KeccakWidth1600_ODInstance *od, unsigned int E, unsigned char *output, size_t outputLen, const unsigned char *toAdd )
{
    if ((E < 1u) || (E > 63u)
        || (od->rho == 0u)
        || (outputLen < od->rho)
        || (toAdd == NULL))
        return 0;

    ALIGN(8) const unsigned char trailenc[8] = { OD_Concat( E, 0u ), 0u, 0u, 0u, 0u, 0u, 0u, 0x80u + DOM_SEP };
    unsigned int            r           = od->rho;

    od->o = r;

    if (SnP_GetFeatures() & SnP_Feature_OD)
        return SnP_ODDuplexingFastOut( &od->s, r/8, output, outputLen, toAdd, XKCP_load64(trailenc) );

    size_t  initialLen = outputLen;
    do {
        SnP_OverwriteWithZeroes( &od->s, r );
        SnP_AddByte( &od->s, 1, 0 );
        SnP_AddBytes( &od->s, trailenc, r, sizeof(trailenc) );
        SnP_Permute( &od->s );
        SnP_ExtractAndAddBytes( &od->s, toAdd, output, 0, r );
        toAdd += r;
        output += r;
        outputLen -= r;
    }
    while ( outputLen >= od->rho );

    return initialLen - outputLen; // bytes processed
}

size_t OD_DuplexingFastOnlyIn( KeccakWidth1600_ODInstance *od, const unsigned char *input, size_t inputLen, unsigned int E )
{
    if ((E < 1u) || (E > 63u)
        || (od->rho == 0u)
        || (inputLen < od->rho))
        return 0;

    ALIGN(8) const unsigned char trailenc[8] = { OD_Concat( E, 1u ), 0u, 0u, 0u, 0u, 0u, 0u, 0x80u + DOM_SEP };
    unsigned int            r           = od->rho;

    od->o = 0;

    if (SnP_GetFeatures() & SnP_Feature_OD)
        return SnP_ODDuplexingFastIn( &od->s, r/8, input, inputLen, XKCP_load64(trailenc) );

    size_t initialLen = inputLen;
    do {
        SnP_OverwriteBytes( &od->s, input, 0, r );
        SnP_AddBytes( &od->s, trailenc, r, sizeof(trailenc) );
        SnP_Permute( &od->s );
        input += r;
        inputLen -= r;
    }
    while ( inputLen > od->rho );

    return initialLen - inputLen; // bytes processed
}

int OD_Duplexing( KeccakWidth1600_ODInstance *od, unsigned char *output, unsigned int outputLen, const unsigned char *input, unsigned int inputLen, unsigned int E, const unsigned char *toAdd )
{
    if ((E < 1u) || (E > 63u)
        || (od->rho == 0u)
        || (inputLen > od->rho)
        || (outputLen > od->rho))
        return 1;

    if ( inputLen == od->rho ) {
        SnP_OverwriteBytes( &od->s, input, 0u, inputLen );
        OD_TrailencAndXor( od, OD_Concat( E, 1u ) );
    }
    else {
        SnP_OverwriteWithZeroes( &od->s, od->rho );
        SnP_OverwriteBytes( &od->s, input, 0u, inputLen );
        SnP_AddByte( &od->s, 1u, inputLen );
        OD_TrailencAndXor( od, OD_Concat( E, 0u ) );
    }
    SnP_Permute( &od->s );
    if ( toAdd != NULL ) {
        SnP_ExtractAndAddBytes( &od->s, toAdd, output, 0u, outputLen );
    }
    else {
        SnP_ExtractBytes( &od->s, output, 0u, outputLen );
    }
    od->o = outputLen;
    return 0;
}

int OD_Squeezing( KeccakWidth1600_ODInstance *od, unsigned char *output, unsigned int outputLen, const unsigned char *toAdd )
{
    if ((od->rho == 0u)
        || (outputLen + od->o > od->rho))
        return 1;

    if ( toAdd != NULL ) {
        SnP_ExtractAndAddBytes( &od->s, toAdd, output, od->o, outputLen );
    }
    else {
        SnP_ExtractBytes( &od->s, output, od->o, outputLen );
    }
    od->o += outputLen;
    return 0;
}

#undef OD_TrailencAndXor
#undef OD_Initialize
#undef OD_Clone
#undef OD_CloneCompact
#undef OD_Duplexing
#undef OD_DuplexingFast
#undef OD_DuplexingFastOnlyOut
#undef OD_DuplexingFastOnlyIn
#undef OD_Squeezing
