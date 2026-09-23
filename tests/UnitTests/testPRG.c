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

#include "config.h"
#ifdef XKCP_has_PRG_Keccak

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "KeccakSponge.h"
#include "KeccakPRG.h"
#include "UT.h"

#define flavor_IUF_AllAtOnce    1
#define flavor_IUF_Pieces       2

#ifdef XKCP_has_PRG_Keccak_width1600
    #define prefix KeccakWidth1600
    #define SnP_width 1600
    #include "testPRG.inc"
    #undef prefix
    #undef SnP_width
#endif


#ifdef UT_OUTPUT
void writeTestSpongePRG(void)
{
    FILE *f;
    unsigned int rho;

    f = fopen("TestKeccakPRG.txt", "w");
    assert(f != NULL);
#ifdef XKCP_has_PRG_Keccak_width1600
    for(rho = 1; rho < 1600/8; rho += (rho < 1024/8) ? 8 : ((rho < 1344/8) ? 4 : 1))
        KeccakWidth1600_writeTestSpongePRG(f, rho);
    KeccakWidth1600_writeTestSpongePRG(f, 1344/8);
    KeccakWidth1600_writeTestSpongePRG(f, 1088/8);
#endif
    fclose(f);
}
#endif

void selfTestSpongePRG(unsigned int rho, unsigned int width, int flavor, const char *expected)
{
#ifdef XKCP_has_PRG_Keccak_width1600
    if (width == 1600)
        KeccakWidth1600_selfTestSpongePRG(rho, flavor, (const uint8_t*)expected);
    else
#endif
        abort();
}

