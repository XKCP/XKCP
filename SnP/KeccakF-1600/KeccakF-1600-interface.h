/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeccakF1600Interface_h_
#define _KeccakF1600Interface_h_

#include <string.h>

#define KeccakF_width 1600
#define KeccakF_laneInBytes 8
#define KeccakF_stateSizeInBytes (KeccakF_width/8)
#define KeccakF_1600

void KeccakF1600_Initialize( void );
void KeccakF1600_StateInitialize(void *state);
void KeccakF1600_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF1600_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF1600_StateOverwriteWithZeroes(void *state, unsigned int byteCount);
void KeccakF1600_StateComplementBit(void *state, unsigned int position);
void KeccakF1600_StatePermute(void *state);
void KeccakF1600_StateExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF1600_StateExtractAndXORBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
size_t KeccakF1600_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakF1600_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen);
size_t KeccakF1600_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakF1600_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
