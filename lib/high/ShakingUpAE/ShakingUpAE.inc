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
#include "xkcp-memcmp.h"

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define OD_Initialize                   JOIN(prefix, _OD_Initialize)
#define OD_Clone                        JOIN(prefix, _OD_Clone)
#define OD_CloneCompact                 JOIN(prefix, _OD_CloneCompact)
#define OD_Duplexing                    JOIN(prefix, _OD_Duplexing)
#define OD_DuplexingFast                JOIN(prefix, _OD_DuplexingFast)
#define OD_DuplexingFastOnlyOut         JOIN(prefix, _OD_DuplexingFastOnlyOut)
#define OD_DuplexingFastOnlyIn          JOIN(prefix, _OD_DuplexingFastOnlyIn)
#define OD_Squeezing                    JOIN(prefix, _OD_Squeezing)
#define DWrap_Initialize                JOIN(prefix, _Wrap_Initialize)
#define DWrap_Initialize128             JOIN(prefix, 128_Wrap_Initialize)
#define DWrap_Initialize256             JOIN(prefix, 256_Wrap_Initialize)
#define DWrap_Clone                     JOIN(prefix, _Wrap_Clone)
#define DWrap_Wrap                      JOIN(prefix, _Wrap_Wrap)
#define DWrap_Unwrap                    JOIN(prefix, _Wrap_Unwrap)
#define FlightDeck_Initialize           JOIN(prefix, _FlightDeck_Initialize)
#define FlightDeck_Clone                JOIN(prefix, _FlightDeck_Clone)
#define FlightDeck_CloneCompact         JOIN(prefix, _FlightDeck_CloneCompact)
#define FlightDeck_Duplexing            JOIN(prefix, _FlightDeck_Duplexing)
#define FlightDeck_Squeezing            JOIN(prefix, _FlightDeck_Squeezing)
#define DeckBO_Initialize               JOIN(prefix, _BO_Initialize)
#define DeckBO_Initialize128            JOIN(prefix, 128_BO_Initialize)
#define DeckBO_Initialize256            JOIN(prefix, 256_BO_Initialize)
#define DeckBO_Clone                    JOIN(prefix, _BO_Clone)
#define DeckBO_Wrap                     JOIN(prefix, _BO_Wrap)
#define DeckBO_Unwrap                   JOIN(prefix, _BO_Unwrap)

// DWrap ----------------------------------------------------------------------

int DWrap_Initialize( KeccakWidth1600_DWrapInstance *D, unsigned int rho, unsigned int c, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen )
{
    if (   (taglen > TAG_LEN_MAX)
        || (rho * 8u + c + 64u != 1600u)
        || (rho > RHO_LEN_MAX)
        || (keyByteLen > rho))
        return 1;

    D->taglen = taglen;
    return OD_Initialize( &D->od, rho, c, key, keyByteLen );
}

int DWrap_Initialize128( KeccakWidth1600_DWrapInstance *D, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen )
{
    return DWrap_Initialize(D, 160, 256, taglen, key, keyByteLen);
}

int DWrap_Initialize256( KeccakWidth1600_DWrapInstance *D, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen )
{
    return DWrap_Initialize(D, 128, 512, taglen, key, keyByteLen);
}

int DWrap_Clone( KeccakWidth1600_DWrapInstance *Dnew, const KeccakWidth1600_DWrapInstance *D )
{
    *Dnew = *D;
    return 0;
}

int DWrap_Wrap( KeccakWidth1600_DWrapInstance *D, unsigned char *C, const unsigned char *A, size_t Alen, const unsigned char *P, size_t Plen )
{
    unsigned int    blocklen;
    unsigned int    lastBlockLen;

    // All A blocks except last one
    if ( Alen > D->od.rho ) {
        size_t bytesProcessed = OD_DuplexingFastOnlyIn( &D->od, A, Alen, 5u );
        A       += bytesProcessed;
        Alen    -= bytesProcessed;
        assert( Alen != 0u );
    }

    if ( Plen > 0u ) {
        if ( Alen > 0u ) {
            // Last A block, start wrap
            blocklen = MYMIN( Plen, D->od.rho );
            OD_Duplexing( &D->od, C, blocklen, A, Alen, 5u, P );
        }
        else {
            // Wrap first P block
            blocklen = MYMIN( Plen, D->od.rho - D->taglen );
            OD_Squeezing( &D->od, C, blocklen, P );
        }
        C += blocklen;
        P += blocklen;
        Plen -= blocklen;
        lastBlockLen = blocklen;

        // Second block with input length smaller than rho
        if ( (Plen > 0) && (lastBlockLen < D->od.rho) ) {
            blocklen = MYMIN( Plen, D->od.rho );
            OD_Duplexing( &D->od, C, blocklen, C - lastBlockLen, lastBlockLen, 4u, P );
            Plen -= blocklen;
            C    += blocklen;
            P    += blocklen;
            lastBlockLen = blocklen;
        }

        // Do all blocks with full rho
        if ( Plen >= D->od.rho )
        {
            assert( lastBlockLen == D->od.rho );
            size_t bytesProcessed = OD_DuplexingFast( &D->od, C - lastBlockLen, Plen, 4u, C, P );
            C += bytesProcessed;
            P += bytesProcessed;
            Plen -= bytesProcessed;
        }

        // Remaining block, if length not a multiple of rho
        assert( Plen < D->od.rho );
        if ( Plen > 0u ) {
            OD_Duplexing( &D->od, C, Plen, C - lastBlockLen, lastBlockLen, 4u, P );
            C += Plen;
            lastBlockLen = Plen;
        }

        // Generate tag
        OD_Duplexing( &D->od, C, D->taglen, C - lastBlockLen, lastBlockLen, 6u, NULL );
    }
    else {
        // No plaintext, last A block, generate tag in C
        OD_Duplexing( &D->od, C, D->taglen, A, Alen, 7u, NULL );
    }
    return 0;
}

