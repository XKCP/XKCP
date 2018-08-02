/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakSpongeWidth1600.h"
#include "XoofffModes.h"

/* #define OUTPUT */
/* #define VERBOSE_WBC */

#if defined(XoodooSmallRAM)
    #define    XoodooSizeMultiplier    2
#else
    #define    XoodooSizeMultiplier    16
#endif

#define SnP_width_sponge        1600
#define SnP_width               (SnP_widthInBytes*8)
#define dataByteSize            (2*XoodooSizeMultiplier*SnP_widthInBytes+SnP_widthInBytes)
#define ADByteSize              (2*XoodooSizeMultiplier*SnP_widthInBytes+SnP_widthInBytes)
#define keyByteSize             (1*SnP_widthInBytes)
#define nonceByteSize           (2*SnP_widthInBytes)
#define WByteSize               (2*SnP_widthInBytes)
#define dataBitSize             (dataByteSize*8)
#define ADBitSize               (ADByteSize*8)
#define keyBitSize              (keyByteSize*8)
#define nonceBitSize            (nonceByteSize*8)
#define WBitSize                (WByteSize*8)
#define expansionLenWBCAE       16

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

#if defined(OUTPUT)
static void outputHex(const unsigned char *data, unsigned char length)
{
    unsigned int i;
    for(i=0; i<length; i++)
        printf("%02x ", (int)data[i]);
    printf("\n\n");
}
#endif

/* ------------------------------------------------------------------------- */

