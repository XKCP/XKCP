/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "Exception.h"
#include "PlSnP.h"

using namespace std;

PlSnP::PlSnP(RoundCountParameterizedPermutation* aF, unsigned int aNumberOfInstances, unsigned int aLaneSizeInBytes, const set<unsigned int>& aSupportedRoundCounts, unsigned int aDefaultRoundCount)
    : f(aF), numberOfInstances(aNumberOfInstances), laneSizeInBytes(aLaneSizeInBytes), supportedRoundCounts(aSupportedRoundCounts), defaultRoundCount(aDefaultRoundCount)
{
    widthInBytes = (f->getWidth() + 7) / 8;
    states.assign(numberOfInstances, vector<uint8_t>(widthInBytes, 0));
}

unsigned int PlSnP::getWidth() const
{
    return f->getWidth();
}

unsigned int PlSnP::getDefaultRoundCount() const
{
    return defaultRoundCount;
}

unsigned int PlSnP::getLaneSizeInBytes() const
{
    return laneSizeInBytes;
}

unsigned int PlSnP::getNumberOfInstances() const
{
    return numberOfInstances;
}

ostream& operator<<(ostream& a, const PlSnP& plsnp)
{
    a << "PlSnP on top of " << *(plsnp.f) << " with " << dec << plsnp.numberOfInstances << " instances in parallel";
    return a;
}

void PlSnP::InitializeAll()
{
    states.assign(numberOfInstances, vector<uint8_t>(widthInBytes, 0));
}

void PlSnP::AddByte(unsigned int instanceIndex, unsigned char data, unsigned int offset)
{
    if (instanceIndex >= numberOfInstances)
        throw Exception("instance index is out of range");
    else if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else
        states[instanceIndex][offset] ^= data;
}

void PlSnP::AddBytes(unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    if (instanceIndex >= numberOfInstances)
        throw Exception("instance index is out of range");
    else if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            AddByte(instanceIndex, data[i], offset + i);
    }
}

void PlSnP::AddLanesAll(const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    if (laneCount*laneSizeInBytes > widthInBytes)
        throw Exception("lane count must be not greater than the width in lanes");
    else
    for(unsigned int i=0; i<numberOfInstances; i++) {
        AddBytes(i, data, 0, laneCount*laneSizeInBytes);
        data += laneOffset*laneSizeInBytes;
    }
}

void PlSnP::OverwriteBytes(unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    if (instanceIndex >= numberOfInstances)
        throw Exception("instance index is out of range");
    else if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            states[instanceIndex][offset + i] = data[i];
    }
}

void PlSnP::OverwriteLanesAll(const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    if (laneCount*laneSizeInBytes > widthInBytes)
        throw Exception("lane count must be not greater than the width in lanes");
    else
    for(unsigned int i=0; i<numberOfInstances; i++) {
        OverwriteBytes(i, data, 0, laneCount*laneSizeInBytes);
        data += laneOffset*laneSizeInBytes;
    }
}

void PlSnP::OverwriteWithZeroes(unsigned int instanceIndex, unsigned int byteCount)
{
    if (instanceIndex >= numberOfInstances)
        throw Exception("instance index is out of range");
    else if (byteCount > widthInBytes)
        throw Exception("byte count must be less than or equal to the permutation width in bytes");
    else {
        for(unsigned int i=0; i<byteCount; i++)
            states[instanceIndex][i] = 0;
    }
}

void PlSnP::PermuteAll()
{
    for(unsigned int i=0; i<numberOfInstances; i++)
        f->apply(states[i], defaultRoundCount);
}

void PlSnP::PermuteAll_Nrounds(unsigned int nrounds)
{
    if (supportedRoundCounts.find(nrounds) == supportedRoundCounts.end())
        throw Exception("nrounds must be a supported number of rounds");
    else
        for(unsigned int i=0; i<numberOfInstances; i++)
            f->apply(states[i], nrounds);
}

void PlSnP::ExtractBytes(unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length) const
{
    if (instanceIndex >= numberOfInstances)
        throw Exception("instance index is out of range");
    else if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            data[i] = states[instanceIndex][offset + i];
    }
}

void PlSnP::ExtractLanesAll(unsigned char *data, unsigned int laneCount, unsigned int laneOffset) const
{
    if (laneCount*laneSizeInBytes > widthInBytes)
        throw Exception("lane count must be not greater than the width in lanes");
    else
    for(unsigned int i=0; i<numberOfInstances; i++) {
        ExtractBytes(i, data, 0, laneCount*laneSizeInBytes);
        data += laneOffset*laneSizeInBytes;
    }
}

void PlSnP::ExtractAndAddBytes(unsigned int instanceIndex, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length) const
{
    if (instanceIndex >= numberOfInstances)
        throw Exception("instance index is out of range");
    else if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            output[i] = input[i] ^ states[instanceIndex][offset + i];
    }
}

void PlSnP::ExtractAndAddLanesAll(const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset) const
{
    if (laneCount*laneSizeInBytes > widthInBytes)
        throw Exception("lane count must be not greater than the width in lanes");
    else
    for(unsigned int i=0; i<numberOfInstances; i++) {
        ExtractAndAddBytes(i, input, output, 0, laneCount*laneSizeInBytes);
        input += laneOffset*laneSizeInBytes;
        output += laneOffset*laneSizeInBytes;
    }
}

size_t PlSnP::FastLoop_Absorb(unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen)
{
    (void)laneCount;
    (void)laneOffsetParallel;
    (void)laneOffsetSerial;
    (void)data;
    (void)dataByteLen;
    // TODO
    return 0;
}
