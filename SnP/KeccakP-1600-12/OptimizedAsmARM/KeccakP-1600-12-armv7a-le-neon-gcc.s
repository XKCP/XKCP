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

@ WARNING: These functions work only on little endian CPU with@ ARMv7A + NEON architecture 
@ WARNING: State must be 256 bit (32 bytes) aligned, best is 64-byte (cache alignment).
@ INFO: Tested on Cortex-A8 (BeagleBone Black), using gcc.


   
.text

@ conditional assembly settings
.equ LoopUnroll, 1

@ offsets in state
.equ _ba	, 0*8
.equ _be	, 1*8
.equ _bi	, 2*8
.equ _bo	, 3*8
.equ _bu	, 4*8
.equ _ga	, 5*8
.equ _ge	, 6*8
.equ _gi	, 7*8
.equ _go	, 8*8
.equ _gu	, 9*8
.equ _ka	, 10*8
.equ _ke	, 11*8
.equ _ki	, 12*8
.equ _ko	, 13*8
.equ _ku	, 14*8
.equ _ma	, 15*8
.equ _me	, 16*8
.equ _mi	, 17*8
.equ _mo	, 18*8
.equ _mu	, 19*8
.equ _sa	, 20*8
.equ _se	, 21*8
.equ _si	, 22*8
.equ _so	, 23*8
.equ _su	, 24*8

@ macros

.macro    LoadState
    vld1.64 d0, [r0:64]!
    vld1.64 d2, [r0:64]!
    vld1.64 d4, [r0:64]!
    vld1.64 d6, [r0:64]!
    vld1.64 d8, [r0:64]!
    vld1.64 d1, [r0:64]!
    vld1.64 d3, [r0:64]!
    vld1.64 d5, [r0:64]!
    vld1.64 d7, [r0:64]!
    vld1.64 d9, [r0:64]!
    vld1.64 d10, [r0:64]!
    vld1.64 d12, [r0:64]!
    vld1.64 d14, [r0:64]!
    vld1.64 d16, [r0:64]!
    vld1.64 d18, [r0:64]!
    vld1.64 d11, [r0:64]!
    vld1.64 d13, [r0:64]!
    vld1.64 d15, [r0:64]!
    vld1.64 d17, [r0:64]!
    vld1.64 d19, [r0:64]!
    vld1.64 { d20, d21 }, [r0:128]!
    vld1.64 { d22, d23 }, [r0:128]!
    vld1.64 d24, [r0:64]
	sub		r0, r0, #24*8
	.endm

.macro    StoreState
    vst1.64 d0, [r0:64]!
    vst1.64 d2, [r0:64]!
    vst1.64 d4, [r0:64]!
    vst1.64 d6, [r0:64]!
    vst1.64 d8, [r0:64]!
    vst1.64 d1, [r0:64]!
    vst1.64 d3, [r0:64]!
    vst1.64 d5, [r0:64]!
    vst1.64 d7, [r0:64]!
    vst1.64 d9, [r0:64]!
    vst1.64 d10, [r0:64]!
    vst1.64 d12, [r0:64]!
    vst1.64 d14, [r0:64]!
    vst1.64 d16, [r0:64]!
    vst1.64 d18, [r0:64]!
    vst1.64 d11, [r0:64]!
    vst1.64 d13, [r0:64]!
    vst1.64 d15, [r0:64]!
    vst1.64 d17, [r0:64]!
    vst1.64 d19, [r0:64]!
    vst1.64 { d20, d21 }, [r0:128]!
    vst1.64 { d22, d23 }, [r0:128]!
    vst1.64 d24, [r0:64]
	.endm

.macro    RhoPi4		dst1, src1, rot1, dst2, src2, rot2, dst3, src3, rot3, dst4, src4, rot4
	.if (\rot1  &  7) != 0
    vshl.u64    \dst1, \src1, #\rot1
	.else
    vext.8      \dst1, \src1, \src1, #8-\rot1/8
	.endif
	.if (\rot2  &  7) != 0
    vshl.u64    \dst2, \src2, #\rot2
	.else
    vext.8      \dst2, \src2, \src2, #8-\rot2/8
	.endif
	.if (\rot3  &  7) != 0
    vshl.u64    \dst3, \src3, #\rot3
	.else
    vext.8      \dst3, \src3, \src3, #8-\rot3/8
	.endif
	.if (\rot4  &  7) != 0
    vshl.u64    \dst4, \src4, #\rot4
	.else
    vext.8      \dst4, \src4, \src4, #8-\rot4/8
	.endif
	.if (\rot1  &  7) != 0
    vsri.u64    \dst1, \src1, #64-\rot1
	.endif
	.if (\rot2  &  7) != 0
    vsri.u64    \dst2, \src2, #64-\rot2
	.endif
	.if (\rot3  &  7) != 0
    vsri.u64    \dst3, \src3, #64-\rot3
	.endif
	.if (\rot4  &  7) != 0
    vsri.u64    \dst4, \src4, #64-\rot4
	.endif
	.endm

