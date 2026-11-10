/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

/** General information
  *
  * SnP stands for "state and permutation". It provides a set of functions to
  * access and process a state, and in particular to apply a given permutation
  * (or family of permutations) on it.
  *
  * A state is  an array of (width) bits, where the width is the number of input
  * (or output) bits of the permutation. To allow optimizations, the state
  * is represented in an opaque way, and the user must go through the various
  * SnP functions to initialize it, to modify it and to extract data from it.
  *
  * The following functions are not actually implemented. Their documentation
  * is generic, with the prefix SnP replaced by
  *  - KeccakP1600 for Keccak-p[1600]
  *  - Xoodoo for Xoodoo
  * Functions can be replaced by macros, when appropriate.
  *
  * An implementation must provide each function (or macro) as listed below,
  * instantiated with the suitable prefix. In addition, the implementation
  * must come with a header "KeccakP-1600-SnP.h", "Xoodoo-SnP.h", or similar
  * that will be included by the users of the corresponding permutation.
  */


/** Administrative functions
  */

/** Function that returns a description of the implementation.
  */
const char * SnP_GetImplementation();

/** Function that returns the set of features that are implemented for this permutation.
  * The return value is a bit mask that can be the sum of the following values
  * defined in "SnP-common.h" (here the SnP prefix is to be taken litterally):
  *   - SnP_Feature_Main
  *   - SnP_Feature_SpongeAbsorb
  *   - SnP_Feature_OD
  *   - SnP_Feature_Cyclist
  *   - SnP_Feature_Farfalle
  *
  * Each of these features corresponds to a set of functions as detailed below.
  */
int SnP_GetFeatures();


/** Main functions
  *
  * The following functions are the main SnP functions.
  * If supported, the SnP_Feature_Main flag must be present in SnP_GetFeatures().
  *
  * For Keccak-p[1600]:
  *     - SnP_Permute() is instantiated as
  *         - KeccakP1600_Permute_12rounds() for Keccak-p[1600, 12] and
  *         - KeccakP1600_Permute_24rounds() for Keccak-f[1600]=Keccak-p[1600, 24].
  *     - SnP_Permute_Nrounds() is instantiated as KeccakP1600_Permute_Nrounds().
  * For Xoodoo:
  *     - SnP_Permute() is instantiated as
  *         - Xoodoo_Permute_6rounds() for Xoodoo[6] and
  *         - Xoodoo_Permute_12rounds() for Xoodoo[12].
  *     - SnP_Permute_Nrounds() is instantiated as Xoodoo_Permute_Nrounds().
  */

/** Function called at least once before any use of the other SnP_*
  * functions, possibly to initialize global variables.
  */
void SnP_StaticInitialize( void );

/** Function to initialize the state to the logical value 0^width.
  * @param  state   Pointer to the state to initialize.
  */
void SnP_Initialize(SnP_state *state);

/** Function to add (in GF(2), using bitwise exclusive-or) a given byte into the state.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + 8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  data    The input byte.
  * @param  offset  Offset in bytes within the state.
  * @pre    0 ≤ @a offset < (width in bytes)
  */
void SnP_AddByte(SnP_state *state, unsigned char data, unsigned int offset);

/** Function to add (in GF(2), using bitwise exclusive-or) data given as bytes into the state.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the input data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void SnP_AddBytes(SnP_state *state, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to overwrite data given as bytes into the state.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the input data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void SnP_OverwriteBytes(SnP_state *state, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to overwrite bytes in the state with zeroes.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of bytes.
  * @param  state   Pointer to the state.
  * @param  byteCount   The number of bytes, i.e., the length of the data
  *                     divided by 8 bits.
  * @pre    0 ≤ @a byteCount ≤ (width in bytes)
  */
void SnP_OverwriteWithZeroes(SnP_state *state, unsigned int byteCount);

/** Function to apply the permutation on the state.
  * @param  state   Pointer to the state.
  */
