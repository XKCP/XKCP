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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "SnP-implementations.h"
#include "testPerformance.h"
#include "testXooPerformance.h"
#ifdef XKCP_has_x86_64_CPU_detection
#include "x86-64-dispatch.h"
#endif
#ifdef XKCP_has_aarch64_CPU_detection
#include "aarch64-dispatch.h"
#endif
#if defined(XKCP_has_x86_64_CPU_detection) || defined(XKCP_has_aarch64_CPU_detection)
#define XKCP_has_CPU_detection
#endif

#define MEASURE_PERF

#if defined(EMBEDDED)

void assert(int condition)
{
    if (!condition)
    {
        for ( ; ; ) ;
    }
}
#endif

#if defined(EMBEDDED) && defined(__ARMCC_VERSION) && defined(MEASURE_PERF)
void CycleMeasureRestart( void );
uint32_t CycleMeasureGet( void );

struct
{
    uint32_t overhead;

    uint32_t Xoodoo_6;
    uint32_t Xoodoo_12;

    uint32_t Xoofff_MaskDerivation;

    uint32_t Xoofff_Compress1;
    uint32_t Xoofff_Compress2;
    uint32_t Xoofff_Compress4;
    uint32_t Xoofff_CompressL;

    uint32_t Xoofff_Expand1;
    uint32_t Xoofff_Expand2;
    uint32_t Xoofff_Expand4;
    uint32_t Xoofff_ExpandL;

    uint32_t XoofffWBC_Enc1;
    uint32_t XoofffWBC_Enc2;
    uint32_t XoofffWBC_Enc4;
    uint32_t XoofffWBC_EncL;

} performanceInCycles;

#endif

int Xoo( void )
{
    #if !defined(EMBEDDED)
    testXooPerformance();
    #endif

    #if defined(EMBEDDED) && defined(__ARMCC_VERSION) && defined(MEASURE_PERF)
    {
        CycleMeasureRestart();
        performanceInCycles.overhead = CycleMeasureGet();

        {
            ALIGN(Xoodoo_stateAlignment)    uint8_t    state[Xoodoo_stateSizeInBytes];
            Xoodoo_StaticInitialize();
            Xoodoo_Initialize(state);

            CycleMeasureRestart();
            Xoodoo_Permute_6rounds(state);
            performanceInCycles.Xoodoo_6 = CycleMeasureGet();
            performanceInCycles.Xoodoo_6 -= performanceInCycles.overhead;

            CycleMeasureRestart();
            Xoodoo_Permute_12rounds(state);
            performanceInCycles.Xoodoo_12 = CycleMeasureGet();
            performanceInCycles.Xoodoo_12 -= performanceInCycles.overhead;
        }

        {
            #define Xoofff_Block    (3*4*32)

            Xoofff_Instance xp;
            BitSequence k[128/8];
            BitSequence AD[16];
            BitSequence tag[32];
            BitSequence data[4*Xoofff_Block];
            BitSequence input[4*Xoofff_Block];
            BitSequence output[4*Xoofff_Block];

            memset(k, 0x55, sizeof(k));
            memset(data, 0xAA, sizeof(data));
            memset(input, 0x33, sizeof(data));
            memset(output, 0x88, sizeof(data));
            memset(AD, 0xCC, sizeof(AD));
            memset(tag, 0x22, sizeof(tag));
            CycleMeasureRestart();
            Xoofff_MaskDerivation(&xp, k, 128);
            performanceInCycles.Xoofff_MaskDerivation = CycleMeasureGet();
            performanceInCycles.Xoofff_MaskDerivation -= performanceInCycles.overhead;

            /*
            **    Compress
            */
            CycleMeasureRestart();
            Xoofff_Compress(&xp, data, 1*Xoofff_Block-8, Xoofff_FlagInit|Xoofff_FlagLastPart);
            performanceInCycles.Xoofff_Compress1 = CycleMeasureGet();
            performanceInCycles.Xoofff_Compress1 -= performanceInCycles.overhead;

            CycleMeasureRestart();
            Xoofff_Compress(&xp, data, 2*Xoofff_Block-8, Xoofff_FlagInit|Xoofff_FlagLastPart);
            performanceInCycles.Xoofff_Compress2 = CycleMeasureGet();
            performanceInCycles.Xoofff_Compress2 -= performanceInCycles.overhead;

            CycleMeasureRestart();
            Xoofff_Compress(&xp, data, 4*Xoofff_Block-8, Xoofff_FlagInit|Xoofff_FlagLastPart);
            performanceInCycles.Xoofff_Compress4 = CycleMeasureGet();
            performanceInCycles.Xoofff_Compress4 -= performanceInCycles.overhead;
            performanceInCycles.Xoofff_CompressL = (performanceInCycles.Xoofff_Compress4 - performanceInCycles.Xoofff_Compress2) / 2;

            /*
            **    Expand
            */
            Xoofff_Compress(&xp, data, 1*Xoofff_Block-8, Xoofff_FlagInit|Xoofff_FlagLastPart);
            CycleMeasureRestart();
            Xoofff_Expand(&xp, data, 1*Xoofff_Block-8, Xoofff_FlagLastPart);
            performanceInCycles.Xoofff_Expand1 = CycleMeasureGet();
            performanceInCycles.Xoofff_Expand1 -= performanceInCycles.overhead;

            Xoofff_Compress(&xp, data, 1*Xoofff_Block-8, Xoofff_FlagInit|Xoofff_FlagLastPart);
            CycleMeasureRestart();
            Xoofff_Expand(&xp, data, 2*Xoofff_Block-8, Xoofff_FlagLastPart);
            performanceInCycles.Xoofff_Expand2 = CycleMeasureGet();
            performanceInCycles.Xoofff_Expand2 -= performanceInCycles.overhead;

            Xoofff_Compress(&xp, data, 1*Xoofff_Block-8, Xoofff_FlagInit|Xoofff_FlagLastPart);
            CycleMeasureRestart();
            Xoofff_Expand(&xp, data, 4*Xoofff_Block-8, Xoofff_FlagLastPart);
            performanceInCycles.Xoofff_Expand4 = CycleMeasureGet();
            performanceInCycles.Xoofff_Expand4 -= performanceInCycles.overhead;
            performanceInCycles.Xoofff_ExpandL = (performanceInCycles.Xoofff_Expand4 - performanceInCycles.Xoofff_Expand2) / 2;

            /*
            **  WBC
            */
            CycleMeasureRestart();
            XoofffWBC_Encipher(&xp, input, output, 1*Xoofff_Block-8, AD, sizeof(AD)*8);
            performanceInCycles.XoofffWBC_Enc1 = CycleMeasureGet();
            performanceInCycles.XoofffWBC_Enc1 -= performanceInCycles.overhead;

            CycleMeasureRestart();
            XoofffWBC_Encipher(&xp, input, output, 2*Xoofff_Block-8, AD, sizeof(AD)*8);
            performanceInCycles.XoofffWBC_Enc2 = CycleMeasureGet();
            performanceInCycles.XoofffWBC_Enc2 -= performanceInCycles.overhead;

            CycleMeasureRestart();
            XoofffWBC_Encipher(&xp, input, output, 4*Xoofff_Block-8, AD, sizeof(AD)*8);
            performanceInCycles.XoofffWBC_Enc4 = CycleMeasureGet();
            performanceInCycles.XoofffWBC_Enc4 -= performanceInCycles.overhead;
            performanceInCycles.XoofffWBC_EncL = (performanceInCycles.XoofffWBC_Enc4 - performanceInCycles.XoofffWBC_Enc2) / 2;

            #undef Xoofff_Block
        }

    }
    #endif

    #if defined(EMBEDDED)

    for (;;);

    #else

    return ( 0 );

    #endif
}

