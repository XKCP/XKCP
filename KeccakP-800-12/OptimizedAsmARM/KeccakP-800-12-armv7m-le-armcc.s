;
; Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
; Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
; denoted as "the implementer".
;
; For more information, feedback or questions, please refer to our websites:
; http://keccak.noekeon.org/
; http://keyak.noekeon.org/
; http://ketje.noekeon.org/
;
; To the extent possible under law, the implementer has waived all copyright
; and related or neighboring rights to the source code in this file.
; http://creativecommons.org/publicdomain/zero/1.0/
;

; WARNING: These functions work only on little endian CPU with ARMv7m architecture (Cortex-M3, ...).

		PRESERVE8
		THUMB
		AREA    |.text|, CODE, READONLY

;//----------------------------------------------------------------------------

_ba		equ  0*4
_be		equ  1*4
_bi		equ  2*4
_bo		equ  3*4
_bu		equ  4*4
_ga		equ  5*4
_ge		equ  6*4
_gi		equ  7*4
_go		equ  8*4
_gu		equ  9*4
_ka		equ 10*4
_ke		equ 11*4
_ki		equ 12*4
_ko		equ 13*4
_ku		equ 14*4
_ma		equ 15*4
_me		equ 16*4
_mi		equ 17*4
_mo		equ 18*4
_mu		equ 19*4
_sa		equ 20*4
_se		equ 21*4
_si		equ 22*4
_so		equ 23*4
_su		equ 24*4

;//----------------------------------------------------------------------------

		MACRO
		KeccakRound		$stateOut,$stateIn,$term

		; prepare Theta
	    xor5		r1, $stateIn, _ba, _ga, _ka, _ma, r9
	    xor5		r2, $stateIn, _be, _ge, _ke, _me, r10
	    xor5		r3, $stateIn, _bi, _gi, _ki, _mi, r11
	    xor5		r4, $stateIn, _bo, _go, _ko, _mo, r12
		eor			r9, r7, r2, ROR #31
	    eor			r10, r1, r3, ROR #31
	    eor			r11, r2, r4, ROR #31
	    eor			r12, r3, r7, ROR #31
	    eor			lr, r4, r1, ROR #31

		; Theta Rho Pi Chi Iota
		ldr			r1, [$stateIn, #_ba]
		ldr			r2, [$stateIn, #_ge]
		ldr			r3, [$stateIn, #_ki]
		ldr			r4, [$stateIn, #_mo]
		eor			r1, r1, r9
		xorrol 		r2, r10, 12
		xorrol 		r3, r11, 11
		xorrol 		r4, r12, 21
		xorrol 		r5, lr, 14
		xandnot		$stateOut, _be, r2, r3, r4, r6
		xandnot		$stateOut, _bi, r3, r4, r5, r6
		xandnot		$stateOut, _bo, r4, r5, r1, r6
		xandnot		$stateOut, _bu, r5, r1, r2, r7
		xandnotRC	$stateOut, _ba, r1, r2, r3

		ldr			r1, [$stateIn, #_bo]
		ldr			r2, [$stateIn, #_gu]
		ldr			r3, [$stateIn, #_ka]
		ldr			r4, [$stateIn, #_me]
		ldr			r5, [$stateIn, #_si]
	    xorrol 		r1, r12, 28
	    xorrol 		r2, lr, 20
	    xorrol 		r3, r9,  3
	    xorrol 		r4, r10, 13
	    xorrol 		r5, r11, 29
		xandnot		$stateOut, _ga, r1, r2, r3, r6
		xandnot		$stateOut, _ge, r2, r3, r4, r6
		xandnot		$stateOut, _gi, r3, r4, r5, r6
		xandnot		$stateOut, _go, r4, r5, r1, r6
		xandnot		$stateOut, _gu, r5, r1, r2, r6
		eor			r7, r7, r6

		ldr			r1, [$stateIn, #_be]
		ldr			r2, [$stateIn, #_gi]
		ldr			r3, [$stateIn, #_ko]
		ldr			r4, [$stateIn, #_mu]
		ldr			r5, [$stateIn, #_sa]
	    xorrol 		r1, r10,  1
	    xorrol 		r2, r11,  6
	    xorrol 		r3, r12, 25
	    xorrol 		r4, lr,  8
	    xorrol 		r5, r9, 18
		xandnot		$stateOut, _ka, r1, r2, r3, r6
		xandnot		$stateOut, _ke, r2, r3, r4, r6
		xandnot		$stateOut, _ki, r3, r4, r5, r6
		xandnot		$stateOut, _ko, r4, r5, r1, r6
		xandnot		$stateOut, _ku, r5, r1, r2, r6
		eor			r7, r7, r6

		ldr			r1, [$stateIn, #_bu]
		ldr			r2, [$stateIn, #_ga]
		ldr			r3, [$stateIn, #_ke]
		ldr			r4, [$stateIn, #_mi]
		ldr			r5, [$stateIn, #_so]
	    xorrol 		r1, lr, 27
	    xorrol 		r2, r9,  4
	    xorrol 		r3, r10, 10
	    xorrol 		r4, r11, 15
	    xorrol 		r5, r12, 24
		xandnot		$stateOut, _ma, r1, r2, r3, r6
		xandnot		$stateOut, _me, r2, r3, r4, r6
		xandnot		$stateOut, _mi, r3, r4, r5, r6
		xandnot		$stateOut, _mo, r4, r5, r1, r6
		xandnot		$stateOut, _mu, r5, r1, r2, r6
		eor			r7, r7, r6

		ldr			r1, [$stateIn, #_bi]
		ldr			r2, [$stateIn, #_go]
		ldr			r3, [$stateIn, #_ku]
		ldr			r4, [$stateIn, #_ma]
		ldr			r5, [$stateIn, #_se]
	    xorrol 		r1, r11, 30
	    xorrol 		r2, r12, 23
	    xorrol 		r3, lr,  7
	    xorrol 		r4, r9,  9
	    xorrol 		r5, r10,  2
		xandnot		$stateOut, _sa, r1, r2, r3, r9
		xandnot		$stateOut, _se, r2, r3, r4, r10
		xandnot		$stateOut, _si, r3, r4, r5, r11
		xandnot		$stateOut, _so, r4, r5, r1, r12
		bic			r1, r2, r1
		if			$term != 0
		ldr			r3, [r8]
		endif
		eor			r5, r5, r1
		eor			r7, r7, r5
		str			r5, [$stateOut, #_su]
		MEND

		MACRO
		xor5		$result,$ptr,$b,$g,$k,$m,$rs

		ldr			$result, [$ptr, #$b]
		ldr			r6, [$ptr, #$g]
		eor			$result, $result, $rs
		ldr			$rs, [$ptr, #$k]
		eor			$result, $result, r6				
		ldr			r6, [$ptr, #$m]
		eor			$result, $result, $rs
		eor			$result, $result, r6				
		MEND

		MACRO
		xorrol 		$b, $yy, $rr

		eor			$b, $b, $yy
		ror			$b, #32-$rr
		MEND


		MACRO
		xandnot 	$resptr, $resofs, $aa, $bb, $cc, $temp

		bic			$temp, $cc, $bb
		eor			$temp, $temp, $aa
		str			$temp, [$resptr, #$resofs]
		MEND

		MACRO
		xandnotRC 	$resptr, $resofs, $aa, $bb, $cc

		ldr			r6, [r8], #4
		bic			$cc, $cc, $bb
		eor			$cc, $cc, r6
		eor			$cc, $cc, $aa
		str			$cc, [$resptr, #$resofs]
		MEND


;//----------------------------------------------------------------------------
;//
;// void KeccakF800_Initialize( void )
;//
		ALIGN
		EXPORT  KeccakF800_Initialize
KeccakF800_Initialize   PROC
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateInitialize(void *state)
;//
		ALIGN
		EXPORT  KeccakF800_StateInitialize
KeccakF800_StateInitialize   PROC
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
		pop		{r4 - r5}
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;//	void KeccakF800_StateComplementBit(void *state, unsigned int position)
;//
		ALIGN
		EXPORT  KeccakF800_StateComplementBit
KeccakF800_StateComplementBit   PROC
		lsrs	r2, r1, #5
		add		r0, r2, LSL #2
		ldr		r2, [r0]
		and		r1, r1, #0x1F
		movs	r3, #1
		lsls	r3, r3, r1
		eors	r3, r3, r2
		str		r3, [r0]
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
;// 
		ALIGN
		EXPORT  KeccakF800_StateXORLanes
KeccakF800_StateXORLanes   PROC
		subs	r2, r2, #4
		bcc		KeccakF800_StateXORLanes_LessThan4Lanes
		push	{ r4 - r5 }
KeccakF800_StateXORLanes_Loop4Lanes
		ldrd	r4, r5, [r0]
		ldr		r3, [r1], #4
		ldr		r12, [r1], #4
		eor		r4, r4, r3
		eor		r5, r5, r12
		strd	r4, r5, [r0], #8
		ldrd	r4, r5, [r0]
		ldr		r3, [r1], #4
		ldr		r12, [r1], #4
		eor		r4, r4, r3
		eor		r5, r5, r12
		strd	r4, r5, [r0], #8
		subs	r2, r2, #4
		bcs		KeccakF800_StateXORLanes_Loop4Lanes
		pop		{ r4 - r5 }
KeccakF800_StateXORLanes_LessThan4Lanes
		adds	r2, r2, #4
		beq		KeccakF800_StateXORLanes_Exit
KeccakF800_StateXORLanes_LoopLane
		ldr		r3, [r1], #4
		ldr		r12, [r0]
		eor		r3, r3, r12
		str		r3, [r0], #4
		subs	r2, r2, #1
		bne		KeccakF800_StateXORLanes_LoopLane
KeccakF800_StateXORLanes_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
;//
		ALIGN
		EXPORT  KeccakF800_StateXORBytesInLane
KeccakF800_StateXORBytesInLane   PROC
		adds	r0, r0, r3
		ldr		r3, [sp]
		cbz		r3, KeccakF800_StateXORBytesInLane_Exit
		adds	r0, r0, r1, LSL #2
KeccakF800_StateXORBytesInLane_Loop
		ldrb	r1, [r0]
		ldrb	r12, [r2], #1
		eors	r1, r1, r12
		strb	r1, [r0], #1
		subs	r3, r3, #1
		bne		KeccakF800_StateXORBytesInLane_Loop
KeccakF800_StateXORBytesInLane_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
;//
		ALIGN
		EXPORT  KeccakF800_StateOverwriteLanes
KeccakF800_StateOverwriteLanes	PROC 
		subs	r2, r2, #8
		bcc		KeccakF800_StateOverwriteLanes_LessThan8Lanes
		push	{ r4 - r5 }
KeccakF800_StateOverwriteLanes_Loop8Lanes
		ldr		r3, [r1], #4
		ldr		r4, [r1], #4
		ldr		r5, [r1], #4
		ldr		r12, [r1], #4
		stm		r0!, { r3 - r5, r12 }
		ldr		r3, [r1], #4
		ldr		r4, [r1], #4
		ldr		r5, [r1], #4
		ldr		r12, [r1], #4
		stm		r0!, { r3 - r5, r12 }
		subs	r2, r2, #8
		bcs		KeccakF800_StateOverwriteLanes_Loop8Lanes
		pop		{ r4 - r5 }
KeccakF800_StateOverwriteLanes_LessThan8Lanes
		add		r2, r2, #8
		subs	r2, r2, #2
		bcc		KeccakF800_StateOverwriteLanes_LessThan2Lanes
KeccakF800_StateOverwriteLanes_Loop2Lanes
		ldr		r3, [r1], #4
		ldr		r12, [r1], #4
		stm		r0!, { r3, r12 }
		subs	r2, r2, #2
		bcs		KeccakF800_StateOverwriteLanes_Loop2Lanes
KeccakF800_StateOverwriteLanes_LessThan2Lanes
		adds	r2, r2, #2
		beq		KeccakF800_StateOverwriteLanes_Exit
		ldr		r3, [r1]
		str		r3, [r0]
KeccakF800_StateOverwriteLanes_Exit
		bx		lr
		ENDP


;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateOverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
;//
		ALIGN
		EXPORT  KeccakF800_StateOverwriteBytesInLane
KeccakF800_StateOverwriteBytesInLane	PROC
		adds	r0, r0, r3
		ldr		r3, [sp]
		cbz		r3, KeccakF800_StateOverwriteBytesInLane_Exit
		adds	r0, r0, r1, LSL #2
KeccakF800_StateOverwriteBytesInLane_Loop
		ldrb	r1, [r2], #1
		strb	r1, [r0], #1
		subs	r3, r3, #1
		bne		KeccakF800_StateOverwriteBytesInLane_Loop
KeccakF800_StateOverwriteBytesInLane_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
;//
		ALIGN
		EXPORT  KeccakF800_StateOverwriteWithZeroes
KeccakF800_StateOverwriteWithZeroes	PROC 
		movs	r3, #0
		lsrs	r2, r1, #2
		beq		KeccakF800_StateOverwriteWithZeroes_Bytes
KeccakF800_StateOverwriteWithZeroes_LoopLanes
		str		r3, [r0], #4
		subs	r2, r2, #1
		bne		KeccakF800_StateOverwriteWithZeroes_LoopLanes
KeccakF800_StateOverwriteWithZeroes_Bytes
		ands	r1, #3
		beq		KeccakF800_StateOverwriteWithZeroes_Exit
KeccakF800_StateOverwriteWithZeroes_LoopBytes
		strb	r3, [r0], #1
		subs	r1, r1, #1
		bne		KeccakF800_StateOverwriteWithZeroes_LoopBytes
KeccakF800_StateOverwriteWithZeroes_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
;//
		ALIGN
		EXPORT  KeccakF800_StateExtractLanes
KeccakF800_StateExtractLanes   PROC
		subs	r2, r2, #8
		bcc		KeccakF800_StateExtractLanes_LessThan8Lanes
		push	{ r4 - r5 }
KeccakF800_StateExtractLanes_Loop8Lanes
		ldm		r0!, { r3 - r5, r12 }
		str		r3, [r1], #4
		str		r4, [r1], #4
		str		r5, [r1], #4
		str		r12, [r1], #4
		ldm		r0!, { r3 - r5, r12 }
		str		r3, [r1], #4
		str		r4, [r1], #4
		str		r5, [r1], #4
		str		r12, [r1], #4
		subs	r2, r2, #8
		bcs		KeccakF800_StateExtractLanes_Loop8Lanes
		pop		{ r4 - r5 }
KeccakF800_StateExtractLanes_LessThan8Lanes
		add		r2, r2, #8
		subs	r2, r2, #2
		bcc		KeccakF800_StateExtractLanes_LessThan2Lanes
KeccakF800_StateExtractLanes_Loop2Lanes
		ldm		r0!, { r3, r12 }
		str		r3, [r1], #4
		str		r12, [r1], #4
		subs	r2, r2, #2
		bcs		KeccakF800_StateExtractLanes_Loop2Lanes
KeccakF800_StateExtractLanes_LessThan2Lanes
		adds	r2, r2, #2
		beq		KeccakF800_StateExtractLanes_Exit
		ldr		r3, [r0], #4
		str		r3, [r1], #4
KeccakF800_StateExtractLanes_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
;//
		ALIGN
		EXPORT  KeccakF800_StateExtractBytesInLane
KeccakF800_StateExtractBytesInLane   PROC
		ldr		r12, [sp]
		cmp		r12, #0
		beq		KeccakF800_StateExtractBytesInLane_Exit
		adds	r0, r0, r1, LSL #2
		adds	r0, r0, r3
KeccakF800_StateExtractBytesInLane_Loop
		ldrb	r1, [r0], #1
		strb	r1, [r2], #1
		subs	r12, r12, #1
		bne		KeccakF800_StateExtractBytesInLane_Loop
KeccakF800_StateExtractBytesInLane_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
;//
		ALIGN
		EXPORT  KeccakF800_StateExtractAndXORLanes
KeccakF800_StateExtractAndXORLanes	PROC
		subs	r2, r2, #4
		bcc		KeccakF800_StateExtractAndXORLanes_LessThan4Lanes
		push	{ r4 - r5 }
KeccakF800_StateExtractAndXORLanes_Loop4Lanes
		ldrd	r4, r5, [r0], #8
		ldr		r3, [r1]
		ldr		r12, [r1, #4]
		eor		r4, r4, r3
		eor		r5, r5, r12
		str		r4, [r1], #4
		str		r5, [r1], #4
		ldrd	r4, r5, [r0], #8
		ldr		r3, [r1]
		ldr		r12, [r1, #4]
		eor		r4, r4, r3
		eor		r5, r5, r12
		str		r4, [r1], #4
		str		r5, [r1], #4
		subs	r2, r2, #4
		bcs		KeccakF800_StateExtractAndXORLanes_Loop4Lanes
		pop		{ r4 - r5 }
KeccakF800_StateExtractAndXORLanes_LessThan4Lanes
		adds	r2, r2, #4
		beq		KeccakF800_StateExtractAndXORLanes_Exit
KeccakF800_StateExtractAndXORLanes_LoopLane
		ldr		r3, [r1]
		ldr		r12, [r0], #4
		eor		r3, r3, r12
		str		r3, [r1], #4
		subs	r2, r2, #1
		bne		KeccakF800_StateExtractAndXORLanes_LoopLane
KeccakF800_StateExtractAndXORLanes_Exit
		bx		lr
		ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF800_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
;//
		ALIGN
		EXPORT  KeccakF800_StateExtractAndXORBytesInLane
KeccakF800_StateExtractAndXORBytesInLane	PROC
		ldr		r12, [sp]
		cmp		r12, #0
		beq		KeccakF800_StateExtractAndXORBytesInLane_Exit
		adds	r0, r0, r1, LSL #2
		adds	r0, r0, r3
KeccakF800_StateExtractAndXORBytesInLane_Loop
		ldrb	r1, [r0], #1
		ldrb	r3, [r2]
		eors	r1, r1, r3
		strb	r1, [r2], #1
		subs	r12, r12, #1
		bne		KeccakF800_StateExtractAndXORBytesInLane_Loop
KeccakF800_StateExtractAndXORBytesInLane_Exit
		bx		lr
		ENDP

		ALIGN
KeccakP800_12_StatePermute_RoundConstantsWithTerminator
		dcd			0x80008009
		dcd			0x8000000a
		dcd			0x8000808b
		dcd			0x0000008b
		dcd			0x00008089
		dcd			0x00008003
		dcd			0x00008002
		dcd			0x00000080
		dcd			0x0000800a
		dcd			0x8000000a
		dcd			0x80008081
		dcd			0x00008080 
		dcd			0			;//terminator

;//----------------------------------------------------------------------------
;//
;// void KeccakP800_12_StatePermute( void *state )
;//
		ALIGN
		EXPORT  KeccakP800_12_StatePermute
KeccakP800_12_StatePermute   PROC
		push		{r4-r12,lr}
		sub			sp, sp, #4*25
		adr			r8, KeccakP800_12_StatePermute_RoundConstantsWithTerminator
	    ldr			r9, [r0, #_sa] 
	    ldr			r10, [r0, #_se] 
	    ldr			r11, [r0, #_si] 
	    ldr			lr, [r0, #_su] 
	    ldr			r12, [r0, #_so] 
		mov			r5, lr
	    xor5		r7, r0, _bu, _gu, _ku, _mu, lr
KeccakP800_StatePermute_RoundLoop
		KeccakRound	sp, r0, 0
		KeccakRound	r0, sp, 1
		cmp			r3, #0
		bne			KeccakP800_StatePermute_RoundLoop
		add			sp,sp,#4*25
		pop			{r4-r12,pc}
		ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakP800_12_SnP_FBWL_Absorb(	void *state, unsigned int laneCount, unsigned char *data, 
;										size_t dataByteLen, unsigned char trailingBits )
;
	ALIGN
	EXPORT	KeccakP800_12_SnP_FBWL_Absorb
KeccakP800_12_SnP_FBWL_Absorb	PROC
	push	{r4-r12,lr}
	mov		r4, #0
	lsr		r3, r3, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Absorb_Exit
KeccakP800_12_SnP_FBWL_Absorb_Loop
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r1, #4
	bcc		KeccakP800_12_SnP_FBWL_Absorb_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Absorb_Loop4Lanes
	ldrd	r10, r11, [r0]
	ldr		r9, [r2], #4
	ldr		r12, [r2], #4
	eor		r10, r10, r9
	eor		r11, r11, r12
	strd	r10, r11, [r0], #8
	ldrd	r10, r11, [r0]
	ldr		r9, [r2], #4
	ldr		r12, [r2], #4
	eor		r10, r10, r9
	eor		r11, r11, r12
	strd	r10, r11, [r0], #8
	subs	r6, r6, #4
	bcs		KeccakP800_12_SnP_FBWL_Absorb_Loop4Lanes
KeccakP800_12_SnP_FBWL_Absorb_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Absorb_TrailingBits
KeccakP800_12_SnP_FBWL_Absorb_LoopLane
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r11, r11, r12
	str		r11, [r0], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Absorb_LoopLane
KeccakP800_12_SnP_FBWL_Absorb_TrailingBits
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+0)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4}
	bl		KeccakP800_12_StatePermute
	pop	{r1-r4}
	subs	r3, r3, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Absorb_Loop
KeccakP800_12_SnP_FBWL_Absorb_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakP800_12_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
;
	ALIGN
	EXPORT	KeccakP800_12_SnP_FBWL_Squeeze
KeccakP800_12_SnP_FBWL_Squeeze	PROC
	push	{r4-r12,lr}
	mov		r4, #0
	lsr		r3, r3, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Squeeze_Exit
KeccakP800_12_SnP_FBWL_Squeeze_Loop
	push	{r1-r4}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r4}
	mov		r6, r1
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r6, #4
	bcc		KeccakP800_12_SnP_FBWL_Squeeze_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Squeeze_Loop4Lanes
	ldrd	r10, r11, [r0], #8
	str		r10, [r2], #4
	str		r11, [r2], #4
	ldrd	r10, r11, [r0], #8
	str		r10, [r2], #4
	str		r11, [r2], #4
	subs	r6, r6, #4
	bcs		KeccakP800_12_SnP_FBWL_Squeeze_Loop4Lanes
KeccakP800_12_SnP_FBWL_Squeeze_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop
KeccakP800_12_SnP_FBWL_Squeeze_LoopLane
	ldr		r11, [r0], #4
	str		r11, [r2], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Squeeze_LoopLane
KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop
	sub		r0, r0, r1, LSL #2
	subs	r3, r3, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Squeeze_Loop
KeccakP800_12_SnP_FBWL_Squeeze_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakP800_12_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
;										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
;
	ALIGN
	EXPORT	KeccakP800_12_SnP_FBWL_Wrap
KeccakP800_12_SnP_FBWL_Wrap	PROC
	push	{r4-r12,lr}
	ldr		r8, [sp, #(10+0)*4]			; dataByteLen
	mov		r4, #0
	lsr		r8, r8, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Wrap_Exit
KeccakP800_12_SnP_FBWL_Wrap_Loop
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r1, #4
	bcc		KeccakP800_12_SnP_FBWL_Wrap_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Wrap_Loop4Lanes
	ldrd	r10, r11, [r0]
	ldr		r9, [r2], #4
	ldr		r12, [r2], #4
	eor		r10, r10, r9
	eor		r11, r11, r12
	str		r10, [r3], #4
	str		r11, [r3], #4
	strd	r10, r11, [r0], #8
	ldrd	r10, r11, [r0]
	ldr		r9, [r2], #4
	ldr		r12, [r2], #4
	eor		r10, r10, r9
	eor		r11, r11, r12
	str		r10, [r3], #4
	str		r11, [r3], #4
	strd	r10, r11, [r0], #8
	subs	r6, r6, #4
	bcs		KeccakP800_12_SnP_FBWL_Wrap_Loop4Lanes
KeccakP800_12_SnP_FBWL_Wrap_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Wrap_TrailingBits
KeccakP800_12_SnP_FBWL_Wrap_LoopLane
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r11, r11, r12
	str		r11, [r0], #4
	str		r11, [r3], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Wrap_LoopLane
KeccakP800_12_SnP_FBWL_Wrap_TrailingBits
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+1)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4,r8-r9}
	bl		KeccakP800_12_StatePermute
	pop	{r1-r4,r8-r9}
	subs	r8, r8, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Wrap_Loop
KeccakP800_12_SnP_FBWL_Wrap_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakP800_12_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
;										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
;
	ALIGN
	EXPORT	KeccakP800_12_SnP_FBWL_Unwrap
KeccakP800_12_SnP_FBWL_Unwrap	PROC
	push	{r4-r12,lr}
	ldr		r8, [sp, #(10+0)*4]				; dataByteLen
	mov		r4, #0
	lsr		r8, r8, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Unwrap_Exit
KeccakP800_12_SnP_FBWL_Unwrap_Loop
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r1, #4
	bcc		KeccakP800_12_SnP_FBWL_Unwrap_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Unwrap_Loop4Lanes
	ldrd	r10, r11, [r0]
	ldr		r9, [r2], #4
	ldr		r12, [r2], #4
	eor		r10, r10, r9
	eor		r11, r11, r12
	str		r10, [r3], #4
	str		r11, [r3], #4
	stm		r0!, { r9, r12 }
	ldrd	r10, r11, [r0]
	ldr		r9, [r2], #4
	ldr		r12, [r2], #4
	eor		r10, r10, r9
	eor		r11, r11, r12
	str		r10, [r3], #4
	str		r11, [r3], #4
	stm		r0!, { r9, r12 }
	subs	r6, r6, #4
	bcs		KeccakP800_12_SnP_FBWL_Unwrap_Loop4Lanes
KeccakP800_12_SnP_FBWL_Unwrap_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits
KeccakP800_12_SnP_FBWL_Unwrap_LoopLane
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r12, r12, r11
	str		r12, [r3], #4
	str		r11, [r0], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Unwrap_LoopLane
KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+1)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4,r8-r9}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r4,r8-r9}
	subs	r8, r8, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Unwrap_Loop
KeccakP800_12_SnP_FBWL_Unwrap_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

	END
