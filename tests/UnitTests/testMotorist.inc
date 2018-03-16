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

#define testMotoristStartEngine         JOIN(prefix, _testMotoristStartEngine)
#define testMotoristWrapUnwrap          JOIN(prefix, _testMotoristWrapUnwrap)
#define testMotoristOneMode             JOIN(prefix, _testMotoristOneMode)
#define testOneMotorist                 JOIN(prefix, _testOneMotorist)

#define Motorist_Instance               JOIN(prefix, _Motorist_Instance)
#define Motorist_Initialize             JOIN(prefix, _Motorist_Initialize)
#define Motorist_StartEngine            JOIN(prefix, _Motorist_StartEngine)
#define Motorist_Wrap                   JOIN(prefix, _Motorist_Wrap)

static void testMotoristStartEngine(Motorist_Instance * motWrap, Motorist_Instance * motUnwrap, FILE * f, unsigned char * SUVcontent, size_t SUVlen, int forgetFlag, int tagFlag)
{
    unsigned char tag[TagLength];
    int rv;

    #ifdef OUTPUT
    fprintf( f, "***\n" );
    fprintf( f, "initialize with:\n" );
    displayByteString(f, "SUV", SUVcontent, (unsigned int)SUVlen);
    fprintf( f, "\n" );
    #endif

    rv = Motorist_StartEngine( motWrap, SUVcontent, SUVlen, tagFlag, tag, Atom_False, forgetFlag);
    assert(rv == Atom_True, "motWrap.StartEngine() did not return true.");
    #ifdef OUTPUT
    if (forgetFlag)
        fprintf( f, "forget\n" );
    displayByteString(f, "start engine tag", tag, tagFlag ? TagLength : 0 );
    #endif

    rv = Motorist_StartEngine( motUnwrap, SUVcontent, SUVlen, tagFlag, tag, Atom_True, forgetFlag);
    assert(rv == Atom_True, "motUnwrap.StartEngine() did not return true.");
}