void printHelp()
{
        printf("Usage: Benchmarks command(s), where the commands can be\n");
        printf("  --help or -h              To display this page\n");
        printf("  --all or -a               All benchmarks\n");
        printf("  --Keccak or -c            Benchmark all Keccak-based algorithms\n");
        printf("  --SnP or -p               Benchmark all available permutations\n");
#ifdef XKCP_has_Sponge_Keccak_width1600
        printf("  --KeccakSponge            Benchmark the Keccak sponge\n");
#endif
#ifdef XKCP_has_TurboSHAKE
        printf("  --TurboSHAKE              Benchmark TurboSHAKE\n");
#endif
#ifdef XKCP_has_SP800_185
        printf("  --SP800-185               Benchmark ParallelHash\n");
#endif
#ifdef XKCP_has_KangarooTwelve
        printf("  --KangarooTwelve or -K12  Benchmark KangarooTwelve\n");
#endif
#ifdef XKCP_has_Kravatte
        printf("  --Kravatte                Benchmark Kravatte\n");
#endif
#ifdef XKCP_has_FlightDeck_Keccak
        printf("  --FlightDeck              Benchmark FlightDeck\n");
#endif
#ifdef XKCP_has_ShakingUpAE
        printf("  --ShakingUpAE             Benchmark FlightDeck, UpperDeck, DWrap and DeckBO\n");
        printf("  --UpperDeck               Benchmark UpperDeck\n");
        printf("  --DWrap                   Benchmark DWrap\n");
        printf("  --DeckBO                  Benchmark DeckBO\n");
#endif
        printf("  --Xoodoo                  Benchmark of all Xoodoo-based functions\n");
#ifdef XKCP_has_Xoofff
        printf("  --Xoofff                  Benchmark Xoofff\n");
#endif
#ifdef XKCP_has_Xoodyak
        printf("  --Xoodyak                 Benchmark Xoodyak\n");
#endif
#ifdef XKCP_has_x86_64_CPU_detection
        printf("  --disableSSSE3            Disable use of SSSE3 implementations\n");
        printf("  --disableAVX2             Disable use of AVX2 implementations\n");
        printf("  --disableAVX512           Disable use of AVX512 implementations\n");
#endif
#ifdef XKCP_has_aarch64_CPU_detection
        printf("  --disableSHA3             Disable use of ARMv8.4-A SHA3 implementations\n");
#endif
}

