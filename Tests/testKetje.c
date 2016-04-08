/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#if !defined(EMBEDDED)
#define OUTPUT
#define VERBOSE
//#define GENERATE
#endif

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#include <stdlib.h>
#endif
#include <string.h>

#ifndef OUTPUT
#define FILE    void
#endif
#include "Ketje.h"
#include "testKetje.h"

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

    for( i=0; i<length; i++) {
        unsigned int iRolled = i*seed1;
        unsigned char byte = (iRolled+length+seed2)%0xFF;
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
        #include "testKetje.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"

    #define prefix                      KetjeSr
    #define SnP_width                   400
        #include "testKetje.inc"
    #undef prefix
    #undef SnP_width
#endif

int testKetje( void )
{

#ifndef KeccakP200_excluded
#ifdef OUTPUT
    printf("KetjeJr (" KeccakP200_implementation ")\n");
#endif
    KetjeJr_test("KetjeJr.txt", "\x3b\x7d\xea\x9d\xf3\xe0\x58\x06\x98\x92\xc3\xc0\x05\x0f\x4b\xfd");
#endif

#ifndef KeccakP400_excluded
#ifdef OUTPUT
    printf("KetjeSr (" KeccakP400_implementation ")\n");
#endif
    KetjeSr_test("KetjeSr.txt", "\x4a\x31\xc7\x51\x18\x7f\x03\x2c\x78\xc3\xcf\x36\x51\x0b\xe3\xb3");
#endif

    return( 0 );
}
