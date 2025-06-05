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

/** General information
  *
  * These functions implement the {SHAKE128, SHAKE256, TurboSHAKE128, TurboSHAKE256} × {-Wrap, -UpperDeck, -BO} ciphers,
  * see https://eprint.iacr.org/2024/1618 for more details.
  *
  * As the documentation is generic, the following functions are actually placeholders
  * for their instantiation obtained by replacing the prefix Prefix with
  * - SHAKE for an overwrite duplex object based on Keccak-f[1600], or
  * - TurboSHAKE for an overwrite duplex object based on Keccak-p[1600, 12 rounds].
  *
  * The KeccakWidth1600_{DWrap, UpperDeck, DeckBO}Instance structures contain the corresponding instance attributes.
  */

#ifdef DontReallyInclude_DocumentationOnly
/**
  * Structure that contains the DWrap instance for use with the
  * Prefix_Wrap* functions.
  */
typedef struct KeccakWidth1600_DWrapInstanceStruct {
    /** The underlying OD instance. */
    KeccakWidth1600_ODInstance od;
    /** The tag length in bytes. */
    unsigned int taglen;
} KeccakWidth1600_DWrapInstance;

/**
  * Function to initialize a DWrap cipher with a given secret key and ciphertext expansion.
  * @param  D           Pointer to the DWrap instance to be initialized.
  * @param  k           The secret key.
  * @param  klen        The length of the secret key in bytes.
  * @param  taglen      The ciphertext expansion (or tag length) in bytes.
  * @param  rho         The block size of the underlying OD in bytes (160 for [Turbo]SHAKE128-Wrap or 128 for [Turbo]SHAKE256-Wrap).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-Wrap or 512 for [Turbo]SHAKE256-Wrap).
  * @pre    @a klen must fit in one block, hence must be at most @a rho.
  */
void Prefix_Wrap_Initialize(KeccakWidth1600_DWrapInstance *D, const uint8_t *k, unsigned int klen, unsigned int taglen, unsigned int rho, unsigned int c);

/**
  * Function to clone a DWrap cipher.
  * @param  Dnew        Pointer to the destination DWrap instance.
  * @param  D           Pointer to the source DWrap instance.
  */
void Prefix_Wrap_Clone(KeccakWidth1600_DWrapInstance *Dnew, const KeccakWidth1600_DWrapInstance *D);

/**
  * Function to wrap a plaintext - associated data pair.
  * @param  D           Pointer to the DWrap instance.
  * @param  C           Pointer to the ciphertext buffer, must be able to contain @a Plen + @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  P           Pointer to the plaintext.
  * @param  Plen        The length of the plaintext in bytes.
  */
void Prefix_Wrap_Wrap(KeccakWidth1600_DWrapInstance *D, uint8_t *C, const uint8_t *A, size_t Alen, const uint8_t *P, size_t Plen);

/**
  * Function to unwrap a ciphertext - associated data pair.
  * @param  D           Pointer to the DWrap instance.
  * @param  P           Pointer to the plaintext buffer, must be able to contain @a Clen - @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  C           Pointer to the ciphertext.
  * @param  Clen        The length of the ciphertext in bytes.
  * @return -1 in case of error (forgery), 0 otherwise
  */
int Prefix_Wrap_Unwrap(KeccakWidth1600_DWrapInstance *D, uint8_t *P, const uint8_t *A, size_t Alen, const uint8_t *C, size_t Clen);

/**
  * Structure that contains the UpperDeck instance for use with the
  * Prefix_UpperDeck* functions.
  */
typedef struct KeccakWidth1600_UpperDeckInstanceStruct {
    /** The underlying OD instance. */
    KeccakWidth1600_ODInstance D;
    /** The OD instance for squeezing. */
    KeccakWidth1600_ODInstance Dsqueeze;
    /** The offset in bytes in the output stream. */
    size_t o;
} KeccakWidth1600_UpperDeckInstance;

/**
  * Function to initialize an UpperDeck cipher with a given secret key.
  * @param  ud          Pointer to the UpperDeck instance to be initialized.
  * @param  k           The secret key.
  * @param  klen        The length of the secret key in bytes.
  * @param  rho         The block size of the underling OD in bytes (160 for [Turbo]SHAKE128-UpperDeck or 128 for [Turbo]SHAKE256-UpperDeck).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-UpperDeck or 512 for [Turbo]SHAKE256-UpperDeck).
  * @pre    @a klen must fit in one OD block, hence must be at most @a rho.
  */
void Prefix_UpperDeck_Initialize(KeccakWidth1600_UpperDeckInstance *ud, const uint8_t *k, unsigned int klen, unsigned int rho, unsigned int c);

/**
  * Function to clone an UpperDeck cipher.
  * @param  udnew       Pointer to the destination UpperDeck instance.
  * @param  ud          Pointer to the source UpperDeck instance.
  */