int DWrap_Unwrap( KeccakWidth1600_DWrapInstance *D, unsigned char *P, const unsigned char *A, size_t Alen, const unsigned char *C, size_t Clen )
{
    KeccakWidth1600_DWrapInstance   Dprime;
    unsigned int                    blocklen;
    unsigned int                    lastBlockLen;
    ALIGN(8) unsigned char          tag[TAG_LEN_MAX];
    unsigned char                   *Pinitial   = P;
    const unsigned char             *Ctag;
    size_t                          ClenInitial;

    if ( Clen < D->taglen ) {
        return -1;
    }
    ClenInitial = Clen -= D->taglen; // Remove tag length from C
    Ctag = C + Clen;
    DWrap_Clone( &Dprime, D );

    // All A blocks except last one
    if ( Alen > D->od.rho ) {
        size_t bytesProcessed = OD_DuplexingFastOnlyIn( &D->od, A, Alen, 5u );
        A       += bytesProcessed;
        Alen    -= bytesProcessed;
        assert( Alen != 0u );
    }
    if ( Clen > 0u ) {
        if ( Alen > 0u ) {
            // Last A block, start unwrap
            blocklen = MYMIN( Clen, D->od.rho );
            OD_Duplexing( &D->od, P, blocklen, A, Alen, 5u, C );
        }
        else {
           // Unwrap first C block
            blocklen = MYMIN( Clen, D->od.rho - D->taglen );
            OD_Squeezing( &D->od, P, blocklen, C );
        }
        C += blocklen;
        P += blocklen;
        Clen -= blocklen;
        lastBlockLen = blocklen;
        // Second block with input length smaller than rho
        if ( (Clen > 0) && (lastBlockLen < D->od.rho) ) {
            unsigned int   blklen  = MYMIN( Clen, D->od.rho );
            OD_Duplexing( &D->od, P, blklen, C - lastBlockLen, lastBlockLen, 4u, C );
            Clen -= blklen;
            C   += blklen;
            P   += blklen;
            lastBlockLen = blklen;
        }
        // All blocks with full rho
        if ( Clen >= D->od.rho ) {
            assert( lastBlockLen == D->od.rho );
            size_t bytesProcessed = OD_DuplexingFast( &D->od, C - lastBlockLen, Clen, 4u, P, C );
            Clen -= bytesProcessed;
            C   += bytesProcessed;
            P   += bytesProcessed;
        }
        // Remaining block, if length not a multiple of rho
        assert( Clen < D->od.rho );
        if ( Clen > 0 ) {
            OD_Duplexing( &D->od, P, Clen, C - lastBlockLen, lastBlockLen, 4u, C );
            C += Clen;
            lastBlockLen = Clen;
        }
        // Generate tag
        OD_Duplexing( &D->od, tag, D->taglen, C - lastBlockLen, lastBlockLen, 6u, NULL );
    }
    else {
        // Last A block, generate tag
        OD_Duplexing( &D->od, tag, D->taglen, A, Alen, 7u, NULL );
    }
    if ( XKCP_memcmp( tag, Ctag, D->taglen ) == 0 ) {
        return 0;
    }
    // Wrong tag
    memset( Pinitial, 0, ClenInitial ); // wipe P
    DWrap_Clone( D, &Dprime );          // Restore initial state
    return -1;
}

// DeckBO ---------------------------------------------------------------------

