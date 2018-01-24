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

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakP1600
    #define SnP KeccakP1600
    #define SnP_width 1600
    #define SnP_Permute KeccakP1600_Permute_24rounds
    #define SnP_Permute_12rounds KeccakP1600_Permute_12rounds
    #define SnP_Permute_Nrounds KeccakP1600_Permute_Nrounds
    #define SnP_Permute_maxRounds 24
    #if defined(KeccakF1600_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF1600_FastLoop_Absorb
    #endif
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix KeccakP800
    #define SnP KeccakP800
    #define SnP_width 800
    #define SnP_Permute KeccakP800_Permute_22rounds
    #define SnP_Permute_12rounds KeccakP800_Permute_12rounds
    #define SnP_Permute_Nrounds KeccakP800_Permute_Nrounds
    #define SnP_Permute_maxRounds 22
    #if defined(KeccakF800_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF800_FastLoop_Absorb
    #endif
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"

    #define prefix KeccakP400
    #define SnP KeccakP400
    #define SnP_width 400
    #define SnP_Permute KeccakP400_Permute_20rounds
    #define SnP_Permute_Nrounds KeccakP400_Permute_Nrounds
    #define SnP_Permute_maxRounds 20
    #if defined(KeccakF400_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF400_FastLoop_Absorb
    #endif
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifndef KeccakP200_excluded
    #include "KeccakP-200-SnP.h"

    #define prefix KeccakP200
    #define SnP KeccakP200
    #define SnP_width 200
    #define SnP_Permute KeccakP200_Permute_18rounds
    #define SnP_Permute_Nrounds KeccakP200_Permute_Nrounds
    #define SnP_Permute_maxRounds 18
    #if defined(KeccakF200_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF200_FastLoop_Absorb
    #endif
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

void testSnP()
{
#ifndef KeccakP200_excluded
    KeccakP200_testSnP("KeccakP-200.txt", "Keccak-p[200]",
        "\x73\x1b\x9a\xc1\x78\x5f\xaf\x1a\x48\xf7\x70\x53\x59\x98\x63\x19\x95\x74\xcd\xe8\x14\xbf\x13\xfe\x9f"
    );
#endif

#ifndef KeccakP400_excluded
    KeccakP400_testSnP("KeccakP-400.txt", "Keccak-p[400]",
        "\xa9\xd6\xb2\x51\x10\xa8\xf2\x75\x77\x4a\xbf\x8c\x7d\xc9\x69\x5a\x87\x49\x2d\x82\x75\x8b\x9a\x00\x84\x25\xb3\x72\x2b\x9b\x48\xd7\xdc\xd9\xc8\xcd\x26\x69\x69\x19\xe5\xf7\x7f\x10\x48\xcc\xdc\xe9\x4b\xe4"
    );
#endif

#ifndef KeccakP800_excluded
    KeccakP800_testSnP("KeccakP-800.txt", "Keccak-p[800]",
        "\x88\x55\x7e\x60\x84\xe4\x32\x1b\xcc\x4b\x57\x0a\x8e\xb4\x33\xbd\xbb\xc2\x05\x0a\x15\xed\x61\xcc\xea\x85\x00\x77\xbd\x3e\xfa\x18\x4b\x3b\x8f\x47\x89\x16\x31\x05\xb9\x27\x24\x09\xc7\x5e\x8d\xa3\x84\x3f\xfb\x35\x61\x5c\xc7\xe9\x3c\xc4\xd2\x2a\xb2\x4d\xd3\x1e\x67\x29\x61\xc7\x38\x6d\x86\xc2\x98\xc6\x43\xf4\x32\x69\xc9\x05\x0a\xef\x30\x93\xd1\xf1\xb1\xd3\x3a\xf6\x53\x58\x02\xa6\x46\x39\xab\x30\x6c\xe8"
    );
#endif

#ifndef KeccakP1600_excluded
    KeccakP1600_testSnP("KeccakP-1600.txt", "Keccak-p[1600]",
        "\x96\xb5\xfa\x45\x4c\xb4\xcf\x9f\x3d\xbf\x60\xa7\x17\x82\x22\xe0\x07\xe0\xf7\x17\x8f\x24\xac\x62\x7b\x11\xbb\x11\xd3\x97\xbb\x3c\xfd\x0d\xf2\xd7\x38\x40\xd2\x48\x6e\x94\xcf\xb2\xf0\x77\x9e\x8f\x1e\x42\xac\x18\xb1\xb6\x11\x58\xac\x5a\xe6\xeb\x85\x83\x18\x78\x6e\xc2\x85\x70\x07\x0e\xbb\x7a\x76\x11\x71\x83\xaf\x9e\x22\x46\x0b\x61\x22\x7a\xbc\xf5\x2d\x56\xd2\x07\x45\xd1\x41\x00\xcf\x86\x85\xc3\xf0\x33\x16\xd4\x8a\xdb\x81\x68\x7e\x74\x61\x8e\x10\x1b\x4e\x9a\x0d\x37\xc6\x1e\x80\xe9\xa0\xb0\x18\x2e\xc3\x50\x19\xdf\x3b\x0b\x76\x7f\x63\xae\x07\x00\x20\x4a\x3d\x79\x58\x3e\xa6\x2d\xd9\xcb\xf9\x40\x5b\x1c\xbb\x8d\x24\xf3\x13\x03\xfe\xff\x57\x3b\x54\x0c\xc1\x9d\x57\x9e\x59\x26\x61\x1d\x00\x1c\x38\x02\xaa\x03\x59\x76\x5a\x28\x55\xd5\x9d\x2b\x1e\x52\xed\xf7\x04\xe6\x1d\x19\x75\x33\xc3\xfc\x21\x8c\xe6\x80"
    );
#endif
}
