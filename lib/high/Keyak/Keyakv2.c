/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Keyak, designed by Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include "config.h"
#include "Keyakv2.h"

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"

    #define prefix                      KeyakWidth800
    #define SnP                         KeccakP800
    #define SnP_width                   800
    #define PlSnP_parallelism           1
    #define SnP_Permute KeccakP800_Permute_12rounds
        #include "Motorist.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef PlSnP_parallelism
    #undef SnP_Permute

    #define prefix                      River
    #define prefixMotorist              KeyakWidth800
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"

    #define prefix                      KeyakWidth1600
    #define SnP                         KeccakP1600
    #define SnP_width                   1600
    #define PlSnP_parallelism           1
    #define SnP_Permute KeccakP1600_Permute_12rounds
        #include "Motorist.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef PlSnP_parallelism
    #undef SnP_Permute

    #define prefix                      Lake
    #define prefixMotorist              KeyakWidth1600
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifdef XKCP_has_KeccakP1600times2
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeyakWidth1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define PlSnP_PermuteAll            KeccakP1600times2_PermuteAll_12rounds
    #define SnP_width                   1600
        #include "Motorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef SnP_width

    #define prefix                      Sea
    #define prefixMotorist              KeyakWidth1600times2
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifdef XKCP_has_KeccakP1600times4
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      KeyakWidth1600times4
    #define PlSnP                       KeccakP1600times4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll            KeccakP1600times4_PermuteAll_12rounds
    #define SnP_width                   1600
        #include "Motorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef SnP_width

    #define prefix                      Ocean
    #define prefixMotorist              KeyakWidth1600times4
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifdef XKCP_has_KeccakP1600times8
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      KeyakWidth1600times8
    #define PlSnP                       KeccakP1600times8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll            KeccakP1600times8_PermuteAll_12rounds
    #define SnP_width                   1600
        #include "Motorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef SnP_width

    #define prefix                      Lunar
    #define prefixMotorist              KeyakWidth1600times8
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif
