/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Xoodoo_SnP_h_
#define _Xoodoo_SnP_h_

#include <stddef.h>
#include <stdint.h>

/** For the documentation, see SnP-documentation.h.
 */

#define Xoodoo_implementation      "AVR8 optimized implementation"
#define Xoodoo_stateSizeInBytes    (3*4*4)
#define Xoodoo_stateAlignment      1
#define Xoodoo_HasNround

#define Xoodoo_StaticInitialize()
void Xoodoo_Initialize(void *state);
#define Xoodoo_AddByte(argS, argData, argOffset)    ((uint8_t*)argS)[argOffset] ^= (argData)
void Xoodoo_AddBytes(void *state, const uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteBytes(void *state, const uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteWithZeroes(void *state, unsigned int byteCount);
void Xoodoo_Permute_Nrounds(void *state, unsigned int nrounds);
void Xoodoo_Permute_6rounds(void *state);
void Xoodoo_Permute_12rounds(void *state);
void Xoodoo_ExtractBytes(const void *state, uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_ExtractAndAddBytes(const void *state, const uint8_t *input, uint8_t *output, unsigned int offset, unsigned int length);

#endif
