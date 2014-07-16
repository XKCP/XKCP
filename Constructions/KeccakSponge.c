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

#include <string.h>
#include "KeccakSponge.h"
#include "SnP-interface.h"
#ifdef KeccakReference
#include "displayIntermediateValues.h"
#endif

/* ---------------------------------------------------------------- */

int Keccak_SpongeInitialize(Keccak_SpongeInstance *instance, unsigned int rate, unsigned int capacity)
{
    if (rate+capacity != SnP_width)
        return 1;
    if ((rate <= 0) || (rate > SnP_width) || ((rate % 8) != 0))
        return 1;
    SnP_StaticInitialize();
    SnP_Initialize(instance->state);
    instance->rate = rate;
    instance->byteIOIndex = 0;
    instance->squeezing = 0;

    return 0;
}

/* ---------------------------------------------------------------- */

int Keccak_SpongeAbsorb(Keccak_SpongeInstance *instance, const unsigned char *data, size_t dataByteLen)
{
    size_t i, j;
    unsigned int partialBlock;
    const unsigned char *curData;
    unsigned int rateInBytes = instance->rate/8;

    if (instance->squeezing)
        return 1; // Too late for additional input

    i = 0;
    curData = data;
    while(i < dataByteLen) {
        if ((instance->byteIOIndex == 0) && (dataByteLen >= (i + rateInBytes))) {
            // processing full blocks first
            if ((rateInBytes % SnP_laneLengthInBytes) == 0) {
                // fast lane: whole lane rate
                j = SnP_FBWL_Absorb(instance->state, rateInBytes/SnP_laneLengthInBytes, curData, dataByteLen - i, 0);
                i += j;
                curData += j;
            }
            else {
                for(j=dataByteLen-i; j>=rateInBytes; j-=rateInBytes) {
                    #ifdef KeccakReference
                    displayBytes(1, "Block to be absorbed", curData, rateInBytes);
                    #endif
                    SnP_XORBytes(instance->state, curData, 0, rateInBytes);
                    SnP_Permute(instance->state);
                    curData+=rateInBytes;
                }
                i = dataByteLen - j;
            }
        }
        else {
            // normal lane: using the message queue
            partialBlock = (unsigned int)(dataByteLen - i);
            if (partialBlock+instance->byteIOIndex > rateInBytes)
                partialBlock = rateInBytes-instance->byteIOIndex;
            #ifdef KeccakReference
            displayBytes(1, "Block to be absorbed (part)", curData, partialBlock);
            #endif
            i += partialBlock;

            SnP_XORBytes(instance->state, curData, instance->byteIOIndex, partialBlock);
            curData += partialBlock;
            instance->byteIOIndex += partialBlock;
            if (instance->byteIOIndex == rateInBytes) {
                SnP_Permute(instance->state);
                instance->byteIOIndex = 0;
            }
        }
    }
    return 0;
}

/* ---------------------------------------------------------------- */

int Keccak_SpongeAbsorbLastFewBits(Keccak_SpongeInstance *instance, unsigned char delimitedData)
{
    unsigned char delimitedData1[1];
    unsigned int rateInBytes = instance->rate/8;

    if (delimitedData == 0)
        return 1;
    if (instance->squeezing)
        return 1; // Too late for additional input

    delimitedData1[0] = delimitedData;
    #ifdef KeccakReference
    displayBytes(1, "Block to be absorbed (last few bits + first bit of padding)", delimitedData1, 1);
    #endif
    // Last few bits, whose delimiter coincides with first bit of padding
    SnP_XORBytes(instance->state, delimitedData1, instance->byteIOIndex, 1);
    // If the first bit of padding is at position rate-1, we need a whole new block for the second bit of padding
    if ((delimitedData >= 0x80) && (instance->byteIOIndex == (rateInBytes-1)))
        SnP_Permute(instance->state);
    // Second bit of padding
    SnP_ComplementBit(instance->state, rateInBytes*8-1);
    #ifdef KeccakReference
    {
        unsigned char block[SnP_width/8];
        memset(block, 0, SnP_width/8);
        block[rateInBytes-1] = 0x80;
        displayBytes(1, "Second bit of padding", block, rateInBytes);
    }
    #endif
    SnP_Permute(instance->state);
    instance->byteIOIndex = 0;
    instance->squeezing = 1;
    #ifdef KeccakReference
    displayText(1, "--- Switching to squeezing phase ---");
    #endif
    return 0;
}

/* ---------------------------------------------------------------- */

int Keccak_SpongeSqueeze(Keccak_SpongeInstance *instance, unsigned char *data, size_t dataByteLen)
{
    size_t i, j;
    unsigned int partialBlock;
    unsigned int rateInBytes = instance->rate/8;
    unsigned char *curData;

    if (!instance->squeezing)
        Keccak_SpongeAbsorbLastFewBits(instance, 0x01);

    i = 0;
    curData = data;
    while(i < dataByteLen) {
        if ((instance->byteIOIndex == rateInBytes) && (dataByteLen >= (i + rateInBytes))) {
            // processing full blocks first
            if ((rateInBytes % SnP_laneLengthInBytes) == 0) {
                // fast lane: whole lane rate
                j = SnP_FBWL_Squeeze(instance->state, rateInBytes/SnP_laneLengthInBytes, curData, dataByteLen - i);
                i += j;
                curData += j;
            }
            else {
                for(j=dataByteLen-i; j>=rateInBytes; j-=rateInBytes) {
                    SnP_Permute(instance->state);
                    SnP_ExtractBytes(instance->state, curData, 0, rateInBytes);
                    #ifdef KeccakReference
                    displayBytes(1, "Squeezed block", curData, rateInBytes);
                    #endif
                    curData+=rateInBytes;
                }
                i = dataByteLen - j;
            }
        }
        else {
            // normal lane: using the message queue
            if (instance->byteIOIndex == rateInBytes) {
                SnP_Permute(instance->state);
                instance->byteIOIndex = 0;
            }
            partialBlock = (unsigned int)(dataByteLen - i);
            if (partialBlock+instance->byteIOIndex > rateInBytes)
                partialBlock = rateInBytes-instance->byteIOIndex;
            i += partialBlock;

            SnP_ExtractBytes(instance->state, curData, instance->byteIOIndex, partialBlock);
            #ifdef KeccakReference
            displayBytes(1, "Squeezed block (part)", curData, partialBlock);
            #endif
            curData += partialBlock;
            instance->byteIOIndex += partialBlock;
        }
    }
    return 0;
}
