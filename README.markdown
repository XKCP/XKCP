# What is the XKCP?

The **eXtended Keccak Code Package** (or the **Xoodoo and Keccak Code Package**, in both cases abbreviated as **XKCP**) gathers different free and open-source implementations of the [Keccak sponge function family](https://keccak.team/keccak.html)
and closely related variants, such as

* the SHAKE extendable-output functions and SHA-3 hash functions from [FIPS 202][fips202_standard],
* the cSHAKE, KMAC, ParallelHash and TupleHash functions from [NIST SP 800-185][sp800_185_standard],
* the [Ketje v2][caesar_ketje] and [Keyak v2][caesar_keyak] authenticated encryption schemes,
* the fast [KangarooTwelve][k12] extendable-output function,
* the [Kravatte](https://keccak.team/kravatte.html) pseudo-random function and its modes,

as well as the [Xoodoo][XoodooCookbook] permutation and

* the [Xoofff][XoodooCookbook] pseudo-random function and its modes (experimental).


# What does the XKCP contain?

First, the services available in this package are divided into high-level and low-level services. In a nutshell, the low level corresponds to Keccak-_f_[1600] and basic state manipulation, while the high level contains the constructions and the modes for, e.g., sponge functions, hashing or authenticated encryption. For more details, please see the section "_How is the code organized?_" below.

Then, the XKCP also contains some utilities for testing and illustration purposes.

Finally, the project contains some standalone implementations.


## High-level services

When used as a library or directly from the sources, the XKCP offers the high-level services documented in the following header files:

* [`SimpleFIPS202`](lib/high/Keccak/FIPS202/SimpleFIPS202.h), the six approved FIPS 202 instances (SHAKE128, SHAKE256 and the SHA-3 hash functions) through simple functions.
* [`KeccakHash`](lib/high/Keccak/FIPS202/KeccakHash.h), the six approved FIPS 202 instances, as well as any Keccak instance based on Keccak-_f_[1600]. This more advanced interface proposes a message queue (init-update-final) and supports bit-level inputs if needed.
* [`SP800-185`](lib/high/Keccak/SP800-185/SP800-185.h), the functions (cSHAKE, KMAC, ParallelHash and TupleHash) in the official NIST SP 800-185 standard.
* [`KeccakSponge`](doc/KeccakSponge-documentation.h), all Keccak sponge functions, with or without a message queue.
* [`KeccakDuplex`](doc/KeccakDuplex-documentation.h), all Keccak duplex objects.
* [`KeccakPRG`](doc/KeccakPRG-documentation.h), a pseudo-random number generator based on Keccak duplex objects.
* [`Keyakv2`](doc/Keyak-documentation.h), the authenticated encryption schemes River, Lake, Sea, Ocean and Lunar Keyak.
* [`Ketjev2`](doc/Ketje-documentation.h), the lightweight authenticated encryption schemes Ketje Jr, Ketje Sr, Ketje Minor and Ketje Major.
* [`KangarooTwelve`](lib/high/KangarooTwelve/KangarooTwelve.h), the fast hashing mode based on Keccak-_p_[1600, 12] and Sakura coding.
* [`Kravatte`](lib/high/Kravatte/Kravatte.h) and [`KravatteModes`](lib/high/Kravatte/KravatteModes.h), the pseudo-random function Kravatte, as well as the modes on top of it (SANE, SANSE, WBC and WBC-AE).
* [`Xoofff`](lib/high/Xoofff/Xoofff.h) and [`XoofffModes`](lib/high/Xoofff/XoofffModes.h), the pseudo-random function Xoofff, as well as the modes on top of it (SANE, SANSE, WBC and WBC-AE).

## Low-level services

The low-level services implement the different permutations Keccak-_f_[200 to 1600] and Keccak-_p_[200 to 1600]. Note that these two permutation families are closely related. In Keccak-_p_ the number of rounds is a parameter while in Keccak-_f_ it is fixed. As Keccak-_f_ are just instances of Keccak-_p_, we focus on the latter here.

The low-level services provide an opaque representation of the state together with functions to add data into and extract data from the state. Together with the permutations themselves, the low-level services implement what we call the **state and permutation** interface (abbreviated **SnP**). For parallelized implementation, we similarly use the **parallel** state and permutation interface or **PlSnP**.

* In [`lib/low/`](lib/low/), one can find implementations of the following permutations for different platforms.
    + [`lib/low/KeccakP-200/`](lib/low/KeccakP-200/) for Keccak-_p_[200].
    + [`lib/low/KeccakP-400/`](lib/low/KeccakP-400/), for Keccak-_p_[400].
    + [`lib/low/KeccakP-800/`](lib/low/KeccakP-800/), for Keccak-_p_[800].
    + [`lib/low/KeccakP-1600/`](lib/low/KeccakP-1600/), for Keccak-_p_[1600]. This is the one used in the six approved FIPS 202 instances.
    + [`lib/low/Xoodoo/`](lib/low/Xoodoo/), for Xoodoo.

* In addition, one can find the implementation of parallelized permutations. There are both implementations based on SIMD instructions and "fallback" implementations relying on a parallelized with a lower degree implementation or on a serial one.

In both cases, the hierarchy first selects a permutation (or a permutation and a degree of parallelism) and then a given implementation. E.g., one finds in [`lib/low/KeccakP-1600-times4/`](lib/low/KeccakP-1600-times4/) the implementations of 4 parallel instances of Keccak-_p_[1600] and in [`lib/low/KeccakP-1600-times4/SIMD256/`](lib/low/KeccakP-1600-times4/SIMD256/) a 256-bit SIMD implementation.

The documentation of the low-level services can be found in [`SnP-documentation.h`](doc/SnP-documentation.h) and [`PlSnP-documentation.h`](doc/PlSnP-documentation.h).


## Utilities

The package contains:

* The possibility to create a static library **`libkeccak.a`** or a dynamic library **`libkeccak.so`**;
* [**Self-tests**](tests/UnitTests/main.c) that ensure that the implementation is working properly;
* [**KeccakSum**](util/KeccakSum/KeccakSum.c) that computes a hash of the file (or multiple files) given in parameter.


## Standalone implementations

The XKCP also provides a number of standalone implementations, including:

* a very [compact](https://keccak.team/2015/tweetfips202.html) C code of the FIPS 202 (SHA-3) standard in [`Standalone/CompactFIPS202/C/`](Standalone/CompactFIPS202/C/);
* a compact implementation in Python in [`Standalone/CompactFIPS202/Python/`](Standalone/CompactFIPS202/Python/);
* the reference code of KangarooTwelve in Python in [`Standalone/KangarooTwelve/Python/`](Standalone/KangarooTwelve/Python/).



# Under which license is the XKCP distributed?

Most of the source and header files in the XKCP are released to the **public domain** and associated to the [CC0](http://creativecommons.org/publicdomain/zero/1.0/) deed. The exceptions are the following:

* [`lib/common/brg_endian.h`](lib/common/brg_endian.h) is copyrighted by Brian Gladman and comes with a BSD 3-clause license;
* [`tests/UnitTests/genKAT.c`](tests/UnitTests/genKAT.c) is based on [SHA-3 contest's code by Larry Bassham, NIST](http://csrc.nist.gov/groups/ST/hash/sha-3/documents/KAT1.zip), which he licensed under a BSD 3-clause license;
* [`tests/UnitTests/timing.h`](tests/UnitTests/timing.h) is based on code by Doug Whiting, which he released to the public domain;
* [`KeccakP-1600-AVX2.s`](lib/low/KeccakP-1600/OptimizedAVX2/KeccakP-1600-AVX2.s) is licensed under the [CRYPTOGAMS license](http://www.openssl.org/~appro/cryptogams/) (BSD-like).




# How can I build the XKCP?

To build, the following tools are needed:

* *GCC*
* *GNU make*
* *xsltproc*

The different targets are defined in [`Makefile.build`](Makefile.build). This file is expanded into a regular makefile using *xsltproc*. To use it, simply type, e.g.,

> `make generic64/KeccakTests`

to build KeccakTests generically optimized for 64-bit platforms. The name before the slash indicates the platform, while the part after the slash is the executable to build. As another example, the static (resp. dynamic) library is built by typing `make generic64/libkeccak.a` (resp. `.so`) or similarly with `generic64` replaced with the appropriate platform name.  An alternate C compiler can be specified via the `CC` environment variable.

Instead of building an executable with *GCC*, one can choose to select the files needed and make a package. For this, simply append `.pack` to the target name, e.g.,

> `make generic64/KeccakTests.pack`

This creates a `.tar.gz` archive with all the necessary files to build the given target.

The list of targets can be found at the end of [`Makefile.build`](Makefile.build) or by running `make` without parameters.


## Microsoft Visual Studio support (experimental)

The XKCP offers support for the creation of Microsoft Visual Studio (VS) project files. To get a project file for a given target, simply append `.vcxproj` to the target name, e.g.,

> `make generic32/KeccakTests.vcxproj`

This is still somehow experimental, but it should be easy to extend/adapt by someone with more experience with VS. (*Help welcome!*) In particular, please note the current limitations:

- The generated project files current target Win32 only, but this should be easy to adapt for 64-bit support. See the file [`ToVCXProj.xsl`](support/Build/ToVCXProj.xsl).
- The assembly code, as used in some targets, follows the GCC syntax and at this point cannot be used directly with VS.


# How do I build/extract just the part I need?

If you wish to make a custom target that integrates the cryptographic functions you need and nothing else, or if you just wish to get the source files to integrate them in another project, you can do this by following the steps described in [`doc/HOWTO-customize.build`](doc/HOWTO-customize.build). Some examples illustrate the process.



# How is the code organized?

The code is organized as illustrated in the following figure:

![Layers](doc/figures/Layers.png)

At the top, the high-level cryptographic services are implemented in plain C, without any specific optimizations. At the bottom, the low-level services implement the permutations and the state input/output functions, which can be optimized for a given platform. The interface between the two layers is called **SnP**.

The idea is to have a single, portable, code base for the high level and the possibility to dedicate the low level to certain platforms for best performance.

The modes and constructions can be found in [`lib/high/`](lib/high/), while the permutations are stored in [`lib/low/`](lib/low/).

The situation is similar for parallelized services, as illustrated on the following figure. The interface is adapated to the parallelism and is called **PlSnP**, with the implementations in [`lib/low/`](lib/low/).

![Parallel layers](doc/figures/ParallelLayers.png)

*Disclaimer*: the above figures aim at illustrative purposes only, as not all modes, constructions or permutations are currently implemented in the XKCP or represented on the figures.



# Where can I find more information?

About the XKCP, we gave some presentations on its motivation and structure, e.g.,

* at [FOSDEM in February 2017][FOSDEM2017] ([slides][slidesAtFOSDEM2017]),
* at [SPEED-B in October 2016][SPEEDB] ([slides][slidesAtSPEEDB]) ([paper][paperAtSPEEDB]),
* at the [SHA-3 Workshop in Santa Barbara in August 2014][SHA3workshop2014] ([slides][KCPslides]).

The XKCP follows an improved version of the structure proposed in the note ["A software interface for Keccak"][keccakinterface].

More information on the cryptographic aspects can be found:

* on Keccak at [`keccak.team`](https://keccak.team/keccak.html)
* on the FIPS 202 standard at [`csrc.nist.gov`](http://csrc.nist.gov/groups/ST/hash/sha-3/fips202_standard_2015.html)
* on the NIST SP 800-185 standard at [`keccak.team`](https://keccak.team/2016/sp_800_185.html)
* on Ketje at [`keccak.team`](https://keccak.team/ketje.html)
* on Keyak at [`keccak.team`](https://keccak.team/keyak.html)
* on KangarooTwelve at [`keccak.team`](https://keccak.team/kangarootwelve.html)
* on cryptographic sponge functions at [`keccak.team`](https://keccak.team/sponge_duplex.html)
* on Kravatte at [`keccak.team`](https://keccak.team/kravatte.html)
* on Xoodoo and Xoofff at [`eprint.iacr.org`](https://eprint.iacr.org/2018/767)
* on the Farfalle construction at [`keccak.team`](https://keccak.team/farfalle.html)



# How can I contribute?

We welcome contributions in various forms, e.g., general feedback, bug reports, improvements and optimized implementations on your favorite platforms. The best is to do this through GitHub. Alternatively, you can send us a mail at `all` _-at-_ `keccak` _-dot-_ `team`.



# How can I use the standard FIPS 202 functions?

If you need to implement the standard FIPS 202 functions, the functions in [`SimpleFIPS202.h`](lib/high/Keccak/FIPS202/SimpleFIPS202.h) provide an easy way to get started. If a message queue and/or bit-level inputs are needed, then the macros in [`KeccakHash.h`](lib/high/Keccak/FIPS202/KeccakHash.h) can be helpful.

## Differences between Keccak and the standard FIPS 202 functions

Compared to the (plain) Keccak sponge function, the [FIPS 202 standard][fips202_standard] adds suffixes to ensure that the hash functions (SHA-3) and the XOFs (SHAKE) are domain separated (i.e., so that their outputs are unrelated even with equal inputs), as well as to make the SHAKE functions compatible with the [Sakura][sakura] tree hashing coding.

[sakura]: https://keccak.team/files/Sakura.pdf "Sakura: a flexible coding for tree hashing"

A brief summary:

* For the SHA-3 functions, append the bits "01" to the message prior to  applying the pad10*1 rule.
* For the SHAKE functions, append the bits "1111" to the message prior to applying the pad10*1 rule.

When they refer to the functions in the [FIPS 202 standard][fips202_standard], the test cases in [`tests/TestVectors/`](tests/TestVectors/) include these suffixes.



# Acknowledgments

- `genKAT.c` based on the SHA-3 contest's genKAT.c by Larry Bassham, NIST
- `brg_endian.h` by Brian Gladman
- `timing.h` based on code by Doug Whiting
- `lib/low/KeccakP-1600/OptimizedAVX2/KeccakP-1600-AVX2.s` by Andy Polyakov and Ronny Van Keer
- `TweetableFIPS202.c` by D. J. Bernstein, Peter Schwabe and Gilles Van Assche
- `lib/low/KeccakP-800/Optimized64AsmARM/*` by Andre C. de Moraes

And thanks to all contributors!

***

The Keccak and Xoodoo designers: Guido Bertoni, Joan Daemen, Seth Hoffert,
Michaël Peeters, Gilles Van Assche, and Ronny Van Keer.

[keccakinterface]: https://keccak.team/files/NoteSoftwareInterface.pdf
[SHA3workshop2014]: http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/index.html
[KCPslides]: http://csrc.nist.gov/groups/ST/hash/sha-3/Aug2014/documents/vanassche_keccak_code.pdf
[FOSDEM2017]: https://fosdem.org/2017/schedule/event/keccak/
[slidesAtFOSDEM2017]: https://fosdem.org/2017/schedule/event/keccak/attachments/slides/1692/export/events/attachments/keccak/slides/1692/KeccakAtFOSDEM2017.pdf
[fips202_standard]: http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf "FIPS 202 standard"
[sp800_185_standard]: https://doi.org/10.6028/NIST.SP.800-185 "NIST SP 800-185 standard"
[caesar_ketje]: https://keccak.team/ketje.html
[caesar_keyak]: https://keccak.team/keyak.html
[k12]: https://keccak.team/kangarootwelve.html
[SPEEDB]: http://ccccspeed.win.tue.nl/
[paperAtSPEEDB]: http://ccccspeed.win.tue.nl/papers/KeccakSoftware.pdf
[slidesAtSPEEDB]: http://ccccspeed.win.tue.nl/presentations/KeccakSoftware-slides.pdf
[XoodooCookbook]: https://eprint.iacr.org/2018/767
