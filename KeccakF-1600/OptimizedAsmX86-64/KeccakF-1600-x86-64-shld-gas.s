#;
#; Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
#; Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
#; denoted as "the implementer".
#;
#; For more information, feedback or questions, please refer to our websites:
#; http://keccak.noekeon.org/
#; http://keyak.noekeon.org/
#; http://ketje.noekeon.org/
#;
#; To the extent possible under law, the implementer has waived all copyright
#; and related or neighboring rights to the source code in this file.
#; http://creativecommons.org/publicdomain/zero/1.0/
#;

#; WARNING: State must be 256 bit (32 bytes) aligned.

	.text

#; --- defines

.equ UseSIMD, 0


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

#;	arguments passed in registers
.equ arg1,	%rdi
.equ arg2,	%rsi
.equ arg3,	%rdx
.equ arg4,	%rcx
.equ arg5,	%r8
.equ arg6,	%r9

#; arguments for 'Bytes' functions
.equ apState,	arg1
.equ apData,	arg2
.equ aOffset,	arg3
.equ aLength,	arg4

#	xor input into state section
.equ xpState,	%r9
.equ xLanes, 	%r8
.equ xComplement, 	%r8

# round vars
.equ rT1,		%rax
.equ rpState,	%rdi
.equ rpStack,	%rsp

.equ rDa,		%rbx
.equ rDe,		%rcx
.equ rDi,		%rdx
.equ rDo,		%r8
.equ rDu,		%r9

.equ rBa,		%r10 
.equ rBe,		%r11
.equ rBi,		%r12
.equ rBo,		%r13
.equ rBu,		%r14

.equ rCa,		%rsi
.equ rCe,		%rbp
.equ rCi,		rBi
.equ rCo,		rBo
.equ rCu,		%r15

