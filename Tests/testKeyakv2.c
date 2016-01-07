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

//#define	NONCE_200


#ifndef OUTPUT
#define	FILE	void
#endif
#include <stdlib.h>
#include <string.h>
#include "Keyakv2.h"
#include "testKeyakv2.h"

#define myMax(a, b)	((a) > (b)) ? (a) : (b)

#ifdef	NONCE_200
#if		MaxNoncelength != 200
#error	"define MaxNoncelength in Keyakv2.h must be changed to 200 to be able to run this test"
#endif
#endif



#ifdef OUTPUT
static void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#else
#define displayByteString(f, synopsis, data, length)
#endif

static void generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        unsigned char byte = seed1 + 161*length - iRolled + i;
        data[i] = byte;
    }
}

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

static void testKeyakInitialize(Keyak_Instance * global, Keyak_Instance * wrap, Keyak_Instance * unwrap, FILE * f, 
	const unsigned char *K, unsigned int Klen, const unsigned char *N, unsigned int Nlen, int forgetFlag, int tagFlag)
{
	unsigned char tag[TagLength];
	unsigned char dummyTag[TagLength];
    int rv;

	#ifdef OUTPUT
    fprintf( f, "*** Keyak[b=%u, nr=12, \316\240=%u, c=%u, \317\204=%u]\n", SnP_width, PlSnP_P, Capacity*8, TagLength*8 );
    fprintf( f, "StartEngine(K, N, tagFlag=%s, T, unwrapFlag=false, forgetFlag=%s), with:\n", tagFlag?"true":"false", forgetFlag?"true":"false" );
    displayByteString(f, "> K", K, Klen);
    displayByteString(f, "> N", N, Nlen);
	#endif

    rv = Keyak_Initialize( wrap, K, Klen, N, Nlen, tagFlag, tag, Atom_False, forgetFlag);
    assert(rv == Atom_True, "wrap.Initialize() did not return true.");
    if (tagFlag)
	{
		#ifdef OUTPUT
        displayByteString(f, "< T (tag)", tag, TagLength);
		#endif
	    Keyak_Wrap( global, 0, 0, 0, tag, TagLength, dummyTag, 0, 0);
	}

    rv = Keyak_Initialize( unwrap, K, Klen, N, Nlen, tagFlag, tag, Atom_True, forgetFlag);
    assert(rv == Atom_True, "unwrap.Initialize() did not return true.");
 	#ifdef OUTPUT
	fprintf( f, "\n" );
	#endif
}

