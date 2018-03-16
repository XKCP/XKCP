/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include "KetSr.h"
#include "KetjeSr.h"

#ifndef KeccakP400_excluded
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
