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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <smmintrin.h>
#include <wmmintrin.h>
#include <immintrin.h>
#include <emmintrin.h>
#include "align.h"
#include "KeccakP-1600-times8-SnP.h"
#include "SIMD512-config.h"

#include "brg_endian.h"
#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
#error Expecting a little-endian platform
#endif

/* Comment the define hereunder when compiling for a CPU with AVX-512 SIMD */
/* 
 * Warning: This code has only been tested on Haswell (AVX2) with SIMULATE_AVX512 defined,
 *          errors will occur if we did a bad interpretation of the AVX-512 intrinsics' 
 *          API or functionality.
 */
//#define SIMULATE_AVX512

typedef uint8_t     UINT8;
typedef uint32_t    UINT32;
typedef uint64_t    UINT64;

#if defined(SIMULATE_AVX512)

typedef struct
{
	UINT64 x[8];
} __m512i;

static __m512i _mm512_xor_si512( __m512i a, __m512i b)
{
	__m512i r;
	unsigned int i;

	for ( i = 0; i < 8; ++i )
		r.x[i] = a.x[i] ^ b.x[i];
	return(r);
}

static __m512i _mm512_ternarylogic_epi64(__m512i a, __m512i b, __m512i c, int imm)
{

	if (imm == 0x96)
		return ( _mm512_xor_si512( _mm512_xor_si512( a, b ), c ) );
	if (imm == 0xD2) {
		__m512i t;
		unsigned int i;

		for ( i = 0; i < 8; ++i )
			t.x[i] = ~b.x[i] & c.x[i];
		return ( _mm512_xor_si512( a, t ) );
	}
	printf( "_mm512_ternarylogic_epi64( a, b, c, %02X) not implemented!\n", imm );
	exit(1);

}

static __m512i _mm512_rol_epi64(__m512i a, int offset)
{
	__m512i r;
	unsigned int i;

	for ( i = 0; i < 8; ++i )
		r.x[i] = (a.x[i] << offset) | (a.x[i] >> (64-offset));
	return(r);
}

static __m512i _mm512_broadcast_f64x4(__m256d a)
{
	__m512i r;
	unsigned int i;
	UINT64 t[4];

	_mm256_store_si256( (__m256i*)t, (__m256i)a );
	for ( i = 0; i < 4; ++i )
		r.x[i+4] = r.x[i] = t[i];
	return(r);
}

static __m512i _mm512_set_epi64(UINT64 a, UINT64 b, UINT64 c, UINT64 d, UINT64 e, UINT64 f, UINT64 g, UINT64 h)
{
	__m512i r;

	r.x[0] = h;
	r.x[1] = g;
	r.x[2] = f;
	r.x[3] = e;
	r.x[4] = d;
	r.x[5] = c;
	r.x[6] = b;
	r.x[7] = a;
	return(r);
}

static __m512i _mm512_i32gather_epi64(__m256i idx, const void *p, int scale)
{
	__m512i r;
	unsigned int i;
	UINT32 offset[16];

	_mm256_store_si256( (__m256i*)offset, idx );
	for ( i = 0; i < 8; ++i )
		r.x[i] = *(const UINT64*)((const char*)p + offset[i] * scale);
	return(r);
}

static void _mm512_i32scatter_epi64( void *p, __m256i idx, __m512i value, int scale)
{
	unsigned int i;
	UINT32 offset[16];

	_mm256_store_si256( (__m256i*)offset, idx );
	for ( i = 0; i < 8; ++i )
		*(UINT64*)((char*)p + offset[i] * scale) = value.x[i];
}

#endif

typedef __m128i     V128;
typedef __m256i     V256;
typedef __m512i     V512;

#if defined(KeccakP1600times8_useAVX512)

#define XOR(a,b) 					_mm512_xor_si512(a,b)
#define XOR3(a,b,c)					_mm512_ternarylogic_epi64(a,b,c,0x96)
#define XOR5(a,b,c,d,e)				XOR3(XOR3(a,b,c),d,e)
#define ROL(a,offset)				_mm512_rol_epi64(a,offset)
#define	Chi(a,b,c)					_mm512_ternarylogic_epi64(a,b,c,0xD2)

