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
  * The KeccakWidth1600_ODInstance structure contains the OD instance attributes for use
  * with the {SHAKE, TurboSHAKE}_OD_* functions.
  *
  * The KeccakWidth1600Times{2,4,8}_ParallelODInstance structure contains the OD instance
  * attributes for 2, 4 or 8 ODs in parallel. These are to be used with the
  * {SHAKE, TurboSHAKE}Times{2, 4, 8}_ParallelOD_* functions.
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
  * @param  key         The secret key.
  * @param  keyByteLen  The length of the secret key in bytes.
  * @return 0 if successful, 1 otherwise.
  * @pre    @a keyByteLen must fit in one block, hence must be at most @a rho.
  */
int Prefix_OD_Initialize(KeccakWidth1600_ODInstance *od, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen);

/**
  * Convenience wrappers around Prefix_OD_Initialize are provided as
  * @c SHAKE128_OD_Initialize and @c TurboSHAKE128_OD_Initialize with
  * (@a rho, @a c) = (160, 256), and @c SHAKE256_OD_Initialize and
  * @c TurboSHAKE256_OD_Initialize with (@a rho, @a c) = (128, 512).
  */

/**
  * Function to clone an OD cipher.
  * @param  odnew       Pointer to the destination OD instance.
  * @param  od          Pointer to the source OD instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_OD_Clone(KeccakWidth1600_ODInstance *odnew, const KeccakWidth1600_ODInstance *od);

/**
  * Function to clone an OD cipher but clearing its output stream.
  * @param  odnew       Pointer to the destination OD instance.
  * @param  od          Pointer to the source OD instance.
  * @return 0 if successful, 1 otherwise.
  */
int Prefix_OD_CloneCompact(KeccakWidth1600_ODInstance *odnew, const KeccakWidth1600_ODInstance *od);

/** Function to perform a duplexing call, that is, to absorb input data and possibly squeeze out data.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  od          Pointer to the OD instance.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested.
  * @param  input       Pointer to the input data.
  * @param  inputLen    The number of input bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  toAdd       The null pointer,
  *                     or a pointer to a buffer of @a outputLen bytes that are XORed to the output stream.
  * @return 0 if successful, 1 otherwise.
  * @pre    1 ≤ @a E ≤ 63
  * @pre    @a inputLen ≤ @a rho
  * @pre    @a outputLen ≤ @a rho
  */
int Prefix_OD_Duplexing(KeccakWidth1600_ODInstance *od, unsigned char *output, unsigned int outputLen, const unsigned char *input, unsigned int inputLen, unsigned int E, const unsigned char *toAdd);

/** Function to perform a squeezing call, that is, further squeeze out data from the previous duplexing call.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * @param  od          Pointer to the OD instance.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested.
  * @param  toAdd       The null pointer,
  *                     or a pointer to a buffer of @a outputLen bytes that are XORed to the output stream.
  * @return 0 if successful, 1 otherwise.
  * @pre    The total number of output bytes requested since the last duplexing call must be at most @a rho.
  */
int Prefix_OD_Squeezing(KeccakWidth1600_ODInstance *od, unsigned char *output, unsigned int outputLen, const unsigned char *toAdd);

