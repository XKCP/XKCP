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

#include "Keyakv2.h"

int Keyak_Initialize(Keyak_Instance *instance, 
	const unsigned char *key, unsigned int keySizeInBytes, 
	const unsigned char *nonce, unsigned int nonceSizeInBytes,
	int tagFlag, unsigned char * tag, 
	int unwrapFlag, int forgetFlag)
{
	unsigned char suv[Lk+MaxNoncelength];

	if ( (keySizeInBytes > (Lk-2)) || ((keySizeInBytes + nonceSizeInBytes) > (Lk-2+MaxNoncelength)) )
		return ( -1 );
    if ( Motorist_Initialize( &instance->motorist ) != 0 )
		return ( -1 );

	/*	Compose SUV */
	suv[0] = Lk;
	memcpy( &suv[1], key, keySizeInBytes );
	suv[1+keySizeInBytes] = 1;
	memset( &suv[1+keySizeInBytes+1], 0, Lk - (1+keySizeInBytes+1) );
	memcpy( &suv[Lk], nonce, nonceSizeInBytes );

	return ( Motorist_StartEngine( &instance->motorist, suv, Lk + nonceSizeInBytes, tagFlag, tag, unwrapFlag, forgetFlag) );
}

int Keyak_Wrap(Keyak_Instance *instance, const unsigned char *input, unsigned char *output, size_t dataSizeInBytes, 
	const unsigned char * AD, size_t ADlen, unsigned char * tag, int unwrapFlag, int forgetFlag )
{
	size_t	processedI, processedMD;

    return ( Keyak_WrapPartial( instance, input, output, dataSizeInBytes, AD, ADlen, tag, unwrapFlag, forgetFlag, Motorist_Wrap_LastCryptAndMeta, &processedI, &processedMD ) );
}