#define CONST8_64(a)          		(V512)_mm512_broadcast_f64x4(_mm256_broadcast_sd((const double*)(&a)))
#define LOAD8_32(a,b,c,d,e,f,g,h)   _mm256_set_epi32((UINT64)(a), (UINT32)(b), (UINT32)(c), (UINT32)(d), (UINT32)(e), (UINT32)(f), (UINT32)(g), (UINT32)(h))
#define LOAD8_64(a,b,c,d,e,f,g,h)   _mm512_set_epi64((UINT64)(a), (UINT64)(b), (UINT64)(c), (UINT64)(d), (UINT64)(e), (UINT64)(f), (UINT64)(g), (UINT64)(h))
#define LOAD_GATHER8_64(idx,p)		_mm512_i32gather_epi64( idx, (const void*)(p), 8)
#define STORE_SCATTER8_64(p,idx, v)	_mm512_i32scatter_epi64( (void*)(p), idx, v, 8)

#endif

#define laneIndex(instanceIndex, lanePosition)  ((lanePosition)*8 + instanceIndex)
#define SnP_laneLengthInBytes                   8

void KeccakP1600times8_InitializeAll(void *states)
{
    memset(states, 0, KeccakP1600times8_statesSizeInBytes);
}

void KeccakP1600times8_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/SnP_laneLengthInBytes;
    unsigned int offsetInLane = offset%SnP_laneLengthInBytes;
    const unsigned char *curData = data;
    UINT64 *statesAsLanes = states;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = SnP_laneLengthInBytes - offsetInLane;
        UINT64 lane = 0;
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        memcpy((unsigned char*)&lane + offsetInLane, curData, bytesInLane);
        statesAsLanes[laneIndex(instanceIndex, lanePosition)] ^= lane;
        sizeLeft -= bytesInLane;
        lanePosition++;
        curData += bytesInLane;
    }

    while(sizeLeft >= SnP_laneLengthInBytes) {
        UINT64 lane = *((const UINT64*)curData);
        statesAsLanes[laneIndex(instanceIndex, lanePosition)] ^= lane;
        sizeLeft -= SnP_laneLengthInBytes;
        lanePosition++;
        curData += SnP_laneLengthInBytes;
    }

    if (sizeLeft > 0) {
        UINT64 lane = 0;
        memcpy(&lane, curData, sizeLeft);
        statesAsLanes[laneIndex(instanceIndex, lanePosition)] ^= lane;
    }
}

void KeccakP1600times8_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    V512 *stateAsLanes = states;
    const UINT64 *dataAsLanes = (const UINT64 *)data;
    unsigned int i;
	V256 index;

    #define Add_In( argIndex )  stateAsLanes[argIndex] = XOR(stateAsLanes[argIndex], LOAD_GATHER8_64(index, dataAsLanes+argIndex))
    index = LOAD8_32(7*laneOffset, 6*laneOffset, 5*laneOffset, 4*laneOffset, 3*laneOffset, 2*laneOffset, 1*laneOffset, 0*laneOffset);
    if ( laneCount >= 16 )  {
        Add_In( 0 );
        Add_In( 1 );
        Add_In( 2 );
        Add_In( 3 );
        Add_In( 4 );
        Add_In( 5 );
        Add_In( 6 );
        Add_In( 7 );
        Add_In( 8 );
        Add_In( 9 );
        Add_In( 10 );
        Add_In( 11 );
        Add_In( 12 );
        Add_In( 13 );
        Add_In( 14 );
        Add_In( 15 );
        if ( laneCount >= 20 )  {
	        Add_In( 16 );
    	    Add_In( 17 );
	        Add_In( 18 );
    	    Add_In( 19 );
            for(i=20; i<laneCount; i++)
                Add_In( i );
        }
        else {
            for(i=16; i<laneCount; i++)
                Add_In( i );
        }
    }
    else {
        for(i=0; i<laneCount; i++)
            Add_In( i );
    }
    #undef  Add_In
}

