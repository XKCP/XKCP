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
; This file implements Keccak-p[200] in a SnP-compatible way.
; Please refer to SnP-documentation.h for more details.
;
; This implementation comes with KeccakP-200-SnP.h in the same folder.
; Please refer to LowLevel.build for the exact list of other files it must be combined with.
;

; WARNING: This implementation assumes a little endian CPU with ARMv6M architecture (e.g., Cortex-M0).

    PRESERVE8
    THUMB
    AREA    |.text|, CODE, READONLY

_ba equ  0
_be equ  1
_bi equ  2
_bo equ  3
_bu equ  4
_ga equ  5
_ge equ  6
_gi equ  7
_go equ  8
_gu equ  9
_ka equ 10
_ke equ 11
_ki equ 12
_ko equ 13
_ku equ 14
_ma equ 15
_me equ 16
_mi equ 17
_mo equ 18
_mu equ 19
_sa equ 20
_se equ 21
_si equ 22
_so equ 23
_su equ 24

    MACRO
    xor5        $result,$ptr,$b,$g,$k,$m,$s
    ldrb        $result, [$ptr, #$b]
    ldrb        r7, [$ptr, #$g]
    eors        $result, $result, r7
    ldrb        r7, [$ptr, #$k]
    eors        $result, $result, r7
    ldrb        r7, [$ptr, #$m]
    eors        $result, $result, r7
    ldrb        r7, [$ptr, #$s]
    eors        $result, $result, r7
    MEND

    MACRO
    xorrol      $b, $yy, $rr
    mov         r7, $yy
    eors        $b, $b, r7
    if          $rr != 0
    lsls        r7, $b, #$rr
    lsrs        $b, $b, #8-$rr
    orrs        $b, $b, r7
    uxtb        $b, $b
    endif
    MEND

    MACRO
    rolxor      $d, $a, $b, $rot
    sxtb        r7, $b
    rors        r7, r7, $rot
    eors        r7, r7, $a
    uxtb        r7, r7
    mov         $d, r7
    MEND

    MACRO
    xandnot     $resptr, $resofs, $aa, $bb, $cc, $temp
    mov         $temp, $cc
    bics        $temp, $temp, $bb
    eors        $temp, $temp, $aa
    strb        $temp, [$resptr, #$resofs]
    MEND

    MACRO
    xandnotRC   $resptr, $resofs, $aa, $bb, $cc, $rco
    bics        $cc, $cc, $bb
    eors        $cc, $cc, $aa
    mov         r7, r8
    ldrb        $bb, [r7, #$rco]
    eors        $cc, $cc, $bb
    strb        $cc, [$resptr, #$resofs]
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
    ldrb        r1, [$sIn, #_bo]
    ldrb        r2, [$sIn, #_gu]
    ldrb        r3, [$sIn, #_ka]
    ldrb        r4, [$sIn, #_me]
    ldrb        r5, [$sIn, #_si]
    xorrol      r1, r12, 4
    xorrol      r2, lr, 4
    xorrol      r3, r9, 3
    xorrol      r4, r10, 5
    xorrol      r5, r11, 5
    xandnot     $sOut, _ga, r1, r2, r3, r7
    xandnot     $sOut, _ge, r2, r3, r4, r7
    xandnot     $sOut, _gi, r3, r4, r5, r7
    xandnot     $sOut, _go, r4, r5, r1, r7
    xandnot     $sOut, _gu, r5, r1, r2, r7
    ldrb        r1, [$sIn, #_be]
    ldrb        r2, [$sIn, #_gi]
    ldrb        r3, [$sIn, #_ko]
    ldrb        r4, [$sIn, #_mu]
    ldrb        r5, [$sIn, #_sa]
    xorrol      r1, r10,  1
    xorrol      r2, r11,  6
    xorrol      r3, r12,  1
    xorrol      r4, lr,  0
    xorrol      r5, r9,  2
    xandnot     $sOut, _ka, r1, r2, r3, r7
    xandnot     $sOut, _ke, r2, r3, r4, r7
    xandnot     $sOut, _ki, r3, r4, r5, r7
    xandnot     $sOut, _ko, r4, r5, r1, r7
    xandnot     $sOut, _ku, r5, r1, r2, r7
    ldrb        r1, [$sIn, #_bu]
    ldrb        r2, [$sIn, #_ga]
    ldrb        r3, [$sIn, #_ke]
    ldrb        r4, [$sIn, #_mi]
    ldrb        r5, [$sIn, #_so]
    xorrol      r1, lr, 3
    xorrol      r2, r9, 4
    xorrol      r3, r10, 2
    xorrol      r4, r11, 7
    xorrol      r5, r12, 0
    xandnot     $sOut, _ma, r1, r2, r3, r7
    xandnot     $sOut, _me, r2, r3, r4, r7
    xandnot     $sOut, _mi, r3, r4, r5, r7
    xandnot     $sOut, _mo, r4, r5, r1, r7
    xandnot     $sOut, _mu, r5, r1, r2, r7
    ldrb        r1, [$sIn, #_bi]
    ldrb        r2, [$sIn, #_go]
    ldrb        r3, [$sIn, #_ku]
    ldrb        r4, [$sIn, #_ma]
    ldrb        r5, [$sIn, #_se]
    xorrol      r1, r11, 6
    xorrol      r2, r12, 7
    xorrol      r3, lr, 7
    xorrol      r4, r9, 1
    xorrol      r5, r10, 2
    xandnot     $sOut, _sa, r1, r2, r3, r7
    xandnot     $sOut, _se, r2, r3, r4, r7
    xandnot     $sOut, _si, r3, r4, r5, r7
    xandnot     $sOut, _so, r4, r5, r1, r7
    xandnot     $sOut, _su, r5, r1, r2, r7
    ldrb        r1, [$sIn, #_ba]
    ldrb        r2, [$sIn, #_ge]
    ldrb        r3, [$sIn, #_ki]
    ldrb        r4, [$sIn, #_mo]
    ldrb        r5, [$sIn, #_su]
    xorrol      r1, r9, 0
    xorrol      r2, r10, 4
    xorrol      r3, r11, 3
    xorrol      r4, r12, 5
    xorrol      r5, lr, 6
    xandnot     $sOut, _be, r2, r3, r4, r7
    xandnot     $sOut, _bi, r3, r4, r5, r7
    xandnot     $sOut, _bo, r4, r5, r1, r7
    xandnot     $sOut, _bu, r5, r1, r2, r7
    xandnotRC   $sOut, _ba, r1, r2, r3, $rco
    MEND

;----------------------------------------------------------------------------
;
; void KeccakP200_StaticInitialize( void )
;
    ALIGN   4
    EXPORT  KeccakP200_StaticInitialize
KeccakP200_StaticInitialize   PROC
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP200_Initialize(void *state)
;
    ALIGN   4
    EXPORT  KeccakP200_Initialize
KeccakP200_Initialize   PROC
    movs    r1, #0
    movs    r2, #0
    movs    r3, #0
    stmia   r0!, { r1 - r3 }
    stmia   r0!, { r1 - r3 }
    strb    r1, [r0]
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP200_AddByte(void *state, unsigned char byte, unsigned int offset)
; 
    ALIGN   4
    EXPORT  KeccakP200_AddByte
KeccakP200_AddByte   PROC
    ldrb    r3, [r0, r2]
    eors    r3, r3, r1
    strb    r3, [r0, r2]
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP200_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
    ALIGN   4
    EXPORT  KeccakP200_AddBytes
KeccakP200_AddBytes   PROC
    subs    r3, r3, #1
    bcc     KeccakP200_AddBytes_Exit
    adds    r0, r0, r2
    push    {r4,lr}
KeccakP200_AddBytes_Loop
    ldrb    r2, [r1, r3]
    ldrb    r4, [r0, r3]
    eors    r2, r2, r4
    strb    r2, [r0, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_AddBytes_Loop
    pop     {r4,pc}
KeccakP200_AddBytes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP200_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
; 
    ALIGN   4
    EXPORT  KeccakP200_OverwriteBytes
KeccakP200_OverwriteBytes   PROC
    subs    r3, r3, #1
    bcc     KeccakP200_OverwriteBytes_Exit
    adds    r0, r0, r2
KeccakP200_OverwriteBytes_Loop
    ldrb    r2, [r1, r3]
    strb    r2, [r0, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_OverwriteBytes_Loop
KeccakP200_OverwriteBytes_Exit
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP200_OverwriteWithZeroes(void *state, unsigned int byteCount)
;
    ALIGN   4
    EXPORT  KeccakP200_OverwriteWithZeroes
KeccakP200_OverwriteWithZeroes  PROC
    movs    r3, #0
    cmp     r1, #0
    beq     KeccakP200_OverwriteWithZeroes_Exit
KeccakP200_OverwriteWithZeroes_LoopBytes
    subs    r1, r1, #1
    strb    r3, [r0, r1]
    bne     KeccakP200_OverwriteWithZeroes_LoopBytes
KeccakP200_OverwriteWithZeroes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP200_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
; 
    ALIGN   4
    EXPORT  KeccakP200_ExtractBytes
KeccakP200_ExtractBytes   PROC
    subs    r3, r3, #1
    bcc     KeccakP200_ExtractBytes_Exit
    adds    r0, r0, r2
KeccakP200_ExtractBytes_Loop
    ldrb    r2, [r0, r3]
    strb    r2, [r1, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_ExtractBytes_Loop
KeccakP200_ExtractBytes_Exit
    bx      lr
    ENDP

; ----------------------------------------------------------------------------
; 
;  void KeccakP200_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
; 
    ALIGN   4
    EXPORT  KeccakP200_ExtractAndAddBytes
KeccakP200_ExtractAndAddBytes   PROC
    push    {r4,r5}
    adds    r0, r0, r3                              ; state += offset (offset register no longer needed, reuse for length)
    ldr     r3, [sp, #8]                            ; get length argument from stack
    subs    r3, r3, #1                              ; if length != 0
    bcc     KeccakP200_ExtractAndAddBytes_Exit
KeccakP200_ExtractAndAddBytes_Loop
    ldrb    r5, [r0, r3]
    ldrb    r4, [r1, r3]
    eors    r5, r5, r4
    strb    r5, [r2, r3]
    subs    r3, r3, #1
    bcs     KeccakP200_ExtractAndAddBytes_Loop
KeccakP200_ExtractAndAddBytes_Exit
    pop     {r4,r5}
    bx      lr
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP200_Permute_Nrounds( void *state, unsigned int nr )
;
    ALIGN   4
    EXPORT  KeccakP200_Permute_Nrounds
KeccakP200_Permute_Nrounds   PROC
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
    beq     KeccakP200_Permute_Nrounds_GoRoundLoop
    ldm     r0!, { r1, r2, r3, r4, r5 }  ; odd number of rounds: copy state to stack
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
KeccakP200_Permute_Nrounds_GoRoundLoop
    b       KeccakP200_Permute_RoundLoop
    ENDP

;----------------------------------------------------------------------------
;
; void KeccakP200_Permute_18rounds( void *state )
;
    ALIGN   4
    EXPORT  KeccakP200_Permute_18rounds
KeccakP200_Permute_18rounds   PROC
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

KeccakP200_Permute_RoundConstants18
    dcb     0x01
    dcb     0x82
    dcb     0x8a
    dcb     0x00
    dcb     0x8b
    dcb     0x01
    dcb     0x81
    dcb     0x09
    dcb     0x8a
    dcb     0x88
    dcb     0x09
    dcb     0x0a
    dcb     0x8b
    dcb     0x8b
    dcb     0x89
    dcb     0x03
    dcb     0x02
    dcb     0x80

    ALIGN   4
KeccakP200_Permute_RoundLoop
    mov     r8, r7
    KeccakRound r6, r0, 0
KeccakP200_Permute_RoundOdd
    KeccakRound r0, r6, 1
    adds    r7, r7, #2
    cmp     r2, #0x80
    beq     KeccakP200_Permute_Done
    b       KeccakP200_Permute_RoundLoop
KeccakP200_Permute_Done
    add     sp,sp,#25+7
    pop     { r1 - r5, r7 }
    mov     r8, r1
    mov     r9, r2
    mov     r10, r3
    mov     r11, r4
    mov     r12, r5
    pop     { r4 - r6, pc }
    ENDP

    END
