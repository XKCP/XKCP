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
; void KeccakP400_StaticInitialize( void )
;
.global KeccakP400_StaticInitialize

;----------------------------------------------------------------------------
;
; void KeccakP400_Initialize(void *state)
;
; argument state   is passed in r24:r25
;
.global KeccakP400_Initialize
KeccakP400_Initialize:
    movw    rZ, r24
    ldi     r23, 2*5        ; clear state (5 bytes/2.5 lanes per iteration)
KeccakP400_Initialize_Loop:
    st      z+, zero
    st      z+, zero
    st      z+, zero
    st      z+, zero
    st      z+, zero
    dec     r23
    brne    KeccakP400_Initialize_Loop
KeccakP400_StaticInitialize:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP400_AddByte(void *state, unsigned char data, unsigned int offset)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23, only LSB (r22) is used
; argument offset    is passed in r20:r21, only LSB (r20) is used
;
.global KeccakP400_AddByte
KeccakP400_AddByte:
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    ld      r0, Z
    eor     r0, r22
    st      Z, r0
    ret

;----------------------------------------------------------------------------
;
; void KeccakP400_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakP400_AddBytes
KeccakP400_AddBytes:
    tst      r18
    breq    KeccakP400_AddBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    movw    rX, r22
KeccakP400_AddBytes_Loop:
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    dec     r18
    brne    KeccakP400_AddBytes_Loop
KeccakP400_AddBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP400_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakP400_OverwriteBytes
KeccakP400_OverwriteBytes:
    tst      r18
    breq    KeccakP400_OverwriteBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    movw    rX, r22
KeccakP400_OverwriteBytes_Loop:
    ld      r0, X+
    st      Z+, r0
    dec     r18
    brne    KeccakP400_OverwriteBytes_Loop
KeccakP400_OverwriteBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP400_OverwriteWithZeroes(void *state, unsigned int byteCount)
;
; argument state        is passed in r24:r25
; argument byteCount    is passed in r22:r23, only LSB (r22) is used
;
.global KeccakP400_OverwriteWithZeroes
KeccakP400_OverwriteWithZeroes:
    movw    rZ, r24         ; rZ = state
    mov     r23, r22
    lsr     r23
    lsr     r23
    breq    KeccakP400_OverwriteWithZeroes_Bytes
KeccakP400_OverwriteWithZeroes_Loop2Lanes:
    st      Z+, r1
    st      Z+, r1
    st      Z+, r1
    st      Z+, r1
    dec     r23
    brne    KeccakP400_OverwriteWithZeroes_Loop2Lanes
KeccakP400_OverwriteWithZeroes_Bytes:
    andi    r22, 3
    breq    KeccakP400_OverwriteWithZeroes_End
KeccakP400_OverwriteWithZeroes_LoopBytes:
    st      Z+, r1
    dec     r22
    brne    KeccakP400_OverwriteWithZeroes_LoopBytes
KeccakP400_OverwriteWithZeroes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP400_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakP400_ExtractBytes
KeccakP400_ExtractBytes:
    tst      r18
    breq    KeccakP400_ExtractBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, zero
    movw    rX, r22
KeccakP400_ExtractBytes_Loop:
    ld      r0, Z+
    st      X+, r0
    dec     r18
    brne    KeccakP400_ExtractBytes_Loop
KeccakP400_ExtractBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakP400_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument input     is passed in r22:r23
; argument output    is passed in r20:r21
; argument offset    is passed in r18:r19, only LSB (r18) is used
; argument length    is passed in r16:r17, only LSB (r16) is used
;
.global KeccakP400_ExtractAndAddBytes
KeccakP400_ExtractAndAddBytes:
    tst     r16
    breq    KeccakP400_ExtractAndAddBytes_End
    push    r16
    push    r28
    push    r29
    movw    rZ, r24
    add     rZ, r18
    adc     rZ+1, zero
    movw    rX, r22
    movw    rY, r20
    subi    r16, 2
    brcs    KeccakP400_ExtractAndAddBytes_Byte
KeccakP400_ExtractAndAddBytes_LoopLane:
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    subi    r16, 2
    brcc    KeccakP400_ExtractAndAddBytes_LoopLane
KeccakP400_ExtractAndAddBytes_Byte:
    ldi     r19, 2
    add     r16, r19
    breq    KeccakP400_ExtractAndAddBytes_Done
KeccakP400_ExtractAndAddBytes_Loop1:
    ld      r21, Z+
    ld      r0, X+
    eor     r0, r21
    st      Y+, r0
    dec     r16
    brne    KeccakP400_ExtractAndAddBytes_Loop1
KeccakP400_ExtractAndAddBytes_Done:
    pop     r29
    pop     r28
    pop     r16
KeccakP400_ExtractAndAddBytes_End:
    ret


