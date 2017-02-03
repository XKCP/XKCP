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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "brg_endian.h"
#ifdef KeccakReference
#include "displayIntermediateValues.h"
#endif

typedef unsigned char UINT8;
typedef unsigned int UINT32;
/* WARNING: on 8-bit and 16-bit platforms, this should be replaced by: */
/* typedef unsigned long       UINT32; */

#define maxNrRounds 24
#define nrLanes 25

#ifdef KeccakReference

static UINT32 KeccakRoundConstants[maxNrRounds][2];
static unsigned int KeccakRhoOffsets[nrLanes];

#endif

/* ---------------------------------------------------------------- */

void toBitInterleaving(UINT32 low, UINT32 high, UINT32 *even, UINT32 *odd);
void fromBitInterleaving(UINT32 even, UINT32 odd, UINT32 *low, UINT32 *high);

void toBitInterleaving(UINT32 low, UINT32 high, UINT32 *even, UINT32 *odd)
{
    unsigned int i;

    *even = 0;
    *odd = 0;
    for(i=0; i<64; i++) {
        unsigned int inBit;
        if (i < 32)
            inBit = (low >> i) & 1;
        else
            inBit = (high >> (i-32)) & 1;
        if ((i % 2) == 0)
            *even |= inBit << (i/2);
        else
            *odd |= inBit << ((i-1)/2);
    }
}

void fromBitInterleaving(UINT32 even, UINT32 odd, UINT32 *low, UINT32 *high)
{
    unsigned int i;

    *low = 0;
    *high = 0;
    for(i=0; i<64; i++) {
        unsigned int inBit;
        if ((i % 2) == 0)
            inBit = (even >> (i/2)) & 1;
        else
            inBit = (odd >> ((i-1)/2)) & 1;
        if (i < 32)
            *low |= inBit << i;
        else
            *high |= inBit << (i-32);
    }
}

#ifdef KeccakReference

/* ---------------------------------------------------------------- */

void KeccakP1600_InitializeRoundConstants(void);
void KeccakP1600_InitializeRhoOffsets(void);
static int LFSR86540(UINT8 *LFSR);

void KeccakP1600_StaticInitialize(void)
{
    KeccakP1600_InitializeRoundConstants();
    KeccakP1600_InitializeRhoOffsets();
}

void KeccakP1600_InitializeRoundConstants(void)
{
    UINT8 LFSRstate = 0x01;
    unsigned int i, j, bitPosition;
    UINT32 low, high;

    for(i=0; i<maxNrRounds; i++) {
        low = high = 0;
        for(j=0; j<7; j++) {
            bitPosition = (1<<j)-1; /* 2^j-1 */
            if (LFSR86540(&LFSRstate)) {
                if (bitPosition < 32)
                    low ^= (UINT32)1 << bitPosition;
                else
                    high ^= (UINT32)1 << (bitPosition-32);
            }
        }
        toBitInterleaving(low, high, &(KeccakRoundConstants[i][0]), &(KeccakRoundConstants[i][1]));
    }
}

