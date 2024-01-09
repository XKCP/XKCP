#!/bin/sh

set -e

# Create the artifacts folder
mkdir -p artifacts/

## For wine to be quiet when trying to use an external DISPLAY
Xvfb :1 &
export DISPLAY=:1

### Current running CPU native support
ARCH=`arch`
SSSE3=`grep -o ssse3 /proc/cpuinfo || /bin/true`
AVX=`grep -o 'avx ' /proc/cpuinfo || /bin/true`
AVX2=`grep -o avx2 /proc/cpuinfo || /bin/true`
AVX512VL=`grep -o avx512vl /proc/cpuinfo || /bin/true`
AVX512F=`grep -o avx512f /proc/cpuinfo || /bin/true`

echo "======== Symmary of the running CPU =======" | tee -a artifacts/CIlog.log
echo "ARCH     : $ARCH" | tee -a artifacts/CIlog.log
echo "SSSE3    : $SSSE3" | tee -a artifacts/CIlog.log
echo "AVX      : $AVX" | tee -a artifacts/CIlog.log
echo "AVX2     : $AVX2" | tee -a artifacts/CIlog.log
echo "AVX512VL : $AVX512VL" | tee -a artifacts/CIlog.log
echo "AVX512F  : $AVX512F" | tee -a artifacts/CIlog.log
echo "===========================================" | tee -a artifacts/CIlog.log

## NOTE: for now, this is not activated by default (OTHER_PLATFORMS=1 must be forced).
## This is due to the fact that for now, XKCP does not pass the tests on these platforms because
## of either endianness issues (missing macros) or alignment issues (see issue #124).
if [ "$OTHER_PLATFORMS" = "1" ]; then
	# Non-x86 and non-ARM platforms little endian platforms (big endian not supported yet for all the functions in XKCP)
	echo "\n\n\n=========== Switching to cross-compilation of generic impletation on non-x86 and non-ARM platforms\n\n\n" | tee -a artifacts/CIlog.log
	for t in generic32 generic32lc generic64 generic64lc; do
		for c in mips-linux-gnu-gcc mipsel-linux-gnu-gcc sparc64-linux-gnu-gcc mips64-linux-gnuabi64-gcc mips64el-linux-gnuabi64-gcc riscv64-linux-gnu-gcc; do
		arch=`echo -n $c | cut -d'-' -f1`
		echo "\n\n\n=========== Compiling with $t with $c compiler for architecture $arch\n\n\n" | tee -a artifacts/CIlog.log
		make clean && CC=$c NO_FLAGS_NATIVE=1 EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
		cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"$c"
		echo "\n\n\n=========== Testing $t (compiled with $c compiler) for architecture $arch\n\n\n" | tee -a artifacts/CIlog.log
		qemu-$arch-static ./bin/$t/UnitTests -p
		done
	done
else
	echo "\n\n\nSkipping non-x86 and non-ARM platforms, as OTHER_PLATFORMS has not been defined ...\n\n\n"
fi

