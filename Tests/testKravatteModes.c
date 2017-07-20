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
#include "KravatteModes.h"

#if !defined(EMBEDDED)
#define OUTPUT
/* #define VERBOSE_SIV */
/* #define VERBOSE_SAE */
/* #define VERBOSE_WBC */
/* #define VERBOSE_WBC_AE */
#else
#undef OUTPUT
#undef VERBOSE_SIV
#undef VERBOSE_SAE
#undef VERBOSE_WBC
#undef VERBOSE_WBC_AE
#endif

#define SnP_width               1600
#define dataByteSize            (16*SnP_widthInBytes)
#define ADByteSize              (16*SnP_widthInBytes)
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

static void performTestKravatte_SIV_OneInput(BitLength keyLen, BitLength dataLen, BitLength ADLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize];
    BitSequence AD[ADByteSize];
    BitSequence key[keyByteSize];
    unsigned char tag[Kravatte_SIV_TagLength];
    Kravatte_Instance kv;
    int result;
    unsigned int seed;

    randomize((unsigned char *)&kv, sizeof(Kravatte_Instance));
    randomize(key, keyByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);
    randomize(AD, ADByteSize);
    randomize(tag, Kravatte_SIV_TagLength);

    seed = keyLen + dataLen + ADLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, 0x4321 - seed, 0x89 + seed);
    if (keyLen & 7)
        key[(keyLen + 7) / 8 - 1] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, 0x6523 - seed, 0x43 + seed);
    if (dataLen & 7)
        input[(dataLen + 7) / 8 - 1] &= (1 << (dataLen & 7)) - 1;
    generateSimpleRawMaterial(AD, (ADLen + 7) / 8, 0x1A29 - seed, 0xC3 + seed);
    if (ADLen & 7)
        AD[(ADLen + 7) / 8 - 1] &= (1 << (ADLen & 7)) - 1;

    #ifdef VERBOSE_SIV
    printf( "keyLen %5u, dataLen %5u, ADLen %5u (in bits)\n", keyLen, dataLen, ADLen);
    #endif

    result = Kravatte_SIV_MaskDerivation(&kv, key, keyLen);
    assert(result == 0);

    result = Kravatte_SIV_Wrap(&kv, input, output, dataLen, AD, ADLen, tag);
    assert(result == 0);

    result = Kravatte_SIV_Unwrap(&kv, output, inputPrime, dataLen, AD, ADLen, tag);
    assert(result == 0);
    assert(!memcmp(input,inputPrime,(dataLen + 7) / 8));

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (dataLen + 7) / 8);
    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, tag, sizeof(tag));

    #ifdef VERBOSE_SIV
    {
        unsigned int i;
        BitLength len;

        printf("Key of %d bits:", keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", dataLen);
        len = (dataLen + 7) /8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (dataLen > 16)
            printf(" ...");
        printf("\n");

        printf("AD of %d bits:", ADLen);
        ADLen += 7;
        ADLen /= 8;
        for(i=0; (i<ADLen) && (i<16); i++)
            printf(" %02x", (int)AD[i]);
        if (ADLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", dataLen);
        len = (dataLen + 7) /8;
        for(i=0; (i<len) && (i<8); i++)
            printf(" %02x", (int)output[i]);
        if (len > 16)
            printf(" ...");
        if (i < (len - 8))
            i = len - 8;
        for( /* empty */; i<len; i++)
            printf(" %02x", (int)output[i]);
        printf("\n");

        printf("Tag of %d bytes:", Kravatte_SIV_TagLength);
        for(i=0; i<Kravatte_SIV_TagLength; i++)
            printf(" %02x", (int)tag[i]);
        printf("\n\n");
        fflush(stdout);
    }
    #endif

}


static void performTestKravatte_SIV(unsigned char *checksum)
{
    BitLength dataLen, ADLen, keyLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    dataLen = 128*8;
    ADLen = 64*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestKravatte_SIV_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("d ");
    #endif
    ADLen = 64*8;
    keyLen = 16*8;
    for(dataLen=0; dataLen<=dataBitSize; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+8)) {
        performTestKravatte_SIV_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("a ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    for(ADLen=0; ADLen<=ADBitSize; ADLen = (ADLen < 2*SnP_width) ? (ADLen+1) : (ADLen+8)) {
        performTestKravatte_SIV_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE_SIV
    {
        unsigned int i;
        printf("Kravatte-SIV\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestKravatte_SIV(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

#if defined(OUTPUT)
    printf("Testing Kravatte-SIV ");
    fflush(stdout);
#endif
    performTestKravatte_SIV(checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
#if defined(OUTPUT)
    printf(" - OK.\n");
#endif
}

#ifdef OUTPUT
void writeTestKravatte_SIV_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Kravatte-SIV ");
    performTestKravatte_SIV(checksum);
    fprintf(f, "    selfTestKravatte_SIV(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestKravatte_SIV(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestKravatte_SIV_One(f);
    fclose(f);
}
#endif

/* ------------------------------------------------------------------------- */

static void performTestKravatte_SAE_OneInput(BitLength keyLen, BitLength nonceLen, BitLength dataLen, BitLength ADLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize];
    BitSequence AD[ADByteSize];
    BitSequence key[keyByteSize];
    BitSequence nonce[nonceByteSize];
    unsigned char tag[Kravatte_SAE_TagLength];
    unsigned char tagInit[Kravatte_SAE_TagLength];
    Kravatte_Instance kvEnc;
    Kravatte_Instance kvDec;
    int result;
    unsigned int seed;
    unsigned int session;

    randomize((unsigned char *)&kvEnc, sizeof(Kravatte_Instance));
    randomize((unsigned char *)&kvDec, sizeof(Kravatte_Instance));
    randomize(key, keyByteSize);
    randomize(nonce, nonceByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);
    randomize(AD, ADByteSize);
    randomize(tag, Kravatte_SAE_TagLength);

    seed = keyLen + nonceLen + dataLen + ADLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, 0x4371 - seed, 0x59 + seed);
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(nonce, (nonceLen + 7) / 8, 0x1327 - seed, 0x84 + seed);
    if (nonceLen & 7)
        nonce[nonceLen / 8] &= (1 << (nonceLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, 0x4861 - seed, 0xb1 + seed);
    if (dataLen & 7)
        input[dataLen / 8] &= (1 << (dataLen & 7)) - 1;
    generateSimpleRawMaterial(AD, (ADLen + 7) / 8, 0x243B - seed, 0x17 + seed);
    if (ADLen & 7)
        AD[ADLen / 8] &= (1 << (ADLen & 7)) - 1;

    #ifdef VERBOSE_SAE
    printf( "keyLen %5u, nonceLen %5u, dataLen %5u, ADLen %5u (in bits)\n", keyLen, nonceLen, dataLen, ADLen);
    #endif

    result = Kravatte_SAE_Initialize(&kvEnc, key, keyLen, nonce, nonceLen, tagInit);
    assert(result == 0);
    result = Kravatte_SAE_Initialize(&kvDec, key, keyLen, nonce, nonceLen, tag);
    assert(result == 0);
    assert(!memcmp(tag, tagInit, Kravatte_SAE_TagLength));
    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, tagInit, Kravatte_SAE_TagLength);

    #ifdef VERBOSE_SAE
    {
        unsigned int i;
        unsigned int len;

        printf("Key of %d bits:", keyLen);
        len = (keyLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");

        printf("Nonce of %d bits:", nonceLen);
        len = (nonceLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)nonce[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", dataLen);
        len = (dataLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");

        printf("AD of %d bits:", ADLen);
        len = (ADLen + 7) / 8;
        for(i=0; (i<len) && (i<16); i++)
            printf(" %02x", (int)AD[i]);
        if (len > 16)
            printf(" ...");
        printf("\n");
    }
    #endif

    for (session = 3; session != 0; --session) {
        result = Kravatte_SAE_Wrap(&kvEnc, input, output, dataLen, AD, ADLen, tag);
        assert(result == 0);
        result = Kravatte_SAE_Unwrap(&kvDec, output, inputPrime, dataLen, AD, ADLen, tag);
        assert(result == 0);
        assert(!memcmp(input,inputPrime,(dataLen + 7) / 8));
        KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (dataLen + 7) / 8);
        KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, tag, Kravatte_SAE_TagLength);
        #ifdef VERBOSE_SAE
        {
            unsigned int i;
            unsigned int len;

            printf("Output of %d bits:", dataLen);
            len = (dataLen + 7) / 8;
            for(i=0; (i<len) && (i<8); i++)
                printf(" %02x", (int)output[i]);
            if (len > 16)
                printf(" ...");
            if (i < (len - 8))
                i = len - 8;
            for( /* empty */; i<len; i++)
                printf(" %02x", (int)output[i]);
            printf("\n");

            printf("Tag of %d bytes:", Kravatte_SAE_TagLength);
            for(i=0; i<Kravatte_SAE_TagLength; i++)
                printf(" %02x", (int)tag[i]);
            printf("\n");
            fflush(stdout);
            if (session == 1)
                printf("\n");
        }
        #endif
    }

}


static void performTestKravatte_SAE(unsigned char *checksum)
{
    BitLength dataLen, ADLen, keyLen, nonceLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    dataLen = 128*8;
    ADLen = 64*8;
    nonceLen = 24*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestKravatte_SAE_OneInput(keyLen, nonceLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("n ");
    #endif
    dataLen = 128*8;
    ADLen = 64*8;
    keyLen = 16*8;
    for(nonceLen=0; nonceLen<=nonceBitSize; nonceLen = (nonceLen < 2*SnP_width) ? (nonceLen+1) : (nonceLen+8)) {
        performTestKravatte_SAE_OneInput(keyLen, nonceLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("d ");
    #endif
    ADLen = 64*8;
    keyLen = 16*8;
    nonceLen = 24*8;
    for(dataLen=0; dataLen<=dataBitSize; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+8)) {
        performTestKravatte_SAE_OneInput(keyLen, nonceLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("a ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    nonceLen = 24*8;
    for(ADLen=0; ADLen<=ADBitSize; ADLen = (ADLen < 2*SnP_width) ? (ADLen+1) : (ADLen+8)) {
        performTestKravatte_SAE_OneInput(keyLen, nonceLen, dataLen, ADLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE_SAE
    {
        unsigned int i;
        printf("Kravatte-SAE\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestKravatte_SAE(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

#if defined(OUTPUT)
    printf("Testing Kravatte-SAE ");
    fflush(stdout);
#endif
    performTestKravatte_SAE(checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
#if defined(OUTPUT)
    printf(" - OK.\n");
#endif
}

#ifdef OUTPUT
void writeTestKravatte_SAE_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Kravatte-SAE ");
    performTestKravatte_SAE(checksum);
    fprintf(f, "    selfTestKravatte_SAE(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestKravatte_SAE(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);
    writeTestKravatte_SAE_One(f);
    fclose(f);
}
#endif

/* ------------------------------------------------------------------------- */

static void performTestKravatte_WBC_OneInput(BitLength keyLen, BitLength dataLen, BitLength WLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize];
    BitSequence key[keyByteSize];
    BitSequence W[WByteSize];
    Kravatte_Instance kvw;
    int result;
    unsigned int seed;

    randomize((unsigned char *)&kvw, sizeof(Kravatte_Instance));
    randomize(key, keyByteSize);
    randomize(W, WByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);

    seed = keyLen + WLen + dataLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, 0x43C1 - seed, 0xB9 + seed);
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(W, (WLen + 7) / 8, 0x1727 - seed, 0x34 + seed);
    if (WLen & 7)
        W[WLen / 8] &= (1 << (WLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, 0x4165 - seed, 0xA9 + seed);
    if (dataLen & 7)
        input[dataLen / 8] &= (1 << (dataLen & 7)) - 1;

    #ifdef VERBOSE_WBC
    printf( "keyLen %5u, WLen %5u, dataLen %5u (in bits)\n", keyLen, WLen, dataLen);
    #endif

    result = Kravatte_WBC_Initialize(&kvw, key, keyLen);
    assert(result == 0);
    result = Kravatte_WBC_Encipher(&kvw, input, output, dataLen, W, WLen);
    assert(result == 0);
    result = Kravatte_WBC_Decipher(&kvw, output, inputPrime, dataLen, W, WLen);
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

    KeccakWidth1600_SpongeAbsorb(pSpongeChecksum, output, (dataLen + 7) / 8);

    #ifdef VERBOSE_WBC
    {
        unsigned int i;
        unsigned int dataByteLen;

        printf("Key of %d bits:", keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("Tweak of %d bits:", WLen);
        WLen += 7;
        WLen /= 8;
        for(i=0; (i<WLen) && (i<16); i++)
            printf(" %02x", (int)W[i]);
        if (WLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", dataLen);
        dataByteLen = (dataLen + 7) / 8;
        for(i=0; (i<dataByteLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (dataByteLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", dataLen);
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


static void performTestKravatte_WBC(unsigned char *checksum)
{
    BitLength dataLen, WLen, keyLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    dataLen = 128*8;
    WLen = 64*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestKravatte_WBC_OneInput(keyLen, dataLen, WLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("d ");
    #endif
    WLen = 64*8;
    keyLen = 16*8;
    for(dataLen=0; dataLen<=dataBitSize; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+7)) {
        performTestKravatte_WBC_OneInput(keyLen, dataLen, WLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("w ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    for(WLen=0; WLen<=WBitSize; WLen = (WLen < 2*SnP_width) ? (WLen+1) : (WLen+8)) {
        performTestKravatte_WBC_OneInput(keyLen, dataLen, WLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE_WBC
    {
        unsigned int i;
        printf("Kravatte-WBC\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestKravatte_WBC(const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

#if defined(OUTPUT)
    printf("Testing Kravatte-WBC ");
    fflush(stdout);
#endif
    performTestKravatte_WBC(checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
#if defined(OUTPUT)
    printf(" - OK.\n");
#endif
}

#ifdef OUTPUT
void writeTestKravatte_WBC_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Kravatte-WBC ");
    performTestKravatte_WBC(checksum);
    fprintf(f, "    selfTestKravatte_WBC(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestKravatte_WBC(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);

    #if 0
    {
        BitLength n, nl, prevnl;

        prevnl = 0xFFFFFFFF;
        for (n = 0; n <= 8*64*1024; ++n )
        {
            nl = Kravatte_WBC_Split(n);
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

    writeTestKravatte_WBC_One(f);
    fclose(f);
}
#endif

/* ------------------------------------------------------------------------- */

static void performTestKravatte_WBC_AE_OneInput(BitLength keyLen, BitLength dataLen, BitLength ADLen, KeccakWidth1600_SpongeInstance *pSpongeChecksum)
{
    BitSequence input[dataByteSize];
    BitSequence inputPrime[dataByteSize];
    BitSequence output[dataByteSize+expansionLenWBCAE];
    BitSequence key[keyByteSize];
    BitSequence AD[ADByteSize];
    Kravatte_Instance kvw;
    unsigned int seed;
    int result;

    randomize(key, keyByteSize);
    randomize(AD, ADByteSize);
    randomize(input, dataByteSize);
    randomize(inputPrime, dataByteSize);
    randomize(output, dataByteSize);

    seed = keyLen + ADLen + dataLen;
    seed ^= seed >> 3;
    generateSimpleRawMaterial(key, (keyLen + 7) / 8, 0x91FC - seed, 0x5A + seed);
    if (keyLen & 7)
        key[keyLen / 8] &= (1 << (keyLen & 7)) - 1;
    generateSimpleRawMaterial(AD, (ADLen + 7) / 8, 0x8181 - seed, 0x9B + seed);
    if (ADLen & 7)
        AD[ADLen / 8] &= (1 << (ADLen & 7)) - 1;
    generateSimpleRawMaterial(input, (dataLen + 7) / 8, 0x1BF0 - seed, 0xC6 + seed);
    if (dataLen & 7)
        input[dataLen / 8] &= (1 << (dataLen & 7)) - 1;

    #ifdef VERBOSE_WBC_AE
    printf( "keyLen %5u, ADLen %5u, dataLen %5u (in bits)\n", keyLen, ADLen, dataLen);
    #endif

    result = Kravatte_WBCAE_Initialize(&kvw, key, keyLen);
    assert(result == 0);

    result = Kravatte_WBCAE_Encipher(&kvw, input, output, dataLen, AD, ADLen);
    assert(result == 0);
    result = Kravatte_WBCAE_Decipher(&kvw, output, inputPrime, dataLen, AD, ADLen);
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

        printf("Key of %d bits:", keyLen);
        keyLen += 7;
        keyLen /= 8;
        for(i=0; (i<keyLen) && (i<16); i++)
            printf(" %02x", (int)key[i]);
        if (keyLen > 16)
            printf(" ...");
        printf("\n");

        printf("AD of %d bits:", ADLen);
        ADLen += 7;
        ADLen /= 8;
        for(i=0; (i<ADLen) && (i<16); i++)
            printf(" %02x", (int)AD[i]);
        if (ADLen > 16)
            printf(" ...");
        printf("\n");

        printf("Input of %d bits:", dataLen);
        dataByteLen = (dataLen + 7) / 8;
        for(i=0; (i<dataByteLen) && (i<16); i++)
            printf(" %02x", (int)input[i]);
        if (dataByteLen > 16)
            printf(" ...");
        printf("\n");

        printf("Output of %d bits:", outputLen);
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


static void performTestKravatte_WBC_AE(unsigned char *checksum)
{
    BitLength dataLen, ADLen, keyLen;

    /* Accumulated test vector */
    KeccakWidth1600_SpongeInstance spongeChecksum;
    KeccakWidth1600_SpongeInitialize(&spongeChecksum, SnP_width, 0);

    #ifdef OUTPUT
    printf("k ");
    #endif
    dataLen = 128*8;
    ADLen = 64*8;
    for(keyLen=0; keyLen<keyBitSize; keyLen = (keyLen < 2*SnP_width) ? (keyLen+1) : (keyLen+8)) {
        performTestKravatte_WBC_AE_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("d ");
    #endif
    ADLen = 64*8;
    keyLen = 16*8;
    for(dataLen=0; dataLen<=dataBitSize-8*expansionLenWBCAE; dataLen = (dataLen < 2*SnP_width) ? (dataLen+1) : (dataLen+7)) {
        performTestKravatte_WBC_AE_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    #ifdef OUTPUT
    printf("a ");
    #endif
    dataLen = 128*8;
    keyLen = 16*8;
    for(ADLen=0; ADLen<=ADBitSize; ADLen = (ADLen < 2*SnP_width) ? (ADLen+1) : (ADLen+8)) {
        performTestKravatte_WBC_AE_OneInput(keyLen, dataLen, ADLen, &spongeChecksum);
    }
    
    KeccakWidth1600_SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);

    #ifdef VERBOSE_WBC_AE
    {
        unsigned int i;
        printf("Kravatte-WBC-AE\n" );
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
    #endif
}

void selfTestKravatte_WBC_AE(const char *expected)
{
    unsigned char checksum[checksumByteSize];

#if defined(OUTPUT)
    printf("Testing Kravatte-WBC-AE ");
    fflush(stdout);
#endif
    performTestKravatte_WBC_AE(checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
#if defined(OUTPUT)
    printf(" - OK.\n");
#endif
}

#ifdef OUTPUT
void writeTestKravatte_WBC_AE_One(FILE *f)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    printf("Writing Kravatte-WBC-AE ");
    performTestKravatte_WBC_AE(checksum);
    fprintf(f, "    selfTestKravatte_WBC_AE(\"");
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\");\n");
    printf("\n");
}

void writeTestKravatte_WBC_AE(const char *filename)
{
    FILE *f = fopen(filename, "w");
    assert(f != NULL);

    #if 0
    {
        BitLength n, nl, prevnl;

        prevnl = 0xFFFFFFFF;
        for (n = 0; n <= 8*64*1024; ++n )
        {
            nl = Kravatte_WBC_AE_Split(n);
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

    writeTestKravatte_WBC_AE_One(f);
    fclose(f);
}
#endif

/* ------------------------------------------------------------------------- */

void testKravatteModes(void)
{
#ifndef KeccakP1600_excluded
#ifdef OUTPUT
//    printKravatteTestVectors();
    writeTestKravatte_SIV("Kravatte_SIV.txt");
    writeTestKravatte_SAE("Kravatte_SAE.txt");
    writeTestKravatte_WBC("Kravatte_WBC.txt");
    writeTestKravatte_WBC("Kravatte_WBC.txt");
    writeTestKravatte_WBC_AE("Kravatte_WBC_AE.txt");
#endif

    selfTestKravatte_SIV("\x35\xab\x95\x41\x36\xcd\x3a\x34\x1d\x29\x89\x72\xee\x2e\xc8\x21");
    selfTestKravatte_SAE("\x51\xc6\x6d\xdb\xeb\xc3\x80\xc4\x90\xab\x88\x75\x60\xfa\x26\xb8");
    selfTestKravatte_WBC("\x4c\xf2\x03\x85\xd9\x63\xbe\xdf\xc0\x8a\x33\xd9\x2d\x93\x3d\x4d");
    selfTestKravatte_WBC_AE("\x13\x3b\xa0\x29\x97\x14\xec\x4f\xae\x1d\x73\x44\x89\x23\x31\xb6");
#endif
}
