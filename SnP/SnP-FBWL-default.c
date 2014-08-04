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
#include "SnP-interface.h"
#ifdef KeccakReference
#include "displayIntermediateValues.h"
#endif

size_t SnP_FBWL_Absorb_Default(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits)
{
    size_t processed = 0;

    while(dataByteLen >= laneCount*SnP_laneLengthInBytes) {
        #ifdef KeccakReference
        if (trailingBits == 0)
            displayBytes(1, "Block to be absorbed", data, laneCount*SnP_laneLengthInBytes);
        else {
            displayBytes(1, "Block to be absorbed (part)", data, laneCount*SnP_laneLengthInBytes);
            displayBytes(1, "Block to be absorbed (trailing bits)", &trailingBits, 1);
        }
        #endif
        SnP_XORBytes(state, data, 0, laneCount*SnP_laneLengthInBytes);
        SnP_XORBytes(state, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        SnP_Permute(state);
        data += laneCount*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*SnP_laneLengthInBytes;
        processed += laneCount*SnP_laneLengthInBytes;
    }
    return processed;
}

size_t SnP_FBWL_Squeeze_Default(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen)
{
    size_t processed = 0;

    while(dataByteLen >= laneCount*SnP_laneLengthInBytes) {
        SnP_Permute(state);
        SnP_ExtractBytes(state, data, 0, laneCount*SnP_laneLengthInBytes);
        #ifdef KeccakReference
        displayBytes(1, "Squeezed block", data, laneCount*SnP_laneLengthInBytes);
        #endif
        data += laneCount*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*SnP_laneLengthInBytes;
        processed += laneCount*SnP_laneLengthInBytes;
    }
    return processed;
}

size_t SnP_FBWL_Wrap_Default(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    size_t processed = 0;

    while(dataByteLen >= laneCount*SnP_laneLengthInBytes) {
        SnP_XORBytes(state, dataIn, 0, laneCount*SnP_laneLengthInBytes);
        SnP_ExtractBytes(state, dataOut, 0, laneCount*SnP_laneLengthInBytes);
        SnP_XORBytes(state, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        SnP_Permute(state);
        dataIn += laneCount*SnP_laneLengthInBytes;
        dataOut += laneCount*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*SnP_laneLengthInBytes;
        processed += laneCount*SnP_laneLengthInBytes;
    }
    return processed;
}

size_t SnP_FBWL_Unwrap_Default(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    size_t processed = 0;

    if (dataIn != dataOut)
        memcpy(dataOut, dataIn, dataByteLen);
    while(dataByteLen >= laneCount*SnP_laneLengthInBytes) {
        SnP_ExtractAndXORBytes(state, dataOut, 0, laneCount*SnP_laneLengthInBytes);
        SnP_XORBytes(state, dataOut, 0, laneCount*SnP_laneLengthInBytes);
        SnP_XORBytes(state, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        SnP_Permute(state);
        dataIn += laneCount*SnP_laneLengthInBytes;
        dataOut += laneCount*SnP_laneLengthInBytes;
        dataByteLen -= laneCount*SnP_laneLengthInBytes;
        processed += laneCount*SnP_laneLengthInBytes;
    }
    return processed;
}
