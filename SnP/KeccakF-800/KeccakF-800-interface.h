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

#ifndef _KeccakF800Interface_h_
#define _KeccakF800Interface_h_

#define KeccakF_width 800
#define KeccakF_laneInBytes 4
#define KeccakF_stateSizeInBytes (KeccakF_width/8)
#define KeccakF_800

void KeccakF800_Initialize( void );
void KeccakF800_StateInitialize(void *state);
void KeccakF800_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF800_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF800_StateOverwriteWithZeroes(void *state, unsigned int byteCount);
void KeccakF800_StateComplementBit(void *state, unsigned int position);
void KeccakF800_StatePermute(void *state);
void KeccakF800_StateExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
void KeccakF800_StateExtractAndXORBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);

#endif
