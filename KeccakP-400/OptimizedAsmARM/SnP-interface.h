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

#include "KeccakP-400-interface.h"

typedef	unsigned short tKeccakLane;

#define SnP_width                           KeccakF_width
#define SnP_stateSizeInBytes                KeccakF_stateSizeInBytes
#define SnP_laneLengthInBytes               KeccakF_laneInBytes
#define SnP_laneCount                       25
#define SnP_maxRounds                       20

#define SnP_StaticInitialize()
#define SnP_Initialize                      KeccakF400_StateInitialize
#define SnP_XORBytesInLane                  KeccakF400_StateXORBytesInLane
#define SnP_XORLanes                        KeccakF400_StateXORLanes
#define SnP_OverwriteBytesInLane            KeccakF400_StateOverwriteBytesInLane
#define SnP_OverwriteLanes                  KeccakF400_StateOverwriteLanes
#define SnP_OverwriteWithZeroes             KeccakF400_StateOverwriteWithZeroes
#define SnP_ComplementBit( argState, argPosition )	\
    ((tKeccakLane*)(argState))[(argPosition)/(sizeof(tKeccakLane)*8)] ^= ((tKeccakLane)1 << ((argPosition)%(sizeof(tKeccakLane)*8)));
#define SnP_ExtractBytesInLane              KeccakF400_StateExtractBytesInLane
#define SnP_ExtractLanes                    KeccakF400_StateExtractLanes
#define SnP_ExtractAndXORBytesInLane        KeccakF400_StateExtractAndXORBytesInLane
#define SnP_ExtractAndXORLanes              KeccakF400_StateExtractAndXORLanes
#define SnP_PermuteRounds                   KeccakP400_StatePermute
#define SnP_Permute(argState)               SnP_PermuteRounds(argState,SnP_maxRounds)

#include "SnP-Relaned.h"

#endif
