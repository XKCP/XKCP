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
#include "KetJr.h"

//#define NO_MISALIGNED_ACCESSES

#define Ket_Minimum( a, b ) (((a) < (b)) ? (a) : (b))

#ifndef KeccakP200_excluded
    #include "KeccakP-200-SnP.h"

    #define prefix                      KetJr
    #define SnP                         KeccakP200
    #define SnP_width                   200
    #define SnP_PermuteRounds           KeccakP200_Permute_Nrounds
	#define tKeccakLane					uint8_t
	#define ReadUnalignedLane(__a)		*(tKeccakLane*)(__a)
	#define WriteUnalignedLane(__a,__v)	*(tKeccakLane*)(__a) = (__v)
        #include "Ket.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_PermuteRounds
	#undef tKeccakLane
	#undef ReadUnalignedLane
	#undef WriteUnalignedLane
#endif
