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

#include "crypto_hash.h"
#ifndef crypto_hash_BYTES
    #define crypto_hash_BYTES 168
#endif
#include "KangarooTwelve.h"

int crypto_hash( unsigned char *out, const unsigned char *in, unsigned long long inlen )
{
    return KangarooTwelve(in, inlen, out, crypto_hash_BYTES, 0, 0 );
}
