/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

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

#include <assert.h>
#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "KeccakSponge.h"

#define flavor_OneCall          1
#define flavor_IUF_AllAtOnce    2
#define flavor_IUF_Pieces       3

#ifndef KeccakP200_excluded
    #define prefix KeccakWidth200
    #define SnP_width 200
    #include "testSponge.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP400_excluded
    #define prefix KeccakWidth400
    #define SnP_width 400
    #include "testSponge.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP800_excluded
    #define prefix KeccakWidth800
    #define SnP_width 800
    #include "testSponge.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP1600_excluded
    #define prefix KeccakWidth1600
    #define SnP_width 1600
    #include "testSponge.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef OUTPUT
void writeTestSponge(void)
{
    FILE *f;
    unsigned int rate;

    f = fopen("TestSponge.txt", "w");
    assert(f != NULL);
#ifndef KeccakP200_excluded
    for(rate = 8; rate <= 200; rate += 8)
        KeccakWidth200_writeTestSponge(f, rate, 200-rate);
#endif
#ifndef KeccakP400_excluded
    for(rate = 16; rate <= 400; rate += (rate < 256) ? 16 : 8)
        KeccakWidth400_writeTestSponge(f, rate, 400-rate);
#endif
#ifndef KeccakP800_excluded
    for(rate = 32; rate <= 800; rate += (rate < 512) ? 32 : ((rate < 672) ? 16 : 8))
        KeccakWidth800_writeTestSponge(f, rate, 800-rate);
#endif
#ifndef KeccakP1600_excluded
    for(rate = 64; rate <= 1600; rate += (rate < 1024) ? 64 : ((rate < 1344) ? 32 : 8))
        KeccakWidth1600_writeTestSponge(f, rate, 1600-rate);
#endif
    fclose(f);
}
#endif

void selfTestSponge(unsigned int rate, unsigned int capacity, int flavor, const unsigned char *expected)
{
#ifndef KeccakP200_excluded
    if (rate+capacity == 200)
        KeccakWidth200_selfTestSponge(rate, capacity, flavor, expected);
    else
#endif
#ifndef KeccakP400_excluded
    if (rate+capacity == 400)
        KeccakWidth400_selfTestSponge(rate, capacity, flavor, expected);
    else
#endif
#ifndef KeccakP800_excluded
    if (rate+capacity == 800)
        KeccakWidth800_selfTestSponge(rate, capacity, flavor, expected);
    else
#endif
#ifndef KeccakP1600_excluded
    if (rate+capacity == 1600)
        KeccakWidth1600_selfTestSponge(rate, capacity, flavor, expected);
    else
#endif
        abort();
}

