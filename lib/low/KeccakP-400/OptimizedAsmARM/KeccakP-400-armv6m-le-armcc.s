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
; This file implements Keccak-p[400] in a SnP-compatible way.
; Please refer to SnP-documentation.h for more details.
;
; This implementation comes with KeccakP-400-SnP.h in the same folder.
; Please refer to LowLevel.build for the exact list of other files it must be combined with.
;

; WARNING: This implementation assumes a little endian CPU with ARMv6M architecture (e.g., Cortex-M0).

    PRESERVE8
    THUMB
    AREA    |.text|, CODE, READONLY

_ba     equ  0*2
_be     equ  1*2
_bi     equ  2*2
_bo     equ  3*2
_bu     equ  4*2
_ga     equ  5*2
_ge     equ  6*2
_gi     equ  7*2
_go     equ  8*2
_gu     equ  9*2
_ka     equ 10*2
_ke     equ 11*2
_ki     equ 12*2
_ko     equ 13*2
_ku     equ 14*2
_ma     equ 15*2
_me     equ 16*2
_mi     equ 17*2
_mo     equ 18*2
_mu     equ 19*2
_sa     equ 20*2
_se     equ 21*2
_si     equ 22*2
_so     equ 23*2
_su     equ 24*2

    MACRO
    xor5        $result,$ptr,$b,$g,$k,$m,$s
    ldrh        $result, [$ptr, #$b]
    ldrh        r7, [$ptr, #$g]
    eors        $result, $result, r7
    ldrh        r7, [$ptr, #$k]
    eors        $result, $result, r7
    ldrh        r7, [$ptr, #$m]
    eors        $result, $result, r7
    ldrh        r7, [$ptr, #$s]
    eors        $result, $result, r7
    MEND

    MACRO
    xorrol      $b, $yy, $rr
    mov         r7, $yy
    eors        $b, $b, r7
    if          $rr != 0
    lsls        r7, $b, #$rr
    lsrs        $b, $b, #16-$rr
    orrs        $b, $b, r7
    uxth        $b, $b
    endif
    MEND

    MACRO
    rolxor      $d, $a, $b, $rot
    sxth        r7, $b
    rors        r7, r7, $rot
    eors        r7, r7, $a
    uxth        r7, r7
    mov         $d, r7
    MEND

    MACRO
    xandnot     $resptr, $resofs, $aa, $bb, $cc, $temp
    mov         $temp, $cc
    bics        $temp, $temp, $bb
    eors        $temp, $temp, $aa
    strh        $temp, [$resptr, #$resofs]
    MEND

    MACRO
    xandnotRC   $resptr, $resofs, $aa, $bb, $cc, $rco
    bics        $cc, $cc, $bb
    eors        $cc, $cc, $aa
    mov         r7, r8
    ldrh        $bb, [r7, #$rco]
    eors        $cc, $cc, $bb
    strh        $cc, [$resptr, #$resofs]
    MEND

    MACRO
    KeccakRound     $sOut, $sIn, $rco

    ;prepTheta
    push        { $sOut }
    movs        $sOut, #31
    xor5        r1, $sIn, _ba, _ga, _ka, _ma, _sa
    xor5        r2, $sIn, _be, _ge, _ke, _me, _se
    xor5        r3, $sIn, _bi, _gi, _ki, _mi, _si
    xor5        r4, $sIn, _bo, _go, _ko, _mo, _so
    xor5        r5, $sIn, _bu, _gu, _ku, _mu, _su
    rolxor      r9, r5, r2, $sOut
    rolxor      r10, r1, r3, $sOut
    rolxor      r11, r2, r4, $sOut
    rolxor      r12, r3, r5, $sOut
    rolxor      lr, r4, r1, $sOut
    pop         { $sOut }

    ;thetaRhoPiChiIota
    ldrh        r1, [$sIn, #_bo]
    ldrh        r2, [$sIn, #_gu]
    ldrh        r3, [$sIn, #_ka]
    ldrh        r4, [$sIn, #_me]
    ldrh        r5, [$sIn, #_si]
    xorrol      r1, r12, 12
    xorrol      r2, lr,  4
    xorrol      r3, r9,  3
    xorrol      r4, r10, 13
    xorrol      r5, r11, 13
    xandnot     $sOut, _ga, r1, r2, r3, r7
    xandnot     $sOut, _ge, r2, r3, r4, r7
    xandnot     $sOut, _gi, r3, r4, r5, r7
    xandnot     $sOut, _go, r4, r5, r1, r7
    xandnot     $sOut, _gu, r5, r1, r2, r7

    ldrh        r1, [$sIn, #_be]
    ldrh        r2, [$sIn, #_gi]
    ldrh        r3, [$sIn, #_ko]
    ldrh        r4, [$sIn, #_mu]
    ldrh        r5, [$sIn, #_sa]
    xorrol      r1, r10,  1
    xorrol      r2, r11,  6
    xorrol      r3, r12,  9
    xorrol      r4, lr,  8
    xorrol      r5, r9,  2
    xandnot     $sOut, _ka, r1, r2, r3, r7
    xandnot     $sOut, _ke, r2, r3, r4, r7
    xandnot     $sOut, _ki, r3, r4, r5, r7
    xandnot     $sOut, _ko, r4, r5, r1, r7
    xandnot     $sOut, _ku, r5, r1, r2, r7

    ldrh        r1, [$sIn, #_bu]
    ldrh        r2, [$sIn, #_ga]
    ldrh        r3, [$sIn, #_ke]
    ldrh        r4, [$sIn, #_mi]
    ldrh        r5, [$sIn, #_so]
    xorrol      r1, lr, 11
    xorrol      r2, r9,  4
    xorrol      r3, r10, 10
    xorrol      r4, r11, 15
    xorrol      r5, r12,  8
    xandnot     $sOut, _ma, r1, r2, r3, r7
    xandnot     $sOut, _me, r2, r3, r4, r7
    xandnot     $sOut, _mi, r3, r4, r5, r7
    xandnot     $sOut, _mo, r4, r5, r1, r7
    xandnot     $sOut, _mu, r5, r1, r2, r7

    ldrh        r1, [$sIn, #_bi]
    ldrh        r2, [$sIn, #_go]
    ldrh        r3, [$sIn, #_ku]
    ldrh        r4, [$sIn, #_ma]
    ldrh        r5, [$sIn, #_se]
    xorrol      r1, r11, 14
    xorrol      r2, r12,  7
    xorrol      r3, lr,  7
    xorrol      r4, r9,  9
    xorrol      r5, r10,  2
    xandnot     $sOut, _sa, r1, r2, r3, r7
    xandnot     $sOut, _se, r2, r3, r4, r7
    xandnot     $sOut, _si, r3, r4, r5, r7
    xandnot     $sOut, _so, r4, r5, r1, r7
    xandnot     $sOut, _su, r5, r1, r2, r7

    ldrh        r1, [$sIn, #_ba]
    ldrh        r2, [$sIn, #_ge]
    ldrh        r3, [$sIn, #_ki]
    ldrh        r4, [$sIn, #_mo]
    ldrh        r5, [$sIn, #_su]
    xorrol      r1, r9, 0
    xorrol      r2, r10, 12
    xorrol      r3, r11, 11
    xorrol      r4, r12,  5
    xorrol      r5, lr, 14
    xandnot     $sOut, _be, r2, r3, r4, r7
    xandnot     $sOut, _bi, r3, r4, r5, r7
    xandnot     $sOut, _bo, r4, r5, r1, r7
    xandnot     $sOut, _bu, r5, r1, r2, r7
    xandnotRC   $sOut, _ba, r1, r2, r3, $rco
    MEND

;----------------------------------------------------------------------------
;
; void KeccakP400_StaticInitialize( void )
;
    ALIGN   4
    EXPORT  KeccakP400_StaticInitialize
KeccakP400_StaticInitialize   PROC
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP400_Initialize(void *state)
;
    ALIGN   4
    EXPORT  KeccakP400_Initialize
KeccakP400_Initialize   PROC
    movs    r1, #0
    movs    r2, #0
    movs    r3, #0
    stmia   r0!, { r1 - r3 }
    stmia   r0!, { r1 - r3 }
    stmia   r0!, { r1 - r3 }
    stmia   r0!, { r1 - r3 }
    strh    r1, [r0]
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP400_AddByte(void *state, unsigned char byte, unsigned int offset)
; 
    ALIGN   4
    EXPORT  KeccakP400_AddByte
KeccakP400_AddByte   PROC
    ldrb    r3, [r0, r2]
    eors    r3, r3, r1
    strb    r3, [r0, r2]
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP400_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
    ALIGN   4
    EXPORT  KeccakP400_AddBytes
KeccakP400_AddBytes   PROC
    subs    r3, r3, #1
    bcc     KeccakP400_AddBytes_Exit
    adds    r0, r0, r2
    push    {r4,lr}
KeccakP400_AddBytes_Loop
    ldrb    r2, [r1, r3]
    ldrb    r4, [r0, r3]
    eors    r2, r2, r4
    strb    r2, [r0, r3]
    subs    r3, r3, #1
    bcs     KeccakP400_AddBytes_Loop
    pop     {r4,pc}
KeccakP400_AddBytes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP400_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
; 
    ALIGN   4
    EXPORT  KeccakP400_OverwriteBytes
KeccakP400_OverwriteBytes   PROC
    subs    r3, r3, #1
    bcc     KeccakP400_OverwriteBytes_Exit
    adds    r0, r0, r2
KeccakP400_OverwriteBytes_Loop
    ldrb    r2, [r1, r3]
    strb    r2, [r0, r3]
    subs    r3, r3, #1
    bcs     KeccakP400_OverwriteBytes_Loop
KeccakP400_OverwriteBytes_Exit
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP400_OverwriteWithZeroes(void *state, unsigned int byteCount)
;
    ALIGN   4
    EXPORT  KeccakP400_OverwriteWithZeroes
KeccakP400_OverwriteWithZeroes  PROC
    movs    r3, #0
    cmp     r1, #0
    beq     KeccakP400_OverwriteWithZeroes_Exit
KeccakP400_OverwriteWithZeroes_LoopBytes
    subs    r1, r1, #1
    strb    r3, [r0, r1]
    bne     KeccakP400_OverwriteWithZeroes_LoopBytes
KeccakP400_OverwriteWithZeroes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP400_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
; 
    ALIGN   4
    EXPORT  KeccakP400_ExtractBytes
KeccakP400_ExtractBytes   PROC
    subs    r3, r3, #1
    bcc     KeccakP400_ExtractBytes_Exit
    adds    r0, r0, r2
KeccakP400_ExtractBytes_Loop
    ldrb    r2, [r0, r3]
    strb    r2, [r1, r3]
    subs    r3, r3, #1
    bcs     KeccakP400_ExtractBytes_Loop
KeccakP400_ExtractBytes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP400_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
; 
    ALIGN   4
    EXPORT  KeccakP400_ExtractAndAddBytes
KeccakP400_ExtractAndAddBytes   PROC
    push    {r4,r5}
    adds    r0, r0, r3                              ; state += offset (offset register no longer needed, reuse for length)
    ldr     r3, [sp, #8]                            ; get length argument from stack
    subs    r3, r3, #1                              ; if length != 0
    bcc     KeccakP400_ExtractAndAddBytes_Exit
KeccakP400_ExtractAndAddBytes_Loop
    ldrb    r5, [r0, r3]
    ldrb    r4, [r1, r3]
    eors    r5, r5, r4
    strb    r5, [r2, r3]
    subs    r3, r3, #1
    bcs     KeccakP400_ExtractAndAddBytes_Loop
KeccakP400_ExtractAndAddBytes_Exit
    pop     {r4,r5}
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP400_Permute_Nrounds( void *state, unsigned int nr )
;
    ALIGN   4
    EXPORT  KeccakP400_Permute_Nrounds
KeccakP400_Permute_Nrounds   PROC
    push    { r4 - r6, lr }
    mov     r2, r8
    mov     r3, r9
    mov     r4, r10
    mov     r5, r11
    mov     r6, r12
    push    { r2 - r7 }
    sub     sp, sp, #25*2+6
    mov     r6, sp
    adr     r7, KeccakP400_Permute_RoundConstants
    subs    r7, r7, r1
    subs    r7, r7, r1
    lsls    r1, r1, #31
    beq     KeccakP400_Permute_Nrounds_GoRoundLoop
    subs    r7, r7, #2                                ; odd number of rounds
    mov     r8, r7
    ldm     r0!, { r1, r2, r3, r4, r5, r7 } ; copy state to stack
    stm     r6!, { r1, r2, r3, r4, r5, r7 }
    ldm     r0!, { r1, r2, r3, r4, r5, r7 }
    stm     r6!, { r1, r2, r3, r4, r5, r7 }
    subs    r0, r0, #48
    subs    r6, r6, #48
    ldrh    r1, [r0, #_su]
    strh    r1, [r6, #_su]
    b       KeccakP400_Permute_RoundOdd
KeccakP400_Permute_Nrounds_GoRoundLoop
    b       KeccakP400_Permute_RoundLoop
    nop
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP400_Permute_20rounds( void *state )
;
    ALIGN   4
    EXPORT  KeccakP400_Permute_20rounds
KeccakP400_Permute_20rounds   PROC
    push        { r4 - r6, lr }
    mov         r2, r8
    mov         r3, r9
    mov         r4, r10
    mov         r5, r11
    mov         r6, r12
    push        { r2 - r7 }
    sub         sp, sp, #25*2+6
    mov         r6, sp
    adr         r7, KeccakP400_Permute_RoundConstants20
    b           KeccakP400_Permute_RoundLoop
    ALIGN       4
KeccakP400_Permute_RoundConstants20
    dcw         0x0001
    dcw         0x8082
    dcw         0x808a
    dcw         0x8000
    dcw         0x808b
    dcw         0x0001
    dcw         0x8081
    dcw         0x8009
    dcw         0x008a
    dcw         0x0088
    dcw         0x8009
    dcw         0x000a
    dcw         0x808b
    dcw         0x008b
    dcw         0x8089
    dcw         0x8003
    dcw         0x8002
    dcw         0x0080
    dcw         0x800a
    dcw         0x000a
KeccakP400_Permute_RoundConstants
    dcw         0xFF            ;terminator

KeccakP400_Permute_RoundLoop
    mov         r8, r7
    KeccakRound r6, r0, 0
KeccakP400_Permute_RoundOdd
    KeccakRound r0, r6, 2
    adds        r7, r7, #4
    ldrh        r1, [r7]
    cmp         r1, #0xFF
    beq         KeccakP400_Permute_Done
    b           KeccakP400_Permute_RoundLoop
KeccakP400_Permute_Done
    add         sp,sp,#25*2+6
    pop         { r1 - r5, r7 }
    mov         r8, r1
    mov         r9, r2
    mov         r10, r3
    mov         r11, r4
    mov         r12, r5
    pop         { r4 - r6, pc }
    ENDP

    END
