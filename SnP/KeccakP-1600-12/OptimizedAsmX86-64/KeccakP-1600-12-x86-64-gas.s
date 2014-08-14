#
# Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
# Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
# denoted as "the implementer".
#
# For more information, feedback or questions, please refer to our websites:
# http://keccak.noekeon.org/
# http://keyak.noekeon.org/
# http://ketje.noekeon.org/
#
# To the extent possible under law, the implementer has waived all copyright
# and related or neighboring rights to the source code in this file.
# http://creativecommons.org/publicdomain/zero/1.0/
#

# WARNING: State must be 256 bit (32 bytes) aligned.

    .text

# conditional assembly settings
.equ UseSIMD,    0    #UseSIMD == 1 not working yet
.equ InlinePerm, 1

# offsets in state
.equ _ba,  0*8
.equ _be,  1*8
.equ _bi,  2*8
.equ _bo,  3*8
.equ _bu,  4*8
.equ _ga,  5*8
.equ _ge,  6*8
.equ _gi,  7*8
.equ _go,  8*8
.equ _gu,  9*8
.equ _ka, 10*8
.equ _ke, 11*8
.equ _ki, 12*8
.equ _ko, 13*8
.equ _ku, 14*8
.equ _ma, 15*8
.equ _me, 16*8
.equ _mi, 17*8
.equ _mo, 18*8
.equ _mu, 19*8
.equ _sa, 20*8
.equ _se, 21*8
.equ _si, 22*8
.equ _so, 23*8
.equ _su, 24*8

# arguments passed in registers
.equ arg1, %rdi
.equ arg2, %rsi
.equ arg3, %rdx
.equ arg4, %rcx
.equ arg5, %r8
.equ arg6, %r9

# temporary registers
.equ rT1,  %rax
.equ rT1a, rT1
.equ rT1e, %rbx
.equ rT1i, %r14
.equ rT1o, %r15
.equ rT1u, arg6
.equ rT2a, %r10
.equ rT2e, %r11
.equ rT2i, %r12
.equ rT2o, %r13
.equ rT2u, arg5

# round vars
.equ rpState, arg1
.equ rpStack, %rsp

.equ rDa, %rbx
.equ rDe, %rcx
.equ rDi, %rdx
.equ rDo, %r8
.equ rDu, %r9

.equ rBa, %r10 
.equ rBe, %r11
.equ rBi, %r12
.equ rBo, %r13
.equ rBu, %r14

.equ rCa, %rsi
.equ rCe, %rbp
.equ rCi, rBi
.equ rCo, rBo
.equ rCu, %r15

.macro  mKeccakRound    iState, oState, rc, lastRound

    # prepare Theta bis
    movq    rCe, rDa
    rolq    rDa

    movq    _bi(\iState), rCi
    xorq    _gi(\iState), rDi
    xorq    rCu, rDa
    xorq    _ki(\iState), rCi
    xorq    _mi(\iState), rDi
    xorq    rDi, rCi

    movq    rCi, rDe
    rolq    rDe

    movq    _bo(\iState), rCo
    xorq    _go(\iState), rDo
    xorq    rCa, rDe
    xorq    _ko(\iState), rCo
    xorq    _mo(\iState), rDo
    xorq    rDo, rCo

    movq    rCo, rDi
    rolq    rDi

    movq    rCu, rDo
    xorq    rCe, rDi
    rolq    rDo

    movq    rCa, rDu
    xorq    rCi, rDo
    rolq    rDu

    # Theta Rho Pi Chi Iota, result b 
    movq    _ba(\iState), rBa
    movq    _ge(\iState), rBe
    xorq    rCo, rDu
    movq    _ki(\iState), rBi
    movq    _mo(\iState), rBo
    movq    _su(\iState), rBu
    xorq    rDe, rBe
    rolq    $44, rBe
    xorq    rDi, rBi
    xorq    rDa, rBa
    rolq    $43, rBi

    movq    rBe, rCa
    movq    $\rc, rT1
    orq     rBi, rCa
    xorq    rBa, rT1
    xorq    rT1, rCa
    movq    rCa, _ba(\oState)

    xorq    rDu, rBu
    rolq    $14, rBu
    movq    rBa, rCu
    andq    rBe, rCu
    xorq    rBu, rCu
    movq    rCu, _bu(\oState)

    xorq    rDo, rBo
    rolq    $21, rBo
    movq    rBo, rT1
    andq    rBu, rT1
    xorq    rBi, rT1
    movq    rT1, _bi(\oState)

    notq    rBi
    orq     rBa, rBu
    orq     rBo, rBi
    xorq    rBo, rBu
    xorq    rBe, rBi
    movq    rBu, _bo(\oState)
    movq    rBi, _be(\oState)
    .if     \lastRound == 0
    movq    rBi, rCe
    .endif

    # Theta Rho Pi Chi, result g 
    movq    _gu(\iState), rBe
    xorq    rDu, rBe
    movq    _ka(\iState), rBi
    rolq    $20, rBe
    xorq    rDa, rBi
    rolq    $3,  rBi
    movq    _bo(\iState), rBa
    movq    rBe, rT1
    orq     rBi, rT1
    xorq    rDo, rBa
    movq    _me(\iState), rBo
    movq    _si(\iState), rBu
    rolq    $28, rBa
    xorq    rBa, rT1
    movq    rT1, _ga(\oState)
    .if     \lastRound == 0
    xorq    rT1, rCa
    .endif

    xorq    rDe, rBo
    rolq    $45, rBo
    movq    rBi, rT1
    andq    rBo, rT1
    xorq    rBe, rT1
    movq    rT1, _ge(\oState)
    .if     \lastRound == 0
    xorq    rT1, rCe
    .endif

    xorq    rDi, rBu
    rolq    $61, rBu
    movq    rBu, rT1
    orq     rBa, rT1
    xorq    rBo, rT1
    movq    rT1, _go(\oState)

    andq    rBe, rBa
    xorq    rBu, rBa
    movq    rBa, _gu(\oState)
    notq    rBu
    .if     \lastRound == 0
    xorq    rBa, rCu
    .endif

    orq     rBu, rBo
    xorq    rBi, rBo
    movq    rBo, _gi(\oState)

    # Theta Rho Pi Chi, result k
    movq    _be(\iState), rBa
    movq    _gi(\iState), rBe
    movq    _ko(\iState), rBi
    movq    _mu(\iState), rBo
    movq    _sa(\iState), rBu
    xorq    rDi, rBe
    rolq    $6,  rBe
    xorq    rDo, rBi
    rolq    $25, rBi
    movq    rBe, rT1
    orq     rBi, rT1
    xorq    rDe, rBa
    rolq    $1,  rBa
    xorq    rBa, rT1
    movq    rT1, _ka(\oState)
    .if     \lastRound == 0
    xorq    rT1, rCa
    .endif

    xorq    rDu, rBo
    rolq    $8,  rBo
    movq    rBi, rT1
    andq    rBo, rT1
    xorq    rBe, rT1
    movq    rT1, _ke(\oState)
    .if     \lastRound == 0
    xorq    rT1, rCe
    .endif

    xorq    rDa, rBu
    rolq    $18, rBu
    notq    rBo
    movq    rBo, rT1
    andq    rBu, rT1
    xorq    rBi, rT1
    movq    rT1, _ki(\oState)

    movq    rBu, rT1
    orq     rBa, rT1
    xorq    rBo, rT1
    movq    rT1, _ko(\oState)

    andq    rBe, rBa
    xorq    rBu, rBa
    movq    rBa, _ku(\oState)
    .if     \lastRound == 0
    xorq    rBa, rCu
    .endif

    # Theta Rho Pi Chi, result m
    movq    _ga(\iState), rBe
    xorq    rDa, rBe
    movq    _ke(\iState), rBi
    rolq    $36, rBe
    xorq    rDe, rBi
    movq    _bu(\iState), rBa
    rolq    $10, rBi
    movq    rBe, rT1
    movq    _mi(\iState), rBo
    andq    rBi, rT1
    xorq    rDu, rBa
    movq    _so(\iState), rBu
    rolq    $27, rBa
    xorq    rBa, rT1
    movq    rT1, _ma(\oState)
    .if     \lastRound == 0
    xorq    rT1, rCa
    .endif

    xorq    rDi, rBo
    rolq    $15, rBo
    movq    rBi, rT1
    orq     rBo, rT1
    xorq    rBe, rT1
    movq    rT1, _me(\oState)
    .if     \lastRound == 0
    xorq    rT1, rCe
    .endif

    xorq    rDo, rBu
    rolq    $56, rBu
    notq    rBo
    movq    rBo, rT1
    orq     rBu, rT1
    xorq    rBi, rT1
    movq    rT1, _mi(\oState)

    orq     rBa, rBe
    xorq    rBu, rBe
    movq    rBe, _mu(\oState)

    andq    rBa, rBu
    xorq    rBo, rBu
    movq    rBu, _mo(\oState)
    .if     \lastRound == 0
    xorq    rBe, rCu
    .endif

    # Theta Rho Pi Chi, result s
    movq    _bi(\iState), rBa
    movq    _go(\iState), rBe
    movq    _ku(\iState), rBi
    xorq    rDi, rBa
    movq    _ma(\iState), rBo
    rolq    $62, rBa
    xorq    rDo, rBe
    movq    _se(\iState), rBu
    rolq    $55, rBe

    xorq    rDu, rBi
    movq    rBa, rDu
    xorq    rDe, rBu
    rolq    $2,  rBu
    andq    rBe, rDu
    xorq    rBu, rDu
    movq    rDu, _su(\oState)

    rolq    $39, rBi
    .if     \lastRound == 0
    xorq    rDu, rCu
    .endif
    notq    rBe
    xorq    rDa, rBo
    movq    rBe, rDa
    andq    rBi, rDa
    xorq    rBa, rDa
    movq    rDa, _sa(\oState)
    .if     \lastRound == 0
    xorq    rDa, rCa
    .endif

    rolq    $41, rBo
    movq    rBi, rDe
    orq     rBo, rDe
    xorq    rBe, rDe
    movq    rDe, _se(\oState)
    .if     \lastRound == 0
    xorq    rDe, rCe
    .endif

    movq    rBo, rDi
    movq    rBu, rDo
    andq    rBu, rDi
    orq     rBa, rDo
    xorq    rBi, rDi
    xorq    rBo, rDo
    movq    rDi, _si(\oState)
    movq    rDo, _so(\oState)

    .endm

