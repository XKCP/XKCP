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

#include "test_crypto_aead.h"

int main()
{
    const unsigned char *key = (const unsigned char *)
        "\x14\x5e\xa8\xf2\x3d\x87\xd1\x1c\x66\xb0\xfa\x45\x8f\xd9\x24\x6e";
    const unsigned char *nonce = (const unsigned char *)
        "\x15\x70\xcb\x27\x82\xdd\x39\x94\xef\x4b\xa6\x02\x5d\xb8\x14\x6f";
    const unsigned char *AD = (const unsigned char *)
        "\x0c\x78\xe4\x51\xbd\x2a";
    const unsigned char *plaintext = (const unsigned char *)
        "\x0e\x8b\x09\x86\x04\x81\xfe";
    const unsigned char *ciphertext = (const unsigned char *)
        "\xdc\xb4\xf2\x62\x3b\xf1\x57\x5e\x6a\x94\xde\xbf\x7a\x8d\x90\xe3\xda\x3e\x07\x03\x18\x7b\x63";
    return test_crypto_aead(key, 16, nonce, 16, AD, 6, plaintext, 7, ciphertext, 16);
}
