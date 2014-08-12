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
#include "KeccakParallelDuplex.h"
#include "PlSnP-interface.h"

int Keccak_ParallelDuplexInitializeAll(Keccak_ParallelDuplexInstances *instances, unsigned int rate, unsigned int capacity)
{
    unsigned int i;

    if (rate+capacity != SnP_width)
        return 1;
    if ((rate <= 2) || (rate > SnP_width))
        return 1;
    PlSnP_StaticInitialize();
    instances->rate = rate;
    PlSnP_InitializeAll(instances->states);
    for(i=0; i<PlSnP_P; i++) {
        instances->byteInputIndex[i] = 0;
        instances->byteOutputIndex[i] = (instances->rate+7)/8;
    }
    return 0;
}

int Keccak_ParallelDuplexingAll(Keccak_ParallelDuplexInstances *instances, unsigned char delimitedSigmaEnd)
{
    unsigned char delimitedSigmaEnd1[1];
    const unsigned int rho_max = instances->rate - 2;
    unsigned int i;

    if (delimitedSigmaEnd == 0)
        return 1;

    delimitedSigmaEnd1[0] = delimitedSigmaEnd;
    // Last few bits, whose delimiter coincides with first bit of padding
    for(i=0; i<PlSnP_P; i++)
        PlSnP_XORBytes(instances->states, i, delimitedSigmaEnd1, instances->byteInputIndex[i], 1);
    // Second bit of padding
    PlSnP_ComplementBitAll(instances->states, instances->rate - 1);

    PlSnP_PermuteAll(instances->states);

    for(i=0; i<PlSnP_P; i++) {
        instances->byteInputIndex[i] = 0;
        instances->byteOutputIndex[i] = 0;
    }
    return 0;
}

int Keccak_ParallelDuplexing(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex, unsigned char delimitedSigmaEnd)
{
    unsigned char delimitedSigmaEnd1[1];
    const unsigned int rho_max = instances->rate - 2;
    unsigned int i;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if (delimitedSigmaEnd == 0)
        return 1;

    delimitedSigmaEnd1[0] = delimitedSigmaEnd;
    // Last few bits, whose delimiter coincides with first bit of padding
    PlSnP_XORBytes(instances->states, instanceIndex, delimitedSigmaEnd1, instances->byteInputIndex[instanceIndex], 1);
    // Second bit of padding
    PlSnP_ComplementBit(instances->states, instanceIndex, instances->rate - 1);

    PlSnP_Permute(instances->states, instanceIndex);

    instances->byteInputIndex[instanceIndex] = 0;
    instances->byteOutputIndex[instanceIndex] = 0;
    return 0;
}

int Keccak_ParallelDuplexingFeedPartialInterleavedInput(Keccak_ParallelDuplexInstances *instances, const unsigned char *in, unsigned int inByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    const unsigned int rho_maxInBytes = rho_max/8;
    unsigned int i;
    unsigned int totalInputIndex = 0;
    unsigned int localSize;

    for(i=0; i<PlSnP_P; i++)
        totalInputIndex += instances->byteInputIndex[i];
    if (totalInputIndex+inByteLen > rho_maxInBytes*PlSnP_P)
        return 1;

    if ((totalInputIndex == 0) && ((rho_maxInBytes % KeccakF_laneInBytes) == 0) && (inByteLen == rho_maxInBytes*PlSnP_P)) {
        PlSnP_XORLanesAll(instances->states, in, rho_maxInBytes/KeccakF_laneInBytes, rho_maxInBytes/KeccakF_laneInBytes);
        for(i=0; i<PlSnP_P; i++)
            instances->byteInputIndex[i] = rho_maxInBytes;
        return 0;
    }

    i = 0;
    while((instances->byteInputIndex[i] == rho_maxInBytes) && (i < PlSnP_P))
        i++;
    while(inByteLen > 0) {
        if (i >= PlSnP_P)
            return 1;
        localSize = rho_maxInBytes - instances->byteInputIndex[i];
        if (localSize > inByteLen)
            localSize = inByteLen;
        PlSnP_XORBytes(instances->states, i, in, instances->byteInputIndex[i], localSize);
        in += localSize;
        instances->byteInputIndex[i] += localSize;
        inByteLen -= localSize;
        if (instances->byteInputIndex[i] == rho_maxInBytes)
            i++;
    }
    return 0;
}

