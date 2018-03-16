/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KetjeSr_h_
#define _KetjeSr_h_

#include "Ketje-common.h"

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"
    KCP_DeclareKetjeStructure(KetjeSr, KeccakP400_stateSizeInBytes, KeccakP400_stateAlignment)
    KCP_DeclareKetjeFunctions(KetjeSr)
#endif

#endif
