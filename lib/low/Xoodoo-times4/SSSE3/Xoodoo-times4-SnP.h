/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Xoodoo permutation, designed by Joan Daemen, Seth Hoffert, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_times4_SnP_h_
#define _Xoodoo_times4_SnP_h_

#include <emmintrin.h>

/** For the documentation, see PlSnP-documentation.h.
 */

typedef __m128i V128;

typedef struct {
    V128 A[12];
} Xoodootimes4_SIMD128_states;

typedef Xoodootimes4_SIMD128_states Xoodootimes4_states;

#define Xoodootimes4_implementation        "128-bit SIMD implementation"

#define Xoodootimes4_StaticInitialize()
void Xoodootimes4_InitializeAll(Xoodootimes4_SIMD128_states *states);
#define Xoodootimes4_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char*)(states))[(instanceIndex)*4 + ((offset)/4)*4*4 + (offset)%4] ^= (byte)
void Xoodootimes4_AddBytes(Xoodootimes4_SIMD128_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes4_AddLanesAll(Xoodootimes4_SIMD128_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes4_OverwriteBytes(Xoodootimes4_SIMD128_states *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes4_OverwriteLanesAll(Xoodootimes4_SIMD128_states *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes4_OverwriteWithZeroes(Xoodootimes4_SIMD128_states *states, unsigned int instanceIndex, unsigned int byteCount);
//void Xoodootimes4_PermuteAll_Nrounds(Xoodootimes4_SIMD128_states *states, unsigned int nr);
void Xoodootimes4_PermuteAll_6rounds(Xoodootimes4_SIMD128_states *states);
void Xoodootimes4_PermuteAll_12rounds(Xoodootimes4_SIMD128_states *states);
void Xoodootimes4_ExtractBytes(const Xoodootimes4_SIMD128_states *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes4_ExtractLanesAll(const Xoodootimes4_SIMD128_states *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes4_ExtractAndAddBytes(const Xoodootimes4_SIMD128_states *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void Xoodootimes4_ExtractAndAddLanesAll(const Xoodootimes4_SIMD128_states *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#endif
