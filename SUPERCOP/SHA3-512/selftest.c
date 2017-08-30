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
        "\x50\x08\x1C\x93\xBF\x73\xEC\xC5\x4A\x5F\xFE\x43\xFC\x14\xF8\xBA\xEE\xDB\xE7\xDA\x03\x02\xAC\x98\x4C\x9E\x66\x83\x89\x88\x6B\xD0\x64\xBA\xB2\x6D\xDC\xB6\x16\xEB\x4E\x0E\x72\x60\x42\xB1\x9F\x3F\xD5\x0B\xDD\x0D\x2C\x5B\x34\x89\x2E\x00\xE6\xF3\x99\xDE\x25\x4F";
    return test_crypto_hash(message, 7, hash, 64);
}
