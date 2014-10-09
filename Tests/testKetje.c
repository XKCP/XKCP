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
#include "Ketje.h"

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

    for( i=0; i<length; i++) {
        unsigned int iRolled = i*seed1;
        unsigned char byte = (iRolled+length+seed2)%0xFF;
        data[i] = byte;
    }
}

void errorIfNotZero( unsigned int result )
{
	if ( result != 0 )
	{
		printf( "\n\nError\n" );
		exit( result );
	}
}

unsigned int myMin(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

void testKetje()
{
    int keySizeInBits;
    int keyMaxSizeInBits = KeccakF_width - 18;
#if (KeccakF_width == 200)
    #ifdef OUTPUT
        FILE *f = fopen("KetjeJr.txt", "w");
    #endif
    const unsigned char *expected = (unsigned char *)"\x3b\x7d\xea\x9d\xf3\xe0\x58\x06\x98\x92\xc3\xc0\x05\x0f\x4b\xfd";
#endif
#if (KeccakF_width == 400)
    #ifdef OUTPUT
        FILE *f = fopen("KetjeSr.txt", "w");
    #endif
    const unsigned char *expected = (unsigned char *)"\x4a\x31\xc7\x51\x18\x7f\x03\x2c\x78\xc3\xcf\x36\x51\x0b\xe3\xb3";
#endif
    Ketje_Instance checksum;
    unsigned char overallChecksum[16];

#ifdef OUTPUT
    assert(f != NULL);
#endif

	Ketje_Initialize(&checksum, 0, 0, 0, 0);

    for( keySizeInBits=keyMaxSizeInBits; keySizeInBits >=96; keySizeInBits -= (keySizeInBits > 200) ? 100 : ((keySizeInBits > 128) ? 27 : 16))
	{
        int nonceMaxSizeInBits = keyMaxSizeInBits - keySizeInBits;
        int nonceSizeInBits;
        for(nonceSizeInBits = nonceMaxSizeInBits; nonceSizeInBits >= ((keySizeInBits < 112) ? 0 : nonceMaxSizeInBits); nonceSizeInBits -= (nonceSizeInBits > 128) ? 161 : 65)
		{
            Ketje_Instance ketje1;
            Ketje_Instance ketje2;
            unsigned char key[50];
            unsigned char nonce[50];
			unsigned int ADlen;

#ifdef OUTPUT
            printf( "Ketje%s, key length is %u bits, nonce length is %u bits\n",
                #if (KeccakF_width == 200)
                    "Jr",
                #endif
                #if (KeccakF_width == 400)
                    "Sr",
                #endif
                    keySizeInBits, nonceSizeInBits );
#endif
            generateSimpleRawMaterial(key, 50, 0x12+nonceSizeInBits, KeccakF_width);
            generateSimpleRawMaterial(nonce, 50, 0x23+keySizeInBits, KeccakF_width);

            errorIfNotZero( Ketje_Initialize( &ketje1, key, keySizeInBits, nonce, nonceSizeInBits) );
            errorIfNotZero( Ketje_Initialize( &ketje2, key, keySizeInBits, nonce, nonceSizeInBits) );

			if ( (keySizeInBits % 8) != 0)
			{
				key[keySizeInBits / 8] &= (1 << (keySizeInBits % 8)) - 1;
			}
			if ( (nonceSizeInBits % 8) != 0)
			{
				nonce[nonceSizeInBits / 8] &= (1 << (nonceSizeInBits % 8)) - 1;
			}

#ifdef OUTPUT
			fprintf(f, "***\n");
			fprintf(f, "initialize with key of %u bits, nonce of %u bits:\n", keySizeInBits, nonceSizeInBits );
			displayByteString(f, "key", key, (keySizeInBits+7)/8);
			displayByteString(f, "nonce", nonce, (nonceSizeInBits+7)/8);
			fprintf(f, "\n");
#endif

            for( ADlen=0; ADlen<=400; ADlen=ADlen+ADlen/3+1+(keySizeInBits-96)+nonceSizeInBits/32)
			{
				unsigned int Mlen;
                for( Mlen=0; Mlen<=400; Mlen=Mlen+Mlen/2+1+ADlen+((ADlen == 0) ? (keySizeInBits-96) : (nonceSizeInBits/4+keySizeInBits/2)))
				{
                    unsigned char associatedData[400];
                    unsigned char plaintext[400];
                    unsigned char ciphertext[400];
                    unsigned char plaintextPrime[400];
                    unsigned char tag1[16], tag2[16];
					//printf("ADlen %u, Mlen %u\n", ADlen, Mlen);
                    generateSimpleRawMaterial(associatedData, ADlen, 0x34+Mlen, 3);
                    generateSimpleRawMaterial(plaintext, Mlen, 0x45+ADlen, 4);

                    {
                        unsigned int split = myMin(ADlen/4, (unsigned int)200);
						unsigned int i;

//                      errorIfNotZero( Ketje_FeedAssociatedData( &ketje1, associatedData,0 ) );
                        for(i=0; i<split; i++)
                            errorIfNotZero( Ketje_FeedAssociatedData( &ketje1, associatedData+i, 1) );
                        if (split < ADlen)
                            errorIfNotZero( Ketje_FeedAssociatedData( &ketje1, associatedData+split, ADlen-split) );
                    }
                    errorIfNotZero( Ketje_FeedAssociatedData( &ketje2, associatedData, ADlen) );

                    {
                        unsigned int split = Mlen/3;
                        memcpy(ciphertext, plaintext, split);
                        errorIfNotZero( Ketje_WrapPlaintext( &ketje1, ciphertext, ciphertext, split) ); // in place
                        errorIfNotZero( Ketje_WrapPlaintext( &ketje1, plaintext+split, ciphertext+split, Mlen-split) );
                    }

                    {
                        unsigned int split = Mlen/3*2;
                        memcpy(plaintextPrime, ciphertext, split);
                        errorIfNotZero( Ketje_UnwrapCiphertext(&ketje2, plaintextPrime, plaintextPrime, split) ); // in place
                        errorIfNotZero( Ketje_UnwrapCiphertext(&ketje2, ciphertext+split, plaintextPrime+split, Mlen-split) );
                    }
                    errorIfNotZero( memcmp(plaintext, plaintextPrime, Mlen) );

                    errorIfNotZero( Ketje_GetTag( &ketje1, tag1, 16) );
                    errorIfNotZero( Ketje_GetTag( &ketje2, tag2, 16) );
                    errorIfNotZero( memcmp(tag1, tag2, 16) );

                    Ketje_FeedAssociatedData(&checksum, ciphertext, Mlen);
                    Ketje_FeedAssociatedData(&checksum, tag1, 16);

#ifdef OUTPUT
					displayByteString(f, "associated data", associatedData, ADlen);
					displayByteString(f, "plaintext", plaintext, Mlen);
					displayByteString(f, "ciphertext", ciphertext, Mlen);
					displayByteString(f, "tag", tag1, 16);
					fprintf(f, "\n");
#endif
                }
            }
        }
    }
    Ketje_WrapPlaintext(&checksum, 0, 0, 0);
    Ketje_GetTag(&checksum, overallChecksum, 16);
#ifdef OUTPUT
    displayByteString(f, "overall checksum", overallChecksum, 16);
    fclose(f);
#endif
    assert(memcmp(overallChecksum, expected, 16) == 0);
}
