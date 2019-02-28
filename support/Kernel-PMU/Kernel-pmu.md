# XKCP ARM cycle count extension with a Cortex-A57 / RPIB3+ example

**By Bruno Pairault, 2019**

![MacDown logo](https://www.enggwave.com/wp-content/uploads/2015/06/ARM-Logo-150x127.png)

**ARM** allow the counting of cycles via few techniques :
**DWT** or nowdays **Performance Monitor Unit** (PMU). There is several versions of PMU for the different
Cortex processor.

The **ARM cycle count implementation** is Linux Kernel dependant to allow the PMU managment for User.
Enabling the PMU on user-mode is provided per **Kernel-PMU** and is independent of XKCP.

The XKCP PMU implementation supports two architectures yet **Aarch64** and **ARMv7-A**. The two XKCP
PMU implementations to read cycles are coded in the XKCP file `tests/UnitTest/Timing.h` This file
is only updated using the several native preprocessor compilation options. **Performance Monitors Cycle
Counters** are then read when required.



## Principle of Kernel-PMU
**Kernel-PMU** is a Linux dependant module that has to be loaded to allow the PMU user-mode.
XKCP `tests/UnitTest/Timing.h` will then be abble to read the register.

The [Cortex-A57](https://en.wikipedia.org/wiki/ARM_Cortex-A57)
documentation can be found on the ARM site for details on the [PMU](https://developer.arm.com/docs/ddi0488/latest/preface) . The Chapter 11
describes the Performance Monitor Unit. For all the **Cortex A**, you can reach the [Cortex-A](https://developer.arm.com/products/processors/cortex-a) technical library.
##Principle of the computation
When the  **Kernel-PMU** is loaded, the processor is likely to answer with the current cycle count for PMU 32
or 64 Architecture. Obviously similar PMU thread exist on Internet however this integration in XKCP follow exactly the principle introduced initially in `Timing.h` by **Doug Whiting**
for Intel via `RTDSC`. Minimal changes are required in **XKCP**. We added the few ASM instructions to read the PMU according to the precompilations options.
The number of cycles of an operation is the difference between the cycle counter before and the cycle
counter after.

## Install

1. Install the kernel-header version for your kernel release number given per the command `#uname -r`.
The method is dependant on your Packet Manager (zypper, apt-get, dpkg).

	* Raspbian  (32 bits): `sudo apt-get install raspberrypi-kernel-headers`

	* OpenSuse Leap 15 (64 bits): Likely on a fresh install `nothing to do` or if the OS has been updated you will have probably to run  `sudo zypper in kernel-dev`.

2. Compile Kernel-PMU

		#cd Kernel-PMU
		#make
			
3. Load Kernel-PMU and Check


		#sudo ./load-module
		#dmesg
	
	
	The expect result for **Cortex-a57** is similar to :
	
	
	
		[ 6014.689804] ENABLE_ARM_PMU enabling user PMU access on CPU-Core #0
		[ 6014.689809] ENABLE_ARM_PMU enabling user PMU access on CPU-Core #2
		[ 6014.689813] ENABLE_ARM_PMU enabling user PMU access on CPU-Core #3
		[ 6014.689820] ENABLE_ARM_PMU enabling user PMU access on CPU-Core #1
	
###Know-Issues	
* Depending on your Linux OS (Raspbian, Ubuntu Mate, OpenSuse) and your OS release you may also
be required to download from source the kernel files to compile the module. The module will not load if
the matching is not correct. Modify the path in the makefile if required.

* The module must be reload after rebooting if you do not implement auto-load @reboot.


##Check the CPU configuration and the native GCC options
* The objective is to understand the compilation you need to added if you use `-mnative`.
* Sanity check the CPU configuration and check all the native `-mXXX` compilations options like `march`,`
mtune`.

		# lscpu
		# cat /proc/cpuinfo
		# gcc -v
		
###Know-Issues Raspbian case with Updated system OS on 2019-01-01
* On Raspbian the Cortex-a53 is declared as an ARMv7l with 4 cores as per `lscpu`. GCC [(gcc
version 6.3.0 20170516 (Raspbian 6.3.0-18+rpi1+deb9u1)] come with native ARMv6 `-march`
option. This is due to the implementation. Type `cat /proc/cpuinfo` and you will discover the
**BCM2835** CPU which is the ARMv6 of the first RPI1(2010). It looks like it’s an ARMv6 for each of the 4
cores. The impact is that we have to force the precompiler options to use the PMU 32 bits for ARM.

* **Cortex-a53** RPIB3+ is an **ARMV8-a 64 bits** running mostly 32 bits as most OS for RPI are 32 bits.


* The preprocessor options `-D__ARM_xx` have to be consistent with the processor and the OS. On a
32 bits implementation the PMU register is the 32 bits PMU while the 64 bits PMU counter will be used
on Aarch64.

##XKCP Compilation options tips for Cortex-a53 and RPIB3+

Depending on what you are trying to achieve you may like to check the compilation options for gcc or
modify the XKCP `lib/LowLevel.build`. Below some suggestions.

For 32 bits OS on ARM PMU on target

The RPI3 on 32Bits OS is declared as a ARMv6 per GCC precompilation options. We have then to force
the `ARM_ARCH_7A` option to read the 32 bit Cycle counter.


	▪ <gcc>-v</gcc> This option add verbose
	▪ <gcc>-D__ARM_ARCH_7A_</gcc>
	
For 64 bits OS on ARM on target : `-mnative` will likely implement natively `__aarch64` . it is
suggested per GCC as good practice to enable `fix-cortex-a53-835769` and
`fix-cortex-a53-843419`.

For Cross Compilation 32 bits

	▪ <gcc>-march=armv8-a+crc</gcc>
	▪ <gcc>-mtune=cortex-a53</gcc>
	▪ <gcc>-mlittle-endian</gcc>

For Cross Compilation 64 bits using aarch64-linux-gnu-gcc

	▪ <gcc>-march=armv8-a+crc</gcc>
	▪ <gcc>-mtune=cortex-a53</gcc>
	▪ <gcc>-mlittle-endian</gcc>
	▪ <gcc>-mabi=lp64</gcc>


Finally compile **KeccakTests**.


##License
This code is for GNU General Public License (GNU GPL or GPL) which guarantees end users the freedom
to run, study, share and modify the Kernel-PMU.
