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

; WARNING: This implementation assumes a little endian CPU with ARMv6M architecture (e.g., Cortex-M0) and the ARMCC compiler.

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

    MACRO
    xor5        $result,$ptr,$b,$g,$k,$m,$s
    ldr         $result, [$ptr, #$b]
    ldr         r6, [$ptr, #$g]
    eors        $result, $result, r6
    ldr         r6, [$ptr, #$k]
    eors        $result, $result, r6
    ldr         r6, [$ptr, #$m]
    eors        $result, $result, r6
    ldr         r6, [$ptr, #$s]
    eors        $result, $result, r6
    MEND

    MACRO       ;  Theta effect
    te          $d, $a, $b
    rors        $b, $b, r4
    eors        $b, $b, $a
    mov         $d, $b
    MEND

    MACRO       ;  Theta Rho Pi
    trp         $rBx, $sIn, $oIn, $rD, $rot
    ldr         $rBx, [$sIn, #$oIn]
    mov         r6, $rD
    eors        $rBx, $rBx, r6
    if          $rot != 0
    movs        r6, #32-$rot
    rors        $rBx, $rBx, r6
    endif
    MEND

    MACRO       ;  Chi Iota
    ci          $sOut, $oOut, $ax0, $ax1, $ax2, $iota, $useax2
    if $useax2 != 0
    bics        $ax2, $ax2, $ax1
    eors        $ax2, $ax2, $ax0
    if $iota != 0
    mov         r6, r8
    ldm         r6!, { $ax1 }
    mov         r8, r6
    eors        $ax2, $ax2, $ax1
    endif
    str         $ax2, [$sOut, #$oOut]
    else
    movs        r6, $ax2
    bics        r6, r6, $ax1
    eors        r6, r6, $ax0
    str         r6, [$sOut, #$oOut]
    endif
    MEND

    MACRO
    KeccakRound     $sOut, $sIn

    ;  Prepare Theta effect
    movs    r4, #31
    xor5    r1, $sIn, _be, _ge, _ke, _me, _se
    xor5    r2, $sIn, _bu, _gu, _ku, _mu, _su
    mov     r6, r1
    te      r9, r2, r6
    xor5    r3, $sIn, _bi, _gi, _ki, _mi, _si
    te      r12, r3, r2
    xor5    r2, $sIn, _ba, _ga, _ka, _ma, _sa
    te      r10, r2, r3
    xor5    r3, $sIn, _bo, _go, _ko, _mo, _so
    te      lr, r3, r2
    te      r11, r1, r3

    ;  ThetaRhoPi ChiIota
    trp     r1, $sIn, _bo, r12, 28
    trp     r2, $sIn, _gu, lr, 20
    trp     r3, $sIn, _ka, r9,  3
    trp     r4, $sIn, _me, r10, 13
    trp     r5, $sIn, _si, r11, 29
    ci      $sOut, _gu, r5, r1, r2, 0, 0
    ci      $sOut, _go, r4, r5, r1, 0, 0
    ci      $sOut, _gi, r3, r4, r5, 0, 1
    ci      $sOut, _ge, r2, r3, r4, 0, 1
    ci      $sOut, _ga, r1, r2, r3, 0, 1

    trp     r1, $sIn, _be, r10,  1
    trp     r2, $sIn, _gi, r11,  6
    trp     r3, $sIn, _ko, r12, 25
    trp     r4, $sIn, _mu, lr,  8
    trp     r5, $sIn, _sa, r9, 18
    ci      $sOut, _ku, r5, r1, r2, 0, 0
    ci      $sOut, _ko, r4, r5, r1, 0, 0
    ci      $sOut, _ki, r3, r4, r5, 0, 1
    ci      $sOut, _ke, r2, r3, r4, 0, 1
    ci      $sOut, _ka, r1, r2, r3, 0, 1

    trp     r1, $sIn, _bu, lr, 27
    trp     r2, $sIn, _ga, r9,  4
    trp     r3, $sIn, _ke, r10, 10
    trp     r4, $sIn, _mi, r11, 15
    trp     r5, $sIn, _so, r12, 24
    ci      $sOut, _mu, r5, r1, r2, 0, 0
    ci      $sOut, _mo, r4, r5, r1, 0, 0
    ci      $sOut, _mi, r3, r4, r5, 0, 1
    ci      $sOut, _me, r2, r3, r4, 0, 1
    ci      $sOut, _ma, r1, r2, r3, 0, 1

    trp     r1, $sIn, _bi, r11, 30
    trp     r2, $sIn, _go, r12, 23
    trp     r3, $sIn, _ku, lr,  7
    trp     r4, $sIn, _ma, r9,  9
    trp     r5, $sIn, _se, r10,  2
    ci      $sOut, _su, r5, r1, r2, 0, 0
    ci      $sOut, _so, r4, r5, r1, 0, 0
    ci      $sOut, _si, r3, r4, r5, 0, 1
    ci      $sOut, _se, r2, r3, r4, 0, 1
    ci      $sOut, _sa, r1, r2, r3, 0, 1

    trp     r1, $sIn, _ba, r9, 0
    trp     r2, $sIn, _ge, r10, 12
    trp     r3, $sIn, _ki, r11, 11
    trp     r4, $sIn, _mo, r12, 21
    trp     r5, $sIn, _su, lr, 14
    ci      $sOut, _bu, r5, r1, r2, 0, 0
    ci      $sOut, _bo, r4, r5, r1, 0, 0
    ci      $sOut, _bi, r3, r4, r5, 0, 1
    ci      $sOut, _be, r2, r3, r4, 0, 1
    ci      $sOut, _ba, r1, r2, r3, 1, 1
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
;  void  KeccakP800_Initialize(void *state)
; 
    ALIGN
    EXPORT   KeccakP800_Initialize
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
    adds    r0, r0, r2                              ; state += offset
    ldrb    r2, [r0]
    eors    r2, r2, r1
    strb    r2, [r0]
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
    movs    r2, r0                                  ; and data pointer and offset both 32-bit aligned
    orrs    r2, r2, r1
    lsls    r2, #30
    bne     KeccakP800_AddBytes_Bytes
KeccakP800_AddBytes_LanesLoop                   ; then, perform on words
    ldr     r2, [r0]
    ldmia   r1!, {r4}
    eors    r2, r2, r4
    stmia   r0!, {r2}
    subs    r3, r3, #4
    bcs     KeccakP800_AddBytes_LanesLoop
KeccakP800_AddBytes_Bytes
    adds    r3, r3, #4
    beq     KeccakP800_AddBytes_Exit
    subs    r3, r3, #1
KeccakP800_AddBytes_BytesLoop
    ldrb    r2, [r0, r3]
    ldrb    r4, [r1, r3]
    eors    r2, r2, r4
    strb    r2, [r0, r3]
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
    movs    r2, r0                                  ; and data pointer and offset both 32-bit aligned
    orrs    r2, r2, r1
    lsls    r2, #30
    bne     KeccakP800_OverwriteBytes_Bytes
KeccakP800_OverwriteBytes_LanesLoop         ; then, perform on words
    ldmia   r1!, {r2}
    stmia   r0!, {r2}
    subs    r3, r3, #4
    bcs     KeccakP800_OverwriteBytes_LanesLoop
KeccakP800_OverwriteBytes_Bytes
    adds    r3, r3, #4
    beq     KeccakP800_OverwriteBytes_Exit
    subs    r3, r3, #1
KeccakP800_OverwriteBytes_BytesLoop
    ldrb    r2, [r1, r3]
    strb    r2, [r0, r3]
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
    stm     r0!, { r3 }
    subs    r2, r2, #1
    bne     KeccakP800_OverwriteWithZeroes_LoopLanes
KeccakP800_OverwriteWithZeroes_Bytes
    lsls    r1, r1, #32-2
    beq     KeccakP800_OverwriteWithZeroes_Exit
    lsrs    r1, r1, #32-2
KeccakP800_OverwriteWithZeroes_LoopBytes
    subs    r1, r1, #1
    strb    r3, [r0, r1]
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
    movs    r2, r0                                  ; and data pointer and offset both 32-bit aligned
    orrs    r2, r2, r1
    lsls    r2, #30
    bne     KeccakP800_ExtractBytes_Bytes
KeccakP800_ExtractBytes_LanesLoop               ; then, perform on words
    ldmia   r0!, {r2}
    stmia   r1!, {r2}
    subs    r3, r3, #4
    bcs     KeccakP800_ExtractBytes_LanesLoop
KeccakP800_ExtractBytes_Bytes
    adds    r3, r3, #4
    beq     KeccakP800_ExtractBytes_Exit
    subs    r3, r3, #1
KeccakP800_ExtractBytes_BytesLoop
    ldrb    r2, [r0, r3]
    strb    r2, [r1, r3]
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
    adds    r0, r0, r3                              ; state += offset (offset register no longer needed, reuse for length)
    ldr     r3, [sp, #8]                            ; get length argument from stack
    subs    r3, r3, #4                              ; if length >= 4
    bcc     KeccakP800_ExtractAndAddBytes_Bytes
    movs    r5, r0                                  ; and input/output/state pointer all 32-bit aligned
    orrs    r5, r5, r1
    orrs    r5, r5, r2
    lsls    r5, #30
    bne     KeccakP800_ExtractAndAddBytes_Bytes
KeccakP800_ExtractAndAddBytes_LanesLoop                 ; then, perform on words
    ldmia   r0!, {r5}
    ldmia   r1!, {r4}
    eors    r5, r5, r4
    stmia   r2!, {r5}
    subs    r3, r3, #4
    bcs     KeccakP800_ExtractAndAddBytes_LanesLoop
KeccakP800_ExtractAndAddBytes_Bytes
    adds    r3, r3, #4
    beq     KeccakP800_ExtractAndAddBytes_Exit
    subs    r3, r3, #1
KeccakP800_ExtractAndAddBytes_BytesLoop
    ldrb    r5, [r0, r3]
    ldrb    r4, [r1, r3]
    eors    r5, r5, r4
    strb    r5, [r2, r3]
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
    lsls    r2, r1, #2
    lsls    r1, r1, #31
    bne     KeccakP800_Permute_NroundsOdd
    adr     r1, KeccakP800_Permute_RoundConstants0
    subs    r1, r1, r2
    b       KeccakP800_Permute
KeccakP800_Permute_NroundsOdd
    adr     r1, KeccakP800_Permute_RoundConstants0
    subs    r1, r1, r2
    push    { r4 - r6, lr }
    mov     r2, r8
    mov     r3, r9
    mov     r4, r10
    mov     r5, r11
    mov     r6, r12
    push    { r2 - r7 }
    sub     sp, sp, #25*4+4
    mov     r8, r1
    ; copy state to stack and use stack state as input
    mov     r7, r0
    mov     r0, sp
    ldmia   r7!, {r1-r5}
    stmia   r0!, {r1-r5}
    ldmia   r7!, {r1-r5}
    stmia   r0!, {r1-r5}
    ldmia   r7!, {r1-r5}
    stmia   r0!, {r1-r5}
    ldmia   r7!, {r1-r5}
    stmia   r0!, {r1-r5}
    ldmia   r7!, {r1-r5}
    stmia   r0!, {r1-r5}
    subs    r0, r0, #100
    subs    r7, r7, #100
    b       KeccakP800_Permute_RoundLoop
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP800_Permute_12rounds( void *state )
; 
    ALIGN
    EXPORT  KeccakP800_Permute_12rounds
KeccakP800_Permute_12rounds   PROC
    adr     r1, KeccakP800_Permute_RoundConstants12
    b       KeccakP800_Permute
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP800_Permute_22rounds( void *state )
; 
    ALIGN
    EXPORT  KeccakP800_Permute_22rounds
KeccakP800_Permute_22rounds   PROC
    adr     r1, KeccakP800_Permute_RoundConstants22
    b       KeccakP800_Permute
    ENDP

    ALIGN
KeccakP800_Permute_RoundConstants22
    dcd         0x00000001
    dcd         0x00008082
    dcd         0x0000808a
    dcd         0x80008000
    dcd         0x0000808b
    dcd         0x80000001
    dcd         0x80008081
    dcd         0x00008009
    dcd         0x0000008a
    dcd         0x00000088
KeccakP800_Permute_RoundConstants12
    dcd         0x80008009
    dcd         0x8000000a
    dcd         0x8000808b
    dcd         0x0000008b
    dcd         0x00008089
    dcd         0x00008003
    dcd         0x00008002
    dcd         0x00000080
    dcd         0x0000800a
    dcd         0x8000000a
    dcd         0x80008081
    dcd         0x00008080
KeccakP800_Permute_RoundConstants0
    dcd         0xFF            ; terminator

; ----------------------------------------------------------------------------
; 
;  void KeccakP800_Permute( void *state, void *rc )
; 
    ALIGN
KeccakP800_Permute   PROC
    push    { r4 - r6, lr }
    mov     r2, r8
    mov     r3, r9
    mov     r4, r10
    mov     r5, r11
    mov     r6, r12
    push    { r2 - r7 }
    sub     sp, sp, #25*4+4
    mov     r8, r1
    mov     r7, sp
KeccakP800_Permute_RoundLoop
    KeccakRound r7, r0
    ldr     r6, [r6]
    cmp     r6, #0xFF
    beq     KeccakP800_Permute_Done
    mov     r6, r7
    mov     r7, r0
    mov     r0, r6
    b       KeccakP800_Permute_RoundLoop
KeccakP800_Permute_Done
    mov     r0, r7
    add     sp,sp,#25*4+4
    pop     { r2 - r7 }
    mov     r8, r2
    mov     r9, r3
    mov     r10, r4
    mov     r11, r5
    mov     r12, r6
    pop     { r4 - r6, pc }
    ENDP

    END
