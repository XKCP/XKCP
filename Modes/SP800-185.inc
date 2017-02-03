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

#define capacity        (2*security)
#define capacityInBytes (capacity/8)
#define capacityInLanes (capacityInBytes/laneSize)
#define rate            (1600-capacity)
#define rateInBytes     (rate/8)
#define rateInLanes     (rateInBytes/laneSize)


#define cSHAKE              JOIN(cSHAKE,security)
#define cSHAKE_Initialize   JOIN(cSHAKE,_Initialize)
#define cSHAKE_Update       JOIN(cSHAKE,_Update)
#define cSHAKE_Final        JOIN(cSHAKE,_Final)
#define cSHAKE_Squeeze      JOIN(cSHAKE,_Squeeze)

int cSHAKE_Initialize(cSHAKE_Instance *csk, BitLength outputBitLen, const BitSequence *name, BitLength nameBitLen, const BitSequence *customization, BitLength customBitLen)
{
    unsigned char encbuf[sizeof(BitLength)+1];

    /* Only full bytes are supported for 'name', otherwise customization string would have to be shifted before absorbing */
    if ((nameBitLen & 7) != 0)
        return 1;
    if (KeccakWidth1600_SpongeInitialize(&csk->sponge, rate, capacity) != 0)
        return 1;
    csk->lastByteBitLen = 0;
    csk->lastByteValue = 0;
    csk->fixedOutputLength = outputBitLen;
    csk->phase = ABSORBING;

    /* Absorb bytepad(.., rate) */
    if (KeccakWidth1600_SpongeAbsorb(&csk->sponge, encbuf, left_encode(encbuf, rateInBytes)) != 0)
        return 1;

    /* Absorb encode_string(name) */
    if (KeccakWidth1600_SpongeAbsorb(&csk->sponge, encbuf, left_encode(encbuf, nameBitLen)) != 0)
        return 1;
    if (KeccakWidth1600_SpongeAbsorb(&csk->sponge, name, nameBitLen / 8) != 0)
        return 1;

    /* Absorb encode_string(customization) */
    if (KeccakWidth1600_SpongeAbsorb(&csk->sponge, encbuf, left_encode(encbuf, customBitLen)) != 0)
        return 1;
    if (KeccakWidth1600_SpongeAbsorb(&csk->sponge, customization, (customBitLen + 7) / 8) != 0) /* allowed to be a bit string, as zero padding is following */
        return 1;
        
    /* Zero padding up to rate */
    if ( csk->sponge.byteIOIndex != 0 ) {
        csk->sponge.byteIOIndex = rateInBytes - 1;
        encbuf[0] = 0;
        return KeccakWidth1600_SpongeAbsorb(&csk->sponge, encbuf, 1);
    }
    return 0;
}

int cSHAKE_Update(cSHAKE_Instance *csk, const BitSequence *input, BitLength inputBitLen)
{

    if (csk->phase != ABSORBING)
        return 1;
    if (csk->lastByteBitLen != 0)    /* check if previous call input were full bytes */
        return 1;
    csk->lastByteBitLen = inputBitLen & 7;
    if(csk->lastByteBitLen != 0)
        csk->lastByteValue = input[inputBitLen / 8] & ((1 << csk->lastByteBitLen) - 1); /* strip unwanted bits */
    return KeccakWidth1600_SpongeAbsorb(&csk->sponge, input, inputBitLen / 8);
}

int cSHAKE_Final(cSHAKE_Instance *csk, BitSequence *output)
{
    unsigned short delimitedLastBytes;
    unsigned char delimitedSuffix;

    if (csk->phase != ABSORBING)
        return 1;

    /* Concatenate the last few input bits with those of the suffix */
    delimitedLastBytes = (unsigned short)(csk->lastByteValue | (0x04 << csk->lastByteBitLen)); /* Suffix '04': 2 zero bits '00' */
    if ((delimitedLastBytes >> 8) == 0) {
        delimitedSuffix = (unsigned char)delimitedLastBytes;
    }
    else {
        unsigned char oneByte[1];
        oneByte[0] = (unsigned char)delimitedLastBytes;
        if(KeccakWidth1600_SpongeAbsorb(&csk->sponge, oneByte, 1) != 0)
            return 1;
        delimitedSuffix = (unsigned char)(delimitedLastBytes >> 8);
    }
    if (KeccakWidth1600_SpongeAbsorbLastFewBits(&csk->sponge, delimitedSuffix) != 0) 
        return 1;
    csk->phase = SQUEEZING;
    if ( csk->fixedOutputLength != 0 ) {
        if(cSHAKE_Squeeze(csk, output, csk->fixedOutputLength) != 0)
            return 1;
        csk->phase = FINAL;
    }
    return 0;
}

int cSHAKE_Squeeze(cSHAKE_Instance *csk, BitSequence *output, BitLength outputBitLen)
{
    if (csk->phase != SQUEEZING)
        return 1;
    if(KeccakWidth1600_SpongeSqueeze(&csk->sponge, output, (outputBitLen + 7) / 8) != 0)
        return 1;
    if ((outputBitLen & 7) !=0) {
        output[outputBitLen / 8] &= (1 << (outputBitLen & 7)) - 1; /* clear unwanted bits */
        csk->phase = FINAL; /* only last output can have an non complete byte, block nexts calls */
    }
    return 0;
}

int cSHAKE( const BitSequence *input, BitLength inputBitLen, BitSequence *output, BitLength outputBitLen, 
        const BitSequence *name, BitLength nameBitLen, const BitSequence *customization, BitLength customBitLen )
{
    cSHAKE_Instance csk;

    if (outputBitLen == 0)
        return 1;
    if (cSHAKE_Initialize(&csk, outputBitLen, name, nameBitLen, customization, customBitLen) != 0)
        return 1;
    if (cSHAKE_Update(&csk, input, inputBitLen) != 0)
        return 1;
    return cSHAKE_Final(&csk, output);
}

#undef  cSHAKE_Initialize
#undef  cSHAKE_Update
#undef  cSHAKE_Final
#undef  cSHAKE_Squeeze
#undef  cSHAKE

/*
    todo Other algos
*/

#undef  JOIN0
#undef  JOIN

#undef  capacity
#undef  capacityInBytes
#undef  capacityInLanes
#undef  rate
#undef  rateInBytes
#undef  rateInLanes
