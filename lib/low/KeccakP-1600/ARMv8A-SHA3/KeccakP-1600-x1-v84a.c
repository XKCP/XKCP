/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by the XKCP contributors, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

Glue for the ARMv8.4-A (SHA3 instructions) single-state backend. The 24-round
permutation calls the verified assembly core (mlkem-native, Becker-Kannwischer);
everything else is shared with the generic 64-bit implementation.

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <stdint.h>
#include <string.h>
#include "KeccakP-1600-v84a.h"
#include "brg_endian.h"
#include "load-store.h"

#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
#error Expecting a little-endian platform
#endif

/* Keccak-f[1600] round constants, standard order (RC[0]..RC[23]). */
static const uint64_t KeccakP1600_v84a_RC[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL
};

/* Verified 24-round permutation core (assembly). */
extern void KeccakP1600_v84a_permute24(uint64_t state[25], const uint64_t rc[24]);

void KeccakP1600_v84a_Permute_24rounds(KeccakP1600_v84a_state *state)
{
    KeccakP1600_v84a_permute24(state->A, KeccakP1600_v84a_RC);
}

void KeccakP1600_v84a_Permute_Nrounds(KeccakP1600_v84a_state *state, unsigned int nrounds)
{
    if (nrounds == 24)
        KeccakP1600_v84a_permute24(state->A, KeccakP1600_v84a_RC);
    else
        KeccakP1600_plain64_Permute_Nrounds(state, nrounds);
}

size_t KeccakF1600_v84a_FastLoop_Absorb(KeccakP1600_v84a_state *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen)
{
    size_t initialByteLen = dataByteLen;
    uint64_t *A = state->A;

    while (dataByteLen >= laneCount * 8) {
        unsigned int i;
        for (i = 0; i < laneCount; i++)
            A[i] ^= XKCP_loadLane64(data, i);
        KeccakP1600_v84a_permute24(A, KeccakP1600_v84a_RC);
        data += laneCount * 8;
        dataByteLen -= laneCount * 8;
    }
    return initialByteLen - dataByteLen;
}