void KeccakP1600times8_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/SnP_laneLengthInBytes;
    unsigned int offsetInLane = offset%SnP_laneLengthInBytes;
    const unsigned char *curData = data;
    UINT64 *statesAsLanes = states;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = SnP_laneLengthInBytes - offsetInLane;
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        memcpy( ((unsigned char *)&statesAsLanes[laneIndex(instanceIndex, lanePosition)]) + offsetInLane, curData, bytesInLane);
        sizeLeft -= bytesInLane;
        lanePosition++;
        curData += bytesInLane;
    }

    while(sizeLeft >= SnP_laneLengthInBytes) {
        UINT64 lane = *((const UINT64*)curData);
        statesAsLanes[laneIndex(instanceIndex, lanePosition)] = lane;
        sizeLeft -= SnP_laneLengthInBytes;
        lanePosition++;
        curData += SnP_laneLengthInBytes;
    }

    if (sizeLeft > 0) {
        memcpy(&statesAsLanes[laneIndex(instanceIndex, lanePosition)], curData, sizeLeft);
    }
}

void KeccakP1600times8_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    V512 *stateAsLanes = states;
    const UINT64 *dataAsLanes = (const UINT64 *)data;
    unsigned int i;
	V256 index;

    #define OverWr( argIndex )  stateAsLanes[argIndex] = LOAD_GATHER8_64(index, dataAsLanes+argIndex)
    index = LOAD8_32(7*laneOffset, 6*laneOffset, 5*laneOffset, 4*laneOffset, 3*laneOffset, 2*laneOffset, 1*laneOffset, 0*laneOffset);
    if ( laneCount >= 16 )  {
        OverWr( 0 );
        OverWr( 1 );
        OverWr( 2 );
        OverWr( 3 );
        OverWr( 4 );
        OverWr( 5 );
        OverWr( 6 );
        OverWr( 7 );
        OverWr( 8 );
        OverWr( 9 );
        OverWr( 10 );
        OverWr( 11 );
        OverWr( 12 );
        OverWr( 13 );
        OverWr( 14 );
        OverWr( 15 );
        if ( laneCount >= 20 )  {
	        OverWr( 16 );
    	    OverWr( 17 );
	        OverWr( 18 );
    	    OverWr( 19 );
            for(i=20; i<laneCount; i++)
                OverWr( i );
        }
        else {
            for(i=16; i<laneCount; i++)
                OverWr( i );
        }
    }
    else {
        for(i=0; i<laneCount; i++)
            OverWr( i );
    }
    #undef  OverWr
}

void KeccakP1600times8_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount)
{
    unsigned int sizeLeft = byteCount;
    unsigned int lanePosition = 0;
    UINT64 *statesAsLanes = states;

    while(sizeLeft >= SnP_laneLengthInBytes) {
        statesAsLanes[laneIndex(instanceIndex, lanePosition)] = 0;
        sizeLeft -= SnP_laneLengthInBytes;
        lanePosition++;
    }

    if (sizeLeft > 0) {
        memset(&statesAsLanes[laneIndex(instanceIndex, lanePosition)], 0, sizeLeft);
    }
}

void KeccakP1600times8_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/SnP_laneLengthInBytes;
    unsigned int offsetInLane = offset%SnP_laneLengthInBytes;
    unsigned char *curData = data;
    const UINT64 *statesAsLanes = states;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = SnP_laneLengthInBytes - offsetInLane;
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        memcpy( curData, ((unsigned char *)&statesAsLanes[laneIndex(instanceIndex, lanePosition)]) + offsetInLane, bytesInLane);
        sizeLeft -= bytesInLane;
        lanePosition++;
        curData += bytesInLane;
    }

    while(sizeLeft >= SnP_laneLengthInBytes) {
        *(UINT64*)curData = statesAsLanes[laneIndex(instanceIndex, lanePosition)];
        sizeLeft -= SnP_laneLengthInBytes;
        lanePosition++;
        curData += SnP_laneLengthInBytes;
    }

    if (sizeLeft > 0) {
        memcpy( curData, &statesAsLanes[laneIndex(instanceIndex, lanePosition)], sizeLeft);
    }
}