void KeccakP1600_InitializeRhoOffsets(void)
{
    unsigned int x, y, t, newX, newY;

    KeccakRhoOffsets[0] = 0;
    x = 1;
    y = 0;
    for(t=0; t<24; t++) {
        KeccakRhoOffsets[5*y+x] = ((t+1)*(t+2)/2) % 64;
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

static const UINT32 KeccakRoundConstants[maxNrRounds][2] =
{
    0x00000001, 0x00000000,
    0x00000000, 0x00000089,
    0x00000000, 0x8000008B,
    0x00000000, 0x80008080,
    0x00000001, 0x0000008B,
    0x00000001, 0x00008000,
    0x00000001, 0x80008088,
    0x00000001, 0x80000082,
    0x00000000, 0x0000000B,
    0x00000000, 0x0000000A,
    0x00000001, 0x00008082,
    0x00000000, 0x00008003,
    0x00000001, 0x0000808B,
    0x00000001, 0x8000000B,
    0x00000001, 0x8000008A,
    0x00000001, 0x80000081,
    0x00000000, 0x80000081,
    0x00000000, 0x80000008,
    0x00000000, 0x00000083,
    0x00000000, 0x80008003,
    0x00000001, 0x80008088,
    0x00000000, 0x80000088,
    0x00000001, 0x00008000,
    0x00000000, 0x80008082
};

static const unsigned int KeccakRhoOffsets[nrLanes] =
{
     0,  1, 62, 28, 27, 36, 44,  6, 55, 20,  3, 10, 43, 25, 39, 41, 45, 15, 21,  8, 18,  2, 61, 56, 14
};

#endif

/* ---------------------------------------------------------------- */

void KeccakP1600_Initialize(void *state)
{
    memset(state, 0, 1600/8);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);

void KeccakP1600_AddByte(void *state, unsigned char byte, unsigned int offset)
{
    unsigned char data[1];

    assert(offset < 200);
    data[0] = byte;
    KeccakP1600_AddBytes(state, data, offset, 1);
}

/* ---------------------------------------------------------------- */

void KeccakP1600_AddBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
{
    if ((lanePosition < 25) && (offset < 8) && (offset+length <= 8)) {
        UINT8 laneAsBytes[8];
        UINT32 low, high;
        UINT32 lane[2];
        UINT32 *stateAsHalfLanes;

        memset(laneAsBytes, 0, 8);
        memcpy(laneAsBytes+offset, data, length);
        low = laneAsBytes[0]
            | ((UINT32)(laneAsBytes[1]) << 8)
            | ((UINT32)(laneAsBytes[2]) << 16)
            | ((UINT32)(laneAsBytes[3]) << 24);
        high = laneAsBytes[4]
            | ((UINT32)(laneAsBytes[5]) << 8)
            | ((UINT32)(laneAsBytes[6]) << 16)
            | ((UINT32)(laneAsBytes[7]) << 24);
        toBitInterleaving(low, high, lane, lane+1);
        stateAsHalfLanes = (UINT32*)state;
        stateAsHalfLanes[lanePosition*2+0] ^= lane[0];
        stateAsHalfLanes[lanePosition*2+1] ^= lane[1];
    }
}

void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int lanePosition = offset/8;
    unsigned int offsetInLane = offset%8;

    assert(offset < 200);
    assert(offset+length <= 200);
    while(length > 0) {
        unsigned int bytesInLane = 8 - offsetInLane;
        if (bytesInLane > length)
            bytesInLane = length;
        KeccakP1600_AddBytesInLane(state, lanePosition, data, offsetInLane, bytesInLane);
        length -= bytesInLane;
        lanePosition++;
        offsetInLane = 0;
        data += bytesInLane;
    }
}

/* ---------------------------------------------------------------- */

void KeccakP1600_ExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length);

void KeccakP1600_OverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
{
    if ((lanePosition < 25) && (offset < 8) && (offset+length <= 8)) {
        UINT8 laneAsBytes[8];
        UINT32 low, high;
        UINT32 lane[2];
        UINT32 *stateAsHalfLanes;

        KeccakP1600_ExtractBytesInLane(state, lanePosition, laneAsBytes, 0, 8);
        memcpy(laneAsBytes+offset, data, length);
        low = laneAsBytes[0]
            | ((UINT32)(laneAsBytes[1]) << 8)
            | ((UINT32)(laneAsBytes[2]) << 16)
            | ((UINT32)(laneAsBytes[3]) << 24);
        high = laneAsBytes[4]
            | ((UINT32)(laneAsBytes[5]) << 8)
            | ((UINT32)(laneAsBytes[6]) << 16)
            | ((UINT32)(laneAsBytes[7]) << 24);
        toBitInterleaving(low, high, lane, lane+1);
        stateAsHalfLanes = (UINT32*)state;
        stateAsHalfLanes[lanePosition*2+0] = lane[0];
        stateAsHalfLanes[lanePosition*2+1] = lane[1];
    }
}

void KeccakP1600_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int lanePosition = offset/8;
    unsigned int offsetInLane = offset%8;

    assert(offset < 200);
    assert(offset+length <= 200);
    while(length > 0) {
        unsigned int bytesInLane = 8 - offsetInLane;
        if (bytesInLane > length)
            bytesInLane = length;
        KeccakP1600_OverwriteBytesInLane(state, lanePosition, data, offsetInLane, bytesInLane);
        length -= bytesInLane;
        lanePosition++;
        offsetInLane = 0;
        data += bytesInLane;
    }
}

/* ---------------------------------------------------------------- */

void KeccakP1600_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    UINT8 laneAsBytes[8];
    unsigned int lanePosition = 0;

    assert(byteCount <= 200);
    memset(laneAsBytes, 0, 8);
    while(byteCount > 0) {
        if (byteCount < 8) {
            KeccakP1600_OverwriteBytesInLane(state, lanePosition, laneAsBytes, 0, byteCount);
            byteCount = 0;
        }
        else {
            UINT32 *stateAsHalfLanes = (UINT32*)state;
            stateAsHalfLanes[lanePosition*2+0] = 0;
            stateAsHalfLanes[lanePosition*2+1] = 0;
            byteCount -= 8;
            lanePosition++;
        }
    }
}

/* ---------------------------------------------------------------- */

void KeccakP1600_PermutationOnWords(UINT32 *state, unsigned int nrRounds);
static void theta(UINT32 *A);
static void rho(UINT32 *A);
static void pi(UINT32 *A);
static void chi(UINT32 *A);
static void iota(UINT32 *A, unsigned int indexRound);
void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);