#define ROT_BIT(a)     ((a) & 7)
#define ROT_BYTE(a)     (((a)/8 + !!(((a)%8) > 4)) & 1)

KeccakP400_RhoPiConstants:
    .BYTE    ROT_BIT( 1), ROT_BYTE( 3), 10 * 2
    .BYTE    ROT_BIT( 3), ROT_BYTE( 6),  7 * 2
    .BYTE    ROT_BIT( 6), ROT_BYTE(10), 11 * 2
    .BYTE    ROT_BIT(10), ROT_BYTE(15), 17 * 2
    .BYTE    ROT_BIT(15), ROT_BYTE(21), 18 * 2
    .BYTE    ROT_BIT(21), ROT_BYTE(28),  3 * 2
    .BYTE    ROT_BIT(28), ROT_BYTE(36),  5 * 2
    .BYTE    ROT_BIT(36), ROT_BYTE(45), 16 * 2
    .BYTE    ROT_BIT(45), ROT_BYTE(55),  8 * 2
    .BYTE    ROT_BIT(55), ROT_BYTE( 2), 21 * 2
    .BYTE    ROT_BIT( 2), ROT_BYTE(14), 24 * 2
    .BYTE    ROT_BIT(14), ROT_BYTE(27),  4 * 2
    .BYTE    ROT_BIT(27), ROT_BYTE(41), 15 * 2
    .BYTE    ROT_BIT(41), ROT_BYTE(56), 23 * 2
    .BYTE    ROT_BIT(56), ROT_BYTE( 8), 19 * 2
    .BYTE    ROT_BIT( 8), ROT_BYTE(25), 13 * 2
    .BYTE    ROT_BIT(25), ROT_BYTE(43), 12 * 2
    .BYTE    ROT_BIT(43), ROT_BYTE(62),  2 * 2
    .BYTE    ROT_BIT(62), ROT_BYTE(18), 20 * 2
    .BYTE    ROT_BIT(18), ROT_BYTE(39), 14 * 2
    .BYTE    ROT_BIT(39), ROT_BYTE(61), 22 * 2
    .BYTE    ROT_BIT(61), ROT_BYTE(20),  9 * 2
    .BYTE    ROT_BIT(20), ROT_BYTE(44),  6 * 2
    .BYTE    ROT_BIT(44), ROT_BYTE( 1),  1 * 2

KeccakP400_RoundConstants_20:
    .BYTE    0x01, 0x00
    .BYTE    0x82, 0x80
    .BYTE    0x8a, 0x80
    .BYTE    0x00, 0x80
    .BYTE    0x8b, 0x80
    .BYTE    0x01, 0x00
    .BYTE    0x81, 0x80
    .BYTE    0x09, 0x80
    .BYTE    0x8a, 0x00
    .BYTE    0x88, 0x00
    .BYTE    0x09, 0x80
    .BYTE    0x0a, 0x00
    .BYTE    0x8b, 0x80
    .BYTE    0x8b, 0x00
    .BYTE    0x89, 0x80
    .BYTE    0x03, 0x80
    .BYTE    0x02, 0x80
    .BYTE    0x80, 0x00
    .BYTE    0x0a, 0x80
    .BYTE    0x0a, 0x00
KeccakP400_RoundConstants_N:
    .BYTE      0xFF, 0      ; terminator

    .text

#define pRound          22  //; 2 regs (22-23)
#define pRound1         23

;----------------------------------------------------------------------------
;
; void KeccakP400_Permute_20rounds( void *state )
;
.global KeccakP400_Permute_20rounds
KeccakP400_Permute_20rounds:
    ldi     pRound,   lo8(KeccakP400_RoundConstants_20)
    ldi     pRound+1, hi8(KeccakP400_RoundConstants_20)
    rjmp    KeccakP400_Permute

;----------------------------------------------------------------------------
;
; void KeccakP400_Permute_Nrounds( void *state, unsigned int nr )
;
; argument state   is passed in r24:r25
; argument nr      is passed in r22:r23
;
.global KeccakP400_Permute_Nrounds
KeccakP400_Permute_Nrounds:
    mov     r0, r22
    lsl     r0
    ldi     pRound,   lo8(KeccakP400_RoundConstants_N)
    ldi     pRound+1, hi8(KeccakP400_RoundConstants_N)
    sub     pRound,   r0
    sbc     pRound+1, r1
