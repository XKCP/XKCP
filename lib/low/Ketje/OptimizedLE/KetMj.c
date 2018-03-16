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
#include "KetMj.h"

//#define NO_MISALIGNED_ACCESSES

#define Ket_Minimum( a, b ) (((a) < (b)) ? (a) : (b))

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix                      KetMj
    #define SnP                         KeccakP1600
    #define SnP_width                   1600
    #define SnP_PermuteRounds           KeccakP1600_Permute_Nrounds
	#define tKeccakLane					uint64_t
	#ifdef NO_MISALIGNED_ACCESSES
	#define ReadUnalignedLane(__a)		(*(__a)|((uint64_t)*((__a)+1)<<8)|((uint64_t)*((__a)+2)<<16)|((uint64_t)*((__a)+3)<<24) \
										|((uint64_t)*((__a)+4)<<32)|((uint64_t)*((__a)+5)<<40)|((uint64_t)*((__a)+6)<<48)|((uint64_t)*((__a)+7)<<56))
	#define WriteUnalignedLane(__a,__v)	*(__a) = (uint8_t)(__v), *((__a)+1) = (uint8_t)((__v)>>8), *((__a)+2) = (uint8_t)((__v)>>16), *((__a)+3) = (uint8_t)((__v)>>24), \
										*((__a)+4) = (uint8_t)((__v)>>32), *((__a)+5) = (uint8_t)((__v)>>40), *((__a)+6) = (uint8_t)((__v)>>48), *((__a)+7) = (uint8_t)((__v)>>56)
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