.macro	mKeccakRound	iState, oState, rc, lastRound

	#; prepare Theta bis
	movq	rCe, rDa
	shld	$1, rDa, rDa

	movq	_bi(\iState), rCi
	xorq	_gi(\iState), rDi
	xorq	_ki(\iState), rCi
	xorq	rCu, rDa
	xorq	_mi(\iState), rDi
	xorq	rDi, rCi

	movq	rCi, rDe
	shld	$1, rDe, rDe

	movq	_bo(\iState), rCo
	xorq	_go(\iState), rDo
	xorq	_ko(\iState), rCo
	xorq	rCa, rDe
	xorq	_mo(\iState), rDo
	xorq	rDo, rCo

	movq	rCo, rDi
	shld	$1, rDi, rDi

	movq	rCu, rDo
	xorq	rCe, rDi
	shld	$1, rDo, rDo

	movq	rCa, rDu
	xorq	rCi, rDo
	shld	$1, rDu, rDu

	#; Theta Rho Pi Chi Iota, result b 
	movq	_ba(\iState), rBa
	movq	_ge(\iState), rBe
	xorq	rCo, rDu
	movq	_ki(\iState), rBi
	movq	_mo(\iState), rBo
	movq	_su(\iState), rBu
	xorq	rDe, rBe
	shld	$44, rBe, rBe
	xorq	rDi, rBi
	xorq	rDa, rBa
	shld	$43, rBi, rBi

	movq	rBe, rCa
	movq	$\rc, rT1
	orq		rBi, rCa
	xorq	rBa, rT1
	xorq	rT1, rCa
	movq	rCa, _ba(\oState)

	xorq	rDu, rBu
	shld	$14, rBu, rBu
	movq	rBa, rCu
	andq	rBe, rCu
	xorq	rBu, rCu
	movq	rCu, _bu(\oState)

	xorq	rDo, rBo
	shld	$21, rBo, rBo
	movq	rBo, rT1
	andq	rBu, rT1
	xorq	rBi, rT1
	movq	rT1, _bi(\oState)

	notq	rBi
	orq		rBa, rBu
	orq		rBo, rBi
	xorq	rBo, rBu
	xorq	rBe, rBi
	movq	rBu, _bo(\oState)
	movq	rBi, _be(\oState)
	.if		\lastRound == 0
	movq	rBi, rCe
	.endif

	#; Theta Rho Pi Chi, result g 
	movq	_gu(\iState), rBe
	xorq	rDu, rBe
	movq	_ka(\iState), rBi
	shld	$20, rBe, rBe
	xorq	rDa, rBi
	shld	$3,  rBi, rBi
	movq	_bo(\iState), rBa
	movq	rBe, rT1
	orq		rBi, rT1
	xorq	rDo, rBa
	movq	_me(\iState), rBo
	movq	_si(\iState), rBu
	shld	$28, rBa, rBa
	xorq	rBa, rT1
	movq	rT1, _ga(\oState)
	.if		\lastRound == 0
	xorq	rT1, rCa
	.endif

	xorq	rDe, rBo
	shld	$45, rBo, rBo
	movq	rBi, rT1
	andq	rBo, rT1
	xorq	rBe, rT1
	movq	rT1, _ge(\oState)
	.if		\lastRound == 0
	xorq	rT1, rCe
	.endif

	xorq	rDi, rBu
	shld	$61, rBu, rBu
	movq	rBu, rT1
	orq		rBa, rT1
	xorq	rBo, rT1
	movq	rT1, _go(\oState)

	andq	rBe, rBa
	xorq	rBu, rBa
	movq	rBa, _gu(\oState)
	notq	rBu
	.if		\lastRound == 0
	xorq	rBa, rCu
	.endif

	orq		rBu, rBo
	xorq	rBi, rBo
	movq	rBo, _gi(\oState)

	#; Theta Rho Pi Chi, result k
	movq	_be(\iState), rBa
	movq	_gi(\iState), rBe
	movq	_ko(\iState), rBi
	movq	_mu(\iState), rBo
	movq	_sa(\iState), rBu
	xorq	rDi, rBe
	shld	$6,  rBe, rBe
	xorq	rDo, rBi
	shld	$25, rBi, rBi
	movq	rBe, rT1
	orq		rBi, rT1
	xorq	rDe, rBa
	shld	$1,  rBa, rBa
	xorq	rBa, rT1
	movq	rT1, _ka(\oState)
	.if		\lastRound == 0
	xorq	rT1, rCa
	.endif

	xorq	rDu, rBo
	shld	$8,  rBo, rBo
	movq	rBi, rT1
	andq	rBo, rT1
	xorq	rBe, rT1
	movq	rT1, _ke(\oState)
	.if		\lastRound == 0
	xorq	rT1, rCe
	.endif

	xorq	rDa, rBu
	shld	$18, rBu, rBu
	notq	rBo
	movq	rBo, rT1
	andq	rBu, rT1
	xorq	rBi, rT1
	movq	rT1, _ki(\oState)

	movq	rBu, rT1
	orq		rBa, rT1
	xorq	rBo, rT1
	movq	rT1, _ko(\oState)

	andq	rBe, rBa
	xorq	rBu, rBa
	movq	rBa, _ku(\oState)
	.if		\lastRound == 0
	xorq	rBa, rCu
	.endif

	#; Theta Rho Pi Chi, result m
	movq	_ga(\iState), rBe
	xorq	rDa, rBe
	movq	_ke(\iState), rBi
	shld	$36, rBe, rBe
	xorq	rDe, rBi
	movq	_bu(\iState), rBa
	shld	$10, rBi, rBi
	movq	rBe, rT1
	movq	_mi(\iState), rBo
	andq	rBi, rT1
	xorq	rDu, rBa
	movq	_so(\iState), rBu
	shld	$27, rBa, rBa
	xorq	rBa, rT1
	movq	rT1, _ma(\oState)
	.if		\lastRound == 0
	xorq	rT1, rCa
	.endif

	xorq	rDi, rBo
	shld	$15, rBo, rBo
	movq	rBi, rT1
	orq		rBo, rT1
	xorq	rBe, rT1
	movq	rT1, _me(\oState)
	.if		\lastRound == 0
	xorq	rT1, rCe
	.endif

	xorq	rDo, rBu
	shld	$56, rBu, rBu
	notq	rBo
	movq	rBo, rT1
	orq		rBu, rT1
	xorq	rBi, rT1
	movq	rT1, _mi(\oState)

	orq		rBa, rBe
	xorq	rBu, rBe
	movq	rBe, _mu(\oState)

	andq	rBa, rBu
	xorq	rBo, rBu
	movq	rBu, _mo(\oState)
	.if		\lastRound == 0
	xorq	rBe, rCu
	.endif

	#; Theta Rho Pi Chi, result s
	movq	_bi(\iState), rBa
	movq	_go(\iState), rBe
	movq	_ku(\iState), rBi
	xorq	rDi, rBa
	movq	_ma(\iState), rBo
	shld	$62, rBa, rBa
	xorq	rDo, rBe
	movq	_se(\iState), rBu
	shld	$55, rBe, rBe

	xorq	rDu, rBi
	movq	rBa, rDu
	xorq	rDe, rBu
	shld	$2,  rBu, rBu
	andq	rBe, rDu
	xorq	rBu, rDu
	movq	rDu, _su(\oState)

	shld	$39, rBi, rBi
	.if		\lastRound == 0
	xorq	rDu, rCu
	.endif
	notq	rBe
	xorq	rDa, rBo
	movq	rBe, rDa
	andq	rBi, rDa
	xorq	rBa, rDa
	movq	rDa, _sa(\oState)
	.if		\lastRound == 0
	xorq	rDa, rCa
	.endif

	shld	$41, rBo, rBo
	movq	rBi, rDe
	orq		rBo, rDe
	xorq	rBe, rDe
	movq	rDe, _se(\oState)
	.if		\lastRound == 0
	xorq	rDe, rCe
	.endif

	movq	rBo, rDi
	movq	rBu, rDo
	andq	rBu, rDi
	orq		rBa, rDo
	xorq	rBi, rDi
	xorq	rBo, rDo
	movq	rDi, _si(\oState)
	movq	rDo, _so(\oState)

	.endm

