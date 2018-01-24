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

; INFO: Tested on ATmega1280 simulator

; Registers used in all routines
#define zero    1
#define rpState 24
#define rX      26
#define rY      28
#define rZ      30
#define sp      0x3D

;----------------------------------------------------------------------------
;
; void KeccakP800_StaticInitialize( void )
;
.global KeccakP800_StaticInitialize

;----------------------------------------------------------------------------
;
; void KeccakP800_Initialize(void *state)
;
; argument state   is passed in r24:r25
;
.global KeccakP800_Initialize
KeccakP800_Initialize:
    movw    rZ, r24
    ldi     r23, 5*5            ; clear state (4 bytes/1 lane per iteration)
KeccakP800_Initialize_Loop:
    st      z+, zero
    st      z+, zero
    st      z+, zero
    st      z+, zero
    dec     r23
    brne    KeccakP800_Initialize_Loop
KeccakP800_StaticInitialize:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP800_AddByte(void *state, unsigned char data, unsigned int offset)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23, only LSB (r22) is used
; argument offset    is passed in r20:r21, only LSB (r20) is used
;
.global KeccakP800_AddByte
KeccakP800_AddByte:
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    ld      r0, Z
    eor     r0, r22
    st      Z, r0
    ret

;----------------------------------------------------------------------------
;
; void KeccakP800_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakP800_AddBytes
KeccakP800_AddBytes:
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    movw    rX, r22
    subi    r18, 8
    brcs    KeccakP800_AddBytes_Byte
    ;do 8 bytes per iteration
KeccakP800_AddBytes_Loop8:
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0

    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0

    subi    r18, 8
    brcc    KeccakP800_AddBytes_Loop8
KeccakP800_AddBytes_Byte:
    ldi     r19, 8
    add     r18, r19
    breq    KeccakP800_AddBytes_End
KeccakP800_AddBytes_Loop1:
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    dec     r18
    brne    KeccakP800_AddBytes_Loop1
KeccakP800_AddBytes_End:
    ret


;----------------------------------------------------------------------------
;
; void KeccakP800_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakP800_OverwriteBytes
KeccakP800_OverwriteBytes:
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    movw    rX, r22
    subi    r18, 8
    brcs    KeccakP800_OverwriteBytes_Byte
    ;do 8 bytes per iteration
KeccakP800_OverwriteBytes_Loop8:
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    ld      r0, X+
    st      Z+, r0
    subi    r18, 8
    brcc    KeccakP800_OverwriteBytes_Loop8
KeccakP800_OverwriteBytes_Byte:
    ldi     r19, 8
    add     r18, r19
    breq    KeccakP800_OverwriteBytes_End
KeccakP800_OverwriteBytes_Loop1:
    ld      r0, X+
    st      Z+, r0
    dec     r18
    brne    KeccakP800_OverwriteBytes_Loop1
KeccakP800_OverwriteBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount)
;
; argument state        is passed in r24:r25
; argument byteCount    is passed in r22:r23, only LSB (r22) is used
;
.global KeccakP800_OverwriteWithZeroes
KeccakP800_OverwriteWithZeroes:
    movw    rZ, r24         ;rZ = state
    mov     r23, r22
    lsr     r23
    lsr     r23
    breq    KeccakP800_OverwriteWithZeroes_Bytes
KeccakP800_OverwriteWithZeroes_LoopLanes:
    st      Z+, r1
    st      Z+, r1
    st      Z+, r1
    st      Z+, r1
    dec     r23
    brne    KeccakP800_OverwriteWithZeroes_LoopLanes
KeccakP800_OverwriteWithZeroes_Bytes:
    andi    r22, 3
    breq    KeccakP800_OverwriteWithZeroes_End
KeccakP800_OverwriteWithZeroes_LoopBytes:
    st      Z+, r1
    dec     r22
    brne    KeccakP800_OverwriteWithZeroes_LoopBytes
KeccakP800_OverwriteWithZeroes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP800_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakP800_ExtractBytes
KeccakP800_ExtractBytes:
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    movw    rX, r22
    subi    r18, 8
    brcs    KeccakP800_ExtractBytes_Byte
    ;do 8 bytes per iteration
KeccakP800_ExtractBytes_Loop8:
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    ld      r0, Z+
    st      X+, r0
    subi    r18, 8
    brcc    KeccakP800_ExtractBytes_Loop8
