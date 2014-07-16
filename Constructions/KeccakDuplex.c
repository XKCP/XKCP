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
#include "KeccakDuplex.h"
#include "SnP-interface.h"
#ifdef KeccakReference
#include "displayIntermediateValues.h"
#endif

int Keccak_DuplexInitialize(Keccak_DuplexInstance *instance, unsigned int rate, unsigned int capacity)
{
    if (rate+capacity != SnP_width)
        return 1;
    if ((rate <= 2) || (rate > SnP_width))
        return 1;
    SnP_StaticInitialize();
    instance->rate = rate;
    SnP_Initialize(instance->state);
    instance->byteInputIndex = 0;
    instance->byteOutputIndex = (instance->rate+7)/8;
    return 0;
}

int Keccak_Duplexing(Keccak_DuplexInstance *instance, const unsigned char *sigmaBegin, unsigned int sigmaBeginByteLen, unsigned char *Z, unsigned int ZByteLen, unsigned char delimitedSigmaEnd)
{
    unsigned char delimitedSigmaEnd1[1];
    const unsigned int rho_max = instance->rate - 2;
    
    if (delimitedSigmaEnd == 0)
        return 1;
    if ((instance->byteInputIndex+sigmaBeginByteLen)*8 > rho_max)
        return 1;
    if (rho_max - sigmaBeginByteLen*8 < 7) {
        unsigned int maxBitsInDelimitedSigmaEnd = rho_max - sigmaBeginByteLen*8;
        if (delimitedSigmaEnd >= (1 << (maxBitsInDelimitedSigmaEnd+1)))
            return 1;
    }
    if (ZByteLen > (instance->rate+7)/8)
        return 1; // The output length must not be greater than the rate (rounded up to a byte)

    SnP_XORBytes(instance->state, sigmaBegin, instance->byteInputIndex, sigmaBeginByteLen);
    #ifdef KeccakReference
    {
        unsigned char block[SnP_width/8];
        memcpy(block, sigmaBegin, sigmaBeginByteLen);
        block[sigmaBeginByteLen] = delimitedSigmaEnd;
        memset(block+sigmaBeginByteLen+1, 0, sizeof(block)-sigmaBeginByteLen-1);
        block[(instance->rate-1)/8] |= 1 << ((instance->rate-1) % 8);
        displayBytes(1, "Block to be absorbed (after padding)", block, (instance->rate+7)/8);
    }
    #endif

    delimitedSigmaEnd1[0] = delimitedSigmaEnd;
    // Last few bits, whose delimiter coincides with first bit of padding
    SnP_XORBytes(instance->state, delimitedSigmaEnd1, instance->byteInputIndex+sigmaBeginByteLen, 1);
    // Second bit of padding
    SnP_ComplementBit(instance->state, instance->rate - 1);
    SnP_Permute(instance->state);
    SnP_ExtractBytes(instance->state, Z, 0, ZByteLen);

    if (ZByteLen*8 > instance->rate) {
        unsigned char mask = (unsigned char)(1 << (instance->rate % 8)) - 1;
        Z[ZByteLen-1] &= mask;
    }

    instance->byteInputIndex = 0;
    instance->byteOutputIndex = ZByteLen;

    return 0;
}

int Keccak_DuplexingFeedPartialInput(Keccak_DuplexInstance *instance, const unsigned char *in, unsigned int inByteLen)
{
    const unsigned int rho_max = instance->rate - 2;

    if ((instance->byteInputIndex+inByteLen)*8 > rho_max)
        return 1;

    SnP_XORBytes(instance->state, in, instance->byteInputIndex, inByteLen);
    instance->byteInputIndex += inByteLen;
    return 0;
}

int Keccak_DuplexingFeedZeroes(Keccak_DuplexInstance *instance, unsigned int inByteLen)
{
    const unsigned int rho_max = instance->rate - 2;

    if ((instance->byteInputIndex+inByteLen)*8 > rho_max)
        return 1;

    instance->byteInputIndex += inByteLen;
    return 0;
}

int Keccak_DuplexingOverwritePartialInput(Keccak_DuplexInstance *instance, const unsigned char *in, unsigned int inByteLen)
{
    const unsigned int rho_max = instance->rate - 2;

    if ((instance->byteInputIndex+inByteLen)*8 > rho_max)
        return 1;

    SnP_OverwriteBytes(instance->state, in, instance->byteInputIndex, inByteLen);
    instance->byteInputIndex += inByteLen;
    return 0;
}

