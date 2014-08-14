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

#ifndef _KeccakP_1600_12_Interface_h_
#define _KeccakP_1600_12_Interface_h_

#include "KeccakF-1600-interface.h"

#undef KeccakF_1600
#define KeccakP_1600_12

/** Function to apply Keccak-p[1600, 12] on the state.
  * @param  state   Pointer to the state.
  */
void KeccakP1600_12_StatePermute(void *state);
size_t KeccakP1600_12_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakP1600_12_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen);
size_t KeccakP1600_12_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);
size_t KeccakP1600_12_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
