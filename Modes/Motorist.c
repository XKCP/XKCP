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

#include "Motorist.h"
#include <assert.h>

#ifdef OUTPUT
#include <stdio.h>

void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length);
#endif

#define Pistons_Phase_Fresh			0x00
#define Pistons_Phase_Running		0x01
#define Pistons_Phase_Full			0x02
#define Pistons_Phase_Done			0x04

#define Engine_Phase_Fresh			0x00
#define Engine_Phase_Crypting		0x01
#define Engine_Phase_Crypted		0x02
#define Engine_Phase_InjectOnly		0x04
#define Engine_Phase_EndOfMessage	0x08



#define OffsetEOM						(Ra+0)
#define OffsetCryptEnd					(Ra+1)
#define OffsetInjectStart				(Ra+2)
#define OffsetInjectEnd					(Ra+3)

#if PlSnP_P == 1

/*	Extended SnP	*/
#if !defined(SnP_XORByte)
	#define	SnP_XORByte(argS,argO,argV)		SnP_XORBytes(argS, &(argV), argO, 1)
#endif

#define DeclarePistonIndex
#define indexP				0
#define ForEveryPiston(argStart)

#define	State_StaticInitialize()								SnP_StaticInitialize()
#define	State_Initialize( argS )								SnP_Initialize( argS )
#define	State_Permute( argS )									SnP_Permute( argS )
#define	State_XORBytes( argS, argI, argO, argL )        		SnP_XORBytes(argS, argI, argO, argL)
#define	State_XORByte( argS, argO, argV )		        		SnP_XORByte(argS, argO, argV)
#define	State_OverwriteBytes( argS, argI, argO, argL )        	SnP_OverwriteBytes(argS, argI, argO, argL)
#define	State_ExtractBytes( argS, argI, argO, argL )       		SnP_ExtractBytes(argS, argI, argO, argL)
#define	State_ExtractAndXORBytes( argS, argI, argO, argL )		SnP_ExtractAndXORBytes(argS, argI, argO, argL)

#else

/*	Extended PlSnP	*/
#if !defined(PlSnP_XORByte)
	#define	PlSnP_XORByte(argS,argSI,argO,argV)			PlSnP_XORBytes(argS, argSI, &(argV), argO, 1)
#endif

#define DeclarePistonIndex	unsigned int indexP;
#define ForEveryPiston(argStart)		for ( indexP = argStart; indexP < PlSnP_P; ++indexP )

#define	State_StaticInitialize()								PlSnP_StaticInitialize()
#define	State_Initialize( argS )								PlSnP_InitializeAll( argS )
#define	State_Permute( argS )									PlSnP_PermuteAll( argS )
#define	State_XORBytes( argS, argI, argO, argL )        		PlSnP_XORBytes(argS, indexP, argI, argO, argL)
#define	State_XORByte( argS, argO, argV )		        		PlSnP_XORByte(argS, indexP, argO, argV)
#define	State_OverwriteBytes( argS, argI, argO, argL )        	PlSnP_OverwriteBytes(argS, indexP, argI, argO, argL)
#define	State_ExtractBytes( argS, argI, argO, argL )       		PlSnP_ExtractBytes(argS, indexP, argI, argO, argL)
#define	State_ExtractAndXORBytes( argS, argI, argO, argL )		PlSnP_ExtractAndXORBytes(argS, indexP, argI, argO, argL)

#endif

/* ------------------------------------------------------------------------ */

