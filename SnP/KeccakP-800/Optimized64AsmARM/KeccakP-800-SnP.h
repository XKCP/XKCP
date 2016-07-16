#ifndef _KeccakP_800_SnP_h_
#define _KeccakP_800_SnP_h_

/** For the documentation, see SnP-documentation.h.
 */

#define KeccakP800_implementation      "64-bit optimized ARMv8a assembler implementation"
#define KeccakP800_stateSizeInBytes    100
#define KeccakP800_stateAlignment      8

//void KeccakP800_StaticInitialize( void );
#define KeccakP800_StaticInitialize()
void KeccakP800_Initialize(void *state);
void KeccakP800_AddByte(void *state, unsigned char data, unsigned int offset);
void KeccakP800_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount);
void KeccakP800_Permute_12rounds(void *state);
void KeccakP800_Permute_22rounds(void *state);
void KeccakP800_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakP800_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#endif

