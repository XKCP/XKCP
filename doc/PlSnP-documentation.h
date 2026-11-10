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
  * PlSnP stands for "parallel states and permutations". It is similar to SnP
  * (see SnP-documentation.h), although it specifically provides functions
  * to access an array of states that evolve in parallel.
  *
  * The following functions are not actually implemented. Their documentation
  * is generic, with the prefix PlSnP replaced by
  *  - KeccakP1600times2 for Keccak-p[1600]×2 (i.e., two states of 1600 bits each)
  *  - KeccakP1600times4 for Keccak-p[1600]×4
  *  - KeccakP1600times8 for Keccak-p[1600]×8 (i.e., eight states of 1600 bits each)
  * - Xoodootimes4 for Xoodoo×4 (i.e., four states of 384 bits each)
  * - Xoodootimes8 for Xoodoo×8
  * - Xoodootimes16 for Xoodoo×16 (i.e., sixteen states of 384 bits each)
  * Functions can be replaced by macros, when appropriate.
  *
  * An implementation must provide each function (or macro) as listed below,
  * instantiated with the suitable prefix. In addition, the implementation
  * must come with a header "KeccakP-1600-times{2,4,8}-SnP.h"
  * or "Xoodoo-times{4, 8, 16}-SnP.h"
  * that will be included by the users of the corresponding permutation.
  */

/** Administrative functions
 */

/** Function that returns a description of the implementation.
 */
const char * PlSnP_GetImplementation();

/** Function that returns the set of features that are implemented for this permutation.
 * The return value is a bit mask that can be the sum of the following values
 * defined in "PlSnP-common.h" (here the PlSnP prefix is to be taken litterally):
 *   - PlSnP_Feature_Main
 *   - PlSnP_Feature_SpongeAbsorb
 *   - PlSnP_Feature_Farfalle
 *   - PlSnP_Feature_KangarooTwelve
 *   - PlSnP_Feature_FlightDeck
 *
 * Each of these features corresponds to a set of functions as detailed below.
 */
int PlSnP_GetFeatures();


/** Main functions
  *
  * The following functions are the main PlSnP functions.
  * If supported, the PlSnP_Feature_Main flag must be present in PlSnP_GetFeatures().
  *
  * For Keccak-p[1600]:
  *     - PlSnP_PermuteAll() is instantiated as
  *         - KeccakP1600times{2,4,8}_PermuteAll_12rounds() for Keccak-p[1600, 12] and
  *         - KeccakP1600times{2,4,8}_PermuteAll_24rounds() for Keccak-f[1600]=Keccak-p[1600, 24].
  * For Xoodoo:
  *     - PlSnP_PermuteAll() is instantiated as
  *         - Xoodootimes{4,8,16}_PermuteAll_6rounds() for Xoodoo[6] and
  *         - Xoodootimes{4,8,16}_PermuteAll_12rounds() for Xoodoo[12].
  */

/** Function called at least once before any use of the other PlSnP_*
  * functions, possibly to initialize global variables.
  */
void PlSnP_StaticInitialize( void );

/** Function to initialize the state to the logical value 0^width for all parallel state instances.
  * @param  states  Pointer to the states to initialize.
  */
void PlSnP_InitializeAll(void *states);

/** Function to add (in GF(2), using bitwise exclusive-or) a given byte into one of the states.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + 8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  data    The input byte.
  * @param  offset  Offset in bytes within the state.
  * @pre    0 ≤ @a offset < (width in bytes)
  */
void PlSnP_AddByte(void *states, unsigned int instanceIndex, unsigned char data, unsigned int offset);

/** Function to add (in GF(2), using bitwise exclusive-or) data given as bytes into one of the states.
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
void PlSnP_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);

/** Function to add (in GF(2), using bitwise exclusive-or) data given as bytes into all the parallel states in an interleaved fashion.
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
void PlSnP_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

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

/** Function to apply the permutation on all the states in parallel.
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

/** Function to retrieve data from one of the states,
  * to add  (in GF(2), using bitwise exclusive-or) them to the input buffer,
  * and to store the result in the output buffer.
  * The bit positions that are affected by this function are
  * from @a offset*8 to @a offset*8 + @a length*8.
  * (The bit positions, the x,y,z coordinates and their link are defined in the "Keccak reference".)
  * @param  states  Pointer to the states.
  * @param  instanceIndex   Index of the state instance from 0 to P-1
  *                         if there are P parallel instances.
  * @param  input   Pointer to the input buffer.
  * @param  output  Pointer to the output buffer.
  * @param  offset  Offset in bytes within the state.
  * @param  length  Number of bytes.
  * @pre    0 ≤ @a offset < (width in bytes)
  * @pre    0 ≤ @a offset + @a length ≤ (width in bytes)
  */
