/*
Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeccakPRGWidth400_h_
#define _KeccakPRGWidth400_h_

#include "KeccakDuplexWidth400.h"
#include "KeccakPRG-common.h"

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"
    KCP_DeclareSpongePRG_Structure(KeccakWidth400, KeccakP400_stateSizeInBytes, KeccakP400_stateAlignment)
    KCP_DeclareSpongePRG_Functions(KeccakWidth400)
#endif

#endif
