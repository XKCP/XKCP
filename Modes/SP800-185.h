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

#ifndef _SP800_185_h_
#define _SP800_185_h_

#include <stddef.h>
#include "align.h"
#include "Phases.h"

#ifndef KeccakP1600_excluded

typedef unsigned char BitSequence;

typedef size_t BitLength;

typedef struct {
    KeccakWidth1600_SpongeInstance sponge;
    BitLength fixedOutputLength;
    unsigned int lastByteBitLen;
    BitSequence lastByteValue;
    KCP_Phases phase;
} cSHAKE_Instance;

/** cSHAKE128 function, as defined in NIST's Special Publication 800-185,
  * published December 2016.
  * @param  input           Pointer to the input message (X).
  * @param  inputBitLen     The length of the input message in bits.
  * @param  output          Pointer to the output buffer.
  * @param  outputBitLen    The desired number of output bits (L).
  * @param  name            Pointer to the function name string (N).
  * @param  nameBitLen      The length of the function name in bits.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE128( const BitSequence *input, BitLength inputBitLen, BitSequence *output, BitLength outputBitLen, const BitSequence *name, BitLength nameBitLen, const BitSequence *customization, BitLength customBitLen );

/**
  * Function to initialize the cSHAKE128 instance used in sequential hashing mode.
  * @param  cskInstance     Pointer to the hash instance to be initialized.
  * @param  outputBitLen    The desired number of output bits (L).
  *                         or 0 for an arbitrarily-long output (XOF).
  * @param  name            Pointer to the function name string (N).
  * @param  nameBitLen      The length of the function name in bits.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE128_Initialize(cSHAKE_Instance *cskInstance, BitLength outputBitLen, const BitSequence *name, BitLength nameBitLen, const BitSequence *customization, BitLength customBitLen);

/**
  * Function to give input data to be absorbed.
  * @param  cskInstance     Pointer to the hash instance initialized by cSHAKE128_Initialize().
  * @param  input           Pointer to the input data.
  * @param  inputBitLen     The number of input bits provided in the input data.
  *                         Only the last update call can input a partial byte, other calls must have a length multiple of 8.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE128_Update(cSHAKE_Instance *cskInstance, const BitSequence *input, BitLength inputBitLen);

/**
  * Function to call after all input blocks have been input and to get
  * output bits if the length was specified when calling cSHAKE128_Initialize().
  * @param  cskInstance     Pointer to the hash instance initialized by cSHAKE128_Initialize().
  *                         If @a outputBitLen was not 0 in the call to cSHAKE128_Initialize(), the number of
  *                         output bits is equal to @a outputBitLen.
  *                         If @a outputBitLen was 0 in the call to cSHAKE128_Initialize(), the output bits
  *                         must be extracted using the cSHAKE128_Squeeze() function.
  * @param  output          Pointer to the buffer where to store the output data.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE128_Final(cSHAKE_Instance *cskInstance, BitSequence *output);

 /**
  * Function to squeeze output data.
  * @param  cskInstance     Pointer to the hash instance initialized by cSHAKE128_Initialize().
  * @param  output          Pointer to the buffer where to store the output data.
  * @param  outputBitLen    The number of output bits desired.
  *                         Only the last squeeze call can output a partial byte, 
  *                         other calls must have a length multiple of 8.
  * @pre    cSHAKE128_Final() must have been already called.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE128_Squeeze(cSHAKE_Instance *cskInstance, BitSequence *output, BitLength outputBitLen);

/* ------------------------------------------------------------------------- */

