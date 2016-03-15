/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#if !defined(EMBEDDED)
//#define OUTPUT
//#define VERBOSE
#endif

#include <assert.h>
#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <string.h>

#ifndef KeccakP800timesN_excluded
    #include "KeccakP-800-times2-SnP.h"

    #define prefix                      KeccakP800times2
    #define PlSnP                       KeccakP800times2
    #define PlSnP_parallelism           2
    #define PlSnP_PermuteAll            KeccakP800times2_PermuteAll_22rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP800times2_PermuteAll_12rounds
    #define SnP_width                   800
    #if defined(KeccakF800times2_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF800times2_FastLoop_Absorb
    #endif
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

#ifndef KeccakP800timesN_excluded
    #include "KeccakP-800-times4-SnP.h"

    #define prefix                      KeccakP800times4
    #define PlSnP                       KeccakP800times4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll            KeccakP800times4_PermuteAll_22rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP800times4_PermuteAll_12rounds
    #define SnP_width                   800
    #if defined(KeccakF800times4_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF800times4_FastLoop_Absorb
    #endif
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

#ifndef KeccakP800timesN_excluded
    #include "KeccakP-800-times8-SnP.h"

    #define prefix                      KeccakP800times8
    #define PlSnP                       KeccakP800times8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll            KeccakP800times8_PermuteAll_22rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP800times8_PermuteAll_12rounds
    #define SnP_width                   800
    #if defined(KeccakF800times8_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF800times8_FastLoop_Absorb
    #endif
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeccakP1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define PlSnP_PermuteAll            KeccakP1600times2_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times2_PermuteAll_12rounds
    #define SnP_width                   1600
    #if defined(KeccakF1600times2_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF1600times2_FastLoop_Absorb
    #endif
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      KeccakP1600times4
    #define PlSnP                       KeccakP1600times4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll            KeccakP1600times4_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times4_PermuteAll_12rounds
    #define SnP_width                   1600
    #if defined(KeccakF1600times4_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF1600times4_FastLoop_Absorb
    #endif
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      KeccakP1600times8
    #define PlSnP                       KeccakP1600times8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll            KeccakP1600times8_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times8_PermuteAll_12rounds
    #define SnP_width                   1600
    #if defined(KeccakF1600times8_FastLoop_supported)
        #define PlSnP_FastLoop_Absorb KeccakF1600times8_FastLoop_Absorb
    #endif
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_12rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

