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

#ifndef _KeccakOD_h_
#define _KeccakOD_h_

#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "config.h"

// Overwrite Duplex (OD) --------------------------------------------------------

#define XKCP_DeclareODStructure(prefix, state_t) \
    typedef struct prefix##_ODInstanceStruct { \
        state_t s; \
        unsigned int rho; \
        unsigned int c; \
        unsigned int o; \
    } prefix##_ODInstance;

#define XKCP_DeclareODFunctions(prefix, inst) \
    int prefix##_OD_Initialize(inst##_ODInstance *od, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen ); \
    int prefix##_OD_Clone(inst##_ODInstance *odnew, const inst##_ODInstance *od ); \
    int prefix##_OD_CloneCompact(inst##_ODInstance *odnew, const inst##_ODInstance *od ); \
    int prefix##_OD_Duplexing(inst##_ODInstance *od, unsigned char *output, unsigned int outputLen, const unsigned char *input, unsigned int inputLen, unsigned int E, const unsigned char *toAdd ); \
    int prefix##_OD_Squeezing(inst##_ODInstance *od, unsigned char *output, unsigned int outputLen, const unsigned char *toAdd ); \
    size_t prefix##_OD_DuplexingFast(inst##_ODInstance *od, const unsigned char *input, size_t len, unsigned int E, unsigned char *output, const unsigned char *toAdd ); \
    size_t prefix##_OD_DuplexingFastOnlyOut(inst##_ODInstance *od, unsigned int E, unsigned char *output, size_t outputLen, const unsigned char *toAdd ); \
    size_t prefix##_OD_DuplexingFastOnlyIn(inst##_ODInstance *od, const unsigned char *input, size_t inputLen, unsigned int E ); \

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"
    XKCP_DeclareODStructure(KeccakWidth1600, KeccakP1600_state)
    XKCP_DeclareODFunctions(SHAKE, KeccakWidth1600)
    XKCP_DeclareODFunctions(TurboSHAKE, KeccakWidth1600)

    int TurboSHAKE128_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE128_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256_OD_Initialize(KeccakWidth1600_ODInstance *od, const unsigned char *key, unsigned int keyByteLen);

    #define XKCP_has_OD_Keccak_width1600
#endif

// Parallel OD --------------------------------------------------------

#define XKCP_DeclareParallelODStructure(pprefix, states_t) \
typedef struct pprefix##_ParallelODInstanceStruct { \
    states_t states; \
    unsigned int rho; \
    unsigned int c; \
    unsigned int o; \
} pprefix##_ParallelODInstance;

#define XKCP_DeclareParallelODFunctions(pprefix, pinst, inst) \
int pprefix##_ParallelOD_Initialize( \
        pinst##_ParallelODInstance *pod, \
        unsigned int rho, \
        unsigned int c, \
        const unsigned char *key, \
        unsigned int keyByteLen \
    ); \
int pprefix##_ParallelOD_Clone( \
        pinst##_ParallelODInstance *podnew, \
        const pinst##_ParallelODInstance *pod \
    ); \
int pprefix##_ParallelOD_CloneCompact( \
        pinst##_ParallelODInstance *podnew, \
        const pinst##_ParallelODInstance *pod \
    ); \
int pprefix##_ParallelOD_CloneCompactFromOD( \
        pinst##_ParallelODInstance *podnew, \
        const inst##_ODInstance *od \
    ); \
int pprefix##_ParallelOD_Duplexing( \
        pinst##_ParallelODInstance *pod, \
        unsigned char *output, \
        unsigned int outputLen, \
        size_t outOffset, \
        const unsigned char *input, \
        unsigned int inputLen, \
        size_t inOffset, \
        unsigned int E, \
        const unsigned char *toAdd \
    ); \
int pprefix##_ParallelOD_Squeezing( \
        pinst##_ParallelODInstance *pod, \
        unsigned char *output, \
        unsigned int outputLen, \
        size_t outOffset, \
        const unsigned char *toAdd \
    ); \
size_t pprefix##_ParallelOD_DuplexingFastOnlyIn( \
        pinst##_ParallelODInstance *pod, \
        const unsigned char *input, \
        size_t inputLen, \
        size_t inOffset, \
        unsigned int E \
    );

#ifdef XKCP_has_KeccakP1600
#include "KeccakP-1600-SnP.h"
#endif

#ifdef XKCP_has_KeccakP1600times2
    #include "KeccakP-1600-times2-SnP.h"
    XKCP_DeclareParallelODStructure(KeccakWidth1600Times2, KeccakP1600times2_states)
    XKCP_DeclareParallelODFunctions(SHAKETimes2, KeccakWidth1600Times2, KeccakWidth1600)
    XKCP_DeclareParallelODFunctions(TurboSHAKETimes2, KeccakWidth1600Times2, KeccakWidth1600)

    int TurboSHAKE128Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE128Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256Times2_ParallelOD_Initialize(KeccakWidth1600Times2_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);

    #define XKCP_has_ParallelOD_Keccak_width1600_times2
#endif

#ifdef XKCP_has_KeccakP1600times4
    #include "KeccakP-1600-times4-SnP.h"
    XKCP_DeclareParallelODStructure(KeccakWidth1600Times4, KeccakP1600times4_states)
    XKCP_DeclareParallelODFunctions(SHAKETimes4, KeccakWidth1600Times4, KeccakWidth1600)
    XKCP_DeclareParallelODFunctions(TurboSHAKETimes4, KeccakWidth1600Times4, KeccakWidth1600)

    int TurboSHAKE128Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE128Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256Times4_ParallelOD_Initialize(KeccakWidth1600Times4_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);

    #define XKCP_has_ParallelOD_Keccak_width1600_times4
#endif

#ifdef XKCP_has_KeccakP1600times8
    #include "KeccakP-1600-times8-SnP.h"
    XKCP_DeclareParallelODStructure(KeccakWidth1600Times8, KeccakP1600times8_states)
    XKCP_DeclareParallelODFunctions(SHAKETimes8, KeccakWidth1600Times8, KeccakWidth1600)
    XKCP_DeclareParallelODFunctions(TurboSHAKETimes8, KeccakWidth1600Times8, KeccakWidth1600)

    int TurboSHAKE128Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE128Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256Times8_ParallelOD_Initialize(KeccakWidth1600Times8_ParallelODInstance *pod, const unsigned char *key, unsigned int keyByteLen);

    #define XKCP_has_ParallelOD_Keccak_width1600_times8
#endif

#endif