/**
  * Function that calls the permutation.
  *
  * @param  instance		    Pointer to the Engine instance structure.
  * @param  EOMFlag             
  *	@param  length              The length of the tag for first Piston (zero if no tag).
  *	@param  lengthNext          The length of the tag for next Pistons.
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
	unsigned char	s[PlSnP_P][SnP_width/8];
	#endif

    instance->tagEndIndex		= length;
	#if PlSnP_P > 1
    instance->tagEndIndexNext	= lengthNext;
	#endif
	if ( EOMFlag != 0 )
	{
		ForEveryPiston(0)
		{
			if ( length == 0 )
				length = 0xFF;
	   	    State_XORByte(instance->pistons.state, OffsetEOM, length );
			length = lengthNext;
		}
	}
	#ifdef OUTPUT
	if ( instance->pistons.file )
	{
		ForEveryPiston(0)
		{
			State_ExtractBytes( instance->pistons.stateShadow, s[indexP], 0, SnP_width/8 );
			State_ExtractAndXORBytes( instance->pistons.state, s[indexP], 0, SnP_width/8 );
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

int Engine_Initialize(Engine_Instance *instance )
{
	State_StaticInitialize();
	State_Initialize( instance->pistons.state );
	#ifdef OUTPUT
	instance->pistons.file = 0;
	State_Initialize( instance->pistons.stateShadow );
	#endif
	instance->tagEndIndex = 0;
	#if PlSnP_P > 1
	instance->tagEndIndexNext = 0;
	#endif
	instance->phase = Engine_Phase_Fresh;
	instance->pistons.phaseCrypt = Pistons_Phase_Fresh;
	instance->pistons.phaseInject = Pistons_Phase_Fresh;
	return ( Atom_Success );
}

int Engine_Crypt(Engine_Instance *instance, const unsigned char *I, size_t Ilen, unsigned char *O, int unwrapFlag, int lastFlag )
{
	DeclarePistonIndex
	unsigned char offset;
	unsigned int length;
	#if PlSnP_P > 1
	int processed;
	#endif

	if ( instance->pistons.phaseCrypt != Pistons_Phase_Running)
	{
		if ( instance->pistons.phaseCrypt != Pistons_Phase_Fresh )
		{
			return ( Atom_Error );		
		}
		#if PlSnP_P > 1
		instance->pistons.indexCrypt = 0;
		#endif
		instance->pistons.offsetCrypt = instance->tagEndIndex;
		instance->pistons.phaseCrypt = Pistons_Phase_Running;
	}

	#if PlSnP_P > 1
	processed = 0;
	#endif
	offset = instance->pistons.offsetCrypt;
	if ( unwrapFlag == Atom_False )
	{
		/*	Wrap	*/
		ForEveryPiston(instance->pistons.indexCrypt)
		{
			length = Rs - offset;
			if (length > Ilen)
				length = (unsigned int)Ilen;
	        State_XORBytes(instance->pistons.state, I, offset, length );
       		State_ExtractBytes(instance->pistons.state, O, offset, length );
			#if PlSnP_P > 1
			I += length;
			O += length;
			processed += length;
			#endif
			Ilen -= length;
			offset += (unsigned char)length;
			if ( offset == Rs )
			{
			    State_XORByte(instance->pistons.state, OffsetCryptEnd, offset);
				#if PlSnP_P > 1
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
		/*	Unwrap	*/
		ForEveryPiston(instance->pistons.indexCrypt)
		{
			length = Rs - offset;
			if (length > Ilen)
				length = (unsigned int)Ilen;
			if ( O != I )
				memcpy( O, I, length );
	        State_ExtractAndXORBytes(instance->pistons.state, O, offset, length);
    	    State_XORBytes(instance->pistons.state, O, offset, length);
			#if PlSnP_P > 1
			I += length;
			O += length;
			processed += length;
			#endif
			Ilen -= length;
			offset += (unsigned char)length;
			if ( offset == Rs )
			{
			    State_XORByte(instance->pistons.state, OffsetCryptEnd, offset);
				#if PlSnP_P > 1
				offset = instance->tagEndIndexNext;
				++instance->pistons.indexCrypt;
				#else
				instance->pistons.phaseCrypt = Pistons_Phase_Full;
				#endif
			}
		}
	}
	instance->pistons.offsetCrypt = offset;

	#if PlSnP_P > 1
	if ( instance->pistons.indexCrypt == PlSnP_P )
	{
		instance->pistons.phaseCrypt = Pistons_Phase_Full;
	}
	#endif
	if ( (Ilen == 0) && ((lastFlag & Motorist_Wrap_LastCryptData) != 0) )
	{
		/*	Done with crypting (out of fuel)	*/
		if ( instance->pistons.phaseCrypt != Pistons_Phase_Full )
		{
			ForEveryPiston(instance->pistons.indexCrypt)
			{
			    State_XORByte(instance->pistons.state, OffsetCryptEnd, offset);
				#if PlSnP_P > 1
				offset = instance->tagEndIndexNext;
				#endif
			}
		}
		instance->pistons.phaseCrypt = Pistons_Phase_Done;
		instance->phase = Engine_Phase_Crypted;
	}
	#if PlSnP_P > 1
	return ( processed );
	#else
	return ( length );
	#endif
}

