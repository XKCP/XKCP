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

#ifndef _Keccak_p_h_
#define _Keccak_p_h_

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include "Permutations.h"

typedef uint64_t LaneValue;

/**
  * Class implementing the Keccak-<i>p</i> permutations.
  */
class KeccakP : public RoundCountParameterizedPermutation {
protected:
    /** The width of the permutation. */
    unsigned int width;
    /** The size of the lanes. */
    unsigned int laneSize;
    /** The nominal number of rounds, as function of the width. */
    unsigned int nominalNrRounds;
    /** The translation offsets for ρ. */
    std::vector<int> rhoOffsets;
    /** The round constants for ι. */
    std::vector<LaneValue> roundConstants;
    /** A 64-bit word whose first laneSize bits are 1 and all others 0. */
    LaneValue mask;
public:
    /**
      * The constructor. The width is given as parameter.
      *
      * @param  aWidth      The width of the Keccak-<i>p</i> permutation.
      *                     It must be one of the valid Keccak-<i>p</i> widths, namely
      *                     25, 50, 100, 200, 400, 800 or 1600.
      */
    KeccakP(unsigned int aWidth);
    /**
      * Method that returns the number of bits of its domain and range.
      */
    unsigned int getWidth() const;
    /**
      * Method that retuns the lane size of the Keccak-<i>p</i> instance.
      */
    unsigned int getLaneSize() const;
    /**
      * Method that returns the nominal number of rounds of Keccak-<i>f</i>
      * according to the specifications for the width of this instance.
      */
    unsigned int getNominalNumberOfRounds() const;
    /**
      * Method that applies the Keccak-<i>f</i> permutation onto the parameter
      * @a state with a given number of rounds.
      *
      * @param  state   A buffer on which to apply the permutation.
      *                 The state must have a size of at least
      *                 ceil(getWidth()/8.0) bytes.
      * @param  roundCount  The number of rounds.
      */
    void apply(std::vector<std::uint8_t>& state, unsigned int roundCount) const;
    /**
      * Method that returns a string describing the instance of the Keccak-<i>f</i>
      * permutation.
      */
    std::string getDescription() const;
    /**
      * Method that maps the coordinates (x, y) onto the lanes
      * numbered from 0 to 24. The formula is (x mod 5)+5*(y mod 5),
      * so that the lanes are ordered in line with the bit ordering defined
      * in the specifications.
      *
      * @param  x           The x coordinate. It can be any signed integer,
      *                     as it will be reduced modulo 5.
      * @param  y           The y coordinate. It can be any signed integer,
      *                     as it will be reduced modulo 5.
      */
    static unsigned int index(int x, int y);
    /**
      * Method that reduces modulo 5 the coordinate x expressed as a signed
      * integer.
      *
      * @param  x           The x coordinate. It can be any signed integer,
      *                     as it will be reduced modulo 5.
      */
    static unsigned int index(int x);
    /**
      * Template method that applies the round function.
      * This function is a template to allow both numerical and symbolic values
      * to be processed (see also KeccakFEquations).
      *
      * @param  A       The given state is organized as a vector of 25 lanes.
      *                 The mapping between the coordinates (x, y) and the
      *                 numbering inside the vector is defined by index().
      *                 The parameter type @a Lane can be a LaneValue for an
      *                 actual evaluation.
      * @param  roundIndex  The round index.
      */
    template<class Lane> void round(std::vector<Lane>& A, int roundIndex) const;
    /**
      * Template method that applies χ.
      * This function is a template to allow both numerical and symbolic values
      * to be processed (see also KeccakFEquations).
      *
      * @param  A       The given state is organized as a vector of 25 lanes.
      *                 The mapping between the coordinates (x, y) and the
      *                 numbering inside the vector is defined by index().
      *                 The parameter type @a Lane can be a LaneValue for an
      *                 actual evaluation.
      */
    template<class Lane> void chi(std::vector<Lane>& A) const;
    /**
      * Template method that applies θ.
      * This function is a template to allow both numerical and symbolic values
      * to be processed (see also KeccakFEquations).
      *
      * @param  A       The given state is organized as a vector of 25 lanes.
      *                 The mapping between the coordinates (x, y) and the
      *                 numbering inside the vector is defined by index().
      *                 The parameter type @a Lane can be a LaneValue for an
      *                 actual evaluation.
      */
    template<class Lane> void theta(std::vector<Lane>& A) const;
    /**
      * Template method that applies π.
      * This function is a template to allow both numerical and symbolic values
      * to be processed (see also KeccakFEquations).
      *
      * @param  A       The given state is organized as a vector of 25 lanes.
      *                 The mapping between the coordinates (x, y) and the
      *                 numbering inside the vector is defined by index().
      *                 The parameter type @a Lane can be a LaneValue for an
      *                 actual evaluation.
      */
    template<class Lane> void pi(std::vector<Lane>& A) const;
    /**
      * Method that applies the π coordinate transformation to a lane position (x,y).
      *
      * @param[in]  x   The input coordinate x.
      * @param[in]  y   The input coordinate y.
      * @param[out] X   The output coordinate x.
      * @param[out] Y   The output coordinate y.
      */
    static void pi(unsigned int x, unsigned int y, unsigned int& X, unsigned int& Y);
    /**
      * Template method that applies ρ.
      * This function is a template to allow both numerical and symbolic values
      * to be processed (see also KeccakFEquations).
      *
      * @param  A       The given state is organized as a vector of 25 lanes.
      *                 The mapping between the coordinates (x, y) and the
      *                 numbering inside the vector is defined by index().
      *                 The parameter type @a Lane can be a LaneValue for an
      *                 actual evaluation.
      */
    template<class Lane> void rho(std::vector<Lane>& A) const;
    /**
      * Template method that applies ι, which is its own inverse.
      * This function is a template to allow both numerical and symbolic values
      * to be processed (see also KeccakFEquations).
      *
      * @param  A       The given state is organized as a vector of 25 lanes.
      *                 The mapping between the coordinates (x, y) and the
      *                 numbering inside the vector is defined by index().
      *                 The parameter type @a Lane can be a LaneValue for an
      *                 actual evaluation.
      * @param  roundIndex  The round index.
      */
    template<class Lane> void iota(std::vector<Lane>& A, int roundIndex) const;
    /**
      * Method that retuns the i-th round constant used by ι.
      *
      * @param  roundIndex  The round index.
      */
    LaneValue getRoundConstant(int roundIndex) const;
    /**
      * Method that implementats ROL when the lane is in a 64-bit word LaneValue.
      *
      * @param  L       The given lane.
      * @param offset   The translation offset. It can be any signed
      *                 integer, as it will be reduced modulo laneSize.
      */
    void ROL(LaneValue& L, int offset) const;
    /**
      * Method that converts a state given as an array of bytes into a vector
      * of lanes in 64-bit words.
      *
      * @param  in      The state as an array of bytes.
      *                 The array @a in must have a size of at least
      *                 ceil(getWidth()/8.0) bytes.
      * @param  out     The state as a vector of lanes.
      *                 It will be resized to 25 if necessary.
      */
    void fromBytesToLanes(const std::vector<uint8_t>& in, std::vector<LaneValue>& out) const;
    /**
      * Method that converts a vector of lanes in 64-bit words into a state
      * given as an array of bytes.
      *
      * @param  in      The state as a vector of lanes.
      *                 in.size() must be equal to 25.
      * @param  out     The state as an array of bytes.
      *                 The array @a out must have a size of at least
      *                 ceil(getWidth()/8.0) bytes.
      */
    void fromLanesToBytes(const std::vector<LaneValue>& in, std::vector<uint8_t>& out) const;
private:
    /**
      * Method that initializes the nominal number of rounds according to the
      * specifications.
      */
    void initializeNominalNumberOfRounds();
    /**
      * Method that initializes the round constants according to the
      * specifications.
      */
    void initializeRoundConstants();
    /**
      * Method that initializes the 25 lane translation offsets for ρ according to
      * the specifications.
      */
    void initializeRhoOffsets();
};

