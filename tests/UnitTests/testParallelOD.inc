/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#define JOIN0(a, b)                     a ## b
#define JOIN(a, b)                      JOIN0(a, b)

#define performTestParallelOD           JOIN(pprefix, _performTestParallelOD)
#define performTestParallelOD_Duplexing JOIN(pprefix, _performTestParallelOD_Duplexing)
#define selfTestParallelOD              JOIN(pprefix, _selfTestParallelOD)
#define writeTestParallelOD             JOIN(pprefix, _writeTestParallelOD)
#define ParallelODInstance              JOIN(pinst, _ParallelODInstance)
#define ParallelOD_Initialize           JOIN(pprefix, _ParallelOD_Initialize)
#define ParallelOD_Clone                JOIN(pprefix, _ParallelOD_Clone)
#define ParallelOD_CloneCompact         JOIN(pprefix, _ParallelOD_CloneCompact)
#define ParallelOD_CloneCompactFromOD   JOIN(pprefix, _ParallelOD_CloneCompactFromOD)
#define ParallelOD_Duplexing            JOIN(pprefix, _ParallelOD_Duplexing)
#define ParallelOD_Squeezing            JOIN(pprefix, _ParallelOD_Squeezing)
#define ParallelOD_DuplexingFastOnlyIn  JOIN(pprefix, _ParallelOD_DuplexingFastOnlyIn)
#define ODInstance                      JOIN(inst, _ODInstance)
#define OD_Initialize                   JOIN(prefix, _OD_Initialize)
#define OD_Clone                        JOIN(prefix, _OD_Clone)
#define OD_CloneCompact                 JOIN(prefix, _OD_CloneCompact)
#define OD_Duplexing                    JOIN(prefix, _OD_Duplexing)
#define OD_Squeezing                    JOIN(prefix, _OD_Squeezing)

void performTestParallelOD_Duplexing(UT_arbitraryChecksumState *checksum, uint32_t *seed, unsigned int rho, unsigned int capacity, unsigned int ilen, unsigned int istep, unsigned int ostep)
{
    ParallelODInstance pod;
    ODInstance od;
    unsigned int keyLength = capacity / 8 / 2;
    unsigned char key[rho];
    UT_generateArbitraryMaterial(key, rho, (*seed)++);

    {
        unsigned char E = ((ilen + istep + ostep) % 63) + 1;
        unsigned int j;
        unsigned char odata[rho*parallelism*2];
        unsigned char odata_ref[rho*parallelism*2];
        unsigned char idata[rho*parallelism*2];
        UT_assert(keyLength <= rho);

        UT_generateArbitraryMaterial(idata, rho*parallelism*2, (*seed)++);
        UT_generateArbitraryMaterial(odata_ref, rho*parallelism*2, (*seed)++);
        memcpy(odata, odata_ref, rho*parallelism*2);
        for(j=0; j<parallelism; j++) {
            UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
            UT_assert(OD_Duplexing(&od, odata_ref+j*ostep, rho, idata+j*istep, ilen, E, NULL) == 0);
        }

        UT_assert(ParallelOD_Initialize(&pod, rho, capacity, key, keyLength) == 0);
        UT_assert(ParallelOD_Duplexing(&pod, odata, rho, ostep, idata, ilen, istep, E, NULL) == 0);
        UT_arbitraryChecksumAbsorb(checksum, odata, rho*parallelism*2);
        UT_assert(memcmp(odata, odata_ref, rho*parallelism*2) == 0);
    }
    {
        unsigned char E = ((ilen + istep + ostep + 1) % 63) + 1;
        unsigned int j;
        unsigned char odata[rho*parallelism*2];
        unsigned char odata_ref[rho*parallelism*2];
        unsigned char oadded[rho*parallelism*2];
        unsigned char idata[rho*parallelism*2];
        UT_assert(keyLength <= rho);

        UT_generateArbitraryMaterial(idata, rho*parallelism*2, (*seed)++);
        UT_generateArbitraryMaterial(odata_ref, rho*parallelism*2, (*seed)++);
        memcpy(odata, odata_ref, rho*parallelism*2);
        UT_generateArbitraryMaterial(oadded, rho*parallelism*2, (*seed)++);
        for(j=0; j<parallelism; j++) {
            UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
            UT_assert(OD_Duplexing(&od, odata_ref+j*ostep, rho, idata+j*istep, ilen, E, oadded+j*ostep) == 0);
        }

        UT_assert(ParallelOD_Initialize(&pod, rho, capacity, key, keyLength) == 0);
        UT_assert(ParallelOD_Duplexing(&pod, odata, rho, ostep, idata, ilen, istep, E, oadded) == 0);
        UT_arbitraryChecksumAbsorb(checksum, odata, rho*parallelism*2);
        UT_assert(memcmp(odata, odata_ref, rho*parallelism*2) == 0);
    }
}