KeccakP800_ExtractBytes_Byte:
    ldi     r19, 8
    add     r18, r19
    breq    KeccakP800_ExtractBytes_End
KeccakP800_ExtractBytes_Loop1:
    ld      r0, Z+
    st      X+, r0
    dec     r18
    brne    KeccakP800_ExtractBytes_Loop1
KeccakP800_ExtractBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP800_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument input     is passed in r22:r23
; argument output    is passed in r20:r21
; argument offset    is passed in r18:r19, only LSB (r18) is used
; argument length    is passed in r16:r17, only LSB (r16) is used
;
.global KeccakP800_ExtractAndAddBytes
KeccakP800_ExtractAndAddBytes:
    tst     r16
    breq    KeccakP800_ExtractAndAddBytes_End
    push    r16
    push    r28
    push    r29
    movw    rZ, r24
    add     rZ, r18
    adc     rZ+1, zero
    movw    rX, r22
    movw    rY, r20
    subi    r16, 4
    brcs    KeccakP800_ExtractAndAddBytes_Byte
KeccakP800_ExtractAndAddBytes_LoopLane:
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    subi    r16, 4
    brcc    KeccakP800_ExtractAndAddBytes_LoopLane
KeccakP800_ExtractAndAddBytes_Byte:
    ldi     r19, 4
    add     r16, r19
    breq    KeccakP800_ExtractAndAddBytes_Done
KeccakP800_ExtractAndAddBytes_Loop1:
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    dec     r16
    brne    KeccakP800_ExtractAndAddBytes_Loop1
KeccakP800_ExtractAndAddBytes_Done:
    pop     r29
    pop     r28
    pop     r16
KeccakP800_ExtractAndAddBytes_End:
    ret


#define ROT_BIT(a)      ((a) & 7)
#define ROT_BYTE(a)     ((((a)/8 + !!(((a)%8) > 4)) & 3) * 5)

KeccakP800_RhoPiConstants:
    .BYTE    ROT_BIT( 1), ROT_BYTE( 3), 10 * 4
    .BYTE    ROT_BIT( 3), ROT_BYTE( 6),  7 * 4
    .BYTE    ROT_BIT( 6), ROT_BYTE(10), 11 * 4
    .BYTE    ROT_BIT(10), ROT_BYTE(15), 17 * 4
    .BYTE    ROT_BIT(15), ROT_BYTE(21), 18 * 4
    .BYTE    ROT_BIT(21), ROT_BYTE(28),  3 * 4
    .BYTE    ROT_BIT(28), ROT_BYTE(36),  5 * 4
    .BYTE    ROT_BIT(36), ROT_BYTE(45), 16 * 4
    .BYTE    ROT_BIT(45), ROT_BYTE(55),  8 * 4
    .BYTE    ROT_BIT(55), ROT_BYTE( 2), 21 * 4
    .BYTE    ROT_BIT( 2), ROT_BYTE(14), 24 * 4
    .BYTE    ROT_BIT(14), ROT_BYTE(27),  4 * 4
    .BYTE    ROT_BIT(27), ROT_BYTE(41), 15 * 4
    .BYTE    ROT_BIT(41), ROT_BYTE(56), 23 * 4
    .BYTE    ROT_BIT(56), ROT_BYTE( 8), 19 * 4
    .BYTE    ROT_BIT( 8), ROT_BYTE(25), 13 * 4
    .BYTE    ROT_BIT(25), ROT_BYTE(43), 12 * 4
    .BYTE    ROT_BIT(43), ROT_BYTE(62),  2 * 4
    .BYTE    ROT_BIT(62), ROT_BYTE(18), 20 * 4
    .BYTE    ROT_BIT(18), ROT_BYTE(39), 14 * 4
    .BYTE    ROT_BIT(39), ROT_BYTE(61), 22 * 4
    .BYTE    ROT_BIT(61), ROT_BYTE(20),  9 * 4
    .BYTE    ROT_BIT(20), ROT_BYTE(44),  6 * 4
    .BYTE    ROT_BIT(44), ROT_BYTE( 1),  1 * 4

