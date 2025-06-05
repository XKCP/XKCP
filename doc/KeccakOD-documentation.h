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
  * These functions implement the SHAKE128-OD, SHAKE256-OD, TurboSHAKE128-OD
  * and TurboSHAKE256-OD duplex ciphers, see https://eprint.iacr.org/2024/1618
  * for more details.
  *
  * As the documentation is generic, the following functions are actually placeholders
  * for their instantiation obtained by replacing the prefix Prefix with
  * - SHAKE for an overwrite duplex object based on Keccak-f[1600], or
  * - TurboSHAKE for an overwrite duplex object based on Keccak-p[1600, 12 rounds].
  *
  * The KeccakWidth1600_ODInstance structure contains the OD instance attributes for use with the Prefix_OD* functions.
  */

#ifdef DontReallyInclude_DocumentationOnly
/**
  * Structure that contains the overwrite duplex (OD) instance for use with the
  * Prefix_OD* functions.
  */
typedef struct KeccakWidth1600_ODInstanceStruct {
    /** The state processed by the permutation. */
    state_t s;
    /** The block size in bytes. */
    unsigned int rho;
    /** The capacity in bits. */
    unsigned int c;
    /** The offset in bytes in the output stream. */
    unsigned int o;
} KeccakWidth1600_ODInstance;

/**
  * Function to initialize an OD cipher with a given secret key.
  * @param  od          Pointer to the OD instance to be initialized.
  * @param  rho         The block size in bytes (160 for [Turbo]SHAKE128-OD or 128 for [Turbo]SHAKE256-OD).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-OD or 512 for [Turbo]SHAKE256-OD).
  * @param  k           The secret key.
  * @param  klen        The length of the secret key in bytes.
  * @pre    @a klen must fit in one block, hence must be at most @a rho.
  */
void Prefix_OD_Initialize(KeccakWidth1600_ODInstance *od, unsigned int rho, unsigned int c, const uint8_t *k, unsigned int klen);

/**
  * Function to clone an OD cipher.
  * @param  odnew       Pointer to the destination OD instance.
  * @param  od          Pointer to the source OD instance.
  */
void Prefix_OD_Clone(KeccakWidth1600_ODInstance *odnew, const KeccakWidth1600_ODInstance *od);

/**
  * Function to clone an OD cipher but clearing its output stream.
  * @param  odnew       Pointer to the destination OD instance.
  * @param  od          Pointer to the source OD instance.
  */
void Prefix_OD_CloneCompact(KeccakWidth1600_ODInstance *odnew, const KeccakWidth1600_ODInstance *od);

/** Function to perform a duplexing call, that is, to absorb input data and possibly squeeze out data.
  * In addition, the caller can optionally pass a pointer (@a odataAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  od          Pointer to the OD instance.
  * @param  odata       Pointer to the output buffer.
  * @param  olen        The number of output bytes requested.
  * @param  idata       Pointer to the input data.
  * @param  ilen        The number of input bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  odataAdd    The null pointer,
  *                     or a pointer to a buffer of @a olen bytes that are XORed to the output stream.
  * @pre    1 ≤ @a E ≤ 63
  * @pre    @a ilen ≤ @a rho
  * @pre    @a olen ≤ @a rho
  */
void Prefix_OD_Duplexing(KeccakWidth1600_ODInstance *od, uint8_t *odata, unsigned int olen, const uint8_t *idata, unsigned int ilen, unsigned int E, const uint8_t *odataAdd);

/** Function to perform a squeezing call, that is, further squeeze out data from the previous duplexing call.
  * In addition, the caller can optionally pass a pointer (@a odataAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  od          Pointer to the OD instance.
  * @param  odata       Pointer to the output buffer.
  * @param  olen        The number of output bytes requested.
  * @param  odataAdd    The null pointer,
  *                     or a pointer to a buffer of @a olen bytes that are XORed to the output stream.
  * @pre    The total number of output bytes requested since the last duplexing call must be at most @a rho.
  */
void Prefix_OD_Squeezing(KeccakWidth1600_ODInstance *od, uint8_t *odata, unsigned int olen, const uint8_t *odataAdd );

/** Function that iterates duplexing calls with @a odataAdd not null.
  * It is equivalent to calling Prefix_OD_Duplexing(od, odata + i*rho, rho, idata + i*rho, rho, E, odataAdd + i*rho)
  * starting from i=0 and as long as @a len allows.
  * @param  od          Pointer to the OD instance.
  * @param  idata       Pointer to the input data.
  * @param  len         The number of input or output bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  odata       Pointer to the output buffer.
  * @param  odataAdd    A pointer to a buffer of bytes that are XORed to the output stream.
  * @return The number of bytes processed.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_OD_DuplexingFast(KeccakWidth1600_ODInstance *od, const uint8_t *idata, size_t len, unsigned int E, uint8_t *odata, const uint8_t *odataAdd );

/** Function that iterates duplexing calls with @a odataAdd not null but empty input data.
  * It is equivalent to calling Prefix_OD_Duplexing(od, odata + i*rho, rho, empty, 0, E, odataAdd + i*rho)
  * starting from i=0 and as long as @a len allows.
  * @param  od          Pointer to the OD instance.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  odata       Pointer to the output buffer.
  * @param  len         The number of output bytes.
  * @param  odataAdd    A pointer to a buffer of bytes that are XORed to the output stream.
  * @return The number of bytes processed.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_OD_DuplexingFastOnlyOut(KeccakWidth1600_ODInstance *od, unsigned int E, uint8_t *odata, size_t len, const uint8_t *odataAdd );

/** Function that iterates duplexing calls without requesting any output.
  * It is equivalent to calling Prefix_OD_Duplexing(od, null, 0, idata + i*rho, rho, E, null)
  * starting from i=0 and as long as @a len allows.
  * @param  od          Pointer to the OD instance.
  * @param  idata       Pointer to the input data.
  * @param  len         The number of input or output bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @return The number of bytes processed.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_OD_DuplexingFastOnlyIn(KeccakWidth1600_ODInstance *od, const uint8_t *idata, size_t len, unsigned int E );
#endif
