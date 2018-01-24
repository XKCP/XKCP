// Implementation by Andre Moraes
//
// This file implements Keccak-p[800] in a SnP-compatible way.
// Please refer to SnP-documentation.h for more details.
//
// This implementation comes with KeccakP-800-SnP.h in the same folder.
// Please refer to LowLevel.build for the exact list of other files it must be combined with.

// INFO: Tested on Cortex-A53(odroid-c2), using gcc.
// WARNING: These functions work only on little endian CPU with ARMv8A architecture
// WARNING: Don't use V8-V15 or X19-X28 since we aren't saving them

// Register-Lane Lookup

// v22 = A[0]   || A[1]    ||  A[2]  || A[3]
// v23 = A[4]

// v24 = A[5]   || A[6]    ||  A[7]  || A[8]
// v25 = A[9]

// v26 = A[10]  || A[11]   ||  A[12] || A[13]
// v27 = A[14]

// v28 = A[15]  || A[16]   ||  A[17] || A[18]
// v29 = A[19]

// v30 = A[20]  || A[21]   ||  A[22] || A[23]
// v31 = A[24]

.macro    LoadState
    ld1     { v22.d }[0], [x0], #8
    ld1     { v22.d }[1], [x0], #8
    ld1     { v23.s }[0], [x0], #4
    ld1     { v24.d }[0], [x0], #8
    ld1     { v24.d }[1], [x0], #8
    ld1     { v25.s }[0], [x0], #4
    ld1     { v26.d }[0], [x0], #8
    ld1     { v26.d }[1], [x0], #8
    ld1     { v27.s }[0], [x0], #4
    ld1     { v28.d }[0], [x0], #8
    ld1     { v28.d }[1], [x0], #8
    ld1     { v29.s }[0], [x0], #4
    ld1     { v30.d }[0], [x0], #8
    ld1     { v30.d }[1], [x0], #8
    ld1     { v31.s }[0], [x0], #4
    sub     x0, x0, #100
    movi    v20.4s, #0
    .endm

.macro    StoreState
    st1     { v22.d }[0], [x0], #8
    st1     { v22.d }[1], [x0], #8
    st1     { v23.s }[0], [x0], #4
    st1     { v24.d }[0], [x0], #8
    st1     { v24.d }[1], [x0], #8
    st1     { v25.s }[0], [x0], #4
    st1     { v26.d }[0], [x0], #8
    st1     { v26.d }[1], [x0], #8
    st1     { v27.s }[0], [x0], #4
    st1     { v28.d }[0], [x0], #8
    st1     { v28.d }[1], [x0], #8
    st1     { v29.s }[0], [x0], #4
    st1     { v30.d }[0], [x0], #8
    st1     { v30.d }[1], [x0], #8
    st1     { v31.s }[0], [x0], #4
    .endm

// NEON has no BIT-wise vector rotate operation
.macro    ROTL32  dst, src, rot
    shl     \dst\().4s, \src\().4s, #\rot
    sri     \dst\().4s, \src\().4s, #32-\rot
    .endm

.macro    RhoPi dst, src, sav, rot
    ror     \src, \src, #32-\rot
    mov     \sav, \dst
    mov     \dst, \src
    .endm

.macro    Chi  src1, src2
    mov     v0.2d[0], \src1\().2d[1]
    mov     v0.s[2], \src2\().s[0]
    mov     v0.s[3], \src1\().s[0]
    ext     v1.16b, \src1\().16b, \src2\().16b, #4
    mov     v2.s[0], \src1\().s[1]
    mov     v3.s[0], \src1\().s[0]
    bic     v4.16b, v0.16b, v1.16b
    bic     v5.16b, v2.16b, v3.16b
    eor     \src1\().16b, \src1\().16b, v4.16b
    eor     \src2\().16b, \src2\().16b, v5.16b
    .endm

