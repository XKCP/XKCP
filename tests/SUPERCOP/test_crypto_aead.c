/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_crypto_aead.h"

int crypto_aead_encrypt(
    unsigned char *c,unsigned long long *clen,
    const unsigned char *m,unsigned long long mlen,
    const unsigned char *ad,unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k
    );
int crypto_aead_decrypt(
    unsigned char *m,unsigned long long *mlen,
    unsigned char *nsec,
    const unsigned char *c,unsigned long long clen,
    const unsigned char *ad,unsigned long long adlen,
    const unsigned char *npub,
    const unsigned char *k
    );

int do_test_crypto_aead(
    const unsigned char *key,           unsigned long long keyLen,
    const unsigned char *nonce,         unsigned long long nonceLen,
    const unsigned char *AD,            unsigned long long ADlen,
    const unsigned char *plaintext,     unsigned long long plaintextLen,
    const unsigned char *ciphertext,
    unsigned int tagLen,
    unsigned char *temp1,
    unsigned char *temp2
    )
{
    unsigned long long clen, mlen, i;

    if (crypto_aead_encrypt(temp1, &clen, plaintext, plaintextLen, AD, ADlen, 0, nonce, key) != 0) {
        printf("!!! crypto_aead_encrypt() did not return 0.\n");
        return 1;
    }
    if (clen != plaintextLen+tagLen) {
        printf("!!! clen does not have the expected value.\n");
        return 1;
    }
    if (memcmp(temp1, ciphertext, (size_t)clen) != 0) {
        printf("!!! The output of crypto_aead_encrypt() is not as expected.\n");
        return 1;
    }

    if (crypto_aead_decrypt(temp1, &mlen, 0, ciphertext, plaintextLen+tagLen, AD, ADlen, nonce, key) != 0) {
        printf("!!! crypto_aead_decrypt() did not return 0.\n");
        return 1;
    }
    if (mlen != plaintextLen) {
        printf("!!! mlen does not have the expected value.\n");
        return 1;
    }
    if (memcmp(temp1, plaintext, (size_t)mlen) != 0) {
        printf("!!! The output of crypto_aead_decrypt() is not as expected.\n");
        return 1;
    }

    memcpy(temp2, ciphertext, (size_t)(plaintextLen+tagLen));
    temp2[0] ^= 0x01;
    if (crypto_aead_decrypt(temp1, &mlen, 0, temp2, plaintextLen+tagLen, AD, ADlen, nonce, key) == 0) {
        printf("!!! Forgery found :-)\n");
        return 1;
    }
    for(i=0; i<plaintextLen; i++) if (temp1[i] != 0) {
        printf("!!! The output buffer is not cleared.\n");
        return 1;
    }

    printf("Self-test OK\n");
    return 0;
}

int test_crypto_aead(
    const unsigned char *key,           unsigned long long keyLen,
    const unsigned char *nonce,         unsigned long long nonceLen,
    const unsigned char *AD,            unsigned long long ADlen,
    const unsigned char *plaintext,     unsigned long long plaintextLen,
    const unsigned char *ciphertext,
    unsigned int tagLen)
{
    unsigned char *temp1 = malloc((size_t)plaintextLen + tagLen);
    unsigned char *temp2 = malloc((size_t)plaintextLen + tagLen);
    int retcode = do_test_crypto_aead(key, keyLen, nonce, nonceLen, AD, ADlen, plaintext, plaintextLen, ciphertext, tagLen, temp1, temp2);
    free(temp1);
    free(temp2);
    return retcode;
}