/** Function that iterates duplexing calls with @a toAdd not null.
  * It is equivalent to calling Prefix_OD_Duplexing(od, output + i*rho, rho, input + i*rho, rho, E, toAdd + i*rho)
  * starting from i=0 and as long as @a len allows.
  * @param  od          Pointer to the OD instance.
  * @param  input       Pointer to the input data.
  * @param  len         The number of input or output bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  output      Pointer to the output buffer.
  * @param  toAdd       A pointer to a buffer of bytes that are XORed to the output stream.
  * @return The number of bytes processed.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_OD_DuplexingFast(KeccakWidth1600_ODInstance *od, const unsigned char *input, size_t len, unsigned int E, unsigned char *output, const unsigned char *toAdd);

/** Function that iterates duplexing calls with @a toAdd not null but empty input data.
  * It is equivalent to calling Prefix_OD_Duplexing(od, output + i*rho, rho, empty, 0, E, toAdd + i*rho)
  * starting from i=0 and as long as @a outputLen allows.
  * @param  od          Pointer to the OD instance.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes.
  * @param  toAdd       A pointer to a buffer of bytes that are XORed to the output stream.
  * @return The number of bytes processed.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_OD_DuplexingFastOnlyOut(KeccakWidth1600_ODInstance *od, unsigned int E, unsigned char *output, size_t outputLen, const unsigned char *toAdd);

/** Function that iterates duplexing calls without requesting any output.
  * It is equivalent to calling Prefix_OD_Duplexing(od, null, 0, input + i*rho, rho, E, null)
  * starting from i=0 and as long as @a inputLen allows.
  * @param  od          Pointer to the OD instance.
  * @param  input       Pointer to the input data.
  * @param  inputLen    The number of input bytes.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @return The number of bytes processed.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_OD_DuplexingFastOnlyIn(KeccakWidth1600_ODInstance *od, const unsigned char *input, size_t inputLen, unsigned int E);


/**
  * Structure that contains the overwrite duplex (OD) instance for use with the
  * Prefix_ParallelOD* functions.
  */
typedef struct KeccakWidth1600TimesX_ParallelODInstanceStruct {
  /** The 2, 4 or 8 states processed by the permutation in parallel. */
  states_t s;
  /** The block size in bytes. */
  unsigned int rho;
  /** The capacity in bits. */
  unsigned int c;
  /** The offset in bytes in the output stream, assumed to be the same in all parallel streams. */
  unsigned int o;
} KeccakWidth1600TimesX_ParallelODInstance;

/**
  * Function to initialize parallel OD ciphers with a given secret key.
  * @param  pod         Pointer to the parallel OD instances to be initialized.
  * @param  rho         The block size in bytes (160 for [Turbo]SHAKE128-OD or 128 for [Turbo]SHAKE256-OD).
  * @param  c           The capacity in bits (256 for [Turbo]SHAKE128-OD or 512 for [Turbo]SHAKE256-OD).
  * @param  key         The secret key.
  * @param  keyByteLen  The length of the secret key in bytes.
  * @pre    @a keyByteLen must fit in one block, hence must be at most @a rho.
  */
void Prefix_ParallelOD_Initialize(KeccakWidth1600TimesX_ParallelODInstance *pod, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen);

/**
  * Convenience wrappers around Prefix_ParallelOD_Initialize are provided for
  * parallelism 2, 4 and 8. The functions
  * @c SHAKE128Times2_ParallelOD_Initialize,
  * @c TurboSHAKE128Times2_ParallelOD_Initialize,
  * @c SHAKE128Times4_ParallelOD_Initialize,
  * @c TurboSHAKE128Times4_ParallelOD_Initialize,
  * @c SHAKE128Times8_ParallelOD_Initialize and
  * @c TurboSHAKE128Times8_ParallelOD_Initialize use (@a rho, @a c) =
  * (160, 256). The functions
  * @c SHAKE256Times2_ParallelOD_Initialize,
  * @c TurboSHAKE256Times2_ParallelOD_Initialize,
  * @c SHAKE256Times4_ParallelOD_Initialize,
  * @c TurboSHAKE256Times4_ParallelOD_Initialize,
  * @c SHAKE256Times8_ParallelOD_Initialize and
  * @c TurboSHAKE256Times8_ParallelOD_Initialize use (@a rho, @a c) =
  * (128, 512).
  */

/**
  * Function to clone parallel OD chipers.
  * @param  podnew      Pointer to the destination parallel OD instances.
  * @param  pod         Pointer to the source parallel OD instances.
  */
void Prefix_ParallelOD_Clone(KeccakWidth1600TimesX_ParallelODInstance *podnew, const KeccakWidth1600TimesX_ParallelODInstance *pod);

/**
  * Function to clone parallel OD chipers and clearing their parallel output streams.
  * @param  podnew      Pointer to the destination parallel OD instances.
  * @param  pod         Pointer to the source parallel OD instances.
  */
void Prefix_ParallelOD_CloneCompact(KeccakWidth1600TimesX_ParallelODInstance *podnew, const KeccakWidth1600TimesX_ParallelODInstance *pod);

