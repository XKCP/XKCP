/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to PlSnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_times2_SnP_h_
#define _KeccakP_1600_times2_SnP_h_

#include <emmintrin.h>
#include "SIMD128-config.h"

typedef __m128i V128;

typedef struct {
    V128 A[25];
} KeccakP1600times2_SIMD128_states;

typedef KeccakP1600times2_SIMD128_states KeccakP1600times2_states;

#define KeccakP1600times2_implementation        "128-bit SIMD implementation (" KeccakP1600times2_implementation_config ")"
#define KeccakF1600times2_FastLoop_supported

#include <stddef.h>

#define KeccakP1600times2_StaticInitialize()
void KeccakP1600times2_InitializeAll(KeccakP1600times2_SIMD128_states *states);
#define KeccakP1600times2_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*8 + ((offset)/8)*2*8 + (offset)%8] ^= (byte)
void KeccakP1600times2_AddBytes(KeccakP1600times2_SIMD128_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times2_AddLanesAll(KeccakP1600times2_SIMD128_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times2_OverwriteBytes(KeccakP1600times2_SIMD128_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times2_OverwriteLanesAll(KeccakP1600times2_SIMD128_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times2_OverwriteWithZeroes(KeccakP1600times2_SIMD128_states *states, unsigned int instanceIndex, unsigned int byteCount);
void KeccakP1600times2_PermuteAll_4rounds(KeccakP1600times2_SIMD128_states *states);
void KeccakP1600times2_PermuteAll_6rounds(KeccakP1600times2_SIMD128_states *states);
void KeccakP1600times2_PermuteAll_12rounds(KeccakP1600times2_SIMD128_states *states);
void KeccakP1600times2_PermuteAll_24rounds(KeccakP1600times2_SIMD128_states *states);
void KeccakP1600times2_ExtractBytes(const KeccakP1600times2_SIMD128_states *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times2_ExtractLanesAll(const KeccakP1600times2_SIMD128_states *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times2_ExtractAndAddBytes(const KeccakP1600times2_SIMD128_states *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void KeccakP1600times2_ExtractAndAddLanesAll(const KeccakP1600times2_SIMD128_states *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);
size_t KeccakF1600times2_FastLoop_Absorb(KeccakP1600times2_SIMD128_states *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);

#endif