KeccakP800_RoundConstants_22:
    .BYTE    0x01, 0x00, 0x00, 0x00
    .BYTE    0x82, 0x80, 0x00, 0x00
    .BYTE    0x8a, 0x80, 0x00, 0x00
    .BYTE    0x00, 0x80, 0x00, 0x80
    .BYTE    0x8b, 0x80, 0x00, 0x00
    .BYTE    0x01, 0x00, 0x00, 0x80
    .BYTE    0x81, 0x80, 0x00, 0x80
    .BYTE    0x09, 0x80, 0x00, 0x00
    .BYTE    0x8a, 0x00, 0x00, 0x00
    .BYTE    0x88, 0x00, 0x00, 0x00
KeccakP800_RoundConstants_12:
    .BYTE    0x09, 0x80, 0x00, 0x80
    .BYTE    0x0a, 0x00, 0x00, 0x80
    .BYTE    0x8b, 0x80, 0x00, 0x80
    .BYTE    0x8b, 0x00, 0x00, 0x00
    .BYTE    0x89, 0x80, 0x00, 0x00
    .BYTE    0x03, 0x80, 0x00, 0x00
    .BYTE    0x02, 0x80, 0x00, 0x00
    .BYTE    0x80, 0x00, 0x00, 0x00
    .BYTE    0x0a, 0x80, 0x00, 0x00
    .BYTE    0x0a, 0x00, 0x00, 0x80
    .BYTE    0x81, 0x80, 0x00, 0x80
    .BYTE    0x80, 0x80, 0x00, 0x00
KeccakP800_RoundConstants_0:
    .BYTE    0xFF, 0        ; terminator

    .text

    #define pRound        22        // 2 regs (22-23)

;----------------------------------------------------------------------------
;
; void KeccakP800_Permute_Nrounds( void *state, unsigned int nrounds )
;
; argument state     is passed in r24:r25
; argument nrounds   is passed in r22:r23 (only LSB (r22) is used)
;
.global KeccakP800_Permute_Nrounds
KeccakP800_Permute_Nrounds:
	mov		r26, r22
    ldi     pRound,   lo8(KeccakP800_RoundConstants_0)
    ldi     pRound+1, hi8(KeccakP800_RoundConstants_0)
	lsl		r26
	lsl		r26
    sub     pRound, r26
    sbc     pRound+1, zero
    rjmp    KeccakP800_Permute

;----------------------------------------------------------------------------
;
; void KeccakP800_Permute_22rounds( void *state )
;
.global KeccakP800_Permute_22rounds
KeccakP800_Permute_22rounds:
    ldi     pRound,   lo8(KeccakP800_RoundConstants_22)
    ldi     pRound+1, hi8(KeccakP800_RoundConstants_22)
    rjmp    KeccakP800_Permute

;----------------------------------------------------------------------------
;
; void KeccakP800_Permute_12rounds( void *state )
;
.global KeccakP800_Permute_12rounds
KeccakP800_Permute_12rounds:
    ldi     pRound,   lo8(KeccakP800_RoundConstants_12)
    ldi     pRound+1, hi8(KeccakP800_RoundConstants_12)
KeccakP800_Permute:
    push    r2
    push    r3
    push    r4
    push    r5
    push    r6
    push    r7
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15
    push    r16
    push    r17
    push    r28
    push    r29

    ; Allocate C variables (5*4)
    in      rZ,   sp
    in      rZ+1, sp+1
    sbiw     rZ, 5*4
    in      r0, 0x3F
    cli
    out     sp+1, rZ+1
    out     sp, rZ                ; Z points to 5 C lanes
    out     0x3F, r0

    ; Variables used in multiple operations
    #define rTemp        2      // 8 regs (2-9)
    #define rTempBis    10      // 8 regs (10-17)
    #define rTempTer    18      // 4 regs (18-21)
    #define pRound      22      // 2 regs (22-23)

    ; Initial Prepare Theta
    #define TCIPx       rTempTer

    ldi     TCIPx, 5
    movw    rY, rpState
