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
#include "KeccakF-800-optimized32-settings.h"
#include "KeccakF-800-interface.h"

typedef unsigned char UINT8;
typedef unsigned int UINT32;	// Uncomment if 32-bit and 64-bit oriented compiler
//typedef unsigned long UINT32;	// Uncomment if  8-bit and 16-bit oriented compiler

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(32))
#else
#define ALIGN
#endif

// Change READ32/WRITE macros if your target does not support unaligned 32-bit accesses.
#if defined (__arm__) && !defined(__GNUC__)
	#define ROL32(a, offset) __ror(a, 32-(offset))
	#define	READ32_UNALIGNED(argAddress)			(*((const __packed UINT32*)(argAddress)))
	#define	WRITE32_UNALIGNED(argAddress, argData)	(*((__packed UINT32*)(argAddress)) = (argData))
#elif defined(_MSC_VER)
	#define ROL32(a, offset) _rotl(a, offset)
	#define	READ32_UNALIGNED(argAddress)			(*((const UINT32*)(argAddress)))
	#define	WRITE32_UNALIGNED(argAddress, argData)	(*((UINT32*)(argAddress)) = (argData))
#else
	#define ROL32(a, offset) ((((UINT32)a) << offset) ^ (((UINT32)a) >> (32-offset)))
	#define	READ32_UNALIGNED(argAddress)			(*((const UINT32*)(argAddress)))
	#define	WRITE32_UNALIGNED(argAddress, argData)	(*((UINT32*)(argAddress)) = (argData))
#endif

#if defined(UseLaneComplementing)
#define UseBebigokimisa
#endif

#include "KeccakF-800-32.macros"
#include "KeccakF-800-unrolling.macros"

#ifdef UseLaneComplementing

