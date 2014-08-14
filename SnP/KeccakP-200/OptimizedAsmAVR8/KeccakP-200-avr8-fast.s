;
; Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
; Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
; denoted as "the implementer".
;
; For more information, feedback or questions, please refer to our websites:
; http://keccak.noekeon.org/
; http://keyak.noekeon.org/
; http://ketje.noekeon.org/
;
; To the extent possible under law, the implementer has waived all copyright
; and related or neighboring rights to the source code in this file.
; http://creativecommons.org/publicdomain/zero/1.0/
;

; INFO: Tested on ATmega1280 simulator

; Registers used in all routines
#define rpState 24
#define rX      26
#define rX1     r27
#define rY      28
#define rY1     r29
#define rZ      30
#define rZ1     r31

;----------------------------------------------------------------------------
;
; void KeccakF200_Initialize( void )
;
.global KeccakF200_Initialize

;----------------------------------------------------------------------------
;
; void KeccakF200_StateInitialize(void *state)
;
; argument state   is passed in r24:r25
;
.global KeccakF200_StateInitialize
KeccakF200_StateInitialize:
    movw    rZ, r24
    ldi     r23, 5          ; clear state (5 bytes/5 lanes per iteration)
KeccakF200_StateInitialize_Loop:
    st      z+, r1
    st      z+, r1
    st      z+, r1
    st      z+, r1
    st      z+, r1
    dec     r23
    brne    KeccakF200_StateInitialize_Loop
KeccakF200_Initialize:
    ret

;----------------------------------------------------------------------------
;
; void KeccakF200_StateComplementBit(void *state, unsigned int position)
;
; argument state    is passed in r24:r25
; argument position is passed in r22:r23, only LSB (r22) is used
;
.global KeccakF200_StateComplementBit
KeccakF200_StateComplementBit:
    movw    rZ, r24
    ldi     r21, 1
    mov     r20, r22        ; bitnumber = position & 7
    andi    r20, 7
    breq    KeccakF200_StateComplementBit_LoopEnd
KeccakF200_StateComplementBit_Loop:
    lsl     r21
    dec     r20
    brne    KeccakF200_StateComplementBit_Loop
KeccakF200_StateComplementBit_LoopEnd:
    lsr     r22             ; bytenumber = position >> 3
    lsr     r22
    lsr     r22
    add     rZ, r22
    adc     rZ+1, r1
    ld      r20, Z
    eor     r20, r21
    st      Z, r20
    ret

;----------------------------------------------------------------------------
;
; void KeccakF200_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakF200_StateXORBytes
KeccakF200_StateXORBytes:
    tst      r18
    breq    KeccakF200_StateXORBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, r1
    movw    rX, r22
KeccakF200_StateXORBytes_Loop:
    ld      r21, X+
    ld      r0, Z
    eor     r0, r21
    st      Z+, r0
    dec     r18
    brne    KeccakF200_StateXORBytes_Loop
KeccakF200_StateXORBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakF200_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakF200_StateOverwriteBytes
KeccakF200_StateOverwriteBytes:
    tst      r18
    breq    KeccakF200_StateOverwriteBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, r1
    movw    rX, r22
KeccakF200_StateOverwriteBytes_Loop:
    ld      r0, X+
    st      Z+, r0
    dec     r18
    brne    KeccakF200_StateOverwriteBytes_Loop
KeccakF200_StateOverwriteBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakF200_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
;
; argument state        is passed in r24:r25
; argument byteCount    is passed in r22:r23, only LSB (r22) is used
;
.global KeccakF200_StateOverwriteWithZeroes
KeccakF200_StateOverwriteWithZeroes:
    movw    rZ, r24         ; rZ = state
    mov     r23, r22
    lsr     r23
    lsr     r23
    breq    KeccakF200_StateOverwriteWithZeroes_Bytes
KeccakF200_StateOverwriteWithZeroes_Loop4Lanes:
    st      Z+, r1
    st      Z+, r1
    st      Z+, r1
    st      Z+, r1
    dec     r23
    brne    KeccakF200_StateOverwriteWithZeroes_Loop4Lanes
