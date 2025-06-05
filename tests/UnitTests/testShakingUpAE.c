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
// #define UT_OUTPUT

#include "ShakingUpAE.h"
#include "TurboSHAKE.h"
#include "UT.h"
#include <time.h>
#include <assert.h>

#define checksumByteSize 16

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

static void testDWrap( int turbo, uint32_t c )
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

static void testDeckBO( int turbo, uint32_t c )
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

static void generateSimpleRawMaterialTestVec( uint8_t *dest, size_t len, const int seed1, int seed2)
{
    for (size_t i = 0; i < len; i++ )
    {
        seed2 %= 8;
        uint8_t iRolled = ((uint8_t)i << seed2) | ((uint8_t)i >> (8 - seed2));
        dest[i] = seed1 + 161 * (uint8_t)len - iRolled + (uint8_t)i;
    }
}

static void testvectorsDWrap( int turbo, uint32_t c, unsigned char *checksum )
{
    const uint8_t   key[]   = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const uint32_t  taglen  = 16;
    const uint32_t  rho     = (1600 - c - 64) / 8;
    uint8_t         A[600];
    uint8_t         P[600];
    uint8_t         C[600+taglen];

    TurboSHAKE_Instance xof;
    TurboSHAKE128_Initialize(&xof);

    generateSimpleRawMaterialTestVec( A, sizeof(A), 167, 13 );
    generateSimpleRawMaterialTestVec( P, sizeof(P), 223, 83 );

#ifdef UT_VERBOSE
    if (turbo) printf( "Turbo" );
    printf( "SHAKE%u-Wrap\n", c / 2 );
#endif
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
            TurboSHAKE_Absorb(&xof, C, plen+taglen);
#ifdef UT_VERBOSE
            printf( "%u,%u ", alen, plen );
            for ( size_t i = 0; i < (plen+taglen); ++i )
            {
                printf( "%02x ", C[i] );
            }
            printf( "\n" );
#endif
        }
    }
#ifdef UT_VERBOSE
    printf( "\n" );
#endif
    TurboSHAKE_AbsorbDomainSeparationByte(&xof, 1);
    TurboSHAKE_Squeeze(&xof, checksum, checksumByteSize);
}

void selfTestDWrap(int turbo, uint32_t c, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    char name[100];

    if (turbo)
        sprintf(name, "TurboSHAKE%d-Wrap", c/2);
    else
        sprintf(name, "SHAKE%d-Wrap", c/2);
    UT_startTest(name, "");
    testDWrap(turbo, c);
    testvectorsDWrap(turbo, c, checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
    UT_endTest();
}

#ifdef UT_OUTPUT
void writeTestDWrap_One(FILE *f, int turbo, uint32_t c)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;
    char name[100];

    testvectorsDWrap(turbo, c, checksum);
    fprintf(f, "    selfTestDWrap(%d, %d, \"", turbo, c);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}
#endif

static void testvectorsDeckBO( int turbo, uint32_t c, unsigned char *checksum )
{
    const uint8_t   key[]   = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const uint32_t  taglen  = 16;
    const uint32_t  rho     = (1600 - c - 64) / 8;
    uint8_t         A[600];
    uint8_t         P[600];
    uint8_t         C[600+taglen];
    
    TurboSHAKE_Instance xof;
    TurboSHAKE128_Initialize(&xof);

    generateSimpleRawMaterialTestVec( A, sizeof(A), 167, 13 );
    generateSimpleRawMaterialTestVec( P, sizeof(P), 223, 83 );

#ifdef UT_VERBOSE
    if (turbo) printf( "Turbo" );
    printf( "SHAKE%u-BO\n", c / 2 );
#endif
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
            TurboSHAKE_Absorb(&xof, C, plen+taglen);
#ifdef UT_VERBOSE
            printf( "%u,%u ", alen, plen );
            for ( size_t i = 0; i < (plen+taglen); ++i )
            {
                printf( "%02x ", C[i] );
            }
            printf( "\n" );
#endif
        }
    }
#ifdef UT_VERBOSE
    printf( "\n" );
#endif
    TurboSHAKE_AbsorbDomainSeparationByte(&xof, 1);
    TurboSHAKE_Squeeze(&xof, checksum, checksumByteSize);
}

void selfTestDeckBO(int turbo, uint32_t c, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    char name[100];

    if (turbo)
        sprintf(name, "TurboSHAKE%d-BO", c/2);
    else
        sprintf(name, "SHAKE%d-BO", c/2);
    UT_startTest(name, "");
    testDeckBO(turbo, c);
    testvectorsDeckBO(turbo, c, checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
    UT_endTest();
}

#ifdef UT_OUTPUT
void writeTestDeckBO_One(FILE *f, int turbo, uint32_t c)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;
    char name[100];

    testvectorsDeckBO(turbo, c, checksum);
    fprintf(f, "    selfTestDeckBO(%d, %d, \"", turbo, c);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTests(const char *filename)
{
    int turbo;
    uint32_t c;
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    for(turbo=0; turbo<=1; turbo++) {
        for(c=256; c<=512; c += 256) {
            writeTestDWrap_One(f, turbo, c);
            writeTestDeckBO_One(f, turbo, c);
        }
    }
    fclose(f);
}
#endif


void testShakingUpAE( void )
{

    #ifdef UT_VERBOSE
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
    #endif
    #ifdef UT_OUTPUT
    writeTests("ShakingUpAE.txt");
    #else
    selfTestDWrap(0, 256, "\x88\x62\xcf\x2a\xef\x45\x72\x4e\x7d\x9a\x14\x31\xa9\xbe\x2f\xd0");
    selfTestDeckBO(0, 256, "\x55\x7f\x18\xaa\x4b\x32\x32\x43\x60\x3d\x22\x53\xd1\x2f\x90\x1c");
    selfTestDWrap(0, 512, "\x50\x9e\x40\xfe\x83\x94\x57\xa9\xe0\xc6\xc6\xbd\xb0\x59\x44\x0f");
    selfTestDeckBO(0, 512, "\xab\xc2\x51\x52\xa4\x83\x5a\xf1\xb4\x56\x41\x1a\xe9\x20\x98\x2d");
    selfTestDWrap(1, 256, "\xd1\x80\xce\xc0\x30\xcc\xfb\x23\x80\x63\x58\x8d\x31\x9f\xf8\x95");
    selfTestDeckBO(1, 256, "\x6c\x36\x26\x7c\xfe\xbc\x9b\x11\x2b\xcc\x77\xb6\xb9\x35\x14\x06");
    selfTestDWrap(1, 512, "\xa9\x42\xf8\x48\x0d\x89\xf1\x05\x14\xe8\x28\x29\x83\x73\xcd\xba");
    selfTestDeckBO(1, 512, "\x7e\x90\x95\x2e\x86\x53\x6a\x3e\x65\x6c\x72\x2f\xdb\xa4\xd2\x15");
    #endif
}

#endif /* XKCP_has_ShakingUpAE */