template<class Lane>
void KeccakP::round(std::vector<Lane>& state, int roundIndex) const
{
    theta(state);
    rho(state);
    pi(state);
    chi(state);
    iota(state, roundIndex);
}

template<class Lane>
void KeccakP::chi(std::vector<Lane>& A) const
{
    std::vector<Lane> C(5);
    for(unsigned int y=0; y<5; y++) {
        for(unsigned int x=0; x<5; x++)
            C[x] = A[index(x,y)] ^ ((~A[index(x+1,y)]) & A[index(x+2,y)]);
        for(unsigned int x=0; x<5; x++)
            A[index(x,y)] = C[index(x)];
    }
}

template<class Lane>
void KeccakP::theta(std::vector<Lane>& A) const
{
    std::vector<Lane> C(5);
    for(unsigned int x=0; x<5; x++) {
        C[x] = A[index(x,0)];
        for(unsigned int y=1; y<5; y++)
            C[x] ^= A[index(x,y)];
    }
    std::vector<Lane> D(5);
    for(unsigned int x=0; x<5; x++) {
        Lane temp = C[index(x+1)];
        ROL(temp, 1);
        D[x] = temp ^ C[index(x-1)];
    }
    for(int x=0; x<5; x++)
        for(unsigned int y=0; y<5; y++)
            A[index(x,y)] ^= D[x];
}

template<class Lane>
void KeccakP::pi(std::vector<Lane>& A) const
{
    std::vector<Lane> a(A);
    for(unsigned int x=0; x<5; x++)
    for(unsigned int y=0; y<5; y++) {
        unsigned int X, Y;
        pi(x, y, X, Y);
        A[index(X,Y)] = a[index(x,y)];
    }
}

template<class Lane>
void KeccakP::rho(std::vector<Lane>& A) const
{
    for(unsigned int x=0; x<5; x++)
    for(unsigned int y=0; y<5; y++)
        ROL(A[index(x,y)], rhoOffsets[index(x,y)]);
}

template<class Lane>
void KeccakP::iota(std::vector<Lane>& A, int roundIndex) const
{
    unsigned int ir = (unsigned int)(((roundIndex % 255) + 255) % 255);
    A[index(0,0)] ^= roundConstants[ir];
}
#endif
