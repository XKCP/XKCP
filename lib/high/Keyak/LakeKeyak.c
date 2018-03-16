/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#include "LakeKeyak.h"

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

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix                      Lake
    #define prefixMotorist              KeyakWidth1600
        #include "Keyakv2.inc"
    #undef prefix
    #undef prefixMotorist
#endif
