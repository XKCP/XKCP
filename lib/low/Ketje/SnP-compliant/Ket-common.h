/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KetCommon_h_
#define _KetCommon_h_

#include <string.h>

#define FRAMEBITSEMPTY  0x01
#define FRAMEBITS0      0x02
#define FRAMEBITS00     0x04
#define FRAMEBITS10     0x05
#define FRAMEBITS01     0x06
#define FRAMEBITS11     0x07

/*  Ketje rounds */
#define Ket_StartRounds     12
#define Ket_StepRounds      1
#define Ket_StrideRounds    6

#define Ketje_LaneSize  (SnP_width/8/25)
#define Ketje_BlockSize (((SnP_width <= 400)?2:4)*Ketje_LaneSize)

#define KCP_DeclareKetFunctions(prefix) \
void prefix##_StateAddByte( void *state, unsigned char value, unsigned int offset ); \
unsigned char prefix##_StateExtractByte( void *state, unsigned int offset ); \
void prefix##_StateOverwrite( void *state, unsigned int offset, const unsigned char *data, unsigned int length ); \
void prefix##_Step( void *state, unsigned int size, unsigned char frameAndPaddingBits ); \
void prefix##_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks ); \
void prefix##_UnwrapBlocks( void *state, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks ); \
void prefix##_WrapBlocks( void *state, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks ); \

#endif
