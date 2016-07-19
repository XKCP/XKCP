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

#include "KeccakCodePackage.h"
#include "KeccakFPH.h"

/* #define OUTPUT */
/* #define VERBOSE */

#define SnP_width               1600
#define blockByteSizeMin        1024
#define blockByteSizeMax        (16*1024)
#define inputByteSize           (80*1024)
#define outputByteSize          512
#define outputByteSizeBis       1024
#define customizationByteSize   32
#define checksumByteSize        16

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <stdlib.h>

#if defined(EMBEDDED)
static void assert(int condition)
{
    if (!condition)
    {
        for ( ; ; ) ;
    }
}
#else
#include <assert.h>
#endif

static void generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled;
        unsigned char byte;
        seed2 = seed2 % 8;
        iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        byte = seed1 + 161*length - iRolled + i;
        data[i] = byte;
    }
}

static void performTestKeccakFPHOneInput(unsigned int securityStrength, unsigned int blockSize, unsigned int inputLen, unsigned int outputLen, unsigned int customLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum, unsigned int mode)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    unsigned char customization[customizationByteSize];
    int result;
    unsigned int i;

    generateSimpleRawMaterial(customization, customizationByteSize, securityStrength/2, 97);
    generateSimpleRawMaterial(input, inputLen, blockSize / 256 + outputLen, inputLen + customLen);

    #ifdef VERBOSE
    printf( "securityStrength %u, blockSize %5u, outputLen %3u, inputLen %5u, customLen % 3u\n", securityStrength, blockSize, outputLen, inputLen, customLen);
    #endif
    if (mode == 0)
    {
        if ( securityStrength == 128 )
            result = Keccak_FPH128( input, inputLen, blockSize, output, outputLen, customization, customLen );
        else
            result = Keccak_FPH256( input, inputLen, blockSize, output, outputLen, customization, customLen );
        assert(result == 0);
    }
    else if (mode == 1)
    {
        if ( securityStrength == 128 )
        {
            Keccak_FPH_Instance fph;
            result = Keccak_FPH128_Initialize(&fph, blockSize, outputLen, customization, customLen);
            assert(result == 0);
            for (i = 0; i < inputLen; ++i )
            {
                result = Keccak_FPH128_Update(&fph, input + i, 1);
                assert(result == 0);
            }
            result =  Keccak_FPH128_Final(&fph, output);
            assert(result == 0);
        }
        else
        {
            Keccak_FPH_Instance fph;
            result = Keccak_FPH256_Initialize(&fph, blockSize, outputLen, customization, customLen);
            assert(result == 0);
            for (i = 0; i < inputLen; ++i )
            {
                result = Keccak_FPH256_Update(&fph, input + i, 1);
                assert(result == 0);
            }
            result =  Keccak_FPH256_Final(&fph, output);
            assert(result == 0);
        }
    }
    else if (mode == 2)
    {
        if ( securityStrength == 128 )
        {
            Keccak_FPH_Instance fph;
            unsigned char *pInput = input;
            result = Keccak_FPH128_Initialize(&fph, blockSize, outputLen, customization, customLen);
            assert(result == 0);
            while (inputLen)
            {
                unsigned int len = ((rand() << 15) ^ rand()) % (inputLen + 1);
                result = Keccak_FPH128_Update(&fph, pInput, len);
                assert(result == 0);
                pInput += len;
                inputLen -= len;
            }
            result =  Keccak_FPH128_Final(&fph, output);
            assert(result == 0);
        }
        else
        {
            Keccak_FPH_Instance fph;
            unsigned char *pInput = input;
            result = Keccak_FPH256_Initialize(&fph, blockSize, outputLen, customization, customLen);
            assert(result == 0);
            while (inputLen)
            {
                unsigned int len = ((rand() << 15) ^ rand()) % (inputLen + 1);
                result = Keccak_FPH256_Update(&fph, pInput, len);
                assert(result == 0);
                pInput += len;
                inputLen -= len;
            }
            result =  Keccak_FPH256_Final(&fph, output);
            assert(result == 0);
        }
    }

    #ifdef VERBOSE
    {
        unsigned int i;

        printf("Keccak-FPH%d\n", securityStrength);
        printf("Input of %d bytes:", inputLen);
        for(i=0; (i<inputLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
            if (inputLen > 16)
                printf(" ...");
        printf("\n");
        printf("Output of %d bytes:", outputLen);
        for(i=0; i<outputLen; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
    }
    #endif

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, outputLen);
}

static void performTestKeccakFPHXOFOneInput(unsigned int securityStrength, unsigned int blockSize, unsigned int inputLen, unsigned int outputLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum, unsigned int mode)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSizeBis];
    int result;
    unsigned int i;

    generateSimpleRawMaterial(input, inputLen, blockSize / 256 + outputLen, inputLen);

    #ifdef VERBOSE
    printf( "securityStrength %u, blockSize %5u, outputLen %3u, inputLen %5u\n", securityStrength, blockSize, outputLen, inputLen);
    #endif
    if (mode == 0)
    {
        if ( securityStrength == 128 )
        {
            Keccak_FPH_Instance fph;
            result = Keccak_FPH128_Initialize(&fph, blockSize, 0, 0, 0);
            assert(result == 0);
            result = Keccak_FPH128_Update(&fph, input, inputLen);
            assert(result == 0);
            result = Keccak_FPH128_Final(&fph, 0);
            assert(result == 0);
            result = Keccak_FPH128_Squeeze(&fph, output, outputLen);
            assert(result == 0);
        }
        else
        {
            Keccak_FPH_Instance fph;
            result = Keccak_FPH256_Initialize(&fph, blockSize, 0, 0, 0);
            assert(result == 0);
            result = Keccak_FPH256_Update(&fph, input, inputLen);
            assert(result == 0);
            result = Keccak_FPH256_Final(&fph, 0);
            assert(result == 0);
            result = Keccak_FPH256_Squeeze(&fph, output, outputLen);
            assert(result == 0);
        }
    }
    else if (mode == 1)
    {
        if ( securityStrength == 128 )
        {
            Keccak_FPH_Instance fph;
            result = Keccak_FPH128_Initialize(&fph, blockSize, 0, 0, 0);
            assert(result == 0);
            result = Keccak_FPH128_Update(&fph, input, inputLen);
            assert(result == 0);
            result = Keccak_FPH128_Final(&fph, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; ++i)
            {
                result =  Keccak_FPH128_Squeeze(&fph, output + i, 1);
                assert(result == 0);
            }
        }
        else
        {
            Keccak_FPH_Instance fph;
            result = Keccak_FPH256_Initialize(&fph, blockSize, 0, 0, 0);
            assert(result == 0);
            result = Keccak_FPH256_Update(&fph, input, inputLen);
            assert(result == 0);
            result = Keccak_FPH256_Final(&fph, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; ++i)
            {
                result =  Keccak_FPH256_Squeeze(&fph, output + i, 1);
                assert(result == 0);
            }
        }
    }
    else if (mode == 2)
    {
        if ( securityStrength == 128 )
        {
            Keccak_FPH_Instance fph;
            unsigned int len;
            result = Keccak_FPH128_Initialize(&fph, blockSize, 0, 0, 0);
            assert(result == 0);
            result = Keccak_FPH128_Update(&fph, input, inputLen);
            assert(result == 0);
            result = Keccak_FPH128_Final(&fph, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; i += len)
            {
                len = ((rand() << 15) ^ rand()) % ((outputLen-i) + 1);
                result = Keccak_FPH128_Squeeze(&fph, output+i, len);
                assert(result == 0);
            }
        }
        else
        {
            Keccak_FPH_Instance fph;
            unsigned int len;
            result = Keccak_FPH256_Initialize(&fph, blockSize, 0, 0, 0);
            assert(result == 0);
            result = Keccak_FPH256_Update(&fph, input, inputLen);
            assert(result == 0);
            result = Keccak_FPH256_Final(&fph, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; i += len)
            {
                len = ((rand() << 15) ^ rand()) % ((outputLen-i) + 1);
                result = Keccak_FPH256_Squeeze(&fph, output+i, len);
                assert(result == 0);
            }
        }
    }

    #ifdef VERBOSE
    {
        unsigned int i;

        printf("Keccak-FPH%d-XOF\n", securityStrength);
        printf("Input of %d bytes:", inputLen);
        for(i=0; (i<inputLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
            if (inputLen > 16)
                printf(" ...");
        printf("\n");
        printf("Output of %d bytes:", outputLen);
        for(i=0; i<outputLen; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
    }
    #endif

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, outputLen);
}

static void performTestKeccakFPH(unsigned int securityStrength, unsigned char *checksum, unsigned int mode)
{
    unsigned int inputLen, outputLen, customLen;
    unsigned int blockSize;

    /* Acumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    outputLen = 256/8;
    customLen = 0;
    for(blockSize=1024; blockSize<=8192; blockSize*=8)
    for(inputLen=0; inputLen<=blockSize*9+123; inputLen++) {
        assert(inputLen <= inputByteSize);
        performTestKeccakFPHOneInput(securityStrength, blockSize, inputLen, outputLen, customLen, &spongeChecksum, mode);
    }
    
    for(blockSize = blockByteSizeMin; blockSize <= blockByteSizeMax; blockSize <<= 1)
    for(outputLen = 128/8; outputLen <= 512/8; outputLen <<= 1)
    for(inputLen = 0; inputLen <= (3*blockSize) && inputLen <= inputByteSize; inputLen = inputLen ? (inputLen + ((securityStrength == 128) ? 167 : 135)) : 1)
    for(customLen = 0; customLen <= customizationByteSize; customLen += 7) {
        assert(inputLen <= inputByteSize);
        performTestKeccakFPHOneInput(securityStrength, blockSize, inputLen, outputLen, customLen, &spongeChecksum, 0);
    }
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE
    {
        unsigned int i;
        printf("Keccak-FPH%d\n", securityStrength);
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

static void performTestKeccakFPHXOF(unsigned int securityStrength, unsigned char *checksum, unsigned int mode)
{
    unsigned int inputLen, outputLen;
    unsigned int blockSize;

    /* Acumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    for(outputLen = 128/8; outputLen <= outputByteSizeBis; outputLen <<= 1)
    for(blockSize = blockByteSizeMin; blockSize <= blockByteSizeMax; blockSize <<= 1)
    for(inputLen = 0; inputLen <= (3*blockSize) && inputLen <= inputByteSize; inputLen = inputLen ? (inputLen << 2) : 1)
    {
        assert(inputLen <= inputByteSize);
        performTestKeccakFPHXOFOneInput(securityStrength, blockSize, inputLen, outputLen, &spongeChecksum, mode);
    }
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE
    {
        unsigned int i;
        printf("Keccak-FPH%d-XOF\n", securityStrength);
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestKeccakFPH(unsigned int securityStrength, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    unsigned int mode;

    for(mode = 0; mode <= 2; ++mode) {
        printf("Testing Keccak-FPH%d %u...", securityStrength, mode);
        fflush(stdout);
        performTestKeccakFPH(securityStrength, checksum, mode);
        assert(memcmp(expected, checksum, checksumByteSize) == 0);
        printf(" - OK.\n");
    }
}

void selfTestKeccakFPHXOF(unsigned int securityStrength, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    unsigned int mode;

    for(mode = 0; mode <= 2; ++mode) {
        printf("Testing Keccak-FPH%d-XOF %u...", securityStrength, mode);
        fflush(stdout);
        performTestKeccakFPHXOF(securityStrength, checksum, mode);
        assert(memcmp(expected, checksum, checksumByteSize) == 0);
        printf(" - OK.\n");
    }
}

#ifdef OUTPUT
void writeTestKeccakFPHOne(FILE *f, unsigned int securityStrength)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestKeccakFPH(securityStrength, checksum, 0);
    fprintf(f, "    selfTestKeccakFPH(%d, \"", securityStrength);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
}

void writeTestKeccakFPHXOFOne(FILE *f, unsigned int securityStrength)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestKeccakFPHXOF(securityStrength, checksum, 0);
    fprintf(f, "    selfTestKeccakFPHXOF(%d, \"", securityStrength);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
}

void writeTestKeccakFPH(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestKeccakFPHOne(f, 128);
    writeTestKeccakFPHOne(f, 256);
    writeTestKeccakFPHXOFOne(f, 128);
    writeTestKeccakFPHXOFOne(f, 256);
    fclose(f);
}
#endif

void testKeccakFPH(void)
{
#ifndef KeccakP1600_excluded
#ifdef OUTPUT
    writeTestKeccakFPH("Keccak-FPH.txt");
#endif

    selfTestKeccakFPH(128, "\xf4\xeb\xb2\xc2\xdf\xf6\xb7\x58\x96\x82\x95\xf9\x79\x7d\x4a\x6d");
    selfTestKeccakFPH(256, "\x61\xe3\x92\xa1\x17\x05\x70\x4a\x58\xfe\x53\x73\x55\x38\xff\x11");

    selfTestKeccakFPHXOF(128, "\xf0\x20\x29\xd0\xa3\x9d\xbd\x1b\x95\xee\x8f\xc3\xb0\xa7\x0e\x0d");
    selfTestKeccakFPHXOF(256, "\x73\x60\x86\x3c\x95\xfe\xc9\x39\xea\xca\xfd\x44\xe9\xaa\xe5\x11");
#endif
}
