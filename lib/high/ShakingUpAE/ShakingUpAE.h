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

#ifndef _ShakingUpAE_h_
#define _ShakingUpAE_h_

#include "config.h"
#include "FlightDeck.h"
#include "KeccakOD.h"

// DWrap ----------------------------------------------------------

#define XKCP_DeclareDWrapStructure(prefix) \
    typedef struct prefix##_DWrapInstanceStruct { \
        prefix##_ODInstance od; \
        unsigned int taglen; \
    } prefix##_DWrapInstance;

#define XKCP_DeclareDWrapFunctions(prefix, inst) \
    int prefix##_Wrap_Initialize(inst##_DWrapInstance *D, unsigned int rho, unsigned int c, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen ); \
    int prefix##_Wrap_Clone(inst##_DWrapInstance *Dnew, const inst##_DWrapInstance *D ); \
    int prefix##_Wrap_Wrap(inst##_DWrapInstance *D, unsigned char *C, const unsigned char *A, size_t Alen, const unsigned char *P, size_t Plen ); \
    int prefix##_Wrap_Unwrap(inst##_DWrapInstance *D, unsigned char *P, const unsigned char *A, size_t Alen, const unsigned char *C, size_t Clen );

// Deck-BO -----------------------------------------------------------------

#define XKCP_DeclareDeckBOStructure(prefix) \
    typedef struct prefix##_DeckBOInstanceStruct { \
        prefix##_FlightDeckInstance D; \
        unsigned int taglen; \
    } prefix##_DeckBOInstance;

#define XKCP_DeclareDeckBOFunctions(prefix, inst) \
    int prefix##_BO_Initialize(inst##_DeckBOInstance *dbo, unsigned int rho, unsigned int c, size_t alpha, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen ); \
    int prefix##_BO_Clone(inst##_DeckBOInstance *dbonew, const inst##_DeckBOInstance *dbo ); \
    int prefix##_BO_Wrap(inst##_DeckBOInstance *dbo, unsigned char *C, const unsigned char *A, size_t Alen, const unsigned char *P, size_t Plen ); \
    int prefix##_BO_Unwrap(inst##_DeckBOInstance *dbo, unsigned char *P, const unsigned char *A, size_t Alen, const unsigned char *C, size_t Clen );

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"
    XKCP_DeclareDWrapStructure(KeccakWidth1600)
    XKCP_DeclareDWrapFunctions(SHAKE, KeccakWidth1600)
    XKCP_DeclareDWrapFunctions(TurboSHAKE, KeccakWidth1600)

    int SHAKE128_Wrap_Initialize(KeccakWidth1600_DWrapInstance *D, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256_Wrap_Initialize(KeccakWidth1600_DWrapInstance *D, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE128_Wrap_Initialize(KeccakWidth1600_DWrapInstance *D, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256_Wrap_Initialize(KeccakWidth1600_DWrapInstance *D, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);

    XKCP_DeclareDeckBOStructure(KeccakWidth1600)
    XKCP_DeclareDeckBOFunctions(SHAKE, KeccakWidth1600)
    XKCP_DeclareDeckBOFunctions(TurboSHAKE, KeccakWidth1600)

    int SHAKE128_BO_Initialize(KeccakWidth1600_DeckBOInstance *dbo, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256_BO_Initialize(KeccakWidth1600_DeckBOInstance *dbo, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE128_BO_Initialize(KeccakWidth1600_DeckBOInstance *dbo, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256_BO_Initialize(KeccakWidth1600_DeckBOInstance *dbo, unsigned int taglen, const unsigned char *key, unsigned int keyByteLen);
#else
#error This requires an implementation of Keccak-p[1600]
#endif

#endif