void SnP_Permute(SnP_state *state);

/** Function to apply on the state the permutation with the given number of rounds
  * among the permutation family.
  * @param  state   Pointer to the state.
  */
void SnP_Permute_Nrounds(SnP_state *state, unsigned int nrounds);

/** Function to retrieve data from the state.
  * The bit positions that are retrieved by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the area where to store output data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void SnP_ExtractBytes(const SnP_state *state, unsigned char *data, unsigned int offset, unsigned int length);

/** Function to retrieve data from the state,
  * to add  (in GF(2), using bitwise exclusive-or) them to the input buffer,
  * and to store the result in the output buffer.
  * The bit positions that are retrieved by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  input   Pointer to the input buffer.
  * @param  output  Pointer to the output buffer, which may be equal to @a input.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void SnP_ExtractAndAddBytes(const SnP_state *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);


/** Sponge absorb functions
  *
  * An implementation may also instantiate the function SnP_FastLoop_Absorb(),
  * in which case the bit SnP_Feature_SpongeAbsorb must be active in SnP_GetFeatures().
  *
  * For Keccak-p[1600]:
  *     - SnP_FastLoop_Absorb() is instantiated as
  *         - KeccakF1600_FastLoop_Absorb() for Keccak-f[1600]
  *         - KeccakP1600_12rounds_FastLoop_Absorb() for Keccak-p[1600, 12 rounds]
  */

/** Function that has the same behavior as repeatedly calling
  *  - SnP_AddBytes() with a block of @a laneCount lanes from data;
  *  - SnP_Permute() on the state @a state;
  *  - and advancing @a data by @a laneCount lane sizes, until not enough data are available.
  * The function returns the number of bytes processed from @a data.
  * @param  state   Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  data    Pointer to the data to use as input.
  * @param  dataByteLen The length of the input data in bytes.
  * @returns    The number of bytes processed.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t SnP_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);


/** Overwrite Duplex (OD) functions
  *
  * An implementation may also instantiate the SnP_ODDuplexingFast* functions,
  * in which case the bit SnP_Feature_OD must be active in SnP_GetFeatures().
  *
  * For Keccak-p[1600], this means the following functions:
  *     - For OD on top of SHAKE128 or SHAKE256:
  *         - KeccakP1600_ODDuplexingFastInOut
  *         - KeccakP1600_ODDuplexingFastOut
  *         - KeccakP1600_ODDuplexingFastIn
  *     - For OD on top of TurboSHAKE128 or TurboSHAKE256:
  *         - KeccakP1600_12rounds_ODDuplexingFastInOut
  *         - KeccakP1600_12rounds_ODDuplexingFastOut
  *         - KeccakP1600_12rounds_ODDuplexingFastIn
  *
  * See https://eprint.iacr.org/2024/1618 for more details.
  */

/** Function that repeatedly calls OD.duplexing(B, E) and XORs the output with data in a buffer.
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_OverwriteBytes() with a block B of @a laneCount lanes (ρ bytes) from @a idata;
  *     - SnP_AddBytes() with a trailer E encoded as @a trailencAsLane;
  *     - SnP_Permute() on the state @a state;
  *     - SnP_ExtractAndAddBytes() with a block of @a laneCount lanes from the state @a state,
  *         XORed with a block from @a odataAdd, and stored in @a odata;
  *     - and advancing @a idata, @a odata and @a odataAdd by @a laneCount lane sizes, until not enough data are available.
  * @param  state       Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes, or ρ/8).
  *                     It must be 20 for (Turbo)SHAKE128 and 16 for (Turbo)SHAKE128.
  * @param  idata       Pointer to the data to use as input, containing @a len bytes.
  * @param  len         The length of the data in bytes.
  * @param  odata       Pointer to the output buffer, capable of storing @a len bytes.
  * @param  odataAdd    Pointer to the data to XOR with the OD ouput, containing @a len bytes.
  * @param  trailencAsLane  The value trailenc(E||1) encoded as a lane (little endian).
  *                     It must be 0x8000000000000000ULL + (E||1) for TurboSHAKE.
  *                     It must be 0x9F00000000000000ULL + (E||1) for SHAKE.
  * @returns    The number of bytes processed.
  */
size_t SnP_ODDuplexingFastInOut(SnP_state *state, unsigned int laneCount, const unsigned char *idata, size_t len, unsigned char *odata, const unsigned char *odataAdd, uint64_t trailencAsLane);

/** Function that repeatedly calls OD.duplexing(empty, E) and XORs the output with data in a buffer.
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_OverwriteWithZeroes() for @a laneCount lanes (ρ bytes);
  *     - SnP_AddByte() with a single 0x01 byte representing pad10∗(empty);
  *     - SnP_AddBytes() with a trailer E encoded as @a trailencAsLane;
  *     - SnP_Permute() on the state @a state;
  *     - SnP_ExtractAndAddBytes() with a block of @a laneCount lanes from the state @a state,
  *         XORed with a block from @a odataAdd, and stored in @a odata;
  *     - and advancing @a odata and @a odataAdd by @a laneCount lane sizes, until not enough data are available.
  * @param  state       Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes, or ρ/8).
  *                     It must be 20 for (Turbo)SHAKE128 and 16 for (Turbo)SHAKE128.
  * @param  odata       Pointer to the output buffer, capable of storing @a len bytes.
  * @param  len         The length of the data in bytes.
  * @param  odataAdd    Pointer to the data to XOR with the OD ouput, containing @a len bytes.
  * @param  trailencAsLane  The value trailenc(E||1) encoded as a lane (little endian).
  *                     It must be 0x8000000000000000ULL + (E||1) for TurboSHAKE.
  *                     It must be 0x9F00000000000000ULL + (E||1) for SHAKE.
  * @returns    The number of bytes processed.
  */
size_t SnP_ODDuplexingFastOut(SnP_state *state, unsigned int laneCount, unsigned char *odata, size_t len, const unsigned char *odataAdd, uint64_t trailencAsLane);

/** Function that repeatedly calls OD.duplexing(B, E).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_OverwriteBytes() with a block B of @a laneCount lanes (ρ bytes) from @a idata;
  *     - SnP_AddBytes() with a trailer E encoded as @a trailencAsLane;
  *     - SnP_Permute() on the state @a state;
  *     - and advancing @a idata by @a laneCount lane sizes, until not enough data are available.
  * @param  state       Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes, or ρ/8).
  *                     It must be 20 for (Turbo)SHAKE128 and 16 for (Turbo)SHAKE128.
  * @param  idata       Pointer to the data to use as input, containing @a len bytes.
  * @param  len         The length of the data in bytes.
  * @param  trailencAsLane  The value trailenc(E||1) encoded as a lane (little endian).
  *                     It must be 0x8000000000000000ULL + (E||1) for TurboSHAKE.
  *                     It must be 0x9F00000000000000ULL + (E||1) for SHAKE.
  * @returns    The number of bytes processed.
  */
size_t SnP_ODDuplexingFastIn(SnP_state *state, unsigned int laneCount, const uint8_t *idata, size_t len, uint64_t trailencAsLane);


/** Cyclist functions
  *
  * An implementation may also instantiate functions that optimize the Cyclist mode,
  * in which case the bit SnP_Feature_Cyclist must be active in SnP_GetFeatures().
  *
  * For Xoodoo, this means the following functions:
  *     - Xoodyak_AbsorbKeyedFullBlocks
  *     - Xoodyak_AbsorbHashFullBlocks
  *     - Xoodyak_SqueezeHashFullBlocks
  *     - Xoodyak_SqueezeKeyedFullBlocks
  *     - Xoodyak_EncryptFullBlocks
  *     - Xoodyak_DecryptFullBlocks
  *
  * See https://tosc.iacr.org/index.php/ToSC/article/view/8618 for more details.
  */

/** Function that absorbs whole blocks using Cyclist's AbsorbAny(),
  * after the first block, when the Cyclist object is initialized in keyed mode.
  * The block size is Rkin bytes (44 bytes for Xoodyak).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_Permute() on the state @a state;
  *     - SnP_AddBytes() with a block of @a X;
  *     - SnP_AddByte() with a byte 0x01;
  *     - and advancing @a X until no whole blocks remain.
  * @param  state       Pointer to the state.
  * @param  X           Pointer to the data to use as input, containing @a XLen bytes.
  * @param  XLen        The length of the data in bytes.
  * @returns    The number of bytes processed.
  * @pre  @a XLen must be at least one block.
  */
size_t Xoodyak_AbsorbKeyedFullBlocks(Xoodoo_state *state, const uint8_t *X, size_t XLen);

/** Function that absorbs whole blocks using Cyclist's AbsorbAny()
  * after the first block, when the Cyclist object is initialized in hash mode.
  * The block size is Rhash bytes (16 bytes for Xoodyak).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_Permute() on the state @a state;
  *     - SnP_AddBytes() with a block of @a X;
  *     - SnP_AddByte() with a byte 0x01;
  *     - and advancing @a X until no whole blocks remain.
  * @param  state       Pointer to the state.
  * @param  X           Pointer to the data to use as input, containing @a XLen bytes.
  * @param  XLen        The length of the data in bytes.
  * @returns    The number of bytes processed.
  * @pre  @a XLen must be at least one block.
  */
size_t Xoodyak_AbsorbHashFullBlocks(Xoodoo_state *state, const uint8_t *X, size_t XLen);

/** Function that squeezes whole blocks using Cyclist's SqueezeAny()
  * after the first block, when the Cyclist object is initialized in keyed mode.
  * The block size is Rkout bytes (24 bytes for Xoodyak).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_AddByte() with a byte 0x01;
  *     - SnP_Permute() on the state @a state;
  *     - SnP_ExtractBytes() of a block into @a Y;
  *     - and advancing @a Y until no whole blocks remain.
  * @param  state       Pointer to the state.
  * @param  Y           Pointer to the output buffer, containing @a XLen bytes.
  * @param  YLen        The length of the data in bytes.
  * @returns    The number of bytes processed.
  * @pre  @a YLen must be at least one block.
  */
size_t Xoodyak_SqueezeKeyedFullBlocks(Xoodoo_state *state, uint8_t *Y, size_t YLen);

/** Function that squeezes whole blocks using Cyclist's SqueezeAny()
  * after the first block, when the Cyclist object is initialized in hash mode.
  * The block size is Rhash bytes (16 bytes for Xoodyak).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_AddByte() with a byte 0x01;
  *     - SnP_Permute() on the state @a state;
  *     - SnP_ExtractBytes() of a block into @a Y;
  *     - and advancing @a Y until no whole blocks remain.
  * @param  state       Pointer to the state.
  * @param  Y           Pointer to the output buffer, containing @a XLen bytes.
  * @param  YLen        The length of the data in bytes.
  * @returns    The number of bytes processed.
  * @pre  @a YLen must be at least one block.
  */
size_t Xoodyak_SqueezeHashFullBlocks(Xoodoo_state *state, uint8_t *Y, size_t YLen);

