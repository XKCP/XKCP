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
#include <stdio.h>

#define JOIN0(a, b)                         a ## b
#define JOIN(a, b)                          JOIN0(a, b)

#define SerialODInstance                    JOIN(inst, _ODInstance)
#define SnP_ExtractBytes                    JOIN(SnP, _ExtractBytes)
#define ParallelODInstance                  JOIN(pinst, _ParallelODInstance)
#define ParallelOD_TrailencAndXor           JOIN(pprefix, _ParallelOD_TrailencAndXor)
#define ParallelOD_Initialize               JOIN(pprefix, _ParallelOD_Initialize)
#define ParallelOD_Clone                    JOIN(pprefix, _ParallelOD_Clone)
#define ParallelOD_CloneCompact             JOIN(pprefix, _ParallelOD_CloneCompact)
#define ParallelOD_CloneCompactFromOD       JOIN(pprefix, _ParallelOD_CloneCompactFromOD)
#define ParallelOD_Duplexing                JOIN(pprefix, _ParallelOD_Duplexing)
#define ParallelOD_Squeezing                JOIN(pprefix, _ParallelOD_Squeezing)
#define ParallelOD_DuplexingFastOnlyIn      JOIN(pprefix, _ParallelOD_DuplexingFastOnlyIn)

#define laneSize                            8

static void ParallelOD_TrailencAndXor( ParallelODInstance *pod, unsigned char D )
{
    ALIGN(8) const unsigned char trailenc[8] = { D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80 + DOM_SEP };
    for (unsigned int i = 0; i < parallelism; i++) {
        PlSnP_AddBytes( &pod->states, i, trailenc, pod->rho, sizeof(trailenc) );
    }
}

int ParallelOD_Initialize( ParallelODInstance *pod, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen )
{
    pod->o       = rho;
    pod->c       = c;
    pod->rho     = rho;
    PlSnP_StaticInitialize();
    PlSnP_InitializeAll( &pod->states );
    if (ParallelOD_Duplexing( pod, NULL, 0, 0, key, keyByteLen, 0, 1, NULL ) != 0)
        return 1;
    return 0;
}

int ParallelOD_Clone( ParallelODInstance *podnew, const ParallelODInstance *pod )
{
    *podnew = *pod;
    return 0;
}

int ParallelOD_CloneCompact( ParallelODInstance *podnew, const ParallelODInstance *pod )
{
    *podnew = *pod;
    podnew->o = pod->rho;
    return 0;
}

int ParallelOD_CloneCompactFromOD( ParallelODInstance *podnew, const SerialODInstance *od )
{
    unsigned char Dstate[SnP_stateSizeInBytes - od->rho];
    SnP_ExtractBytes( &od->s, Dstate, od->rho, SnP_stateSizeInBytes - od->rho );

    // Copy that state into every lane of the parallel instance
    PlSnP_StaticInitialize();
    PlSnP_InitializeAll( &podnew->states );
    for ( unsigned int i = 0; i < parallelism; i++ ) {
        PlSnP_OverwriteBytes( &podnew->states, i, Dstate, od->rho, SnP_stateSizeInBytes - od->rho );
    }

    podnew->rho = od->rho;
    podnew->c   = od->c;
    podnew->o   = od->rho;
    return 0;
}

