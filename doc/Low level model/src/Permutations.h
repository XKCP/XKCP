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

#ifndef _Permutations_h_
#define _Permutations_h_

#include <cstdint>
#include <iostream>
#include <string>

/**
  * Abstract class that represents a permutation from n bits to n bits with a parameterized number of rounds.
  */
class RoundCountParameterizedPermutation {
public:
    RoundCountParameterizedPermutation() {};
    /**
      * Abstract method that returns the number of bits of its domain and range.
      */
    virtual unsigned int getWidth() const = 0;
    /**
      * Abstract method that applies the permutation onto the parameter
      * @a state with a given number of rounds.
      *
      * @param  state   A buffer on which to apply the permutation.
      *                 The state must have a size of at least
      *                 ceil(getWidth()/8.0) bytes.
      */
    virtual void apply(std::vector<std::uint8_t>& state, unsigned int roundCount) const = 0;
    /**
      * Abstract method that returns a string with a description of itself.
      */
    virtual std::string getDescription() const = 0;
    /**
      * Method that prints a brief description of the transformation.
      */
    friend std::ostream& operator<<(std::ostream& a, const RoundCountParameterizedPermutation& f)
    {
        a << f.getDescription();
        return a;
    }
};

#endif
