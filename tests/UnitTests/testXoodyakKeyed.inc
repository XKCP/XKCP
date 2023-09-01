/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define Xoodyak_TagLength			    16
#define Xoodyak_DataSize			    (3*48+1)
#define Xoodyak_MaxKeySize			    (48-4-1)
#define Xoodyak_MaxNonceSize			16

#define testXoodyakKeyed0ne             JOIN(prefix, _testKeyed0ne)
#define testXoodyakKeyed                JOIN(prefix, _testKeyed)

#define Xoodyak_Instance                JOIN(prefix, _Instance)
#define Xoodyak_Initialize              JOIN(prefix, _Initialize)
#define Xoodyak_Absorb                  JOIN(prefix, _Absorb)
#define Xoodyak_Crypt                   JOIN(prefix, _Crypt)
#define Xoodyak_Squeeze                 JOIN(prefix, _Squeeze)
#define Xoodyak_SqueezeKey				JOIN(prefix, _SqueezeKey)

static void testXoodyakKeyed0ne(	Xoodyak_Instance * global, FILE * f, 
									const uint8_t *K, unsigned int Klen, const uint8_t *ID, unsigned int IDlen, const uint8_t *N, unsigned int Nlen,
									uint8_t * AD, size_t ADlen, uint8_t *P, size_t Plen, 
									unsigned int nbrMessagesInSession, unsigned int keyVariant, unsigned int ratchet, unsigned int squeezeKLen, unsigned int align)
{
	Xoodyak_Instance encrypt;
	Xoodyak_Instance decrypt;
    uint8_t newKey[Xoodyak_MaxKeySize];
    uint8_t newKeyPrime[Xoodyak_MaxKeySize];
    uint8_t tag[Xoodyak_TagLength+3];
    uint8_t tagPrime[Xoodyak_TagLength+3];
    uint8_t PPbuffer[Xoodyak_DataSize+3];
    uint8_t Cbuffer[Xoodyak_DataSize+3];

    #if defined(UT_OUTPUT) || defined(UT_VERBOSE)
    fprintf( f, "*** Xoodyak( keyVariant %u, ratchet %u, SqueezeKey %u, nbrMessagesInSession %u ), with:\n", keyVariant, ratchet, squeezeKLen, nbrMessagesInSession );
    UT_displayByteString(f, "> K", K, Klen);
    UT_displayByteString(f, "> ID", ID, IDlen);
    UT_displayByteString(f, "> N", N, Nlen);
    UT_displayByteString(f, "> A (metadata)", AD, (unsigned int)ADlen);
    UT_displayByteString(f, "> I (plaintext)", P, (unsigned int)Plen);
    #endif

	switch (keyVariant)
	{
		case 0: /* Absorb nonce in blocks */
		    Xoodyak_Initialize(&encrypt, K, Klen, ID, IDlen, NULL, 0);
			Xoodyak_Absorb(&encrypt, N, Nlen);
			break;
		case 1: /* Absorb counter in trickling mode */
		    Xoodyak_Initialize(&encrypt, K, Klen, ID, IDlen, N, Nlen);
			break;
	}

	switch (keyVariant)
	{
		case 0: /* Absorb nonce in blocks */
		    Xoodyak_Initialize(&decrypt, K, Klen, ID, IDlen, NULL, 0);
			Xoodyak_Absorb(&decrypt, N, Nlen);
			break;
		case 1: /* Absorb counter in trickling mode */
		    Xoodyak_Initialize(&decrypt, K, Klen, ID, IDlen, N, Nlen);
			break;
	}

	for ( /* empty */; nbrMessagesInSession != 0; --nbrMessagesInSession) {
		if (squeezeKLen != 0) {
			Xoodyak_SqueezeKey(&encrypt, newKey, squeezeKLen);
		}

		Xoodyak_Absorb(&encrypt, AD, ADlen);
		Xoodyak_Encrypt(&encrypt, P, Cbuffer + align, Plen);
		if (ratchet == 1) { /* ratchet before squeeze */
			Xoodyak_Ratchet(&encrypt);
		}
		Xoodyak_Squeeze(&encrypt, tag + align, Xoodyak_TagLength);
		if (ratchet == 2) { /* ratchet after squeeze */
			Xoodyak_Ratchet(&encrypt);
		}

	    #if defined(UT_OUTPUT) || defined(UT_VERBOSE)
	    UT_displayByteString(f, "< O (ciphertext)", Cbuffer + align, (unsigned int)Plen);
	    UT_displayByteString(f, "< T (tag)", tag + align, Xoodyak_TagLength);
		if (squeezeKLen != 0) {
		    UT_displayByteString(f, "< K (new key)", newKey, squeezeKLen);
		}
	    fprintf(f, "\n");
	    #endif

		if (squeezeKLen != 0) {
			Xoodyak_SqueezeKey(&decrypt, newKeyPrime, squeezeKLen);
		}
		Xoodyak_Absorb(&decrypt, AD, ADlen);
	    Xoodyak_Decrypt(&decrypt, Cbuffer + align, PPbuffer + align, Plen);
		if (ratchet == 1) { /* ratchet before squeeze */
			Xoodyak_Ratchet(&decrypt);
		}
		Xoodyak_Squeeze(&decrypt, tagPrime + align, Xoodyak_TagLength);
		if (ratchet == 2) { /* ratchet after squeeze */
			Xoodyak_Ratchet(&decrypt);
		}

	    UT_assert(!memcmp( newKey, newKeyPrime, squeezeKLen ), (char*)"The new keys do not match.");
	    UT_assert(!memcmp( P, PPbuffer + align, Plen ), (char*)"The plaintexts do not match.");
	    UT_assert(!memcmp( tag + align, tagPrime + align, Xoodyak_TagLength ), (char*)"The tags do not match.");

		if (squeezeKLen != 0) {
		    Xoodyak_Absorb( global, newKey, squeezeKLen);
		}
	    Xoodyak_Absorb( global, Cbuffer + align, Plen);
	    Xoodyak_Absorb( global, tag + align, Xoodyak_TagLength);
	}

}

