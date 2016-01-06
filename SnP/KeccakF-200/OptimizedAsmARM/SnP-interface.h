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

#include "KeccakF-200-interface.h"
#include "SnP-FBWL-default.h"

typedef	unsigned char tKeccakLane;

#define SnP_width                           KeccakF_width
#define SnP_stateSizeInBytes                KeccakF_stateSizeInBytes
#define SnP_laneLengthInBytes               KeccakF_laneInBytes
#define SnP_laneCount                       25

#define SnP_StaticInitialize()
#define SnP_Initialize                      KeccakF200_StateInitialize
#define SnP_XORBytesInLane                  KeccakF200_StateXORBytesInLane
#define SnP_XORLanes                        KeccakF200_StateXORLanes
#define SnP_OverwriteBytesInLane            KeccakF200_StateOverwriteBytesInLane
#define SnP_OverwriteLanes                  KeccakF200_StateOverwriteLanes
#define SnP_OverwriteWithZeroes             KeccakF200_StateOverwriteWithZeroes
#define SnP_ComplementBit( argState, argPosition )	\
    ((tKeccakLane*)(argState))[(argPosition)/(sizeof(tKeccakLane)*8)] ^= ((tKeccakLane)1 << ((argPosition)%(sizeof(tKeccakLane)*8)));
#define SnP_Permute                         KeccakF200_StatePermute
#define SnP_ExtractBytesInLane              KeccakF200_StateExtractBytesInLane
#define SnP_ExtractLanes                    KeccakF200_StateExtractLanes
#define SnP_ExtractAndXORBytesInLane        KeccakF200_StateExtractAndXORBytesInLane
#define SnP_ExtractAndXORLanes              KeccakF200_StateExtractAndXORLanes

#include "SnP-Relaned.h"

#define SnP_FBWL_Absorb                     SnP_FBWL_Absorb_Default
#define SnP_FBWL_Squeeze                    SnP_FBWL_Squeeze_Default
#define SnP_FBWL_Wrap                       SnP_FBWL_Wrap_Default
#define SnP_FBWL_Unwrap                     SnP_FBWL_Unwrap_Default

#endif