.macro      mKeccakPermutation

    subq    $8*25, %rsp

    movq    _ba(rpState), rCa             
    movq    _be(rpState), rCe
    movq    _bu(rpState), rCu

    xorq    _ga(rpState), rCa             
    xorq    _ge(rpState), rCe
    xorq    _gu(rpState), rCu             

    xorq    _ka(rpState), rCa             
    xorq    _ke(rpState), rCe
    xorq    _ku(rpState), rCu             

    xorq    _ma(rpState), rCa             
    xorq    _me(rpState), rCe
    xorq    _mu(rpState), rCu             

    xorq    _sa(rpState), rCa
    xorq    _se(rpState), rCe
    movq    _si(rpState), rDi
    movq    _so(rpState), rDo
    xorq    _su(rpState), rCu             

    mKeccakRound    rpState, rpStack, 0x000000008000808b, 0
    mKeccakRound    rpStack, rpState, 0x800000000000008b, 0
    mKeccakRound    rpState, rpStack, 0x8000000000008089, 0
    mKeccakRound    rpStack, rpState, 0x8000000000008003, 0
    mKeccakRound    rpState, rpStack, 0x8000000000008002, 0
    mKeccakRound    rpStack, rpState, 0x8000000000000080, 0
    mKeccakRound    rpState, rpStack, 0x000000000000800a, 0
    mKeccakRound    rpStack, rpState, 0x800000008000000a, 0
    mKeccakRound    rpState, rpStack, 0x8000000080008081, 0
    mKeccakRound    rpStack, rpState, 0x8000000000008080, 0
    mKeccakRound    rpState, rpStack, 0x0000000080000001, 0
    mKeccakRound    rpStack, rpState, 0x8000000080008008, 1
    addq    $8*25, %rsp
    .endm

.macro      mKeccakPermutationInlinable
    .if     InlinePerm == 1
    mKeccakPermutation
    .else
    callq   KeccakP1600_12_StatePermute
    .endif
    .endm

.macro      mPushRegs
    pushq   %rbx
    pushq   %rbp
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15
    .endm

.macro      mPopRegs
    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rbp
    popq    %rbx
    .endm

.macro      mXor128 input, output, offset
    .if     UseSIMD == 0
    movq    \offset(\input), rT1a
    movq    \offset+8(\input), rT1e
    xorq    rT1a, \offset(\output)
    xorq    rT1e, \offset+8(\output)
    .else
    movdqu  \offset(\input), %xmm0
    pxor    \offset(\output), %xmm0
    movdqu  %xmm0, \offset(\output)
    .endif
    .endm

