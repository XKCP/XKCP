/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef KeccakP1600_excluded

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "brg_endian.h"
#include "Kravatte.h"
#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"
    #include "KeccakP-1600-times4-SnP.h"
    #include "KeccakP-1600-times8-SnP.h"
#endif

/* 
** Uncomment this define if calls to KeccakP1600_Initialize() and 
** KeccakP1600times##Parallellism##_InitializeAll() are mandatory to make it work,
** mostly not needed.
**
#define    NEED_INITIALIZE
*/

/* 
** Uncomment this define for more debugging dumps.
**
#define    DEBUG_DUMP 
*/

#define MaxParallellism 8
#define laneSize        8
#define widthInLanes    (SnP_widthInBytes/laneSize)
#define SnP_width       (SnP_widthInBytes*8)

#define MyMin(a, b)     (((a) < (b)) ? (a) : (b))

#if defined(_MSC_VER)
#define ROL64(a, offset) _rotl64(a, offset)
#elif defined(KeccakP1600_useSHLD)
    #define ROL64(x,N) ({ \
    register uint64_t __out; \
    register uint64_t __in = x; \
    __asm__ ("shld %2,%0,%0" : "=r"(__out) : "0"(__in), "i"(N)); \
    __out; \
    })
#else
#define ROL64(a, offset) ((((uint64_t)a) << (offset)) | (((uint64_t)a) >> (64-(offset))))
#endif

#if defined(NEED_INITIALIZE)
#define mInitialize(argState)                   KeccakP1600_Initialize(argState)
#define mInitializePl(argStates, Parallellism)  KeccakP1600times##Parallellism##_InitializeAll(argStates)
#else
#define mInitialize(argState)
#define mInitializePl(argStates, Parallellism)
#endif

#if defined(DEBUG_DUMP)
static void DUMP( const unsigned char * pText, const unsigned char * pData, unsigned int size )
{
    unsigned int i;
    printf("%s (%u bytes):", pText, size);
    for(i=0; i<size; i++)
        printf(" %02x", (int)pData[i]);
    printf("\n");
}

static void DUMP64( const unsigned char * pText, const unsigned char * pData, unsigned int size )
{
    unsigned int i;
    size /=8;
    printf("%s (%u lanes):", pText, size);
    for(i=0; i<size; i++)
        printf(" %016lx", ((uint64_t*)pData)[i]);
    printf("\n");
}
#else
#define DUMP(pText, pData, size )
#define DUMP64(pText, pData, size )
#endif

#define ParallelCompressLoopFast( Parallellism ) \
    if ( messageByteLen >= Parallellism * SnP_widthInBytes ) { \
        size_t processed = KeccakP1600times##Parallellism##_KravatteCompress((uint64_t*)x, (uint64_t*)k, message, messageByteLen); \
        message += processed; \
        messageByteLen -= processed; \
    }

#define ParallelExpandLoopFast( Parallellism ) \
    if ( outputByteLen >= Parallellism * SnP_widthInBytes ) { \
        size_t processed = KeccakP1600times##Parallellism##_KravatteExpand((uint64_t*)kv->yAccu.a, (uint64_t*)kv->kRoll.a, output, outputByteLen); \
        output += processed; \
        outputByteLen -= processed; \
    }

