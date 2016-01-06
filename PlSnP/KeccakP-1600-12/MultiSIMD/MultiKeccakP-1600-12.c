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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PlSnP-interface.h"
#include "Lane-config.h"
#include "Multi-config.h"

#define	PlSnP_Multi									(PlSnP_P/LaneInterleaving)
#define StatePart(argStates,argIndex)				(void*)((unsigned char *)(argStates) + (argIndex) * (PlSnP_statesSizeInBytes / PlSnP_Multi))
#define DataPart(argData,argIndex,argLaneOffset)	((argData) + (argIndex) * LaneInterleaving * (argLaneOffset) * SnP_laneLengthInBytes)

void KeccakF1600_Pln_StaticInitialize( void )
{
}

void KeccakF1600_Pln_InitializeAll(void *states)
{
    memset(states, 0, PlSnP_statesSizeInBytes);
}

void KeccakF1600_Pln_XORBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
	mCall(_XORBytes)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving, data, offset, length);
}

void KeccakF1600_Pln_XORLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
	unsigned int n;

	for ( n = 0; n < PlSnP_Multi; ++n )
	{
		mCall(_XORLanesAll)( StatePart(states, n), DataPart(data, n, laneOffset), laneCount, laneOffset);
	}
}

void KeccakF1600_Pln_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
	mCall(_OverwriteBytes)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving, data, offset, length);
}

void KeccakF1600_Pln_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
	unsigned int n;

	for ( n = 0; n < PlSnP_Multi; ++n )
	{
		mCall(_OverwriteLanesAll)( StatePart(states, n), DataPart(data, n, laneOffset), laneCount, laneOffset);
	}
}

void KeccakF1600_Pln_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount)
{
	mCall(_OverwriteWithZeroes)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving, byteCount);
}

void KeccakF1600_Pln_ComplementBit(void *states, unsigned int instanceIndex, unsigned int position)
{
	mCall(_ComplementBit)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving, position);
}

void KeccakF1600_Pln_ComplementBitAll(void *states, unsigned int position)
{
	unsigned int n;

	for ( n = 0; n < PlSnP_Multi; ++n )
	{
		mCall(_ComplementBitAll)( StatePart(states, n), position );
	}
}

void KeccakF1600_Pln_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length)
{
	mCall(_ExtractBytes)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving, data, offset, length);
}

void KeccakF1600_Pln_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
	unsigned int n;

	for ( n = 0; n < PlSnP_Multi; ++n )
	{
		mCall(_ExtractLanesAll)( StatePart(states, n), DataPart(data, n, laneOffset), laneCount, laneOffset);
	}
}

void KeccakF1600_Pln_ExtractAndXORBytes(const void *states, unsigned int instanceIndex,  unsigned char *data, unsigned int offset, unsigned int length)
{
	mCall(_ExtractAndXORBytes)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving, data, offset, length);
}

void KeccakF1600_Pln_ExtractAndXORLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
	unsigned int n;

	for ( n = 0; n < PlSnP_Multi; ++n )
	{
		mCall(_ExtractAndXORLanesAll)( StatePart(states, n), DataPart(data, n, laneOffset), laneCount, laneOffset);
	}
}

void KeccakP1600_12_Pln_PermuteAll(void *states)
{
	unsigned int n;

	for ( n = 0; n < PlSnP_Multi; ++n )
	{
		mCallP(_PermuteAll)( StatePart(states, n) );
	}
}

void KeccakP1600_12_Pln_Permute(void *states, unsigned int instanceIndex)
{
	mCallP(_Permute)( StatePart(states, instanceIndex / LaneInterleaving), instanceIndex % LaneInterleaving );
}