const UINT32 KeccakF800LaneComplement[25] = {
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

const UINT32 KeccakF800RoundConstants[24] = {
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

void KeccakF800_Initialize( void )
{
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateInitialize(void *state)
{
    memset(state, 0, KeccakF_stateSizeInBytes);
#ifdef UseLaneComplementing
    ((UINT32*)state)[ 1] = ~(UINT32)0;
    ((UINT32*)state)[ 2] = ~(UINT32)0;
    ((UINT32*)state)[ 8] = ~(UINT32)0;
    ((UINT32*)state)[12] = ~(UINT32)0;
    ((UINT32*)state)[17] = ~(UINT32)0;
    ((UINT32*)state)[20] = ~(UINT32)0;
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateXORBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/KeccakF_laneInBytes;
    unsigned int offsetInLane = offset%KeccakF_laneInBytes;
    const unsigned char *curData = data;
    UINT32 *state = argState;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = KeccakF_laneInBytes - offsetInLane;
        UINT32 lane = 0;
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        memcpy((unsigned char*)&lane + offsetInLane, curData, bytesInLane);
        state[lanePosition] ^= lane;
        sizeLeft -= bytesInLane;
        lanePosition++;
        curData += bytesInLane;
    }

    while(sizeLeft >= KeccakF_laneInBytes) {
        state[lanePosition] ^= READ32_UNALIGNED( curData );
        sizeLeft -= KeccakF_laneInBytes;
        lanePosition++;
        curData += KeccakF_laneInBytes;
    }

    if (sizeLeft > 0) {
        UINT32 lane = 0;
        memcpy(&lane, curData, sizeLeft);
        state[lanePosition] ^= lane;
    }
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateOverwriteBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef UseLaneComplementing
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/KeccakF_laneInBytes;
    unsigned int offsetInLane = offset%KeccakF_laneInBytes;
    const unsigned char *curData = data;
    UINT32 *state = (UINT32*)argState;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = KeccakF_laneInBytes - offsetInLane;
	    unsigned char laneComplement = (unsigned char)KeccakF800LaneComplement[lanePosition];
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

    while(sizeLeft >= KeccakF_laneInBytes) {
        state[lanePosition] = READ32_UNALIGNED( curData ) ^ KeccakF800LaneComplement[lanePosition];
        sizeLeft -= KeccakF_laneInBytes;
        lanePosition++;
        curData += KeccakF_laneInBytes;
    }

    if (sizeLeft > 0) {
	    unsigned char laneComplement = (unsigned char)KeccakF800LaneComplement[lanePosition];
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

void KeccakF800_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef UseLaneComplementing
    memcpy((unsigned char*)state, (unsigned char*)KeccakF800LaneComplement, byteCount);
#else
    memset(state, 0, byteCount);
#endif
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateComplementBit(void *state, unsigned int position)
{
    UINT32 lane = (UINT32)1 << (position%32);
    ((UINT32*)state)[position/32] ^= lane;
}

/* ---------------------------------------------------------------- */

void KeccakF800_StatePermute(void *state)
{
    declareBCDE
    #ifndef FullUnrolling
    unsigned int i;
    #endif
    UINT32 *Astate = (UINT32*)state;

    rounds
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateExtractBytes(const void *argState, unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef UseLaneComplementing
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/KeccakF_laneInBytes;
    unsigned int offsetInLane = offset%KeccakF_laneInBytes;
    unsigned char *curData = data;
    const UINT32 *state = (UINT32*)argState;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = KeccakF_laneInBytes - offsetInLane;
	    unsigned char laneComplement = (unsigned char)KeccakF800LaneComplement[lanePosition];
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
		do {
	        *curData++ = *(((unsigned char *)&state[lanePosition]) + offsetInLane) ^ laneComplement;
			++offsetInLane;
		}
		while ( --bytesInLane != 0);
        lanePosition++;
    }

    while(sizeLeft >= KeccakF_laneInBytes) {
        WRITE32_UNALIGNED( curData, state[lanePosition] ^ KeccakF800LaneComplement[lanePosition] );
        sizeLeft -= KeccakF_laneInBytes;
        lanePosition++;
        curData += KeccakF_laneInBytes;
    }

    if (sizeLeft > 0) {
	    unsigned char laneComplement = (unsigned char)KeccakF800LaneComplement[lanePosition];
		unsigned int i;
		for ( i = 0; i < sizeLeft; ++i ) {
	        *curData++ = *(((unsigned char *)&state[lanePosition]) + i) ^ laneComplement;
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

void KeccakF800_StateExtractAndXORBytes(const void *argState, unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#ifdef UseLaneComplementing
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/KeccakF_laneInBytes;
    unsigned int offsetInLane = offset%KeccakF_laneInBytes;
    unsigned char *curData = data;
    const UINT32 *state = (UINT32*)argState;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = KeccakF_laneInBytes - offsetInLane;
	    unsigned char laneComplement = (unsigned char)KeccakF800LaneComplement[lanePosition];
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
		do {
	        *curData++ ^= *(((unsigned char *)&state[lanePosition]) + offsetInLane)  ^ laneComplement;
			++offsetInLane;
		}
		while ( --bytesInLane != 0);
        lanePosition++;
    }

    while(sizeLeft >= KeccakF_laneInBytes) {
        WRITE32_UNALIGNED( curData, READ32_UNALIGNED( curData ) ^ state[lanePosition] ^ KeccakF800LaneComplement[lanePosition] );
        sizeLeft -= KeccakF_laneInBytes;
        lanePosition++;
        curData += KeccakF_laneInBytes;
    }

    if (sizeLeft > 0) {
	    unsigned char laneComplement = (unsigned char)KeccakF800LaneComplement[lanePosition];
		unsigned int i;
		for ( i = 0; i < sizeLeft; ++i ) {
	        *curData++ ^= *(((unsigned char *)&state[lanePosition]) + i) ^ laneComplement;
		}
    }
#else
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/KeccakF_laneInBytes;
    unsigned int offsetInLane = offset%KeccakF_laneInBytes;
    unsigned char *curData = data;
    const UINT32 *state = (UINT32*)argState;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = KeccakF_laneInBytes - offsetInLane;
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
		do {
	        *curData++ ^= *(((unsigned char *)&state[lanePosition]) + offsetInLane);
			++offsetInLane;
		}
		while ( --bytesInLane != 0);
        lanePosition++;
    }

    while(sizeLeft >= KeccakF_laneInBytes) {
        WRITE32_UNALIGNED( curData, READ32_UNALIGNED( curData ) ^ state[lanePosition] );
        sizeLeft -= KeccakF_laneInBytes;
        lanePosition++;
        curData += KeccakF_laneInBytes;
    }

    if (sizeLeft > 0) {
		unsigned int i;
		for ( i = 0; i < sizeLeft; ++i ) {
	        *curData++ ^= *(((unsigned char *)&state[lanePosition]) + i);
		}
    }
#endif
#else
#error "Not yet implemented"
#endif
}

/* ---------------------------------------------------------------- */

size_t KeccakF800_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits)
{
    size_t originalDataByteLen = dataByteLen;
	unsigned int laneCountInBytes = laneCount*KeccakF_laneInBytes;

    while(dataByteLen >= laneCountInBytes) {
        KeccakF800_StateXORBytes(state, data, 0, laneCountInBytes);
        KeccakF800_StateXORBytes(state, &trailingBits, laneCountInBytes, 1);
        KeccakF800_StatePermute( state );
        data += laneCountInBytes;
        dataByteLen -= laneCountInBytes;
    }
    return originalDataByteLen - dataByteLen;
}

/* ---------------------------------------------------------------- */

size_t KeccakF800_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen)
{
    size_t originalDataByteLen = dataByteLen;
	unsigned int laneCountInBytes = laneCount*KeccakF_laneInBytes;

    while(dataByteLen >= laneCountInBytes) {
        KeccakF800_StatePermute( state );
		KeccakF800_StateExtractBytes( state, data, 0, laneCountInBytes );
        data += laneCountInBytes;
        dataByteLen -= laneCountInBytes;
    }
    return originalDataByteLen - dataByteLen;
}

/* ---------------------------------------------------------------- */

size_t KeccakF800_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    size_t originalDataByteLen = dataByteLen;
	unsigned int laneCountInBytes = laneCount*KeccakF_laneInBytes;

    while(dataByteLen >= laneCountInBytes) {
        KeccakF800_StateXORBytes(state, dataIn, 0, laneCountInBytes);
        KeccakF800_StateExtractBytes(state, dataOut, 0, laneCountInBytes);
        KeccakF800_StateXORBytes(state, &trailingBits, laneCountInBytes, 1);
        KeccakF800_StatePermute( state );
        dataIn += laneCountInBytes;
        dataOut += laneCountInBytes;
        dataByteLen -= laneCountInBytes;
    }
    return originalDataByteLen - dataByteLen;
}

/* ---------------------------------------------------------------- */

size_t KeccakF800_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
{
    size_t originalDataByteLen = dataByteLen;
	unsigned int laneCountInBytes = laneCount*KeccakF_laneInBytes;

    if (dataIn != dataOut)
        memcpy(dataOut, dataIn, dataByteLen);
    while(dataByteLen >= laneCountInBytes) {
        KeccakF800_StateExtractAndXORBytes(state, dataOut, 0, laneCountInBytes);
        KeccakF800_StateXORBytes(state, dataOut, 0, laneCountInBytes);
        KeccakF800_StateXORBytes(state, &trailingBits, laneCountInBytes, 1);
        KeccakF800_StatePermute( state );
        dataOut += laneCountInBytes;
        dataByteLen -= laneCountInBytes;
    }
    return originalDataByteLen - dataByteLen;
}
