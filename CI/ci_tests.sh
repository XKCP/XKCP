#!/bin/sh

set -e

## For wine to be quiet when trying to use an external DISPLAY
Xvfb :1 &
export DISPLAY=:1

## NOTE: for now, this is not activated by default (OTHER_PLATFORMS=1 must be forced).
## This is due to the fact that for now, XKCP does not pass the tests on these platforms because
## of either endianness issues (missing macros) or alignment issues (see issue #124).
if [ "$OTHER_PLATFORMS" = "1" ]; then
	# Non-x86 and non-ARM platforms little endian platforms (big endian not supported yet for all the functions in XKCP)
	echo "\n\n\n=========== Switching to cross-compilation of generic impletation on non-x86 and non-ARM platforms\n\n\n"
	for t in generic32 generic32lc generic64 generic64lc; do
		for c in mips-linux-gnu-gcc mipsel-linux-gnu-gcc sparc64-linux-gnu-gcc mips64-linux-gnuabi64-gcc mips64el-linux-gnuabi64-gcc riscv64-linux-gnu-gcc; do
		arch=`echo -n $c | cut -d'-' -f1`
		echo "\n\n\n=========== Compiling with $t with $c compiler for architecture $arch\n\n\n"
		make clean && CC=$c NO_FLAGS_NATIVE=1 EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
		echo "\n\n\n=========== Testing $t (compiled with $c compiler) for architecture $arch\n\n\n"
		qemu-$arch-static ./bin/$t/UnitTests -p
		done
	done
else
	echo "\n\n\nSkipping non-x86 and non-ARM platforms, as OTHER_PLATFORMS has not been defined ...\n\n\n"
fi

## x86_64 targets using the local host compiler (gcc or clang)
for t in generic32 generic32lc generic64 generic64lc SSSE3 AVX AVX2 AVX2noAsm AVX512 AVX512noAsm; do
	for c in gcc clang x86_64-w64-mingw32-gcc; do
		echo "=========== Compiling $t (with $c compiler)\n\n\n"
		make clean && CC=$c make $t/UnitTests -j`nproc`
		# NOTE: AVX512 is not supported by Qemu as of now, so do not test them
		if echo "$t" | grep -q "AVX512"; then
			echo "\n\n\n=========== Skipping run test for $t (compiled with $c compiler): AVX512 not supported yet by Qemu (and not supported by native CI CPUs)\n\n\n"
		else
			if echo "$c" | grep -q "mingw"; then
				# NOTE: for mingw targets, it is complicated to emulate non generic instruction set on any CPU
				if echo "$t" | grep -q "generic"; then
					echo "\n\n\n=========== Testing $t (compiled with $c compiler) for x86_64 windows\n\n\n"
					echo "============== (emulating with wine)"
					wine ./bin/$t/UnitTests.exe -p
				fi
			else
				echo "\n\n\n=========== Testing $t (compiled with $c compiler) for x86_64\n\n\n"
				qemu-x86_64-static ./bin/$t/UnitTests -p
			fi
		fi
	done
done

## x86 32 bits targets on generic implementations
for t in generic32 generic32lc generic64 generic64lc; do
	for c in i686-linux-gnu-gcc i686-w64-mingw32-gcc; do
		echo "=========== Compiling $t (with $c compiler)\n\n\n"
		make clean && CC=$c EXTRA_CFLAGS="-static" make $t/UnitTests -j`nproc`
		if echo "$c" | grep -q "mingw"; then
			# NOTE: for mingw targets, it is complicated to emulate non generic instruction set on any CPU
			if echo "$t" | grep -q "generic"; then
				echo "\n\n\n=========== Testing $t (compiled with $c compiler) for i386 windows\n\n\n"
				echo "============== (emulating with wine)"
				wine ./bin/$t/UnitTests.exe -p
			fi
		else
			echo "\n\n\n=========== Testing $t (compiled with $c compiler) for i386\n\n\n"
			qemu-i386-static ./bin/$t/UnitTests -p
		fi
	done
done

