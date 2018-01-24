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

#ifndef _KeccakP_800_times4_SnP_h_
#define _KeccakP_800_times4_SnP_h_

#include "KeccakP-800-SnP.h"

#define KeccakP800times4_implementation         "fallback on serial implementation (" KeccakP800_implementation ")"
#define KeccakP800times4_statesSizeInBytes      (((KeccakP800_stateSizeInBytes+(KeccakP800_stateAlignment-1))/KeccakP800_stateAlignment)*KeccakP800_stateAlignment*4)
#define KeccakP800times4_statesAlignment        KeccakP800_stateAlignment

void KeccakP800times4_StaticInitialize( void );
void KeccakP800times4_InitializeAll(void *states);
void KeccakP800times4_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);
void KeccakP800times4_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800times4_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP800times4_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800times4_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP800times4_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);
void KeccakP800times4_PermuteAll_12rounds(void *states);
void KeccakP800times4_PermuteAll_22rounds(void *states);
void KeccakP800times4_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800times4_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
void KeccakP800times4_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
void KeccakP800times4_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#endif
