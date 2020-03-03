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

#ifndef _UT_h_
#define _UT_h_

/* #define UT_EMBEDDED */
/* #define UT_OUTPUT */
/* #define UT_VERBOSE */

#include "config.h"
#if (defined(UT_OUTPUT) || defined(UT_VERBOSE) || !defined(UT_EMBEDDED))
#include <stdio.h>
#endif

void UT_startTest(const char *synopsis, const char *implementation);
void UT_endTest();
void UT_assert(int condition, char * synopsis);
void UT_displayInfo(const char *header, const char *contents);
#ifdef UT_EMBEDDED
#define UT_displayByteString(f, synopsis, data, length)
#else
void UT_displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length);
#endif
void UT_generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2);

#endif