int Engine_Inject(Engine_Instance *instance, const unsigned char * A, size_t Alen, int lastFlag)
{
	DeclarePistonIndex
	unsigned int length;
	#if PlSnP_P > 1
	int processed;
	#endif

	if ( instance->pistons.phaseInject != Pistons_Phase_Running )
	{
		if ( instance->pistons.phaseInject != Engine_Phase_Fresh )
		{
			return ( Atom_Error );
		}
		#if PlSnP_P > 1
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
	   		    State_XORByte(instance->pistons.state, OffsetInjectStart, instance->pistons.offsetInject);
			}
		}
		instance->pistons.phaseInject = Pistons_Phase_Running;
	}

	#if PlSnP_P > 1
	processed = 0;
	#endif
	ForEveryPiston(instance->pistons.indexInject)
	{
		length = Ra - instance->pistons.offsetInject;
		if (length > Alen)
			length = (unsigned int)Alen;
        State_XORBytes(instance->pistons.state, A, instance->pistons.offsetInject, length );
		#if PlSnP_P > 1
		A += length;
		processed += (int)length;
		#endif
		Alen -= length;
		instance->pistons.offsetInject += (unsigned char)length;
		if ( instance->pistons.offsetInject == Ra )
		{
		    State_XORByte(instance->pistons.state, OffsetInjectEnd, instance->pistons.offsetInject);
			#if PlSnP_P > 1
			instance->pistons.offsetInject = (instance->pistons.phaseCrypt == Pistons_Phase_Fresh) ? 0 : Rs;
			++instance->pistons.indexInject;
			#else
			instance->pistons.phaseInject = Pistons_Phase_Full;
			#endif
		}
	}

	#if PlSnP_P > 1
	if ( instance->pistons.indexInject == PlSnP_P )
	{
		instance->pistons.phaseInject = Pistons_Phase_Full;
	}
	#endif
	if ( (Alen == 0) && ((lastFlag & Motorist_Wrap_LastMetaData) != 0) )
	{
		/* Done injecting */
		if ( (instance->pistons.phaseInject != Pistons_Phase_Full) && (instance->pistons.offsetInject != 0) )	/* Optimization: don't xor zeroes */
		{
			ForEveryPiston(instance->pistons.indexInject)
			{
			    State_XORByte(instance->pistons.state, OffsetInjectEnd, instance->pistons.offsetInject);
				#if PlSnP_P > 1
				if ( instance->pistons.phaseCrypt == Pistons_Phase_Fresh )
					break;
				instance->pistons.offsetInject = Rs;
				#endif
			}
		}
		instance->pistons.phaseInject = Pistons_Phase_Done;
	}
	#if PlSnP_P > 1
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
		partSize = sizeX;
	*pOffset += partSize;

	ForEveryPiston(0)
	{
   	    State_XORByte(instance->pistons.state, OffsetInjectEnd, offset); /* remove previous OffsetInjectEnd */
        State_XORBytes(instance->pistons.state, X, offset, partSize );
   	    State_XORByte(instance->pistons.state, OffsetInjectEnd, *pOffset);
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
	int	dataAvailableFlag = 0;

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
		if ( (offset == 0) && (dataAvailableFlag != 0) )	/*	spark the last full block */
			Engine_Spark(instance, 0, 0, 0 );
		diversifier[0] = PlSnP_P;
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
	#if PlSnP_P != 1
	if ( lengthNext > Rs )
		return ( Atom_Error );
	#endif

	ForEveryPiston(0)
	{
   	    State_ExtractBytes(instance->pistons.state, tag, 0, length );
		#if PlSnP_P != 1
		tag += length;
		length = lengthNext;
		#endif
	}
	instance->phase = Engine_Phase_Fresh;
	return ( Atom_Success );
}

