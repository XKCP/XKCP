/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Xoodyak, designed by Joan Daemen, Seth Hoffert, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

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
        "\x99\x9d\x58\x65\xb0\xdd\x9f\xa3\x09\x73\x36\x5f\xec\xf0\x41\x77\x8d\x04\x49\xa1\xb0\xc5\x5b\x74\x36\x60\x83\x1a\x7d\x50\x25\xee";
    return test_crypto_hash(message, 7, hash, 32);
}
