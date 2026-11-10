/*
 * The eXtended Keccak Code Package (XKCP)
 * https://github.com/XKCP/XKCP
 *
 * “Shaking up authenticated encryption”: Keccak-based duplex ciphers, deck ciphers and authenticated encryption schemes designed by Joan Daemen, Seth Hoffert, Silvia Mella, Gilles Van Assche and Ronny Van Keer
 *
 * Implementation by Alex Bataille and Gilles Van Assche, hereby denoted as "the implementer".
 *
 * For more information, feedback or questions, please refer to the Keccak Team website:
 * https://keccak.team/
 *
 * To the extent possible under law, the implementer has waived all copyright
 * and related or neighboring rights to the source code in this file.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#ifndef _FlightDeck_h_
#define _FlightDeck_h_

#include "KeccakOD.h"
#include "config.h"
#include <stdio.h>

// FlightDeck --------------------------------------------------------

#define XKCP_DeclareFlightDeckStructure(prefix) \
    typedef struct prefix##_FlightDeckInstanceStruct { \
        prefix##_ODInstance D; \
        prefix##_ODInstance Dsqueeze; \
        size_t alpha; \
        size_t o; \
    } prefix##_FlightDeckInstance;

#define XKCP_DeclareFlightDeckFunctions(prefix, inst) \
    int prefix##_FlightDeck_Initialize( \
            inst##_FlightDeckInstance *fd, \
            unsigned int rho, \
            unsigned int c, \
            size_t alpha, \
            const unsigned char *key, \
            unsigned int keyByteLen \
        ); \
    int prefix##_FlightDeck_Clone( \
            inst##_FlightDeckInstance *fdnew, \
            const inst##_FlightDeckInstance *fd \
        ); \
    int prefix##_FlightDeck_CloneCompact( \
            inst##_FlightDeckInstance *fdnew, \
            const inst##_FlightDeckInstance *fd \
        ); \
    int prefix##_FlightDeck_Duplexing( \
            inst##_FlightDeckInstance *fd, \
            unsigned char *output, \
            size_t outputLen, \
            const unsigned char *input, \
            size_t inputLen, \
            unsigned int E, \
            const unsigned char *toAdd \
        ); \
    int prefix##_FlightDeck_Squeezing( \
            inst##_FlightDeckInstance *fd, \
            unsigned char *output, \
            size_t outputLen, \
            const unsigned char *toAdd \
        );

#ifdef XKCP_has_KeccakP1600
    #include "KeccakP-1600-SnP.h"
    #define XKCP_has_FlightDeck_Keccak_width1600
    XKCP_DeclareFlightDeckStructure(KeccakWidth1600)
    XKCP_DeclareFlightDeckFunctions(SHAKE, KeccakWidth1600)
    XKCP_DeclareFlightDeckFunctions(TurboSHAKE, KeccakWidth1600)

    int TurboSHAKE128_FlightDeck_Initialize(KeccakWidth1600_FlightDeckInstance *fd, const unsigned char *key, unsigned int keyByteLen);
    int TurboSHAKE256_FlightDeck_Initialize(KeccakWidth1600_FlightDeckInstance *fd, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE128_FlightDeck_Initialize(KeccakWidth1600_FlightDeckInstance *fd, const unsigned char *key, unsigned int keyByteLen);
    int SHAKE256_FlightDeck_Initialize(KeccakWidth1600_FlightDeckInstance *fd, const unsigned char *key, unsigned int keyByteLen);
#else
    #error This requires an implementation of Keccak-p[1600]
#endif

#endif  // _FLIGHT_DECK_H_
