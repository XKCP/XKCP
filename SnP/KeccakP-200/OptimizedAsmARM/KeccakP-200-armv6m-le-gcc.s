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
@ ---
@
@ This file implements Keccak-p[200] in a SnP-compatible way.
@ Please refer to SnP-documentation.h for more details.
@
@ This implementation comes with KeccakP-200-SnP.h in the same folder.
@ Please refer to LowLevel.build for the exact list of other files it must be combined with.
@

@ WARNING: This implementation assumes a little endian CPU with ARMv6M architecture (e.g., Cortex-M0) and the GCC compiler.


    .thumb
    .syntax unified
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

.macro  xor5        result,ptr,b,g,k,m,s
    ldrb        \result, [\ptr, #\b]
    ldrb        r7, [\ptr, #\g]
    eors        \result, \result, r7
    ldrb        r7, [\ptr, #\k]
    eors        \result, \result, r7
    ldrb        r7, [\ptr, #\m]
    eors        \result, \result, r7
    ldrb        r7, [\ptr, #\s]
    eors        \result, \result, r7
    .endm

.macro  xorrol      b, yy, rr
    mov         r7, \yy
    eors        \b, \b, r7
    .if         \rr != 0
    lsls        r7, \b, #\rr
    lsrs        \b, \b, #8-\rr
    orrs        \b, \b, r7
    uxtb        \b, \b
    .endif
    .endm

.macro  rolxor      d, a, b, rot
    sxtb        r7, \b
    rors        r7, r7, \rot
    eors        r7, r7, \a
    uxtb        r7, r7
    mov         \d, r7
    .endm

.macro  xandnot     resptr, resofs, aa, bb, cc, temp
    mov         \temp, \cc
    bics        \temp, \temp, \bb
    eors        \temp, \temp, \aa
    strb        \temp, [\resptr, #\resofs]
    .endm

.macro  xandnotRC   resptr, resofs, aa, bb, cc, rco
    bics        \cc, \cc, \bb
    eors        \cc, \cc, \aa
    mov         r7, r8
    ldrb        \bb, [r7, #\rco]
    eors        \cc, \cc, \bb
    strb        \cc, [\resptr, #\resofs]
    .endm

.macro  KeccakRound     sOut, sIn, rco
    @prepTheta
    push        { \sOut }
    movs        \sOut, #31
    xor5        r1, \sIn, _ba, _ga, _ka, _ma, _sa
    xor5        r2, \sIn, _be, _ge, _ke, _me, _se
    xor5        r3, \sIn, _bi, _gi, _ki, _mi, _si
    xor5        r4, \sIn, _bo, _go, _ko, _mo, _so
    xor5        r5, \sIn, _bu, _gu, _ku, _mu, _su
    rolxor      r9, r5, r2, \sOut
    rolxor      r10, r1, r3, \sOut
    rolxor      r11, r2, r4, \sOut
    rolxor      r12, r3, r5, \sOut
    rolxor      lr, r4, r1, \sOut
    pop         { \sOut }
    @thetaRhoPiChiIota
    ldrb        r1, [\sIn, #_bo]
    ldrb        r2, [\sIn, #_gu]
    ldrb        r3, [\sIn, #_ka]
    ldrb        r4, [\sIn, #_me]
    ldrb        r5, [\sIn, #_si]
    xorrol      r1, r12, 4
    xorrol      r2, lr, 4
    xorrol      r3, r9, 3
    xorrol      r4, r10, 5
    xorrol      r5, r11, 5
    xandnot     \sOut, _ga, r1, r2, r3, r7
    xandnot     \sOut, _ge, r2, r3, r4, r7
    xandnot     \sOut, _gi, r3, r4, r5, r7
    xandnot     \sOut, _go, r4, r5, r1, r7
    xandnot     \sOut, _gu, r5, r1, r2, r7
    ldrb        r1, [\sIn, #_be]
    ldrb        r2, [\sIn, #_gi]
    ldrb        r3, [\sIn, #_ko]
    ldrb        r4, [\sIn, #_mu]
    ldrb        r5, [\sIn, #_sa]
    xorrol      r1, r10,  1
    xorrol      r2, r11,  6
    xorrol      r3, r12,  1
    xorrol      r4, lr,  0
    xorrol      r5, r9,  2
    xandnot     \sOut, _ka, r1, r2, r3, r7
    xandnot     \sOut, _ke, r2, r3, r4, r7
    xandnot     \sOut, _ki, r3, r4, r5, r7
    xandnot     \sOut, _ko, r4, r5, r1, r7
    xandnot     \sOut, _ku, r5, r1, r2, r7
    ldrb        r1, [\sIn, #_bu]
    ldrb        r2, [\sIn, #_ga]
    ldrb        r3, [\sIn, #_ke]
    ldrb        r4, [\sIn, #_mi]
    ldrb        r5, [\sIn, #_so]
    xorrol      r1, lr, 3
    xorrol      r2, r9, 4
    xorrol      r3, r10, 2
    xorrol      r4, r11, 7
    xorrol      r5, r12, 0
    xandnot     \sOut, _ma, r1, r2, r3, r7
    xandnot     \sOut, _me, r2, r3, r4, r7
    xandnot     \sOut, _mi, r3, r4, r5, r7
    xandnot     \sOut, _mo, r4, r5, r1, r7
    xandnot     \sOut, _mu, r5, r1, r2, r7
    ldrb        r1, [\sIn, #_bi]
    ldrb        r2, [\sIn, #_go]
    ldrb        r3, [\sIn, #_ku]
    ldrb        r4, [\sIn, #_ma]
    ldrb        r5, [\sIn, #_se]
    xorrol      r1, r11, 6
    xorrol      r2, r12, 7
    xorrol      r3, lr, 7
    xorrol      r4, r9, 1
    xorrol      r5, r10, 2
    xandnot     \sOut, _sa, r1, r2, r3, r7
    xandnot     \sOut, _se, r2, r3, r4, r7
    xandnot     \sOut, _si, r3, r4, r5, r7
    xandnot     \sOut, _so, r4, r5, r1, r7
    xandnot     \sOut, _su, r5, r1, r2, r7
    ldrb        r1, [\sIn, #_ba]
    ldrb        r2, [\sIn, #_ge]
    ldrb        r3, [\sIn, #_ki]
    ldrb        r4, [\sIn, #_mo]
    ldrb        r5, [\sIn, #_su]
    xorrol      r1, r9, 0
    xorrol      r2, r10, 4
    xorrol      r3, r11, 3
    xorrol      r4, r12, 5
    xorrol      r5, lr, 6
    xandnot     \sOut, _be, r2, r3, r4, r7
    xandnot     \sOut, _bi, r3, r4, r5, r7
    xandnot     \sOut, _bo, r4, r5, r1, r7
    xandnot     \sOut, _bu, r5, r1, r2, r7
    xandnotRC   \sOut, _ba, r1, r2, r3, \rco
    .endm

@----------------------------------------------------------------------------
@
@ void KeccakP200_StaticInitialize( void )
@
.align 8
.global   KeccakP200_StaticInitialize
.type	KeccakP200_StaticInitialize, %function;
KeccakP200_StaticInitialize:
    bx      lr


@----------------------------------------------------------------------------
@
@ void KeccakP200_Initialize(void *state)
@
.align 8
.global   KeccakP200_Initialize
.type	KeccakP200_Initialize, %function;
KeccakP200_Initialize:
    movs    r1, #0
    movs    r2, #0
    movs    r3, #0
    stmia   r0!, { r1 - r3 }
    stmia   r0!, { r1 - r3 }
    strb    r1, [r0]
    bx      lr


@ ----------------------------------------------------------------------------
@
@  void KeccakP200_AddByte(void *state, unsigned char byte, unsigned int offset)
@
.align 8
.global   KeccakP200_AddByte
.type	KeccakP200_AddByte, %function;
KeccakP200_AddByte:
    ldrb    r3, [r0, r2]
    eors    r3, r3, r1
    strb    r3, [r0, r2]
    bx      lr


@----------------------------------------------------------------------------
@
@ void KeccakP200_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@
.align 8
.global   KeccakP200_AddBytes
.type	KeccakP200_AddBytes, %function;
KeccakP200_AddBytes:
    subs    r3, r3, #1
    bcc     KeccakP200_AddBytes_Exit
    adds    r0, r0, r2
    push    {r4,lr}
KeccakP200_AddBytes_Loop:
    ldrb    r2, [r1, r3]
    ldrb    r4, [r0, r3]
    eors    r2, r2, r4
    strb    r2, [r0, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_AddBytes_Loop
    pop     {r4,pc}
KeccakP200_AddBytes_Exit:
    bx      lr


@ ----------------------------------------------------------------------------
@
@  void KeccakP200_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@
.align 8
.global   KeccakP200_OverwriteBytes
.type	KeccakP200_OverwriteBytes, %function;
KeccakP200_OverwriteBytes:
    subs    r3, r3, #1
    bcc     KeccakP200_OverwriteBytes_Exit
    adds    r0, r0, r2
KeccakP200_OverwriteBytes_Loop:
    ldrb    r2, [r1, r3]
    strb    r2, [r0, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_OverwriteBytes_Loop
KeccakP200_OverwriteBytes_Exit:
    bx      lr


@----------------------------------------------------------------------------
@
@ void KeccakP200_OverwriteWithZeroes(void *state, unsigned int byteCount)
@
.align 8
.global   KeccakP200_OverwriteWithZeroes
.type	KeccakP200_OverwriteWithZeroes, %function;
KeccakP200_OverwriteWithZeroes:
    movs    r3, #0
    cmp     r1, #0
    beq     KeccakP200_OverwriteWithZeroes_Exit
KeccakP200_OverwriteWithZeroes_LoopBytes:
    subs    r1, r1, #1
    strb    r3, [r0, r1]
    bne     KeccakP200_OverwriteWithZeroes_LoopBytes
KeccakP200_OverwriteWithZeroes_Exit:
    bx      lr


@ ----------------------------------------------------------------------------
@
@  void KeccakP200_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
@
.align 8
.global   KeccakP200_ExtractBytes
.type	KeccakP200_ExtractBytes, %function;
KeccakP200_ExtractBytes:
    subs    r3, r3, #1
    bcc     KeccakP200_ExtractBytes_Exit
    adds    r0, r0, r2
KeccakP200_ExtractBytes_Loop:
    ldrb    r2, [r0, r3]
    strb    r2, [r1, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_ExtractBytes_Loop
KeccakP200_ExtractBytes_Exit:
    bx      lr


@ ----------------------------------------------------------------------------
@
@  void KeccakP200_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
@
.align 8
.global   KeccakP200_ExtractAndAddBytes
.type	KeccakP200_ExtractAndAddBytes, %function;
KeccakP200_ExtractAndAddBytes:
    push    {r4,r5}
    adds    r0, r0, r3                              @ state += offset (offset register no longer needed, reuse for length)
    ldr     r3, [sp, #8]                            @ get length argument from stack
    subs    r3, r3, #1                              @ .if length != 0
    bcc     KeccakP200_ExtractAndAddBytes_Exit
KeccakP200_ExtractAndAddBytes_Loop:
    ldrb    r5, [r0, r3]
    ldrb    r4, [r1, r3]
    eors    r5, r5, r4
    strb    r5, [r2, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_ExtractAndAddBytes_Loop
KeccakP200_ExtractAndAddBytes_Exit:
    pop     {r4,r5}
    bx      lr


@----------------------------------------------------------------------------
@
@ void KeccakP200_Permute_Nrounds( void *state, unsigned int nr )
@
.align 8
.global   KeccakP200_Permute_Nrounds
.type	KeccakP200_Permute_Nrounds, %function;
KeccakP200_Permute_Nrounds:
    push    { r4 - r6, lr }
    mov     r2, r8
    mov     r3, r9
    mov     r4, r10
    mov     r5, r11
    mov     r6, r12
    push    { r2 - r7 }
    sub     sp, sp, #25+7
    mov     r6, sp
    adr     r7, KeccakP200_Permute_RoundConstants18
    adds    r7, r7, #18
    subs    r7, r7, r1
    lsls    r1, r1, #31
    beq     KeccakP200_Permute_RoundLoop
    ldm     r0!, { r1, r2, r3, r4, r5 } @ odd number of rounds: copy state to stack
    subs    r0, r0, #20
    stm     r6!, { r1, r2, r3, r4, r5 }
    subs    r6, r6, #20
    ldr     r1, [r0, #_sa]
    str     r1, [r6, #_sa]
    ldrb    r1, [r0, #_su]
    strb    r1, [r6, #_su]
    subs    r7, r7, #1
    mov     r8, r7
    b       KeccakP200_Permute_RoundOdd
    nop


@----------------------------------------------------------------------------
@
@ void KeccakP200_Permute_18rounds( void *state )
@
.align 8
.global   KeccakP200_Permute_18rounds
.type	KeccakP200_Permute_18rounds, %function;
KeccakP200_Permute_18rounds:
    push    { r4 - r6, lr }
    mov     r2, r8
    mov     r3, r9
    mov     r4, r10
    mov     r5, r11
    mov     r6, r12
    push    { r2 - r7 }
    sub     sp, sp, #25+7
    mov     r6, sp
    adr     r7, KeccakP200_Permute_RoundConstants18
    b       KeccakP200_Permute_RoundLoop
    nop

KeccakP200_Permute_RoundConstants18:
        .byte       0x01
        .byte       0x82
        .byte       0x8a
        .byte       0x00
        .byte       0x8b
        .byte       0x01
        .byte       0x81
        .byte       0x09
        .byte       0x8a
        .byte       0x88
        .byte       0x09
        .byte       0x0a
        .byte       0x8b
        .byte       0x8b
        .byte       0x89
        .byte       0x03
        .byte       0x02
        .byte       0x80

.align 8
KeccakP200_Permute_RoundLoop:
    mov     r8, r7
    KeccakRound r6, r0, 0
KeccakP200_Permute_RoundOdd:
    KeccakRound r0, r6, 1
    adds    r7, r7, #2
    cmp     r2, #0x80
    beq     KeccakP200_Permute_Done
    b       KeccakP200_Permute_RoundLoop
KeccakP200_Permute_Done:
    add     sp,sp,#25+7
    pop     { r1 - r5, r7 }
    mov     r8, r1
    mov     r9, r2
    mov     r10, r3
    mov     r11, r4
    mov     r12, r5
    pop     { r4 - r6, pc }


