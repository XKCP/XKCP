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
        "\x49\xCA\x1E\xB8\xD7\x1D\x1F\xDC\x7A\x72\xDA\xA3\x20\xC8\xF9\xCA\x54\x36\x71\xC2\xCB\x8F\xE9\xB2\x63\x8A\x84\x16\xDF\x50\xA7\x90\xA5\x0D\x0B\xB6\xB8\x87\x41\xD7\x81\x6D\x60\x61\xF4\x6A\xEA\x89";
    return test_crypto_hash(message, 7, hash, 48);
}
