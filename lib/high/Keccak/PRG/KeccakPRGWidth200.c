/*
Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakPRGWidth200.h"

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
