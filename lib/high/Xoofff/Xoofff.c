/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef Xoodoo_excluded

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "brg_endian.h"
#include "Xoodoo.h"
#include "Xoofff.h"

/* 
** Uncomment this define if calls to Xoodoo_Initialize() and 
** Xoodootimes##Parallellism##_InitializeAll() are mandatory to make it work,
** mostly not needed.
**
#define    NEED_INITIALIZE
*/

/* 
** Uncomment this define for more debugging dumps.
**
#define    DEBUG_DUMP 
*/

/*
 * Uncomment this define if your CPU can not handle misaligned memory accesses.
#define NO_MISALIGNED_ACCESSES
 */

#define laneSize        4
#define widthInLanes    (SnP_widthInBytes/laneSize)
#define SnP_width       (SnP_widthInBytes*8)

#define MyMin(a, b)     (((a) < (b)) ? (a) : (b))

#if defined(NEED_INITIALIZE)
#define mInitialize(argState)                   Xoodoo_Initialize(argState)
#define mInitializePl(argStates, Parallellism)  Xoodootimes##Parallellism##_InitializeAll(argStates)
#else
#define mInitialize(argState)
#define mInitializePl(argStates, Parallellism)
#endif

#if defined(DEBUG_DUMP)
static void DUMP( const unsigned char * pText, const unsigned char * pData, unsigned int size )
{
    unsigned int i;
    if (!(size % 4)) {
        uint32_t * p32 = (uint32_t*)pData;
        size /= 4;
        printf("%s:\n", pText, size);
        for(i=0; i<size; i++) {
            if (i&&!(i%12))
                printf("\n");
            printf(" %08x", p32[i]);
        }
        printf("\n");
    }
    else {
        printf("%s (%u bytes):", pText, size);
        for(i=0; i<size; i++)
            printf(" %02x", (int)pData[i]);
        printf("\n");
    }
}

static void DumpBuf( const unsigned char * pText, const unsigned char * pData, unsigned int size )
{
    unsigned int i;
    if (!(size % 4)) {
        uint32_t * p32 = (uint32_t*)pData;
        size /= 4;
        printf("%s:\n", pText, size);
        for(i=0; i<size; i++) {
            if (i&&!(i%12))
                printf("\n");
            printf(" %08x", p32[i]);
        }
        printf("\n");
    }
    else {
        printf("%s (%u bytes):", pText, size);
        for(i=0; i<size; i++)
            printf(" %02x", (int)pData[i]);
        printf("\n");
    }
}

#else
#define DUMP(pText, pData, size )
#define DumpBuf(pText, pData, size )
#endif


#define ParallelCompressLoopFast( Parallellism ) \
    if ( messageByteLen >= Parallellism * SnP_widthInBytes ) { \
        size_t processed = Xooffftimes##Parallellism##_CompressFastLoop((uint8_t*)k, (uint8_t*)x, message, messageByteLen); \
        message += processed; \
        messageByteLen -= processed; \
    }

#define ParallelExpandLoopFast( Parallellism ) \
    if ( outputByteLen >= Parallellism * SnP_widthInBytes ) { \
        size_t processed = Xooffftimes##Parallellism##_ExpandFastLoop((uint8_t*)xp->yAccu.a, (uint8_t*)xp->kRoll.a, output, outputByteLen); \
        output += processed; \
        outputByteLen -= processed; \
    }

