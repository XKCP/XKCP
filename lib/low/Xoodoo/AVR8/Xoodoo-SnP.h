/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Xoodoo permutation, designed by Joan Daemen, Seth Hoffert, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
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

typedef struct {
    uint8_t A[48];
} Xoodoo_plain8_state;

typedef Xoodoo_plain8_state Xoodoo_state;

#define Xoodoo_implementation      "AVR8 optimized implementation"
#define Xoodoo_stateAlignment      1
#define Xoodoo_HasNround

#define Xoodoo_StaticInitialize()
void Xoodoo_Initialize(Xoodoo_plain8_state *state);
#define Xoodoo_AddByte(argS, argData, argOffset)    ((uint8_t*)argS)[argOffset] ^= (argData)
void Xoodoo_AddBytes(Xoodoo_plain8_state *state, const uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteBytes(Xoodoo_plain8_state *state, const uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteWithZeroes(Xoodoo_plain8_state *state, unsigned int byteCount);
void Xoodoo_Permute_Nrounds(Xoodoo_plain8_state *state, unsigned int nrounds);
void Xoodoo_Permute_6rounds(Xoodoo_plain8_state *state);
void Xoodoo_Permute_12rounds(Xoodoo_plain8_state *state);
void Xoodoo_ExtractBytes(const Xoodoo_plain8_state *state, uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_ExtractAndAddBytes(const Xoodoo_plain8_state *state, const uint8_t *input, uint8_t *output, unsigned int offset, unsigned int length);

#endif
