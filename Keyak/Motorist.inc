/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define W                           (SnP_width/8/25)
#define Ra                          (SnP_width/8 - W)
#define Rs                          (SnP_width/8 - Capacity)
#define Cprime                      Capacity

#define OffsetEOM                   (Ra+0)
#define OffsetCryptEnd              (Ra+1)
#define OffsetInjectStart           (Ra+2)
#define OffsetInjectEnd             (Ra+3)

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define Engine_Instance                 JOIN(prefix, _Engine_Instance)
#define Engine_Spark                    JOIN(prefix, _Engine_Spark)
#define Engine_Initialize               JOIN(prefix, _Engine_Initialize)
#define Engine_Crypt                    JOIN(prefix, _Engine_Crypt)
#define Engine_Inject                   JOIN(prefix, _Engine_Inject)
#define Engine_InjectCollectiveStreamer JOIN(prefix, _Engine_InjectCollectiveStreamer)
#define Engine_InjectCollective         JOIN(prefix, _Engine_InjectCollective)
#define Engine_GetTags                  JOIN(prefix, _Engine_GetTags)
#define Engine_FastWrapCryptOnly        JOIN(prefix, _Engine_FastWrapCryptOnly)
#define Engine_FastWrapInjectOnly       JOIN(prefix, _Engine_FastWrapInjectOnly)
#define Engine_FastWrapCryptAndInject   JOIN(prefix, _Engine_FastWrapCryptAndInject)

#define Motorist_Instance               JOIN(prefix, _Motorist_Instance)
#define Motorist_MakeKnot               JOIN(prefix, _Motorist_MakeKnot)
#define Motorist_HandleTag              JOIN(prefix, _Motorist_HandleTag)
#define Motorist_Initialize             JOIN(prefix, _Motorist_Initialize)
#define Motorist_StartEngine            JOIN(prefix, _Motorist_StartEngine)
#define Motorist_Wrap                   JOIN(prefix, _Motorist_Wrap)


#if PlSnP_parallelism > 1

#define DeclarePistonIndex              unsigned int indexP;
#define ForEveryPiston(argStart)        for ( indexP = argStart; indexP < PlSnP_parallelism; ++indexP )

#define State_StaticInitialize()                                    JOIN(PlSnP, _StaticInitialize)()
#define State_Initialize( argS )                                    JOIN(PlSnP, _InitializeAll)(argS)
#define State_Permute( argS )                                       PlSnP_PermuteAll( argS )
#define State_AddBytes( argS, argI, argO, argL )                    JOIN(PlSnP, _AddBytes)(argS, indexP, argI, argO, argL)
#define State_AddByte( argS, argV, argO )                           JOIN(PlSnP, _AddByte)(argS, indexP, argV, argO)
#define State_OverwriteBytes( argS, argI, argO, argL )              JOIN(PlSnP, _OverwriteBytes)(argS, indexP, argI, argO, argL)
#define State_ExtractBytes( argS, argI, argO, argL )                JOIN(PlSnP, _ExtractBytes)(argS, indexP, argI, argO, argL)
#define State_ExtractAndAddBytes( argS, argI, argO, argOfs, argL )  JOIN(PlSnP, _ExtractAndAddBytes)(argS, indexP, argI, argO, argOfs, argL)
#define PlSnP_AddLanesAll                                           JOIN(PlSnP, _AddLanesAll)
#define PlSnP_ExtractLanesAll                                       JOIN(PlSnP, _ExtractLanesAll)
#define PlSnP_ExtractAndAddLanesAll                                 JOIN(PlSnP, _ExtractAndAddLanesAll)

#if PlSnP_parallelism == 2
#define State_AddByteU( argS, argV, argO )                          JOIN(PlSnP, _AddByte)(argS, 0, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 1, argV, argO)
#elif PlSnP_parallelism == 4
#define State_AddByteU( argS, argV, argO )                          JOIN(PlSnP, _AddByte)(argS, 0, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 1, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 2, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 3, argV, argO)
#else
#define State_AddByteU( argS, argV, argO )                          JOIN(PlSnP, _AddByte)(argS, 0, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 1, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 2, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 3, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 4, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 5, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 6, argV, argO); \
                                                                    JOIN(PlSnP, _AddByte)(argS, 7, argV, argO)
#endif

#if PlSnP_parallelism == 2
#define State_AddBytesU( argS, argI, argO, argL )                   JOIN(PlSnP, _AddBytes)(argS, 0, (argI)+0*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 1, (argI)+1*(argL), argO, argL)
#elif PlSnP_parallelism == 4
#define State_AddBytesU( argS, argI, argO, argL )                   JOIN(PlSnP, _AddBytes)(argS, 0, (argI)+0*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 1, (argI)+1*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 2, (argI)+2*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 3, (argI)+3*(argL), argO, argL)
#else
#define State_AddBytesU( argS, argI, argO, argL )                   JOIN(PlSnP, _AddBytes)(argS, 0, (argI)+0*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 1, (argI)+1*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 2, (argI)+2*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 3, (argI)+3*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 4, (argI)+4*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 5, (argI)+5*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 6, (argI)+6*(argL), argO, argL); \
                                                                    JOIN(PlSnP, _AddBytes)(argS, 7, (argI)+7*(argL), argO, argL)
