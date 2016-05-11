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

#define SnP_width               1600
#define blockByteSizeMin        1024
#define blockByteSizeMax        (16*1024)
#define inputByteSize           (80*1024)
#define outputByteSize          512
#define customizationByteSize   32
#define checksumByteSize        16

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif

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

static void performTestKeccakFPHOneInput(unsigned int securityStrength, unsigned int blockSize, unsigned int inputLen, unsigned int outputLen, unsigned int customLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    unsigned char customization[customizationByteSize];
    int result;

    generateSimpleRawMaterial(customization, customizationByteSize, securityStrength/2, 97);
    generateSimpleRawMaterial(input, inputLen, blockSize / 256 + outputLen, inputLen + customLen);

    #ifdef VERBOSE
    printf( "securityStrength %u, blockSize %5u, outputLen %3u, inputLen %5u, customLen % 3u\n", securityStrength, blockSize, outputLen, inputLen, customLen);
    #endif
    if ( securityStrength == 128 )
        result = Keccak_FPH128( input, inputLen, blockSize, output, outputLen, customization, customLen );
    else
        result = Keccak_FPH256( input, inputLen, blockSize, output, outputLen, customization, customLen );
    assert(result == 0);

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

static void performTestKeccakFPH(unsigned int securityStrength, unsigned char *checksum)
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
        performTestKeccakFPHOneInput(securityStrength, blockSize, inputLen, outputLen, customLen, &spongeChecksum);
    }
    
    for(blockSize = blockByteSizeMin; blockSize <= blockByteSizeMax; blockSize <<= 1)
    for(outputLen = 128/8; outputLen <= 512/8; outputLen <<= 1)
    for(inputLen = 0; inputLen <= (3*blockSize) && inputLen <= inputByteSize; inputLen = inputLen ? (inputLen + ((securityStrength == 128) ? 167 : 135)) : 1)
    for(customLen = 0; customLen <= customizationByteSize; customLen += 7) {
        assert(inputLen <= inputByteSize);
        performTestKeccakFPHOneInput(securityStrength, blockSize, inputLen, outputLen, customLen, &spongeChecksum);
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

void selfTestKeccakFPH(unsigned int securityStrength, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    printf("Testing Keccak-FPH%d...", securityStrength);
    fflush(stdout);
    performTestKeccakFPH(securityStrength, checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
    printf(" - OK.\n");
}

#ifdef OUTPUT
void writeTestKeccakFPHOne(FILE *f, unsigned int securityStrength)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestKeccakFPH(securityStrength, checksum);
    fprintf(f, "    selfTestKeccakFPH(%d, \"", securityStrength);
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
#endif
}
