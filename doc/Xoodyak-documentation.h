/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifdef DontReallyInclude_DocumentationOnly

/**
  * Function that initializes a Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure to initialize.
  * @param  K               Pointer to the key. Can be null if no key is given.
  * @param  KLen            The size of the key in bytes. Must be zero if no key is given.
  * @param  ID              Pointer to the id. Can be null if no id is given.
  * @param  IDLen           The size of the id in bytes. Must be zero if no id is given.
  * @param  counter         Pointer to the counter. Can be null if no counter is given.
  * @param  counterLen      The size of the counter in bytes. Must be zero if no counter is given.
  */
void Xoodyak_Initialize(Xoodyak_Instance *instance, const uint8_t *K, size_t KLen, const uint8_t *ID, size_t IDLen, const uint8_t *counter, size_t counterLen);

/**
  * Function to call Absorb() on the given Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure.
  * @param  X               Pointer to the string to absorb.
  * @param  XLen            The length in bytes of the string to absorb.
  */
void Xoodyak_Absorb(Xoodyak_Instance *instance, const uint8_t *X, size_t XLen);

/**
  * Function to call Encrypt() on the given Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure.
  * @param  P               Pointer to the plaintext string to encrypt.
  * @param  C               Pointer to the buffer where the ciphertext has to be stored.
  *                         The buffer must have at least @a PLen bytes.
  * @param  PLen            The length in bytes of the plaintext string.
  */
void Xoodyak_Encrypt(Xoodyak_Instance *instance, const uint8_t *P, uint8_t *C, size_t PLen);

/**
  * Function to call Decrypt() on the given Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure.
  * @param  C               Pointer to the ciphertext string to decrypt.
  * @param  P               Pointer to the buffer where the plaintext has to be stored.
  *                         The buffer must have at least @a CLen bytes.
  * @param  CLen            The length in bytes of the ciphertext string.
  */
void Xoodyak_Decrypt(Xoodyak_Instance *instance, const uint8_t *C, uint8_t *P, size_t CLen);

/**
  * Function to call Squeeze() on the given Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure.
  * @param  Y               Pointer to the buffer where the output has to be stored.
  *                         The buffer must have at least @a YLen bytes.
  * @param  YLen            The length in bytes of the requested output.
  */
void Xoodyak_Squeeze(Xoodyak_Instance *instance, uint8_t *Y, size_t YLen);

/**
  * Function to call SqueezeKey() on the given Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure.
  * @param  K               Pointer to the buffer where the output has to be stored.
  *                         The buffer must have at least @a KLen bytes.
  * @param  KLen            The length in bytes of the requested output.
  */
void Xoodyak_SqueezeKey(Xoodyak_Instance *instance, uint8_t *K, size_t KLen);

/**
  * Function to call Ratchet() on the given Xoodyak object.
  *
  * @param  instance        Pointer to the Xoodyak object structure.
  */
void Xoodyak_Ratchet(Xoodyak_Instance *instance);

#endif
