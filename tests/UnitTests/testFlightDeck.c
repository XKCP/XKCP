/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "config.h"
// #define UT_OUTPUT

#include "FlightDeck.h"
#include "UT.h"
#include <inttypes.h>
#include <time.h>
#include <assert.h>

static int is_all_zeroes(const void *buf, size_t len)
{
    const unsigned char *p = (const unsigned char *)buf;

    for (size_t i = 0; i < len; ++i)
        if (p[i] != 0)
            return 0;
    return 1;
}

#ifdef XKCP_has_FlightDeck_Keccak_width1600
    #define inst KeccakWidth1600

        #define prefix SHAKE
        #include "testFlightDeck.inc"
        #undef prefix

        #define prefix TurboSHAKE
        #include "testFlightDeck.inc"
        #undef prefix

    #undef inst
#endif

#ifdef UT_OUTPUT
void writeTestFlightDeck(void)
{
    FILE *f;
    unsigned int rate;

    f = fopen("TestFlightDeck.txt", "w");
    assert(f != NULL);
    #ifdef XKCP_has_FlightDeck_Keccak_width1600
    SHAKE_writeTestFlightDeck(f, 128);
    SHAKE_writeTestFlightDeck(f, 256);
    TurboSHAKE_writeTestFlightDeck(f, 128);
    TurboSHAKE_writeTestFlightDeck(f, 256);
    #endif
    fclose(f);
}

static void displayByteStringWithTrailer(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length, unsigned char E, unsigned int maxLength)
{
    unsigned int i;

    fprintf(f, "%s (%d) ", synopsis, length);
    if (maxLength != 0) {
        for(i=0; i<length && i<maxLength; i++)
            fprintf(f, "%02x ", (unsigned int)data[i]);
        if (length > maxLength)
            fprintf(f, "... ");
    }
    else {
        for(i=0; i<length; i++)
            fprintf(f, "%02x ", (unsigned int)data[i]);
    }
    if (E > 0)
        fprintf(f, "--%02x \n", (unsigned int)E);
    else
        fprintf(f, "\n");
    fflush(f);
}

static void writeTestVector(uint32_t seed, KeccakWidth1600_FlightDeckInstance *fd, size_t ilen, size_t olen)
{
    unsigned char input[156000];
    unsigned char output[4000];
    unsigned char E = ((ilen + olen) % 31) + 1;
    UT_generateArbitraryMaterial(input, ilen, seed);
    displayByteStringWithTrailer(stdout, "←", input, ilen, E, 16);
    SHAKE_FlightDeck_Duplexing(fd, output, olen, input, ilen, E, 0);
    displayByteStringWithTrailer(stdout, "→", output, olen, 0, 0);
}

static void writeTestVectors(unsigned int s, size_t imaximum, size_t omaximum)
{
    KeccakWidth1600_FlightDeckInstance fd;
    uint32_t seed = 0x23A5BA9F;
    unsigned char key[16];
    UT_generateArbitraryMaterial(key, 16, seed++);

    if (s == 128)
        SHAKE128_FlightDeck_Initialize(&fd, key, 16);
    else
        SHAKE256_FlightDeck_Initialize(&fd, key, 16);
    displayByteStringWithTrailer(stdout, "Key:", key, 16, 0, 0);
    for(size_t ilen = 0; ilen <= imaximum; ilen = (ilen < 200) ? ilen+1 : ((ilen < 1000) ? ilen+17 : ((ilen < 10000) ? ilen+97 : ilen+1333)))
    {
        const size_t olen = 8;
        writeTestVector(seed++, &fd, ilen, olen);
    }
    for(size_t olen = 0; olen <= omaximum; olen = (olen < 200) ? olen+1 : ((olen < 1000) ? olen+19 : olen+87))
    {
        const size_t ilen = 0;
        writeTestVector(seed++, &fd, ilen, olen);
    }
}

static void writeTestVectorsAlpha(unsigned int s, unsigned int alpha)
{
    unsigned int rho = (1600 - 2*s - 64)/8;
    KeccakWidth1600_FlightDeckInstance fd;
    unsigned char key[16];
    UT_generateArbitraryMaterial(key, 16, 0x12345678);

    if (s == 128)
        SHAKE128_FlightDeck_Initialize(&fd, key, 16);
    else
        SHAKE256_FlightDeck_Initialize(&fd, key, 16);
    for(int i =  1; i < 20; i++)
        for(int j = -1; j <= 1; j++)
            writeTestVector(0x12345678, &fd, i*alpha + j, i*rho + j);
}
#endif

void testFlightDeck()
{
    #ifdef UT_OUTPUT
    writeTestFlightDeck();
    if (0) {
        printf("SHAKE128-FlightDeck\n");
        writeTestVectors(128, 155000, 4000);
        printf("SHAKE256-FlightDeck\n");
        writeTestVectors(256, 155000, 4000);
    }
    if (0) {
        writeTestVectorsAlpha(128, 5120);
        writeTestVectorsAlpha(256, 8192);
    }
    #endif

    #ifdef XKCP_has_OD_Keccak_width1600
    UT_startTest("(Turbo)SHAKE-FlightDeck", "");
    SHAKE_selfTestFlightDeck(128, 0x5fffde4d);
    SHAKE_selfTestFlightDeck(256, 0x87f80d7c);
    TurboSHAKE_selfTestFlightDeck(128, 0xb458b993);
    TurboSHAKE_selfTestFlightDeck(256, 0x7812c1cf);
    UT_endTest();
    #endif
}
