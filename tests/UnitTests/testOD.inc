/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define performTestOD                   JOIN(prefix, _performTestOD)
#define selfTestOD                      JOIN(prefix, _selfTestOD)
#define writeTestOD                     JOIN(prefix, _writeTestOD)
#define ODInstance                      JOIN(inst, _ODInstance)
#define OD_Initialize                   JOIN(prefix, _OD_Initialize)
#define OD_Clone                        JOIN(prefix, _OD_Clone)
#define OD_CloneCompact                 JOIN(prefix, _OD_CloneCompact)
#define OD_Duplexing                    JOIN(prefix, _OD_Duplexing)
#define OD_Squeezing                    JOIN(prefix, _OD_Squeezing)
#define OD_DuplexingFast                JOIN(prefix, _OD_DuplexingFast)
#define OD_DuplexingFastOnlyOut         JOIN(prefix, _OD_DuplexingFastOnlyOut)
#define OD_DuplexingFastOnlyIn          JOIN(prefix, _OD_DuplexingFastOnlyIn)

UT_arbitraryChecksum performTestOD(unsigned int rho, unsigned int capacity)
{
    ODInstance od;
    uint32_t seed = 0x8B05AF71;
    unsigned int keyLength = capacity / 8 / 2;
    unsigned char key[rho];
    UT_generateArbitraryMaterial(key, rho, seed++);

    UT_arbitraryChecksumState checksum;
    UT_arbitraryChecksumInitialize(&checksum);

    {
        unsigned int ilen;
        UT_assert(keyLength <= rho);

        OD_Initialize(&od, rho, capacity, key, keyLength);

        for(ilen=0; ilen<=rho; ilen++) {
            unsigned char E = (ilen % 63) + 1;
            unsigned char idata[rho];
            unsigned char odata[rho];
            unsigned int olen = rho;
            UT_generateArbitraryMaterial(idata, ilen, seed++);
            UT_generateArbitraryMaterial(odata, rho, seed++);
            UT_assert(OD_Duplexing(&od, odata, olen, idata, ilen, E, 0) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata, rho);
        }
        for(ilen=0; ilen<=rho; ilen++) {
            unsigned char E = ((ilen + 12) % 63) + 1;
            unsigned char idata[rho];
            unsigned char odata[rho];
            unsigned char oadded[rho];
            unsigned int olen = rho;
            UT_generateArbitraryMaterial(idata, ilen, seed++);
            UT_generateArbitraryMaterial(odata, rho, seed++);
            UT_generateArbitraryMaterial(oadded, rho, seed++);
            UT_assert(OD_Duplexing(&od, odata, olen, idata, ilen, E, oadded) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata, rho);
        }
    }
    {
        unsigned char idata[rho];
        unsigned char odata_ref[rho];
        unsigned char odata[rho];
        unsigned int olen1, olen2;
        ODInstance od_cloned;

        UT_generateArbitraryMaterial(idata, rho, seed++);

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        UT_assert(OD_CloneCompact(&od_cloned, &od) == 0);
        UT_assert(OD_Duplexing(&od, odata_ref, rho, idata, rho, 1, NULL) == 0);
        UT_arbitraryChecksumAbsorb(&checksum, odata_ref, rho);

        UT_assert(OD_Duplexing(&od_cloned, odata, rho, idata, rho, 1, NULL) == 0);
        UT_assert(memcmp(odata, odata_ref, rho) == 0);

        for(olen1=0; olen1<rho; olen1++) {
            for(olen2=0; olen2<=(rho-olen1); olen2++) {
                unsigned int olen3 = rho - olen1 - olen2;
                memset(odata, 0, rho);
                UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
                UT_assert(OD_Duplexing(&od, odata, olen1, idata, rho, 1, NULL) == 0);
                UT_assert(is_all_zeroes(odata+olen1, rho-olen1));
                UT_assert(OD_Squeezing(&od, odata+olen1, olen2, NULL) == 0);
                UT_assert(is_all_zeroes(odata+olen1+olen2, rho-olen1-olen2));
                UT_assert(OD_Clone(&od_cloned, &od) == 0);
                UT_assert(OD_Squeezing(&od_cloned, odata+olen1+olen2, olen3, NULL) == 0);
                UT_assert(memcmp(odata, odata_ref, rho) == 0);
            }
        }
    }
    {
        unsigned char odata[rho];
        unsigned int klen;
        for(klen=0; klen<=rho; klen++) {
            UT_generateArbitraryMaterial(key, klen, seed++);
            UT_assert(OD_Initialize(&od, rho, capacity, key, klen) == 0);
            UT_assert(OD_Duplexing(&od, odata, rho, NULL, 0, 1, NULL) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata, rho);
        }
    }
    {
        size_t size = rho*32;
        size_t sizeDone;
        unsigned char idata[size];
        unsigned char oadded[size];
        unsigned char odata_ref[size];
        unsigned char odata[size];
        size_t i;
        UT_generateArbitraryMaterial(idata, size, seed++);
        UT_generateArbitraryMaterial(oadded, size, seed++);

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        for(i=0; i<size; i+=rho) {
            UT_assert(OD_Duplexing(&od, odata_ref+i, rho, idata+i, rho, 1, oadded+i) == 0);
        }

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        sizeDone = OD_DuplexingFast(&od, idata, size, 1, odata, oadded);
        UT_assert(sizeDone <= size);
        UT_assert(memcmp(odata, odata_ref, sizeDone) == 0);
        UT_arbitraryChecksumAbsorb(&checksum, odata_ref, size);
    }
    {
        size_t size = rho*32;
        size_t sizeDone;
        unsigned char oadded[size];
        unsigned char odata_ref[size];
        unsigned char odata[size];
        size_t i;
        UT_generateArbitraryMaterial(oadded, size, seed++);

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        for(i=0; i<size; i+=rho) {
            UT_assert(OD_Duplexing(&od, odata_ref+i, rho, NULL, 0, 1, oadded+i) == 0);
        }

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        sizeDone = OD_DuplexingFastOnlyOut(&od, 1, odata, size, oadded);
        UT_assert(sizeDone <= size);
        UT_assert(memcmp(odata, odata_ref, sizeDone) == 0);
        UT_arbitraryChecksumAbsorb(&checksum, odata_ref, size);
    }
    {
        size_t size = rho*32;
        size_t sizeDone;
        unsigned char idata[size];
        unsigned char odata_ref[rho];
        unsigned char odata[rho];
        size_t i;
        UT_generateArbitraryMaterial(idata, size, seed++);

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        sizeDone = OD_DuplexingFastOnlyIn(&od, idata, size, 1);
        UT_assert(sizeDone <= size);
        UT_assert(OD_Duplexing(&od, odata, rho, NULL, 0, 1, NULL) == 0);

        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        for(i=0; i<sizeDone; i+=rho) {
            UT_assert(OD_Duplexing(&od, NULL, 0, idata+i, rho, 1, NULL) == 0);
        }
        UT_assert(OD_Duplexing(&od, odata_ref, rho, NULL, 0, 1, NULL) == 0);

        UT_assert(memcmp(odata, odata_ref, rho) == 0);
    }
    return UT_arbitraryChecksumSqueeze(&checksum);
}

void selfTestOD(unsigned int rho, unsigned int capacity, const UT_arbitraryChecksum expected)
{
    UT_arbitraryChecksum checksum = performTestOD(rho, capacity);
    UT_assert(checksum == expected);
}

#define STR0(x) #x
#define STR(x) STR0(x)

#ifdef UT_OUTPUT
void writeTestOD(FILE *f, unsigned int rho, unsigned int capacity)
{
    UT_arbitraryChecksum checksum;
    unsigned int offset;

    checksum = performTestOD(rho, capacity);
    fprintf(f, "    " STR(prefix) "_selfTestOD(%d, %d, 0x%08x);\n", rho, capacity, checksum);
}
#endif

#undef checksumByteSize

#undef performTestOD
#undef selfTestOD
#undef writeTestOD
#undef ODInstance
#undef OD_Initialize
#undef OD_Clone
#undef OD_CloneCompact
#undef OD_Duplexing
#undef OD_Squeezing
#undef OD_DuplexingFast
#undef OD_DuplexingFastOnlyOut
#undef OD_DuplexingFastOnlyIn