void testSponge()
{
    unsigned int flavor;

#ifdef OUTPUT
    writeTestSponge();
#endif

    for(flavor=1; flavor<=3; flavor++) {
#if !defined(EMBEDDED)
        if (flavor == flavor_OneCall)
            printf("Testing Keccak sponge in one call");
        else if (flavor == flavor_IUF_AllAtOnce)
            printf("Testing Keccak sponge with queue all at once");
        else if (flavor == flavor_IUF_Pieces)
            printf("Testing Keccak sponge with queue in pieces");
        fflush(stdout);
#endif
#ifndef KeccakP200_excluded
#if !defined(EMBEDDED)
    printf(" (width 200)");
    fflush(stdout);
#endif
    selfTestSponge(8, 192, flavor, "\x89\xa3\xbc\x59\xf1\x3d\xe6\x3c"); /* Keccak[r=8, c=192] */
    selfTestSponge(16, 184, flavor, "\xff\xfe\xa7\x51\x25\x17\xaa\x83"); /* Keccak[r=16, c=184] */
    selfTestSponge(24, 176, flavor, "\xbb\x95\x35\x88\xe7\xba\x72\x6b"); /* Keccak[r=24, c=176] */
    selfTestSponge(32, 168, flavor, "\x1d\x34\xe6\xae\x6c\x31\xec\x93"); /* Keccak[r=32, c=168] */
    selfTestSponge(40, 160, flavor, "\xf5\x3e\xbb\xfc\xb5\x0f\x56\xa3"); /* Keccak[r=40, c=160] */
    selfTestSponge(48, 152, flavor, "\x93\x8f\x62\xfb\x50\x3d\x28\x0f"); /* Keccak[r=48, c=152] */
    selfTestSponge(56, 144, flavor, "\xae\x7c\xc1\x74\x83\x4f\x15\x36"); /* Keccak[r=56, c=144] */
    selfTestSponge(64, 136, flavor, "\x70\xc2\xa3\xcb\x7e\x56\x65\xc9"); /* Keccak[r=64, c=136] */
    selfTestSponge(72, 128, flavor, "\x2d\xfe\x2f\x0a\x6e\xfa\x1f\x6a"); /* Keccak[r=72, c=128] */
    selfTestSponge(80, 120, flavor, "\xe6\x42\x74\xe9\x19\xa3\x0e\x9d"); /* Keccak[r=80, c=120] */
    selfTestSponge(88, 112, flavor, "\x29\xfc\x00\x85\x26\x54\xc1\x9d"); /* Keccak[r=88, c=112] */
    selfTestSponge(96, 104, flavor, "\x79\xab\x63\x15\x82\x1c\x70\x40"); /* Keccak[r=96, c=104] */
    selfTestSponge(104, 96, flavor, "\x9e\x8e\xbd\x9f\x33\x16\x80\xf8"); /* Keccak[r=104, c=96] */
    selfTestSponge(112, 88, flavor, "\x96\x53\xe9\x49\x61\x33\x99\xba"); /* Keccak[r=112, c=88] */
    selfTestSponge(120, 80, flavor, "\x58\xe7\x99\xb9\xa8\x1b\x82\xea"); /* Keccak[r=120, c=80] */
    selfTestSponge(128, 72, flavor, "\x11\xb0\xe2\xd0\x5b\xc4\xa4\x6e"); /* Keccak[r=128, c=72] */
    selfTestSponge(136, 64, flavor, "\x53\x35\xd0\x2c\x66\x6d\x6b\x4b"); /* Keccak[r=136, c=64] */
    selfTestSponge(144, 56, flavor, "\x72\xde\xeb\x4f\x77\x8f\x02\x1e"); /* Keccak[r=144, c=56] */
    selfTestSponge(152, 48, flavor, "\xfb\x99\xdd\xc4\x78\x7b\x0e\xd9"); /* Keccak[r=152, c=48] */
    selfTestSponge(160, 40, flavor, "\x67\xbc\xeb\x90\x27\x5a\x04\x24"); /* Keccak[r=160, c=40] */
    selfTestSponge(168, 32, flavor, "\xc2\x55\xbc\xf0\x13\x98\xf2\x84"); /* Keccak[r=168, c=32] */
    selfTestSponge(176, 24, flavor, "\x82\x6c\xed\x43\xec\xf5\x40\xf9"); /* Keccak[r=176, c=24] */
    selfTestSponge(184, 16, flavor, "\xae\x63\x9e\x64\x1b\x91\x14\x3a"); /* Keccak[r=184, c=16] */
    selfTestSponge(192, 8, flavor, "\xfa\x99\x62\x44\x5e\xf3\xca\xed"); /* Keccak[r=192, c=8] */
    selfTestSponge(200, 0, flavor, "\xeb\x35\x59\x76\xd0\x0c\x5b\x5e"); /* Keccak[r=200, c=0] */
#endif
#ifndef KeccakP400_excluded
#if !defined(EMBEDDED)
    printf(" (width 400)");
    fflush(stdout);
#endif
    selfTestSponge(16, 384, flavor, "\xdb\xc2\x15\x03\x90\x30\xd6\xc6"); /* Keccak[r=16, c=384] */
    selfTestSponge(32, 368, flavor, "\xae\x6d\x20\xdd\x20\x86\x91\xab"); /* Keccak[r=32, c=368] */
    selfTestSponge(48, 352, flavor, "\xe2\x43\x83\x3f\x35\x89\xf4\xfb"); /* Keccak[r=48, c=352] */
    selfTestSponge(64, 336, flavor, "\x4a\xa2\x20\x4e\xd5\xb9\x2d\xd0"); /* Keccak[r=64, c=336] */
    selfTestSponge(80, 320, flavor, "\x82\x10\xcb\xbd\x71\x8c\x35\x64"); /* Keccak[r=80, c=320] */
    selfTestSponge(96, 304, flavor, "\xc3\x46\xe4\x13\x0f\xbf\x38\x3e"); /* Keccak[r=96, c=304] */
    selfTestSponge(112, 288, flavor, "\xf7\x4b\xa0\xa0\xb8\x75\x7c\x23"); /* Keccak[r=112, c=288] */
    selfTestSponge(128, 272, flavor, "\x4c\xc2\x3f\x20\xc7\x1e\x7a\x41"); /* Keccak[r=128, c=272] */
    selfTestSponge(144, 256, flavor, "\x69\x15\x6f\x45\xf4\x31\x0d\x50"); /* Keccak[r=144, c=256] */
    selfTestSponge(160, 240, flavor, "\x41\xf9\xd7\xd6\xb6\x55\xf7\x92"); /* Keccak[r=160, c=240] */
    selfTestSponge(176, 224, flavor, "\xc2\xcb\x35\xf6\xbd\x12\xa8\x4e"); /* Keccak[r=176, c=224] */
    selfTestSponge(192, 208, flavor, "\x0f\x38\x1a\x5b\x2c\x07\xb9\x0e"); /* Keccak[r=192, c=208] */
    selfTestSponge(208, 192, flavor, "\x93\xa0\x71\xe7\x15\x68\x1d\x0e"); /* Keccak[r=208, c=192] */
    selfTestSponge(224, 176, flavor, "\x32\x0e\xb5\x52\x87\x8d\xbe\x76"); /* Keccak[r=224, c=176] */
    selfTestSponge(240, 160, flavor, "\xc4\x79\x03\x53\xb6\x50\x37\x26"); /* Keccak[r=240, c=160] */
    selfTestSponge(256, 144, flavor, "\x2a\x52\x54\xb9\x12\xad\x84\x33"); /* Keccak[r=256, c=144] */
    selfTestSponge(264, 136, flavor, "\x98\xe0\x64\x69\xc1\x5c\x69\x82"); /* Keccak[r=264, c=136] */
    selfTestSponge(272, 128, flavor, "\x4a\xe0\x64\xbe\x0f\x0d\xc2\x85"); /* Keccak[r=272, c=128] */
    selfTestSponge(280, 120, flavor, "\xdb\x08\xce\x1a\xcd\x24\xc3\x33"); /* Keccak[r=280, c=120] */
    selfTestSponge(288, 112, flavor, "\xcc\x78\x9a\x7e\xeb\x2c\xe2\x09"); /* Keccak[r=288, c=112] */
    selfTestSponge(296, 104, flavor, "\xa5\x8f\xb3\x0f\x36\xa4\xdc\x75"); /* Keccak[r=296, c=104] */
    selfTestSponge(304, 96, flavor, "\x2d\x88\xdd\x28\xca\xfb\xba\xa4"); /* Keccak[r=304, c=96] */
    selfTestSponge(312, 88, flavor, "\x4e\x2c\x17\x38\xbe\x72\xb7\x28"); /* Keccak[r=312, c=88] */
    selfTestSponge(320, 80, flavor, "\x30\x37\x4f\x8e\x81\xbb\xa6\x4b"); /* Keccak[r=320, c=80] */
    selfTestSponge(328, 72, flavor, "\x12\x9c\x73\x40\x3b\xa6\xde\x79"); /* Keccak[r=328, c=72] */
    selfTestSponge(336, 64, flavor, "\xe0\x94\xc1\x97\x50\x82\x59\x69"); /* Keccak[r=336, c=64] */
    selfTestSponge(344, 56, flavor, "\xf4\x9b\x9e\xfa\x5d\x78\x4b\x97"); /* Keccak[r=344, c=56] */
    selfTestSponge(352, 48, flavor, "\xe3\x87\x5d\x00\xcf\x47\x66\x37"); /* Keccak[r=352, c=48] */
    selfTestSponge(360, 40, flavor, "\x50\x62\x4f\x90\x0e\x8a\xf6\x19"); /* Keccak[r=360, c=40] */
    selfTestSponge(368, 32, flavor, "\x7d\x4a\x29\x92\x24\x57\x79\x4e"); /* Keccak[r=368, c=32] */
    selfTestSponge(376, 24, flavor, "\x01\x7f\xc2\x43\x8c\x81\x47\x1f"); /* Keccak[r=376, c=24] */
    selfTestSponge(384, 16, flavor, "\xe8\x03\x8a\x89\x64\x5d\x18\xbf"); /* Keccak[r=384, c=16] */
    selfTestSponge(392, 8, flavor, "\xd5\xd4\x1d\x6e\x4e\x41\x60\x0e"); /* Keccak[r=392, c=8] */
    selfTestSponge(400, 0, flavor, "\xe3\x77\x7e\x01\x55\xf6\xe7\xc5"); /* Keccak[r=400, c=0] */
#endif
#ifndef KeccakP800_excluded
#if !defined(EMBEDDED)
    printf(" (width 800)");
    fflush(stdout);
#endif
    selfTestSponge(32, 768, flavor, "\xd6\x0a\x95\x77\xb8\x75\x75\xab"); /* Keccak[r=32, c=768] */
    selfTestSponge(64, 736, flavor, "\xb7\xb8\xeb\xe0\x28\xa8\x73\xca"); /* Keccak[r=64, c=736] */
    selfTestSponge(96, 704, flavor, "\x33\x42\x97\xb1\xa0\xe5\x67\x53"); /* Keccak[r=96, c=704] */
    selfTestSponge(128, 672, flavor, "\x7b\x82\x9f\x68\x84\xfa\xc0\x6d"); /* Keccak[r=128, c=672] */
    selfTestSponge(160, 640, flavor, "\x50\x75\x7a\x5b\xfb\x25\x3a\xa6"); /* Keccak[r=160, c=640] */
    selfTestSponge(192, 608, flavor, "\x5f\xeb\x46\xa5\xb1\x9e\xfe\x4b"); /* Keccak[r=192, c=608] */
    selfTestSponge(224, 576, flavor, "\x4f\x51\x7f\xa1\xd0\x2c\x9c\xa7"); /* Keccak[r=224, c=576] */
    selfTestSponge(256, 544, flavor, "\xa3\xe5\x45\x32\x3e\xf9\x45\x1e"); /* Keccak[r=256, c=544] */
    selfTestSponge(288, 512, flavor, "\x5d\x21\x41\xa9\xaa\x82\x4e\x6f"); /* Keccak[r=288, c=512] */
    selfTestSponge(320, 480, flavor, "\x6e\xfe\x0d\x9f\x54\x25\x17\x5c"); /* Keccak[r=320, c=480] */
    selfTestSponge(352, 448, flavor, "\xd9\xf0\x5c\x2a\x46\x8b\xde\x4e"); /* Keccak[r=352, c=448] */
    selfTestSponge(384, 416, flavor, "\x3f\x77\x76\x51\x88\x47\x1a\x27"); /* Keccak[r=384, c=416] */
    selfTestSponge(416, 384, flavor, "\x4c\x49\xa8\x0e\xe3\x85\x77\x7e"); /* Keccak[r=416, c=384] */
    selfTestSponge(448, 352, flavor, "\x52\x4a\x09\x6d\x3d\xeb\x61\xef"); /* Keccak[r=448, c=352] */
    selfTestSponge(480, 320, flavor, "\xc2\x5b\x87\x6a\xea\x03\xca\x33"); /* Keccak[r=480, c=320] */
    selfTestSponge(512, 288, flavor, "\x9f\x00\x4b\xb5\x49\xa0\xab\xd9"); /* Keccak[r=512, c=288] */
    selfTestSponge(528, 272, flavor, "\xa8\x52\x65\x82\x76\xeb\x4c\xd5"); /* Keccak[r=528, c=272] */
    selfTestSponge(544, 256, flavor, "\xb3\xaa\x84\xe8\x8f\x90\x43\x7d"); /* Keccak[r=544, c=256] */
    selfTestSponge(560, 240, flavor, "\xcd\xb2\xcd\xf7\x0f\x42\x2c\x48"); /* Keccak[r=560, c=240] */
    selfTestSponge(576, 224, flavor, "\xcb\xf0\x5a\xc0\x03\x1e\xf7\xb4"); /* Keccak[r=576, c=224] */
    selfTestSponge(592, 208, flavor, "\x68\x1a\x04\x17\xe0\x3b\x2f\x1b"); /* Keccak[r=592, c=208] */
    selfTestSponge(608, 192, flavor, "\xc2\x5f\x72\x4c\x8d\xd4\x28\x83"); /* Keccak[r=608, c=192] */
    selfTestSponge(624, 176, flavor, "\x73\x8f\xca\x50\xf8\x40\xa1\x98"); /* Keccak[r=624, c=176] */
    selfTestSponge(640, 160, flavor, "\xfc\x2e\xa9\x00\x9f\xc5\xbb\x59"); /* Keccak[r=640, c=160] */
    selfTestSponge(656, 144, flavor, "\x3d\x5f\x64\x0a\x33\x15\x05\xb9"); /* Keccak[r=656, c=144] */
    selfTestSponge(672, 128, flavor, "\xc7\x66\x73\x3f\xf0\x11\xeb\xb7"); /* Keccak[r=672, c=128] */
    selfTestSponge(680, 120, flavor, "\x73\x35\xcd\x7b\x6d\x08\x36\xfb"); /* Keccak[r=680, c=120] */
    selfTestSponge(688, 112, flavor, "\xf9\x2e\x7b\x7c\xdf\x7b\x43\x8b"); /* Keccak[r=688, c=112] */
    selfTestSponge(696, 104, flavor, "\x87\xd4\x69\x63\x7c\x28\xd7\xa1"); /* Keccak[r=696, c=104] */
    selfTestSponge(704, 96, flavor, "\xe9\xa1\x6a\x01\x39\x36\x43\x8a"); /* Keccak[r=704, c=96] */
    selfTestSponge(712, 88, flavor, "\x62\x99\xce\x37\xd7\x4b\x83\x60"); /* Keccak[r=712, c=88] */
    selfTestSponge(720, 80, flavor, "\x3d\x9c\xd0\xc5\xd3\x10\x63\x67"); /* Keccak[r=720, c=80] */
    selfTestSponge(728, 72, flavor, "\xd3\xa9\xaa\x2e\x7e\xaa\xb9\x4b"); /* Keccak[r=728, c=72] */
    selfTestSponge(736, 64, flavor, "\xab\xf9\x25\x5f\x89\xe8\x5e\x67"); /* Keccak[r=736, c=64] */
    selfTestSponge(744, 56, flavor, "\x46\xda\x48\xd3\xcf\x64\x2b\x21"); /* Keccak[r=744, c=56] */
    selfTestSponge(752, 48, flavor, "\x7e\x63\x78\x25\xee\xdd\xe5\xf3"); /* Keccak[r=752, c=48] */
    selfTestSponge(760, 40, flavor, "\x6d\x61\x95\x7f\x04\xdf\x2a\x62"); /* Keccak[r=760, c=40] */
    selfTestSponge(768, 32, flavor, "\x5b\xc8\x43\x92\x15\x1f\x33\xcf"); /* Keccak[r=768, c=32] */
    selfTestSponge(776, 24, flavor, "\x0e\xe0\x25\x01\xba\xca\xe6\xbe"); /* Keccak[r=776, c=24] */
    selfTestSponge(784, 16, flavor, "\x2b\xfb\xb8\x66\x0f\xc0\xca\xd0"); /* Keccak[r=784, c=16] */
    selfTestSponge(792, 8, flavor, "\x91\x09\x47\xe0\xe5\xc2\x83\xdd"); /* Keccak[r=792, c=8] */
    selfTestSponge(800, 0, flavor, "\xa2\xff\x58\x44\x7a\x90\xbe\x06"); /* Keccak[r=800, c=0] */
#endif
#ifndef KeccakP1600_excluded
#if !defined(EMBEDDED)
    printf(" (width 1600)");
    fflush(stdout);
#endif
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
#endif
#if !defined(EMBEDDED)
    printf("\n");
#endif
    }
}
