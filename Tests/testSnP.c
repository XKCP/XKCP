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

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakP1600
    #define SnP KeccakP1600
    #define SnP_width 1600
    #define SnP_Permute KeccakP1600_Permute_24rounds
    #define SnP_Permute_12rounds KeccakP1600_Permute_12rounds
    #if defined(KeccakF1600_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF1600_FastLoop_Absorb
    #endif
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_FastLoop_Absorb
#endif

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix KeccakP800
    #define SnP KeccakP800
    #define SnP_width 800
    #define SnP_Permute KeccakP800_Permute_22rounds
    #define SnP_Permute_12rounds KeccakP800_Permute_12rounds
    #if defined(KeccakF800_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF800_FastLoop_Absorb
    #endif
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
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
        "\x7f\xe7\xfc\xe3\x92\x8e\xc3\x55\x03\x39\x3c\x76\xba\x4b\x61\xb9\xc0\xe4\xf8\x3f\xb8\xfb\x3b\xcc\xee\x45\x28\x30\xc6\x2e\xe0\x0c\x7e\xb4\x2d\x9c\x6e\x12\xc8\x53\xdf\x1a\xf8\xa3\x2f\xcf\xc3\x3b\xc4\x18\xcc\x28\xa2\x71\xc4\x34\xc9\xa6\x65\xf1\xa2\x77\xac\xfd\x77\x9b\xa6\xbb\xe9\x5f\x74\xfb\x39\x20\x93\xfe\xda\x99\xce\xfc\x11\x21\xdd\x83\x3b\x5b\x3b\x07\xf6\x19\x28\x2d\x72\x6e\x6e\xac\xb1\xaf\x4e\xf2"
    );
#endif

#ifndef KeccakP1600_excluded
    KeccakP1600_testSnP("KeccakP-1600.txt", "Keccak-p[1600]",
        "\xa6\xe0\x2c\x29\xfb\x8f\x85\x21\xba\x3e\x83\x1f\x31\x9e\x39\x60\x84\xe2\x9c\x71\x16\x66\x98\x50\x74\x50\xf5\xe6\x65\x58\x0b\x05\xc9\x3f\xe9\xdc\x63\xe1\xc9\x61\x32\x86\x94\x88\x35\x51\xe8\x58\xc2\x99\xde\x51\x36\xe2\x0b\xdb\x81\x1e\x5e\xbf\xd5\xca\x17\x15\x52\x86\x1f\xe3\x58\x3b\x18\xe4\xa0\x14\xb7\x99\x88\x07\x73\x51\xea\x02\x16\x4d\x1f\xa5\x8d\xd5\x25\x87\xc6\x8d\x3b\x9e\x43\xbd\x22\x6b\x8a\x68\xa4\x12\xff\x18\x63\xfc\x40\xb1\xad\xd5\x92\xea\xd7\x56\x56\xbc\x17\xfb\xd8\x11\xf9\x2f\xfc\x77\x7c\x30\x46\xfe\x3a\xaa\xe6\x0a\x30\xba\x06\x94\x30\xe4\x21\x21\x0e\x74\x12\xa3\xce\xf6\xee\x2c\x4b\xff\x73\x51\x52\x15\xaa\xf5\xc6\x56\x88\x3f\x13\x40\x62\x9c\xa6\x0c\x81\xc2\x67\xec\x64\xdd\x0e\x1c\xd9\x6d\xcb\xee\x07\x20\x60\xc9\x29\xc6\x90\x11\xac\x47\x08\x9b\x60\x77\x4a\x00\x0b\x93\x99\xc0\x19\x0b"
    );
#endif
}
