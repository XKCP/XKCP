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

#include "KeccakP-1600-SnP.h"

#define prefix                          KeccakP1600times4
#define PlSnP_baseParallelism           1
#define PlSnP_targetParallelism         4
#define SnP_laneLengthInBytes           8
#define SnP                             KeccakP1600
#define SnP_Permute                     KeccakP1600_Permute_24rounds
#define SnP_Permute_12rounds            KeccakP1600_Permute_12rounds
#define PlSnP_PermuteAll                KeccakP1600times4_PermuteAll_24rounds
#define PlSnP_PermuteAll_12rounds       KeccakP1600times4_PermuteAll_12rounds

#include "PlSnP-Fallback.inc"
