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

#include "SIMD512-config.h"

#define KeccakP1600times8_implementation        "512-bit SIMD implementation (" KeccakP1600times8_implementation_config ")"
#define KeccakP1600times8_statesSizeInBytes     1600
#define KeccakP1600times8_statesAlignment       64
#define KeccakF1600times8_FastLoop_supported
#define KeccakP1600times8_12rounds_FastLoop_supported
#define KeccakF1600times8_FastKravatte_supported
#define KeccakP1600times8_K12ProcessLeaves_supported

#include <stddef.h>
#include <stdint.h>

#define KeccakP1600times8_StaticInitialize()
void KeccakP1600times8_InitializeAll(void *states);
#define KeccakP1600times8_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*8 + ((offset)/8)*8*8 + (offset)%8] ^= (byte)
void KeccakP1600times8_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times8_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times8_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times8_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times8_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
void KeccakP1600times8_PermuteAll_4rounds(void *states);
void KeccakP1600times8_PermuteAll_6rounds(void *states);
void KeccakP1600times8_PermuteAll_12rounds(void *states);
void KeccakP1600times8_PermuteAll_24rounds(void *states);
void KeccakP1600times8_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP1600times8_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP1600times8_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void KeccakP1600times8_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);
size_t KeccakF1600times8_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
size_t KeccakP1600times8_12rounds_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);
size_t KeccakP1600times8_KravatteCompress(uint64_t *xAccu, uint64_t *kRoll, const unsigned char *input, size_t inputByteLen);
size_t KeccakP1600times8_KravatteExpand(uint64_t *yAccu, const uint64_t *kRoll, unsigned char *output, size_t outputByteLen);
void KeccakP1600times8_K12ProcessLeaves(const unsigned char *input, unsigned char *output);

#endif
