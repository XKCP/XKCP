/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Xoodoo×8 in a PlSnP-compatible way.
Please refer to PlSnP-documentation.h for more details.

This implementation comes with Xoodoo-times8-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include "Xoodoo-SnP.h"

#define prefix                          Xoodootimes8
#define PlSnP_baseParallelism           1
#define PlSnP_targetParallelism         8
#define SnP_laneLengthInBytes           4
#define SnP                             Xoodoo
#define SnP_Permute                     Xoodoo_Permute_6rounds
#define SnP_Permute_12rounds            Xoodoo_Permute_12rounds
#define PlSnP_PermuteAll                Xoodootimes8_PermuteAll_6rounds
#define PlSnP_PermuteAll_12rounds       Xoodootimes8_PermuteAll_12rounds

#include "PlSnP-Fallback.inc"
