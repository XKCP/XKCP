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
  * These functions implement the {SHAKE128, SHAKE256, TurboSHAKE128, TurboSHAKE256} × {-Wrap, -UpperDeck, -FlightDeck, -BO} ciphers,
  * see https://eprint.iacr.org/2024/1618 for more details.
  *
  * As the documentation is generic, the following functions are actually placeholders
  * for their instantiation obtained by replacing the prefix Prefix with
  * - SHAKE for an overwrite duplex object based on Keccak-f[1600], or
  * - TurboSHAKE for an overwrite duplex object based on Keccak-p[1600, 12 rounds].
  *
  * The KeccakWidth1600_{DWrap, UpperDeck, FlightDeck, DeckBO}Instance structures contain the corresponding instance attributes.
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
  * @param  rho         The block size of the underlying OD in bytes (160 for [Turbo]SHAKE128-Wrap or 128 for [Turbo]SHAKE256-Wrap).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-Wrap or 512 for [Turbo]SHAKE256-Wrap).
  * @param  taglen      The ciphertext expansion (or tag length) in bytes.
  * @param  key         The secret key.
  * @param  keyByteLen  The length of the secret key in bytes.
  * @return 0 if successful, 1 otherwise.
  * @pre    @a keyByteLen must fit in one block, hence must be at most @a rho.
  * Dedicated initializers are also available: SHAKE128_Wrap_Initialize, SHAKE256_Wrap_Initialize,
  * TurboSHAKE128_Wrap_Initialize and TurboSHAKE256_Wrap_Initialize.
  */
int Prefix_Wrap_Initialize(KeccakWidth1600_DWrapInstance *D, unsigned int rho, unsigned int c, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);

/**
  * Function to clone a DWrap cipher.
  * @param  Dnew        Pointer to the destination DWrap instance.
  * @param  D           Pointer to the source DWrap instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_Wrap_Clone(KeccakWidth1600_DWrapInstance *Dnew, const KeccakWidth1600_DWrapInstance *D);

/**
  * Function to wrap a plaintext - associated data pair.
  * @param  D           Pointer to the DWrap instance.
  * @param  C           Pointer to the ciphertext buffer, must be able to contain @a Plen + @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  P           Pointer to the plaintext.
  * @param  Plen        The length of the plaintext in bytes.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_Wrap_Wrap(KeccakWidth1600_DWrapInstance *D, unsigned char *C, const unsigned char *A, size_t Alen, const unsigned char *P, size_t Plen);

/**
  * Function to unwrap a ciphertext - associated data pair.
  * @param  D           Pointer to the DWrap instance.
  * @param  P           Pointer to the plaintext buffer, must be able to contain @a Clen - @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  C           Pointer to the ciphertext.
  * @param  Clen        The length of the ciphertext in bytes.
  * @return 0 if successful, -1 in case of forgery attempt, 1 for any other error
  */
int Prefix_Wrap_Unwrap(KeccakWidth1600_DWrapInstance *D, unsigned char *P, const unsigned char *A, size_t Alen, const unsigned char *C, size_t Clen);

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
  * @param  rho         The block size of the underling OD in bytes (160 for [Turbo]SHAKE128-UpperDeck or 128 for [Turbo]SHAKE256-UpperDeck).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-UpperDeck or 512 for [Turbo]SHAKE256-UpperDeck).
  * @param  key         The secret key.
  * @param  keyByteLen  The length of the secret key in bytes.
  * @return 0 if successful, 1 otherwise.
  * @pre    8 * @a rho + @a c + 64 must equal 1600.
  * @pre    @a keyByteLen must fit in one OD block, hence must be at most @a rho.
  * Dedicated initializers are also available: SHAKE128_UpperDeck_Initialize, SHAKE256_UpperDeck_Initialize,
  * TurboSHAKE128_UpperDeck_Initialize and TurboSHAKE256_UpperDeck_Initialize.
  */
int Prefix_UpperDeck_Initialize(KeccakWidth1600_UpperDeckInstance *ud, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen);

