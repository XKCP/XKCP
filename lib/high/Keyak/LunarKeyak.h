/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _LunarKeyak_h_
#define _LunarKeyak_h_

#include "Keyak-common.h"

/** Length of the Lunar Keyak key pack. */
#define LunarKeyak_Lk                  40

/** Maximum nonce length for Lunar Keyak. */
#define LunarKeyak_MaxNoncelength      150

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times8-SnP.h"
    KCP_DeclarePistonsStructure(KeyakWidth1600times8, KeccakP1600times8_statesSizeInBytes, KeccakP1600times8_statesAlignment)
    KCP_DeclareEngineStructure(KeyakWidth1600times8, KeccakP1600times8_statesSizeInBytes, KeccakP1600times8_statesAlignment)
    KCP_DeclareMotoristStructure(KeyakWidth1600times8, KeccakP1600times8_statesSizeInBytes, KeccakP1600times8_statesAlignment)
    KCP_DeclareMotoristFunctions(KeyakWidth1600times8)
    KCP_DeclareKeyakStructure(Lunar, KeyakWidth1600times8, KeccakP1600times8_statesAlignment)
    KCP_DeclareKeyakFunctions(Lunar)
#endif

#endif
