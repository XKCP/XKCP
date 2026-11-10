/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

“Shaking up authenticated encryption”: Keccak-based duplex ciphers, deck ciphers and authenticated encryption schemes designed by Joan Daemen, Seth Hoffert, Silvia Mella, Gilles Van Assche and Ronny Van Keer

Implementation by Ronny Van Keer and Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakOD.h"
#include <assert.h>
#include <stdlib.h>

#if defined(DEBUG_DUMP)
#include <stdio.h>
static void DUMP( const unsigned char * pText, const unsigned char * pData, unsigned int size )
{
    unsigned int i;
    printf("%s (%u bytes):", pText, size);
    for(i=0; i<size; i++)
        printf(" %02x", (int)pData[i]);
    printf("\n");
}
#else
#define DUMP( pText, pData, size )
#endif

// OD -------------------------------------------------------------------------

#define MYMIN( a, b )   (((a) < (b)) ? (a) : (b))

static unsigned int OD_Concat( unsigned int E, unsigned int bt )
{
    assert( E >= 1u );
    assert( E <= 63u );
    assert( bt <= 1u );

    unsigned int top = 1u << 7u;
    while ( E < top ) {
        top >>= 1u;
    }
    return E + (top << bt);
}

#ifdef XKCP_has_KeccakP1600
    #define SnP_GetFeatures                 KeccakP1600_GetFeatures
    #define SnP_StaticInitialize            KeccakP1600_StaticInitialize
    #define SnP_Initialize                  KeccakP1600_Initialize
    #define SnP_AddByte                     KeccakP1600_AddByte
    #define SnP_AddBytes                    KeccakP1600_AddBytes
    #define SnP_OverwriteBytes              KeccakP1600_OverwriteBytes
    #define SnP_OverwriteWithZeroes         KeccakP1600_OverwriteWithZeroes
    #define SnP_ExtractBytes                KeccakP1600_ExtractBytes
    #define SnP_ExtractAndAddBytes          KeccakP1600_ExtractAndAddBytes

    #define SnP_Permute                     KeccakP1600_Permute_12rounds
    #define SnP_ODDuplexingFastInOut        KeccakP1600_12rounds_ODDuplexingFastInOut
    #define SnP_ODDuplexingFastOut          KeccakP1600_12rounds_ODDuplexingFastOut
    #define SnP_ODDuplexingFastIn           KeccakP1600_12rounds_ODDuplexingFastIn
    #define rounds                          rounds12
    #define prefix                          TurboSHAKE
    #define DOM_SEP                         0x00u
        #include "KeccakOD.inc"
    #undef SnP_Permute
    #undef SnP_ODDuplexingFastInOut
    #undef SnP_ODDuplexingFastOut
    #undef SnP_ODDuplexingFastIn
    #undef rounds
    #undef prefix
    #undef DOM_SEP

    #define SnP_Permute                     KeccakP1600_Permute_24rounds
    #define SnP_ODDuplexingFastInOut        KeccakP1600_ODDuplexingFastInOut
    #define SnP_ODDuplexingFastOut          KeccakP1600_ODDuplexingFastOut
    #define SnP_ODDuplexingFastIn           KeccakP1600_ODDuplexingFastIn
    #define rounds                          rounds24
    #define prefix                          SHAKE
    #define DOM_SEP                         0x1Fu
        #include "KeccakOD.inc"
    #undef SnP_Permute
    #undef SnP_ODDuplexingFastInOut
    #undef SnP_ODDuplexingFastOut
    #undef SnP_ODDuplexingFastIn
    #undef rounds
    #undef prefix
    #undef DOM_SEP

    #undef SnP_GetFeatures
    #undef SnP_StaticInitialize
    #undef SnP_Initialize
    #undef SnP_AddByte
    #undef SnP_AddBytes
    #undef SnP_OverwriteBytes
    #undef SnP_OverwriteWithZeroes
    #undef SnP_ExtractBytes
    #undef SnP_ExtractAndAddBytes
#endif

#ifdef XKCP_has_KeccakP1600
int TurboSHAKE128_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKE_OD_Initialize(od, 160, 256, key, keyByteLen);
}

