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

#include "KeccakP-800-SnP.h"

#define prefix                          KeccakP800times2
#define PlSnP_baseParallelism           1
#define PlSnP_targetParallelism         2
#define SnP_laneLengthInBytes           4
#define SnP                             KeccakP800
#define SnP_Permute                     KeccakP800_Permute_22rounds
#define SnP_Permute_12rounds            KeccakP800_Permute_12rounds
#define PlSnP_PermuteAll                KeccakP800times2_PermuteAll_22rounds
#define PlSnP_PermuteAll_12rounds       KeccakP800times2_PermuteAll_12rounds

#include "PlSnP-Fallback.inc"
