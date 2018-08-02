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

#define Xoodootimes8_implementation        "fallback on serial implementation (" Xoodoo_implementation ")"
#define Xoodootimes8_statesSizeInBytes     (((Xoodoo_stateSizeInBytes+(Xoodoo_stateAlignment-1))/Xoodoo_stateAlignment)*Xoodoo_stateAlignment*8)
#define Xoodootimes8_statesAlignment       Xoodoo_stateAlignment
#define Xoodootimes8_isFallback

void Xoodootimes8_StaticInitialize(void);
void Xoodootimes8_InitializeAll(void *states);
void Xoodootimes8_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
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

#endif
