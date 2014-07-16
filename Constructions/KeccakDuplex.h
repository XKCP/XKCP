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

#ifndef _KeccakDuplex_h_
#define _KeccakDuplex_h_

#include "SnP-interface.h"

#ifdef ALIGN
#undef ALIGN
#endif

#if defined(__GNUC__)
#define ALIGN __attribute__ ((aligned(32)))
#elif defined(_MSC_VER)
#define ALIGN __declspec(align(32))
#else
#define ALIGN
#endif

/**
  * Structure that contains the duplex instance for use with the
  * Keccak_Duplex* functions.
  * It gathers the state processed by the permutation as well as
  * the rate.
  */
ALIGN typedef struct Keccak_DuplexInstanceStruct {
    /** The state processed by the permutation. */
    ALIGN unsigned char state[SnP_stateSizeInBytes];
    /** The value of the rate in bits.*/
    unsigned int rate;
    /** The position in the state of the next byte to be input. */
    unsigned int byteInputIndex;
    /** The position in the state of the next byte to be output. */
    unsigned int byteOutputIndex;
} Keccak_DuplexInstance;

/**
  * Function to initialize a duplex object Duplex[Keccak-f[r+c], pad10*1, r].
  * @param  duplexInstance  Pointer to the duplex instance to be initialized.
  * @param  rate        The value of the rate r.
  * @param  capacity    The value of the capacity c.
  * @pre    One must have r+c equal to the supported width of this implementation.
  * @pre    3 ≤ @a rate ≤ width, and otherwise the value of the rate is unrestricted.
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexInitialize(Keccak_DuplexInstance *duplexInstance, unsigned int rate, unsigned int capacity);

/**
  * Function to make a duplexing call to the duplex object initialized 
  * with Keccak_DuplexInitialize().
  * @param  duplexInstance  Pointer to the duplex instance initialized 
  *                     by Keccak_DuplexInitialize().
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
  *                     the bytes given in Keccak_DuplexingFeedPartialInput()
  *                     calls since the last call to Keccak_Duplexing(),
  *                     the bytes in @a sigmaBegin
  *                     and the bits in @a delimitedSigmaEnd before the delimiter.
  * @pre    @a delimitedSigmaEnd ≠ 0x00
  * @pre    @a sigmaBeginByteLen*8+<i>n</i> (+ length of previously queued data) ≤ (r-2)
  * @pre    @a ZByteLen ≤ ceil(r/8)
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_Duplexing(Keccak_DuplexInstance *duplexInstance, const unsigned char *sigmaBegin, unsigned int sigmaBeginByteLen, unsigned char *Z, unsigned int ZByteLen, unsigned char delimitedSigmaEnd);

/**
  * Function to queue input data that will subsequently used in the next
  * call to Keccak_Duplexing().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  in  Pointer to the bytes to queue.
  * @param  inByteLen   The number of input bytes provided in @a in.
  * @pre    The total number of input bytes since the last Keccak_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexingFeedPartialInput(Keccak_DuplexInstance *duplexInstance, const unsigned char *in, unsigned int inByteLen);

/**
  * Function to queue input data that will subsequently used in the next
  * call to Keccak_Duplexing(), where the data here consist of all-zero bytes.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  inByteLen   The number of input bytes 0x00 to feed.
  * @pre    The total number of input bytes since the last Keccak_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexingFeedZeroes(Keccak_DuplexInstance *duplexInstance, unsigned int inByteLen);

/**
  * Function to queue input data that will subsequently used in the next
  * call to Keccak_Duplexing(), with the additional pre-processing that
  * the input data is first XORed with the output data of the previous duplexing
  * call at the same offset.
  * In practice, this comes down to overwriting the input data in the state
  * of the duplex object.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  in  Pointer to the bytes to queue, before they are XORed.
  * @param  inByteLen   The number of input bytes provided in @a in.
  * @pre    The total number of input bytes since the last Keccak_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexingOverwritePartialInput(Keccak_DuplexInstance *duplexInstance, const unsigned char *in, unsigned int inByteLen);

/**
  * Function to queue input data for the next call to Keccak_Duplexing() that
  * is equal to the output data of the previous duplexing call at the same offset.
  * In practice, this comes down to overwriting with zeroes the state
  * of the duplex object.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  inByteLen   The number of bytes to overwrite with zeroes.
  * @pre    No input data may have been queued since the last call
  *         to Keccak_Duplexing().
  * @pre    The total number of input bytes since the last Keccak_Duplexing()
  *         call must not be higher than floor((r-2)/8).
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexingOverwriteWithZeroes(Keccak_DuplexInstance *duplexInstance, unsigned int inByteLen);

/**
  * Function to fetch output data beyond those that were already output since
  * the last call to Keccak_Duplexing().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  out             Pointer to the buffer where to store the output data.
  * @param  outByteLen      The number of output bytes desired.
  * @pre    The total number of output bytes, taken since (and including in)
  *         the last call to Keccak_Duplexing() cannot be higher than ceil(r/8).
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexingGetFurtherOutput(Keccak_DuplexInstance *duplexInstance, unsigned char *out, unsigned int outByteLen);

/**
  * Function to fetch output data beyond those that were already output since
  * the last call to Keccak_Duplexing(), with the additional post-processing
  * that this data is XORed into the given buffer.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  out             Pointer to the buffer where to XOR the output data.
  * @param  outByteLen      The number of output bytes desired.
  * @pre    The total number of output bytes, taken since (and including in)
  *         the last call to Keccak_Duplexing() cannot be higher than ceil(r/8).
  * @return Zero if successful, 1 otherwise.
  */
