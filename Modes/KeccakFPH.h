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

#ifndef _KeccakFPH_h_
#define _KeccakFPH_h_

#include <stddef.h>
#include "align.h"
#include "Phases.h"

#ifndef KeccakP1600_excluded

typedef KCP_Phases Keccak_FPH_Phases;

typedef struct {
    KeccakWidth1600_SpongeInstance queueNode;
    KeccakWidth1600_SpongeInstance finalNode;
    unsigned int fixedOutputLength;
    unsigned int blockLen;
    unsigned int queueAbsorbedLen;
    size_t totalInputSize;
    Keccak_FPH_Phases phase;
} Keccak_FPH_Instance;

/** Fast parallel hash function FPH128, as defined in NIST's draft Special Publication 800-XXX (0.2),
  * posted on the hash forum on April 8th, 2016.
  * WARNING: Based on a draft, still subject to changes.
  * @param  input           Pointer to the input message.
  * @param  inputByteLen    The length of the input message in bytes.
  * @param  blockByteLen    Block size (B) in bytes, must be a power of 2.
  *                         The minimum value is 1024 in this implementation.
  * @param  output          Pointer to the output buffer.
  * @param  outputByteLen   The desired number of output bytes.
  * @param  customization   Pointer to the customization string (S).
  * @param  customByteLen   The length of the customization string in bytes.
  * @return 0 if successful, 1 otherwise.
  */
int Keccak_FPH128( const unsigned char * input, size_t inputByteLen, unsigned int blockByteLen, unsigned char * output, size_t outputByteLen, const unsigned char * customization, size_t customByteLen );

/**
  * Function to initialize the Fast parallel hash function FPH128 instance used in sequential hashing mode.
  * @param  fphInstance     Pointer to the hash instance to be initialized.
  * @param  blockByteLen    Block size (B) in bytes, must be a power of 2.
  *                         The minimum value is 1024 in this implementation.
  * @param  outputByteLen   The desired number of output bytes.
  *                         or 0 for an arbitrarily-long output.
  * @param  customization   Pointer to the customization string (S).
  * @param  customByteLen   The length of the customization string in bytes.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH128_Initialize(Keccak_FPH_Instance *fphInstance, unsigned int blockByteLen, size_t outputByteLen, const unsigned char * customization, size_t customByteLen);

/**
  * Function to give input data to be absorbed.
  * @param  fphInstance     Pointer to the hash instance initialized by Keccak_FPH128_Initialize().
  * @param  input           Pointer to the input data.
  * @param  inputByteLen    The number of input bytes provided in the input data.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH128_Update(Keccak_FPH_Instance *fphInstance, const unsigned char *input, size_t inputByteLen);

/**
  * Function to call after all input blocks have been input and to get
  * output bytes if the length was specified when calling Keccak_FPH128_Initialize().
  * @param  fphInstance     Pointer to the hash instance initialized by Keccak_FPH128_Initialize().
  * If @a outputByteLen was not 0 in the call to Keccak_FPH128_Initialize(), the number of
  *     output bytes is equal to @a outputByteLen.
  * If @a outputByteLen was 0 in the call to Keccak_FPH128_Initialize(), the output bytes
  *     must be extracted using the Keccak_FPH128_Squeeze() function.
  * @param  output          Pointer to the buffer where to store the output data.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH128_Final(Keccak_FPH_Instance *fphInstance, unsigned char * output);

 /**
  * Function to squeeze output data.
  * @param  fphInstance    Pointer to the hash instance initialized by Keccak_FPH128_Initialize().
  * @param  data           Pointer to the buffer where to store the output data.
  * @param  outputByteLen  The number of output bytes desired.
  * @pre    Keccak_FPH128_Final() must have been already called.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH128_Squeeze(Keccak_FPH_Instance *fphInstance, unsigned char * output, size_t outputByteLen);

/** Fast parallel hash function FPH256, as defined in NIST's draft Special Publication 800-XXX (0.2),
  * posted on the hash forum on April 8th, 2016.
  * WARNING: Based on a draft, still subject to changes.
  * @param  input           Pointer to the input message.
  * @param  inputByteLen    The length of the input message in bytes.
  * @param  blockByteLen    Block size (B) in bytes, must be a power of 2.
  *                         The minimum value is 1024 in this implementation.
  * @param  output          Pointer to the output buffer.
  * @param  outputByteLen   The desired number of output bytes.
  * @param  customization   Pointer to the customization string (S).
  * @param  customByteLen   The length of the customization string in bytes.
  * @return 0 if successful, 1 otherwise.
  */
int Keccak_FPH256( const unsigned char * input, size_t inputByteLen, unsigned int blockByteLen, unsigned char * output, size_t outputByteLen, const unsigned char * customization, size_t customByteLen );

/**
  * Function to initialize the Fast parallel hash function FPH256 instance used in sequential hashing mode.
  * @param  fphInstance     Pointer to the hash instance to be initialized.
  * @param  blockByteLen    Block size (B) in bytes, must be a power of 2.
  *                         The minimum value is 1024 in this implementation.
  * @param  outputByteLen   The desired number of output bytes.
  *                         or 0 for an arbitrarily-long output.
  * @param  customization   Pointer to the customization string (S).
  * @param  customByteLen   The length of the customization string in bytes.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH256_Initialize(Keccak_FPH_Instance *fphInstance, unsigned int blockByteLen, size_t outputByteLen, const unsigned char * customization, size_t customByteLen);

/**
  * Function to give input data to be absorbed.
  * @param  fphInstance     Pointer to the hash instance initialized by Keccak_FPH256_Initialize().
  * @param  input           Pointer to the input data.
  * @param  inputByteLen    The number of input bytes provided in the input data.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH256_Update(Keccak_FPH_Instance *fphInstance, const unsigned char *input, size_t inputByteLen);

/**
  * Function to call after all input blocks have been input and to get
  * output bytes if the length was specified when calling Keccak_FPH256_Initialize().
  * @param  fphInstance     Pointer to the hash instance initialized by Keccak_FPH256_Initialize().
  * If @a outputByteLen was not 0 in the call to Keccak_FPH256_Initialize(), the number of
  *     output bytes is equal to @a outputByteLen.
  * If @a outputByteLen was 0 in the call to Keccak_FPH256_Initialize(), the output bytes
  *     must be extracted using the Keccak_FPH256_Squeeze() function.
  * @param  output          Pointer to the buffer where to store the output data.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH256_Final(Keccak_FPH_Instance *fphInstance, unsigned char * output);

 /**
  * Function to squeeze output data.
  * @param  fphInstance    Pointer to the hash instance initialized by Keccak_FPH256_Initialize().
  * @param  data           Pointer to the buffer where to store the output data.
  * @param  outputByteLen  The number of output bytes desired.
  * @pre    Keccak_FPH256_Final() must have been already called.
  * @return SUCCESS if successful, FAIL otherwise.
  */
int Keccak_FPH256_Squeeze(Keccak_FPH_Instance *fphInstance, unsigned char * output, size_t outputByteLen);

#endif

#endif