int Keccak_ParallelDuplexingFeedPartialSingleInput(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex, const unsigned char *in, unsigned int inByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    unsigned int localSize;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if ((instances->byteInputIndex[instanceIndex]+inByteLen)*8 > rho_max)
        return 1;

    PlSnP_XORBytes(instances->states, instanceIndex, in, instances->byteInputIndex[instanceIndex], inByteLen);
    instances->byteInputIndex[instanceIndex] += inByteLen;
    return 0;
}

int Keccak_ParallelDuplexingFeedZeroes(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex, unsigned int inByteLen)
{
    const unsigned int rho_max = instances->rate - 2;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if ((instances->byteInputIndex[instanceIndex]+inByteLen)*8 > rho_max)
        return 1;

    instances->byteInputIndex[instanceIndex] += inByteLen;
    return 0;
}

int Keccak_ParallelDuplexingOverwritePartialInterleavedInput(Keccak_ParallelDuplexInstances *instances, const unsigned char *in, unsigned int inByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    const unsigned int rho_maxInBytes = rho_max/8;
    unsigned int i;
    unsigned int totalInputIndex = 0;
    unsigned int localSize;

    for(i=0; i<PlSnP_P; i++)
        totalInputIndex += instances->byteInputIndex[i];
    if (totalInputIndex+inByteLen > rho_maxInBytes*PlSnP_P)
        return 1;

    if ((totalInputIndex == 0) && ((rho_maxInBytes % KeccakF_laneInBytes) == 0) && (inByteLen == rho_maxInBytes*PlSnP_P)) {
        PlSnP_OverwriteLanesAll(instances->states, in, rho_maxInBytes/KeccakF_laneInBytes, rho_maxInBytes/KeccakF_laneInBytes);
        for(i=0; i<PlSnP_P; i++)
            instances->byteInputIndex[i] = rho_maxInBytes;
        return 0;
    }

    i = 0;
    while((instances->byteInputIndex[i] == rho_maxInBytes) && (i < PlSnP_P))
        i++;
    while(inByteLen > 0) {
        if (i >= PlSnP_P)
            return 1;
        localSize = rho_maxInBytes - instances->byteInputIndex[i];
        if (localSize > inByteLen)
            localSize = inByteLen;
        PlSnP_OverwriteBytes(instances->states, i, in, instances->byteInputIndex[i], localSize);
        in += localSize;
        instances->byteInputIndex[i] += localSize;
        inByteLen -= localSize;
        if (instances->byteInputIndex[i] == rho_maxInBytes)
            i++;
    }
    return 0;
}

int Keccak_ParallelDuplexingOverwritePartialSingleInput(Keccak_ParallelDuplexInstances *instances,  unsigned int instanceIndex, const unsigned char *in, unsigned int inByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    unsigned int localSize;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if ((instances->byteInputIndex[instanceIndex]+inByteLen)*8 > rho_max)
        return 1;

    PlSnP_OverwriteBytes(instances->states, instanceIndex, in, instances->byteInputIndex[instanceIndex], inByteLen);
    instances->byteInputIndex[instanceIndex] += inByteLen;
    return 0;
}

int Keccak_ParallelDuplexingOverwriteWithZeroes(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex, unsigned int inByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    unsigned int i;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if (instances->byteInputIndex[instanceIndex] != 0)
        return 1;
    if (inByteLen*8 > rho_max)
        return 1;

    PlSnP_OverwriteWithZeroes(instances->states, instanceIndex, inByteLen);
    instances->byteInputIndex[instanceIndex] += inByteLen;

    return 0;
}

