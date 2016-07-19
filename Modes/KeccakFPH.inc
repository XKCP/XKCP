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

#define JOIN0(a, b)             a ## b
#define JOIN(a, b)              JOIN0(a, b)
#define Keccak_FPH              JOIN(Keccak_FPH,security)
#define Keccak_FPH_Initialize   JOIN(Keccak_FPH,_Initialize)
#define Keccak_FPH_Update       JOIN(Keccak_FPH,_Update)
#define Keccak_FPH_Final        JOIN(Keccak_FPH,_Final)
#define Keccak_FPH_Squeeze      JOIN(Keccak_FPH,_Squeeze)

#define capacity        (2*security)
#define capacityInBytes (capacity/8)
#define capacityInLanes (capacityInBytes/laneSize)
#define rate            (1600-capacity)
#define rateInBytes     (rate/8)
#define rateInLanes     (rateInBytes/laneSize)

#define ParallelSpongeFastLoop( Parallellism ) \
    while ( inLen >= Parallellism * fphInstance->blockLen ) { \
        ALIGN(KeccakP1600times##Parallellism##_statesAlignment) unsigned char states[KeccakP1600times##Parallellism##_statesSizeInBytes]; \
        unsigned char intermediate[Parallellism*capacityInBytes]; \
        unsigned int localBlockLen = fphInstance->blockLen; \
        const unsigned char * localInput = input; \
        unsigned int i; \
        unsigned int fastLoopOffset; \
        \
        KeccakP1600times##Parallellism##_StaticInitialize(); \
        KeccakP1600times##Parallellism##_InitializeAll(states); \
        fastLoopOffset = KeccakF1600times##Parallellism##_FastLoop_Absorb(states, rateInLanes, fphInstance->blockLen / laneSize, rateInLanes, localInput, Parallellism * fphInstance->blockLen); \
        localBlockLen -= fastLoopOffset; \
        localInput += fastLoopOffset; \
        for ( i = 0; i < Parallellism; ++i, localInput += fphInstance->blockLen ) { \
            KeccakP1600times##Parallellism##_AddBytes(states, i, localInput, 0, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, suffix, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, 0x80, rateInBytes-1); \
        } \
           KeccakP1600times##Parallellism##_PermuteAll_24rounds(states); \
        input += Parallellism * fphInstance->blockLen; \
        inLen -= Parallellism * fphInstance->blockLen; \
        KeccakP1600times##Parallellism##_ExtractLanesAll(states, intermediate, capacityInLanes, capacityInLanes ); \
        if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, intermediate, Parallellism * capacityInBytes) != 0) return 1; \
    }

#define ParallelSpongeLoop( Parallellism ) \
    while ( inLen >= Parallellism * fphInstance->blockLen ) { \
        ALIGN(KeccakP1600times##Parallellism##_statesAlignment) unsigned char states[KeccakP1600times##Parallellism##_statesSizeInBytes]; \
        unsigned char intermediate[Parallellism*capacityInBytes]; \
        unsigned int localBlockLen = fphInstance->blockLen; \
        const unsigned char * localInput = input; \
        unsigned int i; \
        \
        KeccakP1600times##Parallellism##_StaticInitialize(); \
        KeccakP1600times##Parallellism##_InitializeAll(states); \
           while(localBlockLen >= rateInBytes) { \
               KeccakP1600times##Parallellism##_AddLanesAll(states, localInput, rateInLanes, fphInstance->blockLen / laneSize); \
               KeccakP1600times##Parallellism##_PermuteAll_24rounds(states); \
            localBlockLen -= rateInBytes; \
            localInput += rateInBytes; \
           } \
        for ( i = 0; i < Parallellism; ++i, localInput += fphInstance->blockLen ) { \
            KeccakP1600times##Parallellism##_AddBytes(states, i, localInput, 0, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, suffix, localBlockLen); \
            KeccakP1600times##Parallellism##_AddByte(states, i, 0x80, rateInBytes-1); \
        } \
           KeccakP1600times##Parallellism##_PermuteAll_24rounds(states); \
        input += Parallellism * fphInstance->blockLen; \
        inLen -= Parallellism * fphInstance->blockLen; \
        KeccakP1600times##Parallellism##_ExtractLanesAll(states, intermediate, capacityInLanes, capacityInLanes ); \
        if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, intermediate, Parallellism * capacityInBytes) != 0) return 1; \
    }

int Keccak_FPH_Initialize(Keccak_FPH_Instance *fphInstance, unsigned int blockLen, size_t outputLen, const unsigned char * customization, size_t customLen)
{
    unsigned int t;
    unsigned char encbuf[sizeof(size_t)+1];

    if ( blockLen < 1024)   /* blockLen must be greater than or equal to 1024 bytes */
        return 1;
    for ( t = blockLen; t > 1; t >>= 1 )  /* blockLen (in bytes) must be a power of two */
        if ( (t & 1) && (t != 1) )  /* bit0 set and other bits unset */
            return 1;
    if (KeccakWidth1600_SpongeInitialize(&fphInstance->finalNode, rate, capacity) != 0)
        return 1;
    fphInstance->fixedOutputLength = outputLen;
    fphInstance->blockLen = blockLen;
    fphInstance->queueAbsorbedLen = 0;
    fphInstance->totalInputSize = 0;
    fphInstance->phase = ABSORBING;

    /* Absorb string_encode(customization) */
    if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, encbuf, left_enc(encbuf, customLen)) != 0)
        return 1;
    if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, customization, customLen) != 0)
        return 1;
        
    /* Absorb string_encode("fph") */
    if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, "\x01\x03" "fph", 5) != 0)
        return 1;

    /* Zero padding up to rate */
    if ( fphInstance->finalNode.byteIOIndex != 0 ) {
        fphInstance->finalNode.byteIOIndex = rateInBytes - 1;
        encbuf[0] = 0;
        if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, encbuf, 1) != 0)
            return 1;
    }

    /* Absorb B */
    if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, encbuf, left_enc(encbuf, blockLen)) != 0) 
        return 1;
    return 0;
}

