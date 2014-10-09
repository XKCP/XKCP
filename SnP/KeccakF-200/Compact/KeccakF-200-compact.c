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
#include "KeccakF-200-interface.h"

//#define DIVISION_INSTRUCTION    //comment if no division instruction or more compact when not using division
#define UNROLL_CHILOOP        //comment if more compact using for loop

typedef unsigned char UINT8;
typedef unsigned int tSmallUInt; /*INFO It could be more optimized to use "unsigned char" on an 8-bit CPU    */
typedef UINT8 tKeccakLane;

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(8)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(8))
#else
#define ALIGN
#endif

#define ROL8(a, offset) (UINT8)((((UINT8)a) << (offset&7)) ^ (((UINT8)a) >> (8-(offset&7))))

const UINT8 KeccakF_RotationConstants[25] =
{
     1,  3,  6, 10, 15, 21, 28, 36, 45, 55,  2, 14, 27, 41, 56,  8, 25, 43, 62, 18, 39, 61, 20, 44
};

const UINT8 KeccakF_PiLane[25] =
{
    10,  7, 11, 17, 18,  3,  5, 16,  8, 21, 24,  4, 15, 23, 19, 13, 12,  2, 20, 14, 22,  9,  6,  1
};

#if    defined(DIVISION_INSTRUCTION)
#define MOD5(argValue)    ((argValue) % 5)
#else
const UINT8 KeccakF_Mod5[10] =
{
    0, 1, 2, 3, 4, 0, 1, 2, 3, 4
};
#define MOD5(argValue)    KeccakF_Mod5[argValue]
#endif

const UINT8 KeccakF200_RoundConstants[] =
{
	0x01, 0x82, 0x8a, 0x00, 0x8b, 0x01, 0x81, 0x09, 0x8a, 0x88, 0x09, 0x0a, 0x8b, 0x8b, 0x89, 0x03, 0x02, 0x80
};

/* ---------------------------------------------------------------- */

void KeccakF200_Initialize( void )
{
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateInitialize(void *argState)
{
    memset( argState, 0, 25 * sizeof(tKeccakLane) );
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateXORBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
    tSmallUInt i;
    tKeccakLane * state = (tKeccakLane*)argState + offset;
    for(i=0; i<length; i++)
        state[i] ^= data[i];
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy((unsigned char*)state+offset, data, length);
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
{
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateComplementBit(void *state, unsigned int position)
{
    tKeccakLane lane = (tKeccakLane)1 << (position%8);
    ((tKeccakLane*)state)[position/8] ^= lane;
}

/* ---------------------------------------------------------------- */

void KeccakF200_StatePermute(void *argState)
{
    tSmallUInt x, y;
    tKeccakLane temp;
    tKeccakLane BC[5];
    tKeccakLane *state;
	const tKeccakLane *rc;

    state = (tKeccakLane*)argState;
    rc = KeccakF200_RoundConstants;
    do
    {
        // Theta
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

        // Rho Pi
        temp = state[1];
        for ( x = 0; x < 24; ++x )
        {
            BC[0] = state[KeccakF_PiLane[x]];
            state[KeccakF_PiLane[x]] = ROL8( temp, KeccakF_RotationConstants[x] );
            temp = BC[0];
        }

        //    Chi
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

        //    Iota
        temp = *(rc++);
        state[0] ^= temp;
    }
    while( temp != 0x80 );
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy(data, (UINT8*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void KeccakF200_StateExtractAndXORBytes(const void *argState, unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int i;
    tKeccakLane * state = (tKeccakLane*)argState + offset;
    for(i=0; i<length; i++)
        data[i] ^= state[i];
}

/* ---------------------------------------------------------------- */
