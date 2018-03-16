/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

/** General information
  *
  * The following type and functions are not actually implemented. Their
  * documentation is generic, with the prefix Prefix replaced by
  * - KeccakWidth200 for a duplex object based on Keccak-f[200]
  * - KeccakWidth400 for a duplex object based on Keccak-f[400]
  * - KeccakWidth800 for a duplex object based on Keccak-f[800]
  * - KeccakWidth1600 for a duplex object based on Keccak-f[1600]
  *
  * In all these functions, the rate and capacity must sum to the width of the
  * chosen permutation. For instance, to use the duplex object
  * Keccak[r=1346, c=254], one must use the KeccakWidth1600_Duplex* functions.
  *
  * The Prefix_DuplexInstance contains the duplex instance attributes for use
  * with the Prefix_Duplex* functions.
  * It gathers the state processed by the permutation as well as the rate,
  * the position of input/output bytes in the state in case of partial
  * input or output.
  */

#ifdef DontReallyInclude_DocumentationOnly
/**
  * Structure that contains the duplex instance for use with the
  * Prefix_Duplex* functions.
  * It gathers the state processed by the permutation as well as
  * the rate.
  */
typedef struct Prefix_DuplexInstanceStruct {
    /** The state processed by the permutation. */
    unsigned char state[SnP_stateSizeInBytes];
    /** The value of the rate in bits.*/
    unsigned int rate;
    /** The position in the state of the next byte to be input. */
    unsigned int byteInputIndex;
    /** The position in the state of the next byte to be output. */
    unsigned int byteOutputIndex;
} Prefix_DuplexInstance;

