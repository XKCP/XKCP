/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by the designers, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <sstream>
#include "Exception.h"
#include "Keccak-p.h"

using namespace std;

KeccakP::KeccakP(unsigned int aWidth)
{
    width = aWidth;
    initializeNominalNumberOfRounds();
    laneSize = width/25;
    mask = (LaneValue(~0)) >> (64-laneSize);
    initializeRhoOffsets();
    initializeRoundConstants();
}

void KeccakP::initializeNominalNumberOfRounds()
{
    switch(width) {
    case 25:
        nominalNrRounds = 12;
        break;
    case 50:
        nominalNrRounds = 14;
        break;
    case 100:
        nominalNrRounds = 16;
        break;
    case 200:
        nominalNrRounds = 18;
        break;
    case 400:
        nominalNrRounds = 20;
        break;
    case 800:
        nominalNrRounds = 22;
        break;
    case 1600:
        nominalNrRounds = 24;
        break;
    default:
        throw(Exception("The width must be 25 times a power of two between 1 and 64."));
    }
}

unsigned int KeccakP::getWidth() const
{
    return width;
}

unsigned int KeccakP::getLaneSize() const
{
    return laneSize;
}

unsigned int KeccakP::getNominalNumberOfRounds() const
{
    return nominalNrRounds;
}

void KeccakP::apply(vector<uint8_t>& state, unsigned int roundCount) const
{
    if (roundCount > nominalNrRounds)
        throw Exception("The number of rounds is too high.");
    vector<LaneValue> A(25);
    fromBytesToLanes(state, A);
    for(unsigned int i=nominalNrRounds - roundCount; i<nominalNrRounds; i++)
        round(A, i);
    fromLanesToBytes(A, state);
}

unsigned int KeccakP::index(int x, int y)
{
    x %= 5;
    if (x<0) x += 5;
    y %= 5;
    if (y<0) y += 5;

    return(x + (5*y));
}

unsigned int KeccakP::index(int x)
{
    x %= 5;
    if (x<0) x += 5;
    return x;
}

void KeccakP::pi(unsigned int x, unsigned int y, unsigned int& X, unsigned int& Y)
{
    X = (0*x + 1*y)%5;
    Y = (2*x + 3*y)%5;
}

void KeccakP::ROL(LaneValue& L, int offset) const
{
    offset %= (int)laneSize;
    if (offset < 0) offset += laneSize;

    if (offset != 0) {
        L &= mask;
        L = (((LaneValue)L) << offset) ^ (((LaneValue)L) >> (laneSize-offset));
    }
    L &= mask;
}

void KeccakP::fromBytesToLanes(const std::vector<uint8_t>& in, vector<LaneValue>& out) const
{
    out.resize(25);
    if ((laneSize == 1) || (laneSize == 2) || (laneSize == 4) || (laneSize == 8)) {
        for(unsigned int i=0; i<25; i++)
            out[i] = (in[i*laneSize/8] >> ((i*laneSize) % 8)) & mask;
    }
    else if ((laneSize == 16) || (laneSize == 32) || (laneSize == 64)) {
        for(unsigned int i=0; i<25; i++) {
            out[i] = 0;
            for(unsigned int j=0; j<(laneSize/8); j++)
                out[i] |= LaneValue((uint8_t)(in[i*laneSize/8+j])) << (8*j);
        }
    }
}

void KeccakP::fromLanesToBytes(const vector<LaneValue>& in, std::vector<uint8_t>& out) const
{
    if ((laneSize == 1) || (laneSize == 2) || (laneSize == 4) || (laneSize == 8)) {
        for(unsigned int i=0; i<(25*laneSize+7)/8; i++)
            out[i] = 0;
        for(unsigned int i=0; i<25; i++)
            out[i*laneSize/8] |= in[i] << ((i*laneSize) % 8);
    }
    else if ((laneSize == 16) || (laneSize == 32) || (laneSize == 64)) {
        for(unsigned int i=0; i<25; i++)
            for(unsigned int j=0; j<(laneSize/8); j++)
                out[i*(laneSize/8)+j] = (uint8_t)((in[i] >> (8*j)) & 0xFF);
    }
}

string KeccakP::getDescription() const
{
    stringstream a;
    a << "Keccak-p[" << dec << width << "]";
    return a.str();
}

bool LFSR86540(uint8_t& state)
{
    bool result = (state & 0x01) != 0;
    if ((state & 0x80) != 0)
        state = (state << 1) ^ 0x71;
    else
        state <<= 1;
    return result;
}

void KeccakP::initializeRoundConstants()
{
    roundConstants.clear();
    uint8_t LFSRstate = 0x01;
    for(unsigned int i=0; i<255; i++) {
        LaneValue c = 0;
        for(unsigned int j=0; j<7; j++) {
            unsigned int bitPosition = (1<<j)-1; //2^j-1
            if (LFSR86540(LFSRstate))
                c ^= (LaneValue)1<<bitPosition;
        }
        roundConstants.push_back(c & mask);
    }
}

void KeccakP::initializeRhoOffsets()
{
    rhoOffsets.resize(25);
    rhoOffsets[index(0, 0)] = 0;
    unsigned int x = 1;
    unsigned int y = 0;
    for(unsigned int t=0; t<24; t++) {
        rhoOffsets[index(x, y)] = ((t+1)*(t+2)/2) % laneSize;
        unsigned int newX = (0*x + 1*y)%5;
        unsigned int newY = (2*x + 3*y)%5;
        x = newX;
        y = newY;
    }
}

LaneValue KeccakP::getRoundConstant(int roundIndex) const
{
    unsigned int ir = (unsigned int)(((roundIndex % 255) + 255) % 255);
    return roundConstants[ir];
}
