/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "config.h"
#ifdef XKCP_has_ShakingUpAE

#include "ShakingUpAE.h"
#include "UT.h"
#include <time.h>
#include <assert.h>

static void generateSimpleRawMaterial(unsigned char* data, size_t length, unsigned char seed1, unsigned int seed2)
{
    size_t i;

    for(i=0; i<length; i++) {
        unsigned char iRolled;
        unsigned char byte;
        seed2 = seed2 % 8;
        iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        byte = seed1 + 161* (unsigned char)length - iRolled + (unsigned char)i;
        data[i] = byte;
    }
}

static void testDWrap( int turbo )
{

    for ( unsigned int c = 2 * 128; c <= 2 * 256; c <<= 1 )
    {
        unsigned int    rho     = (1600 - c - 64) /8;
        unsigned int    taglen  = c / 8;
        const uint8_t   k[]     = { 1, 2, 3, 4, 5, 6, 7, 8 };
        uint8_t         A[3*200];
        uint8_t         P[3*200];
        uint8_t         C[sizeof(P)+64];
        uint8_t         P2[sizeof(P)];

        generateSimpleRawMaterial( A, sizeof(A), 167, 13 );
        generateSimpleRawMaterial( P, sizeof(P), 223, 83 );

        for ( size_t Alen = 0; Alen <= 3 * rho; ++Alen )
        {
            if ( (Alen % rho) >= 16 && ((Alen % rho) <= (rho - 16)))
                continue;
            for ( size_t Plen = 0; Plen < 3 * rho; ++Plen )
            {
                if ( (Plen % rho) >= 40 && ((Plen % rho) <= (rho - 40)))
                    continue;
                //printf("Alen %u, Plen %u capa %u, rho %u, taglen %u\n", Alen, Plen, c, rho, taglen );
                KeccakWidth1600_DWrapInstance dww;
                if (turbo) {
                    TurboSHAKE_Wrap_Initialize( &dww, k, sizeof(k), taglen, rho, c );
                    TurboSHAKE_Wrap_Wrap( &dww, C, A, Alen, P, Plen );
                }
                else {
                    SHAKE_Wrap_Initialize( &dww, k, sizeof(k), taglen, rho, c );
                    SHAKE_Wrap_Wrap( &dww, C, A, Alen, P, Plen );
                }

                KeccakWidth1600_DWrapInstance dwu;
                size_t Clen = Plen + taglen;
                int rv;
                if (turbo) {
                    TurboSHAKE_Wrap_Initialize( &dwu, k, sizeof(k), taglen, rho, c );
                    rv = TurboSHAKE_Wrap_Unwrap( &dwu, P2, A, Alen, C, Clen );
                }
                else {
                    SHAKE_Wrap_Initialize( &dwu, k, sizeof(k), taglen, rho, c );
                    rv = SHAKE_Wrap_Unwrap( &dwu, P2, A, Alen, C, Clen );
                }
                assert(rv == 0);
                rv = memcmp( P, P2, Plen );
                assert(rv == 0);
            }
        }
    }
}

static void testDeckBO( int turbo )
{
    for ( unsigned int c = 2 * 128; c <= 2 * 256; c <<= 1 )
    {
        unsigned int    rho     = (1600 - c - 64) /8;
        unsigned int    taglen  = c / 8;
        const uint8_t   k[]     = { 11, 12, 13, 14, 15, 16, 17, 18 };
        uint8_t         A[3*200];
        uint8_t         P[3*200];
        uint8_t         C[sizeof(P)+taglen];
        uint8_t         P2[sizeof(P)];

        generateSimpleRawMaterial( A, sizeof(A), 193, 67 );
        generateSimpleRawMaterial( P, sizeof(P), 141, 37 );

        for ( size_t Alen = 0; Alen <= 3 * rho; ++Alen )
        {
            if ( (Alen % rho) >= 16 && ((Alen % rho) <= (rho - 16)))
                continue;
            for ( size_t Plen = 0; Plen < 3 * rho; ++Plen )
            {
                if ( (Plen % rho) >= 40 && ((Plen % rho) <= (rho - 40)))
                    continue;
                //printf("Alen %u, Plen %u capa %u, rho %u, taglen %u\n", Alen, Plen, c, rho, taglen );
                KeccakWidth1600_DeckBOInstance dbow;
                if (turbo) {
                    TurboSHAKE_BO_Initialize( &dbow, k, sizeof(k), taglen, rho, c );
                    TurboSHAKE_BO_Wrap( &dbow, C, A, Alen, P, Plen );
                }
                else {
                    SHAKE_BO_Initialize( &dbow, k, sizeof(k), taglen, rho, c );
                    SHAKE_BO_Wrap( &dbow, C, A, Alen, P, Plen );
                }

                KeccakWidth1600_DeckBOInstance dbou;
                int rv;
                if (turbo) {
                    TurboSHAKE_BO_Initialize( &dbou, k, sizeof(k), taglen, rho, c );
                    rv = TurboSHAKE_BO_Unwrap( &dbou, P2, A, Alen, C, Plen + taglen );
                }
                else {
                    SHAKE_BO_Initialize( &dbou, k, sizeof(k), taglen, rho, c );
                    rv = SHAKE_BO_Unwrap( &dbou, P2, A, Alen, C, Plen + taglen );
                }
                assert(rv == 0);
                memcmp( P, P2, Plen );
                assert(rv == 0);
            }
        }
    }
}