static void testMotoristWrapUnwrap(int mode, Motorist_Instance * motWrap, Motorist_Instance * motUnwrap, Motorist_Instance * motWitness, FILE * f, 
    unsigned char * ADcontent, size_t ADlen, unsigned char *Pcontent, size_t Plen, int forgetFlag, unsigned char *Cbuffer, unsigned char *PPbuffer)
{
    unsigned char tag[TagLength];
    int rv;
    size_t processedI, processedMD;

    #ifdef OUTPUT
    displayByteString(f, "associated data", ADcontent, (unsigned int)ADlen);
    displayByteString(f, "plaintext", Pcontent, (unsigned int)Plen);
    #endif

    switch ( mode )
    {
        case 0:
            rv = Motorist_Wrap( motWrap, Pcontent, Plen, Cbuffer, ADcontent, ADlen, tag, 0, forgetFlag, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD);
            assert(rv == Atom_True, "motWrap.Wrap() did not return true.");
            assert(processedI == Plen, "motWrap.Wrap() did not process all input.");
            assert(processedMD == ADlen, "motWrap.Wrap() did not process all metadata.");
            break;

        case 1:
        case 2:
            {
                size_t tempPlen = Plen, tempADlen = ADlen;
                size_t partialPlen, partialADlen, t;
                size_t totalPlen, totalADlen;
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
                    rv = Motorist_Wrap( motWrap, I, partialPlen, O, AD, partialADlen, tag, 0, forgetFlag, lastFlag, &processedI, &processedMD);
                    assert(rv == Atom_True, "motWrap.Wrap() did not return true.");
                    assert(processedI <= partialPlen, "motWrap.Wrap() did process too much input.");
                    assert(processedMD <= partialADlen, "motWrap.Wrap() did process too much metadata.");
                    I += processedI;
                    O += processedI;
                    AD += processedMD;
                    partialPlen -= processedI;
                    partialADlen -= processedMD;
                    totalPlen  += processedI;
                    totalADlen  += processedMD;
                }
                while ( ((tempPlen | tempADlen) != 0) || (lastFlag != Motorist_Wrap_LastCryptAndMeta) );
                assert(totalPlen == Plen, "motWrap.Wrap() totalPlen != Plen.");
                assert(totalADlen == ADlen, "motWrap.Wrap() totalADlen != ADlen.");
            }
            break;

    }

    #ifdef OUTPUT
    displayByteString(f, "ciphertext", Cbuffer, (unsigned int)Plen);
    displayByteString(f, "tag", tag, TagLength);
    fprintf(f, "\n");
    #endif

    switch ( mode )
    {
        case 0:
            rv = Motorist_Wrap( motUnwrap, Cbuffer, Plen, PPbuffer, ADcontent, ADlen, tag, 1, forgetFlag, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD);
            assert(rv == Atom_True, "motUnwrap.Wrap() did not return true.");
            assert(processedI == Plen, "motUnwrap.Wrap() did not process all input.");
            assert(processedMD == ADlen, "motUnwrap.Wrap() did not process all metadata.");
            assert(!memcmp( Pcontent, PPbuffer, Plen ), "The plaintexts do not match.");
            break;

        case 1:
        case 2:
            {
                size_t tempPlen = Plen, tempADlen = ADlen;
                size_t partialPlen, partialADlen, t;
                size_t totalPlen, totalADlen;
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
                    rv = Motorist_Wrap( motUnwrap, I, partialPlen, O, AD, partialADlen, tag, 1, forgetFlag, lastFlag, &processedI, &processedMD);
                    assert(rv == Atom_True, "motUnwrap.Wrap() did not return true.");
                    assert(processedI <= partialPlen, "motUnwrap.Wrap() did process too much input.");
                    assert(processedMD <= partialADlen, "motUnwrap.Wrap() did process too much metadata.");
                    I += processedI;
                    O += processedI;
                    AD += processedMD;
                    partialPlen -= processedI;
                    partialADlen -= processedMD;
                    totalPlen  += processedI;
                    totalADlen  += processedMD;
                }
                while ( ((tempPlen | tempADlen) != 0) || (lastFlag != Motorist_Wrap_LastCryptAndMeta) );
                assert(totalPlen == Plen, "motUnwrap.Wrap() totalPlen != Plen.");
                assert(totalADlen == ADlen, "motUnwrap.Wrap() totalADlen != ADlen.");
                assert(!memcmp( Pcontent, PPbuffer, Plen ), "The plaintexts do not match.");
                break;
            }
    }

    rv = Motorist_Wrap( motWitness, 0, 0, 0, tag, TagLength, tag, 0, 0, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD);
    assert(rv == Atom_True, "motWitness.Wrap(tag) did not return true.");
    rv = Motorist_Wrap( motWitness, 0, 0, 0, Cbuffer, Plen, tag, 0, 0, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD);
    assert(rv == Atom_True, "motWitness.Wrap(cryptogram) did not return true.");
}