.macro	mKeccakPermutation	

	subq		$8*25, %rsp

	movq		_ba(rpState), rCa             
	movq		_be(rpState), rCe
	movq		_bu(rpState), rCu

	xorq		_ga(rpState), rCa             
	xorq		_ge(rpState), rCe
	xorq		_gu(rpState), rCu             

	xorq		_ka(rpState), rCa             
	xorq		_ke(rpState), rCe
	xorq		_ku(rpState), rCu             

	xorq		_ma(rpState), rCa             
	xorq		_me(rpState), rCe
	xorq		_mu(rpState), rCu             

	xorq		_sa(rpState), rCa
	xorq		_se(rpState), rCe
	movq		_si(rpState), rDi
	movq		_so(rpState), rDo
	xorq		_su(rpState), rCu             

	mKeccakRound	rpState, rpStack, 0x0000000000000001, 0
	mKeccakRound	rpStack, rpState, 0x0000000000008082, 0
	mKeccakRound	rpState, rpStack, 0x800000000000808a, 0
	mKeccakRound	rpStack, rpState, 0x8000000080008000, 0
	mKeccakRound	rpState, rpStack, 0x000000000000808b, 0
	mKeccakRound	rpStack, rpState, 0x0000000080000001, 0
	mKeccakRound	rpState, rpStack, 0x8000000080008081, 0
	mKeccakRound	rpStack, rpState, 0x8000000000008009, 0
	mKeccakRound	rpState, rpStack, 0x000000000000008a, 0
	mKeccakRound	rpStack, rpState, 0x0000000000000088, 0
	mKeccakRound	rpState, rpStack, 0x0000000080008009, 0
	mKeccakRound	rpStack, rpState, 0x000000008000000a, 0
	mKeccakRound	rpState, rpStack, 0x000000008000808b, 0
	mKeccakRound	rpStack, rpState, 0x800000000000008b, 0
	mKeccakRound	rpState, rpStack, 0x8000000000008089, 0
	mKeccakRound	rpStack, rpState, 0x8000000000008003, 0
	mKeccakRound	rpState, rpStack, 0x8000000000008002, 0
	mKeccakRound	rpStack, rpState, 0x8000000000000080, 0
	mKeccakRound	rpState, rpStack, 0x000000000000800a, 0
	mKeccakRound	rpStack, rpState, 0x800000008000000a, 0
	mKeccakRound	rpState, rpStack, 0x8000000080008081, 0
	mKeccakRound	rpStack, rpState, 0x8000000000008080, 0
	mKeccakRound	rpState, rpStack, 0x0000000080000001, 0
	mKeccakRound	rpStack, rpState, 0x8000000080008008, 1
	addq		$8*25, %rsp
	.endm

.macro	mPushRegs	
	pushq	%rbx
	pushq	%rbp
	pushq	%r12
	pushq	%r13
	pushq	%r14
	pushq	%r15
	.endm

.macro	mPopRegs	
	popq	%r15
	popq	%r14
	popq	%r13
	popq	%r12
	popq	%rbp
	popq	%rbx
	.endm