## x86_64 targets using the local host compiler (gcc or clang)
for t in generic32 generic32lc generic64 generic64lc SSSE3 AVX AVX2 AVX2noAsm AVX512 AVX512noAsm; do
	for c in x86_64-linux-gnu-gcc x86_64-w64-mingw32-gcc gcc clang; do
		echo "=========== Compiling $t (with $c compiler)\n\n\n" | tee -a artifacts/CIlog.log
		make clean && CC=$c make $t/UnitTests -j`nproc`
		if echo "$c" | grep -q "mingw"; then
			cp ./bin/$t/UnitTests.exe artifacts/UnitTests_"$t"_"$c".exe
			WINEOK="NOK"
			# NOTE: for mingw targets, it is complicated to emulate non generic instruction set on any CPU
			# Hence, we check that we are 
			if echo "$t" | grep -q "generic"; then
				# For generic targets, we mainly check that this is a x86_64 platform for the
				# x86 compilers
				if echo "$c" | grep -q "x86"; then
					if [ "$ARCH" = "x86_64" ]; then
						WINEOK="OK"
					fi
				fi
			else
				# We check if the native CPU supports of the target instruction set
				if echo "$t" | grep -q "SSSE3"; then
					if [ "$SSSE3" != "" ]; then
						WINEOK="OK"
					fi
				fi
				if [ "$t" = "AVX" ]; then
					if [ "$AVX" != "" ]; then
						WINEOK="OK"
					fi
				fi
				if echo "$t" | grep -q "AVX2"; then
					if [ "$AVX2" != "" ]; then
						WINEOK="OK"
					fi
				fi
				if echo "$t" | grep -q "AVX512"; then
					if [ "$AVX512VL" != "" ] && [ "$AVX512F" != "" ]; then
						WINEOK="OK"
					fi
				fi
			fi
			if [ "$WINEOK" = "OK" ]; then
				echo "\n\n\n=========== Testing $t (compiled with $c compiler) for x86_64 windows\n\n\n" | tee -a artifacts/CIlog.log
				echo "============== (emulating with wine, native CPU support checked)" | tee -a artifacts/CIlog.log
				wine ./bin/$t/UnitTests.exe -p
			else
				echo "\n\n\n=========== Skipping run test with wine for $t (compiled with $c compiler): AVX512 not supported by the native CI CPU\n\n\n" | tee -a artifacts/CIlog.log
			fi
		else
			cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"$c"
			if echo "$t" | grep -q "AVX512"; then
				# NOTE: AVX512 is not supported by Qemu as of now, so dynamically check if the native CPU supports it or not
				if [ "$AVX512VL" != "" ] && [ "$AVX512F" != "" ]; then
					echo "\n\n\n=========== Testing $t (compiled with $c compiler) for x86_64 (native test as AVX512 is not supported yet by Qemu)\n\n\n" | tee -a artifacts/CIlog.log
					./bin/$t/UnitTests -p
				else
					echo "\n\n\n=========== Skipping run test for $t (compiled with $c compiler): AVX512 not supported yet by Qemu and not supported by the native CI CPU\n\n\n" | tee -a artifacts/CIlog.log
				fi
			else
				if [ "$AVX512VL" != "" ] && [ "$AVX512F" != "" ]; then
					# Since the -march=native / -mtune=native toggles are used, we cannot rely on Qemu on
					# CPUs that support AVX512. Hence we execute the test natively
					echo "\n\n\n=========== Testing $t (compiled with $c compiler) for x86_64 (native test as AVX512 is not supported yet by Qemu)\n\n\n" | tee -a artifacts/CIlog.log
                                        ./bin/$t/UnitTests -p
				else
					echo "\n\n\n=========== Testing $t (compiled with $c compiler) for x86_64\n\n\n" | tee -a artifacts/CIlog.log
                                        qemu-x86_64-static ./bin/$t/UnitTests -p
				fi
			fi
		fi
	done
done

## x86 32 bits targets on generic implementations
for t in generic32 generic32lc generic64 generic64lc; do
	for c in i686-linux-gnu-gcc i686-w64-mingw32-gcc; do
		echo "=========== Compiling $t (with $c compiler)\n\n\n" | tee -a artifacts/CIlog.log
		make clean && CC=$c EXTRA_CFLAGS="-static" make $t/UnitTests -j`nproc`
		if echo "$c" | grep -q "mingw"; then
			cp ./bin/$t/UnitTests.exe artifacts/UnitTests_"$t"_"$c".exe
			WINEOK="NOK"
			# NOTE: we should be ensured here that we have a 'generic' target
			# For generic targets, we mainly check that this is a x86_64 platform for the
			# x86 compilers
			if echo "$c" | grep -q "i686"; then
				if [ "$ARCH" = "x86_64" ] || [ "$ARCH" = "i386" ]; then
					WINEOK="OK"
				fi
			fi
			if [ "$WINEOK" = "OK" ]; then
				echo "\n\n\n=========== Testing $t (compiled with $c compiler) for i386 windows\n\n\n" | tee -a artifacts/CIlog.log
				echo "============== (emulating with wine)" | tee -a artifacts/CIlog.log
				wine ./bin/$t/UnitTests.exe -p
			fi
		else
			if [ "$AVX512VL" != "" ] && [ "$AVX512F" != "" ]; then
				# Since the -march=native / -mtune=native toggles are used, we cannot rely on Qemu on
				# CPUs that support AVX512. Hence we execute the test natively
				echo "\n\n\n=========== Testing $t (compiled with $c compiler) for i386 (native test as AVX512 is not supported yet by Qemu)\n\n\n" | tee -a artifacts/CIlog.log
				./bin/$t/UnitTests -p
			else
				cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"$c"
				(echo "\n\n\n=========== Testing $t (compiled with $c compiler) for i386\n\n\n") | tee -a artifacts/CIlog.log
				qemu-i386-static ./bin/$t/UnitTests -p
			fi
		fi
	done
done

