@
@ The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
@ Michaël Peeters and Gilles Van Assche. For more information, feedback or
@ questions, please refer to our website: http://keccak.noekeon.org/
@
@ Implementation by Ronny Van Keer, hereby denoted as "the implementer".
@
@ To the extent possible under law, the implementer has waived all copyright
@ and related or neighboring rights to the source code in this file.
@ http://creativecommons.org/publicdomain/zero/1.0/
@

@ WARNING: These functions work only on little endian CPU with@ ARMv7A + NEON architecture.
@ 
@ INFO: KeccakF1600_StatePermute() execution time is 4650 cycles on a Cortex-A8 (BeagleBone Black)

   @ PRESERVE8
.text
   .align 8

.global   KeccakF1600_Initialize
.global   KeccakF1600_StateInitialize
.global   KeccakF1600_StateComplementBit
.global   KeccakF1600_StateXORLanes
.global   KeccakF1600_StateXORBytesInLane
.global   KeccakF1600_StateExtractLanes
.global   KeccakF1600_StateExtractBytesInLane
.global   KeccakF1600_StatePermute
.global   KeccakF1600_StateXORPermuteExtract

@//----------------------------------------------------------------------------

@// --- offsets in state
.equ Aba, 0*8
.equ Aga, 5*8
.equ Aka, 10*8
.equ Ama, 15*8
.equ Asa, 20*8

@// --- macros