int Keccak_ParallelDuplexingGetFurtherInterleavedOutput(Keccak_ParallelDuplexInstances *instances, unsigned char *out, unsigned int outByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    const unsigned int rho_maxInBytes = rho_max/8;
    unsigned int i;
    unsigned int totalOutputIndex = 0;
    unsigned int localSize;

    for(i=0; i<PlSnP_P; i++)
        totalOutputIndex += instances->byteOutputIndex[i];
    if (totalOutputIndex+outByteLen > rho_maxInBytes*PlSnP_P)
        return 1;

    if ((totalOutputIndex == 0) && ((rho_maxInBytes % KeccakF_laneInBytes) == 0) && (outByteLen == rho_maxInBytes*PlSnP_P)) {
        PlSnP_ExtractLanesAll(instances->states, out, rho_maxInBytes/KeccakF_laneInBytes);
        for(i=0; i<PlSnP_P; i++)
            instances->byteOutputIndex[i] = rho_maxInBytes;
        return 0;
    }

    i = 0;
    while((instances->byteOutputIndex[i] == rho_maxInBytes) && (i < PlSnP_P))
        i++;
    while(outByteLen > 0) {
        if (i >= PlSnP_P)
            return 1;
        localSize = rho_maxInBytes - instances->byteOutputIndex[i];
        if (localSize > outByteLen)
            localSize = outByteLen;
        PlSnP_ExtractBytes(instances->states, i, out, instances->byteOutputIndex[i], localSize);
        out += localSize;
        instances->byteOutputIndex[i] += localSize;
        outByteLen -= localSize;
        if (instances->byteOutputIndex[i] == rho_maxInBytes)
            i++;
    }
    return 0;
}

int Keccak_ParallelDuplexingGetFurtherSingleOutput(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex, unsigned char *out, unsigned int outByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    unsigned int localSize;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if ((instances->byteOutputIndex[instanceIndex]+outByteLen)*8 > rho_max)
        return 1;

    PlSnP_ExtractBytes(instances->states, instanceIndex, out, instances->byteOutputIndex[instanceIndex], outByteLen);
    instances->byteOutputIndex[instanceIndex] += outByteLen;
    return 0;
}

int Keccak_ParallelDuplexingGetFurtherInterleavedOutputAndXOR(Keccak_ParallelDuplexInstances *instances, unsigned char *out, unsigned int outByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    const unsigned int rho_maxInBytes = rho_max/8;
    unsigned int i;
    unsigned int totalOutputIndex = 0;
    unsigned int localSize;

    for(i=0; i<PlSnP_P; i++)
        totalOutputIndex += instances->byteOutputIndex[i];
    if (totalOutputIndex+outByteLen > rho_maxInBytes*PlSnP_P)
        return 1;

    if ((totalOutputIndex == 0) && ((rho_maxInBytes % KeccakF_laneInBytes) == 0) && (outByteLen == rho_maxInBytes*PlSnP_P)) {
        PlSnP_ExtractAndXORLanesAll(instances->states, out, rho_maxInBytes/KeccakF_laneInBytes, rho_maxInBytes/KeccakF_laneInBytes);
        for(i=0; i<PlSnP_P; i++)
            instances->byteOutputIndex[i] = rho_maxInBytes;
        return 0;
    }

    i = 0;
    while((instances->byteOutputIndex[i] == rho_maxInBytes) && (i < PlSnP_P))
        i++;
    while(outByteLen > 0) {
        if (i >= PlSnP_P)
            return 1;
        localSize = rho_maxInBytes - instances->byteOutputIndex[i];
        if (localSize > outByteLen)
            localSize = outByteLen;
        PlSnP_ExtractAndXORBytes(instances->states, i, out, instances->byteOutputIndex[i], localSize);
        out += localSize;
        instances->byteOutputIndex[i] += localSize;
        outByteLen -= localSize;
        if (instances->byteOutputIndex[i] == rho_maxInBytes)
            i++;
    }
    return 0;
}

int Keccak_ParallelDuplexingGetFurtherSingleOutputAndXOR(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex, unsigned char *out, unsigned int outByteLen)
{
    const unsigned int rho_max = instances->rate - 2;
    unsigned int localSize;

    if (instanceIndex >= PlSnP_P)
        return 1;
    if ((instances->byteOutputIndex[instanceIndex]+outByteLen)*8 > rho_max)
        return 1;

    PlSnP_ExtractAndXORBytes(instances->states, instanceIndex, out, instances->byteOutputIndex[instanceIndex], outByteLen);
    instances->byteOutputIndex[instanceIndex] += outByteLen;
    return 0;
}

