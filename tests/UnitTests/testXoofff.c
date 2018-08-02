/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakSpongeWidth1600.h"
#include "Xoofff.h"

/* #define OUTPUT */
/* #define VERBOSE */

#if defined(XoodooSmallRAM)
    #define    XoodooSizeMultiplier    2
#else
    #define    XoodooSizeMultiplier    16
#endif

#define SnP_width_sponge        1600
#define SnP_width               (SnP_widthInBytes*8)
#define inputByteSize           (2*XoodooSizeMultiplier*SnP_widthInBytes+SnP_widthInBytes)
#define outputByteSize          (2*XoodooSizeMultiplier*SnP_widthInBytes+SnP_widthInBytes)
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
void assert(int condition);
uint8_t random8( void );
#define rand    random8
#else
#include <assert.h>
#endif

static void randomize( unsigned char* data, unsigned int length)
{
    #if !defined(EMBEDDED)
    srand((unsigned int)time(0));
    #endif
    while (length--)
    {
        *data++ = (unsigned char)rand();
    }
}

static void generateSimpleRawMaterial(unsigned char* data, unsigned int length, unsigned char seed1, unsigned int seed2)
{
    unsigned int i;

    for(i=0; i<length; i++) {
        unsigned char iRolled;
        unsigned char byte;
        seed2 = seed2 % 8;
        iRolled = ((unsigned char)i << seed2) | ((unsigned char)i >> (8-seed2));
        byte = (unsigned char)(seed1 + 161*length - iRolled + i);
        data[i] = byte;
    }
}

