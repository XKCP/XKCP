/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeccakSpongeWidth400_h_
#define _KeccakSpongeWidth400_h_

#include "KeccakSponge-common.h"

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"
    KCP_DeclareSpongeStructure(KeccakWidth400, KeccakP400_stateSizeInBytes, KeccakP400_stateAlignment)
    KCP_DeclareSpongeFunctions(KeccakWidth400)
#endif

#endif
