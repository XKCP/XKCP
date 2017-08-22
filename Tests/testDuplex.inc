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

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define performTestDuplex               JOIN(prefix, _performTestDuplex)
#define selfTestDuplex                  JOIN(prefix, _selfTestDuplex)
#define writeTestDuplex                 JOIN(prefix, _writeTestDuplex)
#define DuplexInstance                  JOIN(prefix, _DuplexInstance)
#define DuplexInitialize                JOIN(prefix, _DuplexInitialize)
#define Duplexing                        JOIN(prefix, _Duplexing)
#define DuplexingFeedPartialInput       JOIN(prefix, _DuplexingFeedPartialInput)
#define DuplexingOverwritePartialInput  JOIN(prefix, _DuplexingOverwritePartialInput)
#define DuplexingOverwriteWithZeroes    JOIN(prefix, _DuplexingOverwriteWithZeroes)
#define DuplexingGetFurtherOutput       JOIN(prefix, _DuplexingGetFurtherOutput)
#define DuplexingGetFurtherOutputAndAdd JOIN(prefix, _DuplexingGetFurtherOutputAndAdd)
#define SpongeInstance                  JOIN(prefix, _SpongeInstance)
#define SpongeInitialize                JOIN(prefix, _SpongeInitialize)
#define SpongeAbsorb                    JOIN(prefix, _SpongeAbsorb)
#define SpongeSqueeze                   JOIN(prefix, _SpongeSqueeze)

#define checksumByteSize 8

