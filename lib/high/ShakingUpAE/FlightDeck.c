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

#include "FlightDeck.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define TAG_LEN_MAX             64u
#define RHO_LEN_MAX             ((1600u - 2u * 128u - 64u) / 8u)
#define MYMIN( a, b )           (((a) < (b)) ? (a) : (b))

static unsigned int FD_Concat( unsigned int E, unsigned int bt )
{
    assert( E >= 1u );
    assert( E <= 63u );
    assert( bt <= 1u );

    unsigned int top = 1u << 7u;
    while ( E < top ) {
        top >>= 1u;
    }
    return E + (top << bt);
}

size_t FlightDeckString(unsigned char *result, size_t result_size, unsigned int n, size_t granularity)
{
    size_t blen = 0;
    unsigned int temp = n;
    int i;
    do {
        for(i=0; i<granularity; i++) {
            blen++;
            temp >>= 8;
        }
    } while (temp > 0);
    assert(blen <= result_size);
    memset(result, 0, result_size);
    for(size_t i=0; i < blen; i++) {
        result[i] = n & 0xFF;
        n >>= 8;
    }
    return blen;
}

#ifdef XKCP_has_KeccakP1600
    #define prefix              TurboSHAKE
    #define DOM_SEP             0x00u
        #include "FlightDeck.inc"
    #undef prefix
    #undef DOM_SEP

    #define prefix              SHAKE
    #define DOM_SEP             0x1Fu
        #include "FlightDeck.inc"
    #undef prefix
    #undef DOM_SEP
#endif
