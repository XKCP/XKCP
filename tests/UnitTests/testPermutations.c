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

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "UT.h"

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakP1600
    #define SnP KeccakP1600
    #ifdef KeccakP1600_stateAlignment
        #define SnP_stateAlignment KeccakP1600_stateAlignment
    #else
        #define SnP_stateAlignment 8
    #endif
    #define SnP_width 1600
    #define SnP_laneCount 25
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
    #undef SnP_stateAlignment
    #undef SnP_width
    #undef SnP_laneCount
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP1600times2
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeccakP1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define PlSnP_PermuteAll            KeccakP1600times2_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times2_PermuteAll_12rounds
    #define PlSnP_PermuteAll_6rounds    KeccakP1600times2_PermuteAll_6rounds
    #define PlSnP_PermuteAll_4rounds    KeccakP1600times2_PermuteAll_4rounds
    #define PlSnP_X                     4
    #define SnP_width                   1600
    #define SnP_laneCount               25
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
    #undef PlSnP_X
    #undef SnP_width
    #undef SnP_laneCount
    #undef PlSnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP1600times4
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      KeccakP1600times4
    #define PlSnP                       KeccakP1600times4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll            KeccakP1600times4_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times4_PermuteAll_12rounds
    #define PlSnP_PermuteAll_6rounds    KeccakP1600times4_PermuteAll_6rounds
    #define PlSnP_PermuteAll_4rounds    KeccakP1600times4_PermuteAll_4rounds
    #define PlSnP_X                     4
    #define SnP_width                   1600
    #define SnP_laneCount               25
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
    #undef PlSnP_X
    #undef SnP_width
    #undef SnP_laneCount
    #undef PlSnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP1600times8
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      KeccakP1600times8
    #define PlSnP                       KeccakP1600times8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll            KeccakP1600times8_PermuteAll_24rounds
    #define PlSnP_PermuteAll_12rounds   KeccakP1600times8_PermuteAll_12rounds
    #define PlSnP_PermuteAll_6rounds    KeccakP1600times8_PermuteAll_6rounds
    #define PlSnP_PermuteAll_4rounds    KeccakP1600times8_PermuteAll_4rounds
    #define PlSnP_X                     4
    #define SnP_width                   1600
    #define SnP_laneCount               25
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
    #undef PlSnP_X
    #undef SnP_width
    #undef SnP_laneCount
    #undef PlSnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"

    #define prefix KeccakP800
    #define SnP KeccakP800
    #define SnP_width 800
    #define SnP_laneCount 25
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
    #undef SnP_laneCount
    #undef SnP_Permute
    #undef SnP_Permute_12rounds
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP400
    #include "KeccakP-400-SnP.h"

    #define prefix KeccakP400
    #define SnP KeccakP400
    #define SnP_width 400
    #define SnP_laneCount 25
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
    #undef SnP_laneCount
    #undef SnP_Permute
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_KeccakP200
    #include "KeccakP-200-SnP.h"

    #define prefix KeccakP200
    #define SnP KeccakP200
    #define SnP_width 200
    #define SnP_laneCount 25
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
    #undef SnP_laneCount
    #undef SnP_Permute
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_FastLoop_Absorb
#endif

#ifdef XKCP_has_Xoodoo
    #include "Xoodoo-SnP.h"

    #define prefix Xoodoo
    #define SnP Xoodoo
    #ifdef Xoodoo_stateAlignment
        #define SnP_stateAlignment Xoodoo_stateAlignment
    #else
        #define SnP_stateAlignment 4
    #endif
    #define SnP_width (3*4*32)
    #define SnP_laneCount 12
    #define SnP_Permute          Xoodoo_Permute_6rounds
    #define SnP_Permute_6rounds  Xoodoo_Permute_6rounds
    #define SnP_Permute_12rounds Xoodoo_Permute_12rounds
    #ifdef Xoodoo_HasNround
        #define SnP_Permute_Nrounds  Xoodoo_Permute_Nrounds
    #endif
    #define SnP_Permute_maxRounds 12
    #define SnP_NoFastLoopAbsorb
        #include "testSnP.inc"
    #undef prefix
    #undef SnP
    #undef SnP_stateAlignment
    #undef SnP_width
    #undef SnP_laneCount
    #undef SnP_Permute
    #undef SnP_Permute_6rounds
    #undef SnP_Permute_12rounds
    #undef SnP_Permute_Nrounds
    #undef SnP_Permute_maxRounds
    #undef SnP_NoFastLoopAbsorb
