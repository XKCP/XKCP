/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KetjeMj_h_
#define _KetjeMj_h_

#include "Ketje-common.h"

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"
    KCP_DeclareKetjeStructure(KetjeMj, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    KCP_DeclareKetjeFunctions(KetjeMj)
#endif

#endif
