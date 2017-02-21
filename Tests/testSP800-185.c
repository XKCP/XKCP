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
#include "SP800-185.h"

#define OUTPUT
/* #define VERBOSE */

#define SnP_width               1600
#define inputByteSize           (1*1024)
#define outputByteSize          512
#define nameByteSize            16
#define customizationByteSize   16
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

static void performTestcSHAKEOneInput(unsigned int securityStrength, BitLength inputLen, BitLength outputLen, BitLength nameLen, BitLength customLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum, unsigned int mode)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    unsigned char name[nameByteSize];
    unsigned char customization[customizationByteSize];
    int result;
    unsigned int i;

    generateSimpleRawMaterial(customization, customizationByteSize, securityStrength/2, 97);
    generateSimpleRawMaterial(name, nameByteSize, securityStrength/2, 91);
    generateSimpleRawMaterial(input, (inputLen + 7) / 8, outputLen, inputLen + customLen);

    #ifdef VERBOSE
    printf( "securityStrength %u, outputLen %3u, inputLen %5u, nameLen %3u, customLen %3u\n", securityStrength, outputLen, inputLen, nameLen, customLen);
    #endif
    if (mode == 0)
    {
        if ( securityStrength == 128 )
            result = cSHAKE128( input, inputLen, output, outputLen, name, nameLen, customization, customLen );
        else
            result = cSHAKE256( input, inputLen, output, outputLen, name, nameLen, customization, customLen );
        assert(result == 0);
    }
    else if (mode == 1)
    {
        BitLength l;
        if ( securityStrength == 128 )
        {
            cSHAKE_Instance csk;
            result = cSHAKE128_Initialize(&csk, outputLen, name, nameLen, customization, customLen);
            assert(result == 0);
            for (i = 0; i < inputLen; i += l )
            {
                l = inputLen - i;
                if ( l > 8 )
                    l = 8;
                result = cSHAKE128_Update(&csk, input + i / 8, l);
                assert(result == 0);
            }
            result =  cSHAKE128_Final(&csk, output);
            assert(result == 0);
        }
        else
        {
            cSHAKE_Instance csk;
            result = cSHAKE256_Initialize(&csk, outputLen, name, nameLen, customization, customLen);
            assert(result == 0);
            for (i = 0; i < inputLen; i += l )
            {
                l = inputLen - i;
                if ( l > 8 )
                    l = 8;
                result = cSHAKE256_Update(&csk, input + i / 8, l);
                assert(result == 0);
            }
            result =  cSHAKE256_Final(&csk, output);
            assert(result == 0);
        }
    }
    else if (mode == 2)
    {
        if ( securityStrength == 128 )
        {
            cSHAKE_Instance csk;
            unsigned char *pInput = input;
            result = cSHAKE128_Initialize(&csk, outputLen, name, nameLen, customization, customLen);
            assert(result == 0);
            while (inputLen)
            {
                unsigned int len = ((rand() << 15) ^ rand()) % (inputLen + 1);
                if (len < inputLen)
                    len -= len & 7; 
                result = cSHAKE128_Update(&csk, pInput, len);
                assert(result == 0);
                pInput += len / 8;
                inputLen -= len;
            }
            result =  cSHAKE128_Final(&csk, output);
            assert(result == 0);
        }
        else
        {
            cSHAKE_Instance csk;
            unsigned char *pInput = input;
            result = cSHAKE256_Initialize(&csk, outputLen, name, nameLen, customization, customLen);
            assert(result == 0);
            while (inputLen)
            {
                unsigned int len = ((rand() << 15) ^ rand()) % (inputLen + 1);
                if (len < inputLen)
                    len -= len & 7; 
                result = cSHAKE256_Update(&csk, pInput, len);
                assert(result == 0);
                pInput += len / 8;
                inputLen -= len;
            }
            result =  cSHAKE256_Final(&csk, output);
            assert(result == 0);
        }
    }
    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (outputLen + 7) / 8);

    #ifdef VERBOSE
    {
        unsigned int i;

        printf("cSHAKE%d\n", securityStrength);
        printf("Input of %d bits:", inputLen);
        for(i=0; (i<(inputLen + 7) / 8) && (i<16); i++)
            printf(" %02x", (int)input[i]);
            if (inputLen > 16)
                printf(" ...");
        printf("\n");
        printf("Output of %d bits:", outputLen);
        for(i=0; i<(outputLen + 7) / 8; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
    }
    #endif

}

