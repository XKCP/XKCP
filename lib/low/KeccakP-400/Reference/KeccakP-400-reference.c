/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Keccak-p[400] in a SnP-compatible way.
Please refer to SnP-documentation.h for more details.

This implementation comes with KeccakP-400-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brg_endian.h"
#ifdef KeccakReference
#include "displayIntermediateValues.h"
#endif

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef UINT16 tKeccakLane;

#define maxNrRounds 20
#define nrLanes 25
#define index(x, y) (((x)%5)+5*((y)%5))

#ifdef KeccakReference

static tKeccakLane KeccakRoundConstants[maxNrRounds];
static unsigned int KeccakRhoOffsets[nrLanes];

/* ---------------------------------------------------------------- */

void KeccakP400_InitializeRoundConstants(void);
void KeccakP400_InitializeRhoOffsets(void);
static int LFSR86540(UINT8 *LFSR);

void KeccakP400_StaticInitialize(void)
{
    if (sizeof(tKeccakLane) != 2) {
        printf("tKeccakLane should be 16-bit wide\n");
        abort();
    }
    KeccakP400_InitializeRoundConstants();
    KeccakP400_InitializeRhoOffsets();
}

void KeccakP400_InitializeRoundConstants(void)
{
    UINT8 LFSRstate = 0x01;
    unsigned int i, j, bitPosition;

    for(i=0; i<maxNrRounds; i++) {
        KeccakRoundConstants[i] = 0;
        for(j=0; j<7; j++) {
            bitPosition = (1<<j)-1; /* 2^j-1 */
            if (LFSR86540(&LFSRstate) && (bitPosition < (sizeof(tKeccakLane)*8)))
                KeccakRoundConstants[i] ^= (tKeccakLane)(1<<bitPosition);
        }
    }
}

void KeccakP400_InitializeRhoOffsets(void)
{
    unsigned int x, y, t, newX, newY;

    KeccakRhoOffsets[index(0, 0)] = 0;
    x = 1;
    y = 0;
    for(t=0; t<24; t++) {
        KeccakRhoOffsets[index(x, y)] = ((t+1)*(t+2)/2) % (sizeof(tKeccakLane) * 8);
        newX = (0*x+1*y) % 5;
        newY = (2*x+3*y) % 5;
        x = newX;
        y = newY;
    }
}

static int LFSR86540(UINT8 *LFSR)
{
    int result = ((*LFSR) & 0x01) != 0;
    if (((*LFSR) & 0x80) != 0)
        /* Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1 */
        (*LFSR) = ((*LFSR) << 1) ^ 0x71;
    else
        (*LFSR) <<= 1;
    return result;
}

#else

static const tKeccakLane KeccakRoundConstants[maxNrRounds] =
{
    0x0001,
    0x8082,
    0x808a,
    0x8000,
    0x808b,
    0x0001,
    0x8081,
    0x8009,
    0x008a,
    0x0088,
    0x8009,
    0x000a,
    0x808b,
    0x008b,
    0x8089,
    0x8003,
    0x8002,
    0x0080,
    0x800a,
    0x000a,
};

static const unsigned int KeccakRhoOffsets[nrLanes] =
{
     0,  1, 14, 12, 11,  4, 12,  6,  7,  4,  3, 10, 11,  9,  7,  9, 13, 15,  5,  8,  2,  2, 13,  8, 14
};

#endif

/* ---------------------------------------------------------------- */

void KeccakP400_Initialize(void *state)
{
    memset(state, 0, nrLanes * sizeof(tKeccakLane));
}

/* ---------------------------------------------------------------- */

void KeccakP400_AddByte(void *state, unsigned char byte, unsigned int offset)
{
    assert(offset < 50);
    ((unsigned char *)state)[offset] ^= byte;
}

/* ---------------------------------------------------------------- */

void KeccakP400_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int i;

    assert(offset < 50);
    assert(offset+length <= 50);
    for(i=0; i<length; i++)
        ((unsigned char *)state)[offset+i] ^= data[i];
}

/* ---------------------------------------------------------------- */

void KeccakP400_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    assert(offset < 50);
    assert(offset+length <= 50);
    memcpy((unsigned char*)state+offset, data, length);
}

/* ---------------------------------------------------------------- */

void KeccakP400_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    assert(byteCount <= 50);
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

static void fromBytesToWords(tKeccakLane *stateAsWords, const unsigned char *state);
static void fromWordsToBytes(unsigned char *state, const tKeccakLane *stateAsWords);
void KeccakP400OnWords(tKeccakLane *state, unsigned int nrRounds);
void KeccakP400Round(tKeccakLane *state, unsigned int indexRound);
static void theta(tKeccakLane *A);
static void rho(tKeccakLane *A);
static void pi(tKeccakLane *A);
static void chi(tKeccakLane *A);
static void iota(tKeccakLane *A, unsigned int indexRound);

void KeccakP400_Permute_Nrounds(void *state, unsigned int nrounds)
{
#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
    tKeccakLane stateAsWords[nrLanes];
#endif

#ifdef KeccakReference
    displayStateAsBytes(1, "Input of permutation", (const unsigned char *)state, nrLanes * sizeof(tKeccakLane) * 8);
#endif
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    KeccakP400OnWords((tKeccakLane*)state, nrounds);
#else
    fromBytesToWords(stateAsWords, (const unsigned char *)state);
    KeccakP400OnWords(stateAsWords, nrounds);
    fromWordsToBytes((unsigned char *)state, stateAsWords);
#endif
#ifdef KeccakReference
    displayStateAsBytes(1, "State after permutation", (const unsigned char *)state, nrLanes * sizeof(tKeccakLane) * 8);
#endif
}

