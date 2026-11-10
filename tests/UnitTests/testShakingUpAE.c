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
#include "UpperDeck.h"
#include "UT.h"
#include <time.h>

static void testDWrap( int turbo, uint32_t c )
{
    unsigned int    rho     = (1600 - c - 64) /8;
    unsigned int    taglen  = c / 8;
    const unsigned char k[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    unsigned char   A[3*200];
    unsigned char   P[3*200];
    unsigned char   C[sizeof(P)+64];
    unsigned char   P2[sizeof(P)];
    uint32_t        seed = 0x192E5EA3;

    UT_generateArbitraryMaterial(A, sizeof(A), seed++);
    UT_generateArbitraryMaterial(P, sizeof(P), seed++);

    KeccakWidth1600_DWrapInstance dedicated;
    if (turbo) {
        if (c == 256)
            UT_assert(TurboSHAKE128_Wrap_Initialize(&dedicated, taglen, k, sizeof(k)) == 0);
        else
            UT_assert(TurboSHAKE256_Wrap_Initialize(&dedicated, taglen, k, sizeof(k)) == 0);
    }
    else {
        if (c == 256)
            UT_assert(SHAKE128_Wrap_Initialize(&dedicated, taglen, k, sizeof(k)) == 0);
        else
            UT_assert(SHAKE256_Wrap_Initialize(&dedicated, taglen, k, sizeof(k)) == 0);
    }
    UT_assert(dedicated.od.rho == rho);
    UT_assert(dedicated.od.c == c);

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
                UT_assert(TurboSHAKE_Wrap_Initialize( &dww, rho, c, taglen, k, sizeof(k) ) == 0);
                UT_assert(TurboSHAKE_Wrap_Wrap( &dww, C, A, Alen, P, Plen ) == 0);
            }
            else {
                UT_assert(SHAKE_Wrap_Initialize( &dww, rho, c, taglen, k, sizeof(k) ) == 0);
                UT_assert(SHAKE_Wrap_Wrap( &dww, C, A, Alen, P, Plen ) == 0);
            }

            KeccakWidth1600_DWrapInstance dwu;
            size_t Clen = Plen + taglen;
            int rv;
            if (turbo) {
                UT_assert(TurboSHAKE_Wrap_Initialize( &dwu, rho, c, taglen, k, sizeof(k) ) == 0);
                rv = TurboSHAKE_Wrap_Unwrap( &dwu, P2, A, Alen, C, Clen );
            }
            else {
                UT_assert(SHAKE_Wrap_Initialize( &dwu, rho, c, taglen, k, sizeof(k) ) == 0);
                rv = SHAKE_Wrap_Unwrap( &dwu, P2, A, Alen, C, Clen );
            }
            UT_assert(rv == 0);
            rv = memcmp( P, P2, Plen );
            UT_assert(rv == 0);
        }
    }
}

