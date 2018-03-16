/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define performTestSponge               JOIN(prefix, _performTestSponge)
#define selfTestSponge                  JOIN(prefix, _selfTestSponge)
#define writeTestSponge                 JOIN(prefix, _writeTestSponge)
#define SpongeInstance                  JOIN(prefix, _SpongeInstance)
#define SpongeInitialize                JOIN(prefix, _SpongeInitialize)
#define SpongeAbsorb                    JOIN(prefix, _SpongeAbsorb)
#define SpongeAbsorbLastFewBits         JOIN(prefix, _SpongeAbsorbLastFewBits)
#define SpongeSqueeze                   JOIN(prefix, _SpongeSqueeze)
#define Sponge                          JOIN(prefix, _Sponge)

#define inputByteSize 512
#define outputByteSize 512
#define checksumByteSize 8

void performTestSponge(unsigned int rate, unsigned int capacity, int flavor, unsigned char *checksum)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    unsigned int inputBitLength, offset, size;
    int result;

    /* Acumulated test vector */
    SpongeInstance spongeChecksum;
    SpongeInitialize(&spongeChecksum, SnP_width, 0);

    for(inputBitLength = 0; inputBitLength <= (inputByteSize*8); inputBitLength += (inputBitLength < SnP_width) ? 1 : 8) {
        unsigned int i;
        unsigned int inputByteLengthCeiling = (inputBitLength + 7) / 8;

        for(i=0; i<inputByteLengthCeiling; i++)
            input[i] = inputBitLength - i;
        if ((inputBitLength % 8) != 0)
            input[inputByteLengthCeiling-1] &= (1 << (inputBitLength % 8)) - 1;

        if ((flavor == flavor_IUF_AllAtOnce) || (flavor == flavor_IUF_Pieces)) {
            SpongeInstance sponge;
            result = SpongeInitialize(&sponge, rate, capacity);
            assert(result == 0);

            if (flavor == flavor_IUF_Pieces) {
                for(offset = 0; offset < inputBitLength/8; offset += size) {
                    unsigned int p = (inputBitLength/8)%11;
                    /* vary sizes */
                    if (p < 1) size = 1; /* byte per byte */
                    else if (p < 5) size = (rand() % 20); /* small random */
                    else if (p < 9) size = (rand() % 200); /* big random */
                    else size = ((rand() % (inputBitLength/8 - offset)) + 1); /* converging random */

                    if (size > (inputBitLength/8 - offset))
                        size = inputBitLength/8 - offset;

                    result = SpongeAbsorb(&sponge, input + offset, size);
                    assert(result == 0);
                }
            }
            else {
                result = SpongeAbsorb(&sponge, input, inputBitLength/8);
                assert(result == 0);
            }
            if ((inputBitLength % 8) != 0) {
                result = SpongeAbsorbLastFewBits(&sponge, input[inputByteLengthCeiling-1] | (1 << (inputBitLength % 8)));
                assert(result == 0);
            }

            if (flavor == flavor_IUF_Pieces) {
                unsigned char filler = 0xAA + inputBitLength;
                memset(output, filler, sizeof(output));
                for(offset = 0; offset < outputByteSize; offset += size) {
                    unsigned int p = (inputBitLength/8)%11;
                    /* vary sizes */
                    if (p < 1) size = 1; /* byte per byte */
                    else if (p < 5) size = (rand() % 20); /* small random */
                    else if (p < 9) size = (rand() % 200); /* big random */
                    else size = ((rand() % (outputByteSize - offset)) + 1); /* converging random */

                    if (size > (outputByteSize - offset))
                        size = outputByteSize - offset;

                    result = SpongeSqueeze(&sponge, output + offset, size);
                    assert(result == 0);
                    for(i = offset + size; i<sizeof(output); i++)
                        if (output[i] != filler) {
                            #ifndef EMBEDDED
                            printf("Out of range data written!\n");
                            #endif
                            abort();
                        }
                }
            }
            else {
                result = SpongeSqueeze(&sponge, output, outputByteSize);
                assert(result == 0);
            }
        }
        else if (flavor == flavor_OneCall) {
            unsigned char suffix = ((inputBitLength % 8) != 0) ?
                input[inputByteLengthCeiling-1] | (1 << (inputBitLength % 8)) : 0x01;
            result = Sponge(rate, capacity, input, inputBitLength/8, suffix, output, outputByteSize);
            assert(result == 0);
        }
        
#ifdef VERBOSE
        printf("Keccak[r=%d, c=%d]\n", rate, capacity);
        printf("Input of %d bits:", inputBitLength);
        for(i=0; i<inputByteLengthCeiling; i++)
            printf(" %02x", (int)input[i]);
        printf("\n");
        printf("Output of %d bits:", outputByteSize*8);
        for(i=0; i<outputByteSize; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
#endif

        SpongeAbsorb(&spongeChecksum, output, outputByteSize);
    }
    SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);
#ifdef VERBOSE
    {
        unsigned int i;
        printf("Keccak[r=%d, c=%d]\n", rate, capacity);
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
#endif
}

void selfTestSponge(unsigned int rate, unsigned int capacity, int flavor, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    performTestSponge(rate, capacity, flavor, checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
}

#ifdef OUTPUT
void writeTestSponge(FILE *f, unsigned int rate, unsigned int capacity)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestSponge(rate, capacity, flavor_OneCall, checksum);
    fprintf(f, "    selfTestSponge(%d, %d, flavor, \"", rate, capacity);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\"); /* Keccak[r=%d, c=%d] */\n", rate, capacity);
}
#endif

#undef inputByteSize
#undef outputByteSize
#undef checksumByteSize

#undef performTestSponge
#undef selfTestSponge
#undef writeTestSponge
#undef SpongeInstance
#undef SpongeInitialize
#undef SpongeAbsorb
#undef SpongeAbsorbLastFewBits
#undef SpongeSqueeze
#undef Sponge