void KeccakP1600times8_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    const V512 *stateAsLanes = states;
    UINT64 *dataAsLanes = (UINT64 *)data;
    unsigned int i;
	V256 index;

    #define Extr( argIndex )  STORE_SCATTER8_64(dataAsLanes+argIndex, index, stateAsLanes[argIndex])
    index = LOAD8_32(7*laneOffset, 6*laneOffset, 5*laneOffset, 4*laneOffset, 3*laneOffset, 2*laneOffset, 1*laneOffset, 0*laneOffset);
    if ( laneCount >= 16 )  {
        Extr( 0 );
        Extr( 1 );
        Extr( 2 );
        Extr( 3 );
        Extr( 4 );
        Extr( 5 );
        Extr( 6 );
        Extr( 7 );
        Extr( 8 );
        Extr( 9 );
        Extr( 10 );
        Extr( 11 );
        Extr( 12 );
        Extr( 13 );
        Extr( 14 );
        Extr( 15 );
        if ( laneCount >= 20 )  {
	        Extr( 16 );
    	    Extr( 17 );
	        Extr( 18 );
    	    Extr( 19 );
            for(i=20; i<laneCount; i++)
                Extr( i );
        }
        else {
            for(i=16; i<laneCount; i++)
                Extr( i );
        }
    }
    else {
        for(i=0; i<laneCount; i++)
            Extr( i );
    }
    #undef  Extr
}

void KeccakP1600times8_ExtractAndAddBytes(const void *states, unsigned int instanceIndex, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    unsigned int sizeLeft = length;
    unsigned int lanePosition = offset/SnP_laneLengthInBytes;
    unsigned int offsetInLane = offset%SnP_laneLengthInBytes;
    const unsigned char *curInput = input;
    unsigned char *curOutput = output;
    const UINT64 *statesAsLanes = states;

    if ((sizeLeft > 0) && (offsetInLane != 0)) {
        unsigned int bytesInLane = SnP_laneLengthInBytes - offsetInLane;
        UINT64 lane = statesAsLanes[laneIndex(instanceIndex, lanePosition)] >> (8 * offsetInLane);
        if (bytesInLane > sizeLeft)
            bytesInLane = sizeLeft;
        sizeLeft -= bytesInLane;
        do {
            *(curOutput++) = *(curInput++) ^ (unsigned char)lane;
            lane >>= 8;
        } while ( --bytesInLane != 0);
        lanePosition++;
    }

    while(sizeLeft >= SnP_laneLengthInBytes) {
        *((UINT64*)curOutput) = *((UINT64*)curInput) ^ statesAsLanes[laneIndex(instanceIndex, lanePosition)];
        sizeLeft -= SnP_laneLengthInBytes;
        lanePosition++;
        curInput += SnP_laneLengthInBytes;
        curOutput += SnP_laneLengthInBytes;
    }

    if (sizeLeft != 0) {
        UINT64 lane = statesAsLanes[laneIndex(instanceIndex, lanePosition)];
        do {
            *(curOutput++) = *(curInput++) ^ (unsigned char)lane;
            lane >>= 8;
        } while ( --sizeLeft != 0);
    }
}

void KeccakP1600times8_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
{
    const V512 *stateAsLanes = states;
    const UINT64 *inAsLanes = (const UINT64 *)input;
    UINT64 *outAsLanes = (UINT64 *)output;
    unsigned int i;
	V256 index;

    #define ExtrAdd( argIndex )  STORE_SCATTER8_64(outAsLanes+argIndex, index, XOR(stateAsLanes[argIndex], LOAD_GATHER8_64(index, inAsLanes+argIndex)))
    index = LOAD8_32(7*laneOffset, 6*laneOffset, 5*laneOffset, 4*laneOffset, 3*laneOffset, 2*laneOffset, 1*laneOffset, 0*laneOffset);
    if ( laneCount >= 16 )  {
        ExtrAdd( 0 );
        ExtrAdd( 1 );
        ExtrAdd( 2 );
        ExtrAdd( 3 );
        ExtrAdd( 4 );
        ExtrAdd( 5 );
        ExtrAdd( 6 );
        ExtrAdd( 7 );
        ExtrAdd( 8 );
        ExtrAdd( 9 );
        ExtrAdd( 10 );
        ExtrAdd( 11 );
        ExtrAdd( 12 );
        ExtrAdd( 13 );
        ExtrAdd( 14 );
        ExtrAdd( 15 );
        if ( laneCount >= 20 )  {
	        ExtrAdd( 16 );
    	    ExtrAdd( 17 );
	        ExtrAdd( 18 );
    	    ExtrAdd( 19 );
            for(i=20; i<laneCount; i++)
                ExtrAdd( i );
        }
        else {
            for(i=16; i<laneCount; i++)
                ExtrAdd( i );
        }
    }
    else {
        for(i=0; i<laneCount; i++)
            ExtrAdd( i );
    }
    #undef  ExtrAdd

}

