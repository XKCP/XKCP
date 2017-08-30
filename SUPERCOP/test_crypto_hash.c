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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_crypto_hash.h"

int crypto_hash( unsigned char *out, const unsigned char *in, unsigned long long inlen );

int test_crypto_hash(
    const unsigned char *message,       unsigned long long messageLen,
    const unsigned char *hash,          unsigned long long hashLen)
{
    unsigned char temp[200];

	if (hashLen > 200) {
        printf("!!! Test not supported.\n");
        return 1;
	}
	if (crypto_hash(temp, message, messageLen) != 0) {
        printf("!!! crypto_hash() did not return 0.\n");
        return 1;
    }
    if (memcmp(temp, hash, (size_t)hashLen) != 0) {
        printf("!!! The output of crypto_hash() is not as expected.\n");
        {
            unsigned int i;
            for(i=0; i<hashLen; i++)
                printf("\\x%02x", (int)temp[i]);
            printf("\n");
        }
        return 1;
    }
    printf("Test passed.\n");
    return 0;
}
