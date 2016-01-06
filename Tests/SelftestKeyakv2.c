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

#ifndef	EMBEDDED
#define OUTPUT
#include <stdio.h>
#include <stdlib.h>
#endif

#include "Keyakv2.h"
#include "selftestKeyakv2.h"

#ifdef OUTPUT
static void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
   	for(i=0; i<length; i++)
       	fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif

static void assert(int condition, char * synopsis)
{
    if (!condition)
	{
		#ifdef OUTPUT
        printf("%s", synopsis);
		#endif
		#ifdef EMBEDDED
		for ( ; ; )	;
		#else
		exit(1);
		#endif

	}
}

#define	MaxSize	200

int KeyakTester(const unsigned char *key, unsigned int keySizeInBytes, 
				const unsigned char *nonce, unsigned int nonceSizeInBytes,
				const unsigned char *plaintext, size_t plaintextLen, 
				const unsigned char *ciphertext, 
				const unsigned char *AD, size_t ADlen,
				const unsigned char *tag )
{
	Keyak_Instance	keyakWrap;
	Keyak_Instance	keyakUnwrap;
	unsigned char localPlaintext[MaxSize];
	unsigned char localCiphertext[MaxSize];
	unsigned char localTag[TagLength];
	unsigned char localNonce[MaxNoncelength];
	int result;
	int inplace;
	int lastFlag;
	size_t	partialIn, partLenIn, totalIn;
	size_t	partialAD, partLenAD, totalAD;
	size_t	processedIn;
	size_t	processedAD;
	const unsigned char * pIn;

	memset(localNonce, 0, sizeof(localNonce) );
	memcpy(localNonce, nonce, nonceSizeInBytes );

	/*	Test with full blocks	*/
	for ( inplace = 0; inplace < 1; ++inplace )
	{
		result = Keyak_Initialize( &keyakWrap, key, keySizeInBytes, localNonce, sizeof(localNonce), 0, localTag, 0, 0 );
		assert( result == Atom_True, "Keyak_Initialize (wrap) did not return true" );

		if ( inplace )
		{
			memcpy(localCiphertext, plaintext, plaintextLen );
			result = Keyak_Wrap( &keyakWrap, localCiphertext, localCiphertext, plaintextLen, AD, ADlen, localTag, 0, 0 );
		}
		else
		{
			memset(localCiphertext, 0x33, plaintextLen );
			result = Keyak_Wrap( &keyakWrap, plaintext, localCiphertext, plaintextLen, AD, ADlen, localTag, 0, 0 );
		}
		assert( result == Atom_True, "Keyak_Wrap (wrap) did not return true" );
		assert( !memcmp( ciphertext, localCiphertext, plaintextLen ), "Keyak_Wrap ciphertext difference" );
		assert( !memcmp( tag, localTag, TagLength ), "Keyak_Wrap tag difference" );

		result = Keyak_Initialize( &keyakUnwrap, key, keySizeInBytes, localNonce, sizeof(localNonce), 0, localTag, 1, 0 );
		assert( result == Atom_True, "Keyak_Initialize (unwrap) did not return true" );

		if ( inplace )
		{
			memcpy(localPlaintext, ciphertext, plaintextLen );
			result = Keyak_Wrap( &keyakUnwrap, localPlaintext, localPlaintext, plaintextLen, AD, ADlen, localTag, 1, 0 );
		}
		else
		{
			memset(localPlaintext, 0x33, plaintextLen );
			result = Keyak_Wrap( &keyakUnwrap, ciphertext, localPlaintext, plaintextLen, AD, ADlen, localTag, 1, 0 );
		}
		assert( result == Atom_True, "Keyak_Wrap (unwrap) did not return true" );
		assert( !memcmp( plaintext, localPlaintext, plaintextLen ), "Keyak_Wrap plaintext difference" );
	}

	/*	Test with partial blocks	*/
	for ( partialIn = 1; partialIn < plaintextLen; ++partialIn )
	{
		for ( partialAD = 1; partialAD < ADlen; ++partialAD )
		{
			for ( inplace = 0; inplace < 1; ++inplace )
			{
				/*	Wrap it	*/
				result = Keyak_Initialize( &keyakWrap, key, keySizeInBytes, localNonce, sizeof(localNonce), 0, localTag, 0, 0 );
				assert( result == Atom_True, "Keyak_Initialize (wrap) did not return true" );

				partLenIn = partLenAD = 0;
				lastFlag = 0;
				if ( inplace )
				{
					pIn = localCiphertext;
					memcpy(localCiphertext, plaintext, plaintextLen );
				}
				else
				{
					pIn = plaintext;
					memset(localCiphertext, 0x33, plaintextLen );
				}

				for ( totalIn = 0, totalAD = 0; (totalIn < plaintextLen) || (totalAD < ADlen); /**/ )
				{
					partLenIn += min( partialIn, plaintextLen - totalIn );
					if ( (partLenIn + totalIn) == plaintextLen )
						lastFlag |= Motorist_Wrap_LastCryptData;
					partLenAD += min( partialAD, ADlen - totalAD );
					if ( (partLenAD + totalAD) == ADlen )
						lastFlag |= Motorist_Wrap_LastMetaData;
					result = Keyak_WrapPartial( &keyakWrap, pIn+totalIn, localCiphertext+totalIn, partLenIn, AD+totalAD, partLenAD, localTag, 0, 0, lastFlag, &processedIn, &processedAD );
					assert( result == Atom_True, "Keyak_Wrap (wrap) did not return true" );
					totalIn += processedIn;
					totalAD += processedAD;
					partLenIn -= processedIn;
					partLenAD -= processedAD;
				}
				assert( !memcmp( ciphertext, localCiphertext, plaintextLen ), "Keyak_Wrap ciphertext difference" );
				assert( !memcmp( tag, localTag, TagLength ), "Keyak_Wrap tag difference" );

				/*	Unwrap	*/
				result = Keyak_Initialize( &keyakUnwrap, key, keySizeInBytes, localNonce, sizeof(localNonce), 0, localTag, 1, 0 );
				assert( result == Atom_True, "Keyak_Initialize (unwrap) did not return true" );

				partLenIn = partLenAD = 0;
				lastFlag = 0;
				if ( inplace )
				{
					pIn = localPlaintext;
					memcpy(localPlaintext, ciphertext, plaintextLen );
				}
				else
				{
					pIn = ciphertext;
					memset(localPlaintext, 0x33, plaintextLen );
				}

				for ( totalIn = 0, totalAD = 0; (totalIn < plaintextLen) || (totalAD < ADlen); /**/ )
				{
					partLenIn += min( partialIn, plaintextLen - totalIn );
					if ( (partLenIn + totalIn) == plaintextLen )
						lastFlag |= Motorist_Wrap_LastCryptData;
					partLenAD += min( partialAD, ADlen - totalAD );
					if ( (partLenAD + totalAD) == ADlen )
						lastFlag |= Motorist_Wrap_LastMetaData;
					result = Keyak_WrapPartial( &keyakUnwrap, pIn+totalIn, localPlaintext+totalIn, partLenIn, AD+totalAD, partLenAD, localTag, 1, 0, lastFlag, &processedIn, &processedAD );
					assert( result == Atom_True, "Keyak_Wrap (unwrap) did not return true" );
					totalIn += processedIn;
					totalAD += processedAD;
					partLenIn -= processedIn;
					partLenAD -= processedAD;
				}
				assert( !memcmp( plaintext, localPlaintext, plaintextLen ), "Keyak_Wrap plaintext difference" );
			}
		}
	}

	return ( 0 );
}