int process(int argc, char* argv[])
{
    int i;
    int help = 0;
    int K = 0;
    int X = 0;
    int keccakSnP = 0;
    int xoodooSnP = 0;
    int keccakSponge = 0;
    int turboSHAKE = 0;
    int parallelHash = 0;
    int kangarooTwelve = 0;
    int kravatte = 0;
    int flightDeck = 0;
    int upperDeck = 0;
    int dWrap = 0;
    int deckBO = 0;
    int xoofff = 0;
    int xoodyak = 0;

    if (argc == 1)
        help = 1;

    for(i=1; i<argc; i++) {
        if ((strcmp("--help", argv[i]) == 0) || (strcmp("-h", argv[i]) == 0))
            help = 1;
#ifdef XKCP_has_CPU_detection
        else if (XKCP_ProcessCpuFeatureCommandLineOption(argv[i])) {}
#endif
        else if ((strcmp("--all", argv[i]) == 0) || (strcmp("-a", argv[i]) == 0))
            K = X = 1;
        else if ((strcmp("--Keccak", argv[i]) == 0) || (strcmp("-c", argv[i]) == 0))
            K = 1;
        else if ((strcmp("--SnP", argv[i]) == 0) || (strcmp("-p", argv[i]) == 0))
            keccakSnP = xoodooSnP = 1;
#ifdef XKCP_has_Sponge_Keccak_width1600
        else if (strcmp("--KeccakSponge", argv[i]) == 0)
            keccakSponge = 1;
#endif
#ifdef XKCP_has_TurboSHAKE
        else if (strcmp("--TurboSHAKE", argv[i]) == 0)
            turboSHAKE = 1;
#endif
#ifdef XKCP_has_SP800_185
        else if (strcmp("--SP800-185", argv[i]) == 0)
            parallelHash = 1;
#endif
#ifdef XKCP_has_KangarooTwelve
        else if ((strcmp("--KangarooTwelve", argv[i]) == 0) || (strcmp("-K12", argv[i]) == 0))
            kangarooTwelve = 1;
#endif
#ifdef XKCP_has_Kravatte
        else if (strcmp("--Kravatte", argv[i]) == 0)
            kravatte = 1;
#endif
#ifdef XKCP_has_FlightDeck_Keccak
        else if (strcmp("--FlightDeck", argv[i]) == 0)
            flightDeck = 1;
#endif
#ifdef XKCP_has_ShakingUpAE
        else if (strcmp("--ShakingUpAE", argv[i]) == 0)
            flightDeck = upperDeck = dWrap = deckBO = 1;
        else if (strcmp("--UpperDeck", argv[i]) == 0)
            upperDeck = 1;
        else if (strcmp("--DWrap", argv[i]) == 0)
            dWrap = 1;
        else if (strcmp("--DeckBO", argv[i]) == 0)
            deckBO = 1;
#endif
        else if (strcmp("--Xoodoo", argv[i]) == 0)
            X = 1;
#ifdef XKCP_has_Xoofff
        else if (strcmp("--Xoofff", argv[i]) == 0)
            xoofff = 1;
#endif
#ifdef XKCP_has_Xoodyak
        else if (strcmp("--Xoodyak", argv[i]) == 0)
            xoodyak = 1;
#endif
        else {
            printf("Unrecognized command '%s'\n", argv[i]);
            return -1;
        }
    }
    if (help) {
        printHelp();
        return 0;
    }
    XKCP_PrintImplementations();
    if (K) {
        testPerformance();
    }
    else {
        if (keccakSnP)
            testKeccakPermutationPerformance();
        if (keccakSponge)
            testKeccakSpongePerformance();
        if (turboSHAKE)
            testTurboSHAKEPerformance();
#ifdef XKCP_has_SP800_185
        if (parallelHash)
            testParallelHashPerformance();
#endif
#ifdef XKCP_has_KangarooTwelve
        if (kangarooTwelve)
            testKangarooTwelvePerformance();
#endif
#ifdef XKCP_has_Kravatte
        if (kravatte)
            testKravattePerformance();
#endif
#ifdef XKCP_has_FlightDeck_Keccak
        if (flightDeck)
            testFlightDeckPerformance();
#endif
#ifdef XKCP_has_ShakingUpAE
        if (upperDeck)
            testUpperDeckPerformance();
        if (dWrap)
            testDWrapPerformance();
        if (deckBO)
            testDeckBOPerformance();
#endif
    }
    if (X) {
        Xoo();
    }
    else {
        if (xoodooSnP)
            testXoodooSnPPerformance();
#ifdef XKCP_has_Xoofff
        if (xoofff)
            testXoofffPerformance();
#endif
#ifdef XKCP_has_Xoodyak
        if (xoodyak)
            testXoodyakPerformance();
#endif
    }
    return 0;
}

int main(int argc, char* argv[])
{
#ifdef XKCP_has_CPU_detection
    XKCP_EnableAllCpuFeatures();
#endif
    return process(argc, argv);
}
