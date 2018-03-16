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

#define performTestSpongePRG            JOIN(prefix, _performTestSpongePRG)
#define selfTestSpongePRG               JOIN(prefix, _selfTestSpongePRG)
#define writeTestSpongePRG              JOIN(prefix, _writeTestSpongePRG)

#define SpongeInstance                  JOIN(prefix, _SpongeInstance)
#define SpongeInitialize                JOIN(prefix, _SpongeInitialize)
#define SpongeAbsorb                    JOIN(prefix, _SpongeAbsorb)
#define SpongeSqueeze                   JOIN(prefix, _SpongeSqueeze)

#define SpongePRG_Instance              JOIN(prefix, _SpongePRG_Instance)
#define SpongePRG_Initialize            JOIN(prefix, _SpongePRG_Initialize)
#define SpongePRG_Feed                  JOIN(prefix, _SpongePRG_Feed)
#define SpongePRG_Fetch                 JOIN(prefix, _SpongePRG_Fetch)
#define SpongePRG_Forget                JOIN(prefix, _SpongePRG_Forget)

#define inputByteSize 512
#define outputByteSize 512
#define checksumByteSize 8

void performTestSpongePRG(unsigned int rhoInBytes, int flavor, unsigned char *checksum)
{
    unsigned char input[inputByteSize];
    unsigned char output[outputByteSize];
    unsigned int inputByteLength, outputByteLength, offset, size;
    int result;
    unsigned int capacity = SnP_width-(8*rhoInBytes+2);

    /* Acumulated test vector */
    SpongeInstance spongeChecksum;
    SpongeInitialize(&spongeChecksum, SnP_width, 0);

    for(inputByteLength = 0; inputByteLength <= inputByteSize; inputByteLength += (inputByteLength < (SnP_width/8)) ? 1 : 7)
    {
        unsigned int i;
        outputByteLength = (outputByteSize-3+inputByteLength*3)%outputByteSize;

        for(i=0; i<inputByteLength; i++)
            input[i] = inputByteLength - i;

        if ((flavor == flavor_IUF_AllAtOnce) || (flavor == flavor_IUF_Pieces))
        {
            SpongePRG_Instance prg;

            result = SpongePRG_Initialize(&prg, capacity);
            assert(result == 0);

            if (flavor == flavor_IUF_Pieces)
            {
                for(offset = 0; offset < inputByteLength; offset += size)
                {
                    unsigned int p = inputByteLength%11;
                    /* vary sizes */
                    if (p < 1) size = 1; /* byte per byte */
                    else if (p < 5) size = (rand() % 20); /* small random */
                    else if (p < 9) size = (rand() % 200); /* big random */
                    else size = ((rand() % (inputByteLength - offset)) + 1); /* converging random */

                    if (size > (inputByteLength - offset))
                        size = inputByteLength - offset;

                    result = SpongePRG_Feed(&prg, input + offset, size);
                    assert(result == 0);
                }
            }
            else
            {
                result = SpongePRG_Feed(&prg, input, inputByteLength);
                assert(result == 0);
            }

            if (flavor == flavor_IUF_Pieces)
            {
                unsigned char filler = 0xAA + inputByteLength;
                memset(output, filler, sizeof(output));
                for(offset = 0; offset < outputByteLength; offset += size)
                {
                    unsigned int p = inputByteLength%11;
                    /* vary sizes */
                    if (p < 1) size = 1; /* byte per byte */
                    else if (p < 5) size = (rand() % 20); /* small random */
                    else if (p < 9) size = (rand() % 200); /* big random */
                    else size = ((rand() % (outputByteLength - offset)) + 1); /* converging random */

                    if (size > (outputByteLength - offset))
                        size = outputByteLength - offset;

                    result = SpongePRG_Fetch(&prg, output + offset, size);
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
            else
            {
                result = SpongePRG_Fetch(&prg, output, outputByteLength);
                assert(result == 0);
            }
            if ((outputByteLength > 0) && (output[0] & 1 ))
            {
                if (output[0] & 2) {
                    result = SpongePRG_Feed(&prg, input, inputByteLength);
                    assert(result == 0);
                }
                result = SpongePRG_Forget(&prg);
                assert(result == (((rhoInBytes*8) >= capacity) ? 0 : 1));
                if (output[0] & 4) {
                    result = SpongePRG_Fetch(&prg, output, outputByteLength);
                    assert(result == 0);
                }
            }
        }

#ifdef VERBOSE
        printf("Keccak[r=%d, c=%d]\n", rhoInBytes*8+2, SnP_width-(rhoInBytes*8+2));
        printf("Input of %d bytes:", inputByteLength);
        for(i=0; i<inputByteLength; i++)
            printf(" %02x", (int)input[i]);
        printf("\n");
        printf("Output of %d bytes:", outputByteLength);
        for(i=0; i<outputByteLength; i++)
            printf(" %02x", (int)output[i]);
        printf("\n\n");
#endif

        SpongeAbsorb(&spongeChecksum, output, outputByteLength);
    }
    SpongeSqueeze(&spongeChecksum, checksum, checksumByteSize);
#ifdef VERBOSE
    {
        unsigned int i;
        printf("Keccak[r=%d, c=%d]\n", rhoInBytes*8+2, SnP_width-(rhoInBytes*8+2));
        printf("Checksum: ");
        for(i=0; i<checksumByteSize; i++)
            printf("\\x%02x", (int)checksum[i]);
        printf("\n\n");
    }
#endif
}

void selfTestSpongePRG(unsigned int rhoInBytes, int flavor, const unsigned char *expected)
{
    unsigned char checksum[checksumByteSize];

    performTestSpongePRG(rhoInBytes, flavor, checksum);
    assert(memcmp(expected, checksum, checksumByteSize) == 0);
}

#ifdef OUTPUT
void writeTestSpongePRG(FILE *f, unsigned int rhoInBytes)
{
    unsigned char checksum[checksumByteSize];
    unsigned int offset;

    performTestSpongePRG(rhoInBytes, flavor_IUF_AllAtOnce, checksum);
    fprintf(f, "    selfTestSpongePRG(%d, %d, flavor, \"", rhoInBytes, SnP_width);
    for(offset=0; offset<checksumByteSize; offset++)
        fprintf(f, "\\x%02x", checksum[offset]);
    fprintf(f, "\"); /* Keccak[r=%d, c=%d] */\n", rhoInBytes*8+2, SnP_width-(rhoInBytes*8+2) );
}
#endif

#undef inputByteSize
#undef outputByteSize
#undef checksumByteSize

#undef performTestSpongePRG
#undef selfTestSpongePRG
#undef writeTestSpongePRG

#undef SpongeInstance
#undef SpongeInitialize
#undef SpongeAbsorb
#undef SpongeSqueeze

#undef SpongePRG_Instance
#undef SpongePRG_Initialize
#undef SpongePRG_Feed
#undef SpongePRG_Fetch
#undef SpongePRG_Forget