void KeccakP400_Permute_20rounds(void *state)
{
#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
    tKeccakLane stateAsWords[nrLanes];
#endif

#ifdef KeccakReference
    displayStateAsBytes(1, "Input of permutation", (const unsigned char *)state, nrLanes * sizeof(tKeccakLane) * 8);
#endif
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    KeccakP400OnWords((tKeccakLane*)state, maxNrRounds);
#else
    fromBytesToWords(stateAsWords, (const unsigned char *)state);
    KeccakP400OnWords(stateAsWords, maxNrRounds);
    fromWordsToBytes((unsigned char *)state, stateAsWords);
#endif
#ifdef KeccakReference
    displayStateAsBytes(1, "State after permutation", (const unsigned char *)state, nrLanes * sizeof(tKeccakLane) * 8);
#endif
}

static void fromBytesToWords(tKeccakLane *stateAsWords, const unsigned char *state)
{
    unsigned int i, j;

    for(i=0; i<nrLanes; i++) {
        stateAsWords[i] = 0;
        for(j=0; j<sizeof(tKeccakLane); j++)
            stateAsWords[i] |= (tKeccakLane)(state[i*sizeof(tKeccakLane)+j]) << (8*j);
    }
}

static void fromWordsToBytes(unsigned char *state, const tKeccakLane *stateAsWords)
{
    unsigned int i, j;

    for(i=0; i<nrLanes; i++)
        for(j=0; j<sizeof(tKeccakLane); j++)
            state[i*sizeof(tKeccakLane)+j] = (stateAsWords[i] >> (8*j)) & 0xFF;
}

void KeccakP400OnWords(tKeccakLane *state, unsigned int nrRounds)
{
    unsigned int i;

#ifdef KeccakReference
    displayStateAsLanes(3, "Same, with lanes as 16-bit words", state, 400);
#endif

    for(i=(maxNrRounds-nrRounds); i<maxNrRounds; i++)
        KeccakP400Round(state, i);
}

void KeccakP400Round(tKeccakLane *state, unsigned int indexRound)
{
#ifdef KeccakReference
    displayRoundNumber(3, indexRound);
#endif

    theta(state);
#ifdef KeccakReference
    displayStateAsLanes(3, "After theta", state, 400);
#endif

    rho(state);
#ifdef KeccakReference
    displayStateAsLanes(3, "After rho", state, 400);
#endif

    pi(state);
#ifdef KeccakReference
    displayStateAsLanes(3, "After pi", state, 400);
#endif

    chi(state);
#ifdef KeccakReference
    displayStateAsLanes(3, "After chi", state, 400);
#endif

    iota(state, indexRound);
#ifdef KeccakReference
    displayStateAsLanes(3, "After iota", state, 400);
#endif
}

#define ROL16(a, offset) ((offset != 0) ? ((((tKeccakLane)a) << offset) ^ (((tKeccakLane)a) >> (sizeof(tKeccakLane)*8-offset))) : a)

static void theta(tKeccakLane *A)
{
    unsigned int x, y;
    tKeccakLane C[5], D[5];

    for(x=0; x<5; x++) {
        C[x] = 0;
        for(y=0; y<5; y++)
            C[x] ^= A[index(x, y)];
    }
    for(x=0; x<5; x++)
        D[x] = ROL16(C[(x+1)%5], 1) ^ C[(x+4)%5];
    for(x=0; x<5; x++)
        for(y=0; y<5; y++)
            A[index(x, y)] ^= D[x];
}

static void rho(tKeccakLane *A)
{
    unsigned int x, y;

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        A[index(x, y)] = ROL16(A[index(x, y)], KeccakRhoOffsets[index(x, y)]);
}

static void pi(tKeccakLane *A)
{
    unsigned int x, y;
    tKeccakLane tempA[25];

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        tempA[index(x, y)] = A[index(x, y)];
    for(x=0; x<5; x++) for(y=0; y<5; y++)
        A[index(0*x+1*y, 2*x+3*y)] = tempA[index(x, y)];
}

static void chi(tKeccakLane *A)
{
    unsigned int x, y;
    tKeccakLane C[5];

    for(y=0; y<5; y++) {
        for(x=0; x<5; x++)
            C[x] = A[index(x, y)] ^ ((~A[index(x+1, y)]) & A[index(x+2, y)]);
        for(x=0; x<5; x++)
            A[index(x, y)] = C[x];
    }
}

static void iota(tKeccakLane *A, unsigned int indexRound)
{
    A[index(0, 0)] ^= KeccakRoundConstants[indexRound];
}

/* ---------------------------------------------------------------- */

void KeccakP400_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    assert(offset < 50);
    assert(offset+length <= 50);
    memcpy(data, (unsigned char*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void KeccakP400_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    unsigned int i;

    assert(offset < 50);
    assert(offset+length <= 50);
    for(i=0; i<length; i++)
        output[i] = input[i] ^ ((unsigned char *)state)[offset+i];
}

/* ---------------------------------------------------------------- */

void KeccakP400_DisplayRoundConstants(FILE *f)
{
    unsigned int i;

    for(i=0; i<maxNrRounds; i++) {
        fprintf(f, "RC[%02i][0][0] = ", i);
        fprintf(f, "%04X", (unsigned int)(KeccakRoundConstants[i]));
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

void KeccakP400_DisplayRhoOffsets(FILE *f)
{
    unsigned int x, y;

    for(y=0; y<5; y++) for(x=0; x<5; x++) {
        fprintf(f, "RhoOffset[%i][%i] = ", x, y);
        fprintf(f, "%2i", KeccakRhoOffsets[index(x, y)]);
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}