static void performTestXoofffOneInput(BitLength keyLen, BitLength inputLen, BitLength outputLen, int flags, KeccakWidth1600_SpongeInstance *pSpongeChecksum, unsigned int mode)
{
    BitSequence input[inputByteSize];
    BitSequence output[outputByteSize];
    BitSequence key[keyByteSize];
    Xoofff_Instance xp;
    int result;
    unsigned int i;
    unsigned int seed;

    seed = keyLen + outputLen + inputLen;
    seed ^= seed >> 3;
    randomize((unsigned char *)&xp, sizeof(Xoofff_Instance));
    randomize(key, keyByteSize);
    randomize(input, inputByteSize);
    randomize(output, outputByteSize);
    generateSimpleRawMaterial(input, (inputLen + 7) / 8, (unsigned char)(seed + 0x13AD), 0x75 - seed );
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, (unsigned char)(seed + 0x2749), 0x31 - seed );
    if (inputLen & 7)
        input[inputLen / 8] &= (1 << (inputLen & 7)) - 1;
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;

    #ifdef VERBOSE
    printf( "keyLen %5u, outputLen %5u, inputLen %5u (in bits)\n", (unsigned int)keyLen, (unsigned int)outputLen, (unsigned int)inputLen);
    #endif

    result = Xoofff_MaskDerivation(&xp, key, keyLen);
    assert(result == 0);

    if (mode == 0)
    {
        /* Input/Output full size in one call */
        result = Xoofff(&xp, input, inputLen, output, outputLen, flags);
        assert(result == 0);
    }
    else if (mode == 1)
    {
        /* Input/Output one byte per call */
        for (i = 0; i < inputLen / 8; ++i ) {
            result = Xoofff_Compress(&xp, input + i, 8, flags);
            assert(result == 0);
        }
        /* Last bits and final flag */
        result = Xoofff_Compress(&xp, input + i, inputLen & 7, flags | Xoofff_FlagLastPart);
        assert(result == 0);
        for (i = 0; i < outputLen / 8; ++i ) {
            result =  Xoofff_Expand(&xp, output + i, 8, flags);
            assert(result == 0);
        }
        result =  Xoofff_Expand(&xp, output + i, outputLen & 7, flags | Xoofff_FlagLastPart);
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
            int fl = flags | ((len == ilen) ? Xoofff_FlagLastPart : 0);
            if ((fl & Xoofff_FlagLastPart) == 0)
                len &= ~7;
            result = Xoofff_Compress(&xp, pInput, len, fl);
            assert(result == 0);
            pInput += len / 8;
            ilen -= len;
        } while (ilen);
        while (olen)
        {
            unsigned int len = ((rand() << 15) ^ rand()) % (olen + 1);
            int fl = flags | ((len == olen) ? Xoofff_FlagLastPart : 0);
            if ((fl & Xoofff_FlagLastPart) == 0)
                len &= ~7;
            result = Xoofff_Expand(&xp, pOutput, len, fl);
            assert(result == 0);
            pOutput += len / 8;
            olen -= len;
        }
    }

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (outputLen + 7) / 8);

    #ifdef VERBOSE
    {
        unsigned int i;

        printf("Key of %d bits:", (int)keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", (int)inputLen);
        inputLen += 7;
        inputLen /= 8;
        for(i=0; (i<inputLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (inputLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", (int)outputLen);
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

static void performTestXoofff(unsigned char *checksum, unsigned int mode)
{
    BitLength inputLen, outputLen, keyLen;
    int flags;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width_sponge, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    outputLen = 128*8;
    inputLen = 64*8;
    flags = 0;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestXoofffOneInput(keyLen, inputLen, outputLen, flags, &spongeChecksum, mode);
    }
    
    #ifdef OUTPUT
    printf("i ");
    #endif
    outputLen = 128*8;
    keyLen = 16*8;
    for(inputLen=0; inputLen<=inputBitSize; inputLen = (inputLen < 2*SnP_width) ? (inputLen+1) : (inputLen+8)) {
        performTestXoofffOneInput(keyLen, inputLen, outputLen, flags, &spongeChecksum, mode);
    }
    
    #ifdef OUTPUT
    printf("o ");
    #endif
    inputLen = 64*8;
    keyLen = 16*8;
    for(outputLen=0; outputLen<=outputBitSize; outputLen = (outputLen < 2*SnP_width) ? (outputLen+1) : (outputLen+8)) {
        performTestXoofffOneInput(keyLen, inputLen, outputLen, flags, &spongeChecksum, mode);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE
    {
        unsigned int i;
        printf("Xoofff\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestXoofff(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];
    unsigned int mode;

    for(mode = 0; mode <= 2; ++mode) {
        #ifdef OUTPUT
        printf("Testing Xoofff %u ", mode);
        fflush(stdout);
        #endif
        performTestXoofff(checksum, mode);
        #ifdef OUTPUT
        fflush(stdout);
        #endif
        assert(memcmp(expected, checksum, checksumByteSize) == 0);
        #ifdef OUTPUT
        printf(" - OK.\n");
        #endif
    }
}

#ifdef OUTPUT
void writeTestXoofffOne(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Xoofff ");
    performTestXoofff(checksum, 0);
    fprintf(f, "    selfTestXoofff(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestXoofff(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestXoofffOne(f);
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

void printXoofffTestVectors()
{
    unsigned char *M, *C;
    unsigned char output[10032];
    unsigned int i, j, l;

    printf("Xoofff(M=empty, C=empty, 32 output bytes):\n");
    Xoofff(0, 0, output, 32, 0, 0);
    outputHex(output, 32);
    printf("Xoofff(M=empty, C=empty, 64 output bytes):\n");
    Xoofff(0, 0, output, 64, 0, 0);
    outputHex(output, 64);
    printf("Xoofff(M=empty, C=empty, 10032 output bytes), last 32 bytes:\n");
    Xoofff(0, 0, output, 10032, 0, 0);
    outputHex(output+10000, 32);
    for(l=1, i=0; i<7; i++, l=l*17) {
        M = malloc(l);
        for(j=0; j<l; j++)
            M[j] = j%251;
        printf("Xoofff(M=pattern 0x00 to 0xFA for 17^%d bytes, C=empty, 32 output bytes):\n", i);
        Xoofff(M, l, output, 32, 0, 0);
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
        printf("Xoofff(M=%d times byte 0xFF, C=pattern 0x00 to 0xFA for 41^%d bytes, 32 output bytes):\n", ll, i);
        Xoofff(M, ll, output, 32, C, l);
        outputHex(output, 32);
        free(M);
        free(C);
    }
}
#endif

void testXoofff(void)
{
#ifndef KeccakP1600_excluded
#ifdef OUTPUT
/*    printXoofffTestVectors(); */
    writeTestXoofff("Xoofff.txt");
#endif
#if defined(XoodooSmallRAM)
    selfTestXoofff((const unsigned char *)"\xb7\x58\xfa\x31\x88\xd2\xd6\x57\x5b\x39\x09\xeb\x31\x57\x6b\x5a");
#else
    selfTestXoofff((const unsigned char *)"\xca\x8e\x19\x14\xb6\xe2\x8f\xeb\x5f\xcb\xd2\x7d\xc2\x39\x2b\xd5");
#endif
#endif
}