/**
  * Function to clone a single OD cipher into indentical parallel OD chipers
  * and clearing their parallel output streams.
  * @param  podnew      Pointer to the destination parallel OD instances.
  * @param  od          Pointer to the source OD instance.
  */
void Prefix_ParallelOD_CloneCompactFromOD(KeccakWidth1600TimesX_ParallelODInstance *podnew, const KeccakWidth1600_ODInstance *od);

/**
  * Function to perform duplexing calls in parallel, that is,
  * to absorb input data and possibly squeeze out data.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * The parallel input streams are separated by @a inOffset bytes, that is,
  * parallel instance @a j absorbs @a inputLen bytes at @a input + @a j * @a inOffset.
  * The parallel output streams are separated by @a outOffset bytes, that is,
  * parallel instance @a j squeezes out @a outputLen bytes at @a output + @a j * @a outOffset.
  * If @a toAdd is not null, parallel instance @a j takes @a outputLen bytes at @a toAdd + @a j * @a outOffset.
  * @param  pod         Pointer to the parallel OD instances.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested per parallel instance.
  * @param  outOffset   The separation in bytes between output streams processed in parallel.
  * @param  input       Pointer to the input data.
  * @param  inputLen    The number of input bytes absorbed per parallel instance.
  * @param  inOffset    The separation in bytes between input streams processed in parallel.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @param  toAdd       The null pointer,
  *                     or a pointer to a buffer of bytes that are XORed to the output stream.
  * @pre    1 ≤ @a E ≤ 63
  * @pre    @a inputLen ≤ @a rho
  * @pre    @a outputLen ≤ @a rho
  */
void Prefix_ParallelOD_Duplexing(KeccakWidth1600TimesX_ParallelODInstance *pod, unsigned char *output, unsigned int outputLen, size_t outOffset, const unsigned char *input, unsigned int inputLen, size_t inOffset, unsigned int E, const unsigned char *toAdd);

/**
  * Function to perform squeezing calls in parallel, that is,
  * to further squeeze out data from the previous duplexing call.
  * In addition, the caller can optionally pass a pointer (@a toAdd) to a buffer
  * that will be bitwise XORed to the output stream before being stored to the output buffer.
  * The parallel output streams are separated by @a outOffset bytes, that is,
  * parallel instance @a j squeezes out @a outputLen bytes at @a output + @a j * @a outOffset.
  * If @a toAdd is not null, parallel instance @a j takes @a outputLen bytes at @a toAdd + @a j * @a outOffset.
  * @param  pod         Pointer to the parallel OD instances.
  * @param  output      Pointer to the output buffer.
  * @param  outputLen   The number of output bytes requested per parallel instance.
  * @param  outOffset   The separation in bytes between output streams processed in parallel.
  * @param  toAdd       The null pointer,
  *                     or a pointer to a buffer of bytes that are XORed to the output stream.
  * @pre    The total number of output bytes requested since the last duplexing call must be at most @a rho.
  */
void Prefix_ParallelOD_Squeezing(KeccakWidth1600TimesX_ParallelODInstance *pod, unsigned char *output, unsigned int outputLen, size_t outOffset, const unsigned char *toAdd);

/** Function that iterates duplexing calls without requesting any output.
  * It is equivalent to calling Prefix_ParallelOD_Duplexing(pod, null, 0, 0, input + i*rho, rho, inOffset, E, null)
  * starting from i=0 and up to i=floor(@a inputLen / rho)-1.
  * @param  pod         Pointer to the parallel OD instances.
  * @param  input       Pointer to the input data.
  * @param  inputLen    The number of input bytes per input stream.
  * @param  inOffset    The separation in bytes between input streams processed in parallel.
  * @param  E           The trailer value (1 ≤ @a E ≤ 63).
  * @return The number of bytes processed per input stream.
  * @pre    1 ≤ @a E ≤ 63
  */
size_t Prefix_ParallelOD_DuplexingFastOnlyIn(KeccakWidth1600TimesX_ParallelODInstance *pod, const unsigned char *input, size_t inputLen, size_t inOffset, unsigned int E);


#endif
