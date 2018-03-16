@
@ Implementation by Ronny Van Keer, hereby denoted as "the implementer".
@
@ For more information, feedback or questions, please refer to our website:
@ https://keccak.team/
@
@ To the extent possible under law, the implementer has waived all copyright
@ and related or neighboring rights to the source code in this file.
@ http://creativecommons.org/publicdomain/zero/1.0/
@

@ WARNING: These functions work only on little endian CPU with@ ARMv7m architecture (Cortex-M3, ...).


    .thumb
	.syntax unified
.text

.equ _ba       , 0*2
.equ _be       , 1*2
.equ _bi       , 2*2
.equ _bo       , 3*2
.equ _bu       , 4*2
.equ _ga       , 5*2
.equ _ge       , 6*2
.equ _gi       , 7*2
.equ _go       , 8*2
.equ _gu       , 9*2
.equ _ka       , 10*2
.equ _ke       , 11*2
.equ _ki       , 12*2
.equ _ko       , 13*2
.equ _ku       , 14*2
.equ _ma       , 15*2
.equ _me       , 16*2
.equ _mi       , 17*2
.equ _mo       , 18*2
.equ _mu       , 19*2
.equ _sa       , 20*2
.equ _se       , 21*2
.equ _si       , 22*2
.equ _so       , 23*2
.equ _su       , 24*2

.equ _spare         , 25*2    @ 16-bit
.equ _nBlock        , 26*2
.equ _plaintext     , 28*2
.equ _ciphertext    , 30*2
.equ _AllocSize     , 32*2