KeccakP400_Permute:
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

    ; Allocate C variables (5*2)
    in      rZ,   sp
    in      rZ+1, sp+1
    sbiw     rZ, 5*2
    in      r0, 0x3F
    cli
    out     sp+1, rZ+1
    out     sp, rZ          ; Z points to 5 C lanes
    out     0x3F, r0

    ; Variables used in multiple operations
    #define rTemp        2      //; 8 regs (2-9)
    #define rTemp0       2
    #define rTemp1       3
    #define rTemp2       4
    #define rTemp3       5
    #define rTemp4       6
    #define rTemp5       7
    #define rTemp6       8
    #define rTemp7       9
    #define rTempBis    10      //; 8 regs (10-17)
    #define rTempBis0   10
    #define rTempBis1   11
    #define rTempBis2   12
    #define rTempBis3   13
    #define rTempBis4   14
    #define rTempBis5   15
    #define rTempBis6   16
    #define rTempBis7   17
    #define rTempTer    18      //; 4 regs (18-21)
    #define rTempTer0   18
    #define rTempTer1   19

    ; Initial Prepare Theta
    #define TCIPx       rTempTer

    clr     zero
    movw    rY, rpState
    ldi     TCIPx, 5*2
KeccakP_InitialPrepTheta_Loop:
    ld      r0, Y
    adiw    rY, 10
    ld      rTemp, Y
    adiw    rY, 10
    eor     r0, rTemp
    ld      rTemp, Y
    adiw    rY, 10
    eor     r0, rTemp
    ld      rTemp, Y
    eor     r0, rTemp
    ldd     rTemp, Y+10
    eor     r0, rTemp
    st      Z+, r0
    sbiw    rY, 29
    dec     TCIPx
    brne    KeccakP_InitialPrepTheta_Loop
    #undef  TCIPx

KeccakP_RoundLoop:

    ; Theta
    #define TCplus          rX
    #define TCplus1         rX1
    #define TCminus         rZ
    #define TCminus1        rZ1
    #define TCcoordX        rTempTer
    #define TCcoordY        rTempTer1

    in      TCminus,   sp
    in      TCminus+1, sp+1
    movw    TCplus,  TCminus
    adiw    TCminus, 4*2
    adiw    TCplus,  1*2
    movw    rY, rpState
    ldi     TCcoordX, 0x16
KeccakTheta_Loop1:
    ld      rTemp0, X+
    ld      rTemp1, X+

    lsl     rTemp0
    rol     rTemp1
    adc     rTemp0, zero

    ld      r0, Z+
    eor     rTemp0, r0
    ld      r0, Z+
    eor     rTemp1, r0

    ldi     TCcoordY, 5
KeccakTheta_Loop2:
    ld      r0, Y
    eor     r0, rTemp0
    st      Y+, r0
    ld      r0, Y
    eor     r0, rTemp1
    st      Y+, r0
    adiw    rY, 8

    dec     TCcoordY
    brne    KeccakTheta_Loop2

    sbiw    rY, 50-2

    lsr     TCcoordX
    brcc    local1
    brne    KeccakTheta_Loop1
    rjmp    KeccakTheta_End
local1:
    cpi     TCcoordX, 0x0B
    brne    local2
    sbiw    TCminus, 10
    rjmp    KeccakTheta_Loop1
local2:
    sbiw    TCplus, 10
    rjmp    KeccakTheta_Loop1

KeccakTheta_End:
    #undef  TCplus
    #undef  TCminus
    #undef  TCcoordX
    #undef  TCcoordY

    ; Rho Pi
    #define RPpConst    rTempTer        //; 2 regs
    #define RPindex     rTempTer+2
    #define RPByteRot   rTempTer+3
    #define RPpBitRot   rX

    sbiw    rY, 8
    ld      rTemp0, Y+
    ld      rTemp1, Y+
    ldi     RPpConst,   lo8(KeccakP400_RhoPiConstants)
    ldi     RPpConst+1, hi8(KeccakP400_RhoPiConstants)
    ldi     RPpBitRot,   pm_lo8(bit_rot_jmp_table)
    ldi     RPpBitRot+1, pm_hi8(bit_rot_jmp_table)

KeccakRhoPi_Loop:
    ; get rotation codes
    movw    rZ, RPpConst
    lpm     r0, Z+              ; get number of bits to rotate
    lpm     RPByteRot, Z+       ; get number of bytes to rotate
    lpm     RPindex, Z+         ; get index in state
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

    tst      RPByteRot
    brne    KeccakRhoPi_LoadSwapped
    ld      rTempBis0, Y
    ldd     rTempBis1, Y+1
    st      Y+, rTemp0
    st      Y+, rTemp1
    movw    rTemp0, rTempBis0
    subi    RPindex, 2
    brne    KeccakRhoPi_Loop
    rjmp    KeccakRhoPi_Done
KeccakRhoPi_LoadSwapped:
    ld      rTempBis1, Y
    ldd     rTempBis0, Y+1
    st      Y+, rTemp0
    st      Y+, rTemp1
    movw    rTemp0, rTempBis0
    subi    RPindex, 2
    brne    KeccakRhoPi_Loop
