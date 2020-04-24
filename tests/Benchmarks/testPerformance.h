/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _testPerformance_h_
#define _testPerformance_h_

#include <stdint.h>
#include "align.h"

#define ALIGN_DEFAULT ALIGN(64)
#define BIG_BUFFER_SIZE (4*1024*1024)

extern ALIGN_DEFAULT uint8_t bigBuffer1[BIG_BUFFER_SIZE];
extern ALIGN_DEFAULT uint8_t bigBuffer2[BIG_BUFFER_SIZE];

void testPerformance(void);

#endif