/**
  * Function to clone an UpperDeck cipher.
  * @param  udnew       Pointer to the destination UpperDeck instance.
  * @param  ud          Pointer to the source UpperDeck instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_UpperDeck_Clone(KeccakWidth1600_UpperDeckInstance *udnew, const KeccakWidth1600_UpperDeckInstance *ud);

/**
  * Function to clone an UpperDeck cipher but clearing its output stream.
  * @param  udnew       Pointer to the destination UpperDeck instance.
  * @param  ud          Pointer to the source UpperDeck instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_UpperDeck_CloneCompact(KeccakWidth1600_UpperDeckInstance *udnew, const KeccakWidth1600_UpperDeckInstance *ud);

/** Function to perform a duplexing call, that is, to absorb input data and possibly squeeze out data.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  ud          Pointer to the UpperDeck instance.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested.
  * @param  input       Pointer to the input data.
  * @param  inputLen    The number of input bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 31).
  * @param  toAdd       The null pointer,
  *                     or a pointer to a buffer of @a outputLen bytes that are XORed to the output stream.
  * @return 0 if successful, 1 otherwise.
  * @pre    1 ≤ @a E ≤ 31
  */
int Prefix_UpperDeck_Duplexing(KeccakWidth1600_UpperDeckInstance *ud, unsigned char *output, size_t outputLen, const unsigned char *input, size_t inputLen, unsigned int E, const unsigned char *toAdd);

/** Function to perform a squeezing call, that is, further squeeze out data from the previous duplexing call.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  ud          Pointer to the UpperDeck instance.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested.
  * @param  toAdd       The null pointer,
  *                     or a pointer to a buffer of @a outputLen bytes that are XORed to the output stream.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_UpperDeck_Squeezing(KeccakWidth1600_UpperDeckInstance *ud, unsigned char *output, size_t outputLen, const unsigned char *toAdd);

/**
  * Structure that contains the FlightDeck instance for use with the
  * Prefix_FlightDeck* functions.
  */
typedef struct KeccakWidth1600_FlightDeckInstanceStruct {
    /** The underlying OD instance. */
    KeccakWidth1600_ODInstance D;
    /** The OD instance for squeezing. */
    KeccakWidth1600_ODInstance Dsqueeze;
    /** The FlightDeck input parameter alpha, in bytes. */
    size_t alpha;
    /** The offset in bytes in the output stream. */
    size_t o;
} KeccakWidth1600_FlightDeckInstance;

/**
  * Function to initialize a FlightDeck cipher with a given secret key.
  * For [Turbo]SHAKE128-FlightDeck, use rho = 160, c = 256 and alpha = 5120.
  * For [Turbo]SHAKE256-FlightDeck, use rho = 128, c = 512 and alpha = 8192.
  * @param  fd          Pointer to the FlightDeck instance to be initialized.
  * @param  rho         The block size of the underlying OD in bytes.
  * @param  c           The capacity of the underlying OD in bits.
  * @param  alpha       The FlightDeck input parameter in bytes.
  * @param  key         The secret key.
  * @param  keyByteLen  The length of the secret key in bytes.
  * @return 0 if successful, 1 otherwise.
  * @pre    @a keyByteLen must fit in one OD block, hence must be at most @a rho.
  */
int Prefix_FlightDeck_Initialize(KeccakWidth1600_FlightDeckInstance *fd, unsigned int rho, unsigned int c, size_t alpha, const unsigned char *key, unsigned int keyByteLen);

/**
  * Function to clone a FlightDeck cipher.
  * @param  fdnew       Pointer to the destination FlightDeck instance.
  * @param  fd          Pointer to the source FlightDeck instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_FlightDeck_Clone(KeccakWidth1600_FlightDeckInstance *fdnew, const KeccakWidth1600_FlightDeckInstance *fd);

/**
  * Function to clone a FlightDeck cipher but clearing its output stream.
  * @param  fdnew       Pointer to the destination FlightDeck instance.
  * @param  fd          Pointer to the source FlightDeck instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_FlightDeck_CloneCompact(KeccakWidth1600_FlightDeckInstance *fdnew, const KeccakWidth1600_FlightDeckInstance *fd);

/** Function to perform a duplexing call, that is, to absorb input data and possibly squeeze out data.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  fd          Pointer to the FlightDeck instance.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested.
  * @param  input       Pointer to the input data.
  * @param  inputLen    The number of input bytes.
  * @param  E           The trailer value (1 <= @a E <= 63).
  * @param  toAdd       The null pointer, or a pointer to a buffer of @a outputLen bytes that are XORed to the output stream.
  * @return 0 if successful, 1 otherwise.
  * @pre    1 <= @a E <= 63
  */
