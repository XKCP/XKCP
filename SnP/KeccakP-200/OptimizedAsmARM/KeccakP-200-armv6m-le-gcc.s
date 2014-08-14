@
@ Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
@ Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
@ denoted as "the implementer".
@
@ For more information, feedback or questions, please refer to our websites:
@ http://keccak.noekeon.org/
@ http://keyak.noekeon.org/
@ http://ketje.noekeon.org/
@
@ To the extent possible under law, the implementer has waived all copyright
@ and related or neighboring rights to the source code in this file.
@ http://creativecommons.org/publicdomain/zero/1.0/
@

@ WARNING: These functions work only on little endian CPU with@ ARMv6m architecture (Cortex-M0, ...).


	.thumb
.text

.equ _ba, 0
.equ _be, 1
.equ _bi, 2
.equ _bo, 3
.equ _bu, 4
.equ _ga, 5
.equ _ge, 6
.equ _gi, 7
.equ _go, 8
.equ _gu, 9
.equ _ka, 10
.equ _ke, 11
.equ _ki, 12
.equ _ko, 13
.equ _ku, 14
.equ _ma, 15
.equ _me, 16
.equ _mi, 17
.equ _mo, 18
.equ _mu, 19
.equ _sa, 20
.equ _se, 21
.equ _si, 22
.equ _so, 23
.equ _su, 24

