/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <stdlib.h>
#include <string.h>
#include "UT.h"

void UT_startTest(const char *synopsis, const char *implementation)
{
#if !defined(UT_EMBEDDED)
    if (strlen(implementation) == 0)
        printf("* %s\n", synopsis);
    else
        printf("* %s: %s\n", synopsis, implementation);
    fflush(stdout);
#endif
}

void UT_endTest()
{
#if !defined(UT_EMBEDDED)
    printf("    - OK\n");
    fflush(stdout);
#endif
}

void UT_displayInfo(const char *header, const char *contents)
{
#if !defined(UT_EMBEDDED)
    printf("  %s: %s\n", header, contents);
    fflush(stdout);
#endif
}

#ifndef UT_EMBEDDED
void UT_displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif

void UT_assertSyn(int condition, char * synopsis)
{
    if (!condition)
    {
        #ifdef UT_EMBEDDED
        for ( ; ; ) ;
        #else
        printf("%s\n", synopsis);
        exit(1);
        #endif
    }
}

void UT_generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        unsigned char byte = seed1 + 161 * (length - iRolled + i);
        data[i] = byte;
    }
}

void UT_generateArbitraryMaterial(unsigned char* buffer, size_t length, uint32_t seed)
{
    size_t i;
    uint32_t state1 = seed;
    uint32_t state2 = (uint32_t)length;

    for(i=0; i<6; i++) {
        state1 = 5*state1 + state2 + 1;
        state1 = (state1 << 19) | (state1 >> 13);
        state2 = state2 ^ state1;
        state2 = (state2 << 14) | (state2 >> 18);
    }
    for(i=0; i<length; i++) {
        buffer[i] = (unsigned char)(state1 & 0xFF);
        state1 ^= (uint32_t)i;
        state1 = 5*state1 + state2 + 1;
        state1 = (state1 << 19) | (state1 >> 13);
        state2 = state2 ^ state1;
        state2 = (state2 << 14) | (state2 >> 18);
    }
}

void UT_arbitraryChecksumInitialize(UT_arbitraryChecksumState *ac)
{
    ac->state1 = 0;
    ac->state2 = 0;
}
void UT_arbitraryChecksumAbsorb(UT_arbitraryChecksumState *ac, const unsigned char *buffer, size_t length)
{
    size_t i;
    for(i=0; i<length; i++) {
        ac->state1 ^= (uint32_t)buffer[i];
        ac->state1 = 5*ac->state1 + ac->state2 + 1;
        ac->state1 = (ac->state1 << 20) | (ac->state1 >> 12);
        ac->state2 = ac->state2 ^ ac->state1;
        ac->state2 = (ac->state2 << 14) | (ac->state2 >> 18);
    }
}

UT_arbitraryChecksum UT_arbitraryChecksumSqueeze(UT_arbitraryChecksumState *ac)
{
    size_t i;
    ac->state1 ^= (uint32_t)0x100;
    for(i=0; i<6; i++) {
        ac->state1 = 5*ac->state1 + ac->state2 + 1;
        ac->state1 = (ac->state1 << 20) | (ac->state1 >> 12);
        ac->state2 = ac->state2 ^ ac->state1;
        ac->state2 = (ac->state2 << 14) | (ac->state2 >> 18);
    }
    return ac->state1;
}
