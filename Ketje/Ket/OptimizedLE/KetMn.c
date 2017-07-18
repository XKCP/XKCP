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

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include <stdint.h>
#include "KetMn.h"

//#define NO_MISALIGNED_ACCESSES

#define Ket_Minimum( a, b ) (((a) < (b)) ? (a) : (b))

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix                      KetMn
    #define SnP                         KeccakP800
    #define SnP_width                   800
    #define SnP_PermuteRounds           KeccakP800_Permute_Nrounds
	#define tKeccakLane					uint32_t
	#ifdef NO_MISALIGNED_ACCESSES
	#define ReadUnalignedLane(__a)		(*(__a)|(*((__a)+1)<<8)|(*((__a)+2)<<16)|(*((__a)+3)<<24))
	#define WriteUnalignedLane(__a,__v)	*(__a) = (uint8_t)(__v), *((__a)+1) = (uint8_t)((__v)>>8), *((__a)+2) = (uint8_t)((__v)>>16), *((__a)+3) = (uint8_t)((__v)>>24)
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
