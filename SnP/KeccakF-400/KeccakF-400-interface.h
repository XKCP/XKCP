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

#ifndef _KeccakF400Interface_h_
#define _KeccakF400Interface_h_

#define KeccakF_width 400
#define KeccakF_laneInBytes 2
#define KeccakF_stateSizeInBytes (KeccakF_width/8)
#define KeccakF_400

void KeccakF400_Initialize( void );
void KeccakF400_StateInitialize(void *state);
void KeccakF400_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF400_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF400_StateOverwriteWithZeroes(void *state, unsigned int byteCount);
void KeccakF400_StateComplementBit(void *state, unsigned int position);
void KeccakF400_StatePermute(void *state);
void KeccakF400_StateExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF400_StateExtractAndXORBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);

#endif
