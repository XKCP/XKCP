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
  * SnP stands for "state and permutation". It provides a set of function to
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
  *  - KeccakP200 for Keccak-p[200]
  *  - KeccakP400 for Keccak-p[400]
  *  - KeccakP800 for Keccak-p[800]
  *  - KeccakP1600 for Keccak-p[1600]
  * Functions can be replaced by macros, when appropriate.
  *
  * An implementation must provide each function (or macro) as listed below,
  * instantiated with the suitable prefix. In addition, the implementation
  * must come with a header "KeccakP-800-SnP.h", "KeccakP-1600-SnP.h" or similar
  * to define the following symbols (with SnP replaced by the appropriate prefix):
  *     - SnP_implementation: a synopsis of the implementation;
  *     - SnP_stateSizeInBytes: the number of bytes to store the state;
  *     - SnP_stateAlignment: the number of bytes of which the state address
  *         must be a multiple of.
  *
  * An implementation may also instantiate the function SnP_FastLoop_Absorb(),
  * in which case the header must define the symbol SnP_FastLoop_supported.
  *
  *  For Keccak-p[200]:
  *     - SnP_Permute() is instantiated as KeccakP200_Permute_18rounds()
  *         for Keccak-f[200]=Keccak-p[200, 18].
  *     - SnP_Permute_Nrounds() is instantiated as KeccakP200_Permute_Nrounds().
  *     - SnP_FastLoop_Absorb() can be instantiated as KeccakF200_FastLoop_Absorb()
  *         for Keccak-f[200].
  *
  *  For Keccak-p[400]:
  *     - SnP_Permute() is instantiated as KeccakP400_Permute_20rounds()
  *         for Keccak-f[400]=Keccak-p[400, 20].
  *     - SnP_Permute_Nrounds() is instantiated as KeccakP400_Permute_Nrounds().
  *     - SnP_FastLoop_Absorb() can be instantiated as KeccakF400_FastLoop_Absorb()
  *         for Keccak-f[400].
  *
  * For Keccak-p[800]:
  *     - SnP_Permute() is instantiated as
  *         - KeccakP800_Permute_12rounds() for Keccak-p[800, 12] and
  *         - KeccakP800_Permute_22rounds() for Keccak-f[800]=Keccak-p[800, 22].
  *     - SnP_Permute_Nrounds() is instantiated as KeccakP800_Permute_Nrounds().
  *     - SnP_FastLoop_Absorb() can be instantiated as KeccakF800_FastLoop_Absorb() for Keccak-f[800].
  *
  * For Keccak-p[1600]:
  *     - SnP_Permute() is instantiated as
  *         - KeccakP1600_Permute_12rounds() for Keccak-p[1600, 12] and
  *         - KeccakP1600_Permute_24rounds() for Keccak-f[1600]=Keccak-p[1600, 24].
  *     - SnP_Permute_Nrounds() is instantiated as KeccakP1600_Permute_Nrounds().
  *     - SnP_FastLoop_Absorb() can be instantiated as KeccakF1600_FastLoop_Absorb() for Keccak-f[1600].
  */

/** Function called at least once before any use of the other SnP_*
  * functions, possibly to initialize global variables.
  */
void SnP_StaticInitialize( void );

/** Function to initialize the state to the logical value 0^width.
  * @param  state   Pointer to the state to initialize.
  */
void SnP_Initialize(void *state);

/** Function to add (in GF(2), using bitwise exclusive-or) a given byte into the state.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + 8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  data    The input byte.
  * @param  offset  Offset in bytes within the state.
  * @pre    0 ≤ @a offset < (width in bytes)
  */
void SnP_AddByte(void *state, unsigned char data, unsigned int offset);

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
void SnP_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);

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
void SnP_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to overwrite bytes in the state with zeroes.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of bytes.
  * @param  state   Pointer to the state.
  * @param  byteCount   The number of bytes, i.e., the length of the data
  *                     divided by 8 bits.
  * @pre    0 ≤ @a byteCount ≤ (width in bytes)
  */
void SnP_OverwriteWithZeroes(void *state, unsigned int byteCount);

/** Function to apply the permutation on the state.
  * @param  state   Pointer to the state.
  */
void SnP_Permute(void *state);

/** Function to apply on the state the permutation with the given number of rounds
  * among the permutation family.
  * @param  state   Pointer to the state.
  */
void SnP_Permute_Nrounds(void *state, unsigned int nrounds);

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
void SnP_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);

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
void SnP_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

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
