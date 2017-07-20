/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _RiverKeyak_h_
#define _RiverKeyak_h_

#include "Keyak-common.h"

/** Length of the River Keyak key pack. */
#define RiverKeyak_Lk                  36

/** Maximum nonce length for River Keyak. */
#define RiverKeyak_MaxNoncelength      58

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"
    KCP_DeclarePistonsStructure(KeyakWidth800, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    KCP_DeclareEngineStructure(KeyakWidth800, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    KCP_DeclareMotoristStructure(KeyakWidth800, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    KCP_DeclareMotoristFunctions(KeyakWidth800)
    KCP_DeclareKeyakStructure(River, KeyakWidth800, KeccakP800_stateAlignment)
    KCP_DeclareKeyakFunctions(River)
#endif

#endif