.macro    KeccakRound
    // Theta - Build new lanes
    eor     v0.16b, v22.16b, v24.16b
    eor     v0.16b, v0.16b,  v26.16b
    eor     v0.16b, v0.16b,  v28.16b
    eor     v0.16b, v0.16b,  v30.16b     // v0 = B[0] || B[1] || B[2] || B[3]

    eor     v1.16b, v23.16b, v25.16b
    eor     v1.16b, v1.16b,  v27.16b
    eor     v1.16b, v1.16b,  v29.16b
    eor     v1.16b, v1.16b,  v31.16b     // v1 = B[4]

    ROTL32  v2, v0, 1                    // v2 = ROT32(B[0]) || ROT32(B[1]) || ROT32(B[2]) || ROT32(B[3])
    ROTL32  v3, v1, 1                    // v3 = ROT32(B[4])

    ext     v4.16b, v2.16b, v3.16b, #8   // v4 = ROT32(B[2]) || ROT32(B[3]) || ROT32(B[4]) || ????
    mov     v4.s[3], v2.s[0]             // v4 = ROT32(B[2]) || ROT32(B[3]) || ROT32(B[4]) || ROT32(B[0])

    eor     v5.16b, v0.16b, v4.16b       // v5 = ->A[1] || ->A[2] || ->A[3] || ->A[4]

    mov     v6.s[0], v2.s[1]
    eor     v6.16b, v6.16b, v1.16b       // v6 = ->A[0]

    mov     v6.s[1], v5.s[0]
    mov     v6.s[2], v5.s[1]
    mov     v6.s[3], v5.s[2]             // v6 = ->A[0] || ->A[1] || ->A[2] || ->A[3]

    mov     v7.s[0], v5.s[3]             // v7 = ->A[4]

    // Apply Theta
    eor     v22.16b, v22.16b, v6.16b
    eor     v24.16b, v24.16b, v6.16b
    eor     v26.16b, v26.16b, v6.16b
    eor     v28.16b, v28.16b, v6.16b
    eor     v30.16b, v30.16b, v6.16b

    eor     v23.16b, v23.16b, v7.16b
    eor     v25.16b, v25.16b, v7.16b
    eor     v27.16b, v27.16b, v7.16b
    eor     v29.16b, v29.16b, v7.16b
    eor     v31.16b, v31.16b, v7.16b

    // Rho Pi
    mov     w11, v22.s[1]                // w11   = A[1]

    RhoPi   v26.s[0], w11, w10, 1        // A[10] = ROTL64(A[1], 1)
    RhoPi   v24.s[2], w10, w11, 3        // A[7]  = ROTL64(A[10], 3)
    RhoPi   v26.s[1], w11, w10, 6        // A[11] = ROTL64(A[7], 6)
    RhoPi   v28.s[2], w10, w11, 10       // A[17] = ROTL64(A[11], 10)
    RhoPi   v28.s[3], w11, w10, 15       // A[18] = ROTL64(A[17], 15)
    RhoPi   v22.s[3], w10, w11, 21       // A[3]  = ROTL64(A[18], 21)
    RhoPi   v24.s[0], w11, w10, 28       // A[5]  = ROTL64(A[3], 28)
    RhoPi   v28.s[1], w10, w11, 4        // A[16] = ROTL64(A[5], 4)
    RhoPi   v24.s[3], w11, w10, 13       // A[8]  = ROTL64(A[16], 13)
    RhoPi   v30.s[1], w10, w11, 23       // A[21] = ROTL64(A[8], 23)
    RhoPi   v31.s[0], w11, w10, 2        // A[24] = ROTL64(A[21], 2)
    RhoPi   v23.s[0], w10, w11, 14       // A[4]  = ROTL64(A[24], 14)
    RhoPi   v28.s[0], w11, w10, 27       // A[15] = ROTL64(A[4], 27)
    RhoPi   v30.s[3], w10, w11, 9        // A[23] = ROTL64(A[15], 9)
    RhoPi   v29.s[0], w11, w10, 24       // A[19] = ROTL64(A[23], 24)
    RhoPi   v26.s[3], w10, w11, 8        // A[13] = ROTL64(A[19], 8)
    RhoPi   v26.s[2], w11, w10, 25       // A[12] = ROTL64(A[13], 25)
    RhoPi   v22.s[2], w10, w11, 11       // A[2]  = ROTL64(A[12], 11)
    RhoPi   v30.s[0], w11, w10, 30       // A[20] = ROTL64(A[2], 30)
    RhoPi   v27.s[0], w10, w11, 18       // A[14] = ROTL64(A[20], 18)
    RhoPi   v30.s[2], w11, w10, 7        // A[22] = ROTL64(A[14], 7)
    RhoPi   v25.s[0], w10, w11, 29       // A[9]  = ROTL64(A[22], 29)
    RhoPi   v24.s[1], w11, w10, 20       // A[6]  = ROTL64(A[9], 20)

    ror     w10, w10, #20
    mov     v22.s[1], w10                // A[1]  = ROTL64(A[6], 12)

    // Chi
    Chi  v22, v23
    Chi  v24, v25
    Chi  v26, v27
    Chi  v28, v29  
    Chi  v30, v31

    // Iota
    ld1     { v20.s }[0], [x1], #4
    eor     v22.16b, v22.16b, v20.16b

    .endm

