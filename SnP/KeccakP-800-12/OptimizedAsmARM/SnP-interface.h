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

#ifndef _SnP_Interface_h_
#define _SnP_Interface_h_

#include "KeccakP-800-12-interface.h"

typedef	unsigned int tKeccakLane;

#define SnP_width                           KeccakF_width
#define SnP_stateSizeInBytes                KeccakF_stateSizeInBytes
#define SnP_laneLengthInBytes               KeccakF_laneInBytes
#define SnP_laneCount                       25

#define SnP_StaticInitialize()
#define SnP_Initialize                      KeccakF800_StateInitialize
#define SnP_XORBytes                        KeccakF800_StateXORBytes
#define SnP_XORByte(argState,argOfs,argVal)    ((unsigned char *)(argState))[argOfs] ^= argVal
#define SnP_OverwriteBytes                  KeccakF800_StateOverwriteBytes
#define SnP_OverwriteWithZeroes             KeccakF800_StateOverwriteWithZeroes
#define SnP_ComplementBit( argState, argPosition )	\
    ((tKeccakLane*)(argState))[(argPosition)/(sizeof(tKeccakLane)*8)] ^= ((tKeccakLane)1 << ((argPosition)%(sizeof(tKeccakLane)*8)));
#define SnP_Permute                         KeccakP800_12_StatePermute
#define SnP_ExtractBytes                    KeccakF800_StateExtractBytes
#define SnP_ExtractAndXORBytes              KeccakF800_StateExtractAndXORBytes

#define SnP_FBWL_Absorb                     KeccakP800_12_SnP_FBWL_Absorb
#define SnP_FBWL_Squeeze                    KeccakP800_12_SnP_FBWL_Squeeze
#define SnP_FBWL_Wrap                       KeccakP800_12_SnP_FBWL_Wrap
#define SnP_FBWL_Unwrap                     KeccakP800_12_SnP_FBWL_Unwrap

size_t KeccakP800_12_SnP_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakP800_12_SnP_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen);
size_t KeccakP800_12_SnP_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakP800_12_SnP_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