void KeccakP1600_Permute_Nrounds(void *state, unsigned int nrounds)
{
    UINT32 *stateAsHalfLanes = (UINT32*)state;
    {
        UINT8 stateAsBytes[1600/8];
        KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600/8);
#ifdef KeccakReference
        displayStateAsBytes(1, "Input of permutation", stateAsBytes, 1600);
#endif
    }
    KeccakP1600_PermutationOnWords(stateAsHalfLanes, nrounds);
    {
        UINT8 stateAsBytes[1600/8];
        KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600/8);
#ifdef KeccakReference
        displayStateAsBytes(1, "State after permutation", stateAsBytes, 1600);
#endif
    }
}


void KeccakP1600_Permute_12rounds(void *state)
{
    UINT32 *stateAsHalfLanes = (UINT32*)state;
    {
        UINT8 stateAsBytes[1600/8];
        KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600/8);
#ifdef KeccakReference
        displayStateAsBytes(1, "Input of permutation", stateAsBytes, 1600);
#endif
    }
    KeccakP1600_PermutationOnWords(stateAsHalfLanes, 12);
    {
        UINT8 stateAsBytes[1600/8];
        KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600/8);
#ifdef KeccakReference
        displayStateAsBytes(1, "State after permutation", stateAsBytes, 1600);
#endif
    }
}

void KeccakP1600_Permute_24rounds(void *state)
{
    UINT32 *stateAsHalfLanes = (UINT32*)state;
    {
        UINT8 stateAsBytes[1600/8];
        KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600/8);
#ifdef KeccakReference
        displayStateAsBytes(1, "Input of permutation", stateAsBytes, 1600);
#endif
    }
    KeccakP1600_PermutationOnWords(stateAsHalfLanes, 24);
    {
        UINT8 stateAsBytes[1600/8];
        KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600/8);
#ifdef KeccakReference
        displayStateAsBytes(1, "State after permutation", stateAsBytes, 1600);
#endif
    }
}

void KeccakP1600_PermutationOnWords(UINT32 *state, unsigned int nrRounds)
{
    unsigned int i;

#ifdef KeccakReference
    displayStateAs32bitWords(3, "Same, with lanes as pairs of 32-bit words (bit interleaving)", state);
#endif

    for(i=(maxNrRounds-nrRounds); i<maxNrRounds; i++) {
#ifdef KeccakReference
        displayRoundNumber(3, i);
#endif

        theta(state);
#ifdef KeccakReference
        displayStateAs32bitWords(3, "After theta", state);
#endif

        rho(state);
#ifdef KeccakReference
        displayStateAs32bitWords(3, "After rho", state);
#endif

        pi(state);
#ifdef KeccakReference
        displayStateAs32bitWords(3, "After pi", state);
#endif

        chi(state);
#ifdef KeccakReference
        displayStateAs32bitWords(3, "After chi", state);
#endif

        iota(state, i);
#ifdef KeccakReference
        displayStateAs32bitWords(3, "After iota", state);
#endif
    }
}

#define index(x, y,z) ((((x)%5)+5*((y)%5))*2 + z)
#define ROL32(a, offset) ((offset != 0) ? ((((UINT32)a) << offset) ^ (((UINT32)a) >> (32-offset))) : a)

void ROL64(UINT32 inEven, UINT32 inOdd, UINT32 *outEven, UINT32 *outOdd, unsigned int offset)
{
    if ((offset % 2) == 0) {
        *outEven = ROL32(inEven, offset/2);
        *outOdd = ROL32(inOdd, offset/2);
    }
    else {
        *outEven = ROL32(inOdd, (offset+1)/2);
        *outOdd = ROL32(inEven, (offset-1)/2);
    }
}

static void theta(UINT32 *A)
{
    unsigned int x, y, z;
    UINT32 C[5][2], D[5][2];

    for(x=0; x<5; x++) {
        for(z=0; z<2; z++) {
            C[x][z] = 0;
            for(y=0; y<5; y++)
                C[x][z] ^= A[index(x, y, z)];
        }
    }
    for(x=0; x<5; x++) {
        ROL64(C[(x+1)%5][0], C[(x+1)%5][1], &(D[x][0]), &(D[x][1]), 1);
        for(z=0; z<2; z++)
            D[x][z] ^= C[(x+4)%5][z];
    }
    for(x=0; x<5; x++)
        for(y=0; y<5; y++)
            for(z=0; z<2; z++)
                A[index(x, y, z)] ^= D[x][z];
}

static void rho(UINT32 *A)
{
    unsigned int x, y;

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        ROL64(A[index(x, y, 0)], A[index(x, y, 1)], &(A[index(x, y, 0)]), &(A[index(x, y, 1)]), KeccakRhoOffsets[5*y+x]);
}