int ParallelOD_Duplexing( ParallelODInstance *pod, unsigned char *output, unsigned int outputLen, size_t outOffset, const unsigned char *input, unsigned int inputLen, size_t inOffset, unsigned int E, const unsigned char *toAdd )
{
    if ((E < 1u) || (E > 63u)
        || (pod->rho == 0u)
        || (inputLen > pod->rho)
        || (outputLen > pod->rho))
        return 1;

    unsigned int laneCount = inputLen / laneSize;
    unsigned int laneOffset = inOffset / laneSize;
    unsigned char E0 = OD_Concat( E, 0u );
    unsigned char E1 = OD_Concat( E, 1u );

    if (inputLen == pod->rho) {
        if ((inputLen % laneSize == 0) && (inOffset % laneSize == 0)) {
            PlSnP_OverwriteLanesAll( &pod->states, input, inputLen / laneSize, inOffset / laneSize );
        }
        else {
            for(unsigned int i = 0; i < parallelism; i++) {
                PlSnP_OverwriteBytes( &pod->states, i, input+i*inOffset, 0u, pod->rho );
            }
        }
        ParallelOD_TrailencAndXor( pod, E1 );
    }
    else {
        for(unsigned int i = 0; i < parallelism; i++) {
            PlSnP_OverwriteWithZeroes( &pod->states, i, pod->rho );
            PlSnP_OverwriteBytes( &pod->states, i, input+i*inOffset, 0u, inputLen );
            PlSnP_AddByte( &pod->states, i, 1u, inputLen );
        }
        ParallelOD_TrailencAndXor( pod, E0 );
    }
    PlSnP_PermuteAll( &pod->states );
    pod->o = 0;
    return ParallelOD_Squeezing( pod, output, outputLen, outOffset, toAdd );
}

int ParallelOD_Squeezing( ParallelODInstance *pod, unsigned char *output, unsigned int outputLen, size_t outOffset, const unsigned char *toAdd )
{
    if ((pod->rho == 0u)
        || (outputLen + pod->o > pod->rho))
        return 1;

    unsigned int laneCount = outputLen / laneSize;
    unsigned int laneOffset = outOffset / laneSize;

    if (outputLen > 0) {
        if ((pod->o == 0) && (outputLen % laneSize == 0) && (outOffset % laneSize == 0)) {
            if (toAdd != NULL)
                PlSnP_ExtractAndAddLanesAll(&pod->states, toAdd, output, outputLen / laneSize, outOffset / laneSize);
            else
                PlSnP_ExtractLanesAll(&pod->states, output, outputLen / laneSize, outOffset / laneSize);
        }
        else {
            if (toAdd != NULL) {
                for (unsigned int i = 0; i < parallelism; i++) {
                    PlSnP_ExtractAndAddBytes( &pod->states, i, toAdd, output, pod->o, outputLen );
                    toAdd += outOffset;
                    output += outOffset;
                }
            }
            else {
                for (unsigned int i = 0; i < parallelism; i++) {
                    PlSnP_ExtractBytes( &pod->states, i, output, pod->o, outputLen );
                    output += outOffset;
                }
            }
        }
    }

    pod->o += outputLen;
    return 0;
}

size_t ParallelOD_DuplexingFastOnlyIn( ParallelODInstance *pod, const unsigned char *input, size_t inputLen, size_t inOffset, unsigned int E )
{
    if ((E < 1u) || (E > 63u))
        return 0;

    ALIGN(8) const unsigned char trailenc[8] = { OD_Concat( E, 1u ), 0u, 0u, 0u, 0u, 0u, 0u, 0x80u + DOM_SEP };
    pod->o = 0;

    unsigned int r = pod->rho;
    unsigned int laneOffset = inOffset / laneSize;
    unsigned int laneCount = r / laneSize;
    const unsigned char* currentData = input;
    size_t initialLen = inputLen;
    while(inputLen >= r) {
        PlSnP_OverwriteLanesAll( &pod->states, currentData, laneCount, laneOffset );
        for (unsigned int i = 0; i < parallelism; i++) {
            PlSnP_AddBytes( &pod->states, i, trailenc, r, sizeof(trailenc) );
        }
        PlSnP_PermuteAll( &pod->states );
        currentData += r;
        inputLen -= r;
    }

    return initialLen - inputLen; // bytes processed
}

#undef SerialODInstance
#undef SnP_ExtractBytes
#undef ParallelODInstance
#undef ParallelOD_TrailencAndXor
#undef ParallelOD_Initialize
#undef ParallelOD_Clone
#undef ParallelOD_CloneCompact
#undef ParallelOD_CloneCompactFromOD
#undef ParallelOD_Duplexing
#undef ParallelOD_Squeezing
#undef ParallelOD_DuplexingFastOnlyIn
