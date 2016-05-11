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

#define JOIN0(a, b)     a ## b
#define JOIN(a, b)      JOIN0(a, b)
#define Keccak_FPH      JOIN(Keccak_FPH,security)

#define capacity        (2*security)
#define capacityInBytes (capacity/8)
#define capacityInLanes (capacityInBytes/laneSize)
#define rate            (1600-capacity)
#define rateInBytes     (rate/8)
#define rateInLanes     (rateInBytes/laneSize)

#define ParallelSpongeFastLoop( Parallellism ) \
    while ( inLen >= Parallellism * blockLen ) { \
        ALIGN(KeccakP1600times##Parallellism##_statesAlignment) unsigned char states[KeccakP1600times##Parallellism##_statesSizeInBytes]; \
        unsigned char intermediate[Parallellism*capacityInBytes]; \
        unsigned int localBlockLen = blockLen; \
        const unsigned char * localInput = input; \
        unsigned int i; \
        unsigned int fastLoopOffset; \
        \
        KeccakP1600times##Parallellism##_StaticInitialize(); \
        KeccakP1600times##Parallellism##_InitializeAll(states); \
        fastLoopOffset = KeccakF1600times##Parallellism##_FastLoop_Absorb(states, rateInLanes, blockLen / laneSize, rateInLanes, localInput, Parallellism * blockLen); \
        localBlockLen -= fastLoopOffset; \
        localInput += fastLoopOffset; \
        for ( i = 0; i < Parallellism; ++i, localInput += blockLen ) { \
            KeccakP1600times##Parallellism##_AddBytes(states, i, localInput, 0, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, suffix, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, 0x80, rateInBytes-1); \
        } \
           KeccakP1600times##Parallellism##_PermuteAll_24rounds(states); \
        input += Parallellism * blockLen; \
        inLen -= Parallellism * blockLen; \
        KeccakP1600times##Parallellism##_ExtractLanesAll(states, intermediate, capacityInLanes, capacityInLanes ); \
        if (KeccakWidth1600_SpongeAbsorb(&finalNode, intermediate, Parallellism * capacityInBytes) != 0) return 1; \
    }

#define ParallelSpongeLoop( Parallellism ) \
    while ( inLen >= Parallellism * blockLen ) { \
        ALIGN(KeccakP1600times##Parallellism##_statesAlignment) unsigned char states[KeccakP1600times##Parallellism##_statesSizeInBytes]; \
        unsigned char intermediate[Parallellism*capacityInBytes]; \
        unsigned int localBlockLen = blockLen; \
        const unsigned char * localInput = input; \
        unsigned int i; \
        \
        KeccakP1600times##Parallellism##_StaticInitialize(); \
        KeccakP1600times##Parallellism##_InitializeAll(states); \
           while(localBlockLen >= rateInBytes) { \
               KeccakP1600times##Parallellism##_AddLanesAll(states, localInput, rateInLanes, blockLen / laneSize); \
               KeccakP1600times##Parallellism##_PermuteAll_24rounds(states); \
            localBlockLen -= rateInBytes; \
            localInput += rateInBytes; \
           } \
        for ( i = 0; i < Parallellism; ++i, localInput += blockLen ) { \
            KeccakP1600times##Parallellism##_AddBytes(states, i, localInput, 0, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, suffix, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, 0x80, rateInBytes-1); \
        } \
           KeccakP1600times##Parallellism##_PermuteAll_24rounds(states); \
        input += Parallellism * blockLen; \
        inLen -= Parallellism * blockLen; \
        KeccakP1600times##Parallellism##_ExtractLanesAll(states, intermediate, capacityInLanes, capacityInLanes ); \
        if (KeccakWidth1600_SpongeAbsorb(&finalNode, intermediate, Parallellism * capacityInBytes) != 0) return 1; \
    }

int Keccak_FPH( const unsigned char * input, size_t inLen, unsigned int blockLen, unsigned char * output, size_t outLen, const unsigned char * customization, size_t customizationLen )
{
    KeccakWidth1600_SpongeInstance finalNode;
    unsigned int t;
    size_t nBlocks;
    unsigned char encbuf[sizeof(size_t)+1];

    if ( blockLen < 1024)   /* blockLen must be greater than or equal to 1024 bytes */
        return 1;
    for ( t = blockLen; t > 1; t >>= 1 )  /* blockLen (in bytes) must be a power of two */
        if ( (t & 1) && (t != 1) )  /* bit0 set and other bits unset */
            return 1;
    if (KeccakWidth1600_SpongeInitialize(&finalNode, rate, capacity) != 0)
        return 1;

    /* Absorb string_encode(customization) */
    if (KeccakWidth1600_SpongeAbsorb(&finalNode, encbuf, left_enc(encbuf, customizationLen)) != 0)
        return 1;
    if (KeccakWidth1600_SpongeAbsorb(&finalNode, customization, customizationLen) != 0)
        return 1;
        
    /* Absorb string_encode("fph") */
    if (KeccakWidth1600_SpongeAbsorb(&finalNode, "\x01\x03" "fph", 5) != 0)
        return 1;

    /* Zero padding up to rate */
    if ( finalNode.byteIOIndex != 0 )
    {
        finalNode.byteIOIndex = rateInBytes - 1;
        encbuf[0] = 0;
        if (KeccakWidth1600_SpongeAbsorb(&finalNode, encbuf, 1) != 0)
            return 1;
    }

    /* Absorb B */
    if (KeccakWidth1600_SpongeAbsorb(&finalNode, encbuf, left_enc(encbuf, blockLen)) != 0) 
        return 1;

    nBlocks = (inLen + blockLen - 1) / blockLen;

    #if defined(KeccakP1600times8_implementation)
    #if defined(KeccakF1600times8_FastLoop_supported)
    ParallelSpongeFastLoop( 8 )
    #else
    ParallelSpongeLoop( 8 )
    #endif
    #endif

    #if defined(KeccakP1600times4_implementation)
    #if defined(KeccakF1600times4_FastLoop_supported)
    ParallelSpongeFastLoop( 4 )
    #else
    ParallelSpongeLoop( 4 )
    #endif
    #endif

    #if defined(KeccakP1600times2_implementation)
    #if defined(KeccakF1600times2_FastLoop_supported)
    ParallelSpongeFastLoop( 2 )
    #else
    ParallelSpongeLoop( 2 )
    #endif
    #endif

    while ( inLen > 0 )
    {
        unsigned char intermediate[capacityInBytes];
        unsigned int localBlockLen = (inLen < blockLen) ? inLen : blockLen;

        if(KeccakWidth1600_Sponge(rate, capacity, input, localBlockLen, suffix, intermediate, capacityInBytes) != 0)
            return 1;
        input += localBlockLen;
        inLen -= localBlockLen;
        if (KeccakWidth1600_SpongeAbsorb(&finalNode, intermediate, capacityInBytes) != 0)
            return 1;
    }

    if (KeccakWidth1600_SpongeAbsorb(&finalNode, encbuf, right_enc(encbuf, nBlocks)) != 0) /* Absorb number of blocks */
        return 1;
    if (KeccakWidth1600_SpongeAbsorb(&finalNode, encbuf, right_enc(encbuf, outLen*8)) != 0) /* Absorb output length in bits */
        return 1;
    if (KeccakWidth1600_SpongeAbsorbLastFewBits(&finalNode, 0x04) != 0) /* Absorb 2 zero bits '00' */
        return 1;
    return KeccakWidth1600_SpongeSqueeze(&finalNode, output, outLen);
}

#undef  JOIN0
#undef  JOIN
#undef  Keccak_FPH

#undef  capacity
#undef  capacityInBytes
#undef  capacityInLanes
#undef  rate
#undef  rateInBytes
#undef  rateInLanes

#undef  ParallelSpongeFastLoop
#undef  ParallelSpongeLoop
