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

#ifndef _Exception_h_
#define _Exception_h_

#include <string>

/**
 * Exception with a string expressing the reason.
 */
class Exception {
public:
    /** A string expressing the reason for the exception. */
    std::string reason;

public:
    /**
     * The default constructor.
     */
    Exception()
        : reason() {}

    /**
     * The constructor.
     * @param   aReason     A string giving the reason of the exception.
     */
    Exception(const std::string& aReason)
        : reason(aReason) {}
};

#endif
