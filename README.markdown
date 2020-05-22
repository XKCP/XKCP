[![Build Status](http://img.shields.io/travis/XKCP/XKCP.svg)](https://travis-ci.org/XKCP/XKCP)

<img src="doc/logo/XKCP-Anna-banner.svg" width="60%" />

# What is the XKCP?

The **eXtended Keccak Code Package** (or the **Xoodoo and Keccak Code Package**, in both cases abbreviated as **XKCP**) is a repository that gathers different free and open-source implementations of the cryptographic schemes defined by the Keccak team.
This includes the [Keccak sponge function family](https://keccak.team/keccak.html)
and closely related variants, such as

* the SHAKE extendable-output functions and SHA-3 hash functions from [FIPS 202][fips202_standard],
* the cSHAKE, KMAC, ParallelHash and TupleHash functions from [NIST SP 800-185][sp800_185_standard],
* the [Ketje][caesar_ketje] and [Keyak][caesar_keyak] authenticated encryption schemes,
* the fast [KangarooTwelve][k12] extendable-output function,
* the [Kravatte](https://keccak.team/kravatte.html) pseudo-random function and its modes,

as well as the [Xoodoo](https://keccak.team/xoodoo.html) permutation and

* the [Xoofff](https://keccak.team/xoofff.html) pseudo-random function and its modes (experimental),
* the [Xoodyak](https://keccak.team/xoodyak.html) scheme (submission to the NIST lightweight crypto standardization process).

The code in this repository can be built as a library called libXKCP.



# What is libXKCP?

**libXKCP** is a library that contains all the Keccak and Xoodoo-based cryptographic schemes mentioned above.

To build it, the quick answer is to launch:

```
make <target>/libXKCP.so
```

where `<target>` is to be replaced with the actual target (e.g., `ARMv6M` or `AVX512`), and where `.so` can be replaced with `.a` for a static library or with `.dylib` for a dynamic library on macOS.
More details, and in particular the list of targets, can be found in the section on how to build the XKCP below.



# More precisely, what does the XKCP contain?

First, the services available in this package are divided into high-level and low-level services. In a nutshell, the low level corresponds to Keccak-_f_[1600] and basic state manipulation, while the high level contains the constructions and the modes for, e.g., sponge functions, hashing or authenticated encryption. For more details, please see the section "_How is the code organized?_" below.

Second, these high-level and low-level services can be compiled as the libXKCP library.

Then, the XKCP also contains some utilities for testing, benchmarking and illustration purposes.

Finally, the repository contains some standalone implementations.


## High-level services

When used as a library or directly from the sources, the XKCP offers the high-level services documented in the following header files:

* [`SimpleFIPS202`](lib/high/Keccak/FIPS202/SimpleFIPS202.h), the six approved FIPS 202 instances (SHAKE128, SHAKE256 and the SHA-3 hash functions) through simple functions.
* [`KeccakHash`](lib/high/Keccak/FIPS202/KeccakHash.h), the six approved FIPS 202 instances, as well as any Keccak instance based on Keccak-_f_[1600]. This more advanced interface proposes a message queue (init-update-final) and supports bit-level inputs if needed.
* [`SP800-185`](lib/high/Keccak/SP800-185/SP800-185.h), the functions (cSHAKE, KMAC, ParallelHash and TupleHash) in the official NIST SP 800-185 standard.
* [`KeccakSponge`](doc/KeccakSponge-documentation.h), all Keccak sponge functions, with or without a message queue.
* [`KeccakDuplex`](doc/KeccakDuplex-documentation.h), all Keccak duplex objects.
* [`KeccakPRG`](doc/KeccakPRG-documentation.h), a pseudo-random number generator based on Keccak duplex objects.
* [`Keyak`](doc/Keyak-documentation.h), the authenticated encryption schemes River, Lake, Sea, Ocean and Lunar Keyak.
* [`Ketje`](doc/Ketje-documentation.h), the lightweight authenticated encryption schemes Ketje Jr, Ketje Sr, Ketje Minor and Ketje Major.
* [`KangarooTwelve`](lib/high/KangarooTwelve/KangarooTwelve.h), the fast hashing mode based on Keccak-_p_[1600, 12] and Sakura coding.
* [`Kravatte`](lib/high/Kravatte/Kravatte.h) and [`KravatteModes`](lib/high/Kravatte/KravatteModes.h), the pseudo-random function Kravatte, as well as the modes on top of it (SANE, SANSE, WBC and WBC-AE).
* [`Xoofff`](lib/high/Xoofff/Xoofff.h) and [`XoofffModes`](lib/high/Xoofff/XoofffModes.h), the pseudo-random function Xoofff, as well as the modes on top of it (SANE, SANSE, WBC and WBC-AE).
* [`Xoodyak`](doc/Xoodyak-documentation.h), the lightweight cryptographic scheme Xoodyak that can be used for hashing, encryption, MAC computation and authenticated encryption.


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

In both cases, the hierarchy first selects a permutation (or a permutation and a degree of parallelism) and then a given implementation. E.g., one finds in [`lib/low/KeccakP-1600-times4/`](lib/low/KeccakP-1600-times4/) the implementations of 4 parallel instances of Keccak-_p_[1600] and in [`lib/low/KeccakP-1600-times4/AVX2/`](lib/low/KeccakP-1600-times4/AVX2/) a 256-bit SIMD implementation for AVX2.

The documentation of the low-level services can be found in [`SnP-documentation.h`](doc/SnP-documentation.h) and [`PlSnP-documentation.h`](doc/PlSnP-documentation.h).


## Utilities

The package contains:

* The **libXKCP** library;
* [**Self-tests**](tests/UnitTests/main.c) that ensure that the implementation is working properly;
* [**A benchmarking tool**](tests/Benchmarks/main.c) to measure the timing of the various schemes;
* [**KeccakSum**](util/KeccakSum/KeccakSum.c) that computes a hash of the file (or multiple files) given in parameter.

Note that, to run the benchmarks on ARM processors, you may need to include the Kernel-PMU module, see [`Kernel-pmu.md`](support/Kernel-PMU/Kernel-pmu.md) for more details.


## Standalone implementations

The XKCP also provides some standalone implementations, including:

* a very [compact](https://keccak.team/2015/tweetfips202.html) C code of the FIPS 202 (SHA-3) standard in [`Standalone/CompactFIPS202/C/`](Standalone/CompactFIPS202/C/);
* a compact implementation in Python in [`Standalone/CompactFIPS202/Python/`](Standalone/CompactFIPS202/Python/);
* the reference code of KangarooTwelve in Python in [`Standalone/KangarooTwelve/Python/`](Standalone/KangarooTwelve/Python/).



# Under which license is the XKCP distributed?

Most of the source and header files in the XKCP are released to the **public domain** and associated to the [CC0](http://creativecommons.org/publicdomain/zero/1.0/) deed. The exceptions are the following:

* [`lib/common/brg_endian.h`](lib/common/brg_endian.h) is copyrighted by Brian Gladman and comes with a BSD 3-clause license;
* [`tests/UnitTests/genKAT.c`](tests/UnitTests/genKAT.c) is based on [SHA-3 contest's code by Larry Bassham, NIST](http://csrc.nist.gov/groups/ST/hash/sha-3/documents/KAT1.zip), which he licensed under a BSD 3-clause license;
* [`tests/UnitTests/timing.h`](tests/UnitTests/timing.h) is adapted from Google Benchmark and is licensed under the Apache License, Version 2.0;
* [`KeccakP-1600-AVX2.s`](lib/low/KeccakP-1600/AVX2/KeccakP-1600-AVX2.s) is licensed under the [CRYPTOGAMS license](http://www.openssl.org/~appro/cryptogams/) (BSD-like);
* [`support/Kernel-PMU/enable_arm_pmu.c`](support/Kernel-PMU/enable_arm_pmu.c) is licensed under the GNU General Public License by Bruno Pairault.




# How can I build the XKCP?

To build on Linux or macOS, the following tools are needed:

* *GCC* or *clang*
* *GNU make*
* *xsltproc*

The different targets are defined in [`Makefile.build`](Makefile.build). This file is expanded into a regular makefile using *xsltproc*. To use it, simply type, e.g.,

```
make generic64/UnitTests
```

or

```
make AVX512/Benchmarks
```

to build UnitTests using plain 64-bit code or to build the Benchmarks tool with AVX-512 code. The name before the slash indicates the target, i.e., the platform or instruction set used, while the part after the slash is the executable or library to build. As another example, the static (resp. dynamic) library is built by typing `make ARMv7M/libXKCP.a` (resp. `.so`) or similarly with `ARMv7M` replaced with the appropriate platform or instruction set name.  An alternate C compiler can be specified via the `CC` environment variable.

At the time of this writing, the possible target names before the slash are:

* `compact`: plain C compact implementations;
* `generic32`: plain C implementation, generically optimized for 32-bit platforms;
* `generic32lc`: same as `generic32` but featuring the lane complementing technique for platforms without a "and not" instruction;
* `generic64`: plain C implementation, generically optimized for 64-bit platforms;
* `generic64lc`: same as `generic64` but featuring the lane complementing technique for platforms without a "and not" instruction;
* `SSSE3`: implementations selected for the processors that support the SSSE3 instruction set;
* `AVX`: implementations selected for processors that support the AVX instruction set (e.g., Sandy Bridge microarchitectures);
* `XOP`: implementations selected for processors that support the XOP instruction set (e.g., Bulldozer microarchitecture);
* `AVX2`: implementations selected for processors that support the AVX2 instruction set (e.g., Haswell and Skylake microarchitectures);
* `AVX512`: implementations selected for the processors that support the AVX-512 instruction set (e.g., SkylakeX microarchitecture);
* `ARMv6`: implementations selected for processors with the ARMv6 architecture;
* `ARMv6M`: implementations selected for processors with the ARMv6-M architecture;
* `ARMv7M`: implementations selected for processors with the ARMv7-M architecture;
* `ARMv7A`: implementations selected for processors with the ARMv7-A architecture;
* `ARMv8A`: implementations selected for processors with the ARMv8-A architecture;
* `AVR8`: implementations selected for processors with the 8-bit AVR architecture.

Instead of building an executable with *GCC*, one can choose to select the files needed and make a package. For this, simply append `.pack` to the target name, e.g.,

```
make generic64/UnitTests.pack
```

This creates a `.tar.gz` archive with all the necessary files to build the given target.

The list of targets can be found at the end of [`Makefile.build`](Makefile.build) or by running `make` without parameters.


## Microsoft Visual Studio support

The XKCP can be compiled with Microsoft Visual Studio (MSVC). The XKCP build system offers support for the creation of project files. To get a project file for a given target, simply append `.vcxproj` to the target name, e.g.,

```
make AVX512noAsm/KeccakSum.vcxproj
```

As of today, please note the current limitations:

- The assembly code, as used in some targets, follows the GCC syntax and at this point cannot be used directly with MSVC. Note that the `AVX2noAsm` and `AVX512noAsm` targets provide alternatives to `AVX2` and `AVX512`, respectively, without assembly implementations.
- There is no support yet to build a dynamic library like `libXKCP.dll`. However, we are not far: `make <target>/libXKCP.so.vcxproj` gives you a project that compiles correctly (but does not link).


# How do I build/extract just the part I need?

If you wish to make a custom target that integrates the cryptographic functions you need and nothing else, or if you just wish to get the source files to integrate them in another project, you can do this by following the steps described in [`doc/HOWTO-customize.build`](doc/HOWTO-customize.build). Some examples illustrate the process.



# How is the code organized?

The code is organized as illustrated in the following figure:

<p align="center">
<img src="doc/figures/Layers.svg" width="80%" />
</p>

At the top, the high-level cryptographic services are implemented in plain C, without any specific optimizations. At the bottom, the low-level services implement the permutations and the state input/output functions, which can be optimized for a given platform. The interface between the two layers is called **SnP**.

The idea is to have a single, portable, code base for the high level and the possibility to dedicate the low level to certain platforms for best performance.

The modes and constructions can be found in [`lib/high/`](lib/high/), while the permutations are stored in [`lib/low/`](lib/low/).

The situation is similar for parallelized services, as illustrated on the following figure. The interface is adapated to the parallelism and is called **PlSnP**, with the implementations in [`lib/low/`](lib/low/).

<p align="center">
<img src="doc/figures/ParallelLayers.svg" width="80%" />
</p>

*Disclaimer*: the above figures aim at illustrative purposes only, as not all modes, constructions or permutations are currently implemented in the XKCP or represented on the figures.



# How fast is the code in the XKCP?

Whenever possible, we try to integerate the fastest available open-source code into the repository.
Should you find better implementations, do not hesitate to inform us.

Benchmarks using the XKCP and comparisons with other functions can be found on [this page](https://keccak.team/sw_performance.html).



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
* on Xoodoo, Xoofff and Xoodyak at [`eprint.iacr.org`](https://eprint.iacr.org/2018/767)
* on the Farfalle construction at [`keccak.team`](https://keccak.team/farfalle.html)



# How can I contribute?

We welcome contributions in various forms, e.g., general feedback, bug reports, improvements and optimized implementations on your favorite platforms. The best is to do this through GitHub. Alternatively, you can send us a mail at `all` _-at-_ `keccak` _-dot-_ `team`.



# Acknowledgments

We wish to thank all the contributors, and in particular:

- Andre C. de Moraes for ARMv8-A assembly code
- Andy Polyakov and Ronny Van Keer for the AVX2 and AVX-512 assembly implementations of Keccak-_p_[1600]
- Anna Guinet for the hummingbird logo design
- Brian Gladman's `brg_endian.h`
- Bruno Pairault for testing and benchmarking on ARM platforms
- Conno Boel for the NEON implementations of Xoodoo
- D.J. Bernstein, Peter Schwabe and Gilles Van Assche for the tweetable FIPS 202 implementation `TweetableFIPS202.c`
- Hussama Ismail for setting up the continuous integration with Travis
- Kent Ross for various improvements in [XKCP/K12](https://github.com/XKCP/K12) imported here
- Larry Bassham, NIST for the original `genKAT.c` developed during the SHA-3 contest
- Stéphane Léon for helping support macOS


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
