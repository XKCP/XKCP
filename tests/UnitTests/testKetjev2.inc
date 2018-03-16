/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

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
    int Klen;
    #ifdef OUTPUT
        FILE *f = fopen(file, "w");
    #endif
    Ketje_Instance checksum;
    unsigned char overallChecksum[16];

#ifdef OUTPUT
    assert(f != NULL, "Could not open log file");
#endif

    Ketje_Initialize(&checksum, 0, 0, 0, 0);

    for(Klen=(SnP_width - 18)/8; Klen >= 12; Klen -= (Klen > 47 ) ? 25 : ((Klen > 22) ? 5 : ((Klen > 14) ? 2 : 1)))
    {
        int Nlen;
        int keySizeInBits = Klen*8;
        int NlenMax = (SnP_width - 18)/8 - Klen;
        for(Nlen = ((Klen == 16) ? 0 : NlenMax); Nlen <= NlenMax; Nlen+=SnP_width <= 400 ? 1 : (SnP_width/200))
        {
            int nonceSizeInBits = Nlen*8;
            Ketje_Instance ketje1;
            Ketje_Instance ketje2;
            unsigned char key[SnP_width/8];
            unsigned char nonce[SnP_width/8];
            unsigned int Alen;

#ifdef OUTPUT
            #define kname(nnn) #nnn
            printf( "%s, key length is %u bits, nonce length is %u bits\n", kname(prefix), keySizeInBits, nonceSizeInBits );
            #undef kname
#endif
            generateSimpleRawMaterial(key, Klen, 0x12 + Nlen + Klen, 3);
            generateSimpleRawMaterial(nonce, Nlen, 0x23 + Nlen + Klen, 6);


            assert( Ketje_Initialize( &ketje1, key, keySizeInBits, nonce, nonceSizeInBits) == 0, "Ketje_Initialize 1 did not return zero" );
            assert( Ketje_Initialize( &ketje2, key, keySizeInBits, nonce, nonceSizeInBits) == 0, "Ketje_Initialize 2 did not return zero" );

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
            displayByteString(f, "> K (key)", key, (keySizeInBits+7)/8);
            displayByteString(f, "> N (nonce)", nonce, (nonceSizeInBits+7)/8);
            fprintf(f, "\n");
#endif

            for(Alen=0; Alen<=50; Alen+=1+Alen/3+(Klen-12)+Nlen/32)
            {
                unsigned int Blen;
                for(Blen=0; Blen<=50; Blen+=Blen/2+1+Alen+((Alen == 0) ? (Klen-12) : (Nlen/32+Klen/2)))
                {
                    unsigned int ell;
                    for(ell=((Klen==16) ? 0 : 128); ell<=(unsigned int)((Klen==16) ? 256 : 128); ell+=64)
                    {
                        unsigned char associatedData[400];
                        unsigned char plaintext[400];
                        unsigned char ciphertext[400];
                        unsigned char plaintextPrime[400];
                        unsigned char tag1[32], tag2[32];
                        /* printf("Alen %u, Blen %u\n", Alen, Blen); */
                        generateSimpleRawMaterial(associatedData, Alen, 0x34+Alen+Blen+(ell/8), 4);
                        generateSimpleRawMaterial(plaintext, Blen, 0x45+Alen+Blen+(ell/8), 7);

#ifdef OUTPUT
                        displayByteString(f, "> A (associated data)", associatedData, Alen);
                        displayByteString(f, "> B (plaintext)", plaintext, Blen);
#endif
                        {
                            unsigned int split = myMin(Alen/4, (unsigned int)200);
                            unsigned int i;

                            for(i=0; i<split; i++)
                                assert( Ketje_FeedAssociatedData( &ketje1, associatedData+i, 1) == 0, "Ketje_FeedAssociatedData 1a did not return zero" );
                            if (split < Alen)
                                assert( Ketje_FeedAssociatedData( &ketje1, associatedData+split, Alen-split) == 0, "Ketje_FeedAssociatedData 1b did not return zero" );
                        }

                        {
                            unsigned int split = Blen/3;
                            memcpy(ciphertext, plaintext, split);
                            assert( Ketje_WrapPlaintext( &ketje1, ciphertext, ciphertext, split) == 0, "Ketje_WrapPlaintext 1a did not return zero" ); /* in place */
                            assert( Ketje_WrapPlaintext( &ketje1, plaintext+split, ciphertext+split, Blen-split) == 0, "Ketje_WrapPlaintext 1b did not return zero" );
                        }
                        assert( Ketje_GetTag( &ketje1, tag1, ell/8) == 0, "Ketje_GetTag 1 did not return zero" );

#ifdef OUTPUT
                        displayByteString(f, "< C (ciphertext)", ciphertext, Blen);
                        displayByteString(f, "< T (tag)", tag1, ell/8);
#endif
                        assert( Ketje_FeedAssociatedData( &ketje2, associatedData, Alen) == 0, "Ketje_FeedAssociatedData 2 did not return zero" );
                        {
                            unsigned int split = Blen/3*2;
                            memcpy(plaintextPrime, ciphertext, split);
                            assert( Ketje_UnwrapCiphertext(&ketje2, plaintextPrime, plaintextPrime, split) == 0, "Ketje_UnwrapCiphertext 2a did not return zero" ); /* in place */
                            assert( Ketje_UnwrapCiphertext(&ketje2, ciphertext+split, plaintextPrime+split, Blen-split) == 0, "Ketje_UnwrapCiphertext 2b did not return zero" );
                        }
#ifdef OUTPUT
                        if (memcmp(plaintext, plaintextPrime, Blen) != 0) { /* !!! */
                            printf("keySizeInBits: %d\n", keySizeInBits);
                            printf("nonceSizeInBits: %d\n", nonceSizeInBits);
                            printf("Alen: %d\n", Alen);
                            printf("Blen: %d\n", Blen);
                            displayByteString(stdout, "plaintext     ", plaintext, Blen);
                            displayByteString(stdout, "plaintextPrime", plaintextPrime, Blen);
                        }
#endif
                        assert( !memcmp(plaintext, plaintextPrime, Blen), "Unwrapped plaintext does not match" );
                        assert( Ketje_GetTag( &ketje2, tag2, ell/8) == 0, "Ketje_GetTag 2 did not return zero" );
#ifdef OUTPUT
                        if (memcmp(tag1, tag2, ell/8) != 0) { /* !!! */
                            printf("keySizeInBits: %d\n", keySizeInBits);
                            printf("nonceSizeInBits: %d\n", nonceSizeInBits);
                            printf("Alen: %d\n", Alen);
                            printf("Blen: %d\n", Blen);
                            displayByteString(stdout, "tag1", tag1, ell/8);
                            displayByteString(stdout, "tag2", tag2, ell/8);
                        }
#endif
                        assert( !memcmp(tag1, tag2, ell/8), "Tags do not match" );

                        Ketje_FeedAssociatedData(&checksum, ciphertext, Blen);
                        Ketje_FeedAssociatedData(&checksum, tag1, ell/8);

#ifdef OUTPUT
                        fprintf(f, "\n");
#endif
                    }
                }
            }
        }
    }
    Ketje_WrapPlaintext(&checksum, 0, 0, 0);
    Ketje_GetTag(&checksum, overallChecksum, 16);
#ifdef OUTPUT
    displayByteString(f, "+++ Global tag ", overallChecksum, 16);
    fclose(f);
#endif
    assert(memcmp(overallChecksum, expected, 16) == 0, "The global tag does not match.");
}

#undef Ketje_Instance
#undef Ketje_Initialize
#undef Ketje_FeedAssociatedData
#undef Ketje_WrapPlaintext
#undef Ketje_UnwrapCiphertext
#undef Ketje_GetTag

#undef test
