/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include <string.h>
#include "KeccakCodePackage.h"
#include "KeccakFPH.h"

static unsigned int left_enc( unsigned char * encbuf, size_t value )
{
    unsigned int n, i;
    size_t v;

    for ( v = value, n = 0; v && (n < sizeof(size_t)); ++n, v >>= 8 )
        ; /* empty */
    for ( i = 1; i <= n; ++i )
    {
        encbuf[i] = (unsigned char)(value >> (8 * (n-i)));
    }
    encbuf[0] = (unsigned char)n;
    return n + 1;
}

static unsigned int right_enc( unsigned char * encbuf, size_t value )
{
    unsigned int n, i;
    size_t v;

    for ( v = value, n = 0; v && (n < sizeof(size_t)); ++n, v >>= 8 )
        ; /* empty */
    for ( i = 1; i <= n; ++i )
    {
        encbuf[i-1] = (unsigned char)(value >> (8 * (n-i)));
    }
    encbuf[n] = (unsigned char)n;
    return n + 1;
}

#define laneSize        8
#define suffix          0x1F

#define security        128
#include "KeccakFPH.inc"
#undef  security

#define security        256
#include "KeccakFPH.inc"
#undef  security
