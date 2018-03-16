/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifdef  NONCE_200
#if     RiverKeyak_MaxNoncelength != 200 || LakeKeyak_MaxNoncelength != 200
#error  "define MaxNoncelength in Keyakv2.h must be changed to 200 to be able to run this test"
#endif
#endif


#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define W                               (SnP_width/8/25)
#define Ra                              (SnP_width/8 - W)
#define Rs                              (SnP_width/8 - Capacity)

#define testKeyakInitialize             JOIN(prefix, _testKeyakInitialize)
#define testKeyakWrapUnwrap             JOIN(prefix, _testKeyakWrapUnwrap)
#define testKeyakOneMode                JOIN(prefix, _testKeyakOneMode)
#define testOneKeyak                    JOIN(prefix, _testOneKeyak)

#define MaxNoncelength                  JOIN(prefix, Keyak_MaxNoncelength)
#define Keyak_Instance                  JOIN(prefix, Keyak_Instance)
#define Keyak_Initialize                JOIN(prefix, Keyak_Initialize)
#define Keyak_Wrap                      JOIN(prefix, Keyak_Wrap)
#define Keyak_WrapPartial               JOIN(prefix, Keyak_WrapPartial)

static void testKeyakInitialize(Keyak_Instance * global, Keyak_Instance * wrap, Keyak_Instance * unwrap, FILE * f, 
    const unsigned char *K, unsigned int Klen, const unsigned char *N, unsigned int Nlen, int forgetFlag, int tagFlag)
{
    unsigned char tag[TagLength];
    unsigned char dummyTag[TagLength];
    int rv;

    #ifdef OUTPUT
    fprintf( f, "*** Keyak[b=%u, nr=12, \316\240=%u, c=%u, \317\204=%u]\n", SnP_width, PlSnP_parallelism, Capacity*8, TagLength*8 );
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

static void testKeyakWrapUnwrap(int testmode, Keyak_Instance * global, Keyak_Instance * wrap, Keyak_Instance * unwrap, FILE * f, 
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

    switch ( testmode )
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
                        if ( testmode == 1 )
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
                        if ( testmode == 1 )
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

        case 3:
        case 4:
        case 5:
            {
                /* Pass all input and AD, but don't set lastflags in first call */
                size_t partialPlen, partialADlen;
                size_t totalPlen, totalADlen;
                size_t processedI, processedMD;
                unsigned char *I, *O, *AD;

                totalPlen = totalADlen = 0;
                partialPlen = Plen;
                partialADlen = ADlen;
                I = Pcontent;
                O = Cbuffer;
                AD = ADcontent;
                rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, 0, &processedI, &processedMD);
                assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                assert(processedI <= partialPlen, "wrap.Wrap() did process too much input.");
                assert(processedMD <= partialADlen, "wrap.Wrap() did process too much metadata.");
                I  += processedI;
                O  += processedI;
                AD += processedMD;
                partialPlen  -= processedI;
                partialADlen -= processedMD;
                totalPlen  += processedI;
                totalADlen += processedMD;

                /*  blank run   */
                rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, 0, &processedI, &processedMD);
                assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                assert(processedI == 0, "wrap.Wrap() did process bad input length.");
                assert(processedMD == 0, "wrap.Wrap() did process bad metadata length.");

                if ( testmode == 4 )
                {
                    rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, Motorist_Wrap_LastCryptData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI <= partialPlen, "wrap.Wrap() did process bad input length.");
                    assert(processedMD <= partialADlen, "wrap.Wrap() did process bad metadata length.");
                    I  += processedI;
                    O  += processedI;
                    AD += processedMD;
                    partialPlen  -= processedI;
                    partialADlen -= processedMD;
                    totalPlen  += processedI;
                    totalADlen += processedMD;

                    /*  blank run   */
                    rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, Motorist_Wrap_LastCryptData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI == 0, "wrap.Wrap() did process bad input length.");
                    assert(processedMD == 0, "wrap.Wrap() did process bad metadata length.");
                }
                else if ( testmode == 5 )
                {
                    rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, Motorist_Wrap_LastMetaData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI <= partialPlen, "wrap.Wrap() did process bad input length.");
                    assert(processedMD <= partialADlen, "wrap.Wrap() did process bad metadata length.");
                    I  += processedI;
                    O  += processedI;
                    AD += processedMD;
                    partialPlen  -= processedI;
                    partialADlen -= processedMD;
                    totalPlen  += processedI;
                    totalADlen += processedMD;

                    /*  blank run   */
                    rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, Motorist_Wrap_LastMetaData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI == 0, "wrap.Wrap() did process bad input length.");
                    assert(processedMD == 0, "wrap.Wrap() did process bad metadata length.");
                }

                rv = Keyak_WrapPartial( wrap, I, O, partialPlen, AD, partialADlen, tag, 0, forgetFlag, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD);
                assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                assert(processedI == partialPlen, "wrap.Wrap() did process bad input length.");
                assert(processedMD == partialADlen, "wrap.Wrap() did process bad metadata length.");
                totalPlen  += processedI;
                totalADlen += processedMD;
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

    switch ( testmode )
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
                            if ( testmode == 1 )
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
                            if ( testmode == 1 )
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

        case 3:
        case 4:
        case 5:
            {
                /* Pass all input and AD, but don't set lastflags in first call */
                size_t partialPlen, partialADlen;
                size_t totalPlen, totalADlen;
                size_t processedI, processedMD;
                unsigned char *I, *O, *AD;

                totalPlen = totalADlen = 0;
                partialPlen = Plen;
                partialADlen = ADlen;
                I = Cbuffer;
                O = PPbuffer;
                AD = ADcontent;
                rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, 0, &processedI, &processedMD);
                assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                assert(processedI <= partialPlen, "wrap.Wrap() did process too much input.");
                assert(processedMD <= partialADlen, "wrap.Wrap() did process too much metadata.");
                I  += processedI;
                O  += processedI;
                AD += processedMD;
                partialPlen  -= processedI;
                partialADlen -= processedMD;
                totalPlen  += processedI;
                totalADlen += processedMD;

                /*  blank run   */
                rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, 0, &processedI, &processedMD);
                assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                assert(processedI == 0, "wrap.Wrap() did process too much input.");
                assert(processedMD == 0, "wrap.Wrap() did process too much metadata.");

                if ( testmode == 4 )
                {
                    rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, Motorist_Wrap_LastCryptData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI <= partialPlen, "wrap.Wrap() did process bad input length.");
                    assert(processedMD <= partialADlen, "wrap.Wrap() did process bad metadata length.");
                    I  += processedI;
                    O  += processedI;
                    AD += processedMD;
                    partialPlen  -= processedI;
                    partialADlen -= processedMD;
                    totalPlen  += processedI;
                    totalADlen += processedMD;

                    /*  blank run   */
                    rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, Motorist_Wrap_LastCryptData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI == 0, "wrap.Wrap() did process too much input.");
                    assert(processedMD == 0, "wrap.Wrap() did process too much metadata.");
                }
                else if ( testmode == 5 )
                {
                    rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, Motorist_Wrap_LastMetaData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI <= partialPlen, "wrap.Wrap() did process bad input length.");
                    assert(processedMD <= partialADlen, "wrap.Wrap() did process bad metadata length.");
                    I  += processedI;
                    O  += processedI;
                    AD += processedMD;
                    partialPlen  -= processedI;
                    partialADlen -= processedMD;
                    totalPlen  += processedI;
                    totalADlen += processedMD;

                    /*  blank run   */
                    rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, Motorist_Wrap_LastMetaData, &processedI, &processedMD);
                    assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                    assert(processedI == 0, "wrap.Wrap() did process too much input.");
                    assert(processedMD == 0, "wrap.Wrap() did process too much metadata.");
                }

                rv = Keyak_WrapPartial( unwrap, I, O, partialPlen, AD, partialADlen, tag, 1, forgetFlag, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD);
                assert(rv == Atom_True, "wrap.Wrap() did not return true.");
                assert(processedI == partialPlen, "wrap.Wrap() did process bad input length.");
                assert(processedMD == partialADlen, "wrap.Wrap() did process bad metadata length.");
                totalPlen  += processedI;
                totalADlen += processedMD;
                assert(totalPlen == Plen, "wrap.Wrap() totalPlen != Plen.");
                assert(totalADlen == ADlen, "wrap.Wrap() totalADlen != ADlen.");

                assert(!memcmp( Pcontent, PPbuffer, Plen ), "The plaintexts do not match.");
            }
            break;
    }
    Keyak_Wrap( global, 0, 0, 0, Cbuffer, Plen, dummyTag, 0, 0);
    Keyak_Wrap( global, 0, 0, 0, tag, TagLength, dummyTag, 0, 0);

}

