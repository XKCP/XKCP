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

#include "Keyakv2.h"

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix                      River
    #define prefixMotorist              KeyakWidth800
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix                      Lake
    #define prefixMotorist              KeyakWidth1600
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      Sea
    #define prefixMotorist              KeyakWidth1600times2
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      Ocean
    #define prefixMotorist              KeyakWidth1600times4
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      Lunar
    #define prefixMotorist              KeyakWidth1600times8
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif

