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

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "brg_endian.h"
#include "KeccakP-800-SnP.h"

#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
#error Not yet implemented
#endif

#define USE_MEMSET
/* #define DIVISION_INSTRUCTION    /* comment if no division instruction or more compact when not using division */
#define UNROLL_CHILOOP        /* comment more compact using for loop */

typedef unsigned char UINT8;
typedef uint32_t UINT32;
typedef unsigned int tSmallUInt; /*INFO It could be more optimized to use "unsigned char" on an 8-bit CPU    */
typedef UINT32 tKeccakLane;

#if defined (__arm__) && !defined(__GNUC__)
#define ROL32(a, offset) __ror(a, 32-(offset))
#elif defined(_MSC_VER)
#define ROL32(a, offset) _rotl(a, offset)
#else
#define ROL32(a, offset) ((((UINT32)a) << offset) ^ (((UINT32)a) >> (32-offset)))
#endif

#define    cKeccakNumberOfRounds    22

const UINT8 KeccakP800_RotationConstants[25] =
{
     1,  3,  6, 10, 15, 21, 28, 4, 13, 23,  2, 14, 27, 9, 24,  8, 25, 11, 30, 18, 7, 29, 20, 12
};

const UINT8 KeccakP800_PiLane[25] =
{
    10,  7, 11, 17, 18,  3,  5, 16,  8, 21, 24,  4, 15, 23, 19, 13, 12,  2, 20, 14, 22,  9,  6,  1
};

#if    defined(DIVISION_INSTRUCTION)
#define MOD5(argValue)    ((argValue) % 5)
#else
const UINT8 KeccakP800_Mod5[10] =
{
    0, 1, 2, 3, 4, 0, 1, 2, 3, 4
};
#define MOD5(argValue)    KeccakP800_Mod5[argValue]
#endif

/* ---------------------------------------------------------------- */

void KeccakP800_Initialize(void *argState)
{
    #if defined(USE_MEMSET)
    memset( argState, 0, 25 * sizeof(tKeccakLane) );
    #else
    tSmallUInt i;
    tKeccakLane *state;

    state = (tKeccakLane*)argState;
    i = 25;
    do
    {
        *(state++) = 0;
    }
    while ( --i != 0 );
    #endif
}

/* ---------------------------------------------------------------- */

void KeccakP800_AddByte(void *state, unsigned char data, unsigned int offset)
{
    ((unsigned char *)state)[offset] ^= data;
}

/* ---------------------------------------------------------------- */

void KeccakP800_AddBytes(void *argState, const unsigned char *data, unsigned int offset, unsigned int length)
{
    tSmallUInt i;
    unsigned char * state = (unsigned char*)argState + offset;

    for(i=0; i<length; i++)
        ((unsigned char *)state)[i] ^= data[i];
}

/* ---------------------------------------------------------------- */

void KeccakP800_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy((unsigned char*)state+offset, data, length);
}

/* ---------------------------------------------------------------- */

void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount)
{
    memset(state, 0, byteCount);
}

/* ---------------------------------------------------------------- */

void KeccakP800_Permute_Nrounds(void *argState, unsigned int nr)
{
    tSmallUInt x, y;
    tKeccakLane temp;
    tKeccakLane BC[5];
    tKeccakLane *state;
    UINT8       LFSRstate;

    state = (tKeccakLane*)argState;
    LFSRstate = 0x01;
    for ( y = (tSmallUInt)(cKeccakNumberOfRounds - nr); y != 0; --y )
    {
        for( x = 1; x < 128; x <<= 1 )
        {
            if ((LFSRstate & 0x80) != 0)
                /* Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1 */
                LFSRstate = (LFSRstate << 1) ^ 0x71;
            else
                LFSRstate <<= 1;
        }
    }

    do
    {
        /* Theta */
        for ( x = 0; x < 5; ++x )
        {
            BC[x] = state[x] ^ state[5 + x] ^ state[10 + x] ^ state[15 + x] ^ state[20 + x];
        }
        for ( x = 0; x < 5; ++x )
        {
            temp = BC[MOD5(x+4)] ^ ROL32(BC[MOD5(x+1)], 1);
            for ( y = 0; y < 25; y += 5 )
            {
                state[y + x] ^= temp;
            }
        }

        /* Rho Pi */
        temp = state[1];
        for ( x = 0; x < 24; ++x )
        {
            BC[0] = state[KeccakP800_PiLane[x]];
            state[KeccakP800_PiLane[x]] = ROL32( temp, KeccakP800_RotationConstants[x] );
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
        temp = 0;
        for( x = 1; x < 128; x <<= 1 )
        {
            if ( x <= (sizeof(tKeccakLane)*8) )
                temp ^= (tKeccakLane)(LFSRstate & 1) << (x - 1);

            if ((LFSRstate & 0x80) != 0)
                /* Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1 */
                LFSRstate = (LFSRstate << 1) ^ 0x71;
            else
                LFSRstate <<= 1;
        }
        state[0] ^= temp;
    }
    while( --nr != 0 );
}

/* ---------------------------------------------------------------- */

void KeccakP800_Permute_12rounds(void *argState)
{
    KeccakP800_Permute_Nrounds(argState, 12);
}

/* ---------------------------------------------------------------- */

void KeccakP800_Permute_22rounds(void *argState)
{
    KeccakP800_Permute_Nrounds(argState, 22);
}

/* ---------------------------------------------------------------- */

void KeccakP800_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
    memcpy(data, (const UINT8*)state+offset, length);
}

/* ---------------------------------------------------------------- */

void KeccakP800_ExtractAndAddBytes(const void *argState, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    tSmallUInt i;
    const unsigned char * state = (const unsigned char*)argState + offset;

    for(i=0; i<length; i++)
        output[i] = input[i] ^ state[i];
}

/* ---------------------------------------------------------------- */