/* ------------------------------------------------------------------------ */

#define Motorist_Phase_Ready			0x01
#define Motorist_Phase_Riding			0x02
#define Motorist_Phase_Failed			0x04

static int Motorist_MakeKnot(Motorist_Instance *instance );
static int Motorist_MakeKnot(Motorist_Instance *instance )
{
	unsigned char tempTags[PlSnP_P*Cprime];

	if ( Engine_GetTags(&instance->engine, tempTags, Cprime, Cprime ) < 0 )
		return ( Atom_Error );
	return ( Engine_InjectCollective(&instance->engine, tempTags, PlSnP_P * Cprime, 0) );
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
	int				resultI;
	int				resultA;
	unsigned char	*initialO;
	size_t			initialOlen;

	initialO = O;
	initialOlen = Ilen;
	*processedIlen = 0;
	*processedAlen = 0;
	if ( instance->phase != Motorist_Phase_Riding )
		return ( Atom_Error );

	/*	Once a lastFlag has been set, it must remain set during the session	*/
	if ( ((instance->lastFlag & Motorist_Wrap_LastCryptData) != 0) && ((lastFlag & Motorist_Wrap_LastCryptData) == 0) )
		return ( Atom_Error );
	if ( ((instance->lastFlag & Motorist_Wrap_LastMetaData) != 0) && ((lastFlag & Motorist_Wrap_LastMetaData) == 0) )
		return ( Atom_Error );
	instance->lastFlag  = lastFlag;

	if ( instance->engine.phase == Engine_Phase_Fresh )
	{
	 	if ( Ilen != 0 ) 
		{
			/*	Caller wants to crypt. */
			instance->engine.phase = Engine_Phase_Crypting;
		}
		else if ( (lastFlag & Motorist_Wrap_LastCryptData) == 0 )
		{
			/*	Caller does not give input, but does not set lastCrypt flag, 
			**	so we don't know how the phase will evolve, do nothing.	
			*/ 
			return ( Atom_True );
		}
		else
		{
			/* Only metadata can follow (input is empty) */
			instance->engine.phase = Engine_Phase_InjectOnly;
		 	if ( (Alen == 0) && ((lastFlag & Motorist_Wrap_LastMetaData) != 0) )
			{
				/*	Nothing to inject either, perform empty inject */
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
			/*	If more data available and Crypter and Injector are full, then spark */
			if (	((Ilen | Alen) != 0) 
				&&	(instance->engine.pistons.phaseCrypt == Pistons_Phase_Full)
				&&	(instance->engine.pistons.phaseInject >= Pistons_Phase_Full) ) /* Full or Done */
			{
				Engine_Spark(&instance->engine, 0, 0, 0 );
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

			if (	(instance->engine.pistons.phaseCrypt == Pistons_Phase_Done)
				&&	(instance->engine.pistons.phaseInject == Pistons_Phase_Done))
			{
				instance->engine.phase = Engine_Phase_EndOfMessage;
				break;
			}

			if ( (resultI | resultA) == 0 )
			{
				/* 	Can't do more than that */
				return ( Atom_True );
			}
		}
	}

	/*	Input all done, continue injecting	*/
	if (	(instance->engine.phase == Engine_Phase_Crypted)
		||	(instance->engine.phase == Engine_Phase_InjectOnly) )
	{
		while ( Alen != 0 )
		{
			/*	If more data available and Injector is full, then spark */
			if ( instance->engine.pistons.phaseInject == Pistons_Phase_Full )
			{
				Engine_Spark(&instance->engine, 0, 0, 0 );
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

		/*	Everything is processed	*/
		#if PlSnP_P == 1
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
