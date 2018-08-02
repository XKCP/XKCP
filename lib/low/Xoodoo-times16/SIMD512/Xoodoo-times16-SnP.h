/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_times16_SnP_h_
#define _Xoodoo_times16_SnP_h_

/** For the documentation, see PlSnP-documentation.h.
 */

#define Xoodootimes16_implementation        "512-bit SIMD implementation"
#define Xoodootimes16_statesSizeInBytes     (16*3*4*4)
#define Xoodootimes16_statesAlignment       64

#define Xoodootimes16_StaticInitialize()
void Xoodootimes16_InitializeAll(void *states);
#define Xoodootimes16_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*4 + ((offset)/4)*16*4 + (offset)%4] ^= (byte)
void Xoodootimes16_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes16_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes16_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes16_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes16_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
void Xoodootimes16_PermuteAll_Nrounds(void *states, unsigned int nr);
void Xoodootimes16_PermuteAll_6rounds(void *states);
void Xoodootimes16_PermuteAll_12rounds(void *states);
void Xoodootimes16_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes16_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes16_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void Xoodootimes16_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#define Xoodootimes16_FastXoofff_supported
void Xooffftimes16_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen);
size_t Xooffftimes16_CompressFastLoop(unsigned char *k, unsigned char *x, const unsigned char *input, size_t length);
size_t Xooffftimes16_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length);

#endif
