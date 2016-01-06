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

#ifndef _Keyak_h_
#define _Keyak_h_

#include "Motorist.h"

#if	(SnP_width == 800)

/**
  * Length of the Keyak key pack.
  */
#define		Lk				36

/**
  * Maximum SUV length for Keyak.
  */
#define		MaxNoncelength	58

#else

/**
  * Length of the Keyak key pack.
  */
#define		Lk				40

/**
  * Maximum SUV length for Keyak.
  */
#define		MaxNoncelength	150

#endif

/**
  * Structure that contains the Keyak instance for use with the
  * Keyak functions.
  */
typedef struct {
    /** The underlying motorist instance. */
    Motorist_Instance motorist;

} Keyak_Instance;

/**
  * Function that initializes, feeds the key and the nonce. The key and nonce consists of sequences of bytes.
  *
  * @param  instance		  Pointer to the Keyak instance structure.
  * @param  key               Pointer to the key.
  * @param  keySizeInBytes    The size of the key in bytes.
  * @param  nonce             Pointer to the nonce.
  * @param  nonceSizeInBytes  The size of the nonce in bytes.
  * @param  tagFlag           Flag to generate/check a tag on initialization.
  * @param  tag				  Pointer to tag buffer.
  * @param  unwrapFlag        Flag to generate (false) or check (true) the tag, this argument is ignored if tagFlag is false.
  * @param  forgetFlag        Flag to apply the forget operation (true).
  *
  * @pre    phase == *
  * @pre    keySizeInBytes <= (::Lk - 2)
  * @pre    nonceSizeInBytes <= ::MaxNoncelength
  *
  * @post   (phase == motorist.riding) or (phase == motorist.failed)
  *
  * @return 0 if successful, -1 otherwise.
  */
int Keyak_Initialize(Keyak_Instance *instance, 
	const unsigned char *key, unsigned int keySizeInBytes, 
	const unsigned char *nonce, unsigned int nonceSizeInBytes,
	int tagFlag, unsigned char * tag, 
	int unwrapFlag, int forgetFlag);

#define RiverKeyak_Initialize   Keyak_Initialize
#define LakeKeyak_Initialize    Keyak_Initialize
#define SeaKeyak_Initialize     Keyak_Initialize
#define OceanKeyak_Initialize   Keyak_Initialize
#define LunarKeyak_Initialize   Keyak_Initialize

/**
  * Function that presents full input data that consists of a
  * sequence of bytes for (un)wrapping and feeds full associated data 
  * that consists of a sequence of bytes.  
  *
  * @param  instance		    Pointer to the Keyak instance structure.
  * @param  input               Pointer to full input data to wrap or unwrap.
  * @param  output              Pointer to buffer where the full (un)wrapped data will be stored.
  *                             This pointer can be equal to the input buffer to save memory,
  *                             but otherwise the two buffers must not overlap.
  * @param  dataSizeInBytes     The size of the input/output data.
  * @param  AD                  Pointer to the full associated data.
  * @param  ADlen               The size of the associated data in bytes.
  * @param  tag                 The buffer where to store the tag when wrapping, or read the tag to check when unwrapping.
  * @param  unwrapFlag          Wrap if false(zero), unwrap if true(non zero).
  * @param  forgetFlag          Forget if true (non zero).
  *
  * @pre    phase == motorist.riding
  *
  * @post   (phase == motorist.riding) or (phase == motorist.failed)
  *
  * @return -1 if error, 0 if tag check failed, 1 success.
  */
int Keyak_Wrap(Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, 
	const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag );

/**
  * Macro that presents partial input data that consists of a
  * sequence of bytes for (un)wrapping and feeds partial associated data 
  * that consists of a sequence of bytes.  
  *
  * @param  argInstance         Pointer to the Keyak instance structure.
  * @param  argInput            Pointer to full input data to wrap or unwrap.
  * @param  argOutput           Pointer to buffer where the full (un)wrapped data will be stored.
  *                             This pointer can be equal to the input buffer to save memory,
  *                             but otherwise the two buffers must not overlap.
  * @param  argDataLen          The size of the input/output data.
  * @param  argAD               Pointer to the full associated data.
  * @param  argADlen            The size of the associated data in bytes.
  * @param  argTag              The buffer where to store the tag when wrapping, or read the tag to check when unwrapping.
  * @param  argUnwrapFlag       Wrap if false(zero), unwrap if true(non zero).
  * @param  argForgetFlag       Forget if true (non zero).
  * @param  argLastFlag         One of the following values:	
  *                             - 0: Not last partial data for both input and metadata.
  *                             - Motorist_Wrap_LastCryptData: Last partial data for input.
  *                             - Motorist_Wrap_LastMetaData: Last partial data for metadata
  *                             - Motorist_Wrap_LastCryptAndMeta: Last partial data for both input and metadata
  *                             During a session this argument value can only be augmented.
  * @param  argProcIlen         Pointer to a variable where the number of processed input/output bytes will be stored.
  *                             The returned value will be less or equal to argDataLen.
  * @param  argProcADlen        Pointer to a variable where the number of processed associated data bytes will be stored.
  *                             The returned value will be less or equal to argADlen.
  *
  * @pre    phase == motorist.riding
  *
  * @post   (phase == motorist.riding) or (phase == motorist.failed)
  *
  * @return -1 if error, 0 if tag check failed, 1 success.
  */
#define Keyak_WrapPartial( argInstance, argInput, argOutput, argDataLen, argAD, argADlen, argTag, argUnwrapFlag, argForgetFlag, argLastFlag, argProcIlen, argProcADlen) \
    Motorist_Wrap( &((argInstance)->motorist), argInput, argDataLen, argOutput, argAD, argADlen, argTag, argUnwrapFlag, argForgetFlag, argLastFlag, argProcIlen, argProcADlen )

#endif
