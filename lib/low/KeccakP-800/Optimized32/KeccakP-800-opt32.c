/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Keccak-p[800] in a SnP-compatible way.
Please refer to SnP-documentation.h for more details.

This implementation comes with KeccakP-800-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "brg_endian.h"
#include "KeccakP-800-opt32-config.h"
#include "KeccakP-800-SnP.h"

/* Change READ32/WRITE macros if your target does not support unaligned 32-bit accesses. */
#if defined (__arm__) && !defined(__GNUC__)
    #define ROL32(a, offset) __ror(a, 32-(offset))
    #define READ32_UNALIGNED(argAddress)            (*((const __packed uint32_t*)(argAddress)))
    #define WRITE32_UNALIGNED(argAddress, argData)  (*((__packed uint32_t*)(argAddress)) = (argData))
#elif defined(_MSC_VER)
    #define ROL32(a, offset) _rotl(a, offset)
    #define READ32_UNALIGNED(argAddress)            (*((const uint32_t*)(argAddress)))
    #define WRITE32_UNALIGNED(argAddress, argData)  (*((uint32_t*)(argAddress)) = (argData))
#else
    #define ROL32(a, offset) ((((uint32_t)a) << offset) ^ (((uint32_t)a) >> (32-offset)))
    #define READ32_UNALIGNED(argAddress)            (*((const uint32_t*)(argAddress)))
    #define WRITE32_UNALIGNED(argAddress, argData)  (*((uint32_t*)(argAddress)) = (argData))
#endif

#if defined(KeccakP800_useLaneComplementing)
#define UseBebigokimisa
#endif

#if defined(KeccakP800_useFlavorBis)
#include "KeccakP-800-opt32-bis.macros"
#include "KeccakP-800-unrolling-bis.macros"
#else
#include "KeccakP-800-opt32.macros"
#include "KeccakP-800-unrolling.macros"
#endif

#ifdef KeccakP800_useLaneComplementing

const uint32_t KeccakP800LaneComplement[25] = {
    0,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0,
    0,
    0,
    0,
    0,
    0xFFFFFFFF,
    0,
    0,
    0,
    0xFFFFFFFF,
    0,
    0,
    0,
    0,
    0xFFFFFFFF,
    0,
    0,
    0xFFFFFFFF,
    0,
    0,
    0,
    0};

#endif

const uint32_t KeccakF800RoundConstants[24] = {
    0x00000001ULL,
    0x00008082ULL,
    0x0000808aULL,
    0x80008000ULL,
    0x0000808bULL,
    0x80000001ULL,
    0x80008081ULL,
    0x00008009ULL,
    0x0000008aULL,
    0x00000088ULL,
    0x80008009ULL,
    0x8000000aULL,
    0x8000808bULL,
    0x0000008bULL,
    0x00008089ULL,
    0x00008003ULL,
    0x00008002ULL,
    0x00000080ULL,
    0x0000800aULL,
    0x8000000aULL,
    0x80008081ULL,
    0x00008080ULL};

/* ---------------------------------------------------------------- */

void KeccakP800_Initialize(void *state)
{
    memset(state, 0, 100);
#ifdef KeccakP800_useLaneComplementing
    ((uint32_t*)state)[ 1] = ~(uint32_t)0;
    ((uint32_t*)state)[ 2] = ~(uint32_t)0;
    ((uint32_t*)state)[ 8] = ~(uint32_t)0;
    ((uint32_t*)state)[12] = ~(uint32_t)0;
    ((uint32_t*)state)[17] = ~(uint32_t)0;
    ((uint32_t*)state)[20] = ~(uint32_t)0;
#endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_AddByte(void *argState, unsigned char data, unsigned int offset)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    unsigned int lanePosition = offset/4;
    unsigned int offsetInLane = offset%4;
    uint32_t lane = (uint32_t)data << (8*offsetInLane);
    ((uint32_t*)argState)[lanePosition] ^= lane;
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_AddBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/4;
    unsigned int offsetInLane = offset%4;
    const unsigned char *curData = data;
    uint32_t *state = (uint32_t*)argState;

    state += lanePosition;
    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = 4 - offsetInLane;
        uint32_t lane = 0;
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        memcpy((unsigned char*)&lane + offsetInLane, curData, bytesInLane);
        *state++ ^= lane;
        sizeLeft -= bytesInLane;
        curData += bytesInLane;
    }

    while(sizeLeft >= 4) {
        *state++ ^= READ32_UNALIGNED( curData );
        sizeLeft -= 4;
        curData += 4;
    }

    if (sizeLeft > 0) {
        uint32_t lane = 0;
        memcpy(&lane, curData, sizeLeft);
        *state ^= lane;
    }
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_OverwriteBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef KeccakP800_useLaneComplementing
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/4;
    unsigned int offsetInLane = offset%4;
    const unsigned char *curData = data;
    uint32_t *state = (uint32_t*)argState;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = 4 - offsetInLane;
        unsigned char laneComplement = (unsigned char)KeccakP800LaneComplement[lanePosition];
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
        do {
            *(((unsigned char *)&state[lanePosition] + offsetInLane)) = *curData++ ^ laneComplement;
            ++offsetInLane;
        }
        while ( --bytesInLane != 0);
        lanePosition++;
    }

    while(sizeLeft >= 4) {
        state[lanePosition] = READ32_UNALIGNED( curData ) ^ KeccakP800LaneComplement[lanePosition];
        sizeLeft -= 4;
        lanePosition++;
        curData += 4;
    }

    if (sizeLeft > 0) {
        unsigned char laneComplement = (unsigned char)KeccakP800LaneComplement[lanePosition];
        unsigned int i;
        for ( i = 0; i < sizeLeft; ++i ) {
            *((unsigned char *)&state[lanePosition] + i) = *curData++ ^ laneComplement;
        }
    }