void PlSnP_ExtractAndAddBytes(const void *states, unsigned int instanceIndex, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

/** Function to retrieve data from all the parallel states in an interleaved fashion
  * to add  (in GF(2), using bitwise exclusive-or) them to the input buffer,
  * and to store the result in the output buffer.
  * The bits to output are restricted to start from the bit position 0 and
  * to span a whole number of lanes.
  * First, @a laneCount lanes from the first state instance are added to @a input into @a output
  * and @a input and @a output each advance by @a laneOffset lanes,
  * then, @a laneCount lanes from the second state instance are added to @a input into @a output
  * and @a input and @a output each advance by @a laneOffset lanes,
  * and so on.
  * @param  states  Pointer to the states.
  * @param  input   Pointer to the input buffer.
  * @param  output  Pointer to the output buffer.
  * @param  laneCount   The number of lanes, i.e., the length of the data
  *                     divided by the lane size and by the number of parallel state instances.
  * @param  laneOffset  The number of lanes that separate the blocks of data
  *                     for each instance.
  * @pre    0 ≤ @a laneCount ≤ SnP_laneCount
  */
void PlSnP_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);


/** Sponge absorb functions
  *
  * An implementation may also instantiate the function PlSnP_FastLoop_Absorb(),
  * in which case the bit PlSnP_Feature_SpongeAbsorb must be active in PlSnP_GetFeatures().
  *
  * For Keccak-p[1600]:
  *     - PlSnP_PermuteAll() is instantiated as
  *         - KeccakP1600_PermuteAll_12rounds() for Keccak-p[1600, 12] and
  *         - KeccakP1600_PermuteAll_24rounds() for Keccak-f[1600]=Keccak-p[1600, 24].
  *     - PlSnP_FastLoop_Absorb() can be instantiated as
  *         KeccakF1600times{2,4,8}_FastLoop_Absorb() for Keccak-f[1600].
  */

/** Function that has the same behavior as repeatedly calling
  *  - PlSnP_AddLanesAll() with P blocks of @a laneCount lanes from @a data and with offset @a laneOffsetParallel;
  *  - PlSnP_PermuteAll() on the states @a states;
  *  - and advancing @a data by @a laneOffsetSerial lane sizes, until not enough data are available.
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
  * @return The total offset.
  * @pre    0 < @a laneCount < SnP_laneCount
  */
size_t PlSnP_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, unsigned char *data, size_t dataByteLen);


/** Farfalle functions
  *
  * An implementation may also instantiate functions that optimize the Farfalle construction,
  * in which case the bit PlSnP_Feature_Farfalle must be active in PlSnP_GetFeatures().
  *
  * For Kravatte, this means the following functions:
  *     - KeccakP1600times{2,4,8}_KravatteCompress
  *     - KeccakP1600times{2,4,8}_KravatteExpand
  * For Xoofff, this means the following functions:
  *     - Xooffftimes{4,8,16}_AddIs
  *     - Xooffftimes{4,8,16}_CompressFastLoop
  *     - Xooffftimes{4,8,16}_ExpandFastLoop
  *
  * See https://tosc.iacr.org/index.php/ToSC/article/view/855 and https://tosc.iacr.org/index.php/ToSC/article/view/7359 for more details.
  */

/** Function to XOR an input buffer into an output buffer, with bit granularity.
  * If the bit length is not a multipe of 8, the 8 - (@a bitLen mod 8) most significant
  * bits of the output buffer are set to zero.
  * @param  output      Pointer to the output buffer, containing ceil(@a bitLen / 8) bytes.
  * @param  input       Pointer to the input buffer, containing ceil(@a bitLen / 8) bytes.
  * @param  bitLen      Length in bits of the input and output buffers.
  */
void PlSnP_AddIs(unsigned char *output, const unsigned char *input, size_t bitLen);

/** Function to absorb whole input blocks and add their contribution to the accumulator
  * according to the Farfalle construction.
  * For Kravatte, the block length is 200 bytes.
  * For Xoofff, the block length is 48 bytes.
  * @param  kRoll       Pointer to the buffer containing the rolling key, updated by this function.
  * @param  xAccu       Pointer to the buffer containing the accumulator, updated by this function.
  * @param  input       Pointer to the input buffer, containing @a length bytes.
  * @param  length      The length of the input in bytes.
  * @return The number of bytes absorbed.
  * @pre  @a length must be at least one block.
  */
size_t PlSnP_CompressFastLoop(unsigned char *kRoll, unsigned char *xAccu, const unsigned char *input, size_t length);

