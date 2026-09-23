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
#ifdef XKCP_has_Sponge_Keccak

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "KeccakSponge.h"
#include "TurboSHAKE.h"
#include "UT.h"

#define flavor_OneCall          1
#define flavor_IUF_AllAtOnce    2
#define flavor_IUF_Pieces       3

#ifdef XKCP_has_Sponge_Keccak_width1600
    #define prefix KeccakWidth1600
    #define SnP_width 1600
    #include "testSponge.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef XKCP_has_TurboSHAKE
    XKCP_DeclareSpongeFunctions(TurboSHAKE)
    #define prefix TurboSHAKE
    #define SnP_width 1600
    #include "testSponge.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef UT_OUTPUT
void writeTestSponge(void)
{
    FILE *f;
    unsigned int rate;

    f = fopen("TestSponge.txt", "w");
    UT_assert(f != NULL);
#ifdef XKCP_has_Sponge_Keccak_width1600
    for(rate = 64; rate <= 1600; rate += (rate < 1024) ? 64 : ((rate < 1344) ? 32 : 8))
        KeccakWidth1600_writeTestSponge(f, rate, 1600-rate);
#endif
#ifdef XKCP_has_TurboSHAKE
    for(rate = 1088; rate <= 1600; rate += 8)
        TurboSHAKE_writeTestSponge(f, rate, 1600-rate);
#endif
    fclose(f);
}
#endif

void selfTestSponge(unsigned int rate, unsigned int capacity, int flavor, const char *expected)
{
#ifdef XKCP_has_Sponge_Keccak_width1600
    if (rate+capacity == 1600)
        KeccakWidth1600_selfTestSponge(rate, capacity, flavor, (const uint8_t*)expected);
    else
#endif
        abort();
}

#ifdef XKCP_has_TurboSHAKE
void selfTestTurboSHAKE(unsigned int capacity, int flavor, const char *expected)
{
    TurboSHAKE_selfTestSponge(1600-capacity, capacity, flavor, (const uint8_t*)expected);
}
#endif

