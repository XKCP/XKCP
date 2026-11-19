/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by the XKCP contributors, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

Keccak-p[1600]x4 for AArch64 using the Becker-Kannwischer scalar+NEON hybrid
permutation core from mlkem-native. Four states are stored sequentially
(KeccakP1600_state states[4] == uint64_t[100]), matching the core's ABI; all
byte/lane management falls back serially on the single-state (x1) SnP backend
via PlSnP-Fallback.inc, and only the 24-round permutation and absorb fast-loop
use the parallel hybrid core.

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to PlSnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_times4_SnP_h_
#define _KeccakP_1600_times4_SnP_h_

#include <stddef.h>
#include "align.h"
#include "KeccakP-1600-SnP.h"
#include "PlSnP-common.h"

typedef struct {
    ALIGN(32) KeccakP1600_state states[4];
} KeccakP1600times4_states;

#if defined(__ARM_FEATURE_SHA3)
#define KeccakP1600times4_GetImplementation()       "ARMv8.4-A scalar+NEON+SHA3 hybrid implementation (x4, mlkem-native core)"
#else
#define KeccakP1600times4_GetImplementation()       "ARMv8-A scalar+NEON hybrid implementation (x4, mlkem-native core)"
#endif
#define KeccakP1600times4_GetFeatures()             (PlSnP_Feature_Main | PlSnP_Feature_SpongeAbsorb)
#define KeccakP1600times4_statesAlignment           32

void KeccakP1600times4_StaticInitialize(void);
void KeccakP1600times4_InitializeAll(KeccakP1600times4_states *states);
void KeccakP1600times4_AddByte(KeccakP1600times4_states *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
void KeccakP1600times4_AddBytes(KeccakP1600times4_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times4_AddLanesAll(KeccakP1600times4_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times4_OverwriteBytes(KeccakP1600times4_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times4_OverwriteLanesAll(KeccakP1600times4_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times4_OverwriteWithZeroes(KeccakP1600times4_states *states, unsigned int instanceIndex, unsigned int byteCount);
void KeccakP1600times4_PermuteAll_4rounds(KeccakP1600times4_states *states);
void KeccakP1600times4_PermuteAll_6rounds(KeccakP1600times4_states *states);
void KeccakP1600times4_PermuteAll_12rounds(KeccakP1600times4_states *states);
void KeccakP1600times4_PermuteAll_24rounds(KeccakP1600times4_states *states);
void KeccakP1600times4_ExtractBytes(const KeccakP1600times4_states *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times4_ExtractLanesAll(const KeccakP1600times4_states *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times4_ExtractAndAddBytes(const KeccakP1600times4_states *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void KeccakP1600times4_ExtractAndAddLanesAll(const KeccakP1600times4_states *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

size_t KeccakF1600times4_FastLoop_Absorb(KeccakP1600times4_states *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
size_t KeccakP1600times4_12rounds_FastLoop_Absorb(KeccakP1600times4_states *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);

#define KeccakP1600times4_KravatteCompress(...)             0
#define KeccakP1600times4_KravatteExpand(...)               0
#define KeccakP1600times4_KT128ProcessLeaves(...)
#define KeccakP1600times4_KT256ProcessLeaves(...)

#define KeccakP1600times4_TurboSHAKE128_FlightDeckProcessLeaves(...)    0
#define KeccakP1600times4_TurboSHAKE256_FlightDeckProcessLeaves(...)    0
#define KeccakP1600times4_SHAKE128_FlightDeckProcessLeaves(...)         0
#define KeccakP1600times4_SHAKE256_FlightDeckProcessLeaves(...)         0
#define KeccakP1600times4_TurboSHAKE128_FlightDeckSqueeze(...)          0
#define KeccakP1600times4_TurboSHAKE256_FlightDeckSqueeze(...)          0
#define KeccakP1600times4_SHAKE128_FlightDeckSqueeze(...)               0
#define KeccakP1600times4_SHAKE256_FlightDeckSqueeze(...)               0

#endif
