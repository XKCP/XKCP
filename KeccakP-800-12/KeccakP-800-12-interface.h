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

#ifndef _KeccakP_800_12_Interface_h_
#define _KeccakP_800_12_Interface_h_

#include "KeccakF-800-interface.h"

#undef KeccakF_800
#define KeccakP_800_12

/** Function to apply Keccak-p[800, 12] on the state.
  * @param  state   Pointer to the state.
  */
void KeccakP800_12_StatePermute(void *state);

#endif
