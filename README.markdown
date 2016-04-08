# What is the KCP?

The **Keccak Code Package** (abbreviated as **KCP**) gathers different free and open-source implementations of the [Keccak sponge function family](http://keccak.noekeon.org/)
and closely related variants, such as

* the SHAKE exendable-output functions and SHA-3 hash functions from [FIPS 202][fips202_standard],
* the [Ketje][caesar_ketje] and [Keyak v2][caesar_keyak] authenticated encryption schemes.



# What does the KCP contain?

First, the services available in this package are divided into high-level and low-level services. In a nutshell, the low level corresponds to Keccak-_f_[1600] and basic state manipulation, while the high level contains the constructions and the modes for, e.g., sponge functions, hashing or authenticated encryption. For more details, please see the section "_How is the code organized?_" below.

Then, the KCP also contains some utilities for testing and illustration purposes.

Finally, the project contains some standalone implementations.


## High-level services

When used as a library or directly from the sources, the KCP offers the high-level services documented in the following header files:

* [`SimpleFIPS202.h`](Modes/SimpleFIPS202.h), the six approved FIPS 202 instances (SHAKE128, SHAKE256 and the SHA-3 hash functions) through simple functions.
* [`KeccakHash.h`](Modes/KeccakHash.h), the six approved FIPS 202 instances, as well as any Keccak instance based on Keccak-_f_[1600]. This more advanced interface proposes a message queue (init-update-final) and supports bit-level inputs if needed.
* [`KeccakSponge.h`](Constructions/KeccakSponge.h), all Keccak sponge functions, with or without a message queue.
* [`KeccakDuplex.h`](Constructions/KeccakDuplex.h), all Keccak duplex objects.
* [`KeccakPRG.h`](Modes/KeccakPRG.h), a pseudo-random number generator based on Keccak duplex objects.
* [`Keyakv2.h`](Modes/Keyakv2.h), the authenticated encryption schemes River, Lake, Sea, Ocean and Lunar Keyak.
* [`Ketje.h`](Ketje/Ketje.h), the lightweight authenticated encryption schemes Ketje Jr and Ketje Sr.


## Low-level services

The low-level services implement the different permutations Keccak-_f_[200 to 1600] and Keccak-_p_[200 to 1600]. Note that these two permutation families are closely related. In Keccak-_p_ the number of rounds is a parameter while in Keccak-_f_ it is fixed. As Keccak-_f_ are just instances of Keccak-_p_, we focus on the latter here.

The low-level services provide an opaque representation of the state together with functions to add data into and extract data from the state. Together with the permutations themselves, the low-level services implement what we call the **state and permutation** interface (abbreviated **SnP**). For parallelized implementation, we similarly use the **parallel** state and permutation interface or **PlSnP**.

* In [`SnP/`](SnP/), one can find implementations of the following permutations for different platforms.
    + [`SnP/KeccakP-200/`](SnP/KeccakP-200/) for Keccak-_p_[200].
    + [`SnP/KeccakP-400/`](SnP/KeccakP-400/), for Keccak-_p_[400].
    + [`SnP/KeccakP-800/`](SnP/KeccakP-800/), for Keccak-_p_[800].
    + [`SnP/KeccakP-1600/`](SnP/KeccakP-1600/), for Keccak-_p_[1600]. This is the one used in the six approved FIPS 202 instances.

* In [`PlSnP/`](PlSnP/), one can find the implementation of parallelized permutations. There are both implementations based on SIMD instructions and "fallback" implementations relying on a parallelized with a lower degree implementation or on a serial one.

In both cases, the hierarchy first selects a permutation (or a permutation and a degree of parallelism) and then a given implementation. E.g., one finds in [`PlSnP/KeccakP-1600-times4/`](PlSnP/KeccakP-1600-times4/) the implementations of 4 parallel instances of Keccak-_p_[1600] and in [`PlSnP/KeccakP-1600-times4/SIMD256/`](PlSnP/KeccakP-1600-times4/SIMD256/) a 256-bit SIMD implementation.

The documentation of the low-level services can be found in [`SnP-documentation.h`](SnP/SnP-documentation.h) and [`PlSnP-documentation.h`](PlSnP/PlSnP-documentation.h).


## Utilities

The package contains:

* The possibility to create a library **`libkeccak.a`**;
* [**Self-tests**](Tests/main.c) that ensure that the implementation is working properly;
* [**KeccakSum**](KeccakSum/KeccakSum.c) that computes a hash of the file (or multiple files) given in parameter.


## Standalone implementations

The KCP also provides a number of standalone implementations, including:

* a very [compact](http://keccak.noekeon.org/tweetfips202.html) C code of the FIPS 202 (SHA-3) standard in [`Standalone/CompactFIPS202/`](Standalone/CompactFIPS202/);
* a compact implementation in Python in [`Standalone/CompactFIPS202-Python/`](Standalone/CompactFIPS202-Python/).



# Under which license is the KCP distributed?

Most of the source and header files in the KCP are released to the **public domain** and associated to the [CC0](http://creativecommons.org/publicdomain/zero/1.0/) deed. The exceptions are the following:

* [`Common/brg_endian.h`](Common/brg_endian.h) is copyrighted by Brian Gladman and comes with a BSD 3-clause license;
* [`Tests/genKAT.c`](Tests/genKAT.c) is based on [SHA-3 contest's code by Larry Bassham, NIST](http://csrc.nist.gov/groups/ST/hash/sha-3/documents/KAT1.zip), which he licensed under a BSD 3-clause license;
* [`Tests/timing.h`](Tests/timing.h) is based on code by Doug Whiting, which he released to the public domain.



# How can I build the KCP?

To build, the following tools are needed:

* *GCC*
* *make*
* *xsltproc*

The different targets are defined in [`Makefile.build`](Makefile.build). This file is expanded into a regular makefile using *xsltproc*. To use it, simply type, e.g.,

> `make generic64/KeccakTests`

to build KeccakTests generically optimized for 64-bit platforms. The name before the slash indicates the platform, while the part after the slash is the executable to build. As another example, the static library is built by typing `make generic64/libkeccak.a` or similarly with `generic64` replaced with the appropriate platform name.

Instead of building an executable with *GCC*, one can choose to select the files needed and make a package. For this, simply append `.pack` to the target name, e.g.,

> `make generic64/KeccakTests.pack`

This creates a `.tar.gz` archive with all the necessary files to build the given target.

The list of targets can be found at the end of [`Makefile.build`](Makefile.build) or by running `make` without parameters.



# How is the code organized?

The code is organized as illustrated in the following figure:

![Layers](figures/Layers.png)

At the top, the high-level cryptographic services are implemented in plain C, without any specific optimizations. At the bottom, the low-level services implement the permutations and the state input/output functions, which can be optimized for a given platform. The interface between the two layers is called **SnP**.

The idea is to have a single, portable, code base for the high level and the possibility to dedicate the low level to certain platforms for best performance.

The modes and constructions can be found in [`Modes/`](Modes/) and in [`Constructions/`](Constructions/), while the permutations are stored in [`SnP/`](SnP/).

The situation is similar for parallelized services, as illustrated on the following figure. The interface is adapated to the parallelism and is called **PlSnP**, with the implementations in [`PlSnP/`](PlSnP/).

![Parallel layers](figures/ParallelLayers.png)

*Disclaimer*: the above figures aim at illustrative purposes only, as not all modes, constructions or permutations are currently implemented in the KCP or represented on the figures.



# Where can I find more information?

About the KCP, we gave a presentation on its motivation and structure

* at the [SHA-3 Workshop in Santa Barbara in August 2014][SHA3workshop2014] ([slides][KCPslides]) and
* at [FOSDEM 2015][FOSDEM2015] ([slides][KCPslidesAtFOSDEM]).

The KCP follows an improved version of the structure proposed in the note ["A software interface for Keccak"][keccakinterface].

More information on the cryptographic aspects can be found:

* on Keccak in general at [`keccak.noekeon.org`](http://keccak.noekeon.org/)
* on the FIPS 202 standard at [`csrc.nist.gov`](http://csrc.nist.gov/groups/ST/hash/sha-3/fips202_standard_2015.html)
* on Ketje at [`ketje.noekeon.org`](http://ketje.noekeon.org/)
* on Keyak at [`keyak.noekeon.org`](http://keyak.noekeon.org/)
* and on cryptographic sponge functions at [`sponge.noekeon.org`](http://sponge.noekeon.org/)



# How can I contribute?

We welcome contributions in various forms, e.g., general feedback, bug reports, improvements and optimized implementations on your favorite platforms. The best is to do this through GitHub. Alternatively, you can send us a mail at `keyak` _-at-_ `noekeon` _-dot-_ `org`.



# How can I use the standard FIPS 202 functions?

If you need to implement the standard FIPS 202 functions, the functions in [`SimpleFIPS202.h`](Modes/SimpleFIPS202.h) provide an easy way to get started. If a message queue and/or bit-level inputs are needed, then the macros in [`KeccakHash.h`](Modes/KeccakHash.h) can be helpful.

## Differences between Keccak and the standard FIPS 202 functions

Compared to the (plain) Keccak sponge function, the [FIPS 202 standard][fips202_standard] adds suffixes to ensure that the hash functions (SHA-3) and the XOFs (SHAKE) are domain separated (i.e., so that their outputs are unrelated even with equal inputs), as well as to make the SHAKE functions compatible with the [Sakura][sakura] tree hashing coding.

[sakura]: http://keccak.noekeon.org/Sakura.pdf "Sakura: a flexible coding for tree hashing"

A brief summary:

* For the SHA-3 functions, append the bits "01" to the message prior to  applying the pad10*1 rule.
* For the SHAKE functions, append the bits "1111" to the message prior to applying the pad10*1 rule.

When they refer to the functions in the [FIPS 202 standard][fips202_standard], the test cases in [TestVectors](TestVectors) include these suffixes.



# Acknowledgments

- `genKAT.c` based on the SHA-3 contest's genKAT.c by Larry Bassham, NIST
- `brg_endian.h` by Brian Gladman
- `timing.h` based on code by Doug Whiting
- `SnP/KeccakP-1600/OptimizedAVX2/*` by Vladimir Sedach
- `TweetableFIPS202.c` by D. J. Bernstein, Peter Schwabe and Gilles Van Assche

***

The Keccak, Keyak and Ketje Teams: Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche, and Ronny Van Keer.

[keccakrefoptc]: http://keccak.noekeon.org/KeccakReferenceAndOptimized-3.2.zip
[keccakinterface]: http://keccak.noekeon.org/NoteSoftwareInterface.pdf
[SHA3workshop2014]: http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/index.html
[KCPslides]: http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/documents/vanassche_keccak_code.pdf
[KCPslidesAtFOSDEM]: https://archive.fosdem.org/2015/schedule/event/keccak_and_sha3/attachments/slides/794/export/events/attachments/keccak_and_sha3/slides/794/KeccakFosdem2015.pdf
[FOSDEM2015]: https://archive.fosdem.org/2015/
[fips202_standard]: http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf "FIPS 202 standard"
[caesar_ketje]: http://ketje.noekeon.org/
[caesar_keyak]: http://keyak.noekeon.org/