static void testUpperDeckInitialize(void)
{
    const unsigned char key[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    KeccakWidth1600_UpperDeckInstance ud;

    UT_startTest("UpperDeck initialization", "");

    UT_assert(SHAKE128_UpperDeck_Initialize(&ud, key, sizeof(key)) == 0);
    UT_assert(ud.D.rho == 160 && ud.D.c == 256);
    UT_assert(SHAKE256_UpperDeck_Initialize(&ud, key, sizeof(key)) == 0);
    UT_assert(ud.D.rho == 128 && ud.D.c == 512);
    UT_assert(TurboSHAKE128_UpperDeck_Initialize(&ud, key, sizeof(key)) == 0);
    UT_assert(ud.D.rho == 160 && ud.D.c == 256);
    UT_assert(TurboSHAKE256_UpperDeck_Initialize(&ud, key, sizeof(key)) == 0);
    UT_assert(ud.D.rho == 128 && ud.D.c == 512);

    UT_endTest();
}

static int initializeDeckBO(KeccakWidth1600_DeckBOInstance *dbo, int turbo, unsigned int s, const unsigned char *key, unsigned int keyByteLen, unsigned int taglen)
{
    if (turbo) {
        if (s == 128)
            return TurboSHAKE128_BO_Initialize(dbo, taglen, key, keyByteLen);
        if (s == 256)
            return TurboSHAKE256_BO_Initialize(dbo, taglen, key, keyByteLen);
    }
    else {
        if (s == 128)
            return SHAKE128_BO_Initialize(dbo, taglen, key, keyByteLen);
        if (s == 256)
            return SHAKE256_BO_Initialize(dbo, taglen, key, keyByteLen);
    }
    return 1;
}

static void testDeckBO( int turbo, unsigned int s )
{
    unsigned int    c       = s*2;
    unsigned int    rho     = (1600 - c - 64) /8;
    unsigned int    taglen  = c / 8;
    const unsigned char k[] = { 11, 12, 13, 14, 15, 16, 17, 18 };
    unsigned char   A[3*200];
    unsigned char   P[3*200];
    unsigned char   C[sizeof(P)+taglen];
    unsigned char   P2[sizeof(P)];
    uint32_t        seed = 0xCDAB471A;

    UT_generateArbitraryMaterial(A, sizeof(A), seed++);
    UT_generateArbitraryMaterial(P, sizeof(P), seed++);

    unsigned int customRho = rho;
    unsigned int customC = c;
    size_t customAlpha = 2 * customRho + 7;
    size_t customAlen = customAlpha + 3;
    size_t customPlen = customRho + 5;
    KeccakWidth1600_DeckBOInstance customWrap;
    KeccakWidth1600_DeckBOInstance customUnwrap;
    int rv;

    if (turbo) {
        UT_assert(TurboSHAKE_BO_Initialize(&customWrap, customRho, customC, customAlpha, taglen, k, sizeof(k)) == 0);
        UT_assert(TurboSHAKE_BO_Wrap(&customWrap, C, A, customAlen, P, customPlen) == 0);
        UT_assert(TurboSHAKE_BO_Initialize(&customUnwrap, customRho, customC, customAlpha, taglen, k, sizeof(k)) == 0);
        rv = TurboSHAKE_BO_Unwrap(&customUnwrap, P2, A, customAlen, C, customPlen + taglen);
    }
    else {
        UT_assert(SHAKE_BO_Initialize(&customWrap, customRho, customC, customAlpha, taglen, k, sizeof(k)) == 0);
        UT_assert(SHAKE_BO_Wrap(&customWrap, C, A, customAlen, P, customPlen) == 0);
        UT_assert(SHAKE_BO_Initialize(&customUnwrap, customRho, customC, customAlpha, taglen, k, sizeof(k)) == 0);
        rv = SHAKE_BO_Unwrap(&customUnwrap, P2, A, customAlen, C, customPlen + taglen);
    }
    UT_assert(customWrap.D.D.rho == customRho);
    UT_assert(customWrap.D.D.c == customC);
    UT_assert(customWrap.D.alpha == customAlpha);
    UT_assert(rv == 0);
    UT_assert(memcmp(P, P2, customPlen) == 0);

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
                UT_assert(initializeDeckBO( &dbow, turbo, s, k, sizeof(k), taglen ) == 0);
                UT_assert(TurboSHAKE_BO_Wrap( &dbow, C, A, Alen, P, Plen ) == 0);
            }
            else {
                UT_assert(initializeDeckBO( &dbow, turbo, s, k, sizeof(k), taglen ) == 0);
                UT_assert(SHAKE_BO_Wrap( &dbow, C, A, Alen, P, Plen ) == 0);
            }

            KeccakWidth1600_DeckBOInstance dbou;
            int rv;
            if (turbo) {
                UT_assert(initializeDeckBO( &dbou, turbo, s, k, sizeof(k), taglen ) == 0);
                rv = TurboSHAKE_BO_Unwrap( &dbou, P2, A, Alen, C, Plen + taglen );
            }
            else {
                UT_assert(initializeDeckBO( &dbou, turbo, s, k, sizeof(k), taglen ) == 0);
                rv = SHAKE_BO_Unwrap( &dbou, P2, A, Alen, C, Plen + taglen );
            }
            UT_assert(rv == 0);
            rv = memcmp( P, P2, Plen );
            UT_assert(rv == 0);
        }
    }
}

static UT_arbitraryChecksum testvectorsDWrap(int turbo, uint32_t c)
{
    const unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const uint32_t  taglen  = 16;
    const uint32_t  rho     = (1600 - c - 64) / 8;
    unsigned char   A[600];
    unsigned char   P[600];
    unsigned char   C[600+taglen];
    uint32_t        seed = 0xA7139EF1;

    UT_arbitraryChecksumState checksum;
    UT_arbitraryChecksumInitialize(&checksum);

    UT_generateArbitraryMaterial(A, sizeof(A), seed++);
    UT_generateArbitraryMaterial(P, sizeof(P), seed++);

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
                UT_assert(TurboSHAKE_Wrap_Initialize( &dww, rho, c, taglen, key, sizeof(key) ) == 0);
                UT_assert(TurboSHAKE_Wrap_Wrap( &dww, C, A, alen, P, plen ) == 0);
            }
            else {
                UT_assert(SHAKE_Wrap_Initialize( &dww, rho, c, taglen, key, sizeof(key) ) == 0);
                UT_assert(SHAKE_Wrap_Wrap( &dww, C, A, alen, P, plen ) == 0);
            }
            UT_arbitraryChecksumAbsorb(&checksum, C, plen+taglen);
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
    return UT_arbitraryChecksumSqueeze(&checksum);
}

