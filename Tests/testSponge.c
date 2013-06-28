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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KeccakSponge.h"

void testSponge(FILE *f, unsigned int rate, unsigned int capacity, int usingQueue)
{
    #define inputByteSize 512
    #define outputByteSize 512
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    unsigned char acc[outputByteSize];
    unsigned int inputBitLength, offset, size;
    int result;
    Keccak_SpongeInstance sponge;

    // Acumulated test vector
    memset(acc, 0x00, sizeof(acc));

    for(inputBitLength = 0; inputBitLength <= (inputByteSize*8); inputBitLength += (inputBitLength < 1600) ? 1 : 8) {
        unsigned int i;
        unsigned int inputByteLengthCeiling = (inputBitLength + 7) / 8;

        for(i=0; i<inputByteLengthCeiling; i++)
            input[i] = inputBitLength - i;
        if ((inputBitLength % 8) != 0)
            input[inputByteLengthCeiling-1] &= (1 << (inputBitLength % 8)) - 1;

        result = Keccak_SpongeInitialize(&sponge, rate, capacity);

        if (usingQueue) {
            for(offset = 0; offset < inputBitLength/8; offset += size) {
                unsigned int p = (inputBitLength/8)%11;
                // vary sizes
                if (p < 1) size = 1; // byte per byte
                else if (p < 5) size = (rand() % 20); // small random
                else if (p < 9) size = (rand() % 200); // big random
                else size = ((rand() % (inputBitLength/8 - offset)) + 1); // converging random

                if (size > (inputBitLength/8 - offset))
                    size = inputBitLength/8 - offset;

                result = Keccak_SpongeAbsorb(&sponge, input + offset, size);
            }
        }
        else
            result = Keccak_SpongeAbsorb(&sponge, input, inputBitLength/8);
        if ((inputBitLength % 8) != 0)
            result = Keccak_SpongeAbsorbLastFewBits(&sponge, input[inputByteLengthCeiling-1] | (1 << (inputBitLength % 8)));

        if (usingQueue) {
            unsigned char filler = 0xAA + inputBitLength;
            memset(output, filler, sizeof(output));
            for(offset = 0; offset < outputByteSize; offset += size) {
                unsigned int p = (inputBitLength/8)%11;
                // vary sizes
                if (p < 1) size = 1; // byte per byte
                else if (p < 5) size = (rand() % 20); // small random
                else if (p < 9) size = (rand() % 200); // big random
                else size = ((rand() % (outputByteSize - offset)) + 1); // converging random

                if (size > (outputByteSize - offset))
                    size = outputByteSize - offset;

                result = Keccak_SpongeSqueeze(&sponge, output + offset, size);
                for(i = offset + size; i<sizeof(output); i++)
                    if (output[i] != filler) {
                        printf("Out of range data written!\n");
                        abort();
                    }
            }
        }
        else
            result = Keccak_SpongeSqueeze(&sponge, output, outputByteSize);

        for (i = 0; i < outputByteSize; i++)
            acc[i] ^= output[i];
    }
    
    fprintf(f, "Keccak[r=%d, c=%d]: ", rate, capacity);
    for(offset=0; offset<outputByteSize; offset++)
        fprintf(f, "%02x ", acc[offset]);
    fprintf(f, "\n\n");
    #undef inputByteSize
    #undef outputByteSize
}

void testSpongeWithQueue()
{
    FILE *f;
    unsigned int rate;
    
    f = fopen("TestSpongeWithQueue.txt", "w");
    for(rate = 64; rate <= 1600; rate += (rate < 1024) ? 64 : ((rate < 1344) ? 32 : 8))
        testSponge(f, rate, 1600-rate, 1);
    fclose(f);
}

void testSpongeWithoutQueue()
{
    FILE *f;
    unsigned int rate;
    
    f = fopen("TestSpongeWithoutQueue.txt", "w");
    for(rate = 64; rate <= 1600; rate += (rate < 1024) ? 64 : ((rate < 1344) ? 32 : 8))
        testSponge(f, rate, 1600-rate, 0);
    fclose(f);
}

