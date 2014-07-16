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

#include <stdio.h>
#include "displayIntermediateValues.h"
#include "SnP-interface.h"

FILE *intermediateValueFile = 0;
int displayLevel = 0;

void displaySetIntermediateValueFile(FILE *f)
{
    intermediateValueFile = f;
}

void displaySetLevel(int level)
{
    displayLevel = level;
}

void displayBytes(int level, const char *text, const unsigned char *bytes, unsigned int size)
{
    unsigned int i;

    if ((intermediateValueFile) && (level <= displayLevel)) {
        fprintf(intermediateValueFile, "%s:\n", text);
        for(i=0; i<size; i++)
            fprintf(intermediateValueFile, "%02X ", bytes[i]);
        fprintf(intermediateValueFile, "\n");
        fprintf(intermediateValueFile, "\n");
    }
}

void displayBits(int level, const char *text, const unsigned char *data, unsigned int size, int MSBfirst)
{
    unsigned int i, iByte, iBit;

    if ((intermediateValueFile) && (level <= displayLevel)) {
        fprintf(intermediateValueFile, "%s:\n", text);
        for(i=0; i<size; i++) {
            iByte = i/8;
            iBit = i%8;
            if (MSBfirst)
                fprintf(intermediateValueFile, "%d ", ((data[iByte] << iBit) & 0x80) != 0);
            else
                fprintf(intermediateValueFile, "%d ", ((data[iByte] >> iBit) & 0x01) != 0);
        }
        fprintf(intermediateValueFile, "\n");
        fprintf(intermediateValueFile, "\n");
    }
}

void displayStateAsBytes(int level, const char *text, const unsigned char *state)
{
    displayBytes(level, text, state, SnP_width/8);
}

#if (SnP_laneLengthInBytes == 8)
void displayStateAs32bitWords(int level, const char *text, const unsigned int *state)
{
    unsigned int i;

    if ((intermediateValueFile) && (level <= displayLevel)) {
        fprintf(intermediateValueFile, "%s:\n", text);
        for(i=0; i<SnP_width/64; i++) {
            fprintf(intermediateValueFile, "%08X:%08X", (unsigned int)state[2*i+0], (unsigned int)state[2*i+1]);
            if ((i%5) == 4)
                fprintf(intermediateValueFile, "\n");
            else
                fprintf(intermediateValueFile, " ");
        }
    }
}
#endif

void displayStateAsLanes(int level, const char *text, void *statePointer)
{
    unsigned int i;
#if (SnP_laneLengthInBytes == 8)
    unsigned long long int *state = statePointer;
#endif
#if (SnP_laneLengthInBytes == 4)
    unsigned int *state = statePointer;
#endif
#if (SnP_laneLengthInBytes == 2)
    unsigned short *state = statePointer;
#endif
#if (SnP_laneLengthInBytes == 1)
    unsigned char *state = statePointer;
#endif

    if ((intermediateValueFile) && (level <= displayLevel)) {
        fprintf(intermediateValueFile, "%s:\n", text);
#if (SnP_laneLengthInBytes == 8)
        for(i=0; i<25; i++) {
            fprintf(intermediateValueFile, "%08X", (unsigned int)(state[i] >> 32));
            fprintf(intermediateValueFile, "%08X", (unsigned int)(state[i] & 0xFFFFFFFFULL));
            if ((i%5) == 4)
                fprintf(intermediateValueFile, "\n");
            else
                fprintf(intermediateValueFile, " ");
        }
#endif
#if (SnP_laneLengthInBytes == 4)
        for(i=0; i<25; i++) {
            fprintf(intermediateValueFile, "%08X", state[i]);
            if ((i%5) == 4)
                fprintf(intermediateValueFile, "\n");
            else
                fprintf(intermediateValueFile, " ");
        }
#endif
#if (SnP_laneLengthInBytes == 2)
        for(i=0; i<25; i++) {
            fprintf(intermediateValueFile, "%04X ", state[i]);
            if ((i%5) == 4)
                fprintf(intermediateValueFile, "\n");
        }
#endif
#if (SnP_laneLengthInBytes == 1)
        for(i=0; i<25; i++) {
            fprintf(intermediateValueFile, "%02X ", state[i]);
            if ((i%5) == 4)
                fprintf(intermediateValueFile, "\n");
        }
#endif
    }
}

void displayRoundNumber(int level, unsigned int i)
{
    if ((intermediateValueFile) && (level <= displayLevel)) {
        fprintf(intermediateValueFile, "\n");
        fprintf(intermediateValueFile, "--- Round %d ---\n", i);
        fprintf(intermediateValueFile, "\n");
    }
}

void displayText(int level, const char *text)
{
    if ((intermediateValueFile) && (level <= displayLevel)) {
        fprintf(intermediateValueFile, "%s", text);
        fprintf(intermediateValueFile, "\n");
        fprintf(intermediateValueFile, "\n");
    }
}