#define ParallelCompressLoopPlSnP( Parallellism ) \
    if ( messageByteLen >= Parallellism * SnP_widthInBytes ) { \
        ALIGN(Xoodootimes##Parallellism##_statesAlignment) unsigned char states[Xoodootimes##Parallellism##_statesSizeInBytes]; \
        unsigned int i; \
        \
        Xoodootimes##Parallellism##_StaticInitialize(); \
        mInitializePl(states, Parallellism); \
        do { \
            Xoofff_Rollc( (uint32_t*)k, encbuf, Parallellism ); \
            i = 0; \
            do { \
                Xoodootimes##Parallellism##_OverwriteBytes(states, i, encbuf + i * Xoofff_RollSizeInBytes, Xoofff_RollOffset, Xoofff_RollSizeInBytes); \
            } while ( ++i < Parallellism ); \
            Xoodootimes##Parallellism##_AddLanesAll(states, message, widthInLanes, widthInLanes); \
            DUMP("msg pn", message, Parallellism * SnP_widthInBytes); \
            Xoodootimes##Parallellism##_PermuteAll_6rounds(states); \
            i = 0; \
            do { \
                Xoodootimes##Parallellism##_ExtractAndAddBytes(states, i, x, x, 0, SnP_widthInBytes); \
                DUMP("xAc pn", x, SnP_widthInBytes); \
            } while ( ++i < Parallellism ); \
            message += Parallellism * SnP_widthInBytes; \
            messageByteLen -= Parallellism * SnP_widthInBytes; \
        } while ( messageByteLen >= Parallellism * SnP_widthInBytes ); \
    }

#define ParallelExpandLoopPlSnP( Parallellism ) \
    if ( outputByteLen >= Parallellism * SnP_widthInBytes ) { \
        ALIGN(Xoodootimes##Parallellism##_statesAlignment) unsigned char states[Xoodootimes##Parallellism##_statesSizeInBytes]; \
        unsigned int i; \
        \
        Xoodootimes##Parallellism##_StaticInitialize(); \
        mInitializePl(states, Parallellism); \
        do { \
            Xoofff_Rolle( (uint32_t*)xp->yAccu.a, encbuf, Parallellism ); \
            i = 0; \
            do { \
                Xoodootimes##Parallellism##_OverwriteBytes(states, i, encbuf + i * Xoofff_RollSizeInBytes, Xoofff_RollOffset, Xoofff_RollSizeInBytes); \
            } while ( ++i < Parallellism ); \
            Xoodootimes##Parallellism##_PermuteAll_6rounds(states); \
            i = 0; \
            do { \
                Xoodootimes##Parallellism##_ExtractAndAddBytes(states, i, xp->kRoll.a, output, 0, SnP_widthInBytes); \
                DUMP("out n", output, SnP_widthInBytes); \
                output += SnP_widthInBytes; \
            } while ( ++i < Parallellism ); \
            outputByteLen -= Parallellism * SnP_widthInBytes; \
        } while ( outputByteLen >= Parallellism * SnP_widthInBytes ); \
    }

static void Xoofff_Rollc( uint32_t *a, unsigned char *encbuf, unsigned int parallellism )
{
    uint32_t    b[NCOLUMS];
    #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    uint32_t    *pEnc = (uint32_t*)encbuf;
    #endif

    do {
        #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        *(pEnc++) = a[0];
        *(pEnc++) = a[1];
        *(pEnc++) = a[2];
        *(pEnc++) = a[3];
        *(pEnc++) = a[4];
        *(pEnc++) = a[5];
        *(pEnc++) = a[6];
        *(pEnc++) = a[7];
        *(pEnc++) = a[8];
        *(pEnc++) = a[9];
        *(pEnc++) = a[10];
        *(pEnc++) = a[11];
        DUMP("Roll-c", pEnc - Xoofff_RollSizeInBytes/4, Xoofff_RollSizeInBytes);
        #else
        #error todo
        #endif

        a[0] ^= (a[0] << 13) ^ ROTL32(a[4], 3);
        b[0] = a[1];
        b[1] = a[2];
        b[2] = a[3];
        b[3] = a[0];

        a[0] = a[4+0];
        a[1] = a[4+1];
        a[2] = a[4+2];
        a[3] = a[4+3];

        a[4+0] = a[8+0];
        a[4+1] = a[8+1];
        a[4+2] = a[8+2];
        a[4+3] = a[8+3];

        a[8+0] = b[0];
        a[8+1] = b[1];
        a[8+2] = b[2];
        a[8+3] = b[3];
    } while(--parallellism != 0); 
    DUMP("Roll-c next", a, Xoofff_RollSizeInBytes);
}

static void Xoofff_Rolle( uint32_t *a, unsigned char *encbuf, unsigned int parallellism )
{
    uint32_t    b[NCOLUMS];
    #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
    uint32_t    *pEnc = (uint32_t*)encbuf;
    #endif

    do {
        #if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        *(pEnc++) = a[0];
        *(pEnc++) = a[1];
        *(pEnc++) = a[2];
        *(pEnc++) = a[3];
        *(pEnc++) = a[4];
        *(pEnc++) = a[5];
        *(pEnc++) = a[6];
        *(pEnc++) = a[7];
        *(pEnc++) = a[8];
        *(pEnc++) = a[9];
        *(pEnc++) = a[10];
        *(pEnc++) = a[11];
        DUMP("Roll-e", pEnc - Xoofff_RollSizeInBytes/4, Xoofff_RollSizeInBytes);
        #else
        #error todo
        #endif

        a[0] = ROTL32(a[0], 5) ^ ROTL32(a[4], 13) ^ (a[8] & a[4]) ^ 7;
        b[0] = a[1];
        b[1] = a[2];
        b[2] = a[3];
        b[3] = a[0];

        a[0] = a[4+0];
        a[1] = a[4+1];
        a[2] = a[4+2];
        a[3] = a[4+3];

        a[4+0] = a[8+0];
        a[4+1] = a[8+1];
        a[4+2] = a[8+2];
        a[4+3] = a[8+3];

        a[8+0] = b[0];
        a[8+1] = b[1];
        a[8+2] = b[2];
        a[8+3] = b[3];
    } while(--parallellism != 0);
    DUMP("Roll-e next", a, Xoofff_RollSizeInBytes);
}

#if !defined(Xoodoo_FastXoofff_supported)

#if !defined(Xoofff_AddIs)
void Xoofff_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen)
{
    size_t  byteLen = bitLen / 8;

    #if !defined(NO_MISALIGNED_ACCESSES)
    while ( byteLen >= 32 ) {
        *((uint64_t*)(output+0)) ^= *((uint64_t*)(input+0));
        *((uint64_t*)(output+8)) ^= *((uint64_t*)(input+8));
        *((uint64_t*)(output+16)) ^= *((uint64_t*)(input+16));
        *((uint64_t*)(output+24)) ^= *((uint64_t*)(input+24));
        input += 32;
        output += 32;
        byteLen -= 32;
    }
    while ( byteLen >= 8 ) {
        *((uint64_t*)output) ^= *((uint64_t*)input);
        input += 8;
        output += 8;
        byteLen -= 8;
    }
    #endif

    while ( byteLen-- != 0 )
    {
        *output++ ^= *input++;
    }

    bitLen &= 7;
    if (bitLen != 0)
    {
        *output ^= *input;
        *output &= (1 << bitLen) - 1;
    }
}

#endif

size_t Xoofff_CompressFastLoop(unsigned char *k, unsigned char *x, const unsigned char *input, size_t length)
{
    unsigned char encbuf[Xoofff_RollSizeInBytes];
    ALIGN(Xoodoo_stateAlignment) unsigned char state[Xoodoo_stateSizeInBytes];
    size_t    initialLength = length;

    assert(length >= SnP_widthInBytes);
    Xoodoo_StaticInitialize();
    mInitialize(state);
    do {
        Xoodoo_OverwriteBytes(state, k, 0, SnP_widthInBytes);
        Xoofff_Rollc((uint32_t*)k, encbuf, 1);
        Xoodoo_AddBytes(state, input, 0, SnP_widthInBytes);
        DUMP("msg p1", input, SnP_widthInBytes);
        Xoodoo_Permute_6rounds(state);
        Xoodoo_ExtractAndAddBytes(state, x, x, 0, SnP_widthInBytes);
        DUMP("xAc p1", x, SnP_widthInBytes);
        input += SnP_widthInBytes;
        length -= SnP_widthInBytes;
    }
    while (length >= SnP_widthInBytes);

    return initialLength - length;
}

size_t Xoofff_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length)
{
    unsigned char encbuf[Xoofff_RollSizeInBytes];
    ALIGN(Xoodoo_stateAlignment) unsigned char state[Xoodoo_stateSizeInBytes];
    size_t    initialLength = length;

    assert(length >= SnP_widthInBytes);
    Xoodoo_StaticInitialize();
    mInitialize(state);
    do {
        Xoodoo_OverwriteBytes(state, yAccu, 0, SnP_widthInBytes);
        Xoofff_Rolle((uint32_t*)yAccu, encbuf, 1);
        Xoodoo_Permute_6rounds(state);
        Xoodoo_ExtractAndAddBytes(state, kRoll, output, 0, SnP_widthInBytes);
        DUMP("out 1", output, SnP_widthInBytes);
        output += SnP_widthInBytes;
        length -= SnP_widthInBytes;
    } while (length >= SnP_widthInBytes);

    return initialLength - length;
}