#endif


#else

#define DeclarePistonIndex
#define indexP              0
#define ForEveryPiston(argStart)

#define State_StaticInitialize()                                    JOIN(SnP, _StaticInitialize)()
#define State_Initialize( argS )                                    JOIN(SnP, _Initialize)( argS )
#define State_Permute( argS )                                       SnP_Permute( argS )
#define State_AddBytes( argS, argI, argO, argL )                    JOIN(SnP, _AddBytes)(argS, argI, argO, argL)
#define State_AddByte( argS, argV, argO )                           JOIN(SnP, _AddByte)(argS, argV, argO)
#define State_OverwriteBytes( argS, argI, argO, argL )              JOIN(SnP, _OverwriteBytes)(argS, argI, argO, argL)
#define State_ExtractBytes( argS, argI, argO, argL )                JOIN(SnP, _ExtractBytes)(argS, argI, argO, argL)
#define State_ExtractAndAddBytes( argS, argI, argO, argOfs, argL )  JOIN(SnP, _ExtractAndAddBytes)(argS, argI, argO, argOfs, argL)

#define State_AddByteU( argS, argV, argO )                          JOIN(SnP, _AddByte)(argS, argV, argO)
#define State_AddBytesU( argS, argI, argO, argL )                   JOIN(SnP, _AddBytes)(argS, argI, argO, argL);

#endif

/* ------------------------------------------------------------------------ */

/**
  * Function that calls the permutation.
  *
  * @param  instance            Pointer to the Engine instance structure.
  * @param  EOMFlag             
  * @param  length              The length of the tag for first Piston (zero if no tag).
  * @param  lengthNext          The length of the tag for next Pistons.
  *
  * @pre    phase == readyForIgnition
  *
  * @post   phase == fresh
  *
  * @return 0 if successful, 1 otherwise.
  */
static int Engine_Spark(Engine_Instance *instance, int EOMFlag, unsigned char length, unsigned char lengthNext );
static int Engine_Spark(Engine_Instance *instance, int EOMFlag, unsigned char length, unsigned char lengthNext )
{
    DeclarePistonIndex
    #ifdef OUTPUT
    unsigned char   s[PlSnP_parallelism][SnP_width/8];
    #endif

    instance->tagEndIndex       = length;
    #if PlSnP_parallelism > 1
    instance->tagEndIndexNext   = lengthNext;
    #endif
    if ( EOMFlag != 0 )
    {
        ForEveryPiston(0)
        {
            if ( length == 0 )
                length = 0xFF;
            State_AddByte(instance->pistons.state, length, OffsetEOM );
            #if PlSnP_parallelism > 1
            length = lengthNext;
            #endif
        }
    }
    #ifdef OUTPUT
    if ( instance->pistons.file )
    {
        ForEveryPiston(0)
        {
            State_ExtractBytes( instance->pistons.stateShadow, s[indexP], 0, SnP_width/8 );
            State_ExtractAndAddBytes( instance->pistons.state, s[indexP], s[indexP], 0, SnP_width/8 );
        }
    }
    #endif
    State_Permute( instance->pistons.state );
    #ifdef OUTPUT
    if ( instance->pistons.file )
    {
        memcpy( instance->pistons.stateShadow, instance->pistons.state, sizeof(instance->pistons.state) );
        ForEveryPiston(0)
        {
            fprintf( instance->pistons.file, "motWrap#%u XORed since last time", indexP );
            displayByteString( instance->pistons.file, "", s[indexP], SnP_width/8 );
            State_ExtractBytes( instance->pistons.stateShadow, s[indexP], 0, SnP_width/8 );
            fprintf( instance->pistons.file, "motWrap#%u after f()", indexP );
            displayByteString( instance->pistons.file, "", s[indexP], SnP_width/8 );
        }
    }
    #endif
    instance->pistons.phaseCrypt = Pistons_Phase_Fresh;
    instance->pistons.phaseInject = Pistons_Phase_Fresh;

    return ( Atom_Success );
}

static int Engine_Initialize(Engine_Instance *instance )
{
    State_StaticInitialize();
    State_Initialize( instance->pistons.state );
    #ifdef OUTPUT
    instance->pistons.file = 0;
    State_Initialize( instance->pistons.stateShadow );
    #endif
    instance->tagEndIndex = 0;
    #if PlSnP_parallelism > 1
    instance->tagEndIndexNext = 0;
    #endif
    instance->phase = Engine_Phase_Fresh;
    instance->pistons.phaseCrypt = Pistons_Phase_Fresh;
    instance->pistons.phaseInject = Pistons_Phase_Fresh;
    return ( Atom_Success );
}

static int Engine_FastWrapCryptOnly(Engine_Instance *instance, const unsigned char *I, size_t Ilen, unsigned char *O, int unwrapFlag)
{
    size_t initialIlen = Ilen;

    if (unwrapFlag == Atom_False) {
        do {
            #if PlSnP_parallelism > 1
            PlSnP_AddLanesAll(instance->pistons.state, I, Rs/W, Rs/W);
            PlSnP_ExtractLanesAll(instance->pistons.state, O, Rs/W, Rs/W);
            #else
            State_AddBytes(instance->pistons.state, I, 0, Rs);
            State_ExtractBytes(instance->pistons.state, O, 0, Rs);
            #endif
            State_AddByteU(instance->pistons.state, Rs, OffsetCryptEnd);
            State_AddByteU(instance->pistons.state, Rs, OffsetInjectStart);
            State_AddByteU(instance->pistons.state, Rs, OffsetInjectEnd);
            State_Permute( instance->pistons.state );
            I += (Rs*PlSnP_parallelism);
            O += (Rs*PlSnP_parallelism);
            Ilen -= (Rs*PlSnP_parallelism);
        }
        while(Ilen > (Rs*PlSnP_parallelism));
    }
    else {
        do {
            #if PlSnP_parallelism > 1
            PlSnP_ExtractAndAddLanesAll(instance->pistons.state, I, O, Rs/W, Rs/W);
            PlSnP_AddLanesAll(instance->pistons.state, O, Rs/W, Rs/W);
            #else
            State_ExtractAndAddBytes(instance->pistons.state, I, O, 0, Rs);
            State_AddBytes(instance->pistons.state, O, 0, Rs);
            #endif
            State_AddByteU(instance->pistons.state, Rs, OffsetCryptEnd);
            State_AddByteU(instance->pistons.state, Rs, OffsetInjectStart);
            State_AddByteU(instance->pistons.state, Rs, OffsetInjectEnd);
            State_Permute(instance->pistons.state);
            I += (Rs*PlSnP_parallelism);
            O += (Rs*PlSnP_parallelism);
            Ilen -= (Rs*PlSnP_parallelism);
        }
        while(Ilen > (Rs*PlSnP_parallelism));
    }
    return (initialIlen - Ilen); /* number of bytes processed */
}

static size_t Engine_FastWrapInjectOnly(Engine_Instance *instance, const unsigned char *A, size_t Alen)
{
    size_t initialAlen = Alen;

    do
    {
        #if PlSnP_parallelism > 1
        PlSnP_AddLanesAll(instance->pistons.state, A, Ra/W, Ra/W);
        #else
        State_AddBytes(instance->pistons.state, A, 0, Ra);
        #endif
        State_AddByteU(instance->pistons.state, Ra, OffsetInjectEnd);
        State_Permute( instance->pistons.state );
        A += (Ra*PlSnP_parallelism);
        Alen -= (Ra*PlSnP_parallelism);
    }
    while(Alen > (Ra*PlSnP_parallelism));
    return (initialAlen - Alen); /* number of bytes processed */
}

