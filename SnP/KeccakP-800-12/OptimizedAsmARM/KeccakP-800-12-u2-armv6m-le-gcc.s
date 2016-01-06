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
	.syntax unified
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

.macro	te 			d, a, b
	rors		\b, \b, r4
	eors		\b, \b, \a
	mov			\d, \b
	.endm

.macro	trp		 	rBx, sIn, oIn, rD, rot
	ldr			\rBx, [\sIn, #\oIn]
	mov			r6, \rD
	eors		\rBx, \rBx, r6
	.if			\rot != 0
	movs		r6, #32-\rot
	rors		\rBx, \rBx, r6
	.endif
	.endm

.macro	ci	 		sOut, oOut, ax0, ax1, ax2, iota, useax2, temp, earlyT
	.if \useax2 != 0
	bics		\ax2, \ax2, \ax1
	eors		\ax2, \ax2, \ax0
	.if \iota != 0
	mov			\temp, r8
	ldm			\temp!, { \ax1 }
	mov			r8, \temp
	eors		\ax2, \ax2, \ax1
	.endif
	str			\ax2, [\sOut, #\oOut]
	.else
	movs		\temp, \ax2
	bics		\temp, \temp, \ax1
	eors		\temp, \temp, \ax0
	str			\temp, [\sOut, #\oOut]
	.if \earlyT != 0
	eors		r7, \temp
	.endif
	.endif
	.endm

.macro	KeccakRound 	sOut, sIn

	@// Prepare Theta effect (U column already done by earlyTheta
	movs	r4, #31
    xor5	r1, \sIn, _be, _ge, _ke, _me, _se
	mov		r6, r1
	te		r9, r7, r6
    xor5	r2, \sIn, _bi, _gi, _ki, _mi, _si
    te		r12, r2, r7
    xor5	r3, \sIn, _ba, _ga, _ka, _ma, _sa
    te		r10, r3, r2
    xor5	r2, \sIn, _bo, _go, _ko, _mo, _so
    te		lr, r2, r3
    te		r11, r1, r2

	@// ThetaRhoPi ChiIota
    trp 	r1, \sIn, _bo, r12, 28
    trp 	r2, \sIn, _gu, lr, 20
    trp 	r3, \sIn, _ka, r9,  3
    trp 	r4, \sIn, _me, r10, 13
    trp 	r5, \sIn, _si, r11, 29
	ci		\sOut, _gu, r5, r1, r2, 0, 0, r7, 0
	ci		\sOut, _go, r4, r5, r1, 0, 0, r6, 0
	ci		\sOut, _gi, r3, r4, r5, 0, 1, r6, 0
	ci		\sOut, _ge, r2, r3, r4, 0, 1, r6, 0
	ci		\sOut, _ga, r1, r2, r3, 0, 1, r6, 0

    trp 	r1, \sIn, _be, r10,  1
    trp 	r2, \sIn, _gi, r11,  6
    trp 	r3, \sIn, _ko, r12, 25
    trp 	r4, \sIn, _mu, lr,  8
    trp 	r5, \sIn, _sa, r9, 18
	ci		\sOut, _ku, r5, r1, r2, 0, 0, r6, 1
	ci		\sOut, _ko, r4, r5, r1, 0, 0, r6, 0
	ci		\sOut, _ki, r3, r4, r5, 0, 1, r6, 0
	ci		\sOut, _ke, r2, r3, r4, 0, 1, r6, 0
	ci		\sOut, _ka, r1, r2, r3, 0, 1, r6, 0

    trp 	r1, \sIn, _bu, lr, 27
    trp 	r2, \sIn, _ga, r9,  4
    trp 	r3, \sIn, _ke, r10, 10
    trp 	r4, \sIn, _mi, r11, 15
    trp 	r5, \sIn, _so, r12, 24
	ci		\sOut, _mu, r5, r1, r2, 0, 0, r6, 1
	ci		\sOut, _mo, r4, r5, r1, 0, 0, r6, 0
	ci		\sOut, _mi, r3, r4, r5, 0, 1, r6, 0
	ci		\sOut, _me, r2, r3, r4, 0, 1, r6, 0
	ci		\sOut, _ma, r1, r2, r3, 0, 1, r6, 0

    trp 	r1, \sIn, _bi, r11, 30
    trp 	r2, \sIn, _go, r12, 23
    trp 	r3, \sIn, _ku, lr,  7
    trp 	r4, \sIn, _ma, r9,  9
    trp 	r5, \sIn, _se, r10,  2
	ci		\sOut, _su, r5, r1, r2, 0, 0, r6, 1
	ci		\sOut, _so, r4, r5, r1, 0, 0, r6, 0
	ci		\sOut, _si, r3, r4, r5, 0, 1, r6, 0
	ci		\sOut, _se, r2, r3, r4, 0, 1, r6, 0
	ci		\sOut, _sa, r1, r2, r3, 0, 1, r6, 0

	trp		r1, \sIn, _ba, r9, 0
	trp 	r2, \sIn, _ge, r10, 12
	trp 	r3, \sIn, _ki, r11, 11
	trp 	r4, \sIn, _mo, r12, 21
	trp 	r5, \sIn, _su, lr, 14
	ci		\sOut, _bu, r5, r1, r2, 0, 0, r6, 1
	ci		\sOut, _bo, r4, r5, r1, 0, 0, r6, 0
	ci		\sOut, _bi, r3, r4, r5, 0, 1, r6, 0
	ci		\sOut, _be, r2, r3, r4, 0, 1, r6, 0
	ci		\sOut, _ba, r1, r2, r3, 1, 1, r6, 0
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
@// void KeccakF800_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateXORBytes
KeccakF800_StateXORBytes:
	push	{r4,lr}
	adds	r0, r0, r2								@ state += offset
	subs	r3, r3, #4								@ .if length >= 4
	bcc		KeccakF800_StateXORBytes_Bytes
	movs	r2, r0									@ and data pointer and offset both 32-bit aligned
	orrs	r2, r2, r1
	lsls	r2, #30
	bne		KeccakF800_StateXORBytes_Bytes
KeccakF800_StateXORBytes_LanesLoop:					@ then, perform on words
	ldr		r2, [r0]
	ldmia	r1!, {r4}
	eors	r2, r2, r4
	stmia	r0!, {r2}
	subs	r3, r3, #4
	bcs		KeccakF800_StateXORBytes_LanesLoop
KeccakF800_StateXORBytes_Bytes:
	adds	r3, r3, #4
	beq		KeccakF800_StateXORBytes_Exit
	subs	r3, r3, #1
KeccakF800_StateXORBytes_BytesLoop:
	ldrb	r2, [r0, r3]
	ldrb	r4, [r1, r3]
	eors	r2, r2, r4
	strb	r2, [r0, r3]
	subs	r3, r3, #1
	bcs		KeccakF800_StateXORBytes_BytesLoop
KeccakF800_StateXORBytes_Exit:
	pop		{r4,pc}


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateOverwriteBytes
KeccakF800_StateOverwriteBytes:
	adds	r0, r0, r2								@ state += offset
	subs	r3, r3, #4								@ .if length >= 4
	bcc		KeccakF800_StateOverwriteBytes_Bytes
	movs	r2, r0									@ and data pointer and offset both 32-bit aligned
	orrs	r2, r2, r1
	lsls	r2, #30
	bne		KeccakF800_StateOverwriteBytes_Bytes
KeccakF800_StateOverwriteBytes_LanesLoop:			@ then, perform on words
	ldmia	r1!, {r2}
	stmia	r0!, {r2}
	subs	r3, r3, #4
	bcs		KeccakF800_StateOverwriteBytes_LanesLoop
KeccakF800_StateOverwriteBytes_Bytes:
	adds	r3, r3, #4
	beq		KeccakF800_StateOverwriteBytes_Exit
	subs	r3, r3, #1
KeccakF800_StateOverwriteBytes_BytesLoop:
	ldrb	r2, [r1, r3]
	strb	r2, [r0, r3]
	subs	r3, r3, #1
	bcs		KeccakF800_StateOverwriteBytes_BytesLoop
KeccakF800_StateOverwriteBytes_Exit:
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
@// void KeccakF800_StateExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateExtractBytes
KeccakF800_StateExtractBytes:
	adds	r0, r0, r2								@ state += offset
	subs	r3, r3, #4								@ .if length >= 4
	bcc		KeccakF800_StateExtractBytes_Bytes
	movs	r2, r0									@ and data pointer and offset both 32-bit aligned
	orrs	r2, r2, r1
	lsls	r2, #30
	bne		KeccakF800_StateExtractBytes_Bytes
KeccakF800_StateExtractBytes_LanesLoop:				@ then, perform on words
	ldmia	r0!, {r2}
	stmia	r1!, {r2}
	subs	r3, r3, #4
	bcs		KeccakF800_StateExtractBytes_LanesLoop
KeccakF800_StateExtractBytes_Bytes:
	adds	r3, r3, #4
	beq		KeccakF800_StateExtractBytes_Exit
	subs	r3, r3, #1
KeccakF800_StateExtractBytes_BytesLoop:
	ldrb	r2, [r0, r3]
	strb	r2, [r1, r3]
	subs	r3, r3, #1
	bcs		KeccakF800_StateExtractBytes_BytesLoop
KeccakF800_StateExtractBytes_Exit:
	bx		lr


@//----------------------------------------------------------------------------
@//
@// void KeccakF800_StateExtractAndXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@//
.align 8
.global   KeccakF800_StateExtractAndXORBytes
KeccakF800_StateExtractAndXORBytes:
	push	{r4,lr}
	adds	r0, r0, r2								@ state += offset
	subs	r3, r3, #4								@ .if length >= 4
	bcc		KeccakF800_StateExtractAndXORBytes_Bytes
	movs	r2, r0									@ and data pointer and offset both 32-bit aligned
	orrs	r2, r2, r1
	lsls	r2, #30
	bne		KeccakF800_StateExtractAndXORBytes_Bytes
KeccakF800_StateExtractAndXORBytes_LanesLoop:					@ then, perform on words
	ldmia	r0!, {r2}
	ldr		r4, [r1]
	eors	r2, r2, r4
	stmia	r1!, {r2}
	subs	r3, r3, #4
	bcs		KeccakF800_StateExtractAndXORBytes_LanesLoop
KeccakF800_StateExtractAndXORBytes_Bytes:
	adds	r3, r3, #4
	beq		KeccakF800_StateExtractAndXORBytes_Exit
	subs	r3, r3, #1
KeccakF800_StateExtractAndXORBytes_BytesLoop:
	ldrb	r2, [r0, r3]
	ldrb	r4, [r1, r3]
	eors	r2, r2, r4
	strb	r2, [r1, r3]
	subs	r3, r3, #1
	bcs		KeccakF800_StateExtractAndXORBytes_BytesLoop
KeccakF800_StateExtractAndXORBytes_Exit:
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
    xor5	r7, r0, _bu, _gu, _ku, _mu, _su
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
	subs	r3, r3, r1					@ .if (nbrLanes >= laneCount)
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
	subs	r3, r3, r1					@ .if (nbrLanes >= laneCount)
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
	subs	r5, r5, r1					@ .if (nbrLanes >= laneCount)
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
	subs	r5, r5, r1					@ .if (nbrLanes >= laneCount)
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


