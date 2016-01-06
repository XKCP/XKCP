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

#ifndef _Motorist_h_
#define _Motorist_h_

//#define OUTPUT

#include <string.h>
#include "NumberOfParallelInstances.h"
#ifdef OUTPUT
#include <stdio.h>
#endif

#if !defined(PlSnP_P)
#define PlSnP_P 1
#endif
#if PlSnP_P == 1
#include "SnP-interface.h"
#define	State_SizeInBytes SnP_stateSizeInBytes
#else
#include "PlSnP-interface.h"
#define	State_SizeInBytes PlSnP_statesSizeInBytes
#endif

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


#define	W								SnP_laneLengthInBytes
#define	Capacity						(256/8)
#define	TagLength						(128/8)
#define	Ra								(SnP_width/8 - W)
#define	Rs								(SnP_width/8 - Capacity)
#define	Cprime							Capacity

/* ------------------------------------------------------------------------ */

#define	Atom_Error		(-1)
#define	Atom_Success	0

#define	Atom_False		0
#define	Atom_True		1

/* ------------------------------------------------------------------------ */

/**
  * Structure that contains the Pistons instance (Collection of Piston).
  */
typedef struct {

    /** The state processed by the permutation. */
    ALIGN unsigned char state[State_SizeInBytes];

	#if PlSnP_P > 1
	unsigned char		indexCrypt;
	unsigned char		indexInject;
	#endif

	unsigned char		offsetCrypt;
	unsigned char		offsetInject;

    /** The crypt phase. */
    unsigned char phaseCrypt;

    /** The inject phase. */
    unsigned char phaseInject;

	#ifdef OUTPUT
	ALIGN unsigned char stateShadow[State_SizeInBytes];

	FILE * file;
	#endif

} Pistons_Instance;

/* ------------------------------------------------------------------------ */

/**
  * Structure that contains the Engine instance for use with the
  * Engine functions.
  */
typedef struct {

    /** The underlying piston collection instance array. */
    Pistons_Instance pistons;

    /** The phase. */
    unsigned char phase;

    /** The tag end index. */
    unsigned char tagEndIndex;

	#if PlSnP_P > 1
    /** The tag end index. */
    unsigned char tagEndIndexNext;
	#endif

} Engine_Instance;

/**
  * Function that initializes the Engine. 
  *
  * @param  instance		Pointer to the Engine instance structure.
  *
  * @pre    phase == *
  *
  * @post   phase == fresh
  *
  * @return 0 if successful, -1 otherwise.
  */
int Engine_Initialize(Engine_Instance *instance );

/**
  * Function that presents an input stream that consists of a
  * sequence of bytes for wrapping or unwrapping into an output stream.
  *
  * @param  instance		    Pointer to the Engine instance structure.
  * @param  I                   Pointer to input data.
  * @param  Ilen                Number of available bytes in I and O.
  * @param  O                   Pointer to buffer receiving output data (can be equal to pointer I).
  * @param  unwrapFlag          False for wrap, True for unwrap.
  * @param  lastFlag            If True, no new input data will be presented in a next call.
  *
  * @pre    phase == fresh
  *
  * @post   (phase == crypted) or (phase == endOfCrypt)
  *
  * @return >= 0 successful number of bytes processed, -1 otherwise.
  */
int Engine_Crypt(Engine_Instance *instance, const unsigned char *I, size_t Ilen, unsigned char *O, int unwrapFlag, int lastFlag );

/**
  * Function that injects data that consists of a sequence
  * of bytes. The data may be fed in multiple calls to this function.
  *
  * @param  instance		    Pointer to the Engine instance structure.
  * @param  MD	                Pointer to metadata to inject.
  * @param  MDlen               Length in bytes of metadata to inject.
  * @param  lastFlag            If True, no new input metadata will be presented in a next call.
  *
  * @pre    (phase == fresh) or (phase == crypted) or (phase == endOfCrypt)
  *
  * @post   (phase == fresh) or (phase == endOfMessage)
  *
  * @return >= 0 successful number of bytes processed, -1 otherwise.
  */
int Engine_Inject(Engine_Instance *instance, const unsigned char * MD, size_t MDlen, int lastFlag);

/**
  * Function that injects data that consists of a sequence
  * of bytes. The data will be injected to all Pistons.
  *
  * @param  instance		    Pointer to the Engine instance structure.
  * @param  X	                Pointer to data to inject.
  * @param  Xlen                Length in bytes of data to inject.
  * @param  diversifyFlag       if non zero injected data must be diversified.
  *
  * @pre    phase == fresh
  *
  * @post   phase == endOfMessage
  *
  * @return 0 if successful, -1 otherwise.
  */
