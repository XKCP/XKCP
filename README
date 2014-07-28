The Keccak Code Package

This project gathers different implementations of the Keccak sponge function family. Its purpose is to replace the implementations originally in the "Keccak Reference and Optimized Code in C" with cleaner and more flexible code following (an improved version of) the interface proposed in the note "A software interface for Keccak".

This version supports:
* the hash and extendable output functions (XOFs) defined in the FIPS 202 draft (http://csrc.nist.gov/groups/ST/hash/sha-3/sha-3_standard_fips202.html);
* the CAESAR entries Ketje and Keyak.

Since this commit (July 2014), the structure of the Keccak Code Package has gone through some significant changes. These include:
* The internal interface at permutation has been improved and renamed into "SnP" for "state and permutation" (see SnP.h for more details).
* A variant of the internal interface "PlSnP" that supports parallel invocations of the permutation (see PlSnP.h for more details) has been added.
* The duplex construction has gained some extended functionality, including multiple duplex objects running in parallel and taking advantage of PlSnP.

More information can be found:
* on the proposed software interface at http://keccak.noekeon.org/NoteSoftwareInterface.pdf (to be updated!)
* on Keccak in general at http://keccak.noekeon.org/
* on Ketje and Keyak at http://ketje.noekeon.org/ and http://keyak.noekeon.org/

Code contributions are welcome.

To build, the following tools are needed:
* GCC
* make
* xsltproc

The Keccak, Keyak and Ketje Teams, July 2014
Guido Bertoni, Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer

Acknowledgments:
- genKAT.c based on the SHA-3 contest's genKAT.c by NIST
- brg_endian.h by Brian Gladman
