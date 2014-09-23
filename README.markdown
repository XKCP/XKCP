# The Keccak Code Package

This project gathers different implementations of the Keccak sponge function
family. Its purpose is to replace the implementations originally in the
[Keccak Reference and Optimized Code in C][keccakrefoptc] with cleaner and more
flexible code following (an improved version of) the interface proposed in the
note ["A software interface for Keccak"][keccakinterface].

[keccakrefoptc]: http://keccak.noekeon.org/Keccak-reference-3.0-files.zip
[keccakinterface]: http://keccak.noekeon.org/NoteSoftwareInterface.pdf

## Notes for implementors of the draft FIPS-202 functions

If you need to implement the draft FIPS-202 functions, the macros in
[KeccakHash.h][keccakhashh] provide an easy way to get started.

### Changes between Keccak-submission-3 and draft FIPS-202

A note for implementors: Between the specification presented in the Keccak
team's final submission document for SHA-3, and the FIPS-202 draft, additional
domain separation bits were added to ensure that the outputs of the SHA-3 and
SHAKE functions are distinct, as well as to make the SHAKE functions compatible
with the [Sakura][sakura] tree-hashing scheme.

[sakura]: http://keccak.noekeon.org/Sakura.pdf "Sakura: a flexible coding for tree hashing"

A brief summary:
  - For the SHA-3 functions, append the bits "01" to the message prior to
    applying the pad10*1 rule.
  - For the SHAKE functions, append the bits "1111" to the message prior to
    applying the pad10*1 rule.
Please make sure that your implementation correctly appends these domain
separator bits. The test-cases in [TestVectors](tree/master/TestVectors) do
include these domain-separator bits. (The versions in the Keccak submission
zip-file do not!)

## Summary

This version supports:

* the [hash and extendable output functions (XOFs)][keccakhashh] defined in [the FIPS 202
  draft][fips202_draft]
* the CAESAR entries [Ketje][caesar_ketje] and [Keyak][caesar_keyak].

[keccakhashh]: blob/master/Modes/KeccakHash.h
[fips202_draft]: http://csrc.nist.gov/groups/ST/hash/sha-3/sha-3_standard_fips202.html "FIPS-202 draft"
[caesar_ketje]: http://competitions.cr.yp.to/round1/ketjev1.pdf
[caesar_keyak]: http://competitions.cr.yp.to/round1/keyakv1.pdf

Since the previous version (July 2014), the structure of the Keccak Code Package
has gone through some significant changes. These include:

* The internal interface at permutation has been improved and renamed
  into "SnP" for "state and permutation" (see [SnP.h](blob/master/SnP/SnP.h)
  for more details).
* A variant of the internal interface "PlSnP" that supports parallel
  invocations of the permutation (see [PlSnP.h](blob/master/PlSnP/PlSnP.h)
  for more details) has been added.
* The duplex construction has gained some extended functionality, including
  [multiple duplex objects running in parallel](blob/master/Constructions/KeccakParallelDuplex.c)
  and taking advantage of PlSnP.

## More information

More information can be found:

* on Keccak in general at http://keccak.noekeon.org/
* on Ketje at http://ketje.noekeon.org/
* on Keyak at http://keyak.noekeon.org/
* and on cryptographic sponge functions at http://sponge.noekeon.org/

## Building and contributing

Code contributions are welcome.

To build, the following tools are needed:

* GCC
* make
* xsltproc

The Keccak, Keyak and Ketje Teams, July 2014: Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche, and Ronny Van Keer.

## Acknowledgments

- `genKAT.c` based on the SHA-3 contest's genKAT.c by NIST
- `brg_endian.h` by Brian Gladman
