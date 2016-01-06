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

@ WARNING: These functions work only on little endian CPU with@ ARMv7m architecture (Cortex-M3, ...).


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
.equ _RFU, 25*4
.equ _SAS, 26*4

@//----------------------------------------------------------------------------

.macro	xor5	result,ptr,rb,g,k,m,s
	ldr		\result, [\ptr, #\g]
	eors	\result, \result, \rb
	ldr		\rb, [\ptr, #\k]
	eors	\result, \result, \rb
	ldr		\rb, [\ptr, #\m]
	eors	\result, \result, \rb
	ldr		\rb, [\ptr, #\s]
	eors	\result, \result, \rb
	.endm

.macro	mTe		b, yy, rr
	eors	\b, \b, \yy
	.if		\rr != 0
	ror		\b, \b, #32-\rr
	.endif
	.endm

.macro	mCI 	resptr, resofs, ax0, ax1, ax2, temp, iota
	bics	\temp, \ax2, \ax1
	eors	\temp, \temp, \ax0
	.if 		\iota < 0x100
	eors	\temp, \temp, #\iota
	.else
	mov		\ax1, #\iota & 0xFFFF
	.if		\iota >= 0x10000
	movt	\ax1, #\iota >> 16
	.endif
	eors	\temp, \temp, \ax1
	.endif
	str		\temp, [\resptr, #\resofs]
	.endm

.macro	mC	 	resptr, resofs, ax0, ax1, ax2, temp, pTxor, pTreg, save
	bics	\temp, \ax2, \ax1
	eors	\temp, \temp, \ax0
	.if		\save != 0
	str		\temp, [\resptr, #\resofs]
	.endif
	.if 		\pTxor != 0
	eors	\pTreg, \pTreg, \temp
	.endif
	.endm

.macro	mKR		stateOut,stateIn,iota

	@ prepare Theta
    xor5	r1, \stateIn, r9, _ga, _ka, _ma, _sa
    xor5	r2, \stateIn, r10, _ge, _ke, _me, _se
	eor		r9, r7, r2, ROR #31
    eor		r10, r1, r6, ROR #31
    eor		r11, r2, r8, ROR #31
    eor		r12, r6, r7, ROR #31
    eor		lr, r8, r1, ROR #31

	@ Theta Rho Pi Chi Iota
	eors	r1, r3, r11
	rors	r1, r1, #32-30
	ldr		r2, [\stateIn, #_go]
	ldr		r3, [\stateIn, #_ku]
	ldr		r4, [\stateIn, #_ma]
	ldr		r5, [\stateIn, #_se]
    mTe		r2, r12, 23
    mTe		r3, lr,  7
    mTe		r4, r9,  9
    mTe		r5, r10,  2
	mC		\stateOut, _su, r5, r1, r2, r7, 0, 0,   1
	mC		\stateOut, _so, r4, r5, r1, r8, 0, 0,   1
	mC		\stateOut, _si, r3, r4, r5, r6, 0, 0,   1
	mC		\stateOut, _se, r2, r3, r4, r4, 0, 0,   1
	mC		\stateOut, _sa, r1, r2, r3, r3, 0, 0,   1

	ldr		r1, [\stateIn, #_bu]
	ldr		r2, [\stateIn, #_ga]
	ldr		r4, [\stateIn, #_mi]
	ldr		r5, [\stateIn, #_so]
    mTe		r1, lr, 27
    mTe		r2, r9,  4
    mTe		r4, r11, 15
    mTe		r5, r12, 24
	mC		\stateOut, _mu, r5, r1, r2, r3, 1, r7, 1
	mC		\stateOut, _mo, r4, r5, r1, r3, 1, r8, 1
	ldr		r3, [\stateIn, #_ke]
    mTe		r3, r10, 10
	mC		\stateOut, _mi, r3, r4, r5, r5, 1, r6, 1
	mC		\stateOut, _me, r2, r3, r4, r4, 0, 0,   1
	mC		\stateOut, _ma, r1, r2, r3, r3, 0, 0,   1

	ldr		r1, [\stateIn, #_be]
	ldr		r2, [\stateIn, #_gi]
	ldr		r4, [\stateIn, #_mu]
	ldr		r5, [\stateIn, #_sa]
    mTe		r1, r10,  1
    mTe		r2, r11,  6
    mTe		r4, lr,  8
    mTe		r5, r9, 18
	mC		\stateOut, _ku, r5, r1, r2, r3, 1, r7, 1
	mC		\stateOut, _ko, r4, r5, r1, r3, 1, r8, 1
	ldr		r3, [\stateIn, #_ko]
    mTe		r3, r12, 25
	mC		\stateOut, _ki, r3, r4, r5, r5, 1, r6, 1
	mC		\stateOut, _ke, r2, r3, r4, r4, 0, 0,   1
	mC		\stateOut, _ka, r1, r2, r3, r3, 0, 0,   1

	ldr		r1, [\stateIn, #_bo]
	ldr		r2, [\stateIn, #_gu]
	ldr		r4, [\stateIn, #_me]
	ldr		r5, [\stateIn, #_si]
    mTe		r1, r12, 28
    mTe		r2, lr, 20
    mTe		r4, r10, 13
    mTe		r5, r11, 29
	mC		\stateOut, _gu, r5, r1, r2, r3, 1, r7, 1
	mC		\stateOut, _go, r4, r5, r1, r3, 1, r8, 1
	ldr		r3, [\stateIn, #_ka]
    mTe		r3, r9,  3
	mC		\stateOut, _gi, r3, r4, r5, r5, 1, r6, 1
	mC		\stateOut, _ge, r2, r3, r4, r4, 0, 0,   1
	mC		\stateOut, _ga, r1, r2, r3, r3, 0, 0,   1

	ldr		r1, [\stateIn, #_ba]
	ldr		r2, [\stateIn, #_ge]
	ldr		r3, [\stateIn, #_ki]
	ldr		r4, [\stateIn, #_mo]
	ldr		r5, [\stateIn, #_su]
	mTe		r1, r9,  0
	mTe		r2, r10, 12
	mTe		r3, r11, 11
	mTe		r4, r12, 21
	mTe		r5, lr, 14
	mC		\stateOut, _bu, r5, r1, r2, lr, 1, r7, 1
	mC		\stateOut, _bo, r4, r5, r1, r12, 1, r8, 1
	mC		\stateOut, _bi, r3, r4, r5, r11, 1, r6, 0
	mC		\stateOut, _be, r2, r3, r4, r10, 0, 0,   1
	mCI		\stateOut, _ba, r1, r2, r3, r9, \iota
	mov		r3, r11
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
	lsrs	r2, r1, #5
	add		r0, r2, LSL #2
	ldr		r2, [r0]
	and		r1, r1, #0x1F
	movs	r3, #1
	lsls	r3, r3, r1
	eors	r3, r3, r2
	str		r3, [r0]
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
KeccakF800_StateXORBytes_LanesLoop:					@ then, perform on words
	ldr		r2, [r0]
	ldr		r4, [r1], #4
	eors	r2, r2, r4
	str		r2, [r0], #4
	subs	r3, r3, #4
	bcs		KeccakF800_StateXORBytes_LanesLoop
KeccakF800_StateXORBytes_Bytes:
	adds	r3, r3, #3
	bcc		KeccakF800_StateXORBytes_Exit
KeccakF800_StateXORBytes_BytesLoop:
	ldrb	r2, [r0]
	ldrb	r4, [r1], #1
	eors	r2, r2, r4
	strb	r2, [r0], #1
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
KeccakF800_StateOverwriteBytes_LanesLoop:			@ then, perform on words
	ldr		r2, [r1], #4
	str		r2, [r0], #4
	subs	r3, r3, #4
	bcs		KeccakF800_StateOverwriteBytes_LanesLoop
KeccakF800_StateOverwriteBytes_Bytes:
	adds	r3, r3, #3
	bcc		KeccakF800_StateOverwriteBytes_Exit
KeccakF800_StateOverwriteBytes_BytesLoop:
	ldrb	r2, [r1], #1
	strb	r2, [r0], #1
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
	str		r3, [r0], #4
	subs	r2, r2, #1
	bne		KeccakF800_StateOverwriteWithZeroes_LoopLanes
KeccakF800_StateOverwriteWithZeroes_Bytes:
	ands	r1, #3
	beq		KeccakF800_StateOverwriteWithZeroes_Exit
KeccakF800_StateOverwriteWithZeroes_LoopBytes:
	strb	r3, [r0], #1
	subs	r1, r1, #1
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
KeccakF800_StateExtractBytes_LanesLoop:				@ then, handle words
	ldr		r2, [r0], #4
	str		r2, [r1], #4
	subs	r3, r3, #4
	bcs		KeccakF800_StateExtractBytes_LanesLoop
KeccakF800_StateExtractBytes_Bytes:
	adds	r3, r3, #3
	bcc		KeccakF800_StateExtractBytes_Exit
KeccakF800_StateExtractBytes_BytesLoop:
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
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
	adds	r0, r0, r2									@ state += offset
	subs	r3, r3, #4									@ .if length >= 4
	bcc		KeccakF800_StateExtractAndXORBytes_Bytes
KeccakF800_StateExtractAndXORBytes_LanesLoop:			@ then, handle words
	ldr		r2, [r0], #4
	ldr		r4, [r1]
	eors	r2, r2, r4
	str		r2, [r1], #4
	subs	r3, r3, #4
	bcs		KeccakF800_StateExtractAndXORBytes_LanesLoop
KeccakF800_StateExtractAndXORBytes_Bytes:
	adds	r3, r3, #3
	bcc		KeccakF800_StateExtractAndXORBytes_Exit
KeccakF800_StateExtractAndXORBytes_BytesLoop:
	ldrb	r2, [r0], #1
	ldrb	r4, [r1]
	eors	r2, r2, r4
	strb	r2, [r1], #1
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
	push	{r4-r12,lr}
	sub		sp, sp, #_SAS
	ldm		r0, {r9,r10,r11,r12,lr}
	mov		r3, r11
    xor5	r7, r0, lr, _gu, _ku, _mu, _su
    xor5	r8, r0, r12, _go, _ko, _mo, _so
    xor5	r6, r0, r11, _gi, _ki, _mi, _si
	mKR		sp, r0, 0x80008009
	mKR		r0, sp, 0x8000000a
	mKR		sp, r0, 0x8000808b
	mKR		r0, sp, 0x0000008b
	mKR		sp, r0, 0x00008089
	mKR		r0, sp, 0x00008003
	mKR		sp, r0, 0x00008002
	mKR		r0, sp, 0x00000080
	mKR		sp, r0, 0x0000800a
	mKR		r0, sp, 0x8000000a
	mKR		sp, r0, 0x80008081
	mKR		r0, sp, 0x00008080
	str		r11, [r0, #_bi]
	add		sp,sp,#_SAS
	pop		{r4-r12,pc}


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Absorb(	void *state, unsigned int laneCount, unsigned char *data,
@										size_t dataByteLen, unsigned char trailingBits )
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Absorb
KeccakP800_12_SnP_FBWL_Absorb:
	push	{r4-r12,lr}
	mov		r4, #0
	lsr		r3, r3, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					@ .if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Absorb_Exit
KeccakP800_12_SnP_FBWL_Absorb_Loop:
	add		r4, r4, r1, LSL #2			@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r6, r1, #4
	bcc		KeccakP800_12_SnP_FBWL_Absorb_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Absorb_Loop4Lanes:
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
KeccakP800_12_SnP_FBWL_Absorb_LessThan4Lanes:
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Absorb_TrailingBits
KeccakP800_12_SnP_FBWL_Absorb_LoopLane:
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r11, r11, r12
	str		r11, [r0], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Absorb_LoopLane
KeccakP800_12_SnP_FBWL_Absorb_TrailingBits:
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+0)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4}
	bl		KeccakP800_12_StatePermute
	pop	{r1-r4}
	subs	r3, r3, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Absorb_Loop
KeccakP800_12_SnP_FBWL_Absorb_Exit:
	mov		r0, r4
	pop		{r4-r12,pc}


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Squeeze
KeccakP800_12_SnP_FBWL_Squeeze:
	push	{r4-r12,lr}
	mov		r4, #0
	lsr		r3, r3, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r3, r3, r1					@ .if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Squeeze_Exit
KeccakP800_12_SnP_FBWL_Squeeze_Loop:
	push	{r1-r4}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r4}
	mov		r6, r1
	add		r4, r4, r1, LSL #2			@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r6, r6, #4
	bcc		KeccakP800_12_SnP_FBWL_Squeeze_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Squeeze_Loop4Lanes:
	ldrd	r10, r11, [r0], #8
	str		r10, [r2], #4
	str		r11, [r2], #4
	ldrd	r10, r11, [r0], #8
	str		r10, [r2], #4
	str		r11, [r2], #4
	subs	r6, r6, #4
	bcs		KeccakP800_12_SnP_FBWL_Squeeze_Loop4Lanes
KeccakP800_12_SnP_FBWL_Squeeze_LessThan4Lanes:
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop
KeccakP800_12_SnP_FBWL_Squeeze_LoopLane:
	ldr		r11, [r0], #4
	str		r11, [r2], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Squeeze_LoopLane
KeccakP800_12_SnP_FBWL_Squeeze_CheckLoop:
	sub		r0, r0, r1, LSL #2
	subs	r3, r3, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Squeeze_Loop
KeccakP800_12_SnP_FBWL_Squeeze_Exit:
	mov		r0, r4
	pop		{r4-r12,pc}


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn,
@										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Wrap
KeccakP800_12_SnP_FBWL_Wrap:
	push	{r4-r12,lr}
	ldr		r8, [sp, #(10+0)*4]			@ dataByteLen
	mov		r4, #0
	lsr		r8, r8, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1					@ .if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Wrap_Exit
KeccakP800_12_SnP_FBWL_Wrap_Loop:
	add		r4, r4, r1, LSL #2			@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r6, r1, #4
	bcc		KeccakP800_12_SnP_FBWL_Wrap_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Wrap_Loop4Lanes:
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
KeccakP800_12_SnP_FBWL_Wrap_LessThan4Lanes:
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Wrap_TrailingBits
KeccakP800_12_SnP_FBWL_Wrap_LoopLane:
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r11, r11, r12
	str		r11, [r0], #4
	str		r11, [r3], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Wrap_LoopLane
KeccakP800_12_SnP_FBWL_Wrap_TrailingBits:
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+1)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4,r8-r9}
	bl		KeccakP800_12_StatePermute
	pop	{r1-r4,r8-r9}
	subs	r8, r8, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Wrap_Loop
KeccakP800_12_SnP_FBWL_Wrap_Exit:
	mov		r0, r4
	pop		{r4-r12,pc}


@----------------------------------------------------------------------------
@
@ size_t KeccakP800_12_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn,
@										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
@
.align 8
.global 	KeccakP800_12_SnP_FBWL_Unwrap
KeccakP800_12_SnP_FBWL_Unwrap:
	push	{r4-r12,lr}
	ldr		r8, [sp, #(10+0)*4]				@ dataByteLen
	mov		r4, #0
	lsr		r8, r8, #2					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1					@ .if (nbrLanes >= laneCount)
	bcc		KeccakP800_12_SnP_FBWL_Unwrap_Exit
KeccakP800_12_SnP_FBWL_Unwrap_Loop:
	add		r4, r4, r1, LSL #2			@ processed += laneCount*SnP_laneLengthInBytes@
	subs	r6, r1, #4
	bcc		KeccakP800_12_SnP_FBWL_Unwrap_LessThan4Lanes
KeccakP800_12_SnP_FBWL_Unwrap_Loop4Lanes:
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
KeccakP800_12_SnP_FBWL_Unwrap_LessThan4Lanes:
	adds	r6, r6, #4
	beq		KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits
KeccakP800_12_SnP_FBWL_Unwrap_LoopLane:
	ldr		r11, [r2], #4
	ldr		r12, [r0]
	eor		r12, r12, r11
	str		r12, [r3], #4
	str		r11, [r0], #4
	subs	r6, r6, #1
	bne		KeccakP800_12_SnP_FBWL_Unwrap_LoopLane
KeccakP800_12_SnP_FBWL_Unwrap_TrailingBits:
	ldr		r11, [r0]
	ldrb	r12, [sp, #(10+1)*4]
	eor		r11, r11, r12
	str		r11, [r0]
	sub		r0, r0, r1, LSL #2
	push	{r1-r4,r8-r9}
	bl		KeccakP800_12_StatePermute
	pop		{r1-r4,r8-r9}
	subs	r8, r8, r1					@ rx (nbrLanes) = dataByteLen / SnP_laneLengthInBytes
	bcs		KeccakP800_12_SnP_FBWL_Unwrap_Loop
KeccakP800_12_SnP_FBWL_Unwrap_Exit:
	mov		r0, r4
	pop		{r4-r12,pc}