/** cSHAKE256 function, as defined in NIST's Special Publication 800-185,
  * published December 2016.
  * @param  input           Pointer to the input message (X).
  * @param  inputBitLen     The length of the input message in bits.
  * @param  output          Pointer to the output buffer.
  * @param  outputBitLen    The desired number of output bits (L).
  * @param  name            Pointer to the function name string (N).
  * @param  nameBitLen      The length of the function name in bits.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE256( const BitSequence *input, BitLength inputBitLen, BitSequence *output, BitLength outputBitLen, const BitSequence *name, BitLength nameBitLen, const BitSequence *customization, BitLength customBitLen );

/**
  * Function to initialize the cSHAKE256 instance used in sequential hashing mode.
  * @param  cskInstance     Pointer to the hash instance to be initialized.
  * @param  outputBitLen    The desired number of output bits (L).
  *                         or 0 for an arbitrarily-long output (XOF).
  * @param  name            Pointer to the function name string (N).
  * @param  nameBitLen      The length of the function name in bits.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE256_Initialize(cSHAKE_Instance *cskInstance, BitLength outputBitLen, const BitSequence *name, BitLength nameBitLen, const BitSequence *customization, BitLength customBitLen);

/**
  * Function to give input data to be absorbed.
  * @param  cskInstance     Pointer to the hash instance initialized by cSHAKE256_Initialize().
  * @param  input           Pointer to the input data.
  * @param  inputBitLen     The number of input bits provided in the input data.
  *                         Only the last update call can input a partial byte, other calls must have a length multiple of 8.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE256_Update(cSHAKE_Instance *cskInstance, const BitSequence *input, BitLength inputBitLen);

/**
  * Function to call after all input blocks have been input and to get
  * output bits if the length was specified when calling cSHAKE256_Initialize().
  * @param  cskInstance     Pointer to the hash instance initialized by cSHAKE256_Initialize().
  *                         If @a outputBitLen was not 0 in the call to cSHAKE256_Initialize(), the number of
  *                         output bits is equal to @a outputBitLen.
  *                         If @a outputBitLen was 0 in the call to cSHAKE256_Initialize(), the output bits
  *                         must be extracted using the cSHAKE256_Squeeze() function.
  * @param  output          Pointer to the buffer where to store the output data.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE256_Final(cSHAKE_Instance *cskInstance, BitSequence *output);

 /**
  * Function to squeeze output data.
  * @param  cskInstance     Pointer to the hash instance initialized by cSHAKE256_Initialize().
  * @param  output          Pointer to the buffer where to store the output data.
  * @param  outputBitLen    The number of output bits desired.
  *                         Only the last squeeze call can output a partial byte, 
  *                         other calls must have a length multiple of 8.
  * @pre    cSHAKE256_Final() must have been already called.
  * @return 0 if successful, 1 otherwise.
  */
int cSHAKE256_Squeeze(cSHAKE_Instance *cskInstance, BitSequence *output, BitLength outputBitLen);

/* ------------------------------------------------------------------------- */

typedef struct {
    cSHAKE_Instance csi;
    BitLength outputBitLen;
} KMAC_Instance;

/** KMAC128 function, as defined in NIST's Special Publication 800-185,
  * published December 2016.
  * @param  key             Pointer to the key (K).
  * @param  keyBitLen       The length of the key in bits.
  * @param  input           Pointer to the input message (X).
  * @param  inputBitLen     The length of the input message in bits.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @param  output          Pointer to the output buffer.
  * @param  outputBitLen    The desired number of output bits (L).
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC128(const BitSequence *key, BitLength keyBitLen, const BitSequence *input, BitLength inputBitLen,
        BitSequence *output, BitLength outputBitLen, const BitSequence *customization, BitLength customBitLen);

/**
  * Function to initialize the KMAC128 instance used in sequential MACing mode.
  * @param  kmInstance      Pointer to the instance to be initialized.
  * @param  key             Pointer to the key (K).
  * @param  keyBitLen       The length of the key in bits.
  * @param  outputBitLen    The desired number of output bits (L).
  *                         or 0 for an arbitrarily-long output (XOF).
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC128_Initialize(KMAC_Instance *kmkInstance, const BitSequence *key, BitLength keyBitLen, BitLength outputBitLen,
        const BitSequence *customization, BitLength customBitLen);

/**
  * Function to give input data to be MACed.
  * @param  kmInstance      Pointer to the instance initialized by KMAC128_Initialize().
  * @param  input           Pointer to the input data.
  * @param  inputBitLen     The number of input bits provided in the input data.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC128_Update(KMAC_Instance *kmkInstance, const BitSequence *input, BitLength inputBitLen);

/**
  * Function to call after all input data have been input and to get
  * output bits if the length was specified when calling KMAC128_Initialize().
  * @param  kmInstance      Pointer to the instance initialized by KMAC128_Initialize().
  *                         If @a outputBitLen was not 0 in the call to KMAC128_Initialize(), the number of
  *                         output bits is equal to @a outputBitLen.
  *                         If @a outputBitLen was 0 in the call to KMAC128_Initialize(), the output bits
  *                         must be extracted using the KMAC128_Squeeze() function.
  * @param  output          Pointer to the buffer where to store the output data.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC128_Final(KMAC_Instance *kmkInstance, BitSequence *output);

 /**
  * Function to squeeze output data.
  * @param  kmInstance      Pointer to the instance initialized by KMAC128_Initialize().
  * @param  output          Pointer to the buffer where to store the output data.
  * @param  outputBitLen    The number of output bits desired.
  *                         Only the last squeeze call can output a partial byte,
  *                         other calls must have a length multiple of 8.
  * @pre    KMAC128_Final() must have been already called.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC128_Squeeze(KMAC_Instance *kmkInstance, BitSequence *output, BitLength outputBitLen);

/* ------------------------------------------------------------------------- */

