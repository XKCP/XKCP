/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Ketje, designed by Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Ketjev2_h_
#define _Ketjev2_h_

/* For the documentation, please follow the link: */
/* #include "Ketje-documentation.h" */

#include <string.h>
#include "align.h"
#include "config.h"

/** The phase is a data element that expresses what Ketje is doing
 * - virgin: the only operation supported is initialization, loading the key and nonce. This will switch
 *   the phase to feedingAssociatedData
 * - feedingAssociatedData: Ketje is ready for feeding associated data, has started feeding associated data
 *   or has finished feeding associated data. It allows feeding some more associated data in which case the phase does not
 *   change. One can also start wrapping plaintext, that sets the phase to wrapping. Finally, one can
 *   start unwrapping ciphertext, that sets the phase to unwrapping.
 * - wrapping: Ketje is ready for wrapping some more plaintext or for delivering the tag.
 *   Wrapping more plaintext does not modify the phase, asking for the tag sets the phase to feedingAssociatedData.
 * - unwrapping: Ketje is ready for unwrapping some more ciphertext or for delivering the tag.
 *   Unwrapping more ciphertext does not modify the phase, asking for the tag sets the phase to feedingAssociatedData.
 */
enum Phase {
    Ketje_Phase_Virgin          = 0,
    Ketje_Phase_FeedingAssociatedData   = 1,
    Ketje_Phase_Wrapping        = 2,
    Ketje_Phase_Unwrapping      = 4
};

#define XKCP_DeclareKetjeStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##InstanceStruct { \
        unsigned char state[size]; \
        unsigned int phase; \
        unsigned int dataRemainderSize; \
    } prefix##_Instance;

#define XKCP_DeclareKetjeFunctions(prefix) \
    int prefix##_Initialize(prefix##_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce, unsigned int nonceSizeInBits); \
    int prefix##_FeedAssociatedData(prefix##_Instance *instance, const unsigned char *data, unsigned int dataSizeInBytes); \
    int prefix##_WrapPlaintext(prefix##_Instance *instance, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int dataSizeInBytes); \
    int prefix##_UnwrapCiphertext(prefix##_Instance *instance, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int dataSizeInBytes); \
    int prefix##_GetTag(prefix##_Instance *instance, unsigned char *tag, unsigned int tagSizeInBytes);

#ifdef XKCP_has_KeccakP200
    #include "KeccakP-200-SnP.h"
    XKCP_DeclareKetjeStructure(KetjeJr, KeccakP200_stateSizeInBytes, KeccakP200_stateAlignment)
    XKCP_DeclareKetjeFunctions(KetjeJr)
    #define XKCP_has_KetjeJr
#endif

#ifdef XKCP_has_KeccakP400
    #include "KeccakP-400-SnP.h"
    XKCP_DeclareKetjeStructure(KetjeSr, KeccakP400_stateSizeInBytes, KeccakP400_stateAlignment)
    XKCP_DeclareKetjeFunctions(KetjeSr)
    #define XKCP_has_KetjeSr
#endif

#ifdef XKCP_has_KeccakP800
    #include "KeccakP-800-SnP.h"
    XKCP_DeclareKetjeStructure(KetjeMn, KeccakP800_stateSizeInBytes, KeccakP800_stateAlignment)
    XKCP_DeclareKetjeFunctions(KetjeMn)
    #define XKCP_has_KetjeMn
#endif

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"
    XKCP_DeclareKetjeStructure(KetjeMj, KeccakP1600_stateSizeInBytes, KeccakP1600_stateAlignment)
    XKCP_DeclareKetjeFunctions(KetjeMj)
    #define XKCP_has_KetjeMj
#endif

#endif
