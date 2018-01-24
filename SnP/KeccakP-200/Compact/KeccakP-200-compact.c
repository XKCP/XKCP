/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Keccak-p[200] in a SnP-compatible way.
Please refer to SnP-documentation.h for more details.

This implementation comes with KeccakP-200-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include <string.h>
#include <stdlib.h>
#include "KeccakP-200-SnP.h"

/* #define DIVISION_INSTRUCTION    /* comment if no division instruction or more compact when not using division */
#define UNROLL_CHILOOP        /* comment if more compact using for loop */

typedef unsigned char UINT8;
typedef unsigned int tSmallUInt; /*INFO It could be more optimized to use "unsigned char" on an 8-bit CPU    */
typedef UINT8 tKeccakLane;

#define ROL8(a, offset) (UINT8)((((UINT8)a) << (offset&7)) ^ (((UINT8)a) >> (8-(offset&7))))

const UINT8 KeccakP200_RotationConstants[25] =
{
     1,  3,  6, 10, 15, 21, 28, 36, 45, 55,  2, 14, 27, 41, 56,  8, 25, 43, 62, 18, 39, 61, 20, 44
};

const UINT8 KeccakP200_PiLane[25] =
{
    10,  7, 11, 17, 18,  3,  5, 16,  8, 21, 24,  4, 15, 23, 19, 13, 12,  2, 20, 14, 22,  9,  6,  1
};

#if    defined(DIVISION_INSTRUCTION)
#define MOD5(argValue)    ((argValue) % 5)
#else
const UINT8 KeccakP200_Mod5[10] =
{
    0, 1, 2, 3, 4, 0, 1, 2, 3, 4
};
#define MOD5(argValue)    KeccakP200_Mod5[argValue]
#endif

const UINT8 KeccakF200_RoundConstants[] =
{
    0x01, 0x82, 0x8a, 0x00, 0x8b, 0x01, 0x81, 0x09, 0x8a, 0x88, 0x09, 0x0a, 0x8b, 0x8b, 0x89, 0x03, 0x02, 0x80
};

/* ---------------------------------------------------------------- */

void KeccakP200_Initialize(void *argState)
{
    memset( argState, 0, 25 * sizeof(tKeccakLane) );
}

/* ---------------------------------------------------------------- */

void KeccakP200_AddByte(void *argState, unsigned char byte, unsigned int offset)
{
    ((tKeccakLane*)argState)[offset] ^= byte;
}

/* ---------------------------------------------------------------- */

void KeccakP200_AddBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
    tSmallUInt i;
    tKeccakLane * state = (tKeccakLane*)argState + offset;
    for(i=0; i<length; i++)
        state[i] ^= data[i];
}

/* ---------------------------------------------------------------- */

void KeccakP200_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy((unsigned char*)state+offset, data, length);
}

/* ---------------------------------------------------------------- */

void KeccakP200_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

void KeccakP200_Permute_Nrounds(void *argState, unsigned int nr)
{
    tSmallUInt x, y;
    tKeccakLane temp;
    tKeccakLane BC[5];
    tKeccakLane *state;
    const tKeccakLane *rc;

    state = (tKeccakLane*)argState;
    rc = KeccakF200_RoundConstants + 18 - nr;
    do
    {
        /* Theta */
        for ( x = 0; x < 5; ++x )
        {
            BC[x] = state[x] ^ state[5 + x] ^ state[10 + x] ^ state[15 + x] ^ state[20 + x];
        }
        for ( x = 0; x < 5; ++x )
        {
            temp = BC[MOD5(x+4)] ^ ROL8(BC[MOD5(x+1)], 1);
            for ( y = 0; y < 25; y += 5 )
            {
                state[y + x] ^= temp;
            }
        }

        /* Rho Pi */
        temp = state[1];
        for ( x = 0; x < 24; ++x )
        {
            BC[0] = state[KeccakP200_PiLane[x]];
            state[KeccakP200_PiLane[x]] = ROL8( temp, KeccakP200_RotationConstants[x] );
            temp = BC[0];
        }

        /*    Chi */
        for ( y = 0; y < 25; y += 5 )
        {
#if defined(UNROLL_CHILOOP)
            BC[0] = state[y + 0];
            BC[1] = state[y + 1];
            BC[2] = state[y + 2];
            BC[3] = state[y + 3];
            BC[4] = state[y + 4];
#else
            for ( x = 0; x < 5; ++x )
            {
                BC[x] = state[y + x];
            }
#endif
            for ( x = 0; x < 5; ++x )
            {
                state[y + x] = BC[x] ^((~BC[MOD5(x+1)]) & BC[MOD5(x+2)]);
            }
        }

        /*    Iota */
        temp = *(rc++);
        state[0] ^= temp;
    }
    while( temp != 0x80 );
}

/* ---------------------------------------------------------------- */

void KeccakP200_Permute_18rounds(void *argState)
{
    KeccakP200_Permute_Nrounds(argState, 18);
}

/* ---------------------------------------------------------------- */

void KeccakP200_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy(data, (UINT8*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void KeccakP200_ExtractAndAddBytes(const void *argState, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    unsigned int i;
    tKeccakLane * state = (tKeccakLane*)argState + offset;
    for(i=0; i<length; i++)
        output[i] = input[i] ^ state[i];
}

/* ---------------------------------------------------------------- */
