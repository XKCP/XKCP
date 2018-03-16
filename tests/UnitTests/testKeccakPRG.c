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
#include "KeccakPRG.h"

#define flavor_IUF_AllAtOnce    1
#define flavor_IUF_Pieces       2

#ifndef KeccakP200_excluded
    #define prefix KeccakWidth200
    #define SnP_width 200
    #include "testKeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP400_excluded
    #define prefix KeccakWidth400
    #define SnP_width 400
    #include "testKeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP800_excluded
    #define prefix KeccakWidth800
    #define SnP_width 800
    #include "testKeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP1600_excluded
    #define prefix KeccakWidth1600
    #define SnP_width 1600
    #include "testKeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif


#ifdef OUTPUT
void writeTestSpongePRG(void)
{
    FILE *f;
    unsigned int rho;

    f = fopen("TestKeccakPRG.txt", "w");
    assert(f != NULL);
#ifndef KeccakP200_excluded
    for(rho = 1; rho < 200/8; ++rho)
        KeccakWidth200_writeTestSpongePRG(f, rho);
#endif
#ifndef KeccakP400_excluded
    for(rho = 1; rho < 400/8; rho += (rho < 256/8) ? 2 : 1)
        KeccakWidth400_writeTestSpongePRG(f, rho);
#endif
#ifndef KeccakP800_excluded
    for(rho = 1; rho < 800/8; rho += (rho < 512/8) ? 4 : ((rho < 672/8) ? 2 : 1))
        KeccakWidth800_writeTestSpongePRG(f, rho);
#endif
#ifndef KeccakP1600_excluded
    for(rho = 1; rho < 1600/8; rho += (rho < 1024/8) ? 8 : ((rho < 1344/8) ? 4 : 1))
        KeccakWidth1600_writeTestSpongePRG(f, rho);
    KeccakWidth1600_writeTestSpongePRG(f, 1344/8);
    KeccakWidth1600_writeTestSpongePRG(f, 1088/8);
#endif
    fclose(f);
}
#endif

void selfTestSpongePRG(unsigned int rho, unsigned int width, int flavor, const unsigned char *expected)
{
#ifndef KeccakP200_excluded
    if (width == 200)
        KeccakWidth200_selfTestSpongePRG(rho, flavor, expected);
    else
#endif
#ifndef KeccakP400_excluded
    if (width == 400)
        KeccakWidth400_selfTestSpongePRG(rho, flavor, expected);
    else
#endif
#ifndef KeccakP800_excluded
    if (width == 800)
        KeccakWidth800_selfTestSpongePRG(rho, flavor, expected);
    else
#endif
#ifndef KeccakP1600_excluded
    if (width == 1600)
        KeccakWidth1600_selfTestSpongePRG(rho, flavor, expected);
    else
#endif
        abort();
}