int Keccak_DuplexingOverwriteWithZeroes(Keccak_DuplexInstance *instance, unsigned int inByteLen)
{
    const unsigned int rho_max = instance->rate - 2;

    if ((instance->byteInputIndex != 0) || (inByteLen*8 > rho_max))
        return 1;

    SnP_OverwriteWithZeroes(instance->state, inByteLen);
    instance->byteInputIndex = inByteLen;

    return 0;
}

int Keccak_DuplexingGetFurtherOutput(Keccak_DuplexInstance *instance, unsigned char *out, unsigned int outByteLen)
{
    if ((outByteLen+instance->byteOutputIndex) > (instance->rate+7)/8)
        return 1; // The output length must not be greater than the rate (rounded up to a byte)

    SnP_ExtractBytes(instance->state, out, instance->byteOutputIndex, outByteLen);
    instance->byteOutputIndex += outByteLen;
    if (instance->byteOutputIndex*8 > instance->rate) {
        unsigned char mask = (1 << (instance->rate % 8)) - 1;
        out[outByteLen-1] &= mask;
    }
    return 0;
}

int Keccak_DuplexingGetFurtherOutputAndXOR(Keccak_DuplexInstance *instance, unsigned char *out, unsigned int outByteLen)
{
    if ((outByteLen+instance->byteOutputIndex) > (instance->rate+7)/8)
        return 1; // The output length must not be greater than the rate (rounded up to a byte)

    SnP_ExtractAndXORBytes(instance->state, out, instance->byteOutputIndex, outByteLen);
    instance->byteOutputIndex += outByteLen;
    if (instance->byteOutputIndex*8 > instance->rate) {
        unsigned char mask = (1 << (instance->rate % 8)) - 1;
        out[outByteLen-1] &= mask;
    }
    return 0;
}

size_t Keccak_DuplexingFBWLAbsorb(Keccak_DuplexInstance *instance, const unsigned char *dataIn, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int laneCount = instance->rate/(8*SnP_laneLengthInBytes);
    unsigned int rateModuloLaneSize = instance->rate - laneCount*(8*SnP_laneLengthInBytes);
    size_t result;

    if (laneCount == 0)
        return 0;
    if ((rateModuloLaneSize < 2) || (rateModuloLaneSize > 8))
        return 0;
    if ((trailingBits == 0) || (trailingBits >= ((unsigned char)1 << (rateModuloLaneSize-1))))
        return 0;
    trailingBits |= (unsigned char)1 << (rateModuloLaneSize-1);
    if (instance->byteInputIndex != 0)
        return 0;

    result = SnP_FBWL_Absorb(instance->state, laneCount, dataIn, dataByteLen, trailingBits);
    if (result > 0)
        instance->byteOutputIndex = 0;
    return result;
}

size_t Keccak_DuplexingFBWLWrap(Keccak_DuplexInstance *instance, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int laneCount = instance->rate/(8*SnP_laneLengthInBytes);
    unsigned int rateModuloLaneSize = instance->rate - laneCount*(8*SnP_laneLengthInBytes);

    if (laneCount == 0)
        return 0;
    if ((rateModuloLaneSize < 2) || (rateModuloLaneSize > 8))
        return 0;
    if ((trailingBits == 0) || (trailingBits >= ((unsigned char)1 << (rateModuloLaneSize-1))))
        return 0;
    trailingBits |= (unsigned char)1 << (rateModuloLaneSize-1);
    if ((instance->byteInputIndex != 0) || (instance->byteOutputIndex != 0))
        return 0;

    return SnP_FBWL_Wrap(instance->state, laneCount, dataIn, dataOut, dataByteLen, trailingBits);
}

size_t Keccak_DuplexingFBWLUnwrap(Keccak_DuplexInstance *instance, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int laneCount = instance->rate/(8*SnP_laneLengthInBytes);
    unsigned int rateModuloLaneSize = instance->rate - laneCount*(8*SnP_laneLengthInBytes);

    if (laneCount == 0)
        return 0;
    if ((rateModuloLaneSize < 2) || (rateModuloLaneSize > 8))
        return 0;
    if ((trailingBits == 0) || (trailingBits >= ((unsigned char)1 << (rateModuloLaneSize-1))))
        return 0;
    trailingBits |= (unsigned char)1 << (rateModuloLaneSize-1);
    if ((instance->byteInputIndex != 0) || (instance->byteOutputIndex != 0))
        return 0;

    return SnP_FBWL_Unwrap(instance->state, laneCount, dataIn, dataOut, dataByteLen, trailingBits);
}

unsigned int Keccak_DuplexGetInputIndex(Keccak_DuplexInstance *instance)
{
    return instance->byteInputIndex;
}

unsigned int Keccak_DuplexGetOutputIndex(Keccak_DuplexInstance *instance)
{
    return instance->byteOutputIndex;
}