static ALIGN(KeccakP1600times8_statesAlignment) const UINT64 KeccakP1600RoundConstants[24] = {
    0x0000000000000001ULL,
    0x0000000000008082ULL,
    0x800000000000808aULL,
    0x8000000080008000ULL,
    0x000000000000808bULL,
    0x0000000080000001ULL,
    0x8000000080008081ULL,
    0x8000000000008009ULL,
    0x000000000000008aULL,
    0x0000000000000088ULL,
    0x0000000080008009ULL,
    0x000000008000000aULL,
    0x000000008000808bULL,
    0x800000000000008bULL,
    0x8000000000008089ULL,
    0x8000000000008003ULL,
    0x8000000000008002ULL,
    0x8000000000000080ULL,
    0x000000000000800aULL,
    0x800000008000000aULL,
    0x8000000080008081ULL,
    0x8000000000008080ULL,
    0x0000000080000001ULL,
    0x8000000080008008ULL};

#define KeccakP_DeclareVars \
	V512	_Ba, _Be, _Bi, _Bo, _Bu; \
	V512	_Da, _De, _Di, _Do, _Du; \
	V512	_ba, _be, _bi, _bo, _bu; \
	V512	_ga, _ge, _gi, _go, _gu; \
	V512	_ka, _ke, _ki, _ko, _ku; \
	V512	_ma, _me, _mi, _mo, _mu; \
	V512	_sa, _se, _si, _so, _su

#define KeccakP_ThetaRhoPiChi( _L1, _L2, _L3, _L4, _L5, _Bb1, _Bb2, _Bb3, _Bb4, _Bb5, _Rr1, _Rr2, _Rr3, _Rr4, _Rr5 ) \
    _Bb1 = XOR(_L1, _Da); \
    _Bb2 = XOR(_L2, _De); \
    _Bb3 = XOR(_L3, _Di); \
    _Bb4 = XOR(_L4, _Do); \
    _Bb5 = XOR(_L5, _Du); \
    if (_Rr1 != 0) _Bb1 = ROL(_Bb1, _Rr1); \
    _Bb2 = ROL(_Bb2, _Rr2); \
    _Bb3 = ROL(_Bb3, _Rr3); \
    _Bb4 = ROL(_Bb4, _Rr4); \
    _Bb5 = ROL(_Bb5, _Rr5); \
	_L1 = Chi( _Ba, _Be, _Bi); \
	_L2 = Chi( _Be, _Bi, _Bo); \
	_L3 = Chi( _Bi, _Bo, _Bu); \
	_L4 = Chi( _Bo, _Bu, _Ba); \
	_L5 = Chi( _Bu, _Ba, _Be);

#define KeccakP_ThetaRhoPiChiIota0( _L1, _L2, _L3, _L4, _L5, _rc ) \
	_Ba = XOR5( _ba, _ga, _ka, _ma, _sa ); /* Theta effect */ \
	_Be = XOR5( _be, _ge, _ke, _me, _se ); \
	_Bi = XOR5( _bi, _gi, _ki, _mi, _si ); \
	_Bo = XOR5( _bo, _go, _ko, _mo, _so ); \
	_Bu = XOR5( _bu, _gu, _ku, _mu, _su ); \
	_Da = ROL( _Be, 1 ); \
	_De = ROL( _Bi, 1 ); \
	_Di = ROL( _Bo, 1 ); \
	_Do = ROL( _Bu, 1 ); \
	_Du = ROL( _Ba, 1 ); \
	_Da = XOR( _Da, _Bu ); \
	_De = XOR( _De, _Ba ); \
	_Di = XOR( _Di, _Be ); \
	_Do = XOR( _Do, _Bi ); \
	_Du = XOR( _Du, _Bo ); \
    KeccakP_ThetaRhoPiChi( _L1, _L2, _L3, _L4, _L5, _Ba, _Be, _Bi, _Bo, _Bu,  0, 44, 43, 21, 14 ); \
    _L1 = XOR(_L1, _rc) /* Iota */

