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

#ifndef _KeccakP_400_SnP_h_
#define _KeccakP_400_SnP_h_

#define KeccakP400_implementation      "16-bit reference implementation"
#define KeccakP400_stateSizeInBytes    50
#define KeccakP400_stateAlignment      2

#ifdef KeccakReference
void KeccakP400_StaticInitialize( void );
#else
#define KeccakP400_StaticInitialize()
#endif
void KeccakP400_Initialize(void *state);
void KeccakP400_AddByte(void *state, unsigned char data, unsigned int offset);
void KeccakP400_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP400_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP400_OverwriteWithZeroes(void *state, unsigned int byteCount);
void KeccakP400_Permute_Nrounds(void *state, unsigned int nrounds);
void KeccakP400_Permute_20rounds(void *state);
void KeccakP400_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP400_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#endif