KeccakRhoPi_Done:

    #undef  RPindex
    #undef  RPTemp

    ; Chi, Iota, prepare Theta
    #define CIPTa0          rTemp
    #define CIPTa1          rTemp1
    #define CIPTa2          rTemp2
    #define CIPTa3          rTemp3
    #define CIPTa4          rTemp4
    #define CIPTc0          rTempBis
    #define CIPTc1          rTempBis1
    #define CIPTc2          rTempBis2
    #define CIPTc3          rTempBis3
    #define CIPTc4          rTempBis4
    #define CIPTz           rTempBis6
    #define CIPTy           rTempBis7

    movw    rY, rpState
    in      rX, sp
    in      rX+1, sp+1

    movw    rZ, pRound

    ldi     CIPTz, 2
KeccakChiIotaPrepareTheta_zLoop:
    mov     CIPTc0, zero
    mov     CIPTc1, zero
    movw    CIPTc2, CIPTc0
    mov     CIPTc4, zero

    ldi     CIPTy, 5
KeccakChiIotaPrepareTheta_yLoop:
    ld      CIPTa0, Y
    ldd     CIPTa1, Y+2
    ldd     CIPTa2, Y+4
    ldd     CIPTa3, Y+6
    ldd     CIPTa4, Y+8

    ;*p = t = a0 ^ ((~a1) & a2); c0 ^= t;
    mov     r0, CIPTa1
    com     r0
    and     r0, CIPTa2
    eor     r0, CIPTa0
    eor     CIPTc0, r0
    st      Y, r0

    ;*(p+2) = t = a1 ^ ((~a2) & a3); c1 ^= t;
    mov     r0, CIPTa2
    com     r0
    and     r0, CIPTa3
    eor     r0, CIPTa1
    eor     CIPTc1, r0
    std     Y+2, r0

    ;*(p+4) = a2 ^= ((~a3) & a4); c2 ^= a2;
    mov     r0, CIPTa3
    com     r0
    and     r0, CIPTa4
    eor     r0, CIPTa2
    eor     CIPTc2, r0
    std     Y+4, r0

    ;*(p+6) = a3 ^= ((~a4) & a0); c3 ^= a3;
    mov     r0, CIPTa4
    com     r0
    and     r0, CIPTa0
    eor     r0, CIPTa3
    eor     CIPTc3, r0
    std     Y+6, r0

    ;*(p+8) = a4 ^= ((~a0) & a1); c4 ^= a4;
    com     CIPTa0
    and     CIPTa0, CIPTa1
    eor     CIPTa0, CIPTa4
    eor     CIPTc4, CIPTa0
    std     Y+8, CIPTa0

    adiw    rY, 10
    dec     CIPTy
    brne    KeccakChiIotaPrepareTheta_yLoop

    sbiw    rY, 50

    lpm     r0, Z+          ; Round Constant
    ld      CIPTa0, Y
    eor     CIPTa0, r0
    st      Y+, CIPTa0

    movw    pRound, rZ
    movw    rZ, rX
    eor     CIPTc0, r0
    st      Z+, CIPTc0
    std     Z+1, CIPTc1
    std     Z+3, CIPTc2
    std     Z+5, CIPTc3
    std     Z+7, CIPTc4
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
    breq    KeccakP_Done
    rjmp    KeccakP_RoundLoop
KeccakP_Done:

    ; Free C(on stack) and registers
    in      rX, sp          ; free 5 C lanes
    in      rX+1, sp+1
    adiw    rX, 5*2
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
    rjmp    rotate16_1bit_left
    rjmp    rotate16_2bit_left
    rjmp    rotate16_3bit_left
    rjmp    rotate16_4bit_left
    rjmp    rotate16_3bit_right
    rjmp    rotate16_2bit_right
    rjmp    rotate16_1bit_right

rotate16_4bit_left:
    lsl     rTemp
    rol     rTemp+1
    adc     rTemp, r1
rotate16_3bit_left:
    lsl     rTemp
    rol     rTemp+1
    adc     rTemp, r1
rotate16_2bit_left:
    lsl     rTemp
    rol     rTemp+1
    adc     rTemp, r1
rotate16_1bit_left:
    lsl     rTemp
    rol     rTemp+1
    adc     rTemp, r1
    rjmp    KeccakRhoPi_RhoBitRotateDone

rotate16_3bit_right:
    bst     rTemp, 0
    ror     rTemp+1
    ror     rTemp
    bld     rTemp+1, 7
rotate16_2bit_right:
    bst     rTemp, 0
    ror     rTemp+1
    ror     rTemp
    bld     rTemp+1, 7
rotate16_1bit_right:
    bst     rTemp, 0
    ror     rTemp+1
    ror     rTemp
    bld     rTemp+1, 7
    rjmp    KeccakRhoPi_RhoBitRotateDone

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