#define KeccakP_ThetaRhoPiChi1( _L1, _L2, _L3, _L4, _L5 ) \
    KeccakP_ThetaRhoPiChi( _L1, _L2, _L3, _L4, _L5, _Bi, _Bo, _Bu, _Ba, _Be,  3, 45, 61, 28, 20 )

#define KeccakP_ThetaRhoPiChi2( _L1, _L2, _L3, _L4, _L5 ) \
    KeccakP_ThetaRhoPiChi( _L1, _L2, _L3, _L4, _L5, _Bu, _Ba, _Be, _Bi, _Bo, 18,  1,  6, 25,  8 )

#define KeccakP_ThetaRhoPiChi3( _L1, _L2, _L3, _L4, _L5 ) \
    KeccakP_ThetaRhoPiChi( _L1, _L2, _L3, _L4, _L5, _Be, _Bi, _Bo, _Bu, _Ba, 36, 10, 15, 56, 27 )

#define KeccakP_ThetaRhoPiChi4( _L1, _L2, _L3, _L4, _L5 ) \
    KeccakP_ThetaRhoPiChi( _L1, _L2, _L3, _L4, _L5, _Bo, _Bu, _Ba, _Be, _Bi, 41,  2, 62, 55, 39 )

#define KeccakP_4rounds( i ) \
    KeccakP_ThetaRhoPiChiIota0(_ba, _ge, _ki, _mo, _su, CONST8_64(KeccakP1600RoundConstants[i]) ); \
    KeccakP_ThetaRhoPiChi1(    _ka, _me, _si, _bo, _gu ); \
    KeccakP_ThetaRhoPiChi2(    _sa, _be, _gi, _ko, _mu ); \
    KeccakP_ThetaRhoPiChi3(    _ga, _ke, _mi, _so, _bu ); \
    KeccakP_ThetaRhoPiChi4(    _ma, _se, _bi, _go, _ku ); \
\
    KeccakP_ThetaRhoPiChiIota0(_ba, _me, _gi, _so, _ku, CONST8_64(KeccakP1600RoundConstants[i+1]) ); \
    KeccakP_ThetaRhoPiChi1(    _sa, _ke, _bi, _mo, _gu ); \
    KeccakP_ThetaRhoPiChi2(    _ma, _ge, _si, _ko, _bu ); \
    KeccakP_ThetaRhoPiChi3(    _ka, _be, _mi, _go, _su ); \
    KeccakP_ThetaRhoPiChi4(    _ga, _se, _ki, _bo, _mu ); \
\
    KeccakP_ThetaRhoPiChiIota0(_ba, _ke, _si, _go, _mu, CONST8_64(KeccakP1600RoundConstants[i+2]) ); \
    KeccakP_ThetaRhoPiChi1(    _ma, _be, _ki, _so, _gu ); \
    KeccakP_ThetaRhoPiChi2(    _ga, _me, _bi, _ko, _su ); \
    KeccakP_ThetaRhoPiChi3(    _sa, _ge, _mi, _bo, _ku ); \
    KeccakP_ThetaRhoPiChi4(    _ka, _se, _gi, _mo, _bu ); \
\
    KeccakP_ThetaRhoPiChiIota0(_ba, _be, _bi, _bo, _bu, CONST8_64(KeccakP1600RoundConstants[i+3]) ); \
    KeccakP_ThetaRhoPiChi1(    _ga, _ge, _gi, _go, _gu ); \
    KeccakP_ThetaRhoPiChi2(    _ka, _ke, _ki, _ko, _ku ); \
    KeccakP_ThetaRhoPiChi3(    _ma, _me, _mi, _mo, _mu ); \
    KeccakP_ThetaRhoPiChi4(    _sa, _se, _si, _so, _su )

#ifdef KeccakP1600times8_fullUnrolling

#define rounds12 \
	KeccakP_4rounds( 12 ); \
	KeccakP_4rounds( 16 ); \
	KeccakP_4rounds( 20 )

#define rounds24 \
	KeccakP_4rounds( 0 ); \
	KeccakP_4rounds( 4 ); \
	KeccakP_4rounds( 8 ); \
	KeccakP_4rounds( 12 ); \
	KeccakP_4rounds( 16 ); \
	KeccakP_4rounds( 20 )

