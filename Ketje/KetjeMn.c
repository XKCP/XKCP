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

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include "KetMn.h"
#include "KetjeMn.h"

#ifndef KeccakP800_excluded
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
