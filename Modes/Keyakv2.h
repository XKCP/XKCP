/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _Keyak_h_
#define _Keyak_h_

/** General information
  *
  * The following type and functions are not actually implemented. Their
  * documentation is generic, with the prefix Prefix replaced by
  * - River for River Keyak, using a single Keccak-p[800,12]
  * - Lake for Lake Keyak, using a single Keccak-p[1600,12]
  * - Sea for Sea Keyak, using 2 parallel Keccak-p[1600,12]
  * - Ocean for Ocean Keyak, using 4 parallel Keccak-p[1600,12]
  * - Lunar for Lunar Keyak, using 8 parallel Keccak-p[1600,12]
  *
  * The PrefixKeyak_Instance contains the Keyak instance attributes for use
  * with the PrefixKeyak* functions.
  */

#ifdef DontReallyInclude_DocumentationOnly

/**
  * Structure that contains the Keyak instance for use with the
  * PrefixKeyak functions.
  */
typedef struct {
    /** The underlying motorist instance. */
    PrefixMotorist_Motorist_Instance motorist;
} PrefixKeyak_Instance;

/**
  * Function that initializes, feeds the key and the nonce. The key and nonce consists of sequences of bytes.
  *
  * @param  instance          Pointer to the Keyak instance structure.
  * @param  key               Pointer to the key.
  * @param  keySizeInBytes    The size of the key in bytes.
  * @param  nonce             Pointer to the nonce.
  * @param  nonceSizeInBytes  The size of the nonce in bytes.
  * @param  tagFlag           Flag to generate/check a tag on initialization.
  * @param  tag               Pointer to tag buffer.
  * @param  unwrapFlag        Flag to generate (false) or check (true) the tag, this argument is ignored if tagFlag is false.
  * @param  forgetFlag        Flag to apply the forget operation (true).
  *
  * @pre    phase == *
  * @pre    keySizeInBytes ≤ (::PrefixKeyak_Lk - 2)
  * @pre    nonceSizeInBytes ≤ ::PrefixKeyak_MaxNoncelength
  *
  * @post   (phase == motorist.riding) or (phase == motorist.failed)
  *
  * @return 1 if successful, 0 if tag mismatch, -1 other error.
  */
int PrefixKeyak_Initialize(PrefixKeyak_Instance *instance, 
    const unsigned char *key, unsigned int keySizeInBytes, 
    const unsigned char *nonce, unsigned int nonceSizeInBytes,
    int tagFlag, unsigned char * tag, 
    int unwrapFlag, int forgetFlag);

/**
  * Function that presents full input data that consists of a
  * sequence of bytes for (un)wrapping and feeds full associated data 
  * that consists of a sequence of bytes.  
  *
  * @param  instance            Pointer to the Keyak instance structure.
  * @param  input               Pointer to full input data to wrap or unwrap.
  * @param  output              Pointer to buffer where the full (un)wrapped data will be stored.
  *                             This pointer can be equal to the input buffer to save memory,
  *                             but otherwise the two buffers must not overlap.
  * @param  dataSizeInBytes     The size of the input/output data.
  * @param  AD                  Pointer to the full associated data.
  * @param  ADlen               The size of the associated data in bytes.
  * @param  tag                 The buffer where to store the tag when wrapping, or read the tag to check when unwrapping.
  * @param  unwrapFlag          Wrap if false(zero), unwrap if true(non zero).
  * @param  forgetFlag          Forget if true (non zero).
  *
  * @pre    phase == motorist.riding
  *
  * @post   (phase == motorist.riding) or (phase == motorist.failed)
  *
  * @return -1 if error, 0 if tag check failed, 1 success.
  */
int PrefixKeyak_Wrap(PrefixKeyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, 
    const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag );

/**
  * Function that presents partial input data that consists of a
  * sequence of bytes for (un)wrapping and feeds partial associated data 
  * that consists of a sequence of bytes.  
  *
  * @param  instance            Pointer to the Keyak instance structure.
  * @param  input               Pointer to partial input data to wrap or unwrap.
  * @param  output              Pointer to buffer where the partial (un)wrapped data will be stored.
  *                             This pointer can be equal to the input buffer to save memory,
  *                             but otherwise the two buffers must not overlap.
  * @param  dataSizeInBytes     The size of the partial input/output data.
  * @param  AD                  Pointer to the partial associated data.
  * @param  ADlen               The size of the partial associated data in bytes.
  * @param  tag                 The buffer where to store the tag when wrapping, or read the tag to check when unwrapping.
  * @param  unwrapFlag          Wrap if false (zero), unwrap if true (non zero).
  * @param  forgetFlag          Forget if true (non zero).
  * @param  lastFlags           One of the following values:
  *                             - 0: Not last partial data for either input or metadata.
  *                             - Motorist_Wrap_LastCryptData: Last partial data for input.
  *                             - Motorist_Wrap_LastMetaData: Last partial data for metadata
  *                             - Motorist_Wrap_LastCryptAndMeta: Last partial data for both input and metadata
  *                             During a session this argument value must not logically decrease.
  * @param  processedIlen       Pointer to a variable where the number of processed input/output bytes will be stored.
  *                             The returned value will be less or equal to argDataLen.
  * @param  processedADlen      Pointer to a variable where the number of processed associated data bytes will be stored.
  *                             The returned value will be less or equal to argADlen.
  *
  * @pre    phase == motorist.riding
  *
  * @post   (phase == motorist.riding) or (phase == motorist.failed)
  *
  * @return -1 if error, 0 if tag check failed, 1 success.
  */
int PrefixKeyak_WrapPartial( PrefixKeyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes,
    const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag,
    int lastFlags, size_t *processedIlen, size_t *processedAlen);

#endif

#include <string.h>
#include "align.h"
#include "Motorist.h"

/** Length of the River Keyak key pack. */
#define RiverKeyak_Lk                  36

/** Maximum nonce length for River Keyak. */
#define RiverKeyak_MaxNoncelength      58

/** Length of the Lake Keyak key pack. */
#define LakeKeyak_Lk                   40

/** Maximum nonce length for Lake Keyak. */
#define LakeKeyak_MaxNoncelength       150

/** Length of the Sea Keyak key pack. */
#define SeaKeyak_Lk                    LakeKeyak_Lk

/** Maximum nonce length for Sea Keyak. */
#define SeaKeyak_MaxNoncelength        LakeKeyak_MaxNoncelength

/** Length of the Ocean Keyak key pack. */
#define OceanKeyak_Lk                  LakeKeyak_Lk

/** Maximum nonce length for Ocean Keyak. */
#define OceanKeyak_MaxNoncelength      LakeKeyak_MaxNoncelength

/** Length of the Lunar Keyak key pack. */
#define LunarKeyak_Lk                  LakeKeyak_Lk

/** Maximum nonce length for Lunar Keyak. */
#define LunarKeyak_MaxNoncelength      LakeKeyak_MaxNoncelength


#define KCP_DeclareKeyakStructure(prefix, prefixMotorist, alignment) \
    ALIGN(alignment) typedef struct prefix##KeyakInstanceStruct { \
        prefixMotorist##_Motorist_Instance motorist; \
    } prefix##Keyak_Instance;

#define KCP_DeclareKeyakFunctions(prefix) \
    int prefix##Keyak_Initialize(prefix##Keyak_Instance *instance, const unsigned char *key, unsigned int keySizeInBytes, const unsigned char *nonce, unsigned int nonceSizeInBytes, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag); \
    int prefix##Keyak_Wrap(prefix##Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag ); \
    int prefix##Keyak_WrapPartial(prefix##Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag, int lastFlags, size_t *processedIlen, size_t *processedAlen);

#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"
    KCP_DeclareKeyakStructure(River, KeyakWidth800, KeccakP800_stateAlignment)
    KCP_DeclareKeyakFunctions(River)
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"
    KCP_DeclareKeyakStructure(Lake, KeyakWidth1600, KeccakP1600_stateAlignment)
    KCP_DeclareKeyakFunctions(Lake)
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"
    KCP_DeclareKeyakStructure(Sea, KeyakWidth1600times2, KeccakP1600times2_statesAlignment)
    KCP_DeclareKeyakFunctions(Sea)
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times4-SnP.h"
    KCP_DeclareKeyakStructure(Ocean, KeyakWidth1600times4, KeccakP1600times4_statesAlignment)
    KCP_DeclareKeyakFunctions(Ocean)
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times8-SnP.h"
    KCP_DeclareKeyakStructure(Lunar, KeyakWidth1600times8, KeccakP1600times8_statesAlignment)
    KCP_DeclareKeyakFunctions(Lunar)
#endif

#endif