.macro    KeccakRound	

    @Prepare Theta
    veor.64     q13, q0, q5
	vst1.64		{q12}, [r0:128]!
    veor.64     q14, q1, q6
	vst1.64		{q4}, [r0:128]!
    veor.64     d26,  d26,  d27
	vst1.64		{q9}, [r0:128]
    veor.64     d28,  d28,  d29
    veor.64     d26,  d26,  d20
    veor.64     d27,  d28,  d21

    veor.64     q14, q2, q7
    veor.64     q15, q3, q8
    veor.64     q4, q4, q9
    veor.64     d28,  d28,  d29
    veor.64     d30,  d30,  d31
    veor.64     d25,  d8,  d9
    veor.64     d28,  d28,  d22
    veor.64     d29,  d30,  d23
    veor.64     d25,  d25,  d24
	sub			r0, r0, #32

    @Apply Theta
    vadd.u64    d30,  d27,  d27
    vadd.u64    d24,  d28,  d28
    vadd.u64    d8,  d29,  d29
    vadd.u64    d18,  d25,  d25

    vsri.64     d30,  d27,  #63
    vsri.64     d24,  d28,  #63
    vsri.64     d8,  d29,  #63
    vsri.64     d18,  d25,  #63

    veor.64     d30,  d30,  d25
    veor.64     d24,  d24,  d26
    veor.64     d8,  d8,  d27
    vadd.u64    d27,  d26,  d26	@u
    veor.64     d18,  d18,  d28

	vmov.i64	d31,  d30
	vmov.i64	d25,  d24
    vsri.64     d27,  d26,  #63		@u
	vmov.i64	d9,  d8
	vmov.i64	d19,  d18

    veor.64     d20,  d20,  d30
    veor.64     d21,  d21,  d24
    veor.64     d27,  d27,  d29	@u
    veor.64     d22,  d22,  d8
    veor.64     d23,  d23,  d18
	vmov.i64	d26,  d27			@u

    veor.64     q0, q0, q15
    veor.64     q1, q1, q12
    veor.64     q2, q2, q4
    veor.64     q3, q3, q9

    veor.64     q5, q5, q15
    veor.64     q6, q6, q12
	vld1.64		{q12}, [r0:128]!
    veor.64     q7, q7, q4
	vld1.64		{q4}, [r0:128]!
    veor.64     q8, q8, q9
	vld1.64		{q9}, [r0:128]
    veor.64     d24,  d24,  d26	@u
	sub			r0, r0, #32
    veor.64     q4, q4, q13	@u
    veor.64     q9, q9, q13	@u

	@Rho Pi
	vmov.i64	d27, d2
	vmov.i64	d28, d4
	vmov.i64	d29, d6
	vmov.i64	d25, d8

	RhoPi4		d2, d3, 44, d4, d14, 43, d8, d24, 14, d6, d17, 21	@  1 <  6,  2 < 12,  4 < 24,  3 < 18
	RhoPi4		d3, d9, 20, d14, d16, 25, d24, d21,  2, d17, d15, 15	@  6 <  9, 12 < 13, 24 < 21, 18 < 17
	RhoPi4		d9, d22, 61, d16, d19,  8, d21, d7, 55, d15, d12, 10	@  9 < 22, 13 < 19, 21 <  8, 17 < 11	
	RhoPi4		d22, d18, 39, d19, d23, 56, d7, d13, 45, d12, d5,  6	@ 22 < 14, 19 < 23,  8 < 16, 11 < 7
	RhoPi4		d18, d20, 18, d23, d11, 41, d13, d1, 36, d5, d10,  3	@ 14 < 20, 23 < 15, 16 <  5,  7 < 10
	RhoPi4		d20, d28, 62, d11, d25, 27, d1, d29, 28, d10, d27,  1	@ 20 <  2, 15 <  4,  5 <  3, 10 < 1
	
	@Chi	b+g
	vmov.i64	q13, q0			
    vbic.64     q15, q2, q1	@ ba ^= ~be & bi
	veor.64		q0, q15
	vmov.i64	q14, q1			
    vbic.64     q15, q3, q2	@ be ^= ~bi & bo
	veor.64		q1, q15
    vbic.64     q15, q4, q3	@ bi ^= ~bo & bu
	veor.64		q2, q15
    vbic.64     q15, q13, q4	@ bo ^= ~bu & ba
	vbic.64     q13, q14, q13	@ bu ^= ~ba & be
	veor.64		q3, q15
 	veor.64		q4, q13

	@Chi	k+m
	vmov.i64	q13, q5			
    vbic.64     q15, q7, q6	@ ba ^= ~be & bi
	veor.64		q5, q15
	vmov.i64	q14, q6			
    vbic.64     q15, q8, q7	@ be ^= ~bi & bo
	veor.64		q6, q15
    vbic.64     q15, q9, q8	@ bi ^= ~bo & bu
	veor.64		q7, q15
    vbic.64     q15, q13, q9	@ bo ^= ~bu & ba
	vbic.64     q13, q14, q13	@ bu ^= ~ba & be
	veor.64		q8, q15
 	veor.64		q9, q13

	@Chi	s
	vmov.i64	q13, q10			
    vbic.64     d30,  d22,  d21	@ ba ^= ~be & bi
    vbic.64     d31,  d23,  d22	@ be ^= ~bi & bo
	veor.64		q10, q15
    vbic.64     d30,  d24,  d23	@ bi ^= ~bo & bu
    vbic.64     d31,  d26,  d24	@ bo ^= ~bu & ba
	vbic.64     d26,  d27,  d26	@ bu ^= ~ba & be
	veor.64		q11, q15
    vld1.64     d30,   [r1:64]!	@ Iota
 	veor.64		d24,  d26
    veor.64     d0, d0, d30		@ Iota
    .endm

@----------------------------------------------------------------------------
@
@ void KeccakF1600_Initialize( void )
@
   .align 8
.global   KeccakF1600_Initialize
KeccakF1600_Initialize:  
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateInitialize(void *state)
@
   .align 8
.global   KeccakF1600_StateInitialize
KeccakF1600_StateInitialize:  
    vmov.i64    q0, #0
    vmov.i64    q1, #0
    vmov.i64    q2, #0
    vmov.i64    q3, #0
    vstm        r0!, { d0 - d7 }            @ clear 8 lanes at a time
    vstm        r0!, { d0 - d7 }
    vstm        r0!, { d0 - d7 }
    vstm        r0!, { d0 }    
    bx          lr
   

@----------------------------------------------------------------------------
@
@    void KeccakF1600_StateComplementBit(void *state, unsigned int position)
@
   .align 8