echo "\n\n\n=========== Switching to cross-compilation of C variants for ARM Cortex-A platforms\n\n\n" | tee -a artifacts/CIlog.log
for t in generic32 generic32lc generic64 generic64lc; do
	# ARM v6A
	echo "\n\n\n=========== Compiling $t/UnitTests for ARM v6A\n\n\n" | tee -a artifacts/CIlog.log
	make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv6 -mtune=arm1136j-s" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
	cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"armv6"
	# NOTE: there is a bus error with generic64(lc) for now, see issue #124 (https://github.com/XKCP/XKCP/issues/124)
	# Hence, we deactivate the unit tests for this specific case.
	if echo "$t" | grep -q "generic64"; then
		true
	else
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu for ARM v6A\n\n\n" | tee -a artifacts/CIlog.log
		qemu-arm-static ./bin/$t/UnitTests -p
	fi
	# ARM v7A
	echo "\n\n\n=========== Compiling $t/UnitTests for ARM v7A\n\n\n" | tee -a artifacts/CIlog.log
	make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv7-a -mtune=cortex-a15" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
	cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"armv7a"
	# NOTE: there is a bus error with generic64(lc) for now, see issue #124 (https://github.com/XKCP/XKCP/issues/124)
	# Hence, we deactivate the unit tests for this specific case.
	if echo "$t" | grep -q "generic64"; then
		true
	else
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu for ARM v7A\n\n\n" | tee -a artifacts/CIlog.log
		qemu-arm-static ./bin/$t/UnitTests -p
	fi
	# ARM 64
	echo "\n\n\n=========== Compiling $t/UnitTests for ARM v8A (aarch64)\n\n\n" | tee -a artifacts/CIlog.log
	make clean && CC="aarch64-linux-gnu-gcc" EXTRA_CFLAGS="-march=armv8.6-a -mtune=cortex-a75" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
	cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"aarch64"
	echo "\n\n\n=========== Testing $t/UnitTests with Qemu for  ARM v8A (aarch64)\n\n\n" | tee -a artifacts/CIlog.log
	qemu-aarch64-static ./bin/$t/UnitTests -p
done

(echo "\n\n\n=========== Switching to cross-compilation of assembly optimized variants for ARM Cortex-A platforms\n\n\n") | tee -a artifacts/CIlog.log
### ARM A
# ARM v6A
t="ARMv6"
echo "\n\n\n=========== Compiling $t/UnitTests\n\n\n" | tee -a artifacts/CIlog.log
make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv6 -mtune=arm1136j-s" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"armv6a"
echo "\n\n\n=========== Testing $t/UnitTests with Qemu\n\n\n" | tee -a artifacts/CIlog.log
qemu-arm-static ./bin/$t/UnitTests -p
# ARM v7A
t="ARMv7A"
echo "\n\n\n=========== Compiling $t/UnitTests\n\n\n" | tee -a artifacts/CIlog.log
make clean && CC="arm-linux-gnueabi-gcc" EXTRA_CFLAGS="-march=armv7-a -mtune=cortex-a15" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"armv7a"
echo "\n\n\n=========== Testing $t/UnitTests with Qemu\n\n\n" | tee -a artifacts/CIlog.log
qemu-arm-static ./bin/$t/UnitTests -p
# ARM v8A with aarch64
t="ARMv8A"
echo "\n\n\n=========== Compiling $t/UnitTests\n\n\n" | tee -a artifacts/CIlog.log
make clean && CC="aarch64-linux-gnu-gcc" EXTRA_CFLAGS="-march=armv8.6-a -mtune=cortex-a75" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"aarch64"
echo "\n\n\n=========== Testing $t/UnitTests with Qemu\n\n\n" | tee -a artifacts/CIlog.log
qemu-aarch64-static ./bin/$t/UnitTests -p


echo "\n\n\n=========== Switching to cross-compilation of C and assembly optimized variants for ARM Cortex-M platforms\n\n\n" | tee -a artifacts/CIlog.log
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
		echo "\n\n\n=========== Compiling $t/UnitTests for ARMv6M microcontrollers\n\n\n" | tee -a artifacts/CIlog.log
		make clean && CC="arm-none-eabi-gcc" EXTRA_CFLAGS="-march=armv6-m -mtune=cortex-m0 -specs=picolibc.specs --oslib=semihost -TCI/cortexm_layout.ld" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
		cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"armv6m"
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu-system semi-hosting\n\n\n" | tee -a artifacts/CIlog.log
		qemu-system-arm -semihosting-config enable=on -monitor none -serial none -nographic -machine mps2-an385,accel=tcg -no-reboot -kernel ./bin/$t/UnitTests
	fi
	##
	if echo "$t" | grep -q "ARMv6M"; then
		true
	else
		echo "\n\n\n=========== Compiling $t/UnitTests for ARMv7M microcontrollers\n\n\n" | tee -a artifacts/CIlog.log
		make clean && CC="arm-none-eabi-gcc" EXTRA_CFLAGS="-march=armv7-m -mtune=cortex-m3 -specs=picolibc.specs --oslib=semihost -TCI/cortexm_layout.ld" EXTRA_ASMFLAGS=$EXTRA_CFLAGS EXTRA_LDFLAGS="-static" make $t/UnitTests -j`nproc`
		cp ./bin/$t/UnitTests artifacts/UnitTests_"$t"_"armv7m"
		echo "\n\n\n=========== Testing $t/UnitTests with Qemu-system semi-hosting\n\n\n" | tee -a artifacts/CIlog.log
		qemu-system-arm -semihosting-config enable=on -monitor none -serial none -nographic -machine mps2-an385,accel=tcg -no-reboot -kernel ./bin/$t/UnitTests
	fi
done
