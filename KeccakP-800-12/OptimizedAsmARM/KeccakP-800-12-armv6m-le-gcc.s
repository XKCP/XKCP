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

@//----------------------------------------------------------------------------

.equ _ba	, 0*4
.equ _be	, 1*4
.equ _bi	, 2*4
.equ _bo	, 3*4
.equ _bu	, 4*4
.equ _ga	, 5*4
.equ _ge	, 6*4
.equ _gi	, 7*4
.equ _go	, 8*4
.equ _gu	, 9*4
.equ _ka	, 10*4
.equ _ke	, 11*4
.equ _ki	, 12*4
.equ _ko	, 13*4
.equ _ku	, 14*4
.equ _ma	, 15*4
.equ _me	, 16*4
.equ _mi	, 17*4
.equ _mo	, 18*4
.equ _mu	, 19*4
.equ _sa	, 20*4
.equ _se	, 21*4
.equ _si	, 22*4
.equ _so	, 23*4
.equ _su	, 24*4

.macro	xor5		result,ptr,b,g,k,m,s
	ldr			\result, [\ptr, #\b]
	ldr			r6, [\ptr, #\g]
	eors		\result, \result, r6
	ldr			r6, [\ptr, #\k]
	eors		\result, \result, r6				
	ldr			r6, [\ptr, #\m]
	eors		\result, \result, r6				
	ldr			r6, [\ptr, #\s]
	eors		\result, \result, r6				
	.endm

.macro	xorrol 		b, yy, rr
	mov			r6, \yy
	eors		\b, \b, r6
	.if			\rr != 0
	movs		r6, #32-\rr
	rors		\b, \b, r6
	.endif
	.endm

.macro	rolxor 		d, a, b
	movs		r7, #31
	movs		r6, \b
	rors		r6, r6, r7
	eors		r6, r6, \a
	mov			\d, r6
	.endm

.macro	xandnot 	resptr, resofs, aa, bb, cc, temp
	mov			\temp, \cc
	bics		\temp, \temp, \bb
	eors		\temp, \temp, \aa
	str			\temp, [\resptr, #\resofs]
	.endm

.macro	xandnotRC 	resptr, resofs, aa, bb, cc
	bics		\cc, \cc, \bb
	eors		\cc, \cc, \aa
	mov			r6, r8
	ldm			r6!, { \bb }
	mov			r8, r6
	eors		\cc, \cc, \bb
	str			\cc, [\resptr, #\resofs]
	.endm

.macro	KeccakRound 	sOut, sIn

	@//prepTheta
    xor5		r1, \sIn, _ba, _ga, _ka, _ma, _sa
    xor5		r2, \sIn, _be, _ge, _ke, _me, _se
    xor5		r3, \sIn, _bi, _gi, _ki, _mi, _si
    xor5		r4, \sIn, _bo, _go, _ko, _mo, _so
    xor5		r5, \sIn, _bu, _gu, _ku, _mu, _su
	rolxor		r9, r5, r2
    rolxor		r10, r1, r3
    rolxor		r11, r2, r4
    rolxor		r12, r3, r5
    rolxor		lr, r4, r1

	@//thetaRhoPiChiIota
	ldr			r1, [\sIn, #_bo]
	ldr			r2, [\sIn, #_gu]
	ldr			r3, [\sIn, #_ka]
	ldr			r4, [\sIn, #_me]
	ldr			r5, [\sIn, #_si]
    xorrol 		r1, r12, 28
    xorrol 		r2, lr, 20
    xorrol 		r3, r9,  3
    xorrol 		r4, r10, 13
    xorrol 		r5, r11, 29
	xandnot		\sOut, _ga, r1, r2, r3, r6
	xandnot		\sOut, _ge, r2, r3, r4, r6
	xandnot		\sOut, _gi, r3, r4, r5, r6
	xandnot		\sOut, _go, r4, r5, r1, r6
	xandnot		\sOut, _gu, r5, r1, r2, r6

	ldr			r1, [\sIn, #_be]
	ldr			r2, [\sIn, #_gi]
	ldr			r3, [\sIn, #_ko]
	ldr			r4, [\sIn, #_mu]
	ldr			r5, [\sIn, #_sa]
    xorrol 		r1, r10,  1
    xorrol 		r2, r11,  6
    xorrol 		r3, r12, 25
    xorrol 		r4, lr,  8
    xorrol 		r5, r9, 18
	xandnot		\sOut, _ka, r1, r2, r3, r6
	xandnot		\sOut, _ke, r2, r3, r4, r6
	xandnot		\sOut, _ki, r3, r4, r5, r6
	xandnot		\sOut, _ko, r4, r5, r1, r6
	xandnot		\sOut, _ku, r5, r1, r2, r6

	ldr			r1, [\sIn, #_bu]
	ldr			r2, [\sIn, #_ga]
	ldr			r3, [\sIn, #_ke]
	ldr			r4, [\sIn, #_mi]
	ldr			r5, [\sIn, #_so]
    xorrol 		r1, lr, 27
    xorrol 		r2, r9,  4
    xorrol 		r3, r10, 10
    xorrol 		r4, r11, 15
    xorrol 		r5, r12, 24
	xandnot		\sOut, _ma, r1, r2, r3, r6
	xandnot		\sOut, _me, r2, r3, r4, r6
	xandnot		\sOut, _mi, r3, r4, r5, r6
	xandnot		\sOut, _mo, r4, r5, r1, r6
	xandnot		\sOut, _mu, r5, r1, r2, r6

	ldr			r1, [\sIn, #_bi]
	ldr			r2, [\sIn, #_go]
	ldr			r3, [\sIn, #_ku]
	ldr			r4, [\sIn, #_ma]
	ldr			r5, [\sIn, #_se]
    xorrol 		r1, r11, 30
    xorrol 		r2, r12, 23
    xorrol 		r3, lr,  7
    xorrol 		r4, r9,  9
    xorrol 		r5, r10,  2
	xandnot		\sOut, _sa, r1, r2, r3, r6
	xandnot		\sOut, _se, r2, r3, r4, r6
	xandnot		\sOut, _si, r3, r4, r5, r6
	xandnot		\sOut, _so, r4, r5, r1, r6
	xandnot		\sOut, _su, r5, r1, r2, r6

	ldr			r1, [\sIn, #_ba]
	ldr			r2, [\sIn, #_ge]
	ldr			r3, [\sIn, #_ki]
	ldr			r4, [\sIn, #_mo]
	ldr			r5, [\sIn, #_su]
	xorrol		r1, r9, 0
	xorrol 		r2, r10, 12
	xorrol 		r3, r11, 11
	xorrol 		r4, r12, 21
	xorrol 		r5, lr, 14
	xandnot		\sOut, _be, r2, r3, r4, r6
	xandnot		\sOut, _bi, r3, r4, r5, r6
	xandnot		\sOut, _bo, r4, r5, r1, r6
	xandnot		\sOut, _bu, r5, r1, r2, r6
	xandnotRC	\sOut, _ba, r1, r2, r3
	.endm

@//----------------------------------------------------------------------------
@//
@// void KeccakF800_Initialize( void )
@//
.align 8
.global   KeccakF800_Initialize
KeccakF800_Initialize:  
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateInitialize(void *state)
@//
.align 8
.global   KeccakF800_StateInitialize
KeccakF800_StateInitialize:  
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


@//----------------------------------------------------------------------------
@//
@//	void KeccakF800_StateComplementBit(void *state, unsigned int position)
@//
.align 8
.global   KeccakF800_StateComplementBit
KeccakF800_StateComplementBit:  
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
@// void KeccakF800_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
@// 
.align 8
.global   KeccakF800_StateXORLanes
KeccakF800_StateXORLanes:  
	subs	r3, r2, #1
	bcc		KeccakF800_StateXORLanes_Exit
	push	{r4,r5}
	lsls	r4, r1, #30
	bne		KeccakF800_StateXORLanes_Unaligned
	lsrs	r2, r2, #1
	bcc		KeccakF800_StateXORLanes_Loop64
	ldmia	r1!, { r3 }
	ldr		r4, [r0]
	eors	r3, r3, r4
	stmia	r0!, { r3 }
	cmp		r2, #0
	beq		KeccakF800_StateXORLanes_ExitPop
KeccakF800_StateXORLanes_Loop64:
	ldmia	r1!, { r3 }
	ldr		r4, [r0]
	eors	r3, r3, r4
	ldmia	r1!, { r4 }
	ldr		r5, [r0, #4]
	eors	r4, r4, r5
	stmia	r0!, { r3, r4 }
	subs	r2, r2, #1
	bne		KeccakF800_StateXORLanes_Loop64
KeccakF800_StateXORLanes_ExitPop:
	pop		{r4,r5}
	bx		lr
KeccakF800_StateXORLanes_Unaligned:
	lsls	r2, r2, #2
	subs	r2, r2, #1
KeccakF800_StateXORLanes_Loop8:
	ldrb	r3, [r1, r2]
	ldrb	r4, [r0, r2]
	eors	r3, r3, r4
	strb	r3, [r0, r2]
	subs	r2, r2, #1
	bcs		KeccakF800_StateXORLanes_Loop8
	pop		{r4,r5}
KeccakF800_StateXORLanes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateXORBytesInLane
KeccakF800_StateXORBytesInLane:  
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF800_StateXORBytesInLane_Exit
	lsls	r1, r1, #2
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF800_StateXORBytesInLane_Loop:
	ldrb	r1, [r0, r4]
	ldrb	r3, [r2, r4]
	eors	r1, r1, r3
	strb	r1, [r0, r4]
	subs	r4, r4, #1
	bcs		KeccakF800_StateXORBytesInLane_Loop
KeccakF800_StateXORBytesInLane_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
@//
.align 8
.global   KeccakF800_StateOverwriteLanes
KeccakF800_StateOverwriteLanes: 
	subs	r2, r2, #1
	bcc		KeccakF800_StateOverwriteLanes_Exit
	lsls	r2, r2, #2
	lsls	r3, r1, #30
	bne		KeccakF800_StateOverwriteLanes_Unaligned
KeccakF800_StateOverwriteLanes_Loop32:
	ldr		r3, [r1, r2]
	str		r3, [r0, r2]
	subs	r2, r2, #4
	bcs		KeccakF800_StateOverwriteLanes_Loop32
	bx		lr
KeccakF800_StateOverwriteLanes_Unaligned:
	adds	r2, r2, #3
KeccakF800_StateOverwriteLanes_Loop8:
	ldrb	r3, [r1, r2]
	strb	r3, [r0, r2]
	subs	r2, r2, #1
	bcs		KeccakF800_StateOverwriteLanes_Loop8
KeccakF800_StateOverwriteLanes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateOverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateOverwriteBytesInLane
KeccakF800_StateOverwriteBytesInLane:
	adds	r0, r0, r3
	ldr		r3, [sp]
	subs	r3, r3, #1
	bcc		KeccakF800_StateOverwriteBytesInLane_Exit
	lsls	r1, r1, #2
	adds	r0, r0, r1
KeccakF800_StateOverwriteBytesInLane_Loop:
	ldrb	r1, [r2, r3]
	strb	r1, [r0, r3]
	subs	r3, r3, #1
	bcs		KeccakF800_StateOverwriteBytesInLane_Loop
KeccakF800_StateOverwriteBytesInLane_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
@//
.align 8
.global   KeccakF800_StateOverwriteWithZeroes
KeccakF800_StateOverwriteWithZeroes: 
	movs	r3, #0
	lsrs	r2, r1, #2
	beq		KeccakF800_StateOverwriteWithZeroes_Bytes
KeccakF800_StateOverwriteWithZeroes_LoopLanes:
	stm		r0!, { r3 }
	subs	r2, r2, #1
	bne		KeccakF800_StateOverwriteWithZeroes_LoopLanes
KeccakF800_StateOverwriteWithZeroes_Bytes:
	lsls	r1, r1, #32-2
	beq		KeccakF800_StateOverwriteWithZeroes_Exit
	lsrs	r1, r1, #32-2
KeccakF800_StateOverwriteWithZeroes_LoopBytes:
	subs	r1, r1, #1
	strb	r3, [r0, r1]
	bne		KeccakF800_StateOverwriteWithZeroes_LoopBytes
KeccakF800_StateOverwriteWithZeroes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
@//
.align 8
.global   KeccakF800_StateExtractLanes
KeccakF800_StateExtractLanes:  
	subs	r3, r2, #1
	bcc		KeccakF800_StateExtractLanes_Exit
	lsls	r3, r1, #30
	bne		KeccakF800_StateExtractLanes_Unaligned
	lsrs	r2, r2, #1
	bcc		KeccakF800_StateExtractLanes_64bit
	ldmia	r0!, { r3 }
	stmia	r1!, { r3 }
	beq		KeccakF800_StateExtractLanes_Exit
KeccakF800_StateExtractLanes_64bit:
	push	{r4-r6,lr}
	lsrs	r2, r2, #1
	bcc		KeccakF800_StateExtractLanes_Loop128
	ldmia	r0!, { r3-r4 }
	stmia	r1!, { r3-r4 }
	beq		KeccakF800_StateExtractLanes_ExitPop
KeccakF800_StateExtractLanes_Loop128:
	ldmia	r0!, { r3-r6 }
	stmia	r1!, { r3-r6 }
	subs	r2, r2, #1
	bne		KeccakF800_StateExtractLanes_Loop128
KeccakF800_StateExtractLanes_ExitPop:
	pop		{ r4-r6,pc }
KeccakF800_StateExtractLanes_Unaligned:
	lsls	r2, r2, #2
	subs	r2, r2, #1
KeccakF800_StateExtractLanes_Loop8:
	ldrb	r3, [r0, r2]
	strb	r3, [r1, r2]
	subs	r2, r2, #1
	bcs		KeccakF800_StateExtractLanes_Loop8
KeccakF800_StateExtractLanes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateExtractBytesInLane
KeccakF800_StateExtractBytesInLane:  
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF800_StateExtractBytesInLane_Exit
	lsls	r1, r1, #2
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF800_StateExtractBytesInLane_Loop:
	ldrb	r1, [r0, r4]
	strb	r1, [r2, r4]
	subs	r4, r4, #1
	bcs		KeccakF800_StateExtractBytesInLane_Loop
KeccakF800_StateExtractBytesInLane_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
@//
.align 8
.global   KeccakF800_StateExtractAndXORLanes
KeccakF800_StateExtractAndXORLanes:
	subs	r3, r2, #1
	bcc		KeccakF800_StateExtractAndXORLanes_Exit
	push	{r4,r5}
	lsls	r4, r1, #30
	bne		KeccakF800_StateExtractAndXORLanes_Unaligned
	lsrs	r2, r2, #1
	bcc		KeccakF800_StateExtractAndXORLanes_Loop64
	ldmia	r0!, { r4 }
	ldr		r3, [r1]
	eors	r3, r3, r4
	stmia	r1!, { r3 }
	cmp		r2, #0
	beq		KeccakF800_StateExtractAndXORLanes_ExitPop
KeccakF800_StateExtractAndXORLanes_Loop64:
	ldmia	r0!, { r4, r5 }
	ldr		r3, [r1]
	eors	r4, r4, r3
	ldr		r3, [r1, #4]
	eors	r5, r5, r3
	stmia	r1!, { r4, r5 }
	subs	r2, r2, #1
	bne		KeccakF800_StateExtractAndXORLanes_Loop64
KeccakF800_StateExtractAndXORLanes_ExitPop:
	pop		{r4,r5}
KeccakF800_StateExtractAndXORLanes_Exit:
	bx		lr
KeccakF800_StateExtractAndXORLanes_Unaligned:
	lsls	r2, r2, #2
	subs	r2, r2, #1
KeccakF800_StateExtractAndXORLanes_Loop8:
	ldrb	r3, [r1, r2]
	ldrb	r4, [r0, r2]
	eors	r3, r3, r4
	strb	r3, [r1, r2]
	subs	r2, r2, #1
	bcs		KeccakF800_StateExtractAndXORLanes_Loop8
	pop		{r4,r5}
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateExtractAndXORBytesInLane
KeccakF800_StateExtractAndXORBytesInLane:
	push	{r4,lr}
	ldr		r4, [sp, #8]
	subs	r4, r4, #1
	bcc		KeccakF800_StateExtractAndXORBytesInLane_Exit
	lsls	r1, r1, #2
	adds	r0, r0, r1
	adds	r0, r0, r3
KeccakF800_StateExtractAndXORBytesInLane_Loop:
	ldrb	r1, [r0, r4]
	ldrb	r3, [r2, r4]
	eors	r1, r1, r3
	strb	r1, [r2, r4]
	subs	r4, r4, #1
	bcs		KeccakF800_StateExtractAndXORBytesInLane_Loop
KeccakF800_StateExtractAndXORBytesInLane_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakP800_12_StatePermute( void *state )
@//
.align 8
.global   KeccakP800_12_StatePermute
KeccakP800_12_StatePermute:  
	adr		r1, KeccakP800_12_StatePermute_RoundConstants
	b		KeccakP800_StatePermute


.align 8
KeccakP800_12_StatePermute_RoundConstants:
		.long 			0x80008009
		.long 			0x8000000a
		.long 			0x8000808b
		.long 			0x0000008b
		.long 			0x00008089
		.long 			0x00008003
		.long 			0x00008002
		.long 			0x00000080
		.long 			0x0000800a
		.long 			0x8000000a
		.long 			0x80008081
		.long 			0x00008080 
		.long 			0xFF			@//terminator

@//----------------------------------------------------------------------------
@//
@// void KeccakP800_StatePermute( void *state, void *rc )
@//
.align 8
KeccakP800_StatePermute:  
	push	{ r4 - r6, lr }
	mov		r2, r8
	mov		r3, r9
	mov		r4, r10
	mov		r5, r11
	mov		r6, r12
	push	{ r2 - r7 }
	sub		sp, sp, #25*4+4
	mov		r8, r1
KeccakP800_StatePermute_RoundLoop:
	KeccakRound	sp, r0
	KeccakRound	r0, sp
	ldr		r6, [r6]
	cmp		r6, #0xFF
	beq		KeccakP800_StatePermute_Done
	b		KeccakP800_StatePermute_RoundLoop
KeccakP800_StatePermute_Done:
	add		sp,sp,#25*4+4
	pop		{ r2 - r7 }
	mov		r8, r2
	mov		r9, r3
	mov		r10, r4
	mov		r11, r5
	mov		r12, r6
	pop		{ r4 - r6, pc }


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Absorb(	void *state, unsigned int laneCount, unsigned char *data, 
@										size_t dataByteLen, unsigned char trailingBits )
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Absorb
KeccakP800_12_SnP_FBWL_Absorb:
	push	{ r4 - r6, lr }
	mov		r4, r8
	mov		r5, r9
	mov		r6, r10
	push	{ r4 - r7 }
	mov		r4, r11
	mov		r5, r12
	push	{ r4 - r5 }
	movs	r4, #0
	lsrs	r3, r3, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					@ .if (nbrLanes >== laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Absorb_Exit
KeccakP800_12_SnP_FBWL_Absorb_Loop:
	mov		r5, r1
	lsls	r6, r2, #30
	bne		KeccakP800_12_SnP_FBWL_Absorb_Unaligned_LoopLane
	lsrs	r5, r5, #1
	bcc		KeccakP800_12_SnP_FBWL_Absorb_Loop2Lanes
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r6, r6, r7
	stmia	r0!, { r6 }
	cmp		r5, #0
	beq		KeccakP800_12_SnP_FBWL_Absorb_TrailingBits
KeccakP800_12_SnP_FBWL_Absorb_Loop2Lanes:
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r6, r6, r7
	stmia	r0!, { r6 }
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r6, r6, r7
	stmia	r0!, { r6 }
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Absorb_Loop2Lanes
	b		KeccakP800_12_SnP_FBWL_Absorb_TrailingBits
KeccakP800_12_SnP_FBWL_Absorb_Unaligned_LoopLane:
	ldr		r6, [r0]
	ldrb	r7, [r2, #0]
	eors	r6, r6, r7
	ldrb	r7, [r2, #1]
	lsls	r7, r7, #8
	eors	r6, r6, r7
	ldrb	r7, [r2, #2]
	lsls	r7, r7, #16
	eors	r6, r6, r7
	ldrb	r7, [r2, #3]
	lsls	r7, r7, #24
	eors	r6, r6, r7
	stmia	r0!, { r6 }
	adds	r2, r2, #4
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Absorb_Unaligned_LoopLane
KeccakP800_12_SnP_FBWL_Absorb_TrailingBits:
	ldr		r6, [r0]
	ldr		r7, [sp, #(10+0)*4]
	eors	r6, r6, r7
	str		r6, [r0]
	lsls	r6, r1, #2
	subs	r0, r0, r6
	adds	r4, r4, r6					@ processed += laneCount * SnP_laneLengthInBytes
	push	{r1-r4}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r4}
	subs	r3, r3, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Absorb_Loop
KeccakP800_12_SnP_FBWL_Absorb_Exit:
	mov		r0, r4						@ return processed
	pop		{ r4 - r5 }
	mov		r11, r4 
	mov		r12, r5
	pop		{ r4 - r7 }
	mov		r8, r4
	mov		r9, r5
	mov		r10, r6
	pop		{ r4 - r6, pc }


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Squeeze
KeccakP800_12_SnP_FBWL_Squeeze:
	push	{ r4 - r6, lr }
	mov		r4, r8
	mov		r5, r9
	mov		r6, r10
	push	{ r4 - r7 }
	mov		r4, r11
	mov		r5, r12
	push	{ r4 - r5 }
	movs	r4, #0
	lsrs	r3, r3, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					@ .if (nbrLanes >== laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Squeeze_Exit
KeccakP800_12_SnP_FBWL_Squeeze_Loop:
	push	{r1-r4}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r4}
	mov		r5, r1
	lsls	r6, r2, #30
	bne		KeccakP800_12_SnP_FBWL_Squeeze_Unaligned_LoopLane

	subs	r5, r5, #4
	bcc		KeccakP800_12_SnP_FBWL_Squeeze_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Squeeze_Loop4Lanes:
	ldm		r0!, { r6, r7 }
	stm		r2!, { r6, r7 }
	ldm		r0!, { r6, r7 }
	stm		r2!, { r6, r7 }
	subs	r5, r5, #4
	bcs		KeccakP800_12_SnP_FBWL_Squeeze_Loop4Lanes
KeccakP800_12_SnP_FBWL_Squeeze_LessThan4Lanes:
	adds	r5, r5, #4
	beq		KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop
KeccakP800_12_SnP_FBWL_Squeeze_LoopLane:
	ldm		r0!, { r6 }
	stm		r2!, { r6 }
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Squeeze_LoopLane
KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop:
	lsls	r6, r1, #2
	adds	r4, r4, r6					@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r0, r0, r6
	subs	r3, r3, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Squeeze_Loop
KeccakP800_12_SnP_FBWL_Squeeze_Exit:
	mov		r0, r4
	pop		{ r4 - r5 }
	mov		r11, r4 
	mov		r12, r5
	pop		{ r4 - r7 }
	mov		r8, r4
	mov		r9, r5
	mov		r10, r6
	pop		{ r4 - r6, pc }
KeccakP800_12_SnP_FBWL_Squeeze_Unaligned_LoopLane:
	ldm		r0!, { r6 }
	strb	r6, [r2, #0]
	lsrs	r6, r6, #8
	strb	r6, [r2, #1]
	lsrs	r6, r6, #8
	strb	r6, [r2, #2]
	lsrs	r6, r6, #8
	strb	r6, [r2, #3]
	adds	r2, r2, #4
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Squeeze_Unaligned_LoopLane
	b		KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
@										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Wrap
KeccakP800_12_SnP_FBWL_Wrap:
	push	{ r4 - r6, lr }
	mov		r4, r8
	mov		r5, r9
	mov		r6, r10
	push	{ r4 - r7 }
	mov		r4, r11
	mov		r5, r12
	push	{ r4 - r5 }
	ldr		r5, [sp, #(10+0)*4]			@ dataByteLen
	lsrs	r5, r5, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	movs	r4, #0
	subs	r5, r5, r1					@ .if (nbrLanes >== laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Wrap_Exit
KeccakP800_12_SnP_FBWL_Wrap_Loop:
	mov		r8, r5
	mov		r5, r1
	lsls	r6, r2, #30
	bne		KeccakP800_12_SnP_FBWL_Wrap_Unaligned_LoopLane
	lsls	r6, r3, #30
	bne		KeccakP800_12_SnP_FBWL_Wrap_Unaligned_LoopLane
	lsrs	r5, r5, #1
	bcc		KeccakP800_12_SnP_FBWL_Wrap_Loop2Lanes
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r6, r6, r7
	stmia	r3!, { r6 }
	stmia	r0!, { r6 }
	cmp		r5, #0
	beq		KeccakP800_12_SnP_FBWL_Wrap_TrailingBits
KeccakP800_12_SnP_FBWL_Wrap_Loop2Lanes:
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r6, r6, r7
	stmia	r3!, { r6 }
	stmia	r0!, { r6 }
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r6, r6, r7
	stmia	r3!, { r6 }
	stmia	r0!, { r6 }
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Wrap_Loop2Lanes
	b		KeccakP800_12_SnP_FBWL_Wrap_TrailingBits
KeccakP800_12_SnP_FBWL_Wrap_Unaligned_LoopLane:
	ldr		r6, [r0]
	ldrb	r7, [r2, #0]
	eors	r6, r6, r7
	ldrb	r7, [r2, #1]
	lsls	r7, r7, #8
	eors	r6, r6, r7
	ldrb	r7, [r2, #2]
	lsls	r7, r7, #16
	eors	r6, r6, r7
	ldrb	r7, [r2, #3]
	lsls	r7, r7, #24
	eors	r6, r6, r7
	stmia	r0!, { r6 }
	strb	r6, [r3, #0]
	lsrs	r6, r6, #8
	strb	r6, [r3, #1]
	lsrs	r6, r6, #8
	strb	r6, [r3, #2]
	lsrs	r6, r6, #8
	strb	r6, [r3, #3]
	adds	r2, r2, #4
	adds	r3, r3, #4
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Wrap_Unaligned_LoopLane
KeccakP800_12_SnP_FBWL_Wrap_TrailingBits:
	ldr		r6, [r0]
	ldr		r7, [sp, #(10+1)*4]
	eors	r6, r6, r7
	str		r6, [r0]
	lsls	r6, r1, #2
	adds	r4, r4, r6					@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r0, r0, r6
	mov		r5, r8
	push	{r1-r6}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r6}
	subs	r5, r5, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Wrap_Loop
KeccakP800_12_SnP_FBWL_Wrap_Exit:
	mov		r0, r4
	pop		{ r4 - r5 }
	mov		r11, r4 
	mov		r12, r5
	pop		{ r4 - r7 }
	mov		r8, r4
	mov		r9, r5
	mov		r10, r6
	pop		{ r4 - r6, pc }


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
@										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Unwrap
KeccakP800_12_SnP_FBWL_Unwrap:
	push	{ r4 - r6, lr }
	mov		r4, r8
	mov		r5, r9
	mov		r6, r10
	push	{ r4 - r7 }
	mov		r4, r11
	mov		r5, r12
	push	{ r4 - r5 }
	ldr		r5, [sp, #(10+0)*4]			@ dataByteLen
	lsrs	r5, r5, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	movs	r4, #0
	subs	r5, r5, r1					@ .if (nbrLanes >== laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Unwrap_Exit
KeccakP800_12_SnP_FBWL_Unwrap_Loop:
	mov		r8, r5
	mov		r5, r1
	lsls	r6, r2, #30
	bne		KeccakP800_12_SnP_FBWL_Unwrap_Unaligned_LoopLane
	lsls	r6, r3, #30
	bne		KeccakP800_12_SnP_FBWL_Unwrap_Unaligned_LoopLane
	lsrs	r5, r5, #1
	bcc		KeccakP800_12_SnP_FBWL_Unwrap_Loop2Lanes
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r7, r7, r6
	stmia	r3!, { r7 }
	stmia	r0!, { r6 }
	cmp		r5, #0
	beq		KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits
KeccakP800_12_SnP_FBWL_Unwrap_Loop2Lanes:
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r7, r7, r6
	stmia	r3!, { r7 }
	stmia	r0!, { r6 }
	ldmia	r2!, { r6 }
	ldr		r7, [r0]
	eors	r7, r7, r6
	stmia	r3!, { r7 }
	stmia	r0!, { r6 }
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Unwrap_Loop2Lanes
	b		KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits
KeccakP800_12_SnP_FBWL_Unwrap_Unaligned_LoopLane:
	ldrb	r7, [r2, #0]
	ldrb	r6, [r2, #1]
	lsls	r6, r6, #8
	orrs	r7, r7, r6
	ldrb	r6, [r2, #2]
	lsls	r6, r6, #16
	orrs	r7, r7, r6
	ldrb	r6, [r2, #3]
	lsls	r6, r6, #24
	orrs	r7, r7, r6
	ldr		r6, [r0]
	eors	r6, r6, r7
	stmia	r0!, { r7 }
	strb	r6, [r3, #0]
	lsrs	r6, r6, #8
	strb	r6, [r3, #1]
	lsrs	r6, r6, #8
	strb	r6, [r3, #2]
	lsrs	r6, r6, #8
	strb	r6, [r3, #3]
	adds	r2, r2, #4
	adds	r3, r3, #4
	subs	r5, r5, #1
	bne		KeccakP800_12_SnP_FBWL_Unwrap_Unaligned_LoopLane
KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits:
	ldr		r6, [r0]
	ldr		r7, [sp, #(10+1)*4]
	eors	r6, r6, r7
	str		r6, [r0]
	lsls	r6, r1, #2
	adds	r4, r4, r6					@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r0, r0, r6
	mov		r5, r8
	push	{r1-r6}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r6}
	subs	r5, r5, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Unwrap_Loop
KeccakP800_12_SnP_FBWL_Unwrap_Exit:
	mov		r0, r4
	pop		{ r4 - r5 }
	mov		r11, r4 
	mov		r12, r5
	pop		{ r4 - r7 }
	mov		r8, r4
	mov		r9, r5
	mov		r10, r6
	pop		{ r4 - r6, pc }