static void testKeyakWrapUnwrap(int mode, Keyak_Instance * global, Keyak_Instance * wrap, Keyak_Instance * unwrap, FILE * f, 
	unsigned char * ADcontent, size_t ADlen, unsigned char *Pcontent, size_t Plen, int forgetFlag, unsigned char *Cbuffer, unsigned char *PPbuffer)
{
	unsigned char tag[TagLength];
	unsigned char dummyTag[TagLength];
    int rv;

	#ifdef OUTPUT
    fprintf(f, "Wrap(I, O, A, T, unwrapFlag=false, forgetFlag=%s), with:\n", forgetFlag?"true":"false");
    displayByteString(f, "> A (metadata)", ADcontent, (unsigned int)ADlen);
    displayByteString(f, "> I (plaintext)", Pcontent, (unsigned int)Plen);
	#endif

	switch ( mode )
	{
		case 0:
		    rv = Keyak_Wrap( wrap, Pcontent, Cbuffer, Plen, ADcontent, ADlen, tag, 0, forgetFlag);
	    	assert(rv == Atom_True, "wrap.Wrap() did not return true.");
			break;

		case 1:
		case 2:
			{
				size_t tempPlen = Plen, tempADlen = ADlen;
				size_t partialPlen, partialADlen, t;
				size_t totalPlen, totalADlen;
				size_t processedI, processedMD;
				int lastFlag;
				unsigned char *I, *O, *AD;

				totalPlen = totalADlen = 0;
				partialPlen = partialADlen = 0;
				lastFlag = 0;
				I = Pcontent;
				O = Cbuffer;
				AD = ADcontent;
				do
				{
					if ( tempPlen )
					{
						if ( mode == 1 )
						{
							++partialPlen;
							--tempPlen;
						}
						else
						{
							t = rand() % (tempPlen + 1);
							partialPlen += t;
							tempPlen -= t;
						}
					}
					if ( tempPlen == 0 )
						lastFlag |= Motorist_Wrap_LastCryptData;
					if ( tempADlen )
					{
						if ( mode == 1 )
						{
							++partialADlen;
							--tempADlen;
						}
						else
						{
							t = rand() % (tempADlen + 1);
							partialADlen += t;
							tempADlen -= t;
						}
					}
					if ( tempADlen == 0 )
						lastFlag |= Motorist_Wrap_LastMetaData;
				    rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, lastFlag, &processedI, &processedMD);
			    	assert(rv == Atom_True, "wrap.Wrap() did not return true.");
				    assert(processedI <= partialPlen, "wrap.Wrap() did process too much input.");
			    	assert(processedMD <= partialADlen, "wrap.Wrap() did process too much metadata.");
					I += processedI;
					O += processedI;
					AD += processedMD;
					partialPlen -= processedI;
					partialADlen -= processedMD;
					totalPlen  += processedI;
					totalADlen  += processedMD;
				}
				while ( ((tempPlen | tempADlen) != 0) || (lastFlag != Motorist_Wrap_LastCryptAndMeta) );
			    assert(totalPlen == Plen, "wrap.Wrap() totalPlen != Plen.");
			    assert(totalADlen == ADlen, "wrap.Wrap() totalADlen != ADlen.");
			}
			break;
	}


	#ifdef OUTPUT
    displayByteString(f, "< O (ciphertext)", Cbuffer, (unsigned int)Plen);
    displayByteString(f, "< T (tag)", tag, TagLength);
    fprintf(f, "\n");
	#endif

	switch ( mode )
	{
		case 0:
		    rv = Keyak_Wrap( unwrap, Cbuffer, PPbuffer, Plen, ADcontent, ADlen, tag, 1, forgetFlag);
		    assert(rv == Atom_True, "unwrap.Wrap() did not return true.");
	    	assert(!memcmp( Pcontent, PPbuffer, Plen ), "The plaintexts do not match.");
			break;

		case 1:
		case 2:
			{
				size_t tempPlen = Plen, tempADlen = ADlen;
				size_t partialPlen, partialADlen, t;
				size_t totalPlen, totalADlen;
				size_t processedI, processedMD;
				int toggle = 0, lastFlag;
				unsigned char *I, *O, *AD;

				totalPlen = totalADlen = 0;
				partialPlen = partialADlen = 0;
				lastFlag = 0;
				I = Cbuffer;
				O = PPbuffer;
				AD = ADcontent;
				do
				{
					if ( toggle )
					{
						if ( tempPlen )
						{
							if ( mode == 1 )
							{
								++partialPlen;
								--tempPlen;
							}
							else
							{
								t = rand() % (tempPlen + 1);
								partialPlen += t;
								tempPlen -= t;
							}
						}
						if ( tempPlen == 0 )
							lastFlag |= Motorist_Wrap_LastCryptData;
					}
					else
					{
						if ( tempADlen )
						{
							if ( mode == 1 )
							{
								++partialADlen;
								--tempADlen;
							}
							else
							{
								t = rand() % (tempADlen + 1);
								partialADlen += t;
								tempADlen -= t;
							}
						}
						if ( tempADlen == 0 )
							lastFlag |= Motorist_Wrap_LastMetaData;
					}
					toggle ^= 1;
				    rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, lastFlag, &processedI, &processedMD);
			    	assert(rv == Atom_True, "unwrap.Wrap() did not return true.");
				    assert(processedI <= partialPlen, "unwrap.Wrap() did process too much input.");
			    	assert(processedMD <= partialADlen, "unwrap.Wrap() did process too much metadata.");
					I += processedI;
					O += processedI;
					AD += processedMD;
					partialPlen -= processedI;
					partialADlen -= processedMD;
					totalPlen  += processedI;
					totalADlen  += processedMD;
				}
				while ( ((tempPlen | tempADlen) != 0) || (lastFlag != Motorist_Wrap_LastCryptAndMeta) );
			    assert(totalPlen == Plen, "unwrap.Wrap() totalPlen != Plen.");
			    assert(totalADlen == ADlen, "unwrap.Wrap() totalADlen != ADlen.");
		    	assert(!memcmp( Pcontent, PPbuffer, Plen ), "The plaintexts do not match.");
				break;
			}
	}
    Keyak_Wrap( global, 0, 0, 0, Cbuffer, Plen, dummyTag, 0, 0);
    Keyak_Wrap( global, 0, 0, 0, tag, TagLength, dummyTag, 0, 0);

}


#define	Pi	PlSnP_P

