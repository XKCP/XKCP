/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KetjeJr_h_
#define _KetjeJr_h_

#include "Ketje-common.h"

#ifndef KeccakP200_excluded
    #include "KeccakP-200-SnP.h"
    KCP_DeclareKetjeStructure(KetjeJr, KeccakP200_stateSizeInBytes, KeccakP200_stateAlignment)
    KCP_DeclareKetjeFunctions(KetjeJr)
#endif

#endif
