/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Keyak, designed by Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Keyakv2_h_
#define _Keyakv2_h_

#include <string.h>
#include "align.h"
#include "config.h"

/* For the documentation, please follow the link: */
/* #include "Keyak-documentation.h" */

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

#ifdef UT_OUTPUT

#define XKCP_DeclarePistonsStructure(prefix, size, alignment) \
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

#define XKCP_DeclarePistonsStructure(prefix, size, alignment) \
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


#define XKCP_DeclareEngineStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_EngineInstanceStruct { \
        prefix##_Pistons_Instance pistons; \
        unsigned char phase; \
        unsigned char tagEndIndex; \
        unsigned char tagEndIndexNext;  /* only used by parallelized instances */ \
    } prefix##_Engine_Instance;

#define XKCP_DeclareMotoristStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_MotoristInstanceStruct { \
        prefix##_Engine_Instance engine; \
        unsigned char phase; \
        unsigned char lastFlag; \
    } prefix##_Motorist_Instance;

#define XKCP_DeclareMotoristFunctions(prefix) \
    int prefix##_Motorist_Initialize(prefix##_Motorist_Instance *instance); \
    int prefix##_Motorist_StartEngine(prefix##_Motorist_Instance *instance, const unsigned char * SUV, size_t SUVlen, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag); \
    int prefix##_Motorist_Wrap(prefix##_Motorist_Instance *instance, const unsigned char *input, size_t dataSizeInBytes, unsigned char *output, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag, int lastFlags, size_t *processedIlen, size_t *processedAlen);

/* ------------------------------------------------------------------------ */

#define XKCP_DeclareKeyakStructure(prefix, prefixMotorist, alignment) \
    ALIGN(alignment) typedef struct prefix##KeyakInstanceStruct { \
        prefixMotorist##_Motorist_Instance motorist; \
    } prefix##Keyak_Instance;

#define XKCP_DeclareKeyakFunctions(prefix) \
    int prefix##Keyak_Initialize(prefix##Keyak_Instance *instance, const unsigned char *key, unsigned int keySizeInBytes, const unsigned char *nonce, unsigned int nonceSizeInBytes, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag); \
    int prefix##Keyak_Wrap(prefix##Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag ); \
    int prefix##Keyak_WrapPartial(prefix##Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag, int lastFlags, size_t *processedIlen, size_t *processedAlen);

/* ------------------------------------------------------------------------ */

/** Length of the River Keyak key pack. */
#define RiverKeyak_Lk                  36

/** Maximum nonce length for River Keyak. */
#define RiverKeyak_MaxNoncelength      58

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"
    XKCP_DeclarePistonsStructure(KeyakWidth800, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    XKCP_DeclareEngineStructure(KeyakWidth800, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    XKCP_DeclareMotoristStructure(KeyakWidth800, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    XKCP_DeclareMotoristFunctions(KeyakWidth800)
    XKCP_DeclareKeyakStructure(River, KeyakWidth800, KeccakP800_stateAlignment)
    XKCP_DeclareKeyakFunctions(River)
    #define XKCP_has_RiverKeyak
#endif

/* ------------------------------------------------------------------------ */

/** Length of the Lake Keyak key pack. */
#define LakeKeyak_Lk                   40

/** Maximum nonce length for Lake Keyak. */
#define LakeKeyak_MaxNoncelength       150

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"
    XKCP_DeclarePistonsStructure(KeyakWidth1600, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    XKCP_DeclareEngineStructure(KeyakWidth1600, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    XKCP_DeclareMotoristStructure(KeyakWidth1600, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    XKCP_DeclareMotoristFunctions(KeyakWidth1600)
    XKCP_DeclareKeyakStructure(Lake, KeyakWidth1600, KeccakP1600_stateAlignment)
    XKCP_DeclareKeyakFunctions(Lake)
    #define XKCP_has_LakeKeyak
#endif

/* ------------------------------------------------------------------------ */

/** Length of the Sea Keyak key pack. */
#define SeaKeyak_Lk                    40

/** Maximum nonce length for Sea Keyak. */
#define SeaKeyak_MaxNoncelength        150

#ifdef XKCP_has_KeccakP1600times2
    #include "KeccakP-1600-times2-SnP.h"
    XKCP_DeclarePistonsStructure(KeyakWidth1600times2, KeccakP1600times2_statesSizeInBytes, KeccakP1600times2_statesAlignment)
    XKCP_DeclareEngineStructure(KeyakWidth1600times2, KeccakP1600times2_statesSizeInBytes, KeccakP1600times2_statesAlignment)
    XKCP_DeclareMotoristStructure(KeyakWidth1600times2, KeccakP1600times2_statesSizeInBytes, KeccakP1600times2_statesAlignment)
    XKCP_DeclareMotoristFunctions(KeyakWidth1600times2)
    XKCP_DeclareKeyakStructure(Sea, KeyakWidth1600times2, KeccakP1600times2_statesAlignment)
    XKCP_DeclareKeyakFunctions(Sea)
    #define XKCP_has_SeaKeyak
#endif

/* ------------------------------------------------------------------------ */

/** Length of the Ocean Keyak key pack. */
#define OceanKeyak_Lk                  40

/** Maximum nonce length for Ocean Keyak. */
#define OceanKeyak_MaxNoncelength      150

#ifdef XKCP_has_KeccakP1600times4
    #include "KeccakP-1600-times4-SnP.h"
    XKCP_DeclarePistonsStructure(KeyakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    XKCP_DeclareEngineStructure(KeyakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    XKCP_DeclareMotoristStructure(KeyakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    XKCP_DeclareMotoristFunctions(KeyakWidth1600times4)
    XKCP_DeclareKeyakStructure(Ocean, KeyakWidth1600times4, KeccakP1600times4_statesAlignment)
    XKCP_DeclareKeyakFunctions(Ocean)
    #define XKCP_has_OceanKeyak
#endif

/* ------------------------------------------------------------------------ */

/** Length of the Lunar Keyak key pack. */
#define LunarKeyak_Lk                  40

/** Maximum nonce length for Lunar Keyak. */
#define LunarKeyak_MaxNoncelength      150

#ifdef XKCP_has_KeccakP1600times8
    #include "KeccakP-1600-times8-SnP.h"
    XKCP_DeclarePistonsStructure(KeyakWidth1600times8, KeccakP1600times8_statesSizeInBytes, KeccakP1600times8_statesAlignment)
    XKCP_DeclareEngineStructure(KeyakWidth1600times8, KeccakP1600times8_statesSizeInBytes, KeccakP1600times8_statesAlignment)
    XKCP_DeclareMotoristStructure(KeyakWidth1600times8, KeccakP1600times8_statesSizeInBytes, KeccakP1600times8_statesAlignment)
    XKCP_DeclareMotoristFunctions(KeyakWidth1600times8)
    XKCP_DeclareKeyakStructure(Lunar, KeyakWidth1600times8, KeccakP1600times8_statesAlignment)
    XKCP_DeclareKeyakFunctions(Lunar)
    #define XKCP_has_LunarKeyak
#endif

/* ------------------------------------------------------------------------ */

#endif