.macro	mXor128	input, output, offset
	.if		UseSIMD == 0
	movq	\offset(\input), %rax
	movq	\offset+8(\input), %rbx
	xorq	%rax, \offset(\output)
	xorq	%rbx, \offset+8(\output)
	.else
	movdqu	\offset(\input), %xmm0
	pxor	\offset(\output), %xmm0
	movdqu	%xmm0, \offset(\output)
	.endif
	.endm

.macro	mXor256	input, output, offset
	.if		UseSIMD == 0
	movq	\offset(\input), %rax
	movq	\offset+8(\input), %rbx
	movq	\offset+16(\input), %r14
	movq	\offset+24(\input), %r15
	xorq	%rax, \offset(\output)
	xorq	%rbx, \offset+8(\output)
	xorq	%r14, \offset+16(\output)
	xorq	%r15,  \offset+24(\output)
	.else
	movdqu	\offset(\input), %xmm0
	pxor	\offset(\output), %xmm0
	movdqu	\offset+16(\input), %xmm1
	pxor	\offset+16(\output), %xmm1
	movdqu	%xmm0, \offset(\output)
	movdqu	%xmm1, \offset+16(\output)
	.endif
	.endm

.macro	mXor512	input, output, offset
	.if		UseSIMD == 0
	mXor256	\input, \output, \offset
	mXor256	\input, \output, \offset+32
	.else
	movdqu	\offset(\input), %xmm0
	movdqu	\offset+16(\input), %xmm1
	pxor	\offset(\output), %xmm0
	movdqu	\offset+32(\input), %xmm2
	pxor	\offset+16(\output), %xmm1
	movdqu	%xmm0, \offset(\output)
	movdqu	\offset+48(\input), %xmm3
	pxor	\offset+32(\output), %xmm2
	movdqu	%xmm1, \offset+16(\output)
	pxor	\offset+48(\output), %xmm3
	movdqu	%xmm2, \offset+32(\output)
	movdqu	%xmm3, \offset+48(\output)
	.endif
	.endm

# -------------------------------------------------------------------------

	.size	KeccakExtract1024bits, .-KeccakExtract1024bits
	.align	8
	.global	KeccakExtract1024bits
	.type	KeccakExtract1024bits, %function
KeccakExtract1024bits:

	movq		0*8(apState), %rax
	movq		1*8(apState), %rcx
	movq		2*8(apState), %rdx
	movq		3*8(apState), %r8
	notq		%rcx
	notq		%rdx
	movq		%rax, 0*8(%rsi)
	movq		%rcx, 1*8(%rsi)
	movq		%rdx, 2*8(%rsi)
	movq		%r8,  3*8(%rsi)

	movq		4*8(apState), %rax
	movq		5*8(apState), %rcx
	movq		6*8(apState), %rdx
	movq		7*8(apState), %r8
	movq		%rax, 4*8(%rsi)
	movq		%rcx, 5*8(%rsi)
	movq		%rdx, 6*8(%rsi)
	movq		%r8,  7*8(%rsi)

	movq		 8*8(apState), %rax
	movq		 9*8(apState), %rcx
	movq		10*8(apState), %rdx
	movq		11*8(apState), %r8
	notq		%rax
	movq		%rax,  8*8(%rsi)
	movq		%rcx,  9*8(%rsi)
	movq		%rdx, 10*8(%rsi)
	movq		%r8,  11*8(%rsi)

	movq		12*8(apState), %rax
	movq		13*8(apState), %rcx
	movq		14*8(apState), %rdx
	movq		15*8(apState), %r8
	notq		%rax
	movq		%rax, 12*8(%rsi)
	movq		%rcx, 13*8(%rsi)
	movq		%rdx, 14*8(%rsi)
	movq		%r8,  15*8(%rsi)
	ret



#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_Initialize( void )
#;
	.size	KeccakF1600_Initialize, .-KeccakF1600_Initialize
	.align	8
	.global	KeccakF1600_Initialize
	.type	KeccakF1600_Initialize, %function
KeccakF1600_Initialize:
	ret

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StateInitialize(void *state)
#;
	.size	KeccakF1600_StateInitialize, .-KeccakF1600_StateInitialize
	.align 8
	.global	KeccakF1600_StateInitialize
	.type	KeccakF1600_StateInitialize, %function
