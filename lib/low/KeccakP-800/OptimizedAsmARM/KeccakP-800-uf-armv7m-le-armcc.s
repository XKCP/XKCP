;
; Implementation by Ronny Van Keer, hereby denoted as "the implementer".
;
; For more information, feedback or questions, please refer to our website:
; https://keccak.team/
;
; To the extent possible under law, the implementer has waived all copyright
; and related or neighboring rights to the source code in this file.
; http://creativecommons.org/publicdomain/zero/1.0/
;
; ---
;
; This file implements Keccak-p[800] in a SnP-compatible way.
; Please refer to SnP-documentation.h for more details.
;
; This implementation comes with KeccakP-800-SnP.h in the same folder.
; Please refer to LowLevel.build for the exact list of other files it must be combined with.
;

; WARNING: This implementation assumes a little endian CPU with ARMv7M architecture (e.g., Cortex-M3) and the ARMCC compiler.

        PRESERVE8
        THUMB
        AREA    |.text|, CODE, READONLY

; ----------------------------------------------------------------------------

_ba     equ  0*4
_be     equ  1*4
_bi     equ  2*4
_bo     equ  3*4
_bu     equ  4*4
_ga     equ  5*4
_ge     equ  6*4
_gi     equ  7*4
_go     equ  8*4
_gu     equ  9*4
_ka     equ 10*4
_ke     equ 11*4
_ki     equ 12*4
_ko     equ 13*4
_ku     equ 14*4
_ma     equ 15*4
_me     equ 16*4
_mi     equ 17*4
_mo     equ 18*4
_mu     equ 19*4
_sa     equ 20*4
_se     equ 21*4
_si     equ 22*4
_so     equ 23*4
_su     equ 24*4
_RFU    equ 25*4
_SAS    equ 26*4