KeccakInitialPrepTheta_Loop:
    ld      rTemp+0, Y+        ;state[x]
    ld      rTemp+1, Y+
    ld      rTemp+2, Y+
    ld      rTemp+3, Y+

    adiw    rY, 16
    ld      r0, Y+            ;state[5+x]
    eor     rTemp+0, r0
    ld      r0, Y+
    eor     rTemp+1, r0
    ld      r0, Y+
    eor     rTemp+2, r0
    ld      r0, Y+
    eor     rTemp+3, r0

    adiw    rY, 16
    ld      r0, Y+            ;state[10+x]
    eor     rTemp+0, r0
    ld      r0, Y+
    eor     rTemp+1, r0
    ld      r0, Y+
    eor     rTemp+2, r0
    ld      r0, Y+
    eor     rTemp+3, r0

    adiw    rY, 16
    ld      r0, Y+            ;state[15+x]
    eor     rTemp+0, r0
    ld      r0, Y+
    eor     rTemp+1, r0
    ld      r0, Y+
    eor     rTemp+2, r0
    ld      r0, Y+
    eor     rTemp+3, r0

    adiw    rY, 16
    ld      r0, Y+            ;state[20+x]
    eor     rTemp+0, r0
    ld      r0, Y+
    eor     rTemp+1, r0
    ld      r0, Y+
    eor     rTemp+2, r0
    ld      r0, Y+
    eor     rTemp+3, r0

    st      Z+, rTemp+0
    st      Z+, rTemp+1
    st      Z+, rTemp+2
    st      Z+, rTemp+3

    subi    rY, 80
    sbc     rY+1, zero

    subi    TCIPx, 1
    brne    KeccakInitialPrepTheta_Loop
    #undef  TCIPx

Keccak_RoundLoop:

    ; Theta
    #define TCplus          rX
    #define TCminus         rZ
    #define TCcoordX        rTempTer
    #define TCcoordY        rTempTer+1

    in      TCminus, sp
    in      TCminus+1, sp+1
    movw    TCplus, TCminus
    adiw    TCminus, 4*4
    adiw    TCplus, 1*4
    movw    rY, rpState

    ldi     TCcoordX, 0x16
KeccakTheta_Loop1:
    ld      rTemp+0, X+
    ld      rTemp+1, X+
    ld      rTemp+2, X+
    ld      rTemp+3, X+

    lsl     rTemp+0
    rol     rTemp+1
    rol     rTemp+2
    rol     rTemp+3
    adc     rTemp+0, zero

    ld      r0, Z+
    eor     rTemp+0, r0
    ld      r0, Z+
    eor     rTemp+1, r0
    ld      r0, Z+
    eor     rTemp+2, r0
    ld      r0, Z+
    eor     rTemp+3, r0

    ldi     TCcoordY, 5
KeccakTheta_Loop2:
    ld      r0, Y
    eor     r0, rTemp+0
    st      Y+, r0
    ld      r0, Y
    eor     r0, rTemp+1
    st      Y+, r0
    ld      r0, Y
    eor     r0, rTemp+2
    st      Y+, r0
    ld      r0, Y
    eor     r0, rTemp+3
    st      Y+, r0
    adiw    rY, 16

    dec     TCcoordY
    brne    KeccakTheta_Loop2

    subi    rY, 100-4
    sbc     rY+1, zero

    lsr     TCcoordX
    brcc    1f
    brne    KeccakTheta_Loop1
    rjmp    KeccakTheta_End
1:
    cpi     TCcoordX, 0x0B
    brne    2f
    sbiw    TCminus, 20
    rjmp    KeccakTheta_Loop1
2:
    sbiw    TCplus, 20
    rjmp    KeccakTheta_Loop1

KeccakTheta_End:
    #undef  TCplus
    #undef  TCminus
    #undef  TCcoordX
    #undef  TCcoordY

    ; Rho Pi
    #define RPpConst    rTempTer        // 2 regs
    #define RPindex     rTempTer+2
    #define RPpBitRot   rX
    #define RPpByteRot  pRound

    sbiw    rY, 16

    ld      rTemp+0, Y+
    ld      rTemp+1, Y+
    ld      rTemp+2, Y+
    ld      rTemp+3, Y+

    push    pRound
    push    pRound+1
    ldi     RPpConst,   lo8(KeccakP800_RhoPiConstants)
    ldi     RPpConst+1, hi8(KeccakP800_RhoPiConstants)
    ldi     RPpBitRot,   pm_lo8(bit_rot_jmp_table)
    ldi     RPpBitRot+1, pm_hi8(bit_rot_jmp_table)
    ldi     RPpByteRot,   pm_lo8(rotate32_0byte_left)
    ldi     RPpByteRot+1, pm_hi8(rotate32_0byte_left)