KeccakF200_StateOverwriteWithZeroes_Bytes:
    andi    r22, 3
    breq    KeccakF200_StateOverwriteWithZeroes_End
KeccakF200_StateOverwriteWithZeroes_LoopBytes:
    st      Z+, r1
    dec     r22
    brne    KeccakF200_StateOverwriteWithZeroes_LoopBytes
KeccakF200_StateOverwriteWithZeroes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakF200_StateExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakF200_StateExtractBytes
KeccakF200_StateExtractBytes:
    tst      r18
    breq    KeccakF200_StateExtractBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, r1
    movw    rX, r22
KeccakF200_StateExtractBytes_Loop:
    ld      r0, Z+
    st      X+, r0
    dec     r18
    brne    KeccakF200_StateExtractBytes_Loop
KeccakF200_StateExtractBytes_End:
    ret

;----------------------------------------------------------------------------
;
; void KeccakF200_StateExtractAndXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
;
; argument state     is passed in r24:r25
; argument data      is passed in r22:r23
; argument offset    is passed in r20:r21, only LSB (r20) is used
; argument length    is passed in r18:r19, only LSB (r18) is used
;
.global KeccakF200_StateExtractAndXORBytes
KeccakF200_StateExtractAndXORBytes:
    tst      r18
    breq    KeccakF200_StateExtractAndXORBytes_End
    movw    rZ, r24
    add     rZ, r20
    adc     rZ+1, r1
    movw    rX, r22
KeccakF200_StateExtractAndXORBytes_Loop:
    ld      r0, X
    ld      r21, Z+
    eor     r0, r21
    st      X+, r0
    dec     r18
    brne    KeccakF200_StateExtractAndXORBytes_Loop
KeccakF200_StateExtractAndXORBytes_End:
    ret


#define _ba      0
#define _be      1
#define _bi      2
#define _bo      3
#define _bu      4
#define _ga      5
#define _ge      6
#define _gi      7
#define _go      8
#define _gu      9
#define _ka     10
#define _ke     11
#define _ki     12
#define _ko     13
#define _ku     14
#define _ma     15
#define _me     16
#define _mi     17
#define _mo     18
#define _mu     19
#define _sa     20
#define _se     21
#define _si     22
#define _so     23
#define _su     24

#define zero    r25
#define c0      r26
#define c1      r27
#define c2      r28
#define c3      r29
#define c4      r30
#define a0      r31

.MACRO RhoPi rot, val, prev
    mov     \val, \prev
.IF         \rot==1
    lsl     \val
    adc     \val, zero
.ENDIF
.IF         \rot==2
    lsl     \val
    adc     \val, zero
    lsl     \val
    adc     \val, zero
.ENDIF
.IF         \rot==3
    swap    \val
    bst      \val, 0
    lsr     \val
    bld      \val, 7
.ENDIF
.IF         \rot==4
    swap    \val
.ENDIF
.IF         \rot==5
    lsl     \val
    adc     \val, zero
    swap    \val
.ENDIF
.IF         \rot==6
    lsl     \val
    adc     \val, zero
    lsl     \val
    adc     \val, zero
    swap    \val
.ENDIF
.IF         \rot==7
    bst     \val, 0
    lsr     \val
    bld     \val, 7
.ENDIF
.ENDM

.MACRO      Chi a0, cc, a2
    com     \cc
    and     \cc, \a2
    eor     \cc, \a0
.ENDM

.MACRO      ChiS a0, a1, a2
    com     \a1
    and     \a1, \a2
    eor     \a0, \a1
.ENDM

KeccakF200_RoundConstants:
    .BYTE   0x01
    .BYTE   0x82
    .BYTE   0x8a
    .BYTE   0x00
    .BYTE   0x8b
    .BYTE   0x01
    .BYTE   0x81
    .BYTE   0x09
    .BYTE   0x8a
    .BYTE   0x88
    .BYTE   0x09
    .BYTE   0x0a
    .BYTE   0x8b
    .BYTE   0x8b
    .BYTE   0x89
    .BYTE   0x03
    .BYTE   0x02
    .BYTE   0x80
