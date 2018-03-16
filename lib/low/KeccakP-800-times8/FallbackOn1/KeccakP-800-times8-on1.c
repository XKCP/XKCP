/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Keccak-p[800]×8 in a PlSnP-compatible way.
Please refer to PlSnP-documentation.h for more details.

This implementation comes with KeccakP-800-times8-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include "KeccakP-800-SnP.h"

#define prefix                          KeccakP800times8
#define PlSnP_baseParallelism           1
#define PlSnP_targetParallelism         8
#define SnP_laneLengthInBytes           4
#define SnP                             KeccakP800
#define SnP_Permute                     KeccakP800_Permute_22rounds
#define SnP_Permute_12rounds            KeccakP800_Permute_12rounds
#define PlSnP_PermuteAll                KeccakP800times8_PermuteAll_22rounds
#define PlSnP_PermuteAll_12rounds       KeccakP800times8_PermuteAll_12rounds

#include "PlSnP-Fallback.inc"