KeccakF1600_StateInitialize:
	xorq	%rax, %rax
	xorq	%rcx, %rcx
	notq	%rcx
	.if 	UseSIMD == 0
	movq	%rax,  0*8(apState)
	movq	%rcx,  1*8(apState)
	movq	%rcx,  2*8(apState)
	movq	%rax,  3*8(apState)
	movq	%rax,  4*8(apState)
	movq	%rax,  5*8(apState)
	movq	%rax,  6*8(apState)
	movq	%rax,  7*8(apState)
	movq	%rcx,  8*8(apState)
	movq	%rax,  9*8(apState)
	movq	%rax, 10*8(apState)
	movq	%rax, 11*8(apState)
	movq	%rcx, 12*8(apState)
	movq	%rax, 13*8(apState)
	movq	%rax, 14*8(apState)
	movq	%rax, 15*8(apState)
	movq	%rax, 16*8(apState)
	movq	%rcx, 17*8(apState)
	movq	%rax, 18*8(apState)
	movq	%rax, 19*8(apState)
	movq	%rcx, 20*8(apState)
	movq	%rax, 21*8(apState)
	movq	%rax, 22*8(apState)
	movq	%rax, 23*8(apState)
	movq	%rax, 24*8(apState)
	.else
	pxor	%xmm0, %xmm0
	movq	%rax,   0*8(apState)
	movq	%rcx,   1*8(apState)
	movq	%rcx,   2*8(apState)
	movq	%rax,   3*8(apState)
	movdqu	%xmm0,  4*8(apState)
	movdqu	%xmm0,  6*8(apState)
	movq	%rcx,   8*8(apState)
	movq	%rax,   9*8(apState)
	movdqu	%xmm0, 10*8(apState)
	movq	%rcx,  12*8(apState)
	movq	%rax,  13*8(apState)
	movdqu	%xmm0, 14*8(apState)
	movq	%rax,  16*8(apState)
	movq	%rcx,  17*8(apState)
	movdqu	%xmm0, 18*8(apState)
	movq	%rcx,  20*8(apState)
	movq	%rax,  21*8(apState)
	movdqu	%xmm0, 22*8(apState)
	movq	%rax,  24*8(apState)
	.endif
	ret

KeccakPowerOf2:
	.byte	1, 2, 4, 8, 16, 32, 64, 128
   
#;----------------------------------------------------------------------------
#;
#;    void KeccakF1600_StateComplementBit(void *state, unsigned int position)
#;
	.size	KeccakF1600_StateComplementBit, .-KeccakF1600_StateComplementBit
	.align 8
	.global	KeccakF1600_StateComplementBit
	.type	KeccakF1600_StateComplementBit, %function
KeccakF1600_StateComplementBit:
	movq	apState, xpState
	movq	arg2, %rax
	shrq	$3, %rax
	addq	%rax, xpState
	andq	$7, arg2
	movb	KeccakPowerOf2(arg2), %al
	xorb	%al, (xpState)
	ret

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StateXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#; 
	.size	KeccakF1600_StateXORBytes, .-KeccakF1600_StateXORBytes
	.align 8
	.global	KeccakF1600_StateXORBytes
	.type	KeccakF1600_StateXORBytes, %function
KeccakF1600_StateXORBytes:
	pushq   %rbx
	pushq   %r14
	pushq   %r15
	movq	apState, xpState
	addq	aOffset, xpState
	test	$0xF8, aLength
	jz		KeccakF1600_StateXORBytes_Bytes
	movq	aLength, xLanes
	shrq	$3, xLanes
	test	$16, xLanes
	jz		KeccakF1600_StateXORBytes_8Lanes
	mXor512	apData, xpState, 0
	mXor512	apData, xpState, 64
	addq	$128, apData
	addq	$128, xpState
KeccakF1600_StateXORBytes_8Lanes:
	test	$8, xLanes
	jz		KeccakF1600_StateXORBytes_4Lanes
	mXor512	apData, xpState, 0
	addq	$64, apData
	addq	$64, xpState
KeccakF1600_StateXORBytes_4Lanes:
	test	$4, xLanes
	jz		KeccakF1600_StateXORBytes_2Lanes
	mXor256	apData, xpState, 0
	addq	$32, apData
	addq	$32, xpState
KeccakF1600_StateXORBytes_2Lanes:
	test	$2, xLanes
	jz		KeccakF1600_StateXORBytes_1Lane
	mXor128	apData, xpState, 0
	addq	$16, apData
	addq	$16, xpState