KeccakP200_RoundConstants:

    .text


;----------------------------------------------------------------------------
;
; void KeccakP200_StatePermute( void *state, unsigned int nr )
;
; argument state   is passed in r24:r25
; argument nr      is passed in r22:r23
;
.global KeccakP200_StatePermute
KeccakP200_StatePermute:
    mov     r0, r22
    ldi     r22, lo8(KeccakP200_RoundConstants)
    ldi     r23, hi8(KeccakP200_RoundConstants)
    sub     r22, r0
    sbc     r23, r1
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

    ; load state in registers
    movw    rY, rpState
    push    rY
    push    rY1

    movw    rZ, r22         ; RoundConstants

    ld      _ba, Y+
    ld      _be, Y+
    ld      _bi, Y+
    ld      _bo, Y+
    ld      _bu, Y+
    ld      _ga, Y+
    ld      _ge, Y+
    ld      _gi, Y+
    ld      _go, Y+
    ld      _gu, Y+
    ld      _ka, Y+
    ld      _ke, Y+
    ld      _ki, Y+
    ld      _ko, Y+
    ld      _ku, Y+
    ld      _ma, Y+
    ld      _me, Y+
    ld      _mi, Y+
    ld      _mo, Y+
    ld      _mu, Y+
    ld      _sa, Y+
    ld      _se, Y+
    ld      _si, Y+
    ld      _so, Y+
    ld      _su, Y

    ldi     zero, 0