; ----------------------------------------------------------------------------

    MACRO
    xor5    $result,$ptr,$rb,$g,$k,$m,$s
    ldr     $result, [$ptr, #$g]
    eors    $result, $result, $rb
    ldr     $rb, [$ptr, #$k]
    eors    $result, $result, $rb
    ldr     $rb, [$ptr, #$m]
    eors    $result, $result, $rb
    ldr     $rb, [$ptr, #$s]
    eors    $result, $result, $rb
    MEND

    MACRO   ;Theta effect
    mTe     $b, $yy, $rr
    eors    $b, $b, $yy
    if      $rr != 0
    ror     $b, $b, #32-$rr
    endif
    MEND

    MACRO   ;Chi Iota (1 lane)
    mCI     $resptr, $resofs, $ax0, $ax1, $ax2, $temp, $iota
    bics    $temp, $ax2, $ax1
    eors    $temp, $temp, $ax0
    if      $iota < 0x100
    eors    $temp, $temp, #$iota
    else
    mov     $ax1, #$iota & 0xFFFF
    if      $iota >= 0x10000
    movt    $ax1, #$iota >> 16
    endif
    eors    $temp, $temp, $ax1
    endif
    str     $temp, [$resptr, #$resofs]
    MEND

    MACRO   ;Chi (1 lane)
    mC      $resptr, $resofs, $ax0, $ax1, $ax2, $temp, $pTxor, $pTreg, $save
    bics    $temp, $ax2, $ax1
    eors    $temp, $temp, $ax0
    if      $save != 0
    str     $temp, [$resptr, #$resofs]
    endif
    if      $pTxor != 0
    eors    $pTreg, $pTreg, $temp
    endif
    MEND

    MACRO
    mKR     $stateOut,$stateIn,$iota

    ; prepare Theta
    xor5    r1, $stateIn, r9, _ga, _ka, _ma, _sa
    xor5    r2, $stateIn, r10, _ge, _ke, _me, _se
    eor     r9, r8, r2, ROR #31
    eor     r10, r1, r6, ROR #31
    eor     r11, r2, r7, ROR #31
    eor     r12, r6, r8, ROR #31
    eor     lr, r7, r1, ROR #31

    ; Theta Rho Pi Chi Iota
    eors    r1, r3, r11
    rors    r1, r1, #32-30
    ldr     r2, [$stateIn, #_go]
    ldr     r3, [$stateIn, #_ku]
    ldr     r4, [$stateIn, #_ma]
    ldr     r5, [$stateIn, #_se]
    mTe     r2, r12, 23
    mTe     r3, lr,  7
    mTe     r4, r9,  9
    mTe     r5, r10,  2
    mC      $stateOut, _su, r5, r1, r2, r8, 0, 0,   1
    mC      $stateOut, _so, r4, r5, r1, r7, 0, 0,   1
    mC      $stateOut, _si, r3, r4, r5, r6, 0, 0,   1
    mC      $stateOut, _se, r2, r3, r4, r4, 0, 0,   1
    mC      $stateOut, _sa, r1, r2, r3, r3, 0, 0,   1

    ldr     r1, [$stateIn, #_bu]
    ldr     r2, [$stateIn, #_ga]
    ldr     r4, [$stateIn, #_mi]
    ldr     r5, [$stateIn, #_so]
    mTe     r1, lr, 27
    mTe     r2, r9,  4
    mTe     r4, r11, 15
    mTe     r5, r12, 24
    mC      $stateOut, _mu, r5, r1, r2, r3, 1, r8, 1
    mC      $stateOut, _mo, r4, r5, r1, r3, 1, r7, 1
    ldr     r3, [$stateIn, #_ke]
    mTe     r3, r10, 10
    mC      $stateOut, _mi, r3, r4, r5, r5, 1, r6, 1
    mC      $stateOut, _me, r2, r3, r4, r4, 0, 0,   1
    mC      $stateOut, _ma, r1, r2, r3, r3, 0, 0,   1

    ldr     r1, [$stateIn, #_be]
    ldr     r2, [$stateIn, #_gi]
    ldr     r4, [$stateIn, #_mu]
    ldr     r5, [$stateIn, #_sa]
    mTe     r1, r10,  1
    mTe     r2, r11,  6
    mTe     r4, lr,  8
    mTe     r5, r9, 18
    mC      $stateOut, _ku, r5, r1, r2, r3, 1, r8, 1
    mC      $stateOut, _ko, r4, r5, r1, r3, 1, r7, 1
    ldr     r3, [$stateIn, #_ko]
    mTe     r3, r12, 25
    mC      $stateOut, _ki, r3, r4, r5, r5, 1, r6, 1
    mC      $stateOut, _ke, r2, r3, r4, r4, 0, 0,   1
    mC      $stateOut, _ka, r1, r2, r3, r3, 0, 0,   1

    ldr     r1, [$stateIn, #_bo]
    ldr     r2, [$stateIn, #_gu]
    ldr     r4, [$stateIn, #_me]
    ldr     r5, [$stateIn, #_si]
    mTe     r1, r12, 28
    mTe     r2, lr, 20
    mTe     r4, r10, 13
    mTe     r5, r11, 29
    mC      $stateOut, _gu, r5, r1, r2, r3, 1, r8, 1
    mC      $stateOut, _go, r4, r5, r1, r3, 1, r7, 1
    ldr     r3, [$stateIn, #_ka]
    mTe     r3, r9,  3
    mC      $stateOut, _gi, r3, r4, r5, r5, 1, r6, 1
    mC      $stateOut, _ge, r2, r3, r4, r4, 0, 0,   1
    mC      $stateOut, _ga, r1, r2, r3, r3, 0, 0,   1

    ldr     r1, [$stateIn, #_ba]
    ldr     r2, [$stateIn, #_ge]
    ldr     r3, [$stateIn, #_ki]
    ldr     r4, [$stateIn, #_mo]
    ldr     r5, [$stateIn, #_su]
    mTe     r1, r9,  0
    mTe     r2, r10, 12
    mTe     r3, r11, 11
    mTe     r4, r12, 21
    mTe     r5, lr, 14
    mC      $stateOut, _bu, r5, r1, r2, lr, 1, r8, 1
    mC      $stateOut, _bo, r4, r5, r1, r12, 1, r7, 1
    mC      $stateOut, _bi, r3, r4, r5, r11, 1, r6, 0
    mC      $stateOut, _be, r2, r3, r4, r10, 0, 0,   1
    mCI     $stateOut, _ba, r1, r2, r3, r9, $iota
    mov     r3, r11
    MEND

; ----------------------------------------------------------------------------
;
;  void KeccakP800_StaticInitialize( void )
;
    ALIGN
    EXPORT  KeccakP800_StaticInitialize
KeccakP800_StaticInitialize   PROC
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_Initialize(void *state)
;
    ALIGN
    EXPORT  KeccakP800_Initialize
KeccakP800_Initialize   PROC
    push    {r4 - r5}
    movs    r1, #0
    movs    r2, #0
    movs    r3, #0
    movs    r4, #0
    movs    r5, #0
    stmia   r0!, { r1 - r5 }
    stmia   r0!, { r1 - r5 }
    stmia   r0!, { r1 - r5 }
    stmia   r0!, { r1 - r5 }
    stmia   r0!, { r1 - r5 }
    pop     {r4 - r5}
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_AddByte(void *state, unsigned char byte, unsigned int offset)
;
    ALIGN
    EXPORT  KeccakP800_AddByte
KeccakP800_AddByte   PROC
    ldrb    r3, [r0, r2]
    eors    r3, r3, r1
    strb    r3, [r0, r2]
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
    ALIGN
    EXPORT  KeccakP800_AddBytes
KeccakP800_AddBytes   PROC
    push    {r4,lr}
    adds    r0, r0, r2                              ; state += offset
    subs    r3, r3, #4                              ; if length >= 4
    bcc     KeccakP800_AddBytes_Bytes
KeccakP800_AddBytes_LanesLoop                   ; then, perform on words
    ldr     r2, [r0]
    ldr     r4, [r1], #4
    eors    r2, r2, r4
    str     r2, [r0], #4
    subs    r3, r3, #4
    bcs     KeccakP800_AddBytes_LanesLoop
KeccakP800_AddBytes_Bytes
    adds    r3, r3, #3
    bcc     KeccakP800_AddBytes_Exit
KeccakP800_AddBytes_BytesLoop
    ldrb    r2, [r0]
    ldrb    r4, [r1], #1
    eors    r2, r2, r4
    strb    r2, [r0], #1
    subs    r3, r3, #1
    bcs     KeccakP800_AddBytes_BytesLoop
KeccakP800_AddBytes_Exit
    pop     {r4,pc}
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
    ALIGN
    EXPORT  KeccakP800_OverwriteBytes
KeccakP800_OverwriteBytes   PROC
    adds    r0, r0, r2                              ; state += offset
    subs    r3, r3, #4                              ; if length >= 4
    bcc     KeccakP800_OverwriteBytes_Bytes
KeccakP800_OverwriteBytes_LanesLoop         ; then, perform on words
    ldr     r2, [r1], #4
    str     r2, [r0], #4
    subs    r3, r3, #4
    bcs     KeccakP800_OverwriteBytes_LanesLoop
KeccakP800_OverwriteBytes_Bytes
    adds    r3, r3, #3
    bcc     KeccakP800_OverwriteBytes_Exit
KeccakP800_OverwriteBytes_BytesLoop
    ldrb    r2, [r1], #1
    strb    r2, [r0], #1
    subs    r3, r3, #1
    bcs     KeccakP800_OverwriteBytes_BytesLoop
KeccakP800_OverwriteBytes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount)
;
    ALIGN
    EXPORT  KeccakP800_OverwriteWithZeroes
KeccakP800_OverwriteWithZeroes  PROC
    movs    r3, #0
    lsrs    r2, r1, #2
    beq     KeccakP800_OverwriteWithZeroes_Bytes
KeccakP800_OverwriteWithZeroes_LoopLanes
    str     r3, [r0], #4
    subs    r2, r2, #1
    bne     KeccakP800_OverwriteWithZeroes_LoopLanes
KeccakP800_OverwriteWithZeroes_Bytes
    ands    r1, #3
    beq     KeccakP800_OverwriteWithZeroes_Exit
KeccakP800_OverwriteWithZeroes_LoopBytes
    strb    r3, [r0], #1
    subs    r1, r1, #1
    bne     KeccakP800_OverwriteWithZeroes_LoopBytes
KeccakP800_OverwriteWithZeroes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
    ALIGN
    EXPORT  KeccakP800_ExtractBytes
KeccakP800_ExtractBytes   PROC
    adds    r0, r0, r2                              ; state += offset
    subs    r3, r3, #4                              ; if length >= 4
    bcc     KeccakP800_ExtractBytes_Bytes
KeccakP800_ExtractBytes_LanesLoop               ; then, handle words
    ldr     r2, [r0], #4
    str     r2, [r1], #4
    subs    r3, r3, #4
    bcs     KeccakP800_ExtractBytes_LanesLoop
KeccakP800_ExtractBytes_Bytes
    adds    r3, r3, #3
    bcc     KeccakP800_ExtractBytes_Exit
KeccakP800_ExtractBytes_BytesLoop
    ldrb    r2, [r0], #1
    strb    r2, [r1], #1
    subs    r3, r3, #1
    bcs     KeccakP800_ExtractBytes_BytesLoop
KeccakP800_ExtractBytes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
;
    ALIGN
    EXPORT  KeccakP800_ExtractAndAddBytes
KeccakP800_ExtractAndAddBytes   PROC
    push    {r4,r5}
    adds    r0, r0, r3                                  ; state += offset (offset register no longer needed, reuse for length)
    ldr     r3, [sp, #8]                                ; get length argument from stack
    subs    r3, r3, #4                                  ; if length >= 4
    bcc     KeccakP800_ExtractAndAddBytes_Bytes
KeccakP800_ExtractAndAddBytes_LanesLoop         ; then, handle words
    ldr     r5, [r0], #4
    ldr     r4, [r1], #4
    eors    r5, r5, r4
    str     r5, [r2], #4
    subs    r3, r3, #4
    bcs     KeccakP800_ExtractAndAddBytes_LanesLoop
KeccakP800_ExtractAndAddBytes_Bytes
    adds    r3, r3, #3
    bcc     KeccakP800_ExtractAndAddBytes_Exit
KeccakP800_ExtractAndAddBytes_BytesLoop
    ldrb    r5, [r0], #1
    ldrb    r4, [r1], #1
    eors    r5, r5, r4
    strb    r5, [r2], #1
    subs    r3, r3, #1
    bcs     KeccakP800_ExtractAndAddBytes_BytesLoop
KeccakP800_ExtractAndAddBytes_Exit
    pop     {r4,r5}
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_Permute_Nrounds(void *state, unsigned int nrounds)
;
    ALIGN
    EXPORT  KeccakP800_Permute_Nrounds
KeccakP800_Permute_Nrounds   PROC
    push    {r4-r12,lr}
    sub     sp, sp, #_SAS
    adr     r2, KeccakP800_Permute_RoundPointers-4 ; get round function pointer from table
    ldr     r2, [r2, r1, LSL #2]
    orr     r2, r2, #1                            ; set the thumb bit, not needed for Keil, gcc?
    ldm     r0, {r9,r10,r11,r12,lr}
    tst     r1, #1
    beq     KeccakP800_Permute_Nrounds_PrepareTheta
    add     r0, r0, #5*4                  ; odd number of rounds
    mov     r1, sp                                  ; copy state to stack
    stm     r1!, {r9,r10,r11,r12,lr}
    ldm     r0!, {r3,r4,r5,r7,r8}
    stm     r1!, {r3,r4,r5,r7,r8}
    ldm     r0!, {r3,r4,r5,r7,r8}
    stm     r1!, {r3,r4,r5,r7,r8}
    ldm     r0!, {r3,r4,r5,r7,r8}
    stm     r1!, {r3,r4,r5,r7,r8}
    ldm     r0!, {r3,r4,r5,r7,r8}
    stm     r1!, {r3,r4,r5,r7,r8}
    sub     r0, r0, #25*4
KeccakP800_Permute_Nrounds_PrepareTheta
    mov     r3, r11
    xor5    r8, r0, lr, _gu, _ku, _mu, _su
    xor5    r7, r0, r12, _go, _ko, _mo, _so
    xor5    r6, r0, r11, _gi, _ki, _mi, _si
    bx      r2
    ENDP

    ALIGN
KeccakP800_Permute_RoundPointers
    dcd     KeccakP800_Permute_1
    dcd     KeccakP800_Permute_2
    dcd     KeccakP800_Permute_3
    dcd     KeccakP800_Permute_4
    dcd     KeccakP800_Permute_5
    dcd     KeccakP800_Permute_6
    dcd     KeccakP800_Permute_7
    dcd     KeccakP800_Permute_8
    dcd     KeccakP800_Permute_9
    dcd     KeccakP800_Permute_10
    dcd     KeccakP800_Permute_11
    dcd     KeccakP800_Permute_12
    dcd     KeccakP800_Permute_13
    dcd     KeccakP800_Permute_14
    dcd     KeccakP800_Permute_15
    dcd     KeccakP800_Permute_16
    dcd     KeccakP800_Permute_17
    dcd     KeccakP800_Permute_18
    dcd     KeccakP800_Permute_19
    dcd     KeccakP800_Permute_20
    dcd     KeccakP800_Permute_21
    dcd     KeccakP800_Permute_22

; ----------------------------------------------------------------------------
;
;  void KeccakP800_Permute_12rounds( void *state )
;
    ALIGN
    EXPORT  KeccakP800_Permute_12rounds
KeccakP800_Permute_12rounds   PROC
    push    {r4-r12,lr}
    sub     sp, sp, #_SAS
    ldm     r0, {r9,r10,r11,r12,lr}
    mov     r3, r11
    xor5    r8, r0, lr, _gu, _ku, _mu, _su
    xor5    r7, r0, r12, _go, _ko, _mo, _so
    xor5    r6, r0, r11, _gi, _ki, _mi, _si
    mKR     sp, r0, 0x80008009
    mKR     r0, sp, 0x8000000a
    mKR     sp, r0, 0x8000808b
    mKR     r0, sp, 0x0000008b
    mKR     sp, r0, 0x00008089
    mKR     r0, sp, 0x00008003
    mKR     sp, r0, 0x00008002
    mKR     r0, sp, 0x00000080
    mKR     sp, r0, 0x0000800a
    mKR     r0, sp, 0x8000000a
    mKR     sp, r0, 0x80008081
    mKR     r0, sp, 0x00008080
    str     r11, [r0, #_bi]
    add     sp,sp,#_SAS
    pop     {r4-r12,pc}
    ENDP

; ----------------------------------------------------------------------------
;
;  void KeccakP800_Permute_22rounds( void *state )
;
    ALIGN
    EXPORT  KeccakP800_Permute_22rounds
KeccakP800_Permute_22rounds   PROC
    push    {r4-r12,lr}
    sub     sp, sp, #_SAS
    ldm     r0, {r9,r10,r11,r12,lr}
    mov     r3, r11
    xor5    r8, r0, lr, _gu, _ku, _mu, _su
    xor5    r7, r0, r12, _go, _ko, _mo, _so
    xor5    r6, r0, r11, _gi, _ki, _mi, _si
KeccakP800_Permute_22
    mKR     sp, r0, 0x00000001
KeccakP800_Permute_21
    mKR     r0, sp, 0x00008082
KeccakP800_Permute_20
    mKR     sp, r0, 0x0000808a
KeccakP800_Permute_19
    mKR     r0, sp, 0x80008000
KeccakP800_Permute_18
    mKR     sp, r0, 0x0000808b
KeccakP800_Permute_17
    mKR     r0, sp, 0x80000001
KeccakP800_Permute_16
    mKR     sp, r0, 0x80008081
KeccakP800_Permute_15
    mKR     r0, sp, 0x00008009
KeccakP800_Permute_14
    mKR     sp, r0, 0x0000008a
KeccakP800_Permute_13
    mKR     r0, sp, 0x00000088
KeccakP800_Permute_12
    mKR     sp, r0, 0x80008009
KeccakP800_Permute_11
    mKR     r0, sp, 0x8000000a
KeccakP800_Permute_10
    mKR     sp, r0, 0x8000808b
KeccakP800_Permute_9
    mKR     r0, sp, 0x0000008b
KeccakP800_Permute_8
    mKR     sp, r0, 0x00008089
KeccakP800_Permute_7
    mKR     r0, sp, 0x00008003
KeccakP800_Permute_6
    mKR     sp, r0, 0x00008002
KeccakP800_Permute_5
    mKR     r0, sp, 0x00000080
KeccakP800_Permute_4
    mKR     sp, r0, 0x0000800a
KeccakP800_Permute_3
    mKR     r0, sp, 0x8000000a
KeccakP800_Permute_2
    mKR     sp, r0, 0x80008081
KeccakP800_Permute_1
    mKR     r0, sp, 0x00008080
    str     r11, [r0, #_bi]
    add     sp, sp, #_SAS
    pop     {r4-r12,pc}
    ENDP

    END