#elif (KeccakP1600times8_unrolling == 4)

#define rounds12 \
	i = 12; \
	do { \
		KeccakP_4rounds( i ); \
	} while( (i += 4) < 24 )

#define rounds24 \
	i = 0; \
	do { \
		KeccakP_4rounds( i ); \
	} while( (i += 4) < 24 )

#elif (KeccakP1600times8_unrolling == 12)

#define rounds12 \
	KeccakP_4rounds( 12 ); \
	KeccakP_4rounds( 16 ); \
	KeccakP_4rounds( 20 )

#define rounds24 \
	i = 0; \
	do { \
		KeccakP_4rounds( i ); \
		KeccakP_4rounds( i+4 ); \
		KeccakP_4rounds( i+8 ); \
	} while( (i += 12) < 24 )

#else
#error "Unrolling is not correctly specified!"
#endif

#define copyFromState(pState) \
    _ba = pState[ 0]; \
    _be = pState[ 1]; \
    _bi = pState[ 2]; \
    _bo = pState[ 3]; \
    _bu = pState[ 4]; \
    _ga = pState[ 5]; \
    _ge = pState[ 6]; \
    _gi = pState[ 7]; \
    _go = pState[ 8]; \
    _gu = pState[ 9]; \
    _ka = pState[10]; \
    _ke = pState[11]; \
    _ki = pState[12]; \
    _ko = pState[13]; \
    _ku = pState[14]; \
    _ma = pState[15]; \
    _me = pState[16]; \
    _mi = pState[17]; \
    _mo = pState[18]; \
    _mu = pState[19]; \
    _sa = pState[20]; \
    _se = pState[21]; \
    _si = pState[22]; \
    _so = pState[23]; \
    _su = pState[24]

#define copyToState(pState) \
    pState[ 0] = _ba; \
    pState[ 1] = _be; \
    pState[ 2] = _bi; \
    pState[ 3] = _bo; \
    pState[ 4] = _bu; \
    pState[ 5] = _ga; \
    pState[ 6] = _ge; \
    pState[ 7] = _gi; \
    pState[ 8] = _go; \
    pState[ 9] = _gu; \
    pState[10] = _ka; \
    pState[11] = _ke; \
    pState[12] = _ki; \
    pState[13] = _ko; \
    pState[14] = _ku; \
    pState[15] = _ma; \
    pState[16] = _me; \
    pState[17] = _mi; \
    pState[18] = _mo; \
    pState[19] = _mu; \
    pState[20] = _sa; \
    pState[21] = _se; \
    pState[22] = _si; \
    pState[23] = _so; \
    pState[24] = _su

void KeccakP1600times8_PermuteAll_24rounds(void *states)
{
    V512 *statesAsLanes = states;
	KeccakP_DeclareVars;
    #ifndef KeccakP1600times8_fullUnrolling
    unsigned int i;
    #endif

    copyFromState(statesAsLanes);
	rounds24;
    copyToState(statesAsLanes);
}

void KeccakP1600times8_PermuteAll_12rounds(void *states)
{
    V512 *statesAsLanes = states;
	KeccakP_DeclareVars;
    #if (KeccakP1600times8_unrolling < 12)
    unsigned int i;
    #endif

    copyFromState(statesAsLanes);
	rounds12;
    copyToState(statesAsLanes);
}