/**
  * Function to initialize a duplex object Duplex[Keccak-f[r+c], pad10*1, r].
  * @param  duplexInstance  Pointer to the duplex instance to be initialized.
  * @param  rate        The value of the rate r.
  * @param  capacity    The value of the capacity c.
  * @pre    One must have r+c equal to the supported width of this implementation.
  * @pre    3 ≤ @a rate ≤ width, and otherwise the value of the rate is unrestricted.
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexInitialize(Prefix_DuplexInstance *duplexInstance, unsigned int rate, unsigned int capacity);

/**
  * Function to make a duplexing call to the duplex object initialized
  * with Prefix_DuplexInitialize().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  sigmaBegin  Pointer to the first part of the input σ given as bytes.
  *                     Trailing bits are given in @a delimitedSigmaEnd.
  * @param  sigmaBeginByteLen   The number of input bytes provided in @a sigmaBegin.
  * @param  Z           Pointer to the buffer where to store the output data Z.
  * @param  ZByteLen    The number of output bytes desired for Z.
  *                     If @a ZByteLen*8 is greater than the rate r,
  *                     the last byte contains only r modulo 8 bits,
  *                     in the least significant bits.
  * @param  delimitedSigmaEnd   Byte containing from 0 to 7 trailing bits that must be
  *                     appended to the input data in @a sigmaBegin.
  *                     These <i>n</i>=|σ| mod 8 bits must be in the least significant bit positions.
  *                     These bits must be delimited with a bit 1 at position <i>n</i>
  *                     (counting from 0=LSB to 7=MSB) and followed by bits 0
  *                     from position <i>n</i>+1 to position 7.
  *                     Some examples:
  *                         - If |σ| is a multiple of 8, then @a delimitedSigmaEnd must be 0x01.
  *                         - If |σ| mod 8 is 1 and the last bit is 1 then @a delimitedSigmaEnd must be 0x03.
  *                         - If |σ| mod 8 is 4 and the last 4 bits are 0,0,0,1 then @a delimitedSigmaEnd must be 0x18.
  *                         - If |σ| mod 8 is 6 and the last 6 bits are 1,1,1,0,0,1 then @a delimitedSigmaEnd must be 0x67.
  *                     .
  * @note   The input bits σ are the result of the concatenation of
  *                     the bytes given in Prefix_DuplexingFeedPartialInput()
  *                     calls since the last call to Prefix_Duplexing(),
  *                     the bytes in @a sigmaBegin
  *                     and the bits in @a delimitedSigmaEnd before the delimiter.
  * @pre    @a delimitedSigmaEnd ≠ 0x00
  * @pre    @a sigmaBeginByteLen*8+<i>n</i> (+ length of previously queued data) ≤ (r-2)
  * @pre    @a ZByteLen ≤ ceil(r/8)
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_Duplexing(Prefix_DuplexInstance *duplexInstance, const unsigned char *sigmaBegin, unsigned int sigmaBeginByteLen, unsigned char *Z, unsigned int ZByteLen, unsigned char delimitedSigmaEnd);

/**
  * Function to queue input data that will subsequently used in the next
  * call to Prefix_Duplexing().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  input           Pointer to the bytes to queue.
  * @param  inputByteLen    The number of input bytes provided in @a input.
  * @pre    The total number of input bytes since the last Prefix_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexingFeedPartialInput(Prefix_DuplexInstance *duplexInstance, const unsigned char *input, unsigned int inputByteLen);

/**
  * Function to queue input data that will subsequently used in the next
  * call to Prefix_Duplexing(), where the data here consist of all-zero bytes.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  inputByteLen    The number of input bytes 0x00 to feed.
  * @pre    The total number of input bytes since the last Prefix_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexingFeedZeroes(Prefix_DuplexInstance *duplexInstance, unsigned int inputByteLen);

/**
  * Function to queue input data that will subsequently used in the next
  * call to Prefix_Duplexing(), with the additional pre-processing that
  * the input data is first bitwise added with the output data of the previous duplexing
  * call at the same offset.
  * In practice, this comes down to overwriting the input data in the state
  * of the duplex object.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  input           Pointer to the bytes to queue, before they are XORed.
  * @param  inputByteLen    The number of input bytes provided in @a input.
  * @pre    The total number of input bytes since the last Prefix_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexingOverwritePartialInput(Prefix_DuplexInstance *duplexInstance, const unsigned char *input, unsigned int inputByteLen);

/**
  * Function to queue input data for the next call to Prefix_Duplexing() that
  * is equal to the output data of the previous duplexing call at the same offset.
  * In practice, this comes down to overwriting with zeroes the state
  * of the duplex object.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  inputByteLen    The number of bytes to overwrite with zeroes.
  * @pre    No input data may have been queued since the last call
  *         to Prefix_Duplexing().
  * @pre    The total number of input bytes since the last Prefix_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexingOverwriteWithZeroes(Prefix_DuplexInstance *duplexInstance, unsigned int inputByteLen);

/**
  * Function to fetch output data beyond those that were already output since
  * the last call to Prefix_Duplexing().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  output          Pointer to the buffer where to store the output data.
  * @param  outputByteLen   The number of output bytes desired.
  * @pre    The total number of output bytes, taken since (and including in)
  *         the last call to Prefix_Duplexing() cannot be higher than ceil(r/8).
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexingGetFurtherOutput(Prefix_DuplexInstance *duplexInstance, unsigned char *out, unsigned int outByteLen);

/**
  * Function to fetch output data beyond those that were already output since
  * the last call to Prefix_Duplexing(), with the additional post-processing
  * that this data is bitwise added with the given input buffer
  * before it is stored into the given output buffer.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Prefix_DuplexInitialize().
  * @param  input           Pointer to the input buffer.
  * @param  output          Pointer to the output buffer, which may be equal to @a input.
  * @param  outputByteLen   The number of output bytes desired.
  * @pre    The total number of output bytes, taken since (and including in)
  *         the last call to Prefix_Duplexing() cannot be higher than ceil(r/8).
  * @return Zero if successful, 1 otherwise.
  */
int Prefix_DuplexingGetFurtherOutputAndAdd(Prefix_DuplexInstance *duplexInstance, const unsigned char *input, unsigned char *output, unsigned int outputByteLen);
#endif
