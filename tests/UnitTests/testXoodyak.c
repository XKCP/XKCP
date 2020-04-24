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
#ifdef XKCP_has_Xoodyak

#include <string.h>
#include "testXoodyak.h"
#include "Xoodyak.h"
#include "UT.h"

static void generateSimpleRawMaterial(uint8_t* data, size_t length, uint8_t seed1, unsigned int seed2)
{
    size_t i;

    for(i=0; i<length; i++) {
        uint8_t iRolled = ((uint8_t)i << seed2) | ((uint8_t)i >> (8-seed2));
        uint8_t byte = seed1 + 161* (uint8_t)length - iRolled + (uint8_t)i;
        data[i] = byte;
    }
}

#include "Xoodoo-SnP.h"

#define prefix                      Xoodyak
    #include "testXoodyakHash.inc"
    #include "testXoodyakKeyed.inc"
#undef prefix

int testXoodyak( void )
{
    UT_startTest("Xoodyak", Xoodoo_implementation);
    Xoodyak_testHash("XoodyakHash.txt", (uint8_t*)"\x72\xbb\x07\xae\x9c\xae\x32\xb3\x0e\xa4\x73\x65\x67\x01\xf3\xd8\x25\xbd\x56\x82\x1b\xb6\xa4\x5d\x2c\xba\xbc\x50\x78\xab\x4c\x7a");
    Xoodyak_testKeyed("XoodyakKeyed.txt", (uint8_t*)"\xaa\x2c\x40\x75\x31\x3f\xce\x6a\x55\xed\xa0\x40\xf9\xd0\x02\x54\x0e\x4b\xd1\x2e\xa0\x8d\x52\x3d\x48\x86\x34\xe2\x97\x89\xd6\xd8");
    UT_endTest();

    return( 0 );
}
#endif /* XKCP_has_Xoodyak */