static void performTestXoofffWBC_OneInput(BitLength keyLen, BitLength dataLen, BitLength WLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize];
    BitSequence key[keyByteSize];
    BitSequence W[WByteSize];
    Xoofff_Instance xp;
    int result;
    unsigned int seed;

    randomize((unsigned char *)&xp, sizeof(Xoofff_Instance));
    randomize(key, keyByteSize);
    randomize(W, WByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);

    seed = keyLen + WLen + dataLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, (unsigned char)(0x43C1 - seed), 0xB9 + seed);
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(W, (WLen + 7) / 8, (unsigned char)(0x1727 - seed), 0x34 + seed);
    if (WLen & 7)
        W[WLen / 8] &= (1 << (WLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, (unsigned char)(0x4165 - seed), 0xA9 + seed);
    if (dataLen & 7)
        input[dataLen / 8] &= (1 << (dataLen & 7)) - 1;

    #ifdef VERBOSE_WBC
    printf( "keyLen %5u, WLen %5u, dataLen %5u (in bits)\n", (unsigned int)keyLen, (unsigned int)WLen, (unsigned int)dataLen);
    #endif

    result = XoofffWBC_Initialize(&xp, key, keyLen);
    assert(result == 0);

    result = XoofffWBC_Encipher(&xp, input, output, dataLen, W, WLen);
    assert(result == 0);

    result = XoofffWBC_Decipher(&xp, output, inputPrime, dataLen, W, WLen);
    assert(result == 0);
    assert(!memcmp(input,inputPrime,(dataLen + 7) / 8));

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (dataLen + 7) / 8);

    #ifdef VERBOSE_WBC
    {
        unsigned int i;
        unsigned int dataByteLen;

        printf("Key of %d bits:", (int)keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("Tweak of %d bits:", (int)WLen);
        WLen += 7;
        WLen /= 8;
        for(i=0; (i<WLen) && (i<16); i++)
            printf(" %02x", (int)W[i]);
        if (WLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", (int)dataLen);
        dataByteLen = (dataLen + 7) / 8;
        for(i=0; (i<dataByteLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (dataByteLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", (int)dataLen);
        for(i=0; (i<dataByteLen) && (i<8); i++)
            printf(" %02x", (int)output[i]);
        if (dataByteLen > 16)
            printf(" ...");
        if (i < (dataByteLen - 8))
            i = dataByteLen - 8;
        for( /* empty */; i<dataByteLen; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
        fflush(stdout);
    }
    #endif

}


static void performTestXoofffWBC(unsigned char *checksum)
{
    BitLength dataLen, WLen, keyLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width_sponge, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    dataLen = 128*8;
    WLen = 64*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestXoofffWBC_OneInput(keyLen, dataLen, WLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("d ");
    #endif
    WLen = 64*8;
    keyLen = 16*8;
    for(dataLen=0; dataLen<=dataBitSize; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+7)) {
        performTestXoofffWBC_OneInput(keyLen, dataLen, WLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("w ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    for(WLen=0; WLen<=WBitSize; WLen = (WLen < 2*SnP_width) ? (WLen+1) : (WLen+8)) {
        performTestXoofffWBC_OneInput(keyLen, dataLen, WLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE_WBC
    {
        unsigned int i;
        printf("Xoofff-WBC\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestXoofffWBC(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    #if defined(OUTPUT)
    printf("Testing Xoofff-WBC ");
    fflush(stdout);
    #endif
    performTestXoofffWBC(checksum);
    #ifdef OUTPUT
    fflush(stdout);
    #endif
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
    #if defined(OUTPUT)
    printf(" - OK.\n");
    #endif
}

#ifdef OUTPUT
void writeTestXoofffWBC_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Xoofff-WBC ");
    performTestXoofffWBC(checksum);
    fprintf(f, "    selfTestXoofffWBC(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestXoofffWBC(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);

    #if 0
    {
        BitLength n, nl, prevnl;

        prevnl = 0xFFFFFFFF;
        for (n = 0; n <= 8*64*1024; ++n )
        {
            nl = XoofffWBC_Split(n);
            if (nl != prevnl)
            {
                printf("n %6u, nl %6u, nr %6u", n, nl, n - nl);
                if (n >= 2*1536)
                    printf(", 2^x %6u", nl / 1536);
                printf("\n");
                prevnl = nl;
            }
        }
    }
    #endif

    writeTestXoofffWBC_One(f);
    fclose(f);
}
#endif

/* ------------------------------------------------------------------------- */

static void performTestXoofffWBC_AE_OneInput(BitLength keyLen, BitLength dataLen, BitLength ADLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize+expansionLenWBCAE];
    BitSequence key[keyByteSize];
    BitSequence AD[ADByteSize];
    Xoofff_Instance xp;
    unsigned int seed;
    int result;

    randomize(key, keyByteSize);
    randomize(AD, ADByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);

    seed = keyLen + ADLen + dataLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, (unsigned char)(0x91FC - seed), 0x5A + seed);
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(AD, (ADLen + 7) / 8, (unsigned char)(0x8181 - seed), 0x9B + seed);
    if (ADLen & 7)
        AD[ADLen / 8] &= (1 << (ADLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, (unsigned char)(0x1BF0 - seed), 0xC6 + seed);
    if (dataLen & 7)
        input[dataLen / 8] &= (1 << (dataLen & 7)) - 1;

    #ifdef VERBOSE_WBC_AE
    printf( "keyLen %5u, ADLen %5u, dataLen %5u (in bits)\n", (unsigned int)keyLen, (unsigned int)ADLen, (unsigned int)dataLen);
    #endif

    result = XoofffWBCAE_Initialize(&xp, key, keyLen);
    assert(result == 0);

    result = XoofffWBCAE_Encipher(&xp, input, output, dataLen, AD, ADLen);
    assert(result == 0);
    result = XoofffWBCAE_Decipher(&xp, output, inputPrime, dataLen, AD, ADLen);
    assert(result == 0);
    #ifdef VERBOSE_WBC
    if (memcmp(input,inputPrime,(dataLen + 7) / 8) != 0)
    {
        size_t i;
        printf("input  ");
        for(i=0; i<(dataLen + 7) / 8; i++)
            printf(" %02x", (int)input[i]);
        printf("\ninputP ");
        for(i=0; i<(dataLen + 7) / 8; i++)
            printf(" %02x", (int)inputPrime[i]);
        printf("\noutput ");
        for(i=0; i<(dataLen + 7) / 8; i++)
            printf(" %02x", (int)output[i]);
        printf("\n");
    }
    #endif
    assert(!memcmp(input,inputPrime,(dataLen + 7) / 8));

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (dataLen + 8 * expansionLenWBCAE + 7) / 8);

    #ifdef VERBOSE_WBC_AE
    {
        unsigned int i;
        unsigned int dataByteLen;
        BitLength outputLen = dataLen + 8 * expansionLenWBCAE;
        unsigned int outputByteLen;

        printf("Key of %d bits:", (int)keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("AD of %d bits:", (int)ADLen);
        ADLen += 7;
        ADLen /= 8;
        for(i=0; (i<ADLen) && (i<16); i++)
            printf(" %02x", (int)AD[i]);
        if (ADLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", (int)dataLen);
        dataByteLen = (dataLen + 7) / 8;
        for(i=0; (i<dataByteLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (dataByteLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", (int)outputLen);
        outputByteLen = (outputLen + 7) / 8;
        for(i=0; (i<outputByteLen) && (i<8); i++)
            printf(" %02x", (int)output[i]);
        if (outputByteLen > 16)
            printf(" ...");
        if (i < (outputByteLen - 8))
            i = outputByteLen - 8;
        for( /* empty */; i<outputByteLen; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
        fflush(stdout);
    }
    #endif

}


static void performTestXoofffWBC_AE(unsigned char *checksum)
{
    BitLength dataLen, ADLen, keyLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width_sponge, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    dataLen = 128*8;
    ADLen = 64*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestXoofffWBC_AE_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("d ");
    #endif
    ADLen = 64*8;
    keyLen = 16*8;
    for(dataLen=0; dataLen<=dataBitSize-8*expansionLenWBCAE; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+7)) {
        performTestXoofffWBC_AE_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("a ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    for(ADLen=0; ADLen<=ADBitSize; ADLen = (ADLen < 2*SnP_width) ? (ADLen+1) : (ADLen+8)) {
        performTestXoofffWBC_AE_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE_WBC_AE
    {
        unsigned int i;
        printf("Xoofff-WBC-AE\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestXoofffWBC_AE(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    #if defined(OUTPUT)
    printf("Testing Xoofff-WBC-AE ");
    fflush(stdout);
    #endif
    performTestXoofffWBC_AE(checksum);
    #ifdef OUTPUT
    fflush(stdout);
    #endif
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
    #if defined(OUTPUT)
    printf(" - OK.\n");
    #endif
}

#ifdef OUTPUT
void writeTestXoofffWBC_AE_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Xoofff-WBC-AE ");
    performTestXoofffWBC_AE(checksum);
    fprintf(f, "    selfTestXoofffWBC_AE(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestXoofffWBC_AE(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);

    #if 0
    {
        BitLength n, nl, prevnl;
        prevnl = 0xFFFFFFFF;
        for (n = 0; n <= 8*64*1024; ++n )
        {
            nl = XoofffWBC_AE_Split(n);
            if (nl != prevnl)
            {
                printf("n %6u, nl %6u, nr %6u", n, nl, n - nl);
                if (n >= 2*1536)
                    printf(", 2^x %6u", nl / 1536);
                printf("\n");
                prevnl = nl;
            }
        }
    }
    #endif

    writeTestXoofffWBC_AE_One(f);
    fclose(f);
}
#endif

/* ------------------------------------------------------------------------- */
void testXoofffModes(void)
{
#ifndef KeccakP1600_excluded
#ifdef OUTPUT
/*    printXooTestVectors(); */
    writeTestXoofffWBC("Xoofff-WBC.txt");
    writeTestXoofffWBC_AE("Xoofff-WBC-AE.txt");
#endif

#if defined(XoodooSmallRAM)
    selfTestXoofffWBC((const unsigned char *)"\x04\x1f\xb7\x4f\x3a\xba\xae\x79\xb0\x45\x69\x14\x7c\x72\xc3\xce");
    selfTestXoofffWBC_AE((const unsigned char *)"\xb2\x74\x05\x45\x55\xff\x03\x90\xda\xa7\x82\x89\x5d\x60\xb3\x0a");
#else
    selfTestXoofffWBC((const unsigned char *)"\x96\x09\x5c\xeb\x82\xa4\x7c\x94\xfc\x90\x42\xd8\xb0\xe3\xc8\xe1");
    selfTestXoofffWBC_AE((const unsigned char *)"\x45\x56\x9c\x96\x78\x20\x4b\xd4\xfb\xc0\xfe\xcb\x59\x6c\x85\x56");
#endif
#endif
}