int TurboSHAKE256_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKE_OD_Initialize(od, 128, 512, key, keyByteLen);
}

int SHAKE128_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKE_OD_Initialize(od, 160, 256, key, keyByteLen);
}

int SHAKE256_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKE_OD_Initialize(od, 128, 512, key, keyByteLen);
}
#endif

#ifdef XKCP_has_KeccakP1600times2
    #define parallelism                     2
    #define inst                            KeccakWidth1600
    #define SnP                             KeccakP1600
    #define SnP_stateSizeInBytes            200
    #define pinst                           KeccakWidth1600Times2
    #define PlSnP_GetFeatures               KeccakP1600times2_GetFeatures
    #define PlSnP_StaticInitialize          KeccakP1600times2_StaticInitialize
    #define PlSnP_InitializeAll             KeccakP1600times2_InitializeAll
    #define PlSnP_AddByte                   KeccakP1600times2_AddByte
    #define PlSnP_AddBytes                  KeccakP1600times2_AddBytes
    #define PlSnP_AddLanesAll               KeccakP1600times2_AddLanesAll
    #define PlSnP_OverwriteBytes            KeccakP1600times2_OverwriteBytes
    #define PlSnP_OverwriteLanesAll         KeccakP1600times2_OverwriteLanesAll
    #define PlSnP_OverwriteWithZeroes       KeccakP1600times2_OverwriteWithZeroes
    #define PlSnP_ExtractBytes              KeccakP1600times2_ExtractBytes
    #define PlSnP_ExtractLanesAll           KeccakP1600times2_ExtractLanesAll
    #define PlSnP_ExtractAndAddBytes        KeccakP1600times2_ExtractAndAddBytes
    #define PlSnP_ExtractAndAddLanesAll     KeccakP1600times2_ExtractAndAddLanesAll

    #define PlSnP_PermuteAll                KeccakP1600times2_PermuteAll_12rounds
    #define PlSnP_ODDuplexingFastIn         KeccakP1600times2_12rounds_ODDuplexingFastIn
    #define rounds                          rounds12
    #define prefix                          TurboSHAKE
    #define pprefix                         TurboSHAKETimes2
    #define DOM_SEP                         0x00u
        #include "KeccakParallelOD.inc"
    #undef PlSnP_PermuteAll
    #undef PlSnP_ODDuplexingFastIn
    #undef rounds
    #undef prefix
    #undef pprefix
    #undef DOM_SEP

    #define PlSnP_PermuteAll                KeccakP1600times2_PermuteAll_24rounds
    #define PlSnP_ODDuplexingFastIn         KeccakP1600times2_ODDuplexingFastIn
    #define rounds                          rounds24
    #define prefix                          SHAKE
    #define pprefix                         SHAKETimes2
    #define DOM_SEP                         0x1Fu
        #include "KeccakParallelOD.inc"
    #undef PlSnP_PermuteAll
    #undef rounds
    #undef prefix
    #undef pprefix
    #undef DOM_SEP

    #undef parallelism
    #undef inst
    #undef SnP
    #undef SnP_stateSizeInBytes
    #undef pinst
    #undef PlSnP_GetFeatures
    #undef PlSnP_StaticInitialize
    #undef PlSnP_InitializeAll
    #undef PlSnP_AddByte
    #undef PlSnP_AddBytes
    #undef PlSnP_AddLanesAll
    #undef PlSnP_OverwriteBytes
    #undef PlSnP_OverwriteLanesAll
    #undef PlSnP_OverwriteWithZeroes
    #undef PlSnP_ExtractBytes
    #undef PlSnP_ExtractLanesAll
    #undef PlSnP_ExtractAndAddBytes
    #undef PlSnP_ExtractAndAddLanesAll
    #undef PlSnP_ODDuplexingFastIn
#endif

