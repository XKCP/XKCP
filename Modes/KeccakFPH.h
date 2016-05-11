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

#ifndef _KeccakFPH_h_
#define _KeccakFPH_h_

#include <stddef.h>
#include "align.h"

#ifndef KeccakP1600_excluded

/** Fast parallel hash function FPH128, as defined in NIST's draft Special Publication 800-XXX (0.2),
  * posted on the hash forum on April 8th, 2016.
  * WARNING: Based on a draft, still subject to changes.
  * @param  input           Pointer to the input message.
  * @param  inputByteLen    The length of the input message in bytes.
  * @param  blockByteLen    Block size (B) in bytes, must be a power of 2.
  *                         The minimum value is 1024 in this implementation.
  * @param  output          Pointer to the output buffer.
  * @param  outputByteLen   The desired number of output bytes.
  * @param  customization   Pointer to the customization string (S).
  * @param  customByteLen   The length of the customization string in bytes.
  * @return 0 if successful, 1 otherwise.
  */
int Keccak_FPH128( const unsigned char * input, size_t inputByteLen, unsigned int blockByteLen, unsigned char * output, size_t outputByteLen, const unsigned char * customization, size_t customByteLen );

/** Fast parallel hash function FPH256, as defined in NIST's draft Special Publication 800-XXX (0.2),
  * posted on the hash forum on April 8th, 2016.
  * WARNING: Based on a draft, still subject to changes.
  * @param  input           Pointer to the input message.
  * @param  inputByteLen    The length of the input message in bytes.
  * @param  blockByteLen    Block size (B) in bytes, must be a power of 2.
  *                         The minimum value is 1024 in this implementation.
  * @param  output          Pointer to the output buffer.
  * @param  outputByteLen   The desired number of output bytes.
  * @param  customization   Pointer to the customization string (S).
  * @param  customByteLen   The length of the customization string in bytes.
  * @return 0 if successful, 1 otherwise.
  */
int Keccak_FPH256( const unsigned char * input, size_t inputByteLen, unsigned int blockByteLen, unsigned char * output, size_t outputByteLen, const unsigned char * customization, size_t customByteLen );

#endif

#endif
