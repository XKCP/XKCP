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

#if __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !(defined(Keyak)) && !(defined(Ketje))
#include "genKAT.h"
#include "testDuplex.h"
#include "testSponge.h"
#ifndef KeccakReference
#ifdef PlSnP_P
#include "timingPlSnP.h"
#else
#include "timingSnP.h"
#endif
#endif
#endif

#ifdef Keyak
#include "NumberOfParallelInstances.h"
#include "testKeyak.h"
#endif
#ifdef Ketje
#include "testKetje.h"
#endif

#ifdef PlSnP_P
#include "testPlSnP.h"
#else
#include "testSnP.h"
#endif

#ifdef KeccakReference
#include "displayIntermediateValues.h"
#include "KeccakF-reference.h"
#include "KeccakDuplex.h"
#include "KeccakSponge.h"
void displayPermutationIntermediateValues()
{
    unsigned char state[KeccakF_width/8];
#if (KeccakF_width == 1600)
    #ifdef KeccakReference32BI
    const char *fileName = "KeccakF-1600-IntermediateValues32BI.txt";
    #else
    const char *fileName = "KeccakF-1600-IntermediateValues.txt";
    #endif
#endif
#if (KeccakF_width == 800)
    const char *fileName = "KeccakF-800-IntermediateValues.txt";
#endif
#if (KeccakF_width == 400)
    const char *fileName = "KeccakF-400-IntermediateValues.txt";
#endif
#if (KeccakF_width == 200)
    const char *fileName = "KeccakF-200-IntermediateValues.txt";
#endif
    FILE *f;

    f = fopen(fileName, "w");
    if (f == NULL)
        printf("Could not open %s\n", fileName);
    else {
        KeccakF_Initialize();
        fprintf(f, "+++ The round constants +++\n");
        fprintf(f, "\n");
        displayRoundConstants(f);

        fprintf(f, "+++ The rho offsets +++\n");
        fprintf(f, "\n");
        displayRhoOffsets(f);

        displaySetIntermediateValueFile(f);
        displaySetLevel(3);

        fprintf(f, "+++ Example with the all-zero input +++\n");
        fprintf(f, "\n");
        memset(state, 0, KeccakF_width/8);
        KeccakF_StatePermute(state);

        fprintf(f, "+++ Example taking the previous output as input +++\n");
        fprintf(f, "\n");
        KeccakF_StatePermute(state);

        fclose(f);
        displaySetIntermediateValueFile(0);
    }
}

#if (KeccakF_width == 1600)
unsigned int appendSuffixToMessage(char *out, const char *in, unsigned int inputLengthInBits, unsigned char delimitedSuffix)
{
    memcpy(out, in, (inputLengthInBits+7)/8);
    if (delimitedSuffix == 0x00)
        abort();
    while(delimitedSuffix != 0x01) {
        unsigned char bit = delimitedSuffix & 0x01;
        out[inputLengthInBits/8] |= (bit << (inputLengthInBits%8));
        inputLengthInBits++;
        delimitedSuffix >>= 1;
    }
    return inputLengthInBits;
}

void displaySpongeIntermediateValuesOne(const unsigned char *message, unsigned int messageLength, unsigned char delimitedSuffix, unsigned int rate, unsigned int capacity, unsigned int outputLengthInBits)
{
    Keccak_SpongeInstance sponge;
    unsigned char output[512];
    unsigned char *messageWithSuffix;
    unsigned int messageLengthWithSuffix;

    displayBytes(1, "Input message (last byte aligned on LSB)", message, (messageLength+7)/8);
    displayBits(2, "Input message (in bits)", message, messageLength, 0);
    messageWithSuffix = malloc((messageLength+15)/8);
    messageLengthWithSuffix = appendSuffixToMessage(messageWithSuffix, message, messageLength, delimitedSuffix);
    if (delimitedSuffix != 0x01) {
        unsigned char suffix[1];
        suffix[0] = delimitedSuffix;
        displayBytes(2, "Delimited suffix", suffix, 1);
        displayBits(2, "Suffix (in bits)", suffix, messageLengthWithSuffix-messageLength, 0);
        displayBits(2, "Input message with suffix appended to it (in bits)", messageWithSuffix, messageLengthWithSuffix, 0);
        displayBytes(2, "Input message with suffix appended to it (last byte aligned on LSB)", messageWithSuffix, (messageLengthWithSuffix+7)/8);
    }

    Keccak_SpongeInitialize(&sponge, rate, capacity);
    displayStateAsBytes(1, "Initial state", sponge.state);
    Keccak_SpongeAbsorb(&sponge, messageWithSuffix, messageLengthWithSuffix/8);
    if ((messageLengthWithSuffix % 8) != 0)
        Keccak_SpongeAbsorbLastFewBits(&sponge, messageWithSuffix[messageLengthWithSuffix/8] | (1 << (messageLengthWithSuffix % 8)));
    if (outputLengthInBits <= 8*sizeof(output))
        Keccak_SpongeSqueeze(&sponge, output, (outputLengthInBits+7)/8);
    else
        abort();
}