int Prefix_FlightDeck_Duplexing(KeccakWidth1600_FlightDeckInstance *fd, unsigned char *output, size_t outputLen, const unsigned char *input, size_t inputLen, unsigned int E, const unsigned char *toAdd);

/** Function to perform a squeezing call, that is, further squeeze out data from the previous duplexing call.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  fd          Pointer to the FlightDeck instance.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested.
  * @param  toAdd       The null pointer, or a pointer to a buffer of @a outputLen bytes that are XORed to the output stream.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_FlightDeck_Squeezing(KeccakWidth1600_FlightDeckInstance *fd, unsigned char *output, size_t outputLen, const unsigned char *toAdd);

/**
  * Structure that contains the Deck-BO instance for use with the
  * Prefix_BO* functions.
  */
typedef struct KeccakWidth1600_DeckBOInstanceStruct {
    /** The underlying FlightDeck instance. */
    KeccakWidth1600_FlightDeckInstance D;
    /** The tag length in bytes. */
    unsigned int taglen;
} KeccakWidth1600_DeckBOInstance;

/**
  * Function to initialize a Deck-BO cipher with custom FlightDeck parameters.
  * @param  dbo         Pointer to the Deck-BO instance to be initialized.
  * @param  rho         The block size of the underlying OD in bytes.
  * @param  c           The capacity of the underlying OD in bits.
  * @param  alpha       The FlightDeck input parameter in bytes.
  * @param  taglen      The ciphertext expansion (or tag length) in bytes.
  * @param  key         The secret key.
  * @param  keyByteLen  The length of the secret key in bytes.
  * @return 0 if successful, 1 otherwise.
  * @pre    8 * @a rho + @a c + 64 must equal 1600.
  * @pre    @a keyByteLen must fit in one OD block, hence must be at most @a rho.
  * @pre    @a taglen must be at most 64 bytes.
  * For [Turbo]SHAKE128-BO, the standard parameters are rho = 160, c = 256 and alpha = 5120.
  * For [Turbo]SHAKE256-BO, the standard parameters are rho = 128, c = 512 and alpha = 8192.
  * Dedicated initializers are also available: SHAKE128_BO_Initialize, SHAKE256_BO_Initialize,
  * TurboSHAKE128_BO_Initialize and TurboSHAKE256_BO_Initialize.
  */
int Prefix_BO_Initialize(KeccakWidth1600_DeckBOInstance *dbo, unsigned int rho, unsigned int c, size_t alpha, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);

/**
  * Function to clone a Deck-BO cipher.
  * @param  dbonew      Pointer to the destination Deck-BO instance.
  * @param  dbo         Pointer to the source Deck-BO instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_BO_Clone(KeccakWidth1600_DeckBOInstance *dbonew, const KeccakWidth1600_DeckBOInstance *dbo);

/**
  * Function to wrap a plaintext - associated data pair.
  * @param  dbo         Pointer to the Deck-BO instance.
  * @param  C           Pointer to the ciphertext buffer, must be able to contain @a Plen + @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  P           Pointer to the plaintext.
  * @param  Plen        The length of the plaintext in bytes.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_BO_Wrap(KeccakWidth1600_DeckBOInstance *dbo, unsigned char *C, const unsigned char *A, size_t Alen, const unsigned char *P, size_t Plen);

/**
  * Function to unwrap a ciphertext - associated data pair.
  * @param  dbo         Pointer to the Deck-BO instance.
  * @param  P           Pointer to the plaintext buffer, must be able to contain @a Clen - @a taglen bytes.
  * @param  A           Pointer to the associated data.
  * @param  Alen        The length of the associated data in bytes.
  * @param  C           Pointer to the ciphertext.
  * @param  Clen        The length of the ciphertext in bytes.
  * @return 0 if successful, -1 in case of forgery attempt, 1 for any other error
  */
int Prefix_BO_Unwrap(KeccakWidth1600_DeckBOInstance *dbo, unsigned char *P, const unsigned char *A, size_t Alen, const unsigned char *C, size_t Clen);
#endif
