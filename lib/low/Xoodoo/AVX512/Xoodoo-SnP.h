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
    ALIGN(16) uint32_t A[12];
} Xoodoo_align128plain32_state;

typedef Xoodoo_align128plain32_state Xoodoo_state;

#define Xoodoo_implementation      "SIMD-512 optimized implementation"
#define Xoodoo_stateAlignment      16
#define Xoodoo_HasNround

/* void Xoodoo_StaticInitialize( void ); */
#define Xoodoo_StaticInitialize()
void Xoodoo_Initialize(Xoodoo_align128plain32_state *state);
/* void Xoodoo_AddByte(Xoodoo_align128plain32_state *state, uint8_t data, unsigned int offset); */
#define Xoodoo_AddByte(argS, argData, argOffset)    ((uint8_t*)argS)[argOffset] ^= (argData)
void Xoodoo_AddBytes(Xoodoo_align128plain32_state *state, const uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteBytes(Xoodoo_align128plain32_state *state, const uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_OverwriteWithZeroes(Xoodoo_align128plain32_state *state, unsigned int byteCount);
void Xoodoo_Permute_Nrounds(Xoodoo_align128plain32_state *state, unsigned int nrounds);
void Xoodoo_Permute_6rounds(Xoodoo_align128plain32_state *state);
void Xoodoo_Permute_12rounds(Xoodoo_align128plain32_state *state);
void Xoodoo_ExtractBytes(const Xoodoo_align128plain32_state *state, uint8_t *data, unsigned int offset, unsigned int length);
void Xoodoo_ExtractAndAddBytes(const Xoodoo_align128plain32_state *state, const uint8_t *input, uint8_t *output, unsigned int offset, unsigned int length);

#define Xoodoo_FastXoofff_supported
void Xoofff_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen);
size_t Xoofff_CompressFastLoop(unsigned char *kRoll, unsigned char *xAccu, const unsigned char *input, size_t length);
size_t Xoofff_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length);

#define CyclistFullBlocks_supported
size_t Xoodyak_AbsorbKeyedFullBlocks(Xoodoo_align128plain32_state *state, const uint8_t *X, size_t XLen);
size_t Xoodyak_AbsorbHashFullBlocks(Xoodoo_align128plain32_state *state, const uint8_t *X, size_t XLen);
size_t Xoodyak_SqueezeHashFullBlocks(Xoodoo_align128plain32_state *state, uint8_t *Y, size_t YLen);
size_t Xoodyak_SqueezeKeyedFullBlocks(Xoodoo_align128plain32_state *state, uint8_t *Y, size_t YLen);
size_t Xoodyak_EncryptFullBlocks(Xoodoo_align128plain32_state *state, const uint8_t *I, uint8_t *O, size_t IOLen);
size_t Xoodyak_DecryptFullBlocks(Xoodoo_align128plain32_state *state, const uint8_t *I, uint8_t *O, size_t IOLen);

#endif