.macro    xor5        result,ptr,b,g,k,m,rs

    ldrh        \result, [\ptr, #\b]
    ldrh        r6, [\ptr, #\g]
    eor         \result, \result, \rs
    ldrh        \rs, [\ptr, #\k]
    eor         \result, \result, r6
    ldrh        r6, [\ptr, #\m]
    eor         \result, \result, \rs
    eor         \result, \result, r6
    .endm

.macro    xor5D       resultL,resultH,ptr,b,g,k,m,rsL,rsH

    bfi         \rsL, \rsH, #16, #16
    ldr         \resultL, [\ptr, #\b]
    ldr         r6, [\ptr, #\g]
    eor         \resultL, \resultL, \rsL
    ldr         \rsL, [\ptr, #\k]
    eor         \resultL, \resultL, r6
    ldr         r6, [\ptr, #\m]
    eor         \resultL, \resultL, \rsL
    eor         \resultL, \resultL, r6
    lsr         \resultH, \resultL, #16
    uxth        \resultL, \resultL
    .endm

.macro    xorrol      b, yy, rr

    eor         \b, \b, \yy
    lsl         \b, \b, #\rr
    orr         \b, \b, \b, LSR #16
    .endm

.macro    rolxor      d, a, b

    eor         \d, \a, \b, LSL #1
    eor         \d, \d, \b, LSR #15
    uxth        \d, \d
    .endm

.macro    xandnot     resptr, resofs, aa, bb, cc, temp

    bic         \temp, \cc, \bb
    eor         \temp, \temp, \aa
    strh        \temp, [\resptr, #\resofs]
    .endm

.macro    xandnotRC   resptr, resofs, aa, bb, cc

    bic         \cc, \cc, \bb
    eor         \cc, \cc, #0x0a
    eor         \cc, \cc, \aa
    strh        \cc, [\resptr, #\resofs]
    .endm

.macro    KeccakRound sOut, sIn

    @prepTheta
    xor5D       r1, r2, \sIn, _ba, _ga, _ka, _ma, r8, r9
    xor5D       r3, r4, \sIn, _bi, _gi, _ki, _mi, r10, r11
    rolxor      r8, r7, r2
    rolxor      r9, r1, r3
    rolxor      r10, r2, r4
    rolxor      r11, r3, r7
    rolxor      r12, r4, r1

    @thetaRhoPiChiIota
    ldrh        r1, [\sIn, #_ba]
    ldrh        r2, [\sIn, #_ge]
    ldrh        r3, [\sIn, #_ki]
    ldrh        r4, [\sIn, #_mo]
    eor         r1, r1, r8
    xorrol      r2, r9, 12
    xorrol      r3, r10, 11
    xorrol      r4, r11,  5
    xorrol      r5, r12, 14
    xandnot     \sOut, _be, r2, r3, r4, r6
    xandnot     \sOut, _bi, r3, r4, r5, r6
    xandnot     \sOut, _bo, r4, r5, r1, r6
    xandnot     \sOut, _bu, r5, r1, r2, r7
    xandnotRC   \sOut, _ba, r1, r2, r3

    ldrh        r1, [\sIn, #_bo]
    ldrh        r2, [\sIn, #_gu]
    ldrh        r3, [\sIn, #_ka]
    ldrh        r4, [\sIn, #_me]
    ldrh        r5, [\sIn, #_si]
    xorrol      r1, r11, 12
    xorrol      r2, r12,  4
    xorrol      r3, r8,  3
    xorrol      r4, r9, 13
    xorrol      r5, r10, 13
    xandnot     \sOut, _ga, r1, r2, r3, r6
    xandnot     \sOut, _ge, r2, r3, r4, r6
    xandnot     \sOut, _gi, r3, r4, r5, r6
    xandnot     \sOut, _go, r4, r5, r1, r6
    xandnot     \sOut, _gu, r5, r1, r2, r6
    eor         r7, r7, r6

    ldrh        r1, [\sIn, #_be]
    ldrh        r2, [\sIn, #_gi]
    ldrh        r3, [\sIn, #_ko]
    ldrh        r4, [\sIn, #_mu]
    ldrh        r5, [\sIn, #_sa]
    xorrol      r1, r9,  1
    xorrol      r2, r10,  6
    xorrol      r3, r11,  9
    xorrol      r4, r12,  8
    xorrol      r5, r8,  2
    xandnot     \sOut, _ka, r1, r2, r3, r6
    xandnot     \sOut, _ke, r2, r3, r4, r6
    xandnot     \sOut, _ki, r3, r4, r5, r6
    xandnot     \sOut, _ko, r4, r5, r1, r6
    xandnot     \sOut, _ku, r5, r1, r2, r6
    eor         r7, r7, r6

    ldrh        r1, [\sIn, #_bu]
    ldrh        r2, [\sIn, #_ga]
    ldrh        r3, [\sIn, #_ke]
    ldrh        r4, [\sIn, #_mi]
    ldrh        r5, [\sIn, #_so]
    xorrol      r1, r12, 11
    xorrol      r2, r8,  4
    xorrol      r3, r9, 10
    xorrol      r4, r10, 15
    xorrol      r5, r11,  8
    xandnot     \sOut, _ma, r1, r2, r3, r6
    xandnot     \sOut, _me, r2, r3, r4, r6
    xandnot     \sOut, _mi, r3, r4, r5, r6
    xandnot     \sOut, _mo, r4, r5, r1, r6
    xandnot     \sOut, _mu, r5, r1, r2, r6
    eor         r7, r7, r6

    ldrh        r1, [\sIn, #_bi]
    ldrh        r2, [\sIn, #_go]
    ldrh        r3, [\sIn, #_ku]
    ldrh        r4, [\sIn, #_ma]
    ldrh        r5, [\sIn, #_se]
    xorrol      r1, r10, 14
    xorrol      r2, r11,  7
    xorrol      r3, r12,  7
    xorrol      r4, r8,  9
    xorrol      r5, r9,  2
    xandnot     \sOut, _sa, r1, r2, r3, r8
    xandnot     \sOut, _se, r2, r3, r4, r9
    xandnot     \sOut, _si, r3, r4, r5, r10
    xandnot     \sOut, _so, r4, r5, r1, r11
    bic         r1, r2, r1
    eor         r5, r5, r1
    eor         r7, r7, r5
    uxth        r7, r7
    strh        r5, [\sOut, #_su]
    uxth        r5, r5
    .endm

@----------------------------------------------------------------------------
@
@ void KetSr_StateAddByte( void *state, unsigned char value, unsigned int offset )
@
.align 8
.global   KetSr_StateAddByte
.type	KetSr_StateAddByte, %function;
KetSr_StateAddByte:
    adr     r3, Ket_StateTwistIndexes
    lsr     r12, r2, #1
    ldrb    r3, [r3, r12]
    and     r2, r2, #1
    add     r3, r3, r2
    ldrb    r2, [r0, r3]
    eors    r1, r1, r2
    strb    r1, [r0, r3]
    bx      lr


@----------------------------------------------------------------------------
@
@ unsigned char KetSr_StateExtractByte( void *state, unsigned int offset )
@
.align 8
.global   KetSr_StateExtractByte
.type	KetSr_StateExtractByte, %function;
KetSr_StateExtractByte:
    adr     r3, Ket_StateTwistIndexes
    lsrs    r2, r1, #1
    ldrb    r3, [r3, r2]
    and     r1, r1, #1
    add     r3, r3, r1
    ldrb    r0, [r0, r3]
    bx      lr


@----------------------------------------------------------------------------
@
@ void KetSr_StateOverwrite( void *state, unsigned int offset, const unsigned char *data, unsigned int length )
@
.align 8
.global   KetSr_StateOverwrite
.type	KetSr_StateOverwrite, %function;
KetSr_StateOverwrite:
    cmp     r3, #0
    beq     KetSr_StateOverwrite_Exit
    push    {r4-r5}
    adr     r4, Ket_StateTwistIndexes
    adds    r4, r4, r1, LSR #1
    ands    r1, r1, #1
    beq     KetSr_StateOverwrite_Loop
    ldrb    r1, [r4], #1
    b       KetSr_StateOverwrite_OffsetOdd
KetSr_StateOverwrite_Loop:
    ldrb    r1, [r4], #1
    ldrb    r5, [r2], #1
    subs    r3, r3, #1
    strb    r5, [r0, r1]
    beq     KetSr_StateOverwrite_Done
KetSr_StateOverwrite_OffsetOdd:
    adds    r1, r1, #1
    ldrb    r5, [r2], #1
    subs    r3, r3, #1
    strb    r5, [r0, r1]
    bne     KetSr_StateOverwrite_Loop
KetSr_StateOverwrite_Done:
    pop     {r4-r5}
KetSr_StateOverwrite_Exit:
    bx      lr


@----------------------------------------------------------------------------
@
@ void KetSr_Step( void *state, unsigned int size, unsigned char framing )
@
.align 8
.global   KetSr_Step
.type	KetSr_Step, %function;
KetSr_Step:
    push    {r4-r12,lr}
    sub     sp, sp, #_AllocSize
    adr     r4, Ket_StateTwistIndexes       @ framing
    add     r4, r4, r1, LSR #1
    ldrb    r4, [r4]
    and     r1, r1, #1
    add     r1, r1, r4
    ldrb    r4, [r0, r1]
    eors    r2, r2, r4
    strb    r2, [r0, r1]
    ldrb    r2, [r0, #_ki]                  @ padding
    eor     r2, r2, #0x08
    strb    r2, [r0, #_ki]
    mov     r4, sp                         @ Odd number of blocks, so copy state to stack
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    sub     r0, r0, #2*24
    ldrh    r12, [r0, #_su]
    strh    r12, [sp, #_su]
    ldrh    r8, [sp, #_sa]
    ldrh    r9, [sp, #_se]
    ldrh    r10, [sp, #_si]
    ldrh    r12, [sp, #_su]
    ldrh    r11, [sp, #_so]
    mov     r5, r12
    xor5    r7, sp, _bu, _gu, _ku, _mu, r12
    bl      KeccakP400_1_StatePermuteFromStack
    add     sp, sp, #_AllocSize
    pop     {r4-r12,pc}


.align 8
Ket_StateTwistIndexes:
		.byte      0*2,  6*2, 12*2, 18*2, 24*2
		.byte      3*2,  9*2, 10*2, 16*2, 22*2
		.byte      1*2,  7*2, 13*2, 19*2, 20*2
		.byte      4*2,  5*2, 11*2, 17*2, 23*2
		.byte      2*2,  8*2, 14*2, 15*2, 21*2

@----------------------------------------------------------------------------
@
@ void KetSr_FeedAssociatedDataBlocks( void *state, const unsigned char *data, unsigned int nBlocks )
@
.align 8
.global   KetSr_FeedAssociatedDataBlocks
.type	KetSr_FeedAssociatedDataBlocks, %function;
KetSr_FeedAssociatedDataBlocks:
    push    {r4-r12,lr}
    sub     sp, sp, #_AllocSize
    lsrs    r3, r2, #1
    bcc     KetSr_FeedAssociatedDataBlocks_Even
    adds    r2, r2, #1
    str     r2, [sp, #_nBlock]
    mov     r4, sp                         @ Odd number of blocks, so copy state to stack
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    sub     r0, r0, #2*24
    ldrh    r12, [r0, #_su]
    strh    r12, [sp, #_su]
    ldrh    r8, [sp, #_sa]
    ldrh    r9, [sp, #_se]
    ldrh    r10, [sp, #_si]
    ldrh    r12, [sp, #_su]
    ldrh    r11, [sp, #_so]
    mov     r5, r12
    xor5    r7, sp, _bu, _gu, _ku, _mu, r12
    b       KetSr_FeedAssociatedDataBlocks_Odd
KetSr_FeedAssociatedDataBlocks_Even:        @ Even number of blocks
    ldrh    r8, [r0, #_sa]
    ldrh    r9, [r0, #_se]
    ldrh    r10, [r0, #_si]
    ldrh    r12, [r0, #_su]
    ldrh    r11, [r0, #_so]
    mov     r5, r12
    xor5    r7, r0, _bu, _gu, _ku, _mu, r12
KetSr_FeedAssociatedDataBlocks_Loop:
    str     r2, [sp, #_nBlock]
    ldr     r6, [r1], #4                   @ Get data (2 lanes)
    ldrh    lr, [r0, #_ba]                    @ Add lane 1
    eor     lr, lr, r6
    strh    lr, [r0, #_ba]
    ldrh    lr, [r0, #_ge]                    @ Add lane 2
    eor     lr, lr, r6, LSR #16
    strh    lr, [r0, #_ge]
    ldr     r6, [r0, #_ki]                 @ Add FRAMEBITS00 and padding
    eor     r6, r6, #0x0C
    str     r6, [r0, #_ki]
    str     r1, [sp, #_plaintext]
    bl      KeccakP400_1_StatePermuteToStack
    ldr     r1, [sp, #_plaintext]
KetSr_FeedAssociatedDataBlocks_Odd:
    ldr     r6, [r1], #4                   @ Get data (2 lanes)
    ldrh    lr, [sp, #_ba]                    @ Add lane 1
    eor     lr, lr, r6
    strh    lr, [sp, #_ba]
    ldrh    lr, [sp, #_ge]                    @ Add lane 2
    eor     lr, lr, r6, LSR #16
    strh    lr, [sp, #_ge]
    ldr     r6, [sp, #_ki]                 @ Add FRAMEBITS00 and padding
    eor     r6, r6, #0x0C
    str     r6, [sp, #_ki]
    str     r1, [sp, #_plaintext]
    bl      KeccakP400_1_StatePermuteFromStack
    ldr     r1, [sp, #_plaintext]
    ldr     r2, [sp, #_nBlock]
    subs    r2, r2, #2
    bne     KetSr_FeedAssociatedDataBlocks_Loop
    add     sp, sp, #_AllocSize
    pop     {r4-r12,pc}


@----------------------------------------------------------------------------
@
@ void KetSr_UnwrapBlocks( void *state, const unsigned char *ciphertext, unsigned char *plaintext, unsigned int nBlocks )
@
.align 8
.global   KetSr_UnwrapBlocks
.type	KetSr_UnwrapBlocks, %function;
KetSr_UnwrapBlocks:
    push    {r4-r12,lr}
    sub     sp, sp, #_AllocSize
    lsrs    r4, r3, #1
    bcc     KetSr_UnwrapBlocks_Even
    adds    r3, r3, #1
    str     r3, [sp, #_nBlock]
    mov     r4, sp                        @ Odd number of blocks, so copy state to stack
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    sub     r0, r0, #2*24
    ldrh    r12, [r0, #_su]
    strh    r12, [sp, #_su]
    ldrh    r8, [sp, #_sa]
    ldrh    r9, [sp, #_se]
    ldrh    r10, [sp, #_si]
    ldrh    r12, [sp, #_su]
    ldrh    r11, [sp, #_so]
    mov     r5, r12
    xor5    r7, sp, _bu, _gu, _ku, _mu, r12
    b       KetSr_UnwrapBlocks_Odd
KetSr_UnwrapBlocks_Even:                     @ Even number of blocks
    ldrh    r8, [r0, #_sa]
    ldrh    r9, [r0, #_se]
    ldrh    r10, [r0, #_si]
    ldrh    r12, [r0, #_su]
    ldrh    r11, [r0, #_so]
    mov     r5, r12
    xor5    r7, r0, _bu, _gu, _ku, _mu, r12
KetSr_UnwrapBlocks_Loop:
    str     r3, [sp, #_nBlock]
    ldr     r6, [r1], #4                   @ Get ciphertext (2 lanes)
    ldrh    lr, [r0, #_ba]                  @ Lane 1 from state
    eor     lr, lr, r6
    strh    lr, [r2], #2                    @ Save plaintext lane 1
    strh    r6, [r0, #_ba]                 @ Save ciphertext into state lane 1
    ldrh    lr, [r0, #_ge]                  @ Lane 2 from state
    eor     lr, lr, r6, LSR #16
    strh    lr, [r2], #2                    @ Save plaintext lane 2
    lsrs    r6, r6, #16
    strh    r6, [r0, #_ge]                 @ Save ciphertext into state lane 2
    ldr     r6, [r0, #_ki]                 @ Add FRAMEBITS11 and padding
    eor     r6, r6, #0x0F
    str     r6, [r0, #_ki]
    str     r1, [sp, #_ciphertext]
    str     r2, [sp, #_plaintext]
    bl      KeccakP400_1_StatePermuteToStack
    ldr     r1, [sp, #_ciphertext]
    ldr     r2, [sp, #_plaintext]
KetSr_UnwrapBlocks_Odd:
    ldr     r6, [r1], #4                   @ Get ciphertext (2 lanes)
    ldrh    lr, [sp, #_ba]                  @ Get lane 1 from state
    eor     lr, lr, r6
    strh    lr, [r2], #2                    @ Save plaintext lane 1
    strh    r6, [sp, #_ba]                 @ Save ciphertext into state lane 1
    ldrh    lr, [sp, #_ge]                  @ Get lane 2 from state
    eor     lr, lr, r6, LSR #16
    strh    lr, [r2], #2                    @ Save plaintext lane 2
    lsrs    r6, r6, #16
    strh    r6, [sp, #_ge]                 @ Save ciphertext into state lane 2
    ldr     r6, [sp, #_ki]                 @ Add FRAMEBITS11 and padding
    eor     r6, r6, #0x0F
    str     r6, [sp, #_ki]
    str     r1, [sp, #_ciphertext]
    str     r2, [sp, #_plaintext]
    bl      KeccakP400_1_StatePermuteFromStack
    ldr     r1, [sp, #_ciphertext]
    ldr     r2, [sp, #_plaintext]
    ldr     r3, [sp, #_nBlock]
    subs    r3, r3, #2
    bne     KetSr_UnwrapBlocks_Loop
    add     sp, sp, #_AllocSize
    pop     {r4-r12,pc}


@----------------------------------------------------------------------------
@
@ void KetSr_WrapBlocks( void *state, const unsigned char *plaintext, unsigned char *ciphertext, unsigned int nBlocks )
@
.align 8
.global   KetSr_WrapBlocks
.type	KetSr_WrapBlocks, %function;
KetSr_WrapBlocks:
    push    {r4-r12,lr}
    sub     sp, sp, #_AllocSize
    lsrs    r4, r3, #1
    bcc     KetSr_WrapBlocks_Even
    adds    r3, r3, #1
    str     r3, [sp, #_nBlock]
    mov     r4, sp                         @ Odd number of blocks, so copy state to stack
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    ldm     r0!, { r5, r8, r9, r10, r11, r12 }
    stm     r4!,     { r5, r8, r9, r10, r11, r12 }
    sub     r0, r0, #2*24
    ldrh    r12, [r0, #_su]
    strh    r12, [sp, #_su]
    ldrh    r8, [sp, #_sa]
    ldrh    r9, [sp, #_se]
    ldrh    r10, [sp, #_si]
    ldrh    r12, [sp, #_su]
    ldrh    r11, [sp, #_so]
    mov     r5, r12
    xor5    r7, sp, _bu, _gu, _ku, _mu, r12
    b       KetSr_WrapBlocks_Odd
KetSr_WrapBlocks_Even:                       @ Even number of blocks
    ldrh    r8, [r0, #_sa]
    ldrh    r9, [r0, #_se]
    ldrh    r10, [r0, #_si]
    ldrh    r12, [r0, #_su]
    ldrh    r11, [r0, #_so]
    mov     r5, r12
    xor5    r7, r0, _bu, _gu, _ku, _mu, r12
KetSr_WrapBlocks_Loop:
    str     r3, [sp, #_nBlock]
    ldr     r6, [r1], #4                   @ Get plaintext (2 lanes)
    ldrh    lr, [r0, #_ba]                  @ Get lane 1 from state
    eor     lr, lr, r6
    strh    lr, [r2], #2                    @ Save ciphertext lane 1
    strh    lr, [r0, #_ba]                  @ Save ciphertext into state lane 1
    ldrh    lr, [r0, #_ge]                  @ Get lane 2 from state
    eor     lr, lr, r6, LSR #16
    strh    lr, [r2], #2                    @ Save ciphertext lane 2
    strh    lr, [r0, #_ge]                  @ Save ciphertext into state lane 2
    ldr     r6, [r0, #_ki]                 @ Add FRAMEBITS11 and padding
    eor     r6, r6, #0x0F
    str     r6, [r0, #_ki]
    str     r1, [sp, #_plaintext]
    str     r2, [sp, #_ciphertext]
    bl      KeccakP400_1_StatePermuteToStack
    ldr     r1, [sp, #_plaintext]
    ldr     r2, [sp, #_ciphertext]
KetSr_WrapBlocks_Odd:
    ldr     r6, [r1], #4                   @ Get plaintext (2 lanes)
    ldrh    lr, [sp, #_ba]                  @ Get lane 1 from state
    eor     lr, lr, r6
    strh    lr, [r2], #2                    @ Save ciphertext lane 1
    strh    lr, [sp, #_ba]                  @ Save ciphertext into state lane 1
    ldrh    lr, [sp, #_ge]                  @ Get lane 2 from state
    eor     lr, lr, r6, LSR #16
    strh    lr, [r2], #2                    @ Save ciphertext lane 2
    strh    lr, [sp, #_ge]                  @ Save ciphertext into state lane 2
    ldr     r6, [sp, #_ki]                 @ Add FRAMEBITS11 and padding
    eor     r6, r6, #0x0F
    str     r6, [sp, #_ki]
    str     r1, [sp, #_plaintext]
    str     r2, [sp, #_ciphertext]
    bl      KeccakP400_1_StatePermuteFromStack
    ldr     r1, [sp, #_plaintext]
    ldr     r2, [sp, #_ciphertext]
    ldr     r3, [sp, #_nBlock]
    subs    r3, r3, #2
    bne     KetSr_WrapBlocks_Loop
    add     sp, sp, #_AllocSize
    pop     {r4-r12,pc}


@----------------------------------------------------------------------------
@
@ Keccak-P[400, 1] usable from asm only, from r0 to sp
@
KeccakP400_1_StatePermuteToStack:
    KeccakRound sp, r0
    bx          lr


@----------------------------------------------------------------------------
@
@ Keccak-P[400, 1] usable from asm only, from sp to r0
@
KeccakP400_1_StatePermuteFromStack:
    KeccakRound r0, sp
    bx          lr



