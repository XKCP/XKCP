/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Ketje, designed by Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.

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
#include "Ket.h"
#include "Ketjev2.h"

#ifdef XKCP_has_KeccakP200
    #include "KeccakP-200-SnP.h"

    #define prefix                      KetjeJr
    #define prefixKet                   KetJr
    #define SnP                         KeccakP200
    #define SnP_width                   200
    #define SnP_PermuteRounds           KeccakP200_Permute_Nrounds
        #include "Ketjev2.inc"
    #undef prefix
    #undef prefixKet
    #undef SnP
    #undef SnP_width
    #undef SnP_PermuteRounds
#endif

#ifdef XKCP_has_KeccakP400
    #include "KeccakP-400-SnP.h"

    #define prefix                      KetjeSr
    #define prefixKet                   KetSr
    #define SnP                         KeccakP400
    #define SnP_width                   400
    #define SnP_PermuteRounds           KeccakP400_Permute_Nrounds
        #include "Ketjev2.inc"
    #undef prefix
    #undef prefixKet
    #undef SnP
    #undef SnP_width
    #undef SnP_PermuteRounds
#endif

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"

    #define prefix                      KetjeMn
    #define prefixKet                   KetMn
    #define SnP                         KeccakP800
    #define SnP_width                   800
    #define SnP_PermuteRounds           KeccakP800_Permute_Nrounds
        #include "Ketjev2.inc"
    #undef prefix
    #undef prefixKet
    #undef SnP
    #undef SnP_width
    #undef SnP_PermuteRounds
#endif

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"

    #define prefix                      KetjeMj
    #define prefixKet                   KetMj
    #define SnP                         KeccakP1600
    #define SnP_width                   1600
    #define SnP_PermuteRounds           KeccakP1600_Permute_Nrounds
        #include "Ketjev2.inc"
    #undef prefix
    #undef prefixKet
    #undef SnP
    #undef SnP_width
    #undef SnP_PermuteRounds
#endif
