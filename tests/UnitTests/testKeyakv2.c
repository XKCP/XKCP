/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "Keyakv2.h"

#if !defined(EMBEDDED)
#define OUTPUT
#define VERBOSE
/* #define GENERATE */
/* #define NONCE_200 */
#endif

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#include <stdlib.h>
#endif
#include <string.h>

#ifndef OUTPUT
#define FILE    void
#endif
#include "Keyakv2.h"
#include "testKeyakv2.h"

#define myMax(a, b) ((a) > (b)) ? (a) : (b)

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
        unsigned char byte = seed1 + 161*length - iRolled + i;
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

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix                      River
    #define SnP_width                   800
    #define PlSnP_parallelism           1
        #include "testKeyakv2.inc"
    #undef prefix
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix                      Lake
    #define SnP_width                   1600
    #define PlSnP_parallelism           1
        #include "testKeyakv2.inc"
    #undef prefix
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      Sea
    #define SnP_width                   1600
    #define PlSnP_parallelism           2
        #include "testKeyakv2.inc"
    #undef prefix
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      Ocean
    #define SnP_width                   1600
    #define PlSnP_parallelism           4
        #include "testKeyakv2.inc"
    #undef prefix
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      Lunar
    #define SnP_width                   1600
    #define PlSnP_parallelism           8
        #include "testKeyakv2.inc"
    #undef prefix
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

int testKeyak( void )
{
#ifdef NONCE_200

#ifndef KeccakP800_excluded
#ifdef OUTPUT
    printf("River Keyak (" KeccakP800_implementation ")\n");
#endif
    River_testOneKeyak("RiverKeyak.txt", "\x6e\xba\x81\x33\x0b\xb8\x5a\x4d\x8d\xb3\x7f\xde\x4d\x67\xcd\x0e");
#endif

#ifndef KeccakP1600_excluded
#ifdef OUTPUT
    printf("Lake Keyak (" KeccakP1600_implementation ")\n");
#endif
    Lake_testOneKeyak("LakeKeyak.txt", "\x83\x95\xc6\x41\x22\xbb\x43\x04\x32\xd8\xb0\x29\x82\x09\xb7\x36");
#endif

#ifndef KeccakP1600timesN_excluded
#ifdef OUTPUT
    printf("Sea Keyak (" KeccakP1600times2_implementation ")\n");
#endif
    Sea_testOneKeyak("SeaKeyak.txt", "\xb8\xc0\xe2\x35\x22\xcc\x1d\xe1\x4c\x22\xd0\xb8\xaf\x73\x8e\x33");
#endif

#ifndef KeccakP1600timesN_excluded
#ifdef OUTPUT
    printf("Ocean Keyak (" KeccakP1600times4_implementation ")\n");
#endif
    Ocean_testOneKeyak("OceanKeyak.txt", "\x70\x7c\x06\x47\xf9\xe8\x52\xb6\x00\xee\xd0\xf1\x1c\x66\xe1\x1d");
#endif

#ifndef KeccakP1600timesN_excluded
#ifdef OUTPUT
    printf("Lunar Keyak (" KeccakP1600times8_implementation ")\n");
#endif
    Lunar_testOneKeyak("LunarKeyak.txt", "\xb7\xec\x21\x1d\xc0\x30\xd2\x4d\x66\x70\x44\xc2\xed\x34\x52\x11");
#endif

#else

#ifndef KeccakP800_excluded
#ifdef OUTPUT
    printf("River Keyak (" KeccakP800_implementation ")\n");
#endif
    River_testOneKeyak("RiverKeyak.txt", "\xa7\xce\x27\x81\x19\x38\x13\x11\xa1\x1f\x8f\xac\x84\xcb\x6b\x24");
#endif

#ifndef KeccakP1600_excluded
#ifdef OUTPUT
    printf("Lake Keyak (" KeccakP1600_implementation ")\n");
#endif
    Lake_testOneKeyak("LakeKeyak.txt", "\x73\x03\xc4\xba\x1e\xff\xc3\x9d\x48\x80\x65\xc2\xfd\x05\xf7\x52");
#endif

#ifndef KeccakP1600timesN_excluded
#ifdef OUTPUT
    printf("Sea Keyak (" KeccakP1600times2_implementation ")\n");
#endif
    Sea_testOneKeyak("SeaKeyak.txt", "\xc4\xaa\x23\x13\x54\x88\x8d\xb6\xdc\x5c\xc3\xc8\xe9\xba\x86\x76");
#endif

#ifndef KeccakP1600timesN_excluded
#ifdef OUTPUT
    printf("Ocean Keyak (" KeccakP1600times4_implementation ")\n");
#endif
    Ocean_testOneKeyak("OceanKeyak.txt", "\xd2\xff\x93\x7a\xf6\x17\x48\xe9\xfb\x90\xee\x46\x37\x9c\x5b\x02");
#endif

#ifndef KeccakP1600timesN_excluded
#ifdef OUTPUT
    printf("Lunar Keyak (" KeccakP1600times8_implementation ")\n");
#endif
    Lunar_testOneKeyak("LunarKeyak.txt", "\xed\xa4\x43\xe6\xe2\xf8\x36\xb4\x58\xce\xe2\x93\xdf\xb6\xc6\x60");
#endif

#endif
    return( 0 );
}
