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
#include "KeccakF-800-interface.h"

#define USE_MEMSET
//#define DIVISION_INSTRUCTION    //comment if no division instruction or more compact when not using division
#define UNROLL_CHILOOP        //comment more compact using for loop


typedef unsigned char UINT8;
typedef uint32_t UINT32;
typedef unsigned int tSmallUInt; /*INFO It could be more optimized to use "unsigned char" on an 8-bit CPU    */
typedef UINT32 tKeccakLane;

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(32))
#else
#define ALIGN
#endif

#if defined (__arm__)
#define ROL32(a, offset) __ror(a, 32-(offset))
#elif defined(_MSC_VER)
#define ROL32(a, offset) _rotl(a, offset)
#else
#define ROL32(a, offset) ((((UINT32)a) << offset) ^ (((UINT32)a) >> (32-offset)))
#endif

#define    cKeccakNumberOfRounds    22

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

/* ---------------------------------------------------------------- */

void KeccakF800_Initialize( void )
{
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateInitialize(void *argState)
{
    #if defined(USE_MEMSET)
    memset( argState, 0, 25 * sizeof(tKeccakLane) );
    #else
    tSmallUInt i;
    tKeccakLane *state;

    state = argState;
    i = 25;
    do
    {
        *(state++) = 0;
    }
    while ( --i != 0 );
    #endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateXORBytesInLane(void *argState, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
{
    unsigned int i;
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    unsigned char * state = (unsigned char*)argState + lanePosition * sizeof(tKeccakLane) + offset;
    for(i=0; i<length; i++)
        ((unsigned char *)state)[i] ^= data[i];
#else
    tKeccakLane lane = 0;
    for(i=0; i<length; i++)
        lane |= ((tKeccakLane)data[i]) << ((i+offset)*8);
    ((tKeccakLane*)state)[lanePosition] ^= lane;
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    tSmallUInt i;
    laneCount *= sizeof(tKeccakLane);
    for( i = 0; i < laneCount; ++i) {
        ((unsigned char*)state)[i] ^= data[i];
    }
#else
    tSmallUInt i;
    UINT8 *curData = data;
    for(i=0; i<laneCount; i++, curData+=KeccakF_laneInBytes) {
        tKeccakLane lane = (tKeccakLane)curData[0]
            | ((tKeccakLane)curData[1] << 8)
            | ((tKeccakLane)curData[2] << 16)
            | ((tKeccakLane)curData[3] << 24);
        ((tKeccakLane*)state)[i] ^= lane;
    }
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateOverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    memcpy((unsigned char*)state+lanePosition*KeccakF_laneInBytes+offset, data, length);
#else
	#error todo
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    memcpy(state, data, laneCount*KeccakF_laneInBytes);
#else
    tSmallUInt i;
    UINT8 *curData = data;
    for(i=0; i<laneCount; i++, curData+=KeccakF_laneInBytes) {
        tKeccakLane lane = (tKeccakLane)curData[0]
            | ((tKeccakLane)curData[1] << 8)
            | ((tKeccakLane)curData[2] << 16)
            | ((tKeccakLane)curData[3] << 24);
        ((tKeccakLane*)state)[i] = lane;
    }
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    memset(state, 0, byteCount);
#else
	#error	todo
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateComplementBit(void *state, unsigned int position)
{
    tKeccakLane lane = (tKeccakLane)1 << (position%(sizeof(tKeccakLane)*8));
    ((tKeccakLane*)state)[position/(sizeof(tKeccakLane)*8)] ^= lane;
}

/* ---------------------------------------------------------------- */

void KeccakP800_12_StatePermute(void *argState)
{
    tSmallUInt x, y;
	tSmallUInt nr;
    tKeccakLane temp;
    tKeccakLane BC[5];
    tKeccakLane *state;
    UINT8       LFSRstate;

    state = argState;
    LFSRstate = 0x01;
	nr = 12;
	for ( y = (tSmallUInt)(cKeccakNumberOfRounds - 12); y != 0; --y )
	{
        for( x = 1; x < 128; x <<= 1 ) 
        {
            if ((LFSRstate & 0x80) != 0)
                // Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1
                LFSRstate = (LFSRstate << 1) ^ 0x71;
            else
                LFSRstate <<= 1;
        }
	}

    do
    {
        // Theta
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

        // Rho Pi
        temp = state[1];
        for ( x = 0; x < 24; ++x )
        {
            BC[0] = state[KeccakF_PiLane[x]];
            state[KeccakF_PiLane[x]] = ROL32( temp, KeccakF_RotationConstants[x] );
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
        temp = 0;
        for( x = 1; x < 128; x <<= 1 ) 
        {
            if ( x <= (sizeof(tKeccakLane)*8) )
                temp ^= (tKeccakLane)(LFSRstate & 1) << (x - 1);

            if ((LFSRstate & 0x80) != 0)
                // Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1
                LFSRstate = (LFSRstate << 1) ^ 0x71;
            else
                LFSRstate <<= 1;
        }
        state[0] ^= temp;
    }
    while( --nr != 0 );
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    memcpy(data, ((UINT8*)&((tKeccakLane*)state)[lanePosition])+offset, length);
#else
    tSmallUInt i;
    tKeccakLane lane = ((tKeccakLane*)state)[lanePosition];
    lane >>= offset*8;
    for(i=0; i<length; i++) {
        data[i] = lane & 0xFF;
        lane >>= 8;
    }
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    memcpy(data, state, laneCount*sizeof(tKeccakLane));
#else
    tSmallUInt i, j;
    for(i=0; i<laneCount; i++)
    {
        for(j=0; j < KeccakF_laneInBytes; j++)
        {
            bytes[data+(i*KeccakF_laneInBytes] = state[i] >> (8*j)) & 0xFF;
        }
    }
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    unsigned int i;

    for(i=0; i<length; i++)
        data[i] ^= ((unsigned char *)state)[lanePosition*KeccakF_laneInBytes+offset+i];
#else
	#error	todo
#endif
}

/* ---------------------------------------------------------------- */

void KeccakF800_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
{
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    unsigned int i;
    for(i=0; i<laneCount*KeccakF_laneInBytes; i++)
        data[i] ^= ((unsigned char *)state)[i];
#else
	#error	todo
#endif
}

/* ---------------------------------------------------------------- */