size_t Keccak_ParallelDuplexingFBWLAbsorb(Keccak_ParallelDuplexInstances *instances, const unsigned char *dataIn, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int laneCount = instances->rate/(8*SnP_laneLengthInBytes);
    unsigned int rateModuloLaneSize = instances->rate - laneCount*(8*SnP_laneLengthInBytes);
    unsigned int totalIndexes;
    unsigned int i;

    if (laneCount == 0)
        return 0;
    if ((rateModuloLaneSize < 2) || (rateModuloLaneSize > 8))
        return 0;
    if ((trailingBits == 0) || (trailingBits >= ((unsigned char)1 << (rateModuloLaneSize-1))))
        return 0;
    trailingBits |= (unsigned char)1 << (rateModuloLaneSize-1);
    totalIndexes = 0;
    for(i=0; i<PlSnP_P; i++)
        totalIndexes += instances->byteInputIndex[i] + instances->byteOutputIndex[i];
    if (totalIndexes != 0)
        return 0;

    return PlSnP_FBWL_Absorb(instances->states, laneCount, laneCount, PlSnP_P*laneCount, dataIn, dataByteLen, trailingBits);
}

size_t Keccak_ParallelDuplexingFBWLWrap(Keccak_ParallelDuplexInstances *instances, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int laneCount = instances->rate/(8*SnP_laneLengthInBytes);
    unsigned int rateModuloLaneSize = instances->rate - laneCount*(8*SnP_laneLengthInBytes);
    unsigned int i;
    size_t result;

    if (laneCount == 0)
        return 0;
    if ((rateModuloLaneSize < 2) || (rateModuloLaneSize > 8))
        return 0;
    if ((trailingBits == 0) || (trailingBits >= ((unsigned char)1 << (rateModuloLaneSize-1))))
        return 0;
    trailingBits |= (unsigned char)1 << (rateModuloLaneSize-1);
    if (Keccak_ParallelDuplexGetTotalInputIndex(instances) != 0)
        return 0;

    result = PlSnP_FBWL_Wrap(instances->states, laneCount, laneCount, PlSnP_P*laneCount, dataIn, dataOut, dataByteLen, trailingBits);
    if (result > 0)
        for(i=0; i<PlSnP_P; i++)
            instances->byteOutputIndex[i] = 0;
    return result;
}

size_t Keccak_ParallelDuplexingFBWLUnwrap(Keccak_ParallelDuplexInstances *instances, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    unsigned int laneCount = instances->rate/(8*SnP_laneLengthInBytes);
    unsigned int rateModuloLaneSize = instances->rate - laneCount*(8*SnP_laneLengthInBytes);
    unsigned int totalIndexes;
    unsigned int i;

    if (laneCount == 0)
        return 0;
    if ((rateModuloLaneSize < 2) || (rateModuloLaneSize > 8))
        return 0;
    if ((trailingBits == 0) || (trailingBits >= ((unsigned char)1 << (rateModuloLaneSize-1))))
        return 0;
    trailingBits |= (unsigned char)1 << (rateModuloLaneSize-1);
    totalIndexes = 0;
    for(i=0; i<PlSnP_P; i++)
        totalIndexes += instances->byteInputIndex[i] + instances->byteOutputIndex[i];
    if (totalIndexes != 0)
        return 0;

    return PlSnP_FBWL_Unwrap(instances->states, laneCount, laneCount, PlSnP_P*laneCount, dataIn, dataOut, dataByteLen, trailingBits);
}

unsigned int Keccak_ParallelDuplexGetTotalInputIndex(Keccak_ParallelDuplexInstances *instances)
{
    unsigned int totalInputIndex = 0;
    unsigned int i;

    for(i=0; i<PlSnP_P; i++)
        totalInputIndex += instances->byteInputIndex[i];
    return totalInputIndex;
}

unsigned int Keccak_ParallelDuplexGetInputIndex(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex)
{
    if (instanceIndex >= PlSnP_P)
        return 0;
    else
        return instances->byteInputIndex[instanceIndex];
}

unsigned int Keccak_ParallelDuplexGetOutputIndex(Keccak_ParallelDuplexInstances *instances, unsigned int instanceIndex)
{
    if (instanceIndex >= PlSnP_P)
        return 0;
    else
        return instances->byteOutputIndex[instanceIndex];
}
