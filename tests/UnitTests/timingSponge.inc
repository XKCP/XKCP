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

#define measureSponge                   JOIN(prefix, _measureSponge)
#define timingRC                        JOIN(prefix, _timingRC)
#define start                           JOIN(prefix, _start)
#define Sponge                          JOIN(prefix, _Sponge)
#define SpongeInstance                  JOIN(prefix, _SpongeInstance)
#define SpongeInitialize                JOIN(prefix, _SpongeInitialize)
#define SpongeAbsorb                    JOIN(prefix, _SpongeAbsorb)
#define SpongeSqueeze                   JOIN(prefix, _SpongeSqueeze)

#define Many 100
#define bufferSize 20000

uint_32t measureSponge(uint_32t dtMin, unsigned int r, unsigned int c, unsigned int lenIn, unsigned int lenOut)
{
    ALIGN(32) unsigned char I[bufferSize];
    ALIGN(32) unsigned char O[bufferSize];
    measureTimingDeclare

    assert(lenIn <= bufferSize);
    assert(lenOut <= bufferSize);

    memset(I, 0x12, bufferSize);
    memset(O, 0x34, bufferSize);

    measureTimingBeginDeclared
    Sponge(r, c, I, lenIn, 0x01, O, lenOut);
    measureTimingEnd
}

void timingRC(uint_32t calibration, unsigned int r, unsigned int c)
{
    unsigned int block = r/8;
    uint_32t oneBlockI = measureSponge(calibration, r, c, block-1, block);
    uint_32t twoBlocksI = measureSponge(calibration, r, c, 2*block-1, block);
    uint_32t manyBlocksI = measureSponge(calibration, r, c, Many*block-1, block);
    printf("Keccak[r=%4d, c=%4d]  %8d %9d %11d %6.2f %6.2f\n", r, c, oneBlockI, twoBlocksI, manyBlocksI, manyBlocksI/(1.0*Many*block-1), (manyBlocksI-twoBlocksI)/(1.0*(Many-2)*block));
}

uint_32t start(const char *synopsis, const char *implementation)
{
    uint_32t calibration = calibrate();
    printf("*** %s ***\nImplementation: %s\n\n", synopsis, implementation);
    printf("Cycles for " xstr(Sponge) "() with varying input sizes:\n");
    printf("                         1 block  2 blocks  100 blocks  ratio  slope\n");
    return calibration;
}

#undef measureSponge
#undef timingRC
#undef start
#undef Sponge
#undef SpongeInstance
#undef SpongeInitialize
#undef SpongeAbsorb
#undef SpongeSqueeze
#undef Many
#undef bufferSize