size_t KeccakF1600times8_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen)
{
	size_t dataMinimumSize = (laneOffsetParallel*7 + laneCount)*8;

    if (laneCount == 21) {
	    #ifndef KeccakP1600times8_fullUnrolling
        unsigned int i;
		#endif
        const unsigned char *dataStart = data;
	    V512 *statesAsLanes = states;
	    const UINT64 *dataAsLanes = (const UINT64 *)data;
		KeccakP_DeclareVars;
		V256 index;

	    copyFromState(statesAsLanes);
	    index = LOAD8_32(7*laneOffsetParallel, 6*laneOffsetParallel, 5*laneOffsetParallel, 4*laneOffsetParallel, 3*laneOffsetParallel, 2*laneOffsetParallel, 1*laneOffsetParallel, 0*laneOffsetParallel);
        while(dataByteLen >= dataMinimumSize) {
		    #define Add_In( argLane, argIndex )  argLane = XOR(argLane, LOAD_GATHER8_64(index, dataAsLanes+argIndex))
	        Add_In( _ba, 0 );
	        Add_In( _be, 1 );
	        Add_In( _bi, 2 );
	        Add_In( _bo, 3 );
	        Add_In( _bu, 4 );
        	Add_In( _ga, 5 );
	        Add_In( _ge, 6 );
    	    Add_In( _gi, 7 );
	        Add_In( _go, 8 );
    	    Add_In( _gu, 9 );
        	Add_In( _ka, 10 );
        	Add_In( _ke, 11 );
        	Add_In( _ki, 12 );
        	Add_In( _ko, 13 );
        	Add_In( _ku, 14 );
         	Add_In( _ma, 15 );
         	Add_In( _me, 16 );
   	     	Add_In( _mi, 17 );
         	Add_In( _mo, 18 );
   	     	Add_In( _mu, 19 );
         	Add_In( _sa, 20 );
		    #undef  Add_In
            rounds24;
            dataAsLanes += laneOffsetSerial;
            dataByteLen -= laneOffsetSerial*8;
        }
        copyToState(statesAsLanes);
        return (const unsigned char *)dataAsLanes - dataStart;
    }
    else {
        const unsigned char *dataStart = data;

        while(dataByteLen >= dataMinimumSize) {
            KeccakP1600times8_AddLanesAll(states, data, laneCount, laneOffsetParallel);
            KeccakP1600times8_PermuteAll_24rounds(states);
            data += laneOffsetSerial*8;
            dataByteLen -= laneOffsetSerial*8;
        }
        return data - dataStart;
    }
}

size_t KeccakP1600times8_12rounds_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen)
{
	size_t dataMinimumSize = (laneOffsetParallel*7 + laneCount)*8;

    if (laneCount == 21) {
	    #if (KeccakP1600times8_unrolling < 12)
        unsigned int i;
		#endif
        const unsigned char *dataStart = data;
	    V512 *statesAsLanes = states;
	    const UINT64 *dataAsLanes = (const UINT64 *)data;
		KeccakP_DeclareVars;
		V256 index;

	    copyFromState(statesAsLanes);
	    index = LOAD8_32(7*laneOffsetParallel, 6*laneOffsetParallel, 5*laneOffsetParallel, 4*laneOffsetParallel, 3*laneOffsetParallel, 2*laneOffsetParallel, 1*laneOffsetParallel, 0*laneOffsetParallel);
        while(dataByteLen >= dataMinimumSize) {
		    #define Add_In( argLane, argIndex )  argLane = XOR(argLane, LOAD_GATHER8_64(index, dataAsLanes+argIndex))
	        Add_In( _ba, 0 );
	        Add_In( _be, 1 );
	        Add_In( _bi, 2 );
	        Add_In( _bo, 3 );
	        Add_In( _bu, 4 );
        	Add_In( _ga, 5 );
	        Add_In( _ge, 6 );
    	    Add_In( _gi, 7 );
	        Add_In( _go, 8 );
    	    Add_In( _gu, 9 );
        	Add_In( _ka, 10 );
        	Add_In( _ke, 11 );
        	Add_In( _ki, 12 );
        	Add_In( _ko, 13 );
        	Add_In( _ku, 14 );
         	Add_In( _ma, 15 );
         	Add_In( _me, 16 );
   	     	Add_In( _mi, 17 );
         	Add_In( _mo, 18 );
   	     	Add_In( _mu, 19 );
         	Add_In( _sa, 20 );
		    #undef  Add_In
            rounds12;
            dataAsLanes += laneOffsetSerial;
            dataByteLen -= laneOffsetSerial*8;
        }
        copyToState(statesAsLanes);
        return (const unsigned char *)dataAsLanes - dataStart;
    }
    else {
        const unsigned char *dataStart = data;

        while(dataByteLen >= dataMinimumSize) {
            KeccakP1600times8_AddLanesAll(states, data, laneCount, laneOffsetParallel);
            KeccakP1600times8_PermuteAll_12rounds(states);
            data += laneOffsetSerial*8;
            dataByteLen -= laneOffsetSerial*8;
        }
        return data - dataStart;
    }
}