.macro      mXor256 input, output, offset
    .if     UseSIMD == 0
    movq    \offset(\input), rT1a
    movq    \offset+8(\input), rT1e
    movq    \offset+16(\input), rT1i
    movq    \offset+24(\input), rT1o
    xorq    rT1a, \offset(\output)
    xorq    rT1e, \offset+8(\output)
    xorq    rT1i, \offset+16(\output)
    xorq    rT1o, \offset+24(\output)
    .else
    movdqu  \offset(\input), %xmm0
    pxor    \offset(\output), %xmm0
    movdqu  \offset+16(\input), %xmm1
    pxor    \offset+16(\output), %xmm1
    movdqu  %xmm0, \offset(\output)
    movdqu  %xmm1, \offset+16(\output)
    .endif
    .endm

.macro      mXor512 input, output, offset
    .if     UseSIMD == 0
    mXor256 \input, \output, \offset
    mXor256 \input, \output, \offset+32
    .else
    movdqu  \offset(\input), %xmm0
    movdqu  \offset+16(\input), %xmm1
    pxor    \offset(\output), %xmm0
    movdqu  \offset+32(\input), %xmm2
    pxor    \offset+16(\output), %xmm1
    movdqu  %xmm0, \offset(\output)
    movdqu  \offset+48(\input), %xmm3
    pxor    \offset+32(\output), %xmm2
    movdqu  %xmm1, \offset+16(\output)
    pxor    \offset+48(\output), %xmm3
    movdqu  %xmm2, \offset+32(\output)
    movdqu  %xmm3, \offset+48(\output)
    .endif
    .endm

#----------------------------------------------------------------------------
#
# void KeccakF1600_Initialize( void )
#
    .size   KeccakF1600_Initialize, .-KeccakF1600_Initialize
    .align  8
    .global KeccakF1600_Initialize
    .type   KeccakF1600_Initialize, %function
KeccakF1600_Initialize:
    retq

#----------------------------------------------------------------------------
#
# void KeccakF1600_StateInitialize(void *state)
#
    .size   KeccakF1600_StateInitialize, .-KeccakF1600_StateInitialize
    .align  8
    .global KeccakF1600_StateInitialize
    .type   KeccakF1600_StateInitialize, %function
KeccakF1600_StateInitialize:
    xorq    %rax, %rax
    xorq    %rcx, %rcx
    notq    %rcx
    .if     UseSIMD == 0
    movq    %rax, _ba(arg1)
    movq    %rcx, _be(arg1)
    movq    %rcx, _bi(arg1)
    movq    %rax, _bo(arg1)
    movq    %rax, _bu(arg1)
    movq    %rax, _ga(arg1)
    movq    %rax, _ge(arg1)
    movq    %rax, _gi(arg1)
    movq    %rcx, _go(arg1)
    movq    %rax, _gu(arg1)
    movq    %rax, _ka(arg1)
    movq    %rax, _ke(arg1)
    movq    %rcx, _ki(arg1)
    movq    %rax, _ko(arg1)
    movq    %rax, _ku(arg1)
    movq    %rax, _ma(arg1)
    movq    %rax, _me(arg1)
    movq    %rcx, _mi(arg1)
    movq    %rax, _mo(arg1)
    movq    %rax, _mu(arg1)
    movq    %rcx, _sa(arg1)
    movq    %rax, _se(arg1)
    movq    %rax, _si(arg1)
    movq    %rax, _so(arg1)
    movq    %rax, _su(arg1)
    .else
    pxor    %xmm0, %xmm0
    movq    %rax,  _ba(arg1)
    movq    %rcx,  _be(arg1)
    movq    %rcx,  _bi(arg1)
    movq    %rax,  _bo(arg1)
    movdqu  %xmm0, _bu(arg1)
    movdqu  %xmm0, _ge(arg1)
    movq    %rcx,  _go(arg1)
    movq    %rax,  _gu(arg1)
    movdqu  %xmm0, _ka(arg1)
    movq    %rcx,  _ki(arg1)
    movq    %rax,  _ko(arg1)
    movdqu  %xmm0, _ku(arg1)
    movq    %rax,  _me(arg1)
    movq    %rcx,  _mi(arg1)
    movdqu  %xmm0, _mo(arg1)
    movq    %rcx,  _sa(arg1)
    movq    %rax,  _se(arg1)
    movdqu  %xmm0, _si(arg1)
    movq    %rax,  _su(arg1)
    .endif
    retq

KeccakPowerOf2:
    .byte   1, 2, 4, 8, 16, 32, 64, 128
   
#----------------------------------------------------------------------------
#
#    void KeccakF1600_StateComplementBit(void *state, unsigned int position)
#
    .size   KeccakF1600_StateComplementBit, .-KeccakF1600_StateComplementBit
    .align  8
    .global KeccakF1600_StateComplementBit
    .type   KeccakF1600_StateComplementBit, %function
KeccakF1600_StateComplementBit:
    movq    arg2, rT1
    shrq    $3, rT1
    addq    rT1, arg1
    andq    $7, arg2
    movb    KeccakPowerOf2(arg2), %al
    xorb    %al, (arg1)
    retq

#----------------------------------------------------------------------------
#
# void KeccakF1600_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
# 
    .size   KeccakF1600_StateXORBytes, .-KeccakF1600_StateXORBytes
    .align  8
    .global KeccakF1600_StateXORBytes
    .type   KeccakF1600_StateXORBytes, %function
KeccakF1600_StateXORBytes:
    pushq   rT1e
    pushq   rT1i
    pushq   rT1o
    addq    arg3, arg1
    testq   $0xF8, arg4
    jz      KeccakF1600_StateXORBytes_Bytes
    movq    arg4, arg6
    shrq    $3, arg6
    testq   $16, arg6
    jz      KeccakF1600_StateXORBytes_8Lanes
    mXor512 arg2, arg1, 0
    mXor512 arg2, arg1, 64
    addq    $128, arg2
    addq    $128, arg1
KeccakF1600_StateXORBytes_8Lanes:
    testq   $8, arg6
    jz      KeccakF1600_StateXORBytes_4Lanes
    mXor512 arg2, arg1, 0
    addq    $64, arg2
    addq    $64, arg1
KeccakF1600_StateXORBytes_4Lanes:
    testq   $4, arg6
    jz      KeccakF1600_StateXORBytes_2Lanes
    mXor256 arg2, arg1, 0
    addq    $32, arg2
    addq    $32, arg1