#endif

static const unsigned char * Xoodoo_CompressBlocks( unsigned char *k, unsigned char *x, const BitSequence *message, BitLength *messageBitLen, int lastFlag )
{
    ALIGN(Xoodoo_stateAlignment) unsigned char encbuf[XoodooMaxParallellism*Xoofff_RollSizeInBytes];
    size_t messageByteLen = *messageBitLen / 8; /* do not include partial last byte */

    #if (XoodooMaxParallellism >= 16)
    #if defined(Xoodootimes16_FastXoofff_supported)
    ParallelCompressLoopFast( 16 )
    #else
    ParallelCompressLoopPlSnP( 16 )
    #endif
    #endif
    #if (XoodooMaxParallellism >= 8)
    #if defined(Xoodootimes8_FastXoofff_supported)
    ParallelCompressLoopFast( 8 )
    #else
    ParallelCompressLoopPlSnP( 8 )
    #endif
    #endif
    #if (XoodooMaxParallellism >= 4)
    #if defined(Xoodootimes4_FastXoofff_supported)
    ParallelCompressLoopFast( 4 )
    #else
    ParallelCompressLoopPlSnP( 4 )
    #endif
    #endif
    #if (XoodooMaxParallellism >= 2) && 0
    #if defined(Xoodootimes2_FastXoofff_supported)
    ParallelCompressLoopFast( 2 )
    #else
    ParallelCompressLoopPlSnP( 2 )
    #endif
    #endif

    if (messageByteLen >= SnP_widthInBytes) {
        size_t processed = Xoofff_CompressFastLoop(k, x, message, messageByteLen);
        message += processed;
        messageByteLen -= processed;
    }
    *messageBitLen %= SnP_width;
    if ( lastFlag != 0 ) {
        ALIGN(Xoodoo_stateAlignment) unsigned char state[Xoodoo_stateSizeInBytes];

        assert(messageByteLen < SnP_widthInBytes);
        Xoodoo_StaticInitialize();
        mInitialize(state);
        Xoodoo_OverwriteBytes(state, k, 0, SnP_widthInBytes); /* write k */
        Xoofff_Rollc((uint32_t*)k, encbuf, 1);
        Xoodoo_AddBytes(state, message, 0, messageByteLen); /* add message */
        DUMP("msg pL", state, SnP_widthInBytes);
        message += messageByteLen;
        *messageBitLen %= 8;
        if (*messageBitLen != 0) /* padding */
            Xoodoo_AddByte(state, *message++ | (1 << *messageBitLen), messageByteLen);
        else
            Xoodoo_AddByte(state, 1, messageByteLen);
        Xoodoo_Permute_6rounds(state);
        Xoodoo_ExtractAndAddBytes(state, x, x, 0, SnP_widthInBytes);
        DUMP("xAc pL", x, SnP_widthInBytes);
        Xoofff_Rollc((uint32_t*)k, encbuf, 1);
        *messageBitLen = 0;
    }
    return message;
}

