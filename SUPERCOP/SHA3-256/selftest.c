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

#include <string.h>
#include "test_crypto_hash.h"

int main()
{
    const unsigned char *message = (const unsigned char *)
        "\x11\x97\x13\xCC\x83\xEE\xEF";
    const unsigned char *hash = (const unsigned char *)
        "\xE3\x40\xC9\xA4\x43\x73\xEF\xCC\x21\x2F\x3C\xB6\x6A\x04\x7A\xC3\x4C\x87\xFF\x1C\x58\xC4\xA1\x4B\x16\xA2\xBF\xC3\x46\x98\xBB\x1D";
    return test_crypto_hash(message, 7, hash, 32);
}
