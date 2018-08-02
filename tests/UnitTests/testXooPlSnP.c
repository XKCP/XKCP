/*
Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#if !defined(EMBEDDED)

/* #define OUTPUT */
/* #define VERBOSE */
#endif

#if defined(EMBEDDED)
void assert(int condition);
#else
#include <assert.h>
#endif

#if (defined(OUTPUT) || defined(VERBOSE) || !defined(EMBEDDED))
#include <stdio.h>
#endif
#include <string.h>

#include "Xoodoo-SnP.h"

#include "Xoodoo-times4-SnP.h"

    #define prefix                      Xoodootimes4
    #define PlSnP                       Xoodootimes4
    #define PlSnP_parallelism           4
    #define PlSnP_PermuteAll_6rounds    Xoodootimes4_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes4_PermuteAll_12rounds
    #define PlSnP_PermuteAll            PlSnP_PermuteAll_12rounds
    #define SnP_width                   384
        #include "testXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_PermuteAll
    #undef SnP_width

#include "Xoodoo-times8-SnP.h"

    #define prefix                      Xoodootimes8
    #define PlSnP                       Xoodootimes8
    #define PlSnP_parallelism           8
    #define PlSnP_PermuteAll_6rounds    Xoodootimes8_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes8_PermuteAll_12rounds
    #define PlSnP_PermuteAll            PlSnP_PermuteAll_12rounds
    #define SnP_width                   384
        #include "testXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_PermuteAll
    #undef SnP_width

#include "Xoodoo-times16-SnP.h"

    #define prefix                      Xoodootimes16
    #define PlSnP                       Xoodootimes16
    #define PlSnP_parallelism           16
    #define PlSnP_PermuteAll_6rounds    Xoodootimes16_PermuteAll_6rounds
    #define PlSnP_PermuteAll_12rounds   Xoodootimes16_PermuteAll_12rounds
    #define PlSnP_PermuteAll            PlSnP_PermuteAll_12rounds
    #define SnP_width                   384
        #include "testXooPlSnP.inc"
    #undef prefix
    #undef PlSnP
    #undef PlSnP_parallelism
    #undef PlSnP_PermuteAll_6rounds
    #undef PlSnP_PermuteAll_12rounds
    #undef PlSnP_PermuteAll
    #undef SnP_width

void testXooPlSnP(void)
{

    Xoodootimes4_testPlSnP("Xoodoo-times4.txt", "Xoodoo\303\2274",
        "\xb9\x5e\x54\xf5\x66\x51\xb5\x4b\x12\x42\x7b\x29\xbd\x2c\x31\x46\x8a\x50\x41\xe4\xee\x72\x04\xf5\x61\xcf\xf2\xde\xf2\x9d\x8d\x89\xaa\xa4\xc3\x09\x36\x71\x8c\x29\x8a\xa7\x1a\x14\x52\x38\xc5\x06"
    );

    Xoodootimes8_testPlSnP("Xoodoo-times8.txt", "Xoodoo\303\2278",
        "\xc3\x1a\xea\x34\x51\xfe\x13\xf8\x98\xf0\xef\x07\xaa\x30\x93\x71\x69\x05\xef\xe0\x53\xa6\x2f\x46\xac\xa3\x6e\x6b\xb7\x2f\x82\x26\x6b\x0a\x1d\x8a\xf1\xb6\x7a\x8b\x9c\xd9\x38\x13\xe0\x3f\x67\x76"
    );
    Xoodootimes16_testPlSnP("Xoodoo-times16.txt", "Xoodoo\303\22716",
        "\xa6\x50\x15\xb2\xd9\x16\x80\x6c\x62\x41\x09\x4c\x93\x28\x9b\xce\x57\xbe\xe5\x10\x7b\x36\x89\xeb\xed\xb9\x01\xa1\x18\x8c\x2e\x27\xc8\xd2\xcc\xad\x24\xe1\x77\xc4\xe3\x3b\x2c\x63\x81\xb2\xe0\xc0"
    );
}