static int testXoodyakKeyed( const char *file, const uint8_t *expected )
{
    unsigned int Klen;
    unsigned int IDlen;
    unsigned int Nlen;
    unsigned int ratchet;
	unsigned int keyVariant;
	unsigned int nbrMessagesInSession;
    unsigned int Mlen;
    unsigned int Alen;
    unsigned int Mleni;
    unsigned int Aleni;
    unsigned int newKlen;
    unsigned int align;
    uint8_t checksum[32];
    Xoodyak_Instance global;
    uint8_t K[Xoodyak_MaxKeySize];
    uint8_t N[Xoodyak_MaxNonceSize];
    uint8_t Pbuffer[Xoodyak_DataSize+3];
    uint8_t Abuffer[Xoodyak_DataSize+3];
    uint8_t *ID;
    unsigned int c;
    FILE *f = NULL;

    #if defined(UT_OUTPUT)
    f = fopen(file, "w");
    UT_assert(f != NULL, "Could not open file");
    #elif defined(UT_VERBOSE)
    f = stdout;
    #endif

	for (align = 0; align < 4; ++align) {

	    Xoodyak_Initialize(&global, NULL, 0, NULL, 0, NULL, 0);

		nbrMessagesInSession = 1;
	    for( keyVariant = 0; keyVariant < 2; ++keyVariant)
	    for( ratchet = 0; ratchet < 3; ++ratchet) /* 0, no ratchet, 1 ratchet before squeeze, 2 ratchet after squeeze */
	    for( newKlen = 0; newKlen <= Xoodyak_MaxKeySize; newKlen += 16)
	    for( Klen=16; Klen<=Xoodyak_MaxKeySize; Klen++)
	    for( Nlen=0; Nlen<=Xoodyak_MaxNonceSize; Nlen += (Klen == 16) ? 1 : Xoodyak_MaxNonceSize)
	    {
	        generateSimpleRawMaterial(K, Klen, (uint8_t)(Klen+Nlen+0x12), 3);
	        generateSimpleRawMaterial(N, Nlen, (uint8_t)(Klen+Nlen+0x45), 6);
	        c = 0x1234 + keyVariant + 3*ratchet + 5*newKlen + 9*Nlen;

			IDlen = ((Klen <= 16) || (keyVariant == 2)) ? 0 : (c % (Klen - (16 - 1)));
			ID = K + Klen - IDlen;
	        testXoodyakKeyed0ne(&global, f, K, Klen - IDlen, ID, IDlen, N, Nlen, (uint8_t*)"ABC", 3, (uint8_t*)"DEF", 3, nbrMessagesInSession, keyVariant, ratchet, newKlen, 0);
	    }

	    {
	        unsigned int Alengths[5] = { 0, 1, 48-4-1, 48-4, 48-4+1 };

			newKlen = 0;
	        Klen = 16;
	        Nlen = 16;
			keyVariant = 0;
	        for(ratchet = 0; ratchet < 3; ++ratchet)
		    for(nbrMessagesInSession = 1; nbrMessagesInSession <= 3; ++nbrMessagesInSession)
	        for(Aleni=0; Aleni<5; Aleni++)
	        for(Mlen = 0; Mlen <= Xoodyak_DataSize; Mlen+=(Aleni==0)?1:(ratchet*4+1))
	        {
	            Alen = Alengths[Aleni];

	            generateSimpleRawMaterial(K, Klen, (uint8_t)(0x23+Mlen+Alen), 4);
	            generateSimpleRawMaterial(N, Nlen, (uint8_t)(0x56+Mlen+Alen), 7);
	            generateSimpleRawMaterial(Abuffer + align, Alen, (uint8_t)(0xAB+Mlen+Alen), 3);
	            generateSimpleRawMaterial(Pbuffer + align, Mlen, (uint8_t)(0xCD+Mlen+Alen), 4);
		        testXoodyakKeyed0ne(&global, f, K, Klen, NULL, 0, N, Nlen, Pbuffer + align, Mlen, Abuffer + align, Alen, nbrMessagesInSession, keyVariant, ratchet, newKlen, align);
	        }
	    }

	    {
	        unsigned int Mlengths[5] = { 0, 1, 24-1, 24, 24+1 };

			newKlen = 0;
	        Klen = 16;
	        Nlen = 16;
			keyVariant = 0;
	        for(ratchet = 0; ratchet < 3; ++ratchet)
		    for(nbrMessagesInSession = 1; nbrMessagesInSession <= 3; ++nbrMessagesInSession)
	        for(Mleni=0; Mleni<5; Mleni++)
	        for(Alen=0; Alen<=Xoodyak_DataSize; Alen+=(Mleni==0)?1:(ratchet*4+1))
	        {
	            Mlen = Mlengths[Mleni];

	            generateSimpleRawMaterial(K, Klen, (uint8_t)(0x34+Mlen+Alen), 5);
	            generateSimpleRawMaterial(N, Nlen, (uint8_t)(0x45+Mlen+Alen), 6);
	            generateSimpleRawMaterial(Abuffer + align, Alen, (uint8_t)(0x01+Mlen+Alen), 5);
	            generateSimpleRawMaterial(Pbuffer + align, Mlen, (uint8_t)(0x23+Mlen+Alen), 6);
		        testXoodyakKeyed0ne(&global, f, K, Klen, NULL, 0, N, Nlen, Pbuffer + align, Mlen, Abuffer + align, Alen, nbrMessagesInSession, keyVariant, ratchet, newKlen, align);
	        }
	    }

	    {
			Xoodyak_Squeeze(&global, checksum, sizeof(checksum));
            #if defined(UT_OUTPUT) || defined(UT_VERBOSE)
            UT_displayByteString(f, "+++ Global checksum", checksum, sizeof(checksum));
            #if defined(UT_OUTPUT)
            fclose(f);
            #endif
            #endif
	        UT_assert(!memcmp(expected, checksum, sizeof(checksum)), (char*)"The global checksum is incorrect.");
	    }

	}

    return 0;
}

#undef W
#undef Ra
#undef Rs

#undef testXoodyakKeyed0ne
#undef testXoodyakKeyed

#undef Xoodyak_Instance
#undef Xoodyak_Initialize
#undef Xoodyak_Absorb
#undef Xoodyak_Crypt
#undef Xoodyak_Squeeze
#undef Xoodyak_SqueezeKey
