/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KetjeMn_h_
#define _KetjeMn_h_

#include "Ketje-common.h"

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"
    KCP_DeclareKetjeStructure(KetjeMn, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    KCP_DeclareKetjeFunctions(KetjeMn)
#endif

#endif
