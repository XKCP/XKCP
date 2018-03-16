/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

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
  * - KeyakWidth800_ for a Motorist mode based on a single Keccak-p[800,12]
  * - KeyakWidth1600_ for a Motorist mode based on a single Keccak-p[1600,12]
  * - KeyakWidth1600times2_ for a Motorist mode based on 2 parallel Keccak-p[1600,12]
  * - KeyakWidth1600times4_ for a Motorist mode based on 4 parallel Keccak-p[1600,12]
  * - KeyakWidth1600times8_ for a Motorist mode based on 8 parallel Keccak-p[1600,12]
  *
  * The Prefix_Motorist_Instance contains the Motorist instance attributes for use
  * with the Prefix_Motorist* functions.
  */

#ifdef DontReallyInclude_DocumentationOnly

/* ------------------------------------------------------------------------ */

/**
  * Structure that contains the Pistons instance (Collection of Piston).
  */
typedef struct {
    /** The state processed by the permutation. */
    ALIGN unsigned char state[State_SizeInBytes];
    /* Current crypting piston index, only used by parallelized instances */
    unsigned char       indexCrypt;
    /* Current injecting piston index, only used by parallelized instances */
    unsigned char       indexInject;
    /* Current crypting piston offset */
    unsigned char       offsetCrypt;
    /* Current injecting piston offset */
    unsigned char       offsetInject;
    /** The crypt phase. */
    unsigned char phaseCrypt;
    /** The inject phase. */
    unsigned char phaseInject;
    #ifdef OUTPUT
    ALIGN unsigned char stateShadow[State_SizeInBytes];
    FILE * file;
    #endif
} Prefix_Pistons_Instance;

/* ------------------------------------------------------------------------ */

/**
  * Structure that contains the Engine instance for use with the
  * Prefix_Engine functions.
  */
typedef struct {
    /** The underlying piston collection instance array. */
    Prefix_Pistons_Instance pistons;
    /** The phase. */
    unsigned char phase;
    /** The tag end index. */
    unsigned char tagEndIndex;
    /** The tag end next index, only used by parallelized instances */
    unsigned char tagEndIndexNext;
} Prefix_Engine_Instance;

/**
  * Function that initializes the Engine. 
  *
  * @param  instance        Pointer to the Engine instance structure.
  *
  * @pre    phase == *
  *
  * @post   phase == fresh
  *
  * @return 0 if successful, -1 otherwise.
  */
int Prefix_Engine_Initialize(Prefix_Engine_Instance *instance );

/**
  * Function that presents an input stream that consists of a
  * sequence of bytes for wrapping or unwrapping into an output stream.
  *
  * @param  instance            Pointer to the Engine instance structure.
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
int Prefix_Engine_Crypt(Prefix_Engine_Instance *instance, const unsigned char *I, size_t Ilen, unsigned char *O, int unwrapFlag, int lastFlag );

/**
  * Function that injects data that consists of a sequence
  * of bytes. The data may be fed in multiple calls to this function.
  *
  * @param  instance            Pointer to the Engine instance structure.
  * @param  MD                  Pointer to metadata to inject.
  * @param  MDlen               Length in bytes of metadata to inject.
  * @param  lastFlag            If True, no new input metadata will be presented in a next call.
  *
  * @pre    (phase == fresh) or (phase == crypted) or (phase == endOfCrypt)
  *
  * @post   (phase == fresh) or (phase == endOfMessage)
  *
  * @return >= 0 successful number of bytes processed, -1 otherwise.
  */
int Engine_Inject(Prefix_Engine_Instance *instance, const unsigned char * MD, size_t MDlen, int lastFlag);

/**
  * Function that injects data that consists of a sequence
  * of bytes. The data will be injected to all Pistons.
  *
  * @param  instance            Pointer to the Engine instance structure.
  * @param  X                   Pointer to data to inject.
  * @param  Xlen                Length in bytes of data to inject.
  * @param  diversifyFlag       if non zero injected data must be diversified.
  *
  * @pre    phase == fresh
  *
  * @post   phase == endOfMessage
  *
  * @return 0 if successful, -1 otherwise.
  */
int Prefix_Engine_InjectCollective(Prefix_Engine_Instance *instance, const unsigned char *X, unsigned int Xlen, int diversifyFlag);


/**
  * Function that gets a tag of a requested size in bytes. The full tag must be retrieved
  * with a single call to getTags.
  *
  * @param  instance            Pointer to the Engine instance structure.
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
int Prefix_Engine_GetTags(Prefix_Engine_Instance *instance, unsigned char *tag, unsigned char length, unsigned char lengthNext);

/* ------------------------------------------------------------------------ */

/**
  * Structure that contains the Motorist instance for use with the
  * Prefix_Motorist functions.
  */
typedef struct {
    /** The underlying engine instance. */
    Prefix_Engine_Instance engine;
    /** The phase. */
    unsigned char phase;
    /** Last flag. */
    unsigned char lastFlag;
} Prefix_Motorist_Instance;

/**
  * Function that initializes the Motorist. 
  *
  * @param  instance        Pointer to the Motorist instance structure.
  *
  * @pre    phase == *
  *
  * @post   phase == ready
  *
  * @return 0 if successful, -1 otherwise.
  */
int Prefix_Motorist_Initialize(Prefix_Motorist_Instance *instance);

/**
  * Function that starts the Engine.
  *
  * @param  instance            Pointer to the Motorist instance structure.
  * @param  SUV                 Pointer to buffer holding the Secret Unique Value.
  * @param  tagFlag             Flag to generate a tag (wrapping) or check a tag (unwrapping) on the StartEngine.
  * @param  tag                 Pointer to tag buffer.
  * @param  unwrapFlag          Flag to generate (false) or check (true) the tag, this argument is ignored if tagFlag is false.
  * @param  forgetFlag          Flag to apply the forget operation (true).
  *
  * @pre    phase == ready
  *
  * @post   (phase == riding) or (phase == failed)
  *
  * @return 1 if successful, 0 if tag mismatch, -1 other error.
  */
int Prefix_Motorist_StartEngine(Prefix_Motorist_Instance *instance, const unsigned char * SUV, size_t SUVlen, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag );

/**
  * Function that presents a input buffer for (un)wrapping, and a metadata buffer,
  * both consist of a sequence of bytes. The data may be given in multiple calls. 
  *
  * @param  instance            Pointer to the Motorist instance structure.
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
int Prefix_Motorist_Wrap(Prefix_Motorist_Instance *instance, const unsigned char * input, size_t inputLen, unsigned char *output,
        const unsigned char * A, size_t Alen, unsigned char * tag, int unwrapFlag, int forgetFlag,
        int lastFlags, size_t *processedIlen, size_t *processedAlen);

#endif
