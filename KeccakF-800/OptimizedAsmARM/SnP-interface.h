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

#include "KeccakF-800-interface.h"

typedef	unsigned int tKeccakLane;

#define SnP_width                           KeccakF_width
#define SnP_stateSizeInBytes                KeccakF_stateSizeInBytes
#define SnP_laneLengthInBytes               KeccakF_laneInBytes
#define SnP_laneCount                       25

#define SnP_StaticInitialize()
#define SnP_Initialize                      KeccakF800_StateInitialize
#define SnP_XORBytesInLane                  KeccakF800_StateXORBytesInLane
#define SnP_XORLanes                        KeccakF800_StateXORLanes
#define SnP_OverwriteBytesInLane            KeccakF800_StateOverwriteBytesInLane
#define SnP_OverwriteLanes                  KeccakF800_StateOverwriteLanes
#define SnP_OverwriteWithZeroes             KeccakF800_StateOverwriteWithZeroes
#define SnP_ComplementBit( argState, argPosition )	\
    ((tKeccakLane*)(argState))[(argPosition)/(sizeof(tKeccakLane)*8)] ^= ((tKeccakLane)1 << ((argPosition)%(sizeof(tKeccakLane)*8)));
#define SnP_Permute                         KeccakF800_StatePermute
#define SnP_ExtractBytesInLane              KeccakF800_StateExtractBytesInLane
#define SnP_ExtractLanes                    KeccakF800_StateExtractLanes
#define SnP_ExtractAndXORBytesInLane        KeccakF800_StateExtractAndXORBytesInLane
#define SnP_ExtractAndXORLanes              KeccakF800_StateExtractAndXORLanes

#include "SnP-Relaned.h"

#define SnP_FBWL_Absorb                     KeccakF800_SnP_FBWL_Absorb
#define SnP_FBWL_Squeeze                    KeccakF800_SnP_FBWL_Squeeze
#define SnP_FBWL_Wrap                       KeccakF800_SnP_FBWL_Wrap
#define SnP_FBWL_Unwrap                     KeccakF800_SnP_FBWL_Unwrap

size_t KeccakF800_SnP_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakF800_SnP_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen);
size_t KeccakF800_SnP_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakF800_SnP_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
