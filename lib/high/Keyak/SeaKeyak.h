/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _SeaKeyak_h_
#define _SeaKeyak_h_

#include "Keyak-common.h"

/** Length of the Sea Keyak key pack. */
#define SeaKeyak_Lk                    40

/** Maximum nonce length for Sea Keyak. */
#define SeaKeyak_MaxNoncelength        150

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"
    KCP_DeclarePistonsStructure(KeyakWidth1600times2, KeccakP1600times2_statesSizeInBytes, KeccakP1600times2_statesAlignment)
    KCP_DeclareEngineStructure(KeyakWidth1600times2, KeccakP1600times2_statesSizeInBytes, KeccakP1600times2_statesAlignment)
    KCP_DeclareMotoristStructure(KeyakWidth1600times2, KeccakP1600times2_statesSizeInBytes, KeccakP1600times2_statesAlignment)
    KCP_DeclareMotoristFunctions(KeyakWidth1600times2)
    KCP_DeclareKeyakStructure(Sea, KeyakWidth1600times2, KeccakP1600times2_statesAlignment)
    KCP_DeclareKeyakFunctions(Sea)
#endif

#endif