/** Function that encrypts whole blocks using Cyclist's Encrypt()
  * after the first block. The block size is Rkout bytes (24 bytes for Xoodyak).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_Permute() on the state @a state;
  *     - SnP_ExtractAndAddBytes() to read a block, XOR it with @a I and store it in @a O;
  *     - SnP_OverwriteBytes() with the block just stored in @a O;
  *     - SnP_AddByte() with a byte 0x01;
  *     - and advancing @a I and @a O until no whole blocks remain.
  * @param  state       Pointer to the state.
  * @param  I           Pointer to the plaintext buffer, containing @a IOLen bytes.
  * @param  O           Pointer to the ciphertext buffer, containing @a IOLen bytes.
  * @param  IOLen       The length of the data in bytes.
  * @returns    The number of bytes processed.
  * @pre  @a IOLen must be at least one block.
  */
size_t Xoodyak_EncryptFullBlocks(Xoodoo_state *state, const uint8_t *I, uint8_t *O, size_t IOLen);

/** Function that decrypts whole blocks using Cyclist's Decrypt()
  * after the first block. The block size is Rkout bytes (24 bytes for Xoodyak).
  *
  * This function has the same behavior as repeatedly calling:
  *     - SnP_Permute() on the state @a state;
  *     - SnP_ExtractAndAddBytes() to read a block, XOR it with @a I and store it in @a O;
  *     - SnP_AddBytes() with the block just stored in @a O;
  *     - SnP_AddByte() with a byte 0x01;
  *     - and advancing @a I and @a O until no whole blocks remain.
  * @param  state       Pointer to the state.
  * @param  I           Pointer to the ciphertext buffer, containing @a IOLen bytes.
  * @param  O           Pointer to the plaintext buffer, containing @a IOLen bytes.
  * @param  IOLen       The length of the data in bytes.
  * @returns    The number of bytes processed.
  * @pre  @a IOLen must be at least one block.
  */
size_t Xoodyak_DecryptFullBlocks(Xoodoo_state *state, const uint8_t *I, uint8_t *O, size_t IOLen);


/** Farfalle functions
  *
  * An implementation may also instantiate functions that optimize the Farfalle construction,
  * in which case the bit SnP_Feature_Farfalle must be active in SnP_GetFeatures().
  *
  * For Xoofff, this means the following functions:
  *     - Xoofff_AddIs
  *     - Xoofff_CompressFastLoop
  *     - Xoofff_ExpandFastLoop
  *
  * See https://tosc.iacr.org/index.php/ToSC/article/view/7359 for more details.
  */

/** Function to XOR an input buffer into an output buffer, with bit granularity.
  * If the bit length is not a multipe of 8, the 8 - (@a bitLen mod 8) most significant
  * bits of the output buffer are set to zero.
  * @param  output      Pointer to the output buffer, containing ceil(@a bitLen / 8) bytes.
  * @param  input       Pointer to the input buffer, containing ceil(@a bitLen / 8) bytes.
  * @param  bitLen      Length in bits of the input and output buffers.
  */
void SnP_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen);

/** Function to absorb whole input blocks and add their contribution to the accumulator
  * according to the Farfalle construction.
  * For Xoofff, the block length is 48 bytes.
  * @param  kRoll       Pointer to the buffer containing the rolling key, updated by this function.
  * @param  xAccu       Pointer to the buffer containing the accumulator, updated by this function.
  * @param  input       Pointer to the input buffer, containing @a length bytes.
  * @param  length      The length of the input in bytes.
  * @returns    The number of bytes absorbed.
  * @pre  @a length must be at least one block.
  */
size_t SnP_CompressFastLoop(unsigned char *kRoll, unsigned char *xAccu, const unsigned char *input, size_t length);

/** Function to squeeze whole input blocks and update the rolling accumulator
  * according to the Farfalle construction.
  * For Xoofff, the block length is 48 bytes.
  * @param  yAccu       Pointer to the buffer containing the rolling accumulator, updated by this function.
  * @param  kRoll       Pointer to the buffer containing the rolling key.
  * @param  output      Pointer to the output buffer, containing @a length bytes.
  * @param  length      The requested length in bytes.
  * @returns    The number of bytes squeezed.
  * @pre  @a length must be at least one block.
  */
size_t SnP_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length);
