/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Xoodyak, designed by Joan Daemen, Seth Hoffert, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Cyclist_h_
#define _Cyclist_h_

#include <stdint.h>
#include "align.h"

#define Cyclist_ModeHash    1
#define Cyclist_ModeKeyed   2

#define Cyclist_PhaseDown   1
#define Cyclist_PhaseUp     2

#ifdef OUTPUT

#include    <stdio.h>

#define KCP_DeclareCyclistStructure(prefix, state_t)                            \
            typedef struct prefix##_CyclistInstanceStruct {                     \
            state_t         state;                                              \
            state_t         stateShadow[size];                                  \
            FILE            *file;                                              \
            unsigned int    phase;                                              \
            unsigned int    mode;                                               \
            unsigned int    Rabsorb;                                            \
            unsigned int    Rsqueeze;                                           \
        } prefix##_Instance;

#else

#define KCP_DeclareCyclistStructure(prefix, state_t)                            \
            typedef struct prefix##_CyclistInstanceStruct {                     \
            state_t         state;                                              \
            unsigned int    phase;                                              \
            unsigned int    mode;                                               \
            unsigned int    Rabsorb;                                            \
            unsigned int    Rsqueeze;                                           \
        } prefix##_Instance;

#endif

#define KCP_DeclareCyclistFunctions(prefix) \
    int prefix##_Initialize(prefix##_Instance *instance, const uint8_t *K, size_t KLen, const uint8_t *ID, size_t IDLen, const uint8_t *counter, size_t counterLen); \
    int prefix##_Absorb(prefix##_Instance *instance, const uint8_t *X, size_t XLen); \
    int prefix##_Encrypt(prefix##_Instance *instance, const uint8_t *P, uint8_t *C, size_t PLen); \
    int prefix##_Decrypt(prefix##_Instance *instance, const uint8_t *C, uint8_t *P, size_t CLen); \
    int prefix##_Squeeze(prefix##_Instance *instance, uint8_t *Y, size_t YLen); \
    int prefix##_SqueezeKey(prefix##_Instance *instance, uint8_t *K, size_t KLen); \
    int prefix##_Ratchet(prefix##_Instance *instance);
#endif