KeccakRhoPi_Loop:
    ; get rotation codes and state index
    movw    rZ, RPpConst
    lpm     r0, Z+              ;bits
    lpm     rTempBis, Z+        ;bytes
    lpm     RPindex, Z+
    movw    RPpConst, rZ

    ; do bit rotation
    movw    rZ, RPpBitRot
    add     rZ, r0
    adc     rZ+1, zero
    ijmp

KeccakRhoPi_RhoBitRotateDone:
    movw    rY, rpState
    add     rY, RPindex
    adc     rY+1, zero

    movw    rZ, RPpByteRot
    add     rZ, rTempBis
    adc     rZ+1, zero
    ijmp

KeccakRhoPi_PiStore:
    sbiw    rY, 4
    st      Y+, rTemp+0
    st      Y+, rTemp+1
    st      Y+, rTemp+2
    st      Y+, rTemp+3

    movw    rTemp+0, rTempBis+0
    movw    rTemp+2, rTempBis+2
KeccakRhoPi_RhoDone:
    subi    RPindex, 4
    brne    KeccakRhoPi_Loop
    pop     pRound+1
    pop     pRound

    #undef  RPpConst
    #undef  RPindex
    #undef  RPpBitrot
    #undef  RPpByteRot


    ; Chi Iota prepare Theta
    #define CIPTa0          rTemp
    #define CIPTa1          rTemp+1
    #define CIPTa2          rTemp+2
    #define CIPTa3          rTemp+3
    #define CIPTa4          rTemp+4
    #define CIPTc0          rTempBis
    #define CIPTc1          rTempBis+1
    #define CIPTc2          rTempBis+2
    #define CIPTc3          rTempBis+3
    #define CIPTc4          rTempBis+4
    #define CIPTz           rTempBis+6
    #define CIPTy           rTempBis+7

    in      rX, sp          ; 5 * C
    in      rX+1, sp+1
    movw    rY, rpState
    movw    rZ, pRound

    ldi     CIPTz, 4
KeccakChiIotaPrepareTheta_zLoop:
    mov     CIPTc0, zero
    mov     CIPTc1, zero
    movw    CIPTc2, CIPTc0
    mov     CIPTc4, zero

    ldi     CIPTy, 5
KeccakChiIotaPrepareTheta_yLoop:
    ld      CIPTa0, Y
    ldd     CIPTa1, Y+4
    ldd     CIPTa2, Y+8
    ldd     CIPTa3, Y+12
    ldd     CIPTa4, Y+16

    ;*p = t = a0 ^ ((~a1) & a2); c0 ^= t;
    mov     r0, CIPTa1
    com     r0
    and     r0, CIPTa2
    eor     r0, CIPTa0
    eor     CIPTc0, r0
    st      Y, r0

    ;*(p+4) = t = a1 ^ ((~a2) & a3); c1 ^= t;
    mov     r0, CIPTa2
    com     r0
    and     r0, CIPTa3
    eor     r0, CIPTa1
    eor     CIPTc1, r0
    std     Y+4, r0

    ;*(p+8) = a2 ^= ((~a3) & a4); c2 ^= a2;
    mov     r0, CIPTa3
    com     r0
    and     r0, CIPTa4
    eor     r0, CIPTa2
    eor     CIPTc2, r0
    std     Y+8, r0

    ;*(p+12) = a3 ^= ((~a4) & a0); c3 ^= a3;
    mov     r0, CIPTa4
    com     r0
    and     r0, CIPTa0
    eor     r0, CIPTa3
    eor     CIPTc3, r0
    std     Y+12, r0

    ;*(p+16) = a4 ^= ((~a0) & a1); c4 ^= a4;
    com     CIPTa0
    and     CIPTa0, CIPTa1
    eor     CIPTa0, CIPTa4
    eor     CIPTc4, CIPTa0
    std     Y+16, CIPTa0

    adiw    rY, 20
    dec     CIPTy
    brne    KeccakChiIotaPrepareTheta_yLoop

    subi    rY, 100
    sbc     rY+1, zero

    lpm     r0, Z+            ;Round Constant
    ld      CIPTa0, Y
    eor     CIPTa0, r0
    st      Y+, CIPTa0

    movw    pRound, rZ
    movw    rZ, rX
    eor     CIPTc0, r0
    st      Z+, CIPTc0
    std     Z+3, CIPTc1
    std     Z+7, CIPTc2
    std     Z+11, CIPTc3
    std     Z+15, CIPTc4
    movw    rX, rZ
    movw    rZ, pRound

    dec     CIPTz
    brne    KeccakChiIotaPrepareTheta_zLoop

    #undef  CIPTa0
    #undef  CIPTa1
    #undef  CIPTa2
    #undef  CIPTa3
    #undef  CIPTa4
    #undef  CIPTc0
    #undef  CIPTc1
    #undef  CIPTc2
    #undef  CIPTc3
    #undef  CIPTc4
    #undef  CIPTz
    #undef  CIPTy

    ;Check for terminator
    lpm     r0, Z
    inc     r0
    breq    Keccak_Done
    rjmp    Keccak_RoundLoop