int Keccak_DuplexingGetFurtherOutputAndXOR(Keccak_DuplexInstance *duplexInstance, unsigned char *out, unsigned int outByteLen);

/** Function that has the same behavior as repeatedly calling
  *  - Keccak_Duplexing() with an input block of @a laneCount lanes from @a dataIn,
  *         @a delimitedSigmaEnd equal to @a trailingBits, and no output;
  *  - and advancing @a dataIn by @a laneCount lane sizes, until not enough data are available,
  * where @a laneCount is the (rounded down) rate in lanes.
  * The function returns the number of bytes processed from @a dataIn.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataByteLen The length of the input data in bytes.
  * @param  trailingBits    The delimited trailing bits.
  * @returns    The number of bytes processed.
  * @pre    0 < @a laneCount < 25
  * @pre    The rate modulo the lane size must be between 2 and 8 bits.
  * @pre    The input and output indexes must be zero,
  *         i.e., Keccak_DuplexGetInputIndex() and Keccak_DuplexGetOuputIndex()
  *         shall return zero.
  */
size_t Keccak_DuplexingFBWLAbsorb(Keccak_DuplexInstance *duplexInstance, const unsigned char *dataIn, size_t dataByteLen, unsigned char trailingBits);

/** Function that has the same behavior as repeatedly calling
  *  - memcpy() with a block of @a laneCount lanes from @a dataIn to @a dataOut;
  *  - Keccak_DuplexingGetFurtherOutputAndXOR() with a block of @a laneCount lanes onto @a dataOut;
  *  - Keccak_DuplexingOverwritePartialInput() with an input block of @a laneCount lanes from @a dataOut;
  *  - Keccak_Duplexing() with no input, @a delimitedSigmaEnd equal to @a trailingBits, and no output;
  *  - and advancing @a dataIn and @a dataOut by @a laneCount lane sizes, until not enough data are available,
  * where @a laneCount is the (rounded down) rate in lanes.
  * The function returns the number of bytes processed from @a dataIn and @a dataOut.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataOut  Pointer to the data to use as input.
  * @param  dataByteLen The length of the input data in bytes.
  * @param  trailingBits    The delimited trailing bits.
  * @returns    The number of bytes processed.
  * @pre    0 < @a laneCount < 25
  * @pre    The rate modulo the lane size must be between 2 and 8 bits.
  * @pre    The input and output indexes must be zero,
  *         i.e., Keccak_DuplexGetInputIndex() and Keccak_DuplexGetOuputIndex()
  *         shall return zero.
  */
size_t Keccak_DuplexingFBWLWrap(Keccak_DuplexInstance *duplexInstance, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

/** Function that has the same behavior as repeatedly calling
  *  - memcpy() with a block of @a laneCount lanes from @a dataIn to @a dataOut;
  *  - Keccak_DuplexingGetFurtherOutputAndXOR() with a block of @a laneCount lanes onto @a dataOut;
  *  - Keccak_DuplexingFeedPartialInput() with an input block of @a laneCount lanes from @a dataOut;
  *  - Keccak_Duplexing() with no input, @a delimitedSigmaEnd equal to @a trailingBits, and no output;
  *  - and advancing @a dataIn and @a dataOut by @a laneCount lane sizes, until not enough data are available,
  * where @a laneCount is the (rounded down) rate in lanes.
  * The function returns the number of bytes processed from @a dataIn and @a dataOut.
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @param  dataIn  Pointer to the data to use as input.
  * @param  dataOut  Pointer to the data to use as input.
  * @param  dataByteLen The length of the input data in bytes.
  * @param  trailingBits    The delimited trailing bits.
  * @returns    The number of bytes processed.
  * @pre    0 < @a laneCount < 25
  * @pre    The rate modulo the lane size must be between 2 and 8 bits.
  * @pre    The input and output indexes must be zero,
  *         i.e., Keccak_DuplexGetInputIndex() and Keccak_DuplexGetOuputIndex()
  *         shall return zero.
  */
size_t Keccak_DuplexingFBWLUnwrap(Keccak_DuplexInstance *duplexInstance, const unsigned char *dataIn, unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits);

/**
  * Function that returns the number of bytes queued for the next call
  * to Keccak_Duplexing().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @return The number of bytes queued.
  */
unsigned int Keccak_DuplexGetInputIndex(Keccak_DuplexInstance *duplexInstance);

/**
  * Function that returns the number of bytes already returned by the previous
  * call to Keccak_Duplexing() or by subsequent calls to
  * Keccak_DuplexingGetFurtherOutput() or to
  * Keccak_DuplexingGetFurtherOutputAndXOR().
  * @param  duplexInstance  Pointer to the duplex instance initialized
  *                     by Keccak_DuplexInitialize().
  * @return The number of bytes already returned.
  */
unsigned int Keccak_DuplexGetOutputIndex(Keccak_DuplexInstance *duplexInstance);

#endif
