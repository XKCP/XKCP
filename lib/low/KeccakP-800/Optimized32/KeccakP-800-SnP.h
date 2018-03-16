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

Please refer to SnP-documentation.h for more details.
*/

#ifndef _KeccakP_800_SnP_h_
#define _KeccakP_800_SnP_h_

#include "KeccakP-800-opt32-config.h"

#define KeccakP800_implementation      "generic 32-bit optimized implementation (" KeccakP800_implementation_config ")"
#define KeccakP800_stateSizeInBytes    100
#define KeccakP800_stateAlignment      4
#define KeccakF800_FastLoop_supported

#include <stddef.h>

#define KeccakP800_StaticInitialize()
void KeccakP800_Initialize(void *state);
void KeccakP800_AddByte(void *state, unsigned char data, unsigned int offset);
void KeccakP800_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount);
void KeccakP800_Permute_Nrounds(void *state, unsigned int nrounds);
void KeccakP800_Permute_12rounds(void *state);
void KeccakP800_Permute_22rounds(void *state);
void KeccakP800_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
size_t KeccakF800_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);

#endif
