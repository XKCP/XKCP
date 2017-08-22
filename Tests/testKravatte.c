/*
Implementation by the Farfalle and Kravatte designers, namely, Guido Bertoni,
Joan Daemen, Seth Hoffert, Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakSponge.h"
#include "Kravatte.h"

#if !defined(EMBEDDED)
#define OUTPUT
/* #define VERBOSE */
#else
#undef OUTPUT
#undef VERBOSE
#endif


#define SnP_width               1600
#define inputByteSize           (16*SnP_widthInBytes)
#define outputByteSize          (16*SnP_widthInBytes)
#define keyByteSize             (1*SnP_widthInBytes)
#define inputBitSize            (inputByteSize*8)
#define outputBitSize           (outputByteSize*8)
#define keyBitSize              (keyByteSize*8)
#define checksumByteSize        16

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <stdlib.h>
#include <time.h>

#if defined(EMBEDDED)
static void assert(int condition)
{
    if (!condition)
    {
        for ( ; ; ) ;
    }
}
unsigned int random( void );
#define rand    random
#define srand(a)    
#else
#include <assert.h>
#endif

static void randomize( unsigned char* data, unsigned int length)
{
	#if !defined(EMBEDDED)
    srand((unsigned int)time(0));
    while (length--)
    {
        *data++ = rand();
    }
	#endif
}

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