void testPlSnP()
{
#ifndef KeccakP800timesN_excluded
    KeccakP800times2_testPlSnP("KeccakP-800-times2.txt", "Keccak-p[800]\303\2272",
        "\x40\xf1\x05\x3c\xa1\xb7\x5b\xd0\x69\x68\x00\x10\xa7\xd3\xbd\x3d\x5e\x48\xba\x6d\xd8\xe5\x1c\x3b\x61\xd4\x61\x51\xde\xd3\x0b\xa2\x88\x19\x85\x24\xc7\xb5\x2e\x0b\x58\xe3\x7c\x76\xe0\xe0\x59\x07\x45\x83\x31\x3a\x85\x42\x71\x49\xea\x2e\xec\x15\xe2\xfe\xf1\x21\x4a\x82\x9d\x13\x34\xb1\xfe\x0e\x53\xe1\x7e\xcb\xe7\x12\x8b\x63\xb0\x96\xe6\x8d\xe0\x98\x76\xa1\x6c\x1f\xd2\xa1\x90\x0e\x11\x20\xdb\x58\x84\x6e"
    );
    KeccakP800times4_testPlSnP("KeccakP-800-times4.txt", "Keccak-p[800]\303\2274",
        "\x28\x58\x09\xa9\xf9\xd7\xf2\xa4\x8b\x8e\x41\x3a\xdc\x8a\x9e\xcd\x89\x59\xb7\x4d\xf1\x35\x89\x24\x6c\x88\xea\xc3\x3b\x96\x4f\x1d\xb1\x80\x61\x33\xe6\x54\x23\xfe\xb6\x83\x06\x5b\x64\x3b\xee\x9c\xbe\x28\x50\x09\xf1\x2a\x45\xd5\xcf\xc8\xa6\xdf\xdc\x1c\x25\x3f\xdc\xae\x69\x3c\x00\xde\x9a\x56\x10\xff\xb5\x3f\x75\xdf\x50\xb8\x38\x77\x50\xba\x43\xda\x09\xee\x38\xd5\xee\xb8\x75\xd2\xcc\x75\x63\xe6\xfd\xa5"
    );
    KeccakP800times8_testPlSnP("KeccakP-800-times8.txt", "Keccak-p[800]\303\2278",
        "\xbb\x9d\x80\x37\x54\xc2\x30\xcc\x54\xc6\x15\x9a\x6a\x4c\x1b\x24\xb8\x06\xd4\x5d\x80\xdf\x98\x97\xe3\x40\x55\xdc\x45\x3d\xa7\x3a\xdc\x0c\xf1\xf0\x86\x93\xaf\xbd\xf8\x91\x5a\x3b\x7e\xee\x9f\xa9\x10\x0c\x65\x74\x6f\x0c\x51\xc0\x57\xee\x68\x7d\x2f\x0a\x0c\x61\x15\xbb\x58\x2c\xb7\xaa\x34\x90\xce\x58\x7f\xb6\x96\x3e\xf9\xda\x4b\x6f\x02\xb8\x62\xb1\x8d\xec\xba\x81\x28\xe7\xb2\xd6\x2c\x0a\x5e\xb2\x90\xd6"
    );
#endif

#ifndef KeccakP1600timesN_excluded
    KeccakP1600times2_testPlSnP("KeccakP-1600-times2.txt", "Keccak-p[1600]\303\2272",
        "\x53\xfe\x5b\x56\xd3\x16\xa2\x12\x5f\x65\x8e\xec\xe4\xf0\x7d\x96\xa2\x7e\x3a\x68\x18\x80\x4f\xc2\xec\x7f\xd2\x3b\x0d\x67\x2c\x60\x4d\x44\xde\x8b\x4e\x0a\x33\xb0\xd5\xe2\x14\xb2\xff\x37\x34\x3e\xf8\xae\xa6\x00\x53\xa9\xb5\xe6\xc4\x1f\x4e\xdd\xad\x4b\x05\x2d\xa5\x90\x70\xdf\x33\x9d\x92\x8c\x0e\xd1\x50\xc1\x6a\xca\xf0\x15\xce\x14\x4b\x5f\xdf\x21\x73\x8f\x42\x89\xc4\x23\xa5\xe9\xc1\x14\x8f\xc0\x19\xa6\xac\x01\xa0\x17\xd3\x88\xda\x6e\xa2\x6e\x87\xf4\xf5\x5a\x03\xd1\xa9\x59\xf7\xf4\xea\x2a\xb1\xd0\xd4\x5b\x54\x9b\xfa\x06\x86\x05\x08\xce\x9c\xa7\x7b\xb1\xfe\x12\xdf\x64\xa1\xd9\x55\x72\xda\x80\xf2\x16\xb9\x32\xab\xb0\xa6\x75\x3a\x1d\xf8\xf1\x76\xf2\x41\xf6\x37\x95\x49\x46\xf0\x7d\x45\xf4\xf9\x6b\xaf\xb9\xbf\x22\x6d\x35\x45\xc1\xcc\x1b\x38\x75\xf0\xad\x4d\x70\x8b\x36\x7a\x5b\xee\xed\x02\x52\xc3\x8e"
    );
    KeccakP1600times4_testPlSnP("KeccakP-1600-times4.txt", "Keccak-p[1600]\303\2274",
        "\x99\xe9\x74\xee\x21\x2a\xeb\xde\x5b\xe9\xe5\x29\x59\xe4\x9a\x6a\x8c\x4e\xf1\x63\x3f\x1c\xd4\x86\x84\x69\xb7\x0b\x38\xc3\x40\x00\x3d\x7b\xbf\x36\x95\x23\xcb\x9e\x53\x3f\x68\x6d\x6d\xa3\x70\x4b\x41\x48\x61\x8b\xf6\xe6\xef\xd3\x4e\x4e\x91\x87\xd7\xea\x57\xf0\x51\x0f\xd2\x67\x3e\xd9\xe2\x57\x22\x72\xa0\xcd\x84\x9f\x0b\xb0\x70\x0d\xf0\x1a\xc2\x96\x08\x69\x06\xd9\xd5\x33\xdd\x6d\xb6\x64\x1a\x96\x89\xea\xba\x3b\xb7\x40\x99\xf6\x2a\x55\xda\x86\x2a\xba\xf0\x41\xaf\x97\xf8\x7a\xab\x26\xab\x43\x69\x24\xd1\xa2\xb6\x18\x74\xe9\x4d\x13\xd1\xc3\x6e\x64\xa3\xfd\xef\x15\xc7\xdb\x2c\x57\x70\x68\xa1\xbd\x9d\x50\xb3\xb2\xe9\x89\xc3\x3f\x4e\x91\xf9\x67\xba\xc5\x45\x09\xb3\x15\x6d\x53\x82\x7a\x9f\xb9\x77\x6a\x04\xb9\x2c\x12\x72\x45\xd2\x90\x9c\xfe\xf4\xd8\xc4\x8f\xdd\x11\x1f\x97\x1b\x1d\x20\xaf\x50\xc4\x85\xc6"
    );
    KeccakP1600times8_testPlSnP("KeccakP-1600-times8.txt", "Keccak-p[1600]\303\2278",
        "\x0d\x42\x10\xfa\x5e\x4d\x04\x5c\x03\xb7\x24\xf5\x8c\x88\x68\x4f\xa0\x84\x52\x03\x76\x8b\x0a\xd8\xa6\x1e\xd9\xc0\x0d\x98\x7b\x4e\xe8\x96\x13\x50\x47\xdc\xd8\x26\x8f\x68\x5b\xfa\x46\xa4\xf3\x9c\xc7\x76\xcb\x4d\x47\x33\xef\x26\x1e\xc8\x2c\x2a\xec\x4c\xba\x57\x28\x5f\x26\xb3\xa8\xd9\x40\x93\x1b\xd9\xc9\x88\x39\x0e\xf2\xc2\x37\x4c\x49\x2e\x30\x2e\x1b\x29\x78\x3c\x4e\x15\x05\x88\x49\xa2\x12\x1a\x6a\x70\x46\x1f\xf5\xa1\xe2\x42\x70\xe1\x78\x2d\xd3\xa2\x3f\x09\xae\xc2\x83\x05\xb2\x70\x4e\x69\x19\x9e\x3d\x90\xa6\xe8\x0e\xa6\x37\xbc\xb5\xa6\xe2\xc1\x0f\xfa\x32\x6c\x7c\x30\x61\x48\x24\x8d\xb7\xef\x0d\xd4\x80\x30\xa2\x62\xcb\xb6\x31\x3d\x34\x6c\x98\x2a\x78\xfe\x91\x9f\xef\xb3\x94\xa8\x08\xbe\x38\xf1\x1a\xab\xfb\x3d\x32\x3f\x4a\x6f\x9a\x6a\xfa\x31\x25\x70\xbb\x9c\xf3\xdf\xe1\x2a\x77\x88\xc1\xa2\x6c\x72"
    );
#endif
}
