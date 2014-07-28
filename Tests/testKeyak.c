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

#define OUTPUT

#include <assert.h>
#ifdef OUTPUT
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "Keyak.h"

#ifdef OUTPUT
void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif

void generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        unsigned char byte = seed1 + 161*length - iRolled + i;
        data[i] = byte;
    }
}

unsigned int myMin(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

void testKeyak()
{
    unsigned int keyLen;
#if ((!defined(PlSnP_P)) || (PlSnP_P == 1))
    #if (KeccakF_width == 1600)
        #ifdef OUTPUT
            FILE *f = fopen("LakeKeyak.txt", "w");
        #endif
        #define Keyak_Initialize LakeKeyak_Initialize
        const unsigned char *expected = (unsigned char *)"\xc6\xce\x70\x1c\x23\x5d\x3f\x01\x25\xc3\xb0\x4b\x59\x1f\x62\xe0";
    #elif (KeccakF_width == 800)
        #ifdef OUTPUT
            FILE *f = fopen("RiverKeyak.txt", "w");
        #endif
        #define Keyak_Initialize RiverKeyak_Initialize
        const unsigned char *expected = (unsigned char *)"\x5c\x51\xef\x23\xd4\xc1\x18\x4e\x8b\xc3\x6d\x7d\x5b\x29\x4c\xd1";
    #else
        #error "Which Keyak is this?"
    #endif
    const unsigned int P = 1;
#else
    #if (PlSnP_P == 2)
        #ifdef OUTPUT
            FILE *f = fopen("SeaKeyak.txt", "w");
        #endif
        #define Keyak_Initialize SeaKeyak_Initialize
        const unsigned char *expected = (unsigned char *)"\x56\x3e\xb4\x9c\xf8\x29\xcd\x5b\x05\x78\xf5\xdd\xfb\x78\x8e\x8e";
    #elif (PlSnP_P == 4)
        #ifdef OUTPUT
            FILE *f = fopen("OceanKeyak.txt", "w");
        #endif
        #define Keyak_Initialize OceanKeyak_Initialize
        const unsigned char *expected = (unsigned char *)"\x11\x1b\x89\xa3\xee\x50\x10\x0b\xcb\x8f\xaa\x2e\x3a\x83\xa8\xc5";
    #else
        #error "Which Keyak is this?"
    #endif
    const unsigned int P = PlSnP_P;
#endif
#ifdef OUTPUT
    assert(f != NULL);
#endif

    Keyak_Instance checksum;
    unsigned char zerononce[16];
    unsigned char overallChecksum[16];
    memset(zerononce, 0, sizeof(zerononce));
    Keyak_Initialize(&checksum, 0, 0, zerononce);

    for(keyLen=16; keyLen<=18; keyLen++) {
        Keyak_Instance keyak1;
        Keyak_Instance keyak2;
        unsigned char key[18];
        unsigned char nonce[16];
        unsigned int ADlen;
        unsigned int Mlen;
        generateSimpleRawMaterial(key, keyLen, 0x12, 1+P);
        generateSimpleRawMaterial(nonce, 16, 0x23+keyLen, 2+P);

#ifdef OUTPUT
        fprintf(f, "***\n");
        fprintf(f, "initialize with:\n");
        displayByteString(f, "key", key, keyLen);
        displayByteString(f, "nonce", nonce, 16);
        fprintf(f, "\n");
#endif

        Keyak_Initialize(&keyak1, key, keyLen*8, nonce);
        Keyak_Initialize(&keyak2, key, keyLen*8, nonce);

        for(ADlen=0; ADlen<=600; ADlen+=(keyLen > 16) ? 600 : (ADlen/3+1))
        for(Mlen=0; Mlen<=800; Mlen+=(keyLen > 16) ? 800 : (Mlen/2+1+ADlen)) {
            unsigned char associatedData[600];
            unsigned char plaintext[800];
            unsigned char ciphertext[800];
            unsigned char plaintextPrime[800];
            unsigned char tag1[16];
            unsigned char tag2[16];
            generateSimpleRawMaterial(associatedData, ADlen, 0x34+Mlen, 3);
            generateSimpleRawMaterial(plaintext, Mlen, 0x45+ADlen, 4);

            {
                unsigned int split = (keyLen == 16) ? myMin(ADlen/4, (unsigned int)200) : 0;
                unsigned int i;
                for(i=0; i<split; i++)
                    Keyak_FeedAssociatedData(&keyak1, associatedData+i, 1);
                if (split < ADlen)
                    Keyak_FeedAssociatedData(&keyak1, associatedData+split, ADlen-split);
            }
            Keyak_FeedAssociatedData(&keyak2, associatedData, ADlen);

            if (Mlen > 0) {
                unsigned int split = (keyLen == 16) ? Mlen/3+ADlen%100 : 0;
                if (split > Mlen) split = Mlen;
                memcpy(ciphertext, plaintext, split);
                Keyak_WrapPlaintext(&keyak1, ciphertext, ciphertext, split); // in place
                Keyak_WrapPlaintext(&keyak1, plaintext+split, ciphertext+split, Mlen-split);
            }

            {
                unsigned int split = (keyLen == 16) ? Mlen/3*2+ADlen%55 : 0;
                if (split > Mlen) split = Mlen;
                memcpy(plaintextPrime, ciphertext, split);
                Keyak_UnwrapCiphertext(&keyak2, plaintextPrime, plaintextPrime, split); // in place
                Keyak_UnwrapCiphertext(&keyak2, ciphertext+split, plaintextPrime+split, Mlen-split);
            }
            assert(memcmp(plaintext, plaintextPrime, Mlen) == 0); // The plaintexts do not match.

            Keyak_GetTag(&keyak1, tag1, 16);
            Keyak_GetTag(&keyak2, tag2, 16);
            assert(memcmp(tag1, tag2, 16) == 0); // The tags do not match.

            Keyak_FeedAssociatedData(&checksum, ciphertext, Mlen);
            Keyak_FeedAssociatedData(&checksum, tag1, 16);

#ifdef OUTPUT
            displayByteString(f, "associated data", associatedData, ADlen);
            displayByteString(f, "plaintext", plaintext, Mlen);
            displayByteString(f, "ciphertext", ciphertext, Mlen);
            displayByteString(f, "tag", tag1, 16);
            fprintf(f, "\n");
#endif

            if (((ADlen+Mlen+keyLen) %4) == 1) {
                Keyak_Forget(&keyak1);
                Keyak_Forget(&keyak2);
#ifdef OUTPUT
                fprintf(f, "forget\n\n");
#endif
            }
        }
    }

    {
        Keyak_Instance keyak;
        unsigned char buffer[2000];
        generateSimpleRawMaterial(buffer, sizeof(buffer), 0x56, 5);

#ifdef OUTPUT
        fprintf(f, "***\n");
        fprintf(f, "initialize with:\n");
        displayByteString(f, "key", buffer, 16);
        displayByteString(f, "nonce", buffer, 16);
        fprintf(f, "\n");
#endif

        Keyak_Initialize(&keyak, buffer, 128, buffer);
#ifdef OUTPUT
        displayByteString(f, "associated data", buffer, sizeof(buffer));
#endif
        Keyak_FeedAssociatedData(&keyak, buffer, sizeof(buffer));
#ifdef OUTPUT
        displayByteString(f, "plaintext", buffer, sizeof(buffer));
#endif
        Keyak_WrapPlaintext(&keyak, buffer, buffer, sizeof(buffer));
#ifdef OUTPUT
        displayByteString(f, "ciphertext", buffer, sizeof(buffer));
#endif
        Keyak_FeedAssociatedData(&checksum, buffer, sizeof(buffer));
        Keyak_GetTag(&keyak, buffer, sizeof(buffer));
#ifdef OUTPUT
        displayByteString(f, "tag", buffer, sizeof(buffer));
#endif
        Keyak_FeedAssociatedData(&checksum, buffer, sizeof(buffer));
#ifdef OUTPUT
        fprintf(f, "\n");
#endif
    }
    {
        Keyak_Instance keyak;
        unsigned char buffer[2000];
        generateSimpleRawMaterial(buffer, sizeof(buffer), 0x67, 6);

#ifdef OUTPUT
        fprintf(f, "***\n");
        fprintf(f, "initialize with:\n");
        displayByteString(f, "key", buffer, 16);
        displayByteString(f, "nonce", buffer, 16);
        fprintf(f, "\n");
#endif

        Keyak_Initialize(&keyak, buffer, 128, buffer);
#ifdef OUTPUT
        displayByteString(f, "associated data", buffer, sizeof(buffer));
#endif
        Keyak_FeedAssociatedData(&keyak, buffer, sizeof(buffer));
#ifdef OUTPUT
        displayByteString(f, "ciphertext", buffer, sizeof(buffer));
#endif
        Keyak_UnwrapCiphertext(&keyak, buffer, buffer, sizeof(buffer));
#ifdef OUTPUT
        displayByteString(f, "plaintext", buffer, sizeof(buffer));
#endif
        Keyak_FeedAssociatedData(&checksum, buffer, sizeof(buffer));
        Keyak_GetTag(&keyak, buffer, sizeof(buffer));
#ifdef OUTPUT
        displayByteString(f, "tag", buffer, sizeof(buffer));
#endif
        Keyak_FeedAssociatedData(&checksum, buffer, sizeof(buffer));
#ifdef OUTPUT
        fprintf(f, "\n");
#endif
    }

    Keyak_GetTag(&checksum, overallChecksum, 16);
#ifdef OUTPUT
    displayByteString(f, "overall checksum", overallChecksum, 16);
    fclose(f);
#endif
    assert(memcmp(overallChecksum, expected, 16) == 0);
}