static void performTestKravatteOneInput(BitLength keyLen, BitLength inputLen, BitLength outputLen, int flags, KeccakWidth1600_SpongeInstance *pSpongeChecksum, unsigned int mode)
{
    BitSequence input[inputByteSize];
    BitSequence output[outputByteSize];
    BitSequence key[keyByteSize];
    Kravatte_Instance kv;
    int result;
    unsigned int i;
    unsigned int seed;

    seed = keyLen + outputLen + inputLen;
    seed ^= seed >> 3;
    randomize((unsigned char *)&kv, sizeof(Kravatte_Instance));
    randomize(key, keyByteSize);
    randomize(input, inputByteSize);
    randomize(output, outputByteSize);
    generateSimpleRawMaterial(input, (inputLen + 7) / 8, seed + 0x13AD, 0x75 - seed );
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, seed + 0x2749, 0x31 - seed );
    if (inputLen & 7)
        input[inputLen / 8] &= (1 << (inputLen & 7)) - 1;
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;

    #ifdef VERBOSE
    printf( "keyLen %5u, outputLen %5u, inputLen %5u (in bits)\n", keyLen, outputLen, inputLen);
    #endif

    result = Kravatte_MaskDerivation(&kv, key, keyLen);
    assert(result == 0);

    if (mode == 0)
    {
        /* Input/Output full size in one call */
        result = Kravatte(&kv, input, inputLen, output, outputLen, flags);
        assert(result == 0);
    }
    else if (mode == 1)
    {
        /* Input/Output one byte per call */
        for (i = 0; i < inputLen / 8; ++i ) {
            result = Kra(&kv, input + i, 8, flags);
            assert(result == 0);
        }
        /* Last bits and final flag */
        result = Kra(&kv, input + i, inputLen & 7, flags | KRAVATTE_FLAG_LAST_PART);
        assert(result == 0);
        for (i = 0; i < outputLen / 8; ++i ) {
            result =  Vatte(&kv, output + i, 8, flags);
            assert(result == 0);
        }
        result =  Vatte(&kv, output + i, outputLen & 7, flags | KRAVATTE_FLAG_LAST_PART);
        assert(result == 0);
    }
    else if (mode == 2)
    {
        /* Input/Output random number of bytes per call */
        BitSequence *pInput = input;
        BitSequence *pOutput = output;
        BitLength ilen = inputLen, olen = outputLen;
        
        do
        {
            unsigned int len = ((rand() << 15) ^ rand()) % (ilen + 1);
            int fl = flags | ((len == ilen) ? KRAVATTE_FLAG_LAST_PART : 0);
            if ((fl & KRAVATTE_FLAG_LAST_PART) == 0)
                len &= ~7;
            result = Kra(&kv, pInput, len, fl);
            assert(result == 0);
            pInput += len / 8;
            ilen -= len;
        } while (ilen);
        while (olen)
        {
            unsigned int len = ((rand() << 15) ^ rand()) % (olen + 1);
            int fl = flags | ((len == olen) ? KRAVATTE_FLAG_LAST_PART : 0);
            if ((fl & KRAVATTE_FLAG_LAST_PART) == 0)
                len &= ~7;
            result = Vatte(&kv, pOutput, len, fl);
            assert(result == 0);
            pOutput += len / 8;
            olen -= len;
        }
    }

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (outputLen + 7) / 8);

    #ifdef VERBOSE
    {
        unsigned int i;

        printf("Key of %d bits:", keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", inputLen);
        inputLen += 7;
        inputLen /= 8;
        for(i=0; (i<inputLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (inputLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", outputLen);
        outputLen += 7;
        outputLen /= 8;
        for(i=0; (i<outputLen) && (i<8); i++)
            printf(" %02x", (int)output[i]);
        if (outputLen > 16)
            printf(" ...");
        if (i < (outputLen - 8))
            i = outputLen - 8;
        for( /* empty */; i<outputLen; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
        fflush(stdout);
    }
    #endif
}


static void performTestKravatte(unsigned char *checksum, unsigned int mode)
{
    BitLength inputLen, outputLen, keyLen;
    int flags;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    outputLen = 128*8;
    inputLen = 64*8;
    flags = 0;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestKravatteOneInput(keyLen, inputLen, outputLen, flags, &spongeChecksum, mode);
    }
    
    #ifdef OUTPUT
    printf("i ");
    #endif
    outputLen = 128*8;
    keyLen = 16*8;
    for(inputLen=0; inputLen<=inputBitSize; inputLen = (inputLen < 2*SnP_width) ? (inputLen+1) : (inputLen+8)) {
        performTestKravatteOneInput(keyLen, inputLen, outputLen, flags, &spongeChecksum, mode);
    }
    
    #ifdef OUTPUT
    printf("o ");
    #endif
    inputLen = 64*8;
    keyLen = 16*8;
    for(outputLen=0; outputLen<=outputBitSize; outputLen = (outputLen < 2*SnP_width) ? (outputLen+1) : (outputLen+8)) {
        performTestKravatteOneInput(keyLen, inputLen, outputLen, flags, &spongeChecksum, mode);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE
    {
        unsigned int i;
        printf("Kravatte\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestKravatte(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    unsigned int mode;

    for(mode = 0; mode <= 2; ++mode) {
#ifdef OUTPUT
        printf("Testing Kravatte %u ", mode);
        fflush(stdout);
#endif
        performTestKravatte(checksum, mode);
        assert(memcmp(expected, checksum, checksumByteSize) == 0);
#ifdef OUTPUT
        printf(" - OK.\n");
#endif
    }
}

#ifdef OUTPUT
void writeTestKravatteOne(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Kravatte ");
    performTestKravatte(checksum, 0);
    fprintf(f, "    selfTestKravatte(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestKravatte(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestKravatteOne(f);
    fclose(f);
}
#endif

#if 0
static void outputHex(const unsigned char *data, unsigned char length)
{
    unsigned int i;
    for(i=0; i<length; i++)
        printf("%02x ", (int)data[i]);
    printf("\n\n");
}

void printKravatteTestVectors()
{
    unsigned char *M, *C;
    unsigned char output[10032];
    unsigned int i, j, l;

    printf("Kravatte(M=empty, C=empty, 32 output bytes):\n");
    Kravatte(0, 0, output, 32, 0, 0);
    outputHex(output, 32);
    printf("Kravatte(M=empty, C=empty, 64 output bytes):\n");
    Kravatte(0, 0, output, 64, 0, 0);
    outputHex(output, 64);
    printf("Kravatte(M=empty, C=empty, 10032 output bytes), last 32 bytes:\n");
    Kravatte(0, 0, output, 10032, 0, 0);
    outputHex(output+10000, 32);
    for(l=1, i=0; i<7; i++, l=l*17) {
        M = malloc(l);
        for(j=0; j<l; j++)
            M[j] = j%251;
        printf("Kravatte(M=pattern 0x00 to 0xFA for 17^%d bytes, C=empty, 32 output bytes):\n", i);
        Kravatte(M, l, output, 32, 0, 0);
        outputHex(output, 32);
        free(M);
    }
    for(l=1, i=0; i<4; i++, l=l*41) {
        unsigned int ll = (1 << i)-1;
        M = malloc(ll);
        memset(M, 0xFF, ll);
        C = malloc(l);
        for(j=0; j<l; j++)
            C[j] = j%251;
        printf("Kravatte(M=%d times byte 0xFF, C=pattern 0x00 to 0xFA for 41^%d bytes, 32 output bytes):\n", ll, i);
        Kravatte(M, ll, output, 32, C, l);
        outputHex(output, 32);
        free(M);
        free(C);
    }
}
#endif

void testKravatte(void)
{

#ifndef KeccakP1600_excluded
#ifdef OUTPUT
//    printKravatteTestVectors();
    writeTestKravatte("Kravatte.txt");
#endif
    selfTestKravatte("\x6e\x08\x99\x96\x16\x4a\xe5\x75\x39\x74\xe1\xc6\x59\x79\x37\xa0");
#endif
}