UT_arbitraryChecksum performTestParallelOD(unsigned int rho, unsigned int capacity)
{
    ParallelODInstance pod;
    ODInstance od;
    uint32_t seed = 0x4C925D77;
    unsigned int keyLength = capacity / 8 / 2;
    unsigned char key[rho];
    UT_generateArbitraryMaterial(key, rho, seed++);

    UT_arbitraryChecksumState checksum;
    UT_arbitraryChecksumInitialize(&checksum);

    {
        unsigned int ilen, istep, ostep;

        istep = ostep = rho;
        for(ilen=0; ilen<=rho; ilen++) {
            performTestParallelOD_Duplexing(&checksum, &seed, rho, capacity, ilen, istep, ostep);
        }

        ilen = ostep = rho;
        for(istep=0; istep<=2*rho; istep = (istep != 0) ? istep+1 : rho) {
            performTestParallelOD_Duplexing(&checksum, &seed, rho, capacity, ilen, istep, ostep);
        }

        ilen = istep = rho;
        for(ostep=rho; ostep<=2*rho; ostep++) {
            performTestParallelOD_Duplexing(&checksum, &seed, rho, capacity, ilen, istep, ostep);
        }
    }
    {
        unsigned int j;
        unsigned char odata[rho*parallelism];
        unsigned char odata_ref[rho];
        unsigned char idata[rho];
        UT_assert(keyLength <= rho);

        UT_generateArbitraryMaterial(idata, rho, seed++);
        UT_assert(OD_Initialize(&od, rho, capacity, key, keyLength) == 0);
        UT_assert(ParallelOD_CloneCompactFromOD(&pod, &od) == 0);
        UT_assert(OD_Duplexing(&od, odata_ref, rho, idata, rho, 1, NULL) == 0);
        UT_assert(ParallelOD_Duplexing(&pod, odata, rho, rho, idata, rho, 0, 1, NULL) == 0);
        UT_arbitraryChecksumAbsorb(&checksum, odata, rho*parallelism);
        for(j=0; j<parallelism; j++) {
            UT_assert(memcmp(odata+j*rho, odata_ref, rho) == 0);
        }
    }
    {
        unsigned char idata[rho*parallelism];
        unsigned char odata_ref[rho*parallelism];
        unsigned char odata[rho*parallelism];
        unsigned int ostep, olen1, olen2, j;
        ParallelODInstance pod_cloned;

        UT_generateArbitraryMaterial(idata, rho*parallelism, seed++);

        for(ostep=1; ostep<=rho; ostep++) {
            UT_assert(ParallelOD_Initialize(&pod, rho, capacity, key, keyLength) == 0);
            UT_assert(ParallelOD_CloneCompact(&pod_cloned, &pod) == 0);
            UT_assert(ParallelOD_Duplexing(&pod, odata_ref, ostep, ostep, idata, rho, rho, 1, NULL) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata_ref,ostep*parallelism);

            UT_assert(ParallelOD_Duplexing(&pod_cloned, odata, ostep, ostep, idata, rho, rho, 1, NULL) == 0);
            UT_assert(memcmp(odata, odata_ref, ostep*parallelism) == 0);

            for(olen1=(ostep < rho) ? ostep-1 : 0; olen1<ostep; olen1++)
            for(olen2=0; olen2<=(ostep-olen1); olen2++) {
                unsigned int olen3 = ostep - olen1 - olen2;
                memset(odata, 0, ostep*parallelism);
                UT_assert(ParallelOD_Initialize(&pod, rho, capacity, key, keyLength) == 0);
                UT_assert(ParallelOD_Duplexing(&pod, odata, olen1, ostep, idata, rho, rho, 1, NULL) == 0);
                for(j=0; j<parallelism; j++)
                    UT_assert(is_all_zeroes(odata+j*ostep+olen1, ostep-olen1));
                UT_assert(ParallelOD_Squeezing(&pod, odata+olen1, olen2, ostep, NULL) == 0);
                for(j=0; j<parallelism; j++)
                    UT_assert(is_all_zeroes(odata+j*ostep+olen1+olen2, ostep-olen1-olen2));
                UT_assert(ParallelOD_Clone(&pod_cloned, &pod) == 0);
                UT_assert(ParallelOD_Squeezing(&pod_cloned, odata+olen1+olen2, olen3, ostep, NULL) == 0);
                UT_assert(memcmp(odata, odata_ref, ostep*parallelism) == 0);
            }
        }
    }
    {
        size_t size = rho*32;
        size_t sizeDone;
        unsigned char idata[size*parallelism];
        unsigned char odata_ref[rho*parallelism];
        unsigned char odata[rho*parallelism];
        size_t i;
        UT_generateArbitraryMaterial(idata, size*parallelism, seed++);

        UT_assert(ParallelOD_Initialize(&pod, rho, capacity, key, keyLength) == 0);
        sizeDone = ParallelOD_DuplexingFastOnlyIn(&pod, idata, size, rho*32, 1);
        UT_assert(sizeDone <= size);
        UT_assert(ParallelOD_Duplexing(&pod, odata, rho, rho, NULL, 0, 0, 1, NULL) == 0);

        UT_assert(ParallelOD_Initialize(&pod, rho, capacity, key, keyLength) == 0);
        for(i=0; i<sizeDone; i+=rho) {
            UT_assert(ParallelOD_Duplexing(&pod, NULL, 0, 0, idata+i, rho, rho*32, 1, NULL) == 0);
        }
        UT_assert(ParallelOD_Duplexing(&pod, odata_ref, rho, rho, NULL, 0, 0, 1, NULL) == 0);

        UT_assert(memcmp(odata, odata_ref, rho*parallelism) == 0);
    }
    return UT_arbitraryChecksumSqueeze(&checksum);
}