Keccak_Done:

    ; Free C(on stack) and registers
    in      rX, sp          ; free 5 C lanes
    in      rX+1, sp+1
    adiw    rX, 5*4
    in      r0, 0x3F
    cli
    out     sp+1, rX+1
    out     sp, rX
    out     0x3F, r0

    pop     r29
    pop     r28
    pop     r17
    pop     r16
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     r7
    pop     r6
    pop     r5
    pop     r4
    pop     r3
    pop     r2
    ret

bit_rot_jmp_table:
    rjmp    KeccakRhoPi_RhoBitRotateDone
    rjmp    rotate32_1bit_left
    rjmp    rotate32_2bit_left
    rjmp    rotate32_3bit_left
    rjmp    rotate32_4bit_left
    rjmp    rotate32_3bit_right
    rjmp    rotate32_2bit_right
    rjmp    rotate32_1bit_right

rotate32_4bit_left:
    lsl     rTemp
    rol     rTemp+1
    rol     rTemp+2
    rol     rTemp+3
    adc     rTemp, r1
rotate32_3bit_left:
    lsl     rTemp
    rol     rTemp+1
    rol     rTemp+2
    rol     rTemp+3
    adc     rTemp, r1
rotate32_2bit_left:
    lsl     rTemp
    rol     rTemp+1
    rol     rTemp+2
    rol     rTemp+3
    adc     rTemp, r1
rotate32_1bit_left:
    lsl     rTemp
    rol     rTemp+1
    rol     rTemp+2
    rol     rTemp+3
    adc     rTemp, r1
    rjmp    KeccakRhoPi_RhoBitRotateDone

rotate32_3bit_right:
    bst     rTemp, 0
    ror     rTemp+3
    ror     rTemp+2
    ror     rTemp+1
    ror     rTemp
    bld     rTemp+3, 7
rotate32_2bit_right:
    bst     rTemp, 0
    ror     rTemp+3
    ror     rTemp+2
    ror     rTemp+1
    ror     rTemp
    bld     rTemp+3, 7
rotate32_1bit_right:
    bst     rTemp, 0
    ror     rTemp+3
    ror     rTemp+2
    ror     rTemp+1
    ror     rTemp
    bld     rTemp+3, 7
    rjmp    KeccakRhoPi_RhoBitRotateDone


; Each byte rotate routine must be 5 instructions long.

rotate32_0byte_left:
    ld      rTempBis+0, Y+
    ld      rTempBis+1, Y+
    ld      rTempBis+2, Y+
    ld      rTempBis+3, Y+
    rjmp    KeccakRhoPi_PiStore

rotate32_1byte_left:
    ld      rTempBis+1, Y+
    ld      rTempBis+2, Y+
    ld      rTempBis+3, Y+
    ld      rTempBis+0, Y+
    rjmp    KeccakRhoPi_PiStore

rotate32_2byte_left:
    ld      rTempBis+2, Y+
    ld      rTempBis+3, Y+
    ld      rTempBis+0, Y+
    ld      rTempBis+1, Y+
    rjmp    KeccakRhoPi_PiStore

rotate32_3byte_left:
    ld      rTempBis+3, Y+
    ld      rTempBis+0, Y+
    ld      rTempBis+1, Y+
    ld      rTempBis+2, Y+
    rjmp    KeccakRhoPi_PiStore

    #undef  rTemp
    #undef  rTempBis
    #undef  rTempTer
    #undef  pRound

    #undef  rpState
    #undef  zero
    #undef  rX
    #undef  rY
    #undef  rZ
    #undef  sp
