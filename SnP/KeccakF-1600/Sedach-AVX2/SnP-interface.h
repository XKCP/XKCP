/*
Implementation by Vladimir Sedach, hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

////////////////////////////////////////////////////////////////////////////////
// Important: "state" parameter must be SnP_align byte aligned and SnP_stateSizeInBytes long.
// Compile with either -mavx2 or /arch:AVX and -O2 or /O2 options.

#ifndef _SnP_Interface_h_
#define _SnP_Interface_h_

#include <stddef.h>

#define SnP_width				1600
#define SnP_laneLengthInBytes	8
#define SnP_stateSizeInBytes	(7 * 4 * SnP_laneLengthInBytes)
#define SnP_laneCount			25
#define SnP_align				32
#define KeccakF_1600			//for testSnP.c

typedef unsigned char			UINT8;

#ifdef __cplusplus
extern "C" {
#endif

void SnP_StaticInitialize		(void);
void SnP_Initialize				(void *state);
void SnP_XORBytes				(void *state, const UINT8 *data, size_t offset, size_t length);
void SnP_OverwriteBytes			(void *state, const UINT8 *data, size_t offset, size_t length);
void SnP_OverwriteWithZeroes	(void *state, size_t byteCount);
void SnP_ComplementBit			(void *state, size_t position);
void SnP_Permute				(void *state);
void SnP_ExtractBytes			(const void *state, UINT8 *data, size_t offset, size_t length);
void SnP_ExtractAndXORBytes		(const void *state, UINT8 *data, size_t offset, size_t length);

size_t SnP_FBWL_Absorb			(void *state, size_t laneCount, const UINT8 *data, size_t dataByteLen, UINT8 trailingBits);
size_t SnP_FBWL_Squeeze			(void *state, size_t laneCount, UINT8 *data, size_t dataByteLen);
size_t SnP_FBWL_Wrap			(void *state, size_t laneCount, const UINT8 *dataIn, UINT8 *dataOut, size_t dataByteLen, UINT8 trailingBits);
size_t SnP_FBWL_Unwrap			(void *state, size_t laneCount, const UINT8 *dataIn, UINT8 *dataOut, size_t dataByteLen, UINT8 trailingBits);

#ifdef __cplusplus
}
#endif

#endif //_SnP_Interface_h_
