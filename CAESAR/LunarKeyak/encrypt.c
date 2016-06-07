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

#include "crypto_aead.h"
#include "Keyakv2.h"

int crypto_aead_encrypt(
    unsigned char *c,unsigned long long *clen,
    const unsigned char *m,unsigned long long mlen,
    const unsigned char *ad,unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k
    )
{
    LunarKeyak_Instance instance;

    LunarKeyak_Initialize( &instance, k, 16, npub, 150, 0, 0, 0, 0 );
    LunarKeyak_Wrap( &instance, m, c, (size_t)mlen, ad, (size_t)adlen, c+mlen, 0, 0 );
    *clen = mlen + 16;
    return 0;
}

int crypto_aead_decrypt(
    unsigned char *m,unsigned long long *mlen,
    unsigned char *nsec,
    const unsigned char *c,unsigned long long clen,
    const unsigned char *ad,unsigned long long adlen,
    const unsigned char *npub,
    const unsigned char *k
    )
{
    LunarKeyak_Instance instance;
    unsigned long long mlen_;

    if (clen < 16)
        return -1;
    LunarKeyak_Initialize( &instance, k, 16, npub, 150, 0, 0, 1, 0 );
    mlen_ = clen - 16;
    *mlen = mlen_;
    if ( LunarKeyak_Wrap( &instance, c, m, (size_t)mlen_, ad, (size_t)adlen, (unsigned char*)c+mlen_, 1, 0 ) == 1 )
        return 0;
    return -1;
}
