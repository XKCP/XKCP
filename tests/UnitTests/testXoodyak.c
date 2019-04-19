/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "Xoodyak.h"

#if !defined(EMBEDDED)
//#define OUTPUT
#define VERBOSE
/* #define GENERATE */
/* #define NONCE_200 */
#endif

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#include <stdlib.h>
#endif
#include <string.h>

#ifdef OUTPUT
#define PRINTS(ttt) printf(ttt);
#else
#define FILE    void
#define PRINTS(ttt)
#endif

#include "testXoodyak.h"

#define myMax(a, b) ((a) > (b)) ? (a) : (b)

#ifdef OUTPUT
static void displayByteString(FILE *f, const char* synopsis, const uint8_t *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif

static void generateSimpleRawMaterial(uint8_t* data, unsigned int length, uint8_t seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        uint8_t iRolled = ((uint8_t)i << seed2) | ((uint8_t)i >> (8-seed2));
        uint8_t byte = (uint8_t)(seed1 + 161*length - iRolled + i);
        data[i] = byte;
    }
}

static void assert(int condition, char * synopsis)
{
    if (!condition)
    {
        #ifdef OUTPUT
        printf("\n%s", synopsis);
        #endif
        #ifdef EMBEDDED
        for ( ; ; ) ;
        #else
        exit(1);
        #endif
    }
}

#ifndef Xoodoo_excluded
    #include "Xoodoo-SnP.h"

    #define prefix                      Xoodyak
        #include "testXoodyakHash.inc"
        #include "testXoodyakKeyed.inc"
    #undef prefix

#endif

int testXoodyak( void )
{
#ifndef Xoodoo_excluded

    PRINTS("Xoodyak(" Xoodoo_implementation ")\n");
    Xoodyak_testHash("XoodyakHash.txt", (uint8_t*)"\x72\xbb\x07\xae\x9c\xae\x32\xb3\x0e\xa4\x73\x65\x67\x01\xf3\xd8\x25\xbd\x56\x82\x1b\xb6\xa4\x5d\x2c\xba\xbc\x50\x78\xab\x4c\x7a");
    Xoodyak_testKeyed("XoodyakKeyed.txt", (uint8_t*)"\xf9\x58\xff\x34\x0f\x78\xf0\x49\xc8\x05\x14\x8f\xee\x9a\x5f\xc4\x72\xe5\x83\xbc\x1e\x5d\x43\xd7\x71\x3b\xa8\x1f\xb4\x4a\x4b\xb6");

#endif
    return( 0 );
}
