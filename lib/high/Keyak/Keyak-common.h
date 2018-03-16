/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeyakCommon_h_
#define _KeyakCommon_h_

#include <string.h>
#ifdef OUTPUT
#include <stdio.h>
#endif
#include "align.h"

#define Capacity                        (256/8)
#define TagLength                       (128/8)

#define Pistons_Phase_Fresh         0x00
#define Pistons_Phase_Running       0x01
#define Pistons_Phase_Full          0x02
#define Pistons_Phase_Done          0x04

#define Engine_Phase_Fresh          0x00
#define Engine_Phase_Crypting       0x01
#define Engine_Phase_Crypted        0x02
#define Engine_Phase_InjectOnly     0x04
#define Engine_Phase_EndOfMessage   0x08

#define Motorist_Phase_Ready        0x01
#define Motorist_Phase_Riding       0x02
#define Motorist_Phase_Failed       0x04

#define Motorist_Wrap_LastCryptData     1
#define Motorist_Wrap_LastMetaData      2
#define Motorist_Wrap_LastCryptAndMeta  (Motorist_Wrap_LastCryptData|Motorist_Wrap_LastMetaData)

/* ------------------------------------------------------------------------ */

#define Atom_Error      (-1)
#define Atom_Success    0

#define Atom_False      0
#define Atom_True       1

#ifdef OUTPUT

#define KCP_DeclarePistonsStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_PistonsInstanceStruct { \
        unsigned char state[size]; \
        unsigned char indexCrypt;   /* indexes only used by parallelized instances */ \
        unsigned char indexInject; \
        unsigned char offsetCrypt; \
        unsigned char offsetInject; \
        unsigned char phaseCrypt; \
        unsigned char phaseInject; \
        ALIGN(alignment) unsigned char stateShadow[size]; \
        FILE * file; \
    } prefix##_Pistons_Instance;

#else

#define KCP_DeclarePistonsStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_PistonsInstanceStruct { \
        unsigned char state[size]; \
        unsigned char indexCrypt;   /* indexes only used by parallelized instances */ \
        unsigned char indexInject; \
        unsigned char offsetCrypt; \
        unsigned char offsetInject; \
        unsigned char phaseCrypt; \
        unsigned char phaseInject; \
    } prefix##_Pistons_Instance;

#endif


#define KCP_DeclareEngineStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_EngineInstanceStruct { \
        prefix##_Pistons_Instance pistons; \
        unsigned char phase; \
        unsigned char tagEndIndex; \
        unsigned char tagEndIndexNext;  /* only used by parallelized instances */ \
    } prefix##_Engine_Instance;

#define KCP_DeclareMotoristStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_MotoristInstanceStruct { \
        prefix##_Engine_Instance engine; \
        unsigned char phase; \
        unsigned char lastFlag; \
    } prefix##_Motorist_Instance;

#define KCP_DeclareMotoristFunctions(prefix) \
    int prefix##_Motorist_Initialize(prefix##_Motorist_Instance *instance); \
    int prefix##_Motorist_StartEngine(prefix##_Motorist_Instance *instance, const unsigned char * SUV, size_t SUVlen, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag); \
    int prefix##_Motorist_Wrap(prefix##_Motorist_Instance *instance, const unsigned char *input, size_t dataSizeInBytes, unsigned char *output, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag, int lastFlags, size_t *processedIlen, size_t *processedAlen);

/* ------------------------------------------------------------------------ */

#define KCP_DeclareKeyakStructure(prefix, prefixMotorist, alignment) \
    ALIGN(alignment) typedef struct prefix##KeyakInstanceStruct { \
        prefixMotorist##_Motorist_Instance motorist; \
    } prefix##Keyak_Instance;

#define KCP_DeclareKeyakFunctions(prefix) \
    int prefix##Keyak_Initialize(prefix##Keyak_Instance *instance, const unsigned char *key, unsigned int keySizeInBytes, const unsigned char *nonce, unsigned int nonceSizeInBytes, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag); \
    int prefix##Keyak_Wrap(prefix##Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag ); \
    int prefix##Keyak_WrapPartial(prefix##Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag, int lastFlags, size_t *processedIlen, size_t *processedAlen);

#endif
