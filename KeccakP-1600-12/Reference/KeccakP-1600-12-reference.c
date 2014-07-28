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

#include "brg_endian.h"
#include "displayIntermediateValues.h"

typedef unsigned char UINT8;
typedef unsigned long long UINT64;
typedef UINT64 tKeccakLane;

void fromBytesToWords(tKeccakLane *stateAsWords, const unsigned char *state); // From KeccakF-1600/Reference
void fromWordsToBytes(unsigned char *state, const tKeccakLane *stateAsWords); // From KeccakF-1600/Reference
void KeccakP1600_12_OnWords(tKeccakLane *state);
void KeccakF1600Round(tKeccakLane *state, unsigned int indexRound); // From KeccakF-1600/Reference

void KeccakP1600_12_StatePermute(void *state)
{
#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
    tKeccakLane stateAsWords[KeccakF_width/64];
#endif

    displayStateAsBytes(1, "Input of permutation", (const unsigned char *)state);
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    KeccakP1600_12_OnWords((tKeccakLane*)state);
#else
    fromBytesToWords(stateAsWords, (const unsigned char *)state);
    KeccakP1600_12_OnWords(stateAsWords);
    fromWordsToBytes((unsigned char *)state, stateAsWords);
#endif
    displayStateAsBytes(1, "State after permutation", (const unsigned char *)state);
}

void KeccakP1600_12_OnWords(tKeccakLane *state)
{
    unsigned int i;

    displayStateAsLanes(3, "Same, with lanes as 64-bit words", state);

    for(i=12; i<24; i++)
        KeccakF1600Round(state, i);
}
