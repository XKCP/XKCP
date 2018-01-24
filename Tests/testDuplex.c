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
#define OUTPUT
/* #define VERBOSE */
#endif

#include <assert.h>
#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "KeccakDuplex.h"
#include "KeccakSponge.h"

#define flavor_DuplexingOnly    1
#define flavor_PartialIO        2
#define flavor_OverwriteAndAdd  3

#ifndef KeccakP200_excluded
    #define prefix KeccakWidth200
    #define SnP_width 200
    #include "testDuplex.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP400_excluded
    #define prefix KeccakWidth400
    #define SnP_width 400
    #include "testDuplex.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP800_excluded
    #define prefix KeccakWidth800
    #define SnP_width 800
    #include "testDuplex.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP1600_excluded
    #define prefix KeccakWidth1600
    #define SnP_width 1600
    #include "testDuplex.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifdef OUTPUT
void writeTestDuplex(void)
{
    FILE *f;
    unsigned int rate;

    f = fopen("TestDuplex.txt", "w");
    assert(f != NULL);
#ifndef KeccakP200_excluded
    for(rate = 3; rate <= 200-2; rate += (rate < 68) ? 1 : ((rate < 220) ? 5 : 25))
        KeccakWidth200_writeTestDuplex(f, rate, 200-rate, flavor_DuplexingOnly);
#endif
#ifndef KeccakP400_excluded
    for(rate = 3; rate <= 400-2; rate += (rate < 68) ? 1 : ((rate < 220) ? 5 : 25))
        KeccakWidth400_writeTestDuplex(f, rate, 400-rate, flavor_DuplexingOnly);
#endif
#ifndef KeccakP800_excluded
    for(rate = 3; rate <= 800-2; rate += (rate < 68) ? 1 : ((rate < 220) ? 5 : 25))
        KeccakWidth800_writeTestDuplex(f, rate, 800-rate, flavor_DuplexingOnly);
#endif
#ifndef KeccakP1600_excluded
    for(rate = 3; rate <= 1600-2; rate += (rate < 68) ? 1 : ((rate < 220) ? 5 : 25))
        KeccakWidth1600_writeTestDuplex(f, rate, 1600-rate, flavor_DuplexingOnly);
#endif
    fclose(f);
}
#endif

void selfTestDuplex(unsigned int rate, unsigned int capacity, int flavor, const unsigned char *expected)
{
#ifndef KeccakP200_excluded
    if (rate+capacity == 200)
        KeccakWidth200_selfTestDuplex(rate, capacity, flavor, expected);
    else
#endif
#ifndef KeccakP400_excluded
    if (rate+capacity == 400)
        KeccakWidth400_selfTestDuplex(rate, capacity, flavor, expected);
    else
#endif
#ifndef KeccakP800_excluded
    if (rate+capacity == 800)
        KeccakWidth800_selfTestDuplex(rate, capacity, flavor, expected);
    else
#endif
#ifndef KeccakP1600_excluded
    if (rate+capacity == 1600)
        KeccakWidth1600_selfTestDuplex(rate, capacity, flavor, expected);
    else
#endif
        abort();
}

