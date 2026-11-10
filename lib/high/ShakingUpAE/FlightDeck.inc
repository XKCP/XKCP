/*
 * The eXtended Keccak Code Package (XKCP)
 * https://github.com/XKCP/XKCP
 *
 * “Shaking up authenticated encryption”: Keccak-based duplex ciphers, deck ciphers and authenticated encryption schemes designed by Joan Daemen, Seth Hoffert, Silvia Mella, Gilles Van Assche and Ronny Van Keer
 *
 * Implementation by Alex Bataille and Gilles Van Assche, hereby denoted as "the implementer".
 *
 * For more information, feedback or questions, please refer to the Keccak Team website:
 * https://keccak.team/
 *
 * To the extent possible under law, the implementer has waived all copyright
 * and related or neighboring rights to the source code in this file.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "align.h"
#include <string.h>
#include <stdio.h>

#define JOIN0(a,b)                                      a##b
#define JOIN(a,b)                                       JOIN0(a,b)

#define OD_Initialize                                   JOIN(prefix, _OD_Initialize)
#define OD_Clone                                        JOIN(prefix, _OD_Clone)
#define OD_CloneCompact                                 JOIN(prefix, _OD_CloneCompact)
#define OD_Duplexing                                    JOIN(prefix, _OD_Duplexing)
#define OD_DuplexingFast                                JOIN(prefix, _OD_DuplexingFast)
#define OD_DuplexingFastOnlyOut                         JOIN(prefix, _OD_DuplexingFastOnlyOut)
#define OD_DuplexingFastOnlyIn                          JOIN(prefix, _OD_DuplexingFastOnlyIn)
#define OD_Squeezing                                    JOIN(prefix, _OD_Squeezing)

#define ODTimes2_Clone                                  JOIN(prefix, Times2_ParallelOD_Clone)
#define ODTimes2_CloneCompactFromOD                     JOIN(prefix, Times2_ParallelOD_CloneCompactFromOD)
#define ODTimes2_DuplexingFastOnlyIn                    JOIN(prefix, Times2_ParallelOD_DuplexingFastOnlyIn)
#define ODTimes2_Duplexing                              JOIN(prefix, Times2_ParallelOD_Duplexing)
#define ODTimes2_Squeezing                              JOIN(prefix, Times2_ParallelOD_Squeezing)
#define ODTimes4_Clone                                  JOIN(prefix, Times4_ParallelOD_Clone)
#define ODTimes4_CloneCompactFromOD                     JOIN(prefix, Times4_ParallelOD_CloneCompactFromOD)
#define ODTimes4_DuplexingFastOnlyIn                    JOIN(prefix, Times4_ParallelOD_DuplexingFastOnlyIn)
#define ODTimes4_Duplexing                              JOIN(prefix, Times4_ParallelOD_Duplexing)
#define ODTimes4_Squeezing                              JOIN(prefix, Times4_ParallelOD_Squeezing)
#define ODTimes8_Clone                                  JOIN(prefix, Times8_ParallelOD_Clone)
#define ODTimes8_CloneCompactFromOD                     JOIN(prefix, Times8_ParallelOD_CloneCompactFromOD)
#define ODTimes8_DuplexingFastOnlyIn                    JOIN(prefix, Times8_ParallelOD_DuplexingFastOnlyIn)
#define ODTimes8_Duplexing                              JOIN(prefix, Times8_ParallelOD_Duplexing)
#define ODTimes8_Squeezing                              JOIN(prefix, Times8_ParallelOD_Squeezing)

#define FlightDeck_Initialize128                        JOIN(prefix, 128_FlightDeck_Initialize)
#define FlightDeck_Initialize256                        JOIN(prefix, 256_FlightDeck_Initialize)
#define FlightDeck_Initialize                           JOIN(prefix, _FlightDeck_Initialize)
#define FlightDeck_Clone                                JOIN(prefix, _FlightDeck_Clone)
#define FlightDeck_CloneCompact                         JOIN(prefix, _FlightDeck_CloneCompact)
#define FlightDeck_Duplexing                            JOIN(prefix, _FlightDeck_Duplexing)
#define FlightDeck_Squeezing                            JOIN(prefix, _FlightDeck_Squeezing)

#define ALPHA   fd->alpha

#define PlSnP_prefix(Parallelism, level)                JOIN(JOIN(KeccakP1600times, JOIN(Parallelism, _)), JOIN(prefix, level))

#define ParallelDuplexingLoop( Parallelism ) \
    if (KeccakP1600times##Parallelism##_GetFeatures()) { \
        while(inputLen >= Parallelism * ALPHA) { \
            size_t bytesProcessed = 0; \
            if ((KeccakP1600times##Parallelism##_GetFeatures() & PlSnP_Feature_FlightDeck) && (fd->D.c == 256) && (ALPHA == 5120))  { \
                unsigned char Dlast[40]; \
                KeccakP1600_ExtractBytes(&D0.s, Dlast, 160, 40); \
                bytesProcessed = JOIN(PlSnP_prefix(Parallelism, 128), _FlightDeckProcessLeaves)(Dlast, input, rootBuffer + rootBufferCount); \
            } \
            if ((KeccakP1600times##Parallelism##_GetFeatures() & PlSnP_Feature_FlightDeck) && (fd->D.c == 512) && (ALPHA == 8192))  { \
                unsigned char Dlast[72]; \
                KeccakP1600_ExtractBytes(&D0.s, Dlast, 128, 72); \
                bytesProcessed = JOIN(PlSnP_prefix(Parallelism, 256), _FlightDeckProcessLeaves)(Dlast, input, rootBuffer + rootBufferCount); \
            } \
            if (bytesProcessed == 0) { \
                KeccakWidth1600Times##Parallelism##_ParallelODInstance Dp; \
                ODTimes##Parallelism##_CloneCompactFromOD( &Dp, &D0 ); \
                bytesProcessed = ODTimes##Parallelism##_DuplexingFastOnlyIn( &Dp, input, ALPHA-1, ALPHA, 1u ); \
                ODTimes##Parallelism##_Duplexing( &Dp, NULL, 0, 0, input+bytesProcessed, ALPHA-bytesProcessed, ALPHA, 4u, NULL ); \
                bytesProcessed = ALPHA*Parallelism; \
                ODTimes##Parallelism##_Squeezing( &Dp, rootBuffer + rootBufferCount, Dp.c/8, Dp.c/8, NULL ); \
            } \
            rootBufferCount += Parallelism*fd->D.c/8; \
            input += bytesProcessed; \
            inputLen -= bytesProcessed; \
            if (rootBufferCount > fd->D.rho) { \
                rootProcessed = OD_DuplexingFastOnlyIn(&fd->D, rootBuffer, rootBufferCount, 1u); \
                memmove(rootBuffer, rootBuffer + rootProcessed, rootBufferCount - rootProcessed); \
                rootBufferCount -= rootProcessed; \
            } \
        } \
    }

// TODO: this is little-endian specific, make this endian neutral
#define ParallelSqueezingLoop( Parallelism ) \
    if (KeccakP1600times##Parallelism##_GetFeatures()) { \
        size_t bytesProcessed = 0; \
        if ((KeccakP1600times##Parallelism##_GetFeatures() & PlSnP_Feature_FlightDeck) && (fd->D.c == 256))  { \
            unsigned char Dlast[40]; \
            KeccakP1600_ExtractBytes(&fd->D.s, Dlast, 160, 40); \
            bytesProcessed = JOIN(PlSnP_prefix(Parallelism, 128), _FlightDeckSqueeze)(Dlast, fd->o, output, outputLen - processed, toAdd); \
            if (toAdd != NULL) toAdd += bytesProcessed; \
            processed += bytesProcessed; \
            output += bytesProcessed; \
            fd->o += bytesProcessed; \
        } \
        if ((KeccakP1600times##Parallelism##_GetFeatures() & PlSnP_Feature_FlightDeck) && (fd->D.c == 512))  { \
            unsigned char Dlast[72]; \
            KeccakP1600_ExtractBytes(&fd->D.s, Dlast, 128, 72); \
            bytesProcessed = JOIN(PlSnP_prefix(Parallelism, 256), _FlightDeckSqueeze)(Dlast, fd->o, output, outputLen - processed, toAdd); \
            if (toAdd != NULL) toAdd += bytesProcessed; \
            processed += bytesProcessed; \
            output += bytesProcessed; \
            fd->o += bytesProcessed; \
        } \
        if (bytesProcessed == 0) { \
            KeccakWidth1600Times##Parallelism##_ParallelODInstance Ds0; \
            ODTimes##Parallelism##_CloneCompactFromOD(&Ds0, &fd->D); \
            uint64_t countersBuffer[Parallelism*fd->D.rho/8]; \
            memset(countersBuffer, 0, sizeof(countersBuffer)); \
            for (unsigned int i = 0; i < Parallelism; i++) \
                countersBuffer[i*fd->D.rho/8] = (fd->o / fd->D.rho) + i; \
            while (outputLen - processed >= (size_t)(Parallelism) * fd->D.rho) { \
                KeccakWidth1600Times##Parallelism##_ParallelODInstance Ds; \
                ODTimes##Parallelism##_Clone(&Ds, &Ds0); \
                ODTimes##Parallelism##_Duplexing( &Ds, output, fd->D.rho, fd->D.rho, (const unsigned char*)countersBuffer, fd->D.rho, fd->D.rho, 2u, toAdd ); \
                if ( toAdd != NULL ) toAdd += (size_t) Parallelism * fd->D.rho; \
                processed += (size_t)Parallelism * fd->D.rho; \
                output += (size_t)Parallelism * fd->D.rho; \
                fd->o += (size_t)Parallelism * fd->D.rho; \
                for (unsigned int i = 0; i < Parallelism; i++) \
                    countersBuffer[i*fd->D.rho/8] += Parallelism; \
            } \
        } \
    }

int FlightDeck_Initialize(KeccakWidth1600_FlightDeckInstance *fd, unsigned int rho, unsigned int c, size_t alpha, const unsigned char *key, unsigned int keyByteLen)
{
    if ((rho > RHO_LEN_MAX) || (keyByteLen > rho))
        return 1;
    if (OD_Initialize( &fd->D, rho, c, key, keyByteLen ) != 0)
        return 1;
    memset( &fd->Dsqueeze, 0u, sizeof(fd->Dsqueeze) );
    fd->alpha = alpha;
    fd->o = (size_t) -1;
    return 0;
}

int FlightDeck_Initialize128(KeccakWidth1600_FlightDeckInstance *fd, const unsigned char *key, unsigned int keyByteLen)
{
    return FlightDeck_Initialize(fd, 160, 256, 5120, key, keyByteLen);
}

int FlightDeck_Initialize256(KeccakWidth1600_FlightDeckInstance *fd, const unsigned char *key, unsigned int keyByteLen)
{
    return FlightDeck_Initialize(fd, 128, 512, 8192, key, keyByteLen);
}

int FlightDeck_Clone(KeccakWidth1600_FlightDeckInstance *fdnew, const KeccakWidth1600_FlightDeckInstance *fd)
{
    fdnew->alpha = fd->alpha;
    fdnew->o = fd->o;
    if (OD_Clone( &fdnew->D, &fd->D ) != 0)
        return 1;
    if (OD_Clone( &fdnew->Dsqueeze, &fd->Dsqueeze ) != 0)
        return 1;
    return 0;
}

int FlightDeck_CloneCompact(KeccakWidth1600_FlightDeckInstance *fdnew, const KeccakWidth1600_FlightDeckInstance *fd)
{
    fdnew->alpha = fd->alpha;
    fdnew->o = (size_t)-1;
    if (OD_CloneCompact( &fdnew->D, &fd->D ) != 0)
        return 1;
    memset( &fdnew->Dsqueeze, 0u, sizeof(fdnew->Dsqueeze) );
    return 0;
}

int FlightDeck_Duplexing(KeccakWidth1600_FlightDeckInstance *fd, unsigned char *output, size_t outputLen, const unsigned char *input, size_t inputLen, unsigned int E, const unsigned char *toAdd)
{
    if ( inputLen > ALPHA ) {
        KeccakWidth1600_ODInstance D0;
        unsigned char rootBuffer[2*fd->D.rho + 8*fd->D.c/8];
        size_t rootBufferCount;
        size_t rootProcessed;

        OD_Clone(&D0, &fd->D);
        rootProcessed = OD_DuplexingFastOnlyIn(&fd->D, input, ALPHA, 1u);
        rootBufferCount = ALPHA - rootProcessed;
        memcpy(rootBuffer, input + rootProcessed, rootBufferCount);
        input += ALPHA;
        inputLen -= ALPHA;
#ifdef XKCP_has_KeccakP1600times8
        ParallelDuplexingLoop( 8 )
#endif
#ifdef XKCP_has_KeccakP1600times4
        ParallelDuplexingLoop( 4 )
#endif
#ifdef XKCP_has_KeccakP1600times2
        ParallelDuplexingLoop( 2 )
#endif
        while (inputLen > 0) {
            size_t blockLen = MYMIN(ALPHA, inputLen);
            KeccakWidth1600_ODInstance Dp;
            size_t bytesProcessed;

            OD_CloneCompact(&Dp, &D0);
            bytesProcessed = 0;
            if (blockLen > fd->D.rho)
                bytesProcessed = OD_DuplexingFastOnlyIn(&Dp, input, blockLen, 1u);
            OD_Duplexing(&Dp, NULL, 0, input + bytesProcessed, blockLen - bytesProcessed, 4u, NULL);
            OD_Squeezing(&Dp, rootBuffer + rootBufferCount, fd->D.c/8, NULL);
            rootBufferCount += fd->D.c/8;
            input += blockLen;
            inputLen -= blockLen;
            if (rootBufferCount > fd->D.rho) {
                rootProcessed = OD_DuplexingFastOnlyIn(&fd->D, rootBuffer, rootBufferCount, 1u);
                memmove(rootBuffer, rootBuffer + rootProcessed, rootBufferCount - rootProcessed);
                rootBufferCount -= rootProcessed;
            }
        }
        OD_Duplexing(&fd->D, NULL, 0, rootBuffer, rootBufferCount, FD_Concat(E, 1u), NULL);
        fd->o = 0;
        memset( &fd->Dsqueeze, 0u, sizeof(fd->Dsqueeze) );
        FlightDeck_Squeezing( fd, output, outputLen, toAdd );
    }
    else {
        if (inputLen > fd->D.rho) {
            size_t bytesProcessed = OD_DuplexingFastOnlyIn(&fd->D, input, inputLen, 1u);
            input += bytesProcessed;
            inputLen -= bytesProcessed;
        }
        OD_Duplexing(&fd->D, NULL, 0, input, inputLen, FD_Concat(E, 1u), NULL);
        fd->o = 0;
        memset(&fd->Dsqueeze, 0u, sizeof(fd->Dsqueeze));
        FlightDeck_Squeezing(fd, output, outputLen, toAdd);
    }
    return 0;
}

int FlightDeck_Squeezing(KeccakWidth1600_FlightDeckInstance *fd, unsigned char *output, size_t outputLen, const unsigned char *toAdd)
{
    size_t processed = 0;
    if ( fd->o <= fd->D.rho ) {
        unsigned int x = MYMIN( fd->D.rho - fd->o, outputLen );
        OD_Squeezing( &fd->D, output, x, toAdd );
        output += x;
        if ( toAdd != NULL ) toAdd += x;
        fd->o     += x;
        processed += x;
    }
#ifdef XKCP_has_KeccakP1600times8
    ParallelSqueezingLoop( 8 )
#endif
#ifdef XKCP_has_KeccakP1600times4
    ParallelSqueezingLoop( 4 )
#endif
#ifdef XKCP_has_KeccakP1600times2
    ParallelSqueezingLoop( 2 )
#endif
    while ( processed < outputLen ) {
        unsigned char orho[fd->D.rho];
        size_t orholen = FlightDeckString(orho, sizeof(orho), fd->o / fd->D.rho, fd->D.rho);
        OD_CloneCompact( &fd->Dsqueeze, &fd->D );
        OD_Duplexing( &fd->Dsqueeze, NULL, 0, orho, orholen, 2u, toAdd );
        unsigned int x = MYMIN(outputLen - processed, fd->D.rho);
        OD_Squeezing( &fd->Dsqueeze, output, x, toAdd );
        if ( toAdd != NULL ) toAdd += x;
        processed += x;
        output += x;
        fd->o += x;
    }
    return 0;
}

#undef OD_Initialize
#undef OD_Clone
#undef OD_CloneCompact
#undef OD_Duplexing
#undef OD_DuplexingFast
#undef OD_DuplexingFastOnlyOut
#undef OD_DuplexingFastOnlyIn
#undef OD_Squeezing

#undef ODTimes2_Clone
#undef ODTimes2_CloneCompactFromOD
#undef ODTimes2_DuplexingFastOnlyIn
#undef ODTimes2_Duplexing
#undef ODTimes2_Squeezing
#undef ODTimes4_Clone
#undef ODTimes4_CloneCompactFromOD
#undef ODTimes4_DuplexingFastOnlyIn
#undef ODTimes4_Duplexing
#undef ODTimes4_Squeezing
#undef ODTimes8_Clone
#undef ODTimes8_CloneCompactFromOD
#undef ODTimes8_DuplexingFastOnlyIn
#undef ODTimes8_Duplexing
#undef ODTimes8_Squeezing

#undef FlightDeck_Initialize128
#undef FlightDeck_Initialize256
#undef FlightDeck_Initialize
#undef FlightDeck_Clone
#undef FlightDeck_CloneCompact
#undef FlightDeck_Duplexing
#undef FlightDeck_Squeezing

#undef ALPHA
#undef PlSnP_prefix
#undef ParallelDuplexingLoop
#undef ParallelSqueezingLoop
