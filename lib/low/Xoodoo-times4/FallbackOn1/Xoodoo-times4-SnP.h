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

#define Xoodootimes4_implementation        "fallback on serial implementation (" Xoodoo_implementation ")"
#define Xoodootimes4_statesSizeInBytes     (((Xoodoo_stateSizeInBytes+(Xoodoo_stateAlignment-1))/Xoodoo_stateAlignment)*Xoodoo_stateAlignment*4)
#define Xoodootimes4_statesAlignment       Xoodoo_stateAlignment
#define Xoodootimes4_isFallback

void Xoodootimes4_StaticInitialize(void);
void Xoodootimes4_InitializeAll(void *states);
void Xoodootimes4_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
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

#endif