int testKeyak( int mode )
{
    unsigned int Klen;
    unsigned int Nlen;
    unsigned int forgetFlag;
    unsigned int tagFlag;
	unsigned int Mlen;
	unsigned int Alen;
	unsigned int Mleni;
	unsigned int Aleni;
	unsigned char tag[TagLength];
	Keyak_Instance wrap;
	Keyak_Instance unwrap;
	Keyak_Instance global;
	int rv;
	unsigned char K[32];
	unsigned char N[200];
	unsigned char Pbuffer[Rs*Pi*2];
	unsigned char PPbuffer[Rs*Pi*2];
	unsigned char Cbuffer[Rs*Pi*2];
	unsigned char Abuffer[Ra*Pi*2];

#ifdef NONCE_200
	#if (PlSnP_P == 1)
	    #if (KeccakF_width == 1600)
	        #ifdef OUTPUT
	            FILE *f = fopen("LakeKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\x83\x95\xc6\x41\x22\xbb\x43\x04\x32\xd8\xb0\x29\x82\x09\xb7\x36";
	    #elif (KeccakF_width == 800)
	        #ifdef OUTPUT
	            FILE *f = fopen("RiverKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\x6e\xba\x81\x33\x0b\xb8\x5a\x4d\x8d\xb3\x7f\xde\x4d\x67\xcd\x0e";
	    #else
	        #error "Which Keyak is this?"
	    #endif
	#else
	    #if (PlSnP_P == 2)
	        #ifdef OUTPUT
	            FILE *f = fopen("SeaKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\xb8\xc0\xe2\x35\x22\xcc\x1d\xe1\x4c\x22\xd0\xb8\xaf\x73\x8e\x33";
	    #elif (PlSnP_P == 4)
	        #ifdef OUTPUT
	            FILE *f = fopen("OceanKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\x70\x7c\x06\x47\xf9\xe8\x52\xb6\x00\xee\xd0\xf1\x1c\x66\xe1\x1d";
	    #elif (PlSnP_P == 8)
	        #ifdef OUTPUT
	            FILE *f = fopen("LunarKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\xb7\xec\x21\x1d\xc0\x30\xd2\x4d\x66\x70\x44\xc2\xed\x34\x52\x11";
	    #else
	        #error "Which Keyak is this?"
	    #endif
	#endif
#else
	#if (PlSnP_P == 1)
	    #if (KeccakF_width == 1600)
	        #ifdef OUTPUT
	            FILE *f = fopen("LakeKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\x73\x03\xc4\xba\x1e\xff\xc3\x9d\x48\x80\x65\xc2\xfd\x05\xf7\x52";
	    #elif (KeccakF_width == 800)
	        #ifdef OUTPUT
	            FILE *f = fopen("RiverKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\xa7\xce\x27\x81\x19\x38\x13\x11\xa1\x1f\x8f\xac\x84\xcb\x6b\x24";
	    #else
	        #error "Which Keyak is this?"
	    #endif
	#else
	    #if (PlSnP_P == 2)
	        #ifdef OUTPUT
	            FILE *f = fopen("SeaKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\xc4\xaa\x23\x13\x54\x88\x8d\xb6\xdc\x5c\xc3\xc8\xe9\xba\x86\x76";
	    #elif (PlSnP_P == 4)
	        #ifdef OUTPUT
	            FILE *f = fopen("OceanKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\xd2\xff\x93\x7a\xf6\x17\x48\xe9\xfb\x90\xee\x46\x37\x9c\x5b\x02";
	    #elif (PlSnP_P == 8)
	        #ifdef OUTPUT
	            FILE *f = fopen("LunarKeyak.txt", "w");
	        #endif
	        const unsigned char *expected = (unsigned char *)"\xed\xa4\x43\xe6\xe2\xf8\x36\xb4\x58\xce\xe2\x93\xdf\xb6\xc6\x60";
	    #else
	        #error "Which Keyak is this?"
	    #endif
	#endif
#endif

#ifdef OUTPUT
    assert(f != NULL, "Could not open file");
#else
	void *f = NULL;
#endif

    rv = Keyak_Initialize( &global, 0, 0, 0, 0, Atom_False, 0, Atom_False, Atom_False);
    assert(rv == Atom_True, "global.Initialize() did not return true.");

    for( Klen=16; Klen<=32; Klen++)
    for( Nlen=0; Nlen<=MaxNoncelength; Nlen += (Klen == 16) ? 1 : MaxNoncelength)
    for( forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
    for( tagFlag = 0; tagFlag < 2; ++tagFlag)
    {
        generateSimpleRawMaterial(K, Klen, Klen+Nlen+0x12, 3),
        generateSimpleRawMaterial(N, Nlen, Klen+Nlen+0x45, 6),
        testKeyakInitialize(&global, &wrap, &unwrap, f, K, Klen, N, Nlen, forgetFlag, tagFlag);
        testKeyakWrapUnwrap( mode, &global, &wrap, &unwrap, f, "ABC", 3, "DEF", 3, 0, Cbuffer, PPbuffer);
    }

    {
        unsigned int Alengths[5] = { 0, 1, Pi*(Ra-Rs)-1, Pi*(Ra-Rs), Pi*(Ra-Rs)+1 };

        for(forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for(tagFlag = 0; tagFlag < 2; ++tagFlag)
        for(Aleni=0; Aleni<5; Aleni++)
        for(Mlen=0; Mlen<=(Rs*Pi+1); Mlen+=(Aleni==0)?1:((Pi+forgetFlag)*(W+tagFlag)+1))
        {
            Klen = 16;
            Nlen = (SnP_width == 1600) ? 150 : 58;
            Alen = Alengths[Aleni];

            generateSimpleRawMaterial(K, Klen, 0x23+Mlen+Alen, 4),
            generateSimpleRawMaterial(N, Nlen, 0x56+Mlen+Alen, 7),
            testKeyakInitialize(&global, &wrap, &unwrap, f, K, Klen, N, Nlen, forgetFlag, tagFlag);

            generateSimpleRawMaterial(Abuffer, Alen, 0xAB+Mlen+Alen, 3),
            generateSimpleRawMaterial(Pbuffer, Mlen, 0xCD+Mlen+Alen, 4),
            testKeyakWrapUnwrap(mode, &global, &wrap, &unwrap, f, Abuffer, Alen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);

            generateSimpleRawMaterial(Abuffer, Alen, 0xCD+Mlen+Alen, 3),
            generateSimpleRawMaterial(Pbuffer, Mlen, 0xEF+Mlen+Alen, 4),
            testKeyakWrapUnwrap(mode, &global, &wrap, &unwrap, f, Abuffer, Alen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);
        }
    }

    {
        unsigned int Mlengths[5] = { 0, 1, Pi*Rs-1, Pi*Rs, Pi*Rs+1 };

        for(forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for(tagFlag = 0; tagFlag < 2; ++tagFlag)
        for(Mleni=0; Mleni<5; Mleni++)
        for(Alen=0; Alen<=(Ra*Pi+1); Alen+=(Mleni==0)?1:((Pi+forgetFlag)*(W+tagFlag)+1))
        {
            Klen = 16;
            Nlen = (SnP_width == 1600) ? 150 : 58;
            Mlen = Mlengths[Mleni];

            generateSimpleRawMaterial(K, Klen, 0x34+Mlen+Alen, 5),
            generateSimpleRawMaterial(N, Nlen, 0x45+Mlen+Alen, 6),
            testKeyakInitialize(&global, &wrap, &unwrap, f, K, Klen, N, Nlen, forgetFlag, tagFlag);

            generateSimpleRawMaterial(Abuffer, Alen, 0x01+Mlen+Alen, 5),
            generateSimpleRawMaterial(Pbuffer, Mlen, 0x23+Mlen+Alen, 6),
            testKeyakWrapUnwrap(mode, &global, &wrap, &unwrap, f, Abuffer, Alen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);

            generateSimpleRawMaterial(Abuffer, Alen, 0x45+Mlen+Alen, 5),
            generateSimpleRawMaterial(Pbuffer, Mlen, 0x67+Mlen+Alen, 6),
            testKeyakWrapUnwrap(mode, &global, &wrap, &unwrap, f, Abuffer, Alen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);
        }
    }

    {
        for(forgetFlag=0; forgetFlag<2; forgetFlag++)
        for(tagFlag=0; tagFlag<2; tagFlag++) {
            Klen = 16;
            Nlen = (SnP_width == 1600) ? 150 : 58;

            generateSimpleRawMaterial(K, Klen, forgetFlag*2+tagFlag, 1),
            generateSimpleRawMaterial(N, Nlen, forgetFlag*2+tagFlag, 2),
            testKeyakInitialize(&global, &wrap, &unwrap, f, K, Klen, N, Nlen, forgetFlag, tagFlag);

            for(Alen=0; Alen<=(Ra*Pi*2); Alen+=(Alen/3+1))
            for(Mlen=0; Mlen<=(Rs*Pi*2); Mlen+=(Mlen/2+1+Alen))
            {
                generateSimpleRawMaterial(Abuffer, Alen, 0x34+Mlen+Alen, 3),
                generateSimpleRawMaterial(Pbuffer, Mlen, 0x45+Mlen+Alen, 4),
                testKeyakWrapUnwrap(mode, &global, &wrap, &unwrap, f, Abuffer, Alen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);
            }
        }
    }

    {
	    rv = Keyak_Wrap( &global, 0, 0, 0, 0, 0, tag, 0, 0);
	    assert(rv == Atom_True, "global.Wrap(final tag) did not return true.");
        displayByteString(f, "+++ Global tag", tag, TagLength);
	    assert(!memcmp( expected, tag, TagLength ), "The global tag is incorrect.");
    }

	#ifdef OUTPUT
	printf( "Keyak v2 testmode %u ok\n", mode );
	#endif
	
    return (0);
}
