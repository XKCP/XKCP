/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _LakeKeyak_h_
#define _LakeKeyak_h_

#include "Keyak-common.h"

/** Length of the Lake Keyak key pack. */
#define LakeKeyak_Lk                   40

/** Maximum nonce length for Lake Keyak. */
#define LakeKeyak_MaxNoncelength       150

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"
    KCP_DeclarePistonsStructure(KeyakWidth1600, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    KCP_DeclareEngineStructure(KeyakWidth1600, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    KCP_DeclareMotoristStructure(KeyakWidth1600, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    KCP_DeclareMotoristFunctions(KeyakWidth1600)
    KCP_DeclareKeyakStructure(Lake, KeyakWidth1600, KeccakP1600_stateAlignment)
    KCP_DeclareKeyakFunctions(Lake)
#endif

#endif