.global   KeccakF1600_StateComplementBit
KeccakF1600_StateComplementBit:  
    lsrs    r2, r1, #5
    movs    r3, #1
    and     r1, r1, #31
    lsls    r3, r3, r1
    ldr     r1, [r0, r2, LSL #2]
    eors    r1, r1, r3
    str     r1, [r0, r2, LSL #2]
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
@ 
   .align 8
.global   KeccakF1600_StateXORLanes
KeccakF1600_StateXORLanes:  
    cmp     r2, #0
    beq     KeccakF1600_StateXORLanes_Exit
    push    {r4 - r7}
KeccakF1600_StateXORLanes_Loop:
    ldr     r4, [r1], #4
    ldr     r5, [r1], #4
    ldrd    r6, r7, [r0]
    eors    r6, r6, r4
    eors    r7, r7, r5
    strd    r6, r7, [r0], #8
    subs    r2, r2, #1
    bne     KeccakF1600_StateXORLanes_Loop
    pop     {r4 - r7}
KeccakF1600_StateXORLanes_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@
   .align 8
.global   KeccakF1600_StateXORBytesInLane
KeccakF1600_StateXORBytesInLane:  
    add     r0, r0, r3
    ldr     r3, [sp]
    cmp     r3, #0
    beq     KeccakF1600_StateXORBytesInLane_Exit
    add     r0, r0, r1, LSL #3
KeccakF1600_StateXORBytesInLane_Loop:
    ldrb    r1, [r2], #1
    ldrb    r12, [r0]
	eor		r1, r1, r12
    subs    r3, r3, #1
    strb    r1, [r0], #1
    bne     KeccakF1600_StateXORBytesInLane_Loop
KeccakF1600_StateXORBytesInLane_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateOverwriteLanes(void *state, const unsigned char *data, unsigned int laneCount)
@
   .align 8
.global   KeccakF1600_StateOverwriteLanes
KeccakF1600_StateOverwriteLanes: 
    cmp     r2, #0
    beq     KeccakF1600_StateOverwriteLanes_Exit
    push    {r4 - r5}
KeccakF1600_StateOverwriteLanes_Loop:
    ldr     r4, [r1], #4
    ldr     r5, [r1], #4
    subs    r2, r2, #1
    strd    r4, r5, [r0], #8
    bne     KeccakF1600_StateOverwriteLanes_Loop
    pop     {r4 - r5}
KeccakF1600_StateOverwriteLanes_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateOverwriteBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@
   .align 8
.global   KeccakF1600_StateOverwriteBytesInLane
KeccakF1600_StateOverwriteBytesInLane:
    add     r0, r0, r3
    ldr     r3, [sp]
    cmp     r3, #0
    beq     KeccakF1600_StateOverwriteBytesInLane_Exit
    add     r0, r0, r1, LSL #3
KeccakF1600_StateOverwriteBytesInLane_Loop:
    ldrb    r1, [r2], #1
    subs    r3, r3, #1
    strb    r1, [r0], #1
    bne     KeccakF1600_StateOverwriteBytesInLane_Loop
KeccakF1600_StateOverwriteBytesInLane_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
@
   .align 8
.global   KeccakF1600_StateOverwriteWithZeroes
KeccakF1600_StateOverwriteWithZeroes: 
	lsrs	r2, r1, #3
	beq		KeccakF1600_StateOverwriteWithZeroes_Bytes
    vmov.i64 d0, #0
KeccakF1600_StateOverwriteWithZeroes_LoopLanes:
	subs	r2, r2, #1
    vstm    r0!, { d0 }    
	bne		KeccakF1600_StateOverwriteWithZeroes_LoopLanes
KeccakF1600_StateOverwriteWithZeroes_Bytes:
	ands	r1, #7
	beq		KeccakF1600_StateOverwriteWithZeroes_Exit
	movs	r3, #0
KeccakF1600_StateOverwriteWithZeroes_LoopBytes:
	subs	r1, r1, #1
	strb	r3, [r0], #1
	bne		KeccakF1600_StateOverwriteWithZeroes_LoopBytes
KeccakF1600_StateOverwriteWithZeroes_Exit:
	bx		lr


@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
@
.align 8
.global   KeccakF1600_StateExtractLanes
KeccakF1600_StateExtractLanes:  
    cmp     r2, #0
    beq     KeccakF1600_StateExtractLanes_Exit
    lsls    r3, r1, #32-3
    bne     KeccakF1600_StateExtractLanes_Unaligned
    lsrs    r2, r2, #1
    bcc     KeccakF1600_StateExtractLanes_LoopAligned
    vldm    r0!, { d0 }
    vstm    r1!, { d0 }
    beq     KeccakF1600_StateExtractLanes_Exit
KeccakF1600_StateExtractLanes_LoopAligned:
    vldm    r0!, { d0 - d1 }
    vstm    r1!, { d0 - d1 }
    subs    r2, r2, #1
    bne     KeccakF1600_StateExtractLanes_LoopAligned
    bx      lr
KeccakF1600_StateExtractLanes_Unaligned:
    push    { r4, r5 }
KeccakF1600_StateExtractLanes_LoopUnaligned:
    ldrd    r4, r5, [r0], #8
    str     r4, [r1], #4
    subs    r2, r2, #1
    str     r5, [r1], #4
    bne     KeccakF1600_StateExtractLanes_LoopUnaligned
    pop     { r4, r5 }
KeccakF1600_StateExtractLanes_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@
   .align 8
.global   KeccakF1600_StateExtractBytesInLane
KeccakF1600_StateExtractBytesInLane:  
    add     r0, r0, r3
    ldr     r3, [sp]
    cmp     r3, #0
    beq     KeccakF1600_StateExtractBytesInLane_Exit
    add     r0, r0, r1, LSL #3
KeccakF1600_StateExtractBytesInLane_Loop:
    ldrb    r1, [r0], #1
    subs    r3, r3, #1
    strb    r1, [r2], #1
    bne     KeccakF1600_StateExtractBytesInLane_Loop
KeccakF1600_StateExtractBytesInLane_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateExtractAndXORLanes(const void *state, unsigned char *data, unsigned int laneCount)
@
   .align 8
.global   KeccakF1600_StateExtractAndXORLanes
KeccakF1600_StateExtractAndXORLanes:
    cmp     r2, #0
    beq     KeccakF1600_StateExtractAndXORLanes_Exit
KeccakF1600_StateExtractAndXORLanes_Loop:
    ldr     r3, [r0], #4
    ldr     r12, [r1]
	eor		r3, r3, r12
    str     r3, [r1], #4
    ldr     r3, [r0], #4
    ldr     r12, [r1]
    subs    r2, r2, #1
	eor		r3, r3, r12
    str     r3, [r1], #4
    bne     KeccakF1600_StateExtractAndXORLanes_Loop
KeccakF1600_StateExtractAndXORLanes_Exit:
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakF1600_StateExtractAndXORBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@
   .align 8
.global   KeccakF1600_StateExtractAndXORBytesInLane
KeccakF1600_StateExtractAndXORBytesInLane:
    add     r0, r0, r3
    ldr     r3, [sp]
    cmp     r3, #0
    beq     KeccakF1600_StateExtractAndXORBytesInLane_Exit
    add     r0, r0, r1, LSL #3
KeccakF1600_StateExtractAndXORBytesInLane_Loop:
    ldrb    r1, [r0], #1
    ldrb    r12, [r2]
    subs    r3, r3, #1
	eor		r1, r1, r12
    strb    r1, [r2], #1
    bne     KeccakF1600_StateExtractAndXORBytesInLane_Loop
KeccakF1600_StateExtractAndXORBytesInLane_Exit:
    bx      lr
   

   .align 8
KeccakP1600_12_StatePermute_RoundConstants:
		.quad      0x000000008000808b
		.quad      0x800000000000008b
		.quad      0x8000000000008089
		.quad      0x8000000000008003
		.quad      0x8000000000008002
		.quad      0x8000000000000080
		.quad      0x000000000000800a
		.quad      0x800000008000000a
		.quad      0x8000000080008081
		.quad      0x8000000000008080
		.quad      0x0000000080000001
		.quad      0x8000000080008008

   .align 8
KeccakP1600_12_StateXORandPermuteAsmOnly:  

	add		pc, pc, r5, LSL #3
	mov		r1, #0								@ dummy instruction for PC alignment, not executed
	veor.64	d0, d0, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d2, d2, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d4, d4, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d6, d6, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d8, d8, d30
	b		KeccakP1600_12_StatePermuteAsmOnly

	veor.64	d1, d1, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d3, d3, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d5, d5, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d7, d7, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d9, d9, d30
	b		KeccakP1600_12_StatePermuteAsmOnly

	veor.64	d10, d10, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d12, d12, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d14, d14, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d16, d16, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d18, d18, d30
	b		KeccakP1600_12_StatePermuteAsmOnly

	veor.64	d11, d11, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d13, d13, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d15, d15, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d17, d17, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d19, d19, d30
	b		KeccakP1600_12_StatePermuteAsmOnly

	veor.64	d20, d20, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d21, d21, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d22, d22, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d23, d23, d30
	b		KeccakP1600_12_StatePermuteAsmOnly
	veor.64	d24, d24, d30
KeccakP1600_12_StatePermuteAsmOnly:
    adr     r1, KeccakP1600_12_StatePermute_RoundConstants
	.if LoopUnroll == 0
    movs    r2, #12
KeccakP1600_12_StatePermute_RoundLoop:
    KeccakRound
    subs    r2, #1
    bne     KeccakP1600_12_StatePermute_RoundLoop
	.else
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
    KeccakRound
	.endif
    bx      lr
   

@----------------------------------------------------------------------------
@
@ void KeccakP1600_12_StatePermute( void *state )
@
   .align 8
.global   KeccakP1600_12_StatePermute
KeccakP1600_12_StatePermute:  
	mov		r3, lr
    vpush   {q4-q7}
    LoadState
	bl		KeccakP1600_12_StatePermuteAsmOnly
    StoreState
    vpop    {q4-q7}
    bx      r3
   

@----------------------------------------------------------------------------
@
@ size_t KeccakP1600_12_SnP_FBWL_Absorb(	void *state, unsigned int laneCount, unsigned char *data, 
@										size_t dataByteLen, unsigned char trailingBits )
@
.align 8
.global 	KeccakP1600_12_SnP_FBWL_Absorb
KeccakP1600_12_SnP_FBWL_Absorb:
	push	{r4-r8,lr}							@ 6 CPU registers (24 bytes)
	lsr		r3, r3, #3							@ r3 nbrLanes = dataByteLen / SnP_laneLengthInBytes
	mov		r6, r2								@ r6 data pointer
	subs	r3, r3, r1							@ .if (nbrLanes >== laneCount)
	mov		r4, r2								@ r4 initial data pointer
	bcc		KeccakP1600_12_SnP_FBWL_Absorb_Exit
	mov		r5, r1
    vpush   {q4-q7}								@ 4 quad registers (64 bytes)
    LoadState

	sub		sp, sp, #8							@ alloc space for trailingBits lane
	veor.64	d30, d30, d30
	add		r7, sp, #(6+16+2)*4
	vld1.8	{d30[0]}, [r7]
	vst1.64	{d30}, [sp:64]

	cmp		r5, #21
	bne		KeccakP1600_12_SnP_FBWL_Absorb_Not21Lanes
KeccakP1600_12_SnP_FBWL_Absorb_Loop21Lanes:
    vld1.64	{ d26, d27, d28, d29 }, [r6]!	@ XOR first 21 lanes
	veor.64	d0, d0, d26
	veor.64	d2, d2, d27
	veor.64	d4, d4, d28
	veor.64	d6, d6, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d8, d8, d26
	veor.64	d1, d1, d27
	veor.64	d3, d3, d28
	veor.64	d5, d5, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d7, d7, d26
	veor.64	d9, d9, d27
	veor.64	d10, d10, d28
	veor.64	d12, d12, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d14, d14, d26
	veor.64	d16, d16, d27
	veor.64	d18, d18, d28
	veor.64	d11, d11, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d13, d13, d26
	veor.64	d15, d15, d27
	veor.64	d17, d17, d28
	veor.64	d19, d19, d29
    vld1.64	{ d26 }, [r6]!
	veor.64	d20, d20, d26

	vld1.64	{d30}, [sp:64]					@ xor trailingBits
	veor.64	d21, d21, d30
	bl		KeccakP1600_12_StatePermuteAsmOnly
	subs	r3, r3, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Absorb_Loop21Lanes
KeccakP1600_12_SnP_FBWL_Absorb_Done:
	add		sp, sp, #8							@ free trailingBits lane
    StoreState
    vpop	{q4-q7}
KeccakP1600_12_SnP_FBWL_Absorb_Exit:
	sub		r0, r6, r4							@ processed = data pointer - initial data pointer
	pop		{r4-r8,pc}
KeccakP1600_12_SnP_FBWL_Absorb_Not21Lanes:
	cmp		r5, #16
	mvn		r7, #7								@ r7 = -8
	blo		KeccakP1600_12_SnP_FBWL_Absorb_LoopLessThan16Lanes
KeccakP1600_12_SnP_FBWL_Absorb_Loop16OrMoreLanes:
    vld1.64	{ d26, d27, d28, d29 }, [r6]!	@ XOR first 16 lanes
	veor.64	d0, d0, d26
	veor.64	d2, d2, d27
	veor.64	d4, d4, d28
	veor.64	d6, d6, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d8, d8, d26
	veor.64	d1, d1, d27
	veor.64	d3, d3, d28
	veor.64	d5, d5, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d7, d7, d26
	veor.64	d9, d9, d27
	veor.64	d10, d10, d28
	veor.64	d12, d12, d29
    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d14, d14, d26
	veor.64	d16, d16, d27
	veor.64	d18, d18, d28
	veor.64	d11, d11, d29

	sub		r2, r5, #16							@ XOR last n lanes, maximum 9
	rsb		r1, r2, #9
	add		r6, r6, r2, LSL #3					@ data += n lanes * 8
	sub		r2, r6, #8							@ r2 tempdata =  data - 8
	add		pc, pc, r1, LSL #3
	mov		r1, #0								@ dummy instruction for PC alignment, not executed
    vld1.64 d30, [r2], r7
	veor.64	d24, d24, d30
    vld1.64 d30, [r2], r7
	veor.64	d23, d23, d30
    vld1.64 d30, [r2], r7
	veor.64	d22, d22, d30
    vld1.64 d30, [r2], r7
	veor.64	d21, d21, d30
    vld1.64 d30, [r2], r7
	veor.64	d20, d20, d30

    vld1.64 d30, [r2], r7
	veor.64	d19, d19, d30
    vld1.64 d30, [r2], r7
	veor.64	d17, d17, d30
    vld1.64 d30, [r2], r7
	veor.64	d15, d15, d30
    vld1.64 d30, [r2], r7
	veor.64	d13, d13, d30

	vld1.64	{d30}, [sp:64]
	bl		KeccakP1600_12_StateXORandPermuteAsmOnly
	subs	r3, r3, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Absorb_Loop16OrMoreLanes
	b		KeccakP1600_12_SnP_FBWL_Absorb_Done
KeccakP1600_12_SnP_FBWL_Absorb_LoopLessThan16Lanes:
	rsb		r1, r5, #15							@ XOR up to 15 lanes
	add		r6, r6, r5, LSL #3					@ data += laneCount * 8
	sub		r2, r6, #8							@ r2 tempdata =  data - 8
	add		pc, pc, r1, LSL #3
	mov		r1, #0								@ dummy instruction for PC alignment, not executed

    vld1.64 d30, [r2], r7
	veor.64	d18, d18, d30
    vld1.64 d30, [r2], r7
	veor.64	d16, d16, d30
    vld1.64 d30, [r2], r7
	veor.64	d14, d14, d30
    vld1.64 d30, [r2], r7
	veor.64	d12, d12, d30
    vld1.64 d30, [r2], r7
	veor.64	d10, d10, d30

    vld1.64 d30, [r2], r7
	veor.64	d9, d9, d30
    vld1.64 d30, [r2], r7
	veor.64	d7, d7, d30
    vld1.64 d30, [r2], r7
	veor.64	d5, d5, d30
    vld1.64 d30, [r2], r7
	veor.64	d3, d3, d30
    vld1.64 d30, [r2], r7
	veor.64	d1, d1, d30

    vld1.64 d30, [r2], r7
	veor.64	d8, d8, d30
    vld1.64 d30, [r2], r7
	veor.64	d6, d6, d30
    vld1.64 d30, [r2], r7
	veor.64	d4, d4, d30
    vld1.64 d30, [r2], r7
	veor.64	d2, d2, d30
    vld1.64 d30, [r2], r7
	veor.64	d0, d0, d30

	vld1.64	{d30}, [sp:64]
	bl		KeccakP1600_12_StateXORandPermuteAsmOnly
	subs	r3, r3, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Absorb_LoopLessThan16Lanes
	b		KeccakP1600_12_SnP_FBWL_Absorb_Done


@----------------------------------------------------------------------------
@
@ size_t KeccakP1600_12_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
@
.align 8
.global 	KeccakP1600_12_SnP_FBWL_Squeeze
KeccakP1600_12_SnP_FBWL_Squeeze:
	push	{r4-r8,lr}
	lsr		r3, r3, #3							@ r3 nbrLanes = dataByteLen / SnP_laneLengthInBytes
	mov		r6, r2								@ r6 data pointer
	subs	r3, r3, r1							@ .if (nbrLanes >== laneCount)
	mov		r4, r2								@ r4 initial data pointer
	bcc		KeccakP1600_12_SnP_FBWL_Squeeze_Exit
	mov		r5, r1
	mvn		r7, #7								@ r7 = -8
    vpush   {q4-q7}
    LoadState
	cmp		r5, #21
	bne		KeccakP1600_12_SnP_FBWL_Squeeze_Not21Lanes
KeccakP1600_12_SnP_FBWL_Squeeze_Loop21Lanes:
	bl		KeccakP1600_12_StatePermuteAsmOnly
    vst1.64	{ d0 }, [r6]!					@ Extract first 21 lanes
    vst1.64	{ d2 }, [r6]!
    vst1.64	{ d4 }, [r6]!
    vst1.64	{ d6 }, [r6]!
    vst1.64	{ d8 }, [r6]!
    vst1.64	{ d1 }, [r6]!
    vst1.64	{ d3 }, [r6]!
    vst1.64	{ d5 }, [r6]!
    vst1.64	{ d7 }, [r6]!
    vst1.64	{ d9 }, [r6]!
    vst1.64	{ d10 }, [r6]!
    vst1.64	{ d12 }, [r6]!
    vst1.64	{ d14 }, [r6]!
    vst1.64	{ d16 }, [r6]!
    vst1.64	{ d18 }, [r6]!
    vst1.64	{ d11 }, [r6]!
    vst1.64	{ d13 }, [r6]!
    vst1.64	{ d15 }, [r6]!
    vst1.64	{ d17 }, [r6]!
	subs	r3, r3, r5							@ nbrLanes -= laneCount
    vst1.64	{ d19 }, [r6]!
    vst1.64	{ d20 }, [r6]!
	bcs		KeccakP1600_12_SnP_FBWL_Squeeze_Loop21Lanes
    StoreState
    vpop	{q4-q7}
KeccakP1600_12_SnP_FBWL_Squeeze_Exit:
	sub		r0, r6, r4							@ processed = data poiner - initial data pointer
	pop		{r4-r8,pc}
KeccakP1600_12_SnP_FBWL_Squeeze_Not21Lanes:
	cmp		r5, #16
	blo		KeccakP1600_12_SnP_FBWL_Squeeze_LoopLessThan16Lanes
KeccakP1600_12_SnP_FBWL_Squeeze_Loop16OrMoreLanes:
	bl		KeccakP1600_12_StatePermuteAsmOnly
    vst1.64	{ d0 }, [r6]!					@ Extract first 16 lanes
    vst1.64	{ d2 }, [r6]!
    vst1.64	{ d4 }, [r6]!
    vst1.64	{ d6 }, [r6]!
    vst1.64	{ d8 }, [r6]!
    vst1.64	{ d1 }, [r6]!
    vst1.64	{ d3 }, [r6]!
    vst1.64	{ d5 }, [r6]!
    vst1.64	{ d7 }, [r6]!
    vst1.64	{ d9 }, [r6]!
    vst1.64	{ d10 }, [r6]!
	sub		r2, r5, #16							@ Extract last n lanes, maximum 9
    vst1.64	{ d12 }, [r6]!
    vst1.64	{ d14 }, [r6]!
	rsb		r1, r2, #9
    vst1.64	{ d16 }, [r6]!
    vst1.64	{ d18 }, [r6]!
    vst1.64	{ d11 }, [r6]!
	add		r6, r6, r2, LSL #3					@ data += laneCount * 8
	sub		r2, r6, #8							@ r2 tempdata = data - 8
	add		pc, pc, r1, LSL #2
	mov		r1, #0								@ dummy instruction for PC alignment, not executed
    vst1.64 d24, [r2], r7
    vst1.64 d23, [r2], r7
    vst1.64 d22, [r2], r7
    vst1.64 d21, [r2], r7
    vst1.64 d20, [r2], r7
    vst1.64 d19, [r2], r7
    vst1.64 d17, [r2], r7
    vst1.64 d15, [r2], r7
    vst1.64 d13, [r2], r7
	subs	r3, r3, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Squeeze_Loop16OrMoreLanes
    StoreState
    vpop	{q4-q7}
	sub		r0, r6, r4							@ processed = data poiner - initial data pointer
	pop		{r4-r8,pc}
KeccakP1600_12_SnP_FBWL_Squeeze_LoopLessThan16Lanes:
	bl		KeccakP1600_12_StatePermuteAsmOnly
	add		r6, r6, r5, LSL #3					@ data += laneCount * 8
	rsb		r1, r5, #15							@ extract up to 15 lanes
	sub		r2, r6, #8							@ r2 tempdata = data - 8
	add		pc, pc, r1, LSL #2
	mov		r1, #0								@ dummy instruction for PC alignment, not executed
    vst1.64 d18, [r2], r7
    vst1.64 d16, [r2], r7
    vst1.64 d14, [r2], r7
    vst1.64 d12, [r2], r7
    vst1.64 d10, [r2], r7
    vst1.64 d9, [r2], r7
    vst1.64 d7, [r2], r7
    vst1.64 d5, [r2], r7
    vst1.64 d3, [r2], r7
    vst1.64 d1, [r2], r7
    vst1.64 d8, [r2], r7
    vst1.64 d6, [r2], r7
    vst1.64 d4, [r2], r7
    vst1.64 d2, [r2], r7
    vst1.64 d0, [r2], r7
	subs	r3, r3, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Squeeze_LoopLessThan16Lanes
    StoreState
    vpop	{q4-q7}
	sub		r0, r6, r4							@ processed = data pointer - initial data pointer
	pop		{r4-r8,pc}


@----------------------------------------------------------------------------
@
@ size_t KeccakP1600_12_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
@										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
@
.align 8
.global 	KeccakP1600_12_SnP_FBWL_Wrap
KeccakP1600_12_SnP_FBWL_Wrap:
	push	{r4-r10,lr}							@ 8 CPU registers (32 bytes)
	ldr		r8, [sp, #8*4+0]					@ r8 dataByteLen
	mov		r6, r2								@ r6 dataIn pointer
	lsr		r8, r8, #3							@ r8 nbrLanes = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1							@ .if (nbrLanes >== laneCount)
	mov		r4, r2								@ r4 initial dataIn pointer
	bcc		KeccakP1600_12_SnP_FBWL_Wrap_Exit
	mov		r5, r1
    vpush   {q4-q7}								@ 4 quad registers (64 bytes)
    LoadState

	sub		sp, sp, #8							@ alloc space for trailingBits lane
	veor.64	d30, d30, d30
	add		r7, sp, #(8+16+1+2)*4
	vld1.8	{d30[0]}, [r7]
	vst1.64	{d30}, [sp:64]

	cmp		r5, #21
	bne		KeccakP1600_12_SnP_FBWL_Wrap_Not21Lanes
KeccakP1600_12_SnP_FBWL_Wrap_Loop21Lanes:
    vld1.64	{ d26, d27, d28, d29 }, [r6]!	@ XOR and extract first 21 lanes
	veor.64	d0, d0, d26
	veor.64	d2, d2, d27
	veor.64	d4, d4, d28
	veor.64	d6, d6, d29
    vst1.64	{ d0 }, [r3]!	
    vst1.64	{ d2 }, [r3]!	
    vst1.64	{ d4 }, [r3]!	
    vst1.64	{ d6 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d8, d8, d26
	veor.64	d1, d1, d27
	veor.64	d3, d3, d28
	veor.64	d5, d5, d29
    vst1.64	{ d8 }, [r3]!	
    vst1.64	{ d1 }, [r3]!	
    vst1.64	{ d3 }, [r3]!	
    vst1.64	{ d5 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d7, d7, d26
	veor.64	d9, d9, d27
	veor.64	d10, d10, d28
	veor.64	d12, d12, d29
    vst1.64	{ d7 }, [r3]!	
    vst1.64	{ d9 }, [r3]!	
    vst1.64	{ d10 }, [r3]!	
    vst1.64	{ d12 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d14, d14, d26
	veor.64	d16, d16, d27
	veor.64	d18, d18, d28
	veor.64	d11, d11, d29
    vst1.64	{ d14 }, [r3]!	
    vst1.64	{ d16 }, [r3]!	
    vst1.64	{ d18 }, [r3]!	
    vst1.64	{ d11 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d13, d13, d26
	veor.64	d15, d15, d27
	veor.64	d17, d17, d28
	veor.64	d19, d19, d29
    vst1.64	{ d13 }, [r3]!	
    vst1.64	{ d15 }, [r3]!	
    vst1.64	{ d17 }, [r3]!	
    vst1.64	{ d19 }, [r3]!	

    vld1.64	{ d26 }, [r6]!
	veor.64	d20, d20, d26
    vst1.64	{ d20 }, [r3]!	

	vld1.64	{d30}, [sp:64]					@ xor trailingBits
	veor.64	d21, d21, d30
	bl		KeccakP1600_12_StatePermuteAsmOnly
	subs	r8, r8, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Wrap_Loop21Lanes
KeccakP1600_12_SnP_FBWL_Wrap_Done:
	add		sp, sp, #8							@ free trailingBits lane
    StoreState
    vpop	{q4-q7}
KeccakP1600_12_SnP_FBWL_Wrap_Exit:
	sub		r0, r6, r4							@ processed = dataIn pointer - initial dataIn pointer
	pop		{r4-r10,pc}
KeccakP1600_12_SnP_FBWL_Wrap_Not21Lanes:
	cmp		r5, #16
	mvn		r7, #7								@ r7 = -8
	blo		KeccakP1600_12_SnP_FBWL_Wrap_LoopLessThan16Lanes
KeccakP1600_12_SnP_FBWL_Wrap_Loop16OrMoreLanes:
    vld1.64	{ d26, d27, d28, d29 }, [r6]!	@ XOR and extract first 16 lanes
	veor.64	d0, d0, d26
	veor.64	d2, d2, d27
	veor.64	d4, d4, d28
	veor.64	d6, d6, d29
    vst1.64	{ d0 }, [r3]!	
    vst1.64	{ d2 }, [r3]!	
    vst1.64	{ d4 }, [r3]!	
    vst1.64	{ d6 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d8, d8, d26
	veor.64	d1, d1, d27
	veor.64	d3, d3, d28
	veor.64	d5, d5, d29
    vst1.64	{ d8 }, [r3]!	
    vst1.64	{ d1 }, [r3]!	
    vst1.64	{ d3 }, [r3]!	
    vst1.64	{ d5 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d7, d7, d26
	veor.64	d9, d9, d27
	veor.64	d10, d10, d28
	veor.64	d12, d12, d29
    vst1.64	{ d7 }, [r3]!	
    vst1.64	{ d9 }, [r3]!	
    vst1.64	{ d10 }, [r3]!	
    vst1.64	{ d12 }, [r3]!	

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d14, d14, d26
	veor.64	d16, d16, d27
	veor.64	d18, d18, d28
	veor.64	d11, d11, d29
    vst1.64	{ d14 }, [r3]!	
    vst1.64	{ d16 }, [r3]!	
    vst1.64	{ d18 }, [r3]!	
    vst1.64	{ d11 }, [r3]!	

	sub		r2, r5, #16							@ XOR last n lanes, maximum 9
	rsb		r1, r2, #9
	add		r6, r6, r2, LSL #3					@ dataIn += n lanes * 8
	add		r1, r1, r1, LSL #1
	add		r3, r3, r2, LSL #3					@ dataOut += n lanes * 8
	sub		r2, r6, #8							@ tempdataIn = dataIn - 8
	sub		r9, r3, #8							@ tempdataOut = dataOut - 8
	add		pc, pc, r1, LSL #2
	mov		r1, #0								@ dummy instruction for PC alignment, not executed
    vld1.64 d30, [r2], r7
	veor.64	d24, d24, d30
    vst1.64 d24, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d23, d23, d30
    vst1.64 d23, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d22, d22, d30
    vst1.64 d22, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d21, d21, d30
    vst1.64 d21, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d20, d20, d30
    vst1.64 d20, [r9], r7

    vld1.64 d30, [r2], r7
	veor.64	d19, d19, d30
    vst1.64 d19, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d17, d17, d30
    vst1.64 d17, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d15, d15, d30
    vst1.64 d15, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d13, d13, d30
    vst1.64 d13, [r9], r7

	vld1.64	{d30}, [sp:64]
	bl		KeccakP1600_12_StateXORandPermuteAsmOnly
	subs	r8, r8, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Wrap_Loop16OrMoreLanes
	b		KeccakP1600_12_SnP_FBWL_Wrap_Done
KeccakP1600_12_SnP_FBWL_Wrap_LoopLessThan16Lanes:
	rsb		r1, r5, #15							@ XOR up to 15 lanes
	add		r6, r6, r5, LSL #3					@ dataIn += laneCount * 8
	add		r1, r1, r1, LSL #1
	add		r3, r3, r5, LSL #3					@ dataOut += laneCount * 8
	sub		r2, r6, #8							@ tempdataIn = dataIn - 8
	sub		r9, r3, #8							@ tempdataOut = dataOut - 8
	add		pc, pc, r1, LSL #2
	mov		r1, #0								@ dummy instruction for PC alignment, not executed

    vld1.64 d30, [r2], r7
	veor.64	d18, d18, d30
    vst1.64 d18, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d16, d16, d30
    vst1.64 d16, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d14, d14, d30
    vst1.64 d14, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d12, d12, d30
    vst1.64 d12, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d10, d10, d30
    vst1.64 d10, [r9], r7

    vld1.64 d30, [r2], r7
	veor.64	d9, d9, d30
    vst1.64 d9, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d7, d7, d30
    vst1.64 d7, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d5, d5, d30
    vst1.64 d5, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d3, d3, d30
    vst1.64 d3, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d1, d1, d30
    vst1.64 d1, [r9], r7

    vld1.64 d30, [r2], r7
	veor.64	d8, d8, d30
    vst1.64 d8, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d6, d6, d30
    vst1.64 d6, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d4, d4, d30
    vst1.64 d4, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d2, d2, d30
    vst1.64 d2, [r9], r7
    vld1.64 d30, [r2], r7
	veor.64	d0, d0, d30
    vst1.64 d0, [r9], r7

	vld1.64	{d30}, [sp:64]
	bl		KeccakP1600_12_StateXORandPermuteAsmOnly
	subs	r8, r8, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Wrap_LoopLessThan16Lanes
	b		KeccakP1600_12_SnP_FBWL_Wrap_Done


@----------------------------------------------------------------------------
@
@ size_t KeccakP1600_12_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
@										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
@
.align 8
.global 	KeccakP1600_12_SnP_FBWL_Unwrap
KeccakP1600_12_SnP_FBWL_Unwrap:
	push	{r4-r10,lr}							@ 8 CPU registers (32 bytes)
	ldr		r8, [sp, #8*4+0]					@ r8 dataByteLen
	mov		r6, r2								@ r6 dataIn pointer
	lsr		r8, r8, #3							@ r8 nbrLanes = dataByteLen / SnP_laneLengthInBytes
	subs	r8, r8, r1							@ .if (nbrLanes >== laneCount)
	mov		r4, r2								@ r4 initial dataIn pointer
	bcc		KeccakP1600_12_SnP_FBWL_Unwrap_Exit
	mov		r5, r1
    vpush   {q4-q7}								@ 4 quad registers (64 bytes)
    LoadState

	sub		sp, sp, #8							@ alloc space for trailingBits lane
	veor.64	d30, d30, d30
	add		r7, sp, #(8+16+1+2)*4
	vld1.8	{d30[0]}, [r7]
	vst1.64	{d30}, [sp:64]

	cmp		r5, #21
	bne		KeccakP1600_12_SnP_FBWL_Unwrap_Not21Lanes
KeccakP1600_12_SnP_FBWL_Unwrap_Loop21Lanes:
    vld1.64	{ d26, d27, d28, d29 }, [r6]!	@ XOR and extract, overwrite first 21 lanes
	veor.64	d0, d0, d26
	veor.64	d2, d2, d27
	veor.64	d4, d4, d28
	veor.64	d6, d6, d29
    vst1.64	{ d0 }, [r3]!	
	vmov.64	d0, d26
    vst1.64	{ d2 }, [r3]!	
	vmov.64	d2, d27
    vst1.64	{ d4 }, [r3]!	
	vmov.64	d4, d28
    vst1.64	{ d6 }, [r3]!	
	vmov.64	d6, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d8, d8, d26
	veor.64	d1, d1, d27
	veor.64	d3, d3, d28
	veor.64	d5, d5, d29
    vst1.64	{ d8 }, [r3]!	
	vmov.64	d8, d26
    vst1.64	{ d1 }, [r3]!	
	vmov.64	d1, d27
    vst1.64	{ d3 }, [r3]!	
	vmov.64	d3, d28
    vst1.64	{ d5 }, [r3]!	
	vmov.64	d5, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d7, d7, d26
	veor.64	d9, d9, d27
	veor.64	d10, d10, d28
	veor.64	d12, d12, d29
    vst1.64	{ d7 }, [r3]!	
	vmov.64	d7, d26
    vst1.64	{ d9 }, [r3]!	
	vmov.64	d9, d27
    vst1.64	{ d10 }, [r3]!	
	vmov.64	d10, d28
    vst1.64	{ d12 }, [r3]!	
	vmov.64	d12, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d14, d14, d26
	veor.64	d16, d16, d27
	veor.64	d18, d18, d28
	veor.64	d11, d11, d29
    vst1.64	{ d14 }, [r3]!	
	vmov.64	d14, d26
    vst1.64	{ d16 }, [r3]!	
	vmov.64	d16, d27
    vst1.64	{ d18 }, [r3]!	
	vmov.64	d18, d28
    vst1.64	{ d11 }, [r3]!	
	vmov.64	d11, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d13, d13, d26
	veor.64	d15, d15, d27
	veor.64	d17, d17, d28
	veor.64	d19, d19, d29
    vst1.64	{ d13 }, [r3]!	
	vmov.64	d13, d26
    vst1.64	{ d15 }, [r3]!	
	vmov.64	d15, d27
    vst1.64	{ d17 }, [r3]!	
	vmov.64	d17, d28
    vst1.64	{ d19 }, [r3]!	
	vmov.64	d19, d29

    vld1.64	{ d26 }, [r6]!
	veor.64	d20, d20, d26
    vst1.64	{ d20 }, [r3]!	
	vmov.64	d20, d26

	vld1.64	{d30}, [sp:64]					@ xor trailingBits
	veor.64	d21, d21, d30
	bl		KeccakP1600_12_StatePermuteAsmOnly
	subs	r8, r8, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Unwrap_Loop21Lanes
KeccakP1600_12_SnP_FBWL_Unwrap_Done:
	add		sp, sp, #8							@ free trailingBits lane
    StoreState
    vpop	{q4-q7}
KeccakP1600_12_SnP_FBWL_Unwrap_Exit:
	sub		r0, r6, r4							@ processed = dataIn pointer - initial dataIn pointer
	pop		{r4-r10,pc}
KeccakP1600_12_SnP_FBWL_Unwrap_Not21Lanes:
	cmp		r5, #16
	mvn		r7, #7								@ r7 = -8
	blo		KeccakP1600_12_SnP_FBWL_Unwrap_LoopLessThan16Lanes
KeccakP1600_12_SnP_FBWL_Unwrap_Loop16OrMoreLanes:
    vld1.64	{ d26, d27, d28, d29 }, [r6]!	@ XOR and extract first 16 lanes
	veor.64	d0, d0, d26
	veor.64	d2, d2, d27
	veor.64	d4, d4, d28
	veor.64	d6, d6, d29
    vst1.64	{ d0 }, [r3]!	
	vmov.64	d0, d26
    vst1.64	{ d2 }, [r3]!	
	vmov.64	d2, d27
    vst1.64	{ d4 }, [r3]!	
	vmov.64	d4, d28
    vst1.64	{ d6 }, [r3]!	
	vmov.64	d6, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d8, d8, d26
	veor.64	d1, d1, d27
	veor.64	d3, d3, d28
	veor.64	d5, d5, d29
    vst1.64	{ d8 }, [r3]!	
	vmov.64	d8, d26
    vst1.64	{ d1 }, [r3]!	
	vmov.64	d1, d27
    vst1.64	{ d3 }, [r3]!	
	vmov.64	d3, d28
    vst1.64	{ d5 }, [r3]!	
	vmov.64	d5, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d7, d7, d26
	veor.64	d9, d9, d27
	veor.64	d10, d10, d28
	veor.64	d12, d12, d29
    vst1.64	{ d7 }, [r3]!	
	vmov.64	d7, d26
    vst1.64	{ d9 }, [r3]!	
	vmov.64	d9, d27
    vst1.64	{ d10 }, [r3]!	
	vmov.64	d10, d28
    vst1.64	{ d12 }, [r3]!	
	vmov.64	d12, d29

    vld1.64	{ d26, d27, d28, d29 }, [r6]!
	veor.64	d14, d14, d26
	veor.64	d16, d16, d27
	veor.64	d18, d18, d28
	veor.64	d11, d11, d29
    vst1.64	{ d14 }, [r3]!	
	vmov.64	d14, d26
    vst1.64	{ d16 }, [r3]!	
	vmov.64	d16, d27
    vst1.64	{ d18 }, [r3]!	
	vmov.64	d18, d28
    vst1.64	{ d11 }, [r3]!	
	vmov.64	d11, d29

	sub		r2, r5, #16							@ XOR last n lanes, maximum 9
	rsb		r1, r2, #9
	add		r6, r6, r2, LSL #3					@ dataIn += n lanes * 8
	add		r3, r3, r2, LSL #3					@ dataOut += n lanes * 8
	sub		r2, r6, #8							@ tempdataIn = dataIn - 8
	sub		r9, r3, #8							@ tempdataOut = dataOut - 8
	add		pc, pc, r1, LSL #4
	mov		r1, #0								@ dummy instruction for PC alignment, not executed
    vld1.64 d30, [r2], r7
	veor.64	d24, d24, d30
    vst1.64 d24, [r9], r7
	vmov.64	d24, d30
    vld1.64 d30, [r2], r7
	veor.64	d23, d23, d30
    vst1.64 d23, [r9], r7
	vmov.64	d23, d30
    vld1.64 d30, [r2], r7
	veor.64	d22, d22, d30
    vst1.64 d22, [r9], r7
	vmov.64	d22, d30
    vld1.64 d30, [r2], r7
	veor.64	d21, d21, d30
    vst1.64 d21, [r9], r7
	vmov.64	d21, d30
    vld1.64 d30, [r2], r7
	veor.64	d20, d20, d30
    vst1.64 d20, [r9], r7
	vmov.64	d20, d30
    vld1.64 d30, [r2], r7
	veor.64	d19, d19, d30
    vst1.64 d19, [r9], r7
	vmov.64	d19, d30
    vld1.64 d30, [r2], r7
	veor.64	d17, d17, d30
    vst1.64 d17, [r9], r7
	vmov.64	d17, d30
    vld1.64 d30, [r2], r7
	veor.64	d15, d15, d30
    vst1.64 d15, [r9], r7
	vmov.64	d15, d30
    vld1.64 d30, [r2], r7
	veor.64	d13, d13, d30
    vst1.64 d13, [r9], r7
	vmov.64	d13, d30

	vld1.64	{d30}, [sp:64]
	bl		KeccakP1600_12_StateXORandPermuteAsmOnly
	subs	r8, r8, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Unwrap_Loop16OrMoreLanes
	b		KeccakP1600_12_SnP_FBWL_Unwrap_Done
KeccakP1600_12_SnP_FBWL_Unwrap_LoopLessThan16Lanes:
	rsb		r1, r5, #15							@ XOR up to 15 lanes
	add		r6, r6, r5, LSL #3					@ dataIn += laneCount * 8
	add		r3, r3, r5, LSL #3					@ dataOut += laneCount * 8
	sub		r2, r6, #8							@ tempdataIn = dataIn - 8
	sub		r9, r3, #8							@ tempdataOut = dataOut - 8
	add		pc, pc, r1, LSL #4
	mov		r1, #0								@ dummy instruction for PC alignment, not executed

    vld1.64 d30, [r2], r7
	veor.64	d18, d18, d30
    vst1.64 d18, [r9], r7
	vmov.64	d18, d30
    vld1.64 d30, [r2], r7
	veor.64	d16, d16, d30
    vst1.64 d16, [r9], r7
	vmov.64	d16, d30
    vld1.64 d30, [r2], r7
	veor.64	d14, d14, d30
    vst1.64 d14, [r9], r7
	vmov.64	d14, d30
    vld1.64 d30, [r2], r7
	veor.64	d12, d12, d30
    vst1.64 d12, [r9], r7
	vmov.64	d12, d30
    vld1.64 d30, [r2], r7
	veor.64	d10, d10, d30
    vst1.64 d10, [r9], r7
	vmov.64	d10, d30

    vld1.64 d30, [r2], r7
	veor.64	d9, d9, d30
    vst1.64 d9, [r9], r7
	vmov.64	d9, d30
    vld1.64 d30, [r2], r7
	veor.64	d7, d7, d30
    vst1.64 d7, [r9], r7
	vmov.64	d7, d30
    vld1.64 d30, [r2], r7
	veor.64	d5, d5, d30
    vst1.64 d5, [r9], r7
	vmov.64	d5, d30
    vld1.64 d30, [r2], r7
	veor.64	d3, d3, d30
    vst1.64 d3, [r9], r7
	vmov.64	d3, d30
    vld1.64 d30, [r2], r7
	veor.64	d1, d1, d30
    vst1.64 d1, [r9], r7
	vmov.64	d1, d30

    vld1.64 d30, [r2], r7
	veor.64	d8, d8, d30
    vst1.64 d8, [r9], r7
	vmov.64	d8, d30
    vld1.64 d30, [r2], r7
	veor.64	d6, d6, d30
    vst1.64 d6, [r9], r7
	vmov.64	d6, d30
    vld1.64 d30, [r2], r7
	veor.64	d4, d4, d30
    vst1.64 d4, [r9], r7
	vmov.64	d4, d30
    vld1.64 d30, [r2], r7
	veor.64	d2, d2, d30
    vst1.64 d2, [r9], r7
	vmov.64	d2, d30
    vld1.64 d30, [r2], r7
	veor.64	d0, d0, d30
    vst1.64 d0, [r9], r7
	vmov.64	d0, d30

	vld1.64	{d30}, [sp:64]
	bl		KeccakP1600_12_StateXORandPermuteAsmOnly
	subs	r8, r8, r5							@ nbrLanes -= laneCount
	bcs		KeccakP1600_12_SnP_FBWL_Unwrap_LoopLessThan16Lanes
	b		KeccakP1600_12_SnP_FBWL_Unwrap_Done