static int Engine_FastWrapCryptAndInject(Engine_Instance *instance, const unsigned char *I, size_t Ilen, size_t *ptrIprocessed, unsigned char *O, const unsigned char *A, size_t Alen, int unwrapFlag)
{
    size_t initialIlen = Ilen;
    size_t initialAlen = Alen;

    if (unwrapFlag == Atom_False) {
        do {
            #if PlSnP_parallelism > 1
            PlSnP_AddLanesAll(instance->pistons.state, I, Rs/W, Rs/W);
            PlSnP_ExtractLanesAll(instance->pistons.state, O, Rs/W, Rs/W);
            #else
            State_AddBytes(instance->pistons.state, I, 0, Rs);
            State_ExtractBytes(instance->pistons.state, O, 0, Rs);
            #endif
            State_AddBytesU(instance->pistons.state, A, Rs, Ra-Rs );
            State_AddByteU(instance->pistons.state, Rs, OffsetCryptEnd);
            State_AddByteU(instance->pistons.state, Rs, OffsetInjectStart);
            State_AddByteU(instance->pistons.state, Ra, OffsetInjectEnd);
            State_Permute( instance->pistons.state );
            I += (Rs*PlSnP_parallelism);
            O += (Rs*PlSnP_parallelism);
            A += (Ra-Rs)*PlSnP_parallelism;
            Ilen -= (Rs*PlSnP_parallelism);
            Alen -= (Ra-Rs)*PlSnP_parallelism;
        }
        while( (Ilen > (Rs*PlSnP_parallelism)) && (Alen > ((Ra-Rs)*PlSnP_parallelism)) );
    }
    else {
        do {
            #if PlSnP_parallelism > 1
            PlSnP_ExtractAndAddLanesAll(instance->pistons.state, I, O, Rs/W, Rs/W);
            PlSnP_AddLanesAll(instance->pistons.state, O, Rs/W, Rs/W);
            #else
            State_ExtractAndAddBytes(instance->pistons.state, I, O, 0, Rs);
            State_AddBytes(instance->pistons.state, O, 0, Rs);
            #endif
            State_AddBytesU(instance->pistons.state, A, Rs, Ra-Rs );
            State_AddByteU(instance->pistons.state, Rs, OffsetCryptEnd);
            State_AddByteU(instance->pistons.state, Rs, OffsetInjectStart);
            State_AddByteU(instance->pistons.state, Ra, OffsetInjectEnd);
            State_Permute( instance->pistons.state );
            I += (Rs*PlSnP_parallelism);
            O += (Rs*PlSnP_parallelism);
            A += (Ra-Rs)*PlSnP_parallelism;
            Ilen -= (Rs*PlSnP_parallelism);
            Alen -= (Ra-Rs)*PlSnP_parallelism;
        }
        while( (Ilen > (Rs*PlSnP_parallelism)) && (Alen > ((Ra-Rs)*PlSnP_parallelism)) );
    }
    *ptrIprocessed = initialIlen - Ilen;
    return (initialAlen - Alen); /* number of bytes processed */
}

static int Engine_Crypt(Engine_Instance *instance, const unsigned char *I, size_t Ilen, unsigned char *O, int unwrapFlag, int lastFlag )
{
    DeclarePistonIndex
    unsigned char offset;
    unsigned int length;
    #if PlSnP_parallelism > 1
    int processed;
    #endif

    if ( instance->pistons.phaseCrypt != Pistons_Phase_Running)
    {
        if ( instance->pistons.phaseCrypt != Pistons_Phase_Fresh )
        {
            return ( Atom_Error );      
        }
        #if PlSnP_parallelism > 1
        instance->pistons.indexCrypt = 0;
        #endif
        instance->pistons.offsetCrypt = instance->tagEndIndex;
        instance->pistons.phaseCrypt = Pistons_Phase_Running;
    }

    #if PlSnP_parallelism > 1
    processed = 0;
    #endif
    offset = instance->pistons.offsetCrypt;
    if ( unwrapFlag == Atom_False )
    {
        /*  Wrap    */
        ForEveryPiston(instance->pistons.indexCrypt)
        {
            length = Rs - offset;
            if (length > Ilen)
                length = (unsigned int)Ilen;
            State_AddBytes(instance->pistons.state, I, offset, length );
            State_ExtractBytes(instance->pistons.state, O, offset, length );
            #if PlSnP_parallelism > 1
            I += length;
            O += length;
            processed += length;
            #endif
            Ilen -= length;
            offset += (unsigned char)length;
            if ( offset == Rs )
            {
                State_AddByte(instance->pistons.state, offset, OffsetCryptEnd);
                #if PlSnP_parallelism > 1
                offset = instance->tagEndIndexNext;
                ++instance->pistons.indexCrypt;
                #else
                instance->pistons.phaseCrypt = Pistons_Phase_Full;
                #endif
            }
        }
    }
    else
    {
        /*  Unwrap  */
        ForEveryPiston(instance->pistons.indexCrypt)
        {
            length = Rs - offset;
            if (length > Ilen)
                length = (unsigned int)Ilen;
            State_ExtractAndAddBytes(instance->pistons.state, I, O, offset, length);
            State_AddBytes(instance->pistons.state, O, offset, length);
            #if PlSnP_parallelism > 1
            I += length;
            O += length;
            processed += length;
            #endif
            Ilen -= length;
            offset += (unsigned char)length;
            if ( offset == Rs )
            {
                State_AddByte(instance->pistons.state, offset, OffsetCryptEnd);
                #if PlSnP_parallelism > 1
                offset = instance->tagEndIndexNext;
                ++instance->pistons.indexCrypt;
                #else
                instance->pistons.phaseCrypt = Pistons_Phase_Full;
                #endif
            }
        }
    }
    instance->pistons.offsetCrypt = offset;

    #if PlSnP_parallelism > 1
    if ( instance->pistons.indexCrypt == PlSnP_parallelism )
    {
        instance->pistons.phaseCrypt = Pistons_Phase_Full;
    }
    #endif
    if ( (Ilen == 0) && ((lastFlag & Motorist_Wrap_LastCryptData) != 0) )
    {
        /*  Done with crypting (out of fuel)    */
        if ( instance->pistons.phaseCrypt != Pistons_Phase_Full )
        {
            ForEveryPiston(instance->pistons.indexCrypt)
            {
                State_AddByte(instance->pistons.state, offset, OffsetCryptEnd);
                #if PlSnP_parallelism > 1
                offset = instance->tagEndIndexNext;
                #endif
            }
        }
        instance->pistons.phaseCrypt = Pistons_Phase_Done;
        instance->phase = Engine_Phase_Crypted;
    }
    #if PlSnP_parallelism > 1
    return ( processed );
    #else
    return ( length );
    #endif
}

