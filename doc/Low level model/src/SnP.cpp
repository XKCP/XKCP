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
#include "SnP.h"

using namespace std;

SnP::SnP(RoundCountParameterizedPermutation* aF, unsigned int aLaneSizeInBytes, const set<unsigned int>& aSupportedRoundCounts, unsigned int aDefaultRoundCount)
    : f(aF), laneSizeInBytes(aLaneSizeInBytes), supportedRoundCounts(aSupportedRoundCounts), defaultRoundCount(aDefaultRoundCount)
{
    widthInBytes = (f->getWidth() + 7) / 8;
    state.assign(widthInBytes, 0);
}

unsigned int SnP::getWidth() const
{
    return f->getWidth();
}

unsigned int SnP::getDefaultRoundCount() const
{
    return defaultRoundCount;
}

unsigned int SnP::getLaneSizeInBytes() const
{
    return laneSizeInBytes;
}

ostream& operator<<(ostream& a, const SnP& snp)
{
    a << "SnP on top of " << *(snp.f);
    return a;
}

void SnP::Initialize()
{
    state.assign(widthInBytes, 0);
}

void SnP::AddByte(unsigned char data, unsigned int offset)
{
    if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else
        state[offset] ^= data;
}

void SnP::AddBytes(const unsigned char *data, unsigned int offset, unsigned int length)
{
    if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            AddByte(data[i], offset + i);
    }
}

void SnP::OverwriteBytes(const unsigned char *data, unsigned int offset, unsigned int length)
{
    if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            state[offset + i] = data[i];
    }
}

void SnP::OverwriteWithZeroes(unsigned int byteCount)
{
    if (byteCount > widthInBytes)
        throw Exception("byteCount must be less than or equal to the permutation width in bytes");
    else {
        for(unsigned int i=0; i<byteCount; i++)
            state[i] = 0;
    }
}

void SnP::Permute()
{
    f->apply(state, defaultRoundCount);
}

void SnP::Permute_Nrounds(unsigned int nrounds)
{
    if (supportedRoundCounts.find(nrounds) == supportedRoundCounts.end())
        throw Exception("nrounds must be a supported number of rounds");
    else
        f->apply(state, nrounds);
}

void SnP::ExtractBytes(unsigned char *data, unsigned int offset, unsigned int length) const
{
    if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            data[i] = state[offset + i];
    }
}

void SnP::ExtractAndAddBytes(const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length) const
{
    if (offset >= widthInBytes)
        throw Exception("offset must be less than the width in bytes");
    else if ((offset+length) > widthInBytes)
        throw Exception("offset+length must be not greater than the width in bytes");
    else {
        for(unsigned int i=0; i<length; i++)
            output[i] = input[i] ^ state[offset + i];
    }
}

size_t SnP::FastLoop_Absorb(unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
{
    size_t processed = 0;

    while(dataByteLen >= laneCount*laneSizeInBytes) {
        AddBytes(data, 0, laneCount*laneSizeInBytes);
        Permute();
        data += laneCount*laneSizeInBytes;
        dataByteLen -= laneCount*laneSizeInBytes;
        processed += laneCount*laneSizeInBytes;
    }
    return processed;
}

SnP_KeccakP::SnP_KeccakP(unsigned int width)
    : SnP(new KeccakP(width), 0, {}, 0)
{
    laneSizeInBytes = ((KeccakP*)(f.get()))->getLaneSize() / 8;
    defaultRoundCount = ((KeccakP*)(f.get()))->getNominalNumberOfRounds();
    for(unsigned int i=1; i<=defaultRoundCount; i++)
        supportedRoundCounts.insert(i);
}

SnP_KeccakP::SnP_KeccakP(unsigned int width, const std::set<unsigned int>& aSupportedRoundCounts)
    : SnP(new KeccakP(width), 0, aSupportedRoundCounts, 0)
{
    laneSizeInBytes = ((KeccakP*)(f.get()))->getLaneSize() / 8;
    defaultRoundCount = ((KeccakP*)(f.get()))->getNominalNumberOfRounds();
}

SnP_Xoodoo::SnP_Xoodoo()
    : SnP(new Xoodoo(), 4, {}, 6)
{
    for(unsigned int i=1; i<=12; i++)
        supportedRoundCounts.insert(i);
}