void selfTestParallelOD(unsigned int rho, unsigned int capacity, const UT_arbitraryChecksum expected)
{
    UT_arbitraryChecksum checksum = performTestParallelOD(rho, capacity);
    UT_assert(checksum == expected);
}

#define STR0(x) #x
#define STR(x) STR0(x)

#ifdef UT_OUTPUT
void writeTestParallelOD(FILE *f, unsigned int rho, unsigned int capacity)
{
    UT_arbitraryChecksum checksum;
    unsigned int offset;

    checksum = performTestParallelOD(rho, capacity);
    fprintf(f, "    " STR(pprefix) "_selfTestParallelOD(%d, %d, 0x%08x);\n", rho, capacity, checksum);
}
#endif

#undef checksumByteSize

#undef performTestParallelOD
#undef performTestParallelOD_Duplexing
#undef selfTestParallelOD
#undef writeTestParallelOD
#undef ParallelODInstance
#undef ParallelOD_Initialize
#undef ParallelOD_Clone
#undef ParallelOD_CloneCompact
#undef ParallelOD_CloneCompactFromOD
#undef ParallelOD_Duplexing
#undef ParallelOD_Squeezing
#undef ParallelOD_DuplexingFastOnlyIn
#undef ODInstance
#undef OD_Initialize
#undef OD_Clone
#undef OD_CloneCompact
#undef OD_Duplexing
#undef OD_Squeezing
