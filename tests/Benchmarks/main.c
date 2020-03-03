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
#include "testPerformance.h"
#include "testXooPerformance.h"

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
        printf("  --all or -a               All benchmarsk\n");
        printf("  --Keccak                  Benchmark of all Keccak-p-based functions\n");
        printf("  --Xoodoo                  Benchmark of all Xoodoo-based functions\n");
}

int process(int argc, char* argv[])
{
    int i;
    int help = 0;
    int K = 0;
    int X = 0;

    if (argc == 1)
        help = 1;

    for(i=1; i<argc; i++) {
        if ((strcmp("--help", argv[i]) == 0) || (strcmp("-h", argv[i]) == 0))
            help = 1;
        else if ((strcmp("--all", argv[i]) == 0) || (strcmp("-a", argv[i]) == 0))
            K = X = 1;
        else if ((strcmp("--Keccak", argv[i]) == 0) || (strcmp("-c", argv[i]) == 0))
            K = 1;
        else if (strcmp("--Xoodoo", argv[i]) == 0)
            X = 1;
        else {
            printf("Unrecognized command '%s'\n", argv[i]);
            return -1;
        }
    }
    if (help) {
        printHelp();
        return 0;
    }
    if (K) {
        testPerformance();
    }
    if (X) {
        Xoo();
    }
    return 0;
}

int main(int argc, char* argv[])
{
    return process(argc, argv);
}