#endif

#ifdef XKCP_has_Xoodootimes4
    #include "Xoodoo-times4-SnP.h"

    #define prefix                      Xoodootimes4
    #define PlSnP                       Xoodootimes4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll_6rounds    Xoodootimes4_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes4_PermuteAll_12rounds
    #define PlSnP_PermuteAll            PlSnP_PermuteAll_12rounds
    #define PlSnP_NoFastLoopAbsorb
    #define PlSnP_X                     8
    #define SnP_width                   384
    #define SnP_laneCount               12
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_PermuteAll
    #undef PlSnP_NoFastLoopAbsorb
    #undef PlSnP_X
    #undef SnP_width
    #undef SnP_laneCount
#endif

#ifdef XKCP_has_Xoodootimes8
    #include "Xoodoo-times8-SnP.h"

    #define prefix                      Xoodootimes8
    #define PlSnP                       Xoodootimes8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll_6rounds    Xoodootimes8_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes8_PermuteAll_12rounds
    #define PlSnP_PermuteAll            PlSnP_PermuteAll_12rounds
    #define PlSnP_NoFastLoopAbsorb
    #define PlSnP_X                     8
    #define SnP_width                   384
    #define SnP_laneCount               12
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_PermuteAll
    #undef PlSnP_NoFastLoopAbsorb
    #undef PlSnP_X
    #undef SnP_width
    #undef SnP_laneCount
#endif

#ifdef XKCP_has_Xoodootimes16
    #include "Xoodoo-times16-SnP.h"

    #define prefix                      Xoodootimes16
    #define PlSnP                       Xoodootimes16
    #define PlSnP_parallelism           16
    #define PlSnP_PermuteAll_6rounds    Xoodootimes16_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes16_PermuteAll_12rounds
    #define PlSnP_PermuteAll            PlSnP_PermuteAll_12rounds
    #define PlSnP_NoFastLoopAbsorb
    #define PlSnP_X                     8
    #define SnP_width                   384
    #define SnP_laneCount               12
        #include "testPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_PermuteAll
    #undef PlSnP_NoFastLoopAbsorb
    #undef PlSnP_X
    #undef SnP_width
    #undef SnP_laneCount
#endif