static void performTestcSHAKEXOFOneInput(unsigned int securityStrength, unsigned int inputLen, unsigned int outputLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum, unsigned int mode)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    int result;
    unsigned int i;

    generateSimpleRawMaterial(input, (inputLen + 7) / 8, outputLen, inputLen);

    #ifdef VERBOSE
    printf( "securityStrength %u, outputLen %4u, inputLen %5u\n", securityStrength, outputLen, inputLen);
    #endif
    if (mode == 0)
    {
        if ( securityStrength == 128 )
        {
            cSHAKE_Instance csk;
            result = cSHAKE128_Initialize(&csk, 0, 0, 0, 0, 0);
            assert(result == 0);
            result = cSHAKE128_Update(&csk, input, inputLen);
            assert(result == 0);
            result = cSHAKE128_Final(&csk, 0);
            assert(result == 0);
            result = cSHAKE128_Squeeze(&csk, output, outputLen);
            assert(result == 0);
        }
        else
        {
            cSHAKE_Instance csk;
            result = cSHAKE256_Initialize(&csk, 0, 0, 0, 0, 0);
            assert(result == 0);
            result = cSHAKE256_Update(&csk, input, inputLen);
            assert(result == 0);
            result = cSHAKE256_Final(&csk, 0);
            assert(result == 0);
            result = cSHAKE256_Squeeze(&csk, output, outputLen);
            assert(result == 0);
        }
    }
    else if (mode == 1)
    {
        BitLength l;
        if ( securityStrength == 128 )
        {
            cSHAKE_Instance csk;
            result = cSHAKE128_Initialize(&csk, 0, 0, 0, 0, 0);
            assert(result == 0);
            result = cSHAKE128_Update(&csk, input, inputLen);
            assert(result == 0);
            result = cSHAKE128_Final(&csk, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; i += l)
            {
                l = outputLen - i;
                if ( l > 8 )
                    l = 8;
                result =  cSHAKE128_Squeeze(&csk, output + i / 8, l);
                assert(result == 0);
            }
        }
        else
        {
            cSHAKE_Instance csk;
            result = cSHAKE256_Initialize(&csk, 0, 0, 0, 0, 0);
            assert(result == 0);
            result = cSHAKE256_Update(&csk, input, inputLen);
            assert(result == 0);
            result = cSHAKE256_Final(&csk, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; i += l)
            {
                l = outputLen - i;
                if ( l > 8 )
                    l = 8;
                result =  cSHAKE256_Squeeze(&csk, output + i / 8, l);
                assert(result == 0);
            }
        }
    }
    else if (mode == 2)
    {
        if ( securityStrength == 128 )
        {
            cSHAKE_Instance csk;
            unsigned int len;
            result = cSHAKE128_Initialize(&csk, 0, 0, 0, 0, 0);
            assert(result == 0);
            result = cSHAKE128_Update(&csk, input, inputLen);
            assert(result == 0);
            result = cSHAKE128_Final(&csk, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; i += len)
            {
                len = ((rand() << 15) ^ rand()) % ((outputLen-i) + 1);
                if (len < (outputLen-i))
                    len -= len & 7; 
                result = cSHAKE128_Squeeze(&csk, output+i / 8, len);
                assert(result == 0);
            }
        }
        else
        {
            cSHAKE_Instance csk;
            unsigned int len;
            result = cSHAKE256_Initialize(&csk, 0, 0, 0, 0, 0);
            assert(result == 0);
            result = cSHAKE256_Update(&csk, input, inputLen);
            assert(result == 0);
            result = cSHAKE256_Final(&csk, 0);
            assert(result == 0);

            for (i = 0; i < outputLen; i += len)
            {
                len = ((rand() << 15) ^ rand()) % ((outputLen-i) + 1);
                if (len < (outputLen-i))
                    len -= len & 7; 
                result = cSHAKE256_Squeeze(&csk, output+i / 8, len);
                assert(result == 0);
            }
        }
    }
    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (outputLen + 7) / 8);

    #ifdef VERBOSE
    {
        unsigned int i;

        printf("cSHAKE%d-XOF\n", securityStrength);
        printf("Input of %d bits:", inputLen);
        for(i=0; (i<(inputLen + 7) / 8) && (i<16); i++)
            printf(" %02x", (int)input[i]);
            if (inputLen > 16)
                printf(" ...");
        printf("\n");
        printf("Output of %d bits:", outputLen);
        for(i=0; i<(outputLen + 7) / 8; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
    }
    #endif
}

