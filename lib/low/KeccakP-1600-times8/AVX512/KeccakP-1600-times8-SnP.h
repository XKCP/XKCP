/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to PlSnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_times8_SnP_h_
#define _KeccakP_1600_times8_SnP_h_

#include <immintrin.h>
#include <stdint.h>
#include "SIMD512-config.h"

typedef __m512i  V512;

typedef struct {
    V512 A[25];
} KeccakP1600times8_SIMD512_states;

typedef KeccakP1600times8_SIMD512_states KeccakP1600times8_states;

#define KeccakP1600times8_implementation        "512-bit SIMD implementation (" KeccakP1600times8_implementation_config ")"
#define KeccakF1600times8_FastLoop_supported
#define KeccakP1600times8_12rounds_FastLoop_supported
#define KeccakF1600times8_FastKravatte_supported
#define KeccakP1600times8_K12ProcessLeaves_supported

#define KeccakP1600times8_StaticInitialize()
void KeccakP1600times8_InitializeAll(KeccakP1600times8_SIMD512_states *states);
#define KeccakP1600times8_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*8 + ((offset)/8)*8*8 + (offset)%8] ^= (byte)
void KeccakP1600times8_AddBytes(KeccakP1600times8_SIMD512_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times8_AddLanesAll(KeccakP1600times8_SIMD512_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times8_OverwriteBytes(KeccakP1600times8_SIMD512_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times8_OverwriteLanesAll(KeccakP1600times8_SIMD512_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times8_OverwriteWithZeroes(KeccakP1600times8_SIMD512_states *states, unsigned int instanceIndex, unsigned int byteCount);
void KeccakP1600times8_PermuteAll_4rounds(KeccakP1600times8_SIMD512_states *states);
void KeccakP1600times8_PermuteAll_6rounds(KeccakP1600times8_SIMD512_states *states);
void KeccakP1600times8_PermuteAll_12rounds(KeccakP1600times8_SIMD512_states *states);
void KeccakP1600times8_PermuteAll_24rounds(KeccakP1600times8_SIMD512_states *states);
void KeccakP1600times8_ExtractBytes(const KeccakP1600times8_SIMD512_states *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times8_ExtractLanesAll(const KeccakP1600times8_SIMD512_states *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times8_ExtractAndAddBytes(const KeccakP1600times8_SIMD512_states *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void KeccakP1600times8_ExtractAndAddLanesAll(const KeccakP1600times8_SIMD512_states *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);
size_t KeccakF1600times8_FastLoop_Absorb(KeccakP1600times8_SIMD512_states *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
size_t KeccakP1600times8_12rounds_FastLoop_Absorb(KeccakP1600times8_SIMD512_states *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
size_t KeccakP1600times8_KravatteCompress(uint64_t *xAccu, uint64_t *kRoll, const unsigned char *input, size_t inputByteLen);
size_t KeccakP1600times8_KravatteExpand(uint64_t *yAccu, const uint64_t *kRoll, unsigned char *output, size_t outputByteLen);
void KeccakP1600times8_K12ProcessLeaves(const unsigned char *input, unsigned char *output);

#endif
