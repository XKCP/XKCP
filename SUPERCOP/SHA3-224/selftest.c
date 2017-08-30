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
        "\x8B\x44\x98\x49\xCB\x7C\x47\x76\xC5\x93\xDE\x58\xFD\x5C\x2E\x32\x2C\xB5\x31\x6B\xE0\x8A\x75\x05\x7A\x01\xED\x6A";
    return test_crypto_hash(message, 7, hash, 28);
}