void performTestDuplex(unsigned int rate, unsigned int capacity, int flavor, unsigned char *checksum)
{
    unsigned char sigma[SnP_width/8];
    unsigned char Z[SnP_width/8];
    unsigned int sigmaBitLength, ZByteLen, i, offset, size;
    DuplexInstance duplex;

    /* Acumulated test vector */
    SpongeInstance spongeChecksum;
    SpongeInitialize(&spongeChecksum, SnP_width, 0);

    DuplexInitialize(&duplex, rate, capacity);

    /* Varying input size, maximum output size */
    for(sigmaBitLength=0; sigmaBitLength<=rate-2; sigmaBitLength++) {
        unsigned int sigmaByteLenCeiling = (sigmaBitLength + 7)/8;
        unsigned char delimitedSigmaEnd;
        unsigned char filler = 0xAA + sigmaBitLength;

        ZByteLen = (rate + 7)/8;
        for(i=0; i<sigmaByteLenCeiling; i++)
            sigma[i] = (unsigned char)(sigmaBitLength - i);
        if ((sigmaBitLength % 8) != 0) {
            sigma[sigmaByteLenCeiling-1] &= (1 << (sigmaBitLength % 8)) - 1;
            delimitedSigmaEnd = sigma[sigmaByteLenCeiling-1] | (1 << (sigmaBitLength % 8));
        }
        else
            delimitedSigmaEnd = 0x01;

        memset(Z, filler, sizeof(Z));
        if (flavor == flavor_DuplexingOnly)
            Duplexing(&duplex, sigma, sigmaBitLength/8, Z, ZByteLen, delimitedSigmaEnd);
        else if (flavor == flavor_PartialIO) {
            for(offset = 0; offset < sigmaBitLength/8; offset += size) {
                unsigned int p = sigmaBitLength%11;
                /* vary sizes */
                if (p < 3) size = 1; /* byte per byte */
                else if (p < 9) size = (rand() % 20); /* small random */
                else size = ((rand() % (sigmaBitLength/8 - offset)) + 1); /* converging random */

                if (size > (sigmaBitLength/8 - offset))
                    size = sigmaBitLength/8 - offset;

                if (duplex.byteInputIndex != offset) {
                    #ifdef OUTPUT
                    printf("duplex.byteInputIndex = %d, offset = %d\n", duplex.byteInputIndex, offset);
                    #endif
                    abort();
                }
                DuplexingFeedPartialInput(&duplex, sigma+offset, size);
            }
            Duplexing(&duplex, 0, 0, 0, 0, delimitedSigmaEnd);
            for(offset = 0; offset < ZByteLen; offset += size) {
                unsigned int p = sigmaBitLength%7;
                /* vary sizes */
                if (p < 2) size = 1; /* byte per byte */
                else if (p < 5) size = (rand() % 20); /* small random */
                else size = ((rand() % (ZByteLen - offset)) + 1); /* converging random */

                if (size > (ZByteLen - offset))
                    size = ZByteLen - offset;

                if (duplex.byteOutputIndex != offset) {
                    #ifdef OUTPUT
                    printf("duplex.byteOutputIndex = %d, offset = %d\n", duplex.byteOutputIndex, offset);
                    #endif
                    abort();
                }
                DuplexingGetFurtherOutput(&duplex, Z+offset, size);
            }
        }
        else if (flavor == flavor_OverwriteAndAdd) {
            duplex.byteOutputIndex = 0;
            DuplexingGetFurtherOutputAndAdd(&duplex, sigma, sigma, sigmaBitLength/8);
            DuplexingOverwritePartialInput(&duplex, sigma, sigmaBitLength/8);
            Duplexing(&duplex, 0, 0, Z, ZByteLen, delimitedSigmaEnd);
        }

        SpongeAbsorb(&spongeChecksum, Z, ZByteLen);
        for(i=ZByteLen; i<sizeof(Z); i++)
            if (Z[i] != filler) {
                #ifdef OUTPUT
                printf("Out of range data written!\n");
                #endif
                abort();
            }
    }

    /* Testing overwriting with zeroes */
    for(sigmaBitLength=0; sigmaBitLength<=rate-2; sigmaBitLength+=8) {
        unsigned int ZByteLen = (rate + 7)/8;
        for(i=0; i<sigmaBitLength/8; i++)
            sigma[i] = (unsigned char)(sigmaBitLength - i);
        Duplexing(&duplex, sigma, sigmaBitLength/8, 0, 0, 0x01);
        if (flavor == flavor_DuplexingOnly) {
            DuplexingGetFurtherOutput(&duplex, sigma, sigmaBitLength/8);
            DuplexingFeedPartialInput(&duplex, sigma, sigmaBitLength/8);
        }
        else if (flavor == flavor_PartialIO) {
            memset(sigma, 0, sigmaBitLength/8);
            DuplexingOverwritePartialInput(&duplex, sigma, sigmaBitLength/8);
        }
        else if (flavor == flavor_OverwriteAndAdd) {
            DuplexingOverwriteWithZeroes(&duplex, sigmaBitLength/8);
        }
        Duplexing(&duplex, 0, 0, Z, ZByteLen, 0x01);
        SpongeAbsorb(&spongeChecksum, Z, ZByteLen);
    }

    /* No input, varying output size */
    for(ZByteLen=0; ZByteLen<=(rate+7)/8; ZByteLen++) {
        unsigned char filler = 0x33 + sigmaBitLength;

        memset(Z, filler, sizeof(Z));
        Duplexing(&duplex, 0, 0, Z, ZByteLen, 0x01);

        SpongeAbsorb(&spongeChecksum, Z, ZByteLen);
        for(i=ZByteLen; i<sizeof(Z); i++)
            if (Z[i] != filler) {
                #ifdef OUTPUT
                printf("Out of range data written!\n");
                #endif
                abort();
            }
    }
    SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);
}

void selfTestDuplex(unsigned int rate, unsigned int capacity, int flavor, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    performTestDuplex(rate, capacity, flavor, checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
}

#ifdef OUTPUT
void writeTestDuplex(FILE *f, unsigned int rate, unsigned int capacity, int flavor)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestDuplex(rate, capacity, flavor, checksum);
    fprintf(f, "    selfTestDuplex(%d, %d, flavor, \"", rate, capacity);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\"); /* Keccak[r=%d, c=%d] */\n" , rate, capacity);
}
#endif

#undef checksumByteSize

#undef performTestDuplex
#undef selfTestDuplex
#undef writeTestDuplex
#undef DuplexInstance
#undef DuplexInitialize
#undef Duplexing
#undef DuplexingFeedPartialInput
#undef DuplexingOverwritePartialInput
#undef DuplexingOverwriteWithZeroes
#undef DuplexingGetFurtherOutput
#undef DuplexingGetFurtherOutputAndAdd
#undef SpongeInstance
#undef SpongeInitialize
#undef SpongeAbsorb
#undef SpongeSqueeze