int Keccak_FPH_Update(Keccak_FPH_Instance *fphInstance, const unsigned char *input, size_t inLen)
{

    if (fphInstance->phase != ABSORBING)
        return 1;
    fphInstance->totalInputSize += inLen;
    if ( fphInstance->queueAbsorbedLen != 0 ) {
        /* There is data in the queue, absorb further in queue until full */
        unsigned int len = (inLen < (fphInstance->blockLen - fphInstance->queueAbsorbedLen)) ? inLen : (fphInstance->blockLen - fphInstance->queueAbsorbedLen);
        if (KeccakWidth1600_SpongeAbsorb(&fphInstance->queueNode, input, len) != 0)
            return 1;
        input += len;
        inLen -= len;
        fphInstance->queueAbsorbedLen += len;
        if ( fphInstance->queueAbsorbedLen == fphInstance->blockLen ) {
            unsigned char intermediate[capacityInBytes];
            fphInstance->queueAbsorbedLen = 0;
            if (KeccakWidth1600_SpongeAbsorbLastFewBits(&fphInstance->queueNode, suffix) != 0)
                return 1;
            if (KeccakWidth1600_SpongeSqueeze(&fphInstance->queueNode, intermediate, capacityInBytes) != 0)
                return 1;
            if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, intermediate, capacityInBytes) != 0)
                return 1;
        }
    }

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

    while ( inLen > 0 ) {
        unsigned int len = (inLen < fphInstance->blockLen) ? inLen : fphInstance->blockLen;
        if (KeccakWidth1600_SpongeInitialize(&fphInstance->queueNode, rate, capacity) != 0)
            return 1;
        if (KeccakWidth1600_SpongeAbsorb(&fphInstance->queueNode, input, len) != 0)
            return 1;
        input += len;
        inLen -= len;
        if ( len == fphInstance->blockLen ) {
            unsigned char intermediate[capacityInBytes];
            if (KeccakWidth1600_SpongeAbsorbLastFewBits(&fphInstance->queueNode, suffix) != 0)
                return 1;
            if (KeccakWidth1600_SpongeSqueeze(&fphInstance->queueNode, intermediate, capacityInBytes) != 0)
                return 1;
            if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, intermediate, capacityInBytes) != 0)
                return 1;
        }
        else
            fphInstance->queueAbsorbedLen = len;
    }

    return 0;
}

int Keccak_FPH_Final(Keccak_FPH_Instance *fphInstance, unsigned char * output)
{
    unsigned char encbuf[sizeof(size_t)+1];
    size_t nBlocks;

    if (fphInstance->phase != ABSORBING)
        return 1;
    if ( fphInstance->queueAbsorbedLen != 0 ) {
        /* There is data in the queue */
        unsigned char intermediate[capacityInBytes];
        if (KeccakWidth1600_SpongeAbsorbLastFewBits(&fphInstance->queueNode, suffix) != 0)
            return 1;
        if (KeccakWidth1600_SpongeSqueeze(&fphInstance->queueNode, intermediate, capacityInBytes) != 0)
            return 1;
        if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, intermediate, capacityInBytes) != 0)
            return 1;
    }

    nBlocks = (fphInstance->totalInputSize + fphInstance->blockLen - 1) / fphInstance->blockLen;
    if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, encbuf, right_enc(encbuf, nBlocks)) != 0) /* Absorb number of blocks */
        return 1;
    if (KeccakWidth1600_SpongeAbsorb(&fphInstance->finalNode, encbuf, right_enc(encbuf, fphInstance->fixedOutputLength*8)) != 0) /* Absorb output length in bits */
        return 1;
    if (KeccakWidth1600_SpongeAbsorbLastFewBits(&fphInstance->finalNode, 0x04) != 0) /* Absorb 2 zero bits '00' */
        return 1;
    if ( fphInstance->fixedOutputLength != 0 ) {
        fphInstance->phase = FINAL;
        return KeccakWidth1600_SpongeSqueeze(&fphInstance->finalNode, output, fphInstance->fixedOutputLength);
    }
    fphInstance->phase = SQUEEZING;
    return 0;
}

int Keccak_FPH_Squeeze(Keccak_FPH_Instance *fphInstance, unsigned char * output, size_t outputLen)
{
    if (fphInstance->phase != SQUEEZING)
        return 1;
    return KeccakWidth1600_SpongeSqueeze(&fphInstance->finalNode, output, outputLen);
}

int Keccak_FPH( const unsigned char * input, size_t inLen, unsigned int blockLen, unsigned char * output, size_t outLen, const unsigned char * customization, size_t customLen )
{
    Keccak_FPH_Instance fph;

    if (outLen == 0)
        return 1;
    if (Keccak_FPH_Initialize(&fph, blockLen, outLen, customization, customLen) != 0)
        return 1;
    if (Keccak_FPH_Update(&fph, input, inLen) != 0)
        return 1;
    return Keccak_FPH_Final(&fph, output);
}

#undef  JOIN0
#undef  JOIN
#undef  Keccak_FPH
#undef  Keccak_FPH_Initialize
#undef  Keccak_FPH_Update
#undef  Keccak_FPH_Final
#undef  Keccak_FPH_Squeeze

#undef  capacity
#undef  capacityInBytes
#undef  capacityInLanes
#undef  rate
#undef  rateInBytes
#undef  rateInLanes

#undef  ParallelSpongeFastLoop
#undef  ParallelSpongeLoop
