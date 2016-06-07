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

#include <string.h>
#include "Ketje.h"

int crypto_aead_encrypt(
    unsigned char *c,unsigned long long *clen,
    const unsigned char *m,unsigned long long mlen,
    const unsigned char *ad,unsigned long long adlen,
    const unsigned char *nsec,
    const unsigned char *npub,
    const unsigned char *k
    )
{
    KetjeSr_Instance instance;

    KetjeSr_Initialize(&instance, k, 128, npub, 128);
    KetjeSr_FeedAssociatedData(&instance, ad, adlen);
    KetjeSr_WrapPlaintext(&instance, m, c, mlen);
    *clen = mlen;
    KetjeSr_GetTag(&instance, c+mlen, 16);
    *clen += 16;

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
    KetjeSr_Instance instance;
    unsigned char tag[16];

    if (clen < 16)
        return -1;

    KetjeSr_Initialize(&instance, k, 128, npub, 128);
    KetjeSr_FeedAssociatedData(&instance, ad, adlen);
    *mlen = clen-16;
    KetjeSr_UnwrapCiphertext(&instance, c, m, *mlen);
    KetjeSr_GetTag(&instance, tag, 16);
    if (memcmp(tag, c+(*mlen), 16) != 0) {
        memset(m, 0, *mlen);
        return -1;
    }
    else
        return 0;
}