void testPermutations()
{
#ifdef XKCP_has_KeccakP200
    KeccakP200_testSnP("KeccakP-200.txt", "Keccak-p[200]",
        (const uint8_t*)"\x73\x1b\x9a\xc1\x78\x5f\xaf\x1a\x48\xf7\x70\x53\x59\x98\x63\x19\x95\x74\xcd\xe8\x14\xbf\x13\xfe\x9f"
    );
#endif

#ifdef XKCP_has_KeccakP400
    KeccakP400_testSnP("KeccakP-400.txt", "Keccak-p[400]",
        (const uint8_t*)"\xa9\xd6\xb2\x51\x10\xa8\xf2\x75\x77\x4a\xbf\x8c\x7d\xc9\x69\x5a\x87\x49\x2d\x82\x75\x8b\x9a\x00\x84\x25\xb3\x72\x2b\x9b\x48\xd7\xdc\xd9\xc8\xcd\x26\x69\x69\x19\xe5\xf7\x7f\x10\x48\xcc\xdc\xe9\x4b\xe4"
    );
#endif

#ifdef XKCP_has_KeccakP800
    KeccakP800_testSnP("KeccakP-800.txt", "Keccak-p[800]",
        (const uint8_t*)"\x88\x55\x7e\x60\x84\xe4\x32\x1b\xcc\x4b\x57\x0a\x8e\xb4\x33\xbd\xbb\xc2\x05\x0a\x15\xed\x61\xcc\xea\x85\x00\x77\xbd\x3e\xfa\x18\x4b\x3b\x8f\x47\x89\x16\x31\x05\xb9\x27\x24\x09\xc7\x5e\x8d\xa3\x84\x3f\xfb\x35\x61\x5c\xc7\xe9\x3c\xc4\xd2\x2a\xb2\x4d\xd3\x1e\x67\x29\x61\xc7\x38\x6d\x86\xc2\x98\xc6\x43\xf4\x32\x69\xc9\x05\x0a\xef\x30\x93\xd1\xf1\xb1\xd3\x3a\xf6\x53\x58\x02\xa6\x46\x39\xab\x30\x6c\xe8"
    );
#endif

#ifdef XKCP_has_KeccakP1600
    KeccakP1600_testSnP("KeccakP-1600.txt", "Keccak-p[1600]",
        (const uint8_t*)"\x96\xb5\xfa\x45\x4c\xb4\xcf\x9f\x3d\xbf\x60\xa7\x17\x82\x22\xe0\x07\xe0\xf7\x17\x8f\x24\xac\x62\x7b\x11\xbb\x11\xd3\x97\xbb\x3c\xfd\x0d\xf2\xd7\x38\x40\xd2\x48\x6e\x94\xcf\xb2\xf0\x77\x9e\x8f\x1e\x42\xac\x18\xb1\xb6\x11\x58\xac\x5a\xe6\xeb\x85\x83\x18\x78\x6e\xc2\x85\x70\x07\x0e\xbb\x7a\x76\x11\x71\x83\xaf\x9e\x22\x46\x0b\x61\x22\x7a\xbc\xf5\x2d\x56\xd2\x07\x45\xd1\x41\x00\xcf\x86\x85\xc3\xf0\x33\x16\xd4\x8a\xdb\x81\x68\x7e\x74\x61\x8e\x10\x1b\x4e\x9a\x0d\x37\xc6\x1e\x80\xe9\xa0\xb0\x18\x2e\xc3\x50\x19\xdf\x3b\x0b\x76\x7f\x63\xae\x07\x00\x20\x4a\x3d\x79\x58\x3e\xa6\x2d\xd9\xcb\xf9\x40\x5b\x1c\xbb\x8d\x24\xf3\x13\x03\xfe\xff\x57\x3b\x54\x0c\xc1\x9d\x57\x9e\x59\x26\x61\x1d\x00\x1c\x38\x02\xaa\x03\x59\x76\x5a\x28\x55\xd5\x9d\x2b\x1e\x52\xed\xf7\x04\xe6\x1d\x19\x75\x33\xc3\xfc\x21\x8c\xe6\x80"
    );
#endif
#ifdef XKCP_has_KeccakP1600times2
    KeccakP1600times2_testPlSnP("KeccakP-1600-times2.txt", "Keccak-p[1600]\303\2272",
        (const uint8_t*)"\xf8\x84\x8f\x67\x6a\x80\x87\x4e\xc1\x65\x2e\xea\xb1\x59\xd5\x48\x35\x7b\x12\xa4\xb4\x16\x98\x8d\xdd\xeb\xf8\xa8\x7d\xa8\xf3\xec\xfc\x4e\x86\x22\x60\x96\x43\x48\x39\xc6\xd2\x9f\xca\xa6\xaf\xab\xb2\x20\x63\xdb\x5f\xf9\xc2\xbf\x28\xb3\xff\x9f\x9e\xa8\xca\xb2\xe3\xee\xbf\x73\x3a\x8e\xe6\x68\x6a\x21\x2a\x5a\x55\x34\x64\x3e\xaf\x90\xed\xc4\x7e\x94\x69\x93\x2c\x6c\xbf\x90\x17\x49\xcd\x6e\x65\xab\x26\x7f\xd7\x63\xee\xc0\x1b\x09\x8c\x00\xc2\xf3\x69\x38\xd9\x29\x12\x66\x1a\x95\x5e\x0c\xe2\x4b\x28\x0c\x7b\x90\x90\x1b\x84\xaf\x9f\x43\x34\xad\x9e\x4f\xaa\xdd\xfb\x89\xae\xe6\x00\x05\x6e\xc0\xe0\xe2\x8c\x11\xc6\x46\x5b\xc5\x06\x8c\x23\xa6\xdc\xeb\xb0\x4d\xc0\xee\xc7\x63\xe5\xd2\x85\xd8\x07\xf0\x8f\xf0\x68\x95\xab\xac\xc2\xdf\x9d\xe8\x3c\xb6\x1d\x00\xfc\x87\x38\x6e\x66\x01\x81\x1d\x4d\x83\xe0\xc4\x8c\x05"
    );
#endif
#ifdef XKCP_has_KeccakP1600times4
    KeccakP1600times4_testPlSnP("KeccakP-1600-times4.txt", "Keccak-p[1600]\303\2274",
        (const uint8_t*)"\x68\x78\x6d\x20\x2d\x2e\x1f\x54\x2a\x1f\x0d\x20\xcf\x66\x2e\x16\x96\xda\x51\x39\x5e\xbe\xbc\x61\xc0\x67\x14\x97\x27\x59\x5e\xef\x4a\x55\x75\x59\x47\xe9\x17\x48\x96\x84\x44\x65\x81\xb7\x4a\x05\xe1\x8c\xb4\x98\xb7\x3f\x53\x83\x6a\xaa\xd1\x85\x9f\xf5\xf0\x77\x4c\x09\x65\x07\x25\x5c\xef\x5d\x04\x3b\xb2\x03\xe1\xe5\xd9\x2e\xb5\x86\x4d\x8c\x3e\x78\x97\x23\x1e\x7b\x1a\xd4\x88\xf0\x75\xe8\x34\x4d\x1b\x49\xba\x65\x8e\xc8\xd0\x3a\x0f\x39\x65\x83\xac\xba\xdd\xb9\x85\xcd\x86\x32\x55\x32\x63\x80\xe6\x30\x1f\x7b\xb1\x66\xa3\x09\x55\x13\x9f\xc4\x3b\x04\xec\x35\x0f\x0e\x25\xfb\x83\x6a\xbd\xb8\x99\xac\xa7\x3b\xe4\x9d\x9d\x42\xbc\x5d\x0b\xad\x05\x5a\x5d\x48\x1b\x3c\xc6\x94\xea\x1e\x94\x89\x50\xf7\x67\x63\x1f\xe6\xc3\xe1\x5e\xcd\xb2\xb9\x45\x5a\x3b\x5a\x73\x1a\xdf\x3d\xf0\x41\x06\x47\x20\x92\x4b\xcc\xf0\x59"
    );
#endif
#ifdef XKCP_has_KeccakP1600times8
    KeccakP1600times8_testPlSnP("KeccakP-1600-times8.txt", "Keccak-p[1600]\303\2278",
        (const uint8_t*)"\xc5\x13\xbc\xba\xfd\x29\xd2\xdf\xef\xfe\x45\x6c\x58\x11\xab\xf6\xd1\xb9\xfa\xbf\x9b\x0d\x34\x54\x60\xeb\x0b\x75\x32\x89\x27\x8e\x15\x50\xec\xcf\x9f\x88\x48\xd2\x54\x0d\x1f\xe6\x64\xba\xbc\xc5\x94\x42\xb5\xa4\x32\x2c\xe2\xae\xad\xcc\x24\x12\x8e\x1c\x8d\x46\xa4\xda\x0f\xa7\xa0\xd1\xd0\xb6\xc9\xad\xbe\x6c\xb4\x90\xd9\x95\x21\xe6\x43\xfd\x6f\x1f\x05\x47\x2a\xad\x6d\x13\xe6\xd9\xfc\x27\xfa\x63\x71\xab\x01\xe2\x19\x19\x24\x4b\x20\xe7\xf6\x54\x43\xcb\xf8\x0b\x30\xd7\xb5\x48\x61\x71\x72\x94\x1f\x6c\xb7\xb6\x6b\xfe\xff\x10\xec\x7c\x53\x28\x24\x53\xea\x57\xc4\x3b\xfb\xa6\x16\xfd\x3a\xe1\x23\x04\x74\xbb\x62\x19\x2c\x79\x78\x6e\x9b\xb6\xa4\x59\xe0\xdb\xd1\xe0\x7e\x90\x27\x33\xb2\x8f\x88\xe0\xd8\x12\x4a\xd2\x52\x6a\x8e\x30\x47\x6e\xb9\xea\x28\x2e\xbb\x45\x73\xca\xc8\x2f\xbf\x09\xa2\x06\x39\x07\xde\xd5"
    );
#endif
#ifdef XKCP_has_Xoodoo
    #if defined(Xoodoo_HasNround)
    Xoodoo_testSnP("Xoodoo.txt", "Xoodoo", (const uint8_t*)"\xaf\x76\x84\xce\x28\xc2\x6d\xe1\xbc\x2d\x12\x36\x66\x64\x22\x13\xd4\x13\x06\x5a\x51\x41\x81\xf2\xa9\x01\xd2\xf6\x9a\x2f\x67\x0c\xe0\x00\x21\x1b\x36\x0c\xaa\x58\x90\x24\x1b\xb7\xed\x0c\x09\x99");
    #else
    Xoodoo_testSnP("Xoodoo.txt", "Xoodoo", (const uint8_t*)"\x71\xc5\xff\xa0\x9d\x60\x47\xc2\xbe\x07\x50\xc0\xb1\x73\x20\xaa\xae\x13\xe9\x32\xc6\x16\xcc\x3a\x27\xc6\xce\x00\x35\xb9\x56\x30\x11\x9a\xdb\x51\x49\x36\xb5\xc8\xf1\xaa\x0c\xf0\x47\xd7\x59\x5e");
    #endif
#endif
#ifdef XKCP_has_Xoodootimes4
    Xoodootimes4_testPlSnP("Xoodoo-times4.txt", "Xoodoo\303\2274",
        (const uint8_t*)"\xb9\x5e\x54\xf5\x66\x51\xb5\x4b\x12\x42\x7b\x29\xbd\x2c\x31\x46\x8a\x50\x41\xe4\xee\x72\x04\xf5\x61\xcf\xf2\xde\xf2\x9d\x8d\x89\xaa\xa4\xc3\x09\x36\x71\x8c\x29\x8a\xa7\x1a\x14\x52\x38\xc5\x06"
    );
#endif
#ifdef XKCP_has_Xoodootimes8
    Xoodootimes8_testPlSnP("Xoodoo-times8.txt", "Xoodoo\303\2278",
        (const uint8_t*)"\xc3\x1a\xea\x34\x51\xfe\x13\xf8\x98\xf0\xef\x07\xaa\x30\x93\x71\x69\x05\xef\xe0\x53\xa6\x2f\x46\xac\xa3\x6e\x6b\xb7\x2f\x82\x26\x6b\x0a\x1d\x8a\xf1\xb6\x7a\x8b\x9c\xd9\x38\x13\xe0\x3f\x67\x76"
    );
#endif
#ifdef XKCP_has_Xoodootimes16
    Xoodootimes16_testPlSnP("Xoodoo-times16.txt", "Xoodoo\303\22716",
        (const uint8_t*)"\xa6\x50\x15\xb2\xd9\x16\x80\x6c\x62\x41\x09\x4c\x93\x28\x9b\xce\x57\xbe\xe5\x10\x7b\x36\x89\xeb\xed\xb9\x01\xa1\x18\x8c\x2e\x27\xc8\xd2\xcc\xad\x24\xe1\x77\xc4\xe3\x3b\x2c\x63\x81\xb2\xe0\xc0"
    );
#endif
}