/** Function to squeeze whole input blocks and update the rolling accumulator
  * according to the Farfalle construction.
  * For Kravatte, the block length is 200 bytes.
  * For Xoofff, the block length is 48 bytes.
  * @param  yAccu       Pointer to the buffer containing the rolling accumulator, updated by this function.
  * @param  kRoll       Pointer to the buffer containing the rolling key.
  * @param  output      Pointer to the output buffer, containing @a length bytes.
  * @param  length      The requested length in bytes.
  * @return The number of bytes squeezed.
  * @pre  @a length must be at least one block.
  */
size_t PlSnP_ExpandFastLoop(unsigned char *yAccu, const unsigned char *kRoll, unsigned char *output, size_t length);


/** KangarooTwelve optimizing functions
  *
  * An implementation may also instantiate functions that optimize KT128 and KT256,
  * in which case the bit PlSnP_Feature_KangarooTwelve must be active in PlSnP_GetFeatures().
  */

/** Function that processes N leaves of KT128 and outputs N chaining values,
  * with N the number of permutations computed in parallel.
  * @param  input       Pointer to the buffer containing N leaves, that is N*8192 bytes.
  * @param  output      Pointer to the buffer to store the N chaining values, that is N*32 bytes.
  */
void PlSnP_KT128ProcessLeaves(const unsigned char *input, unsigned char *output);

/** Function that processes N leaves of KT256 and outputs N chaining values,
  * with N the number of permutations computed in parallel.
  * @param  input       Pointer to the buffer containing N leaves, that is N*8192 bytes.
  * @param  output      Pointer to the buffer to store the N chaining values, that is N*64 bytes.
  */
void PlSnP_KT256ProcessLeaves(const unsigned char *input, unsigned char *output);


/** FlightDeck optimizing functions
  *
  * An implementation may also instantiate functions that optimize
  * [Turbo]SHAKE{128, 256}-FlightDeck, in which case the bit PlSnP_Feature_FlightDeck
  * must be active in PlSnP_GetFeatures().
  */

/** Functions that process N leaves of [Turbo]SHAKE{128, 256}-FlightDeck and outputs N chaining values,
  * with N the number of permutations computed in parallel.
  * @param  Dlast       Pointer to the last 40 ([Turbo]SHAKE128) or 72 ([TurboSHAKE]256) bytes
  *                     of the state of the OD object D.
  * @param  input       Pointer to the buffer containing N leaves, that is N*α bytes.
  * @param  output      Pointer to the buffer to store the N chaining values,
  *                     that is N*32 ([Turbo]SHAKE128) or N*64 ([Turbo]SHAKE256) bytes.
  * @return The total number of bytes processed (N*α) or 0 if this function is not supported.
  */
size_t PlSnP_TurboSHAKE128_FlightDeckProcessLeaves(const unsigned char *Dlast, const unsigned char *input, unsigned char *output);
size_t PlSnP_TurboSHAKE256_FlightDeckProcessLeaves(const unsigned char *Dlast, const unsigned char *input, unsigned char *output);
size_t PlSnP_SHAKE128_FlightDeckProcessLeaves(const unsigned char *Dlast, const unsigned char *input, unsigned char *output);
size_t PlSnP_SHAKE256_FlightDeckProcessLeaves(const unsigned char *Dlast, const unsigned char *input, unsigned char *output);

/** Functions that squeeze output bytes from a [Turbo]SHAKE{128, 256}-FlightDeck object.
  * @param  Dlast       Pointer to the last 40 ([Turbo]SHAKE128) or 72 ([TurboSHAKE]256) bytes
  *                     of the state of the OD object D.
  * @param  offset      The value of the offset o at which to start squeezing.
  * @param  odata       Pointer to the output buffer.
  * @param  olen        The number of output bytes requested.
  * @param  oadded      The null pointer,
  *                     or a pointer to a buffer of @a olen bytes that are XORed to the output stream.
  * @pre    @a offset must be a multiple of ρ (160 for [Turbo]SHAKE128, 128 for [Turbo]SHAKE256).
  * @return The actual number of bytes output, either the largest multipe of N*ρ not greater than @a olen
  *         or 0 if @a offset is not a multiple of ρ or if this function is not supported.
  */
size_t PlSnP_TurboSHAKE128_FlightDeckSqueeze(const unsigned char *Dlast, size_t offset, unsigned char *odata, size_t olen, const unsigned char *oadded);
size_t PlSnP_TurboSHAKE256_FlightDeckSqueeze(const unsigned char *Dlast, size_t offset, unsigned char *odata, size_t olen, const unsigned char *oadded);
size_t PlSnP_SHAKE128_FlightDeckSqueeze(const unsigned char *Dlast, size_t offset, unsigned char *odata, size_t olen, const unsigned char *oadded);
size_t PlSnP_SHAKE256_FlightDeckSqueeze(const unsigned char *Dlast, size_t offset, unsigned char *odata, size_t olen, const unsigned char *oadded);