.macro	xor5		result,ptr,b,g,k,m,s
	ldrb		\result, [\ptr, #\b]
	ldrb		r7, [\ptr, #\g]
	eors		\result, \result, r7
	ldrb		r7, [\ptr, #\k]
	eors		\result, \result, r7				
	ldrb		r7, [\ptr, #\m]
	eors		\result, \result, r7				
	ldrb		r7, [\ptr, #\s]
	eors		\result, \result, r7				
	.endm

.macro	xorrol 		b, yy, rr
	mov			r7, \yy
	eors		\b, \b, r7
	.if			\rr != 0
	lsls		r7, \b, #\rr
	lsrs		\b, \b, #8-\rr
	orrs		\b, \b, r7
	uxtb		\b
	.endif
	.endm

.macro	rolxor 		d, a, b, rot
	sxtb		r7, \b
	rors		r7, r7, \rot
	eors		r7, r7, \a
	uxtb		r7
	mov			\d, r7
	.endm

.macro	xandnot 	resptr, resofs, aa, bb, cc, temp
	mov			\temp, \cc
	bics		\temp, \temp, \bb
	eors		\temp, \temp, \aa
	strb		\temp, [\resptr, #\resofs]
	.endm

.macro	xandnotRC 	resptr, resofs, aa, bb, cc, rco
	bics		\cc, \cc, \bb
	eors		\cc, \cc, \aa
	mov			r7, r8
	ldrb		\bb, [r7, #\rco]
	eors		\cc, \cc, \bb
	strb		\cc, [\resptr, #\resofs]
	.endm

.macro	KeccakRound 	sOut, sIn, rco
	@//prepTheta
	push		{ \sOut }
	movs		\sOut, #31
    xor5		r1, \sIn, _ba, _ga, _ka, _ma, _sa
    xor5		r2, \sIn, _be, _ge, _ke, _me, _se
    xor5		r3, \sIn, _bi, _gi, _ki, _mi, _si
    xor5		r4, \sIn, _bo, _go, _ko, _mo, _so
    xor5		r5, \sIn, _bu, _gu, _ku, _mu, _su
	rolxor		r9, r5, r2, \sOut
    rolxor		r10, r1, r3, \sOut
    rolxor		r11, r2, r4, \sOut
    rolxor		r12, r3, r5, \sOut
    rolxor		lr, r4, r1, \sOut
	pop			{ \sOut }
	@//thetaRhoPiChiIota
	ldrb		r1, [\sIn, #_bo]
	ldrb		r2, [\sIn, #_gu]
	ldrb		r3, [\sIn, #_ka]
	ldrb		r4, [\sIn, #_me]
	ldrb		r5, [\sIn, #_si]
    xorrol 		r1, r12, 4
    xorrol 		r2, lr, 4
    xorrol 		r3, r9, 3
    xorrol 		r4, r10, 5
    xorrol 		r5, r11, 5
	xandnot		\sOut, _ga, r1, r2, r3, r7
	xandnot		\sOut, _ge, r2, r3, r4, r7
	xandnot		\sOut, _gi, r3, r4, r5, r7
	xandnot		\sOut, _go, r4, r5, r1, r7
	xandnot		\sOut, _gu, r5, r1, r2, r7
	ldrb		r1, [\sIn, #_be]
	ldrb		r2, [\sIn, #_gi]
	ldrb		r3, [\sIn, #_ko]
	ldrb		r4, [\sIn, #_mu]
	ldrb		r5, [\sIn, #_sa]
    xorrol 		r1, r10,  1
    xorrol 		r2, r11,  6
    xorrol 		r3, r12,  1
    xorrol 		r4, lr,  0
    xorrol 		r5, r9,  2
	xandnot		\sOut, _ka, r1, r2, r3, r7
	xandnot		\sOut, _ke, r2, r3, r4, r7
	xandnot		\sOut, _ki, r3, r4, r5, r7
	xandnot		\sOut, _ko, r4, r5, r1, r7
	xandnot		\sOut, _ku, r5, r1, r2, r7
	ldrb		r1, [\sIn, #_bu]
	ldrb		r2, [\sIn, #_ga]
	ldrb		r3, [\sIn, #_ke]
	ldrb		r4, [\sIn, #_mi]
	ldrb		r5, [\sIn, #_so]
    xorrol 		r1, lr, 3
    xorrol 		r2, r9, 4
    xorrol 		r3, r10, 2
    xorrol 		r4, r11, 7
    xorrol 		r5, r12, 0
	xandnot		\sOut, _ma, r1, r2, r3, r7
	xandnot		\sOut, _me, r2, r3, r4, r7
	xandnot		\sOut, _mi, r3, r4, r5, r7
	xandnot		\sOut, _mo, r4, r5, r1, r7
	xandnot		\sOut, _mu, r5, r1, r2, r7
	ldrb		r1, [\sIn, #_bi]
	ldrb		r2, [\sIn, #_go]
	ldrb		r3, [\sIn, #_ku]
	ldrb		r4, [\sIn, #_ma]
	ldrb		r5, [\sIn, #_se]
    xorrol 		r1, r11, 6
    xorrol 		r2, r12, 7
    xorrol 		r3, lr, 7
    xorrol 		r4, r9, 1
    xorrol 		r5, r10, 2
	xandnot		\sOut, _sa, r1, r2, r3, r7
	xandnot		\sOut, _se, r2, r3, r4, r7
	xandnot		\sOut, _si, r3, r4, r5, r7
	xandnot		\sOut, _so, r4, r5, r1, r7
	xandnot		\sOut, _su, r5, r1, r2, r7
	ldrb		r1, [\sIn, #_ba]
	ldrb		r2, [\sIn, #_ge]
	ldrb		r3, [\sIn, #_ki]
	ldrb		r4, [\sIn, #_mo]
	ldrb		r5, [\sIn, #_su]
	xorrol		r1, r9, 0
	xorrol 		r2, r10, 4
	xorrol 		r3, r11, 3
	xorrol 		r4, r12, 5
	xorrol 		r5, lr, 6
	xandnot		\sOut, _be, r2, r3, r4, r7
	xandnot		\sOut, _bi, r3, r4, r5, r7
	xandnot		\sOut, _bo, r4, r5, r1, r7
	xandnot		\sOut, _bu, r5, r1, r2, r7
	xandnotRC	\sOut, _ba, r1, r2, r3, \rco
	.endm

@//----------------------------------------------------------------------------
@//
@// void KeccakF200_Initialize( void )
@//
.align 8
.global   KeccakF200_Initialize
KeccakF200_Initialize:  
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateInitialize(void *state)
@//
.align 8
.global   KeccakF200_StateInitialize
KeccakF200_StateInitialize:  
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	stmia	r0!, { r1 - r3 }
	stmia	r0!, { r1 - r3 }
	strb	r1, [r0]
	bx		lr


@//----------------------------------------------------------------------------
@//
@//	void KeccakF200_StateComplementBit(void *state, unsigned int position)
@//
.align 8
.global   KeccakF200_StateComplementBit
KeccakF200_StateComplementBit:  
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


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
@// 
.align 8
.global   KeccakF200_StateXORLanes
KeccakF200_StateXORLanes:  
	subs	r2, r2, #1
	bcc		KeccakF200_StateXORLanes_Exit
	push	{r4-r5}
KeccakF200_StateXORLanes_Loop:
	ldrb	r3, [r1, r2]
	ldrb	r4, [r0, r2]
	eors	r3, r3, r4
	strb	r3, [r0, r2]
	subs	r2, r2, #1
	bcs		KeccakF200_StateXORLanes_Loop
	pop		{r4-r5}
KeccakF200_StateXORLanes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF200_StateXORBytesInLane
KeccakF200_StateXORBytesInLane:  
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF200_StateXORBytesInLane_Exit
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF200_StateXORBytesInLane_Loop:
	ldrb	r1, [r0, r4]
	ldrb	r3, [r2, r4]
	eors	r1, r1, r3
	strb	r1, [r0, r4]
	subs	r4, r4, #1
	bcs		KeccakF200_StateXORBytesInLane_Loop
KeccakF200_StateXORBytesInLane_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
@//
.align 8
.global   KeccakF200_StateOverwriteLanes
KeccakF200_StateOverwriteLanes: 
	subs	r2, r2, #1
	bcc		KeccakF200_StateOverwriteLanes_Exit
KeccakF200_StateOverwriteLanes_Loop:
	ldrb	r3, [r1, r2]
	strb	r3, [r0, r2]
	subs	r2, r2, #1
	bcs		KeccakF200_StateOverwriteLanes_Loop
KeccakF200_StateOverwriteLanes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateOverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF200_StateOverwriteBytesInLane
KeccakF200_StateOverwriteBytesInLane:
	adds	r0, r0, r3
	ldr		r3, [sp]
	subs	r3, r3, #1
	bcc		KeccakF200_StateOverwriteBytesInLane_Exit
	adds	r0, r0, r1
KeccakF200_StateOverwriteBytesInLane_Loop:
	ldrb	r1, [r2, r3]
	strb	r1, [r0, r3]
	subs	r3, r3, #1
	bcs		KeccakF200_StateOverwriteBytesInLane_Loop
KeccakF200_StateOverwriteBytesInLane_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
@//
.align 8
.global   KeccakF200_StateOverwriteWithZeroes
KeccakF200_StateOverwriteWithZeroes: 
	movs	r3, #0
	cmp		r1, #0
	beq		KeccakF200_StateOverwriteWithZeroes_Exit
KeccakF200_StateOverwriteWithZeroes_LoopBytes:
	subs	r1, r1, #1
	strb	r3, [r0, r1]
	bne		KeccakF200_StateOverwriteWithZeroes_LoopBytes
KeccakF200_StateOverwriteWithZeroes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
@//
.align 8
.global   KeccakF200_StateExtractLanes
KeccakF200_StateExtractLanes:  
	subs	r2, r2, #1
	bcc		KeccakF200_StateExtractLanes_Exit
KeccakF200_StateExtractLanes_Loop:
	ldrb	r3, [r0, r2]
	strb	r3, [r1, r2]
	subs	r2, r2, #1
	bcs		KeccakF200_StateExtractLanes_Loop
KeccakF200_StateExtractLanes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF200_StateExtractBytesInLane
KeccakF200_StateExtractBytesInLane:  
	add		r0, r0, r1
	add		r0, r0, r3
	ldr		r1, [sp]
	subs	r1, r1, #1
	bcc		KeccakF200_StateExtractBytesInLane_Exit
KeccakF200_StateExtractBytesInLane_Loop:
	ldrb	r3, [r0, r1]
	strb	r3, [r2, r1]
	subs	r1, r1, #1
	bcs		KeccakF200_StateExtractBytesInLane_Loop
KeccakF200_StateExtractBytesInLane_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
@//
.align 8
.global   KeccakF200_StateExtractAndXORLanes
KeccakF200_StateExtractAndXORLanes:
	push	{r4,lr}
	subs	r2, r2, #1
	bcc		KeccakF200_StateExtractAndXORLanes_Exit
KeccakF200_StateExtractAndXORLanes_Loop:
	ldrb	r3, [r0, r2]
	ldrb	r4, [r1, r2]
	eors	r3, r3, r4
	strb	r3, [r1, r2]
	subs	r2, r2, #1
	bcs		KeccakF200_StateExtractAndXORLanes_Loop
KeccakF200_StateExtractAndXORLanes_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakF200_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF200_StateExtractAndXORBytesInLane
KeccakF200_StateExtractAndXORBytesInLane:
	push	{r4,lr}
	add		r0, r0, r1
	add		r0, r0, r3
	ldr		r1, [sp, #8]
	subs	r1, r1, #1
	bcc		KeccakF200_StateExtractAndXORBytesInLane_Exit
KeccakF200_StateExtractAndXORBytesInLane_Loop:
	ldrb	r3, [r0, r1]
	ldrb	r4, [r2, r1]
	eors	r3, r3, r4
	strb	r3, [r2, r1]
	subs	r1, r1, #1
	bcs		KeccakF200_StateExtractAndXORBytesInLane_Loop
KeccakF200_StateExtractAndXORBytesInLane_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakP200_StatePermute( void *state, unsigned int nr )
@//
.align 8
.global   KeccakP200_StatePermute
KeccakP200_StatePermute:  
	push	{ r4 - r6, lr }
	mov		r2, r8
	mov		r3, r9
	mov		r4, r10
	mov		r5, r11
	mov		r6, r12
	push	{ r2 - r7 }
	sub		sp, sp, #25+7
	mov		r6, sp
	adr		r7, KeccakP200_StatePermute_RoundConstants
	adds	r7, r7, #18
	subs	r7, r7, r1
	lsls	r1, r1, #31
	beq		KeccakP200_StatePermute_RoundLoop
	ldm		r0!, { r1, r2, r3, r4, r5 }	@ odd number of rounds: copy state to stack
	subs	r0, r0, #20
	stm		r6!, { r1, r2, r3, r4, r5 }
	subs	r6, r6, #20
	ldr		r1, [r0, #_sa]
	str		r1, [r6, #_sa]
	ldrb	r1, [r0, #_su]
	strb	r1, [r6, #_su]
	subs	r7, r7, #1
	mov		r8, r7
	b		KeccakP200_StatePermute_RoundOdd
	nop
KeccakP200_StatePermute_RoundConstants:
		.byte 		0x01
		.byte 		0x82
		.byte 		0x8a
		.byte 		0x00
		.byte 		0x8b
		.byte 		0x01
		.byte 		0x81
		.byte 		0x09
		.byte 		0x8a
		.byte 		0x88
		.byte 		0x09
		.byte 		0x0a
		.byte 		0x8b
		.byte 		0x8b
		.byte 		0x89
		.byte 		0x03
		.byte 		0x02
		.byte 		0x80

.align 8
KeccakP200_StatePermute_RoundLoop:
	mov		r8, r7
	KeccakRound	r6, r0, 0
KeccakP200_StatePermute_RoundOdd:
	KeccakRound	r0, r6, 1
	adds	r7, r7, #2
	cmp		r2, #0x80
	beq		KeccakP200_StatePermute_Done
	b		KeccakP200_StatePermute_RoundLoop
KeccakP200_StatePermute_Done:
	add		sp,sp,#25+7
	pop		{ r1 - r5, r7 }
	mov		r8, r1
	mov		r9, r2
	mov		r10, r3
	mov		r11, r4
	mov		r12, r5
	pop		{ r4 - r6, pc }