/** KMAC256 function, as defined in NIST's Special Publication 800-185,
  * published December 2016.
  * @param  key             Pointer to the key (K).
  * @param  keyBitLen       The length of the key in bits.
  * @param  input           Pointer to the input message (X).
  * @param  inputBitLen     The length of the input message in bits.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @param  output          Pointer to the output buffer.
  * @param  outputBitLen    The desired number of output bits (L).
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC256(const BitSequence *key, BitLength keyBitLen, const BitSequence *input, BitLength inputBitLen,
        BitSequence *output, BitLength outputBitLen, const BitSequence *customization, BitLength customBitLen);

/**
  * Function to initialize the KMAC256 instance used in sequential MACing mode.
  * @param  kmInstance      Pointer to the instance to be initialized.
  * @param  key             Pointer to the key (K).
  * @param  keyBitLen       The length of the key in bits.
  * @param  outputBitLen    The desired number of output bits (L).
  *                         or 0 for an arbitrarily-long output (XOF).
  * @param  customization   Pointer to the customization string (S).
  * @param  customBitLen    The length of the customization string in bits.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC256_Initialize(KMAC_Instance *kmkInstance, const BitSequence *key, BitLength keyBitLen, BitLength outputBitLen,
        const BitSequence *customization, BitLength customBitLen);

/**
  * Function to give input data to be MACed.
  * @param  kmInstance      Pointer to the instance initialized by KMAC256_Initialize().
  * @param  input           Pointer to the input data.
  * @param  inputBitLen     The number of input bits provided in the input data.
  *                         Only full bytes are supported, length must be a multiple of 8.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC256_Update(KMAC_Instance *kmkInstance, const BitSequence *input, BitLength inputBitLen);

/**
  * Function to call after all input data have been input and to get
  * output bits if the length was specified when calling KMAC256_Initialize().
  * @param  kmInstance      Pointer to the instance initialized by KMAC256_Initialize().
  *                         If @a outputBitLen was not 0 in the call to KMAC256_Initialize(), the number of
  *                         output bits is equal to @a outputBitLen.
  *                         If @a outputBitLen was 0 in the call to KMAC256_Initialize(), the output bits
  *                         must be extracted using the KMAC256_Squeeze() function.
  * @param  output          Pointer to the buffer where to store the output data.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC256_Final(KMAC_Instance *kmkInstance, BitSequence *output);

 /**
  * Function to squeeze output data.
  * @param  kmInstance      Pointer to the instance initialized by KMAC256_Initialize().
  * @param  output          Pointer to the buffer where to store the output data.
  * @param  outputBitLen    The number of output bits desired.
  *                         Only the last squeeze call can output a partial byte,
  *                         other calls must have a length multiple of 8.
  * @pre    KMAC256_Final() must have been already called.
  * @return 0 if successful, 1 otherwise.
  */
int KMAC256_Squeeze(KMAC_Instance *kmkInstance, BitSequence *output, BitLength outputBitLen);

/* ------------------------------------------------------------------------- */

/*
  todo Other algorithms defined in SP800-185
*/

#endif

#endif
