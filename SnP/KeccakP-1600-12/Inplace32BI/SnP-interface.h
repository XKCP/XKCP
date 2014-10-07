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

#include "KeccakP-1600-12-interface.h"
#include "SnP-FBWL-default.h"

#define SnP_width                           KeccakF_width
#define SnP_stateSizeInBytes                KeccakF_stateSizeInBytes
#define SnP_laneLengthInBytes               KeccakF_laneInBytes
#define SnP_laneCount                       25

#define SnP_StaticInitialize                KeccakF1600_Initialize
#define SnP_Initialize                      KeccakF1600_StateInitialize
#define SnP_XORBytesInLane                  KeccakF1600_StateXORBytesInLane
#define SnP_XORLanes                        KeccakF1600_StateXORLanes
#define SnP_OverwriteBytesInLane            KeccakF1600_StateOverwriteBytesInLane
#define SnP_OverwriteLanes                  KeccakF1600_StateOverwriteLanes
#define SnP_OverwriteWithZeroes             KeccakF1600_StateOverwriteWithZeroes
#define SnP_ComplementBit                   KeccakF1600_StateComplementBit
#define SnP_Permute                         KeccakP1600_12_StatePermute
#define SnP_ExtractBytesInLane              KeccakF1600_StateExtractBytesInLane
#define SnP_ExtractLanes                    KeccakF1600_StateExtractLanes
#define SnP_ExtractAndXORBytesInLane        KeccakF1600_StateExtractAndXORBytesInLane
#define SnP_ExtractAndXORLanes              KeccakF1600_StateExtractAndXORLanes

#include "SnP-Relaned.h"

#define SnP_FBWL_Absorb                     SnP_FBWL_Absorb_Default
#define SnP_FBWL_Squeeze                    SnP_FBWL_Squeeze_Default
#define SnP_FBWL_Wrap                       SnP_FBWL_Wrap_Default
#define SnP_FBWL_Unwrap                     SnP_FBWL_Unwrap_Default

#endif