static int testMotoristOneMode( int mode, const char *file, const unsigned char *expected )
{
    unsigned int SUVlen;
    unsigned int forgetFlag;
    unsigned int tagFlag;
    unsigned int Mlen;
    unsigned int ADlen;
    unsigned int Mleni;
    unsigned int ADleni;
    unsigned char tag[TagLength];
    size_t dummy;

    Motorist_Instance motWrap;
    Motorist_Instance motUnwrap;
    Motorist_Instance motWitness;
    int rv;
    unsigned char SUVcontent[200];
    unsigned char Pbuffer[myMax(800,200*PlSnP_parallelism)];
    unsigned char PPbuffer[myMax(800,200*PlSnP_parallelism)];
    unsigned char Cbuffer[myMax(800,200*PlSnP_parallelism)];
    unsigned char ADbuffer[myMax(600,200*PlSnP_parallelism)];

    #ifdef OUTPUT
    FILE *f = fopen(file, "w");
    assert(f != NULL, "Could not open file");
    #else
    void *f = NULL;
    #endif

    rv = Motorist_Initialize( &motWitness );
    assert(rv == Atom_Success, "motWitness.Init() did not return zero.");
    rv = Motorist_StartEngine( &motWitness, 0, 0, Atom_False, 0, Atom_False, Atom_False);
    assert(rv == Atom_True, "motWitness.StartEngine() did not return true.");

    {
        for( SUVlen=0; SUVlen<=200; SUVlen++)
        for( forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for( tagFlag = 0; tagFlag < 2; ++tagFlag) {
            rv = Motorist_Initialize( &motWrap );
            assert(rv == Atom_Success, "motWrap.Init() did not return zero.");
            rv = Motorist_Initialize( &motUnwrap );
            assert(rv == Atom_Success, "motUnwrap.Init() did not return zero.");

            generateSimpleRawMaterial( SUVcontent, SUVlen, SUVlen+0x21, 3);
            testMotoristStartEngine(&motWrap, &motUnwrap, f, SUVcontent, SUVlen, forgetFlag, tagFlag);
            testMotoristWrapUnwrap( mode, &motWrap, &motUnwrap, &motWitness, f, "ABC", 3, "DEF", 3, 0, Cbuffer, PPbuffer );
        }
    }

    {
        unsigned int ADlengths[6] = { 0, 1, 28, 0000, 0000, 0000 };
        ADlengths[3] = 24*PlSnP_parallelism-1;
        ADlengths[4] = 24*PlSnP_parallelism;
        ADlengths[5] = 24*PlSnP_parallelism+1;
        for( Mlen=0; Mlen<=(200*PlSnP_parallelism); Mlen++)
        for( ADleni=0; ADleni<6; ADleni++)
        for( forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for( tagFlag = 0; tagFlag < 2; ++tagFlag) {
            SUVlen = 40;
            ADlen = ADlengths[ADleni];
            rv = Motorist_Initialize( &motWrap );
            assert(rv == Atom_Success, "motWrap.Init() did not return zero.");
            rv = Motorist_Initialize( &motUnwrap );
            assert(rv == Atom_Success, "motUnwrap.Init() did not return zero.");

            generateSimpleRawMaterial(SUVcontent, SUVlen, SUVlen+0x32, 4);
            testMotoristStartEngine(&motWrap, &motUnwrap, f, SUVcontent, SUVlen, forgetFlag, tagFlag);

            generateSimpleRawMaterial(ADbuffer, ADlen, 0xAB+Mlen, 3);
            generateSimpleRawMaterial(Pbuffer, Mlen, 0xCD+ADlen, 4);
            testMotoristWrapUnwrap( mode, &motWrap, &motUnwrap, &motWitness, f, ADbuffer, ADlen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer );

            generateSimpleRawMaterial(ADbuffer, ADlen, 0xCD+Mlen, 3);
            generateSimpleRawMaterial(Pbuffer, Mlen, 0xEF+ADlen, 4);
            testMotoristWrapUnwrap( mode, &motWrap, &motUnwrap, &motWitness, f, ADbuffer, ADlen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);
        }
    }

    {
        unsigned int Mlengths[6] = { 0, 1, 68, 0000, 0000, 0000 };
        Mlengths[3] = 168*PlSnP_parallelism-1;
        Mlengths[4] = 168*PlSnP_parallelism;
        Mlengths[5] = 168*PlSnP_parallelism+1;
        for( ADlen=0; ADlen<=(200*PlSnP_parallelism); ADlen++)
        for( Mleni=0; Mleni<6; Mleni++)
        for( forgetFlag = 0; forgetFlag < 2; ++forgetFlag)
        for( tagFlag = 0; tagFlag < 2; ++tagFlag) 
        {
            SUVlen = 40;
            Mlen = Mlengths[Mleni];
            rv = Motorist_Initialize( &motWrap );
            assert(rv == Atom_Success, "motWrap.Init() did not return zero.");
            rv = Motorist_Initialize( &motUnwrap );
            assert(rv == Atom_Success, "motUnwrap.Init() did not return zero.");

            generateSimpleRawMaterial(SUVcontent, SUVlen, SUVlen+0x43, 4);
            testMotoristStartEngine( &motWrap, &motUnwrap, f, SUVcontent, SUVlen, forgetFlag, tagFlag);

            generateSimpleRawMaterial(ADbuffer, ADlen, 0x01+Mlen, 5);
            generateSimpleRawMaterial(Pbuffer, Mlen, 0x23+ADlen, 6);
            testMotoristWrapUnwrap( mode, &motWrap, &motUnwrap, &motWitness, f, ADbuffer, ADlen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);

            generateSimpleRawMaterial(ADbuffer, ADlen, 0x45+Mlen, 5);
            generateSimpleRawMaterial(Pbuffer, Mlen, 0x67+ADlen, 6);
            testMotoristWrapUnwrap( mode, &motWrap, &motUnwrap, &motWitness, f, ADbuffer, ADlen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);
        }
    }

    for( SUVlen=32; SUVlen<=(32+3); SUVlen++)
    {
        for ( forgetFlag = 0; forgetFlag < 2; ++forgetFlag )
        for ( tagFlag = 0; tagFlag < 2; ++tagFlag )
        {
            rv = Motorist_Initialize( &motWrap );
            assert(rv == Atom_Success, "motWrap.Init() did not return zero.");
            rv = Motorist_Initialize( &motUnwrap );
            assert(rv == Atom_Success, "motUnwrap.Init() did not return zero.");

            generateSimpleRawMaterial(SUVcontent, SUVlen, SUVlen+0x12, 1);
            testMotoristStartEngine(&motWrap, &motUnwrap, f, SUVcontent, SUVlen, forgetFlag, tagFlag);

            for(ADlen=0; ADlen<=600; ADlen+=(SUVlen > 32) ? 600 : (ADlen/3+1))
            for(Mlen=0; Mlen<=800; Mlen+=(SUVlen > 32) ? 800 : (Mlen/2+1+ADlen)) 
            {
                generateSimpleRawMaterial(ADbuffer, ADlen, 0x34+Mlen, 3);
                generateSimpleRawMaterial(Pbuffer, Mlen, 0x45+ADlen, 4);
                testMotoristWrapUnwrap( mode, &motWrap, &motUnwrap, &motWitness, f, ADbuffer, ADlen, Pbuffer, Mlen, forgetFlag, Cbuffer, PPbuffer);
            }
        }
    }

    rv = Motorist_Wrap( &motWitness, 0, 0, 0, 0, 0, tag, 0, 0, Motorist_Wrap_LastCryptAndMeta, &dummy, &dummy);
    assert(rv == Atom_True, "motWitness.Wrap(final tag) did not return true.");
    #if defined(GENERATE)
    for ( Mlen = 0; Mlen < TagLength; ++Mlen )
    {
        printf( "\\x%02x", tag[Mlen] );
    }
    printf("\n");
    getch();
    #else
    assert(!memcmp( expected, tag, TagLength ), "motWitness tag check fail.");
    #if !defined(EMBEDDED)
    printf("It works (%u)!\n", mode);
    #endif
    #endif
    return (0);
}

int testOneMotorist( const char *file, const unsigned char *expected )
{
    int mode;

    for ( mode = 0; mode <= 2; ++mode )
    {
        testMotoristOneMode( mode, file, expected );
    }
    return (0);
}

#undef testMotoristStartEngine
#undef testMotoristWrapUnwrap
#undef testMotoristOneMode
#undef testOneMotorist

#undef Motorist_Instance
#undef Motorist_Initialize
#undef Motorist_StartEngine
#undef Motorist_Wrap