#ifdef XKCP_has_KeccakP1600times4
    #define parallelism                     4
    #define inst                            KeccakWidth1600
    #define SnP                             KeccakP1600
    #define SnP_stateSizeInBytes            200
    #define pinst                           KeccakWidth1600Times4
    #define PlSnP_GetFeatures               KeccakP1600times4_GetFeatures
    #define PlSnP_StaticInitialize          KeccakP1600times4_StaticInitialize
    #define PlSnP_InitializeAll             KeccakP1600times4_InitializeAll
    #define PlSnP_AddByte                   KeccakP1600times4_AddByte
    #define PlSnP_AddBytes                  KeccakP1600times4_AddBytes
    #define PlSnP_AddLanesAll               KeccakP1600times4_AddLanesAll
    #define PlSnP_OverwriteBytes            KeccakP1600times4_OverwriteBytes
    #define PlSnP_OverwriteLanesAll         KeccakP1600times4_OverwriteLanesAll
    #define PlSnP_OverwriteWithZeroes       KeccakP1600times4_OverwriteWithZeroes
    #define PlSnP_ExtractBytes              KeccakP1600times4_ExtractBytes
    #define PlSnP_ExtractLanesAll           KeccakP1600times4_ExtractLanesAll
    #define PlSnP_ExtractAndAddBytes        KeccakP1600times4_ExtractAndAddBytes
    #define PlSnP_ExtractAndAddLanesAll     KeccakP1600times4_ExtractAndAddLanesAll

    #define PlSnP_PermuteAll                KeccakP1600times4_PermuteAll_12rounds
    #define PlSnP_ODDuplexingFastIn         KeccakP1600times4_12rounds_ODDuplexingFastIn
    #define rounds                          rounds12
    #define prefix                          TurboSHAKE
    #define pprefix                         TurboSHAKETimes4
    #define DOM_SEP                         0x00u
        #include "KeccakParallelOD.inc"
    #undef PlSnP_PermuteAll
    #undef PlSnP_ODDuplexingFastIn
    #undef rounds
    #undef prefix
    #undef pprefix
    #undef DOM_SEP

    #define PlSnP_PermuteAll                KeccakP1600times4_PermuteAll_24rounds
    #define PlSnP_ODDuplexingFastIn         KeccakP1600times4_ODDuplexingFastIn
    #define rounds                          rounds24
    #define prefix                          SHAKE
    #define pprefix                         SHAKETimes4
    #define DOM_SEP                         0x1Fu
        #include "KeccakParallelOD.inc"
    #undef PlSnP_PermuteAll
    #undef rounds
    #undef prefix
    #undef pprefix
    #undef DOM_SEP

    #undef parallelism
    #undef inst
    #undef SnP
    #undef SnP_stateSizeInBytes
    #undef pinst
    #undef PlSnP_GetFeatures
    #undef PlSnP_StaticInitialize
    #undef PlSnP_InitializeAll
    #undef PlSnP_AddByte
    #undef PlSnP_AddBytes
    #undef PlSnP_AddLanesAll
    #undef PlSnP_OverwriteBytes
    #undef PlSnP_OverwriteLanesAll
    #undef PlSnP_OverwriteWithZeroes
    #undef PlSnP_ExtractBytes
    #undef PlSnP_ExtractLanesAll
    #undef PlSnP_ExtractAndAddBytes
    #undef PlSnP_ExtractAndAddLanesAll
    #undef PlSnP_ODDuplexingFastIn
#endif