#else
    memcpy((unsigned char*)argState+offset, data, length);
#endif
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef KeccakP800_useLaneComplementing
    memcpy((unsigned char*)state, (unsigned char*)KeccakP800LaneComplement, byteCount);
#else
    memset(state, 0, byteCount);
#endif
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_Permute_Nrounds(void *state, unsigned int nr)
{
    declareBCDE
    unsigned int i;
    uint32_t *Astate = (uint32_t*)state;

    roundsN(A,E,nr)
}

/* ---------------------------------------------------------------- */

void KeccakP800_Permute_12rounds(void *state)
{
    declareBCDE
    #ifndef KeccakP800_fullUnrolling
    unsigned int i;
    #endif
    uint32_t *Astate = (uint32_t*)state;

    rounds12
}

/* ---------------------------------------------------------------- */

void KeccakP800_Permute_22rounds(void *state)
{
    declareBCDE
    #ifndef KeccakP800_fullUnrolling
    unsigned int i;
    #endif
    uint32_t *Astate = (uint32_t*)state;

    rounds22
}

/* ---------------------------------------------------------------- */

void KeccakP800_ExtractBytes(const void *argState, unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef KeccakP800_useLaneComplementing
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/4;
    unsigned int offsetInLane = offset%4;
    unsigned char *curData = data;
    const uint32_t *state = (const uint32_t*)argState;
    const uint32_t *pLaneComplement;

    state += lanePosition;
    pLaneComplement = KeccakP800LaneComplement + lanePosition;
    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = 4 - offsetInLane;
        uint32_t  lane = (*state++ ^ *pLaneComplement++) >> (offsetInLane * 8);
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
        do {
            *curData++ = (unsigned char)lane;
            lane >>= 8;
        }
        while ( --bytesInLane != 0);
        lanePosition++;
    }

    while(sizeLeft >= 4) {
        WRITE32_UNALIGNED( curData, *state++ ^ *pLaneComplement++ );
        sizeLeft -= 4;
        curData += 4;
    }

    if (sizeLeft > 0) {
        uint32_t  lane = *state ^ *pLaneComplement;
        unsigned int i;
        for ( i = 0; i < sizeLeft; ++i ) {
            *curData++ = (unsigned char)lane;
            lane >>= 8;
        }
    }
#else
    memcpy(data, (unsigned char*)argState+offset, length);
#endif
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_ExtractAndAddBytes(const void *argState, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef KeccakP800_useLaneComplementing
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/4;
    unsigned int offsetInLane = offset%4;
    const unsigned char *curInput = input;
    unsigned char *curOutput = output;
    const uint32_t *state = (const uint32_t*)argState;
    const uint32_t *pLaneComplement;

    state += lanePosition;
    pLaneComplement = KeccakP800LaneComplement + lanePosition;
    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = 4 - offsetInLane;
        uint32_t  lane = (*state++ ^ *pLaneComplement++) >> (offsetInLane * 8);
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
        do {
            *curOutput++ = (*curInput++) ^ (unsigned char)lane;
            lane >>= 8;
        }
        while ( --bytesInLane != 0);
    }

    while(sizeLeft >= 4) {
        WRITE32_UNALIGNED( curOutput, READ32_UNALIGNED( curInput ) ^ *state++ ^ *pLaneComplement++ );
        sizeLeft -= 4;
        curInput += 4;
        curOutput += 4;
    }

    if (sizeLeft > 0) {
        uint32_t  lane = *state ^ *pLaneComplement;
        do {
            *curOutput++ = (*curInput++) ^ (unsigned char)lane;
            lane >>= 8;
        }
        while ( --sizeLeft != 0 );
    }
#else
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/4;
    unsigned int offsetInLane = offset%4;
    const unsigned char *curInput = input;
    unsigned char *curOutput = output;
    const uint32_t *state = (const uint32_t*)argState;

    state += lanePosition;
    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = 4 - offsetInLane;
        uint32_t  lane = *state++ >> (offsetInLane * 8);
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
        do {
            *curOutput++ = (*curInput++) ^ (unsigned char)lane;
            lane >>= 8;
        }
        while ( --bytesInLane != 0);
    }

    while(sizeLeft >= 4) {
        WRITE32_UNALIGNED( curOutput, READ32_UNALIGNED( curInput ) ^ *state++ );
        sizeLeft -= 4;
        curInput += 4;
        curOutput += 4;
    }

    if (sizeLeft > 0) {
        uint32_t  lane = *state;
        do {
            *curOutput++ = (*curInput++) ^ (unsigned char)lane;
            lane >>= 8;
        }
        while ( --sizeLeft != 0 );
    }
#endif
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

size_t KeccakF800_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
{
    size_t originalDataByteLen = dataByteLen;
    unsigned int laneCountInBytes = laneCount*4;

    while(dataByteLen >= laneCountInBytes) {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        uint32_t * pState  = (uint32_t*)state;
        unsigned int lc;

        for ( lc = laneCount; lc >= 4; lc -= 4 ) {
            *pState++ ^= READ32_UNALIGNED( data );
            data += 4;
            *pState++ ^= READ32_UNALIGNED( data );
            data += 4;
            *pState++ ^= READ32_UNALIGNED( data );
            data += 4;
            *pState++ ^= READ32_UNALIGNED( data );
            data += 4;
        }
        while ( lc-- != 0 ) {
            *pState++ ^= READ32_UNALIGNED( data );
            data += 4;
        }
#else
        KeccakP800_AddBytes(state, data, 0, laneCountInBytes);
        data += laneCountInBytes;
#endif
        KeccakP800_Permute_22rounds( state );
        dataByteLen -= laneCountInBytes;
    }
    return originalDataByteLen - dataByteLen;
}
