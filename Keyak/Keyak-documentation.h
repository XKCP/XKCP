/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

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