void testDuplex()
{
    unsigned int flavor;

#ifdef OUTPUT
    writeTestDuplex();
#endif

    for(flavor=1; flavor<=3; flavor++) {
#if !defined(EMBEDDED)
        if (flavor == flavor_DuplexingOnly)
            printf("Testing Keccak duplexing in one call");
        else if (flavor == flavor_PartialIO)
            printf("Testing Keccak duplexing with partial input/output");
        else if (flavor == flavor_OverwriteAndAdd)
            printf("Testing Keccak duplexing with overwrite and add");
        fflush(stdout);
#endif
#ifndef KeccakP200_excluded
#if !defined(EMBEDDED)
    printf(" (width 200)");
    fflush(stdout);
#endif
    selfTestDuplex(3, 197, flavor, "\x5d\x7f\xfc\xa6\x5e\x89\xb1\x24"); /* Keccak[r=3, c=197] */
    selfTestDuplex(4, 196, flavor, "\x27\xee\x04\xc6\xac\x95\x6d\x23"); /* Keccak[r=4, c=196] */
    selfTestDuplex(5, 195, flavor, "\x28\x54\xa8\xbb\xc6\x9a\x51\xe3"); /* Keccak[r=5, c=195] */
    selfTestDuplex(6, 194, flavor, "\x9a\x8c\x3b\x23\x81\x37\x53\x69"); /* Keccak[r=6, c=194] */
    selfTestDuplex(7, 193, flavor, "\xac\x16\x2e\x3b\xd8\x56\x0f\xfd"); /* Keccak[r=7, c=193] */
    selfTestDuplex(8, 192, flavor, "\x03\x64\xe3\xca\xc8\xd9\x01\x04"); /* Keccak[r=8, c=192] */
    selfTestDuplex(9, 191, flavor, "\xe3\x2f\xaa\x22\xd3\x21\xea\x23"); /* Keccak[r=9, c=191] */
    selfTestDuplex(10, 190, flavor, "\x72\xcd\x7a\xb2\x31\xa7\x64\x62"); /* Keccak[r=10, c=190] */
    selfTestDuplex(11, 189, flavor, "\x21\xd6\xa0\xc7\x5a\xd2\xfd\xf3"); /* Keccak[r=11, c=189] */
    selfTestDuplex(12, 188, flavor, "\x9d\x12\x6f\x13\xbd\xc1\x57\x8b"); /* Keccak[r=12, c=188] */
    selfTestDuplex(13, 187, flavor, "\xf1\x71\x17\x5b\x73\xc4\x55\x63"); /* Keccak[r=13, c=187] */
    selfTestDuplex(14, 186, flavor, "\x00\x12\x24\xd5\xdb\x6c\xce\x93"); /* Keccak[r=14, c=186] */
    selfTestDuplex(15, 185, flavor, "\x1d\x82\x44\x23\x6e\x3b\xe0\x76"); /* Keccak[r=15, c=185] */
    selfTestDuplex(16, 184, flavor, "\xdb\xe0\x51\xeb\xde\x65\x5c\x85"); /* Keccak[r=16, c=184] */
    selfTestDuplex(17, 183, flavor, "\x5e\xe8\xc0\xb5\xad\x7f\x06\x19"); /* Keccak[r=17, c=183] */
    selfTestDuplex(18, 182, flavor, "\x2d\xf0\x24\x88\x10\xac\x7e\xc7"); /* Keccak[r=18, c=182] */
    selfTestDuplex(19, 181, flavor, "\x37\x46\x08\x9d\x35\x5e\x6d\xea"); /* Keccak[r=19, c=181] */
    selfTestDuplex(20, 180, flavor, "\x36\x28\x7c\x93\x8b\xa5\x83\x4d"); /* Keccak[r=20, c=180] */
    selfTestDuplex(21, 179, flavor, "\x7c\x07\xc8\xac\x08\xb7\x98\xa2"); /* Keccak[r=21, c=179] */
    selfTestDuplex(22, 178, flavor, "\x1f\x9b\x8f\x21\x24\x01\xe8\x98"); /* Keccak[r=22, c=178] */
    selfTestDuplex(23, 177, flavor, "\xc0\x12\x0a\x80\xeb\x03\x23\x0f"); /* Keccak[r=23, c=177] */
    selfTestDuplex(24, 176, flavor, "\x0a\x39\xdf\x37\x28\x14\x07\x7b"); /* Keccak[r=24, c=176] */
    selfTestDuplex(25, 175, flavor, "\xb9\xb6\xec\xad\x6a\x2c\x99\x85"); /* Keccak[r=25, c=175] */
    selfTestDuplex(26, 174, flavor, "\xc6\xa3\xc1\x3b\x2a\x52\xd9\xee"); /* Keccak[r=26, c=174] */
    selfTestDuplex(27, 173, flavor, "\xe7\x3c\xe8\xdc\x47\xcd\xcf\x5a"); /* Keccak[r=27, c=173] */
    selfTestDuplex(28, 172, flavor, "\x74\x2a\x8d\xff\x07\xd8\x82\xa0"); /* Keccak[r=28, c=172] */
    selfTestDuplex(29, 171, flavor, "\x63\x34\x9f\xd2\x8f\x59\x3f\x99"); /* Keccak[r=29, c=171] */
    selfTestDuplex(30, 170, flavor, "\x56\x9b\x37\x00\x67\xfc\xa5\xd0"); /* Keccak[r=30, c=170] */
    selfTestDuplex(31, 169, flavor, "\x3f\x99\x50\xff\x6a\xda\xe9\x1c"); /* Keccak[r=31, c=169] */
    selfTestDuplex(32, 168, flavor, "\x4e\xa2\xf4\xa7\x04\x73\xe3\x8f"); /* Keccak[r=32, c=168] */
    selfTestDuplex(33, 167, flavor, "\x06\x90\x09\xb3\x3f\x3d\x8f\x24"); /* Keccak[r=33, c=167] */
    selfTestDuplex(34, 166, flavor, "\x3a\xf5\xcb\x8c\x11\x39\x22\xb0"); /* Keccak[r=34, c=166] */
    selfTestDuplex(35, 165, flavor, "\x8a\x8c\xfc\x3d\x0d\x97\x11\xbd"); /* Keccak[r=35, c=165] */
    selfTestDuplex(36, 164, flavor, "\x08\xea\xf3\x72\xf1\xe6\x42\x66"); /* Keccak[r=36, c=164] */
    selfTestDuplex(37, 163, flavor, "\xcc\xfb\x35\x1c\xcc\x47\x96\x4b"); /* Keccak[r=37, c=163] */
    selfTestDuplex(38, 162, flavor, "\x47\x7b\xe0\x8f\xc4\x9c\xf1\xe8"); /* Keccak[r=38, c=162] */
    selfTestDuplex(39, 161, flavor, "\xe5\xc8\x96\x90\xb7\xb7\xf1\xfc"); /* Keccak[r=39, c=161] */
    selfTestDuplex(40, 160, flavor, "\xaf\xc8\xa4\xfe\xb2\x37\xe3\x85"); /* Keccak[r=40, c=160] */
    selfTestDuplex(41, 159, flavor, "\x2b\x6b\x9b\x5d\xf3\xda\xc2\xb4"); /* Keccak[r=41, c=159] */
    selfTestDuplex(42, 158, flavor, "\x87\x26\x61\x59\x93\xdb\x2d\x59"); /* Keccak[r=42, c=158] */
    selfTestDuplex(43, 157, flavor, "\xb7\x5d\x5f\x60\x70\x6b\x47\xbe"); /* Keccak[r=43, c=157] */
    selfTestDuplex(44, 156, flavor, "\x4e\x0d\xe9\x6a\x0a\x46\x7d\x12"); /* Keccak[r=44, c=156] */
    selfTestDuplex(45, 155, flavor, "\x55\xad\x06\x6a\x0d\x2a\xf2\xfc"); /* Keccak[r=45, c=155] */
    selfTestDuplex(46, 154, flavor, "\x4d\xfe\x8d\x78\x4e\xe8\xd3\x97"); /* Keccak[r=46, c=154] */
    selfTestDuplex(47, 153, flavor, "\x71\xd4\x5f\x3c\xb9\xb7\xbb\x1a"); /* Keccak[r=47, c=153] */
    selfTestDuplex(48, 152, flavor, "\x85\xcf\xf0\x6e\x3d\x13\xc9\x7f"); /* Keccak[r=48, c=152] */
    selfTestDuplex(49, 151, flavor, "\xcf\xa7\x60\xec\x7d\xaf\x28\x0c"); /* Keccak[r=49, c=151] */
    selfTestDuplex(50, 150, flavor, "\x7d\x97\x05\x40\xc8\x1c\x79\xf3"); /* Keccak[r=50, c=150] */
    selfTestDuplex(51, 149, flavor, "\xec\xb1\xd4\x70\xdd\x65\x84\xb2"); /* Keccak[r=51, c=149] */
    selfTestDuplex(52, 148, flavor, "\x76\xcf\xd8\x11\x9a\x9e\x1f\xaf"); /* Keccak[r=52, c=148] */
    selfTestDuplex(53, 147, flavor, "\x86\xa1\xc5\x44\xdc\x3c\xa2\xa7"); /* Keccak[r=53, c=147] */
    selfTestDuplex(54, 146, flavor, "\xcd\xb5\x2e\x2b\xcf\xa7\x84\xfe"); /* Keccak[r=54, c=146] */
    selfTestDuplex(55, 145, flavor, "\x7a\x92\xa2\xc6\x5f\x9a\x43\x7a"); /* Keccak[r=55, c=145] */
    selfTestDuplex(56, 144, flavor, "\x9f\xbc\xd0\x40\x7a\x76\x0f\x62"); /* Keccak[r=56, c=144] */
    selfTestDuplex(57, 143, flavor, "\x75\x73\x78\x15\x66\xa4\x6d\xb2"); /* Keccak[r=57, c=143] */
    selfTestDuplex(58, 142, flavor, "\x45\x67\xbb\x77\x5f\xe4\x5d\x5a"); /* Keccak[r=58, c=142] */
    selfTestDuplex(59, 141, flavor, "\xcc\x4c\xa6\x9e\x2d\xfc\xa7\xc9"); /* Keccak[r=59, c=141] */
    selfTestDuplex(60, 140, flavor, "\xc2\x99\x55\x3e\xe0\xb9\xbb\xd2"); /* Keccak[r=60, c=140] */
    selfTestDuplex(61, 139, flavor, "\xad\xff\xb6\x45\xdb\x20\x3b\x44"); /* Keccak[r=61, c=139] */
    selfTestDuplex(62, 138, flavor, "\x7b\xb4\x0b\xfa\x12\x50\xce\x8d"); /* Keccak[r=62, c=138] */
    selfTestDuplex(63, 137, flavor, "\x67\x80\xa6\x4b\x9f\x91\x6c\xcb"); /* Keccak[r=63, c=137] */
    selfTestDuplex(64, 136, flavor, "\x1d\x29\x62\x62\xad\x28\x10\x12"); /* Keccak[r=64, c=136] */
    selfTestDuplex(65, 135, flavor, "\xa7\x3c\x95\x39\xe6\x6c\xc7\xb6"); /* Keccak[r=65, c=135] */
    selfTestDuplex(66, 134, flavor, "\xe3\xad\xcd\xdc\xfb\x6d\x08\xd7"); /* Keccak[r=66, c=134] */
    selfTestDuplex(67, 133, flavor, "\xde\x67\x7f\x94\xc8\x63\x5a\xee"); /* Keccak[r=67, c=133] */
    selfTestDuplex(68, 132, flavor, "\x43\xe9\x93\xc0\x08\x23\xdc\xe5"); /* Keccak[r=68, c=132] */
    selfTestDuplex(73, 127, flavor, "\x38\x63\xcd\x6b\xf2\xd0\xfe\xae"); /* Keccak[r=73, c=127] */
    selfTestDuplex(78, 122, flavor, "\xa3\xe8\x75\xc4\x2e\x2b\xcb\x8b"); /* Keccak[r=78, c=122] */
    selfTestDuplex(83, 117, flavor, "\xe5\x5d\xda\x9e\x13\xde\x3b\x82"); /* Keccak[r=83, c=117] */
    selfTestDuplex(88, 112, flavor, "\xc5\x4a\x68\x69\x52\x67\x33\xfa"); /* Keccak[r=88, c=112] */
    selfTestDuplex(93, 107, flavor, "\xe4\x4b\x8e\x36\x52\x39\x84\xc8"); /* Keccak[r=93, c=107] */
    selfTestDuplex(98, 102, flavor, "\xae\xd8\xad\x77\x28\x40\x02\x49"); /* Keccak[r=98, c=102] */
    selfTestDuplex(103, 97, flavor, "\x7d\xfe\x47\x49\xce\xd1\x11\x6a"); /* Keccak[r=103, c=97] */
    selfTestDuplex(108, 92, flavor, "\x09\x97\x58\x0c\x42\x8d\x97\xee"); /* Keccak[r=108, c=92] */
    selfTestDuplex(113, 87, flavor, "\x89\x37\xac\x2e\xe0\x5e\x38\x02"); /* Keccak[r=113, c=87] */
    selfTestDuplex(118, 82, flavor, "\xa9\x22\xba\xcb\xd7\x60\xfd\x5a"); /* Keccak[r=118, c=82] */
    selfTestDuplex(123, 77, flavor, "\x94\xff\xc9\x17\xb2\xfd\xb0\xda"); /* Keccak[r=123, c=77] */
    selfTestDuplex(128, 72, flavor, "\x53\x92\x1a\x4e\xbd\x41\xc1\xbe"); /* Keccak[r=128, c=72] */
    selfTestDuplex(133, 67, flavor, "\x58\x2b\x6b\x1c\xa5\xd5\xeb\x80"); /* Keccak[r=133, c=67] */
    selfTestDuplex(138, 62, flavor, "\x3e\xee\xee\x70\x8a\xb4\x37\x7b"); /* Keccak[r=138, c=62] */
    selfTestDuplex(143, 57, flavor, "\xac\xa3\x70\xc8\x75\x2f\xf5\xe8"); /* Keccak[r=143, c=57] */
    selfTestDuplex(148, 52, flavor, "\xe0\xd8\x17\x90\x21\x95\x7a\x61"); /* Keccak[r=148, c=52] */
    selfTestDuplex(153, 47, flavor, "\x83\x07\x7e\xc2\x24\x52\xbd\x22"); /* Keccak[r=153, c=47] */
    selfTestDuplex(158, 42, flavor, "\xb6\xd3\xe7\x5e\x29\xc7\xe4\xde"); /* Keccak[r=158, c=42] */
    selfTestDuplex(163, 37, flavor, "\xc0\xd2\x6f\xa3\xef\xde\x4e\xd0"); /* Keccak[r=163, c=37] */
    selfTestDuplex(168, 32, flavor, "\x1e\xc5\x7b\x75\x39\x8c\xd4\x2a"); /* Keccak[r=168, c=32] */
    selfTestDuplex(173, 27, flavor, "\x50\xc4\x75\x25\x73\xba\x7d\xdd"); /* Keccak[r=173, c=27] */
    selfTestDuplex(178, 22, flavor, "\xfc\x04\x1a\x0b\xe6\xce\xbc\x82"); /* Keccak[r=178, c=22] */
    selfTestDuplex(183, 17, flavor, "\x84\xa6\x26\x89\x17\x87\x4b\x99"); /* Keccak[r=183, c=17] */
    selfTestDuplex(188, 12, flavor, "\x1b\xa5\x04\xbe\x9e\xeb\xf6\xd6"); /* Keccak[r=188, c=12] */
    selfTestDuplex(193, 7, flavor, "\xf6\xd4\x3f\x8f\xe7\xb5\xeb\x76"); /* Keccak[r=193, c=7] */
    selfTestDuplex(198, 2, flavor, "\x1b\x28\xe6\x49\xd8\xdb\xca\xad"); /* Keccak[r=198, c=2] */
#endif
#ifndef KeccakP400_excluded
#if !defined(EMBEDDED)
    printf(" (width 400)");
    fflush(stdout);
#endif
    selfTestDuplex(3, 397, flavor, "\x40\x9c\x3c\x75\x90\x85\x72\x3a"); /* Keccak[r=3, c=397] */
    selfTestDuplex(4, 396, flavor, "\x92\xe4\x34\xe0\x30\x79\x14\xe3"); /* Keccak[r=4, c=396] */
    selfTestDuplex(5, 395, flavor, "\xed\x26\xdd\xba\x82\x74\x44\xe2"); /* Keccak[r=5, c=395] */
    selfTestDuplex(6, 394, flavor, "\xe4\x01\x51\x1f\x0e\xd4\x03\x50"); /* Keccak[r=6, c=394] */
    selfTestDuplex(7, 393, flavor, "\x1a\x1d\xe7\xfa\x2d\xdb\xda\xa8"); /* Keccak[r=7, c=393] */
    selfTestDuplex(8, 392, flavor, "\xea\x99\x55\xa5\x84\xb5\x63\xce"); /* Keccak[r=8, c=392] */
    selfTestDuplex(9, 391, flavor, "\x21\x23\xd0\x66\x7c\x30\x72\x5d"); /* Keccak[r=9, c=391] */
    selfTestDuplex(10, 390, flavor, "\x7a\xc4\xf9\x57\x44\x4a\x30\x65"); /* Keccak[r=10, c=390] */
    selfTestDuplex(11, 389, flavor, "\xc0\x08\x5e\xd8\x44\x88\xbc\x2f"); /* Keccak[r=11, c=389] */
    selfTestDuplex(12, 388, flavor, "\x93\x8b\x50\x4c\x35\x28\xfe\x60"); /* Keccak[r=12, c=388] */
    selfTestDuplex(13, 387, flavor, "\xbe\xc5\x79\xfa\xdc\x97\x48\x2e"); /* Keccak[r=13, c=387] */
    selfTestDuplex(14, 386, flavor, "\xa7\x7f\x2e\xd3\x8b\xc5\x7b\x14"); /* Keccak[r=14, c=386] */
    selfTestDuplex(15, 385, flavor, "\x0e\x2f\x0e\x1d\xa7\xbf\xca\x30"); /* Keccak[r=15, c=385] */
    selfTestDuplex(16, 384, flavor, "\xbf\xa8\xda\x7f\x20\x1f\x66\x4b"); /* Keccak[r=16, c=384] */
    selfTestDuplex(17, 383, flavor, "\x01\x64\x31\xb7\x65\x74\x39\x35"); /* Keccak[r=17, c=383] */
    selfTestDuplex(18, 382, flavor, "\x2c\xf5\x04\x21\xd0\x96\x1c\xbf"); /* Keccak[r=18, c=382] */
    selfTestDuplex(19, 381, flavor, "\x09\xf2\x75\x77\x15\xa1\xdf\x4c"); /* Keccak[r=19, c=381] */
    selfTestDuplex(20, 380, flavor, "\xa8\xe5\xfb\x95\x0a\x54\x23\x95"); /* Keccak[r=20, c=380] */
    selfTestDuplex(21, 379, flavor, "\x2b\xe9\x52\x9a\xfe\x29\xd9\x7a"); /* Keccak[r=21, c=379] */
    selfTestDuplex(22, 378, flavor, "\x2c\x5b\xc2\xb3\xbb\x2f\xcb\x7f"); /* Keccak[r=22, c=378] */
    selfTestDuplex(23, 377, flavor, "\xf8\x46\xf3\x3c\xb2\x85\x60\x1b"); /* Keccak[r=23, c=377] */
    selfTestDuplex(24, 376, flavor, "\x30\x75\x41\x08\x05\x7d\x7a\x9e"); /* Keccak[r=24, c=376] */
    selfTestDuplex(25, 375, flavor, "\x7f\x05\x8f\xf9\xfd\x07\x18\xa4"); /* Keccak[r=25, c=375] */
    selfTestDuplex(26, 374, flavor, "\x2b\x48\x23\xdd\xdd\x66\x06\xd8"); /* Keccak[r=26, c=374] */
    selfTestDuplex(27, 373, flavor, "\x58\x1d\x0e\x5b\x0e\x7a\xd5\x7b"); /* Keccak[r=27, c=373] */
    selfTestDuplex(28, 372, flavor, "\xac\x10\xe9\xe3\xb4\xdc\x0c\xbc"); /* Keccak[r=28, c=372] */
    selfTestDuplex(29, 371, flavor, "\xb0\x01\xd1\x21\x8a\x55\x26\x87"); /* Keccak[r=29, c=371] */
    selfTestDuplex(30, 370, flavor, "\x86\x32\xe4\x42\x93\x37\x8a\x49"); /* Keccak[r=30, c=370] */
    selfTestDuplex(31, 369, flavor, "\x98\xcd\xc2\x8c\x0f\xa0\x25\x1a"); /* Keccak[r=31, c=369] */
    selfTestDuplex(32, 368, flavor, "\x74\xf2\x66\x48\x33\xca\x1a\x6f"); /* Keccak[r=32, c=368] */
    selfTestDuplex(33, 367, flavor, "\x77\xc9\xa4\x46\x08\xb0\xec\x5b"); /* Keccak[r=33, c=367] */
    selfTestDuplex(34, 366, flavor, "\x29\x13\x76\x19\x73\x86\xf4\x94"); /* Keccak[r=34, c=366] */
    selfTestDuplex(35, 365, flavor, "\xf4\xd0\xea\x21\xb2\x64\x5d\xd5"); /* Keccak[r=35, c=365] */
    selfTestDuplex(36, 364, flavor, "\x75\x4c\x22\xf5\xf9\x33\x11\x75"); /* Keccak[r=36, c=364] */
    selfTestDuplex(37, 363, flavor, "\x84\x62\x7a\x62\xb2\x94\x9e\xa5"); /* Keccak[r=37, c=363] */
    selfTestDuplex(38, 362, flavor, "\xa6\xdd\xa6\xb5\xda\xcd\x4b\x3c"); /* Keccak[r=38, c=362] */
    selfTestDuplex(39, 361, flavor, "\x7f\x9d\x07\x0e\x7b\x92\x4e\xfb"); /* Keccak[r=39, c=361] */
    selfTestDuplex(40, 360, flavor, "\x5f\x21\x6c\x6b\x80\xe1\x81\x42"); /* Keccak[r=40, c=360] */
    selfTestDuplex(41, 359, flavor, "\x27\xbf\x8e\xa8\x88\x58\xec\x02"); /* Keccak[r=41, c=359] */
    selfTestDuplex(42, 358, flavor, "\xe6\x24\x49\x24\x94\x39\xfe\x00"); /* Keccak[r=42, c=358] */
    selfTestDuplex(43, 357, flavor, "\xff\x97\x45\x71\x07\xf2\x5c\x67"); /* Keccak[r=43, c=357] */
    selfTestDuplex(44, 356, flavor, "\x64\x4b\xbd\xf1\x1d\x66\xdf\xb2"); /* Keccak[r=44, c=356] */
    selfTestDuplex(45, 355, flavor, "\x69\x04\x16\x30\xf1\x37\x39\x45"); /* Keccak[r=45, c=355] */
    selfTestDuplex(46, 354, flavor, "\x3e\x93\x39\xfb\x56\x49\x79\x7a"); /* Keccak[r=46, c=354] */
    selfTestDuplex(47, 353, flavor, "\xd4\xaa\xe1\xb9\x1c\x60\x96\xab"); /* Keccak[r=47, c=353] */
    selfTestDuplex(48, 352, flavor, "\xba\x7c\xd1\x6c\x66\x7e\x1f\xf2"); /* Keccak[r=48, c=352] */
    selfTestDuplex(49, 351, flavor, "\xf2\x02\x76\xc1\x7e\x3d\x3a\x15"); /* Keccak[r=49, c=351] */
    selfTestDuplex(50, 350, flavor, "\x25\x94\xeb\x12\xab\x82\x10\x49"); /* Keccak[r=50, c=350] */
    selfTestDuplex(51, 349, flavor, "\xb4\x8c\xd4\x03\x5f\x29\xbe\x39"); /* Keccak[r=51, c=349] */
    selfTestDuplex(52, 348, flavor, "\x1c\x80\x67\xe8\xaf\xd3\x97\x2e"); /* Keccak[r=52, c=348] */
    selfTestDuplex(53, 347, flavor, "\x38\x5c\xee\x9d\x93\xa8\x42\xed"); /* Keccak[r=53, c=347] */
    selfTestDuplex(54, 346, flavor, "\xb0\x23\xb9\xa0\x96\x46\x23\x00"); /* Keccak[r=54, c=346] */
    selfTestDuplex(55, 345, flavor, "\x7f\xe3\xfe\x64\x7e\x18\xe7\x63"); /* Keccak[r=55, c=345] */
    selfTestDuplex(56, 344, flavor, "\x13\x77\x02\xeb\xd2\x96\x47\x48"); /* Keccak[r=56, c=344] */
    selfTestDuplex(57, 343, flavor, "\x6f\x5e\x85\xc2\x9a\x1b\x9e\x1b"); /* Keccak[r=57, c=343] */
    selfTestDuplex(58, 342, flavor, "\x1f\x10\x89\x04\xbb\x21\x10\xba"); /* Keccak[r=58, c=342] */
    selfTestDuplex(59, 341, flavor, "\x21\x1c\x8e\x76\xb8\xf5\x55\x60"); /* Keccak[r=59, c=341] */
    selfTestDuplex(60, 340, flavor, "\xe4\xf1\xcf\xf6\xc7\xba\xb7\xaf"); /* Keccak[r=60, c=340] */
    selfTestDuplex(61, 339, flavor, "\xdd\xb3\xaf\xb8\x3f\xad\xaf\xaf"); /* Keccak[r=61, c=339] */
    selfTestDuplex(62, 338, flavor, "\x63\x98\x72\x01\xbc\x99\x2e\x3e"); /* Keccak[r=62, c=338] */
    selfTestDuplex(63, 337, flavor, "\x39\x6b\xe6\x43\x6a\x5e\x18\x1d"); /* Keccak[r=63, c=337] */
    selfTestDuplex(64, 336, flavor, "\x8c\x86\xbb\x63\x09\xf9\x19\x45"); /* Keccak[r=64, c=336] */
    selfTestDuplex(65, 335, flavor, "\xca\x13\x66\xc4\x54\xc0\xba\x93"); /* Keccak[r=65, c=335] */
    selfTestDuplex(66, 334, flavor, "\x10\x01\xf2\x0e\xb5\x87\x34\xd3"); /* Keccak[r=66, c=334] */
    selfTestDuplex(67, 333, flavor, "\x4b\x38\x9e\xcb\x67\x2e\x75\x75"); /* Keccak[r=67, c=333] */
    selfTestDuplex(68, 332, flavor, "\x12\xdb\x8c\xc9\x5c\x3b\x4e\x9a"); /* Keccak[r=68, c=332] */
    selfTestDuplex(73, 327, flavor, "\xe6\xb0\x0c\x42\xfb\x32\x91\x91"); /* Keccak[r=73, c=327] */
    selfTestDuplex(78, 322, flavor, "\xfb\xe8\x36\x3d\x3b\xf3\x02\xba"); /* Keccak[r=78, c=322] */
    selfTestDuplex(83, 317, flavor, "\x80\xc3\x1b\x67\xa6\x69\xe1\xce"); /* Keccak[r=83, c=317] */
    selfTestDuplex(88, 312, flavor, "\x42\xfb\x61\xc3\xc2\x0d\x58\x47"); /* Keccak[r=88, c=312] */
    selfTestDuplex(93, 307, flavor, "\xee\x73\xce\x99\xe0\x50\xfc\x21"); /* Keccak[r=93, c=307] */
    selfTestDuplex(98, 302, flavor, "\xa7\xc5\x1e\xe9\xf0\x43\x26\xba"); /* Keccak[r=98, c=302] */
    selfTestDuplex(103, 297, flavor, "\x3e\x9d\x70\xb5\x2b\xbc\xc6\x00"); /* Keccak[r=103, c=297] */
    selfTestDuplex(108, 292, flavor, "\xea\x06\x17\xcd\x85\xd2\x58\x84"); /* Keccak[r=108, c=292] */
    selfTestDuplex(113, 287, flavor, "\x9d\xa9\xb3\x5a\x70\x05\xee\xc8"); /* Keccak[r=113, c=287] */
    selfTestDuplex(118, 282, flavor, "\xb6\x6e\xc4\xd0\xcd\xf5\xaf\x41"); /* Keccak[r=118, c=282] */
    selfTestDuplex(123, 277, flavor, "\x4d\x0f\x7a\x59\x04\xd0\x71\x05"); /* Keccak[r=123, c=277] */
    selfTestDuplex(128, 272, flavor, "\xfd\xa5\x80\x04\x54\xc4\xc2\x62"); /* Keccak[r=128, c=272] */
    selfTestDuplex(133, 267, flavor, "\xc8\x3c\x5e\x8e\xf3\x53\x76\x6c"); /* Keccak[r=133, c=267] */
    selfTestDuplex(138, 262, flavor, "\xa1\x57\xfa\x38\xf3\x46\xfc\x5c"); /* Keccak[r=138, c=262] */
    selfTestDuplex(143, 257, flavor, "\xc5\x86\x92\xee\x70\x68\x9e\x8b"); /* Keccak[r=143, c=257] */
    selfTestDuplex(148, 252, flavor, "\x0e\x23\x52\x18\x2a\xf6\xfa\x1c"); /* Keccak[r=148, c=252] */
    selfTestDuplex(153, 247, flavor, "\x39\x6e\x0a\xe4\xa6\xf5\x8c\x89"); /* Keccak[r=153, c=247] */
    selfTestDuplex(158, 242, flavor, "\x80\x59\xf8\xd5\xf0\xc6\x9b\xfa"); /* Keccak[r=158, c=242] */
    selfTestDuplex(163, 237, flavor, "\xbc\x03\xa6\x3a\x55\x2b\xf4\xc1"); /* Keccak[r=163, c=237] */
    selfTestDuplex(168, 232, flavor, "\x57\xb1\x3c\x0c\xf8\x5d\x1f\xe2"); /* Keccak[r=168, c=232] */
    selfTestDuplex(173, 227, flavor, "\xbb\x5f\x86\x22\x70\xb2\x98\xc8"); /* Keccak[r=173, c=227] */
    selfTestDuplex(178, 222, flavor, "\x28\xe8\x73\x3e\x38\xa9\xaa\xc1"); /* Keccak[r=178, c=222] */
    selfTestDuplex(183, 217, flavor, "\xc6\xb7\x47\xb6\x99\xe3\xfc\xfb"); /* Keccak[r=183, c=217] */
    selfTestDuplex(188, 212, flavor, "\xde\x77\xfd\x66\x6f\x4f\x31\x5a"); /* Keccak[r=188, c=212] */
    selfTestDuplex(193, 207, flavor, "\xec\x4c\x0d\x7b\x43\x30\x05\x5c"); /* Keccak[r=193, c=207] */
    selfTestDuplex(198, 202, flavor, "\x7a\x13\xfe\xc6\x32\x2c\x66\xe3"); /* Keccak[r=198, c=202] */
    selfTestDuplex(203, 197, flavor, "\xc3\xaf\xa8\x8d\x66\xab\xef\x21"); /* Keccak[r=203, c=197] */
    selfTestDuplex(208, 192, flavor, "\xad\xfb\xdf\xb8\xf6\xbf\x3f\x4b"); /* Keccak[r=208, c=192] */
    selfTestDuplex(213, 187, flavor, "\x90\xa0\x79\x8e\x0e\xc4\xc4\x79"); /* Keccak[r=213, c=187] */
    selfTestDuplex(218, 182, flavor, "\xf6\x71\x27\x11\x37\x2a\x39\x66"); /* Keccak[r=218, c=182] */
    selfTestDuplex(223, 177, flavor, "\x91\x0b\x29\xd1\x72\xee\x3c\x16"); /* Keccak[r=223, c=177] */
    selfTestDuplex(248, 152, flavor, "\x6d\xe1\xd7\x68\x16\x0e\x1a\x74"); /* Keccak[r=248, c=152] */
    selfTestDuplex(273, 127, flavor, "\x94\x13\x50\x94\x71\x95\xcd\xc8"); /* Keccak[r=273, c=127] */
    selfTestDuplex(298, 102, flavor, "\xaa\x93\xe9\x97\xff\x03\x37\x91"); /* Keccak[r=298, c=102] */
    selfTestDuplex(323, 77, flavor, "\x0e\x9e\xc5\x06\xdf\xd3\x28\x22"); /* Keccak[r=323, c=77] */
    selfTestDuplex(348, 52, flavor, "\x1e\x70\x89\xa5\xd2\x6e\xb9\x0a"); /* Keccak[r=348, c=52] */
    selfTestDuplex(373, 27, flavor, "\xb4\x74\xc6\x45\x6d\xd6\x15\x1f"); /* Keccak[r=373, c=27] */
    selfTestDuplex(398, 2, flavor, "\x43\x10\x51\x0d\xdf\xd1\x8d\x15"); /* Keccak[r=398, c=2] */
#endif
#ifndef KeccakP800_excluded
#if !defined(EMBEDDED)
    printf(" (width 800)");
    fflush(stdout);
#endif
    selfTestDuplex(3, 797, flavor, "\x45\xc6\x88\x66\xe4\x16\xd2\xcf"); /* Keccak[r=3, c=797] */
    selfTestDuplex(4, 796, flavor, "\x09\x64\xf1\x47\xf9\xfa\x41\xbe"); /* Keccak[r=4, c=796] */
    selfTestDuplex(5, 795, flavor, "\xe7\x87\xba\x89\x2a\xf0\x39\x26"); /* Keccak[r=5, c=795] */
    selfTestDuplex(6, 794, flavor, "\x3b\x5f\x8f\x61\xe3\x86\x9f\xca"); /* Keccak[r=6, c=794] */
    selfTestDuplex(7, 793, flavor, "\x7a\x6a\xe4\xac\x93\xd7\x27\x36"); /* Keccak[r=7, c=793] */
    selfTestDuplex(8, 792, flavor, "\x11\xa2\x70\x38\x86\x6d\x56\x15"); /* Keccak[r=8, c=792] */
    selfTestDuplex(9, 791, flavor, "\xed\xbe\x98\xee\x26\x6e\x80\xa5"); /* Keccak[r=9, c=791] */
    selfTestDuplex(10, 790, flavor, "\x4c\x35\x3f\x33\x2e\x3a\xab\x36"); /* Keccak[r=10, c=790] */
    selfTestDuplex(11, 789, flavor, "\x56\x15\x7a\x21\xdd\x5c\x41\xd2"); /* Keccak[r=11, c=789] */
    selfTestDuplex(12, 788, flavor, "\x41\x9c\x0d\xff\xe3\xb5\x32\xb8"); /* Keccak[r=12, c=788] */
    selfTestDuplex(13, 787, flavor, "\x5b\xee\x05\xdc\x69\xda\xf5\x58"); /* Keccak[r=13, c=787] */
    selfTestDuplex(14, 786, flavor, "\xe4\xc0\xa5\xc7\x07\xc1\xa1\x2f"); /* Keccak[r=14, c=786] */
    selfTestDuplex(15, 785, flavor, "\xfe\x8c\x0d\xb3\xcd\x74\xf2\x42"); /* Keccak[r=15, c=785] */
    selfTestDuplex(16, 784, flavor, "\x07\xb1\x14\xf6\x6d\xa6\x3f\x0b"); /* Keccak[r=16, c=784] */
    selfTestDuplex(17, 783, flavor, "\x40\x32\xf0\x70\xae\xa2\x16\x23"); /* Keccak[r=17, c=783] */
    selfTestDuplex(18, 782, flavor, "\xc4\x71\xb1\x4a\x23\x71\xf3\x5f"); /* Keccak[r=18, c=782] */
    selfTestDuplex(19, 781, flavor, "\xaa\x33\x66\x84\xac\x74\x08\xc5"); /* Keccak[r=19, c=781] */
    selfTestDuplex(20, 780, flavor, "\xac\x81\x17\xf9\x1a\x9d\x92\x29"); /* Keccak[r=20, c=780] */
    selfTestDuplex(21, 779, flavor, "\x6e\x17\x0e\x5b\x3e\x4b\xc5\xcd"); /* Keccak[r=21, c=779] */
    selfTestDuplex(22, 778, flavor, "\x69\xf0\xfb\x89\x2c\xb0\x3e\x19"); /* Keccak[r=22, c=778] */
    selfTestDuplex(23, 777, flavor, "\x43\xf7\x43\xfb\x52\x22\x90\xb4"); /* Keccak[r=23, c=777] */
    selfTestDuplex(24, 776, flavor, "\xd5\xdb\x5f\x82\x02\x9f\x85\xf6"); /* Keccak[r=24, c=776] */
    selfTestDuplex(25, 775, flavor, "\x6a\xb1\xb2\xda\xde\xb1\x4e\x5e"); /* Keccak[r=25, c=775] */
    selfTestDuplex(26, 774, flavor, "\x1b\x29\x54\xed\xce\x73\x8c\x47"); /* Keccak[r=26, c=774] */
    selfTestDuplex(27, 773, flavor, "\xf1\x71\xb3\x2e\xc1\x7a\x9b\x74"); /* Keccak[r=27, c=773] */
    selfTestDuplex(28, 772, flavor, "\x91\x96\x63\xc1\x60\xfd\x13\xcb"); /* Keccak[r=28, c=772] */
    selfTestDuplex(29, 771, flavor, "\x34\x2f\x08\x49\xf3\xee\xbb\xaa"); /* Keccak[r=29, c=771] */
    selfTestDuplex(30, 770, flavor, "\x03\x4a\x6f\x17\x98\xac\x76\xda"); /* Keccak[r=30, c=770] */
    selfTestDuplex(31, 769, flavor, "\x51\x16\x14\xa4\xa4\xb0\xf9\xc5"); /* Keccak[r=31, c=769] */
    selfTestDuplex(32, 768, flavor, "\xdb\x03\xc3\x0c\xca\x2d\xf0\x02"); /* Keccak[r=32, c=768] */
    selfTestDuplex(33, 767, flavor, "\xb1\x22\x6a\x6e\x73\x0d\x82\x2b"); /* Keccak[r=33, c=767] */
    selfTestDuplex(34, 766, flavor, "\xf1\xcb\x32\xe3\x1d\xac\x83\xec"); /* Keccak[r=34, c=766] */
    selfTestDuplex(35, 765, flavor, "\x4a\x84\x6c\xde\xa2\x73\x92\x45"); /* Keccak[r=35, c=765] */
    selfTestDuplex(36, 764, flavor, "\x43\x27\xf4\x0a\x0c\x52\xd4\xa8"); /* Keccak[r=36, c=764] */
    selfTestDuplex(37, 763, flavor, "\xe1\xeb\x53\xf6\x58\xb9\xb6\x2c"); /* Keccak[r=37, c=763] */
    selfTestDuplex(38, 762, flavor, "\x95\x63\x9d\x01\x98\xad\x5d\x94"); /* Keccak[r=38, c=762] */
    selfTestDuplex(39, 761, flavor, "\x85\x9b\x2e\x17\x3c\x66\x6a\x15"); /* Keccak[r=39, c=761] */
    selfTestDuplex(40, 760, flavor, "\x52\x91\xff\xeb\xe4\x94\x1d\xc7"); /* Keccak[r=40, c=760] */
    selfTestDuplex(41, 759, flavor, "\x95\xa1\x57\x2f\x07\x40\x29\xe5"); /* Keccak[r=41, c=759] */
    selfTestDuplex(42, 758, flavor, "\x2b\x48\xfd\x73\x51\x6b\xe9\xb6"); /* Keccak[r=42, c=758] */
    selfTestDuplex(43, 757, flavor, "\xf9\xa7\x2f\xdb\x3f\x53\x98\x66"); /* Keccak[r=43, c=757] */
    selfTestDuplex(44, 756, flavor, "\x6f\x46\x24\xc8\xe0\x4c\x17\xaf"); /* Keccak[r=44, c=756] */
    selfTestDuplex(45, 755, flavor, "\xe0\x3d\x62\x6a\x46\x44\x3b\x19"); /* Keccak[r=45, c=755] */
    selfTestDuplex(46, 754, flavor, "\x76\x62\x80\xb3\x9c\x3c\x1d\xe9"); /* Keccak[r=46, c=754] */
    selfTestDuplex(47, 753, flavor, "\x7a\x76\xa9\x34\x37\xeb\xb8\x2c"); /* Keccak[r=47, c=753] */
    selfTestDuplex(48, 752, flavor, "\xc1\x6d\x32\x62\xc1\xac\x4a\xe9"); /* Keccak[r=48, c=752] */
    selfTestDuplex(49, 751, flavor, "\x83\x8e\x9a\x31\x18\xee\x79\xe9"); /* Keccak[r=49, c=751] */
    selfTestDuplex(50, 750, flavor, "\x18\xc3\x39\x08\x89\x73\x6d\x85"); /* Keccak[r=50, c=750] */
    selfTestDuplex(51, 749, flavor, "\xa5\xe6\x31\xca\xe7\xd5\x36\xff"); /* Keccak[r=51, c=749] */
    selfTestDuplex(52, 748, flavor, "\xe4\x17\x9a\x5f\x8b\x54\x6b\x78"); /* Keccak[r=52, c=748] */
    selfTestDuplex(53, 747, flavor, "\x64\xbd\xc9\xbf\xd0\x5a\x40\x2c"); /* Keccak[r=53, c=747] */
    selfTestDuplex(54, 746, flavor, "\x9c\xc4\x25\xf8\xbb\x41\x62\x89"); /* Keccak[r=54, c=746] */
    selfTestDuplex(55, 745, flavor, "\xc8\xe8\x5a\x42\x35\x0c\x2f\x8c"); /* Keccak[r=55, c=745] */
    selfTestDuplex(56, 744, flavor, "\x30\x24\x65\x2e\xed\x7a\x47\x32"); /* Keccak[r=56, c=744] */
    selfTestDuplex(57, 743, flavor, "\xe4\x4b\x25\x63\x0b\xa1\x83\x84"); /* Keccak[r=57, c=743] */
    selfTestDuplex(58, 742, flavor, "\xe2\xfc\xf5\x69\xe1\xa2\xca\xba"); /* Keccak[r=58, c=742] */
    selfTestDuplex(59, 741, flavor, "\xdb\xef\xc2\x12\x41\x2e\xa6\x9a"); /* Keccak[r=59, c=741] */
    selfTestDuplex(60, 740, flavor, "\x99\x35\x28\xd9\x11\x9d\xe7\xa4"); /* Keccak[r=60, c=740] */
    selfTestDuplex(61, 739, flavor, "\x57\xd3\x86\xd1\x0f\x34\xd7\x41"); /* Keccak[r=61, c=739] */
    selfTestDuplex(62, 738, flavor, "\xe8\x85\xe6\x67\x37\xbd\x9c\x70"); /* Keccak[r=62, c=738] */
    selfTestDuplex(63, 737, flavor, "\x7c\x91\x3a\x2d\xbd\xd5\x78\x88"); /* Keccak[r=63, c=737] */
    selfTestDuplex(64, 736, flavor, "\x71\xd2\xf4\xfb\x99\xfc\xa7\x45"); /* Keccak[r=64, c=736] */
    selfTestDuplex(65, 735, flavor, "\x2b\x02\x15\xb5\x2e\x27\x48\xb5"); /* Keccak[r=65, c=735] */
    selfTestDuplex(66, 734, flavor, "\xf3\x0c\x8e\x57\xed\x5f\x52\xa2"); /* Keccak[r=66, c=734] */
    selfTestDuplex(67, 733, flavor, "\x4f\xa5\x7c\x7f\x2d\x4e\x7b\x5a"); /* Keccak[r=67, c=733] */
    selfTestDuplex(68, 732, flavor, "\xb9\x83\x86\x57\x5b\xe6\x97\x5d"); /* Keccak[r=68, c=732] */
    selfTestDuplex(73, 727, flavor, "\x7d\xa7\xe4\x56\x2d\xb3\x0c\xaf"); /* Keccak[r=73, c=727] */
    selfTestDuplex(78, 722, flavor, "\xbb\xe5\x5f\x6b\xc3\xce\xaf\x44"); /* Keccak[r=78, c=722] */
    selfTestDuplex(83, 717, flavor, "\x06\x70\xf4\xbf\xa2\x3d\x67\x0a"); /* Keccak[r=83, c=717] */
    selfTestDuplex(88, 712, flavor, "\x43\xaa\x05\x2c\x92\x65\xd9\x65"); /* Keccak[r=88, c=712] */
    selfTestDuplex(93, 707, flavor, "\x7f\xeb\x2d\x96\xf5\x83\xd4\x20"); /* Keccak[r=93, c=707] */
    selfTestDuplex(98, 702, flavor, "\xd7\x3b\x06\x24\x0e\xc8\xad\x9c"); /* Keccak[r=98, c=702] */
    selfTestDuplex(103, 697, flavor, "\x7c\xec\x58\x67\x92\xc3\x04\x86"); /* Keccak[r=103, c=697] */
    selfTestDuplex(108, 692, flavor, "\xff\x89\x30\x13\xeb\xa1\xa2\x49"); /* Keccak[r=108, c=692] */
    selfTestDuplex(113, 687, flavor, "\x68\xd7\xfc\x31\xb1\x64\xd6\x98"); /* Keccak[r=113, c=687] */
    selfTestDuplex(118, 682, flavor, "\x02\x9e\x52\x77\x11\x41\x88\x59"); /* Keccak[r=118, c=682] */
    selfTestDuplex(123, 677, flavor, "\x6f\xb6\xb5\x29\x4e\x08\x8b\x91"); /* Keccak[r=123, c=677] */
    selfTestDuplex(128, 672, flavor, "\x3a\x96\xd8\x9a\xb9\xce\x61\xe5"); /* Keccak[r=128, c=672] */
    selfTestDuplex(133, 667, flavor, "\x26\x79\xb9\x39\x3c\x34\x96\x23"); /* Keccak[r=133, c=667] */
    selfTestDuplex(138, 662, flavor, "\x03\x3e\xb3\xd0\x23\x8f\xd3\xf2"); /* Keccak[r=138, c=662] */
    selfTestDuplex(143, 657, flavor, "\xd9\x90\x41\xf7\x65\x73\x39\x98"); /* Keccak[r=143, c=657] */
    selfTestDuplex(148, 652, flavor, "\x87\xb8\x53\x9e\x8f\x61\xdc\x5b"); /* Keccak[r=148, c=652] */
    selfTestDuplex(153, 647, flavor, "\x0a\xfe\x3f\x28\xe9\xd8\xdb\x85"); /* Keccak[r=153, c=647] */
    selfTestDuplex(158, 642, flavor, "\x2e\x93\xe1\xce\x51\xcd\x35\x66"); /* Keccak[r=158, c=642] */
    selfTestDuplex(163, 637, flavor, "\x55\x5f\x65\xe2\xa7\x39\xc6\xc3"); /* Keccak[r=163, c=637] */
    selfTestDuplex(168, 632, flavor, "\x44\x5f\x3e\xcf\xa2\x47\xdd\xc0"); /* Keccak[r=168, c=632] */
    selfTestDuplex(173, 627, flavor, "\x67\xb2\x1e\xd2\x7b\xfe\x46\x47"); /* Keccak[r=173, c=627] */
    selfTestDuplex(178, 622, flavor, "\x14\x87\x42\xf2\x7f\x3b\x30\x6b"); /* Keccak[r=178, c=622] */
    selfTestDuplex(183, 617, flavor, "\xc0\x01\xc0\x7f\xbf\x38\x2a\x23"); /* Keccak[r=183, c=617] */
    selfTestDuplex(188, 612, flavor, "\xdf\x5f\x92\xfc\x35\xe8\x71\x88"); /* Keccak[r=188, c=612] */
    selfTestDuplex(193, 607, flavor, "\x3c\x71\xb6\xb0\x32\x3c\xd8\x81"); /* Keccak[r=193, c=607] */
    selfTestDuplex(198, 602, flavor, "\xfe\x9d\x0e\x12\xbe\xcc\x2b\x75"); /* Keccak[r=198, c=602] */
    selfTestDuplex(203, 597, flavor, "\xbf\x77\x55\x77\xdb\x0e\xc2\x06"); /* Keccak[r=203, c=597] */
    selfTestDuplex(208, 592, flavor, "\xd2\x76\xcd\x01\x18\x99\x7c\x7f"); /* Keccak[r=208, c=592] */
    selfTestDuplex(213, 587, flavor, "\x03\xf7\x28\x11\xcd\xab\x81\xdd"); /* Keccak[r=213, c=587] */
    selfTestDuplex(218, 582, flavor, "\x4e\x3e\xd4\x73\xfc\x4e\x07\xbc"); /* Keccak[r=218, c=582] */
    selfTestDuplex(223, 577, flavor, "\x5a\x9a\x04\x4d\x26\x82\x87\xa1"); /* Keccak[r=223, c=577] */
    selfTestDuplex(248, 552, flavor, "\x8e\xf2\x42\x3a\x32\xfb\xdf\x1b"); /* Keccak[r=248, c=552] */
    selfTestDuplex(273, 527, flavor, "\xc8\x56\xaf\x20\x0a\x31\x1d\xdf"); /* Keccak[r=273, c=527] */
    selfTestDuplex(298, 502, flavor, "\xab\x3d\x59\x70\x20\x06\x36\x7f"); /* Keccak[r=298, c=502] */
    selfTestDuplex(323, 477, flavor, "\xff\xb0\xa9\xe0\xeb\x46\xba\x54"); /* Keccak[r=323, c=477] */
    selfTestDuplex(348, 452, flavor, "\x0d\xc6\xf4\x68\x69\x1a\x5f\x57"); /* Keccak[r=348, c=452] */
    selfTestDuplex(373, 427, flavor, "\xff\x21\xf9\xb9\xe6\x0c\x65\xf1"); /* Keccak[r=373, c=427] */
    selfTestDuplex(398, 402, flavor, "\x86\x93\xfc\x72\xad\x42\x51\xe9"); /* Keccak[r=398, c=402] */
    selfTestDuplex(423, 377, flavor, "\x5a\x1b\x8d\xd1\x14\x55\xf5\xe5"); /* Keccak[r=423, c=377] */
    selfTestDuplex(448, 352, flavor, "\x65\xff\xe4\x15\xde\xd7\x5a\x92"); /* Keccak[r=448, c=352] */
    selfTestDuplex(473, 327, flavor, "\x9c\x13\x62\xeb\xa9\x07\xa1\xa9"); /* Keccak[r=473, c=327] */
    selfTestDuplex(498, 302, flavor, "\x94\xdc\x09\x74\x03\x49\x3d\x46"); /* Keccak[r=498, c=302] */
    selfTestDuplex(523, 277, flavor, "\xef\x40\x89\x4c\x3c\x32\x24\x9d"); /* Keccak[r=523, c=277] */
    selfTestDuplex(548, 252, flavor, "\x81\x38\x3d\xfc\xb5\x84\x12\x7c"); /* Keccak[r=548, c=252] */
    selfTestDuplex(573, 227, flavor, "\x61\x2d\x86\xd2\x21\xc9\x4b\x1b"); /* Keccak[r=573, c=227] */
    selfTestDuplex(598, 202, flavor, "\xf6\x53\xe2\xee\xe0\x86\x25\x86"); /* Keccak[r=598, c=202] */
    selfTestDuplex(623, 177, flavor, "\xb4\xd3\x7e\x9e\xf9\x22\x12\xaf"); /* Keccak[r=623, c=177] */
    selfTestDuplex(648, 152, flavor, "\xba\xad\xa1\x12\x47\x6a\xd0\xa9"); /* Keccak[r=648, c=152] */
    selfTestDuplex(673, 127, flavor, "\xd0\x0a\x0c\xc7\x0d\xbf\x74\x20"); /* Keccak[r=673, c=127] */
    selfTestDuplex(698, 102, flavor, "\x72\xf0\xba\x6c\xab\xfb\x03\x10"); /* Keccak[r=698, c=102] */
    selfTestDuplex(723, 77, flavor, "\x55\x43\xd8\xfc\xc8\xc4\xa1\x71"); /* Keccak[r=723, c=77] */
    selfTestDuplex(748, 52, flavor, "\xde\x44\x22\x05\xd0\x11\x7d\xaf"); /* Keccak[r=748, c=52] */
    selfTestDuplex(773, 27, flavor, "\x39\x3f\xd0\x31\xcf\xbb\x53\xbf"); /* Keccak[r=773, c=27] */
    selfTestDuplex(798, 2, flavor, "\x06\x5c\x79\xb4\x37\x47\xe7\x68"); /* Keccak[r=798, c=2] */
#endif
#ifndef KeccakP1600_excluded
#if !defined(EMBEDDED)
    printf(" (width 1600)");
    fflush(stdout);
#endif
    selfTestDuplex(3, 1597, flavor, "\x0d\x85\xef\x10\xb7\x9f\x5e\xe0"); /* Keccak[r=3, c=1597] */
    selfTestDuplex(4, 1596, flavor, "\x0f\x9f\xf7\x73\x7d\x20\x3b\x64"); /* Keccak[r=4, c=1596] */
    selfTestDuplex(5, 1595, flavor, "\x50\x86\xe1\x23\xa3\x5e\xd2\xfa"); /* Keccak[r=5, c=1595] */
    selfTestDuplex(6, 1594, flavor, "\x5b\x41\x2c\xe8\xec\xc6\xca\x5e"); /* Keccak[r=6, c=1594] */
    selfTestDuplex(7, 1593, flavor, "\xcc\x5c\xf6\x13\x1a\x90\x56\x2a"); /* Keccak[r=7, c=1593] */
    selfTestDuplex(8, 1592, flavor, "\x20\x7e\x1c\x05\x2e\x3b\xa8\xf8"); /* Keccak[r=8, c=1592] */
    selfTestDuplex(9, 1591, flavor, "\x9a\xcc\xa1\xf4\x97\xc1\x38\x52"); /* Keccak[r=9, c=1591] */
    selfTestDuplex(10, 1590, flavor, "\xa7\x50\x5e\x83\xa1\xab\x5c\xa7"); /* Keccak[r=10, c=1590] */
    selfTestDuplex(11, 1589, flavor, "\xfa\x9a\x24\x02\x81\x7e\xac\x4a"); /* Keccak[r=11, c=1589] */
    selfTestDuplex(12, 1588, flavor, "\x8e\xec\xde\xee\x9b\xbc\xe3\x17"); /* Keccak[r=12, c=1588] */
    selfTestDuplex(13, 1587, flavor, "\x3c\x51\xe6\x12\xc9\x61\xce\x51"); /* Keccak[r=13, c=1587] */
    selfTestDuplex(14, 1586, flavor, "\x88\x27\x45\x7b\xb6\x69\x1d\xd0"); /* Keccak[r=14, c=1586] */
    selfTestDuplex(15, 1585, flavor, "\x23\xaa\x2f\xdc\x9e\x3d\x96\x06"); /* Keccak[r=15, c=1585] */
    selfTestDuplex(16, 1584, flavor, "\x55\x1f\x43\xe9\x80\xed\x8e\x58"); /* Keccak[r=16, c=1584] */
    selfTestDuplex(17, 1583, flavor, "\x53\x16\x86\xa6\x14\x39\xc0\x4b"); /* Keccak[r=17, c=1583] */
    selfTestDuplex(18, 1582, flavor, "\xba\xe1\x97\xf9\x1c\x71\xf9\xf5"); /* Keccak[r=18, c=1582] */
    selfTestDuplex(19, 1581, flavor, "\x93\x2f\x75\x2a\x35\x41\x98\x74"); /* Keccak[r=19, c=1581] */
    selfTestDuplex(20, 1580, flavor, "\x12\xa4\x13\x86\xa1\xea\x45\x5a"); /* Keccak[r=20, c=1580] */
    selfTestDuplex(21, 1579, flavor, "\x9a\x04\x09\x58\x0a\x1a\x94\xb8"); /* Keccak[r=21, c=1579] */
    selfTestDuplex(22, 1578, flavor, "\x2c\x1c\x08\x1f\x6f\xb6\xd7\xad"); /* Keccak[r=22, c=1578] */
    selfTestDuplex(23, 1577, flavor, "\x46\x1c\x7a\x7d\x4d\xc8\x3e\x5a"); /* Keccak[r=23, c=1577] */
    selfTestDuplex(24, 1576, flavor, "\x13\x6e\xc4\x38\x43\xb5\x21\x51"); /* Keccak[r=24, c=1576] */
    selfTestDuplex(25, 1575, flavor, "\x92\x96\x0e\x89\x56\xbd\xff\x78"); /* Keccak[r=25, c=1575] */
    selfTestDuplex(26, 1574, flavor, "\x2d\xb2\x13\x6f\x95\x4f\xbb\xe6"); /* Keccak[r=26, c=1574] */
    selfTestDuplex(27, 1573, flavor, "\xf1\x85\x51\x14\x64\x66\xa4\x70"); /* Keccak[r=27, c=1573] */
    selfTestDuplex(28, 1572, flavor, "\xfe\x58\x23\x0a\xd3\x07\x14\x88"); /* Keccak[r=28, c=1572] */
    selfTestDuplex(29, 1571, flavor, "\x22\xfe\x64\x51\x30\x19\xb2\x6d"); /* Keccak[r=29, c=1571] */
    selfTestDuplex(30, 1570, flavor, "\x30\xbf\xea\xcd\xd0\x02\x56\xe2"); /* Keccak[r=30, c=1570] */
    selfTestDuplex(31, 1569, flavor, "\x5f\xc8\xd0\x81\xae\x5e\x94\x7a"); /* Keccak[r=31, c=1569] */
    selfTestDuplex(32, 1568, flavor, "\x08\x8e\x89\x3b\x12\x8e\x29\x09"); /* Keccak[r=32, c=1568] */
    selfTestDuplex(33, 1567, flavor, "\xa8\x3b\xb0\xd1\x1c\x54\x9a\xf1"); /* Keccak[r=33, c=1567] */
    selfTestDuplex(34, 1566, flavor, "\xe9\x51\xad\x33\x01\x6d\x2a\x4b"); /* Keccak[r=34, c=1566] */
    selfTestDuplex(35, 1565, flavor, "\xb7\x5d\x8f\xce\x90\x32\x9d\x03"); /* Keccak[r=35, c=1565] */
    selfTestDuplex(36, 1564, flavor, "\xe3\xc4\x71\xf6\xfd\x42\x6f\xe4"); /* Keccak[r=36, c=1564] */
    selfTestDuplex(37, 1563, flavor, "\x0b\x26\xa7\x84\x34\xfb\x8c\x19"); /* Keccak[r=37, c=1563] */
    selfTestDuplex(38, 1562, flavor, "\x17\xf4\x24\x5b\x84\x3d\x38\xe8"); /* Keccak[r=38, c=1562] */
    selfTestDuplex(39, 1561, flavor, "\xbc\xe1\x63\x27\x55\x9a\x7c\x37"); /* Keccak[r=39, c=1561] */
    selfTestDuplex(40, 1560, flavor, "\xf9\x68\x98\x88\x51\xb6\xbe\x49"); /* Keccak[r=40, c=1560] */
    selfTestDuplex(41, 1559, flavor, "\x66\x66\x0e\x24\x13\x96\x4e\x06"); /* Keccak[r=41, c=1559] */
    selfTestDuplex(42, 1558, flavor, "\x33\x20\x6d\x45\x57\xb5\x7f\xe4"); /* Keccak[r=42, c=1558] */
    selfTestDuplex(43, 1557, flavor, "\xa4\x75\xbe\x73\x37\x36\xbc\xe4"); /* Keccak[r=43, c=1557] */
    selfTestDuplex(44, 1556, flavor, "\x62\x21\x17\x44\x35\x06\x8a\x66"); /* Keccak[r=44, c=1556] */
    selfTestDuplex(45, 1555, flavor, "\x8c\x6a\xc1\x33\x71\x8c\xd5\xf9"); /* Keccak[r=45, c=1555] */
    selfTestDuplex(46, 1554, flavor, "\x9d\xfc\xb7\xaf\x8d\xe7\x0e\xd5"); /* Keccak[r=46, c=1554] */
    selfTestDuplex(47, 1553, flavor, "\xf8\x95\x0e\xae\x24\x24\xcd\x31"); /* Keccak[r=47, c=1553] */
    selfTestDuplex(48, 1552, flavor, "\xe3\xfc\x9e\x05\xd3\x65\x34\xa9"); /* Keccak[r=48, c=1552] */
    selfTestDuplex(49, 1551, flavor, "\x51\xa9\x45\x96\x1a\x32\x34\x1d"); /* Keccak[r=49, c=1551] */
    selfTestDuplex(50, 1550, flavor, "\x5a\x7a\x87\xbc\xa5\x66\x8c\x95"); /* Keccak[r=50, c=1550] */
    selfTestDuplex(51, 1549, flavor, "\x75\xbd\xd2\xa1\xde\xc9\xac\x22"); /* Keccak[r=51, c=1549] */
    selfTestDuplex(52, 1548, flavor, "\x72\x49\x0a\xb8\xd7\xad\xaa\x12"); /* Keccak[r=52, c=1548] */
    selfTestDuplex(53, 1547, flavor, "\x1b\xf5\x6f\x1f\xef\xa8\x8a\xe8"); /* Keccak[r=53, c=1547] */
    selfTestDuplex(54, 1546, flavor, "\xb2\x39\xb9\xf4\x2d\x37\xe6\x69"); /* Keccak[r=54, c=1546] */
    selfTestDuplex(55, 1545, flavor, "\x1c\xe1\x02\x2c\xb0\x6b\x8a\x6e"); /* Keccak[r=55, c=1545] */
    selfTestDuplex(56, 1544, flavor, "\xe5\x60\x71\x95\xa6\xf3\xd4\xf4"); /* Keccak[r=56, c=1544] */
    selfTestDuplex(57, 1543, flavor, "\xba\x8b\xfb\x20\x6f\xf9\x1e\x4d"); /* Keccak[r=57, c=1543] */
    selfTestDuplex(58, 1542, flavor, "\x06\xb2\x2e\x8c\x45\x36\x77\x4a"); /* Keccak[r=58, c=1542] */
    selfTestDuplex(59, 1541, flavor, "\x20\x52\x9d\x35\x0f\x73\x1b\x11"); /* Keccak[r=59, c=1541] */
    selfTestDuplex(60, 1540, flavor, "\x01\x13\xc8\x48\x3f\x43\x55\xac"); /* Keccak[r=60, c=1540] */
    selfTestDuplex(61, 1539, flavor, "\x6b\xf5\x20\x74\xf5\x74\x66\x20"); /* Keccak[r=61, c=1539] */
    selfTestDuplex(62, 1538, flavor, "\xc0\x16\x35\xe3\x62\xdf\xf3\x32"); /* Keccak[r=62, c=1538] */
    selfTestDuplex(63, 1537, flavor, "\x85\xea\xb4\x37\x79\x28\x78\xfd"); /* Keccak[r=63, c=1537] */
    selfTestDuplex(64, 1536, flavor, "\x42\x7f\x91\xfb\x62\xdb\xb1\xed"); /* Keccak[r=64, c=1536] */
    selfTestDuplex(65, 1535, flavor, "\xa3\x59\x56\xb2\x1b\x23\xd2\xd7"); /* Keccak[r=65, c=1535] */
    selfTestDuplex(66, 1534, flavor, "\x42\xc7\x13\x2c\x5b\x3c\x86\x2a"); /* Keccak[r=66, c=1534] */
    selfTestDuplex(67, 1533, flavor, "\x7e\xcf\x89\xcc\xbd\x2c\x75\x6e"); /* Keccak[r=67, c=1533] */
    selfTestDuplex(68, 1532, flavor, "\xd1\xe0\xb3\xad\xe0\xf3\x14\x94"); /* Keccak[r=68, c=1532] */
    selfTestDuplex(73, 1527, flavor, "\xbc\x4d\xed\x17\xdb\x81\x1b\x9e"); /* Keccak[r=73, c=1527] */
    selfTestDuplex(78, 1522, flavor, "\xd5\xad\xa2\x59\xf0\xb3\xec\x16"); /* Keccak[r=78, c=1522] */
    selfTestDuplex(83, 1517, flavor, "\x2f\x94\xdf\x15\xb1\x96\x0b\x8e"); /* Keccak[r=83, c=1517] */
    selfTestDuplex(88, 1512, flavor, "\x43\x89\x89\x58\xf1\x86\x70\xd3"); /* Keccak[r=88, c=1512] */
    selfTestDuplex(93, 1507, flavor, "\xc2\xf9\xf5\xa0\xbc\x38\x28\x5c"); /* Keccak[r=93, c=1507] */
    selfTestDuplex(98, 1502, flavor, "\x66\x51\x77\x34\x3b\x59\x1a\xb8"); /* Keccak[r=98, c=1502] */
    selfTestDuplex(103, 1497, flavor, "\x1c\x76\xe2\xc9\x1a\x3b\x00\xc9"); /* Keccak[r=103, c=1497] */
    selfTestDuplex(108, 1492, flavor, "\x29\x9a\x1f\x24\x97\xeb\x83\xc5"); /* Keccak[r=108, c=1492] */
    selfTestDuplex(113, 1487, flavor, "\x5a\x8c\xcf\xd6\xda\x9e\x11\xec"); /* Keccak[r=113, c=1487] */
    selfTestDuplex(118, 1482, flavor, "\x4f\x60\x19\xf3\x2a\xf6\x5a\xba"); /* Keccak[r=118, c=1482] */
    selfTestDuplex(123, 1477, flavor, "\xcc\x87\x0c\x18\x87\x00\x35\x98"); /* Keccak[r=123, c=1477] */
    selfTestDuplex(128, 1472, flavor, "\xe4\xa5\x8c\xaf\x8a\xea\x3e\xdb"); /* Keccak[r=128, c=1472] */
    selfTestDuplex(133, 1467, flavor, "\xee\xb6\x68\xc1\x3e\x56\xe1\xab"); /* Keccak[r=133, c=1467] */
    selfTestDuplex(138, 1462, flavor, "\x3c\xa5\xdc\x53\x6d\x0d\xb8\x83"); /* Keccak[r=138, c=1462] */
    selfTestDuplex(143, 1457, flavor, "\xc6\x31\xb2\xdc\xa0\x35\x4b\x6b"); /* Keccak[r=143, c=1457] */
    selfTestDuplex(148, 1452, flavor, "\x73\xaa\x16\xa7\x3b\xcc\x4d\xf1"); /* Keccak[r=148, c=1452] */
    selfTestDuplex(153, 1447, flavor, "\xad\x80\xf5\xe9\xbe\xde\xd2\x31"); /* Keccak[r=153, c=1447] */
    selfTestDuplex(158, 1442, flavor, "\xae\x13\x52\x3e\xa5\x86\x89\x1f"); /* Keccak[r=158, c=1442] */
    selfTestDuplex(163, 1437, flavor, "\x25\xee\x23\x8d\x4c\x6e\x04\xfb"); /* Keccak[r=163, c=1437] */
    selfTestDuplex(168, 1432, flavor, "\x3b\x51\x9d\x74\x04\x4c\x1b\xa3"); /* Keccak[r=168, c=1432] */
    selfTestDuplex(173, 1427, flavor, "\xb7\xe7\x81\xac\xf1\x0f\xb3\xd7"); /* Keccak[r=173, c=1427] */
    selfTestDuplex(178, 1422, flavor, "\x07\x01\xb1\xbc\xb2\xe3\x24\xec"); /* Keccak[r=178, c=1422] */
    selfTestDuplex(183, 1417, flavor, "\x0d\x0f\xf1\x56\x08\x5c\x41\x9c"); /* Keccak[r=183, c=1417] */
    selfTestDuplex(188, 1412, flavor, "\xe7\x39\x02\x70\xe8\x9a\xfa\x96"); /* Keccak[r=188, c=1412] */
    selfTestDuplex(193, 1407, flavor, "\x06\x26\x83\x6d\x72\x84\xa6\xda"); /* Keccak[r=193, c=1407] */
    selfTestDuplex(198, 1402, flavor, "\xd7\x4f\x56\x6d\x21\x4a\xf4\x64"); /* Keccak[r=198, c=1402] */
    selfTestDuplex(203, 1397, flavor, "\x6c\x06\x8f\xc3\x94\x73\xd4\x7e"); /* Keccak[r=203, c=1397] */
    selfTestDuplex(208, 1392, flavor, "\xbb\x96\x81\x5d\x42\xc8\xb4\x76"); /* Keccak[r=208, c=1392] */
    selfTestDuplex(213, 1387, flavor, "\x6c\x4c\x44\x9e\x13\x7c\x60\x45"); /* Keccak[r=213, c=1387] */
    selfTestDuplex(218, 1382, flavor, "\xea\x89\xc6\xfb\xa5\xef\x88\x22"); /* Keccak[r=218, c=1382] */
    selfTestDuplex(223, 1377, flavor, "\xb9\xd5\xe3\x0f\x60\x33\x75\x35"); /* Keccak[r=223, c=1377] */
    selfTestDuplex(248, 1352, flavor, "\x0d\xb6\x1f\xaa\xbb\x4f\x79\x01"); /* Keccak[r=248, c=1352] */
    selfTestDuplex(273, 1327, flavor, "\xe7\x38\x00\x8f\x51\xa5\xd4\xd9"); /* Keccak[r=273, c=1327] */
    selfTestDuplex(298, 1302, flavor, "\xc5\x15\x0d\x8a\xb7\x6a\xf2\x0c"); /* Keccak[r=298, c=1302] */
    selfTestDuplex(323, 1277, flavor, "\x1f\x2d\xa8\xaf\x3c\xa7\x48\x25"); /* Keccak[r=323, c=1277] */
    selfTestDuplex(348, 1252, flavor, "\xff\xea\x64\xf9\xfa\x89\xa8\x82"); /* Keccak[r=348, c=1252] */
    selfTestDuplex(373, 1227, flavor, "\x55\x15\xc0\x8f\xa6\x20\x38\x7f"); /* Keccak[r=373, c=1227] */
    selfTestDuplex(398, 1202, flavor, "\x22\xce\x93\x1f\xcf\x5f\x4d\x0c"); /* Keccak[r=398, c=1202] */
    selfTestDuplex(423, 1177, flavor, "\xba\xeb\x41\x8c\x8b\x00\x6c\x5e"); /* Keccak[r=423, c=1177] */
    selfTestDuplex(448, 1152, flavor, "\x9c\x04\xa0\x6a\xd4\x67\xcc\xdc"); /* Keccak[r=448, c=1152] */
    selfTestDuplex(473, 1127, flavor, "\xe5\x48\xd6\x6c\x00\xf1\xc3\x86"); /* Keccak[r=473, c=1127] */
    selfTestDuplex(498, 1102, flavor, "\xf7\xca\x7f\xb5\xae\xa3\x2d\xa7"); /* Keccak[r=498, c=1102] */
    selfTestDuplex(523, 1077, flavor, "\x79\x91\x6e\x50\x13\xf6\x40\x66"); /* Keccak[r=523, c=1077] */
    selfTestDuplex(548, 1052, flavor, "\xf0\xb3\x13\xe5\x59\xff\xe2\xfc"); /* Keccak[r=548, c=1052] */
    selfTestDuplex(573, 1027, flavor, "\xa7\x79\x4f\x99\x03\xad\x0b\xe7"); /* Keccak[r=573, c=1027] */
    selfTestDuplex(598, 1002, flavor, "\xb5\x13\x0d\x4d\x46\xfa\x51\xd1"); /* Keccak[r=598, c=1002] */
    selfTestDuplex(623, 977, flavor, "\x26\x8c\xcf\x07\xa0\xb0\xbe\x5b"); /* Keccak[r=623, c=977] */
    selfTestDuplex(648, 952, flavor, "\x33\x58\xde\xc1\x9f\x72\xd9\x7f"); /* Keccak[r=648, c=952] */
    selfTestDuplex(673, 927, flavor, "\xa3\xc0\x45\xf5\xb1\x2e\xe9\x08"); /* Keccak[r=673, c=927] */
    selfTestDuplex(698, 902, flavor, "\x58\xd7\xf3\x0e\xdb\xf5\x67\xf1"); /* Keccak[r=698, c=902] */
    selfTestDuplex(723, 877, flavor, "\x72\x1a\xbe\xba\xd6\xe7\x68\x3f"); /* Keccak[r=723, c=877] */
    selfTestDuplex(748, 852, flavor, "\xb9\xb8\x46\xf1\xb9\x9c\x22\x68"); /* Keccak[r=748, c=852] */
    selfTestDuplex(773, 827, flavor, "\xec\x0e\xc7\xe7\x10\x54\xff\x72"); /* Keccak[r=773, c=827] */
    selfTestDuplex(798, 802, flavor, "\x4e\xe2\xfc\xc3\x49\x9c\xa4\x08"); /* Keccak[r=798, c=802] */
    selfTestDuplex(823, 777, flavor, "\xd8\x33\xe2\x95\xd8\x99\xdd\x91"); /* Keccak[r=823, c=777] */
    selfTestDuplex(848, 752, flavor, "\x37\xb3\x1e\xe4\x55\x21\x14\xc4"); /* Keccak[r=848, c=752] */
    selfTestDuplex(873, 727, flavor, "\x99\xca\x45\xe1\xc9\x23\x63\xdd"); /* Keccak[r=873, c=727] */
    selfTestDuplex(898, 702, flavor, "\x4f\x2d\x55\x38\x68\x96\xa9\x08"); /* Keccak[r=898, c=702] */
    selfTestDuplex(923, 677, flavor, "\x5c\x6b\xd7\xf0\x95\x16\x6c\x9d"); /* Keccak[r=923, c=677] */
    selfTestDuplex(948, 652, flavor, "\xb7\x56\x18\x4d\x61\x77\x83\x4b"); /* Keccak[r=948, c=652] */
    selfTestDuplex(973, 627, flavor, "\x65\x28\x9a\x19\x1c\xed\x06\x96"); /* Keccak[r=973, c=627] */
    selfTestDuplex(998, 602, flavor, "\x68\xeb\xa2\x02\x77\x9a\x96\xe3"); /* Keccak[r=998, c=602] */
    selfTestDuplex(1023, 577, flavor, "\x1e\x4b\xb4\x7f\x29\x64\x0d\x7e"); /* Keccak[r=1023, c=577] */
    selfTestDuplex(1048, 552, flavor, "\xca\x8c\xdc\x76\xe6\x19\x13\x4e"); /* Keccak[r=1048, c=552] */
    selfTestDuplex(1073, 527, flavor, "\x70\x8a\xbc\xfc\x89\x03\xc6\x67"); /* Keccak[r=1073, c=527] */
    selfTestDuplex(1098, 502, flavor, "\xf3\x6b\x2e\x1a\xf5\x71\xf4\xcb"); /* Keccak[r=1098, c=502] */
    selfTestDuplex(1123, 477, flavor, "\xd0\x60\x08\x29\xd3\xd0\xf4\x22"); /* Keccak[r=1123, c=477] */
    selfTestDuplex(1148, 452, flavor, "\x08\x5b\x51\xc5\x61\x52\x1b\x43"); /* Keccak[r=1148, c=452] */
    selfTestDuplex(1173, 427, flavor, "\x7b\x71\xe2\x1d\x63\xeb\x3f\x80"); /* Keccak[r=1173, c=427] */
    selfTestDuplex(1198, 402, flavor, "\x8c\x81\xec\x7d\xc0\x36\xe8\xde"); /* Keccak[r=1198, c=402] */
    selfTestDuplex(1223, 377, flavor, "\xa9\x78\xbb\xdf\xcb\x2b\xda\x55"); /* Keccak[r=1223, c=377] */
    selfTestDuplex(1248, 352, flavor, "\x86\x8d\x6b\xc7\x4d\xef\x18\xd2"); /* Keccak[r=1248, c=352] */
    selfTestDuplex(1273, 327, flavor, "\x40\x6c\xdf\xa0\x7a\x43\x5b\xb3"); /* Keccak[r=1273, c=327] */
    selfTestDuplex(1298, 302, flavor, "\xad\x92\x7d\x83\xf8\x22\xdd\xa0"); /* Keccak[r=1298, c=302] */
    selfTestDuplex(1323, 277, flavor, "\x37\x22\x0c\x42\x82\x79\x86\x07"); /* Keccak[r=1323, c=277] */
    selfTestDuplex(1348, 252, flavor, "\x7c\x61\x90\x43\x8c\x9b\xef\x74"); /* Keccak[r=1348, c=252] */
    selfTestDuplex(1373, 227, flavor, "\xda\xc2\x06\x1a\xee\x8c\x6f\xd7"); /* Keccak[r=1373, c=227] */
    selfTestDuplex(1398, 202, flavor, "\xf0\x49\x79\xa9\x5b\x15\xde\xb1"); /* Keccak[r=1398, c=202] */
    selfTestDuplex(1423, 177, flavor, "\x05\x54\xaf\xee\xaa\xdd\x6b\x0a"); /* Keccak[r=1423, c=177] */
    selfTestDuplex(1448, 152, flavor, "\xb7\x42\x50\x42\xcd\x6a\x20\xb6"); /* Keccak[r=1448, c=152] */
    selfTestDuplex(1473, 127, flavor, "\x56\xcf\x98\x1b\xad\x83\x96\xb2"); /* Keccak[r=1473, c=127] */
    selfTestDuplex(1498, 102, flavor, "\xeb\x6e\xb2\x8f\x51\xf7\xf7\xe6"); /* Keccak[r=1498, c=102] */
    selfTestDuplex(1523, 77, flavor, "\x85\x8b\xe0\x4e\x47\x5b\xc6\x74"); /* Keccak[r=1523, c=77] */
    selfTestDuplex(1548, 52, flavor, "\x0b\x24\x99\x0d\xd2\x05\x2c\x2d"); /* Keccak[r=1548, c=52] */
    selfTestDuplex(1573, 27, flavor, "\x7f\x3f\x68\xeb\x74\x29\x23\x4c"); /* Keccak[r=1573, c=27] */
    selfTestDuplex(1598, 2, flavor, "\xe0\x17\xcc\x63\x9f\xc5\x57\x5f"); /* Keccak[r=1598, c=2] */
#endif
#if !defined(EMBEDDED)
    printf("\n");
#endif
    }
}