KeccakF1600_StateXORBytes_2Lanes:
    testq   $2, arg6
    jz      KeccakF1600_StateXORBytes_1Lane
    mXor128 arg2, arg1, 0
    addq    $16, arg2
    addq    $16, arg1
KeccakF1600_StateXORBytes_1Lane:
    testq   $1, arg6
    jz      KeccakF1600_StateXORBytes_Bytes
    movq    (arg2), rT1
    xorq    rT1, (arg1)
    addq    $8, arg2
    addq    $8, arg1
KeccakF1600_StateXORBytes_Bytes:
    andq    $7, arg4
    jz      KeccakF1600_StateXORBytes_Exit
KeccakF1600_StateXORBytes_BytesLoop:
    movb    (arg2), %al
    xorb    %al, (arg1)
    addq    $1, arg2
    addq    $1, arg1
    subq    $1, arg4
    jnz     KeccakF1600_StateXORBytes_BytesLoop
KeccakF1600_StateXORBytes_Exit:
    popq   rT1o
    popq   rT1i
    popq   rT1e
    retq


KeccakLaneComplementTable:
    .quad   0
    .quad   0xFFFFFFFFFFFFFFFF  #  1 be
    .quad   0xFFFFFFFFFFFFFFFF  #  2 bi
    .quad   0
    .quad   0

    .quad   0
    .quad   0
    .quad   0
    .quad   0xFFFFFFFFFFFFFFFF  #  8 go
    .quad   0

    .quad   0
    .quad   0
    .quad   0xFFFFFFFFFFFFFFFF  # 12 ki
    .quad   0
    .quad   0

    .quad   0
    .quad   0
    .quad   0xFFFFFFFFFFFFFFFF  # 17 mi
    .quad   0
    .quad   0

    .quad   0xFFFFFFFFFFFFFFFF  # 20 sa
    .quad   0
    .quad   0
    .quad   0
    .quad   0

#----------------------------------------------------------------------------
#
# void KeccakF1600_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#
    .size   KeccakF1600_StateOverwriteBytes, .-KeccakF1600_StateOverwriteBytes
    .align  8
    .global KeccakF1600_StateOverwriteBytes
    .type   KeccakF1600_StateOverwriteBytes, %function
KeccakF1600_StateOverwriteBytes:
    addq    arg3, arg1
    leaq    KeccakLaneComplementTable, arg5
    addq    arg3, arg5
    subq    $8, arg4
    jc      KeccakF1600_StateOverwriteBytes_Bytes
KeccakF1600_StateOverwriteBytes_LanesLoop:
    movq    (arg2), rT1
    xorq    (arg5), rT1
    movq    rT1, (arg1)
    addq    $8, arg2
    addq    $8, arg5
    addq    $8, arg1
    subq    $8, arg4
    jnc     KeccakF1600_StateOverwriteBytes_LanesLoop
KeccakF1600_StateOverwriteBytes_Bytes:
    addq    $8, arg4
    jz      KeccakF1600_StateOverwriteBytes_Exit
KeccakF1600_StateOverwriteBytes_BytesLoop:
    movb    (arg2), %al
    xorb    (arg5), %al
    movb    %al, (arg1)
    addq    $1, arg2
    addq    $1, arg5
    addq    $1, arg1
    subq    $1, arg4
    jnz     KeccakF1600_StateOverwriteBytes_BytesLoop
KeccakF1600_StateOverwriteBytes_Exit:
    retq

#----------------------------------------------------------------------------
#
# void KeccakF1600_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
#
    .size   KeccakF1600_StateOverwriteWithZeroes, .-KeccakF1600_StateOverwriteWithZeroes
    .align  8
    .global KeccakF1600_StateOverwriteWithZeroes
    .type   KeccakF1600_StateOverwriteWithZeroes, %function
KeccakF1600_StateOverwriteWithZeroes:
    leaq    KeccakLaneComplementTable, arg5
    subq    $8, arg2
    jc      KeccakF1600_StateOverwriteWithZeroes_Bytes
KeccakF1600_StateOverwriteWithZeroes_LanesLoop:
    movq    $0, rT1
    xorq    (arg5), rT1
    movq    rT1, (arg1)
    addq    $8, arg5
    addq    $8, arg1
    subq    $8, arg2
    jnc     KeccakF1600_StateOverwriteWithZeroes_LanesLoop
KeccakF1600_StateOverwriteWithZeroes_Bytes:
    addq    $8, arg2
    jz      KeccakF1600_StateOverwriteWithZeroes_Exit
KeccakF1600_StateOverwriteWithZeroes_BytesLoop:
    movb    $0, %al
    xorb    (arg5), %al
    movb    %al, (arg1)
    addq    $1, arg5
    addq    $1, arg1
    subq    $1, arg2
    jnz     KeccakF1600_StateOverwriteWithZeroes_BytesLoop
KeccakF1600_StateOverwriteWithZeroes_Exit:
    retq

#----------------------------------------------------------------------------
#
# void KeccakF1600_StateExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#
    .size   KeccakF1600_StateExtractBytes, .-KeccakF1600_StateExtractBytes
    .align  8
    .global KeccakF1600_StateExtractBytes
    .type   KeccakF1600_StateExtractBytes, %function
KeccakF1600_StateExtractBytes:
    addq    arg3, arg1
    leaq    KeccakLaneComplementTable, arg5
    addq    arg3, arg5
    subq    $8, arg4
    jc      KeccakF1600_StateExtractBytes_Bytes
KeccakF1600_StateExtractBytes_LanesLoop:
    movq    (arg1), rT1
    xorq    (arg5), rT1
    movq    rT1, (arg2)
    addq    $8, arg2
    addq    $8, arg5
    addq    $8, arg1
    subq    $8, arg4
    jnc     KeccakF1600_StateExtractBytes_LanesLoop
KeccakF1600_StateExtractBytes_Bytes:
    addq    $8, arg4
    jz      KeccakF1600_StateExtractBytes_Exit
KeccakF1600_StateExtractBytes_BytesLoop:
    movb    (arg1), %al
    xorb    (arg5), %al
    movb    %al, (arg2)
    addq    $1, arg2
    addq    $1, arg5
    addq    $1, arg1
    subq    $1, arg4
    jnz     KeccakF1600_StateExtractBytes_BytesLoop
KeccakF1600_StateExtractBytes_Exit:
    retq

#----------------------------------------------------------------------------
#
# void KeccakF1600_StateExtractAndXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#
    .size   KeccakF1600_StateExtractAndXORBytes, .-KeccakF1600_StateExtractAndXORBytes
    .align  8
    .global KeccakF1600_StateExtractAndXORBytes
    .type   KeccakF1600_StateExtractAndXORBytes, %function
