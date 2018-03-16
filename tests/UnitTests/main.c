/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "genKAT.h"
#include "KeccakDuplex.h"
#include "SimpleFIPS202.h"
#include "testDuplex.h"
#include "testKangarooTwelve.h"
#include "testKetjev2.h"
#include "testKeyakv2.h"
#include "testMotorist.h"
#include "testPerformance.h"
#include "testPlSnP.h"
#include "testSnP.h"
#include "testSponge.h"
#include "testKeccakPRG.h"
#include "testKravatte.h"
#include "testKravatteModes.h"
#include "testSP800-185.h"

#ifdef KeccakReference
#include "displayIntermediateValues.h"

#ifndef KeccakP200_excluded
#include "KeccakP-200-reference.h"
void displayKeccakF200IntermediateValues()
{
    unsigned char state[KeccakP200_stateSizeInBytes];
    const char *fileName = "KeccakF-200-IntermediateValues.txt";
    FILE *f;

    f = fopen(fileName, "w");
    if (f == NULL)
        printf("Could not open %s\n", fileName);
    else {
        KeccakP200_StaticInitialize();
        fprintf(f, "+++ The round constants +++\n");
        fprintf(f, "\n");
        KeccakP200_DisplayRoundConstants(f);

        fprintf(f, "+++ The rho offsets +++\n");
        fprintf(f, "\n");
        KeccakP200_DisplayRhoOffsets(f);

        displaySetIntermediateValueFile(f);
        displaySetLevel(3);

        fprintf(f, "+++ Example with the all-zero input +++\n");
        fprintf(f, "\n");
        KeccakP200_Initialize(state);
        KeccakP200_Permute_18rounds(state);

        fprintf(f, "+++ Example taking the previous output as input +++\n");
        fprintf(f, "\n");
        KeccakP200_Permute_18rounds(state);

        fclose(f);
        displaySetIntermediateValueFile(0);
    }
}
#endif

#ifndef KeccakP400_excluded
#include "KeccakP-400-reference.h"
void displayKeccakF400IntermediateValues()
{
    unsigned char state[KeccakP400_stateSizeInBytes];
    const char *fileName = "KeccakF-400-IntermediateValues.txt";
    FILE *f;

    f = fopen(fileName, "w");
    if (f == NULL)
        printf("Could not open %s\n", fileName);
    else {
        KeccakP400_StaticInitialize();
        fprintf(f, "+++ The round constants +++\n");
        fprintf(f, "\n");
        KeccakP400_DisplayRoundConstants(f);

        fprintf(f, "+++ The rho offsets +++\n");
        fprintf(f, "\n");
        KeccakP400_DisplayRhoOffsets(f);

        displaySetIntermediateValueFile(f);
        displaySetLevel(3);

        fprintf(f, "+++ Example with the all-zero input +++\n");
        fprintf(f, "\n");
        KeccakP400_Initialize(state);
        KeccakP400_Permute_20rounds(state);

        fprintf(f, "+++ Example taking the previous output as input +++\n");
        fprintf(f, "\n");
        KeccakP400_Permute_20rounds(state);

        fclose(f);
        displaySetIntermediateValueFile(0);
    }
}
#endif

#ifndef KeccakP800_excluded
#include "KeccakP-800-reference.h"
void displayKeccakF800IntermediateValues()
{
    unsigned char state[KeccakP800_stateSizeInBytes];
    const char *fileName = "KeccakF-800-IntermediateValues.txt";
    FILE *f;

    f = fopen(fileName, "w");
    if (f == NULL)
        printf("Could not open %s\n", fileName);
    else {
        KeccakP800_StaticInitialize();
        fprintf(f, "+++ The round constants +++\n");
        fprintf(f, "\n");
        KeccakP800_DisplayRoundConstants(f);

        fprintf(f, "+++ The rho offsets +++\n");
        fprintf(f, "\n");
        KeccakP800_DisplayRhoOffsets(f);

        displaySetIntermediateValueFile(f);
        displaySetLevel(3);

        fprintf(f, "+++ Example with the all-zero input +++\n");
        fprintf(f, "\n");
        KeccakP800_Initialize(state);
        KeccakP800_Permute_22rounds(state);

        fprintf(f, "+++ Example taking the previous output as input +++\n");
        fprintf(f, "\n");
        KeccakP800_Permute_22rounds(state);

        fclose(f);
        displaySetIntermediateValueFile(0);
    }
}
#endif