int DeckBO_Initialize( KeccakWidth1600_DeckBOInstance *dbo, unsigned int rho, unsigned int c, size_t alpha, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen )
{
    if (taglen > TAG_LEN_MAX)
        return 1;
    if (FlightDeck_Initialize( &dbo->D, rho, c, alpha, key, keyByteLen ) != 0)
        return 1;
    dbo->taglen = taglen;
    return 0;
}

int DeckBO_Initialize128( KeccakWidth1600_DeckBOInstance *dbo, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen )
{
    return DeckBO_Initialize(dbo, 160, 256, 5120, taglen, key, keyByteLen);
}

int DeckBO_Initialize256( KeccakWidth1600_DeckBOInstance *dbo, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen )
{
    return DeckBO_Initialize(dbo, 128, 512, 8192, taglen, key, keyByteLen);
}

int DeckBO_Clone( KeccakWidth1600_DeckBOInstance *dbonew, const KeccakWidth1600_DeckBOInstance *dbo )
{
    *dbonew = *dbo;
    return 0;
}

int DeckBO_Wrap( KeccakWidth1600_DeckBOInstance *dbo, unsigned char *C, const unsigned char *A, size_t Alen, const unsigned char *P, size_t Plen )
{
    if ( Plen == 0 ) {
        return FlightDeck_Duplexing( &dbo->D, C, dbo->taglen, A, Alen, 4u, NULL );
    }
    if ( Alen != 0 ) {
        if (FlightDeck_Duplexing( &dbo->D, NULL, 0, A, Alen, 5u, NULL ) != 0)
            return 1;
    }
    KeccakWidth1600_FlightDeckInstance Dkey;
    if (FlightDeck_CloneCompact( &Dkey, &dbo->D ) != 0)
        return 1;
    if (FlightDeck_Duplexing( &dbo->D, C + Plen, dbo->taglen, P, Plen, 14u, NULL ) != 0)
        return 1;
    return FlightDeck_Duplexing( &Dkey, C, Plen, C + Plen, dbo->taglen, 13u, P );
}

int DeckBO_Unwrap( KeccakWidth1600_DeckBOInstance *dbo, unsigned char *P, const unsigned char *A, size_t Alen, const unsigned char *C, size_t Clen )
{
    KeccakWidth1600_FlightDeckInstance  udSaved;
    ALIGN(8) unsigned char      tagPrime[TAG_LEN_MAX];

    if ( Clen < dbo->taglen ) {
        return -1;
    }
    if (FlightDeck_Clone( &udSaved, &dbo->D ) != 0)
        return 1;
    if ( Clen == dbo->taglen ) {
        if (FlightDeck_Duplexing( &dbo->D, tagPrime, dbo->taglen, A, Alen, 4u, NULL ) != 0)
            return 1;
        if ( XKCP_memcmp( tagPrime, C, dbo->taglen ) != 0 ) {
            FlightDeck_Clone( &dbo->D, &udSaved );
            return -1;
        }
    }
    else {
        Clen -= dbo->taglen;
        if ( Alen != 0 ) {
            if (FlightDeck_Duplexing( &dbo->D, NULL, 0u, A, Alen, 5u, NULL ) != 0)
                return 1;
        }
        KeccakWidth1600_FlightDeckInstance Dkey;
        if (FlightDeck_Clone( &Dkey, &dbo->D ) != 0)
            return 1;
        if (FlightDeck_Duplexing( &Dkey, P, Clen, C + Clen, dbo->taglen, 13u, C ) != 0)
            return 1;
        if (FlightDeck_Duplexing( &dbo->D, tagPrime, dbo->taglen, P, Clen, 14u, NULL ) != 0)
            return 1;
        if ( XKCP_memcmp( tagPrime, C + Clen, dbo->taglen ) != 0 ) {
            memset( P, 0, Clen ); // Wipe plaintext
            FlightDeck_Clone( &dbo->D, &udSaved );
            return -1;
        }
    }
    return 0;
}

#undef OD_Initialize
#undef OD_Clone
#undef OD_CloneCompact
#undef OD_Duplexing
#undef OD_DuplexingFast
#undef OD_DuplexingFastOnlyOut
#undef OD_DuplexingFastOnlyIn
#undef OD_Squeezing
#undef DWrap_Initialize
#undef DWrap_Initialize128
#undef DWrap_Initialize256
#undef DWrap_Clone
#undef DWrap_Wrap
#undef DWrap_Unwrap
#undef FlightDeck_Initialize
#undef FlightDeck_Clone
#undef FlightDeck_CloneCompact
#undef FlightDeck_Duplexing
#undef FlightDeck_Squeezing
#undef DeckBO_Initialize
#undef DeckBO_Initialize128
#undef DeckBO_Initialize256
#undef DeckBO_Clone
#undef DeckBO_Wrap
#undef DeckBO_Unwrap
