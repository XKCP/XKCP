/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KravatteModes_h_
#define _KravatteModes_h_

#ifndef KeccakP1600_excluded

#include <stddef.h>
#include <stdint.h>
#include "align.h"
#include "Kravatte.h"

/**
  * Kravatte-SIV Tag Length in bytes.
  */
#define Kravatte_SIV_TagLength      32

/**
  * Macro to initialize a Kravatte_SIV instance with given key.
  * @param  kv              Pointer to the instance to be initialized.
  * @param  Key             Pointer to the key (K).
  * @param  KeyBitLen       The length of the key in bits.
  * @return 0 if successful, 1 otherwise.
  */
#define Kravatte_SIV_MaskDerivation(kv, Key, KeyBitLen)    Kravatte_MaskDerivation(kv, Key, KeyBitLen)

/**
  * Function to wrap plaintext into ciphertext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_SIV_MaskDerivation().
  * @param  plaintext       Pointer to plaintext data to wrap.
  * @param  ciphertext      Pointer to buffer where the full wrapped data will be stored.
  *                         The ciphertext buffer must not overlap plaintext.
  * @param  dataBitLen      The size of the plaintext/ciphertext data.
  * @param  AD              Pointer to the Associated Data.
  * @param  ADBitLen        The number of bytes provided in the Associated Data.
  * @param  tag             The buffer where to store the tag.
  *                         This buffer must be minimum Kravatte_SIV_TagLength bytes long.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_SIV_Wrap(Kravatte_Instance *kvInstance, const BitSequence *plaintext, BitSequence *ciphertext, BitLength dataBitLen, 
                        const BitSequence *AD, BitLength ADBitLen, unsigned char *tag);

/**
  * Function to unwrap ciphertext into plaintext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_SIV_MaskDerivation().
  * @param  ciphertext      Pointer to ciphertext data to unwrap.
  * @param  plaintext       Pointer to buffer where the full unwrapped data will be stored.
  *                         The plaintext buffer must not overlap ciphertext.
  * @param  dataBitLen      The size of the ciphertext/plaintext data.
  * @param  tag             The buffer where to read the tag to check (when lastFlag is set).
  *                         This buffer must be minimum Kravatte_SIV_TagLength bytes long.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_SIV_Unwrap(Kravatte_Instance *kvInstance, const BitSequence *ciphertext, BitSequence *plaintext, BitLength dataBitLen, 
                            const BitSequence *AD, BitLength ADBitLen, const unsigned char *tag);

/* ------------------------------------------------------------------------- */

/**
  * Kravatte-SAE Tag Length in bytes.
  */
#define Kravatte_SAE_TagLength      16

/**
  * Definition of the constant l.
  */
#define Kravatte_SAE_l              128

/**
  * Function to initialize a Kravatte SAE instance with given key and nonce.
  * @param  kvInstance      Pointer to the instance to be initialized.
  * @param  Key             Pointer to the key (K).
  * @param  KeyBitLen       The length of the key in bits.
  * @param  Nonce           Pointer to the nonce (N).
  * @param  NonceBitLen     The length of the nonce in bits.
  * @param  tag             The buffer where to store the tag.
  *                         This buffer must be minimum Kravatte_SAE_TagLength bytes long.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_SAE_Initialize(Kravatte_Instance *kvInstance, const BitSequence *Key, BitLength KeyBitLen, 
                            const BitSequence *Nonce, BitLength NonceBitLen, unsigned char *tag);

/**
  * Function to wrap plaintext into ciphertext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_SAE_Initialize().
  * @param  plaintext       Pointer to plaintext data to wrap.
  * @param  ciphertext      Pointer to buffer where the full wrapped data will be stored.
  *                         The ciphertext buffer must not overlap plaintext.
  * @param  dataBitLen      The size of the plaintext/ciphertext data.
  * @param  AD              Pointer to the Associated Data.
  * @param  ADBitLen        The number of bytes provided in the Associated Data.
  * @param  tag             The buffer where to store the tag.
  *                         This buffer must be minimum Kravatte_SAE_TagLength bytes long.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_SAE_Wrap(Kravatte_Instance *kvInstance, const BitSequence *plaintext, BitSequence *ciphertext, BitLength dataBitLen, 
                        const BitSequence *AD, BitLength ADBitLen, unsigned char *tag);

/**
  * Function to unwrap ciphertext into plaintext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_SAE_Initialize().
  * @param  ciphertext      Pointer to ciphertext data to unwrap.
  * @param  plaintext       Pointer to buffer where the full unwrapped data will be stored.
  *                         The plaintext buffer must not overlap ciphertext.
  * @param  dataBitLen      The size of the ciphertext/plaintext data.
  * @param  tag             The buffer where to read the tag to check (when lastFlag is set).
  *                         This buffer must be minimum Kravatte_SAE_TagLength bytes long.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_SAE_Unwrap(Kravatte_Instance *kvInstance, const BitSequence *ciphertext, BitSequence *plaintext, BitLength dataBitLen, 
                            const BitSequence *AD, BitLength ADBitLen, const unsigned char *tag);

/* ------------------------------------------------------------------------- */

