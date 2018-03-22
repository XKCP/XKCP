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
#include <string.h>

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeccakP1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define PlSnP_PermuteAll            KeccakP1600times2_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times2_PermuteAll_12rounds
    #define PlSnP_PermuteAll_6rounds    KeccakP1600times2_PermuteAll_6rounds
    #define PlSnP_PermuteAll_4rounds    KeccakP1600times2_PermuteAll_4rounds
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
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_4rounds
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
    #define PlSnP_PermuteAll_6rounds    KeccakP1600times4_PermuteAll_6rounds
    #define PlSnP_PermuteAll_4rounds    KeccakP1600times4_PermuteAll_4rounds
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
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_4rounds
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
    #define PlSnP_PermuteAll_6rounds    KeccakP1600times8_PermuteAll_6rounds
    #define PlSnP_PermuteAll_4rounds    KeccakP1600times8_PermuteAll_4rounds
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
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_4rounds
    #undef SnP_width
    #undef PlSnP_FastLoop_Absorb
#endif

void testPlSnP()
{
#ifndef KeccakP1600timesN_excluded
    KeccakP1600times2_testPlSnP("KeccakP-1600-times2.txt", "Keccak-p[1600]\303\2272",
        "\xf8\x84\x8f\x67\x6a\x80\x87\x4e\xc1\x65\x2e\xea\xb1\x59\xd5\x48\x35\x7b\x12\xa4\xb4\x16\x98\x8d\xdd\xeb\xf8\xa8\x7d\xa8\xf3\xec\xfc\x4e\x86\x22\x60\x96\x43\x48\x39\xc6\xd2\x9f\xca\xa6\xaf\xab\xb2\x20\x63\xdb\x5f\xf9\xc2\xbf\x28\xb3\xff\x9f\x9e\xa8\xca\xb2\xe3\xee\xbf\x73\x3a\x8e\xe6\x68\x6a\x21\x2a\x5a\x55\x34\x64\x3e\xaf\x90\xed\xc4\x7e\x94\x69\x93\x2c\x6c\xbf\x90\x17\x49\xcd\x6e\x65\xab\x26\x7f\xd7\x63\xee\xc0\x1b\x09\x8c\x00\xc2\xf3\x69\x38\xd9\x29\x12\x66\x1a\x95\x5e\x0c\xe2\x4b\x28\x0c\x7b\x90\x90\x1b\x84\xaf\x9f\x43\x34\xad\x9e\x4f\xaa\xdd\xfb\x89\xae\xe6\x00\x05\x6e\xc0\xe0\xe2\x8c\x11\xc6\x46\x5b\xc5\x06\x8c\x23\xa6\xdc\xeb\xb0\x4d\xc0\xee\xc7\x63\xe5\xd2\x85\xd8\x07\xf0\x8f\xf0\x68\x95\xab\xac\xc2\xdf\x9d\xe8\x3c\xb6\x1d\x00\xfc\x87\x38\x6e\x66\x01\x81\x1d\x4d\x83\xe0\xc4\x8c\x05"
    );
    KeccakP1600times4_testPlSnP("KeccakP-1600-times4.txt", "Keccak-p[1600]\303\2274",
        "\x68\x78\x6d\x20\x2d\x2e\x1f\x54\x2a\x1f\x0d\x20\xcf\x66\x2e\x16\x96\xda\x51\x39\x5e\xbe\xbc\x61\xc0\x67\x14\x97\x27\x59\x5e\xef\x4a\x55\x75\x59\x47\xe9\x17\x48\x96\x84\x44\x65\x81\xb7\x4a\x05\xe1\x8c\xb4\x98\xb7\x3f\x53\x83\x6a\xaa\xd1\x85\x9f\xf5\xf0\x77\x4c\x09\x65\x07\x25\x5c\xef\x5d\x04\x3b\xb2\x03\xe1\xe5\xd9\x2e\xb5\x86\x4d\x8c\x3e\x78\x97\x23\x1e\x7b\x1a\xd4\x88\xf0\x75\xe8\x34\x4d\x1b\x49\xba\x65\x8e\xc8\xd0\x3a\x0f\x39\x65\x83\xac\xba\xdd\xb9\x85\xcd\x86\x32\x55\x32\x63\x80\xe6\x30\x1f\x7b\xb1\x66\xa3\x09\x55\x13\x9f\xc4\x3b\x04\xec\x35\x0f\x0e\x25\xfb\x83\x6a\xbd\xb8\x99\xac\xa7\x3b\xe4\x9d\x9d\x42\xbc\x5d\x0b\xad\x05\x5a\x5d\x48\x1b\x3c\xc6\x94\xea\x1e\x94\x89\x50\xf7\x67\x63\x1f\xe6\xc3\xe1\x5e\xcd\xb2\xb9\x45\x5a\x3b\x5a\x73\x1a\xdf\x3d\xf0\x41\x06\x47\x20\x92\x4b\xcc\xf0\x59"
    );
    KeccakP1600times8_testPlSnP("KeccakP-1600-times8.txt", "Keccak-p[1600]\303\2278",
        "\xc5\x13\xbc\xba\xfd\x29\xd2\xdf\xef\xfe\x45\x6c\x58\x11\xab\xf6\xd1\xb9\xfa\xbf\x9b\x0d\x34\x54\x60\xeb\x0b\x75\x32\x89\x27\x8e\x15\x50\xec\xcf\x9f\x88\x48\xd2\x54\x0d\x1f\xe6\x64\xba\xbc\xc5\x94\x42\xb5\xa4\x32\x2c\xe2\xae\xad\xcc\x24\x12\x8e\x1c\x8d\x46\xa4\xda\x0f\xa7\xa0\xd1\xd0\xb6\xc9\xad\xbe\x6c\xb4\x90\xd9\x95\x21\xe6\x43\xfd\x6f\x1f\x05\x47\x2a\xad\x6d\x13\xe6\xd9\xfc\x27\xfa\x63\x71\xab\x01\xe2\x19\x19\x24\x4b\x20\xe7\xf6\x54\x43\xcb\xf8\x0b\x30\xd7\xb5\x48\x61\x71\x72\x94\x1f\x6c\xb7\xb6\x6b\xfe\xff\x10\xec\x7c\x53\x28\x24\x53\xea\x57\xc4\x3b\xfb\xa6\x16\xfd\x3a\xe1\x23\x04\x74\xbb\x62\x19\x2c\x79\x78\x6e\x9b\xb6\xa4\x59\xe0\xdb\xd1\xe0\x7e\x90\x27\x33\xb2\x8f\x88\xe0\xd8\x12\x4a\xd2\x52\x6a\x8e\x30\x47\x6e\xb9\xea\x28\x2e\xbb\x45\x73\xca\xc8\x2f\xbf\x09\xa2\x06\x39\x07\xde\xd5"
    );
#endif
}