int Engine_Inject(Engine_Instance *instance, const unsigned char * A, size_t Alen, int lastFlag)
{
    DeclarePistonIndex
    unsigned int length;
    #if PlSnP_parallelism > 1
    int processed;
    #endif

    if ( instance->pistons.phaseInject != Pistons_Phase_Running )
    {
        if ( instance->pistons.phaseInject != Engine_Phase_Fresh )
        {
            return ( Atom_Error );
        }
        #if PlSnP_parallelism > 1
        instance->pistons.indexInject = 0;
        #endif
        if ( instance->pistons.phaseCrypt == Pistons_Phase_Fresh )
        {
            instance->pistons.offsetInject = 0;
        }
        else
        {
            instance->pistons.offsetInject = Rs;
            ForEveryPiston(0)
            {
                State_AddByte(instance->pistons.state, instance->pistons.offsetInject, OffsetInjectStart);
            }
        }
        instance->pistons.phaseInject = Pistons_Phase_Running;
    }

    #if PlSnP_parallelism > 1
    processed = 0;
    #endif
    ForEveryPiston(instance->pistons.indexInject)
    {
        length = Ra - instance->pistons.offsetInject;
        if (length > Alen)
            length = (unsigned int)Alen;
        State_AddBytes(instance->pistons.state, A, instance->pistons.offsetInject, length );
        #if PlSnP_parallelism > 1
        A += length;
        processed += (int)length;
        #endif
        Alen -= length;
        instance->pistons.offsetInject += (unsigned char)length;
        if ( instance->pistons.offsetInject == Ra )
        {
            State_AddByte(instance->pistons.state, instance->pistons.offsetInject, OffsetInjectEnd);
            #if PlSnP_parallelism > 1
            instance->pistons.offsetInject = (instance->pistons.phaseCrypt == Pistons_Phase_Fresh) ? 0 : Rs;
            ++instance->pistons.indexInject;
            #else
            instance->pistons.phaseInject = Pistons_Phase_Full;
            #endif
        }
    }

    #if PlSnP_parallelism > 1
    if ( instance->pistons.indexInject == PlSnP_parallelism )
    {
        instance->pistons.phaseInject = Pistons_Phase_Full;
    }
    #endif
    if ( (Alen == 0) && ((lastFlag & Motorist_Wrap_LastMetaData) != 0) )
    {
        /* Done injecting */
        if ( (instance->pistons.phaseInject != Pistons_Phase_Full) && (instance->pistons.offsetInject != 0) )   /* Optimization: don't xor zeroes */
        {
            ForEveryPiston(instance->pistons.indexInject)
            {
                State_AddByte(instance->pistons.state, instance->pistons.offsetInject, OffsetInjectEnd);
                #if PlSnP_parallelism > 1
                if ( instance->pistons.phaseCrypt == Pistons_Phase_Fresh )
                    break;
                instance->pistons.offsetInject = Rs;
                #endif
            }
        }
        instance->pistons.phaseInject = Pistons_Phase_Done;
    }
    #if PlSnP_parallelism > 1
    return ( processed );
    #else
    return ( length );
    #endif
}

static int Engine_InjectCollectiveStreamer(Engine_Instance *instance, unsigned char *X, unsigned int sizeX, unsigned char * pOffset, int diversifyFlag);
static int Engine_InjectCollectiveStreamer(Engine_Instance *instance, unsigned char *X, unsigned int sizeX, unsigned char * pOffset, int diversifyFlag)
{
    DeclarePistonIndex
    unsigned char partSize;
    unsigned char offset;

    offset = *pOffset;
    partSize = Ra - offset;
    if ( partSize > sizeX )
        partSize = (unsigned char)sizeX;
    *pOffset += partSize;

    ForEveryPiston(0)
    {
        State_AddByte(instance->pistons.state, offset, OffsetInjectEnd); /* remove previous OffsetInjectEnd */
        State_AddBytes(instance->pistons.state, X, offset, partSize );
        State_AddByte(instance->pistons.state, *pOffset, OffsetInjectEnd);
        if ( (diversifyFlag != Atom_False) && (partSize == sizeX) )
            X[sizeX-1]++;
    }
    sizeX -= partSize;

    /* Check block full and more data to follow */
    if ( *pOffset == Ra )
    {
        *pOffset = 0;
        if ( sizeX != 0 )
        {
            Engine_Spark(instance, 0, 0, 0 );
        }
    }
    return ( partSize );
}