int Xoofff_MaskDerivation(Xoofff_Instance *xp, const BitSequence *Key, BitLength KeyBitLen)
{
    ALIGN(Xoodoo_stateAlignment) unsigned char state[Xoodoo_stateSizeInBytes];
    BitSequence lastByte;
    unsigned int numberOfBits;

    /* Check max K length (b-1) */
    if (KeyBitLen >= SnP_width)
        return 1;
    /* Compute k from K */
    memset(xp->k.a, 0, SnP_widthInBytes);
    memcpy(xp->k.a, Key, KeyBitLen/8);
    numberOfBits = KeyBitLen & 7;
    if ((numberOfBits) != 0) {
        lastByte = (Key[KeyBitLen/8] & ((1 << numberOfBits) - 1)) | (1 << numberOfBits);
    }
    else {
        lastByte = 1;
    }
    xp->k.a[KeyBitLen/8] = lastByte;
    Xoodoo_StaticInitialize();
    mInitialize(state);
    Xoodoo_OverwriteBytes(state, xp->k.a, 0, SnP_widthInBytes);
    Xoodoo_Permute_6rounds(state);
    Xoodoo_ExtractBytes(state, xp->k.a, 0, SnP_widthInBytes);
    memcpy(xp->kRoll.a, xp->k.a, SnP_widthInBytes);
    memset(xp->xAccu.a, 0, SnP_widthInBytes);
    xp->phase = COMPRESSING;
    xp->queueOffset = 0;

    return 0;
}

