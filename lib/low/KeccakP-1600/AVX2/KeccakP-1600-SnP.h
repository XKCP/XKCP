/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to SnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_SnP_h_
#define _KeccakP_1600_SnP_h_

#include <stddef.h>
#include <stdint.h>

#ifdef __MINGW32__
#define FORCE_SYSV __attribute__((sysv_abi))
#else
#define FORCE_SYSV
#endif

typedef struct {
    uint64_t Aba, Abe, Abi, Abo, Abu;
    uint64_t Aka, Asa, Aga, Ama;
    uint64_t Ame, Agi, Aso, Aku;
    uint64_t Ake, Asi, Ago, Amu;
    uint64_t Ase, Ami, Ako, Agu;
    uint64_t Age, Aki, Amo, Asu;
} KeccakP1600_AVX2_state;

typedef KeccakP1600_AVX2_state KeccakP1600_state;

#define KeccakP1600_implementation      "AVX2 optimized implementation"
#define KeccakF1600_FastLoop_supported
#define KeccakP1600_12rounds_FastLoop_supported

#define KeccakP1600_StaticInitialize()
FORCE_SYSV void KeccakP1600_Initialize(KeccakP1600_AVX2_state *state);
FORCE_SYSV void KeccakP1600_AddByte(KeccakP1600_AVX2_state *state, unsigned char data, unsigned int offset);
FORCE_SYSV void KeccakP1600_AddBytes(KeccakP1600_AVX2_state *state, const unsigned char *data, unsigned int offset, unsigned int length);
FORCE_SYSV void KeccakP1600_OverwriteBytes(KeccakP1600_AVX2_state *state, const unsigned char *data, unsigned int offset, unsigned int length);
FORCE_SYSV void KeccakP1600_OverwriteWithZeroes(KeccakP1600_AVX2_state *state, unsigned int byteCount);
FORCE_SYSV void KeccakP1600_Permute_Nrounds(KeccakP1600_AVX2_state *state, unsigned int nrounds);
FORCE_SYSV void KeccakP1600_Permute_12rounds(KeccakP1600_AVX2_state *state);
FORCE_SYSV void KeccakP1600_Permute_24rounds(KeccakP1600_AVX2_state *state);
FORCE_SYSV void KeccakP1600_ExtractBytes(const KeccakP1600_AVX2_state *state, unsigned char *data, unsigned int offset, unsigned int length);
FORCE_SYSV void KeccakP1600_ExtractAndAddBytes(const KeccakP1600_AVX2_state *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
FORCE_SYSV size_t KeccakF1600_FastLoop_Absorb(KeccakP1600_AVX2_state *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
FORCE_SYSV size_t KeccakP1600_12rounds_FastLoop_Absorb(KeccakP1600_AVX2_state *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);

#endif
