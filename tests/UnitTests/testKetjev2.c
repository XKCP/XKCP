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
#ifdef XKCP_has_Ketje

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "UT.h"

#include "Ketjev2.h"
#include "testKetjev2.h"

static unsigned int myMin(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

#ifdef XKCP_has_KetjeJr
    #include "KeccakP-200-SnP.h"

    #define prefix                      KetjeJr
    #define SnP_width                   200
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef XKCP_has_KetjeSr
    #include "KeccakP-400-SnP.h"

    #define prefix                      KetjeSr
    #define SnP_width                   400
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef XKCP_has_KetjeMn
    #include "KeccakP-800-SnP.h"

    #define prefix                      KetjeMn
    #define SnP_width                   800
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef XKCP_has_KetjeMj
    #include "KeccakP-1600-SnP.h"

    #define prefix                      KetjeMj
    #define SnP_width                   1600
        #include "testKetjev2.inc"
    #undef prefix
    #undef SnP_width
#endif

int testKetje( void )
{
#ifdef XKCP_has_KetjeJr
    UT_startTest("KetjeJr", KeccakP200_implementation);
    KetjeJr_test("KetjeJr.txt", (const uint8_t*)"\x6b\x2d\xb5\xc5\x76\x51\x36\x6c\xf8\x3e\x42\xdc\xb3\x69\x0e\x51");
    UT_endTest();
#endif

#ifdef XKCP_has_KetjeSr
    UT_startTest("KetjeSr", KeccakP400_implementation);
    KetjeSr_test("KetjeSr.txt", (const uint8_t*)"\x92\xaf\x55\x88\x48\xdf\x0a\x4e\x9b\x94\xf6\x33\xee\x2f\xe9\x71");
    UT_endTest();
#endif

#ifdef XKCP_has_KetjeMn
    UT_startTest("KetjeMn", KeccakP800_implementation);
    KetjeMn_test("KetjeMn.txt", (const uint8_t*)"\xae\x36\xc9\xe0\xea\xbc\x11\x92\xf6\x7a\x9f\xb6\x93\x8a\xe3\x58");
    UT_endTest();
#endif

#ifdef XKCP_has_KetjeMj
    UT_startTest("KetjeMj", KeccakP1600_implementation);
    KetjeMj_test("KetjeMj.txt", (const uint8_t*)"\x1e\x7c\x6c\x56\x42\x4f\x8c\x1f\xe0\xbd\x04\x2d\x03\xda\x3a\x1e");
    UT_endTest();
#endif

    return( 0 );
}
#endif /* XKCP_has_Ketje */