int Engine_InjectCollective(Engine_Instance *instance, const unsigned char *X, unsigned int sizeX, int diversifyFlag)
{
    unsigned char offset;
    int result;
    unsigned char diversifier[2];
    int dataAvailableFlag = 0;

    if ( instance->phase != Engine_Phase_Fresh )
        return ( Atom_Error );
    offset = 0;

    /* Inject X and spark while full blocks and more X available */
    if ( sizeX != 0 )
    {
        dataAvailableFlag = 1;
        do
        {
            result = Engine_InjectCollectiveStreamer(instance, (unsigned char *)X, sizeX, &offset, 0 );
            if ( result < 0 )
                return ( Atom_Error );
            X += result;
        }
        while ( (sizeX -= result) != 0 );
    }
    if ( diversifyFlag != Atom_False )
    {
        if ( (offset == 0) && (dataAvailableFlag != 0) )    /*  spark the last full block */
            Engine_Spark(instance, 0, 0, 0 );
        diversifier[0] = PlSnP_parallelism;
        diversifier[1] = 0;
        for ( result = 0, sizeX = 2; sizeX != 0; sizeX -= result )
        {
            result = Engine_InjectCollectiveStreamer(instance, diversifier + result, sizeX, &offset, 1 );
            if ( result < 0 )
                return ( Atom_Error );
        }
    }
    instance->phase = Engine_Phase_EndOfMessage;
    return ( Atom_Success );
}

int Engine_GetTags(Engine_Instance *instance, unsigned char *tag, unsigned char length, unsigned char lengthNext)
{
    DeclarePistonIndex

    if ( instance->phase != Engine_Phase_EndOfMessage )
        return ( Atom_Error );
    if ( Engine_Spark(instance, 1, length, lengthNext) != 0 )
        return ( Atom_Error );
    if ( length > Rs )
        return ( Atom_Error );
    #if PlSnP_parallelism != 1
    if ( lengthNext > Rs )
        return ( Atom_Error );
    #endif

    ForEveryPiston(0)
    {
        State_ExtractBytes(instance->pistons.state, tag, 0, length );
        #if PlSnP_parallelism != 1
        tag += length;
        length = lengthNext;
        #endif
    }
    instance->phase = Engine_Phase_Fresh;
    return ( Atom_Success );
}

/* ------------------------------------------------------------------------ */

static int Motorist_MakeKnot(Motorist_Instance *instance );
static int Motorist_MakeKnot(Motorist_Instance *instance )
{
    unsigned char tempTags[PlSnP_parallelism*Cprime];

    if ( Engine_GetTags(&instance->engine, tempTags, Cprime, Cprime ) < 0 )
        return ( Atom_Error );
    return ( Engine_InjectCollective(&instance->engine, tempTags, PlSnP_parallelism * Cprime, 0) );
}

static int Motorist_HandleTag(Motorist_Instance *instance, int tagFlag, unsigned char * tag, int unwrapFlag );
static int Motorist_HandleTag(Motorist_Instance *instance, int tagFlag, unsigned char * tag, int unwrapFlag )
{
    unsigned char tempTag[TagLength];

    if ( tagFlag == Atom_False )
    {
        if ( Engine_GetTags(&instance->engine, tempTag, 0, 0 ) < 0 )
            return ( Atom_Error );
    }
    else
    {
        if ( Engine_GetTags(&instance->engine, tempTag, TagLength, 0) < 0 )
            return ( Atom_Error );
        if ( unwrapFlag == Atom_False )
        {
            memcpy( tag, tempTag, TagLength );
        }
        else if ( memcmp( tempTag, tag, TagLength ) != 0 )
        {
            instance->phase = Motorist_Phase_Failed;
            return ( Atom_False );
        }
    }
    return ( Atom_True );
}

int Motorist_Initialize(Motorist_Instance *instance)
{
    if ( Engine_Initialize(&instance->engine) != 0 )
        return ( Atom_Error );
    instance->phase = Motorist_Phase_Ready;
    instance->lastFlag = 0;
    return ( Atom_Success );
}

int Motorist_StartEngine(Motorist_Instance *instance, const unsigned char * SUV, size_t SUVlen, int tagFlag, unsigned char * tag, int unwrapFlag, int forgetFlag )
{
    int result = Atom_False;

    if ( instance->phase != Motorist_Phase_Ready )
        return ( Atom_Error );
    if ( Engine_InjectCollective(&instance->engine, SUV, (unsigned int)SUVlen, 1) != 0 )
        return ( Atom_Error );
    if ( forgetFlag != Atom_False )
        if ( Motorist_MakeKnot(instance) < 0 )
                return ( Atom_Error );
    result = Motorist_HandleTag(instance, tagFlag, tag, unwrapFlag );
    if ( result == Atom_True )
        instance->phase = Motorist_Phase_Riding;
    return ( result );
}


