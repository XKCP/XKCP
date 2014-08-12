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

size_t PlSnP_FBWL_Absorb_Default(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int i;
    const unsigned char *dataStart = data;

    while(dataByteLen >= (laneOffsetParallel*PlSnP_P - laneOffsetParallel + laneCount)*SnP_laneLengthInBytes) {
        PlSnP_XORLanesAll(states, data, laneCount, laneOffsetParallel);
        for(i=0; i<PlSnP_P; i++)
            PlSnP_XORBytes(states, i, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        PlSnP_PermuteAll(states);
        data += laneOffsetSerial*SnP_laneLengthInBytes;
        dataByteLen -= laneOffsetSerial*SnP_laneLengthInBytes;
    }
    return data - dataStart;
}

size_t PlSnP_FBWL_Squeeze_Default(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen)
{
    const unsigned char *dataStart = data;

    while(dataByteLen >= (laneOffsetParallel*PlSnP_P - laneOffsetParallel + laneCount)*SnP_laneLengthInBytes) {
        PlSnP_PermuteAll(states);
        PlSnP_ExtractLanesAll(states, data, laneCount, laneOffsetParallel);
        data += laneOffsetSerial*SnP_laneLengthInBytes;
        dataByteLen -= laneOffsetSerial*SnP_laneLengthInBytes;
    }
    return data - dataStart;
}

size_t PlSnP_FBWL_Wrap_Default(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int i;
    const unsigned char *dataInStart = dataIn;

    while(dataByteLen >= (laneOffsetParallel*PlSnP_P - laneOffsetParallel + laneCount)*SnP_laneLengthInBytes) {
        PlSnP_XORLanesAll(states, dataIn, laneCount, laneOffsetParallel);
        PlSnP_ExtractLanesAll(states, dataOut, laneCount, laneOffsetParallel);
        for(i=0; i<PlSnP_P; i++)
            PlSnP_XORBytes(states, i, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        PlSnP_PermuteAll(states);
        dataIn += laneOffsetSerial*SnP_laneLengthInBytes;
        dataOut += laneOffsetSerial*SnP_laneLengthInBytes;
        dataByteLen -= laneOffsetSerial*SnP_laneLengthInBytes;
    }
    return dataIn - dataInStart;
}

size_t PlSnP_FBWL_Unwrap_Default(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int i;
    const unsigned char *dataInStart = dataIn;

    while(dataByteLen >= (laneOffsetParallel*PlSnP_P - laneOffsetParallel + laneCount)*SnP_laneLengthInBytes) {
        if (dataIn != dataOut) {
            if (laneOffsetParallel == laneCount)
                memcpy(dataOut, dataIn, laneCount*PlSnP_P*SnP_laneLengthInBytes);
            else {
                const unsigned char *curIn = dataIn;
                unsigned char *curOut = dataOut;
                for(i=0; i<PlSnP_P; i++) {
                    memcpy(curOut, curIn, laneCount*SnP_laneLengthInBytes);
                    curIn += laneOffsetParallel*SnP_laneLengthInBytes;
                    curOut += laneOffsetParallel*SnP_laneLengthInBytes;
                }
            }
        }
        PlSnP_ExtractAndXORLanesAll(states, dataOut, laneCount, laneOffsetParallel);
        PlSnP_XORLanesAll(states, dataOut, laneCount, laneOffsetParallel);
        for(i=0; i<PlSnP_P; i++)
            PlSnP_XORBytes(states, i, &trailingBits, laneCount*SnP_laneLengthInBytes, 1);
        PlSnP_PermuteAll(states);
        dataIn += laneOffsetSerial*SnP_laneLengthInBytes;
        dataOut += laneOffsetSerial*SnP_laneLengthInBytes;
        dataByteLen -= laneOffsetSerial*SnP_laneLengthInBytes;
    }
    return dataIn - dataInStart;
}
