;
; The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
; Michaël Peeters and Gilles Van Assche. For more information, feedback or
; questions, please refer to our website: http://keccak.noekeon.org/
;
; Implementation by Ronny Van Keer, hereby denoted as "the implementer".
;
; To the extent possible under law, the implementer has waived all copyright
; and related or neighboring rights to the source code in this file.
; http://creativecommons.org/publicdomain/zero/1.0/
;

; WARNING: These functions work only on little endian CPU with ARMv7m architecture.

		PRESERVE8
		THUMB
		AREA    |.text|, CODE, READONLY
		ALIGN

		EXPORT  KeccakF1600_Initialize
		EXPORT  KeccakF1600_StateInitialize
		EXPORT  KeccakF1600_StateComplementBit
		EXPORT  KeccakF1600_StateXORLanes
		EXPORT  KeccakF1600_StateXORBytesInLane
		EXPORT  KeccakF1600_StateExtractLanes
		EXPORT  KeccakF1600_StateExtractBytesInLane
		EXPORT  KeccakF1600_StatePermute
		EXPORT  KeccakF1600_StateXORPermuteExtract

;//----------------------------------------------------------------------------

		;// Credit: Henry S. Warren, Hacker's Delight, Addison-Wesley, 2002
		MACRO
		toBitInterleaving	$x0,$x1,$s0,$s1,$t

		and		$t,$x0,#0x55555555
		orr		$t,$t,$t, LSR #1
		and		$t,$t,#0x33333333
		orr		$t,$t,$t, LSR #2
		and		$t,$t,#0x0F0F0F0F
		orr		$t,$t,$t, LSR #4
		and		$t,$t,#0x00FF00FF
		bfi		$t,$t,#8, #8
		eor		$s0,$s0,$t, LSR #8

		and		$t,$x1,#0x55555555
		orr		$t,$t,$t, LSR #1
		and		$t,$t,#0x33333333
		orr		$t,$t,$t, LSR #2
		and		$t,$t,#0x0F0F0F0F
		orr		$t,$t,$t, LSR #4
		and		$t,$t,#0x00FF00FF
		orr		$t,$t,$t, LSR #8
		eor		$s0,$s0,$t, LSL #16

		and		$t,$x0,#0xAAAAAAAA
		orr		$t,$t,$t, LSL #1
		and		$t,$t,#0xCCCCCCCC
		orr		$t,$t,$t, LSL #2
		and		$t,$t,#0xF0F0F0F0
		orr		$t,$t,$t, LSL #4
		and		$t,$t,#0xFF00FF00
		orr		$t,$t,$t, LSL #8
		eor		$s1,$s1,$t, LSR #16

		and		$t,$x1,#0xAAAAAAAA
		orr		$t,$t,$t, LSL #1
		and		$t,$t,#0xCCCCCCCC
		orr		$t,$t,$t, LSL #2
		and		$t,$t,#0xF0F0F0F0
		orr		$t,$t,$t, LSL #4
		and		$t,$t,#0xFF00FF00
		orr		$t,$t,$t, LSL #8
		bfc		$t, #0, #16
		eors	$s1,$s1,$t
		MEND

		;// Credit: Henry S. Warren, Hacker's Delight, Addison-Wesley, 2002
		MACRO
		fromBitInterleaving		$x0, $x1, $t

		movs	$t, $x0					;// t = x0;
		bfi		$x0, $x1, #16, #16		;// x0 = (x0 & 0x0000FFFF) | (x1 << 16);
		bfc		$x1, #0, #16			;//	x1 = (t >> 16) | (x1 & 0xFFFF0000);
		orr		$x1, $x1, $t, LSR #16

	    eor		$t, $x0, $x0, LSR #8    ;// t = (x0 ^ (x0 >>  8)) & 0x0000FF00UL;  x0 = x0 ^ t ^ (t <<  8);
		and		$t, #0x0000FF00
	    eors	$x0, $x0, $t
	    eor		$x0, $x0, $t, LSL #8

	    eor		$t, $x0, $x0, LSR #4	;// t = (x0 ^ (x0 >>  4)) & 0x00F000F0UL;  x0 = x0 ^ t ^ (t <<  4);
		and		$t, #0x00F000F0
	    eors	$x0, $x0, $t
	    eor		$x0, $x0, $t, LSL #4

	    eor		$t, $x0, $x0, LSR #2	;// t = (x0 ^ (x0 >>  2)) & 0x0C0C0C0CUL;  x0 = x0 ^ t ^ (t <<  2);
		and		$t, #0x0C0C0C0C
	    eors	$x0, $x0, $t
	    eor		$x0, $x0, $t, LSL #2

	    eor		$t, $x0, $x0, LSR #1	;// t = (x0 ^ (x0 >>  1)) & 0x22222222UL;  x0 = x0 ^ t ^ (t <<  1);
		and		$t, #0x22222222
	    eors	$x0, $x0, $t
	    eor		$x0, $x0, $t, LSL #1

	    eor		$t, $x1, $x1, LSR #8    ;// t = (x1 ^ (x1 >>  8)) & 0x0000FF00UL;  x1 = x1 ^ t ^ (t <<  8);
		and		$t, #0x0000FF00
	    eors	$x1, $x1, $t
	    eor		$x1, $x1, $t, LSL #8

	    eor		$t, $x1, $x1, LSR #4	;// t = (x1 ^ (x1 >>  4)) & 0x00F000F0UL;  x1 = x1 ^ t ^ (t <<  4);
		and		$t, #0x00F000F0
	    eors	$x1, $x1, $t
	    eor		$x1, $x1, $t, LSL #4

	    eor		$t, $x1, $x1, LSR #2	;// t = (x1 ^ (x1 >>  2)) & 0x0C0C0C0CUL;  x1 = x1 ^ t ^ (t <<  2);
		and		$t, #0x0C0C0C0C
	    eors	$x1, $x1, $t
	    eor		$x1, $x1, $t, LSL #2

	    eor		$t, $x1, $x1, LSR #1	;// t = (x1 ^ (x1 >>  1)) & 0x22222222UL;  x1 = x1 ^ t ^ (t <<  1);
		and		$t, #0x22222222
	    eors	$x1, $x1, $t
	    eor		$x1, $x1, $t, LSL #1
		MEND

;//	--- offsets in state
Aba0	equ  0*4
Aba1	equ  1*4
Abe0	equ  2*4
Abe1	equ  3*4
Abi0	equ  4*4
Abi1	equ  5*4
Abo0	equ  6*4
Abo1	equ  7*4
Abu0	equ  8*4
Abu1	equ  9*4
Aga0	equ 10*4
Aga1	equ 11*4
Age0	equ 12*4
Age1	equ 13*4
Agi0	equ 14*4
Agi1	equ 15*4
Ago0	equ 16*4
Ago1	equ 17*4
Agu0	equ 18*4
Agu1	equ 19*4
Aka0	equ 20*4
Aka1	equ 21*4
Ake0	equ 22*4
Ake1	equ 23*4
Aki0	equ 24*4
Aki1	equ 25*4
Ako0	equ 26*4
Ako1	equ 27*4
Aku0	equ 28*4
Aku1	equ 29*4
Ama0	equ 30*4
Ama1	equ 31*4
Ame0	equ 32*4
Ame1	equ 33*4
Ami0	equ 34*4
Ami1	equ 35*4
Amo0	equ 36*4
Amo1	equ 37*4
Amu0	equ 38*4
Amu1	equ 39*4
Asa0	equ 40*4
Asa1	equ 41*4
Ase0	equ 42*4
Ase1	equ 43*4
Asi0	equ 44*4
Asi1	equ 45*4
Aso0	equ 46*4
Aso1	equ 47*4
Asu0	equ 48*4
Asu1	equ 49*4

;//	--- offsets on stack
mDa0	equ 0*4
mDa1	equ 1*4
mDo0	equ 2*4
mDo1	equ 3*4
mDi0	equ 4*4
mRC		equ 5*4
mSize	equ 6*4


		MACRO
		xor5		$result,$b,$g,$k,$m,$s

		ldr			$result, [r0, #$b]
		ldr			r1, [r0, #$g]
		eors		$result, $result, r1				
		ldr			r1, [r0, #$k]
		eors		$result, $result, r1
		ldr			r1, [r0, #$m]
		eors		$result, $result, r1				
		ldr			r1, [r0, #$s]
		eors		$result, $result, r1
		MEND

		MACRO
		xorrol 		$result, $aa, $bb

		eor			$result, $aa, $bb, ROR #31
		MEND

		MACRO
		xandnot 	$resofs, $aa, $bb, $cc

		bic			r1, $cc, $bb
		eors		r1, r1, $aa
		str			r1, [r0, #$resofs]
		MEND

		MACRO
		KeccakThetaRhoPiChiIota $aA1, $aDax, $aA2, $aDex, $rot2, $aA3, $aDix, $rot3, $aA4, $aDox, $rot4, $aA5, $aDux, $rot5, $offset, $last
		ldr		r3, [r0, #$aA1]
		ldr		r4, [r0, #$aA2]
		ldr		r5, [r0, #$aA3]
		ldr		r6, [r0, #$aA4]
		ldr		r7, [r0, #$aA5]
		eors	r3, r3, $aDax
		eors	r5, r5, $aDix
		eors	r4, r4, $aDex
		eors	r6, r6, $aDox
		eors	r7, r7, $aDux
		rors	r4, #32-$rot2
		rors	r5, #32-$rot3
		rors	r6, #32-$rot4
		rors	r7, #32-$rot5
	    xandnot $aA2, r4, r5, r6
	    xandnot $aA3, r5, r6, r7
	    xandnot $aA4, r6, r7, r3
	    xandnot $aA5, r7, r3, r4
		ldr		r1, [sp, #mRC]
		bics	r5, r5, r4
		ldr		r4, [r1, #$offset]
		eors	r3, r3, r5
		eors	r3, r3, r4
		IF	$last == 1
		ldr		r4, [r1, #32]!
		str		r1, [sp, #mRC]
		cmp		r4, #0xFF
		ENDIF
		str		r3, [r0, #$aA1]
		MEND

		MACRO
		KeccakThetaRhoPiChi $aB1, $aA1, $aDax, $rot1, $aB2, $aA2, $aDex, $rot2, $aB3, $aA3, $aDix, $rot3, $aB4, $aA4, $aDox, $rot4, $aB5, $aA5, $aDux, $rot5
		ldr		$aB1, [r0, #$aA1]
		ldr		$aB2, [r0, #$aA2]
		ldr		$aB3, [r0, #$aA3]
		ldr		$aB4, [r0, #$aA4]
		ldr		$aB5, [r0, #$aA5]
		eors	$aB1, $aB1, $aDax
		eors	$aB3, $aB3, $aDix
		eors	$aB2, $aB2, $aDex
		eors	$aB4, $aB4, $aDox
		eors	$aB5, $aB5, $aDux
		rors	$aB1, #32-$rot1
		IF	$rot2 > 0
		rors	$aB2, #32-$rot2
		ENDIF
		rors	$aB3, #32-$rot3
		rors	$aB4, #32-$rot4
		rors	$aB5, #32-$rot5
		xandnot $aA1, r3, r4, r5
	    xandnot $aA2, r4, r5, r6
	    xandnot $aA3, r5, r6, r7
	    xandnot $aA4, r6, r7, r3
	    xandnot $aA5, r7, r3, r4
		MEND

		MACRO
		KeccakRound0

        xor5        r3,  Abu0, Agu0, Aku0, Amu0, Asu0
        xor5        r7, Abe1, Age1, Ake1, Ame1, Ase1
        xorrol      r6, r3, r7
		str			r6, [sp, #mDa0]
        xor5        r6,  Abu1, Agu1, Aku1, Amu1, Asu1
        xor5        lr, Abe0, Age0, Ake0, Ame0, Ase0
        eors        r8, r6, lr
		str			r8, [sp, #mDa1]

        xor5        r5,  Abi0, Agi0, Aki0, Ami0, Asi0
        xorrol      r9, r5, r6
		str			r9, [sp, #mDo0]
        xor5        r4,  Abi1, Agi1, Aki1, Ami1, Asi1
		eors		r3, r3, r4
		str			r3, [sp, #mDo1]

        xor5        r3,  Aba0, Aga0, Aka0, Ama0, Asa0
        xorrol      r10, r3, r4
        xor5        r6,  Aba1, Aga1, Aka1, Ama1, Asa1
        eors        r11, r6, r5

        xor5        r4,  Abo1, Ago1, Ako1, Amo1, Aso1
        xorrol      r5, lr, r4
		str			r5, [sp, #mDi0]
        xor5        r5,  Abo0, Ago0, Ako0, Amo0, Aso0
        eors        r2, r7, r5

        xorrol      r12, r5, r6
        eors        lr, r4, r3

		KeccakThetaRhoPiChi r5, Aka1, r8,  2, r6, Ame1, r11, 23, r7, Asi1, r2, 31, r3, Abo0, r9, 14, r4, Agu0, r12, 10
		KeccakThetaRhoPiChi r7, Asa1, r8,  9, r3, Abe0, r10,  0, r4, Agi1, r2,  3, r5, Ako0, r9, 12, r6, Amu1, lr,  4
		ldr			r8, [sp, #mDa0]
		KeccakThetaRhoPiChi r4, Aga0, r8, 18, r5, Ake0, r10,  5, r6, Ami1, r2,  8, r7, Aso0, r9, 28, r3, Abu1, lr, 14
		KeccakThetaRhoPiChi r6, Ama0, r8, 20, r7, Ase1, r11,  1, r3, Abi1, r2, 31, r4, Ago0, r9, 27, r5, Aku0, r12, 19
		ldr			r9, [sp, #mDo1]
		KeccakThetaRhoPiChiIota  Aba0, r8,          Age0, r10, 22,      Aki1, r2, 22,      Amo1, r9, 11,      Asu0, r12,  7, 0, 0

		ldr			r2, [sp, #mDi0]
		KeccakThetaRhoPiChi r5, Aka0, r8,  1, r6, Ame0, r10, 22, r7, Asi0, r2, 30, r3, Abo1, r9, 14, r4, Agu1, lr, 10
		KeccakThetaRhoPiChi r7, Asa0, r8,  9, r3, Abe1, r11,  1, r4, Agi0, r2,  3, r5, Ako1, r9, 13, r6, Amu0, r12,  4
		ldr			r8, [sp, #mDa1]
		KeccakThetaRhoPiChi r4, Aga1, r8, 18, r5, Ake1, r11,  5, r6, Ami0, r2,  7, r7, Aso1, r9, 28, r3, Abu0, r12, 13
		KeccakThetaRhoPiChi r6, Ama1, r8, 21, r7, Ase0, r10,  1, r3, Abi0, r2, 31, r4, Ago1, r9, 28, r5, Aku1, lr, 20
		ldr			r9, [sp, #mDo0]
		KeccakThetaRhoPiChiIota  Aba1, r8,          Age1, r11, 22,      Aki0, r2, 21,      Amo0, r9, 10,      Asu1, lr,  7, 4, 0
		MEND

		MACRO
		KeccakRound1

        xor5        r3,  Asu0, Agu0, Amu0, Abu1, Aku1
        xor5        r7, Age1, Ame0, Abe0, Ake1, Ase1
        xorrol      r6, r3, r7
		str			r6, [sp, #mDa0]
        xor5        r6,  Asu1, Agu1, Amu1, Abu0, Aku0
        xor5        lr, Age0, Ame1, Abe1, Ake0, Ase0
        eors        r8, r6, lr
		str			r8, [sp, #mDa1]

        xor5        r5,  Aki1, Asi1, Agi0, Ami1, Abi0
        xorrol      r9, r5, r6
		str			r9, [sp, #mDo0]
        xor5        r4,  Aki0, Asi0, Agi1, Ami0, Abi1
		eors		r3, r3, r4
		str			r3, [sp, #mDo1]

        xor5        r3,  Aba0, Aka1, Asa0, Aga0, Ama1
        xorrol      r10, r3, r4
        xor5        r6,  Aba1, Aka0, Asa1, Aga1, Ama0
        eors        r11, r6, r5

        xor5        r4,  Amo0, Abo1, Ako0, Aso1, Ago0
        xorrol      r5, lr, r4
		str			r5, [sp, #mDi0]
        xor5        r5,  Amo1, Abo0, Ako1, Aso0, Ago1
        eors        r2, r7, r5

        xorrol      r12, r5, r6
        eors        lr, r4, r3

		KeccakThetaRhoPiChi r5, Asa1, r8,  2, r6, Ake1, r11, 23, r7, Abi1, r2, 31, r3, Amo1, r9, 14, r4, Agu0, r12, 10
		KeccakThetaRhoPiChi r7, Ama0, r8,  9, r3, Age0, r10,  0, r4, Asi0, r2,  3, r5, Ako1, r9, 12, r6, Abu0, lr,  4
		ldr			r8, [sp, #mDa0]
		KeccakThetaRhoPiChi r4, Aka1, r8, 18, r5, Abe1, r10,  5, r6, Ami0, r2,  8, r7, Ago1, r9, 28, r3, Asu1, lr, 14
		KeccakThetaRhoPiChi r6, Aga0, r8, 20, r7, Ase1, r11,  1, r3, Aki0, r2, 31, r4, Abo0, r9, 27, r5, Amu0, r12, 19
		ldr			r9, [sp, #mDo1]
		KeccakThetaRhoPiChiIota  Aba0, r8,          Ame1, r10, 22,      Agi1, r2, 22,      Aso1, r9, 11,      Aku1, r12,  7, 8, 0

		ldr			r2, [sp, #mDi0]
		KeccakThetaRhoPiChi r5, Asa0, r8,  1, r6, Ake0, r10, 22, r7, Abi0, r2, 30, r3, Amo0, r9, 14, r4, Agu1, lr, 10
		KeccakThetaRhoPiChi r7, Ama1, r8,  9, r3, Age1, r11,  1, r4, Asi1, r2,  3, r5, Ako0, r9, 13, r6, Abu1, r12,  4
		ldr			r8, [sp, #mDa1]
		KeccakThetaRhoPiChi r4, Aka0, r8, 18, r5, Abe0, r11,  5, r6, Ami1, r2,  7, r7, Ago0, r9, 28, r3, Asu0, r12, 13
		KeccakThetaRhoPiChi r6, Aga1, r8, 21, r7, Ase0, r10,  1, r3, Aki1, r2, 31, r4, Abo1, r9, 28, r5, Amu1, lr, 20
		ldr			r9, [sp, #mDo0]
		KeccakThetaRhoPiChiIota  Aba1, r8,          Ame0, r11, 22,      Agi0, r2, 21,      Aso0, r9, 10,      Aku0, lr,  7, 12, 0
		MEND

		MACRO
		KeccakRound2

        xor5        r3, Aku1, Agu0, Abu1, Asu1, Amu1
        xor5        r7, Ame0, Ake0, Age0, Abe0, Ase1
        xorrol      r6, r3, r7
		str			r6, [sp, #mDa0]
        xor5        r6,  Aku0, Agu1, Abu0, Asu0, Amu0
        xor5        lr, Ame1, Ake1, Age1, Abe1, Ase0
        eors        r8, r6, lr
		str			r8, [sp, #mDa1]

        xor5        r5,  Agi1, Abi1, Asi1, Ami0, Aki1
        xorrol      r9, r5, r6
		str			r9, [sp, #mDo0]
        xor5        r4,  Agi0, Abi0, Asi0, Ami1, Aki0
		eors		r3, r3, r4
		str			r3, [sp, #mDo1]

        xor5        r3,  Aba0, Asa1, Ama1, Aka1, Aga1
        xorrol      r10, r3, r4
        xor5        r6,  Aba1, Asa0, Ama0, Aka0, Aga0
        eors        r11, r6, r5

        xor5        r4,  Aso0, Amo0, Ako1, Ago0, Abo0
        xorrol      r5, lr, r4
		str			r5, [sp, #mDi0]
        xor5        r5,  Aso1, Amo1, Ako0, Ago1, Abo1
        eors        r2, r7, r5

        xorrol      r12, r5, r6
        eors        lr, r4, r3

		KeccakThetaRhoPiChi r5, Ama0, r8,  2, r6, Abe0, r11, 23, r7, Aki0, r2, 31, r3, Aso1, r9, 14, r4, Agu0, r12, 10
		KeccakThetaRhoPiChi r7, Aga0, r8,  9, r3, Ame1, r10,  0, r4, Abi0, r2,  3, r5, Ako0, r9, 12, r6, Asu0, lr,  4
		ldr			r8, [sp, #mDa0]
		KeccakThetaRhoPiChi r4, Asa1, r8, 18, r5, Age1, r10,  5, r6, Ami1, r2,  8, r7, Abo1, r9, 28, r3, Aku0, lr, 14
		KeccakThetaRhoPiChi r6, Aka1, r8, 20, r7, Ase1, r11,  1, r3, Agi0, r2, 31, r4, Amo1, r9, 27, r5, Abu1, r12, 19
		ldr			r9, [sp, #mDo1]
		KeccakThetaRhoPiChiIota  Aba0, r8,          Ake1, r10, 22,      Asi0, r2, 22,      Ago0, r9, 11,      Amu1, r12,  7, 16, 0

		ldr			r2, [sp, #mDi0]
		KeccakThetaRhoPiChi r5, Ama1, r8,  1, r6, Abe1, r10, 22, r7, Aki1, r2, 30, r3, Aso0, r9, 14, r4, Agu1, lr, 10
		KeccakThetaRhoPiChi r7, Aga1, r8,  9, r3, Ame0, r11,  1, r4, Abi1, r2,  3, r5, Ako1, r9, 13, r6, Asu1, r12,  4
		ldr			r8, [sp, #mDa1]
		KeccakThetaRhoPiChi r4, Asa0, r8, 18, r5, Age0, r11,  5, r6, Ami0, r2,  7, r7, Abo0, r9, 28, r3, Aku1, r12, 13
		KeccakThetaRhoPiChi r6, Aka0, r8, 21, r7, Ase0, r10,  1, r3, Agi1, r2, 31, r4, Amo0, r9, 28, r5, Abu0, lr, 20
		ldr			r9, [sp, #mDo0]
		KeccakThetaRhoPiChiIota  Aba1, r8,          Ake0, r11, 22,      Asi1, r2, 21,      Ago1, r9, 10,      Amu0, lr,  7, 20, 0
		MEND

		MACRO
		KeccakRound3

        xor5        r3,  Amu1, Agu0, Asu1, Aku0, Abu0
        xor5        r7, Ake0, Abe1, Ame1, Age0, Ase1
        xorrol      r6, r3, r7
		str			r6, [sp, #mDa0]
        xor5        r6,  Amu0, Agu1, Asu0, Aku1, Abu1
        xor5        lr, Ake1, Abe0, Ame0, Age1, Ase0
        eors        r8, r6, lr
		str			r8, [sp, #mDa1]

        xor5        r5,  Asi0, Aki0, Abi1, Ami1, Agi1
        xorrol      r9, r5, r6
		str			r9, [sp, #mDo0]
        xor5        r4,  Asi1, Aki1, Abi0, Ami0, Agi0
		eors		r3, r3, r4
		str			r3, [sp, #mDo1]

        xor5        r3,  Aba0, Ama0, Aga1, Asa1, Aka0
        xorrol      r10, r3, r4
        xor5        r6,  Aba1, Ama1, Aga0, Asa0, Aka1
        eors        r11, r6, r5

        xor5        r4,  Ago1, Aso0, Ako0, Abo0, Amo1
        xorrol      r5, lr, r4
		str			r5, [sp, #mDi0]
        xor5        r5,  Ago0, Aso1, Ako1, Abo1, Amo0
        eors        r2, r7, r5

        xorrol      r12, r5, r6
        eors        lr, r4, r3

		KeccakThetaRhoPiChi r5, Aga0, r8,  2, r6, Age0, r11, 23, r7, Agi0, r2, 31, r3, Ago0, r9, 14, r4, Agu0, r12, 10
		KeccakThetaRhoPiChi r7, Aka1, r8,  9, r3, Ake1, r10,  0, r4, Aki1, r2,  3, r5, Ako1, r9, 12, r6, Aku1, lr,  4
		ldr			r8, [sp, #mDa0]
		KeccakThetaRhoPiChi r4, Ama0, r8, 18, r5, Ame0, r10,  5, r6, Ami0, r2,  8, r7, Amo0, r9, 28, r3, Amu0, lr, 14
		KeccakThetaRhoPiChi r6, Asa1, r8, 20, r7, Ase1, r11,  1, r3, Asi1, r2, 31, r4, Aso1, r9, 27, r5, Asu1, r12, 19
		ldr			r9, [sp, #mDo1]
		KeccakThetaRhoPiChiIota  Aba0, r8,          Abe0, r10, 22,      Abi0, r2, 22,      Abo0, r9, 11,      Abu0, r12,  7, 24, 0

		ldr			r2, [sp, #mDi0]
		KeccakThetaRhoPiChi r5, Aga1, r8,  1, r6, Age1, r10, 22, r7, Agi1, r2, 30, r3, Ago1, r9, 14, r4, Agu1, lr, 10
		KeccakThetaRhoPiChi r7, Aka0, r8,  9, r3, Ake0, r11,  1, r4, Aki0, r2,  3, r5, Ako0, r9, 13, r6, Aku0, r12,  4
		ldr			r8, [sp, #mDa1]
		KeccakThetaRhoPiChi r4, Ama1, r8, 18, r5, Ame1, r11,  5, r6, Ami1, r2,  7, r7, Amo1, r9, 28, r3, Amu1, r12, 13
		KeccakThetaRhoPiChi r6, Asa0, r8, 21, r7, Ase0, r10,  1, r3, Asi0, r2, 31, r4, Aso0, r9, 28, r5, Asu0, lr, 20
		ldr			r9, [sp, #mDo0]
		KeccakThetaRhoPiChiIota  Aba1, r8,          Abe1, r11, 22,      Abi1, r2, 21,      Abo1, r9, 10,      Abu1, lr,  7, 28, 1
		MEND


;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_Initialize( void )
;//
KeccakF1600_Initialize   PROC

		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StateInitialize(void *state)
;//
KeccakF1600_StateInitialize   PROC

		push	{r4 - r5}

		movs	r1, #0
		movs	r2, #0
		movs	r3, #0
		movs	r4, #0
		movs	r5, #0

		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }

		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }
		stmia	r0!, { r1 - r5 }

		pop		{r4 - r5}
		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;//	void KeccakF1600_StateComplementBit(void *state, unsigned int position)
;//
KeccakF1600_StateComplementBit   PROC

		and		r3, r1, #1
		lsrs	r2, r1, #6
		add		r0, r0, r3, LSL #2
		add		r0, r0, r2, LSL #3

		movs	r2, #1
		ubfx	r3, r1, #1, #5
		lsls	r2, r2, r3

		ldr		r3, [r0]
		eors	r3, r3, r2
		str		r3, [r0]
		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
;// 
KeccakF1600_StateXORLanes   PROC

		cmp		r2, #0
		beq		KeccakF1600_StateXORLanes_Exit
		push	{r4 - r7}
KeccakF1600_StateXORLanes_LoopAligned
		ldr		r4, [r1], #4
		ldr		r5, [r1], #4
		ldrd    r6, r7, [r0]
		toBitInterleaving	r4, r5, r6, r7, r3
		strd	r6, r7, [r0], #8
		subs	r2, r2, #1
		bne		KeccakF1600_StateXORLanes_LoopAligned
		pop		{r4 - r7}
KeccakF1600_StateXORLanes_Exit
		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
;//
KeccakF1600_StateXORBytesInLane   PROC

		push	{r4 - r7}
		ldr		r7, [sp, #4*4]
		cmp		r7, #0
		beq		KeccakF1600_StateXORBytesInLane_Exit
	
		movs	r4, #0
		movs	r5, #0
		push	{ r4 - r5 }

		add		r3, r3, sp
KeccakF1600_StateXORBytesInLane_Loop
		ldrb	r5, [r2], #1
		strb	r5, [r3], #1
		subs	r7, r7, #1
		bne		KeccakF1600_StateXORBytesInLane_Loop

		pop		{ r4 - r5 }

		add		r0, r0, r1, LSL #3
		ldrd    r6, r7, [r0]
		toBitInterleaving	r4, r5, r6, r7, r3
		strd	r6, r7, [r0]

KeccakF1600_StateXORBytesInLane_Exit
		pop		{r4 - r7}
		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
;//
KeccakF1600_StateExtractLanes   PROC

		cmp		r2, #0
		beq		KeccakF1600_StateExtractLanes_Exit

		push	{r4 - r5}
KeccakF1600_StateExtractLanes_LoopAligned
		ldrd	r4, r5, [r0], #8
		fromBitInterleaving	r4, r5, r3
		str		r4, [r1], #4
		subs	r2, r2, #1
		str		r5, [r1], #4
		bne		KeccakF1600_StateExtractLanes_LoopAligned
		pop		{r4 - r5}

KeccakF1600_StateExtractLanes_Exit
		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
;//
KeccakF1600_StateExtractBytesInLane   PROC

		push	{r4 - r5}
		ldr		r5, [sp, #2*4]
		cmp		r5, #0
		beq		KeccakF1600_StateExtractBytesInLane_Exit

		add		r0, r0, r1, LSL #3
		ldr		r1, [r0, #4]
		ldr		r0, [r0]
		fromBitInterleaving	r0, r1, r4
		push	{r0, r1 }

		add		r0, sp, r3
KeccakF1600_StateExtractBytesInLane_Loop
		ldrb	r1, [r0], #1
		subs	r5, r5, #1
		strb	r1, [r2], #1
		bne		KeccakF1600_StateExtractBytesInLane_Loop
		add		sp, #8
KeccakF1600_StateExtractBytesInLane_Exit
		pop		{r4 - r5}
		bx		lr

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StatePermute( const void *state )
;//
KeccakF1600_StatePermute   PROC

		push	{ r4 - r12, lr }
		sub		sp, #mSize

		adr		r1, KeccakF1600_StatePermute_RoundConstantsWithTerminator
		str		r1, [sp, #mRC]

		b		KeccakF1600_StatePermute_RoundLoop

KeccakF1600_StatePermute_RoundConstantsWithTerminator
		;//		0			1
		dcd		0x00000001,	0x00000000
		dcd		0x00000000,	0x00000089
		dcd		0x00000000,	0x8000008b
		dcd		0x00000000,	0x80008080

		dcd		0x00000001,	0x0000008b
		dcd		0x00000001,	0x00008000
		dcd		0x00000001,	0x80008088
		dcd		0x00000001,	0x80000082

		dcd		0x00000000,	0x0000000b
		dcd		0x00000000,	0x0000000a
		dcd		0x00000001,	0x00008082
		dcd		0x00000000,	0x00008003

		dcd		0x00000001,	0x0000808b
		dcd		0x00000001,	0x8000000b
		dcd		0x00000001,	0x8000008a
		dcd		0x00000001,	0x80000081

		dcd		0x00000000,	0x80000081
		dcd		0x00000000,	0x80000008
		dcd		0x00000000,	0x00000083
		dcd		0x00000000,	0x80008003

		dcd		0x00000001,	0x80008088
		dcd		0x00000000,	0x80000088
		dcd		0x00000001,	0x00008000
		dcd		0x00000000,	0x80008082

		dcd		0x000000FF	;//terminator

KeccakF1600_StatePermute_RoundLoop
		KeccakRound0
		KeccakRound1
		KeccakRound2
		KeccakRound3
		bne		KeccakF1600_StatePermute_RoundLoop
		add		sp, #mSize
		pop		{ r4 - r12, pc }

		ENDP
		ALIGN

;//----------------------------------------------------------------------------
;//
;// void KeccakF1600_StateXORPermuteExtract(void *state, const unsigned char *inData, unsigned int inLaneCount, unsigned char *outData, unsigned int outLaneCount)
;//
KeccakF1600_StateXORPermuteExtract   PROC

		push	{ r4 - r6, lr }
		mov		r4, r0
		mov		r5, r3
		bl		KeccakF1600_StateXORLanes
		mov		r0, r4
		bl		KeccakF1600_StatePermute
		mov		r0, r4
		mov		r1, r5
		ldr		r2, [sp, #16]
		bl		KeccakF1600_StateExtractLanes
		pop		{ r4 - r6, pc }

		ENDP
		ALIGN

		END