int Motorist_Wrap(Motorist_Instance *instance, const unsigned char * I, size_t Ilen, unsigned char *O, 
    const unsigned char * A, size_t Alen, unsigned char * tag, int unwrapFlag, int forgetFlag,
    int lastFlag, size_t *processedIlen, size_t *processedAlen)
{
    int             resultI;
    int             resultA;
    unsigned char   *initialO;
    size_t          initialOlen;

    initialO = O;
    initialOlen = Ilen;
    *processedIlen = 0;
    *processedAlen = 0;
    if ( instance->phase != Motorist_Phase_Riding )
        return ( Atom_Error );

    /*  Once a lastFlag has been set, it must remain set during the session */
    if ( ((instance->lastFlag & Motorist_Wrap_LastCryptData) != 0) && ((lastFlag & Motorist_Wrap_LastCryptData) == 0) )
        return ( Atom_Error );
    if ( ((instance->lastFlag & Motorist_Wrap_LastMetaData) != 0) && ((lastFlag & Motorist_Wrap_LastMetaData) == 0) )
        return ( Atom_Error );
    instance->lastFlag  = (unsigned char)lastFlag;
    if ( ((lastFlag & Motorist_Wrap_LastCryptData) == 0) && (Ilen != 0) )
        --Ilen;
    if ( ((lastFlag & Motorist_Wrap_LastMetaData) == 0) && (Alen != 0) )
        --Alen;

    if ( instance->engine.phase == Engine_Phase_Fresh )
    {
        if ( Ilen != 0 ) 
        {
            /*  Caller wants to crypt. */
            instance->engine.phase = Engine_Phase_Crypting;
        }
        else if ( (lastFlag & Motorist_Wrap_LastCryptData) == 0 )
        {
            /*  Caller does not give input, but does not set lastCrypt flag, 
            **  so we don't know how the phase will evolve, do nothing. 
            */ 
            return ( Atom_True );
        }
        else
        {
            /* Only metadata can follow (input is empty) */
            instance->engine.phase = Engine_Phase_InjectOnly;
            if ( (Alen == 0) && ((lastFlag & Motorist_Wrap_LastMetaData) != 0) )
            {
                /*  Nothing to inject either, perform empty inject */
                if ( Engine_Inject(&instance->engine, A, 0, Motorist_Wrap_LastMetaData ) != 0 )
                    return ( Atom_Error );
                instance->engine.phase = Engine_Phase_EndOfMessage;
            }
        }
    }

    if ( instance->engine.phase == Engine_Phase_Crypting )
    {
        while ( Ilen != 0 )
        {
            /*  If more data available and Crypter and Injector are full, then spark */
            if ( ((Ilen | Alen) != 0) && (instance->engine.pistons.phaseCrypt == Pistons_Phase_Full) )
            {
                if ( instance->engine.pistons.phaseInject >= Pistons_Phase_Full ) /* Full or Done */
                {
                    Engine_Spark(&instance->engine, 0, 0, 0 );
                }
                else if ( (Alen == 0) && ((lastFlag & Motorist_Wrap_LastMetaData) != 0) ) /* AD done, but Pistons not aware */
                {
                    resultA = Engine_Inject(&instance->engine, 0, 0, Motorist_Wrap_LastMetaData );
                    if ( resultA < 0 )
                        return ( Atom_Error );
                    Engine_Spark(&instance->engine, 0, 0, 0 );
                }
            }

            if ( (Ilen > (Rs*PlSnP_parallelism)) && (instance->engine.pistons.phaseCrypt == Pistons_Phase_Fresh) && (instance->engine.tagEndIndex == 0) )
            {
                if ( Alen > ((Ra-Rs)*PlSnP_parallelism) )
                {
                    size_t processedI;
                    size_t processedA = Engine_FastWrapCryptAndInject(&instance->engine, I, Ilen, &processedI, O, A, Alen, unwrapFlag);
                    Alen -= processedA;
                    A += processedA;
                    *processedAlen += processedA;
                    Ilen -= processedI;
                    I += processedI;
                    O += processedI;
                    *processedIlen += processedI;
                }
                else if ( (Alen == 0) && ((lastFlag & Motorist_Wrap_LastMetaData) != 0) )
                {
                    size_t processedI = Engine_FastWrapCryptOnly(&instance->engine, I, Ilen, O, unwrapFlag);
                    Ilen -= processedI;
                    I += processedI;
                    O += processedI;
                    *processedIlen += processedI;
                }
            }

            if ( instance->engine.pistons.phaseCrypt == Pistons_Phase_Full )
                resultI = 0;
            else
            {
                resultI = Engine_Crypt(&instance->engine, I, Ilen, O, unwrapFlag, lastFlag & Motorist_Wrap_LastCryptData );
                if ( resultI < 0 )
                    return ( Atom_Error );
                *processedIlen += resultI;
                Ilen -= resultI;
                I += resultI;
                O += resultI;
            }

            if ( instance->engine.pistons.phaseInject >= Pistons_Phase_Full ) /* Full or Done */
                resultA = 0;
            else
            {
                resultA = Engine_Inject(&instance->engine, A, Alen, lastFlag & Motorist_Wrap_LastMetaData );
                if ( resultA < 0 )
                    return ( Atom_Error );
                *processedAlen += resultA;
                Alen -= resultA;
                A += resultA;
            }

            if (    (instance->engine.pistons.phaseCrypt == Pistons_Phase_Done)
                &&  (instance->engine.pistons.phaseInject == Pistons_Phase_Done))
            {
                instance->engine.phase = Engine_Phase_EndOfMessage;
                break;
            }

            if ( (resultI | resultA) == 0 )
            {
                /*  Can't do more than that */
                return ( Atom_True );
            }
        }
    }

    /*  Input all done, continue injecting  */
    if (    (instance->engine.phase == Engine_Phase_Crypted)
        ||  (instance->engine.phase == Engine_Phase_InjectOnly) )
    {
        while ( (Alen != 0) || ((lastFlag & Motorist_Wrap_LastMetaData) != 0) )
        {
            /*  If more data available and Injector is full, then spark */
            if ( instance->engine.pistons.phaseInject == Pistons_Phase_Full )
            {
                Engine_Spark(&instance->engine, 0, 0, 0 );
            }

            if ((Alen > (Ra*PlSnP_parallelism))
                &&  (instance->engine.pistons.phaseInject == Pistons_Phase_Fresh)
                && (instance->engine.pistons.phaseCrypt == Pistons_Phase_Fresh) )
            {
                size_t processed = Engine_FastWrapInjectOnly(&instance->engine, A, Alen);
                Alen -= processed;
                A += processed;
                *processedAlen += processed;
                instance->engine.pistons.offsetInject = 0;
                #if PlSnP_parallelism > 1
                instance->engine.pistons.indexInject = 0;
                #endif
            }

            resultA = Engine_Inject(&instance->engine, A, Alen, lastFlag & Motorist_Wrap_LastMetaData );
            if ( resultA < 0 )
                return ( Atom_Error );
            *processedAlen += resultA;
            Alen -= resultA;
            A += resultA;

            if ( instance->engine.pistons.phaseInject == Pistons_Phase_Done )
            {
                instance->engine.phase = Engine_Phase_EndOfMessage;
                break;
            }
        }
    }

    if ( instance->engine.phase == Engine_Phase_EndOfMessage )
    {
        instance->lastFlag = 0;

        /*  Everything is processed */
        #if PlSnP_parallelism == 1
        if ( forgetFlag != Atom_False )
        #endif
            if ( Motorist_MakeKnot(instance) < 0 )
                    return ( Atom_Error );
        resultI = Motorist_HandleTag(instance, Atom_True, tag, unwrapFlag );
        if ( resultI != Atom_True )
            memset( initialO, 0, initialOlen );
        return ( resultI );
    }
    return ( Atom_True );
}

