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

#define measureWrap                     JOIN(prefix, _measureWrap)
#define timing                          JOIN(prefix, _timing)
#define Keyak_Instance                  JOIN(prefix, _Instance)
#define Keyak_Initialize                JOIN(prefix, _Initialize)
#define Keyak_Wrap                      JOIN(prefix, _Wrap)
#define BlockRs                         (P*Rs)
#define BlockRa                         (P*Ra)
#define BlockRsRa                       (BlockRa-BlockRs)

#define Many 100
#define bufferSize (25000*P)

uint_32t measureWrap(uint_32t dtMin, unsigned int lenI, unsigned int lenA)
{
    Keyak_Instance keyak;
    ALIGN(32) unsigned char key[16];
    ALIGN(32) unsigned char nonce[16];
    ALIGN(32) unsigned char I[bufferSize];
    ALIGN(32) unsigned char O[bufferSize];
    ALIGN(32) unsigned char A[bufferSize];
    ALIGN(32) unsigned char T[16];
    measureTimingDeclare

    assert(lenI <= bufferSize);
    assert(lenA <= bufferSize);

    memset(key, 0x00, 16);
    memset(nonce, 0x00, 16);
    memset(I, 0x22, bufferSize);
    memset(O, 0x33, bufferSize);
    memset(A, 0x44, bufferSize);
    memset(T, 0x55, 16);

    measureTimingBeginDeclared
    Keyak_Initialize(&keyak, key, 16, nonce, 16, 0, (unsigned char *)0, 0, 0);
    Keyak_Wrap(&keyak, I, O, lenI, A, lenA, T, 0, 0);
    measureTimingEnd
}

void timing(const char *synopsis, const char *implementation)
{
    uint_32t calibration = calibrate();
    uint_32t oneBlockIA = measureWrap(calibration, BlockRs, BlockRsRa);
    uint_32t twoBlocksIA = measureWrap(calibration, 2*BlockRs, 2*BlockRsRa);
    uint_32t manyBlocksIA = measureWrap(calibration, Many*BlockRs, Many*BlockRsRa);
    uint_32t oneBlockI = measureWrap(calibration, BlockRs, 0);
    uint_32t twoBlocksI = measureWrap(calibration, 2*BlockRs, 0);
    uint_32t manyBlocksI = measureWrap(calibration, Many*BlockRs, 0);
    uint_32t oneBlockA = measureWrap(calibration, 0, BlockRa);
    uint_32t twoBlocksA = measureWrap(calibration, 0, 2*BlockRa);
    uint_32t manyBlocksA = measureWrap(calibration, 0, Many*BlockRa);
    printf("*** %s ***\nImplementation: %s\n\n", synopsis, implementation);
    printf("Cycles for " xstr(Keyak_Initialize) "() + " xstr(Keyak_Wrap) "():\n");
    printf("          I    A    1 block  2 blocks  100 blocks  ratio  slope\n");
    printf("I/A:   %4d %4d   %8d %9d %11d %6.2f %6.2f\n", BlockRs, BlockRsRa, oneBlockIA, twoBlocksIA, manyBlocksIA, manyBlocksIA/(1.0*Many*BlockRa), (manyBlocksIA-twoBlocksIA)/(1.0*(Many-2)*BlockRa));
    printf("I/.:   %4d %4d   %8d %9d %11d %6.2f %6.2f\n", BlockRs, 0, oneBlockI, twoBlocksI, manyBlocksI, manyBlocksI/(1.0*Many*BlockRs), (manyBlocksI-twoBlocksI)/(1.0*(Many-2)*BlockRs));
    printf("A/A:   %4d %4d   %8d %9d %11d %6.2f %6.2f\n", 0, BlockRa, oneBlockA, twoBlocksA, manyBlocksA, manyBlocksA/(1.0*Many*BlockRa), (manyBlocksA-twoBlocksA)/(1.0*(Many-2)*BlockRa));
    printf("\n\n");
}

#undef measureWrap
#undef timing
#undef Keyak_Instance
#undef Keyak_Initialize
#undef Keyak_Wrap
#undef BlockRs
#undef BlockRa
#undef BlockRsRa
#undef Many
#undef bufferSize