void testKeccakPRG(void)
{
    unsigned int flavor;

#ifdef OUTPUT
    writeTestSpongePRG();
#endif

    for(flavor=1; flavor<=2; flavor++) {
#if !defined(EMBEDDED)
        if (flavor == flavor_IUF_AllAtOnce)
            printf("Testing KeccakPRG with Feed/Fetch all at once");
        else if (flavor == flavor_IUF_Pieces)
            printf("Testing KeccakPRG with Feed/Fetch in pieces");
        fflush(stdout);
#endif
#ifndef KeccakP200_excluded
#if !defined(EMBEDDED)
    printf(" (width 200)");
    fflush(stdout);
#endif
    selfTestSpongePRG(1, 200, flavor, "\x0b\xab\xcc\x4e\xe2\x7f\x3a\xf7"); /* Keccak[r=10, c=190] */
    selfTestSpongePRG(2, 200, flavor, "\xaf\x58\x31\x02\xe7\x2e\xee\x17"); /* Keccak[r=18, c=182] */
    selfTestSpongePRG(3, 200, flavor, "\x43\xbb\x5f\x95\x70\xfe\x70\x3b"); /* Keccak[r=26, c=174] */
    selfTestSpongePRG(4, 200, flavor, "\x8c\x3f\x46\x5e\xbd\x8d\x52\xb7"); /* Keccak[r=34, c=166] */
    selfTestSpongePRG(5, 200, flavor, "\x48\x4d\xce\x35\x31\x83\x14\x47"); /* Keccak[r=42, c=158] */
    selfTestSpongePRG(6, 200, flavor, "\x56\x23\xa3\x23\x2e\xc3\xe4\x53"); /* Keccak[r=50, c=150] */
    selfTestSpongePRG(7, 200, flavor, "\x04\xab\x4d\xbe\xfc\x4e\xc0\x5c"); /* Keccak[r=58, c=142] */
    selfTestSpongePRG(8, 200, flavor, "\x02\xd3\xda\x9c\xe4\xd0\xae\x1b"); /* Keccak[r=66, c=134] */
    selfTestSpongePRG(9, 200, flavor, "\x5b\x83\xb9\xa7\x86\xac\x93\xfb"); /* Keccak[r=74, c=126] */
    selfTestSpongePRG(10, 200, flavor, "\x50\x1f\xb4\x41\xa6\x77\x7e\xfb"); /* Keccak[r=82, c=118] */
    selfTestSpongePRG(11, 200, flavor, "\xe2\xc3\x8b\x27\x1a\xc1\x3a\xdc"); /* Keccak[r=90, c=110] */
    selfTestSpongePRG(12, 200, flavor, "\xb4\xd9\xbd\xf0\xc0\xa3\x37\x3b"); /* Keccak[r=98, c=102] */
    selfTestSpongePRG(13, 200, flavor, "\x6c\x73\xe2\x3e\xc6\x17\x98\x39"); /* Keccak[r=106, c=94] */
    selfTestSpongePRG(14, 200, flavor, "\xee\x0d\x5d\xec\x15\x18\xad\x30"); /* Keccak[r=114, c=86] */
    selfTestSpongePRG(15, 200, flavor, "\xf5\x27\x4e\x04\x71\xbf\x98\xb1"); /* Keccak[r=122, c=78] */
    selfTestSpongePRG(16, 200, flavor, "\x7f\xd9\x4b\x7f\xbf\x84\xbf\x01"); /* Keccak[r=130, c=70] */
    selfTestSpongePRG(17, 200, flavor, "\x39\xde\x97\x3d\x25\x0f\x60\x00"); /* Keccak[r=138, c=62] */
    selfTestSpongePRG(18, 200, flavor, "\x31\x69\x51\xfc\x47\x80\x8b\xfa"); /* Keccak[r=146, c=54] */
    selfTestSpongePRG(19, 200, flavor, "\xb8\x35\xb8\x20\x68\x3c\x07\x76"); /* Keccak[r=154, c=46] */
    selfTestSpongePRG(20, 200, flavor, "\x20\x80\xbb\xf7\x5f\x49\x5d\xad"); /* Keccak[r=162, c=38] */
    selfTestSpongePRG(21, 200, flavor, "\xdf\x82\xc6\x8d\xe2\x00\x4c\x60"); /* Keccak[r=170, c=30] */
    selfTestSpongePRG(22, 200, flavor, "\x7b\xd3\xd1\x0e\x94\x3f\x45\x25"); /* Keccak[r=178, c=22] */
    selfTestSpongePRG(23, 200, flavor, "\x27\xf8\x85\x50\x5e\xab\xa7\x0e"); /* Keccak[r=186, c=14] */
    selfTestSpongePRG(24, 200, flavor, "\x07\x84\x82\xd8\x78\x6c\xd3\x70"); /* Keccak[r=194, c=6] */
#endif
#ifndef KeccakP400_excluded
#if !defined(EMBEDDED)
    printf(" (width 400)");
    fflush(stdout);
#endif
    selfTestSpongePRG(1, 400, flavor, "\xf0\xda\x92\xf6\x55\xfc\xa9\x9c"); /* Keccak[r=10, c=390] */
    selfTestSpongePRG(3, 400, flavor, "\x18\x9d\xd2\x59\xef\x7d\x7d\x65"); /* Keccak[r=26, c=374] */
    selfTestSpongePRG(5, 400, flavor, "\x30\xd6\xf0\x99\x33\x5f\x63\xab"); /* Keccak[r=42, c=358] */
    selfTestSpongePRG(7, 400, flavor, "\xa6\x3b\x40\x5d\xc0\x5c\xfb\x6b"); /* Keccak[r=58, c=342] */
    selfTestSpongePRG(9, 400, flavor, "\x11\xd6\x7c\x87\x4b\x4e\xdc\x0d"); /* Keccak[r=74, c=326] */
    selfTestSpongePRG(11, 400, flavor, "\xa2\x9a\x62\x54\xa6\xeb\xed\xaa"); /* Keccak[r=90, c=310] */
    selfTestSpongePRG(13, 400, flavor, "\x83\x2c\x74\x3d\xdd\x81\xd1\x89"); /* Keccak[r=106, c=294] */
    selfTestSpongePRG(15, 400, flavor, "\x1b\x42\x80\x51\x81\xba\x8d\x4f"); /* Keccak[r=122, c=278] */
    selfTestSpongePRG(17, 400, flavor, "\xad\x05\xc4\x08\x12\x1b\xbe\x2b"); /* Keccak[r=138, c=262] */
    selfTestSpongePRG(19, 400, flavor, "\x43\x4a\x19\x54\xc5\xf8\xbc\xcf"); /* Keccak[r=154, c=246] */
    selfTestSpongePRG(21, 400, flavor, "\x90\x95\x76\x22\x84\x91\x75\x98"); /* Keccak[r=170, c=230] */
    selfTestSpongePRG(23, 400, flavor, "\xc5\x2c\xea\xbb\x3c\xa9\x09\x12"); /* Keccak[r=186, c=214] */
    selfTestSpongePRG(25, 400, flavor, "\xf0\x84\x4c\xdd\xad\xbb\x4a\x3a"); /* Keccak[r=202, c=198] */
    selfTestSpongePRG(27, 400, flavor, "\x40\x03\x7f\x7a\x8c\xd5\x85\x4f"); /* Keccak[r=218, c=182] */
    selfTestSpongePRG(29, 400, flavor, "\x77\xb1\x4d\xd3\x25\x6f\xb2\x37"); /* Keccak[r=234, c=166] */
    selfTestSpongePRG(31, 400, flavor, "\xd4\xb6\x4d\x5a\x95\x40\x51\x3d"); /* Keccak[r=250, c=150] */
    selfTestSpongePRG(33, 400, flavor, "\x1f\xb8\x49\xe5\x7f\xbc\x85\x02"); /* Keccak[r=266, c=134] */
    selfTestSpongePRG(34, 400, flavor, "\xb6\x9f\xd8\x74\x51\x48\x68\x2a"); /* Keccak[r=274, c=126] */
    selfTestSpongePRG(35, 400, flavor, "\xac\x01\xac\x43\x3c\x76\xb4\xc9"); /* Keccak[r=282, c=118] */
    selfTestSpongePRG(36, 400, flavor, "\x7d\x3f\x7c\x69\x3c\x6b\x87\x8f"); /* Keccak[r=290, c=110] */
    selfTestSpongePRG(37, 400, flavor, "\x1c\x33\xa7\x27\x03\x72\x76\x0b"); /* Keccak[r=298, c=102] */
    selfTestSpongePRG(38, 400, flavor, "\x8f\xc6\x16\x7e\x64\x44\x8a\x85"); /* Keccak[r=306, c=94] */
    selfTestSpongePRG(39, 400, flavor, "\xaa\x3e\xe9\x52\x49\x04\xf4\x20"); /* Keccak[r=314, c=86] */
    selfTestSpongePRG(40, 400, flavor, "\x26\xc6\x6d\xc1\x73\x9c\x06\x76"); /* Keccak[r=322, c=78] */
    selfTestSpongePRG(41, 400, flavor, "\xb9\x81\x4e\x80\xab\x5e\x53\xb0"); /* Keccak[r=330, c=70] */
    selfTestSpongePRG(42, 400, flavor, "\x59\x0c\x8f\x6e\xde\x27\x47\x3f"); /* Keccak[r=338, c=62] */
    selfTestSpongePRG(43, 400, flavor, "\x06\x2f\x8f\x56\x8e\x8c\x4f\x86"); /* Keccak[r=346, c=54] */
    selfTestSpongePRG(44, 400, flavor, "\xf5\x00\x88\xe9\x8f\x27\xc6\xba"); /* Keccak[r=354, c=46] */
    selfTestSpongePRG(45, 400, flavor, "\x43\xfb\x4c\xc3\xbb\x0c\x39\x16"); /* Keccak[r=362, c=38] */
    selfTestSpongePRG(46, 400, flavor, "\x35\x62\x61\x29\x0a\xff\x4b\xc8"); /* Keccak[r=370, c=30] */
    selfTestSpongePRG(47, 400, flavor, "\xfa\xdb\xf2\x9f\x0e\xbd\x1b\xb0"); /* Keccak[r=378, c=22] */
    selfTestSpongePRG(48, 400, flavor, "\x8c\x56\x44\xf2\x87\x47\xad\xbf"); /* Keccak[r=386, c=14] */
    selfTestSpongePRG(49, 400, flavor, "\x58\xb7\x86\x3f\xdc\x1a\x70\x7f"); /* Keccak[r=394, c=6] */
#endif
#ifndef KeccakP800_excluded
#if !defined(EMBEDDED)
    printf(" (width 800)");
    fflush(stdout);
#endif
    selfTestSpongePRG(1, 800, flavor, "\x11\x0c\x4e\xde\x6f\x8f\x06\xde"); /* Keccak[r=10, c=790] */
    selfTestSpongePRG(5, 800, flavor, "\x4e\xbb\xf6\xd9\xb2\xfd\x29\xd1"); /* Keccak[r=42, c=758] */
    selfTestSpongePRG(9, 800, flavor, "\x2e\x2b\xd8\xa3\xe3\xb3\x0f\xba"); /* Keccak[r=74, c=726] */
    selfTestSpongePRG(13, 800, flavor, "\xb9\x61\xaa\xe8\xa6\xcc\x8f\xe9"); /* Keccak[r=106, c=694] */
    selfTestSpongePRG(17, 800, flavor, "\x72\x10\xc9\xb7\xcf\x30\xbc\xd7"); /* Keccak[r=138, c=662] */
    selfTestSpongePRG(21, 800, flavor, "\x71\x3a\x28\xcc\x34\x3f\x0b\x3b"); /* Keccak[r=170, c=630] */
    selfTestSpongePRG(25, 800, flavor, "\xb8\x54\x83\x25\x2b\xd1\x55\x08"); /* Keccak[r=202, c=598] */
    selfTestSpongePRG(29, 800, flavor, "\x8c\x4c\x3f\xae\x67\xa4\xf5\xdc"); /* Keccak[r=234, c=566] */
    selfTestSpongePRG(33, 800, flavor, "\x48\x2d\x8d\xf2\x54\x0b\xb4\x62"); /* Keccak[r=266, c=534] */
    selfTestSpongePRG(37, 800, flavor, "\xd2\x67\xe0\x23\x91\xc4\xb5\xe6"); /* Keccak[r=298, c=502] */
    selfTestSpongePRG(41, 800, flavor, "\x91\x4d\x4e\xb9\xf9\x60\xf4\x29"); /* Keccak[r=330, c=470] */
    selfTestSpongePRG(45, 800, flavor, "\x30\x3a\x9a\xbb\xdd\xbb\xaf\x5e"); /* Keccak[r=362, c=438] */
    selfTestSpongePRG(49, 800, flavor, "\x16\xb9\x6b\xa8\x8d\x88\x5c\x86"); /* Keccak[r=394, c=406] */
    selfTestSpongePRG(53, 800, flavor, "\x22\x9a\xb4\x4f\x4c\xf5\x14\x05"); /* Keccak[r=426, c=374] */
    selfTestSpongePRG(57, 800, flavor, "\x8d\xc8\xe5\x40\xf9\xc5\xad\xc8"); /* Keccak[r=458, c=342] */
    selfTestSpongePRG(61, 800, flavor, "\x2c\xab\xf5\x8b\x08\x9f\xcd\xf4"); /* Keccak[r=490, c=310] */
    selfTestSpongePRG(65, 800, flavor, "\x7c\x48\x2e\x5a\xfc\xb4\x19\x2a"); /* Keccak[r=522, c=278] */
    selfTestSpongePRG(67, 800, flavor, "\x6a\x66\xf9\xde\xcb\xc2\xb2\xaf"); /* Keccak[r=538, c=262] */
    selfTestSpongePRG(69, 800, flavor, "\x09\xf6\xdf\x48\x86\x19\x3d\xb8"); /* Keccak[r=554, c=246] */
    selfTestSpongePRG(71, 800, flavor, "\x21\xe0\x25\x1e\xda\x7a\xfe\xe7"); /* Keccak[r=570, c=230] */
    selfTestSpongePRG(73, 800, flavor, "\x98\x35\x9f\xc4\x71\x4d\x4c\xb5"); /* Keccak[r=586, c=214] */
    selfTestSpongePRG(75, 800, flavor, "\x7e\x91\xff\x39\xd3\x0d\x5c\x2d"); /* Keccak[r=602, c=198] */
    selfTestSpongePRG(77, 800, flavor, "\xe5\xd1\xac\x7e\x47\x08\x78\x72"); /* Keccak[r=618, c=182] */
    selfTestSpongePRG(79, 800, flavor, "\x5a\x34\x22\x61\x74\xf5\x3b\xa9"); /* Keccak[r=634, c=166] */
    selfTestSpongePRG(81, 800, flavor, "\xca\x1b\xfc\x3c\xf1\x94\xe7\x52"); /* Keccak[r=650, c=150] */
    selfTestSpongePRG(83, 800, flavor, "\x51\x5f\x4c\xe4\x98\x8b\x05\x5f"); /* Keccak[r=666, c=134] */
    selfTestSpongePRG(85, 800, flavor, "\xe0\x9c\xb5\x64\xa7\x84\x63\xbc"); /* Keccak[r=682, c=118] */
    selfTestSpongePRG(86, 800, flavor, "\x52\x8c\x37\x81\x0d\xe7\x13\xcb"); /* Keccak[r=690, c=110] */
    selfTestSpongePRG(87, 800, flavor, "\xfb\x2e\xee\x09\xac\x7f\x88\x33"); /* Keccak[r=698, c=102] */
    selfTestSpongePRG(88, 800, flavor, "\x42\x9d\x4c\x90\x74\xf0\xfd\x7b"); /* Keccak[r=706, c=94] */
    selfTestSpongePRG(89, 800, flavor, "\xd6\xc4\x4f\x2f\x3a\xb0\x0f\x00"); /* Keccak[r=714, c=86] */
    selfTestSpongePRG(90, 800, flavor, "\x6f\x7c\xb8\xd3\xd3\x45\x57\x5b"); /* Keccak[r=722, c=78] */
    selfTestSpongePRG(91, 800, flavor, "\x7a\xee\x97\x0f\x53\x00\x28\xbe"); /* Keccak[r=730, c=70] */
    selfTestSpongePRG(92, 800, flavor, "\xdb\x7d\x5e\xcd\x76\x8d\xa2\xf9"); /* Keccak[r=738, c=62] */
    selfTestSpongePRG(93, 800, flavor, "\x17\x0c\xe0\x07\x14\xe1\x98\x49"); /* Keccak[r=746, c=54] */
    selfTestSpongePRG(94, 800, flavor, "\xbb\x88\xd0\x03\x2d\x9b\xf1\x09"); /* Keccak[r=754, c=46] */
    selfTestSpongePRG(95, 800, flavor, "\xf5\x82\x56\x72\xd2\xc4\xa2\x9f"); /* Keccak[r=762, c=38] */
    selfTestSpongePRG(96, 800, flavor, "\x76\xa0\xbb\x95\x59\x3c\x30\xef"); /* Keccak[r=770, c=30] */
    selfTestSpongePRG(97, 800, flavor, "\xa7\x32\xa2\x14\x58\x57\x54\xe5"); /* Keccak[r=778, c=22] */
    selfTestSpongePRG(98, 800, flavor, "\x25\xf3\xe2\xab\xe4\xd0\xbe\xfe"); /* Keccak[r=786, c=14] */
    selfTestSpongePRG(99, 800, flavor, "\x82\xe4\x94\x0b\x0c\xa0\x87\xbd"); /* Keccak[r=794, c=6] */
#endif
#ifndef KeccakP1600_excluded
#if !defined(EMBEDDED)
    printf(" (width 1600)");
    fflush(stdout);
#endif
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
#endif
#if !defined(EMBEDDED)
    printf("\n");
#endif
    }
}