typedef const unsigned char CU8;

int KeyakSelftest( void )
{
    CU8 key[] = "\x5a\x4b\x3c\x2d\x1e\x0f\x00\xf1\xe2\xd3\xc4\xb5\xa6\x97\x88\x79";
    CU8 nonce[] = "\x6b\x4c\x2d\x0e\xef\xd0\xb1\x92\x72\x53\x34\x15\xf6\xd7\xb8\x99";
    CU8 AD[] = "\x32\xf3\xb4\x75\x35\xf6";
    CU8 plaintext[] = "\xe4\x65\xe5\x66\xe6\x67\xe7";
	#if	(SnP_width == 800)
    CU8 ciphertext[] = "\x36\xf1\x9c\x60\x8a\xa6\xa5";
    CU8 *tag = (CU8 *)"\x9b\x0f\x7f\xd0\x78\x6e\x58\x6d\xfe\xac\x84\x76\x92\x06\x46\x53";
	#elif PlSnP_P == 1
    CU8 ciphertext[] = "\x42\x9f\x92\x5a\x23\x6a\x5b";
    CU8 *tag = (CU8 *)"\xdf\x2e\x65\x91\xeb\x82\xc1\x5f\x96\xaa\x90\x8c\xe1\x3d\x6d\x56";
	#elif PlSnP_P == 2
    CU8 ciphertext[] = "\xb9\xf6\x7e\xf7\x68\xc1\xb4";
    CU8 *tag = (CU8 *)"\x97\xcf\xad\x20\x00\x6d\xae\x22\x27\xe3\x7a\x6a\x95\xf3\x7d\x4d";
	#elif PlSnP_P == 4
    CU8 ciphertext[] = "\xfd\x67\x7c\x8f\x28\xc8\x64";
    CU8 *tag = (CU8 *)"\x6f\x2d\xf1\x11\xe5\x95\xcf\x14\xed\x4b\x3b\xbf\xe0\xd6\xc3\xac";
	#elif PlSnP_P == 8
    CU8 ciphertext[] = "\x20\xfe\xc6\x15\x45\x02\xc4";
    CU8 *tag = (CU8 *)"\x77\x6b\x6a\x02\xba\xd7\xf9\xd3\x31\xc9\x6b\x62\x6c\x49\xda\xf2";
	#endif

	return ( KeyakTester( key, sizeof(key)-1, nonce, sizeof(nonce)-1, plaintext, sizeof(plaintext)-1, ciphertext, AD, sizeof(AD)-1, tag ) );
}

