/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#if !defined(EMBEDDED)
#define OUTPUT
#define VERBOSE
/* #define GENERATE */
#endif

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#include <stdlib.h>
#endif
#include <string.h>

#ifndef OUTPUT
#define FILE    void
#endif
#include "Ketjev2.h"
#include "testKetjev2.h"

#ifdef OUTPUT
static void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif

static void generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        unsigned char byte = seed1 + 161 * (length - iRolled + i);
        data[i] = byte;
    }
}

static unsigned int myMin(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

static void assert(int condition, char * synopsis)
{
    if (!condition)
    {
        #ifdef OUTPUT
        printf("%s", synopsis);
        #endif
        #ifdef EMBEDDED
        for ( ; ; ) ;
        #else
        exit(1);
        #endif
    }
}

#ifndef KeccakP200_excluded
    #include "KeccakP-200-SnP.h"

    #define prefix                      KetjeJr
    #define SnP_width                   200
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"

    #define prefix                      KetjeSr
    #define SnP_width                   400
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix                      KetjeMn
    #define SnP_width                   800
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix                      KetjeMj
    #define SnP_width                   1600
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

int testKetje( void )
{

#ifndef KeccakP200_excluded
#ifdef OUTPUT
    printf("KetjeJr (" KeccakP200_implementation ")\n");
#endif
    KetjeJr_test("KetjeJr.txt", "\x6b\x2d\xb5\xc5\x76\x51\x36\x6c\xf8\x3e\x42\xdc\xb3\x69\x0e\x51");
#endif

#ifndef KeccakP400_excluded
#ifdef OUTPUT
    printf("KetjeSr (" KeccakP400_implementation ")\n");
#endif
    KetjeSr_test("KetjeSr.txt", "\x92\xaf\x55\x88\x48\xdf\x0a\x4e\x9b\x94\xf6\x33\xee\x2f\xe9\x71");
#endif

#ifndef KeccakP800_excluded
#ifdef OUTPUT
    printf("KetjeMn (" KeccakP800_implementation ")\n");
#endif
    KetjeMn_test("KetjeMn.txt", "\xae\x36\xc9\xe0\xea\xbc\x11\x92\xf6\x7a\x9f\xb6\x93\x8a\xe3\x58");
#endif

#ifndef KeccakP1600_excluded
#ifdef OUTPUT
    printf("KetjeMj (" KeccakP1600_implementation ")\n");
#endif
    KetjeMj_test("KetjeMj.txt", "\x1e\x7c\x6c\x56\x42\x4f\x8c\x1f\xe0\xbd\x04\x2d\x03\xda\x3a\x1e");
#endif

    return( 0 );
}