int Engine_InjectCollective(Engine_Instance *instance, const unsigned char *X, unsigned int Xlen, int diversifyFlag);


/**
  * Function that gets a tag of a requested size in bytes. The full tag must be retrieved
  * with a single call to getTags.
  *
  * @param  instance		    Pointer to the Engine instance structure.
  * @param  tag                 Pointer to buffer receiving the tag.
  * @param  length              Tag length requested from first Piston.
  * @param  lengthNext          Tag length requested from next Piston(s).
  *
  * @pre    phase == readyForTag
  *
  * @post   phase == fresh
  *
  * @return 0 if successful, -1 otherwise.
  */
int Engine_GetTags(Engine_Instance *instance, unsigned char *tag, unsigned char length, unsigned char lengthNext);

/* ------------------------------------------------------------------------ */

#define Motorist_Wrap_LastCryptData		1
#define Motorist_Wrap_LastMetaData		2
#define Motorist_Wrap_LastCryptAndMeta	(Motorist_Wrap_LastCryptData|Motorist_Wrap_LastMetaData)

/**
  * Structure that contains the Motorist instance for use with the
  * Motorist functions.
  */
typedef struct {
    /** The underlying engine instance. */
    Engine_Instance engine;

    /** The phase. */
    unsigned char phase;

    /** Last flag. */
    unsigned char lastFlag;

} Motorist_Instance;

/**
  * Function that initializes the Motorist. 
  *
  * @param  instance		Pointer to the Motorist instance structure.
  *
  * @pre    phase == *
  *
  * @post   phase == ready
  *
  * @return 0 if successful, -1 otherwise.
  */
int Motorist_Initialize(Motorist_Instance *instance);

/**
  * Function that starts the Engine.
  *
  * @param  instance		    Pointer to the Motorist instance structure.
  * @param  SUV                 Pointer to buffer holding the Secret Unique Value.
  * @param  tagFlag             Flag to generate a tag (wrapping) or check a tag (unwrapping) on the StartEngine.
  * @param  tag					Pointer to tag buffer.
  * @param  unwrapFlag          Flag to generate (false) or check (true) the tag, this argument is ignored if tagFlag is false.
  * @param  forgetFlag          Flag to apply the forget operation (true).
  *
  * @pre    phase == ready
  *
  * @post   (phase == riding) or (phase == failed)
  *
  * @return 1 if successful, 0 if tag mismatch, -1 other error.
  */
int Motorist_StartEngine(Motorist_Instance *instance, const unsigned char * SUV, size_t SUVlen, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag );

/**
  * Function that presents a input buffer for (un)wrapping, and a metadata buffer,
  * both consist of a sequence of bytes. The data may be given in multiple calls. 
  *
  * @param  instance		    Pointer to the Motorist instance structure.
  * @param  input               Pointer to the (partial) input buffer.
  * @param  inputLen            The size in bytes of the (partial) input data.
  * @param  output              Pointer to the buffer to store the output data.
  *                             This pointer can be equal to the input buffer to save memory,
  *                             but otherwise the two buffers must not overlap.
  * @param  A                   Pointer to the (partial) metadata buffer.
  * @param  ALen                The size in bytes of the (partial) metadata.
  * @param  tag                 The buffer where to store the tag when wrapping, or read the tag to check when unwrapping.
  * @param  unwrapFlag          Wrap if false(zero), unwrap if true(non zero).
  * @param  forgetFlag          Forget if true (non zero).
  * @param  lastFlags           One of the following values:
  *                             - 0: Not last partial data for both input and metadata.
  *                             - Motorist_Wrap_LastCryptData: Last partial data for input.
  *                             - Motorist_Wrap_LastMetaData: Last partial data for metadata
  *                             - Motorist_Wrap_LastCryptAndMeta: Last partial data for both input and metadata
  *                             During a session this argument value can only be augmented.
  * @param  processedIlen       Pointer to a variable where the number of processed input/output bytes will be stored.
  *                             The returned value will be less or equal to inputLen.
  * @param  processedAlen       Pointer to a variable where the number of processed metadata bytes will be stored.
  *                             The returned value will be less or equal to MDLen.
  *
  *
  * @pre    phase == riding
  *
  * @post   (phase == riding) or (phase == failed)
  *
  * @return 0 if successful, 1 otherwise.
  */
int Motorist_Wrap(Motorist_Instance *instance, const unsigned char * input, size_t inputLen, unsigned char *output,
	const unsigned char * A, size_t Alen, unsigned char * tag, int unwrapFlag, int forgetFlag,
	int lastFlags, size_t *processedIlen, size_t *processedAlen);

#endif
