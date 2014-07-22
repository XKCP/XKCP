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

// Important note: State must be 256 bit (32 bytes) aligned.

#include "KeccakF-1600-interface.h"

typedef	unsigned long long tKeccakLane;

#define SnP_width                           KeccakF_width
#define SnP_stateSizeInBytes                KeccakF_stateSizeInBytes
#define SnP_laneLengthInBytes               KeccakF_laneInBytes

#define SnP_StaticInitialize()
#define SnP_Initialize                      KeccakF1600_StateInitialize
#define SnP_ComplementBit( argState, argPosition )	\
    ((tKeccakLane*)(argState))[(argPosition)/(sizeof(tKeccakLane)*8)] ^= ((tKeccakLane)1 << ((argPosition)%(sizeof(tKeccakLane)*8)));
#define SnP_XORBytes                        KeccakF1600_StateXORBytes
#define SnP_OverwriteBytes                  KeccakF1600_StateOverwriteBytes
#define SnP_OverwriteWithZeroes             KeccakF1600_StateOverwriteWithZeroes
#define SnP_Permute                         KeccakF1600_StatePermute
#define SnP_ExtractBytes                    KeccakF1600_StateExtractBytes
#define SnP_ExtractAndXORBytes              KeccakF1600_StateExtractAndXORBytes

#define SnP_FBWL_Absorb                     SnP_FBWL_Absorb_Default
#define SnP_FBWL_Squeeze                    SnP_FBWL_Squeeze_Default
#define SnP_FBWL_Wrap                       SnP_FBWL_Wrap_Default
#define SnP_FBWL_Unwrap                     SnP_FBWL_Unwrap_Default

//#define SnP_FBWL_Absorb                     KeccakF1600_SnP_FBWL_Absorb
//#define SnP_FBWL_Squeeze                    KeccakF1600_SnP_FBWL_Squeeze
//#define SnP_FBWL_Wrap                       KeccakF1600_SnP_FBWL_Wrap
//#define SnP_FBWL_Unwrap                     KeccakF1600_SnP_FBWL_Unwrap

size_t KeccakF1600_SnP_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakF1600_SnP_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen);
size_t KeccakF1600_SnP_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakF1600_SnP_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
