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
  * - KetjeJr for Ketje Jr, using Keccak-p[200]
  * - KetjeSr for Ketje Sr, using Keccak-p[400]
  * - KetjeMn for Ketje Minor, using Keccak-p[800]
  * - KetjeMj for Ketje Major, using Keccak-p[1600]
  *
  * The Prefix_Instance contains the Keyak instance attributes for use
  * with the Prefix* functions.
  */

#ifdef DontReallyInclude_DocumentationOnly

/**
  * Structure that contains the Ketje instance for use with the
  * Prefix* functions.
  */
ALIGN typedef struct {
    /** The state processed by the permutation. */
    unsigned char state[KeccakF_stateSizeInBytes];

    /** The phase. */
    unsigned int phase;

    /** The amount of associated or plaintext data that has been
      * XORed into the state after the last call to step or stride */
    unsigned int dataRemainderSize;
} Prefix_Instance;

/**
  * Function that feeds the key and the nonce. Both are bit strings and consist of a sequence of bytes.
  *
  * @param  instance        Pointer to the Ketje instance structure.
  * @param  key             Pointer to the key.
  * @param  keySizeInBits   The size of the key in bits.
  * @param  nonce           Pointer to the nonce.
  * @param  nonceSizeInBits The size of the nonce in bits.
  *
  * @pre    phase == *
  * @pre    8*((keySizeInBits+16)/8) + nonceSizeInBits + 2 <= 8*SnP_width
  *
  * @post   phase == feedingAssociatedData
  *
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_Initialize(Ketje_Instance *instance, const unsigned char *key, unsigned int keySizeInBits, const unsigned char *nonce, unsigned int nonceSizeInBits);

/**
  * Function that feeds (partial) associated data that consists of a sequence
  * of bytes. Associated data may be fed in multiple calls to this function.
  * The end of it is indicated by a call to wrap or unwrap.
  *
  * @param  instance            Pointer to the Ketje instance structure.
  * @param  data                Pointer to the (partial) associated data.
  * @param  dataSizeInBytes     The size of the (partial) associated data in bytes.
  *
  * @pre    phase == feedingAssociatedData
  *
  * @post   phase == feedingAssociatedData
  *
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_FeedAssociatedData(Ketje_Instance *instance, const unsigned char *data, unsigned int dataSizeInBytes);

/**
  * Function that presents a (partial) plaintext body that consists of a
  * sequence of bytes for wrapping. A plaintext body may be wrapped in
  * multiple calls to wrap. The end of the plaintext body is indicated
  * by a call to getTag.
  *
  * @param  instance            Pointer to the Ketje instance structure.
  * @param  plaintext           The (partial) plaintext body.
  * @param  ciphertext          The buffer where enciphered data will be stored, can be equal to plaintext buffer.
  * @param  dataSizeInBytes     The size of the (partial) plaintext body.
  *
  * @pre    ( phase == feedingAssociatedData ) or ( phase == wrapping )
  *
  * @post   phase == wrapping
  *
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_WrapPlaintext(Ketje_Instance *instance, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int dataSizeInBytes);

/**
  * Function that presents a (partial) ciphertext body that consists of a
  * sequence of bytes for unwrapping. A ciphertext body may be wrapped in
  * multiple calls to unwrap. The end of the ciphertext body is indicated
  * by a call to getTag.
  *
  * @param  instance            Pointer to the Ketje instance structure.
  * @param  ciphertext          The (partial) ciphertext body.
  * @param  plaintext           The buffer where deciphered data will be stored, can be equal to ciphertext buffer.
  * @param  dataSizeInBytes     The size of the (partial) ciphertext body.
  *
  * @pre    ( phase == feedingAssociatedData ) or ( phase == unwrapping )
  *
  * @post   phase == unwrapping
  *
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_UnwrapCiphertext(Ketje_Instance *instance, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int dataSizeInBytes);

/**
  * Function that gets a tag of a requested size in bytes. The full tag must be retrieved
  * with a single call to getTag.
  *
  * @param  instance            Pointer to the Ketje instance structure.
  * @param  tag                 The buffer where to store the tag.
  * @param  tagSizeInBytes      The length in bytes of the tag requested.
  *
  * @pre    ( phase == wrapping ) or ( phase == unwrapping )
  *
  * @post   phase == feedingAssociatedData
  *
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_GetTag(Ketje_Instance *instance, unsigned char *tag, unsigned int tagSizeInBytes);

#endif