#ifdef XKCP_has_KeccakP1600times8
    #define parallelism                     8
    #define inst                            KeccakWidth1600
    #define SnP                             KeccakP1600
    #define SnP_stateSizeInBytes            200
    #define pinst                           KeccakWidth1600Times8
    #define PlSnP_GetFeatures               KeccakP1600times8_GetFeatures
    #define PlSnP_StaticInitialize          KeccakP1600times8_StaticInitialize
    #define PlSnP_InitializeAll             KeccakP1600times8_InitializeAll
    #define PlSnP_AddByte                   KeccakP1600times8_AddByte
    #define PlSnP_AddBytes                  KeccakP1600times8_AddBytes
    #define PlSnP_AddLanesAll               KeccakP1600times8_AddLanesAll
    #define PlSnP_OverwriteBytes            KeccakP1600times8_OverwriteBytes
    #define PlSnP_OverwriteLanesAll         KeccakP1600times8_OverwriteLanesAll
    #define PlSnP_OverwriteWithZeroes       KeccakP1600times8_OverwriteWithZeroes
    #define PlSnP_ExtractBytes              KeccakP1600times8_ExtractBytes
    #define PlSnP_ExtractLanesAll           KeccakP1600times8_ExtractLanesAll
    #define PlSnP_ExtractAndAddBytes        KeccakP1600times8_ExtractAndAddBytes
    #define PlSnP_ExtractAndAddLanesAll     KeccakP1600times8_ExtractAndAddLanesAll

    #define PlSnP_PermuteAll                KeccakP1600times8_PermuteAll_12rounds
    #define PlSnP_ODDuplexingFastIn         KeccakP1600times8_12rounds_ODDuplexingFastIn
    #define rounds                          rounds12
    #define prefix                          TurboSHAKE
    #define pprefix                         TurboSHAKETimes8
    #define DOM_SEP                         0x00u
        #include "KeccakParallelOD.inc"
    #undef PlSnP_PermuteAll
    #undef PlSnP_ODDuplexingFastIn
    #undef rounds
    #undef prefix
    #undef pprefix
    #undef DOM_SEP

    #define PlSnP_PermuteAll                KeccakP1600times8_PermuteAll_24rounds
    #define PlSnP_ODDuplexingFastIn         KeccakP1600times8_ODDuplexingFastIn
    #define rounds                          rounds24
    #define prefix                          SHAKE
    #define pprefix                         SHAKETimes8
    #define DOM_SEP                         0x1Fu
        #include "KeccakParallelOD.inc"
    #undef PlSnP_PermuteAll
    #undef rounds
    #undef prefix
    #undef pprefix
    #undef DOM_SEP

    #undef parallelism
    #undef inst
    #undef SnP
    #undef SnP_stateSizeInBytes
    #undef pinst
    #undef PlSnP_GetFeatures
    #undef PlSnP_StaticInitialize
    #undef PlSnP_InitializeAll
    #undef PlSnP_AddByte
    #undef PlSnP_AddBytes
    #undef PlSnP_AddLanesAll
    #undef PlSnP_OverwriteBytes
    #undef PlSnP_OverwriteLanesAll
    #undef PlSnP_OverwriteWithZeroes
    #undef PlSnP_ExtractBytes
    #undef PlSnP_ExtractLanesAll
    #undef PlSnP_ExtractAndAddBytes
    #undef PlSnP_ExtractAndAddLanesAll
    #undef PlSnP_ODDuplexingFastIn
#endif

#ifdef XKCP_has_KeccakP1600times2
int TurboSHAKE128Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKETimes2_ParallelOD_Initialize(pod, 160, 256, key, keyByteLen);
}

int TurboSHAKE256Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKETimes2_ParallelOD_Initialize(pod, 128, 512, key, keyByteLen);
}

int SHAKE128Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKETimes2_ParallelOD_Initialize(pod, 160, 256, key, keyByteLen);
}

int SHAKE256Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKETimes2_ParallelOD_Initialize(pod, 128, 512, key, keyByteLen);
}
#endif

#ifdef XKCP_has_KeccakP1600times4
int TurboSHAKE128Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKETimes4_ParallelOD_Initialize(pod, 160, 256, key, keyByteLen);
}

int TurboSHAKE256Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKETimes4_ParallelOD_Initialize(pod, 128, 512, key, keyByteLen);
}

int SHAKE128Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKETimes4_ParallelOD_Initialize(pod, 160, 256, key, keyByteLen);
}

int SHAKE256Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKETimes4_ParallelOD_Initialize(pod, 128, 512, key, keyByteLen);
}
#endif

#ifdef XKCP_has_KeccakP1600times8
int TurboSHAKE128Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKETimes8_ParallelOD_Initialize(pod, 160, 256, key, keyByteLen);
}

int TurboSHAKE256Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return TurboSHAKETimes8_ParallelOD_Initialize(pod, 128, 512, key, keyByteLen);
}

int SHAKE128Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKETimes8_ParallelOD_Initialize(pod, 160, 256, key, keyByteLen);
}

int SHAKE256Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen)
{
    return SHAKETimes8_ParallelOD_Initialize(pod, 128, 512, key, keyByteLen);
}
#endif
