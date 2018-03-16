/*
Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakPRGWidth1600.h"

#ifdef KeccakReference
    #include <string.h>
    #include "displayIntermediateValues.h"
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix KeccakWidth1600
    #define SnP_width 1600
        #include "KeccakPRG.inc"
    #undef prefix
    #undef SnP_width
#endif