KeccakF1600_StateExtractAndXORBytes:
    addq    arg3, arg1
    leaq    KeccakLaneComplementTable, arg5
    addq    arg3, arg5
    subq    $8, arg4
    jc      KeccakF1600_StateExtractAndXORBytes_Bytes
KeccakF1600_StateExtractAndXORBytes_LanesLoop:
    movq    (arg1), rT1
    xorq    (arg5), rT1
    xorq    rT1, (arg2)
    addq    $8, arg2
    addq    $8, arg5
    addq    $8, arg1
    subq    $8, arg4
    jnc     KeccakF1600_StateExtractAndXORBytes_LanesLoop
KeccakF1600_StateExtractAndXORBytes_Bytes:
    addq    $8, arg4
    jz      KeccakF1600_StateExtractAndXORBytes_Exit
KeccakF1600_StateExtractAndXORBytes_BytesLoop:
    movb    (arg1), %al
    xorb    (arg5), %al
    xorb    %al, (arg2)
    addq    $1, arg2
    addq    $1, arg5
    addq    $1, arg1
    subq    $1, arg4
    jnz     KeccakF1600_StateExtractAndXORBytes_BytesLoop
KeccakF1600_StateExtractAndXORBytes_Exit:
    retq

#----------------------------------------------------------------------------
#
# void KeccakP1600_12_StatePermute( void *state )
#
    .size   KeccakP1600_12_StatePermute, .-KeccakP1600_12_StatePermute
    .align  8
    .global KeccakP1600_12_StatePermute
    .type   KeccakP1600_12_StatePermute, %function
KeccakP1600_12_StatePermute:
    mPushRegs
    mKeccakPermutation
    mPopRegs
    retq

#----------------------------------------------------------------------------
#
# size_t KeccakP1600_12_SnP_FBWL_Absorb( void *state, unsigned int laneCount, unsigned char *data, 
#                                     size_t dataByteLen, unsigned char trailingBits )
#
    .size   KeccakP1600_12_SnP_FBWL_Absorb, .-KeccakP1600_12_SnP_FBWL_Absorb
    .align  8
    .global KeccakP1600_12_SnP_FBWL_Absorb
    .type   KeccakP1600_12_SnP_FBWL_Absorb, %function
KeccakP1600_12_SnP_FBWL_Absorb:
    mPushRegs
    pushq   arg3                        # save initial data pointer
    pushq   arg5                        # save trailingBits
    shrq    $3, arg4                    # nbrLanes = dataByteLen / SnP_laneLengthInBytes
    subq    arg2, arg4                  # if (nbrLanes >= laneCount)
    jc      KeccakP1600_12_SnP_FBWL_Absorb_Exit
    cmpq    $21, arg2
    jnz     KeccakP1600_12_SnP_FBWL_Absorb_VariableLaneCountLoop
KeccakP1600_12_SnP_FBWL_Absorb_Loop21:     # Fixed laneCount = 21 (rate = 1344, capacity = 256)
    movq    _ba(arg3), rT1a
    movq    _be(arg3), rT1e
    movq    _bi(arg3), rT1i
    movq    _bo(arg3), rT1o
    movq    _bu(arg3), rT1u
    movq    _ga(arg3), rT2a
    movq    _ge(arg3), rT2e
    movq    _gi(arg3), rT2i
    movq    _go(arg3), rT2o
    movq    _gu(arg3), rT2u
    xorq    rT1a, _ba(arg1)
    xorq    rT1e, _be(arg1)
    xorq    rT1i, _bi(arg1)
    xorq    rT1o, _bo(arg1)
    xorq    rT1u, _bu(arg1)
    xorq    rT2a, _ga(arg1)
    xorq    rT2e, _ge(arg1)
    xorq    rT2i, _gi(arg1)
    xorq    rT2o, _go(arg1)
    xorq    rT2u, _gu(arg1)
    movq    _ka(arg3), rT1a
    movq    _ke(arg3), rT1e
    movq    _ki(arg3), rT1i
    movq    _ko(arg3), rT1o
    movq    _ku(arg3), rT1u
    movq    _ma(arg3), rT2a
    movq    _me(arg3), rT2e
    movq    _mi(arg3), rT2i
    movq    _mo(arg3), rT2o
    movq    _mu(arg3), rT2u
    xorq    rT1a, _ka(arg1)
    xorq    rT1e, _ke(arg1)
    xorq    rT1i, _ki(arg1)
    xorq    rT1o, _ko(arg1)
    xorq    rT1u, _ku(arg1)
    movq    _sa(arg3), rT1a
    movq    (%rsp), rT1e                # xor trailingBits
    xorq    rT2a, _ma(arg1)
    xorq    rT2e, _me(arg1)
    xorq    rT2i, _mi(arg1)
    addq    $_se, arg3
    xorq    rT2o, _mo(arg1)
    xorq    rT2u, _mu(arg1)
    xorq    rT1a, _sa(arg1)
    xorq    rT1e, _se(arg1)
    pushq   arg3
    pushq   arg4
    mKeccakPermutationInlinable
    popq    arg4
    popq    arg3
    subq    $21, arg4                   # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Absorb_Loop21
KeccakP1600_12_SnP_FBWL_Absorb_Exit:
    addq    $8, %rsp                    # free trailingBits
    popq    rT1a                        # restore initial data pointer
    subq    rT1a, arg3                  # processed = data pointer - initial data pointer
    movq    arg3, rT1a
    mPopRegs
    retq
KeccakP1600_12_SnP_FBWL_Absorb_VariableLaneCountLoop:
    pushq   arg4
    pushq   arg2
    pushq   arg1
    movq    arg2, arg4                  # prepare xor call: length (in bytes)
    shlq    $3, arg4                    
    movq    arg3, arg2                  # data pointer
    xorq    arg3, arg3                  # offset = 0
    callq   KeccakF1600_StateXORBytes   #  (void *state, const unsigned char *data, unsigned int offset, unsigned int length)
    movq    arg2, arg3                  # updated data pointer
    movq    24(%rsp), rT1a              # xor trailingBits
    xorq    rT1a, (arg1)
    popq    arg1
    pushq   arg3
    callq   KeccakP1600_12_StatePermute
    popq    arg3
    popq    arg2    
    popq    arg4
    subq    arg2, arg4                  # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Absorb_VariableLaneCountLoop
    jmp     KeccakP1600_12_SnP_FBWL_Absorb_Exit

