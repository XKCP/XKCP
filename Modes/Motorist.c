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

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include "Motorist.h"
#include <assert.h>

#ifdef OUTPUT
#include <stdio.h>

void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length);

#ifdef OUTPUT
static void displayByteString(FILE *f, const char* synopsis, const unsigned char *data, unsigned int length)
{
    unsigned int i;

    fprintf(f, "%s:", synopsis);
    for(i=0; i<length; i++)
        fprintf(f, " %02x", (unsigned int)data[i]);
    fprintf(f, "\n");
}
#endif
#endif


#define Pistons_Phase_Fresh         0x00
#define Pistons_Phase_Running       0x01
#define Pistons_Phase_Full          0x02
#define Pistons_Phase_Done          0x04

#define Engine_Phase_Fresh          0x00
#define Engine_Phase_Crypting       0x01
#define Engine_Phase_Crypted        0x02
#define Engine_Phase_InjectOnly     0x04
#define Engine_Phase_EndOfMessage   0x08

#define Motorist_Phase_Ready        0x01
#define Motorist_Phase_Riding       0x02
#define Motorist_Phase_Failed       0x04


#ifndef KeccakP800_excluded
    #include "KeccakP-800-SnP.h"

    #define prefix                      KeyakWidth800
    #define SnP                         KeccakP800
    #define SnP_width                   800
    #define PlSnP_parallelism           1
    #define SnP_Permute KeccakP800_Permute_12rounds
        #include "Motorist.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef PlSnP_parallelism
    #undef SnP_Permute
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix                      KeyakWidth1600
    #define SnP                         KeccakP1600
    #define SnP_width                   1600
    #define PlSnP_parallelism           1
    #define SnP_Permute KeccakP1600_Permute_12rounds
        #include "Motorist.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef PlSnP_parallelism
    #undef SnP_Permute
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times2-SnP.h"

    #define prefix                      KeyakWidth1600times2
    #define PlSnP                       KeccakP1600times2
    #define PlSnP_parallelism           2
    #define PlSnP_PermuteAll            KeccakP1600times2_PermuteAll_12rounds
    #define SnP_width                   1600
        #include "Motorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef SnP_width
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times4-SnP.h"

    #define prefix                      KeyakWidth1600times4
    #define PlSnP                       KeccakP1600times4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll            KeccakP1600times4_PermuteAll_12rounds
    #define SnP_width                   1600
        #include "Motorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef SnP_width
#endif

#ifndef KeccakP1600timesN_excluded
    #include "KeccakP-1600-times8-SnP.h"

    #define prefix                      KeyakWidth1600times8
    #define PlSnP                       KeccakP1600times8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll            KeccakP1600times8_PermuteAll_12rounds
    #define SnP_width                   1600
        #include "Motorist.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef SnP_width
#endif
