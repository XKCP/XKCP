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

/*
 * We would like to thank Vladimir Sedach, we have used parts of his Keccak AVX-512 C++ code.
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
#include "brg_endian.h"
#include "KeccakP-1600-AVX512-config.h"

#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
#error Expecting a little-endian platform
#endif

#ifdef KeccakP1600_fullUnrolling
#define FullUnrolling
#else
#define Unrolling KeccakP1600_unrolling
#endif

/* Comment the define hereunder when compiling for a CPU with AVX-512 SIMD */
/* 
 * Warning: This code has only been tested on Haswell (AVX2) with SIMULATE_AVX512 defined,
 *          errors will occur if we did a bad interpretation of the AVX-512 intrinsics' 
 *          API or functionality.
 */
/* #define SIMULATE_AVX512 */

typedef uint8_t     UINT8;
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

static __m512i _mm512_rolv_epi64(__m512i a, __m512i offset)
{
    __m512i r;
    unsigned int i;

    for ( i = 0; i < 8; ++i )
        r.x[i] = (a.x[i] << offset.x[i]) | (a.x[i] >> (64-offset.x[i]));
    return(r);
}

static __m512i _mm512_setr_epi64(UINT64 a, UINT64 b, UINT64 c, UINT64 d, UINT64 e, UINT64 f, UINT64 g, UINT64 h)
{
    __m512i r;

    r.x[0] = a;
    r.x[1] = b;
    r.x[2] = c;
    r.x[3] = d;
    r.x[4] = e;
    r.x[5] = f;
    r.x[6] = g;
    r.x[7] = h;
    return(r);
}

static __m512i _mm512_permutexvar_epi64(__m512i idx, __m512i v)
{
    __m512i r;
    unsigned int i;

    for ( i = 0; i < 8; ++i )
        r.x[i] = v.x[idx.x[i]];
    return(r);
}

static __m512i _mm512_permutex2var_epi64(__m512i a, __m512i idx, __m512i b)
{
    __m512i r;
    unsigned int i;
    unsigned int index;

    for ( i = 0; i < 8; ++i ) {
        index = idx.x[i] & 7;
        r.x[i] = (idx.x[i] & 8) ? b.x[index] : a.x[index];
    }
    return(r);
}

static __m512i _mm512_unpacklo_epi64(__m512i a, __m512i b)
{
    __m512i r;
    unsigned int i;

    for ( i = 0; i < 8; i += 2 ) {
        r.x[i]   = a.x[i];
        r.x[i+1] = b.x[i];
    }
    return(r);
}

static __m512i _mm512_unpackhi_epi64(__m512i a, __m512i b)
{
    __m512i r;
    unsigned int i;

    for ( i = 0; i < 8; i += 2 ) {
        r.x[i]   = a.x[i+1];
        r.x[i+1] = b.x[i+1];
    }
    return(r);
}

static __m512i _mm512_mask_blend_epi64(unsigned char mask, __m512i a, __m512i b)
{
    __m512i r;
    unsigned int i;

    for ( i = 0; i < 8; ++i, mask >>= 1 )
        r.x[i] = (mask & 1) ? b.x[i] : a.x[i];
    return(r);
}

static __m512i _mm512_maskz_loadu_epi64( unsigned char mask, const void * a)
{
    __m512i r;
    unsigned int i;
    const UINT64 *p = a;

    for ( i = 0; i < 8; ++i, mask >>= 1 )
        r.x[i] = (mask & 1) ? p[i] : 0;
    return(r);
}

static void _mm512_mask_storeu_epi64( void * a, unsigned char mask, __m512i v)
{
    unsigned int i;
    UINT64 *p = a;

    for ( i = 0; i < 8; ++i, mask >>= 1 )
        if ( mask & 1 )
            p[i] = v.x[i];
}


#endif

typedef __m512i     V512;

#define XOR(a,b)                    _mm512_xor_si512(a,b)
#define XOR3(a,b,c)                 _mm512_ternarylogic_epi64(a,b,c,0x96)
#define XOR5(a,b,c,d,e)             XOR3(XOR3(a,b,c),d,e)
#define ROL(a,offset)               _mm512_rol_epi64(a,offset)
#define Chi(a,b,c)                  _mm512_ternarylogic_epi64(a,b,c,0xD2)