void displaySpongeIntermediateValuesFew(const char *fileName, unsigned char delimitedSuffix, unsigned int rate, unsigned int capacity, unsigned int desiredOutputLengthInBits)
{
    const unsigned char *message0 = (unsigned char *) "";
    const unsigned char *message5 = (unsigned char *) "\x13"; // 11001
    const unsigned char *message30 = (unsigned char *) "\x53\x58\x7B\x19"; // 110010100001101011011110100110
    const unsigned char *message2008 = (unsigned char *)
        "\x83\xAF\x34\x27\x9C\xCB\x54\x30\xFE\xBE\xC0\x7A\x81\x95\x0D\x30"
        "\xF4\xB6\x6F\x48\x48\x26\xAF\xEE\x74\x56\xF0\x07\x1A\x51\xE1\xBB"
        "\xC5\x55\x70\xB5\xCC\x7E\xC6\xF9\x30\x9C\x17\xBF\x5B\xEF\xDD\x7C"
        "\x6B\xA6\xE9\x68\xCF\x21\x8A\x2B\x34\xBD\x5C\xF9\x27\xAB\x84\x6E"
        "\x38\xA4\x0B\xBD\x81\x75\x9E\x9E\x33\x38\x10\x16\xA7\x55\xF6\x99"
        "\xDF\x35\xD6\x60\x00\x7B\x5E\xAD\xF2\x92\xFE\xEF\xB7\x35\x20\x7E"
        "\xBF\x70\xB5\xBD\x17\x83\x4F\x7B\xFA\x0E\x16\xCB\x21\x9A\xD4\xAF"
        "\x52\x4A\xB1\xEA\x37\x33\x4A\xA6\x64\x35\xE5\xD3\x97\xFC\x0A\x06"
        "\x5C\x41\x1E\xBB\xCE\x32\xC2\x40\xB9\x04\x76\xD3\x07\xCE\x80\x2E"
        "\xC8\x2C\x1C\x49\xBC\x1B\xEC\x48\xC0\x67\x5E\xC2\xA6\xC6\xF3\xED"
        "\x3E\x5B\x74\x1D\x13\x43\x70\x95\x70\x7C\x56\x5E\x10\xD8\xA2\x0B"
        "\x8C\x20\x46\x8F\xF9\x51\x4F\xCF\x31\xB4\x24\x9C\xD8\x2D\xCE\xE5"
        "\x8C\x0A\x2A\xF5\x38\xB2\x91\xA8\x7E\x33\x90\xD7\x37\x19\x1A\x07"
        "\x48\x4A\x5D\x3F\x3F\xB8\xC8\xF1\x5C\xE0\x56\xE5\xE5\xF8\xFE\xBE"
        "\x5E\x1F\xB5\x9D\x67\x40\x98\x0A\xA0\x6C\xA8\xA0\xC2\x0F\x57\x12"
        "\xB4\xCD\xE5\xD0\x32\xE9\x2A\xB8\x9F\x0A\xE1";

    FILE *f = fopen(fileName, "w");
    if (f == NULL) {
        printf("Could not open %s\n", fileName);
        return;
    }
    displaySetIntermediateValueFile(f);
    displaySetLevel(2);

    fprintf(f, "+++ Example with a small message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the empty string.\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message0, 0, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

    fprintf(f, "+++ Example with a small message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 5 from http://csrc.nist.gov/groups/ST/toolkit/examples.html .\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message5, 5, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

    fprintf(f, "+++ Example with a small message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 30 from http://csrc.nist.gov/groups/ST/toolkit/examples.html .\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message30, 30, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

    fprintf(f, "+++ Example with a larger message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 2008 from ShortMsgKAT.txt.\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message2008, 2008, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

            fclose(f);
            displaySetIntermediateValueFile(0);
        }

void displaySpongeIntermediateValues(void)
{
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_r1344c256.txt", 0x01, 1344,  256, 4096);
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_SHAKE128.txt", 0x1F, 1344,  256, 4096);
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_SHAKE256.txt", 0x1F, 1088,  512, 4096);
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_SHA3-224.txt", 0x06, 1152,  448,  224);
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_SHA3-256.txt", 0x06, 1088,  512,  256);
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_SHA3-384.txt", 0x06,  832,  768,  384);
    displaySpongeIntermediateValuesFew("KeccakSpongeIntermediateValues_SHA3-512.txt", 0x06,  576, 1024,  512);
}

void displayDuplexIntermediateValuesOne(FILE *f, unsigned int rate, unsigned int capacity)
{
    Keccak_DuplexInstance duplex;
    unsigned char input[512];
    unsigned int inBitLen;
    unsigned char output[512];
    unsigned int outBitLen;
    unsigned int i, j;
    const unsigned int M = 239*251;
    unsigned int x = 33;

    Keccak_DuplexInitialize(&duplex, rate, capacity);
    displayStateAsBytes(1, "Initial state", duplex.state);

    for(i=0; i<=rate+120; i+=123) {
        inBitLen = i;
        if (inBitLen > (rate-2)) inBitLen = rate-2;
        memset(input, 0, 512);
        for(j=0; j<inBitLen; j++) {
            x = (x*x) % M;
            if ((x % 2) != 0)
                input[j/8] |= 1 << (j%8);
        }
        {
            char text[100];
            sprintf(text, "Input (%d bits)", inBitLen);
            displayBytes(1, text, input, (inBitLen+7)/8);
        }
        outBitLen = rate;
        if ((inBitLen%8) == 0)
            Keccak_Duplexing(&duplex, input, inBitLen/8, output, (outBitLen+7)/8, 0x01);
        else
            Keccak_Duplexing(&duplex, input, inBitLen/8, output, (outBitLen+7)/8, input[inBitLen/8] | (1 << (inBitLen%8)));
        {
            char text[100];
            sprintf(text, "Output (%d bits)", outBitLen);
            displayBytes(1, text, output, (outBitLen+7)/8);
        }
    }
}

void displayDuplexIntermediateValues(void)
{
    char fileName[256];
    FILE *f;
    unsigned int rate;

    for(rate=1026; rate<=1027; rate++) {
        unsigned int capacity = 1600-rate;
        sprintf(fileName, "KeccakDuplexIntermediateValues_r%dc%d.txt", rate, capacity);
        f = fopen(fileName, "w");
        if (f == NULL)
            printf("Could not open %s\n", fileName);
        else {
            displaySetIntermediateValueFile(f);
            displaySetLevel(2);

            displayDuplexIntermediateValuesOne(f, rate, capacity);

            fclose(f);
            displaySetIntermediateValueFile(0);
        }
    }
}
#endif /* KeccakF_width == 1600 */
#endif /* defined KeccakReference */

#ifdef Keyak
int main()
{
#ifdef PlSnP_P
    testPlSnP();
#else
    testSnP();
#endif
#ifndef KeyakReference
#ifdef PlSnP_P
    doTimingPlSnP();
#else
    doTimingSnP();
#endif
#endif
    testKeyak();
    return 0;
}
#elif defined(Ketje)
int main()
{
    testSnP();
    testKetje();
}
#else
int main()
{
#ifdef PlSnP_P
    testPlSnP();
#else
    testSnP();
#endif
    testSponge();
    testDuplex();
#ifdef KeccakReference
    displayPermutationIntermediateValues();
#if (KeccakF_width == 1600)
    displaySpongeIntermediateValues();
    displayDuplexIntermediateValues();
#endif
#else
#ifdef PlSnP_P
    doTimingPlSnP();
#else
    doTimingSnP();
#endif
#endif
    return genKAT_main();
}
#endif