int testKeyakOneMode( int mode, const char *file, const unsigned char *expected )
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
    unsigned char Pbuffer[Rs*PlSnP_parallelism*2];
    unsigned char PPbuffer[Rs*PlSnP_parallelism*2];
    unsigned char Cbuffer[Rs*PlSnP_parallelism*2];
    unsigned char Abuffer[Ra*PlSnP_parallelism*2];

    #ifdef OUTPUT
    FILE *f = fopen(file, "w");
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
        unsigned int Alengths[5] = { 0, 1, PlSnP_parallelism*(Ra-Rs)-1, PlSnP_parallelism*(Ra-Rs), PlSnP_parallelism*(Ra-Rs)+1 };

        for(forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for(tagFlag = 0; tagFlag < 2; ++tagFlag)
        for(Aleni=0; Aleni<5; Aleni++)
        for(Mlen=0; Mlen<=(Rs*PlSnP_parallelism+1); Mlen+=(Aleni==0)?1:((PlSnP_parallelism+forgetFlag)*(W+tagFlag)+1))
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
        unsigned int Mlengths[5] = { 0, 1, PlSnP_parallelism*Rs-1, PlSnP_parallelism*Rs, PlSnP_parallelism*Rs+1 };


        for(forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for(tagFlag = 0; tagFlag < 2; ++tagFlag)
        for(Mleni=0; Mleni<5; Mleni++)
        for(Alen=0; Alen<=(Ra*PlSnP_parallelism+1); Alen+=(Mleni==0)?1:((PlSnP_parallelism+forgetFlag)*(W+tagFlag)+1))
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

            for(Alen=0; Alen<=(Ra*PlSnP_parallelism*2); Alen+=(Alen/3+1))
            for(Mlen=0; Mlen<=(Rs*PlSnP_parallelism*2); Mlen+=(Mlen/2+1+Alen))
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
        #ifdef OUTPUT
        displayByteString(f, "+++ Global tag", tag, TagLength);
        fclose(f);
        #endif
        assert(!memcmp( expected, tag, TagLength ), "The global tag is incorrect.");
    }

    return (0);
}

int testOneKeyak( const char *file, const unsigned char *expected )
{
    int testmode;

    for ( testmode = 0; testmode <= 5; ++testmode )
    {
        #ifdef OUTPUT
        printf( "%u", testmode );
        #endif
        testKeyakOneMode( testmode, file, expected );
        #ifdef OUTPUT
        printf( "v " );
        #endif
    }
    #ifdef OUTPUT
    printf( "\n" );
    #endif
    return (0);
}

#undef W
#undef Ra
#undef Rs

#undef testKeyakInitialize
#undef testKeyakWrapUnwrap
#undef testKeyakOneMode
#undef testOneKeyak

#undef MaxNoncelength
#undef Keyak_Instance
#undef Keyak_Initialize
#undef Keyak_Wrap
#undef Keyak_WrapPartial