/**
  * Definition of the constant l, used to split the input into two parts.
  * The left part of the input will be a multiple of l bits.
  */
#define Kravatte_WBC_l      8

/**
  * Definition of the constant b block length.
  */
#define Kravatte_WBC_b      (SnP_widthInBytes*8)

/**
  * Macro to initialize a Kravatte_WBC instance with given key.
  * @param  kvw             Pointer to the instance to be initialized.
  * @param  Key             Pointer to the key (K).
  * @param  KeyBitLen       The length of the key in bits.
  * @return 0 if successful, 1 otherwise.
  */
#define Kravatte_WBC_Initialize(kvw, Key, KeyBitLen)        Kravatte_MaskDerivation(kvw, Key, KeyBitLen)

/**
  * Function to encipher plaintext into ciphertext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_WBC_Initialize().
  * @param  plaintext       Pointer to plaintext data to encipher.
  * @param  ciphertext      Pointer to buffer where the enciphered data will be stored.
  *                         The ciphertext buffer must not overlap plaintext.
  * @param  dataBitLen      The size in bits of the plaintext/ciphertext data.
  * @param  W               Pointer to the tweak W.
  * @param  WBitLen         The number of bits provided in the tweak.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_WBC_Encipher(Kravatte_Instance *kvwInstance, const BitSequence *plaintext, BitSequence *ciphertext, BitLength dataBitLen, 
                        const BitSequence *W, BitLength WBitLen);

/**
  * Function to decipher ciphertext into plaintext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_WBC_Initialize().
  * @param  ciphertext      Pointer to ciphertext data to decipher.
  * @param  plaintext       Pointer to buffer where the deciphered data will be stored.
  *                         The plaintext buffer must not overlap ciphertext.
  * @param  dataBitLen      The size in bits of the plaintext/ciphertext data.
  * @param  W               Pointer to the tweak W.
  * @param  WBitLen         The number of bits provided in the tweak.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_WBC_Decipher(Kravatte_Instance *kvwInstance, const BitSequence *ciphertext, BitSequence *plaintext, BitLength dataBitLen, 
                        const BitSequence *W, BitLength WBitLen);

/* ------------------------------------------------------------------------- */

/**
  * Definition of the constant t, expansion length (in bits).
  */
#define Kravatte_WBCAE_t      128

/**
  * Macro to initialize a Kravatte_WBC instance with given key.
  * @param  kvw             Pointer to the instance to be initialized.
  * @param  Key             Pointer to the key (K).
  * @param  KeyBitLen       The length of the key in bits.
  * @return 0 if successful, 1 otherwise.
  */
#define Kravatte_WBCAE_Initialize(kvw, Key, KeyBitLen)      Kravatte_MaskDerivation(kvw, Key, KeyBitLen)

/**
  * Function to encipher plaintext into ciphertext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_WBC_Initialize().
  * @param  plaintext       Pointer to plaintext data to encipher.
  *                         The last ::Kravatte_WBCAE_t bits of the buffer will be overwritten with zeros.
  * @param  ciphertext      Pointer to buffer where the enciphered data will be stored.
  *                         The ciphertext buffer must not overlap plaintext.
  *                         Ciphertext will be ::Kravatte_WBCAE_t bits longer than plaintext.
  * @param  dataBitLen      The size in bits of the plaintext data.
  *                         Plaintext and ciphertext buffers must be ::Kravatte_WBCAE_t bits longer than dataBitLen.
  * @param  AD              Pointer to the metadata AD.
  * @param  ADBitLen        The number of bits provided in the metadata.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_WBCAE_Encipher(Kravatte_Instance *kvwInstance, BitSequence *plaintext, BitSequence *ciphertext, BitLength dataBitLen, 
                        const BitSequence *AD, BitLength ADBitLen);

/**
  * Function to decipher ciphertext into plaintext.
  * @param  kvInstance      Pointer to the instance initialized by Kravatte_WBC_Initialize().
  * @param  ciphertext      Pointer to ciphertext data to decipher.
  *                         Ciphertext is ::Kravatte_WBCAE_t bits longer than plaintext.
  * @param  plaintext       Pointer to buffer where the deciphered data will be stored.
  *                         The plaintext buffer must not overlap ciphertext.
  * @param  dataBitLen      The size in bits of the plaintext data.
  *                         Ciphertext and plaintext buffers must be ::Kravatte_WBCAE_t bits longer than dataBitLen.
  * @param  AD              Pointer to the metadata AD.
  * @param  ADBitLen        The number of bits provided in the metadata.
  * @return 0 if successful, 1 otherwise.
  */
int Kravatte_WBCAE_Decipher(Kravatte_Instance *kvwInstance, const BitSequence *ciphertext, BitSequence *plaintext, BitLength dataBitLen, 
                        const BitSequence *AD, BitLength ADBitLen);

#endif

#endif
