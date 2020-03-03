/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Keccak-p[1600]×4 in a PlSnP-compatible way.
Please refer to PlSnP-documentation.h for more details.

This implementation comes with KeccakP-1600-times4-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include "KeccakP-1600-times2-SnP.h"

#define prefix                          KeccakP1600times4
#define PlSnP_baseParallelism           2
#define PlSnP_targetParallelism         4
#define SnP_laneLengthInBytes           8
#define SnP                             KeccakP1600times2
#define SnP_PermuteAll                  KeccakP1600times2_PermuteAll_24rounds
#define SnP_PermuteAll_12rounds         KeccakP1600times2_PermuteAll_12rounds
#define SnP_PermuteAll_6rounds          KeccakP1600times2_PermuteAll_6rounds
#define SnP_PermuteAll_4rounds          KeccakP1600times2_PermuteAll_4rounds
#define PlSnP_PermuteAll                KeccakP1600times4_PermuteAll_24rounds
#define PlSnP_PermuteAll_12rounds       KeccakP1600times4_PermuteAll_12rounds
#define PlSnP_PermuteAll_6rounds        KeccakP1600times4_PermuteAll_6rounds
#define PlSnP_PermuteAll_4rounds        KeccakP1600times4_PermuteAll_4rounds

#include "PlSnP-Fallback.inc"
