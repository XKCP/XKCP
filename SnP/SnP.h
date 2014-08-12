/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _SnP_h_
#define _SnP_h_

/** Function called at least once before any use of the other SnP_*
  * functions, possibly to initialize global variables.
  */
void SnP_StaticInitialize( void );

/** Function to initialize the state to the logical value 0^width.
  * @param  state   Pointer to the state to initialize.
  */
void SnP_Initialize(void *state);

/** Function to XOR data given as bytes into the state.
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
void SnP_XORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);

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

/** Function to complement the value of a given bit in the state.
  * This function is typically used to XOR the second bit of the multi-rate
  * padding into the state.
  * @param  state   Pointer to the state.
  * @param  position    The position of the bit to complement.
  * @pre    0 ≤ @a position < (width in bits)
  */
void SnP_ComplementBit(void *state, unsigned int position);

/** Function to apply Keccak-f on the state.
  * @param  state   Pointer to the state.
  */
void SnP_Permute(void *state);

/** Function to retrieve data from the state into bytes.
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

/** Function to retrieve data from the state into bytes and
  * to XOR them into the output buffer.
  * The bit positions that are retrieved by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  state   Pointer to the state.
  * @param  data    Pointer to the area where to XOR output data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void SnP_ExtractAndXORBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);

/** Function that has the same behavior as repeatedly calling
  *  - SnP_XORBytes() with a block of @a laneCount lanes from data;
  *  - SnP_XORBytes() with the byte @a trailingBits at offset 0 of lane index @a laneCount;
  *  - SnP_Permute() on the state @a state;
  *  - and advancing @a data by @a laneCount lane sizes, until not enough data are available.
  * The function returns the number of bytes processed from @a data.
  * @param  state   Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  data    Pointer to the data to use as input.
  * @param  dataByteLen The length of the input data in bytes.
  * @param  trailingBits    The byte to XOR at the end of each block.
  * @returns    The number of bytes processed.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t SnP_FBWL_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen, unsigned char trailingBits);

/** Function that has the same behavior as repeatedly calling
  *  - SnP_Permute() on the state @a state;
  *  - SnP_ExtractBytes() with a block of @a laneCount lanes to data;
  *  - and advancing @a data by @a laneCount lane sizes, until not enough room is available.
  * The function returns the number of bytes produced in @a data.
  * @param  state   Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  data    Pointer to the area to use as output.
  * @param  dataByteLen The length of the output area in bytes.
  * @returns    The number of bytes produced.
  * @pre    0 < @a laneCount ≤ SnP_laneCount
  */
size_t SnP_FBWL_Squeeze(void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen);

/** Function that has the same behavior as repeatedly calling
  *  - SnP_ExtractAndXORBytes() with a block of @a laneCount lanes from @a dataIn to @a dataOut;
  *  - SnP_OverwriteBytes() with the block of @a laneCount lanes just produced in @a dataOut;
  *  - SnP_XORBytes() with the byte @a trailingBits at offset 0 of lane index @a laneCount;
  *  - SnP_Permute() on the state @a state;
  *  - and advancing @a dataIn and @a dataOut by @a laneCount lane sizes, until not enough data are available.
  * The function returns the number of bytes processed from @a dataIn.
  * @param  state   Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataOut Pointer to the area to use as output. This can be the same as @a dataIn.
  * @param  dataByteLen The length of the input data and of the output area in bytes.
  * @param  trailingBits    The byte to XOR at the end of each block.
  * @returns    The number of bytes processed and produced.
  * @note   @a dataIn and @a dataOut can point to the same buffer, in which case
  *         the data are processed in-place.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t SnP_FBWL_Wrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

/** Function that has the same behavior as repeatedly calling
  *  - SnP_ExtractAndXORBytes() with a block of @a laneCount lanes from @a dataIn to @a dataOut;
  *  - SnP_XORBytes() with the block of @a laneCount lanes just produced in @a dataOut;
  *  - SnP_XORBytes() with the byte @a trailingBits at offset 0 of lane index @a laneCount;
  *  - SnP_Permute() on the state @a state;
  *  - and advancing @a dataIn and @a dataOut by @a laneCount lane sizes, until not enough data are available.
  * The function returns the number of bytes processed from @a dataIn.
  * @param  state   Pointer to the state.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataOut Pointer to the area to use as output. This can be the same as @a dataIn.
  * @param  dataByteLen The length of the input data and of the output area in bytes.
  * @param  trailingBits    The byte to XOR at the end of each block.
  * @returns    The number of bytes processed and produced.
  * @note   @a dataIn and @a dataOut can point to the same buffer, in which case
  *         the data are processed in-place.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t SnP_FBWL_Unwrap(void *state, unsigned int laneCount, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