static void pi(UINT32 *A)
{
    unsigned int x, y, z;
    UINT32 tempA[50];

    for(x=0; x<5; x++) for(y=0; y<5; y++) for(z=0; z<2; z++)
        tempA[index(x, y, z)] = A[index(x, y, z)];
    for(x=0; x<5; x++) for(y=0; y<5; y++) for(z=0; z<2; z++)
        A[index(0*x+1*y, 2*x+3*y, z)] = tempA[index(x, y, z)];
}

static void chi(UINT32 *A)
{
    unsigned int x, y, z;
    UINT32 C[5][2];

    for(y=0; y<5; y++) {
        for(x=0; x<5; x++)
            for(z=0; z<2; z++)
                C[x][z] = A[index(x, y, z)] ^ ((~A[index(x+1, y, z)]) & A[index(x+2, y, z)]);
        for(x=0; x<5; x++)
            for(z=0; z<2; z++)
                A[index(x, y, z)] = C[x][z];
    }
}

static void iota(UINT32 *A, unsigned int indexRound)
{
    A[index(0, 0, 0)] ^= KeccakRoundConstants[indexRound][0];
    A[index(0, 0, 1)] ^= KeccakRoundConstants[indexRound][1];
}

/* ---------------------------------------------------------------- */

void KeccakP1600_ExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
{
    if ((lanePosition < 25) && (offset < 8) && (offset+length <= 8)) {
        UINT32 *stateAsHalfLanes = (UINT32*)state;
        UINT32 lane[2];
        UINT8 laneAsBytes[8];
        fromBitInterleaving(stateAsHalfLanes[lanePosition*2], stateAsHalfLanes[lanePosition*2+1], lane, lane+1);
        laneAsBytes[0] = lane[0] & 0xFF;
        laneAsBytes[1] = (lane[0] >> 8) & 0xFF;
        laneAsBytes[2] = (lane[0] >> 16) & 0xFF;
        laneAsBytes[3] = (lane[0] >> 24) & 0xFF;
        laneAsBytes[4] = lane[1] & 0xFF;
        laneAsBytes[5] = (lane[1] >> 8) & 0xFF;
        laneAsBytes[6] = (lane[1] >> 16) & 0xFF;
        laneAsBytes[7] = (lane[1] >> 24) & 0xFF;
        memcpy(data, laneAsBytes+offset, length);
    }
}

void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int lanePosition = offset/8;
    unsigned int offsetInLane = offset%8;

    assert(offset < 200);
    assert(offset+length <= 200);
    while(length > 0) {
        unsigned int bytesInLane = 8 - offsetInLane;
        if (bytesInLane > length)
            bytesInLane = length;
        KeccakP1600_ExtractBytesInLane(state, lanePosition, data, offsetInLane, bytesInLane);
        length -= bytesInLane;
        lanePosition++;
        offsetInLane = 0;
        data += bytesInLane;
    }
}

/* ---------------------------------------------------------------- */

void KeccakP1600_ExtractAndAddBytesInLane(const void *state, unsigned int lanePosition, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    if ((lanePosition < 25) && (offset < 8) && (offset+length <= 8)) {
        UINT8 laneAsBytes[8];
        unsigned int i;

        KeccakP1600_ExtractBytesInLane(state, lanePosition, laneAsBytes, offset, length);
        for(i=0; i<length; i++)
            output[i] = input[i] ^ laneAsBytes[i];
    }
}

void KeccakP1600_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    unsigned int lanePosition = offset/8;
    unsigned int offsetInLane = offset%8;

    assert(offset < 200);
    assert(offset+length <= 200);
    while(length > 0) {
        unsigned int bytesInLane = 8 - offsetInLane;
        if (bytesInLane > length)
            bytesInLane = length;
        KeccakP1600_ExtractAndAddBytesInLane(state, lanePosition, input, output, offsetInLane, bytesInLane);
        length -= bytesInLane;
        lanePosition++;
        offsetInLane = 0;
        input += bytesInLane;
        output += bytesInLane;
    }
}

/* ---------------------------------------------------------------- */

void KeccakP1600_DisplayRoundConstants(FILE *f)
{
    unsigned int i;

    for(i=0; i<maxNrRounds; i++) {
        fprintf(f, "RC[%02i][0][0] = ", i);
        fprintf(f, "%08X:%08X", (unsigned int)(KeccakRoundConstants[i][0]), (unsigned int)(KeccakRoundConstants[i][1]));
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}

void KeccakP1600_DisplayRhoOffsets(FILE *f)
{
    unsigned int x, y;

    for(y=0; y<5; y++) for(x=0; x<5; x++) {
        fprintf(f, "RhoOffset[%i][%i] = ", x, y);
        fprintf(f, "%2i", KeccakRhoOffsets[5*y+x]);
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
}
