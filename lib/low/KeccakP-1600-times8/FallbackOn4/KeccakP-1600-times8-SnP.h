/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to PlSnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_times8_SnP_h_
#define _KeccakP_1600_times8_SnP_h_

#include "KeccakP-1600-times4-SnP.h"

#define KeccakP1600times8_implementation        "fallback on times-4 implementation (" KeccakP1600times4_implementation ")"
#define KeccakP1600times8_statesSizeInBytes     (((KeccakP1600times4_statesSizeInBytes+(KeccakP1600times4_statesAlignment-1))/KeccakP1600times4_statesAlignment)*KeccakP1600times4_statesAlignment*2)
#define KeccakP1600times8_statesAlignment       KeccakP1600times4_statesAlignment

void KeccakP1600times8_StaticInitialize( void );
void KeccakP1600times8_InitializeAll(void *states);
void KeccakP1600times8_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
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

#endif