KeccakF1600_StateXORBytes_1Lane:
	test	$1, xLanes
	jz		KeccakF1600_StateXORBytes_Bytes
	movq	(apData), %rax
	xorq	%rax, (xpState)
	addq	$8, apData
	addq	$8, xpState
KeccakF1600_StateXORBytes_Bytes:
	andq	$7, aLength
	jz		KeccakF1600_StateXORBytes_Exit
KeccakF1600_StateXORBytes_BytesLoop:
	movb	(apData), %al
	xorb	%al, (xpState)
	incq	apData
	incq	xpState
	decq	aLength
	jnz		KeccakF1600_StateXORBytes_BytesLoop
KeccakF1600_StateXORBytes_Exit:
	popq   %r15
	popq   %r14
	popq   %rbx
	ret


KeccakLaneComplementTable:
	.quad	0
	.quad	0xFFFFFFFFFFFFFFFF	#; 1
	.quad	0xFFFFFFFFFFFFFFFF	#; 2
	.quad	0
	.quad	0

	.quad	0
	.quad	0
	.quad	0
	.quad	0xFFFFFFFFFFFFFFFF	#; 8
	.quad	0

	.quad	0
	.quad	0
	.quad	0xFFFFFFFFFFFFFFFF	#; 12
	.quad	0
	.quad	0

	.quad	0
	.quad	0
	.quad	0xFFFFFFFFFFFFFFFF	#; 17
	.quad	0
	.quad	0

	.quad	0xFFFFFFFFFFFFFFFF	#; 20
	.quad	0
	.quad	0
	.quad	0
	.quad	0

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StateOverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#;
	.size	KeccakF1600_StateOverwriteBytes, .-KeccakF1600_StateOverwriteBytes
	.align 8
	.global	KeccakF1600_StateOverwriteBytes
	.type	KeccakF1600_StateOverwriteBytes, %function
KeccakF1600_StateOverwriteBytes:
	movq	apState, xpState
	addq	aOffset, xpState
	leaq	KeccakLaneComplementTable, xComplement
	addq	aOffset, xComplement
	subq	$8, aLength
	jc		KeccakF1600_StateOverwriteBytes_Bytes
KeccakF1600_StateOverwriteBytes_LanesLoop:
	movq	(apData), %rax
	xorq	(xComplement), %rax
	movq	%rax, (xpState)
	addq	$8, apData
	addq	$8, xComplement
	addq	$8, xpState
	subq	$8, aLength
	jnc		KeccakF1600_StateOverwriteBytes_LanesLoop
KeccakF1600_StateOverwriteBytes_Bytes:
	addq	$8, aLength
	jz		KeccakF1600_StateOverwriteBytes_Exit
KeccakF1600_StateOverwriteBytes_BytesLoop:
	movb	(apData), %al
	xorb	(xComplement), %al
	movb	%al, (xpState)
	incq	apData
	incq	xComplement
	incq	xpState
	decq	aLength
	jnz		KeccakF1600_StateOverwriteBytes_BytesLoop
KeccakF1600_StateOverwriteBytes_Exit:
	ret

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StateOverwriteWithZeroes(void *state, unsigned int byteCount)
#;
	.size	KeccakF1600_StateOverwriteWithZeroes, .-KeccakF1600_StateOverwriteWithZeroes
	.align 8
	.global	KeccakF1600_StateOverwriteWithZeroes
	.type	KeccakF1600_StateOverwriteWithZeroes, %function
KeccakF1600_StateOverwriteWithZeroes:
	movq	apState, xpState
	leaq	KeccakLaneComplementTable, xComplement
	subq	$8, arg2
	jc		KeccakF1600_StateOverwriteWithZeroes_Bytes
KeccakF1600_StateOverwriteWithZeroes_LanesLoop:
	movq	$0, %rax
	xorq	(xComplement), %rax
	movq	%rax, (xpState)
	addq	$8, xComplement
	addq	$8, xpState
	subq	$8, arg2
	jnc		KeccakF1600_StateOverwriteWithZeroes_LanesLoop
KeccakF1600_StateOverwriteWithZeroes_Bytes:
	addq	$8, arg2
	jz		KeccakF1600_StateOverwriteWithZeroes_Exit
KeccakF1600_StateOverwriteWithZeroes_BytesLoop:
	movb	$0, %al
	xorb	(xComplement), %al
	movb	%al, (xpState)
	incq	xComplement
	incq	xpState
	decq	arg2
	jnz		KeccakF1600_StateOverwriteWithZeroes_BytesLoop