.align 8
KeccakP800_Permute_RoundConstants22:
    .word   0x00000001
    .word   0x00008082
    .word   0x0000808a
    .word   0x80008000
    .word   0x0000808b
    .word   0x80000001
    .word   0x80008081
    .word   0x00008009
    .word   0x0000008a
    .word   0x00000088
KeccakP800_Permute_RoundConstants12:
    .word   0x80008009
    .word   0x8000000a
    .word   0x8000808b
    .word   0x0000008b
    .word   0x00008089
    .word   0x00008003
    .word   0x00008002
    .word   0x00000080
    .word   0x0000800a
    .word   0x8000000a
    .word   0x80008081
    .word   0x00008080
KeccakP800_Permute_RoundConstants0:

//----------------------------------------------------------------------------
//
// void KeccakP800_Initialize(void *state)
//
.align 8
.global   KeccakP800_Initialize
KeccakP800_Initialize:
    movi    v0.2d, #0
    movi    v1.2d, #0
    st2     { v0.2d, v1.2d }, [x0], #32
    st2     { v0.2d, v1.2d }, [x0], #32
    st2     { v0.2d, v1.2d }, [x0], #32
    st1     { v0.s }[0], [x0]
    ret


// ----------------------------------------------------------------------------
//
//  void KeccakP800_AddByte(void *state, unsigned char byte, unsigned int offset)
//
.align 8
.global   KeccakP800_AddByte
KeccakP800_AddByte:
    ldrb    w3, [x0, x2]
    eor     w3, w3, w1
    strb    w3, [x0, x2]
    ret


// ----------------------------------------------------------------------------
//
//  void KeccakP800_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
//
.align 8
.global   KeccakP800_AddBytes
KeccakP800_AddBytes:
    add     x0, x0, x2
    subs    w4, w3, #1
    b.cc    KeccakP800_AddBytes_Exit // length 0, move along
KeccakP800_AddBytes_LanesLoop: // Go 2 lanes=8 bytes at a time
    subs    w3, w3, #8
    b.cc    KeccakP800_AddBytes_Bytes
    ld1     { v0.d }[0], [x0]
    ld1     { v4.d }[0], [x1], #8
    eor     v0.8b, v0.8b, v4.8b
    st1     { v0.d }[0], [x0], #8
    b       KeccakP800_AddBytes_LanesLoop
KeccakP800_AddBytes_Bytes:
    add     w3, w3, #8
KeccakP800_AddBytes_BytesLoop: // Same thing but go 1 byte at a time
    subs    w3, w3, #1
    b.cc    KeccakP800_AddBytes_Exit
    ldrb    w4, [x0]
    ldrb    w5, [x1], #1
    eor     w4, w4, w5
    strb    w4, [x0], #1
    b       KeccakP800_AddBytes_BytesLoop
KeccakP800_AddBytes_Exit:
    ret

// ----------------------------------------------------------------------------
//
//  void KeccakP800_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
//
.align 8
.global   KeccakP800_OverwriteBytes
KeccakP800_OverwriteBytes:
    add     x0, x0, x2
    subs    w4, w3, #1
    b.cc    KeccakP800_OverwriteBytes_Exit
KeccakP800_OverwriteBytes_LanesLoop:
    subs    w3, w3, #8
    b.cc    KeccakP800_OverwriteBytes_Bytes
    ld1     { v0.d }[0], [x1], #8
    st1     { v0.d }[0], [x0], #8
    b       KeccakP800_OverwriteBytes_LanesLoop
KeccakP800_OverwriteBytes_Bytes:
    add    w3, w3, #8
KeccakP800_OverwriteBytes_BytesLoop:
    subs    w3, w3, #1
    b.cc    KeccakP800_OverwriteBytes_Exit
    ldrb    w4, [x1], #1
    strb    w4, [x0], #1
    b       KeccakP800_OverwriteBytes_BytesLoop
KeccakP800_OverwriteBytes_Exit:
    ret


//----------------------------------------------------------------------------
//
// void KeccakP800_OverwriteWithZeroes(void *state, unsigned int byteCount)
//
.align 8
.global   KeccakP800_OverwriteWithZeroes
KeccakP800_OverwriteWithZeroes:
    subs    w2, w1, #1
    b.cc    KeccakP800_OverwriteWithZeroes_Exit
    movi    v0.2d, #0
    mov     w2, #0
KeccakP800_OverwriteWithZeroes_LanesLoop:
    subs    w1, w1, #8
    b.cc    KeccakP800_OverwriteWithZeroes_Bytes
    st1     { v0.d }[0], [x0], #8
    b       KeccakP800_OverwriteWithZeroes_LanesLoop