#define LOAD_Lanes(m,a)             _mm512_maskz_loadu_epi64(m,a)
#define LOAD_Lane(a)                LOAD_Lanes(0x01,a)
#define LOAD_Plane(a)               LOAD_Lanes(0x1F,a)
#define LOAD_8Lanes(a)              LOAD_Lanes(0xFF,a)
#define STORE_Lanes(a,m,v)          _mm512_mask_storeu_epi64(a,m,v)
#define STORE_Lane(a,v)             STORE_Lanes(a,0x01,v)
#define STORE_Plane(a,v)            STORE_Lanes(a,0x1F,v)
#define STORE_8Lanes(a,v)           STORE_Lanes(a,0xFF,v)

/* ---------------------------------------------------------------- */

void KeccakP1600_Initialize(void *state)
{
    memset(state, 0, 1600/8);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    UINT8  *stateAsBytes;
    UINT64 *stateAsLanes;

    for( stateAsBytes = (UINT8*)state; ((offset % 8) != 0) && (length != 0); ++offset, --length)
        stateAsBytes[offset] ^= *(data++);
    for (stateAsLanes = (UINT64*)(stateAsBytes + offset); length >= 8*8; stateAsLanes += 8, data += 8*8, length -= 8*8)
        STORE_8Lanes( stateAsLanes, XOR(LOAD_8Lanes(stateAsLanes), LOAD_8Lanes((const UINT64*)data)));
    for (/* empty */; length >= 8; ++stateAsLanes, data += 8, length -= 8)
        STORE_Lane( stateAsLanes, XOR(LOAD_Lane(stateAsLanes), LOAD_Lane((const UINT64*)data)));
    for ( stateAsBytes = (UINT8*)stateAsLanes; length != 0; --length)
        *(stateAsBytes++) ^= *(data++);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy((unsigned char*)state+offset, data, length);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy(data, (unsigned char*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    UINT8  *stateAsBytes;
    UINT64 *stateAsLanes;

    for( stateAsBytes = (UINT8*)state; ((offset % 8) != 0) && (length != 0); ++offset, --length)
        *(output++) = stateAsBytes[offset] ^ *(input++);
    for (stateAsLanes = (UINT64*)(stateAsBytes + offset); length >= 8*8; stateAsLanes += 8, input += 8*8, output += 8*8, length -= 8*8)
        STORE_8Lanes( (UINT64*)output, XOR(LOAD_8Lanes(stateAsLanes), LOAD_8Lanes((const UINT64*)input)));
    for (/* empty */; length >= 8; ++stateAsLanes, input += 8, output += 8, length -= 8)
        STORE_Lane( (UINT64*)output, XOR(LOAD_Lane(stateAsLanes), LOAD_Lane((const UINT64*)input)));
    for ( stateAsBytes = (UINT8*)stateAsLanes; length != 0; --length)
        *(output++) = *(stateAsBytes++) ^ *(input++);
}

const UINT64 KeccakP1600RoundConstants[24] = {
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
    0x8000000080008008ULL };

#define KeccakP_DeclareVars \
    V512    b0, b1, b2, b3, b4; \
    V512    Baeiou, Gaeiou, Kaeiou, Maeiou, Saeiou; \
    V512    moveThetaPrev = _mm512_setr_epi64(4, 0, 1, 2, 3, 5, 6, 7); \
    V512    moveThetaNext = _mm512_setr_epi64(1, 2, 3, 4, 0, 5, 6, 7); \
    V512    rhoB = _mm512_setr_epi64( 0,  1, 62, 28, 27, 0, 0, 0); \
    V512    rhoG = _mm512_setr_epi64(36, 44,  6, 55, 20, 0, 0, 0); \
    V512    rhoK = _mm512_setr_epi64( 3, 10, 43, 25, 39, 0, 0, 0); \
    V512    rhoM = _mm512_setr_epi64(41, 45, 15, 21,  8, 0, 0, 0); \
    V512    rhoS = _mm512_setr_epi64(18,  2, 61, 56, 14, 0, 0, 0); \
    V512    pi1B = _mm512_setr_epi64(0, 3, 1, 4, 2, 5, 6, 7); \
    V512    pi1G = _mm512_setr_epi64(1, 4, 2, 0, 3, 5, 6, 7); \
    V512    pi1K = _mm512_setr_epi64(2, 0, 3, 1, 4, 5, 6, 7); \
    V512    pi1M = _mm512_setr_epi64(3, 1, 4, 2, 0, 5, 6, 7); \
    V512    pi1S = _mm512_setr_epi64(4, 2, 0, 3, 1, 5, 6, 7); \
    V512    pi2S1 = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 0+8, 2+8); \
    V512    pi2S2 = _mm512_setr_epi64(0, 1, 2, 3, 4, 5, 1+8, 3+8); \
    V512    pi2BG = _mm512_setr_epi64(0, 1, 0+8, 1+8, 6, 5, 6, 7); \
    V512    pi2KM = _mm512_setr_epi64(2, 3, 2+8, 3+8, 7, 5, 6, 7); \
    V512    pi2S3 = _mm512_setr_epi64(4, 5, 4+8, 5+8, 4, 5, 6, 7);

#define copyFromState(pState) \
    Baeiou = LOAD_Plane(pState+ 0); \
    Gaeiou = LOAD_Plane(pState+ 5); \
    Kaeiou = LOAD_Plane(pState+10); \
    Maeiou = LOAD_Plane(pState+15); \
    Saeiou = LOAD_Plane(pState+20);

#define copyToState(pState) \
    STORE_Plane(pState+ 0, Baeiou); \
    STORE_Plane(pState+ 5, Gaeiou); \
    STORE_Plane(pState+10, Kaeiou); \
    STORE_Plane(pState+15, Maeiou); \
    STORE_Plane(pState+20, Saeiou);

#define KeccakP_Round(i) \
    /* Theta */ \
    b0 = XOR5( Baeiou, Gaeiou, Kaeiou, Maeiou, Saeiou ); \
    b1 = _mm512_permutexvar_epi64(moveThetaPrev, b0); \
    b0 = _mm512_permutexvar_epi64(moveThetaNext, b0); \
    b0 = _mm512_rol_epi64(b0, 1); \
    Baeiou = XOR3( Baeiou, b0, b1 ); \
    Gaeiou = XOR3( Gaeiou, b0, b1 ); \
    Kaeiou = XOR3( Kaeiou, b0, b1 ); \
    Maeiou = XOR3( Maeiou, b0, b1 ); \
    Saeiou = XOR3( Saeiou, b0, b1 ); \
    /* Rho */ \
    Baeiou = _mm512_rolv_epi64(Baeiou, rhoB); \
    Gaeiou = _mm512_rolv_epi64(Gaeiou, rhoG); \
    Kaeiou = _mm512_rolv_epi64(Kaeiou, rhoK); \
    Maeiou = _mm512_rolv_epi64(Maeiou, rhoM); \
    Saeiou = _mm512_rolv_epi64(Saeiou, rhoS); \
    /* Pi 1 */ \
    b0 = _mm512_permutexvar_epi64(pi1B, Baeiou); \
    b1 = _mm512_permutexvar_epi64(pi1G, Gaeiou); \
    b2 = _mm512_permutexvar_epi64(pi1K, Kaeiou); \
    b3 = _mm512_permutexvar_epi64(pi1M, Maeiou); \
    b4 = _mm512_permutexvar_epi64(pi1S, Saeiou); \
    /* Chi */ \
    Baeiou = Chi(b0, b1, b2); \
    Gaeiou = Chi(b1, b2, b3); \
    Kaeiou = Chi(b2, b3, b4); \
    Maeiou = Chi(b3, b4, b0); \
    Saeiou = Chi(b4, b0, b1); \
    /* Iota */ \
    Baeiou = XOR(Baeiou, LOAD_Lane(KeccakP1600RoundConstants+i)); \
    /* Pi 2 */ \
    b0 = _mm512_unpacklo_epi64(Baeiou, Gaeiou); \
    b1 = _mm512_unpacklo_epi64(Kaeiou, Maeiou); \
    b0 = _mm512_permutex2var_epi64(b0, pi2S1, Saeiou); \
    b2 = _mm512_unpackhi_epi64(Baeiou, Gaeiou); \
    b3 = _mm512_unpackhi_epi64(Kaeiou, Maeiou); \
    b2 = _mm512_permutex2var_epi64(b2, pi2S2, Saeiou); \
    Baeiou = _mm512_permutex2var_epi64(b0, pi2BG, b1); \
    Gaeiou = _mm512_permutex2var_epi64(b2, pi2BG, b3); \
    Kaeiou = _mm512_permutex2var_epi64(b0, pi2KM, b1); \
    Maeiou = _mm512_permutex2var_epi64(b2, pi2KM, b3); \
    b0 = _mm512_permutex2var_epi64(b0, pi2S3, b1); \
    Saeiou = _mm512_mask_blend_epi64(0x10, b0, Saeiou)

#ifdef FullUnrolling

#define rounds12 \
    KeccakP_Round( 12 ); \
    KeccakP_Round( 13 ); \
    KeccakP_Round( 14 ); \
    KeccakP_Round( 15 ); \
    KeccakP_Round( 16 ); \
    KeccakP_Round( 17 ); \
    KeccakP_Round( 18 ); \
    KeccakP_Round( 19 ); \
    KeccakP_Round( 20 ); \
    KeccakP_Round( 21 ); \
    KeccakP_Round( 22 ); \
    KeccakP_Round( 23 )

#define rounds24 \
    KeccakP_Round(  0 ); \
    KeccakP_Round(  1 ); \
    KeccakP_Round(  2 ); \
    KeccakP_Round(  3 ); \
    KeccakP_Round(  4 ); \
    KeccakP_Round(  5 ); \
    KeccakP_Round(  6 ); \
    KeccakP_Round(  7 ); \
    KeccakP_Round(  8 ); \
    KeccakP_Round(  9 ); \
    KeccakP_Round( 10 ); \
    KeccakP_Round( 11 ); \
    KeccakP_Round( 12 ); \
    KeccakP_Round( 13 ); \
    KeccakP_Round( 14 ); \
    KeccakP_Round( 15 ); \
    KeccakP_Round( 16 ); \
    KeccakP_Round( 17 ); \
    KeccakP_Round( 18 ); \
    KeccakP_Round( 19 ); \
    KeccakP_Round( 20 ); \
    KeccakP_Round( 21 ); \
    KeccakP_Round( 22 ); \
    KeccakP_Round( 23 )

#elif (Unrolling == 6)

#define rounds12 \
    i = 12; \
    do { \
        KeccakP_Round( i+ 0 ); \
        KeccakP_Round( i+ 1 ); \
        KeccakP_Round( i+ 2 ); \
        KeccakP_Round( i+ 3 ); \
        KeccakP_Round( i+ 4 ); \
        KeccakP_Round( i+ 5 ); \
    } while( (i += 6) < 24 )

#define rounds24 \
    i = 0; \
    do { \
        KeccakP_Round( i+ 0 ); \
        KeccakP_Round( i+ 1 ); \
        KeccakP_Round( i+ 2 ); \
        KeccakP_Round( i+ 3 ); \
        KeccakP_Round( i+ 4 ); \
        KeccakP_Round( i+ 5 ); \
    } while( (i += 6) < 24 )

#elif (Unrolling == 12)

#define rounds12 \
    KeccakP_Round( 12 ); \
    KeccakP_Round( 13 ); \
    KeccakP_Round( 14 ); \
    KeccakP_Round( 15 ); \
    KeccakP_Round( 16 ); \
    KeccakP_Round( 17 ); \
    KeccakP_Round( 18 ); \
    KeccakP_Round( 19 ); \
    KeccakP_Round( 20 ); \
    KeccakP_Round( 21 ); \
    KeccakP_Round( 22 ); \
    KeccakP_Round( 23 )

#define rounds24 \
    i = 0; \
    do { \
        KeccakP_Round( i+ 0 ); \
        KeccakP_Round( i+ 1 ); \
        KeccakP_Round( i+ 2 ); \
        KeccakP_Round( i+ 3 ); \
        KeccakP_Round( i+ 4 ); \
        KeccakP_Round( i+ 5 ); \
        KeccakP_Round( i+ 6 ); \
        KeccakP_Round( i+ 7 ); \
        KeccakP_Round( i+ 8 ); \
        KeccakP_Round( i+ 9 ); \
        KeccakP_Round( i+10 ); \
        KeccakP_Round( i+11 ); \
    } while( (i += 12) < 24 )

#else
#error "Unrolling is not correctly specified!"
#endif

void KeccakP1600_Permute_Nrounds(void *state, unsigned int nrounds)
{
    KeccakP_DeclareVars
    unsigned int i;
    UINT64 *stateAsLanes = (UINT64*)state;

    copyFromState(stateAsLanes);
    if ((nrounds & 1) != 0) {
        KeccakP_Round( 24-nrounds );
        --nrounds;
    }
    if ((nrounds & 2) != 0) {
        KeccakP_Round( 24+0-nrounds );
        KeccakP_Round( 24+1-nrounds );
        nrounds -= 2;
    }
    for (i = 24-nrounds; i < 24; i+= 4) {
        KeccakP_Round( i );
        KeccakP_Round( i+1 );
        KeccakP_Round( i+2 );
        KeccakP_Round( i+3 );
    }
    copyToState(stateAsLanes);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_Permute_12rounds(void *state)
{
    KeccakP_DeclareVars
    #ifndef KeccakP1600_fullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;

    copyFromState(stateAsLanes);
    rounds12;
    copyToState(stateAsLanes);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_Permute_24rounds(void *state)
{
    KeccakP_DeclareVars
    #ifndef KeccakP1600_fullUnrolling
    unsigned int i;
    #endif
    UINT64 *stateAsLanes = (UINT64*)state;

    copyFromState(stateAsLanes);
    rounds24;
    copyToState(stateAsLanes);
}

size_t KeccakF1600_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
{
    size_t originalDataByteLen = dataByteLen;

    if (laneCount == 21) {
        KeccakP_DeclareVars;
        #ifndef KeccakP1600_fullUnrolling
        unsigned int i;
        #endif
        UINT64 *stateAsLanes = (UINT64*)state;
        UINT64 *inDataAsLanes = (UINT64*)data;

        copyFromState(stateAsLanes);
        while(dataByteLen >= 21*8) {
            Baeiou = XOR(Baeiou, LOAD_Plane(inDataAsLanes+ 0));
            Gaeiou = XOR(Gaeiou, LOAD_Plane(inDataAsLanes+ 5));
            Kaeiou = XOR(Kaeiou, LOAD_Plane(inDataAsLanes+10));
            Maeiou = XOR(Maeiou, LOAD_Plane(inDataAsLanes+15));
            Saeiou = XOR(Saeiou, LOAD_Lane(inDataAsLanes+20));
            rounds24;
            inDataAsLanes += 21;
            dataByteLen -= 21*8;
        }
        copyToState(stateAsLanes);
    }
    else {
        while(dataByteLen >= laneCount*8) {
            KeccakP1600_AddBytes(state, data, 0, laneCount*8);
            KeccakP1600_Permute_24rounds(state);
            data += laneCount*8;
            dataByteLen -= laneCount*8;
        }
    }
    return originalDataByteLen - dataByteLen;
}

size_t KeccakP1600_12rounds_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
{
    size_t originalDataByteLen = dataByteLen;

    if (laneCount == 21) {
        KeccakP_DeclareVars;
        #if !defined(KeccakP1600_fullUnrolling) && (KeccakP1600_unrolling < 12)
        unsigned int i;
        #endif
        UINT64 *stateAsLanes = (UINT64*)state;
        UINT64 *inDataAsLanes = (UINT64*)data;

        copyFromState(stateAsLanes);
        while(dataByteLen >= 21*8) {
            Baeiou = XOR(Baeiou, LOAD_Plane(inDataAsLanes+ 0));
            Gaeiou = XOR(Gaeiou, LOAD_Plane(inDataAsLanes+ 5));
            Kaeiou = XOR(Kaeiou, LOAD_Plane(inDataAsLanes+10));
            Maeiou = XOR(Maeiou, LOAD_Plane(inDataAsLanes+15));
            Saeiou = XOR(Saeiou, LOAD_Lane(inDataAsLanes+20));
            rounds12;
            inDataAsLanes += 21;
            dataByteLen -= 21*8;
        }
        copyToState(stateAsLanes);
    }
    else {
        while(dataByteLen >= laneCount*8) {
            KeccakP1600_AddBytes(state, data, 0, laneCount*8);
            KeccakP1600_Permute_24rounds(state);
            data += laneCount*8;
            dataByteLen -= laneCount*8;
        }
    }
    return originalDataByteLen - dataByteLen;
}