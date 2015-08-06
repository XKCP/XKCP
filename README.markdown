# The Keccak Code Package

This project gathers different implementations of the Keccak sponge function
family. Its purpose is to replace the implementations originally in the
[Keccak Reference and Optimized Code in C][keccakrefoptc] with cleaner and more
flexible code following (an improved version of) the interface proposed in the
note ["A software interface for Keccak"][keccakinterface].

We gave an updated presentation on the motivation, structure and status of the Keccak Code Package at the [SHA-3 Workshop in Santa Barbara in August 2014][SHA3workshop2014] ([slides][KCPslides]).

[keccakrefoptc]: http://keccak.noekeon.org/KeccakReferenceAndOptimized-3.2.zip
[keccakinterface]: http://keccak.noekeon.org/NoteSoftwareInterface.pdf
[SHA3workshop2014]: http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/index.html
[KCPslides]: http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/documents/vanassche_keccak_code.pdf

## Summary

This version supports:

* the [hash and extendable output functions (XOFs)][keccakhashh] defined in [the FIPS 202
  standard][fips202_standard]
* the CAESAR entries [Ketje][caesar_ketje] and [Keyak][caesar_keyak].

[fips202_standard]: http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf "FIPS 202 standard"
[caesar_ketje]: http://ketje.noekeon.org/
[caesar_keyak]: http://keyak.noekeon.org/

Since the previous version (July 2014), the structure of the Keccak Code Package
has gone through some significant changes. These include:

* The internal interface at permutation has been improved and renamed
  into "SnP" for "state and permutation" (see [SnP.h](SnP/SnP.h)
  for more details).
* A variant of the internal interface "PlSnP" that supports parallel
  invocations of the permutation (see [PlSnP.h](PlSnP/PlSnP.h)
  for more details) has been added.
* The duplex construction has gained some extended functionality, including
  [multiple duplex objects running in parallel](Constructions/KeccakParallelDuplex.c)
  and taking advantage of PlSnP.

## License

Most of the source and header files in the Keccak Code Package are released to the public domain and associated to the [CC0](http://creativecommons.org/publicdomain/zero/1.0/) deed. The exceptions are the following:

* [`Common/brg_endian.h`](Common/brg_endian.h) is copyrighted by Brian Gladman and comes with a BSD 3-clause license;
* [`Tests/genKAT.c`](Tests/genKAT.c) is based on [SHA-3 contest's code by Larry Bassham, NIST](http://csrc.nist.gov/groups/ST/hash/sha-3/documents/KAT1.zip), which he licensed under a BSD 3-clause license;
* [`Tests/timing.h`](Tests/timing.h) is based on code by Doug Whiting, which he released to the public domain.

## More information

More information can be found:

* on Keccak in general at [`keccak.noekeon.org`](http://keccak.noekeon.org/)
* on the FIPS 202 standard at [`csrc.nist.gov`](http://csrc.nist.gov/groups/ST/hash/sha-3/fips202_standard_2015.html)
* on Ketje at [`ketje.noekeon.org`](http://ketje.noekeon.org/)
* on Keyak at [`keyak.noekeon.org`](http://keyak.noekeon.org/)
* and on cryptographic sponge functions at [`sponge.noekeon.org`](http://sponge.noekeon.org/)

## Building and contributing

Code contributions are welcome.

To build, the following tools are needed:

* GCC
* make
* xsltproc

The Keccak, Keyak and Ketje Teams, July 2014: Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche, and Ronny Van Keer.

## Acknowledgments

- `genKAT.c` based on the SHA-3 contest's genKAT.c by Larry Bassham, NIST
- `brg_endian.h` by Brian Gladman
- `timing.h` based on code by Doug Whiting
- `SnP/KeccakF-1600/Sedach-AVX2/*` by Vladimir Sedach

## Notes for implementors of the standard FIPS 202 functions

If you need to implement the standard FIPS 202 functions, the macros in
[KeccakHash.h][keccakhashh] provide an easy way to get started.

[keccakhashh]: Modes/KeccakHash.h

### Differences between Keccak and the standard FIPS 202 functions

Compared to the (plain) Keccak sponge function, the [FIPS 202 standard][fips202_standard] adds suffixes to ensure that the hash functions (SHA-3) and the XOFs (SHAKE) are domain separated (i.e., so that their outputs are unrelated), as well as to make the SHAKE functions compatible with the [Sakura][sakura] tree hashing coding.

[sakura]: http://keccak.noekeon.org/Sakura.pdf "Sakura: a flexible coding for tree hashing"

A brief summary:
  - For the SHA-3 functions, append the bits "01" to the message prior to
    applying the pad10*1 rule.
  - For the SHAKE functions, append the bits "1111" to the message prior to
    applying the pad10*1 rule.
When they refer to the functions in the [FIPS 202 standard][fips202_standard], the test cases in [TestVectors](TestVectors) include these suffixes.