#ifndef KeccakP1600_excluded
#include "KeccakP-1600-reference.h"
void displayKeccakF1600IntermediateValues()
{
    unsigned char state[KeccakP1600_stateSizeInBytes];
    #ifdef KeccakReference32BI
    const char *fileName = "KeccakF-1600-IntermediateValues32BI.txt";
    #else
    const char *fileName = "KeccakF-1600-IntermediateValues.txt";
    #endif
    FILE *f;

    f = fopen(fileName, "w");
    if (f == NULL)
        printf("Could not open %s\n", fileName);
    else {
        KeccakP1600_StaticInitialize();
        fprintf(f, "+++ The round constants +++\n");
        fprintf(f, "\n");
        KeccakP1600_DisplayRoundConstants(f);

        fprintf(f, "+++ The rho offsets +++\n");
        fprintf(f, "\n");
        KeccakP1600_DisplayRhoOffsets(f);

        displaySetIntermediateValueFile(f);
        displaySetLevel(3);

        fprintf(f, "+++ Example with the all-zero input +++\n");
        fprintf(f, "\n");
        KeccakP1600_Initialize(state);
        KeccakP1600_Permute_24rounds(state);

        fprintf(f, "+++ Example taking the previous output as input +++\n");
        fprintf(f, "\n");
        KeccakP1600_Permute_24rounds(state);

        fclose(f);
        displaySetIntermediateValueFile(0);
    }
}

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
    KeccakWidth1600_SpongeInstance sponge;
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

    KeccakWidth1600_SpongeInitialize(&sponge, rate, capacity);
    displayStateAsBytes(1, "Initial state", sponge.state, 1600);
    KeccakWidth1600_SpongeAbsorb(&sponge, messageWithSuffix, messageLengthWithSuffix/8);
    if ((messageLengthWithSuffix % 8) != 0)
        KeccakWidth1600_SpongeAbsorbLastFewBits(&sponge, messageWithSuffix[messageLengthWithSuffix/8] | (1 << (messageLengthWithSuffix % 8)));
    if (outputLengthInBits <= 8*sizeof(output))
        KeccakWidth1600_SpongeSqueeze(&sponge, output, (outputLengthInBits+7)/8);
    else
        abort();
}