KeccakP800_OverwriteWithZeroes_Bytes:
    add     w1, w1, #8
KeccakP800_OverwriteWithZeroes_LoopBytes:
    subs    w1, w1, #1
    b.cc    KeccakP800_OverwriteWithZeroes_Exit
    strb    w2, [x0], #1
    b       KeccakP800_OverwriteWithZeroes_LoopBytes
KeccakP800_OverwriteWithZeroes_Exit:
    ret


// ----------------------------------------------------------------------------
//
//  void KeccakP800_ExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
//
.align 8
.global   KeccakP800_ExtractBytes
KeccakP800_ExtractBytes:
    add     x0, x0, x2
    subs    w4, w3, #1
    b.cc    KeccakP800_ExtractBytes_Exit
KeccakP800_ExtractBytes_LanesLoop:
    subs    w3, w3, #8
    b.cc    KeccakP800_ExtractBytes_Bytes
    ld1     { v0.d }[0], [x0], #8
    st1     { v0.d }[0], [x1], #8
    b       KeccakP800_ExtractBytes_LanesLoop
KeccakP800_ExtractBytes_Bytes:
    add     w3, w3, #8
KeccakP800_ExtractBytes_BytesLoop:
    subs    w3, w3, #1
    b.cc    KeccakP800_ExtractBytes_Exit
    ldrb    w4, [x0], #1
    strb    w4, [x1], #1
    b       KeccakP800_ExtractBytes_BytesLoop
KeccakP800_ExtractBytes_Exit:
    ret


// ----------------------------------------------------------------------------
//
//  void KeccakP800_ExtractAndAddBytes(void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
//
.align 8
.global   KeccakP800_ExtractAndAddBytes
KeccakP800_ExtractAndAddBytes:
    add     x0, x0, x3
    subs    w5, w4, #1
    b.cc    KeccakP800_ExtractAndAddBytes_Exit
KeccakP800_ExtractAndAddBytes_LanesLoop:
    subs    w4, w4, #8
    b.cc    KeccakP800_ExtractAndAddBytes_Bytes
    ld1     { v0.d }[0], [x0], #8
    ld1     { v4.d }[0], [x1], #8
    eor     v0.8b, v0.8b, v4.8b
    st1     { v0.d }[0], [x2], #8
    b       KeccakP800_ExtractAndAddBytes_LanesLoop
KeccakP800_ExtractAndAddBytes_Bytes:
    add     w4, w4, #8
KeccakP800_ExtractAndAddBytes_BytesLoop:
    subs    w4, w4, #1
    b.cc    KeccakP800_ExtractAndAddBytes_Exit
    ldrb    w5, [x0], #1
    ldrb    w6, [x1], #1
    eor     w5, w5, w6
    strb    w5, [x2], #1
    b       KeccakP800_ExtractAndAddBytes_BytesLoop
KeccakP800_ExtractAndAddBytes_Exit:
    ret

// ----------------------------------------------------------------------------
//
//  void KeccakP800_Permute_Nrounds( void *state, unsigned int nrounds )
//
.align 8
.global   KeccakP800_Permute_Nrounds
KeccakP800_Permute_Nrounds:
    mov     x2, x1
    adr     x1, KeccakP800_Permute_RoundConstants0
	lsl		x3, x2, #2
	sub		x1, x1, x3
    b       KeccakP800_Permute

// ----------------------------------------------------------------------------
//
//  void KeccakP800_Permute_12rounds( void *state )
//
.align 8
.global   KeccakP800_Permute_12rounds
KeccakP800_Permute_12rounds:
    adr     x1, KeccakP800_Permute_RoundConstants12
    mov     x2, #12
    b       KeccakP800_Permute

// ----------------------------------------------------------------------------
//
//  void KeccakP800_Permute_22rounds( void *state )
//
.align 8
.global   KeccakP800_Permute_22rounds
KeccakP800_Permute_22rounds:
    adr     x1, KeccakP800_Permute_RoundConstants22
    mov     x2, #22
    b       KeccakP800_Permute

//----------------------------------------------------------------------------
//
// void KeccakP800_Permute( void *state, uint32_t *rc, unsigned int nrounds )
//
.align 8
.global   KeccakP800_Permute
KeccakP800_Permute:
    LoadState
KeccakP800_Permute_RoundLoop:
    KeccakRound
    subs    w2, w2, #1
    bne     KeccakP800_Permute_RoundLoop
KeccakP800_Permute_Exit:
    StoreState
    ret