static void performTestcSHAKE(unsigned int securityStrength, unsigned char *checksum, unsigned int mode)
{
    unsigned int inputLen, outputLen, nameLen, customLen;

    /* Acumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    outputLen = 256;
    nameLen = 0;
    customLen = 0;
    for(inputLen=0; inputLen<=inputByteSize*8; inputLen++) {
        performTestcSHAKEOneInput(securityStrength, inputLen, outputLen, nameLen, customLen, &spongeChecksum, mode);
    }

    outputLen = 256;
    for(inputLen = 0; inputLen <= inputByteSize*8; inputLen = inputLen ? (inputLen + ((securityStrength == 128) ? 167*8 : 135*8)) : 1)
    for(nameLen = 0; nameLen <= nameByteSize*8; nameLen += 8)
    for(customLen = 0; customLen <= customizationByteSize*8; customLen += 7) {
        performTestcSHAKEOneInput(securityStrength, inputLen, outputLen, nameLen, customLen, &spongeChecksum, 0);
    }
    
    nameLen = 0;
    customLen = 0;
    for(outputLen = 1; outputLen <= 512; outputLen += (outputLen >= 16) ? ((securityStrength == 128) ? 167*8 : 135*8) : 1)
    for(inputLen = 0; inputLen <= inputByteSize*8; inputLen = inputLen ? (inputLen + ((securityStrength == 128) ? 167*8 : 135*8)) : 1) {
        performTestcSHAKEOneInput(securityStrength, inputLen, outputLen, nameLen, customLen, &spongeChecksum, 0);
    }
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE
    {
        unsigned int i;
        printf("cSHAKE%d\n", securityStrength);
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

static void performTestcSHAKEXOF(unsigned int securityStrength, unsigned char *checksum, unsigned int mode)
{
    unsigned int inputLen, outputLen;

    /* Acumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    for(outputLen = 128*8; outputLen <= outputByteSize*8; outputLen <<= 1)
    for(inputLen = 0; inputLen <= inputByteSize*8; inputLen = inputLen ? (inputLen << 2) : 1)
    {
        performTestcSHAKEXOFOneInput(securityStrength, inputLen, outputLen, &spongeChecksum, mode);
    }
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE
    {
        unsigned int i;
        printf("cSHAKE%d-XOF\n", securityStrength);
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestcSHAKE(unsigned int securityStrength, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    unsigned int mode;

    for(mode = 0; mode <= 2; ++mode) {
        printf("Testing cSHAKE%d %u...", securityStrength, mode);
        fflush(stdout);
        performTestcSHAKE(securityStrength, checksum, mode);
        assert(memcmp(expected, checksum, checksumByteSize) == 0);
        printf(" - OK.\n");
    }
}

void selfTestcSHAKEXOF(unsigned int securityStrength, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    unsigned int mode;

    for(mode = 0; mode <= 2; ++mode) {
        printf("Testing cSHAKE%d-XOF %u...", securityStrength, mode);
        fflush(stdout);
        performTestcSHAKEXOF(securityStrength, checksum, mode);
        assert(memcmp(expected, checksum, checksumByteSize) == 0);
        printf(" - OK.\n");
    }
}

#ifdef OUTPUT
void writeTestcSHAKEOne(FILE *f, unsigned int securityStrength)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestcSHAKE(securityStrength, checksum, 0);
    fprintf(f, "    selfTestcSHAKE(%d, \"", securityStrength);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
}

void writeTestcSHAKEXOFOne(FILE *f, unsigned int securityStrength)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestcSHAKEXOF(securityStrength, checksum, 0);
    fprintf(f, "    selfTestcSHAKEXOF(%d, \"", securityStrength);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
}

void writeTestcSHAKE(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestcSHAKEOne(f, 128);
    writeTestcSHAKEOne(f, 256);
    writeTestcSHAKEXOFOne(f, 128);
    writeTestcSHAKEXOFOne(f, 256);
    fclose(f);
}
#endif

static void performTestcSHAKE_NIST(void)
{
    unsigned char output[512/8];
    int result;

    {
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *N = "";
        BitSequence *S = "Email Signature";
        BitSequence *O = "\xC1\xC3\x69\x25\xB6\x40\x9A\x04\xF1\xB5\x04\xFC\xBC\xA9\xD8\x2B\x40\x17\x27\x7C\xB5\xED\x2B\x20\x65\xFC\x1D\x38\x14\xD5\xAA\xF5";

        result = cSHAKE128( data, 32, output, 256, N, strlen(N) * 8, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST cSHAKE128 test vector 1 OK\n");
        #endif
    }

    {
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *N = "";
        BitSequence *S = "Email Signature";
        BitSequence *O = "\xC5\x22\x1D\x50\xE4\xF8\x22\xD9\x6A\x2E\x88\x81\xA9\x61\x42\x0F\x29\x4B\x7B\x24\xFE\x3D\x20\x94\xBA\xED\x2C\x65\x24\xCC\x16\x6B";

        result = cSHAKE128( data, 1600, output, 256, N, strlen(N) * 8, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST cSHAKE128 test vector 2 OK\n");
        #endif
    }

    {
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *N = "";
        BitSequence *S = "Email Signature";
        BitSequence *O = "\xD0\x08\x82\x8E\x2B\x80\xAC\x9D\x22\x18\xFF\xEE\x1D\x07\x0C\x48\xB8\xE4\xC8\x7B\xFF\x32\xC9\x69\x9D\x5B\x68\x96\xEE\xE0\xED\xD1"
                         "\x64\x02\x0E\x2B\xE0\x56\x08\x58\xD9\xC0\x0C\x03\x7E\x34\xA9\x69\x37\xC5\x61\xA7\x4C\x41\x2B\xB4\xC7\x46\x46\x95\x27\x28\x1C\x8C";


        result = cSHAKE256( data, 32, output, 512, N, strlen(N) * 8, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST cSHAKE256 test vector 3 OK\n");
        #endif
    }

    {
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *N = "";
        BitSequence *S = "Email Signature";
        BitSequence *O = "\x07\xDC\x27\xB1\x1E\x51\xFB\xAC\x75\xBC\x7B\x3C\x1D\x98\x3E\x8B\x4B\x85\xFB\x1D\xEF\xAF\x21\x89\x12\xAC\x86\x43\x02\x73\x09\x17"
                         "\x27\xF4\x2B\x17\xED\x1D\xF6\x3E\x8E\xC1\x18\xF0\x4B\x23\x63\x3C\x1D\xFB\x15\x74\xC8\xFB\x55\xCB\x45\xDA\x8E\x25\xAF\xB0\x92\xBB";

        result = cSHAKE256( data, 1600, output, 512, N, strlen(N) * 8, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST cSHAKE256 test vector 4 OK\n");
        #endif
    }
}

static void performTestKMAC_NIST(void)
{
    unsigned char output[512/8];
    int result;

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *S = "";
        BitSequence *O = "\xE5\x78\x0B\x0D\x3E\xA6\xF7\xD3\xA4\x29\xC5\x70\x6A\xA4\x3A\x00\xFA\xDB\xD7\xD4\x96\x28\x83\x9E\x31\x87\x24\x3F\x45\x6E\xE1\x4E";

        result = KMAC128( K, 256, data, 32, output, 256, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC128 test vector 1 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\x3B\x1F\xBA\x96\x3C\xD8\xB0\xB5\x9E\x8C\x1A\x6D\x71\x88\x8B\x71\x43\x65\x1A\xF8\xBA\x0A\x70\x70\xC0\x97\x9E\x28\x11\x32\x4A\xA5";

        result = KMAC128( K, 256, data, 32, output, 256, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC128 test vector 2 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\x1F\x5B\x4E\x6C\xCA\x02\x20\x9E\x0D\xCB\x5C\xA6\x35\xB8\x9A\x15\xE2\x71\xEC\xC7\x60\x07\x1D\xFD\x80\x5F\xAA\x38\xF9\x72\x92\x30"; 

        result = KMAC128( K, 256, data, 1600, output, 256, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC128 test vector 3 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\x20\xC5\x70\xC3\x13\x46\xF7\x03\xC9\xAC\x36\xC6\x1C\x03\xCB\x64\xC3\x97\x0D\x0C\xFC\x78\x7E\x9B\x79\x59\x9D\x27\x3A\x68\xD2\xF7"
                         "\xF6\x9D\x4C\xC3\xDE\x9D\x10\x4A\x35\x16\x89\xF2\x7C\xF6\xF5\x95\x1F\x01\x03\xF3\x3F\x4F\x24\x87\x10\x24\xD9\xC2\x77\x73\xA8\xDD";


        result = KMAC256( K, 256, data, 32, output, 512, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC256 test vector 4 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *S = "";
        BitSequence *O = "\x75\x35\x8C\xF3\x9E\x41\x49\x4E\x94\x97\x07\x92\x7C\xEE\x0A\xF2\x0A\x3F\xF5\x53\x90\x4C\x86\xB0\x8F\x21\xCC\x41\x4B\xCF\xD6\x91"
                         "\x58\x9D\x27\xCF\x5E\x15\x36\x9C\xBB\xFF\x8B\x9A\x4C\x2E\xB1\x78\x00\x85\x5D\x02\x35\xFF\x63\x5D\xA8\x25\x33\xEC\x6B\x75\x9B\x69";


        result = KMAC256( K, 256, data, 1600, output, 512, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC256 test vector 5 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\xB5\x86\x18\xF7\x1F\x92\xE1\xD5\x6C\x1B\x8C\x55\xDD\xD7\xCD\x18\x8B\x97\xB4\xCA\x4D\x99\x83\x1E\xB2\x69\x9A\x83\x7D\xA2\xE4\xD9"
                         "\x70\xFB\xAC\xFD\xE5\x00\x33\xAE\xA5\x85\xF1\xA2\x70\x85\x10\xC3\x2D\x07\x88\x08\x01\xBD\x18\x28\x98\xFE\x47\x68\x76\xFC\x89\x65";


        result = KMAC256( K, 256, data, 1600, output, 512, S, strlen(S) * 8 );
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC256 test vector 6 OK\n");
        #endif
    }
}

static void performTestKMACXOF_NIST(void)
{
    unsigned char output[512/8];
    int result;
    KMAC_Instance km;

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *S = "";
        BitSequence *O = "\xCD\x83\x74\x0B\xBD\x92\xCC\xC8\xCF\x03\x2B\x14\x81\xA0\xF4\x46\x0E\x7C\xA9\xDD\x12\xB0\x8A\x0C\x40\x31\x17\x8B\xAC\xD6\xEC\x35";

        result = KMAC128_Initialize(&km, K, 256, 0, S, strlen(S) * 8);
        assert(result == 0);
        result = KMAC128_Update(&km, data, 32);
        assert(result == 0);
        result = KMAC128_Final(&km, output);
        assert(result == 0);
        result = KMAC128_Squeeze(&km, output, 256);
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC128 XOF test vector 1 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\x31\xA4\x45\x27\xB4\xED\x9F\x5C\x61\x01\xD1\x1D\xE6\xD2\x6F\x06\x20\xAA\x5C\x34\x1D\xEF\x41\x29\x96\x57\xFE\x9D\xF1\xA3\xB1\x6C";

        result = KMAC128_Initialize(&km, K, 256, 0, S, strlen(S) * 8);
        assert(result == 0);
        result = KMAC128_Update(&km, data, 32);
        assert(result == 0);
        result = KMAC128_Final(&km, output);
        assert(result == 0);
        result = KMAC128_Squeeze(&km, output, 256);
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC128 XOF test vector 2 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\x47\x02\x6C\x7C\xD7\x93\x08\x4A\xA0\x28\x3C\x25\x3E\xF6\x58\x49\x0C\x0D\xB6\x14\x38\xB8\x32\x6F\xE9\xBD\xDF\x28\x1B\x83\xAE\x0F";

        result = KMAC128_Initialize(&km, K, 256, 0, S, strlen(S) * 8);
        assert(result == 0);
        result = KMAC128_Update(&km, data, 1600);
        assert(result == 0);
        result = KMAC128_Final(&km, output);
        assert(result == 0);
        result = KMAC128_Squeeze(&km, output, 256);
        assert(result == 0);
        assert(memcmp(O, output, 256/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC128 XOF test vector 3 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\x17\x55\x13\x3F\x15\x34\x75\x2A\xAD\x07\x48\xF2\xC7\x06\xFB\x5C\x78\x45\x12\xCA\xB8\x35\xCD\x15\x67\x6B\x16\xC0\xC6\x64\x7F\xA9"
                         "\x6F\xAA\x7A\xF6\x34\xA0\xBF\x8F\xF6\xDF\x39\x37\x4F\xA0\x0F\xAD\x9A\x39\xE3\x22\xA7\xC9\x20\x65\xA6\x4E\xB1\xFB\x08\x01\xEB\x2B";

        result = KMAC256_Initialize(&km, K, 256, 0, S, strlen(S) * 8);
        assert(result == 0);
        result = KMAC256_Update(&km, data, 32);
        assert(result == 0);
        result = KMAC256_Final(&km, output);
        assert(result == 0);
        result = KMAC256_Squeeze(&km, output, 512);
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC256 XOF test vector 4 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *S = "";
        BitSequence *O = "\xFF\x7B\x17\x1F\x1E\x8A\x2B\x24\x68\x3E\xED\x37\x83\x0E\xE7\x97\x53\x8B\xA8\xDC\x56\x3F\x6D\xA1\xE6\x67\x39\x1A\x75\xED\xC0\x2C"
                         "\xA6\x33\x07\x9F\x81\xCE\x12\xA2\x5F\x45\x61\x5E\xC8\x99\x72\x03\x1D\x18\x33\x73\x31\xD2\x4C\xEB\x8F\x8C\xA8\xE6\xA1\x9F\xD9\x8B";

        result = KMAC256_Initialize(&km, K, 256, 0, S, strlen(S) * 8);
        assert(result == 0);
        result = KMAC256_Update(&km, data, 1600);
        assert(result == 0);
        result = KMAC256_Final(&km, output);
        assert(result == 0);
        result = KMAC256_Squeeze(&km, output, 512);
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC256 XOF test vector 5 OK\n");
        #endif
    }

    {
        BitSequence *K = "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F";
        BitSequence *data = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                            "\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F"
                            "\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5A\x5B\x5C\x5D\x5E\x5F"
                            "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B\x7C\x7D\x7E\x7F"
                            "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                            "\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF"
                            "\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7";
        BitSequence *S = "My Tagged Application";
        BitSequence *O = "\xD5\xBE\x73\x1C\x95\x4E\xD7\x73\x28\x46\xBB\x59\xDB\xE3\xA8\xE3\x0F\x83\xE7\x7A\x4B\xFF\x44\x59\xF2\xF1\xC2\xB4\xEC\xEB\xB8\xCE"
                         "\x67\xBA\x01\xC6\x2E\x8A\xB8\x57\x8D\x2D\x49\x9B\xD1\xBB\x27\x67\x68\x78\x11\x90\x02\x0A\x30\x6A\x97\xDE\x28\x1D\xCC\x30\x30\x5D";

        result = KMAC256_Initialize(&km, K, 256, 0, S, strlen(S) * 8);
        assert(result == 0);
        result = KMAC256_Update(&km, data, 1600);
        assert(result == 0);
        result = KMAC256_Final(&km, output);
        assert(result == 0);
        result = KMAC256_Squeeze(&km, output, 512);
        assert(result == 0);
        assert(memcmp(O, output, 512/8) == 0);
        #ifdef OUTPUT
        printf("NIST KMAC256 XOF test vector 6 OK\n");
        #endif
    }
}

void testSP800_185(void)
{
#ifndef KeccakP1600_excluded

    performTestcSHAKE_NIST();
    performTestKMAC_NIST();
    performTestKMACXOF_NIST();

#ifdef OUTPUT
    writeTestcSHAKE("cSHAKE.txt");
#endif

    selfTestcSHAKE(128, "\x2f\xa1\x0d\x72\x81\x29\x5c\x97\x43\x63\xcd\xe9\x3f\xcf\x06\xa1");
    selfTestcSHAKE(256, "\xf1\xdb\x1c\xb9\xc1\x9c\xad\xf4\x3a\x4f\x59\x3a\xd6\x70\x6e\xbb");

    selfTestcSHAKEXOF(128, "\x84\xd0\x58\xb3\x86\xc6\x64\xf0\xb5\x58\xa2\x6d\xaf\x05\x4a\xf8");
    selfTestcSHAKEXOF(256, "\x0b\xb6\x5b\x5c\xcb\x08\x05\x02\x9c\x79\x28\x8c\xf7\xcb\x9f\x03");
#endif
}