#----------------------------------------------------------------------------
#
# size_t KeccakP1600_12_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
#
    .size   KeccakP1600_12_SnP_FBWL_Squeeze, .-KeccakP1600_12_SnP_FBWL_Squeeze
    .align  8
    .global KeccakP1600_12_SnP_FBWL_Squeeze
    .type   KeccakP1600_12_SnP_FBWL_Squeeze, %function
KeccakP1600_12_SnP_FBWL_Squeeze:
    mPushRegs
    pushq   arg3                        # save initial data pointer
    shrq    $3, arg4                    # nbrLanes = dataByteLen / SnP_laneLengthInBytes
    subq    arg2, arg4                  # if (nbrLanes >= laneCount)
    jc      KeccakP1600_12_SnP_FBWL_Squeeze_Exit
    cmpq    $21, arg2
    jnz     KeccakP1600_12_SnP_FBWL_Squeeze_VariableLaneCountLoop
KeccakP1600_12_SnP_FBWL_Squeeze_Loop21:    # Fixed laneCount = 21 (rate = 1344, capacity = 256)
    pushq   arg4
    pushq   arg3
    mKeccakPermutationInlinable
    popq    arg3
    
    movq    _ba(arg1), rT1a
    movq    _be(arg1), rT1e
    movq    _bi(arg1), rT1i
    movq    _bo(arg1), rT1o
    notq    rT1e                # be 
    notq    rT1i                # bi
    movq    _bu(arg1), rT1u
    movq    rT1a, _ba(arg3)
    movq    rT1e, _be(arg3)
    movq    rT1i, _bi(arg3)
    movq    rT1o, _bo(arg3)
    movq    rT1u, _bu(arg3)

    movq    _ga(arg1), rT1a
    movq    _ge(arg1), rT1e
    movq    _gi(arg1), rT1i
    movq    _go(arg1), rT1o
    movq    _gu(arg1), rT1u
    movq    rT1a, _ga(arg3)
    notq    rT1o                # go
    movq    rT1e, _ge(arg3)
    movq    rT1i, _gi(arg3)
    movq    rT1o, _go(arg3)
    movq    rT1u, _gu(arg3)

    movq    _ka(arg1), rT1a
    movq    _ke(arg1), rT1e
    movq    _ki(arg1), rT1i
    movq    _ko(arg1), rT1o
    movq    _ku(arg1), rT1u
    notq    rT1i                # ki
    movq    rT1a, _ka(arg3)
    movq    rT1e, _ke(arg3)
    movq    rT1i, _ki(arg3)
    movq    rT1o, _ko(arg3)
    movq    rT1u, _ku(arg3)

    movq    _ma(arg1), rT1a
    movq    _me(arg1), rT1e
    movq    _mi(arg1), rT1i
    movq    _mo(arg1), rT1o
    movq    _mu(arg1), rT1u
    notq    rT1i                # mi
    movq    rT1a, _ma(arg3)
    movq    rT1e, _me(arg3)
    movq    rT1i, _mi(arg3)
    movq    rT1o, _mo(arg3)
    movq    rT1u, _mu(arg3)

    movq    _sa(arg1), rT1a
    notq    rT1a                # sa
    movq    rT1a, _sa(arg3)

    popq    arg4
    addq    $_se, arg3
    subq    $21, arg4                   # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Squeeze_Loop21
KeccakP1600_12_SnP_FBWL_Squeeze_Exit:
    popq    rT1a                        # restore initial data pointer
    subq    rT1a, arg3                  # processed = data pointer - initial data pointer
    movq    arg3, rT1a
    mPopRegs
    retq
KeccakP1600_12_SnP_FBWL_Squeeze_VariableLaneCountLoop:
    pushq   arg4
    pushq   arg2
    pushq   arg3
    callq   KeccakP1600_12_StatePermute
    popq    arg2                        # data pointer, arg3 on stack into arg2
    movq    (%rsp), arg4                # prepare extract call: length (in lanes) from arg2 on stack
    pushq   arg1
    shlq    $3, arg4                    # length (in bytes)
    xorq    arg3, arg3                  # offset = 0
     callq   KeccakF1600_StateExtractBytes #     (void *state, const unsigned char *data, unsigned int offset, unsigned int length)
    movq    arg2, arg3                  # updated data pointer
    popq    arg1
    popq    arg2
    popq    arg4
    subq    arg2, arg4                  # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Squeeze_VariableLaneCountLoop
    jmp     KeccakP1600_12_SnP_FBWL_Squeeze_Exit

#----------------------------------------------------------------------------
#
# size_t KeccakP1600_12_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
#                                   unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
#
    .size   KeccakP1600_12_SnP_FBWL_Wrap, .-KeccakP1600_12_SnP_FBWL_Wrap
    .align  8
    .global KeccakP1600_12_SnP_FBWL_Wrap
    .type   KeccakP1600_12_SnP_FBWL_Wrap, %function
KeccakP1600_12_SnP_FBWL_Wrap:
    mPushRegs
    pushq   arg3                        # save initial dataIn pointer
    pushq   arg6                        # save trailingBits
    shrq    $3, arg5                    # nbrLanes = dataByteLen / SnP_laneLengthInBytes
    subq    arg2, arg5                  # if (nbrLanes >= laneCount)
    jc      KeccakP1600_12_SnP_FBWL_Wrap_Exit
    cmpq    $21, arg2
    jnz     KeccakP1600_12_SnP_FBWL_Wrap_VariableLaneCountLoop
