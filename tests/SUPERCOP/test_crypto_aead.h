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

int test_crypto_aead(
    const unsigned char *key,           unsigned long long keyLen,
    const unsigned char *nonce,         unsigned long long nonceLen,
    const unsigned char *AD,            unsigned long long ADlen,
    const unsigned char *plaintext,     unsigned long long plaintextLen,
    const unsigned char *ciphertext,
    unsigned int tagLen);
