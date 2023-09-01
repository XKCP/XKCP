/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/


#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define testXoodyakHashOne              JOIN(prefix, _testHashOne)
#define testXoodyakHash                 JOIN(prefix, _testHash)

#define Xoodyak_Instance                JOIN(prefix, _Instance)
#define Xoodyak_Initialize              JOIN(prefix, _Initialize)
#define Xoodyak_Absorb                  JOIN(prefix, _Absorb)
#define Xoodyak_Squeeze                 JOIN(prefix, _Squeeze)

#define	MAX_NUMBER_MESSAGES	3u
#define	MAX_MESSAGE_LEN		(3u*48u+1u)
#define	MAX_HASH_LEN		(3u*48u+1u)
#define	TYPICAL_HASH_LEN	32u

static void testXoodyakHashOne(Xoodyak_Instance * global, FILE *f, size_t messageLen, size_t hashLen, unsigned int numberOfMessages, unsigned int align)
{
	Xoodyak_Instance	instance;
	uint8_t				hashBuffer[MAX_HASH_LEN+3];
	uint8_t				messageBuffer[MAX_MESSAGE_LEN+3];
	unsigned int		i;

    Xoodyak_Initialize(&instance, NULL, 0, NULL, 0, NULL, 0);
	for (i = 0; i < numberOfMessages; ++i)	{
        generateSimpleRawMaterial(messageBuffer + align, messageLen, (uint8_t)(messageLen+hashLen+0x12), 3);
	    #if defined(UT_OUTPUT) || defined(UT_VERBOSE)
	    fprintf( f, "Absorb" );
	    UT_displayByteString(f, "> M", messageBuffer + align, messageLen);
	    #endif
	    Xoodyak_Absorb(&instance, messageBuffer + align, messageLen);
	}
    Xoodyak_Squeeze(&instance, hashBuffer + align, hashLen);
    #if defined(UT_OUTPUT) || defined(UT_VERBOSE)
    fprintf( f, "Squeeze" );
    UT_displayByteString(f, "> H", hashBuffer + align, hashLen);
    #endif
    Xoodyak_Absorb(global, hashBuffer + align, hashLen);

}

static void testXoodyakHash( const char *file, const uint8_t *expected )
{
	Xoodyak_Instance    global;
	uint8_t				checksum[TYPICAL_HASH_LEN];
    FILE                *f = NULL;
	size_t              messageLen;
	size_t              hashLen;
	unsigned int        numberOfMessages;
	unsigned int        align;

    #if defined(UT_OUTPUT)
    f = fopen(file, "w");
    UT_assert(f != NULL, (char*)"Could not open file");
    #elif defined(UT_VERBOSE)
    f = stdout;
    #endif

	for (align = 0; align < 4; ++align) {

	    Xoodyak_Initialize(&global, NULL, 0, NULL, 0, NULL, 0);

		hashLen = TYPICAL_HASH_LEN;
		for (numberOfMessages = 1u; numberOfMessages < MAX_NUMBER_MESSAGES; ++numberOfMessages) {
			for (messageLen = 0u; messageLen < MAX_MESSAGE_LEN; ++messageLen) {
				testXoodyakHashOne( &global, f, messageLen, hashLen, numberOfMessages, align );
			}
		}

		messageLen = MAX_MESSAGE_LEN;
		numberOfMessages = 1u;
		for (hashLen = 1u; hashLen < MAX_HASH_LEN; ++hashLen) {
			testXoodyakHashOne( &global, f, messageLen, hashLen, numberOfMessages, align );
		}

	    Xoodyak_Squeeze(&global, checksum, sizeof(checksum));
	    #if defined(UT_OUTPUT) || defined(UT_VERBOSE)
	    UT_displayByteString(f, "+++ Global checksum", checksum, sizeof(checksum));
	    #if defined(UT_OUTPUT)
	    fclose(f);
	    #endif
	    #endif
	    UT_assert(!memcmp(expected, checksum, sizeof(checksum)), (char*)"The global checksum is incorrect.");

	}

}

#undef Xoodyak_Instance
#undef Xoodyak_Initialize
#undef Xoodyak_Absorb
#undef Xoodyak_Squeeze

#undef testXoodyakHashOne
#undef testXoodyakHash