#define ParallelCompressLoopPlSnP( Parallellism ) \
    if ( messageByteLen >= Parallellism * SnP_widthInBytes ) { \
        ALIGN(KeccakP1600times##Parallellism##_statesAlignment) unsigned char states[KeccakP1600times##Parallellism##_statesSizeInBytes]; \
        unsigned int i; \
        \
        KeccakP1600times##Parallellism##_StaticInitialize(); \
        mInitializePl(states, Parallellism); \
        do { \
            Kravatte_Rollc( (uint64_t*)k, encbuf, Parallellism ); \
            KeccakP1600times##Parallellism##_OverwriteLanesAll(states, k, Kravatte_RollcOffset/8, 0); \
            i = 0; \
            do { \
                KeccakP1600times##Parallellism##_OverwriteBytes(states, i, encbuf + i * Kravatte_RollcSizeInBytes, Kravatte_RollcOffset, Kravatte_RollcSizeInBytes); \
            } while ( ++i < Parallellism ); \
            KeccakP1600times##Parallellism##_AddLanesAll(states, message, widthInLanes, widthInLanes); \
            DUMP("msg pn", message, Parallellism * SnP_widthInBytes); \
            KeccakP1600times##Parallellism##_PermuteAll_6rounds(states); \
            i = 0; \
            do { \
                KeccakP1600times##Parallellism##_ExtractAndAddBytes(states, i, x, x, 0, SnP_widthInBytes); \
                DUMP("xAc pn", x, SnP_widthInBytes); \
            } while ( ++i < Parallellism ); \
            message += Parallellism * SnP_widthInBytes; \
            messageByteLen -= Parallellism * SnP_widthInBytes; \
        } while ( messageByteLen >= Parallellism * SnP_widthInBytes ); \
    }

#define ParallelExpandLoopPlSnP( Parallellism ) \
    if ( outputByteLen >= Parallellism * SnP_widthInBytes ) { \
        ALIGN(KeccakP1600times##Parallellism##_statesAlignment) unsigned char states[KeccakP1600times##Parallellism##_statesSizeInBytes]; \
        unsigned int i; \
        \
        KeccakP1600times##Parallellism##_StaticInitialize(); \
        mInitializePl(states, Parallellism); \
        do { \
            Kravatte_Rolle( (uint64_t*)kv->yAccu.a, encbuf, Parallellism ); \
            KeccakP1600times##Parallellism##_OverwriteLanesAll(states, kv->yAccu.a, Kravatte_RolleOffset/8, 0); \
            i = 0; \
            do { \
                KeccakP1600times##Parallellism##_OverwriteBytes(states, i, encbuf + i * Kravatte_RolleSizeInBytes, Kravatte_RolleOffset, Kravatte_RolleSizeInBytes); \
            } while ( ++i < Parallellism ); \
            KeccakP1600times##Parallellism##_PermuteAll_6rounds(states); \
            i = 0; \
            do { \
                KeccakP1600times##Parallellism##_ExtractAndAddBytes(states, i, kv->kRoll.a, output, 0, SnP_widthInBytes); \
                DUMP("out n", output, SnP_widthInBytes); \
                output += SnP_widthInBytes; \
            } while ( ++i < Parallellism ); \
            outputByteLen -= Parallellism * SnP_widthInBytes; \
        } while ( outputByteLen >= Parallellism * SnP_widthInBytes ); \
    }

static void Kravatte_Rollc( uint64_t *x, unsigned char *encbuf, unsigned int parallellism )
{
    uint64_t    x0 = x[20];
    uint64_t    x1 = x[21];
    uint64_t    x2 = x[22];
    uint64_t    x3 = x[23];
    uint64_t    x4 = x[24];
    uint64_t    t;
    #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    uint64_t    *pEnc = (uint64_t*)encbuf;
    #endif

    do {
        #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        *(pEnc++) = x0;
        *(pEnc++) = x1;
        *(pEnc++) = x2;
        *(pEnc++) = x3;
        *(pEnc++) = x4;
        DUMP("Rollc", pEnc - Kravatte_RollcSizeInBytes/8, Kravatte_RollcSizeInBytes);
        #else
        #error todo
        #endif

        t  = x0;
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = ROL64(t, 7) ^ x0 ^ (x0 >> 3);
    } while(--parallellism != 0); 

    x[20] = x0;
    x[21] = x1;
    x[22] = x2;
    x[23] = x3;
    x[24] = x4;
    DUMP("Rollc state", pEnc - Kravatte_RollcSizeInBytes/8, Kravatte_RollcSizeInBytes);

}

static void Kravatte_Rolle( uint64_t *x, unsigned char *encbuf, unsigned int parallellism )
{
    uint64_t    x0 = x[15];
    uint64_t    x1 = x[16];
    uint64_t    x2 = x[17];
    uint64_t    x3 = x[18];
    uint64_t    x4 = x[19];
    uint64_t    x5 = x[20];
    uint64_t    x6 = x[21];
    uint64_t    x7 = x[22];
    uint64_t    x8 = x[23];
    uint64_t    x9 = x[24];
    uint64_t    t;
    #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    uint64_t    *pEnc = (uint64_t*)encbuf;
    #endif

    do {
        #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        *(pEnc++) = x0;
        *(pEnc++) = x1;
        *(pEnc++) = x2;
        *(pEnc++) = x3;
        *(pEnc++) = x4;
        *(pEnc++) = x5;
        *(pEnc++) = x6;
        *(pEnc++) = x7;
        *(pEnc++) = x8;
        *(pEnc++) = x9;
        DUMP("Rolle", pEnc - Kravatte_RolleSizeInBytes/8, Kravatte_RolleSizeInBytes);
        #else
        #error todo
        #endif

        t  = x0;
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = x5;
        x5 = x6;
        x6 = x7;
        x7 = x8;
        x8 = x9;
        x9 = ROL64(t, 7) ^ ROL64(x0, 18) ^ (x1 & (x0 >> 1));
    } while(--parallellism != 0); 

    x[15] = x0;
    x[16] = x1;
    x[17] = x2;
    x[18] = x3;
    x[19] = x4;
    x[20] = x5;
    x[21] = x6;
    x[22] = x7;
    x[23] = x8;
    x[24] = x9;
    DUMP("Rolle state", pEnc - Kravatte_RolleSizeInBytes/8, Kravatte_RolleSizeInBytes);

}

static const unsigned char * Kra_Compress( unsigned char *k, unsigned char *x, const BitSequence *message, BitLength *messageBitLen, int lastFlag )
{
    unsigned char encbuf[MaxParallellism*Kravatte_RollcSizeInBytes];
    size_t messageByteLen = *messageBitLen / 8; /* do not include partial last byte */

    #if defined(KeccakP1600times8_implementation) && !defined(KeccakP1600times8_isFallback)
    #if defined(KeccakF1600times8_FastKravatte_supported)
    ParallelCompressLoopFast( 8 )
    #else
    ParallelCompressLoopPlSnP( 8 )
    #endif
    #endif
    #if defined(KeccakP1600times4_implementation) && !defined(KeccakP1600times4_isFallback)
    #if defined(KeccakF1600times4_FastKravatte_supported)
    ParallelCompressLoopFast( 4 )
    #else
    ParallelCompressLoopPlSnP( 4 )
    #endif
    #endif
    #if defined(KeccakP1600times2_implementation) && !defined(KeccakP1600times2_isFallback)
    #if defined(KeccakF1600times2_FastKravatte_supported)
    ParallelCompressLoopFast( 2 )
    #else
    ParallelCompressLoopPlSnP( 2 )
    #endif
    #endif

    if (messageByteLen >= SnP_widthInBytes) {
        ALIGN(KeccakP1600_stateAlignment) unsigned char state[KeccakP1600_stateSizeInBytes];

        KeccakP1600_StaticInitialize();
        mInitialize(state);
        do {
            KeccakP1600_OverwriteBytes(state, k, 0, SnP_widthInBytes);
            Kravatte_Rollc((uint64_t*)k, encbuf, 1);
            KeccakP1600_AddBytes(state, message, 0, SnP_widthInBytes);
            DUMP("msg p1", message, SnP_widthInBytes);
            KeccakP1600_Permute_Nrounds(state, 6);
            KeccakP1600_ExtractAndAddBytes(state, x, x, 0, SnP_widthInBytes);
            DUMP("xAc p1", x, SnP_widthInBytes);
            message += SnP_widthInBytes;
            messageByteLen -= SnP_widthInBytes;
        } while ( messageByteLen >= SnP_widthInBytes );
    }
    *messageBitLen %= SnP_width;
    if ( lastFlag != 0 ) {
        ALIGN(KeccakP1600_stateAlignment) unsigned char state[KeccakP1600_stateSizeInBytes];

        assert(messageByteLen < SnP_widthInBytes);
        KeccakP1600_StaticInitialize();
        mInitialize(state);
        KeccakP1600_OverwriteBytes(state, k, 0, SnP_widthInBytes); /* write k */
        Kravatte_Rollc((uint64_t*)k, encbuf, 1);
        KeccakP1600_AddBytes(state, message, 0, messageByteLen); /* add message */
        DUMP("msg pL", state, SnP_widthInBytes);
        message += messageByteLen;
        *messageBitLen %= 8;
        if (*messageBitLen != 0) /* padding */
            KeccakP1600_AddByte(state, *message++ | (1 << *messageBitLen), messageByteLen);
        else
            KeccakP1600_AddByte(state, 1, messageByteLen);
        KeccakP1600_Permute_Nrounds(state, 6);
        KeccakP1600_ExtractAndAddBytes(state, x, x, 0, SnP_widthInBytes);
        DUMP("xAc pL", x, SnP_widthInBytes);
        Kravatte_Rollc((uint64_t*)k, encbuf, 1);
        *messageBitLen = 0;
    }
    return message;
}

int Kravatte_MaskDerivation(Kravatte_Instance *kv, const BitSequence *Key, BitLength KeyBitLen)
{
    ALIGN(KeccakP1600_stateAlignment) unsigned char state[KeccakP1600_stateSizeInBytes];
    BitSequence lastByte;
    unsigned int numberOfBits;

    /* Check max K length (b-1) */
    if (KeyBitLen >= SnP_width)
        return 1;
    /* Compute k from K */
    memset(kv->k.a, 0, SnP_widthInBytes);
    memcpy(kv->k.a, Key, KeyBitLen/8);
    numberOfBits = KeyBitLen & 7;
    if ((numberOfBits) != 0) {
        lastByte = (Key[KeyBitLen/8] & ((1 << numberOfBits) - 1)) | (1 << numberOfBits);
    }
    else {
        lastByte = 1;
    }
    kv->k.a[KeyBitLen/8] = lastByte;
    KeccakP1600_StaticInitialize();
    mInitialize(state);
    KeccakP1600_OverwriteBytes(state, kv->k.a, 0, SnP_widthInBytes);
    KeccakP1600_Permute_Nrounds(state, 6);
    KeccakP1600_ExtractBytes(state, kv->k.a, 0, SnP_widthInBytes);
    memcpy( kv->kRoll.a, kv->k.a, SnP_widthInBytes );
    memset( kv->xAccu.a, 0, SnP_widthInBytes );
    kv->phase = COMPRESSING;
    kv->queueOffset = 0;

    return 0;
}

int Kra(Kravatte_Instance *kv, const BitSequence *input, BitLength inputBitLen, int flags)
{
    int finalFlag = flags & KRAVATTE_FLAG_LAST_PART;

    if ((finalFlag == 0) && ((inputBitLen & 7) != 0))
        return 1;
    if ( (flags & KRAVATTE_FLAG_INIT) != 0 ) {
        memcpy(kv->kRoll.a, kv->k.a, SnP_widthInBytes);
        memset(kv->xAccu.a, 0, SnP_widthInBytes);
        kv->queueOffset = 0;
    }
    if (kv->phase != COMPRESSING) {
        kv->phase = COMPRESSING;
        kv->queueOffset = 0;
    }
    else if ( kv->queueOffset != 0 ) { /* we have already some data queued */
        BitLength bitlen = MyMin(inputBitLen, SnP_width - kv->queueOffset);
        unsigned int bytelen = (bitlen + 7) / 8;

        memcpy(kv->queue.a + kv->queueOffset / 8, input, bytelen);
        input += bytelen;
        inputBitLen -= bitlen;
        kv->queueOffset += bitlen;
        if ( kv->queueOffset == SnP_width ) { /* queue full */
            Kra_Compress(kv->kRoll.a, kv->xAccu.a, kv->queue.a, &kv->queueOffset, 0);
            kv->queueOffset = 0;
        } 
        else if ( finalFlag != 0 ) {
            Kra_Compress(kv->kRoll.a, kv->xAccu.a, kv->queue.a, &kv->queueOffset, 1);
            return 0;
        }
    }
    if ( (inputBitLen >= SnP_width) || (finalFlag != 0) ) { /* Compress blocks */
        input = Kra_Compress(kv->kRoll.a, kv->xAccu.a, input, &inputBitLen, finalFlag);
    }
    if ( inputBitLen != 0 ) { /* Queue eventual residual message bytes */
        assert( inputBitLen < SnP_width );
        assert( finalFlag == 0 );
        memcpy(kv->queue.a, input, inputBitLen/8);
        kv->queueOffset = inputBitLen;
    }
    return 0;
}

int Vatte(Kravatte_Instance *kv, BitSequence *output, BitLength outputBitLen, int flags)
{
    size_t outputByteLen;
    unsigned char encbuf[MaxParallellism*Kravatte_RolleSizeInBytes];
    int finalFlag = flags & KRAVATTE_FLAG_LAST_PART;

    if ((finalFlag == 0) && ((outputBitLen & 7) != 0))
        return 1;
    if ( kv->phase == COMPRESSING) {
        if ( kv->queueOffset != 0 )
            return 1;
        if ((flags & KRAVATTE_FLAG_SHORT) != 0) {
            memcpy(kv->yAccu.a, kv->xAccu.a, SnP_widthInBytes);
        }
        else {
            ALIGN(KeccakP1600_stateAlignment) unsigned char state[KeccakP1600_stateSizeInBytes];

            KeccakP1600_StaticInitialize();
            mInitialize(state);
            KeccakP1600_OverwriteBytes(state, kv->xAccu.a, 0, SnP_widthInBytes);
            KeccakP1600_Permute_Nrounds(state, 6);
            KeccakP1600_ExtractBytes(state, kv->yAccu.a, 0, SnP_widthInBytes);
        }
        kv->phase = EXPANDING;
        DUMP("yAccu", kv->yAccu.a, SnP_widthInBytes);
        DUMP("key  ", kv->k.a, SnP_widthInBytes);
    }
    else if (kv->phase != EXPANDING)
        return 1;
    if ( kv->queueOffset != 0 ) { /* we have already some data for output in stock */
        unsigned int bitlen = MyMin(outputBitLen, SnP_widthInBytes*8 - kv->queueOffset);
        unsigned int bytelen = (bitlen + 7) / 8;

        memcpy(output, kv->queue.a + kv->queueOffset / 8, bytelen);
        kv->queueOffset += bitlen;
        if (kv->queueOffset == SnP_widthInBytes*8)
            kv->queueOffset = 0;
        output += bytelen;
        outputBitLen -= bitlen;
        if ((finalFlag != 0) && (outputBitLen == 0)) {
            bitlen &= 7;
            if (bitlen != 0) /* cleanup last incomplete byte */
                *(output - 1) &= (1 << bitlen) - 1;
            kv->phase = EXPANDED;
            return 0;
        }
    }

    outputByteLen = (outputBitLen + 7) / 8;
    #if defined(KeccakP1600times8_implementation) && !defined(KeccakP1600times8_isFallback)
    #if defined(KeccakF1600times8_FastKravatte_supported)
    ParallelExpandLoopFast( 8 )
    #else
    ParallelExpandLoopPlSnP( 8 )
    #endif
    #endif
    #if defined(KeccakP1600times4_implementation) && !defined(KeccakP1600times4_isFallback)
    #if defined(KeccakF1600times4_FastKravatte_supported)
    ParallelExpandLoopFast( 4 )
    #else
    ParallelExpandLoopPlSnP( 4 )
    #endif
    #endif
    #if defined(KeccakP1600times2_implementation) && !defined(KeccakP1600times2_isFallback)
    #if defined(KeccakF1600times2_FastKravatte_supported)
    ParallelExpandLoopFast( 2 )
    #else
    ParallelExpandLoopPlSnP( 2 )
    #endif
    #endif
    if ( outputByteLen != 0 ) {
        ALIGN(KeccakP1600_stateAlignment) unsigned char state[KeccakP1600_stateSizeInBytes];
        unsigned int len;

        KeccakP1600_StaticInitialize();
        mInitialize(state);
        do {
            len = MyMin(outputByteLen, SnP_widthInBytes);
            KeccakP1600_OverwriteBytes(state, kv->yAccu.a, 0, SnP_widthInBytes);
            Kravatte_Rolle((uint64_t*)kv->yAccu.a, encbuf, 1);
            KeccakP1600_Permute_Nrounds(state, 6);
            KeccakP1600_ExtractAndAddBytes(state, kv->kRoll.a, output, 0, len);
            DUMP("out 1", output, len);
            output += len;
            outputByteLen -= len;
        } while ( outputByteLen != 0 );
        if (!finalFlag && (len != SnP_widthInBytes)) { /* Put rest of expanded data into queue */
            unsigned int offset = len;
            len = SnP_widthInBytes - len;
            KeccakP1600_ExtractAndAddBytes(state, kv->kRoll.a + offset, kv->queue.a + offset, offset, len);
            kv->queueOffset = offset * 8; /* current bit offset in queue buffer */
        }
    }
    if (finalFlag != 0) {
        outputBitLen &= 7;
        if (outputBitLen != 0) { /* cleanup last incomplete byte */
            *(output - 1) &= (1 << outputBitLen) - 1;
            DUMP("out L", output - 1, 1);
        }
        kv->phase = EXPANDED;
    }
     return 0;
}

int Kravatte(Kravatte_Instance *kv, const BitSequence *input, BitLength inputBitLen, BitSequence *output, BitLength outputBitLen, int flags)
{

    flags |= KRAVATTE_FLAG_LAST_PART;
    if ( Kra(kv, input, inputBitLen, flags) != 0 )
        return 1;
    return Vatte(kv, output, outputBitLen, flags);
}

#endif
