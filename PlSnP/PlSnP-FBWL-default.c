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
#include "PlSnP-interface.h"

size_t PlSnP_FBWL_Absorb_Default(void *states, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int i;
    size_t processed = 0;

    while(dataByteLen >= laneCount*PlSnP_P*SnP_laneLengthInBytes) {
        PlSnP_XORLanesAll(states, data, laneCount);
        for(i=0; i<PlSnP_P; i++)
            PlSnP_XORBytes(states, i, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        PlSnP_PermuteAll(states);
        data += laneCount*PlSnP_P*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*PlSnP_P*SnP_laneLengthInBytes;
        processed += laneCount*PlSnP_P*SnP_laneLengthInBytes;
    }
    return processed;
}

size_t PlSnP_FBWL_Squeeze_Default(void *states, unsigned int laneCount, unsigned char *data, size_t dataByteLen)
{
    size_t processed = 0;

    while(dataByteLen >= laneCount*PlSnP_P*SnP_laneLengthInBytes) {
        PlSnP_PermuteAll(states);
        PlSnP_ExtractLanesAll(states, data, laneCount);
        data += laneCount*PlSnP_P*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*PlSnP_P*SnP_laneLengthInBytes;
        processed += laneCount*PlSnP_P*SnP_laneLengthInBytes;
    }
    return processed;
}

size_t PlSnP_FBWL_Wrap_Default(void *states, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int i;
    size_t processed = 0;

    while(dataByteLen >= laneCount*PlSnP_P*SnP_laneLengthInBytes) {
        PlSnP_XORLanesAll(states, dataIn, laneCount);
        PlSnP_ExtractLanesAll(states, dataOut, laneCount);
        for(i=0; i<PlSnP_P; i++)
            PlSnP_XORBytes(states, i, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        PlSnP_PermuteAll(states);
        dataIn += laneCount*PlSnP_P*SnP_laneLengthInBytes;
        dataOut += laneCount*PlSnP_P*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*PlSnP_P*SnP_laneLengthInBytes;
        processed += laneCount*PlSnP_P*SnP_laneLengthInBytes;
    }
    return processed;
}

size_t PlSnP_FBWL_Unwrap_Default(void *states, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int i;
    size_t processed = 0;

    while(dataByteLen >= laneCount*PlSnP_P*SnP_laneLengthInBytes) {
        if (dataIn != dataOut)
            memcpy(dataOut, dataIn, laneCount*PlSnP_P*SnP_laneLengthInBytes);
        PlSnP_ExtractAndXORLanesAll(states, dataOut, laneCount);
        PlSnP_XORLanesAll(states, dataOut, laneCount);
        for(i=0; i<PlSnP_P; i++)
            PlSnP_XORBytes(states, i, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        PlSnP_PermuteAll(states);
        dataIn += laneCount*PlSnP_P*SnP_laneLengthInBytes;
        dataOut += laneCount*PlSnP_P*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*PlSnP_P*SnP_laneLengthInBytes;
        processed += laneCount*PlSnP_P*SnP_laneLengthInBytes;
    }
    return processed;
}
