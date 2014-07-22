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

; WARNING: These functions work only on little endian CPU with ARMv6m architecture (Cortex-M0, ...).

	PRESERVE8
	THUMB
	AREA    |.text|, CODE, READONLY

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
	xor5		$result,$ptr,$b,$g,$k,$m,$s
	ldrh		$result, [$ptr, #$b]
	ldrh		r7, [$ptr, #$g]
	eors		$result, $result, r7
	ldrh		r7, [$ptr, #$k]
	eors		$result, $result, r7				
	ldrh		r7, [$ptr, #$m]
	eors		$result, $result, r7				
	ldrh		r7, [$ptr, #$s]
	eors		$result, $result, r7				
	MEND

	MACRO
	xorrol 		$b, $yy, $rr
	mov			r7, $yy
	eors		$b, $b, r7
	if			$rr != 0
	lsls		r7, $b, #$rr
	lsrs		$b, $b, #16-$rr
	orrs		$b, $b, r7
	uxth		$b
	endif
	MEND

	MACRO
	rolxor 		$d, $a, $b, $rot
	sxth		r7, $b
	rors		r7, r7, $rot
	eors		r7, r7, $a
	uxth		r7
	mov			$d, r7
	MEND

	MACRO
	xandnot 	$resptr, $resofs, $aa, $bb, $cc, $temp
	mov			$temp, $cc
	bics		$temp, $temp, $bb
	eors		$temp, $temp, $aa
	strh		$temp, [$resptr, #$resofs]
	MEND

	MACRO
	xandnotRC 	$resptr, $resofs, $aa, $bb, $cc, $rco
	bics		$cc, $cc, $bb
	eors		$cc, $cc, $aa
	mov			r7, r8
	ldrh		$bb, [r7, #$rco]
	eors		$cc, $cc, $bb
	strh		$cc, [$resptr, #$resofs]
	MEND

	MACRO
	KeccakRound 	$sOut, $sIn, $rco

	;//prepTheta
	push		{ $sOut }
	movs		$sOut, #31
    xor5		r1, $sIn, _ba, _ga, _ka, _ma, _sa
    xor5		r2, $sIn, _be, _ge, _ke, _me, _se
    xor5		r3, $sIn, _bi, _gi, _ki, _mi, _si
    xor5		r4, $sIn, _bo, _go, _ko, _mo, _so
    xor5		r5, $sIn, _bu, _gu, _ku, _mu, _su
	rolxor		r9, r5, r2, $sOut
    rolxor		r10, r1, r3, $sOut
    rolxor		r11, r2, r4, $sOut
    rolxor		r12, r3, r5, $sOut
    rolxor		lr, r4, r1, $sOut
	pop			{ $sOut }

	;//thetaRhoPiChiIota
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
	xandnot		$sOut, _ga, r1, r2, r3, r7
	xandnot		$sOut, _ge, r2, r3, r4, r7
	xandnot		$sOut, _gi, r3, r4, r5, r7
	xandnot		$sOut, _go, r4, r5, r1, r7
	xandnot		$sOut, _gu, r5, r1, r2, r7

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
	xandnot		$sOut, _ka, r1, r2, r3, r7
	xandnot		$sOut, _ke, r2, r3, r4, r7
	xandnot		$sOut, _ki, r3, r4, r5, r7
	xandnot		$sOut, _ko, r4, r5, r1, r7
	xandnot		$sOut, _ku, r5, r1, r2, r7

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
	xandnot		$sOut, _ma, r1, r2, r3, r7
	xandnot		$sOut, _me, r2, r3, r4, r7
	xandnot		$sOut, _mi, r3, r4, r5, r7
	xandnot		$sOut, _mo, r4, r5, r1, r7
	xandnot		$sOut, _mu, r5, r1, r2, r7

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
	xandnot		$sOut, _sa, r1, r2, r3, r7
	xandnot		$sOut, _se, r2, r3, r4, r7
	xandnot		$sOut, _si, r3, r4, r5, r7
	xandnot		$sOut, _so, r4, r5, r1, r7
	xandnot		$sOut, _su, r5, r1, r2, r7

	ldrh		r1, [$sIn, #_ba]
	ldrh		r2, [$sIn, #_ge]
	ldrh		r3, [$sIn, #_ki]
	ldrh		r4, [$sIn, #_mo]
	ldrh		r5, [$sIn, #_su]
	xorrol		r1, r9, 0
	xorrol 		r2, r10, 12
	xorrol 		r3, r11, 11
	xorrol 		r4, r12,  5
	xorrol 		r5, lr, 14
	xandnot		$sOut, _be, r2, r3, r4, r7
	xandnot		$sOut, _bi, r3, r4, r5, r7
	xandnot		$sOut, _bo, r4, r5, r1, r7
	xandnot		$sOut, _bu, r5, r1, r2, r7
	xandnotRC	$sOut, _ba, r1, r2, r3, $rco
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
	lsrs	r2, r1, #3
	add		r0, r2
	ldrb	r2, [r0]
	lsls	r1, r1, #32-3
	lsrs	r1, r1, #32-3
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
	subs	r2, r2, #1
	bcc		KeccakF400_StateXORLanes_Exit
	lsls	r2, r2, #1
	push	{r4,lr}
	lsls	r4, r1, #31
	bne		KeccakF400_StateXORLanes_Unaligned
KeccakF400_StateXORLanes_Loop16
	ldrh	r3, [r1, r2]
	ldrh	r4, [r0, r2]
	eors	r3, r3, r4
	strh	r3, [r0, r2]
	subs	r2, r2, #2
	bcs		KeccakF400_StateXORLanes_Loop16
	pop		{r4,pc}
KeccakF400_StateXORLanes_Unaligned
	adds	r2, r2, #1
KeccakF400_StateXORLanes_Loop8
	ldrb	r3, [r1, r2]
	ldrb	r4, [r0, r2]
	eors	r3, r3, r4
	strb	r3, [r0, r2]
	subs	r2, r2, #1
	bcs		KeccakF400_StateXORLanes_Loop8
	pop		{r4,pc}
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
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF400_StateXORBytesInLane_Exit
	lsls	r1, r1, #1
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF400_StateXORBytesInLane_Loop
	ldrb	r1, [r0, r4]
	ldrb	r3, [r2, r4]
	eors	r1, r1, r3
	strb	r1, [r0, r4]
	subs	r4, r4, #1
	bcs		KeccakF400_StateXORBytesInLane_Loop
KeccakF400_StateXORBytesInLane_Exit
	pop		{r4,pc}
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
;//
	ALIGN
	EXPORT  KeccakF400_StateOverwriteLanes
KeccakF400_StateOverwriteLanes	PROC 
	subs	r2, r2, #1
	bcc		KeccakF400_StateOverwriteLanes_Exit
	lsls	r2, r2, #1
	lsls	r3, r1, #31
	bne		KeccakF400_StateOverwriteLanes_Unaligned
KeccakF400_StateOverwriteLanes_Loop16
	ldrh	r3, [r1, r2]
	strh	r3, [r0, r2]
	subs	r2, r2, #2
	bcs		KeccakF400_StateOverwriteLanes_Loop16
	bx		lr
KeccakF400_StateOverwriteLanes_Unaligned
	adds	r2, r2, #1
KeccakF400_StateOverwriteLanes_Loop8
	ldrb	r3, [r1, r2]
	strb	r3, [r0, r2]
	subs	r2, r2, #1
	bcs		KeccakF400_StateOverwriteLanes_Loop8
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
	subs	r3, r3, #1
	bcc		KeccakF400_StateOverwriteBytesInLane_Exit
	lsls	r1, r1, #1
	adds	r0, r0, r1
KeccakF400_StateOverwriteBytesInLane_Loop
	ldrb	r1, [r2, r3]
	strb	r1, [r0, r3]
	subs	r3, r3, #1
	bcs		KeccakF400_StateOverwriteBytesInLane_Loop
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
	cmp		r1, #0
	beq		KeccakF400_StateOverwriteWithZeroes_Exit
KeccakF400_StateOverwriteWithZeroes_LoopBytes
	subs	r1, r1, #1
	strb	r3, [r0, r1]
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
	subs	r2, r2, #1
	bcc		KeccakF400_StateExtractLanes_Exit
	lsls	r2, r2, #1
	lsls	r3, r1, #31
	bne		KeccakF400_StateExtractLanes_Unaligned
KeccakF400_StateExtractLanes_Loop32
	ldrh	r3, [r0, r2]
	strh	r3, [r1, r2]
	subs	r2, r2, #2
	bcs		KeccakF400_StateExtractLanes_Loop32
	bx		lr
KeccakF400_StateExtractLanes_Unaligned
	adds	r2, r2, #1
KeccakF400_StateExtractLanes_Loop8
	ldrb	r3, [r0, r2]
	strb	r3, [r1, r2]
	subs	r2, r2, #1
	bcs		KeccakF400_StateExtractLanes_Loop8
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
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF400_StateExtractBytesInLane_Exit
	lsls	r1, r1, #1
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF400_StateExtractBytesInLane_Loop
	ldrb	r1, [r0, r4]
	strb	r1, [r2, r4]
	subs	r4, r4, #1
	bcs		KeccakF400_StateExtractBytesInLane_Loop
KeccakF400_StateExtractBytesInLane_Exit
	pop		{r4,pc}
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
;//
	ALIGN
	EXPORT  KeccakF400_StateExtractAndXORLanes
KeccakF400_StateExtractAndXORLanes	PROC
	push	{r4,lr}
	subs	r2, r2, #1
	bcc		KeccakF400_StateExtractAndXORLanes_Exit
	lsls	r2, r2, #1
	lsls	r3, r1, #31
	bne		KeccakF400_StateExtractAndXORLanes_Unaligned
KeccakF400_StateExtractAndXORLanes_Loop32
	ldrh	r3, [r0, r2]
	ldrh	r4, [r1, r2]
	eors	r3, r3, r4
	strh	r3, [r1, r2]
	subs	r2, r2, #2
	bcs		KeccakF400_StateExtractAndXORLanes_Loop32
	pop		{r4,pc}
KeccakF400_StateExtractAndXORLanes_Unaligned
	adds	r2, r2, #1
KeccakF400_StateExtractAndXORLanes_Loop8
	ldrb	r3, [r0, r2]
	ldrb	r4, [r1, r2]
	eors	r3, r3, r4
	strb	r3, [r1, r2]
	subs	r2, r2, #1
	bcs		KeccakF400_StateExtractAndXORLanes_Loop8
KeccakF400_StateExtractAndXORLanes_Exit
	pop		{r4,pc}
	ENDP

;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
;//
	ALIGN
	EXPORT  KeccakF400_StateExtractAndXORBytesInLane
KeccakF400_StateExtractAndXORBytesInLane	PROC
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF400_StateExtractAndXORBytesInLane_Exit
	lsls	r1, r1, #1
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF400_StateExtractAndXORBytesInLane_Loop
	ldrb	r1, [r0, r4]
	ldrb	r3, [r2, r4]
	eors	r1, r1, r3
	strb	r1, [r2, r4]
	subs	r4, r4, #1
	bcs		KeccakF400_StateExtractAndXORBytesInLane_Loop
KeccakF400_StateExtractAndXORBytesInLane_Exit
	pop		{r4,pc}
	ENDP

SECTION_KECCAK_F_START
;//----------------------------------------------------------------------------
;//
;// void KeccakF400_StatePermute( void *state )
;//

	ALIGN
	EXPORT  KeccakF400_StatePermute
KeccakF400_StatePermute   PROC
	push		{ r4 - r6, lr }
	mov			r2, r8
	mov			r3, r9
	mov			r4, r10
	mov			r5, r11
	mov			r6, r12
	push		{ r2 - r7 }
	sub			sp, sp, #25*2+6
	mov			r6, sp
	adr			r7, KeccakF400_StatePermute_RoundConstants
	b			KeccakF400_StatePermute_RoundLoop
	nop
SECTION_KECCAK_F_END
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
	dcw			0xFF			;//terminator

KeccakF400_StatePermute_RoundLoop
	mov			r8, r7
	KeccakRound	r6, r0, 0
KeccakF400_StatePermute_RoundOdd
	KeccakRound	r0, r6, 2
	adds		r7, r7, #4
	ldrh		r1, [r7]
	cmp			r1, #0xFF
	beq			KeccakF400_StatePermute_Done
	b			KeccakF400_StatePermute_RoundLoop
KeccakF400_StatePermute_Done
	add			sp,sp,#25*2+6
	pop			{ r1 - r5, r7 }
	mov			r8, r1
	mov			r9, r2
	mov			r10, r3
	mov			r11, r4
	mov			r12, r5
	pop			{ r4 - r6, pc }
	ENDP

	END
