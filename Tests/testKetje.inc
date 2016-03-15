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

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define Ketje_Instance                  JOIN(prefix, _Instance)
#define Ketje_Initialize                JOIN(prefix, _Initialize)
#define Ketje_FeedAssociatedData        JOIN(prefix, _FeedAssociatedData)
#define Ketje_WrapPlaintext             JOIN(prefix, _WrapPlaintext)
#define Ketje_UnwrapCiphertext          JOIN(prefix, _UnwrapCiphertext)
#define Ketje_GetTag                    JOIN(prefix, _GetTag)

#define test                            JOIN(prefix, _test)

void test( const char *file, const unsigned char *expected )
{
    int keySizeInBits;
    int keyMaxSizeInBits = SnP_width - 18;
    #ifdef OUTPUT
        FILE *f = fopen(file, "w");
    #endif
    Ketje_Instance checksum;
    unsigned char overallChecksum[16];

#ifdef OUTPUT
    assert(f != NULL, "Could not open log file");
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
            #define kname(nnn) #nnn
            printf( "%s, key length is %u bits, nonce length is %u bits\n", kname(prefix), keySizeInBits, nonceSizeInBits );
            #undef kname
#endif
            generateSimpleRawMaterial(key, 50, 0x12+nonceSizeInBits, SnP_width);
            generateSimpleRawMaterial(nonce, 50, 0x23+keySizeInBits, SnP_width);

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
                    if (memcmp(plaintext, plaintextPrime, Mlen) != 0) { // !!!
                        printf("keySizeInBits: %d\n", keySizeInBits);
                        printf("nonceSizeInBits: %d\n", nonceSizeInBits);
                        printf("ADlen: %d\n", ADlen);
                        printf("Mlen: %d\n", Mlen);
                        displayByteString(stdout, "plaintext     ", plaintext, Mlen);
                        displayByteString(stdout, "plaintextPrime", plaintextPrime, Mlen);
                    }
                    errorIfNotZero( memcmp(plaintext, plaintextPrime, Mlen) );

                    errorIfNotZero( Ketje_GetTag( &ketje1, tag1, 16) );
                    errorIfNotZero( Ketje_GetTag( &ketje2, tag2, 16) );
                    if (memcmp(tag1, tag2, 16) != 0) { // !!!
                        printf("keySizeInBits: %d\n", keySizeInBits);
                        printf("nonceSizeInBits: %d\n", nonceSizeInBits);
                        printf("ADlen: %d\n", ADlen);
                        printf("Mlen: %d\n", Mlen);
                        displayByteString(stdout, "tag1", tag1, 16);
                        displayByteString(stdout, "tag2", tag2, 16);
                    }
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
    assert(memcmp(overallChecksum, expected, 16) == 0, "Wrong checksum");
}

#undef Ketje_Instance
#undef Ketje_Initialize
#undef Ketje_FeedAssociatedData
#undef Ketje_WrapPlaintext
#undef Ketje_UnwrapCiphertext
#undef Ketje_GetTag

#undef test
