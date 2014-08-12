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

#include "SnP-interface.h"
#include "NumberOfParallelInstances.h"

#define stateWithIndex(i) ((unsigned char *)states+(i*SnP_stateSizeInBytes))

void PlSnP_StaticInitialize_Serial( void )
{
    SnP_StaticInitialize();
}

void PlSnP_InitializeAll_Serial(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++)
        SnP_Initialize(stateWithIndex(i));
}

void PlSnP_XORBytes_Serial(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    SnP_XORBytes(stateWithIndex(instanceIndex), data, offset, length);
}

void PlSnP_XORLanesAll_Serial(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++) {
        SnP_XORBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        data += laneOffset*SnP_laneLengthInBytes;
    }
}

void PlSnP_OverwriteBytes_Serial(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    SnP_OverwriteBytes(stateWithIndex(instanceIndex), data, offset, length);
}

void PlSnP_OverwriteLanesAll_Serial(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++) {
        SnP_OverwriteBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        data += laneOffset*SnP_laneLengthInBytes;
    }
}

void PlSnP_OverwriteWithZeroes_Serial(void *states, unsigned int instanceIndex, unsigned int byteCount)
{
    SnP_OverwriteWithZeroes(stateWithIndex(instanceIndex), byteCount);
}

void PlSnP_ComplementBit_Serial(void *states, unsigned int instanceIndex, unsigned int position)
{
    SnP_ComplementBit(stateWithIndex(instanceIndex), position);
}

void PlSnP_ComplementBitAll_Serial(void *states, unsigned int position)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++)
        SnP_ComplementBit(stateWithIndex(i), position);
}

void PlSnP_Permute_Serial(void *states, unsigned int instanceIndex)
{
    SnP_Permute(stateWithIndex(instanceIndex));
}

void PlSnP_PermuteAll_Serial(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++)
        SnP_Permute(stateWithIndex(i));
}

void PlSnP_ExtractBytes_Serial(void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length)
{
    SnP_ExtractBytes(stateWithIndex(instanceIndex), data, offset, length);
}

void PlSnP_ExtractLanesAll_Serial(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++) {
        SnP_ExtractBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        data += laneOffset*SnP_laneLengthInBytes;
    }
}

void PlSnP_ExtractAndXORBytes_Serial(void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length)
{
    SnP_ExtractAndXORBytes(stateWithIndex(instanceIndex), data, offset, length);
}

void PlSnP_ExtractAndXORLanesAll_Serial(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_P; i++) {
        SnP_ExtractAndXORBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        data += laneOffset*SnP_laneLengthInBytes;
    }
}
