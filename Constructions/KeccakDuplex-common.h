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

#ifndef _KeccakDuplexCommon_h_
#define _KeccakDuplexCommon_h_

#include "align.h"

#define KCP_DeclareDuplexStructure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_DuplexInstanceStruct { \
        unsigned char state[size]; \
        unsigned int rate; \
        unsigned int byteInputIndex; \
        unsigned int byteOutputIndex; \
    } prefix##_DuplexInstance;

#define KCP_DeclareDuplexFunctions(prefix) \
    int prefix##_DuplexInitialize(prefix##_DuplexInstance *duplexInstance, unsigned int rate, unsigned int capacity); \
    int prefix##_Duplexing(prefix##_DuplexInstance *duplexInstance, const unsigned char *sigmaBegin, unsigned int sigmaBeginByteLen, unsigned char *Z, unsigned int ZByteLen, unsigned char delimitedSigmaEnd); \
    int prefix##_DuplexingFeedPartialInput(prefix##_DuplexInstance *duplexInstance, const unsigned char *input, unsigned int inputByteLen); \
    int prefix##_DuplexingFeedZeroes(prefix##_DuplexInstance *duplexInstance, unsigned int inputByteLen); \
    int prefix##_DuplexingOverwritePartialInput(prefix##_DuplexInstance *duplexInstance, const unsigned char *input, unsigned int inputByteLen); \
    int prefix##_DuplexingOverwriteWithZeroes(prefix##_DuplexInstance *duplexInstance, unsigned int inputByteLen); \
    int prefix##_DuplexingGetFurtherOutput(prefix##_DuplexInstance *duplexInstance, unsigned char *out, unsigned int outByteLen); \
    int prefix##_DuplexingGetFurtherOutputAndAdd(prefix##_DuplexInstance *duplexInstance, const unsigned char *input, unsigned char *output, unsigned int outputByteLen);

#endif