void selfTestDWrap(int turbo, uint32_t c, const UT_arbitraryChecksum expected)
{
    char name[100];

    if (turbo)
        sprintf(name, "TurboSHAKE%d-Wrap", c/2);
    else
        sprintf(name, "SHAKE%d-Wrap", c/2);
    UT_startTest(name, "");
    testDWrap(turbo, c);
    UT_arbitraryChecksum checksum = testvectorsDWrap(turbo, c);
    UT_assert(checksum == expected);
    UT_endTest();
}

#ifdef UT_OUTPUT
void writeTestDWrap_One(FILE *f, int turbo, uint32_t c)
{
    unsigned int offset;
    char name[100];

    UT_arbitraryChecksum checksum = testvectorsDWrap(turbo, c);
    fprintf(f, "    selfTestDWrap(%d, %d, 0x%08X);\n", turbo, c, checksum);
}
#endif

static UT_arbitraryChecksum testvectorsDeckBO(int turbo, unsigned int s)
{
    unsigned int    c       = s*2;
    const unsigned char key[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    const uint32_t  taglen  = 16;
    const uint32_t  rho     = (1600 - c - 64) / 8;
    unsigned char   A[600];
    unsigned char   P[600];
    unsigned char   C[600+taglen];
    uint32_t        seed = 0x3DF6BBF4;

    UT_arbitraryChecksumState checksum;
    UT_arbitraryChecksumInitialize(&checksum);

    UT_generateArbitraryMaterial(A, sizeof(A), seed++);
    UT_generateArbitraryMaterial(P, sizeof(P), seed++);

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
                UT_assert(initializeDeckBO( &dbo, turbo, s, key, sizeof(key), taglen ) == 0);
                UT_assert(TurboSHAKE_BO_Wrap( &dbo, C, A, alen, P, plen ) == 0);
            }
            else {
                UT_assert(initializeDeckBO( &dbo, turbo, s, key, sizeof(key), taglen ) == 0);
                UT_assert(SHAKE_BO_Wrap( &dbo, C, A, alen, P, plen ) == 0);
            }
            UT_arbitraryChecksumAbsorb(&checksum, C, plen+taglen);
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
    return UT_arbitraryChecksumSqueeze(&checksum);
}

void selfTestDeckBO(int turbo, unsigned int s, const UT_arbitraryChecksum expected)
{
    char name[100];

    if (turbo)
        sprintf(name, "TurboSHAKE%d-BO", s);
    else
        sprintf(name, "SHAKE%d-BO", s);
    UT_startTest(name, "");
    testDeckBO(turbo, s);
    UT_arbitraryChecksum checksum = testvectorsDeckBO(turbo, s);
    UT_assert(checksum == expected);
    UT_endTest();
}

#ifdef UT_OUTPUT
void writeTestDeckBO_One(FILE *f, int turbo, unsigned int s)
{
    unsigned int offset;
    char name[100];

    UT_arbitraryChecksum checksum = testvectorsDeckBO(turbo, s);
    fprintf(f, "    selfTestDeckBO(%d, %d, 0x%08X);\n", turbo, s, checksum);
}

void writeTests(const char *filename)
{
    int turbo;
    uint32_t c;
    FILE *f = fopen(filename, "w");
    UT_assert(f != NULL);
    for(turbo=0; turbo<=1; turbo++) {
        for(c=256; c<=512; c += 256) {
            writeTestDWrap_One(f, turbo, c);
            writeTestDeckBO_One(f, turbo, c/2);
        }
    }
    fclose(f);
}
#endif

void testShakingUpAE( void )
{
    testUpperDeckInitialize();
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
    selfTestDWrap(0, 256, 0x9FA05B32);
    selfTestDeckBO(0, 128, 0xC70E192F);
    selfTestDWrap(0, 512, 0xF2B99F08);
    selfTestDeckBO(0, 256, 0x857E62E0);
    selfTestDWrap(1, 256, 0x764F9DCD);
    selfTestDeckBO(1, 128, 0x709D96F8);
    selfTestDWrap(1, 512, 0x400D6951);
    selfTestDeckBO(1, 256, 0x3610BD4A);
    #endif
}

#endif /* XKCP_has_ShakingUpAE */