KeccakP1600_12_SnP_FBWL_Wrap_Loop21:       # Fixed laneCount = 21 (rate = 1344, capacity = 256)
    movq    _ba(arg1), rT1a
    movq    _be(arg1), rT1e
    movq    _bi(arg1), rT1i
    movq    _bo(arg1), rT1o
    movq    _bu(arg1), rT1u
    xorq    _ba(arg3), rT1a
    xorq    _be(arg3), rT1e
    xorq    _bi(arg3), rT1i
    xorq    _bo(arg3), rT1o
    xorq    _bu(arg3), rT1u
    movq    rT1a, _ba(arg1)
    movq    rT1e, _be(arg1)
    movq    rT1i, _bi(arg1)
    movq    rT1o, _bo(arg1)
    movq    rT1u, _bu(arg1)
    notq    rT1e                # be 
    notq    rT1i                # bi
    movq    rT1a, _ba(arg4)
    movq    rT1e, _be(arg4)
    movq    rT1i, _bi(arg4)
    movq    rT1o, _bo(arg4)
    movq    rT1u, _bu(arg4)

    movq    _ga(arg1), rT1a
    movq    _ge(arg1), rT1e
    movq    _gi(arg1), rT1i
    movq    _go(arg1), rT1o
    movq    _gu(arg1), rT1u
    xorq    _ga(arg3), rT1a
    xorq    _ge(arg3), rT1e
    xorq    _gi(arg3), rT1i
    xorq    _go(arg3), rT1o
    xorq    _gu(arg3), rT1u
    movq    rT1a, _ga(arg1)
    movq    rT1e, _ge(arg1)
    movq    rT1i, _gi(arg1)
    movq    rT1o, _go(arg1)
    movq    rT1u, _gu(arg1)
    notq    rT1o                # go
    movq    rT1a, _ga(arg4)
    movq    rT1e, _ge(arg4)
    movq    rT1i, _gi(arg4)
    movq    rT1o, _go(arg4)
    movq    rT1u, _gu(arg4)

    movq    _ka(arg1), rT1a
    movq    _ke(arg1), rT1e
    movq    _ki(arg1), rT1i
    movq    _ko(arg1), rT1o
    movq    _ku(arg1), rT1u
    xorq    _ka(arg3), rT1a
    xorq    _ke(arg3), rT1e
    xorq    _ki(arg3), rT1i
    xorq    _ko(arg3), rT1o
    xorq    _ku(arg3), rT1u
    movq    rT1a, _ka(arg1)
    movq    rT1e, _ke(arg1)
    movq    rT1i, _ki(arg1)
    movq    rT1o, _ko(arg1)
    movq    rT1u, _ku(arg1)
    notq    rT1i                # ki
    movq    rT1a, _ka(arg4)
    movq    rT1e, _ke(arg4)
    movq    rT1i, _ki(arg4)
    movq    rT1o, _ko(arg4)
    movq    rT1u, _ku(arg4)

    movq    _ma(arg1), rT1a
    movq    _me(arg1), rT1e
    movq    _mi(arg1), rT1i
    movq    _mo(arg1), rT1o
    movq    _mu(arg1), rT1u
    xorq    _ma(arg3), rT1a
    xorq    _me(arg3), rT1e
    xorq    _mi(arg3), rT1i
    xorq    _mo(arg3), rT1o
    xorq    _mu(arg3), rT1u
    movq    rT1a, _ma(arg1)
    movq    rT1e, _me(arg1)
    movq    rT1i, _mi(arg1)
    movq    rT1o, _mo(arg1)
    movq    rT1u, _mu(arg1)
    notq    rT1i                # mi
    movq    rT1a, _ma(arg4)
    movq    rT1e, _me(arg4)
    movq    rT1i, _mi(arg4)
    movq    rT1o, _mo(arg4)
    movq    rT1u, _mu(arg4)

    movq    _sa(arg1), rT1a
    xorq    _sa(arg3), rT1a
    movq    rT1a, _sa(arg1)
    notq    rT1a                # sa
    movq    rT1a, _sa(arg4)

    addq    $_se, arg3                  # update dataIn, dataOut
    addq    $_se, arg4
    movq    (%rsp), rT1a                # xor trailingBits
    xorq    rT1a, _se(arg1)
    pushq   arg3                        # permute
    pushq   arg4
    pushq   arg5
    mKeccakPermutationInlinable
    popq    arg5
    popq    arg4
    popq    arg3
    subq    $21, arg5                   # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Wrap_Loop21
KeccakP1600_12_SnP_FBWL_Wrap_Exit:
    addq    $8, %rsp                    # free trailingBits
    popq    rT1a                        # restore initial data pointer
    subq    rT1a, arg3                  # processed = data pointer - initial data pointer
    movq    arg3, rT1a
    mPopRegs
    retq
KeccakP1600_12_SnP_FBWL_Wrap_VariableLaneCountLoop:
    pushq   arg5                        # length to go
    pushq   arg2                        # nr lanes

    pushq   arg4                        # dataOut

    movq    arg2, arg4                  # prepare xor call: length (in bytes)
    shlq    $3, arg4
    pushq   arg4                        # save length
    movq    arg3, arg2                  # dataIn pointer
    xorq    arg3, arg3                  # offset = 0
    callq   KeccakF1600_StateXORBytes   # (void *state, const unsigned char *data, unsigned int offset, unsigned int length)
    movq    arg2, arg3                  # updated dataIn pointer

    popq    arg4                        # restore length
    popq    arg2                        # restore dataOut pointer
    pushq   arg3                        # save dataIn
    pushq   arg4                        # save length
    subq    arg4, arg1                  # restore state pointer
    xorq    arg3, arg3                  # offset = 0
    callq   KeccakF1600_StateExtractBytes # (void *state, const unsigned char *data, unsigned int offset, unsigned int length)

    popq    arg4                        # restore length
    movq    24(%rsp), rT1a              # xor trailingBits
    xorq    rT1a, (arg1)
    subq    arg4, arg1                  # restore state pointer

    movq    arg2, arg4                  # updated dataOut pointer
    pushq   arg4
    callq   KeccakP1600_12_StatePermute
    popq    arg4
    popq    arg3

    popq    arg2    
    popq    arg5
    subq    arg2, arg5                  # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Wrap_VariableLaneCountLoop
    jmp     KeccakP1600_12_SnP_FBWL_Wrap_Exit

#----------------------------------------------------------------------------
#
# size_t KeccakP1600_12_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
#                                     unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
#
    .size   KeccakP1600_12_SnP_FBWL_Unwrap, .-KeccakP1600_12_SnP_FBWL_Unwrap
    .align  8
    .global KeccakP1600_12_SnP_FBWL_Unwrap
    .type   KeccakP1600_12_SnP_FBWL_Unwrap, %function
KeccakP1600_12_SnP_FBWL_Unwrap:
    mPushRegs
    pushq   arg4                        # save initial dataOut pointer
    pushq   arg6                        # save trailingBits
    shrq    $3, arg5                    # nbrLanes = dataByteLen / SnP_laneLengthInBytes
    subq    arg2, arg5                  # if (nbrLanes >= laneCount)
    jc      KeccakP1600_12_SnP_FBWL_Unwrap_Exit
    cmpq    $21, arg2
    jnz     KeccakP1600_12_SnP_FBWL_Unwrap_VariableLaneCountLoop
