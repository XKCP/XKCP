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

#ifndef _PlSnP_h_
#define _PlSnP_h_

/** Function called at least once before any use of the other PlSnP_*
  * functions, possibly to initialize global variables.
  */
void PlSnP_StaticInitialize( void );

/** Function to initialize the state to the logical value 0^width for all parallel state instances.
  * @param  states  Pointer to the states to initialize.
  */
void PlSnP_InitializeAll(void *states);

/** Function to XOR data given as bytes into one of the states.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  data    Pointer to the input data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void PlSnP_XORBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to XOR data given as bytes into all the parallel states in an interleaved fashion.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * First, @a laneCount lanes from @a data are processed in the first state instance
  * and @a data advances by @a laneOffset lanes,
  * then @a laneCount lanes from @a data are processed in the second state instance
  * and @a data advances by @a laneOffset lanes,
  * and so on.
  * @param  states  Pointer to the states.
  * @param  data    Pointer to the input data.
  * @param  laneCount   The number of lanes per state, i.e., the length of the data
  *                     divided by the lane size and by the number of parallel state instances.
  * @param  laneOffset  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @pre    0 ≤ @a laneCount ≤ SnP_laneCount
  */
void PlSnP_XORLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

/** Function to overwrite data given as bytes into one of the states.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  data    Pointer to the input data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void PlSnP_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to overwrite data given as bytes into all the parallel states in an interleaved fashion.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * First, @a laneCount lanes from @a data are processed in the first state instance
  * and @a data advances by @a laneOffset lanes,
  * then @a laneCount lanes from @a data are processed in the second state instance
  * and @a data advances by @a laneOffset lanes,
  * and so on.
  * @param  states  Pointer to the states.
  * @param  data    Pointer to the input data.
  * @param  laneCount   The number of lanes per state, i.e., the length of the data
  *                     divided by the lane size and by the number of parallel state instances.
  * @param  laneOffset  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @pre    0 ≤ @a laneCount ≤ SnP_laneCount
  */
void PlSnP_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

/** Function to overwrite bytes in the one of the states with zeroes.
  * The bits to modify are restricted to start from the bit position 0 and
  * to span a whole number of bytes.
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  byteCount   The number of bytes, i.e., the length of the data
  *                     divided by 8 bits.
  * @pre    0 ≤ @a byteCount ≤ (width in bytes)
  */
void PlSnP_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);

/** Function to complement the value of a given bit in one of the states.
  * This function is typically used to XOR the second bit of the multi-rate
  * padding into the state.
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  position    The position of the bit to complement.
  * @pre    0 ≤ @a position < (width in bits)
  */
void PlSnP_ComplementBit(void *states, unsigned int instanceIndex, unsigned int position);

/** Function to complement the value of a given bit in all the states.
  * This function is typically used to XOR the second bit of the multi-rate
  * padding into the state.
  * @param  states  Pointer to the states.
  * @param  position    The position of the bit to complement.
  * @pre    0 ≤ @a position < (width in bits)
  */
void PlSnP_ComplementBitAll(void *states, unsigned int position);

/** Function to apply Keccak-f on one of the states.
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  */
void PlSnP_Permute(void *states, unsigned int instanceIndex);

/** Function to apply Keccak-f on all the states in parallel.
  * @param  states  Pointer to the states.
  */
void PlSnP_PermuteAll(void *states);

/** Function to retrieve data from one of the states into bytes.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  data    Pointer to the area where to store output data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void PlSnP_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);

/** Function to retrieve data from all the parallel states in an interleaved fashion.
  * The bits to output are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * First, @a laneCount lanes from the first state instance are stored in @a data
  * and @a data advances by @a laneOffset lanes,
  * then, @a laneCount lanes from the second state instance are stored in @a data
  * and @a data advances by @a laneOffset lanes,
  * and so on.
  * @param  states  Pointer to the states.
  * @param  data    Pointer to the area where to store output data.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size and by the number of parallel state instances.
  * @param  laneOffset  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @pre    0 ≤ @a laneCount ≤ SnP_laneCount
  */
void PlSnP_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

/** Function to retrieve data from one of the states and
  * to XOR them into the output buffer.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  data    Pointer to the area where to XOR output data.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void PlSnP_ExtractAndXORBytes(const void *states, unsigned int instanceIndex,  unsigned char *data, unsigned int offset, unsigned int length);

/** Function to retrieve data from all the parallel states in an interleaved fashion and
  * to XOR them into the output buffer.
  * The bits to output are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * First, @a laneCount lanes from the first state instance are XORed into @a data
  * and @a data advances by @a laneOffset lanes,
  * then, @a laneCount lanes from the second state instance are XORed into @a data
  * and @a data advances by @a laneOffset lanes,
  * and so on.
  * @param  states  Pointer to the states.
  * @param  data    Pointer to the area where to XOR output data.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size and by the number of parallel state instances.
  * @param  laneOffset  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @pre    0 ≤ @a laneCount ≤ SnP_laneCount
  */