int Xoofff_Compress(Xoofff_Instance *xp, const BitSequence *input, BitLength inputBitLen, int flags)
{
    int finalFlag = flags & Xoofff_FlagLastPart;

    if ((finalFlag == 0) && ((inputBitLen & 7) != 0))
        return 1;
    if ( (flags & Xoofff_FlagInit) != 0 ) {
        memcpy(xp->kRoll.a, xp->k.a, SnP_widthInBytes);
        memset(xp->xAccu.a, 0, SnP_widthInBytes);
        xp->queueOffset = 0;
    }
    if (xp->phase != COMPRESSING) {
        xp->phase = COMPRESSING;
        xp->queueOffset = 0;
    }
    else if ( xp->queueOffset != 0 ) { /* we have already some data queued */
        BitLength bitlen = MyMin(inputBitLen, SnP_width - xp->queueOffset);
        unsigned int bytelen = (bitlen + 7) / 8;

        memcpy(xp->queue.a + xp->queueOffset / 8, input, bytelen);
        input += bytelen;
        inputBitLen -= bitlen;
        xp->queueOffset += bitlen;
        if ( xp->queueOffset == SnP_width ) { /* queue full */
            Xoodoo_CompressBlocks(xp->kRoll.a, xp->xAccu.a, xp->queue.a, &xp->queueOffset, 0);
            xp->queueOffset = 0;
        } 
        else if ( finalFlag != 0 ) {
            Xoodoo_CompressBlocks(xp->kRoll.a, xp->xAccu.a, xp->queue.a, &xp->queueOffset, 1);
            return 0;
        }
    }
    if ( (inputBitLen >= SnP_width) || (finalFlag != 0) ) { /* Compress blocks */
        input = Xoodoo_CompressBlocks(xp->kRoll.a, xp->xAccu.a, input, &inputBitLen, finalFlag);
    }
    if ( inputBitLen != 0 ) { /* Queue eventual residual message bytes */
        assert( inputBitLen < SnP_width );
        assert( finalFlag == 0 );
        memcpy(xp->queue.a, input, inputBitLen/8);
        xp->queueOffset = inputBitLen;
    }
    return 0;
}

