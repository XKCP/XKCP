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

#ifndef _SnP_h_
#define _SnP_h_

#include <cstdint>
#include <memory>
#include <vector>
#include <set>
#include "Keccak-p.h"
#include "Permutations.h"
#include "Xoodoo.h"

class SnP {
protected:
    std::unique_ptr<RoundCountParameterizedPermutation> f;
    unsigned int laneSizeInBytes;
    std::set<unsigned int> supportedRoundCounts;
    unsigned int defaultRoundCount;
    unsigned int widthInBytes;
protected:
        std::vector<std::uint8_t> state;
public:
    SnP(RoundCountParameterizedPermutation* aF, unsigned int aLaneSizeInBytes, const std::set<unsigned int>& aSupportedRoundCounts, unsigned int aDefaultRoundCount);
    friend std::ostream& operator<<(std::ostream& a, const SnP& snp);
    unsigned int getWidth() const;
    unsigned int getDefaultRoundCount() const;
    unsigned int getLaneSizeInBytes() const;

    void Initialize();
    void AddByte(unsigned char data, unsigned int offset);
    void AddBytes(const unsigned char *data, unsigned int offset, unsigned int length);
    void OverwriteBytes(const unsigned char *data, unsigned int offset, unsigned int length);
    void OverwriteWithZeroes(unsigned int byteCount);
    void Permute();
    void Permute_Nrounds(unsigned int nrounds);
    void ExtractBytes(unsigned char *data, unsigned int offset, unsigned int length) const;
    void ExtractAndAddBytes(const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length) const;
    size_t FastLoop_Absorb(unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
};

class SnP_KeccakP : public SnP {
public:
    SnP_KeccakP(unsigned int width);
    SnP_KeccakP(unsigned int width, const std::set<unsigned int>& aSupportedRoundCounts);
    size_t KeccakP1600_12rounds_FastLoop_Absorb(unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
};

class SnP_Xoodoo : public SnP {
public:
    SnP_Xoodoo();
};

#endif
