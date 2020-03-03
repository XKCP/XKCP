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
#ifdef XKCP_has_Keyak

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "Keyakv2.h"
#include "UT.h"

#define myMax(a, b) ((a) > (b)) ? (a) : (b)

static void generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        unsigned char byte = seed1 + 161*length - iRolled + i;
        data[i] = byte;
    }
}

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"

    #define prefix                     KeyakWidth800
    #define SnP                        KeccakP800
    #define SnP_width                  800
    #define PlSnP_parallelism          1
        #include "testMotorist.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"

    #define prefix                     KeyakWidth1600
    #define SnP                        KeccakP1600
    #define SnP_width                  1600
    #define PlSnP_parallelism          1
        #include "testMotorist.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef PlSnP_parallelism
#endif

#ifdef XKCP_has_KeccakP1600times2
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeyakWidth1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define SnP_width                   1600
        #include "testMotorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
#endif

#ifdef XKCP_has_KeccakP1600times4
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      KeyakWidth1600times4
    #define PlSnP                       KeccakP1600times4
    #define PlSnP_parallelism           4
    #define SnP_width                   1600
        #include "testMotorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
#endif

#ifdef XKCP_has_KeccakP1600times8
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      KeyakWidth1600times8
    #define PlSnP                       KeccakP1600times8
    #define PlSnP_parallelism           8
    #define SnP_width                   1600
        #include "testMotorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef SnP_width
#endif

int testMotorist( void )
{

#ifdef XKCP_has_KeccakP800
    UT_startTest("Motorist on Keccak-p[800]", KeccakP800_implementation);
    KeyakWidth800_testOneMotorist("Motorist-Keccak-p[800].txt", (const uint8_t*)"\x48\x62\xc7\x9b\x33\xb8\xd0\xea\x9d\x18\x55\xa0\x4a\xff\x61\xcf");
    UT_endTest();
#endif

#ifdef XKCP_has_KeccakP1600
    UT_startTest("Motorist on Keccak-p[1600]", KeccakP1600_implementation);
    KeyakWidth1600_testOneMotorist("Motorist-Keccak-p[1600].txt", (const uint8_t*)"\xfb\x91\x63\x61\xd4\x9b\xa4\x0d\xd1\xe4\xa4\xd7\x58\xb9\x04\x61");
    UT_endTest();
#endif

#ifdef XKCP_has_KeccakP1600times2
    UT_startTest("Motorist on Keccak-p[1600]\303\2272", KeccakP1600times2_implementation);
    KeyakWidth1600times2_testOneMotorist("Motorist-Keccak-p[1600]-times2.txt", (const uint8_t*)"\x8c\xb4\x28\x1e\x45\xef\x1e\xbc\x7e\x67\x16\xa8\xd1\x74\xc2\x43");
    UT_endTest();
#endif

#ifdef XKCP_has_KeccakP1600times4
    UT_startTest("Motorist on Keccak-p[1600]\303\2274", KeccakP1600times4_implementation);
    KeyakWidth1600times4_testOneMotorist("Motorist-Keccak-p[1600]-times4.txt", (const uint8_t*)"\xc7\xa2\xf9\x5a\x77\x6d\x12\x6d\x3c\x1f\x18\x6f\x3f\x43\x1c\xef");
    UT_endTest();
#endif

#ifdef XKCP_has_KeccakP1600times8
    UT_startTest("Motorist on Keccak-p[1600]\303\2278", KeccakP1600times8_implementation);
    KeyakWidth1600times8_testOneMotorist("Motorist-Keccak-p[1600]-times8.txt", (const uint8_t*)"\x2b\x6d\x17\x2a\x6b\x90\xff\x74\xb2\xc5\x6b\xd1\xaf\xf3\x9d\xb6");
    UT_endTest();
#endif

    return( 0 );
}
#endif /* XKCP_has_Keyak */
