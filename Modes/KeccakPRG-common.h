/*
Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeccakPRGCommon_h_
#define _KeccakPRGCommon_h_

#include "align.h"

#define KCP_DeclareSpongePRG_Structure(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_SpongePRG_InstanceStruct { \
        prefix##_DuplexInstance duplex; \
    } prefix##_SpongePRG_Instance;

#define KCP_DeclareSpongePRG_Functions(prefix) \
    int prefix##_SpongePRG_Initialize(prefix##_SpongePRG_Instance *instance, unsigned int capacity); \
    int prefix##_SpongePRG_Feed(prefix##_SpongePRG_Instance *instance, const unsigned char *input, unsigned int inputByteLen); \
    int prefix##_SpongePRG_Fetch(prefix##_SpongePRG_Instance *Instance, unsigned char *out, unsigned int outByteLen); \
    int prefix##_SpongePRG_Forget(prefix##_SpongePRG_Instance *instance);

#endif