void displaySpongeIntermediateValuesFew(const char *fileName, unsigned char delimitedSuffix, unsigned int rate, unsigned int capacity, unsigned int desiredOutputLengthInBits)
{
    const unsigned char *message0 = (unsigned char *) "";
    const unsigned char *message5 = (unsigned char *) "\x13"; /* 11001 */
    const unsigned char *message30 = (unsigned char *) "\x53\x58\x7B\x19"; /* 110010100001101011011110100110 */
    const unsigned char *message1600 = (unsigned char *)
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3";
    const unsigned char *message1605 = (unsigned char *)
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\x03";
    const unsigned char *message1630 = (unsigned char *)
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3\xA3"
        "\xA3\xA3\xA3\x23";
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
    fprintf(f, "This is the message of length 1600 from http://csrc.nist.gov/groups/ST/toolkit/examples.html .\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message1600, 1600, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

    fprintf(f, "+++ Example with a larger message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 1605 from http://csrc.nist.gov/groups/ST/toolkit/examples.html .\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message1605, 1605, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

    fprintf(f, "+++ Example with a larger message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 1630 from http://csrc.nist.gov/groups/ST/toolkit/examples.html .\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message1630, 1630, delimitedSuffix, rate, capacity, desiredOutputLengthInBits);

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
    KeccakWidth1600_DuplexInstance duplex;
    unsigned char input[512];
    unsigned int inBitLen;
    unsigned char output[512];
    unsigned int outBitLen;
    unsigned int i, j;
    const unsigned int M = 239*251;
    unsigned int x = 33;

    KeccakWidth1600_DuplexInitialize(&duplex, rate, capacity);
    displayStateAsBytes(1, "Initial state", duplex.state, 1600);

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
            KeccakWidth1600_Duplexing(&duplex, input, inBitLen/8, output, (outBitLen+7)/8, 0x01);
        else
            KeccakWidth1600_Duplexing(&duplex, input, inBitLen/8, output, (outBitLen+7)/8, input[inBitLen/8] | (1 << (inBitLen%8)));
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
#endif
#endif /* defined KeccakReference */

void testFIPS202()
{
    const unsigned char *input = (const unsigned char *)"\x21\xF1\x34\xAC\x57";
    const unsigned char *outputSHAKE128 = (const unsigned char *)
        "\x7B\xFB\xB4\x0D\xA3\x70\x4A\x55\x82\x91\xB3\x9E\x1E\x56\xED\x9F"
        "\x6F\x56\xAE\x78\x32\x70\xAB\x02\xA2\x02\x06\x0C\x91\x73\xFB\xB0"
        "\xB4\x55\x75\xB3\x23\x48\xA6\xED\x2C\x92\x7A\x39\xA3\x0D\xA0\xA2"
        "\xBB\xC1\x80\x74\x97\xAD\x50\xF2\x7A\x10\x77\x54\xAF\x62\x76\x2C";
    const unsigned char *outputSHAKE256 = (const unsigned char *)
        "\xBB\x8A\x84\x47\x51\x7B\xA9\xCA\x7F\xA3\x4E\xC9\x9A\x80\x00\x4F"
        "\x22\x8A\xB2\x82\x47\x28\x41\xEB\x3D\x3A\x76\x22\x5C\x9D\xBE\x77"
        "\xF7\xE4\x0A\x06\x67\x76\xD3\x2C\x74\x94\x12\x02\xF9\xF4\xAA\x43"
        "\xD1\x2C\x62\x64\xAF\xA5\x96\x39\xC4\x4E\x11\xF5\xE1\x4F\x1E\x56";
    const unsigned char *outputSHA3_224 = (const unsigned char *)
        "\x10\xE5\x80\xA3\x21\x99\x59\x61\x69\x33\x1A\xD4\x3C\xFC\xF1\x02"
        "\x64\xF8\x15\x65\x03\x70\x40\x02\x8A\x06\xB4\x58";
    const unsigned char *outputSHA3_256 = (const unsigned char *)
        "\x55\xBD\x92\x24\xAF\x4E\xED\x0D\x12\x11\x49\xE3\x7F\xF4\xD7\xDD"
        "\x5B\xE2\x4B\xD9\xFB\xE5\x6E\x01\x71\xE8\x7D\xB7\xA6\xF4\xE0\x6D";
    const unsigned char *outputSHA3_384 = (const unsigned char *)
        "\xE2\x48\xD6\xFF\x34\x2D\x35\xA3\x0E\xC2\x30\xBA\x51\xCD\xB1\x61"
        "\x02\x5D\x6F\x1C\x25\x1A\xCA\x6A\xE3\x53\x1F\x06\x82\xC1\x64\xA1"
        "\xFC\x07\x25\xB1\xBE\xFF\x80\x8A\x20\x0C\x13\x15\x57\xA2\x28\x09";
    const unsigned char *outputSHA3_512 = (const unsigned char *)
        "\x58\x42\x19\xA8\x4E\x87\x96\x07\x6B\xF1\x17\x8B\x14\xB9\xD1\xE2"
        "\xF9\x6A\x4B\x4E\xF1\x1F\x10\xCC\x51\x6F\xBE\x1A\x29\x63\x9D\x6B"
        "\xA7\x4F\xB9\x28\x15\xF9\xE3\xC5\x19\x2E\xD4\xDC\xA2\x0A\xEA\x5B"
        "\x10\x9D\x52\x23\x7C\x99\x56\x40\x1F\xD4\x4B\x22\x1F\x82\xAB\x37";
    unsigned char buffer[64];

    assert(SHAKE128(buffer, 64, input, 5) == 0);
    assert(memcmp(buffer, outputSHAKE128, 64) == 0);
    assert(SHAKE256(buffer, 64, input, 5) == 0);
    assert(memcmp(buffer, outputSHAKE256, 64) == 0);
    assert(SHA3_224(buffer, input, 5) == 0);
    assert(memcmp(buffer, outputSHA3_224, 28) == 0);
    assert(SHA3_256(buffer, input, 5) == 0);
    assert(memcmp(buffer, outputSHA3_256, 32) == 0);
    assert(SHA3_384(buffer, input, 5) == 0);
    assert(memcmp(buffer, outputSHA3_384, 48) == 0);
    assert(SHA3_512(buffer, input, 5) == 0);
    assert(memcmp(buffer, outputSHA3_512, 64) == 0);
}

void printHelp()
{
        printf("Usage: KeccakTests command(s), where the commands can be\n");
        printf("  --help or -h              To display this page\n");
        printf("  --all or -a               All tests\n");
        printf("  --SnP or -p               Tests on Keccak-p permutations\n");
        printf("  --Keccak or -c            Tests on Keccak sponge and duplex\n");
        printf("  --KeccakSponge            Tests on Keccak sponge\n");
        printf("  --KeccakDuplex            Tests on Keccak duplex\n");
        printf("  --KeccakPRG               Tests on KeccakPRG\n");
        printf("  --FIPS202 or -f           Tests on FIPS202 and ShortMsgKAT generation\n");
        printf("  --SP800-185               Tests on SP800-185 functions\n");
        printf("  --Keyak or -y             Tests on the Keyak authentication encryption scheme\n");
        printf("  --Ketje or -t             Tests on the Ketje authentication encryption scheme\n");
        printf("  --KangarooTwelve or -K12  Tests on KangarooTwelve\n");
        printf("  --Kravatte                Tests on Kravatte\n");
#ifdef KeccakReference
        printf("  --examples or -e          Generation of example files\n");
#else
        printf("  --speed or -s             Speed measuresments\n");
#endif
}

int process(int argc, char* argv[])
{
    int i;
    int help = 0;
    int SnP = 0;
    int KeccakSponge = 0;
    int KeccakDuplex = 0;
    int KeccakPRG = 0;
    int FIPS202 = 0;
    int Keyak = 0;
    int Ketje = 0;
    int KangarooTwelve = 0;
    int Kravatte = 0;
    int SP800_185 = 0;
    int examples = 0;
    int speed = 0;

    if (argc == 1)
        help = 1;

    for(i=1; i<argc; i++) {
        if ((strcmp("--help", argv[i]) == 0) || (strcmp("-h", argv[i]) == 0))
            help = 1;
        else if ((strcmp("--all", argv[i]) == 0) || (strcmp("-a", argv[i]) == 0))
            SnP = KeccakSponge = KeccakDuplex = KeccakPRG = FIPS202 = Keyak = Ketje = KangarooTwelve = Kravatte = SP800_185 = examples = speed = 1;
        else if ((strcmp("--SnP", argv[i]) == 0) || (strcmp("-p", argv[i]) == 0))
            SnP = 1;
        else if ((strcmp("--Keccak", argv[i]) == 0) || (strcmp("-c", argv[i]) == 0))
            KeccakSponge = KeccakDuplex = 1;
        else if (strcmp("--KeccakSponge", argv[i]) == 0)
            KeccakSponge = 1;
        else if (strcmp("--KeccakDuplex", argv[i]) == 0)
            KeccakDuplex = 1;
        else if (strcmp("--KeccakPRG", argv[i]) == 0)
            KeccakPRG = 1;
        else if ((strcmp("--FIPS202", argv[i]) == 0) || (strcmp("-f", argv[i]) == 0))
            FIPS202 = 1;
        else if ((strcmp("--Keyak", argv[i]) == 0) || (strcmp("-y", argv[i]) == 0))
            Keyak = 1;
        else if ((strcmp("--Ketje", argv[i]) == 0) || (strcmp("-t", argv[i]) == 0))
            Ketje = 1;
        else if ((strcmp("--KangarooTwelve", argv[i]) == 0) || (strcmp("-K12", argv[i]) == 0))
            KangarooTwelve = 1;
        else if (strcmp("--Kravatte", argv[i]) == 0)
            Kravatte = 1;
        else if (strcmp("--SP800-185", argv[i]) == 0)
            SP800_185 = 1;
#ifdef KeccakReference
        else if ((strcmp("--examples", argv[i]) == 0) || (strcmp("-e", argv[i]) == 0))
            examples = 1;
#else
        else if ((strcmp("--speed", argv[i]) == 0) || (strcmp("-s", argv[i]) == 0))
            speed = 1;
#endif
        else {
            printf("Unrecognized command '%s'\n", argv[i]);
            return -1;
        }
    }
    if (help) {
        printHelp();
        return 0;
    }
    if (SnP) {
        testSnP();
        testPlSnP();
    }
    if (KeccakSponge) {
        testSponge();
    }
    if (KeccakDuplex) {
        testDuplex();
    }
    if (KangarooTwelve) {
        testKangarooTwelve();
    }
    if (SP800_185) {
        testSP800_185();
    }
    if (KeccakPRG) {
        testKeccakPRG();
    }
    if (FIPS202) {
        testFIPS202();
        genKAT_main();
    }
    if (Keyak) {
        testMotorist();
        testKeyak();
    }
    if (Ketje) {
        testKetje();
    }
    if (Kravatte) {
        testKravatte();
        testKravatteModes();
    }
#ifdef KeccakReference
    if (examples) {
#ifndef KeccakP200_excluded
        displayKeccakF200IntermediateValues();
#endif
#ifndef KeccakP400_excluded
        displayKeccakF400IntermediateValues();
#endif
#ifndef KeccakP800_excluded
        displayKeccakF800IntermediateValues();
#endif
#ifndef KeccakP1600_excluded
        displayKeccakF1600IntermediateValues();
#endif
        displaySpongeIntermediateValues();
        displayDuplexIntermediateValues();
    }
#else
    if (speed) {
        testPerformance();
    }
#endif
    return 0;
}

int main(int argc, char* argv[])
{
    return process(argc, argv);
}
