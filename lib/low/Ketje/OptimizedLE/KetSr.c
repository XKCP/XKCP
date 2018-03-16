/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include <stdint.h>
#include "KetSr.h"

//#define NO_MISALIGNED_ACCESSES

#define Ket_Minimum( a, b ) (((a) < (b)) ? (a) : (b))

#ifndef KeccakP400_excluded
    #include "KeccakP-400-SnP.h"

    #define prefix                      KetSr
    #define SnP                         KeccakP400
    #define SnP_width                   400
    #define SnP_PermuteRounds           KeccakP400_Permute_Nrounds
	#define tKeccakLane					uint16_t
	#ifdef NO_MISALIGNED_ACCESSES
	#define ReadUnalignedLane(__a)		(*(__a)|(*((__a)+1)<<8))
	#define WriteUnalignedLane(__a,__v)	*(__a) = (uint8_t)(__v), *((__a)+1) = (uint8_t)((__v)>> 8)
	#else
	#define ReadUnalignedLane(__a)		*(tKeccakLane*)(__a)
	#define WriteUnalignedLane(__a,__v)	*(tKeccakLane*)(__a) = (__v)
	#endif
        #include "Ket.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_PermuteRounds
	#undef tKeccakLane
	#undef ReadUnalignedLane
	#undef WriteUnalignedLane
#endif