KeccakP1600_12_SnP_FBWL_Unwrap_Loop21:     # Fixed laneCount = 21 (rate = 1344, capacity = 256)
    pushq   arg5

    movq    _ba(arg3), rT1a
    movq    _be(arg3), rT1e
    movq    _bi(arg3), rT1i
    movq    _bo(arg3), rT1o
    movq    _bu(arg3), rT1u
    notq    rT1e                # be 
    notq    rT1i                # bi
    movq    _ba(arg1), rT2a
    movq    _be(arg1), rT2e
    movq    _bi(arg1), rT2i
    movq    _bo(arg1), rT2o
    movq    _bu(arg1), rT2u
    movq    rT1a, _ba(arg1)
    movq    rT1e, _be(arg1)
    movq    rT1i, _bi(arg1)
    movq    rT1o, _bo(arg1)
    movq    rT1u, _bu(arg1)
    xorq    rT2a, rT1a
    xorq    rT2e, rT1e
    xorq    rT2i, rT1i
    xorq    rT2o, rT1o
    xorq    rT2u, rT1u
    movq    rT1a, _ba(arg4)
    movq    rT1e, _be(arg4)
    movq    rT1i, _bi(arg4)
    movq    rT1o, _bo(arg4)
    movq    rT1u, _bu(arg4)

    movq    _ga(arg3), rT1a
    movq    _ge(arg3), rT1e
    movq    _gi(arg3), rT1i
    movq    _go(arg3), rT1o
    movq    _gu(arg3), rT1u
    notq    rT1o                # go
    movq    _ga(arg1), rT2a
    movq    _ge(arg1), rT2e
    movq    _gi(arg1), rT2i
    movq    _go(arg1), rT2o
    movq    _gu(arg1), rT2u
    movq    rT1a, _ga(arg1)
    movq    rT1e, _ge(arg1)
    movq    rT1i, _gi(arg1)
    movq    rT1o, _go(arg1)
    movq    rT1u, _gu(arg1)
    xorq    rT2a, rT1a
    xorq    rT2e, rT1e
    xorq    rT2i, rT1i
    xorq    rT2o, rT1o
    xorq    rT2u, rT1u
    movq    rT1a, _ga(arg4)
    movq    rT1e, _ge(arg4)
    movq    rT1i, _gi(arg4)
    movq    rT1o, _go(arg4)
    movq    rT1u, _gu(arg4)

    movq    _ka(arg3), rT1a
    movq    _ke(arg3), rT1e
    movq    _ki(arg3), rT1i
    movq    _ko(arg3), rT1o
    movq    _ku(arg3), rT1u
    notq    rT1i                # ki
    movq    _ka(arg1), rT2a
    movq    _ke(arg1), rT2e
    movq    _ki(arg1), rT2i
    movq    _ko(arg1), rT2o
    movq    _ku(arg1), rT2u
    movq    rT1a, _ka(arg1)
    movq    rT1e, _ke(arg1)
    movq    rT1i, _ki(arg1)
    movq    rT1o, _ko(arg1)
    movq    rT1u, _ku(arg1)
    xorq    rT2a, rT1a
    xorq    rT2e, rT1e
    xorq    rT2i, rT1i
    xorq    rT2o, rT1o
    xorq    rT2u, rT1u
    movq    rT1a, _ka(arg4)
    movq    rT1e, _ke(arg4)
    movq    rT1i, _ki(arg4)
    movq    rT1o, _ko(arg4)
    movq    rT1u, _ku(arg4)

    movq    _ma(arg3), rT1a
    movq    _me(arg3), rT1e
    movq    _mi(arg3), rT1i
    movq    _mo(arg3), rT1o
    movq    _mu(arg3), rT1u
    notq    rT1i                # mi
    movq    _ma(arg1), rT2a
    movq    _me(arg1), rT2e
    movq    _mi(arg1), rT2i
    movq    _mo(arg1), rT2o
    movq    _mu(arg1), rT2u
    movq    rT1a, _ma(arg1)
    movq    rT1e, _me(arg1)
    movq    rT1i, _mi(arg1)
    movq    rT1o, _mo(arg1)
    movq    rT1u, _mu(arg1)
    xorq    rT2a, rT1a
    xorq    rT2e, rT1e
    xorq    rT2i, rT1i
    xorq    rT2o, rT1o
    xorq    rT2u, rT1u
    movq    rT1a, _ma(arg4)
    movq    rT1e, _me(arg4)
    movq    rT1i, _mi(arg4)
    movq    rT1o, _mo(arg4)
    movq    rT1u, _mu(arg4)

    movq    _sa(arg3), rT1a
    notq    rT1a                # sa
    movq    _sa(arg1), rT2a
    movq    rT1a, _sa(arg1)
    xorq    rT2a, rT1a
    movq    rT1a, _sa(arg4)

    addq    $_se, arg3                  # update dataIn, dataOut
    addq    $_se, arg4
    movq    8(%rsp), rT1a               # xor trailingBits
    xorq    rT1a, _se(arg1)
    pushq   arg4
    pushq   arg3                        # permute
    mKeccakPermutationInlinable
    popq    arg3
    popq    arg4
    popq    arg5
    subq    $21, arg5                   # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Unwrap_Loop21
KeccakP1600_12_SnP_FBWL_Unwrap_Exit:
    addq    $8, %rsp                    # free trailingBits
    popq    rT1a                        # restore initial data pointer
    subq    rT1a, arg4                  # processed = data pointer - initial data pointer
    movq    arg4, rT1a
    mPopRegs
    retq
KeccakP1600_12_SnP_FBWL_Unwrap_VariableLaneCountLoop:
    pushq   arg5                        # nr lanes to go
    pushq   arg2                        # laneCount

    leaq    KeccakLaneComplementTable, arg6
    pushq   arg1
KeccakP1600_12_SnP_FBWL_Unwrap_VariableLaneCount_LaneLoop:
    movq    (arg3), rT1a
    xorq    (arg6), rT1a
    movq    (arg1), rT1e
    movq    rT1a, (arg1)
    xorq    rT1e, rT1a
    movq    rT1a, (arg4)
    addq    $8, arg3
    addq    $8, arg4
    addq    $8, arg6
    addq    $8, arg1
    subq    $1, arg2
    jnz     KeccakP1600_12_SnP_FBWL_Unwrap_VariableLaneCount_LaneLoop
    movq    24(%rsp), rT1a              # xor trailingBits
    xorq    rT1a, (arg1)
    popq    arg1                        # restore state pointer
    pushq   arg3
    pushq   arg4
    callq   KeccakP1600_12_StatePermute
    popq    arg4
    popq    arg3
    popq    arg2    
    popq    arg5
    subq    arg2, arg5                  # while (nbrLanes >= 21)
    jnc     KeccakP1600_12_SnP_FBWL_Unwrap_VariableLaneCountLoop
    jmp     KeccakP1600_12_SnP_FBWL_Unwrap_Exit