void PlSnP_ExtractAndXORLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

/** Function that has the same behavior as repeatedly calling
  *  - PlSnP_XORLanesAll() with P blocks of @a laneCount lanes from @a data and with offset @a laneOffsetParallel;
  *  - PlSnP_XORBytesInLane() with the byte @a trailingBits at offset 0 of lane index @a laneCount for each instance;
  *  - PlSnP_PermuteAll() on the states @a states;
  *  - and advancing @a data by P times @a laneOffsetSerial lane sizes, until not enough data are available.
  * The function returns the total offset of the @a data pointer or, equivalently,
  * the number of iterations times @a laneOffsetSerial lane sizes in bytes.
  * @param  states  Pointer to the states.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  laneOffsetParallel  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @param  laneOffsetSerial    The number of lanes that separate the blocks of data
  *                     between each iteration.
  * @param  data    Pointer to the data to use as input.
  * @param  dataByteLen The length of the input data in bytes.
  * @param  trailingBits    The byte to XOR at the end of each block.
  * @returns    The total offset.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t PlSnP_FBWL_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen, unsigned char trailingBits);

/** Function that has the same behavior as repeatedly calling
  *  - PlSnP_PermuteAll() on the states @a states;
  *  - PlSnP_ExtractLanesAll() with P blocks of @a laneCount lanes to @a data and with offset @a laneOffsetParallel;
  *  - and advancing @a data by P times @a laneOffsetSerial lane sizes, until not enough room is available.
  * The function returns the total offset of the @a data pointer or, equivalently,
  * the number of iterations times @a laneOffsetSerial lane sizes in bytes.
  * @param  states  Pointer to the states.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  laneOffsetParallel  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @param  laneOffsetSerial    The number of lanes that separate the blocks of data
  *                     between each iteration.
  * @param  data    Pointer to the area to use as output.
  * @param  dataByteLen The length of the output area in bytes.
  * @returns    The total offset.
  * @pre    0 < @a laneCount ≤ SnP_laneCount
  */
size_t PlSnP_FBWL_Squeeze(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen);

/** Function that has the same behavior as repeatedly calling
  *  - PlSnP_ExtractAndXORLanesAll() with P blocks of @a laneCount lanes from @a dataIn to @a dataOut and with offset @a laneOffsetParallel;
  *  - PlSnP_OverwriteLanesAll() with the P blocks of @a laneCount lanes just produced in @a dataOut and with offset @a laneOffsetParallel;
  *  - PlSnP_XORBytesInLane() with the byte @a trailingBits at offset 0 of lane index @a laneCount for each instance;
  *  - PlSnP_PermuteAll() on the states @a states;
  *  - and advancing @a dataIn and @a dataOut by P times @a laneOffsetParallel lane sizes, until not enough data are available.
  * The function returns the total offset of the @a data pointer or, equivalently,
  * the number of iterations times @a laneOffsetSerial lane sizes in bytes.
  * @param  states  Pointer to the states.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  laneOffsetParallel  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @param  laneOffsetSerial    The number of lanes that separate the blocks of data
  *                     between each iteration.
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataOut Pointer to the area to use as output. This can be the same as @a dataIn.
  * @param  dataByteLen The length of the input data and of the output area in bytes.
  * @param  trailingBits    The byte to XOR at the end of each block.
  * @returns    The total offset.
  * @note   @a dataIn and @a dataOut can point to the same buffer, in which case
  *         the data are processed in-place.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t PlSnP_FBWL_Wrap(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

/** Function that has the same behavior as repeatedly calling
  *  - PlSnP_ExtractAndXORLanesAll() with P blocks of @a laneCount lanes from @a dataIn to @a dataOut and with offset @a laneOffsetParallel;
  *  - PlSnP_XORLanesAll() with the P blocks of @a laneCount lanes just produced in @a dataOut and with offset @a laneOffsetParallel;
  *  - PlSnP_XORBytesInLane() with the byte @a trailingBits at offset 0 of lane index @a laneCount for each instance;
  *  - PlSnP_PermuteAll() on the states @a states;
  *  - and advancing @a dataIn and @a dataOut by P times @a laneOffsetParallel lane sizes, until not enough data are available.
  * The function returns the total offset of the @a data pointer or, equivalently,
  * the number of iterations times @a laneOffsetSerial lane sizes in bytes.
  * @param  states  Pointer to the states.
  * @param  laneCount   The number of lanes processed each time (i.e., the block size in lanes).
  * @param  laneOffsetParallel  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @param  laneOffsetSerial    The number of lanes that separate the blocks of data
  *                     between each iteration.
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataOut Pointer to the area to use as output. This can be the same as @a dataIn.
  * @param  dataByteLen The length of the input data and of the output area in bytes.
  * @param  trailingBits    The byte to XOR at the end of each block.
  * @returns    The total offset.
  * @note   @a dataIn and @a dataOut can point to the same buffer, in which case
  *         the data are processed in-place.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t PlSnP_FBWL_Unwrap(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

#endif
