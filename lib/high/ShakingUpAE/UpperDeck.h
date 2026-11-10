/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

“Shaking up authenticated encryption”: Keccak-based duplex ciphers, deck ciphers and authenticated encryption schemes designed by Joan Daemen, Seth Hoffert, Silvia Mella, Gilles Van Assche and Ronny Van Keer

Implementation by Ronny Van Keer and Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _UpperDeck_h_
#define _UpperDeck_h_

#include "config.h"
#include "KeccakOD.h"

#define XKCP_DeclareUpperDeckStructure(prefix) \
    typedef struct prefix##_UpperDeckInstanceStruct { \
        prefix##_ODInstance D; \
        prefix##_ODInstance Dsqueeze; \
        size_t o; \
    } prefix##_UpperDeckInstance;

#define XKCP_DeclareUpperDeckFunctions(prefix, inst) \
    int prefix##_UpperDeck_Initialize(inst##_UpperDeckInstance *ud, unsigned int rho, unsigned int c, const unsigned char *key, unsigned int keyByteLen ); \
    int prefix##_UpperDeck_Clone(inst##_UpperDeckInstance *udnew, const inst##_UpperDeckInstance *ud ); \
    int prefix##_UpperDeck_CloneCompact(inst##_UpperDeckInstance *udnew, const inst##_UpperDeckInstance *ud ); \
    int prefix##_UpperDeck_Duplexing(inst##_UpperDeckInstance *ud, unsigned char *output, size_t outputLen, const unsigned char *input, size_t inputLen, unsigned int E, const unsigned char *toAdd ); \
    int prefix##_UpperDeck_Squeezing(inst##_UpperDeckInstance *ud, unsigned char *output, size_t outputLen, const unsigned char *toAdd );

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"
    XKCP_DeclareUpperDeckStructure(KeccakWidth1600)
    XKCP_DeclareUpperDeckFunctions(SHAKE, KeccakWidth1600)
    XKCP_DeclareUpperDeckFunctions(TurboSHAKE, KeccakWidth1600)

    int SHAKE128_UpperDeck_Initialize(KeccakWidth1600_UpperDeckInstance *ud, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256_UpperDeck_Initialize(KeccakWidth1600_UpperDeckInstance *ud, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE128_UpperDeck_Initialize(KeccakWidth1600_UpperDeckInstance *ud, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256_UpperDeck_Initialize(KeccakWidth1600_UpperDeckInstance *ud, const unsigned char *key, unsigned int keyByteLen);
#else
#error This requires an implementation of Keccak-p[1600]
#endif

#endif
