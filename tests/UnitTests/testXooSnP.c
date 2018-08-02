/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#if !defined(EMBEDDED)
/* #define OUTPUT */
/* #define VERBOSE */
#endif

#if defined(EMBEDDED)
void assert(int condition);
#else
#include <assert.h>
#endif

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <string.h>

#ifndef Xoodoo_excluded
    #include "Xoodoo-SnP.h"

    #define prefix Xoodoo
    #define SnP Xoodoo
    #define SnP_width (3*4*32)
    #define SnP_Permute          Xoodoo_Permute_6rounds
    #define SnP_Permute_6rounds  Xoodoo_Permute_6rounds
    #define SnP_Permute_12rounds Xoodoo_Permute_12rounds
    #define SnP_Permute_Nrounds  Xoodoo_Permute_Nrounds
    #define SnP_Permute_maxRounds 12
        #include "testXooSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_6rounds
    #undef SnP_Permute_12rounds
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
#endif


void testXooSnP( void )
{
#ifndef Xoodoo_excluded
    #if defined(Xoodoo_HasNround)
    Xoodoo_testSnP("Xoodoo.txt", "Xoodoo", (const unsigned char *)"\xaf\x76\x84\xce\x28\xc2\x6d\xe1\xbc\x2d\x12\x36\x66\x64\x22\x13\xd4\x13\x06\x5a\x51\x41\x81\xf2\xa9\x01\xd2\xf6\x9a\x2f\x67\x0c\xe0\x00\x21\x1b\x36\x0c\xaa\x58\x90\x24\x1b\xb7\xed\x0c\x09\x99");
    #else
    Xoodoo_testSnP("Xoodoo.txt", "Xoodoo", (const unsigned char *)"\x71\xc5\xff\xa0\x9d\x60\x47\xc2\xbe\x07\x50\xc0\xb1\x73\x20\xaa\xae\x13\xe9\x32\xc6\x16\xcc\x3a\x27\xc6\xce\x00\x35\xb9\x56\x30\x11\x9a\xdb\x51\x49\x36\xb5\xc8\xf1\xaa\x0c\xf0\x47\xd7\x59\x5e");
    #endif
#endif
}
