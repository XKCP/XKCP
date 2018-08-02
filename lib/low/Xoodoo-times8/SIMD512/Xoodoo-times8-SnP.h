/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_times8_SnP_h_
#define _Xoodoo_times8_SnP_h_

/** For the documentation, see PlSnP-documentation.h.
 */

#define Xoodootimes8_implementation        "512-bit SIMD (AVX-512) implementation"
#define Xoodootimes8_statesSizeInBytes     (8*3*4*4)
#define Xoodootimes8_statesAlignment       64

#define Xoodootimes8_StaticInitialize()
void Xoodootimes8_InitializeAll(void *states);
#define Xoodootimes8_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*4 + ((offset)/4)*8*4 + (offset)%4] ^= (byte)
void Xoodootimes8_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes8_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes8_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes8_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes8_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
void Xoodootimes8_PermuteAll_6rounds(void *states);
void Xoodootimes8_PermuteAll_12rounds(void *states);
void Xoodootimes8_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes8_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes8_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void Xoodootimes8_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#define Xoodootimes8_FastXoofff_supported
void Xooffftimes8_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen);
size_t Xooffftimes8_CompressFastLoop(unsigned char *k, unsigned char *x, const unsigned char *input, size_t length);
size_t Xooffftimes8_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length);

#endif