void testSponge()
{
    unsigned int flavor;
    const char *flavorString = 0;

#ifdef UT_OUTPUT
    writeTestSponge();
#endif

    for(flavor=1; flavor<=3; flavor++) {
#if !defined(UT_EMBEDDED)
        if (flavor == flavor_OneCall)
            flavorString = "Testing sponge in one call";
        else if (flavor == flavor_IUF_AllAtOnce)
            flavorString = "Testing sponge with queue all at once";
        else if (flavor == flavor_IUF_Pieces)
            flavorString = "Testing sponge with queue in pieces";
#endif
#ifdef XKCP_has_Sponge_Keccak_width1600
    UT_startTest("Keccak with width 1600", flavorString);
    selfTestSponge(64, 1536, flavor, "\x54\x77\x74\x62\x25\x98\x8f\x9e"); /* Keccak[r=64, c=1536] */
    selfTestSponge(128, 1472, flavor, "\xcb\x2a\x3f\x46\x01\x03\xcd\xbb"); /* Keccak[r=128, c=1472] */
    selfTestSponge(192, 1408, flavor, "\x21\x08\x0f\x2e\x93\x2f\x13\xd2"); /* Keccak[r=192, c=1408] */
    selfTestSponge(256, 1344, flavor, "\x92\xd9\xe0\x5b\xce\x47\x79\x2f"); /* Keccak[r=256, c=1344] */
    selfTestSponge(320, 1280, flavor, "\x90\xc9\x9e\x04\x89\x51\x4e\xce"); /* Keccak[r=320, c=1280] */
    selfTestSponge(384, 1216, flavor, "\x57\xbd\xc9\xba\x73\xa0\x23\x26"); /* Keccak[r=384, c=1216] */
    selfTestSponge(448, 1152, flavor, "\x46\x82\x7e\xdc\x43\xdf\x48\xe8"); /* Keccak[r=448, c=1152] */
    selfTestSponge(512, 1088, flavor, "\x4d\xdc\x6c\x49\xa4\x49\x6a\x8f"); /* Keccak[r=512, c=1088] */
    selfTestSponge(576, 1024, flavor, "\xfb\x03\x99\x50\x65\xf5\xbd\x80"); /* Keccak[r=576, c=1024] */
    selfTestSponge(640, 960, flavor, "\xb6\x29\x43\x9c\x9e\x0d\xa6\xe5"); /* Keccak[r=640, c=960] */
    selfTestSponge(704, 896, flavor, "\x9a\x58\xc4\xab\x76\xcb\x80\x8c"); /* Keccak[r=704, c=896] */
    selfTestSponge(768, 832, flavor, "\x5b\xf5\x96\xec\xdd\x27\x7e\xf9"); /* Keccak[r=768, c=832] */
    selfTestSponge(832, 768, flavor, "\x39\xe6\x43\x9d\xb3\x73\xd3\x01"); /* Keccak[r=832, c=768] */
    selfTestSponge(896, 704, flavor, "\xd3\xbe\xb4\x7d\xe4\x45\x1a\x16"); /* Keccak[r=896, c=704] */
    selfTestSponge(960, 640, flavor, "\x6a\xf8\x5c\x73\xd2\x80\xc1\x78"); /* Keccak[r=960, c=640] */
    selfTestSponge(1024, 576, flavor, "\xe5\x87\x79\xbd\x1a\xc6\x6f\xd4"); /* Keccak[r=1024, c=576] */
    selfTestSponge(1056, 544, flavor, "\x5b\x58\x13\xb9\x9b\x77\xb1\x9d"); /* Keccak[r=1056, c=544] */
    selfTestSponge(1088, 512, flavor, "\x53\x8b\xa6\x7e\xb0\x77\xab\xc1"); /* Keccak[r=1088, c=512] */
    selfTestSponge(1120, 480, flavor, "\x65\xe7\x48\x82\xb0\xeb\x9c\x50"); /* Keccak[r=1120, c=480] */
    selfTestSponge(1152, 448, flavor, "\x4d\x2f\xed\x3c\x47\x9a\xb9\x5f"); /* Keccak[r=1152, c=448] */
    selfTestSponge(1184, 416, flavor, "\xe5\x16\xd6\xb3\x35\x82\x48\x74"); /* Keccak[r=1184, c=416] */
    selfTestSponge(1216, 384, flavor, "\xce\xa2\x10\x72\x20\xb5\x66\x46"); /* Keccak[r=1216, c=384] */
    selfTestSponge(1248, 352, flavor, "\x2e\x8d\x7f\x87\xcd\xc3\x71\x78"); /* Keccak[r=1248, c=352] */
    selfTestSponge(1280, 320, flavor, "\xfb\x51\xeb\x06\x70\x3a\xb6\x09"); /* Keccak[r=1280, c=320] */
    selfTestSponge(1312, 288, flavor, "\x7d\x42\x0e\x1d\x4c\x39\xa1\x34"); /* Keccak[r=1312, c=288] */
    selfTestSponge(1344, 256, flavor, "\x47\xca\x6e\xf2\x15\x0f\xbe\x34"); /* Keccak[r=1344, c=256] */
    selfTestSponge(1352, 248, flavor, "\x77\x26\x81\x38\x2f\x79\x94\xfb"); /* Keccak[r=1352, c=248] */
    selfTestSponge(1360, 240, flavor, "\xf4\xcd\x04\xe1\x8b\x7a\x6b\x06"); /* Keccak[r=1360, c=240] */
    selfTestSponge(1368, 232, flavor, "\xd6\xb2\x11\xc3\xc5\x5b\x6b\x0d"); /* Keccak[r=1368, c=232] */
    selfTestSponge(1376, 224, flavor, "\xf6\x0d\x05\x1b\xba\x7a\x47\xca"); /* Keccak[r=1376, c=224] */
    selfTestSponge(1384, 216, flavor, "\x08\xff\x23\x11\x21\xd6\xee\xd0"); /* Keccak[r=1384, c=216] */
    selfTestSponge(1392, 208, flavor, "\x4e\xd0\x62\xf2\x7f\xbe\x2f\xd9"); /* Keccak[r=1392, c=208] */
    selfTestSponge(1400, 200, flavor, "\x70\x1c\xe5\x63\xc7\x8d\x51\x9b"); /* Keccak[r=1400, c=200] */
    selfTestSponge(1408, 192, flavor, "\xe4\xaf\xf7\xe9\x04\x8e\xdb\xc0"); /* Keccak[r=1408, c=192] */
    selfTestSponge(1416, 184, flavor, "\xa7\x88\xfa\x05\x8d\xe7\x13\x8f"); /* Keccak[r=1416, c=184] */
    selfTestSponge(1424, 176, flavor, "\xfe\x59\x58\xed\x09\xcd\x10\x3f"); /* Keccak[r=1424, c=176] */
    selfTestSponge(1432, 168, flavor, "\x87\x4d\x0f\xf5\x59\x40\x05\x12"); /* Keccak[r=1432, c=168] */
    selfTestSponge(1440, 160, flavor, "\x97\x15\x63\x6e\x28\xd3\x35\x7a"); /* Keccak[r=1440, c=160] */
    selfTestSponge(1448, 152, flavor, "\x25\xc7\xce\x8d\x20\xd9\x56\xae"); /* Keccak[r=1448, c=152] */
    selfTestSponge(1456, 144, flavor, "\x26\x22\x33\xbe\x2d\x17\xe5\xe3"); /* Keccak[r=1456, c=144] */
    selfTestSponge(1464, 136, flavor, "\xcb\x8b\x11\x17\x02\x5c\x57\xa9"); /* Keccak[r=1464, c=136] */
    selfTestSponge(1472, 128, flavor, "\xa7\x2f\xba\x93\x8a\xf1\xf5\xd9"); /* Keccak[r=1472, c=128] */
    selfTestSponge(1480, 120, flavor, "\xe2\xd4\x0a\xe4\x1c\xce\x34\x6b"); /* Keccak[r=1480, c=120] */
    selfTestSponge(1488, 112, flavor, "\xd2\xcd\x4e\xae\xf8\xe2\xc7\xee"); /* Keccak[r=1488, c=112] */
    selfTestSponge(1496, 104, flavor, "\x09\x52\x62\x4b\x47\xb9\xe8\x3a"); /* Keccak[r=1496, c=104] */
    selfTestSponge(1504, 96, flavor, "\xcf\xea\x23\x46\xd4\xd3\x5c\xb0"); /* Keccak[r=1504, c=96] */
    selfTestSponge(1512, 88, flavor, "\x01\x3e\x85\x05\xfa\x47\xc1\x06"); /* Keccak[r=1512, c=88] */
    selfTestSponge(1520, 80, flavor, "\x8c\xd5\x37\xdc\x1f\xeb\x86\xbb"); /* Keccak[r=1520, c=80] */
    selfTestSponge(1528, 72, flavor, "\xc4\xad\x79\x05\x63\xce\x45\x0f"); /* Keccak[r=1528, c=72] */
    selfTestSponge(1536, 64, flavor, "\xb4\x95\xfb\x65\x46\x2d\x0b\x4e"); /* Keccak[r=1536, c=64] */
    selfTestSponge(1544, 56, flavor, "\xec\x6b\x56\x78\xe7\xd5\xec\x4a"); /* Keccak[r=1544, c=56] */
    selfTestSponge(1552, 48, flavor, "\x6d\x9d\x95\x2e\xc8\x0e\x55\x01"); /* Keccak[r=1552, c=48] */
    selfTestSponge(1560, 40, flavor, "\xde\xc3\xe0\xc5\x64\x53\x15\x7a"); /* Keccak[r=1560, c=40] */
    selfTestSponge(1568, 32, flavor, "\x0f\xe2\x4d\xc9\xbb\xb4\x1e\xa7"); /* Keccak[r=1568, c=32] */
    selfTestSponge(1576, 24, flavor, "\x4b\xc8\x5e\x79\x24\xe0\x09\x95"); /* Keccak[r=1576, c=24] */
    selfTestSponge(1584, 16, flavor, "\xca\x7a\xf0\xbd\x9a\x5d\xda\x4c"); /* Keccak[r=1584, c=16] */
    selfTestSponge(1592, 8, flavor, "\x61\xec\x09\x23\xcf\xc5\xe5\x29"); /* Keccak[r=1592, c=8] */
    selfTestSponge(1600, 0, flavor, "\x7a\x4d\x47\x73\xf2\xf6\xf8\xbc"); /* Keccak[r=1600, c=0] */
    UT_endTest();
#endif
#ifdef XKCP_has_TurboSHAKE
    UT_startTest("TurboSHAKE", flavorString);
    selfTestTurboSHAKE(512, flavor, "\x5e\x8c\xdc\x83\xa9\x84\x55\xc4"); /* TurboSHAKE256 */
    selfTestTurboSHAKE(504, flavor, "\xb3\xd0\x3d\x52\x04\x31\xf1\x53"); /* TurboSHAKE[c=504] */
    selfTestTurboSHAKE(496, flavor, "\x8d\x39\xe4\x39\x1a\x16\x10\xbe"); /* TurboSHAKE[c=496] */
    selfTestTurboSHAKE(488, flavor, "\x52\xde\x8d\x83\x11\xc0\xc4\x5c"); /* TurboSHAKE[c=488] */
    selfTestTurboSHAKE(480, flavor, "\x84\x09\xd9\x03\x8a\xe9\x88\xaf"); /* TurboSHAKE[c=480] */
    selfTestTurboSHAKE(472, flavor, "\xa1\xe6\x03\x8b\x37\x6a\x13\x43"); /* TurboSHAKE[c=472] */
    selfTestTurboSHAKE(464, flavor, "\x9d\x62\xa8\xf6\x93\x1c\xdf\x95"); /* TurboSHAKE[c=464] */
    selfTestTurboSHAKE(456, flavor, "\xa9\x0f\xb4\x40\xa9\x1f\x04\xdf"); /* TurboSHAKE[c=456] */
    selfTestTurboSHAKE(448, flavor, "\xdd\x95\xa4\x01\x27\x39\x15\x88"); /* TurboSHAKE[c=448] */
    selfTestTurboSHAKE(440, flavor, "\xeb\x2f\xe8\x5b\x64\xec\x68\x92"); /* TurboSHAKE[c=440] */
    selfTestTurboSHAKE(432, flavor, "\x2f\x31\x9d\xd0\xe5\xca\x67\x4f"); /* TurboSHAKE[c=432] */
    selfTestTurboSHAKE(424, flavor, "\x1a\x28\x33\x98\x24\x6b\xc4\x65"); /* TurboSHAKE[c=424] */
    selfTestTurboSHAKE(416, flavor, "\x04\x4e\x18\xe0\x12\x9c\xd9\x2d"); /* TurboSHAKE[c=416] */
    selfTestTurboSHAKE(408, flavor, "\x8c\x72\x0a\x32\x1e\xef\x84\xcc"); /* TurboSHAKE[c=408] */
    selfTestTurboSHAKE(400, flavor, "\x7c\xa4\x72\xf8\x22\x06\x80\xe0"); /* TurboSHAKE[c=400] */
    selfTestTurboSHAKE(392, flavor, "\x45\x3c\x03\x44\x5e\xd3\xcc\x50"); /* TurboSHAKE[c=392] */
    selfTestTurboSHAKE(384, flavor, "\xc8\x09\x03\x09\x77\xec\x73\x71"); /* TurboSHAKE[c=384] */
    selfTestTurboSHAKE(376, flavor, "\xb9\xe0\xa1\x2c\x5d\xd6\x4c\xe1"); /* TurboSHAKE[c=376] */
    selfTestTurboSHAKE(368, flavor, "\xd3\x45\xe9\x81\x79\xc4\x6a\x62"); /* TurboSHAKE[c=368] */
    selfTestTurboSHAKE(360, flavor, "\x1c\xeb\x6a\x25\xd4\x5e\x4e\x51"); /* TurboSHAKE[c=360] */
    selfTestTurboSHAKE(352, flavor, "\xa5\xfb\x53\xe2\xd9\x4c\xc0\x15"); /* TurboSHAKE[c=352] */
    selfTestTurboSHAKE(344, flavor, "\xff\x9b\x1b\x6d\x30\x31\x8b\xfe"); /* TurboSHAKE[c=344] */
    selfTestTurboSHAKE(336, flavor, "\x11\xb0\xf7\x4d\xf0\x53\x3a\x50"); /* TurboSHAKE[c=336] */
    selfTestTurboSHAKE(328, flavor, "\x28\x6f\xf4\x71\xa8\x4f\x2c\xaa"); /* TurboSHAKE[c=328] */
    selfTestTurboSHAKE(320, flavor, "\x4f\x13\x68\x21\x4e\x15\x98\x9e"); /* TurboSHAKE[c=320] */
    selfTestTurboSHAKE(312, flavor, "\x82\xe0\xba\x51\x30\x72\xeb\xdd"); /* TurboSHAKE[c=312] */
    selfTestTurboSHAKE(304, flavor, "\x96\xb1\x43\x2d\x12\x2d\x9a\xf7"); /* TurboSHAKE[c=304] */
    selfTestTurboSHAKE(296, flavor, "\x8e\xd0\x3d\x09\xaf\x61\x9d\x65"); /* TurboSHAKE[c=296] */
    selfTestTurboSHAKE(288, flavor, "\xde\x00\x56\xbe\x64\x0e\xbe\x20"); /* TurboSHAKE[c=288] */
    selfTestTurboSHAKE(280, flavor, "\x10\x0d\xd7\x81\x65\x94\x3a\xc3"); /* TurboSHAKE[c=280] */
    selfTestTurboSHAKE(272, flavor, "\xd5\x64\xb1\xf3\xa8\x3f\xc8\xdd"); /* TurboSHAKE[c=272] */
    selfTestTurboSHAKE(264, flavor, "\xd8\xe3\x4e\xaa\x03\x38\x66\x17"); /* TurboSHAKE[c=264] */
    selfTestTurboSHAKE(256, flavor, "\xb6\x34\x08\xef\x1e\x12\x02\x50"); /* TurboSHAKE128 */
    selfTestTurboSHAKE(248, flavor, "\x16\xcd\x24\x09\x4f\xf0\x26\xda"); /* TurboSHAKE[c=248] */
    selfTestTurboSHAKE(240, flavor, "\x2d\x5b\xab\xc5\x44\x6e\xc6\x4f"); /* TurboSHAKE[c=240] */
    selfTestTurboSHAKE(232, flavor, "\x0f\x07\x40\xc8\x42\x39\xdb\xb8"); /* TurboSHAKE[c=232] */
    selfTestTurboSHAKE(224, flavor, "\xdf\x51\xc3\xd5\x8b\x76\x3c\x32"); /* TurboSHAKE[c=224] */
    selfTestTurboSHAKE(216, flavor, "\x59\x34\xc5\xbb\x58\x5e\x8f\xbd"); /* TurboSHAKE[c=216] */
    selfTestTurboSHAKE(208, flavor, "\x55\x9f\x34\xa6\x68\x1f\xa5\xd6"); /* TurboSHAKE[c=208] */
    selfTestTurboSHAKE(200, flavor, "\xed\xb7\x57\x0c\x34\x85\xe8\x0d"); /* TurboSHAKE[c=200] */
    selfTestTurboSHAKE(192, flavor, "\x6f\xbf\x58\xb0\x2a\x38\x90\x5c"); /* TurboSHAKE[c=192] */
    selfTestTurboSHAKE(184, flavor, "\x97\xc8\x93\xa6\xb3\x3b\xd4\xc0"); /* TurboSHAKE[c=184] */
    selfTestTurboSHAKE(176, flavor, "\x8a\xac\x65\x53\xbb\x25\xb4\x79"); /* TurboSHAKE[c=176] */
    selfTestTurboSHAKE(168, flavor, "\x38\x59\xcf\x97\x5f\xca\xee\xf0"); /* TurboSHAKE[c=168] */
    selfTestTurboSHAKE(160, flavor, "\x98\xef\xe9\xad\x99\xc1\xb2\x98"); /* TurboSHAKE[c=160] */
    selfTestTurboSHAKE(152, flavor, "\x62\x2b\x94\x87\x64\x0a\x8e\x61"); /* TurboSHAKE[c=152] */
    selfTestTurboSHAKE(144, flavor, "\x42\x27\x3e\x9c\x2e\xca\x40\x4c"); /* TurboSHAKE[c=144] */
    selfTestTurboSHAKE(136, flavor, "\x9e\x28\x47\xf7\x11\x75\x7e\x00"); /* TurboSHAKE[c=136] */
    selfTestTurboSHAKE(128, flavor, "\x7b\xb4\x96\x76\x11\x17\x84\x10"); /* TurboSHAKE[c=128] */
    selfTestTurboSHAKE(120, flavor, "\xbd\xb1\xb3\x75\x0e\xf2\x51\x2a"); /* TurboSHAKE[c=120] */
    selfTestTurboSHAKE(112, flavor, "\x54\xc8\x63\x02\x49\xe4\x63\xee"); /* TurboSHAKE[c=112] */
    selfTestTurboSHAKE(104, flavor, "\xd5\xb8\xe8\x16\x1b\x39\x0c\x2f"); /* TurboSHAKE[c=104] */
    selfTestTurboSHAKE( 96, flavor, "\x4d\x94\x3a\xf2\x2e\xd3\x82\x2f"); /* TurboSHAKE[c=96] */
    selfTestTurboSHAKE( 88, flavor, "\x51\xb0\x57\x9c\x5e\xab\x51\x35"); /* TurboSHAKE[c=88] */
    selfTestTurboSHAKE( 80, flavor, "\x13\x8e\x3d\x7b\x97\x10\xf1\x09"); /* TurboSHAKE[c=80] */
    selfTestTurboSHAKE( 72, flavor, "\x23\x81\x43\x92\xc4\x9a\x93\x38"); /* TurboSHAKE[c=72] */
    selfTestTurboSHAKE( 64, flavor, "\x61\x31\xf0\x5b\x9b\xc0\x67\x54"); /* TurboSHAKE[c=64] */
    selfTestTurboSHAKE( 56, flavor, "\x27\x2b\xda\x2d\xb4\xcf\x69\x0f"); /* TurboSHAKE[c=56] */
    selfTestTurboSHAKE( 48, flavor, "\x0e\x6f\xb9\xa1\xe4\xd7\x8c\xe4"); /* TurboSHAKE[c=48] */
    selfTestTurboSHAKE( 40, flavor, "\xe9\xd4\xb4\x2e\x2a\xf0\xdc\x9b"); /* TurboSHAKE[c=40] */
    selfTestTurboSHAKE( 32, flavor, "\xa0\xed\x15\x6f\x06\xbc\x4e\x60"); /* TurboSHAKE[c=32] */
    selfTestTurboSHAKE( 24, flavor, "\x9e\xb4\x43\x3a\xe0\x34\xfb\xd6"); /* TurboSHAKE[c=24] */
    selfTestTurboSHAKE( 16, flavor, "\xb6\xb8\x35\xa6\xd0\xe9\x9f\x16"); /* TurboSHAKE[c=16] */
    selfTestTurboSHAKE(  8, flavor, "\xa4\x65\x48\x48\xd7\xef\x3f\xdf"); /* TurboSHAKE[c=8] */
    selfTestTurboSHAKE(  0, flavor, "\xcc\x9d\xba\x4f\x3c\xc9\xc5\xcc"); /* TurboSHAKE[c=0] */
    UT_endTest();
#endif
    }
}
#endif /* XKCP_has_Sponge_Keccak */
