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

#define Xoodootimes16_implementation        "fallback on serial implementation (" Xoodoo_implementation ")"
#define Xoodootimes16_statesSizeInBytes     (((Xoodoo_stateSizeInBytes+(Xoodoo_stateAlignment-1))/Xoodoo_stateAlignment)*Xoodoo_stateAlignment*16)
#define Xoodootimes16_statesAlignment       Xoodoo_stateAlignment
#define Xoodootimes16_isFallback

void Xoodootimes16_StaticInitialize(void);
void Xoodootimes16_InitializeAll(void *states);
void Xoodootimes16_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
void Xoodootimes16_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes16_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes16_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes16_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes16_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
void Xoodootimes16_PermuteAll_6rounds(void *states);
void Xoodootimes16_PermuteAll_12rounds(void *states);
void Xoodootimes16_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void Xoodootimes16_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void Xoodootimes16_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void Xoodootimes16_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#endif