void testPRG(void)
{
    unsigned int flavor;
    const char *flavorString = 0;

#ifdef UT_OUTPUT
    writeTestSpongePRG();
#endif

    for(flavor=1; flavor<=2; flavor++) {
#if !defined(UT_EMBEDDED)
        if (flavor == flavor_IUF_AllAtOnce)
            flavorString = "Testing PRG with Feed/Fetch all at once";
        else if (flavor == flavor_IUF_Pieces)
            flavorString = "Testing PRG with Feed/Fetch in pieces";
#endif
#ifdef XKCP_has_PRG_Keccak_width1600
    UT_startTest("Keccak with width 1600", flavorString);
    selfTestSpongePRG(1, 1600, flavor, "\x5c\x3f\xb3\xb9\x0c\x8c\x7c\x12"); /* Keccak[r=10, c=1590] */
    selfTestSpongePRG(9, 1600, flavor, "\xdd\x7e\x77\x29\xd3\x8b\x8a\x6a"); /* Keccak[r=74, c=1526] */
    selfTestSpongePRG(17, 1600, flavor, "\xd1\xb9\x06\x68\x4c\xee\x4e\x40"); /* Keccak[r=138, c=1462] */
    selfTestSpongePRG(25, 1600, flavor, "\x9f\x9c\x92\x0c\x7e\xff\xe4\x55"); /* Keccak[r=202, c=1398] */
    selfTestSpongePRG(33, 1600, flavor, "\xdb\x72\x71\x79\x02\xfd\xe1\xe8"); /* Keccak[r=266, c=1334] */
    selfTestSpongePRG(41, 1600, flavor, "\x53\x8a\xbe\x5e\x19\x2e\xf1\x7c"); /* Keccak[r=330, c=1270] */
    selfTestSpongePRG(49, 1600, flavor, "\x23\xa0\x62\xc2\xcc\x57\x8a\x69"); /* Keccak[r=394, c=1206] */
    selfTestSpongePRG(57, 1600, flavor, "\x15\xed\x2d\xf3\xeb\x3e\x86\x19"); /* Keccak[r=458, c=1142] */
    selfTestSpongePRG(65, 1600, flavor, "\x60\x88\x07\x5a\x1b\xbc\xb0\xa0"); /* Keccak[r=522, c=1078] */
    selfTestSpongePRG(73, 1600, flavor, "\xcb\xb7\x7e\x03\x6e\x89\xe7\x1c"); /* Keccak[r=586, c=1014] */
    selfTestSpongePRG(81, 1600, flavor, "\xb3\xea\x41\x88\xc5\xb2\x4b\x29"); /* Keccak[r=650, c=950] */
    selfTestSpongePRG(89, 1600, flavor, "\x36\x42\xf4\xa0\xc1\x38\xb9\xfc"); /* Keccak[r=714, c=886] */
    selfTestSpongePRG(97, 1600, flavor, "\x79\xdf\x80\x58\x93\xec\xd8\xeb"); /* Keccak[r=778, c=822] */
    selfTestSpongePRG(105, 1600, flavor, "\x1d\xc3\x29\x5c\xb5\x86\x46\x32"); /* Keccak[r=842, c=758] */
    selfTestSpongePRG(113, 1600, flavor, "\x29\x60\x1c\x60\x81\x8a\x9d\x00"); /* Keccak[r=906, c=694] */
    selfTestSpongePRG(121, 1600, flavor, "\xff\x23\xca\x78\xaf\x49\xeb\x6d"); /* Keccak[r=970, c=630] */
    selfTestSpongePRG(129, 1600, flavor, "\xb6\x27\x59\xce\x98\xea\x7c\xf4"); /* Keccak[r=1034, c=566] */
    selfTestSpongePRG(133, 1600, flavor, "\x04\x12\x40\x79\xc2\x1f\xe0\xb8"); /* Keccak[r=1066, c=534] */
    selfTestSpongePRG(137, 1600, flavor, "\xde\x82\xc3\x94\x62\xb3\x7b\x62"); /* Keccak[r=1098, c=502] */
    selfTestSpongePRG(141, 1600, flavor, "\x41\x4f\x3a\x4a\xb5\xbb\xbd\x57"); /* Keccak[r=1130, c=470] */
    selfTestSpongePRG(145, 1600, flavor, "\x41\xe0\x86\x87\x6d\x8c\xc9\xd1"); /* Keccak[r=1162, c=438] */
    selfTestSpongePRG(149, 1600, flavor, "\xc8\xb5\x94\x05\xcb\x05\x2a\x6a"); /* Keccak[r=1194, c=406] */
    selfTestSpongePRG(153, 1600, flavor, "\x21\x40\x31\x19\x85\xb0\x2d\xa6"); /* Keccak[r=1226, c=374] */
    selfTestSpongePRG(157, 1600, flavor, "\xa9\x92\x32\x34\x6e\x28\x65\xdd"); /* Keccak[r=1258, c=342] */
    selfTestSpongePRG(161, 1600, flavor, "\x45\xfa\x06\x0a\x1f\x53\x80\x88"); /* Keccak[r=1290, c=310] */
    selfTestSpongePRG(165, 1600, flavor, "\x85\x33\x0d\x64\x80\x22\x7c\x5c"); /* Keccak[r=1322, c=278] */
    selfTestSpongePRG(169, 1600, flavor, "\xc7\x94\x5c\x37\xcb\x5a\xe9\x54"); /* Keccak[r=1354, c=246] */
    selfTestSpongePRG(170, 1600, flavor, "\xd5\xba\xb7\x58\x6d\x9a\x04\x55"); /* Keccak[r=1362, c=238] */
    selfTestSpongePRG(171, 1600, flavor, "\x00\x08\xac\x04\xd2\xcc\x12\x89"); /* Keccak[r=1370, c=230] */
    selfTestSpongePRG(172, 1600, flavor, "\x56\xff\x0c\x77\x7a\x7b\x29\x53"); /* Keccak[r=1378, c=222] */
    selfTestSpongePRG(173, 1600, flavor, "\xe7\x27\x6f\x53\x40\xbe\xc1\x81"); /* Keccak[r=1386, c=214] */
    selfTestSpongePRG(174, 1600, flavor, "\xbf\x0f\x17\x1a\x15\x03\x68\xca"); /* Keccak[r=1394, c=206] */
    selfTestSpongePRG(175, 1600, flavor, "\x6f\x2b\x40\x59\xcd\x8a\xfc\x52"); /* Keccak[r=1402, c=198] */
    selfTestSpongePRG(176, 1600, flavor, "\xcb\xfb\x70\x03\x7d\x38\x2d\xed"); /* Keccak[r=1410, c=190] */
    selfTestSpongePRG(177, 1600, flavor, "\xa8\xf0\xc7\x4f\x09\x31\x3d\xac"); /* Keccak[r=1418, c=182] */
    selfTestSpongePRG(178, 1600, flavor, "\x72\x7c\xaf\x13\xd3\x7e\x5c\x2e"); /* Keccak[r=1426, c=174] */
    selfTestSpongePRG(179, 1600, flavor, "\x3e\xa0\x1a\x45\xd6\x3f\x90\x0e"); /* Keccak[r=1434, c=166] */
    selfTestSpongePRG(180, 1600, flavor, "\xc0\x95\x1d\x3c\x75\xbb\x71\xe3"); /* Keccak[r=1442, c=158] */
    selfTestSpongePRG(181, 1600, flavor, "\xc2\xb7\xf3\xa3\x98\x17\xf7\xe8"); /* Keccak[r=1450, c=150] */
    selfTestSpongePRG(182, 1600, flavor, "\x51\x97\x73\xf7\x98\xbb\x8e\xd2"); /* Keccak[r=1458, c=142] */
    selfTestSpongePRG(183, 1600, flavor, "\xce\xe8\x37\x2e\xcc\xa1\x25\x1e"); /* Keccak[r=1466, c=134] */
    selfTestSpongePRG(184, 1600, flavor, "\x3a\x63\x93\xb2\x36\x71\xa2\x5f"); /* Keccak[r=1474, c=126] */
    selfTestSpongePRG(185, 1600, flavor, "\xb2\x94\x32\x2c\x20\x37\x2d\xfe"); /* Keccak[r=1482, c=118] */
    selfTestSpongePRG(186, 1600, flavor, "\xfc\xe9\x0b\x8c\x60\x60\xe2\x38"); /* Keccak[r=1490, c=110] */
    selfTestSpongePRG(187, 1600, flavor, "\x1b\xd7\x5d\x5f\xf0\x95\x35\x1b"); /* Keccak[r=1498, c=102] */
    selfTestSpongePRG(188, 1600, flavor, "\x44\x3c\xd4\x4d\xd5\xf8\x6b\x05"); /* Keccak[r=1506, c=94] */
    selfTestSpongePRG(189, 1600, flavor, "\x6d\x8e\x3d\x10\x40\xaf\x1b\x18"); /* Keccak[r=1514, c=86] */
    selfTestSpongePRG(190, 1600, flavor, "\x4e\x8d\x94\x4f\x17\x6e\xeb\x05"); /* Keccak[r=1522, c=78] */
    selfTestSpongePRG(191, 1600, flavor, "\xb3\xfe\x96\xf3\x9c\xc4\x2e\x65"); /* Keccak[r=1530, c=70] */
    selfTestSpongePRG(192, 1600, flavor, "\x36\x3a\x57\x3e\x63\x6e\xb5\xc8"); /* Keccak[r=1538, c=62] */
    selfTestSpongePRG(193, 1600, flavor, "\x48\x26\x73\xbb\x0a\x4a\x87\x01"); /* Keccak[r=1546, c=54] */
    selfTestSpongePRG(194, 1600, flavor, "\x67\x36\x60\x13\x39\x82\x3a\x10"); /* Keccak[r=1554, c=46] */
    selfTestSpongePRG(195, 1600, flavor, "\xb0\x3d\x32\x97\xc4\xe2\xb6\x25"); /* Keccak[r=1562, c=38] */
    selfTestSpongePRG(196, 1600, flavor, "\x19\x19\xa1\x41\x3c\x97\x56\x90"); /* Keccak[r=1570, c=30] */
    selfTestSpongePRG(197, 1600, flavor, "\xf1\x14\xfe\x7d\x43\x87\x4c\x50"); /* Keccak[r=1578, c=22] */
    selfTestSpongePRG(198, 1600, flavor, "\xef\x16\x06\xa9\xe4\x42\xe2\xd3"); /* Keccak[r=1586, c=14] */
    selfTestSpongePRG(199, 1600, flavor, "\x02\x20\x84\x2a\xd4\x85\xc4\xa4"); /* Keccak[r=1594, c=6] */
    selfTestSpongePRG(168, 1600, flavor, "\xe7\x83\x56\x5d\x92\xdf\xfb\xa4"); /* Keccak[r=1346, c=254] */
    selfTestSpongePRG(136, 1600, flavor, "\xf6\x7e\x5c\xa8\x9e\x16\xb9\x4a"); /* Keccak[r=1090, c=510] */
    UT_endTest();
#endif
    }
}
#endif /* XKCP_has_PRG_Keccak */
