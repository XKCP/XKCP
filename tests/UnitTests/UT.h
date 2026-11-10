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
#include <stdint.h>

#define STR0(x) #x
#define STR(x) STR0(x)
#define UT_assert(condition)    UT_assertSyn((condition), "Assertion `" #condition "' failed at " __FILE__ ":" STR(__LINE__))

void UT_startTest(const char *synopsis, const char *implementation);
void UT_endTest();
void UT_assertSyn(int condition, char * synopsis);
void UT_displayInfo(const char *header, const char *contents);
#ifdef UT_EMBEDDED
#define UT_displayByteString(f, synopsis, data, length)
#else
void UT_displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length);
#endif
void UT_generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2);
void UT_generateArbitraryMaterial(unsigned char* buffer, size_t length, uint32_t seed);

typedef struct {
    uint32_t state1;
    uint32_t state2;
} UT_arbitraryChecksumState;
typedef uint32_t UT_arbitraryChecksum;

void UT_arbitraryChecksumInitialize(UT_arbitraryChecksumState *ac);
void UT_arbitraryChecksumAbsorb(UT_arbitraryChecksumState *ac, const unsigned char *buffer, size_t length);
UT_arbitraryChecksum UT_arbitraryChecksumSqueeze(UT_arbitraryChecksumState *ac);

#endif