int Xoofff_Expand(Xoofff_Instance *xp, BitSequence *output, BitLength outputBitLen, int flags)
{
    size_t outputByteLen;
    ALIGN(Xoofff_Alignment) unsigned char encbuf[XoodooMaxParallellism*Xoofff_RollSizeInBytes];
    int finalFlag = flags & Xoofff_FlagLastPart;

    if ((finalFlag == 0) && ((outputBitLen & 7) != 0))
        return 1;
    if ( xp->phase == COMPRESSING) {
        if ( xp->queueOffset != 0 )
            return 1;
        if ((flags & Xoofff_FlagXoofffie) != 0) {
            memcpy(xp->yAccu.a, xp->xAccu.a, SnP_widthInBytes);
        }
        else {
            ALIGN(Xoodoo_stateAlignment) unsigned char state[Xoodoo_stateSizeInBytes];

            Xoodoo_StaticInitialize();
            mInitialize(state);
            Xoodoo_OverwriteBytes(state, xp->xAccu.a, 0, SnP_widthInBytes);
            Xoodoo_Permute_6rounds(state);
            Xoodoo_ExtractBytes(state, xp->yAccu.a, 0, SnP_widthInBytes);
        }
        xp->phase = EXPANDING;
        DUMP("yAccu", xp->yAccu.a, SnP_widthInBytes);
        DUMP("key  ", xp->k.a, SnP_widthInBytes);
    }
    else if (xp->phase != EXPANDING)
        return 1;
    if ( xp->queueOffset != 0 ) { /* we have already some data for output in stock */
        unsigned int bitlen = MyMin(outputBitLen, SnP_widthInBytes*8 - xp->queueOffset);
        unsigned int bytelen = (bitlen + 7) / 8;

        memcpy(output, xp->queue.a + xp->queueOffset / 8, bytelen);
        xp->queueOffset += bitlen;
        if (xp->queueOffset == SnP_widthInBytes*8)
            xp->queueOffset = 0;
        output += bytelen;
        outputBitLen -= bitlen;
        if ((finalFlag != 0) && (outputBitLen == 0)) {
            bitlen &= 7;
            if (bitlen != 0) /* cleanup last incomplete byte */
                *(output - 1) &= (1 << bitlen) - 1;
            xp->phase = EXPANDED;
            return 0;
        }
    }

    outputByteLen = (outputBitLen + 7) / 8;
    #if (XoodooMaxParallellism >= 16)
    #if defined(Xoodootimes16_FastXoofff_supported)
    ParallelExpandLoopFast( 16 )
    #else
    ParallelExpandLoopPlSnP( 16 )
    #endif
    #endif
    #if (XoodooMaxParallellism >= 8)
    #if defined(Xoodootimes8_FastXoofff_supported)
    ParallelExpandLoopFast( 8 )
    #else
    ParallelExpandLoopPlSnP( 8 )
    #endif
    #endif
    #if (XoodooMaxParallellism >= 4)
    #if defined(Xoodootimes4_FastXoofff_supported)
    ParallelExpandLoopFast( 4 )
    #else
    ParallelExpandLoopPlSnP( 4 )
    #endif
    #endif
    #if (XoodooMaxParallellism >= 2) && 0
    #if defined(Xoodootimes2_FastXoofff_supported)
    ParallelExpandLoopFast( 2 )
    #else
    ParallelExpandLoopPlSnP( 2 )
    #endif
    #endif
    if ( outputByteLen >= SnP_widthInBytes ) {
        size_t processed = Xoofff_ExpandFastLoop(xp->yAccu.a, xp->kRoll.a, output, outputByteLen);
        output += processed;
        outputByteLen -= processed;
    }
    if ( outputByteLen != 0 ) {    /* Last incomplete block */
        ALIGN(Xoodoo_stateAlignment) unsigned char state[Xoodoo_stateSizeInBytes];

        Xoodoo_StaticInitialize();
        mInitialize(state);
        Xoodoo_OverwriteBytes(state, xp->yAccu.a, 0, SnP_widthInBytes);
        Xoofff_Rolle((uint32_t*)xp->yAccu.a, encbuf, 1);
        Xoodoo_Permute_6rounds(state);
        Xoodoo_ExtractAndAddBytes(state, xp->kRoll.a, output, 0, outputByteLen);
        DUMP("out 1", output, outputByteLen);
        output += outputByteLen;
        if (!finalFlag) { /* Put rest of expanded data into queue */
            unsigned int offset = outputByteLen;
            Xoodoo_ExtractAndAddBytes(state, xp->kRoll.a + offset, xp->queue.a + offset, offset, SnP_widthInBytes - outputByteLen);
            xp->queueOffset = offset * 8; /* current bit offset in queue buffer */
        }
    }
    if (finalFlag != 0) {
        outputBitLen &= 7;
        if (outputBitLen != 0) { /* cleanup last incomplete byte */
            *(output - 1) &= (1 << outputBitLen) - 1;
            DUMP("out L", output - 1, 1);
        }
        xp->phase = EXPANDED;
    }
    return 0;
}

int Xoofff(Xoofff_Instance *xp, const BitSequence *input, BitLength inputBitLen, BitSequence *output, BitLength outputBitLen, int flags)
{

    flags |= Xoofff_FlagLastPart;
    if ( Xoofff_Compress(xp, input, inputBitLen, flags) != 0 )
        return 1;
    return Xoofff_Expand(xp, output, outputBitLen, flags);
}

#endif
