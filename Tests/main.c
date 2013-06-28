/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "displayIntermediateValues.h"
#include "genKAT.h"
#include "KeccakDuplex.h"
#include "KeccakSponge.h"
#include "KeccakF-1600-reference.h"
#include "testDuplex.h"
#include "testPermutationAndStateMgt.h"
#include "testSponge.h"

#ifdef KeccakReference
void displayPermutationIntermediateValues()
{
    unsigned char state[KeccakF_width/8];
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
        KeccakF1600_Initialize();
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
        KeccakF1600_StatePermute(state);

        fprintf(f, "+++ Example taking the previous output as input +++\n");
        fprintf(f, "\n");
        KeccakF1600_StatePermute(state);

        fclose(f);
        displaySetIntermediateValueFile(0);
    }
}

void alignLastByteOnLSB(const unsigned char *in, unsigned char *out, unsigned int length)
{
    unsigned int lengthInBytes;

    lengthInBytes = (length+7)/8;
    memcpy(out, in, lengthInBytes);
    if ((length % 8) != 0)
        out[lengthInBytes-1] = out[lengthInBytes-1] >> (8-(length%8));
}

void displaySpongeIntermediateValuesOne(const unsigned char *message, unsigned int messageLength, unsigned int rate, unsigned int capacity)
{
    Keccak_SpongeInstance sponge;
    unsigned char output[512];
    unsigned char *messageInternal;

    messageInternal = malloc((messageLength+7)/8);
    alignLastByteOnLSB(message, messageInternal, messageLength);

    displayBytes(1, "Input message (last byte aligned on MSB)", message, (messageLength+7)/8);
    displayBits(2, "Input message (in bits)", message, messageLength, 1);
    displayBits(2, "Input message (in bits, after the formal bit reordering)", messageInternal, messageLength, 0);
    displayBytes(2, "Input message (last byte aligned on LSB)", messageInternal, (messageLength+7)/8);

    Keccak_SpongeInitialize(&sponge, rate, capacity);
    displayStateAsBytes(1, "Initial state", sponge.state);
    Keccak_SpongeAbsorb(&sponge, messageInternal, messageLength/8);
    if ((messageLength % 8) != 0)
        Keccak_SpongeAbsorbLastFewBits(&sponge, messageInternal[messageLength/8] | (1 << (messageLength % 8)));
    Keccak_SpongeSqueeze(&sponge, output, sizeof(output));

    free(messageInternal);
}

void displaySpongeIntermediateValuesFew(FILE *f, unsigned int rate, unsigned int capacity)
{
    const unsigned char *message1 = "\x53\x58\x7B\xC8";
    unsigned int message1Length = 29;
    const unsigned char *message2 = 
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
    unsigned int message2Length = 2008;

    fprintf(f, "+++ Example with a small message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 29 from ShortMsgKAT.txt.\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message1, message1Length, rate, capacity);

    fprintf(f, "+++ Example with a larger message +++\n");
    fprintf(f, "\n");
    fprintf(f, "This is the message of length 2008 from ShortMsgKAT.txt.\n");
    fprintf(f, "\n");
    displaySpongeIntermediateValuesOne(message2, message2Length, rate, capacity);
}

void displaySpongeIntermediateValues()
{
    const unsigned int capacities[3] = {256, 512, 576};
    char fileName[256];
    FILE *f;
    unsigned int i;

    for(i=0; i<3; i++) {
        unsigned int capacity = capacities[i];
        unsigned int rate = 1600-capacity;
        sprintf(fileName, "KeccakSpongeIntermediateValues_r%dc%d.txt", rate, capacity);
        f = fopen(fileName, "w");
        if (f == NULL)
            printf("Could not open %s\n", fileName);
        else {
            displaySetIntermediateValueFile(f);
            displaySetLevel(2);

            displaySpongeIntermediateValuesFew(f, rate, capacity);

            fclose(f);
            displaySetIntermediateValueFile(0);
        }
    }
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

void displayDuplexIntermediateValues()
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

int main()
{
    testPermutationAndStateMgt();
    testSpongeWithQueue();
    testSpongeWithoutQueue();
    testDuplex();
#ifdef KeccakReference
    displayPermutationIntermediateValues();
    displaySpongeIntermediateValues();
    displayDuplexIntermediateValues();
#else
    doTiming();
#endif
    return genKAT_main();
}
