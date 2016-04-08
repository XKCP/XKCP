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

#include "KeccakPRG.h"

#ifdef KeccakReference
    #include <string.h>
    #include "displayIntermediateValues.h"
#endif

#ifndef KeccakP200_excluded
    #include "KeccakP-200-SnP.h"

    #define prefix KeccakWidth200
    #define SnP_width 200
        #include "KeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"

    #define prefix KeccakWidth400
    #define SnP_width 400
        #include "KeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix KeccakWidth800
    #define SnP_width 800
        #include "KeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakWidth1600
    #define SnP_width 1600
        #include "KeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif
