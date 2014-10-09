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

; WARNING: These functions work only on little endian CPU with ARMv7a architecture.

	PRESERVE8
	AREA    |.text|, CODE, READONLY

_ba	equ  0*4
_be	equ  1*4
_bi	equ  2*4
_bo	equ  3*4
_bu	equ  4*4
_ga	equ  5*4
_ge	equ  6*4
_gi	equ  7*4
_go	equ  8*4
_gu	equ  9*4
_ka	equ 10*4
_ke	equ 11*4
_ki	equ 12*4
_ko	equ 13*4
_ku	equ 14*4
_ma	equ 15*4
_me	equ 16*4
_mi	equ 17*4
_mo	equ 18*4
_mu	equ 19*4
_sa	equ 20*4
_se	equ 21*4
_si	equ 22*4
_so	equ 23*4
_su	equ 24*4

	MACRO
	ThetaRhoPiChiIota	$stateOut, $stateIn, $ofsOut, $ofs1, $ofs2, $ofs3, $ofs4, $dd1, $dd2, $dd3, $dd4, $dd5, $rr2, $rr3, $rr4, $rr5
	ldr			r2, [$stateIn, #$ofs1]
	ldr			r3, [$stateIn, #$ofs2]
	ldr			r4, [$stateIn, #$ofs3]
	eor			r2, r2, $dd1
	ldr			r5, [$stateIn, #$ofs4]
	eor			r3, r3, $dd2
	eor			r4, r4, $dd3
	eor			r5, r5, $dd4
	eor			r6, r6, $dd5
	ror			r3, r3, #32-$rr2
	ror			r4, r4, #32-$rr3
	ror			r5, r5, #32-$rr4
	ror			r6, r6, #32-$rr5

	bic			r1, r5, r4
	eor			r1, r1, r3
	str			r1, [$stateOut, #$ofsOut+4]
	bic			r1, r6, r5
	eor			r1, r1, r4
	str			r1, [$stateOut, #$ofsOut+8]
	bic			r1, r2, r6
	eor			r1, r1, r5
	str			r1, [$stateOut, #$ofsOut+12]
	bic			r7, r3, r2
	eor			r7, r7, r6
	str			r7, [$stateOut, #$ofsOut+16]
	ldr			r1, [lr], #4
	bic			r4, r4, r3
	eor			r4, r4, r1
	eor			r4, r4, r2
	str			r4, [$stateOut, #$ofsOut+0]
	MEND

	MACRO
	ThetaRhoPiChi		$stateOut, $stateIn, $ofsOut, $ofs1, $ofs2, $ofs3, $ofs4, $ofs5, $dd1, $dd2, $dd3, $dd4, $dd5, $rr1, $rr2, $rr3, $rr4, $rr5
	ldr			r2, [$stateIn, #$ofs1]
	ldr			r3, [$stateIn, #$ofs2]
	ldr			r4, [$stateIn, #$ofs3]
	eor			r2, r2, $dd1
	ldr			r5, [$stateIn, #$ofs4]
	eor			r3, r3, $dd2
	ldr			r6, [$stateIn, #$ofs5]
	eor			r4, r4, $dd3
	eor			r5, r5, $dd4
	ror			r2, r2, #32-$rr1
	eor			r6, r6, $dd5
	ror			r3, r3, #32-$rr2
	ror			r4, r4, #32-$rr3
	ror			r5, r5, #32-$rr4
	ror			r6, r6, #32-$rr5

	bic			r1, r4, r3
	eor			r1, r1, r2
	str			r1, [$stateOut, #$ofsOut+0]
	bic			r1, r5, r4
	eor			r1, r1, r3
	str			r1, [$stateOut, #$ofsOut+4]
	bic			r1, r6, r5
	eor			r1, r1, r4
	str			r1, [$stateOut, #$ofsOut+8]
	bic			r1, r2, r6
	bic			r2, r3, r2
	eor			r1, r1, r5
	eor			r2, r2, r6
	str			r1, [$stateOut, #$ofsOut+12]
	eor			r7, r7, r2
	str			r2, [$stateOut, #$ofsOut+16]
	MEND

	MACRO
	ThetaRhoPiChiLast	$stateOut, $stateIn, $ofsOut, $ofs1, $ofs2, $ofs3, $ofs4, $ofs5, $dd1, $dd2, $dd3, $dd4, $dd5, $rr1, $rr2, $rr3, $rr4, $rr5
	ldr			r2, [$stateIn, #$ofs1]
	ldr			r3, [$stateIn, #$ofs2]
	ldr			r4, [$stateIn, #$ofs3]
	eor			r2, r2, $dd1
	ldr			r5, [$stateIn, #$ofs4]
	eor			r3, r3, $dd2
	ldr			r6, [$stateIn, #$ofs5]
	eor			r4, r4, $dd3
	eor			r5, r5, $dd4
	ror			r2, r2, #32-$rr1
	eor			r6, r6, $dd5
	ror			r3, r3, #32-$rr2
	ror			r4, r4, #32-$rr3
	ror			r5, r5, #32-$rr4
	ror			r6, r6, #32-$rr5

	bic			r8, r4, r3
	bic			r9, r5, r4
	bic			r10, r6, r5
	bic			r11, r2, r6
	bic			r1, r3, r2
	eor			r8, r8, r2
	eor			r9, r9, r3
	eor			r10, r10, r4
	str			r8, [$stateOut, #$ofsOut+0]
	eor			r11, r11, r5
	str			r9, [$stateOut, #$ofsOut+4]
	eor			r6, r6, r1
	str			r10, [$stateOut, #$ofsOut+8]
	str			r11, [$stateOut, #$ofsOut+12]
	eor			r7, r7, r6
	str			r6, [$stateOut, #$ofsOut+16]
	MEND

	MACRO
	KeccakRound	$stateOut, $stateIn
	;// prepare Theta
	ldr			r2, [$stateIn, #_ba]
	ldr			r3, [$stateIn, #_be]
	ldr			r4, [$stateIn, #_bi]
	ldr			r5, [$stateIn, #_bo]
	eor			r2, r2, r8
	eor			r3, r3, r9
	eor			r4, r4, r10
	eor			r5, r5, r11
	ldr			r8, [$stateIn, #_ga]
	ldr			r9, [$stateIn, #_ge]
	ldr			r10, [$stateIn, #_gi]
	ldr			r11, [$stateIn, #_go]
	eor			r2, r2, r8
	eor			r3, r3, r9
	eor			r4, r4, r10
	eor			r5, r5, r11
	ldr			r8, [$stateIn, #_ka]
	ldr			r9, [$stateIn, #_ke]
	ldr			r10, [$stateIn, #_ki]
	ldr			r11, [$stateIn, #_ko]
	eor			r2, r2, r8
	eor			r3, r3, r9
	eor			r4, r4, r10
	eor			r5, r5, r11
	ldr			r8, [$stateIn, #_ma]
	ldr			r9, [$stateIn, #_me]
	ldr			r10, [$stateIn, #_mi]
	ldr			r11, [$stateIn, #_mo]
	eor			r2, r2, r8
	eor			r3, r3, r9
	eor			r4, r4, r10
	eor			r5, r5, r11
	eor			r8, r7, r3, ROR #31
    eor			r9, r2, r4, ROR #31
    eor			r10, r3, r5, ROR #31
    eor			r11, r4, r7, ROR #31
    eor			r12, r5, r2, ROR #31

	ThetaRhoPiChiIota	$stateOut, $stateIn, _ba, _ba, _ge, _ki, _mo,      r8, r9, r10, r11, r12,     12, 11, 21, 14
	ThetaRhoPiChi		$stateOut, $stateIn, _ga, _bo, _gu, _ka, _me, _si, r11, r12, r8, r9, r10, 28, 20,  3, 13, 29
	ThetaRhoPiChi		$stateOut, $stateIn, _ka, _be, _gi, _ko, _mu, _sa, r9, r10, r11, r12, r8,  1,  6, 25,  8, 18
	ThetaRhoPiChi		$stateOut, $stateIn, _ma, _bu, _ga, _ke, _mi, _so, r12, r8, r9, r10, r11, 27,  4, 10, 15, 24
	ThetaRhoPiChiLast	$stateOut, $stateIn, _sa, _bi, _go, _ku, _ma, _se, r10, r11, r12, r8, r9, 30, 23,  7,  9,  2
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
	cmp		r3, #0
	beq		KeccakF800_StateXORBytesInLane_Exit
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
	cmp		r3, #0
	beq		KeccakF800_StateOverwriteBytesInLane_Exit
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
KeccakF800_StatePermute_RoundConstantsWithTerminator
	dcd			0x00000001
	dcd			0x00008082
	dcd			0x0000808a
	dcd			0x80008000
	dcd			0x0000808b
	dcd			0x80000001
	dcd			0x80008081
	dcd			0x00008009
	dcd			0x0000008a
	dcd			0x00000088
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
;// void KeccakF800_StatePermute( void *state )
;//
	ALIGN
	EXPORT  KeccakF800_StatePermute
KeccakF800_StatePermute   PROC
	push		{r4-r12,lr}
	adr			lr, KeccakF800_StatePermute_RoundConstantsWithTerminator
	add			r2, r0, #_sa
	sub			sp, sp, #4*25+4
    ldmia		r2, { r8 - r12 }
	ldr			r7, [r0, #_bu]
	ldr			r1, [r0, #_gu]
	mov			r6, r12
	eor			r7, r7, r12
	ldr			r12, [r0, #_ku]
	eor			r7, r7, r1
	ldr			r1, [r0, #_mu]
	eor			r7, r7, r12
	eor			r7, r7, r1
KeccakF800_StatePermute_RoundLoop
	KeccakRound	sp, r0
	KeccakRound	r0, sp
	ldr			r4, [lr]
	cmp			r4, #0
	bne			KeccakF800_StatePermute_RoundLoop
	add			sp,sp,#4*25+4
	pop			{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakF800_SnP_FBWL_Absorb(	void *state, unsigned int laneCount, unsigned char *data,
;										size_t dataByteLen, unsigned char trailingBits )
;
	ALIGN
	EXPORT	KeccakF800_SnP_FBWL_Absorb
KeccakF800_SnP_FBWL_Absorb	PROC
	push	{r4-r12,lr}
	mov		r4, #0
	lsr		r3, r3, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakF800_SnP_FBWL_Absorb_Exit
KeccakF800_SnP_FBWL_Absorb_Loop
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r1, #4
	bcc		KeccakF800_SnP_FBWL_Absorb_LessThan4Lanes
KeccakF800_SnP_FBWL_Absorb_Loop4Lanes
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
	bcs		KeccakF800_SnP_FBWL_Absorb_Loop4Lanes
KeccakF800_SnP_FBWL_Absorb_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakF800_SnP_FBWL_Absorb_TrailingBits
KeccakF800_SnP_FBWL_Absorb_LoopLane
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r11, r11, r12
	str		r11, [r0], #4
	subs	r6, r6, #1
	bne		KeccakF800_SnP_FBWL_Absorb_LoopLane
KeccakF800_SnP_FBWL_Absorb_TrailingBits
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+0)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4}
	bl		KeccakF800_StatePermute
	pop	{r1-r4}
	subs	r3, r3, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakF800_SnP_FBWL_Absorb_Loop
KeccakF800_SnP_FBWL_Absorb_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakF800_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
;
	ALIGN
	EXPORT	KeccakF800_SnP_FBWL_Squeeze
KeccakF800_SnP_FBWL_Squeeze	PROC
	push	{r4-r12,lr}
	mov		r4, #0
	lsr		r3, r3, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakF800_SnP_FBWL_Squeeze_Exit
KeccakF800_SnP_FBWL_Squeeze_Loop
	push	{r1-r4}
	bl		KeccakF800_StatePermute
	pop		{r1-r4}
	mov		r6, r1
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r6, #4
	bcc		KeccakF800_SnP_FBWL_Squeeze_LessThan4Lanes
KeccakF800_SnP_FBWL_Squeeze_Loop4Lanes
	ldrd	r10, r11, [r0], #8
	str		r10, [r2], #4
	str		r11, [r2], #4
	ldrd	r10, r11, [r0], #8
	str		r10, [r2], #4
	str		r11, [r2], #4
	subs	r6, r6, #4
	bcs		KeccakF800_SnP_FBWL_Squeeze_Loop4Lanes
KeccakF800_SnP_FBWL_Squeeze_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakF800_SnP_FBWL_Squeeze_CheckLoop
KeccakF800_SnP_FBWL_Squeeze_LoopLane
	ldr		r11, [r0], #4
	str		r11, [r2], #4
	subs	r6, r6, #1
	bne		KeccakF800_SnP_FBWL_Squeeze_LoopLane
KeccakF800_SnP_FBWL_Squeeze_CheckLoop
	sub		r0, r0, r1, LSL #2
	subs	r3, r3, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakF800_SnP_FBWL_Squeeze_Loop
KeccakF800_SnP_FBWL_Squeeze_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakF800_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn,
;										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
;
	ALIGN
	EXPORT	KeccakF800_SnP_FBWL_Wrap
KeccakF800_SnP_FBWL_Wrap	PROC
	push	{r4-r12,lr}
	ldr		r8, [sp, #(10+0)*4]			; dataByteLen
	mov		r4, #0
	lsr		r8, r8, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakF800_SnP_FBWL_Wrap_Exit
KeccakF800_SnP_FBWL_Wrap_Loop
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r1, #4
	bcc		KeccakF800_SnP_FBWL_Wrap_LessThan4Lanes
KeccakF800_SnP_FBWL_Wrap_Loop4Lanes
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
	bcs		KeccakF800_SnP_FBWL_Wrap_Loop4Lanes
KeccakF800_SnP_FBWL_Wrap_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakF800_SnP_FBWL_Wrap_TrailingBits
KeccakF800_SnP_FBWL_Wrap_LoopLane
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r11, r11, r12
	str		r11, [r0], #4
	str		r11, [r3], #4
	subs	r6, r6, #1
	bne		KeccakF800_SnP_FBWL_Wrap_LoopLane
KeccakF800_SnP_FBWL_Wrap_TrailingBits
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+1)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4,r8-r9}
	bl		KeccakF800_StatePermute
	pop	{r1-r4,r8-r9}
	subs	r8, r8, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakF800_SnP_FBWL_Wrap_Loop
KeccakF800_SnP_FBWL_Wrap_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

;----------------------------------------------------------------------------
;
; size_t KeccakF800_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn,
;										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
;
	ALIGN
	EXPORT	KeccakF800_SnP_FBWL_Unwrap
KeccakF800_SnP_FBWL_Unwrap	PROC
	push	{r4-r12,lr}
	ldr		r8, [sp, #(10+0)*4]				; dataByteLen
	mov		r4, #0
	lsr		r8, r8, #2					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1					; if (nbrLanes >= laneCount)
	bcc		KeccakF800_SnP_FBWL_Unwrap_Exit
KeccakF800_SnP_FBWL_Unwrap_Loop
	add		r4, r4, r1, LSL #2			; processed += laneCount*SnP_laneLengthInBytes;
	subs	r6, r1, #4
	bcc		KeccakF800_SnP_FBWL_Unwrap_LessThan4Lanes
KeccakF800_SnP_FBWL_Unwrap_Loop4Lanes
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
	bcs		KeccakF800_SnP_FBWL_Unwrap_Loop4Lanes
KeccakF800_SnP_FBWL_Unwrap_LessThan4Lanes
	adds	r6, r6, #4
	beq		KeccakF800_SnP_FBWL_Unwrap_TrailingBits
KeccakF800_SnP_FBWL_Unwrap_LoopLane
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r12, r12, r11
	str		r12, [r3], #4
	str		r11, [r0], #4
	subs	r6, r6, #1
	bne		KeccakF800_SnP_FBWL_Unwrap_LoopLane
KeccakF800_SnP_FBWL_Unwrap_TrailingBits
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+1)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4,r8-r9}
	bl		KeccakF800_StatePermute
	pop		{r1-r4,r8-r9}
	subs	r8, r8, r1					; rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakF800_SnP_FBWL_Unwrap_Loop
KeccakF800_SnP_FBWL_Unwrap_Exit
	mov		r0, r4
	pop		{r4-r12,pc}
	ENDP

	END