.macro    KeccakThetaRhoPiChiIota argA1, argA2, argA3, argA4, argA5

    @Prepare Theta
    @ Ca = Aba^Aga^Aka^Ama^Asa
    @ Ce = Abe^Age^Ake^Ame^Ase
    @ Ci = Abi^Agi^Aki^Ami^Asi
    @ Co = Abo^Ago^Ako^Amo^Aso
    @ Cu = Abu^Agu^Aku^Amu^Asu
    @ De = Ca^ROL64(Ci, 1)
    @ Di = Ce^ROL64(Co, 1)
    @ Do = Ci^ROL64(Cu, 1)
    @ Du = Co^ROL64(Ca, 1)
    @ Da = Cu^ROL64(Ce, 1)
    veor.64     q4,   q6, q7
    veor.64     q5,   q9, q10
    veor.64     d8,    d8,   d9
    veor.64     d10,    d10,   d11
    veor.64     d1,    d8,   d16
    veor.64     d2,    d10,   d17

    veor.64     q4,   q11, q12
    veor.64     q5,   q14, q15
    veor.64     d8,    d8,   d9
    veor.64     d10,    d10,   d11
    veor.64     d3,    d8,   d26

    vadd.u64    q4,   q1,  q1
    veor.64     d4,    d10,   d27
    vmov.64     d0,    d5
    vsri.64     q4,   q1,  #63

    vadd.u64    q5,   q2,  q2
    veor.64     q4,   q4,  q0
    vsri.64     q5,   q2,  #63
    vadd.u64    d7,    d1,   d1
    veor.64     \argA2, \argA2,d8
    veor.64     q5,   q5,  q1

    vsri.64     d7,    d1,   #63
    vshl.u64    d1,    \argA2,#44
    veor.64     \argA3, \argA3,d9
    veor.64     d7,    d7,   d4

    @ Ba = argA1^Da
    @ Be = ROL64((argA2^De), 44)
    @ Bi = ROL64((argA3^Di), 43)
    @ Bo = ROL64((argA4^Do), 21)
    @ Bu = ROL64((argA5^Du), 14)
    @ argA2 =   Be ^((~Bi)& Bo )
    @ argA3 =   Bi ^((~Bo)& Bu )
    @ argA4 =   Bo ^((~Bu)& Ba )
    @ argA5 =   Bu ^((~Ba)& Be ) 
    @ argA1 =   Ba ^((~Be)& Bi )
    @ argA1 ^= KeccakF1600RoundConstants[i+round]
    vsri.64     d1,    \argA2,   #64-44
    vshl.u64    d2,    \argA3,   #43
    vldr.64     d0,    [r0, #\argA1]
    veor.64     \argA4, \argA4,   d10
    vsri.64     d2,    \argA3,   #64-43
    vshl.u64    d3,    \argA4,   #21
    veor.64     \argA5, \argA5,   d11
    veor.64     d0,    d0,      d7
    vsri.64     d3,    \argA4,   #64-21
    vbic.64     d5,    d2,      d1
    vshl.u64    d4,    \argA5,   #14
    vbic.64     \argA2, d3,      d2
    vld1.64     d6,    [r1]!
    veor.64     d5,    d0
    vsri.64     d4,    \argA5,   #64-14
    veor.64     d5,    d6
    vbic.64     \argA5, d1,      d0
    vbic.64     \argA3, d4,      d3
    vbic.64     \argA4, d0,      d4
    veor.64     \argA2, d1
    vstr.64     d5,    [r0, #\argA1]
    veor.64     \argA3, d2    
    veor.64     \argA4, d3
    veor.64     \argA5, d4
    .endm

.macro    KeccakThetaRhoPiChi1 argA1, argA2, argA3, argA4, argA5

    @ Bi = ROL64((argA1^Da), 3)
    @ Bo = ROL64((argA2^De), 45)
    @ Bu = ROL64((argA3^Di), 61)
    @ Ba = ROL64((argA4^Do), 28)
    @ Be = ROL64((argA5^Du), 20)
    @ argA1 =   Ba ^((~Be)&  Bi )
    @ Ca ^= argA1
    @ argA2 =   Be ^((~Bi)&  Bo )
    @ argA3 =   Bi ^((~Bo)&  Bu )
    @ argA4 =   Bo ^((~Bu)&  Ba )
    @ argA5 =   Bu ^((~Ba)&  Be )
    veor.64     \argA2, \argA2,   d8
    veor.64     \argA3, \argA3,   d9
    vshl.u64    d3,    \argA2,   #45
    vldr.64     d6,    [r0, #\argA1]
    vshl.u64    d4,    \argA3,   #61
    veor.64     \argA4, \argA4,   d10
    vsri.64     d3,    \argA2,   #64-45
    veor.64     \argA5, \argA5,   d11
    vsri.64     d4,    \argA3,   #64-61
    vshl.u64    d0,    \argA4,   #28
    veor.64     d6,    d6,      d7
    vshl.u64    d1,    \argA5,   #20
    vbic.64     \argA3, d4,      d3
    vsri.64     d0,    \argA4,   #64-28
    vbic.64     \argA4, d0,      d4
    vshl.u64    d2,    d6,      #3
    vsri.64     d1,    \argA5,   #64-20
    veor.64     \argA4, d3
    vsri.64     d2,    d6,      #64-3
    vbic.64     \argA5, d1,     d0
    vbic.64     d6,    d2,     d1
    vbic.64     \argA2, d3,     d2
    veor.64     d6,    d0
    veor.64     \argA2, d1
    vstr.64     d6,    [r0, #\argA1]
    veor.64     \argA3, d2
    veor.64     d5,    d6
    veor.64     \argA5, d4
    .endm

.macro    KeccakThetaRhoPiChi2 argA1, argA2, argA3, argA4, argA5

    @ Bu = ROL64((argA1^Da), 18)
    @ Ba = ROL64((argA2^De), 1)
    @ Be = ROL64((argA3^Di), 6)
    @ Bi = ROL64((argA4^Do), 25)
    @ Bo = ROL64((argA5^Du), 8)
    @ argA1 =   Ba ^((~Be)&  Bi )
    @ Ca ^= argA1@
    @ argA2 =   Be ^((~Bi)&  Bo )
    @ argA3 =   Bi ^((~Bo)&  Bu )
    @ argA4 =   Bo ^((~Bu)&  Ba )
    @ argA5 =   Bu ^((~Ba)&  Be )
    veor.64     \argA3, \argA3,   d9
    veor.64     \argA4, \argA4,   d10
    vshl.u64    d1,    \argA3,   #6
    vldr.64     d6,    [r0, #\argA1]
    vshl.u64    d2,    \argA4,   #25
    veor.64     \argA5, \argA5,   d11
    vsri.64     d1,    \argA3,   #64-6
    veor.64     \argA2, \argA2,   d8
    vsri.64     d2,    \argA4,   #64-25
    vext.8       d3,   \argA5,   \argA5, #7
    veor.64     d6,    d6,      d7
    vbic.64     \argA3, d2,      d1
    vadd.u64    d0,    \argA2,   \argA2
    vbic.64     \argA4, d3,      d2
    vsri.64     d0,    \argA2,   #64-1
    vshl.u64    d4,    d6,      #18
    veor.64     \argA2, d1,      \argA4
    veor.64     \argA3, d0
    vsri.64     d4,    d6,      #64-18
    vstr.64     \argA3, [r0, #\argA1]
    veor.64     d5,    \argA3
    vbic.64     \argA5, d1,      d0
    vbic.64     \argA3, d4,      d3
    vbic.64     \argA4, d0,      d4
    veor.64     \argA3, d2
    veor.64     \argA4, d3
    veor.64     \argA5, d4
    .endm

.macro    KeccakThetaRhoPiChi3 argA1, argA2, argA3, argA4, argA5

    @ Be = ROL64((argA1^Da), 36)
    @ Bi = ROL64((argA2^De), 10)
    @ Bo = ROL64((argA3^Di), 15)
    @ Bu = ROL64((argA4^Do), 56)
    @ Ba = ROL64((argA5^Du), 27)
    @ argA1 =   Ba ^((~Be)&  Bi )
    @ Ca ^= argA1
    @ argA2 =   Be ^((~Bi)&  Bo )
    @ argA3 =   Bi ^((~Bo)&  Bu )
    @ argA4 =   Bo ^((~Bu)&  Ba )
    @ argA5 =   Bu ^((~Ba)&  Be )
    veor.64     \argA2, \argA2,   d8
    veor.64     \argA3, \argA3,   d9
    vshl.u64    d2,    \argA2,   #10
    vldr.64     d6,    [r0, #\argA1]
    vshl.u64    d3,    \argA3,   #15
    veor.64     \argA4, \argA4,   d10
    vsri.64     d2,    \argA2,   #64-10
    vsri.64     d3,    \argA3,   #64-15
    veor.64     \argA5, \argA5,   d11
    vext.8      d4,    \argA4,   \argA4, #1
    vbic.64     \argA2, d3,      d2
    vshl.u64    d0,    \argA5,   #27
    veor.64     d6,    d6,      d7
    vbic.64     \argA3, d4,      d3
    vsri.64     d0,    \argA5,   #64-27
    vshl.u64    d1,    d6,      #36
    veor.64     \argA3, d2
    vbic.64     \argA4, d0,      d4
    vsri.64     d1,    d6,      #64-36
    veor.64     \argA4, d3
    vbic.64     d6,    d2,      d1
    vbic.64     \argA5, d1,      d0
    veor.64     d6,    d0
    veor.64     \argA2, d1
    vstr.64     d6,    [r0, #\argA1]
    veor.64     d5,    d6
    veor.64     \argA5, d4
    .endm

.macro    KeccakThetaRhoPiChi4 argA1, argA2, argA3, argA4, argA5

    @ Bo = ROL64((argA1^Da), 41)
    @ Bu = ROL64((argA2^De), 2)
    @ Ba = ROL64((argA3^Di), 62)
    @ Be = ROL64((argA4^Do), 55)
    @ Bi = ROL64((argA5^Du), 39)
    @ argA1 =   Ba ^((~Be)&  Bi )
    @ Ca ^= argA1
    @ argA2 =   Be ^((~Bi)&  Bo )
    @ argA3 =   Bi ^((~Bo)&  Bu )
    @ argA4 =   Bo ^((~Bu)&  Ba )
    @ argA5 =   Bu ^((~Ba)&  Be )
    veor.64     \argA2, \argA2,   d8
    veor.64     \argA3, \argA3,   d9
    vshl.u64    d4,    \argA2,   #2
    veor.64     \argA5, \argA5,   d11
    vshl.u64    d0,    \argA3,   #62
    vldr.64     d6,    [r0, #\argA1]
    vsri.64     d4,    \argA2,   #64-2
    veor.64     \argA4, \argA4,   d10
    vsri.64     d0,    \argA3,   #64-62
    vshl.u64    d1,    \argA4,   #55
    veor.64     d6,    d6,      d7
    vshl.u64    d2,    \argA5,   #39
    vsri.64     d1,    \argA4,   #64-55
    vbic.64     \argA4, d0,      d4
    vsri.64     d2,    \argA5,   #64-39
    vbic.64     \argA2, d1,      d0
    vshl.u64    d3,    d6,      #41
    veor.64     \argA5, d4,      \argA2
    vbic.64     \argA2, d2,      d1
    vsri.64     d3,    d6,      #64-41
    veor.64     d6,    d0,      \argA2
    vbic.64     \argA2, d3,      d2
    vbic.64     \argA3, d4,      d3
    veor.64     \argA2, d1
    vstr.64     d6,    [r0, #\argA1]
    veor.64     d5,    d6
    veor.64     \argA3, d2
    veor.64     \argA4, d3
    .endm

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_Initialize( void )
@//
KeccakF1600_Initialize:  

    bx      lr

   
   .align 8

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StateInitialize(void *state)
@//
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

   
   .align 8

@//----------------------------------------------------------------------------
@//
@//    void KeccakF1600_StateComplementBit(void *state, unsigned int position)
@//
KeccakF1600_StateComplementBit:  

    lsrs    r2, r1, #5

    movs    r3, #1
    and     r1, r1, #31
    lsls    r3, r3, r1

    ldr     r1, [r0, r2, LSL #2]
    eors    r1, r1, r3
    str     r1, [r0, r2, LSL #2]
    bx      lr

   
   .align 8

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StateXORLanes(void *state, const unsigned char *data, unsigned int laneCount)
@// 
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

   
   .align 8

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StateXORBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
@//
KeccakF1600_StateXORBytesInLane:  

    push    {r4 - r7}
    ldr     r7, [sp, #4*4]
    cmp     r7, #0
    beq     KeccakF1600_StateXORBytesInLane_Exit

    movs    r4, #0
    movs    r5, #0
    push    { r4 - r5 }

    add     r3, r3, sp
KeccakF1600_StateXORBytesInLane_Loop:
    ldrb    r5, [r2], #1
    strb    r5, [r3], #1
    subs    r7, r7, #1
    bne     KeccakF1600_StateXORBytesInLane_Loop

    pop     { r4 - r5 }

    add     r0, r0, r1, LSL #3
    ldrd    r6, r7, [r0]
    eor     r6, r4
    eor     r7, r5
    strd    r6, r7, [r0]

KeccakF1600_StateXORBytesInLane_Exit:
    pop     {r4 - r7}
    bx      lr

   
   .align 8

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StateExtractLanes(const void *state, unsigned char *data, unsigned int laneCount)
@//
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

   
   .align 8

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StateExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
@//
KeccakF1600_StateExtractBytesInLane:  

    push    {r4 - r5}
    ldr     r5, [sp, #2*4]
    cmp     r5, #0
    beq     KeccakF1600_StateExtractBytesInLane_Exit

    add     r0, r0, r1, LSL #3
    ldr     r1, [r0, #4]
    ldr     r0, [r0]
    push    {r0, r1 }

    add     r0, sp, r3
KeccakF1600_StateExtractBytesInLane_Loop:
    ldrb    r1, [r0], #1
    subs    r5, r5, #1
    strb    r1, [r2], #1
    bne     KeccakF1600_StateExtractBytesInLane_Loop
    add     sp, #8
KeccakF1600_StateExtractBytesInLane_Exit:
    pop     {r4 - r5}
    bx      lr

   
   .align 8

    .ltorg
KeccakF1600_StatePermute_RoundConstantsWithTerminator:
		.quad      0x0000000000000001
		.quad      0x0000000000008082
		.quad      0x800000000000808a
		.quad      0x8000000080008000
		.quad      0x000000000000808b
		.quad      0x0000000080000001
		.quad      0x8000000080008081
		.quad      0x8000000000008009
		.quad      0x000000000000008a
		.quad      0x0000000000000088
		.quad      0x0000000080008009
		.quad      0x000000008000000a
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

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StatePermute( const void *state )
@//
KeccakF1600_StatePermute:  

    vpush   {q4-q7}

    vldr.64 d0,  [r0, #0*8]
    vldr.64 d12, [r0, #1*8]
    vldr.64 d17, [r0, #2*8]
    vldr.64 d22, [r0, #3*8]
    vldr.64 d27, [r0, #4*8]

    vldr.64 d1,  [r0, #5*8]
    vldr.64 d13, [r0, #6*8]
    vldr.64 d18, [r0, #7*8]
    vldr.64 d23, [r0, #8*8]
    vldr.64 d28, [r0, #9*8]

    vldr.64 d2,  [r0, #10*8]
    vldr.64 d14, [r0, #11*8]
    vldr.64 d19, [r0, #12*8]
    vldr.64 d24, [r0, #13*8]
    vldr.64 d29, [r0, #14*8]

    vldr.64 d3,  [r0, #15*8]
    vldr.64 d15, [r0, #16*8]
    vldr.64 d20, [r0, #17*8]
    vldr.64 d25, [r0, #18*8]
    vldr.64 d30, [r0, #19*8]

    vldr.64 d4,  [r0, #20*8]
    vldr.64 d16, [r0, #21*8]
    vldr.64 d21, [r0, #22*8]
    vldr.64 d26, [r0, #23*8]
    vldr.64 d31, [r0, #24*8]

    veor.64 q0, q0, q1
    veor.64 d5, d0,  d1
    veor.64 d5, d5,  d4

    adr     r1, KeccakF1600_StatePermute_RoundConstantsWithTerminator
    movs    r2, #24

KeccakF1600_StatePermute_RoundLoop:
    KeccakThetaRhoPiChiIota     Aba, d13, d19, d25, d31
    KeccakThetaRhoPiChi1        Aka, d15, d21, d22, d28
    KeccakThetaRhoPiChi2        Asa, d12, d18, d24, d30
    KeccakThetaRhoPiChi3        Aga, d14, d20, d26, d27
    KeccakThetaRhoPiChi4        Ama, d16, d17, d23, d29

    KeccakThetaRhoPiChiIota     Aba, d15, d18, d26, d29
    KeccakThetaRhoPiChi1        Asa, d14, d17, d25, d28
    KeccakThetaRhoPiChi2        Ama, d13, d21, d24, d27
    KeccakThetaRhoPiChi3        Aka, d12, d20, d23, d31
    KeccakThetaRhoPiChi4        Aga, d16, d19, d22, d30

    KeccakThetaRhoPiChiIota     Aba, d14, d21, d23, d30
    KeccakThetaRhoPiChi1        Ama, d12, d19, d26, d28
    KeccakThetaRhoPiChi2        Aga, d15, d17, d24, d31
    KeccakThetaRhoPiChi3        Asa, d13, d20, d22, d29
    KeccakThetaRhoPiChi4        Aka, d16, d18, d25, d27

    KeccakThetaRhoPiChiIota     Aba, d12, d17, d22, d27
    KeccakThetaRhoPiChi1        Aga, d13, d18, d23, d28
    KeccakThetaRhoPiChi2        Aka, d14, d19, d24, d29
    KeccakThetaRhoPiChi3        Ama, d15, d20, d25, d30
    subs    r2, #4
    KeccakThetaRhoPiChi4        Asa, d16, d21, d26, d31
    bne     KeccakF1600_StatePermute_RoundLoop

    vstr.64 d12, [r0, #1*8]
    vstr.64 d17, [r0, #2*8]
    vstr.64 d22, [r0, #3*8]
    vstr.64 d27, [r0, #4*8]

    vstr.64 d13, [r0, #6*8]
    vstr.64 d18, [r0, #7*8]
    vstr.64 d23, [r0, #8*8]
    vstr.64 d28, [r0, #9*8]

    vstr.64 d14, [r0, #11*8]
    vstr.64 d19, [r0, #12*8]
    vstr.64 d24, [r0, #13*8]
    vstr.64 d29, [r0, #14*8]

    vstr.64 d15, [r0, #16*8]
    vstr.64 d20, [r0, #17*8]
    vstr.64 d25, [r0, #18*8]
    vstr.64 d30, [r0, #19*8]

    vstr.64 d16, [r0, #21*8]
    vstr.64 d21, [r0, #22*8]
    vstr.64 d26, [r0, #23*8]
    vstr.64 d31, [r0, #24*8]

    vpop    {q4-q7}
    bx      lr

   
   .align 8

@//----------------------------------------------------------------------------
@//
@// void KeccakF1600_StateXORPermuteExtract(void *state, const unsigned char *inData, unsigned int inLaneCount, unsigned char *outData, unsigned int outLaneCount)
@//
KeccakF1600_StateXORPermuteExtract:  

    push    { r4 - r6, lr }
    mov     r4, r0
    mov     r5, r3
    bl      KeccakF1600_StateXORLanes
    mov     r0, r4
    bl      KeccakF1600_StatePermute
    mov     r0, r4
    mov     r1, r5
    ldr     r2, [sp, #4*4]
    bl      KeccakF1600_StateExtractLanes
    pop     { r4 - r6, pc }

   
   .align 8

