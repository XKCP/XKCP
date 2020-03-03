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

#ifndef _PlSnP_h_
#define _PlSnP_h_

#include <cstdint>
#include <memory>
#include <vector>
#include <set>
#include "Keccak-p.h"
#include "Permutations.h"
#include "Xoodoo.h"

class PlSnP {
protected:
    std::unique_ptr<RoundCountParameterizedPermutation> f;
    unsigned int numberOfInstances;
    unsigned int laneSizeInBytes;
    std::set<unsigned int> supportedRoundCounts;
    unsigned int defaultRoundCount;
    unsigned int widthInBytes;
protected:
        std::vector<std::vector<std::uint8_t> > states;
public:
    PlSnP(RoundCountParameterizedPermutation* aF, unsigned int aNumberOfInstances, unsigned int aLaneSizeInBytes, const std::set<unsigned int>& aSupportedRoundCounts, unsigned int aDefaultRoundCount);
    friend std::ostream& operator<<(std::ostream& a, const PlSnP& snp);
    unsigned int getWidth() const;
    unsigned int getDefaultRoundCount() const;
    unsigned int getLaneSizeInBytes() const;
    unsigned int getNumberOfInstances() const;

    void InitializeAll();
    void AddByte(unsigned int instanceIndex, unsigned char data, unsigned int offset);
    void AddBytes(unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
    void AddLanesAll(const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
    void OverwriteBytes(unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);
    void OverwriteLanesAll(const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);
    void OverwriteWithZeroes(unsigned int instanceIndex, unsigned int byteCount);
    void PermuteAll();
    void PermuteAll_Nrounds(unsigned int nrounds);
    void ExtractBytes(unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length) const;
    void ExtractLanesAll(unsigned char *data, unsigned int laneCount, unsigned int laneOffset) const;
    void ExtractAndAddBytes(unsigned int instanceIndex, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length) const;
    void ExtractAndAddLanesAll(const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset) const;
    size_t FastLoop_Absorb(unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen);
};

#endif
