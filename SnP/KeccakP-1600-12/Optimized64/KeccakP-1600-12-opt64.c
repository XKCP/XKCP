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
#include <stdlib.h>
#include "brg_endian.h"
#include "KeccakF-1600-opt64-settings.h"
#include "KeccakP-1600-12-interface.h"

typedef unsigned char UINT8;
typedef unsigned long long int UINT64;

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(32))
#else
#define ALIGN
#endif

#if defined(UseLaneComplementing)
#define UseBebigokimisa
#endif

#if defined(_MSC_VER)
#define ROL64(a, offset) _rotl64(a, offset)
#elif defined(UseSHLD)
    #define ROL64(x,N) ({ \
    register UINT64 __out; \
    register UINT64 __in = x; \
    __asm__ ("shld %2,%0,%0" : "=r"(__out) : "0"(__in), "i"(N)); \
    __out; \
    })
#else
#define ROL64(a, offset) ((((UINT64)a) << offset) ^ (((UINT64)a) >> (64-offset)))
#endif

#include "KeccakF-1600-64.macros"
#include "KeccakP-1600-12-unrolling.macros"

extern const UINT64 KeccakF1600RoundConstants[24];

/* ---------------------------------------------------------------- */

void KeccakP1600_12_StatePermute(void *state)
{
    declareABCDE
    #ifndef FullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;

    copyFromState(A, stateAsLanes)
    rounds
    copyToState(stateAsLanes, A)
}

/* ---------------------------------------------------------------- */

size_t KeccakP1600_12_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits)
{
    size_t originalDataByteLen = dataByteLen;
    declareABCDE
    #ifndef FullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;
    UINT64 *inDataAsLanes = (UINT64*)data;

    copyFromState(A, stateAsLanes)
    while(dataByteLen >= laneCount*8) {
        XORinputAndTrailingBits(A, inDataAsLanes, laneCount, ((UINT64)trailingBits))
        rounds
        inDataAsLanes += laneCount;
        dataByteLen -= laneCount*8;
    }
    copyToState(stateAsLanes, A)
    return originalDataByteLen - dataByteLen;
}

/* ---------------------------------------------------------------- */

size_t KeccakP1600_12_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen)
{
    size_t originalDataByteLen = dataByteLen;
    declareABCDE
    #ifndef FullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;
    UINT64 *outDataAsLanes = (UINT64*)data;

    copyFromState(A, stateAsLanes)
    while(dataByteLen >= laneCount*8) {
        rounds
        output(A, outDataAsLanes, laneCount)
        outDataAsLanes += laneCount;
        dataByteLen -= laneCount*8;
    }
    copyToState(stateAsLanes, A)
    return originalDataByteLen - dataByteLen;
}

/* ---------------------------------------------------------------- */

size_t KeccakP1600_12_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    size_t originalDataByteLen = dataByteLen;
    declareABCDE
    #ifndef FullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;
    UINT64 *inDataAsLanes = (UINT64*)dataIn;
    UINT64 *outDataAsLanes = (UINT64*)dataOut;

    copyFromState(A, stateAsLanes)
    while(dataByteLen >= laneCount*8) {
        wrap(A, inDataAsLanes, outDataAsLanes, laneCount, ((UINT64)trailingBits))
        rounds
        inDataAsLanes += laneCount;
        outDataAsLanes += laneCount;
        dataByteLen -= laneCount*8;
    }
    copyToState(stateAsLanes, A)
    return originalDataByteLen - dataByteLen;
}

/* ---------------------------------------------------------------- */

size_t KeccakP1600_12_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    size_t originalDataByteLen = dataByteLen;
    declareABCDE
    #ifndef FullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;
    UINT64 *inDataAsLanes = (UINT64*)dataIn;
    UINT64 *outDataAsLanes = (UINT64*)dataOut;

    copyFromState(A, stateAsLanes)
    while(dataByteLen >= laneCount*8) {
        unwrap(A, inDataAsLanes, outDataAsLanes, laneCount, ((UINT64)trailingBits))
        rounds
        inDataAsLanes += laneCount;
        outDataAsLanes += laneCount;
        dataByteLen -= laneCount*8;
    }
    copyToState(stateAsLanes, A)
    return originalDataByteLen - dataByteLen;
}
