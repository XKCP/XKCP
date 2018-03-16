/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _OceanKeyak_h_
#define _OceanKeyak_h_

#include "Keyak-common.h"

/** Length of the Ocean Keyak key pack. */
#define OceanKeyak_Lk                  40

/** Maximum nonce length for Ocean Keyak. */
#define OceanKeyak_MaxNoncelength      150

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times4-SnP.h"
    KCP_DeclarePistonsStructure(KeyakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    KCP_DeclareEngineStructure(KeyakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    KCP_DeclareMotoristStructure(KeyakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    KCP_DeclareMotoristFunctions(KeyakWidth1600times4)
    KCP_DeclareKeyakStructure(Ocean, KeyakWidth1600times4, KeccakP1600times4_statesAlignment)
    KCP_DeclareKeyakFunctions(Ocean)
#endif

#endif