static void generateSimpleRawMaterialTestVec( uint8_t *dest, size_t len, const int seed1, int seed2)
{
    for (size_t i = 0; i < len; i++ )
    {
        seed2 %= 8;
        uint8_t iRolled = ((uint8_t)i << seed2) | ((uint8_t)i >> (8 - seed2));
        dest[i] = seed1 + 161 * (uint8_t)len - iRolled + (uint8_t)i;
    }
}

static void testvectorsDWrap( int turbo, uint32_t c )
{
    const uint8_t   key[]   = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const uint32_t  taglen  = 16;
    const uint32_t  rho     = (1600 - c - 64) / 8;
    uint8_t         A[600];
    uint8_t         P[600];
    uint8_t         C[600+taglen];
    
    generateSimpleRawMaterialTestVec( A, sizeof(A), 167, 13 );
    generateSimpleRawMaterialTestVec( P, sizeof(P), 223, 83 );

    if (turbo) printf( "Turbo" );
    printf( "SHAKE%u-Wrap\n", c / 2 );
    for (size_t alen = 0; alen < sizeof(A); alen += 4)
    {
        for (size_t plen = 0; plen < sizeof(P); plen += 4)
        {
            KeccakWidth1600_DWrapInstance dww;
            if (turbo) {
                TurboSHAKE_Wrap_Initialize( &dww, key, sizeof(key), taglen, rho, c );
                TurboSHAKE_Wrap_Wrap( &dww, C, A, alen, P, plen );
            }
            else {
                SHAKE_Wrap_Initialize( &dww, key, sizeof(key), taglen, rho, c );
                SHAKE_Wrap_Wrap( &dww, C, A, alen, P, plen );
            }
            printf( "%u,%u ", alen, plen );
            for ( size_t i = 0; i < (plen+taglen); ++i )
            {
                printf( "%02x ", C[i] );
            }
            printf( "\n" );
        }
    }
    printf( "\n" );
}

static void testvectorsDeckBO( int turbo, uint32_t c )
{
    const uint8_t   key[]   = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const uint32_t  taglen  = 16;
    const uint32_t  rho     = (1600 - c - 64) / 8;
    uint8_t         A[600];
    uint8_t         P[600];
    uint8_t         C[600+taglen];
    
    generateSimpleRawMaterialTestVec( A, sizeof(A), 167, 13 );
    generateSimpleRawMaterialTestVec( P, sizeof(P), 223, 83 );

    if (turbo) printf( "Turbo" );
    printf( "SHAKE%u-BO\n", c / 2 );
    for (size_t alen = 0; alen < sizeof(A); alen += 4)
    {
        for (size_t plen = 0; plen < sizeof(P); plen += 4)
        {
            KeccakWidth1600_DeckBOInstance dbo;
            if (turbo) {
                TurboSHAKE_BO_Initialize( &dbo, key, sizeof(key), taglen, rho, c );
                TurboSHAKE_BO_Wrap( &dbo, C, A, alen, P, plen );
            }
            else {
                SHAKE_BO_Initialize( &dbo, key, sizeof(key), taglen, rho, c );
                SHAKE_BO_Wrap( &dbo, C, A, alen, P, plen );
            }
            printf( "%u,%u ", alen, plen );
            for ( size_t i = 0; i < (plen+taglen); ++i )
            {
                printf( "%02x ", C[i] );
            }
            printf( "\n" );
        }
    }
    printf( "\n" );
}

void testShakingUpAE( void )
{

    #if 1
    /* SHAKE128-Wrap */
    testvectorsDWrap(  0, 256 );
    /* SHAKE256-Wrap */
    testvectorsDWrap(  0, 512 );
    /* SHAKE128-BO */
    testvectorsDeckBO( 0, 256 );
    /* SHAKE256-BO */
    testvectorsDeckBO( 0, 512 );
    /* TurboSHAKE128-Wrap */
    testvectorsDWrap(  1, 256 );
    /* TurboSHAKE256-Wrap */
    testvectorsDWrap(  1, 512 );
    /* TurboSHAKE128-BO */
    testvectorsDeckBO( 1, 256 );
    /* TurboSHAKE256-BO */
    testvectorsDeckBO( 1, 512 );
    #else
    UT_startTest("Keccak " PT "DWrap", "");
    testDWrap();
    UT_endTest();
    UT_startTest("Keccak " PT "DeckBO", "");
    testDeckBO();
    UT_endTest();
    #endif
}

#endif /* XKCP_has_ShakingUpAE */