KeccakP200_StatePermute_RoundLoop:
    push    rZ
    push    rZ1

    ; Prepare Theta
    movw    c0, _ba
    movw    c2, _bi
    mov     c4, _bu

    eor     c0, _ga
    eor     c0, _ka
    eor     c0, _ma
    eor     c0, _sa

    eor     c1, _ge
    eor     c1, _ke
    eor     c1, _me
    eor     c1, _se

    eor     c2, _gi
    eor     c2, _ki
    eor     c2, _mi
    eor     c2, _si

    eor     c3, _go
    eor     c3, _ko
    eor     c3, _mo
    eor     c3, _so

    eor     c4, _gu
    eor     c4, _ku
    eor     c4, _mu
    eor     c4, _su

    ; Theta
    mov     a0, c1
    lsl     a0
    adc     a0, zero
    eor     a0, c4
    eor     _ba, a0
    eor     _ga, a0
    eor     _ka, a0
    eor     _ma, a0
    eor     _sa, a0

    mov     a0, c2
    lsl     a0
    adc     a0, zero
    eor     a0, c0
    eor     _be, a0
    eor     _ge, a0
    eor     _ke, a0
    eor     _me, a0
    eor     _se, a0

    mov     a0, c3
    lsl     a0
    adc     a0, zero
    eor     a0, c1
    eor     _bi, a0
    eor     _gi, a0
    eor     _ki, a0
    eor     _mi, a0
    eor     _si, a0

    lsl     c4
    adc     c4, zero
    eor     c4, c2
    eor     _bo, c4
    eor     _go, c4
    eor     _ko, c4
    eor     _mo, c4
    eor     _so, c4

    lsl     c0
    adc     c0, zero
    eor     c0, c3
    eor     _bu, c0
    eor     _gu, c0
    eor     _ku, c0
    eor     _mu, c0
    eor     _su, c0

    ; Rho Pi
    mov     c0, _be
    RhoPi   4, _be, _ge     ;  1 <  6
    RhoPi   4, _ge, _gu     ;  6 <  9
    RhoPi   5, _gu, _si     ;  9 < 22
    RhoPi   7, _si, _ku     ; 22 < 14
    RhoPi   2, _ku, _sa     ; 14 < 20
    RhoPi   6, _sa, _bi     ; 20 <  2
    RhoPi   3, _bi, _ki     ;  2 < 12
    RhoPi   1, _ki, _ko     ; 12 < 13
    RhoPi   0, _ko, _mu     ; 13 < 19
    RhoPi   0, _mu, _so     ; 19 < 23
    RhoPi   1, _so, _ma     ; 23 < 15
    RhoPi   3, _ma, _bu     ; 15 <  4
    RhoPi   6, _bu, _su     ;  4 < 24
    RhoPi   2, _su, _se     ; 24 < 21
    RhoPi   7, _se, _go     ; 21 <  8
    RhoPi   5, _go, _me     ;  8 < 16
    RhoPi   4, _me, _ga     ; 16 <  5
    RhoPi   4, _ga, _bo     ;  5 <  3
    RhoPi   5, _bo, _mo     ;  3 < 18
    RhoPi   7, _mo, _mi     ; 18 < 17
    RhoPi   2, _mi, _ke     ; 17 < 11
    RhoPi   6, _ke, _gi     ; 11 <  7
    RhoPi   3, _gi, _ka     ;  7 < 10
    RhoPi   1, _ka, c0      ; 10 <  1

    ; Chi
    mov     c0, _be
    mov     c1, _bi
    mov     c2, _bu
    Chi     _ba,  c0, _bi
    Chi     _be,  c1, _bo
    Chi     _bo,  c2, _ba
    ChiS    _bi, _bo, _bu
    ChiS    _bu, _ba, _be
    movw    _ba, c0
    mov     _bo, c2

    movw    c0, _ge
    mov     c2, _gu
    Chi     _ga,  c0, _gi
    Chi     _ge,  c1, _go
    Chi     _go,  c2, _ga
    ChiS    _gi, _go, _gu
    ChiS    _gu, _ga, _ge
    mov     _ga, c0
    mov     _ge, c1
    mov     _go, c2

    mov     c0, _ke
    mov     c1, _ki
    mov     c2, _ku
    Chi     _ka,  c0, _ki
    Chi     _ke,  c1, _ko
    Chi     _ko,  c2, _ka
    ChiS    _ki, _ko, _ku
    ChiS    _ku, _ka, _ke
    movw    _ka, c0
    mov     _ko, c2

    movw    c0, _me
    mov     c2, _mu
    Chi     _ma,  c0, _mi
    Chi     _me,  c1, _mo
    Chi     _mo,  c2, _ma
    ChiS    _mi, _mo, _mu
    ChiS    _mu, _ma, _me
    mov     _ma, c0
    mov     _me, c1
    mov     _mo, c2

    mov     c0, _se
    mov     c1, _si
    mov     c2, _su
    Chi     _sa,  c0, _si
    Chi     _se,  c1, _so
    Chi     _so,  c2, _sa
    ChiS    _si, _so, _su
    ChiS    _su, _sa, _se
    movw    _sa, c0
    mov     _so, c2

    ; Iota
    pop     rZ1
    pop     rZ
    lpm     c0, Z+          ; Round Constant
    eor     _ba, c0

    ; Check for last round constant
    cpi     c0, 0x80
    breq    KeccakP200_StatePermute_Done
    rjmp    KeccakP200_StatePermute_RoundLoop
KeccakP200_StatePermute_Done:

    ; store registers to RAM state
    pop     rY1
    pop     rY
    st      Y+, _ba
    st      Y+, _be
    st      Y+, _bi
    st      Y+, _bo
    st      Y+, _bu
    st      Y+, _ga
    st      Y+, _ge
    st      Y+, _gi
    st      Y+, _go
    st      Y+, _gu
    st      Y+, _ka
    st      Y+, _ke
    st      Y+, _ki
    st      Y+, _ko
    st      Y+, _ku
    st      Y+, _ma
    st      Y+, _me
    st      Y+, _mi
    st      Y+, _mo
    st      Y+, _mu
    st      Y+, _sa
    st      Y+, _se
    st      Y+, _si
    st      Y+, _so
    st      Y , _su

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
    eor     r1, r1
    ret

    #undef  rpState
    #undef  zero    
    #undef  rX
    #undef  rY
    #undef  rZ