void Prefix_UpperDeck_Clone(KeccakWidth1600_UpperDeckInstance *udnew, const KeccakWidth1600_UpperDeckInstance *ud);

/**
  * Function to clone an UpperDeck cipher but clearing its output stream.
  * @param  udnew       Pointer to the destination UpperDeck instance.
  * @param  ud          Pointer to the source UpperDeck instance.
  */
void Prefix_UpperDeck_CloneCompact(KeccakWidth1600_UpperDeckInstance *udnew, const KeccakWidth1600_UpperDeckInstance *ud);

/** Function to perform a duplexing call, that is, to absorb input data and possibly squeeze out data.
  * In addition, the caller can optionally pass a pointer (@a Yadd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  ud          Pointer to the UpperDeck instance.
  * @param  Z           Pointer to the output buffer.
  * @param  Zlen        The number of output bytes requested.
  * @param  X           Pointer to the input data.
  * @param  Xlen        The number of input bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 31).
  * @param  Yadd    The null pointer,
  *                     or a pointer to a buffer of @a Zlen bytes that are XORed to the output stream.
  * @pre    1 ≤ @a E ≤ 31
  */
void Prefix_UpperDeck_Duplexing(KeccakWidth1600_UpperDeckInstance *ud, uint8_t *Z, size_t Zlen, const uint8_t *X, size_t Xlen, unsigned int E, const uint8_t *Yadd);

/** Function to perform a squeezing call, that is, further squeeze out data from the previous duplexing call.
  * In addition, the caller can optionally pass a pointer (@a odataAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  ud          Pointer to the UpperDeck instance.
  * @param  Z           Pointer to the output buffer.
  * @param  Zlen        The number of output bytes requested.
  * @param  Yadd    The null pointer,
  *                     or a pointer to a buffer of @a Zlen bytes that are XORed to the output stream.
  */
void Prefix_UpperDeck_Squeezing(KeccakWidth1600_UpperDeckInstance *ud, uint8_t *Z, size_t Zlen, const uint8_t *Yadd);

/**
  * Structure that contains the Deck-BO instance for use with the
  * Prefix_BO* functions.
  */
typedef struct KeccakWidth1600_DeckBOInstanceStruct {
    /** The underlying UpperDeck instance. */
    KeccakWidth1600_UpperDeckInstance D;
    /** The tag length in bytes. */
    unsigned int taglen;
} KeccakWidth1600_DeckBOInstance;

/**
  * Function to initialize a Deck-BO cipher with a given secret key and ciphertext expansion.
  * @param  dbo         Pointer to the Deck-BO instance to be initialized.
  * @param  k           The secret key.
  * @param  klen        The length of the secret key in bytes.
  * @param  taglen      The ciphertext expansion (or tag length) in bytes.
  * @param  rho         The block size of the underlying OD in bytes (160 for [Turbo]SHAKE128-Wrap or 128 for [Turbo]SHAKE256-Wrap).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-Wrap or 512 for [Turbo]SHAKE256-Wrap).
  * @pre    @a klen must fit in one block, hence must be at most @a rho.
  */
void Prefix_BO_Initialize(KeccakWidth1600_Deck-BOInstance *dbo, const uint8_t *k, unsigned int klen, unsigned int taglen, unsigned int rho, unsigned int c);

/**
  * Function to clone a Deck-BO cipher.
  * @param  dbonew      Pointer to the destination Deck-BO instance.
  * @param  dbo         Pointer to the source Deck-BO instance.
  */
void Prefix_BO_Clone(KeccakWidth1600_Deck-BOInstance *dbonew, const KeccakWidth1600_Deck-BOInstance *dbo);

/**
  * Function to wrap a plaintext - associated data pair.
  * @param  dbo         Pointer to the Deck-BO instance.
  * @param  C           Pointer to the ciphertext buffer, must be able to contain @a Plen + @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  P           Pointer to the plaintext.
  * @param  Plen        The length of the plaintext in bytes.
  */
void Prefix_BO_Wrap(KeccakWidth1600_Deck-BOInstance *dbo, uint8_t *C, const uint8_t *A, size_t Alen, const uint8_t *P, size_t Plen);

/**
  * Function to unwrap a ciphertext - associated data pair.
  * @param  dbo         Pointer to the Deck-BO instance.
  * @param  P           Pointer to the plaintext buffer, must be able to contain @a Clen - @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  C           Pointer to the ciphertext.
  * @param  Clen        The length of the ciphertext in bytes.
  * @return -1 in case of error (forgery), 0 otherwise
  */
int Prefix_BO_Unwrap(KeccakWidth1600_Deck-BOInstance *dbo, uint8_t *P, const uint8_t *A, size_t Alen, const uint8_t *C, size_t Clen);
#endif
