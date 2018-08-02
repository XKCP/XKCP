/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_times4_SnP_h_
#define _Xoodoo_times4_SnP_h_

/** For the documentation, see PlSnP-documentation.h.
 */

#define Xoodootimes4_implementation        "512-bit SIMD (AVX-512) implementation"
#define Xoodootimes4_statesSizeInBytes     (4*3*4*4)
#define Xoodootimes4_statesAlignment       64

#define Xoodootimes4_StaticInitialize()
void Xoodootimes4_InitializeAll(void *states);
#define Xoodootimes4_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*4 + ((offset)/4)*4*4 + (offset)%4] ^= (byte)
void Xoodootimes4_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes4_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes4_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes4_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes4_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
void Xoodootimes4_PermuteAll_6rounds(void *states);
void Xoodootimes4_PermuteAll_12rounds(void *states);
void Xoodootimes4_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes4_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes4_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void Xoodootimes4_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#define Xoodootimes4_FastXoofff_supported
void Xooffftimes4_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen);
size_t Xooffftimes4_CompressFastLoop(unsigned char *k, unsigned char *x, const unsigned char *input, size_t length);
size_t Xooffftimes4_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length);

#endif
