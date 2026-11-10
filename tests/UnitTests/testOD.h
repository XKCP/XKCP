/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _testOD_h_
#define _testOD_h_

#include "config.h"
#if defined(XKCP_has_OD_Keccak) || defined(XKCP_has_ParallelOD_Keccak)
void testKeccakOD(void);
#endif

#endif