#undef  W
#undef  Ra
#undef  Rs
#undef  Cprime

#undef  OffsetEOM
#undef  OffsetCryptEnd
#undef  OffsetInjectStart
#undef  OffsetInjectEnd

#undef Engine_Instance
#undef Engine_Spark
#undef Engine_Initialize
#undef Engine_Crypt
#undef Engine_Inject
#undef Engine_InjectCollectiveStreamer
#undef Engine_InjectCollective
#undef Engine_GetTags
#undef Engine_FastWrapCryptOnly
#undef Engine_FastWrapInjectOnly
#undef Engine_FastWrapCryptAndInject

#undef Motorist_Instance
#undef Motorist_MakeKnot
#undef Motorist_HandleTag
#undef Motorist_Initialize
#undef Motorist_StartEngine
#undef Motorist_Wrap

#undef  DeclarePistonIndex
#if PlSnP_parallelism == 1
#undef  indexP
#endif
#undef  ForEveryPiston

#undef  State_StaticInitialize
#undef  State_Initialize
#undef  State_Permute
#undef  State_AddBytes
#undef  State_AddByte
#undef  State_OverwriteBytes
#undef  State_ExtractBytes
#undef  State_ExtractAndAddBytes
#if PlSnP_parallelism > 1
#undef  PlSnP_AddLanesAll
#undef  PlSnP_ExtractLanesAll
#undef  PlSnP_ExtractAndAddLanesAll
#endif
#undef  State_AddByteU
#undef  State_AddBytesU
