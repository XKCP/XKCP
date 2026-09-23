#ifndef _xkcp_memcmp_
#define _xkcp_memcmp_

#include <stddef.h>
#include <stdint.h>

static inline int XKCP_memcmp(const uint8_t *a, const uint8_t *b, size_t n)
{
	volatile uint8_t d = 0;

	for (size_t i = 0; i < n; i++)
	{
		d |= a[i] ^ b[i];
	}

	// Force non-zero values to 1 in a bitwise constant-time manner
	return (int)((d | (0 - d)) >> 7);
}

#endif
