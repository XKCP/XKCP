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

_ba		equ  0*2
_be		equ  1*2
_bi		equ  2*2
_bo		equ  3*2
_bu		equ  4*2
_ga		equ  5*2
_ge		equ  6*2
_gi		equ  7*2
_go		equ  8*2
_gu		equ  9*2
_ka		equ 10*2
_ke		equ 11*2
_ki		equ 12*2
_ko		equ 13*2
_ku		equ 14*2
_ma		equ 15*2
_me		equ 16*2
_mi		equ 17*2
_mo		equ 18*2
_mu		equ 19*2
_sa		equ 20*2
_se		equ 21*2
_si		equ 22*2
_so		equ 23*2
_su		equ 24*2

	MACRO
	xor5		$result,$ptr,$b,$g,$k,$m,$rs
	ldrh		$result, [$ptr, #$b]
	ldrh		r6, [$ptr, #$g]
	eor			$result, $result, $rs
	ldrh		$rs, [$ptr, #$k]
	eor			$result, $result, r6				
	ldrh		r6, [$ptr, #$m]
	eor			$result, $result, $rs
	eor			$result, $result, r6				
	MEND

	MACRO
	xor5D		$resultL,$resultH,$ptr,$b,$g,$k,$m,$rsL,$rsH
	bfi			$rsL, $rsH, #16, #16
	ldr			$resultL, [$ptr, #$b]
	ldr			r6, [$ptr, #$g]
	eor			$resultL, $resultL, $rsL
	ldr			$rsL, [$ptr, #$k]
	eor			$resultL, $resultL, r6				
	ldr			r6, [$ptr, #$m]
	eor			$resultL, $resultL, $rsL
	eor			$resultL, $resultL, r6				
	lsr			$resultH, $resultL, #16
	uxth 		$resultL, $resultL
	MEND

	MACRO
	xorrol 		$b, $yy, $rr
	eor			$b, $b, $yy
	if $rr != 8
	lsl			$b, $b, #$rr
	orr			$b, $b, $b, LSR #16
	else
	rev16		$b, $b
	endif
	MEND

	MACRO
	rolxor 		$d, $a, $b
	eor			$d, $a, $b, LSL #1
	eor			$d, $d, $b, LSR #15
	uxth		$d
	MEND

	MACRO
	xandnot 	$resptr, $resofs, $aa, $bb, $cc, $temp
	bic			$temp, $cc, $bb
	eor			$temp, $temp, $aa
	strh		$temp, [$resptr, #$resofs]
	MEND

	MACRO
	xandnotRC 	$resptr, $resofs, $aa, $bb, $cc
	ldrh		r6, [r8], #2
	bic			$cc, $cc, $bb
	eor			$cc, $cc, r6
	eor			$cc, $cc, $aa
	strh		$cc, [$resptr, #$resofs]
	MEND

	MACRO
	KeccakRound 	$sOut, $sIn
	;//prepTheta
    xor5D		r1, r2, $sIn, _ba, _ga, _ka, _ma, r9, r10
    xor5D		r3, r4, $sIn, _bi, _gi, _ki, _mi, r11, r12
	rolxor		r9, r7, r2
    rolxor		r10, r1, r3
    rolxor		r11, r2, r4
    rolxor		r12, r3, r7
    rolxor		lr, r4, r1

	;//thetaRhoPiChiIota
	ldrh		r1, [$sIn, #_ba]
	ldrh		r2, [$sIn, #_ge]
	ldrh		r3, [$sIn, #_ki]
	ldrh		r4, [$sIn, #_mo]
	eor			r1, r1, r9
	xorrol 		r2, r10, 12
	xorrol 		r3, r11, 11
	xorrol 		r4, r12,  5
	xorrol 		r5, lr, 14
	xandnot		$sOut, _be, r2, r3, r4, r6
	xandnot		$sOut, _bi, r3, r4, r5, r6
	xandnot		$sOut, _bo, r4, r5, r1, r6
	xandnot		$sOut, _bu, r5, r1, r2, r7
	xandnotRC	$sOut, _ba, r1, r2, r3

	ldrh		r1, [$sIn, #_bo]
	ldrh		r2, [$sIn, #_gu]
	ldrh		r3, [$sIn, #_ka]
	ldrh		r4, [$sIn, #_me]
	ldrh		r5, [$sIn, #_si]
    xorrol 		r1, r12, 12
    xorrol 		r2, lr,  4
    xorrol 		r3, r9,  3
    xorrol 		r4, r10, 13
    xorrol 		r5, r11, 13
	xandnot		$sOut, _ga, r1, r2, r3, r6
	xandnot		$sOut, _ge, r2, r3, r4, r6
	xandnot		$sOut, _gi, r3, r4, r5, r6
	xandnot		$sOut, _go, r4, r5, r1, r6
	xandnot		$sOut, _gu, r5, r1, r2, r6
	eor			r7, r7, r6

	ldrh		r1, [$sIn, #_be]
	ldrh		r2, [$sIn, #_gi]
	ldrh		r3, [$sIn, #_ko]
	ldrh		r4, [$sIn, #_mu]
	ldrh		r5, [$sIn, #_sa]
    xorrol 		r1, r10,  1
    xorrol 		r2, r11,  6
    xorrol 		r3, r12,  9
    xorrol 		r4, lr,  8
    xorrol 		r5, r9,  2
	xandnot		$sOut, _ka, r1, r2, r3, r6
	xandnot		$sOut, _ke, r2, r3, r4, r6
	xandnot		$sOut, _ki, r3, r4, r5, r6
	xandnot		$sOut, _ko, r4, r5, r1, r6
	xandnot		$sOut, _ku, r5, r1, r2, r6
	eor			r7, r7, r6

	ldrh		r1, [$sIn, #_bu]
	ldrh		r2, [$sIn, #_ga]
	ldrh		r3, [$sIn, #_ke]
	ldrh		r4, [$sIn, #_mi]
	ldrh		r5, [$sIn, #_so]
    xorrol 		r1, lr, 11
    xorrol 		r2, r9,  4
    xorrol 		r3, r10, 10
    xorrol 		r4, r11, 15
    xorrol 		r5, r12,  8
	xandnot		$sOut, _ma, r1, r2, r3, r6
	xandnot		$sOut, _me, r2, r3, r4, r6
	xandnot		$sOut, _mi, r3, r4, r5, r6
	xandnot		$sOut, _mo, r4, r5, r1, r6
	xandnot		$sOut, _mu, r5, r1, r2, r6
	eor			r7, r7, r6

	ldrh		r1, [$sIn, #_bi]
	ldrh		r2, [$sIn, #_go]
	ldrh		r3, [$sIn, #_ku]
	ldrh		r4, [$sIn, #_ma]
	ldrh		r5, [$sIn, #_se]
    xorrol 		r1, r11, 14
    xorrol 		r2, r12,  7
    xorrol 		r3, lr,  7
    xorrol 		r4, r9,  9
    xorrol 		r5, r10,  2
	xandnot		$sOut, _sa, r1, r2, r3, r9
	xandnot		$sOut, _se, r2, r3, r4, r10
	xandnot		$sOut, _si, r3, r4, r5, r11
	xandnot		$sOut, _so, r4, r5, r1, r12
	bic			r1, r2, r1
	eor			r5, r5, r1
	eor			r7, r7, r5
	uxth		r7, r7
	strh		r5, [$sOut, #_su]
	uxth		r5, r5
	MEND

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_Initialize( void )
;//
	ALIGN
	EXPORT  KeccakF400_Initialize
KeccakF400_Initialize   PROC
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateInitialize(void *state)
;//
	ALIGN
	EXPORT  KeccakF400_StateInitialize
KeccakF400_StateInitialize   PROC
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	stmia	r0!, { r1 - r3 }
	stmia	r0!, { r1 - r3 }
	stmia	r0!, { r1 - r3 }
	stmia	r0!, { r1 - r3 }
	strh	r1, [r0]
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;//	void KeccakF400_StateComplementBit(void *state, unsigned int position)
;//
	ALIGN
	EXPORT  KeccakF400_StateComplementBit
KeccakF400_StateComplementBit   PROC
	add		r0, r1, LSR #3
	ldrb	r2, [r0]
	and		r1, r1, #7
	movs	r3, #1
	lsls	r3, r3, r1
	eors	r3, r3, r2
	strb	r3, [r0]
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
;// 
	ALIGN
	EXPORT  KeccakF400_StateXORLanes
KeccakF400_StateXORLanes   PROC
	cbz		r2, KeccakF400_StateXORLanes_Exit
	subs	r3, r2, #2
	bcc		KeccakF400_StateXORLanes_1Lane
KeccakF400_StateXORLanes_Loop32
	ldr		r2, [r1], #4
	ldr		r12, [r0]
	eor		r2, r2, r12
	subs	r3, r3, #2
	str		r2, [r0], #4
	bcs		KeccakF400_StateXORLanes_Loop32
	adds	r3, r3, #2
	beq		KeccakF400_StateXORLanes_Exit
KeccakF400_StateXORLanes_1Lane
	ldrh	r3, [r1], #2
	ldrh	r12, [r0]
	eor		r3, r3, r12
	strh	r3, [r0], #2
KeccakF400_StateXORLanes_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
;//
	ALIGN
	EXPORT  KeccakF400_StateXORBytesInLane
KeccakF400_StateXORBytesInLane   PROC
	ldr		r12, [sp]
	cmp		r12, #0
	beq		KeccakF400_StateXORBytesInLane_Exit
	add		r0, r0, r1, LSL #1
	add		r0, r0, r3
KeccakF400_StateXORBytesInLane_Loop
	ldrb	r1, [r0]
	ldrb	r3, [r2], #1
	eors	r1, r1, r3
	strb	r1, [r0], #1
	subs	r12, r12, #1
	bne		KeccakF400_StateXORBytesInLane_Loop
KeccakF400_StateXORBytesInLane_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
;//
	ALIGN
	EXPORT  KeccakF400_StateOverwriteLanes
KeccakF400_StateOverwriteLanes	PROC 
	cbz		r2, KeccakF400_StateOverwriteLanes_Exit
KeccakF400_StateOverwriteLanes_Loop
	ldrh	r3, [r1], #2
	strh	r3, [r0], #2
	subs	r2, r2, #1
	bne		KeccakF400_StateOverwriteLanes_Loop
KeccakF400_StateOverwriteLanes_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateOverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
;//
	ALIGN
	EXPORT  KeccakF400_StateOverwriteBytesInLane
KeccakF400_StateOverwriteBytesInLane	PROC
	adds	r0, r0, r3
	ldr		r3, [sp]
	cbz		r3, KeccakF400_StateOverwriteBytesInLane_Exit
	adds	r0, r0, r1, LSL #1
KeccakF400_StateOverwriteBytesInLane_Loop
	ldrb	r1, [r2], #1
	strb	r1, [r0], #1
	subs	r3, r3, #1
	bne		KeccakF400_StateOverwriteBytesInLane_Loop
KeccakF400_StateOverwriteBytesInLane_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
;//
	ALIGN
	EXPORT  KeccakF400_StateOverwriteWithZeroes
KeccakF400_StateOverwriteWithZeroes	PROC 
	movs	r3, #0
	lsrs	r2, r1, #2
	beq		KeccakF400_StateOverwriteWithZeroes_Bytes
KeccakF400_StateOverwriteWithZeroes_Loop2Lanes
	str		r3, [r0], #4
	subs	r2, r2, #1
	bne		KeccakF400_StateOverwriteWithZeroes_Loop2Lanes
KeccakF400_StateOverwriteWithZeroes_Bytes
	ands	r1, #3
	beq		KeccakF400_StateOverwriteWithZeroes_Exit
KeccakF400_StateOverwriteWithZeroes_LoopBytes
	strb	r3, [r0], #1
	subs	r1, r1, #1
	bne		KeccakF400_StateOverwriteWithZeroes_LoopBytes
KeccakF400_StateOverwriteWithZeroes_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
;//
	ALIGN
	EXPORT  KeccakF400_StateExtractLanes
KeccakF400_StateExtractLanes   PROC
	cbz		r2, KeccakF400_StateExtractLanes_Exit
	subs	r3, r2, #2
	bcc		KeccakF400_StateExtractLanes_1Lane
KeccakF400_StateExtractLanes_Loop32
	ldr		r2, [r0], #4
	subs	r3, r3, #2
	str		r2, [r1], #4
	bcs		KeccakF400_StateExtractLanes_Loop32
	adds	r3, r3, #2
	beq		KeccakF400_StateExtractLanes_Exit
KeccakF400_StateExtractLanes_1Lane
	ldrh	r3, [r0], #2
	strh	r3, [r1], #2
KeccakF400_StateExtractLanes_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
;//
	ALIGN
	EXPORT  KeccakF400_StateExtractBytesInLane
KeccakF400_StateExtractBytesInLane   PROC
	ldr		r12, [sp]
	cmp		r12, #0
	beq		KeccakF400_StateExtractBytesInLane_Exit
	add		r0, r0, r1, LSL #1
	add		r0, r0, r3
KeccakF400_StateExtractBytesInLane_Loop
	ldrb	r1, [r0], #1
	subs	r12, r12, #1
	strb	r1, [r2], #1
	bne		KeccakF400_StateExtractBytesInLane_Loop
KeccakF400_StateExtractBytesInLane_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
;//
	ALIGN
	EXPORT  KeccakF400_StateExtractAndXORLanes
KeccakF400_StateExtractAndXORLanes	PROC
	cbz		r2, KeccakF400_StateExtractAndXORLanes_Exit
KeccakF400_StateExtractAndXORLanes_Loop
	ldrh	r3, [r0], #2
	ldrh	r12, [r1]
	eors	r3, r3, r12
	strh	r3, [r1], #2
	subs	r2, r2, #1
	bne		KeccakF400_StateExtractAndXORLanes_Loop
KeccakF400_StateExtractAndXORLanes_Exit
	bx		lr
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
;//
	ALIGN
	EXPORT  KeccakF400_StateExtractAndXORBytesInLane
KeccakF400_StateExtractAndXORBytesInLane	PROC
	ldr		r12, [sp]
	cmp		r12, #0
	beq		KeccakF400_StateExtractAndXORBytesInLane_Exit
	adds	r0, r0, r1, LSL #1
	adds	r0, r0, r3
KeccakF400_StateExtractAndXORBytesInLane_Loop
	ldrb	r1, [r0], #1
	ldrb	r3, [r2]
	eors	r1, r1, r3
	strb	r1, [r2], #1
	subs	r12, r12, #1
	bne		KeccakF400_StateExtractAndXORBytesInLane_Loop
KeccakF400_StateExtractAndXORBytesInLane_Exit
	bx		lr
	ENDP

;;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StatePermute( void *state )
;//
	ALIGN
	EXPORT  KeccakF400_StatePermute
KeccakF400_StatePermute   PROC
	push		{r4-r12,lr}
	sub			sp, sp, #2*25+6
	adr			r8, KeccakF400_StatePermute_RoundConstants
KeccakF400_StatePermuteIntern
    ldrh		r9, [r0, #_sa] 
    ldrh		r10, [r0, #_se] 
    ldrh		r11, [r0, #_si] 
    ldrh		lr, [r0, #_su] 
    ldrh		r12, [r0, #_so] 
	mov			r5, lr
    xor5		r7, r0, _bu, _gu, _ku, _mu, lr
KeccakF400_StatePermute_RoundLoop
	KeccakRound	sp, r0
KeccakF400_StatePermute_RoundOdd
	KeccakRound	r0, sp
	ldrh		r3, [r8]
	cmp			r3, #0
	bne			KeccakF400_StatePermute_RoundLoop
	add			sp,sp,#2*25+6
	pop			{r4-r12,pc}

	ALIGN
KeccakF400_StatePermute_RoundConstants
	dcw			0x0001
	dcw			0x8082
	dcw			0x808a
	dcw			0x8000
	dcw			0x808b
	dcw			0x0001
	dcw			0x8081
	dcw			0x8009
	dcw			0x008a
	dcw			0x0088
	dcw			0x8009
	dcw			0x000a
	dcw			0x808b
	dcw			0x008b
	dcw			0x8089
	dcw			0x8003
	dcw			0x8002
	dcw			0x0080
	dcw			0x800a
	dcw			0x000a
KeccakP400_StatePermute_RoundConstants
	dcw			0		; terminator
	ENDP

	END
