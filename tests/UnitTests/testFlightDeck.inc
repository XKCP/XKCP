/*
The eXtended Keccak Cfde Package (XKCP)
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

#define performTestFlightDeck           JOIN(prefix, _performTestFlightDeck)
#define performTestFlightDeck_Duplexing JOIN(prefix, _performTestFlightDeck_Duplexing)
#define selfTestFlightDeck              JOIN(prefix, _selfTestFlightDeck)
#define writeTestFlightDeck             JOIN(prefix, _writeTestFlightDeck)
#define FlightDeckInstance              JOIN(inst, _FlightDeckInstance)
#define FlightDeck_Initialize128        JOIN(prefix, 128_FlightDeck_Initialize)
#define FlightDeck_Initialize256        JOIN(prefix, 256_FlightDeck_Initialize)
#define FlightDeck_Initialize(fd, s, key, keyByteLen) \
    (((s) == 128) ? FlightDeck_Initialize128((fd), (key), (keyByteLen)) : \
     ((s) == 256) ? FlightDeck_Initialize256((fd), (key), (keyByteLen)) : 1)
#define FlightDeck_Clone                JOIN(prefix, _FlightDeck_Clone)
#define FlightDeck_CloneCompact         JOIN(prefix, _FlightDeck_CloneCompact)
#define FlightDeck_Duplexing            JOIN(prefix, _FlightDeck_Duplexing)
#define FlightDeck_Squeezing            JOIN(prefix, _FlightDeck_Squeezing)

void performTestFlightDeck_Duplexing(UT_arbitraryChecksumState *checksum, uint32_t *seed, FlightDeckInstance *fd, size_t ilen, size_t olen)
{
    unsigned char idata[ilen];
    unsigned char odata[olen];
    unsigned char oadded[olen];

    UT_generateArbitraryMaterial(idata, ilen, (*seed)++);
    memset(odata, 0, olen);
    UT_assert(FlightDeck_Duplexing(fd, odata, olen, idata, ilen, ((ilen + olen + 1) %31) + 1, 0) == 0);
    UT_arbitraryChecksumAbsorb(checksum, odata, olen);

    UT_generateArbitraryMaterial(oadded, olen, (*seed)++);
    memset(odata, 0, olen);
    UT_assert(FlightDeck_Duplexing(fd, odata, olen, idata, ilen, ((ilen + olen + 2) %31) + 1, oadded) == 0);
    UT_arbitraryChecksumAbsorb(checksum, odata, olen);
}


UT_arbitraryChecksum performTestFlightDeck(unsigned int s)
{
    FlightDeckInstance fd;
    uint32_t seed = 0xA894552E;
    unsigned int keyLength = s / 8;
    unsigned int rho = (1600 - 2*s - 64)/8;
    unsigned char key[rho];
    UT_generateArbitraryMaterial(key, rho, seed++);

    UT_arbitraryChecksumState checksum;
    UT_arbitraryChecksumInitialize(&checksum);

    {
        unsigned int ilen;
        UT_assert(keyLength <= rho);

        FlightDeck_Initialize(&fd, s, key, keyLength);

        for(ilen=0; ilen<=rho; ilen++) {
            unsigned char E = (ilen % 31) + 1;
            unsigned char idata[rho];
            unsigned char odata[rho];
            unsigned int olen = rho;
            UT_generateArbitraryMaterial(idata, ilen, seed++);
            UT_generateArbitraryMaterial(odata, rho, seed++);
            UT_assert(FlightDeck_Duplexing(&fd, odata, olen, idata, ilen, E, 0) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata, rho);
        }
        for(ilen=0; ilen<=rho; ilen++) {
            unsigned char E = ((ilen + 12) % 31) + 1;
            unsigned char idata[rho];
            unsigned char odata[rho];
            unsigned char oadded[rho];
            unsigned int olen = rho;
            UT_generateArbitraryMaterial(idata, ilen, seed++);
            UT_generateArbitraryMaterial(odata, rho, seed++);
            UT_generateArbitraryMaterial(oadded, rho, seed++);
            UT_assert(FlightDeck_Duplexing(&fd, odata, olen, idata, ilen, E, oadded) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata, rho);
        }
    }
    {
        unsigned char idata[rho];
        unsigned char odata_ref[rho];
        unsigned char odata[rho];
        unsigned int olen1, olen2;
        FlightDeckInstance fd_cloned;

        UT_generateArbitraryMaterial(idata, rho, seed++);

        UT_assert(FlightDeck_Initialize(&fd, s, key, keyLength) == 0);
        UT_assert(FlightDeck_CloneCompact(&fd_cloned, &fd) == 0);
        UT_assert(FlightDeck_Duplexing(&fd, odata_ref, rho, idata, rho, 1, NULL) == 0);
        UT_arbitraryChecksumAbsorb(&checksum, odata_ref, rho);

        UT_assert(FlightDeck_Duplexing(&fd_cloned, odata, rho, idata, rho, 1, NULL) == 0);
        UT_assert(memcmp(odata, odata_ref, rho) == 0);

        for(olen1=0; olen1<rho; olen1++) {
            for(olen2=0; olen2<=(rho-olen1); olen2++) {
                unsigned int olen3 = rho - olen1 - olen2;
                memset(odata, 0, rho);
                UT_assert(FlightDeck_Initialize(&fd, s, key, keyLength) == 0);
                UT_assert(FlightDeck_Duplexing(&fd, odata, olen1, idata, rho, 1, NULL) == 0);
                UT_assert(is_all_zeroes(odata+olen1, rho-olen1));
                UT_assert(FlightDeck_Squeezing(&fd, odata+olen1, olen2, NULL) == 0);
                UT_assert(is_all_zeroes(odata+olen1+olen2, rho-olen1-olen2));
                UT_assert(FlightDeck_Clone(&fd_cloned, &fd) == 0);
                UT_assert(FlightDeck_Squeezing(&fd_cloned, odata+olen1+olen2, olen3, NULL) == 0);
                UT_assert(memcmp(odata, odata_ref, rho) == 0);
            }
        }
    }
    {
        unsigned char odata[rho];
        unsigned int klen;
        for(klen=0; klen<=rho; klen++) {
            UT_generateArbitraryMaterial(key, klen, seed++);
            UT_assert(FlightDeck_Initialize(&fd, s, key, klen) == 0);
            UT_assert(FlightDeck_Duplexing(&fd, odata, rho, NULL, 0, 1, NULL) == 0);
            UT_arbitraryChecksumAbsorb(&checksum, odata, rho);
        }
    }
    {
        size_t ilen, olen;
        const size_t imaximum = 155000;
        const size_t omaximum = 4000;
        UT_generateArbitraryMaterial(key, rho, seed++);
        UT_assert(FlightDeck_Initialize(&fd, s, key, keyLength) == 0);

        for(ilen = 0; ilen <= imaximum; ilen = (ilen < 200) ? ilen+1 : ((ilen < 1000) ? ilen+17 : ((ilen < 10000) ? ilen+97 : ilen+1333)))
        {
            olen = 8;
            performTestFlightDeck_Duplexing(&checksum, &seed, &fd, ilen, olen);
        }
        for(olen = 0; olen <= omaximum; olen = (olen < 200) ? olen+1 : ((olen < 1000) ? olen+19 : olen+87))
        {
            ilen = 0;
            performTestFlightDeck_Duplexing(&checksum, &seed, &fd, ilen, olen);
        }
        for(int i =  1; i < 20; i++)
        for(int j = -1; j <= 1; j++) {
            ilen = i*8192 + j;
            olen = i*rho + j;
            performTestFlightDeck_Duplexing(&checksum, &seed, &fd, ilen, olen);
        }
    }
    return UT_arbitraryChecksumSqueeze(&checksum);
}

void selfTestFlightDeck(unsigned int s, const UT_arbitraryChecksum expected)
{
    UT_arbitraryChecksum checksum = performTestFlightDeck(s);
    UT_assert(checksum == expected);
}

#define STR0(x) #x
#define STR(x) STR0(x)

#ifdef UT_OUTPUT
void writeTestFlightDeck(FILE *f, unsigned int s)
{
    UT_arbitraryChecksum checksum;
    unsigned int offset;

    checksum = performTestFlightDeck(s);
    fprintf(f, "    " STR(prefix) "_selfTestFlightDeck(%d, 0x%08x);\n", s, checksum);
}
#endif

#undef checksumByteSize

#undef performTestFlightDeck
#undef performTestFlightDeck_Duplexing
#undef selfTestFlightDeck
#undef writeTestFlightDeck
#undef FlightDeckInstance
#undef FlightDeck_Initialize
#undef FlightDeck_Initialize128
#undef FlightDeck_Initialize256
#undef FlightDeck_Clone
#undef FlightDeck_CloneCompact
#undef FlightDeck_Duplexing
#undef FlightDeck_Squeezing
#undef FlightDeck_DuplexingFast
#undef FlightDeck_DuplexingFastOnlyOut
#undef FlightDeck_DuplexingFastOnlyIn