KeccakF1600_StateOverwriteWithZeroes_Exit:
	ret

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StateExtractBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#;
	.size	KeccakF1600_StateExtractBytes, .-KeccakF1600_StateExtractBytes
	.align 8
	.global	KeccakF1600_StateExtractBytes
	.type	KeccakF1600_StateExtractBytes, %function
KeccakF1600_StateExtractBytes:
	movq	apState, xpState
	addq	aOffset, xpState
	leaq	KeccakLaneComplementTable, xComplement
	addq	aOffset, xComplement
	subq	$8, aLength
	jc		KeccakF1600_StateExtractBytes_Bytes
KeccakF1600_StateExtractBytes_LanesLoop:
	movq	(xpState), %rax
 	xorq	(xComplement), %rax
	movq	%rax, (apData)
	addq	$8, apData
	addq	$8, xComplement
	addq	$8, xpState
	subq	$8, aLength
	jnc		KeccakF1600_StateExtractBytes_LanesLoop
KeccakF1600_StateExtractBytes_Bytes:
	addq	$8, aLength
	jz		KeccakF1600_StateExtractBytes_Exit
KeccakF1600_StateExtractBytes_BytesLoop:
	movb	(xpState), %al
	xorb	(xComplement), %al
	movb	%al, (apData)
	incq	apData
	incq	xComplement
	incq	xpState
	decq	aLength
	jnz		KeccakF1600_StateExtractBytes_BytesLoop
KeccakF1600_StateExtractBytes_Exit:
	ret

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StateExtractAndXORBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
#;
	.size	KeccakF1600_StateExtractAndXORBytes, .-KeccakF1600_StateExtractAndXORBytes
	.align 8
	.global	KeccakF1600_StateExtractAndXORBytes
	.type	KeccakF1600_StateExtractAndXORBytes, %function
KeccakF1600_StateExtractAndXORBytes:
	movq	apState, xpState
	addq	aOffset, xpState
	leaq	KeccakLaneComplementTable, xComplement
	addq	aOffset, xComplement
	subq	$8, aLength
	jc		KeccakF1600_StateExtractAndXORBytes_Bytes
KeccakF1600_StateExtractAndXORBytes_LanesLoop:
	movq	(xpState), %rax
	xorq	(xComplement), %rax
	xorq	%rax, (apData)
	addq	$8, apData
	addq	$8, xComplement
	addq	$8, xpState
	subq	$8, aLength
	jnc		KeccakF1600_StateExtractAndXORBytes_LanesLoop
KeccakF1600_StateExtractAndXORBytes_Bytes:
	addq	$8, aLength
	jz		KeccakF1600_StateExtractAndXORBytes_Exit
KeccakF1600_StateExtractAndXORBytes_BytesLoop:
	movb	(xpState), %al
	xorb	(xComplement), %al
	xorb	%al, (apData)
	incq	apData
	incq	xComplement
	incq	xpState
	decq	aLength
	jnz		KeccakF1600_StateExtractAndXORBytes_BytesLoop
KeccakF1600_StateExtractAndXORBytes_Exit:
	ret

#;----------------------------------------------------------------------------
#;
#; void KeccakF1600_StatePermute( void *state )
#;
	.size	KeccakF1600_StatePermute, .-KeccakF1600_StatePermute
	.align 8
	.global	KeccakF1600_StatePermute
	.type	KeccakF1600_StatePermute, %function
KeccakF1600_StatePermute:
	mPushRegs
	mKeccakPermutation
	mPopRegs
	ret

#;----------------------------------------------------------------------------
#;
#; size_t KeccakF1600_SnP_FBWL_Absorb(	void *state, unsigned int laneCount, unsigned char *data, 
#;										size_t dataByteLen, unsigned char trailingBits )
#;

#;----------------------------------------------------------------------------
#;
#; size_t KeccakF1600_SnP_FBWL_Squeeze( void *state, unsigned int laneCount, unsigned char *data, size_t dataByteLen )
#;

#;----------------------------------------------------------------------------
#;
#; size_t KeccakF1600_SnP_FBWL_Wrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
#;										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits )
#;

#;----------------------------------------------------------------------------
#;
#; size_t KeccakF1600_SnP_FBWL_Unwrap( void *state, unsigned int laneCount, const unsigned char *dataIn, 
#;										unsigned char *dataOut, size_t dataByteLen, unsigned char trailingBits)
#;