echo "\n\n\n=========== Switching to cross-compilation of C variants for ARM Cortex-A platforms\n\n\n"
for t in generic32 generic32lc generic64 generic64lc; do
	# ARM v6A
	echo "\n\n\n=========== Compiling $t/UnitTests for ARM v6A\n\n\n"
	make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv6 -mtune=arm1136j-s" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
	# NOTE: there is a bus error with generic64(lc) for now, see issue #124 (https://github.com/XKCP/XKCP/issues/124)
	# Hence, we deactivate the unit tests for this specific case.
	if echo "$t" | grep -q "generic64"; then
		true
	else
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu for ARM v6A\n\n\n"
		qemu-arm-static ./bin/$t/UnitTests -p
	fi
	# ARM v7A
	echo "\n\n\n=========== Compiling $t/UnitTests for ARM v7A\n\n\n"
	make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv7-a -mtune=cortex-a15" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
	# NOTE: there is a bus error with generic64(lc) for now, see issue #124 (https://github.com/XKCP/XKCP/issues/124)
	# Hence, we deactivate the unit tests for this specific case.
	if echo "$t" | grep -q "generic64"; then
		true
	else
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu for ARM v7A\n\n\n"
		qemu-arm-static ./bin/$t/UnitTests -p
	fi
	# ARM 64
	echo "\n\n\n=========== Compiling $t/UnitTests for ARM v8A (aarch64)\n\n\n"
	make clean && CC="aarch64-linux-gnu-gcc" EXTRA_CFLAGS="-march=armv8.6-a -mtune=cortex-a75" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
	echo "\n\n\n=========== Testing $t/UnitTests with Qemu for  ARM v8A (aarch64)\n\n\n"
	qemu-aarch64-static ./bin/$t/UnitTests -p
done

echo "\n\n\n=========== Switching to cross-compilation of assembly optimized variants for ARM Cortex-A platforms\n\n\n"
### ARM A
# ARM v6A
t="ARMv6"
echo "\n\n\n=========== Compiling $t/UnitTests\n\n\n"
make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv6 -mtune=arm1136j-s" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
echo "\n\n\n=========== Testing $t/UnitTests with Qemu\n\n\n"
qemu-arm-static ./bin/$t/UnitTests -p
# ARM v7A
t="ARMv7A"
echo "\n\n\n=========== Compiling $t/UnitTests\n\n\n"
make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv7-a -mtune=cortex-a15" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
echo "\n\n\n=========== Testing $t/UnitTests with Qemu\n\n\n"
qemu-arm-static ./bin/$t/UnitTests -p
# ARM v8A with aarch64
t="ARMv8A"
echo "\n\n\n=========== Compiling $t/UnitTests\n\n\n"
make clean && CC="aarch64-linux-gnu-gcc" EXTRA_CFLAGS="-march=armv8.6-a -mtune=cortex-a75" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
echo "\n\n\n=========== Testing $t/UnitTests with Qemu\n\n\n"
qemu-aarch64-static ./bin/$t/UnitTests -p


echo "\n\n\n=========== Switching to cross-compilation of C and assembly optimized variants for ARM Cortex-M platforms\n\n\n"
### ARM M dedicated compilations: they need special care as we test them with semi-hosting qemu-system
## NOTE: we use some trick to force Qemu in semi-hosting mode to exit, as well as patch the call to "process" with the appropriate arguments
## as args are note taken by qemu-system
awk '/#include/ && !done { print "#include \"../../CI/qemu_semihosting_exit.h\""; done=1;}; 1;' tests/UnitTests/main.c > /tmp/main.c
sed -i 's/return process(argc, argv);/    char *args[] = {"", "-p"}; process(2, args); _exit_qemu();/' /tmp/main.c
mv /tmp/main.c tests/UnitTests/main.c

## NOTE: we would want to compile and test all the generic C code on the microcontrollers. However, some of them have some issues
## to be investigated. Hence, we only compile and test the ARMv6M and ARMv7M assembly versions.
#
#for t in generic32 generic32lc generic64 generic64lc ARMv6M ARMv7M; do
#
for t in ARMv6M ARMv7M; do
	if echo "$t" | grep -q "ARMv7M"; then
		true
	else
		echo "\n\n\n=========== Compiling $t/UnitTests for ARMv6M microcontrollers\n\n\n"
		make clean && CC="arm-none-eabi-gcc" EXTRA_CFLAGS="-march=armv6-m -mtune=cortex-m0 -specs=picolibc.specs --oslib=semihost -TCI/cortexm_layout.ld" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu-system semi-hosting\n\n\n"
		qemu-system-arm -semihosting-config enable=on -monitor none -serial none -nographic -machine mps2-an385,accel=tcg -no-reboot -kernel ./bin/$t/UnitTests
	fi
	##
	if echo "$t" | grep -q "ARMv6M"; then
		true
	else
		echo "\n\n\n=========== Compiling $t/UnitTests for ARMv7M microcontrollers\n\n\n"
		make clean && CC="arm-none-eabi-gcc" EXTRA_CFLAGS="-march=armv7-m -mtune=cortex-m3 -specs=picolibc.specs --oslib=semihost -TCI/cortexm_layout.ld" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu-system semi-hosting\n\n\n"
		qemu-system-arm -semihosting-config enable=on -monitor none -serial none -nographic -machine mps2-an385,accel=tcg -no-reboot -kernel ./bin/$t/UnitTests
	fi
done
