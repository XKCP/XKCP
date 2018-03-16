    .file   "KeccakP-1600-opt64.c"
    .text
    .p2align 4,,15
    .globl  KeccakP1600_Initialize
    .type   KeccakP1600_Initialize, @function
KeccakP1600_Initialize:
.LFB22:
    .cfi_startproc
    movq    %rdi, %rsi
    movl    $200, %r8d
    testb   $1, %sil
    jne .L27
    testb   $2, %dil
    jne .L28
.L3:
    testb   $4, %dil
    jne .L29
.L4:
    movl    %r8d, %ecx
    xorl    %eax, %eax
    shrl    $3, %ecx
    testb   $4, %r8b
    rep stosq
    je  .L5
    movl    $0, (%rdi)
    addq    $4, %rdi
.L5:
    testb   $2, %r8b
    je  .L6
    movw    $0, (%rdi)
    addq    $2, %rdi
.L6:
    andl    $1, %r8d
    je  .L7
    movb    $0, (%rdi)
.L7:
    movq    $-1, 8(%rsi)
    movq    $-1, 16(%rsi)
    movq    $-1, 64(%rsi)
    movq    $-1, 96(%rsi)
    movq    $-1, 136(%rsi)
    movq    $-1, 160(%rsi)
    ret
    .p2align 4,,10
    .p2align 3
.L27:
    leaq    1(%rsi), %rdi
    movb    $0, (%rsi)
    movb    $-57, %r8b
    testb   $2, %dil
    je  .L3
    .p2align 4,,10
    .p2align 3
.L28:
    movw    $0, (%rdi)
    addq    $2, %rdi
    subl    $2, %r8d
    testb   $4, %dil
    je  .L4
    .p2align 4,,10
    .p2align 3
.L29:
    movl    $0, (%rdi)
    subl    $4, %r8d
    addq    $4, %rdi
    jmp .L4
    .cfi_endproc
.LFE22:
    .size   KeccakP1600_Initialize, .-KeccakP1600_Initialize
    .p2align 4,,15
    .globl  KeccakP1600_AddBytesInLane
    .type   KeccakP1600_AddBytesInLane, @function
KeccakP1600_AddBytesInLane:
.LFB23:
    .cfi_startproc
    movq    %rbp, -8(%rsp)
    .cfi_offset 6, -16
    movl    %esi, %ebp
    movq    %rbx, -16(%rsp)
    subq    $56, %rsp
    .cfi_def_cfa_offset 64
    .cfi_offset 3, -24
    testl   %r8d, %r8d
    je  .L30
    cmpl    $1, %r8d
    movq    %rdi, %rbx
    movq    %rdx, %rsi
    je  .L38
    leaq    16(%rsp), %rdi
    movl    %r8d, %edx
    movl    %ecx, 8(%rsp)
    movq    $0, 16(%rsp)
    call    memcpy
    movq    16(%rsp), %rax
    movl    8(%rsp), %ecx
.L33:
    sall    $3, %ecx
    salq    %cl, %rax
    xorq    %rax, (%rbx,%rbp,8)
.L30:
    movq    40(%rsp), %rbx
    movq    48(%rsp), %rbp
    addq    $56, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 8
    ret
    .p2align 4,,10
    .p2align 3
.L38:
    .cfi_restore_state
    movzbl  (%rdx), %eax
    jmp .L33
    .cfi_endproc
.LFE23:
    .size   KeccakP1600_AddBytesInLane, .-KeccakP1600_AddBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_AddLanes
    .type   KeccakP1600_AddLanes, @function
KeccakP1600_AddLanes:
.LFB24:
    .cfi_startproc
    cmpl    $7, %edx
    jbe .L47
    movl    $8, %ecx
    xorl    %eax, %eax
    jmp .L41
    .p2align 4,,10
    .p2align 3
.L48:
    movl    %r8d, %ecx
.L41:
    movl    %eax, %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    1(%rax), %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    2(%rax), %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    3(%rax), %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    4(%rax), %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    5(%rax), %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    6(%rax), %r8d
    addl    $7, %eax
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    movq    (%rsi,%rax,8), %r8
    xorq    %r8, (%rdi,%rax,8)
    leal    8(%rcx), %r8d
    movl    %ecx, %eax
    cmpl    %edx, %r8d
    jbe .L48
    leal    4(%rcx), %eax
    leal    2(%rcx), %r8d
.L40:
    cmpl    %eax, %edx
    jae .L53
    jmp .L57
    .p2align 4,,10
    .p2align 3
.L50:
    movl    %r8d, %eax
.L53:
    movl    %ecx, %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    1(%rcx), %r8d
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    leal    2(%rcx), %r8d
    addl    $3, %ecx
    movq    (%rsi,%r8,8), %r9
    xorq    %r9, (%rdi,%r8,8)
    movq    (%rsi,%rcx,8), %r8
    xorq    %r8, (%rdi,%rcx,8)
    leal    4(%rax), %r8d
    movl    %eax, %ecx
    cmpl    %r8d, %edx
    jae .L50
    leal    2(%rax), %r8d
.L42:
    cmpl    %r8d, %edx
    jae .L54
    jmp .L58
    .p2align 4,,10
    .p2align 3
.L52:
    movl    %ecx, %r8d
.L54:
    movl    %eax, %ecx
    addl    $1, %eax
    movq    (%rsi,%rcx,8), %r9
    xorq    %r9, (%rdi,%rcx,8)
    movq    (%rsi,%rax,8), %rcx
    xorq    %rcx, (%rdi,%rax,8)
    leal    2(%r8), %ecx
    movl    %r8d, %eax
    cmpl    %ecx, %edx
    jae .L52
.L44:
    cmpl    %r8d, %edx
    jbe .L39
    movq    (%rsi,%r8,8), %rax
    xorq    %rax, (%rdi,%r8,8)
.L39:
    rep
    ret
.L47:
    movl    $2, %r8d
    movl    $4, %eax
    xorl    %ecx, %ecx
    jmp .L40
.L57:
    movl    %ecx, %eax
    jmp .L42
.L58:
    movl    %eax, %r8d
    jmp .L44
    .cfi_endproc
.LFE24:
    .size   KeccakP1600_AddLanes, .-KeccakP1600_AddLanes
    .p2align 4,,15
    .globl  KeccakP1600_AddByte
    .type   KeccakP1600_AddByte, @function
KeccakP1600_AddByte:
.LFB25:
    .cfi_startproc
    movl    %edx, %eax
    andl    $7, %edx
    movzbl  %sil, %esi
    leal    0(,%rdx,8), %ecx
    shrl    $3, %eax
    salq    %cl, %rsi
    xorq    %rsi, (%rdi,%rax,8)
    ret
    .cfi_endproc
.LFE25:
    .size   KeccakP1600_AddByte, .-KeccakP1600_AddByte
    .p2align 4,,15
    .globl  KeccakP1600_AddBytes
    .type   KeccakP1600_AddBytes, @function
KeccakP1600_AddBytes:
.LFB26:
    .cfi_startproc
    pushq   %r15
    .cfi_def_cfa_offset 16
    .cfi_offset 15, -16
    movq    %rdi, %r8
    pushq   %r14
    .cfi_def_cfa_offset 24
    .cfi_offset 14, -24
    pushq   %r13
    .cfi_def_cfa_offset 32
    .cfi_offset 13, -32
    movq    %rsi, %r13
    pushq   %r12
    .cfi_def_cfa_offset 40
    .cfi_offset 12, -40
    pushq   %rbp
    .cfi_def_cfa_offset 48
    .cfi_offset 6, -48
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $40, %rsp
    .cfi_def_cfa_offset 96
    testl   %edx, %edx
    je  .L75
    movl    %edx, %r14d
    movl    %edx, %r12d
    shrl    $3, %r14d
    andl    $7, %r12d
    testl   %ecx, %ecx
    je  .L60
    movl    %ecx, %ebx
.L69:
    movl    $8, %eax
    movl    %ebx, %ebp
    subl    %r12d, %eax
    cmpl    %ebx, %eax
    cmovbe  %eax, %ebp
    cmpl    $1, %ebp
    jne .L67
    movzbl  0(%r13), %edx
    movl    $1, %r15d
.L68:
    leal    0(,%r12,8), %ecx
    movq    %rdx, %r12
    movl    %r14d, %eax
    salq    %cl, %r12
    addl    $1, %r14d
    addq    %r15, %r13
    xorq    %r12, (%r8,%rax,8)
    xorl    %r12d, %r12d
    subl    %ebp, %ebx
    jne .L69
.L60:
    addq    $40, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
    .p2align 4,,10
    .p2align 3
.L67:
    .cfi_restore_state
    movl    %ebp, %r15d
    movq    %r13, %rsi
    movq    %r8, (%rsp)
    leaq    16(%rsp), %rdi
    movq    %r15, %rdx
    movq    $0, 16(%rsp)
    call    memcpy
    movq    16(%rsp), %rdx
    movq    (%rsp), %r8
    jmp .L68
    .p2align 4,,10
    .p2align 3
.L75:
    movl    %ecx, %ebp
    movl    %ecx, 8(%rsp)
    shrl    $3, %ebp
    movq    %r8, (%rsp)
    movl    %ebp, %edx
    call    KeccakP1600_AddLanes
    movl    8(%rsp), %ecx
    movq    (%rsp), %r8
    movl    %ecx, %ebx
    andl    $7, %ebx
    je  .L60
    leal    0(,%rbp,8), %esi
    addq    %r13, %rsi
    cmpl    $1, %ebx
    jne .L64
    movzbl  (%rsi), %eax
.L65:
    xorq    %rax, (%r8,%rbp,8)
    addq    $40, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
.L64:
    .cfi_restore_state
    leaq    16(%rsp), %rdi
    movl    %ebx, %edx
    movq    %r8, (%rsp)
    movq    $0, 16(%rsp)
    call    memcpy
    movq    16(%rsp), %rax
    movq    (%rsp), %r8
    jmp .L65
    .cfi_endproc
.LFE26:
    .size   KeccakP1600_AddBytes, .-KeccakP1600_AddBytes
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteBytesInLane
    .type   KeccakP1600_OverwriteBytesInLane, @function
KeccakP1600_OverwriteBytesInLane:
.LFB27:
    .cfi_startproc
    leal    -1(%rsi), %eax
    movq    %rdx, %r9
    cmpl    $1, %eax
    jbe .L81
    cmpl    $8, %esi
    jne .L77
.L81:
    testl   %r8d, %r8d
    je  .L90
    leal    (%rcx,%rsi,8), %eax
    movq    %r9, %rdx
    addl    %eax, %r8d
    .p2align 4,,10
    .p2align 3
.L83:
    movzbl  (%rdx), %ecx
    movl    %eax, %esi
    addl    $1, %eax
    addq    $1, %rdx
    cmpl    %r8d, %eax
    notl    %ecx
    movb    %cl, (%rdi,%rsi)
    jne .L83
    rep
    ret
    .p2align 4,,10
    .p2align 3
.L77:
    cmpl    $17, %esi
    je  .L81
    cmpl    $12, %esi
    .p2align 4,,5
    je  .L81
    cmpl    $20, %esi
    .p2align 4,,2
    je  .L81
    leal    0(,%rsi,8), %eax
    movl    %ecx, %ecx
    movl    %r8d, %edx
    addq    %rcx, %rax
    movq    %r9, %rsi
    addq    %rax, %rdi
    jmp memcpy
.L90:
    rep
    ret
    .cfi_endproc
.LFE27:
    .size   KeccakP1600_OverwriteBytesInLane, .-KeccakP1600_OverwriteBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteLanes
    .type   KeccakP1600_OverwriteLanes, @function
KeccakP1600_OverwriteLanes:
.LFB28:
    .cfi_startproc
    xorl    %eax, %eax
    testl   %edx, %edx
    jne .L98
    jmp .L91
    .p2align 4,,10
    .p2align 3
.L100:
    cmpl    $8, %eax
    je  .L93
    cmpl    $17, %eax
    .p2align 4,,5
    je  .L93
    cmpl    $12, %eax
    .p2align 4,,2
    je  .L93
    cmpl    $20, %eax
    .p2align 4,,2
    je  .L93
    movq    (%rsi,%rax,8), %rcx
    movq    %rcx, (%rdi,%rax,8)
    addq    $1, %rax
    cmpl    %eax, %edx
    jbe .L91
    .p2align 4,,10
    .p2align 3
.L98:
    leal    -1(%rax), %r8d
    cmpl    $1, %r8d
    ja  .L100
.L93:
    movq    (%rsi,%rax,8), %rcx
    notq    %rcx
    movq    %rcx, (%rdi,%rax,8)
    addq    $1, %rax
    cmpl    %eax, %edx
    ja  .L98
.L91:
    rep
    ret
    .cfi_endproc
.LFE28:
    .size   KeccakP1600_OverwriteLanes, .-KeccakP1600_OverwriteLanes
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteBytes
    .type   KeccakP1600_OverwriteBytes, @function
KeccakP1600_OverwriteBytes:
.LFB29:
    .cfi_startproc
    pushq   %r15
    .cfi_def_cfa_offset 16
    .cfi_offset 15, -16
    pushq   %r14
    .cfi_def_cfa_offset 24
    .cfi_offset 14, -24
    pushq   %r13
    .cfi_def_cfa_offset 32
    .cfi_offset 13, -32
    pushq   %r12
    .cfi_def_cfa_offset 40
    .cfi_offset 12, -40
    movq    %rdi, %r12
    pushq   %rbp
    .cfi_def_cfa_offset 48
    .cfi_offset 6, -48
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $24, %rsp
    .cfi_def_cfa_offset 80
    testl   %edx, %edx
    je  .L134
    movl    %edx, %r14d
    movl    %edx, %eax
    shrl    $3, %r14d
    andl    $7, %eax
    testl   %ecx, %ecx
    je  .L101
    leal    0(,%r14,8), %r15d
    movl    %ecx, %r13d
    movq    %rsi, %rbp
    movl    $8, %ecx
    jmp .L120
    .p2align 4,,10
    .p2align 3
.L135:
    cmpl    $8, %r14d
    je  .L117
    cmpl    $17, %r14d
    je  .L117
    cmpl    $12, %r14d
    je  .L117
    cmpl    $20, %r14d
    je  .L117
    movl    %r15d, %edi
    movl    %ebx, %r8d
    movq    %rbp, %rsi
    addq    %rax, %rdi
    movq    %r8, %rdx
    movl    %ecx, (%rsp)
    addq    %r12, %rdi
    movq    %r8, 8(%rsp)
    call    memcpy
    movq    8(%rsp), %r8
    movl    (%rsp), %ecx
    .p2align 4,,10
    .p2align 3
.L119:
    addl    $1, %r14d
    addq    %r8, %rbp
    addl    $8, %r15d
    xorl    %eax, %eax
    subl    %ebx, %r13d
    je  .L101
.L120:
    leal    -1(%r14), %edx
    movl    %ecx, %ebx
    subl    %eax, %ebx
    cmpl    %r13d, %ebx
    cmova   %r13d, %ebx
    cmpl    $1, %edx
    ja  .L135
.L117:
    leal    (%rax,%r15), %esi
    xorl    %eax, %eax
    .p2align 4,,10
    .p2align 3
.L116:
    movzbl  0(%rbp,%rax), %edx
    leal    (%rsi,%rax), %r8d
    addq    $1, %rax
    cmpl    %eax, %ebx
    notl    %edx
    movb    %dl, (%r12,%r8)
    ja  .L116
    movl    %ebx, %r8d
    jmp .L119
.L138:
    leal    0(,%r8,8), %eax
    movl    %ecx, %r13d
    leal    -1(%r8), %edi
    movl    %eax, %ecx
    andl    $7, %r13d
    addq    %rcx, %rsi
    cmpl    $1, %edi
    movq    %rcx, %rdx
    ja  .L136
.L108:
    testl   %r13d, %r13d
    movq    %rsi, %rbp
    leal    0(%r13,%rax), %esi
    jne .L129
    jmp .L101
    .p2align 4,,10
    .p2align 3
.L137:
    movl    %eax, %ecx
.L129:
    movzbl  0(%rbp), %edx
    addl    $1, %eax
    addq    $1, %rbp
    cmpl    %esi, %eax
    notl    %edx
    movb    %dl, (%r12,%rcx)
    jne .L137
    .p2align 4,,10
    .p2align 3
.L101:
    addq    $24, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
.L134:
    .cfi_restore_state
    movl    %ecx, %r8d
    shrl    $3, %r8d
    testl   %r8d, %r8d
    je  .L103
    xorl    %eax, %eax
    jmp .L107
    .p2align 4,,10
    .p2align 3
.L139:
    cmpl    $8, %eax
    je  .L104
    cmpl    $17, %eax
    .p2align 4,,3
    je  .L104
    cmpl    $12, %eax
    .p2align 4,,2
    je  .L104
    cmpl    $20, %eax
    .p2align 4,,2
    je  .L104
    movq    (%rsi,%rax,8), %rdx
    movq    %rdx, (%r12,%rax,8)
    .p2align 4,,10
    .p2align 3
.L106:
    addq    $1, %rax
    cmpl    %eax, %r8d
    jbe .L138
.L107:
    leal    -1(%rax), %edi
    cmpl    $1, %edi
    ja  .L139
.L104:
    movq    (%rsi,%rax,8), %rdx
    notq    %rdx
    movq    %rdx, (%r12,%rax,8)
    jmp .L106
.L136:
    cmpl    $8, %r8d
    je  .L108
    cmpl    $17, %r8d
    je  .L108
    cmpl    $12, %r8d
    je  .L108
    cmpl    $20, %r8d
    je  .L108
.L109:
    leaq    (%r12,%rdx), %rdi
    addq    $24, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 56
    movl    %r13d, %edx
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    jmp memcpy
.L103:
    .cfi_restore_state
    movl    %ecx, %r13d
    xorl    %edx, %edx
    andl    $7, %r13d
    jmp .L109
    .cfi_endproc
.LFE29:
    .size   KeccakP1600_OverwriteBytes, .-KeccakP1600_OverwriteBytes
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteWithZeroes
    .type   KeccakP1600_OverwriteWithZeroes, @function
KeccakP1600_OverwriteWithZeroes:
.LFB30:
    .cfi_startproc
    movl    %esi, %r8d
    xorl    %eax, %eax
    movq    %rdi, %rdx
    shrl    $3, %r8d
    testl   %r8d, %r8d
    jne .L151
    jmp .L148
    .p2align 4,,10
    .p2align 3
.L154:
    cmpl    $8, %eax
    je  .L144
    cmpl    $17, %eax
    .p2align 4,,5
    je  .L144
    cmpl    $12, %eax
    .p2align 4,,2
    je  .L144
    cmpl    $20, %eax
    .p2align 4,,2
    je  .L144
    addl    $1, %eax
    movq    $0, (%rdx)
    addq    $8, %rdx
    cmpl    %r8d, %eax
    je  .L148
    .p2align 4,,10
    .p2align 3
.L151:
    leal    -1(%rax), %ecx
    cmpl    $1, %ecx
    ja  .L154
.L144:
    addl    $1, %eax
    movq    $-1, (%rdx)
    addq    $8, %rdx
    cmpl    %r8d, %eax
    jne .L151
.L148:
    andl    $7, %esi
    je  .L155
    leal    -1(%r8), %eax
    cmpl    $1, %eax
    jbe .L149
    cmpl    $8, %r8d
    je  .L149
    cmpl    $17, %r8d
    je  .L149
    cmpl    $12, %r8d
    je  .L149
    cmpl    $20, %r8d
    je  .L149
    sall    $3, %r8d
    movl    %esi, %edx
    xorl    %esi, %esi
    addq    %r8, %rdi
    jmp memset
    .p2align 4,,10
    .p2align 3
.L149:
    sall    $3, %r8d
    movl    %esi, %edx
    movl    $255, %esi
    addq    %r8, %rdi
    jmp memset
    .p2align 4,,10
    .p2align 3
.L155:
    rep
    ret
    .cfi_endproc
.LFE30:
    .size   KeccakP1600_OverwriteWithZeroes, .-KeccakP1600_OverwriteWithZeroes
    .p2align 4,,15
    .globl  KeccakP1600_Permute_24rounds
    .type   KeccakP1600_Permute_24rounds, @function
KeccakP1600_Permute_24rounds:
.LFB31:
    .cfi_startproc
    pushq   %r15
    .cfi_def_cfa_offset 16
    .cfi_offset 15, -16
    pushq   %r14
    .cfi_def_cfa_offset 24
    .cfi_offset 14, -24
    pushq   %r13
    .cfi_def_cfa_offset 32
    .cfi_offset 13, -32
    pushq   %r12
    .cfi_def_cfa_offset 40
    .cfi_offset 12, -40
    pushq   %rbp
    .cfi_def_cfa_offset 48
    .cfi_offset 6, -48
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $240, %rsp
    .cfi_def_cfa_offset 296
    movq    %rdi, 216(%rsp)
    movq    (%rdi), %rbx
    movq    8(%rdi), %rax
    movq    16(%rdi), %rdx
    movq    24(%rdi), %rcx
    movq    32(%rdi), %rsi
    movq    40(%rdi), %rdi
    movq    %rax, 16(%rsp)
    movq    %rdx, 8(%rsp)
    movq    %rsi, (%rsp)
    movq    %rdi, -8(%rsp)
    movq    216(%rsp), %rdi
    movq    80(%rdi), %r10
    movq    56(%rdi), %rbp
    movq    64(%rdi), %r8
    movq    72(%rdi), %r9
    movq    88(%rdi), %r11
    movq    %r10, -40(%rsp)
    movq    104(%rdi), %r12
    movq    %rbp, -16(%rsp)
    movq    112(%rdi), %r14
    movq    %r8, -24(%rsp)
    movq    48(%rdi), %r13
    movq    %r9, -32(%rsp)
    movq    96(%rdi), %rbp
    movq    %r11, -48(%rsp)
    movq    %r12, -56(%rsp)
    movq    %r14, -64(%rsp)
    movq    128(%rdi), %rax
    movq    152(%rdi), %rsi
    movq    120(%rdi), %r15
    movq    136(%rdi), %rdx
    movq    144(%rdi), %r11
    movq    %rax, -80(%rsp)
    movq    160(%rdi), %rdi
    movq    %rsi, -96(%rsp)
    movq    16(%rsp), %r14
    movq    %r15, -72(%rsp)
    movq    %rdx, -88(%rsp)
    movq    -16(%rsp), %rdx
    xorq    (%rsp), %r9
    movq    %rdi, -104(%rsp)
    movq    216(%rsp), %rdi
    xorq    %r13, %r14
    xorq    -48(%rsp), %r14
    xorq    8(%rsp), %rdx
    movq    168(%rdi), %rdi
    xorq    -64(%rsp), %r9
    xorq    %rax, %r14
    movq    -24(%rsp), %rax
    xorq    %rbp, %rdx
    movq    %rdi, -112(%rsp)
    movq    216(%rsp), %rdi
    xorq    -88(%rsp), %rdx
    xorq    %rsi, %r9
    xorq    %rcx, %rax
    xorq    -112(%rsp), %r14
    xorq    -56(%rsp), %rax
    movq    184(%rdi), %r8
    movq    176(%rdi), %r15
    movq    192(%rdi), %r12
    movq    %r14, %rdi
    xorq    %r11, %rax
    rolq    %rdi
    movq    %r8, -120(%rsp)
    movq    -8(%rsp), %r8
    xorq    -120(%rsp), %rax
    xorq    %r15, %rdx
    movq    %rdx, %rsi
    xorq    %r12, %r9
    rolq    %rsi
    xorq    %r9, %rdi
    rolq    %r9
    xorq    %rbx, %r8
    xorq    %rdx, %r9
    xorq    %rdi, %rbx
    xorq    %r10, %r8
    movq    %rax, %r10
    xorq    -72(%rsp), %r8
    rolq    %r10
    xorq    %r14, %r10
    xorq    -104(%rsp), %r8
    xorq    %r8, %rsi
    rolq    %r8
    movq    %rsi, %rdx
    xorq    %rax, %r8
    xorq    %r13, %rdx
    movq    %r8, %rax
    rolq    $44, %rdx
    xorq    %r10, %rbp
    xorq    %r12, %rax
    rolq    $43, %rbp
    rolq    $14, %rax
    xorq    %r9, %r11
    movq    %rbx, %r12
    movq    %rbp, %r13
    rolq    $21, %r11
    xorq    $1, %r12
    movq    %rax, %r14
    notq    %r13
    movq    %r12, 112(%rsp)
    orq %r11, %r13
    andq    %r11, %r14
    movq    %rbp, %r12
    xorq    %rdx, %r13
    xorq    %rbp, %r14
    orq %rdx, %r12
    movq    %rax, %rbp
    andq    %rbx, %rdx
    xorq    %rax, %rdx
    orq %rbx, %rbp
    movq    -80(%rsp), %rax
    movq    -40(%rsp), %rbx
    xorq    %r11, %rbp
    movq    %rdx, 40(%rsp)
    movq    -32(%rsp), %r11
    movq    %r10, %rdx
    movq    %r14, 56(%rsp)
    xorq    %r15, %rdx
    movq    %r13, 24(%rsp)
    xorq    %r9, %rcx
    rolq    $61, %rdx
    xorq    %rsi, %rax
    rolq    $28, %rcx
    xorq    %rdi, %rbx
    rolq    $45, %rax
    movq    %rdx, %r14
    rolq    $3, %rbx
    xorq    %r8, %r11
    movq    %rax, %r13
    xorq    %r12, 112(%rsp)
    rolq    $20, %r11
    movq    %rbx, %r12
    notq    %r14
    orq %r11, %r12
    movq    %rbp, 32(%rsp)
    andq    %rbx, %r13
    orq %rax, %r14
    movq    %rdx, %r15
    xorq    %rcx, %r12
    xorq    %r11, %r13
    xorq    %rbx, %r14
    orq %rcx, %r15
    movq    %r12, 88(%rsp)
    andq    %rcx, %r11
    xorq    %rax, %r15
    movq    -16(%rsp), %rbx
    xorq    %rdx, %r11
    movq    %r15, %rax
    movq    -96(%rsp), %r12
    movq    %r11, 128(%rsp)
    xorq    %rbp, %rax
    movq    -56(%rsp), %rbp
    movq    %r11, %rcx
    movq    16(%rsp), %rdx
    movq    %r13, 72(%rsp)
    movq    -104(%rsp), %r11
    xorq    %r10, %rbx
    movq    %r14, 48(%rsp)
    xorq    %r8, %r12
    rolq    $6, %rbx
    xorq    40(%rsp), %rcx
    xorq    %r9, %rbp
    rolq    $8, %r12
    movq    %r15, 80(%rsp)
    rolq    $25, %rbp
    xorq    %rsi, %rdx
    movq    %r12, %r14
    movq    %rbp, %r13
    rolq    %rdx
    xorq    %rdi, %r11
    orq %rbx, %r13
    andq    %rbp, %r14
    notq    %r12
    xorq    %rdx, %r13
    rolq    $18, %r11
    xorq    %rbx, %r14
    movq    %r13, 96(%rsp)
    andq    %rdx, %rbx
    movq    %r12, %r13
    xorq    %r11, %rbx
    andq    %r11, %r13
    movq    %r11, %r15
    xorq    %rbp, %r13
    orq %rdx, %r15
    movq    %rbx, 120(%rsp)
    xorq    %rbx, %rcx
    movq    -8(%rsp), %rbp
    xorq    %r12, %r15
    movq    (%rsp), %rbx
    xorq    %r15, %rax
    movq    %r14, 64(%rsp)
    movq    -48(%rsp), %r12
    movq    %r15, 104(%rsp)
    movq    -88(%rsp), %r14
    xorq    %rdi, %rbp
    movq    -120(%rsp), %r11
    xorq    %r8, %rbx
    rolq    $36, %rbp
    rolq    $27, %rbx
    xorq    %rsi, %r12
    rolq    $10, %r12
    xorq    %r10, %r14
    movq    %r12, %rdx
    rolq    $15, %r14
    xorq    %r9, %r11
    andq    %rbp, %rdx
    movq    %r14, %r15
    notq    %r14
    xorq    %rbx, %rdx
    rolq    $56, %r11
    orq %r12, %r15
    movq    %rdx, 136(%rsp)
    movq    88(%rsp), %rdx
    xorq    %rbp, %r15
    orq %rbx, %rbp
    xorq    %r11, %rbp
    xorq    %rbp, %rcx
    xorq    112(%rsp), %rdx
    xorq    96(%rsp), %rdx
    xorq    136(%rsp), %rdx
    movq    %r14, 144(%rsp)
    orq %r11, %r14
    xorq    -24(%rsp), %r9
    xorq    %r12, %r14
    movq    %r11, %r12
    xorq    -64(%rsp), %r8
    andq    %rbx, %r12
    movq    %r14, 184(%rsp)
    xorq    8(%rsp), %r10
    xorq    -72(%rsp), %rdi
    rolq    $55, %r9
    xorq    144(%rsp), %r12
    movq    %rbp, 144(%rsp)
    movq    %r9, %r11
    xorq    -112(%rsp), %rsi
    rolq    $39, %r8
    notq    %r11
    rolq    $62, %r10
    movq    %r11, %r14
    rolq    $41, %rdi
    andq    %r8, %r14
    movq    %rdi, %rbx
    xorq    %r12, %rax
    xorq    %r10, %r14
    rolq    $2, %rsi
    xorq    %r14, %rdx
    orq %r8, %rbx
    movq    %rsi, %rbp
    xorq    %r11, %rbx
    movq    64(%rsp), %r11
    orq %r10, %rbp
    movq    %rbx, 152(%rsp)
    andq    %r9, %r10
    xorq    %rdi, %rbp
    xorq    %rsi, %r10
    xorq    %rbp, %rax
    xorq    %r10, %rcx
    movq    %rax, %r9
    xorq    72(%rsp), %r11
    rolq    %r9
    xorq    %r15, %r11
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    andq    %rdi, %rbx
    xorq    24(%rsp), %r11
    xorq    %r8, %rbx
    movq    56(%rsp), %r8
    movq    %r11, %rdi
    xorq    %r11, %r9
    xorq    %rbx, %r8
    rolq    %rdi
    xorq    48(%rsp), %r8
    xorq    %rcx, %rdi
    rolq    %rcx
    xorq    %r13, %r8
    xorq    %r9, %r13
    xorq    184(%rsp), %r8
    movq    %r8, %rsi
    xorq    %r8, %rcx
    movq    112(%rsp), %r8
    rolq    %rsi
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    72(%rsp), %rax
    xorq    %rdi, %r8
    movq    %r8, %r11
    xorq    %rsi, %rax
    rolq    $44, %rax
    rolq    $43, %r13
    xorq    $32898, %r11
    movq    %r11, 72(%rsp)
    movq    %r13, %r11
    xorq    %rcx, %r12
    orq %rax, %r11
    rolq    $21, %r12
    xorq    %rdx, %r10
    xorq    %r11, 72(%rsp)
    movq    %r13, %r11
    rolq    $14, %r10
    notq    %r11
    xorq    %r9, %rbx
    orq %r12, %r11
    rolq    $61, %rbx
    xorq    %rsi, %r15
    xorq    %rax, %r11
    andq    %r8, %rax
    rolq    $45, %r15
    movq    %r11, 112(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    andq    %r12, %r11
    movq    %rax, 176(%rsp)
    movq    32(%rsp), %rax
    xorq    %r13, %r11
    movq    %r10, %r13
    orq %r8, %r13
    movq    96(%rsp), %r8
    movq    %r11, 192(%rsp)
    movq    128(%rsp), %r11
    xorq    %r12, %r13
    movq    %r15, %r12
    movq    %r13, 168(%rsp)
    movq    %rbx, %r13
    xorq    %rcx, %rax
    notq    %r13
    rolq    $28, %rax
    xorq    %rdi, %r8
    orq %r15, %r13
    rolq    $3, %r8
    xorq    %rdx, %r11
    rolq    $20, %r11
    xorq    %r8, %r13
    andq    %r8, %r12
    movq    %r8, %r10
    movq    %rbx, %r8
    xorq    %r11, %r12
    orq %r11, %r10
    orq %rax, %r8
    movq    %r12, 96(%rsp)
    xorq    %rax, %r10
    xorq    %r15, %r8
    andq    %rax, %r11
    xorq    %rbx, %r11
    movq    104(%rsp), %rax
    movq    %r10, 128(%rsp)
    movq    144(%rsp), %rbx
    xorq    %rdi, %r14
    movq    %r13, 32(%rsp)
    movq    48(%rsp), %r10
    movq    %r8, 160(%rsp)
    rolq    $18, %r14
    movq    24(%rsp), %r8
    movq    %r11, 200(%rsp)
    movq    %r14, %r13
    xorq    %rcx, %rax
    xorq    %rdx, %rbx
    rolq    $25, %rax
    xorq    %r9, %r10
    rolq    $8, %rbx
    movq    %rax, %r11
    rolq    $6, %r10
    xorq    %rsi, %r8
    movq    %rbx, %r12
    rolq    %r8
    orq %r10, %r11
    andq    %rax, %r12
    xorq    %r8, %r11
    xorq    %r10, %r12
    notq    %rbx
    movq    %r11, 104(%rsp)
    orq %r8, %r13
    andq    %r8, %r10
    movq    %r12, 144(%rsp)
    movq    %rbx, %r12
    xorq    %rbx, %r13
    andq    %r14, %r12
    xorq    %r14, %r10
    movq    64(%rsp), %rbx
    xorq    %rax, %r12
    movq    %r10, 224(%rsp)
    movq    88(%rsp), %r10
    movq    160(%rsp), %rax
    movq    %r13, 208(%rsp)
    movq    40(%rsp), %r8
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    xorq    %rdi, %r10
    xorq    168(%rsp), %rax
    rolq    $36, %r10
    movq    %rbx, %r14
    xorq    %rdx, %r8
    rolq    $27, %r8
    xorq    %r13, %rax
    movq    184(%rsp), %r13
    xorq    %r9, %r13
    rolq    $15, %r13
    xorq    %rcx, %rbp
    andq    %r10, %r14
    xorq    %r8, %r14
    rolq    $56, %rbp
    movq    %r13, %r11
    movq    %r14, 40(%rsp)
    movq    %r13, %r14
    orq %rbx, %r11
    notq    %r14
    movq    %rbp, %r13
    xorq    80(%rsp), %rcx
    movq    %r14, %r15
    xorq    136(%rsp), %rdi
    andq    %r8, %r13
    orq %rbp, %r15
    xorq    120(%rsp), %rdx
    xorq    %r10, %r11
    xorq    %rbx, %r15
    movq    %r10, %rbx
    xorq    56(%rsp), %r9
    orq %r8, %rbx
    movq    128(%rsp), %r8
    rolq    $55, %rcx
    rolq    $41, %rdi
    xorq    %rbp, %rbx
    movq    %rcx, %r10
    rolq    $39, %rdx
    movq    %rdi, %rbp
    notq    %r10
    orq %rdx, %rbp
    xorq    %r14, %r13
    movq    %r10, %r14
    xorq    72(%rsp), %r8
    xorq    %r10, %rbp
    rolq    $62, %r9
    movq    144(%rsp), %r10
    andq    %rdx, %r14
    xorq    %r13, %rax
    xorq    152(%rsp), %rsi
    xorq    %r9, %r14
    movq    %rbp, 80(%rsp)
    movq    %r15, 88(%rsp)
    xorq    104(%rsp), %r8
    xorq    96(%rsp), %r10
    rolq    $2, %rsi
    movq    %rsi, %r15
    xorq    40(%rsp), %r8
    xorq    %r11, %r10
    xorq    %r14, %r8
    xorq    %rbp, %r10
    movq    %rsi, %rbp
    orq %r9, %rbp
    andq    %r9, %rcx
    movq    200(%rsp), %r9
    andq    %rdi, %r15
    xorq    112(%rsp), %r10
    xorq    %rdi, %rbp
    xorq    %rdx, %r15
    movq    192(%rsp), %rdx
    xorq    %rbp, %rax
    xorq    %rsi, %rcx
    movq    %rbp, 64(%rsp)
    movq    %rax, %rbp
    xorq    176(%rsp), %r9
    rolq    %rbp
    xorq    %r10, %rbp
    movq    %r10, %rdi
    xorq    %r15, %rdx
    rolq    %rdi
    xorq    32(%rsp), %rdx
    xorq    224(%rsp), %r9
    xorq    %r12, %rdx
    xorq    %rbp, %r12
    xorq    88(%rsp), %rdx
    xorq    %rbx, %r9
    rolq    $43, %r12
    xorq    %rcx, %r9
    movq    %r9, %r10
    xorq    %r9, %rdi
    movq    72(%rsp), %r9
    rolq    %r10
    xorq    %rdx, %r10
    movq    %rdx, %rsi
    movq    %r8, %rdx
    rolq    %rdx
    rolq    %rsi
    xorq    %r10, %r13
    xorq    %rax, %rdx
    movq    96(%rsp), %rax
    xorq    %r8, %rsi
    movabsq $-9223372036854742902, %r8
    xorq    %rdi, %r9
    xorq    %rsi, %rax
    rolq    $44, %rax
    rolq    $21, %r13
    xorq    %r9, %r8
    movq    %r8, 24(%rsp)
    movq    %rax, %r8
    xorq    %rdx, %rcx
    orq %r12, %r8
    rolq    $14, %rcx
    xorq    %rbp, %r15
    xorq    %r8, 24(%rsp)
    movq    %r12, %r8
    rolq    $61, %r15
    notq    %r8
    xorq    %rsi, %r11
    orq %r13, %r8
    rolq    $45, %r11
    xorq    %rax, %r8
    andq    %r9, %rax
    movq    %r8, 48(%rsp)
    movq    %rcx, %r8
    xorq    %rcx, %rax
    andq    %r13, %r8
    movq    %rax, 120(%rsp)
    movq    200(%rsp), %rax
    xorq    %r12, %r8
    movq    %rcx, %r12
    movq    168(%rsp), %rcx
    movq    %r8, 72(%rsp)
    movq    104(%rsp), %r8
    orq %r9, %r12
    xorq    %r13, %r12
    movq    %r11, %r9
    xorq    %rdx, %rax
    movq    %r12, 96(%rsp)
    movq    %r15, %r12
    rolq    $20, %rax
    xorq    %r10, %rcx
    notq    %r12
    xorq    %rdi, %r8
    rolq    $28, %rcx
    orq %r11, %r12
    rolq    $3, %r8
    movq    %r8, %r13
    xorq    %r8, %r12
    andq    %r8, %r9
    orq %rax, %r13
    movq    112(%rsp), %r8
    xorq    %rax, %r9
    xorq    %rcx, %r13
    andq    %rcx, %rax
    movq    %r9, 136(%rsp)
    movq    %r13, 104(%rsp)
    movq    %r15, %r13
    movq    %rdx, %r9
    orq %rcx, %r13
    movq    208(%rsp), %rcx
    movq    %r12, 56(%rsp)
    xorq    %r11, %r13
    xorq    %r15, %rax
    xorq    %rdi, %r14
    movq    %rax, 152(%rsp)
    movq    32(%rsp), %rax
    xorq    %rsi, %r8
    rolq    $18, %r14
    xorq    %rbx, %r9
    rolq    %r8
    rolq    $8, %r9
    movq    %r14, %r11
    xorq    %r10, %rcx
    orq %r8, %r11
    movq    %r9, %rbx
    notq    %r9
    rolq    $25, %rcx
    xorq    %r9, %r11
    xorq    %rbp, %rax
    rolq    $6, %rax
    andq    %rcx, %rbx
    movq    %r11, 200(%rsp)
    movq    %rcx, %r15
    movq    144(%rsp), %r11
    xorq    %rax, %rbx
    movq    %r13, 184(%rsp)
    orq %rax, %r15
    movq    128(%rsp), %r13
    xorq    %r8, %r15
    movq    %rbx, 168(%rsp)
    andq    %rax, %r8
    movq    %r9, %rbx
    movq    176(%rsp), %rax
    xorq    %r14, %r8
    andq    %r14, %rbx
    movq    64(%rsp), %r9
    movq    %r8, 208(%rsp)
    movq    88(%rsp), %r14
    xorq    %rsi, %r11
    xorq    %rdi, %r13
    movq    24(%rsp), %r8
    rolq    $10, %r11
    rolq    $36, %r13
    xorq    %rdx, %rax
    movq    %r11, %r12
    xorq    %rcx, %rbx
    rolq    $27, %rax
    xorq    %r10, %r9
    andq    %r13, %r12
    xorq    %rbp, %r14
    rolq    $56, %r9
    xorq    %rax, %r12
    rolq    $15, %r14
    xorq    104(%rsp), %r8
    movq    %r11, %rcx
    orq %r14, %rcx
    notq    %r14
    xorq    160(%rsp), %r10
    xorq    40(%rsp), %rdi
    movq    %r15, 32(%rsp)
    xorq    %r13, %rcx
    movq    %r12, 88(%rsp)
    xorq    224(%rsp), %rdx
    xorq    %r15, %r8
    movq    %r14, %r15
    xorq    80(%rsp), %rsi
    xorq    %r12, %r8
    movq    48(%rsp), %r12
    orq %r9, %r15
    xorq    %r11, %r15
    rolq    $41, %rdi
    movq    %r9, %r11
    movq    %r15, 128(%rsp)
    movq    %r13, %r15
    rolq    $55, %r10
    orq %rax, %r15
    rolq    $39, %rdx
    andq    %rax, %r11
    xorq    136(%rsp), %r12
    rolq    $2, %rsi
    xorq    %r9, %r15
    movq    %r10, %rax
    movq    %rdi, %r9
    xorq    192(%rsp), %rbp
    notq    %rax
    orq %rdx, %r9
    movq    %rsi, %r13
    xorq    %rax, %r9
    xorq    %r14, %r11
    andq    %rdi, %r13
    movq    %rax, %r14
    xorq    168(%rsp), %r12
    xorq    %rdx, %r13
    movq    %r9, 40(%rsp)
    andq    %rdx, %r14
    movq    56(%rsp), %rdx
    rolq    $62, %rbp
    movq    %rsi, %rax
    xorq    %rbp, %r14
    xorq    %rcx, %r12
    xorq    %r14, %r8
    xorq    %r9, %r12
    movq    152(%rsp), %r9
    xorq    %rbx, %rdx
    xorq    72(%rsp), %rdx
    orq %rbp, %rax
    andq    %r10, %rbp
    xorq    %rdi, %rax
    xorq    %rsi, %rbp
    movq    %rax, 80(%rsp)
    movq    184(%rsp), %rax
    xorq    %rbp, %r9
    xorq    120(%rsp), %r9
    movq    %r12, %rdi
    xorq    128(%rsp), %rdx
    rolq    %rdi
    xorq    96(%rsp), %rax
    xorq    208(%rsp), %r9
    xorq    %r13, %rdx
    movq    %rdx, %rsi
    xorq    %r11, %rax
    rolq    %rsi
    xorq    200(%rsp), %rax
    xorq    %r15, %r9
    xorq    %r8, %rsi
    xorq    %r9, %rdi
    rolq    %r8
    rolq    %r9
    xorq    %rdx, %r9
    movq    24(%rsp), %rdx
    xorq    80(%rsp), %rax
    xorq    %r9, %r11
    rolq    $21, %r11
    xorq    %rdi, %rdx
    xorq    %rax, %r8
    movq    %rax, %r10
    movq    136(%rsp), %rax
    rolq    %r10
    xorq    %r8, %rbp
    xorq    %r12, %r10
    movabsq $-9223372034707259392, %r12
    xorq    %r10, %rbx
    xorq    %rsi, %rax
    rolq    $43, %rbx
    rolq    $44, %rax
    rolq    $14, %rbp
    xorq    %rdx, %r12
    movq    %r12, 24(%rsp)
    movq    %rbx, %r12
    xorq    %r10, %r13
    orq %rax, %r12
    rolq    $61, %r13
    xorq    %rsi, %rcx
    xorq    %r12, 24(%rsp)
    movq    %rbx, %r12
    rolq    $45, %rcx
    notq    %r12
    orq %r11, %r12
    xorq    %rax, %r12
    andq    %rdx, %rax
    movq    %r12, 112(%rsp)
    movq    %rbp, %r12
    xorq    %rbp, %rax
    andq    %r11, %r12
    movq    %rax, 144(%rsp)
    movq    32(%rsp), %rax
    xorq    %rbx, %r12
    movq    %rbp, %rbx
    orq %rdx, %rbx
    movq    96(%rsp), %rdx
    movq    %r12, 64(%rsp)
    xorq    %r11, %rbx
    movq    152(%rsp), %r11
    movq    %rcx, %r12
    xorq    %rdi, %rax
    movq    %rbx, 136(%rsp)
    movq    %r13, %rbx
    rolq    $3, %rax
    notq    %rbx
    xorq    %r9, %rdx
    movq    %rax, %rbp
    andq    %rax, %r12
    xorq    %r8, %r11
    rolq    $28, %rdx
    orq %rcx, %rbx
    rolq    $20, %r11
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r12
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r13, %r11
    movq    %rbx, 32(%rsp)
    movq    %rbp, 96(%rsp)
    movq    %r13, %rbp
    movq    200(%rsp), %rbx
    orq %rdx, %rbp
    movq    %r11, 176(%rsp)
    movq    48(%rsp), %rdx
    xorq    %rcx, %rbp
    movq    %r11, %rcx
    movq    56(%rsp), %r11
    movq    %rbp, %rax
    movq    %r12, 152(%rsp)
    xorq    136(%rsp), %rax
    movq    %rbp, 192(%rsp)
    xorq    144(%rsp), %rcx
    xorq    %r9, %rbx
    xorq    %r8, %r15
    rolq    $25, %rbx
    xorq    %r10, %r11
    rolq    $8, %r15
    rolq    $6, %r11
    xorq    %rsi, %rdx
    movq    %rbx, %r12
    movq    %r15, %r13
    rolq    %rdx
    orq %r11, %r12
    andq    %rbx, %r13
    xorq    %rdi, %r14
    xorq    %rdx, %r12
    xorq    %r11, %r13
    notq    %r15
    movq    %r12, 48(%rsp)
    rolq    $18, %r14
    andq    %rdx, %r11
    movq    %r13, 56(%rsp)
    movq    %r15, %r13
    movq    168(%rsp), %r12
    andq    %r14, %r13
    xorq    %r14, %r11
    movq    104(%rsp), %rbp
    xorq    %rbx, %r13
    movq    %r14, %rbx
    movq    %r11, 200(%rsp)
    orq %rdx, %rbx
    movq    128(%rsp), %r14
    xorq    %r11, %rcx
    xorq    %r15, %rbx
    movq    80(%rsp), %r11
    xorq    %rsi, %r12
    movq    %rbx, 160(%rsp)
    xorq    %rbx, %rax
    movq    120(%rsp), %rbx
    movq    96(%rsp), %rdx
    rolq    $10, %r12
    xorq    %rdi, %rbp
    rolq    $36, %rbp
    xorq    %r10, %r14
    movq    %r12, %r15
    xorq    %r9, %r11
    rolq    $15, %r14
    xorq    %r8, %rbx
    rolq    $56, %r11
    rolq    $27, %rbx
    andq    %rbp, %r15
    xorq    24(%rsp), %rdx
    xorq    88(%rsp), %rdi
    xorq    %rbx, %r15
    xorq    184(%rsp), %r9
    movq    %r15, 80(%rsp)
    xorq    208(%rsp), %r8
    xorq    48(%rsp), %rdx
    rolq    $41, %rdi
    xorq    40(%rsp), %rsi
    rolq    $55, %r9
    xorq    72(%rsp), %r10
    rolq    $39, %r8
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 104(%rsp)
    orq %r12, %r15
    orq %r11, %r14
    xorq    %rbp, %r15
    xorq    %r12, %r14
    orq %rbx, %rbp
    movq    %r11, %r12
    xorq    %r11, %rbp
    movq    %r9, %r11
    andq    %rbx, %r12
    movq    %rdi, %rbx
    notq    %r11
    orq %r8, %rbx
    movq    %r14, 128(%rsp)
    movq    %r11, %r14
    xorq    %r11, %rbx
    movq    56(%rsp), %r11
    rolq    $2, %rsi
    xorq    104(%rsp), %r12
    rolq    $62, %r10
    andq    %r8, %r14
    xorq    %r10, %r14
    movq    %rbx, 40(%rsp)
    xorq    %rbp, %rcx
    xorq    %r14, %rdx
    movq    %rbp, 104(%rsp)
    movq    %rsi, %rbp
    xorq    152(%rsp), %r11
    xorq    %r12, %rax
    xorq    %r15, %r11
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    xorq    112(%rsp), %r11
    andq    %rdi, %rbx
    orq %r10, %rbp
    xorq    %r8, %rbx
    movq    64(%rsp), %r8
    andq    %r9, %r10
    xorq    %rdi, %rbp
    xorq    %rsi, %r10
    xorq    %r10, %rcx
    xorq    %rbp, %rax
    movq    %r11, %rdi
    movq    %rax, %r9
    rolq    %rdi
    xorq    %rbx, %r8
    rolq    %r9
    xorq    32(%rsp), %r8
    xorq    %rcx, %rdi
    rolq    %rcx
    xorq    %r11, %r9
    xorq    %r13, %r8
    xorq    %r9, %r13
    xorq    128(%rsp), %r8
    rolq    $43, %r13
    movq    %r8, %rsi
    xorq    %r8, %rcx
    movq    24(%rsp), %r8
    rolq    %rsi
    xorq    %rcx, %r12
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    152(%rsp), %rax
    rolq    $21, %r12
    xorq    %rdi, %r8
    xorq    %rdx, %r10
    movq    %r8, %r11
    rolq    $14, %r10
    xorq    $32907, %r11
    xorq    %rsi, %rax
    movq    %r11, 88(%rsp)
    movq    %r13, %r11
    rolq    $44, %rax
    orq %rax, %r11
    xorq    %r9, %rbx
    xorq    %rsi, %r15
    xorq    %r11, 88(%rsp)
    movq    %r13, %r11
    rolq    $61, %rbx
    notq    %r11
    rolq    $45, %r15
    orq %r12, %r11
    xorq    %rax, %r11
    andq    %r8, %rax
    movq    %r11, 24(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    andq    %r12, %r11
    movq    %rax, 184(%rsp)
    movq    136(%rsp), %rax
    xorq    %r13, %r11
    movq    %r10, %r13
    orq %r8, %r13
    movq    48(%rsp), %r8
    movq    %r11, 72(%rsp)
    movq    176(%rsp), %r11
    xorq    %r12, %r13
    movq    %r15, %r12
    movq    %r13, 120(%rsp)
    movq    %rbx, %r13
    xorq    %rcx, %rax
    notq    %r13
    rolq    $28, %rax
    xorq    %rdi, %r8
    orq %r15, %r13
    rolq    $3, %r8
    xorq    %rdx, %r11
    rolq    $20, %r11
    xorq    %r8, %r13
    andq    %r8, %r12
    movq    %r8, %r10
    movq    %rbx, %r8
    xorq    %r11, %r12
    orq %r11, %r10
    orq %rax, %r8
    andq    %rax, %r11
    xorq    %rax, %r10
    xorq    %r15, %r8
    xorq    %rbx, %r11
    movq    %r10, 136(%rsp)
    movq    32(%rsp), %r10
    movq    %r8, 168(%rsp)
    movq    112(%rsp), %r8
    movq    104(%rsp), %rbx
    movq    %r12, 152(%rsp)
    movq    160(%rsp), %rax
    movq    %r11, 176(%rsp)
    xorq    %r9, %r10
    movq    %r13, 48(%rsp)
    xorq    %rsi, %r8
    rolq    %r8
    rolq    $6, %r10
    xorq    %rdx, %rbx
    xorq    %rcx, %rax
    rolq    $8, %rbx
    xorq    %rdi, %r14
    rolq    $25, %rax
    movq    %rbx, %r12
    notq    %rbx
    andq    %rax, %r12
    rolq    $18, %r14
    movq    %rax, %r11
    xorq    %r10, %r12
    movq    %r14, %r13
    orq %r10, %r11
    movq    %r12, 160(%rsp)
    movq    %rbx, %r12
    orq %r8, %r13
    andq    %r14, %r12
    andq    %r8, %r10
    xorq    %rbx, %r13
    xorq    %rax, %r12
    movq    168(%rsp), %rax
    xorq    %r14, %r10
    movq    56(%rsp), %rbx
    movq    %r10, 224(%rsp)
    xorq    %r8, %r11
    movq    96(%rsp), %r10
    movq    %r13, 208(%rsp)
    xorq    %rcx, %rbp
    movq    144(%rsp), %r8
    movq    %r11, 104(%rsp)
    rolq    $56, %rbp
    xorq    120(%rsp), %rax
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    xorq    %rdi, %r10
    rolq    $36, %r10
    xorq    %rdx, %r8
    movq    %rbx, %r14
    xorq    %r13, %rax
    movq    128(%rsp), %r13
    rolq    $27, %r8
    andq    %r10, %r14
    xorq    %r8, %r14
    movq    %r14, 128(%rsp)
    xorq    %r9, %r13
    rolq    $15, %r13
    movq    %r13, %r14
    movq    %r13, %r11
    movq    %rbp, %r13
    notq    %r14
    orq %rbx, %r11
    movq    %r14, %r15
    xorq    %r10, %r11
    orq %rbp, %r15
    andq    %r8, %r13
    xorq    192(%rsp), %rcx
    xorq    %rbx, %r15
    movq    %r10, %rbx
    xorq    80(%rsp), %rdi
    orq %r8, %rbx
    movq    136(%rsp), %r8
    movq    %r15, 96(%rsp)
    xorq    200(%rsp), %rdx
    xorq    %rbp, %rbx
    xorq    %r14, %r13
    xorq    40(%rsp), %rsi
    rolq    $55, %rcx
    xorq    %r13, %rax
    rolq    $41, %rdi
    movq    %rcx, %r10
    xorq    64(%rsp), %r9
    xorq    88(%rsp), %r8
    movq    %rdi, %rbp
    notq    %r10
    rolq    $39, %rdx
    movq    %r10, %r14
    rolq    $2, %rsi
    orq %rdx, %rbp
    andq    %rdx, %r14
    xorq    %r10, %rbp
    movq    %rsi, %r15
    rolq    $62, %r9
    xorq    104(%rsp), %r8
    andq    %rdi, %r15
    xorq    %r9, %r14
    xorq    %rdx, %r15
    xorq    128(%rsp), %r8
    movq    %rbp, 40(%rsp)
    movq    160(%rsp), %r10
    movq    72(%rsp), %rdx
    xorq    %r14, %r8
    xorq    152(%rsp), %r10
    xorq    %r15, %rdx
    xorq    48(%rsp), %rdx
    xorq    %r11, %r10
    xorq    %rbp, %r10
    movq    %rsi, %rbp
    xorq    24(%rsp), %r10
    xorq    %r12, %rdx
    orq %r9, %rbp
    andq    %r9, %rcx
    movq    176(%rsp), %r9
    xorq    %rdi, %rbp
    xorq    %rbp, %rax
    xorq    96(%rsp), %rdx
    xorq    %rsi, %rcx
    movq    %rbp, 80(%rsp)
    movq    %rax, %rbp
    rolq    %rbp
    movq    %r10, %rdi
    xorq    184(%rsp), %r9
    xorq    %r10, %rbp
    rolq    %rdi
    movq    %rdx, %rsi
    xorq    %rbp, %r12
    rolq    %rsi
    rolq    $43, %r12
    xorq    %r8, %rsi
    xorq    224(%rsp), %r9
    xorq    %rbx, %r9
    xorq    %rcx, %r9
    movq    %r9, %r10
    xorq    %r9, %rdi
    movq    88(%rsp), %r9
    rolq    %r10
    xorq    %rdx, %r10
    movq    %r8, %rdx
    movl    $2147483649, %r8d
    rolq    %rdx
    xorq    %r10, %r13
    xorq    %rax, %rdx
    movq    152(%rsp), %rax
    xorq    %rdi, %r9
    xorq    %r9, %r8
    rolq    $21, %r13
    xorq    %rdx, %rcx
    movq    %r8, 112(%rsp)
    rolq    $14, %rcx
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %rax, %r8
    orq %r12, %r8
    xorq    %r8, 112(%rsp)
    movq    %r12, %r8
    notq    %r8
    orq %r13, %r8
    xorq    %rsi, %r11
    xorq    %rbp, %r15
    rolq    $45, %r11
    xorq    %rax, %r8
    andq    %r9, %rax
    movq    %r8, 32(%rsp)
    movq    %rcx, %r8
    xorq    %rcx, %rax
    andq    %r13, %r8
    movq    %rax, 144(%rsp)
    rolq    $61, %r15
    xorq    %r12, %r8
    movq    176(%rsp), %rax
    movq    %rcx, %r12
    movq    %r8, 88(%rsp)
    movq    104(%rsp), %r8
    orq %r9, %r12
    movq    120(%rsp), %rcx
    xorq    %r13, %r12
    movq    %r11, %r9
    movq    %r12, 64(%rsp)
    movq    %r15, %r12
    xorq    %rdx, %rax
    notq    %r12
    xorq    %rdi, %r8
    rolq    $20, %rax
    orq %r11, %r12
    rolq    $3, %r8
    xorq    %r10, %rcx
    movq    %r8, %r13
    rolq    $28, %rcx
    andq    %r8, %r9
    orq %rax, %r13
    xorq    %rax, %r9
    xorq    %r8, %r12
    xorq    %rcx, %r13
    movq    %r9, 120(%rsp)
    andq    %rcx, %rax
    movq    %r13, 104(%rsp)
    movq    %r15, %r13
    movq    208(%rsp), %r9
    orq %rcx, %r13
    movq    24(%rsp), %r8
    xorq    %r15, %rax
    movq    48(%rsp), %rcx
    movq    %rax, 192(%rsp)
    movq    %rdx, %rax
    xorq    %r11, %r13
    movq    %r12, 56(%rsp)
    xorq    %r10, %r9
    movq    %r13, 152(%rsp)
    xorq    %rsi, %r8
    xorq    %rbp, %rcx
    rolq    %r8
    rolq    $6, %rcx
    rolq    $25, %r9
    xorq    %rbx, %rax
    rolq    $8, %rax
    xorq    %rdi, %r14
    movq    %r9, %r15
    movq    %rax, %rbx
    notq    %rax
    rolq    $18, %r14
    andq    %r9, %rbx
    orq %rcx, %r15
    xorq    %rcx, %rbx
    xorq    %r8, %r15
    movq    %rbx, 200(%rsp)
    movq    %rax, %rbx
    andq    %r14, %rbx
    movq    %r15, 176(%rsp)
    xorq    %r9, %rbx
    movq    %r14, %r9
    orq %r8, %r9
    andq    %rcx, %r8
    xorq    %rax, %r9
    xorq    %r14, %r8
    movq    %r9, 208(%rsp)
    movq    %r8, 232(%rsp)
    movq    160(%rsp), %r11
    movq    112(%rsp), %r8
    movq    136(%rsp), %r13
    movq    184(%rsp), %rax
    xorq    %rsi, %r11
    movq    80(%rsp), %r9
    xorq    104(%rsp), %r8
    rolq    $10, %r11
    xorq    %rdi, %r13
    movq    %r11, %r12
    movq    96(%rsp), %r14
    rolq    $36, %r13
    xorq    %rdx, %rax
    movq    %r11, %rcx
    rolq    $27, %rax
    andq    %r13, %r12
    xorq    %r10, %r9
    xorq    %rax, %r12
    xorq    %r15, %r8
    rolq    $56, %r9
    movq    %r12, 80(%rsp)
    xorq    %r12, %r8
    movq    32(%rsp), %r12
    xorq    %rbp, %r14
    rolq    $15, %r14
    orq %r14, %rcx
    notq    %r14
    xorq    120(%rsp), %r12
    xorq    %r13, %rcx
    movq    %r14, %r15
    xorq    200(%rsp), %r12
    orq %r9, %r15
    xorq    168(%rsp), %r10
    xorq    %r11, %r15
    movq    %r9, %r11
    xorq    40(%rsp), %rsi
    movq    %r15, 96(%rsp)
    movq    %r13, %r15
    xorq    128(%rsp), %rdi
    andq    %rax, %r11
    orq %rax, %r15
    xorq    224(%rsp), %rdx
    xorq    %r9, %r15
    xorq    %r14, %r11
    rolq    $55, %r10
    xorq    72(%rsp), %rbp
    xorq    %rcx, %r12
    rolq    $2, %rsi
    movq    %r10, %rax
    rolq    $41, %rdi
    notq    %rax
    movq    %rsi, %r13
    rolq    $39, %rdx
    andq    %rdi, %r13
    movq    %rax, %r14
    movq    %rdi, %r9
    xorq    %rdx, %r13
    andq    %rdx, %r14
    orq %rdx, %r9
    movq    56(%rsp), %rdx
    rolq    $62, %rbp
    xorq    %rax, %r9
    movq    %rsi, %rax
    xorq    %rbp, %r14
    orq %rbp, %rax
    xorq    %r9, %r12
    movq    %r9, 40(%rsp)
    xorq    %rdi, %rax
    movq    192(%rsp), %r9
    xorq    %r14, %r8
    xorq    %rbx, %rdx
    movq    %rax, 72(%rsp)
    movq    152(%rsp), %rax
    xorq    88(%rsp), %rdx
    movq    %r12, %rdi
    xorq    64(%rsp), %rax
    xorq    96(%rsp), %rdx
    xorq    %r13, %rdx
    xorq    %r11, %rax
    andq    %r10, %rbp
    xorq    %rsi, %rbp
    xorq    208(%rsp), %rax
    rolq    %rdi
    xorq    %rbp, %r9
    movq    %rdx, %rsi
    xorq    144(%rsp), %r9
    rolq    %rsi
    xorq    %r8, %rsi
    rolq    %r8
    xorq    72(%rsp), %rax
    xorq    232(%rsp), %r9
    xorq    %rax, %r8
    movq    %rax, %r10
    movq    120(%rsp), %rax
    rolq    %r10
    xorq    %r8, %rbp
    xorq    %r15, %r9
    xorq    %r12, %r10
    rolq    $14, %rbp
    movabsq $-9223372034707259263, %r12
    xorq    %r9, %rdi
    rolq    %r9
    xorq    %rdx, %r9
    movq    112(%rsp), %rdx
    xorq    %r10, %rbx
    rolq    $43, %rbx
    xorq    %rsi, %rax
    xorq    %r9, %r11
    rolq    $44, %rax
    rolq    $21, %r11
    xorq    %rdi, %rdx
    xorq    %rdx, %r12
    movq    %r12, 24(%rsp)
    movq    %rbx, %r12
    orq %rax, %r12
    xorq    %r12, 24(%rsp)
    movq    %rbx, %r12
    notq    %r12
    orq %r11, %r12
    xorq    %rax, %r12
    movq    %r12, 48(%rsp)
    movq    %rbp, %r12
    andq    %r11, %r12
    andq    %rdx, %rax
    xorq    %rsi, %rcx
    xorq    %rbx, %r12
    xorq    %rbp, %rax
    movq    %rbp, %rbx
    movq    %rax, 136(%rsp)
    orq %rdx, %rbx
    movq    64(%rsp), %rdx
    movq    176(%rsp), %rax
    xorq    %r11, %rbx
    xorq    %r10, %r13
    movq    192(%rsp), %r11
    rolq    $45, %rcx
    rolq    $61, %r13
    movq    %rbx, 120(%rsp)
    movq    %r13, %rbx
    xorq    %r9, %rdx
    movq    %r12, 128(%rsp)
    movq    %rcx, %r12
    xorq    %rdi, %rax
    rolq    $28, %rdx
    notq    %rbx
    rolq    $3, %rax
    xorq    %r8, %r11
    orq %rcx, %rbx
    rolq    $20, %r11
    movq    %rax, %rbp
    andq    %rax, %r12
    orq %r11, %rbp
    xorq    %r11, %r12
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r13, %r11
    xorq    %rax, %rbx
    movq    %rbp, 64(%rsp)
    movq    %r13, %rbp
    orq %rdx, %rbp
    movq    %rbx, 112(%rsp)
    movq    32(%rsp), %rdx
    xorq    %rcx, %rbp
    movq    208(%rsp), %rbx
    movq    %r11, %rcx
    movq    %r11, 168(%rsp)
    movq    56(%rsp), %r11
    movq    %rbp, %rax
    xorq    120(%rsp), %rax
    movq    %r12, 184(%rsp)
    xorq    %rsi, %rdx
    xorq    136(%rsp), %rcx
    movq    %rbp, 192(%rsp)
    xorq    %r9, %rbx
    rolq    %rdx
    movq    104(%rsp), %rbp
    xorq    %r10, %r11
    rolq    $6, %r11
    rolq    $25, %rbx
    xorq    %r8, %r15
    rolq    $8, %r15
    xorq    %rdi, %r14
    movq    %rbx, %r12
    movq    %r15, %r13
    notq    %r15
    rolq    $18, %r14
    andq    %rbx, %r13
    orq %r11, %r12
    xorq    %rdi, %rbp
    xorq    %r11, %r13
    xorq    %rdx, %r12
    andq    %rdx, %r11
    movq    %r13, 176(%rsp)
    movq    %r15, %r13
    xorq    %r14, %r11
    andq    %r14, %r13
    movq    %r12, 56(%rsp)
    movq    200(%rsp), %r12
    xorq    %rbx, %r13
    movq    %r14, %rbx
    movq    96(%rsp), %r14
    orq %rdx, %rbx
    movq    64(%rsp), %rdx
    movq    %r11, 208(%rsp)
    xorq    %r15, %rbx
    xorq    %r11, %rcx
    rolq    $36, %rbp
    movq    %rbx, 160(%rsp)
    xorq    %rbx, %rax
    xorq    %rsi, %r12
    movq    144(%rsp), %rbx
    rolq    $10, %r12
    xorq    %r10, %r14
    xorq    24(%rsp), %rdx
    movq    %r12, %r15
    rolq    $15, %r14
    movq    72(%rsp), %r11
    andq    %rbp, %r15
    xorq    %r8, %rbx
    xorq    56(%rsp), %rdx
    rolq    $27, %rbx
    xorq    %rbx, %r15
    xorq    %r9, %r11
    movq    %r15, 72(%rsp)
    rolq    $56, %r11
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    orq %r12, %r15
    movq    %r14, 32(%rsp)
    xorq    %rbp, %r15
    xorq    152(%rsp), %r9
    orq %r11, %r14
    xorq    80(%rsp), %rdi
    xorq    %r12, %r14
    orq %rbx, %rbp
    xorq    232(%rsp), %r8
    movq    %r11, %r12
    xorq    %r11, %rbp
    andq    %rbx, %r12
    xorq    88(%rsp), %r10
    movq    %r14, 96(%rsp)
    rolq    $55, %r9
    xorq    32(%rsp), %r12
    movq    %rbp, 104(%rsp)
    rolq    $41, %rdi
    movq    %r9, %r11
    xorq    40(%rsp), %rsi
    rolq    $39, %r8
    movq    %rdi, %rbx
    notq    %r11
    orq %r8, %rbx
    rolq    $62, %r10
    xorq    %r11, %rbx
    movq    %r11, %r14
    xorq    %r12, %rax
    movq    %rbx, 40(%rsp)
    movq    176(%rsp), %r11
    andq    %r8, %r14
    rolq    $2, %rsi
    xorq    %r10, %r14
    xorq    %rbp, %rcx
    xorq    %r14, %rdx
    movq    %rsi, %rbp
    xorq    184(%rsp), %r11
    xorq    %r15, %r11
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    andq    %rdi, %rbx
    xorq    48(%rsp), %r11
    xorq    %r8, %rbx
    movq    128(%rsp), %r8
    xorq    %rbx, %r8
    xorq    112(%rsp), %r8
    xorq    %r13, %r8
    xorq    96(%rsp), %r8
    orq %r10, %rbp
    andq    %r9, %r10
    xorq    %rdi, %rbp
    xorq    %rsi, %r10
    movq    %r11, %rdi
    xorq    %r10, %rcx
    xorq    %rbp, %rax
    rolq    %rdi
    movq    %rax, %r9
    movq    %r8, %rsi
    xorq    %rcx, %rdi
    rolq    %rcx
    rolq    %rsi
    xorq    %r8, %rcx
    movq    24(%rsp), %r8
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rcx, %r12
    rolq    %r9
    xorq    %rax, %rdx
    movq    184(%rsp), %rax
    xorq    %r11, %r9
    rolq    $21, %r12
    xorq    %rdx, %r10
    xorq    %rdi, %r8
    xorq    %r9, %r13
    rolq    $14, %r10
    movabsq $-9223372036854743031, %r11
    rolq    $43, %r13
    xorq    %r8, %r11
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %r11, 24(%rsp)
    movq    %r13, %r11
    orq %rax, %r11
    xorq    %r11, 24(%rsp)
    movq    %r13, %r11
    notq    %r11
    orq %r12, %r11
    xorq    %rax, %r11
    movq    %r11, 32(%rsp)
    movq    %r10, %r11
    andq    %r12, %r11
    xorq    %r13, %r11
    movq    %r10, %r13
    orq %r8, %r13
    andq    %r8, %rax
    movq    56(%rsp), %r8
    xorq    %r10, %rax
    movq    %r11, 88(%rsp)
    movq    168(%rsp), %r11
    movq    %rax, 184(%rsp)
    movq    120(%rsp), %rax
    xorq    %r9, %rbx
    rolq    $61, %rbx
    xorq    %r12, %r13
    xorq    %rsi, %r15
    rolq    $45, %r15
    movq    %r13, 80(%rsp)
    xorq    %rdi, %r8
    movq    %rbx, %r13
    rolq    $3, %r8
    xorq    %rdx, %r11
    xorq    %rcx, %rax
    movq    %r15, %r12
    notq    %r13
    rolq    $28, %rax
    rolq    $20, %r11
    andq    %r8, %r12
    orq %r15, %r13
    movq    %r8, %r10
    xorq    %r11, %r12
    xorq    %r8, %r13
    orq %r11, %r10
    movq    %rbx, %r8
    andq    %rax, %r11
    orq %rax, %r8
    xorq    %rax, %r10
    xorq    %rbx, %r11
    movq    104(%rsp), %rbx
    xorq    %r15, %r8
    movq    160(%rsp), %rax
    movq    %r10, 120(%rsp)
    xorq    %rdi, %r14
    movq    %r8, 152(%rsp)
    movq    112(%rsp), %r10
    movq    48(%rsp), %r8
    movq    %r12, 144(%rsp)
    xorq    %rdx, %rbx
    movq    %r11, 168(%rsp)
    xorq    %rcx, %rax
    rolq    $8, %rbx
    movq    %r13, 56(%rsp)
    rolq    $25, %rax
    xorq    %r9, %r10
    movq    %rbx, %r12
    xorq    %rsi, %r8
    rolq    $6, %r10
    notq    %rbx
    rolq    %r8
    rolq    $18, %r14
    andq    %rax, %r12
    xorq    %r10, %r12
    movq    %rax, %r11
    movq    %r14, %r13
    movq    %r12, 112(%rsp)
    movq    %rbx, %r12
    orq %r10, %r11
    andq    %r14, %r12
    orq %r8, %r13
    andq    %r8, %r10
    xorq    %rax, %r12
    movq    152(%rsp), %rax
    xorq    %rbx, %r13
    xorq    %r14, %r10
    movq    176(%rsp), %rbx
    xorq    %r8, %r11
    movq    136(%rsp), %r8
    movq    %r13, 104(%rsp)
    xorq    %rcx, %rbp
    movq    %r10, 160(%rsp)
    movq    64(%rsp), %r10
    rolq    $56, %rbp
    xorq    80(%rsp), %rax
    movq    %r11, 48(%rsp)
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    xorq    %rdx, %r8
    xorq    %rdi, %r10
    movq    %rbx, %r14
    rolq    $27, %r8
    xorq    %r13, %rax
    movq    96(%rsp), %r13
    rolq    $36, %r10
    andq    %r10, %r14
    xorq    %r8, %r14
    movq    %r14, 96(%rsp)
    xorq    %r9, %r13
    rolq    $15, %r13
    movq    %r13, %r14
    movq    %r13, %r11
    movq    %rbp, %r13
    notq    %r14
    orq %rbx, %r11
    andq    %r8, %r13
    movq    %r14, %r15
    xorq    %r14, %r13
    xorq    %r10, %r11
    orq %rbp, %r15
    xorq    %r13, %rax
    xorq    %rbx, %r15
    movq    %r10, %rbx
    orq %r8, %rbx
    movq    120(%rsp), %r8
    movq    %r15, 64(%rsp)
    xorq    %rbp, %rbx
    xorq    192(%rsp), %rcx
    xorq    72(%rsp), %rdi
    xorq    208(%rsp), %rdx
    xorq    40(%rsp), %rsi
    rolq    $55, %rcx
    xorq    24(%rsp), %r8
    rolq    $41, %rdi
    movq    %rcx, %r10
    xorq    128(%rsp), %r9
    rolq    $39, %rdx
    movq    %rdi, %rbp
    notq    %r10
    orq %rdx, %rbp
    rolq    $2, %rsi
    xorq    %r10, %rbp
    movq    %r10, %r14
    movq    112(%rsp), %r10
    movq    %rsi, %r15
    andq    %rdx, %r14
    xorq    48(%rsp), %r8
    andq    %rdi, %r15
    rolq    $62, %r9
    movq    %rbp, 40(%rsp)
    xorq    %rdx, %r15
    movq    88(%rsp), %rdx
    xorq    %r9, %r14
    xorq    144(%rsp), %r10
    andq    %r9, %rcx
    xorq    96(%rsp), %r8
    xorq    %r15, %rdx
    xorq    %r11, %r10
    xorq    56(%rsp), %rdx
    xorq    %rbp, %r10
    movq    %rsi, %rbp
    orq %r9, %rbp
    movq    168(%rsp), %r9
    xorq    %r14, %r8
    xorq    %rdi, %rbp
    xorq    32(%rsp), %r10
    xorq    %r12, %rdx
    xorq    %rbp, %rax
    movq    %rbp, 72(%rsp)
    xorq    64(%rsp), %rdx
    xorq    %rsi, %rcx
    movq    %rax, %rbp
    xorq    184(%rsp), %r9
    rolq    %rbp
    xorq    %r10, %rbp
    movq    %r10, %rdi
    rolq    %rdi
    xorq    %rbp, %r12
    movq    %rdx, %rsi
    rolq    $43, %r12
    xorq    160(%rsp), %r9
    rolq    %rsi
    xorq    %r8, %rsi
    xorq    %rbx, %r9
    xorq    %rcx, %r9
    movq    %r9, %r10
    xorq    %r9, %rdi
    movq    24(%rsp), %r9
    rolq    %r10
    xorq    %rdx, %r10
    movq    %r8, %rdx
    rolq    %rdx
    xorq    %r10, %r13
    xorq    %rax, %rdx
    movq    144(%rsp), %rax
    xorq    %rdi, %r9
    movq    %r9, %r8
    rolq    $21, %r13
    xorq    %rdx, %rcx
    xorb    $-118, %r8b
    rolq    $14, %rcx
    movq    %r8, 128(%rsp)
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %rax, %r8
    orq %r12, %r8
    xorq    %r8, 128(%rsp)
    movq    %r12, %r8
    notq    %r8
    orq %r13, %r8
    xorq    %rax, %r8
    movq    %r8, 24(%rsp)
    movq    %rcx, %r8
    andq    %r13, %r8
    xorq    %r12, %r8
    movq    %rcx, %r12
    orq %r9, %r12
    movq    %r8, 136(%rsp)
    movq    48(%rsp), %r8
    xorq    %r13, %r12
    andq    %r9, %rax
    xorq    %rsi, %r11
    xorq    %rcx, %rax
    movq    80(%rsp), %rcx
    rolq    $45, %r11
    movq    %rax, 192(%rsp)
    xorq    %rbp, %r15
    movq    %r11, %r9
    movq    168(%rsp), %rax
    xorq    %rdi, %r8
    rolq    $61, %r15
    rolq    $3, %r8
    movq    %r12, 144(%rsp)
    movq    %r15, %r12
    xorq    %r10, %rcx
    movq    %r8, %r13
    andq    %r8, %r9
    rolq    $28, %rcx
    notq    %r12
    xorq    %rdi, %r14
    xorq    %rdx, %rax
    orq %r11, %r12
    rolq    $18, %r14
    rolq    $20, %rax
    xorq    %r8, %r12
    orq %rax, %r13
    xorq    %rax, %r9
    andq    %rcx, %rax
    xorq    %rcx, %r13
    xorq    %r15, %rax
    movq    %r13, 80(%rsp)
    movq    %r15, %r13
    movq    32(%rsp), %r8
    movq    %r9, 168(%rsp)
    movq    104(%rsp), %r9
    orq %rcx, %r13
    movq    56(%rsp), %rcx
    movq    %rax, 200(%rsp)
    movq    %rdx, %rax
    xorq    %rbx, %rax
    xorq    %r11, %r13
    movq    112(%rsp), %r11
    xorq    %rsi, %r8
    rolq    $8, %rax
    movq    %r13, 176(%rsp)
    xorq    %r10, %r9
    rolq    %r8
    movq    %rax, %rbx
    rolq    $25, %r9
    xorq    %rbp, %rcx
    notq    %rax
    rolq    $6, %rcx
    movq    %r9, %r15
    movq    120(%rsp), %r13
    orq %rcx, %r15
    movq    %r12, 48(%rsp)
    xorq    %r8, %r15
    andq    %r9, %rbx
    xorq    %rsi, %r11
    xorq    %rcx, %rbx
    rolq    $10, %r11
    movq    %r15, 32(%rsp)
    movq    %rbx, 56(%rsp)
    movq    %rax, %rbx
    xorq    %rdi, %r13
    andq    %r14, %rbx
    rolq    $36, %r13
    movq    %r11, %r12
    xorq    %r9, %rbx
    movq    %r14, %r9
    andq    %r13, %r12
    orq %r8, %r9
    andq    %rcx, %r8
    movq    %r11, %rcx
    xorq    %r14, %r8
    xorq    %rax, %r9
    movq    184(%rsp), %rax
    movq    %r8, 208(%rsp)
    movq    128(%rsp), %r8
    movq    %r9, 104(%rsp)
    movq    72(%rsp), %r9
    movq    64(%rsp), %r14
    xorq    %rdx, %rax
    xorq    80(%rsp), %r8
    rolq    $27, %rax
    xorq    %rax, %r12
    xorq    %r10, %r9
    movq    %r12, 72(%rsp)
    xorq    %rbp, %r14
    rolq    $56, %r9
    rolq    $15, %r14
    xorq    %r15, %r8
    orq %r14, %rcx
    notq    %r14
    xorq    %r12, %r8
    movq    24(%rsp), %r12
    movq    %r14, %r15
    orq %r9, %r15
    xorq    %r13, %rcx
    xorq    %r11, %r15
    movq    %r9, %r11
    movq    %r15, 64(%rsp)
    andq    %rax, %r11
    movq    %r13, %r15
    xorq    168(%rsp), %r12
    xorq    %r14, %r11
    xorq    56(%rsp), %r12
    xorq    %rcx, %r12
    orq %rax, %r15
    xorq    152(%rsp), %r10
    xorq    96(%rsp), %rdi
    xorq    %r9, %r15
    xorq    160(%rsp), %rdx
    xorq    40(%rsp), %rsi
    xorq    88(%rsp), %rbp
    rolq    $55, %r10
    rolq    $41, %rdi
    movq    %r10, %rax
    rolq    $39, %rdx
    movq    %rdi, %r9
    notq    %rax
    rolq    $2, %rsi
    orq %rdx, %r9
    movq    %rax, %r14
    xorq    %rax, %r9
    rolq    $62, %rbp
    movq    %rsi, %rax
    orq %rbp, %rax
    movq    %rsi, %r13
    andq    %rdx, %r14
    xorq    %rdi, %rax
    andq    %rdi, %r13
    xorq    %r9, %r12
    xorq    %rdx, %r13
    movq    %rax, 88(%rsp)
    movq    48(%rsp), %rdx
    movq    176(%rsp), %rax
    movq    %r9, 40(%rsp)
    xorq    %rbp, %r14
    movq    200(%rsp), %r9
    andq    %r10, %rbp
    xorq    %r14, %r8
    xorq    %rsi, %rbp
    movq    %r12, %rdi
    xorq    %rbx, %rdx
    xorq    144(%rsp), %rax
    xorq    136(%rsp), %rdx
    xorq    %rbp, %r9
    xorq    %r11, %rax
    xorq    64(%rsp), %rdx
    xorq    104(%rsp), %rax
    xorq    %r13, %rdx
    xorq    88(%rsp), %rax
    movq    %rdx, %rsi
    xorq    192(%rsp), %r9
    rolq    %rdi
    rolq    %rsi
    xorq    %r8, %rsi
    rolq    %r8
    xorq    %rax, %r8
    movq    %rax, %r10
    movq    168(%rsp), %rax
    xorq    208(%rsp), %r9
    rolq    %r10
    xorq    %r8, %rbp
    xorq    %r12, %r10
    rolq    $14, %rbp
    xorq    %r10, %rbx
    rolq    $43, %rbx
    xorq    %rsi, %rax
    xorq    %r15, %r9
    rolq    $44, %rax
    xorq    %r9, %rdi
    rolq    %r9
    xorq    %rdx, %r9
    movq    128(%rsp), %rdx
    xorq    %r9, %r11
    rolq    $21, %r11
    xorq    %rdi, %rdx
    movq    %rdx, %r12
    xorb    $-120, %r12b
    movq    %r12, 128(%rsp)
    movq    %rbx, %r12
    orq %rax, %r12
    xorq    %r12, 128(%rsp)
    movq    %rbx, %r12
    notq    %r12
    orq %r11, %r12
    xorq    %rax, %r12
    andq    %rdx, %rax
    movq    %r12, 112(%rsp)
    movq    %rbp, %r12
    xorq    %rbp, %rax
    andq    %r11, %r12
    movq    %rax, 184(%rsp)
    movq    32(%rsp), %rax
    xorq    %rbx, %r12
    movq    %rbp, %rbx
    orq %rdx, %rbx
    movq    144(%rsp), %rdx
    movq    %r12, 96(%rsp)
    xorq    %r11, %rbx
    movq    200(%rsp), %r11
    movq    %rbx, 120(%rsp)
    xorq    %r9, %rdx
    xorq    %rdi, %rax
    xorq    %r10, %r13
    rolq    $3, %rax
    rolq    $61, %r13
    xorq    %r8, %r11
    xorq    %rsi, %rcx
    rolq    $20, %r11
    movq    %rax, %rbp
    rolq    $45, %rcx
    movq    %r13, %rbx
    rolq    $28, %rdx
    orq %r11, %rbp
    movq    %rcx, %r12
    notq    %rbx
    xorq    %rdx, %rbp
    andq    %rax, %r12
    orq %rcx, %rbx
    movq    %rbp, 144(%rsp)
    xorq    %r11, %r12
    xorq    %rax, %rbx
    andq    %rdx, %r11
    movq    %r13, %rbp
    movq    %rbx, 32(%rsp)
    xorq    %r13, %r11
    movq    104(%rsp), %rbx
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    movq    %r11, 160(%rsp)
    movq    %r11, %rcx
    movq    48(%rsp), %r11
    xorq    %r8, %r15
    movq    %r12, 152(%rsp)
    movq    24(%rsp), %rdx
    rolq    $8, %r15
    xorq    %rdi, %r14
    xorq    %r9, %rbx
    movq    %rbp, %rax
    movq    %r15, %r13
    rolq    $25, %rbx
    rolq    $18, %r14
    xorq    120(%rsp), %rax
    xorq    %r10, %r11
    movq    %rbx, %r12
    xorq    184(%rsp), %rcx
    rolq    $6, %r11
    xorq    %rsi, %rdx
    movq    %rbp, 168(%rsp)
    rolq    %rdx
    orq %r11, %r12
    notq    %r15
    xorq    %rdx, %r12
    andq    %rbx, %r13
    movq    80(%rsp), %rbp
    xorq    %r11, %r13
    andq    %rdx, %r11
    movq    %r12, 104(%rsp)
    movq    %r13, 200(%rsp)
    movq    %r15, %r13
    movq    56(%rsp), %r12
    andq    %r14, %r13
    xorq    %r14, %r11
    xorq    %rbx, %r13
    movq    %r14, %rbx
    movq    64(%rsp), %r14
    orq %rdx, %rbx
    movq    144(%rsp), %rdx
    xorq    %r11, %rcx
    xorq    %r15, %rbx
    movq    %r11, 232(%rsp)
    xorq    %rsi, %r12
    movq    %rbx, 224(%rsp)
    xorq    %rbx, %rax
    movq    88(%rsp), %r11
    movq    192(%rsp), %rbx
    rolq    $10, %r12
    xorq    %rdi, %rbp
    xorq    128(%rsp), %rdx
    rolq    $36, %rbp
    movq    %r12, %r15
    xorq    %r10, %r14
    andq    %rbp, %r15
    rolq    $15, %r14
    xorq    %r9, %r11
    xorq    %r8, %rbx
    rolq    $56, %r11
    xorq    104(%rsp), %rdx
    rolq    $27, %rbx
    xorq    %rbx, %r15
    movq    %r15, 88(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 24(%rsp)
    orq %r11, %r14
    orq %r12, %r15
    xorq    %r12, %r14
    movq    %r11, %r12
    xorq    %rbp, %r15
    andq    %rbx, %r12
    movq    %r14, 80(%rsp)
    xorq    24(%rsp), %r12
    xorq    %r12, %rax
    orq %rbx, %rbp
    xorq    72(%rsp), %rdi
    xorq    176(%rsp), %r9
    xorq    %r11, %rbp
    xorq    208(%rsp), %r8
    movq    %rbp, 64(%rsp)
    xorq    %rbp, %rcx
    xorq    40(%rsp), %rsi
    rolq    $41, %rdi
    xorq    136(%rsp), %r10
    rolq    $55, %r9
    movq    %rdi, %rbx
    rolq    $39, %r8
    movq    %r9, %r11
    notq    %r11
    orq %r8, %rbx
    rolq    $2, %rsi
    xorq    %r11, %rbx
    movq    %r11, %r14
    movq    200(%rsp), %r11
    movq    %rbx, 40(%rsp)
    andq    %r8, %r14
    rolq    $62, %r10
    movq    %rsi, %rbp
    xorq    %r10, %r14
    orq %r10, %rbp
    andq    %r9, %r10
    xorq    %r14, %rdx
    xorq    152(%rsp), %r11
    xorq    %rdi, %rbp
    xorq    %rsi, %r10
    xorq    %rbp, %rax
    movq    %rax, %r9
    xorq    %r15, %r11
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    andq    %rdi, %rbx
    xorq    112(%rsp), %r11
    xorq    %r8, %rbx
    movq    96(%rsp), %r8
    movq    %r11, %rdi
    xorq    %rbx, %r8
    xorq    32(%rsp), %r8
    xorq    %r13, %r8
    xorq    80(%rsp), %r8
    xorq    %r10, %rcx
    rolq    %rdi
    xorq    %rcx, %rdi
    rolq    %rcx
    rolq    %r9
    xorq    %r11, %r9
    movl    $2147516425, %r11d
    movq    %r8, %rsi
    xorq    %r8, %rcx
    movq    128(%rsp), %r8
    rolq    %rsi
    xorq    %r9, %r13
    xorq    %rcx, %r12
    rolq    $43, %r13
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    152(%rsp), %rax
    rolq    $21, %r12
    xorq    %rdi, %r8
    xorq    %rdx, %r10
    xorq    %r8, %r11
    rolq    $14, %r10
    movq    %r11, 24(%rsp)
    movq    %r13, %r11
    xorq    %rsi, %rax
    rolq    $44, %rax
    orq %rax, %r11
    xorq    %r11, 24(%rsp)
    movq    %r13, %r11
    notq    %r11
    orq %r12, %r11
    xorq    %rax, %r11
    andq    %r8, %rax
    movq    %r11, 48(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    andq    %r12, %r11
    movq    %rax, 136(%rsp)
    movq    120(%rsp), %rax
    xorq    %r13, %r11
    movq    %r10, %r13
    movq    %r11, 72(%rsp)
    movq    160(%rsp), %r11
    orq %r8, %r13
    movq    104(%rsp), %r8
    xorq    %r12, %r13
    xorq    %rcx, %rax
    movq    %r13, 128(%rsp)
    rolq    $28, %rax
    xorq    %rdx, %r11
    xorq    %r9, %rbx
    xorq    %rsi, %r15
    rolq    $61, %rbx
    rolq    $45, %r15
    xorq    %rdi, %r8
    movq    %rbx, %r13
    rolq    $3, %r8
    movq    %r15, %r12
    notq    %r13
    rolq    $20, %r11
    andq    %r8, %r12
    orq %r15, %r13
    movq    %r8, %r10
    xorq    %r11, %r12
    xorq    %r8, %r13
    orq %r11, %r10
    movq    %rbx, %r8
    andq    %rax, %r11
    xorq    %rax, %r10
    orq %rax, %r8
    xorq    %rbx, %r11
    movq    224(%rsp), %rax
    xorq    %r15, %r8
    movq    64(%rsp), %rbx
    movq    %r10, 104(%rsp)
    xorq    %rdi, %r14
    movq    32(%rsp), %r10
    movq    %r12, 120(%rsp)
    rolq    $18, %r14
    movq    %r8, 152(%rsp)
    movq    112(%rsp), %r8
    xorq    %rcx, %rax
    movq    %r11, 192(%rsp)
    xorq    %rdx, %rbx
    rolq    $25, %rax
    movq    %r13, 56(%rsp)
    xorq    %r9, %r10
    rolq    $8, %rbx
    movq    %rax, %r11
    rolq    $6, %r10
    xorq    %rsi, %r8
    movq    %rbx, %r12
    rolq    %r8
    orq %r10, %r11
    andq    %rax, %r12
    xorq    %r8, %r11
    xorq    %r10, %r12
    notq    %rbx
    movq    %r11, 32(%rsp)
    movq    %r14, %r13
    movq    %r12, 64(%rsp)
    movq    %rbx, %r12
    andq    %r14, %r12
    xorq    %rax, %r12
    movq    152(%rsp), %rax
    orq %r8, %r13
    andq    %r8, %r10
    xorq    %rbx, %r13
    movq    200(%rsp), %rbx
    xorq    %r14, %r10
    movq    184(%rsp), %r8
    xorq    %rcx, %rbp
    movq    %r10, 160(%rsp)
    movq    144(%rsp), %r10
    rolq    $56, %rbp
    xorq    128(%rsp), %rax
    movq    %r13, 176(%rsp)
    xorq    %rsi, %rbx
    xorq    168(%rsp), %rcx
    rolq    $10, %rbx
    xorq    %rdx, %r8
    xorq    232(%rsp), %rdx
    xorq    %rdi, %r10
    movq    %rbx, %r14
    rolq    $36, %r10
    xorq    %r13, %rax
    movq    80(%rsp), %r13
    rolq    $27, %r8
    andq    %r10, %r14
    rolq    $55, %rcx
    xorq    %r8, %r14
    movq    %r14, 80(%rsp)
    xorq    %r9, %r13
    xorq    96(%rsp), %r9
    rolq    $15, %r13
    movq    %r13, %r14
    movq    %r13, %r11
    movq    %rbp, %r13
    notq    %r14
    orq %rbx, %r11
    andq    %r8, %r13
    movq    %r14, %r15
    xorq    %r14, %r13
    rolq    $62, %r9
    orq %rbp, %r15
    xorq    %r10, %r11
    xorq    %r13, %rax
    xorq    %rbx, %r15
    movq    %r10, %rbx
    movq    %rcx, %r10
    orq %r8, %rbx
    notq    %r10
    movq    104(%rsp), %r8
    xorq    %rbp, %rbx
    rolq    $39, %rdx
    xorq    88(%rsp), %rdi
    movq    %r10, %r14
    xorq    40(%rsp), %rsi
    andq    %r9, %rcx
    andq    %rdx, %r14
    movq    %r15, 184(%rsp)
    xorq    %r9, %r14
    xorq    24(%rsp), %r8
    rolq    $41, %rdi
    movq    %rdi, %rbp
    rolq    $2, %rsi
    orq %rdx, %rbp
    movq    %rsi, %r15
    xorq    %rsi, %rcx
    xorq    %r10, %rbp
    movq    64(%rsp), %r10
    andq    %rdi, %r15
    movq    %rbp, 40(%rsp)
    xorq    %rdx, %r15
    movq    72(%rsp), %rdx
    xorq    32(%rsp), %r8
    xorq    120(%rsp), %r10
    xorq    %r15, %rdx
    xorq    56(%rsp), %rdx
    xorq    80(%rsp), %r8
    xorq    %r11, %r10
    xorq    %rbp, %r10
    movq    %rsi, %rbp
    orq %r9, %rbp
    movq    192(%rsp), %r9
    xorq    %r12, %rdx
    xorq    48(%rsp), %r10
    xorq    %rdi, %rbp
    xorq    %r14, %r8
    xorq    %rbp, %rax
    xorq    184(%rsp), %rdx
    movq    %rbp, 88(%rsp)
    movq    %rax, %rbp
    xorq    136(%rsp), %r9
    movq    %r10, %rdi
    rolq    %rdi
    movq    %rdx, %rsi
    xorq    160(%rsp), %r9
    xorq    %rbx, %r9
    xorq    %rcx, %r9
    xorq    %r9, %rdi
    rolq    %rbp
    xorq    %r10, %rbp
    movq    %r9, %r10
    movq    24(%rsp), %r9
    rolq    %r10
    rolq    %rsi
    xorq    %rbp, %r12
    xorq    %rdx, %r10
    movq    %r8, %rdx
    xorq    %r8, %rsi
    rolq    %rdx
    movl    $2147483658, %r8d
    rolq    $43, %r12
    xorq    %rax, %rdx
    movq    120(%rsp), %rax
    xorq    %rdi, %r9
    xorq    %r9, %r8
    xorq    %r10, %r13
    xorq    %rdx, %rcx
    movq    %r8, 24(%rsp)
    rolq    $21, %r13
    rolq    $14, %rcx
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %rax, %r8
    orq %r12, %r8
    xorq    %r8, 24(%rsp)
    movq    %r12, %r8
    notq    %r8
    orq %r13, %r8
    xorq    %rax, %r8
    andq    %r9, %rax
    movq    %r8, 112(%rsp)
    movq    %rcx, %r8
    xorq    %rcx, %rax
    andq    %r13, %r8
    movq    %rax, 144(%rsp)
    movq    192(%rsp), %rax
    xorq    %r12, %r8
    movq    %rcx, %r12
    movq    128(%rsp), %rcx
    movq    %r8, 96(%rsp)
    movq    32(%rsp), %r8
    orq %r9, %r12
    xorq    %r13, %r12
    xorq    %rdx, %rax
    movq    %r12, 120(%rsp)
    xorq    %r10, %rcx
    rolq    $20, %rax
    rolq    $28, %rcx
    xorq    %rdi, %r8
    xorq    %rsi, %r11
    rolq    $3, %r8
    rolq    $45, %r11
    xorq    %rbp, %r15
    movq    %r8, %r13
    movq    %r11, %r9
    rolq    $61, %r15
    orq %rax, %r13
    andq    %r8, %r9
    movq    %r15, %r12
    xorq    %rcx, %r13
    xorq    %rax, %r9
    andq    %rcx, %rax
    movq    %r13, 128(%rsp)
    movq    %r15, %r13
    xorq    %r15, %rax
    movq    %r9, 192(%rsp)
    orq %rcx, %r13
    movq    56(%rsp), %rcx
    movq    176(%rsp), %r9
    notq    %r12
    xorq    %rdi, %r14
    movq    %rax, 200(%rsp)
    movq    %rdx, %rax
    orq %r11, %r12
    xorq    %rbx, %rax
    xorq    %r8, %r12
    movq    48(%rsp), %r8
    rolq    $8, %rax
    xorq    %rbp, %rcx
    rolq    $18, %r14
    xorq    %r10, %r9
    movq    %rax, %rbx
    rolq    $6, %rcx
    rolq    $25, %r9
    notq    %rax
    xorq    %r11, %r13
    andq    %r9, %rbx
    xorq    %rsi, %r8
    movq    %r9, %r15
    xorq    %rcx, %rbx
    rolq    %r8
    orq %rcx, %r15
    movq    %rbx, 176(%rsp)
    movq    %rax, %rbx
    xorq    %r8, %r15
    andq    %r14, %rbx
    movq    64(%rsp), %r11
    movq    %r13, 168(%rsp)
    xorq    %r9, %rbx
    movq    %r14, %r9
    movq    104(%rsp), %r13
    orq %r8, %r9
    movq    %r12, 32(%rsp)
    xorq    %rax, %r9
    andq    %rcx, %r8
    movq    136(%rsp), %rax
    xorq    %r14, %r8
    xorq    %rsi, %r11
    movq    %r9, 208(%rsp)
    movq    %r8, 224(%rsp)
    movq    24(%rsp), %r8
    xorq    %rdi, %r13
    rolq    $10, %r11
    rolq    $36, %r13
    movq    88(%rsp), %r9
    xorq    %rdx, %rax
    movq    %r11, %r12
    movq    184(%rsp), %r14
    rolq    $27, %rax
    andq    %r13, %r12
    movq    %r11, %rcx
    xorq    128(%rsp), %r8
    xorq    %rax, %r12
    movq    %r15, 56(%rsp)
    movq    %r12, 88(%rsp)
    xorq    %r10, %r9
    xorq    152(%rsp), %r10
    xorq    %rbp, %r14
    rolq    $56, %r9
    xorq    72(%rsp), %rbp
    rolq    $15, %r14
    xorq    %r15, %r8
    orq %r14, %rcx
    notq    %r14
    xorq    %r12, %r8
    movq    112(%rsp), %r12
    movq    %r14, %r15
    orq %r9, %r15
    xorq    %r13, %rcx
    rolq    $62, %rbp
    xorq    %r11, %r15
    movq    %r9, %r11
    rolq    $55, %r10
    movq    %r15, 64(%rsp)
    movq    %r13, %r15
    andq    %rax, %r11
    xorq    192(%rsp), %r12
    orq %rax, %r15
    xorq    %r14, %r11
    xorq    %r9, %r15
    movq    %r10, %rax
    notq    %rax
    movq    %rax, %r14
    xorq    176(%rsp), %r12
    xorq    %rcx, %r12
    xorq    160(%rsp), %rdx
    xorq    80(%rsp), %rdi
    xorq    40(%rsp), %rsi
    rolq    $39, %rdx
    rolq    $41, %rdi
    andq    %rdx, %r14
    movq    %rdi, %r9
    rolq    $2, %rsi
    xorq    %rbp, %r14
    orq %rdx, %r9
    movq    %rsi, %r13
    xorq    %r14, %r8
    xorq    %rax, %r9
    movq    %rsi, %rax
    andq    %rdi, %r13
    orq %rbp, %rax
    movq    %r9, 40(%rsp)
    xorq    %rdx, %r13
    xorq    %rdi, %rax
    movq    32(%rsp), %rdx
    xorq    %r9, %r12
    movq    %rax, 72(%rsp)
    movq    168(%rsp), %rax
    andq    %r10, %rbp
    movq    200(%rsp), %r9
    xorq    %rsi, %rbp
    movq    %r12, %rdi
    rolq    %rdi
    xorq    %rbx, %rdx
    xorq    120(%rsp), %rax
    xorq    96(%rsp), %rdx
    xorq    %rbp, %r9
    xorq    144(%rsp), %r9
    xorq    %r11, %rax
    xorq    64(%rsp), %rdx
    xorq    208(%rsp), %rax
    xorq    224(%rsp), %r9
    xorq    %r13, %rdx
    xorq    72(%rsp), %rax
    movq    %rdx, %rsi
    xorq    %r15, %r9
    rolq    %rsi
    xorq    %r9, %rdi
    xorq    %r8, %rsi
    rolq    %r9
    xorq    %rdx, %r9
    rolq    %r8
    movq    24(%rsp), %rdx
    xorq    %rax, %r8
    movq    %rax, %r10
    movq    192(%rsp), %rax
    rolq    %r10
    xorq    %r9, %r11
    xorq    %r8, %rbp
    rolq    $21, %r11
    xorq    %r12, %r10
    movl    $2147516555, %r12d
    xorq    %rdi, %rdx
    xorq    %r10, %rbx
    rolq    $14, %rbp
    rolq    $43, %rbx
    xorq    %rdx, %r12
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %r12, 24(%rsp)
    movq    %rbx, %r12
    orq %rax, %r12
    xorq    %r12, 24(%rsp)
    movq    %rbx, %r12
    notq    %r12
    orq %r11, %r12
    xorq    %rax, %r12
    andq    %rdx, %rax
    movq    %r12, 48(%rsp)
    movq    %rbp, %r12
    xorq    %rbp, %rax
    andq    %r11, %r12
    movq    %rax, 136(%rsp)
    movq    56(%rsp), %rax
    xorq    %rbx, %r12
    movq    %rbp, %rbx
    orq %rdx, %rbx
    movq    120(%rsp), %rdx
    movq    %r12, 80(%rsp)
    xorq    %r11, %rbx
    movq    200(%rsp), %r11
    xorq    %rdi, %rax
    movq    %rbx, 104(%rsp)
    rolq    $3, %rax
    xorq    %r9, %rdx
    movq    %rax, %rbp
    xorq    %r8, %r11
    rolq    $28, %rdx
    rolq    $20, %r11
    xorq    %rsi, %rcx
    xorq    %r10, %r13
    rolq    $61, %r13
    rolq    $45, %rcx
    orq %r11, %rbp
    movq    %r13, %rbx
    movq    %rcx, %r12
    xorq    %rdx, %rbp
    notq    %rbx
    andq    %rax, %r12
    movq    %rbp, 120(%rsp)
    orq %rcx, %rbx
    xorq    %r11, %r12
    movq    %r13, %rbp
    xorq    %rax, %rbx
    andq    %rdx, %r11
    orq %rdx, %rbp
    xorq    %r13, %r11
    movq    %rbx, 56(%rsp)
    movq    208(%rsp), %rbx
    xorq    %rcx, %rbp
    movq    %r11, 192(%rsp)
    movq    %r11, %rcx
    movq    32(%rsp), %r11
    xorq    %r8, %r15
    movq    %r12, 184(%rsp)
    movq    112(%rsp), %rdx
    rolq    $8, %r15
    xorq    %rdi, %r14
    xorq    %r9, %rbx
    movq    %r15, %r13
    movq    %rbp, %rax
    rolq    $25, %rbx
    notq    %r15
    rolq    $18, %r14
    xorq    %r10, %r11
    movq    %rbx, %r12
    andq    %rbx, %r13
    rolq    $6, %r11
    xorq    %rsi, %rdx
    xorq    104(%rsp), %rax
    rolq    %rdx
    orq %r11, %r12
    xorq    136(%rsp), %rcx
    xorq    %rdx, %r12
    xorq    %r11, %r13
    movq    %rbp, 152(%rsp)
    movq    %r12, 32(%rsp)
    movq    176(%rsp), %r12
    movq    %r13, 160(%rsp)
    movq    %r15, %r13
    movq    128(%rsp), %rbp
    andq    %r14, %r13
    xorq    %rbx, %r13
    movq    %r14, %rbx
    orq %rdx, %rbx
    xorq    %r15, %rbx
    andq    %rdx, %r11
    movq    120(%rsp), %rdx
    movq    %rbx, 200(%rsp)
    xorq    %rbx, %rax
    xorq    %r14, %r11
    movq    144(%rsp), %rbx
    xorq    %r11, %rcx
    xorq    %rsi, %r12
    movq    64(%rsp), %r14
    movq    %r11, 208(%rsp)
    xorq    %rdi, %rbp
    xorq    24(%rsp), %rdx
    rolq    $10, %r12
    rolq    $36, %rbp
    movq    72(%rsp), %r11
    movq    %r12, %r15
    xorq    %r8, %rbx
    andq    %rbp, %r15
    rolq    $27, %rbx
    xorq    %r10, %r14
    xorq    96(%rsp), %r10
    xorq    32(%rsp), %rdx
    xorq    %rbx, %r15
    rolq    $15, %r14
    xorq    %r9, %r11
    movq    %r15, 72(%rsp)
    xorq    168(%rsp), %r9
    rolq    $56, %r11
    rolq    $62, %r10
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 64(%rsp)
    orq %r11, %r14
    orq %r12, %r15
    xorq    %r12, %r14
    movq    %r11, %r12
    xorq    %rbp, %r15
    andq    %rbx, %r12
    orq %rbx, %rbp
    movq    %r14, 128(%rsp)
    xorq    64(%rsp), %r12
    xorq    %r11, %rbp
    xorq    %rbp, %rcx
    movq    %rbp, 64(%rsp)
    xorq    %r12, %rax
    rolq    $55, %r9
    xorq    88(%rsp), %rdi
    xorq    224(%rsp), %r8
    movq    %r9, %r11
    notq    %r11
    xorq    40(%rsp), %rsi
    movq    %r11, %r14
    rolq    $41, %rdi
    rolq    $39, %r8
    movq    %rdi, %rbx
    orq %r8, %rbx
    rolq    $2, %rsi
    andq    %r8, %r14
    xorq    %r11, %rbx
    movq    160(%rsp), %r11
    movq    %rsi, %rbp
    movq    %rbx, 40(%rsp)
    orq %r10, %rbp
    xorq    %r10, %r14
    xorq    %rdi, %rbp
    andq    %r9, %r10
    xorq    %r14, %rdx
    xorq    %rbp, %rax
    xorq    %rsi, %r10
    xorq    184(%rsp), %r11
    movq    %rax, %r9
    xorq    %r10, %rcx
    rolq    %r9
    xorq    %r15, %r11
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    andq    %rdi, %rbx
    xorq    48(%rsp), %r11
    xorq    %r8, %rbx
    movq    80(%rsp), %r8
    movq    %r11, %rdi
    xorq    %rbx, %r8
    rolq    %rdi
    xorq    56(%rsp), %r8
    xorq    %rcx, %rdi
    xorq    %r13, %r8
    xorq    128(%rsp), %r8
    movq    %r8, %rsi
    rolq    %rsi
    xorq    %rdx, %rsi
    xorq    %r11, %r9
    rolq    %rcx
    xorq    %r8, %rcx
    rolq    %rdx
    movq    24(%rsp), %r8
    xorq    %rax, %rdx
    movq    184(%rsp), %rax
    xorq    %r9, %r13
    movabsq $-9223372036854775669, %r11
    xorq    %rcx, %r12
    xorq    %rdx, %r10
    rolq    $43, %r13
    rolq    $21, %r12
    xorq    %rsi, %r15
    rolq    $14, %r10
    xorq    %rdi, %r8
    rolq    $45, %r15
    xorq    %r8, %r11
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %r11, 24(%rsp)
    movq    %r13, %r11
    orq %rax, %r11
    xorq    %r11, 24(%rsp)
    movq    %r13, %r11
    notq    %r11
    orq %r12, %r11
    xorq    %rax, %r11
    andq    %r8, %rax
    movq    %r11, 112(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    andq    %r12, %r11
    movq    %rax, 184(%rsp)
    movq    104(%rsp), %rax
    xorq    %r13, %r11
    movq    %r10, %r13
    movq    %r11, 88(%rsp)
    orq %r8, %r13
    movq    192(%rsp), %r11
    movq    32(%rsp), %r8
    xorq    %r12, %r13
    movq    %r15, %r12
    xorq    %rcx, %rax
    movq    %r13, 96(%rsp)
    rolq    $28, %rax
    xorq    %rdx, %r11
    xorq    %rdi, %r8
    rolq    $20, %r11
    rolq    $3, %r8
    xorq    %r9, %rbx
    xorq    %rdi, %r14
    rolq    $61, %rbx
    andq    %r8, %r12
    movq    %r8, %r10
    movq    %rbx, %r13
    xorq    %r11, %r12
    orq %r11, %r10
    notq    %r13
    andq    %rax, %r11
    xorq    %rax, %r10
    orq %r15, %r13
    xorq    %rbx, %r11
    movq    %r10, 104(%rsp)
    xorq    %r8, %r13
    movq    %rbx, %r8
    movq    64(%rsp), %rbx
    orq %rax, %r8
    movq    200(%rsp), %rax
    rolq    $18, %r14
    movq    56(%rsp), %r10
    movq    %r12, 144(%rsp)
    xorq    %r15, %r8
    movq    %r8, 192(%rsp)
    movq    48(%rsp), %r8
    xorq    %rdx, %rbx
    movq    %r11, 168(%rsp)
    xorq    %rcx, %rax
    rolq    $8, %rbx
    movq    %r13, 32(%rsp)
    rolq    $25, %rax
    xorq    %r9, %r10
    movq    %rbx, %r12
    rolq    $6, %r10
    andq    %rax, %r12
    notq    %rbx
    xorq    %r10, %r12
    movq    %rax, %r11
    xorq    %rsi, %r8
    movq    %r12, 64(%rsp)
    movq    %rbx, %r12
    rolq    %r8
    andq    %r14, %r12
    movq    %r14, %r13
    orq %r10, %r11
    xorq    %rax, %r12
    movq    192(%rsp), %rax
    orq %r8, %r13
    xorq    %rbx, %r13
    andq    %r8, %r10
    xorq    %r8, %r11
    movq    160(%rsp), %rbx
    movq    %r13, 176(%rsp)
    movq    136(%rsp), %r8
    movq    %r11, 56(%rsp)
    xorq    96(%rsp), %rax
    xorq    %r13, %rax
    xorq    %r14, %r10
    movq    128(%rsp), %r13
    movq    %r10, 200(%rsp)
    movq    120(%rsp), %r10
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    xorq    %rdx, %r8
    xorq    %rcx, %rbp
    movq    %rbx, %r14
    rolq    $27, %r8
    xorq    152(%rsp), %rcx
    xorq    %r9, %r13
    rolq    $56, %rbp
    xorq    208(%rsp), %rdx
    xorq    %rdi, %r10
    rolq    $15, %r13
    xorq    80(%rsp), %r9
    rolq    $36, %r10
    movq    %r13, %r11
    xorq    72(%rsp), %rdi
    andq    %r10, %r14
    orq %rbx, %r11
    xorq    40(%rsp), %rsi
    xorq    %r8, %r14
    rolq    $55, %rcx
    movq    %r14, 128(%rsp)
    movq    %r13, %r14
    movq    %rbp, %r13
    notq    %r14
    xorq    %r10, %r11
    andq    %r8, %r13
    movq    %r14, %r15
    rolq    $39, %rdx
    xorq    %r14, %r13
    orq %rbp, %r15
    rolq    $62, %r9
    xorq    %r13, %rax
    rolq    $41, %rdi
    xorq    %rbx, %r15
    movq    %r10, %rbx
    orq %r8, %rbx
    movq    %rcx, %r10
    movq    104(%rsp), %r8
    notq    %r10
    xorq    %rbp, %rbx
    rolq    $2, %rsi
    movq    %r10, %r14
    movq    %rdi, %rbp
    movq    %r15, 120(%rsp)
    andq    %rdx, %r14
    movq    %rsi, %r15
    xorq    %r9, %r14
    xorq    24(%rsp), %r8
    orq %rdx, %rbp
    xorq    %r10, %rbp
    andq    %rdi, %r15
    xorq    %rdx, %r15
    andq    %r9, %rcx
    xorq    %rsi, %rcx
    xorq    56(%rsp), %r8
    xorq    128(%rsp), %r8
    movq    %rbp, 40(%rsp)
    movq    64(%rsp), %r10
    movq    88(%rsp), %rdx
    xorq    %r14, %r8
    xorq    144(%rsp), %r10
    xorq    %r15, %rdx
    xorq    32(%rsp), %rdx
    xorq    %r11, %r10
    xorq    %rbp, %r10
    movq    %rsi, %rbp
    orq %r9, %rbp
    movq    168(%rsp), %r9
    xorq    %r12, %rdx
    xorq    112(%rsp), %r10
    xorq    %rdi, %rbp
    xorq    120(%rsp), %rdx
    xorq    %rbp, %rax
    movq    %rbp, 72(%rsp)
    movq    %rax, %rbp
    xorq    184(%rsp), %r9
    rolq    %rbp
    xorq    %r10, %rbp
    movq    %r10, %rdi
    movq    %rdx, %rsi
    rolq    %rdi
    rolq    %rsi
    xorq    200(%rsp), %r9
    xorq    %r8, %rsi
    xorq    %rbx, %r9
    xorq    %rcx, %r9
    movq    %r9, %r10
    xorq    %r9, %rdi
    movq    24(%rsp), %r9
    rolq    %r10
    xorq    %rdx, %r10
    movq    %r8, %rdx
    xorq    %rbp, %r12
    rolq    %rdx
    rolq    $43, %r12
    xorq    %r10, %r13
    xorq    %rax, %rdx
    movq    144(%rsp), %rax
    xorq    %rdi, %r9
    movabsq $-9223372036854742903, %r8
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    xorq    %r9, %r8
    rolq    $21, %r13
    xorq    %rbp, %r15
    movq    %r8, 24(%rsp)
    rolq    $14, %rcx
    rolq    $45, %r11
    xorq    %rsi, %rax
    rolq    $61, %r15
    rolq    $44, %rax
    movq    %rax, %r8
    orq %r12, %r8
    xorq    %r8, 24(%rsp)
    movq    %r12, %r8
    notq    %r8
    orq %r13, %r8
    xorq    %rax, %r8
    andq    %r9, %rax
    movq    %r8, 48(%rsp)
    movq    %rcx, %r8
    xorq    %rcx, %rax
    andq    %r13, %r8
    movq    %rax, 144(%rsp)
    movq    168(%rsp), %rax
    xorq    %r12, %r8
    movq    %rcx, %r12
    movq    96(%rsp), %rcx
    movq    %r8, 80(%rsp)
    movq    56(%rsp), %r8
    orq %r9, %r12
    xorq    %r13, %r12
    movq    %r11, %r9
    xorq    %rdx, %rax
    movq    %r12, 136(%rsp)
    movq    %r15, %r12
    xorq    %r10, %rcx
    rolq    $20, %rax
    notq    %r12
    xorq    %rdi, %r8
    rolq    $28, %rcx
    rolq    $3, %r8
    movq    %r8, %r13
    orq %rax, %r13
    andq    %r8, %r9
    orq %r11, %r12
    xorq    %rcx, %r13
    xorq    %rax, %r9
    andq    %rcx, %rax
    movq    %r13, 96(%rsp)
    movq    %r15, %r13
    xorq    %r15, %rax
    movq    %r9, 152(%rsp)
    orq %rcx, %r13
    movq    32(%rsp), %rcx
    movq    176(%rsp), %r9
    xorq    %r8, %r12
    xorq    %rdi, %r14
    movq    %rax, 160(%rsp)
    movq    %rdx, %rax
    movq    112(%rsp), %r8
    xorq    %rbx, %rax
    rolq    $18, %r14
    xorq    %r11, %r13
    rolq    $8, %rax
    xorq    %rbp, %rcx
    movq    %r12, 56(%rsp)
    xorq    %r10, %r9
    movq    %rax, %rbx
    rolq    $6, %rcx
    rolq    $25, %r9
    notq    %rax
    xorq    %rsi, %r8
    andq    %r9, %rbx
    movq    %r9, %r15
    rolq    %r8
    xorq    %rcx, %rbx
    orq %rcx, %r15
    movq    %r13, 168(%rsp)
    movq    %rbx, 176(%rsp)
    movq    %rax, %rbx
    xorq    %r8, %r15
    andq    %r14, %rbx
    movq    %r15, 32(%rsp)
    xorq    %r9, %rbx
    movq    %r14, %r9
    orq %r8, %r9
    andq    %rcx, %r8
    xorq    %rax, %r9
    xorq    %r14, %r8
    movq    %r9, 208(%rsp)
    movq    %r8, 224(%rsp)
    movq    184(%rsp), %rax
    movq    104(%rsp), %r13
    movq    24(%rsp), %r8
    movq    64(%rsp), %r11
    xorq    %rdx, %rax
    movq    120(%rsp), %r14
    xorq    %rdi, %r13
    rolq    $27, %rax
    movq    72(%rsp), %r9
    rolq    $36, %r13
    xorq    96(%rsp), %r8
    xorq    %rsi, %r11
    xorq    128(%rsp), %rdi
    rolq    $10, %r11
    xorq    %rbp, %r14
    xorq    200(%rsp), %rdx
    movq    %r11, %r12
    rolq    $15, %r14
    xorq    88(%rsp), %rbp
    andq    %r13, %r12
    xorq    %r15, %r8
    xorq    40(%rsp), %rsi
    xorq    %rax, %r12
    movq    %r11, %rcx
    movq    %r12, 72(%rsp)
    xorq    %r12, %r8
    movq    48(%rsp), %r12
    xorq    %r10, %r9
    orq %r14, %rcx
    xorq    192(%rsp), %r10
    notq    %r14
    rolq    $56, %r9
    movq    %r14, %r15
    rolq    $41, %rdi
    xorq    %r13, %rcx
    xorq    152(%rsp), %r12
    orq %r9, %r15
    rolq    $39, %rdx
    xorq    %r11, %r15
    rolq    $55, %r10
    movq    %r15, 64(%rsp)
    movq    %r9, %r11
    movq    %r13, %r15
    andq    %rax, %r11
    orq %rax, %r15
    movq    %r10, %rax
    xorq    176(%rsp), %r12
    notq    %rax
    xorq    %r14, %r11
    xorq    %r9, %r15
    movq    %rax, %r14
    rolq    $2, %rsi
    movq    %rdi, %r9
    rolq    $62, %rbp
    movq    %rsi, %r13
    xorq    %rcx, %r12
    andq    %rdx, %r14
    orq %rdx, %r9
    xorq    %rax, %r9
    movq    %rsi, %rax
    andq    %rdi, %r13
    orq %rbp, %rax
    xorq    %rdx, %r13
    movq    56(%rsp), %rdx
    xorq    %rdi, %rax
    xorq    %r9, %r12
    movq    %r9, 40(%rsp)
    movq    %rax, 88(%rsp)
    movq    168(%rsp), %rax
    xorq    %rbp, %r14
    movq    160(%rsp), %r9
    andq    %r10, %rbp
    movq    %r12, %rdi
    xorq    %rsi, %rbp
    xorq    %rbx, %rdx
    rolq    %rdi
    xorq    80(%rsp), %rdx
    xorq    %r14, %r8
    xorq    136(%rsp), %rax
    xorq    %rbp, %r9
    xorq    144(%rsp), %r9
    xorq    64(%rsp), %rdx
    xorq    %r11, %rax
    xorq    208(%rsp), %rax
    xorq    224(%rsp), %r9
    xorq    %r13, %rdx
    movq    %rdx, %rsi
    xorq    88(%rsp), %rax
    rolq    %rsi
    xorq    %r15, %r9
    xorq    %r8, %rsi
    xorq    %r9, %rdi
    rolq    %r9
    xorq    %rdx, %r9
    rolq    %r8
    movq    24(%rsp), %rdx
    movq    %rax, %r10
    rolq    %r10
    xorq    %r12, %r10
    xorq    %rax, %r8
    movq    152(%rsp), %rax
    xorq    %rdi, %rdx
    xorq    %r10, %rbx
    xorq    %r9, %r11
    movabsq $-9223372036854743037, %r12
    xorq    %r8, %rbp
    xorq    %r10, %r13
    rolq    $43, %rbx
    xorq    %rdx, %r12
    rolq    $21, %r11
    xorq    %rsi, %rax
    movq    %r12, 24(%rsp)
    movq    %rbx, %r12
    rolq    $44, %rax
    rolq    $14, %rbp
    xorq    %rsi, %rcx
    rolq    $61, %r13
    orq %rax, %r12
    rolq    $45, %rcx
    xorq    %r12, 24(%rsp)
    movq    %rbx, %r12
    notq    %r12
    orq %r11, %r12
    xorq    %rax, %r12
    andq    %rdx, %rax
    movq    %r12, 112(%rsp)
    movq    %rbp, %r12
    xorq    %rbp, %rax
    andq    %r11, %r12
    movq    %rax, 120(%rsp)
    movq    32(%rsp), %rax
    xorq    %rbx, %r12
    movq    %rbp, %rbx
    orq %rdx, %rbx
    movq    136(%rsp), %rdx
    movq    %r12, 128(%rsp)
    xorq    %r11, %rbx
    movq    160(%rsp), %r11
    movq    %rcx, %r12
    xorq    %rdi, %rax
    movq    %rbx, 104(%rsp)
    movq    %r13, %rbx
    rolq    $3, %rax
    notq    %rbx
    xorq    %r9, %rdx
    movq    %rax, %rbp
    xorq    %r8, %r11
    rolq    $28, %rdx
    rolq    $20, %r11
    orq %r11, %rbp
    xorq    %rdx, %rbp
    andq    %rax, %r12
    orq %rcx, %rbx
    movq    %rbp, 136(%rsp)
    xorq    %r11, %r12
    xorq    %rax, %rbx
    andq    %rdx, %r11
    movq    %r13, %rbp
    movq    %rbx, 32(%rsp)
    xorq    %r13, %r11
    movq    208(%rsp), %rbx
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    movq    %r11, 192(%rsp)
    movq    %r11, %rcx
    movq    56(%rsp), %r11
    xorq    %r8, %r15
    xorq    %rdi, %r14
    rolq    $8, %r15
    movq    48(%rsp), %rdx
    rolq    $18, %r14
    xorq    %r9, %rbx
    movq    %r15, %r13
    notq    %r15
    rolq    $25, %rbx
    movq    %rbp, %rax
    movq    %r12, 184(%rsp)
    xorq    %r10, %r11
    andq    %rbx, %r13
    xorq    104(%rsp), %rax
    rolq    $6, %r11
    xorq    %rsi, %rdx
    xorq    120(%rsp), %rcx
    xorq    %r11, %r13
    rolq    %rdx
    movq    %rbp, 152(%rsp)
    movq    %r13, 160(%rsp)
    movq    %r15, %r13
    movq    %rbx, %r12
    andq    %r14, %r13
    orq %r11, %r12
    andq    %rdx, %r11
    xorq    %rbx, %r13
    movq    %r14, %rbx
    xorq    %rdx, %r12
    orq %rdx, %rbx
    movq    136(%rsp), %rdx
    xorq    %r14, %r11
    xorq    %r15, %rbx
    movq    %r12, 56(%rsp)
    movq    176(%rsp), %r12
    movq    %rbx, 200(%rsp)
    xorq    %rbx, %rax
    movq    96(%rsp), %rbp
    xorq    %r11, %rcx
    movq    64(%rsp), %r14
    movq    %r11, 208(%rsp)
    movq    144(%rsp), %rbx
    movq    88(%rsp), %r11
    xorq    %r8, %rbx
    rolq    $27, %rbx
    xorq    24(%rsp), %rdx
    xorq    %rsi, %r12
    rolq    $10, %r12
    xorq    %rdi, %rbp
    xorq    72(%rsp), %rdi
    rolq    $36, %rbp
    movq    %r12, %r15
    xorq    224(%rsp), %r8
    xorq    %r10, %r14
    andq    %rbp, %r15
    xorq    80(%rsp), %r10
    xorq    56(%rsp), %rdx
    xorq    %rbx, %r15
    rolq    $15, %r14
    xorq    %r9, %r11
    movq    %r15, 88(%rsp)
    xorq    168(%rsp), %r9
    rolq    $56, %r11
    xorq    40(%rsp), %rsi
    rolq    $41, %rdi
    rolq    $39, %r8
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    orq %r12, %r15
    rolq    $55, %r9
    movq    %r14, 48(%rsp)
    orq %r11, %r14
    xorq    %rbp, %r15
    xorq    %r12, %r14
    movq    %r11, %r12
    orq %rbx, %rbp
    andq    %rbx, %r12
    xorq    %r11, %rbp
    movq    %rdi, %rbx
    xorq    48(%rsp), %r12
    movq    %r9, %r11
    rolq    $62, %r10
    xorq    %rbp, %rcx
    notq    %r11
    movq    %r14, 96(%rsp)
    movq    %rbp, 64(%rsp)
    movq    %r11, %r14
    xorq    %r12, %rax
    rolq    $2, %rsi
    orq %r8, %rbx
    xorq    %r11, %rbx
    andq    %r8, %r14
    movq    %rsi, %rbp
    movq    %rbx, 40(%rsp)
    movq    160(%rsp), %r11
    xorq    %r10, %r14
    orq %r10, %rbp
    andq    %r9, %r10
    xorq    %r14, %rdx
    xorq    %rdi, %rbp
    xorq    %rsi, %r10
    xorq    %r10, %rcx
    xorq    %rbp, %rax
    xorq    184(%rsp), %r11
    movq    %rax, %r9
    rolq    %r9
    xorq    %r15, %r11
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    andq    %rdi, %rbx
    xorq    112(%rsp), %r11
    xorq    %r8, %rbx
    movq    128(%rsp), %r8
    movq    %r11, %rdi
    xorq    %r11, %r9
    movabsq $-9223372036854743038, %r11
    xorq    %rbx, %r8
    rolq    %rdi
    xorq    32(%rsp), %r8
    xorq    %rcx, %rdi
    rolq    %rcx
    xorq    %r13, %r8
    xorq    96(%rsp), %r8
    movq    %r8, %rsi
    xorq    %r8, %rcx
    movq    24(%rsp), %r8
    rolq    %rsi
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    184(%rsp), %rax
    xorq    %rdi, %r8
    xorq    %rsi, %rax
    xorq    %r9, %r13
    xorq    %r8, %r11
    rolq    $43, %r13
    rolq    $44, %rax
    movq    %r11, 24(%rsp)
    movq    %r13, %r11
    xorq    %rcx, %r12
    xorq    %rdx, %r10
    orq %rax, %r11
    rolq    $21, %r12
    xorq    %rsi, %r15
    xorq    %r11, 24(%rsp)
    movq    %r13, %r11
    rolq    $14, %r10
    notq    %r11
    xorq    %r9, %rbx
    rolq    $61, %rbx
    orq %r12, %r11
    rolq    $45, %r15
    xorq    %rax, %r11
    andq    %r8, %rax
    movq    %r11, 48(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    andq    %r12, %r11
    movq    %rax, 184(%rsp)
    movq    104(%rsp), %rax
    xorq    %r13, %r11
    movq    %r10, %r13
    orq %r8, %r13
    movq    56(%rsp), %r8
    movq    %r11, 72(%rsp)
    movq    192(%rsp), %r11
    xorq    %r12, %r13
    movq    %r15, %r12
    movq    %r13, 80(%rsp)
    xorq    %rcx, %rax
    movq    %rbx, %r13
    rolq    $28, %rax
    notq    %r13
    xorq    %rdi, %r8
    rolq    $3, %r8
    xorq    %rdx, %r11
    rolq    $20, %r11
    movq    %r8, %r10
    andq    %r8, %r12
    orq %r11, %r10
    xorq    %r11, %r12
    xorq    %rax, %r10
    orq %r15, %r13
    andq    %rax, %r11
    xorq    %r8, %r13
    xorq    %rbx, %r11
    movq    %rbx, %r8
    movq    64(%rsp), %rbx
    orq %rax, %r8
    movq    %r10, 104(%rsp)
    movq    200(%rsp), %rax
    xorq    %r15, %r8
    xorq    %rdi, %r14
    movq    32(%rsp), %r10
    rolq    $18, %r14
    movq    %r13, 56(%rsp)
    movq    %r12, 144(%rsp)
    movq    %r14, %r13
    xorq    %rdx, %rbx
    movq    %r8, 192(%rsp)
    movq    112(%rsp), %r8
    xorq    %rcx, %rax
    rolq    $8, %rbx
    movq    %r11, 168(%rsp)
    rolq    $25, %rax
    xorq    %r9, %r10
    movq    %rbx, %r12
    rolq    $6, %r10
    andq    %rax, %r12
    notq    %rbx
    xorq    %r10, %r12
    movq    %rax, %r11
    xorq    %rsi, %r8
    movq    %r12, 64(%rsp)
    movq    %rbx, %r12
    rolq    %r8
    andq    %r14, %r12
    orq %r10, %r11
    andq    %r8, %r10
    xorq    %rax, %r12
    movq    192(%rsp), %rax
    xorq    %r14, %r10
    orq %r8, %r13
    xorq    %r8, %r11
    movq    %r10, 200(%rsp)
    xorq    %rbx, %r13
    movq    120(%rsp), %r8
    movq    %r11, 32(%rsp)
    movq    136(%rsp), %r10
    movq    %r13, 176(%rsp)
    xorq    80(%rsp), %rax
    movq    160(%rsp), %rbx
    xorq    %rdx, %r8
    xorq    %rdi, %r10
    rolq    $27, %r8
    xorq    %r13, %rax
    movq    96(%rsp), %r13
    rolq    $36, %r10
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    xorq    %rcx, %rbp
    movq    %rbx, %r14
    rolq    $56, %rbp
    xorq    %r9, %r13
    andq    %r10, %r14
    rolq    $15, %r13
    xorq    %r8, %r14
    movq    %r14, 96(%rsp)
    movq    %r13, %r14
    movq    %r13, %r11
    notq    %r14
    orq %rbx, %r11
    movq    %rbp, %r13
    movq    %r14, %r15
    andq    %r8, %r13
    xorq    152(%rsp), %rcx
    orq %rbp, %r15
    xorq    88(%rsp), %rdi
    xorq    %r10, %r11
    xorq    %rbx, %r15
    movq    %r10, %rbx
    xorq    208(%rsp), %rdx
    orq %r8, %rbx
    movq    104(%rsp), %r8
    xorq    %r14, %r13
    xorq    128(%rsp), %r9
    rolq    $55, %rcx
    xorq    %rbp, %rbx
    movq    %rcx, %r10
    xorq    40(%rsp), %rsi
    rolq    $41, %rdi
    notq    %r10
    rolq    $39, %rdx
    movq    %r15, 120(%rsp)
    xorq    24(%rsp), %r8
    movq    %r10, %r14
    movq    %rdi, %rbp
    rolq    $62, %r9
    andq    %rdx, %r14
    xorq    %r9, %r14
    orq %rdx, %rbp
    rolq    $2, %rsi
    xorq    %r13, %rax
    movq    %rsi, %r15
    xorq    32(%rsp), %r8
    xorq    96(%rsp), %r8
    xorq    %r14, %r8
    xorq    %r10, %rbp
    movq    64(%rsp), %r10
    movq    %rbp, 40(%rsp)
    andq    %r9, %rcx
    andq    %rdi, %r15
    xorq    %rdx, %r15
    movq    72(%rsp), %rdx
    xorq    %rsi, %rcx
    xorq    144(%rsp), %r10
    xorq    %r15, %rdx
    xorq    56(%rsp), %rdx
    xorq    %r11, %r10
    xorq    %rbp, %r10
    movq    %rsi, %rbp
    orq %r9, %rbp
    movq    168(%rsp), %r9
    xorq    48(%rsp), %r10
    xorq    %rdi, %rbp
    xorq    %r12, %rdx
    xorq    %rbp, %rax
    xorq    120(%rsp), %rdx
    movq    %rbp, 88(%rsp)
    movq    %rax, %rbp
    xorq    184(%rsp), %r9
    rolq    %rbp
    xorq    %r10, %rbp
    movq    %r10, %rdi
    movq    %rdx, %rsi
    rolq    %rdi
    rolq    %rsi
    xorq    200(%rsp), %r9
    xorq    %r8, %rsi
    xorq    %rbx, %r9
    xorq    %rcx, %r9
    movq    %r9, %r10
    xorq    %r9, %rdi
    movq    24(%rsp), %r9
    rolq    %r10
    xorq    %rdx, %r10
    movq    %r8, %rdx
    movabsq $-9223372036854775680, %r8
    rolq    %rdx
    xorq    %rax, %rdx
    movq    144(%rsp), %rax
    xorq    %rdi, %r9
    xorq    %rsi, %rax
    rolq    $44, %rax
    xorq    %rbp, %r12
    xorq    %r9, %r8
    rolq    $43, %r12
    movq    %r8, 24(%rsp)
    movq    %rax, %r8
    orq %r12, %r8
    xorq    %r10, %r13
    xorq    %rdx, %rcx
    xorq    %r8, 24(%rsp)
    movq    %r12, %r8
    rolq    $21, %r13
    notq    %r8
    rolq    $14, %rcx
    orq %r13, %r8
    xorq    %rsi, %r11
    xorq    %rbp, %r15
    rolq    $45, %r11
    xorq    %rax, %r8
    andq    %r9, %rax
    movq    %r8, 112(%rsp)
    movq    %rcx, %r8
    xorq    %rcx, %rax
    andq    %r13, %r8
    movq    %rax, 144(%rsp)
    rolq    $61, %r15
    xorq    %r12, %r8
    movq    168(%rsp), %rax
    movq    %rcx, %r12
    movq    %r8, 128(%rsp)
    movq    32(%rsp), %r8
    orq %r9, %r12
    movq    80(%rsp), %rcx
    xorq    %r13, %r12
    movq    %r11, %r9
    movq    %r12, 136(%rsp)
    movq    %r15, %r12
    xorq    %rdx, %rax
    notq    %r12
    xorq    %rdi, %r8
    rolq    $20, %rax
    orq %r11, %r12
    rolq    $3, %r8
    xorq    %r10, %rcx
    movq    %r8, %r13
    rolq    $28, %rcx
    andq    %r8, %r9
    orq %rax, %r13
    xorq    %rax, %r9
    xorq    %r8, %r12
    xorq    %rcx, %r13
    movq    %r13, 80(%rsp)
    movq    %r15, %r13
    movq    48(%rsp), %r8
    orq %rcx, %r13
    movq    %r9, 152(%rsp)
    andq    %rcx, %rax
    movq    176(%rsp), %r9
    xorq    %r15, %rax
    xorq    %rdi, %r14
    movq    56(%rsp), %rcx
    movq    %rax, 160(%rsp)
    movq    %rdx, %rax
    xorq    %rbx, %rax
    rolq    $18, %r14
    xorq    %rsi, %r8
    rolq    $8, %rax
    rolq    %r8
    xorq    %r11, %r13
    xorq    %r10, %r9
    movq    %rax, %rbx
    notq    %rax
    rolq    $25, %r9
    xorq    %rbp, %rcx
    movq    %r13, 168(%rsp)
    rolq    $6, %rcx
    andq    %r9, %rbx
    movq    %r9, %r15
    xorq    %rcx, %rbx
    orq %rcx, %r15
    movq    64(%rsp), %r11
    movq    %rbx, 56(%rsp)
    movq    %rax, %rbx
    xorq    %r8, %r15
    andq    %r14, %rbx
    movq    104(%rsp), %r13
    movq    %r12, 32(%rsp)
    xorq    %r9, %rbx
    movq    %r14, %r9
    movq    %r15, 48(%rsp)
    orq %r8, %r9
    andq    %rcx, %r8
    xorq    %rsi, %r11
    xorq    %rax, %r9
    xorq    %r14, %r8
    movq    184(%rsp), %rax
    movq    %r9, 176(%rsp)
    movq    120(%rsp), %r14
    xorq    %rdi, %r13
    movq    88(%rsp), %r9
    rolq    $36, %r13
    rolq    $10, %r11
    movq    %r8, 208(%rsp)
    movq    24(%rsp), %r8
    movq    %r11, %r12
    xorq    %rdx, %rax
    movq    %r11, %rcx
    xorq    %rbp, %r14
    rolq    $27, %rax
    xorq    %r10, %r9
    rolq    $15, %r14
    rolq    $56, %r9
    xorq    80(%rsp), %r8
    andq    %r13, %r12
    xorq    %rax, %r12
    xorq    192(%rsp), %r10
    orq %r14, %rcx
    movq    %r12, 88(%rsp)
    notq    %r14
    xorq    96(%rsp), %rdi
    xorq    200(%rsp), %rdx
    xorq    %r13, %rcx
    xorq    %r15, %r8
    movq    %r14, %r15
    xorq    72(%rsp), %rbp
    xorq    %r12, %r8
    movq    112(%rsp), %r12
    orq %r9, %r15
    xorq    %r11, %r15
    xorq    40(%rsp), %rsi
    rolq    $55, %r10
    movq    %r15, 64(%rsp)
    movq    %r9, %r11
    movq    %r13, %r15
    andq    %rax, %r11
    orq %rax, %r15
    movq    %r10, %rax
    xorq    152(%rsp), %r12
    rolq    $41, %rdi
    notq    %rax
    rolq    $39, %rdx
    xorq    %r14, %r11
    xorq    %r9, %r15
    movq    %rax, %r14
    movq    %rdi, %r9
    rolq    $2, %rsi
    rolq    $62, %rbp
    andq    %rdx, %r14
    xorq    56(%rsp), %r12
    orq %rdx, %r9
    xorq    %rbp, %r14
    xorq    %rax, %r9
    movq    %rsi, %r13
    movq    %rsi, %rax
    xorq    %r14, %r8
    movq    %r9, 40(%rsp)
    xorq    %rcx, %r12
    xorq    %r9, %r12
    andq    %rdi, %r13
    orq %rbp, %rax
    xorq    %rdi, %rax
    xorq    %rdx, %r13
    movq    32(%rsp), %rdx
    movq    %rax, 72(%rsp)
    movq    168(%rsp), %rax
    andq    %r10, %rbp
    movq    160(%rsp), %r9
    xorq    %rsi, %rbp
    movq    %r12, %rdi
    rolq    %rdi
    xorq    %rbx, %rdx
    xorq    136(%rsp), %rax
    xorq    128(%rsp), %rdx
    xorq    %rbp, %r9
    xorq    144(%rsp), %r9
    xorq    %r11, %rax
    xorq    64(%rsp), %rdx
    xorq    176(%rsp), %rax
    xorq    208(%rsp), %r9
    xorq    %r13, %rdx
    xorq    72(%rsp), %rax
    movq    %rdx, %rsi
    xorq    %r15, %r9
    rolq    %rsi
    xorq    %r9, %rdi
    rolq    %r9
    xorq    %r8, %rsi
    xorq    %rdx, %r9
    rolq    %r8
    movq    24(%rsp), %rdx
    xorq    %rax, %r8
    movq    %rax, %r10
    movq    152(%rsp), %rax
    rolq    %r10
    xorq    %r12, %r10
    xorq    %rdi, %rdx
    xorq    %r10, %rbx
    xorq    %rsi, %rax
    movq    %rdx, %r12
    rolq    $43, %rbx
    rolq    $44, %rax
    xorq    %r9, %r11
    xorq    $32778, %r12
    movq    %r12, 96(%rsp)
    movq    %rbx, %r12
    rolq    $21, %r11
    orq %rax, %r12
    xorq    %r8, %rbp
    xorq    %r10, %r13
    xorq    %r12, 96(%rsp)
    movq    %rbx, %r12
    rolq    $14, %rbp
    notq    %r12
    rolq    $61, %r13
    orq %r11, %r12
    xorq    %rsi, %rcx
    xorq    %rax, %r12
    andq    %rdx, %rax
    rolq    $45, %rcx
    movq    %r12, 24(%rsp)
    movq    %rbp, %r12
    xorq    %rbp, %rax
    andq    %r11, %r12
    movq    %rax, 184(%rsp)
    movq    48(%rsp), %rax
    xorq    %rbx, %r12
    movq    %rbp, %rbx
    orq %rdx, %rbx
    movq    136(%rsp), %rdx
    movq    %r12, 104(%rsp)
    xorq    %r11, %rbx
    movq    160(%rsp), %r11
    movq    %rcx, %r12
    xorq    %rdi, %rax
    movq    %rbx, 120(%rsp)
    movq    %r13, %rbx
    rolq    $3, %rax
    notq    %rbx
    xorq    %r9, %rdx
    movq    %rax, %rbp
    orq %rcx, %rbx
    xorq    %r8, %r11
    rolq    $28, %rdx
    xorq    %rax, %rbx
    rolq    $20, %r11
    andq    %rax, %r12
    movq    %rbx, 48(%rsp)
    orq %r11, %rbp
    xorq    %r11, %r12
    movq    176(%rsp), %rbx
    xorq    %rdx, %rbp
    movq    %r12, 152(%rsp)
    movq    %rbp, 136(%rsp)
    movq    %r13, %rbp
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    movq    %rbp, %rax
    movq    %rbp, 192(%rsp)
    movq    80(%rsp), %rbp
    xorq    120(%rsp), %rax
    andq    %rdx, %r11
    xorq    %r8, %r15
    xorq    %r13, %r11
    xorq    %r9, %rbx
    movq    112(%rsp), %rdx
    movq    %r11, 160(%rsp)
    movq    %r11, %rcx
    movq    32(%rsp), %r11
    rolq    $8, %r15
    rolq    $25, %rbx
    xorq    %rdi, %r14
    movq    %r15, %r13
    notq    %r15
    rolq    $18, %r14
    andq    %rbx, %r13
    xorq    %rsi, %rdx
    movq    %rbx, %r12
    xorq    %r10, %r11
    rolq    %rdx
    xorq    184(%rsp), %rcx
    rolq    $6, %r11
    xorq    %rdi, %rbp
    xorq    %r11, %r13
    orq %r11, %r12
    andq    %rdx, %r11
    movq    %r13, 200(%rsp)
    movq    %r15, %r13
    xorq    %rdx, %r12
    andq    %r14, %r13
    xorq    %r14, %r11
    movq    %r12, 176(%rsp)
    xorq    %rbx, %r13
    movq    %r14, %rbx
    movq    56(%rsp), %r12
    orq %rdx, %rbx
    movq    64(%rsp), %r14
    rolq    $36, %rbp
    xorq    %r15, %rbx
    movq    136(%rsp), %rdx
    xorq    %r11, %rcx
    movq    %rbx, 224(%rsp)
    xorq    %rbx, %rax
    movq    144(%rsp), %rbx
    xorq    %rsi, %r12
    movq    %r11, 232(%rsp)
    movq    72(%rsp), %r11
    xorq    %r10, %r14
    rolq    $10, %r12
    movq    %r12, %r15
    xorq    %r8, %rbx
    rolq    $27, %rbx
    rolq    $15, %r14
    xorq    96(%rsp), %rdx
    andq    %rbp, %r15
    xorq    %r9, %r11
    xorq    %rbx, %r15
    rolq    $56, %r11
    movq    %r15, 72(%rsp)
    xorq    176(%rsp), %rdx
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 32(%rsp)
    orq %r11, %r14
    orq %r12, %r15
    xorq    %r12, %r14
    xorq    %rbp, %r15
    movq    %r11, %r12
    movq    %r14, 80(%rsp)
    xorq    168(%rsp), %r9
    orq %rbx, %rbp
    xorq    88(%rsp), %rdi
    xorq    %r11, %rbp
    andq    %rbx, %r12
    xorq    208(%rsp), %r8
    xorq    %rbp, %rcx
    movq    %rbp, 64(%rsp)
    xorq    128(%rsp), %r10
    rolq    $55, %r9
    xorq    32(%rsp), %r12
    rolq    $41, %rdi
    movq    %r9, %r11
    xorq    40(%rsp), %rsi
    rolq    $39, %r8
    movq    %rdi, %rbx
    notq    %r11
    orq %r8, %rbx
    rolq    $62, %r10
    xorq    %r11, %rbx
    movq    %r11, %r14
    movq    200(%rsp), %r11
    andq    %r8, %r14
    rolq    $2, %rsi
    xorq    %r12, %rax
    xorq    %r10, %r14
    movq    %rbx, 40(%rsp)
    movq    %rsi, %rbp
    xorq    %r14, %rdx
    xorq    152(%rsp), %r11
    orq %r10, %rbp
    andq    %r9, %r10
    xorq    %rdi, %rbp
    xorq    %rsi, %r10
    xorq    %r10, %rcx
    xorq    %rbp, %rax
    movq    %rax, %r9
    xorq    %r15, %r11
    rolq    %r9
    xorq    %rbx, %r11
    movq    %rsi, %rbx
    andq    %rdi, %rbx
    xorq    24(%rsp), %r11
    xorq    %r8, %rbx
    movq    104(%rsp), %r8
    movq    %r11, %rdi
    xorq    %r11, %r9
    movabsq $-9223372034707292150, %r11
    xorq    %rbx, %r8
    rolq    %rdi
    xorq    48(%rsp), %r8
    xorq    %rcx, %rdi
    rolq    %rcx
    xorq    %r13, %r8
    xorq    %r9, %r13
    xorq    80(%rsp), %r8
    rolq    $43, %r13
    movq    %r8, %rsi
    xorq    %r8, %rcx
    movq    96(%rsp), %r8
    rolq    %rsi
    xorq    %rcx, %r12
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    152(%rsp), %rax
    rolq    $21, %r12
    xorq    %rdi, %r8
    xorq    %rsi, %rax
    rolq    $44, %rax
    xorq    %rdx, %r10
    xorq    %r8, %r11
    movq    %r11, 112(%rsp)
    movq    %r13, %r11
    rolq    $14, %r10
    orq %rax, %r11
    xorq    %r9, %rbx
    xorq    %rsi, %r15
    xorq    %r11, 112(%rsp)
    movq    %r13, %r11
    rolq    $61, %rbx
    notq    %r11
    rolq    $45, %r15
    orq %r12, %r11
    xorq    %rax, %r11
    andq    %r8, %rax
    movq    %r11, 32(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    andq    %r12, %r11
    movq    %rax, 96(%rsp)
    movq    120(%rsp), %rax
    xorq    %r13, %r11
    movq    %r10, %r13
    orq %r8, %r13
    movq    176(%rsp), %r8
    movq    %r11, 88(%rsp)
    xorq    %r12, %r13
    movq    160(%rsp), %r11
    movq    %r15, %r12
    movq    %r13, 128(%rsp)
    movq    %rbx, %r13
    xorq    %rcx, %rax
    notq    %r13
    rolq    $28, %rax
    xorq    %rdi, %r8
    orq %r15, %r13
    rolq    $3, %r8
    xorq    %rdx, %r11
    xorq    %r8, %r13
    andq    %r8, %r12
    movq    %r8, %r10
    movq    %rbx, %r8
    rolq    $20, %r11
    movq    %r13, 56(%rsp)
    orq %rax, %r8
    xorq    %r11, %r12
    orq %r11, %r10
    xorq    %r15, %r8
    andq    %rax, %r11
    xorq    %rax, %r10
    xorq    %rbx, %r11
    movq    %r8, 152(%rsp)
    movq    64(%rsp), %rbx
    movq    24(%rsp), %r8
    movq    %r10, 120(%rsp)
    movq    224(%rsp), %rax
    movq    %r12, 144(%rsp)
    movq    48(%rsp), %r10
    movq    %r11, 168(%rsp)
    xorq    %rsi, %r8
    xorq    %rdx, %rbx
    xorq    %rdi, %r14
    xorq    %rcx, %rax
    rolq    $8, %rbx
    xorq    %rcx, %rbp
    rolq    $25, %rax
    xorq    %r9, %r10
    rolq    $18, %r14
    rolq    $6, %r10
    movq    %rax, %r11
    movq    %rbx, %r12
    notq    %rbx
    rolq    %r8
    orq %r10, %r11
    andq    %rax, %r12
    movq    %rbx, %r13
    movq    %r14, %r15
    xorq    %r8, %r11
    xorq    %r10, %r12
    andq    %r14, %r13
    orq %r8, %r15
    andq    %r8, %r10
    movq    %r11, 64(%rsp)
    xorq    %rbx, %r15
    xorq    %rax, %r13
    xorq    %r14, %r10
    movq    152(%rsp), %rax
    movq    %r10, 208(%rsp)
    rolq    $56, %rbp
    movq    200(%rsp), %rbx
    movq    %r12, 176(%rsp)
    movq    136(%rsp), %r10
    movq    %r15, 160(%rsp)
    movq    184(%rsp), %r8
    movq    80(%rsp), %r12
    xorq    128(%rsp), %rax
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    xorq    %rdi, %r10
    rolq    $36, %r10
    xorq    %rdx, %r8
    movq    %rbx, %r11
    rolq    $27, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    %r15, %rax
    rolq    $15, %r12
    xorq    %r8, %r11
    movq    %r11, 80(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbx, %r11
    xorq    72(%rsp), %rdi
    orq %rbp, %r14
    xorq    192(%rsp), %rcx
    xorq    %r10, %r11
    xorq    %rbx, %r14
    movq    %rbp, %rbx
    xorq    232(%rsp), %rdx
    andq    %r8, %rbx
    movq    %r14, 136(%rsp)
    xorq    40(%rsp), %rsi
    xorq    %r12, %rbx
    movq    %r10, %r12
    xorq    104(%rsp), %r9
    orq %r8, %r12
    movq    120(%rsp), %r8
    rolq    $41, %rdi
    rolq    $55, %rcx
    rolq    $39, %rdx
    movq    %rdi, %r15
    movq    %rcx, %r10
    orq %rdx, %r15
    rolq    $2, %rsi
    notq    %r10
    rolq    $62, %r9
    xorq    %rbp, %r12
    xorq    112(%rsp), %r8
    xorq    %r10, %r15
    movq    %r10, %r14
    movq    176(%rsp), %r10
    andq    %rdx, %r14
    movq    %r15, 40(%rsp)
    xorq    %r9, %r14
    movq    %rsi, %rbp
    xorq    %rbx, %rax
    xorq    64(%rsp), %r8
    xorq    144(%rsp), %r10
    xorq    80(%rsp), %r8
    xorq    %r11, %r10
    xorq    %r15, %r10
    movq    %rsi, %r15
    andq    %rdi, %r15
    xorq    32(%rsp), %r10
    xorq    %r14, %r8
    xorq    %rdx, %r15
    movq    88(%rsp), %rdx
    orq %r9, %rbp
    andq    %r9, %rcx
    movq    168(%rsp), %r9
    xorq    %rdi, %rbp
    xorq    %rsi, %rcx
    xorq    %rbp, %rax
    movq    %rbp, 72(%rsp)
    movq    %r10, %rdi
    xorq    %r15, %rdx
    movq    %rax, %rbp
    rolq    %rdi
    xorq    96(%rsp), %r9
    rolq    %rbp
    xorq    56(%rsp), %rdx
    xorq    %r10, %rbp
    xorq    208(%rsp), %r9
    xorq    %r13, %rdx
    xorq    %rbp, %r13
    xorq    136(%rsp), %rdx
    rolq    $43, %r13
    xorq    %r12, %r9
    xorq    %rcx, %r9
    movq    %r9, %r10
    movq    %rdx, %rsi
    xorq    %r9, %rdi
    rolq    %r10
    movq    112(%rsp), %r9
    rolq    %rsi
    xorq    %rdx, %r10
    movq    %r8, %rdx
    xorq    %r8, %rsi
    movabsq $-9223372034707259263, %r8
    rolq    %rdx
    xorq    %r10, %rbx
    xorq    %rax, %rdx
    movq    144(%rsp), %rax
    rolq    $21, %rbx
    xorq    %rdi, %r9
    xorq    %rdx, %rcx
    rolq    $14, %rcx
    xorq    %rsi, %rax
    rolq    $44, %rax
    xorq    %r9, %r8
    xorq    %rbp, %r15
    movq    %r8, 24(%rsp)
    movq    %rax, %r8
    rolq    $61, %r15
    orq %r13, %r8
    xorq    %rsi, %r11
    xorq    %r8, 24(%rsp)
    movq    %r13, %r8
    rolq    $45, %r11
    notq    %r8
    orq %rbx, %r8
    xorq    %rax, %r8
    andq    %r9, %rax
    movq    %r8, 48(%rsp)
    movq    %rcx, %r8
    xorq    %rcx, %rax
    andq    %rbx, %r8
    movq    %rax, 144(%rsp)
    movq    168(%rsp), %rax
    xorq    %r13, %r8
    movq    %rcx, %r13
    movq    128(%rsp), %rcx
    movq    %r8, 104(%rsp)
    movq    64(%rsp), %r8
    orq %r9, %r13
    xorq    %rbx, %r13
    movq    %r11, %r9
    xorq    %rdx, %rax
    movq    %r13, 184(%rsp)
    movq    %r15, %r13
    rolq    $20, %rax
    xorq    %r10, %rcx
    notq    %r13
    xorq    %rdi, %r8
    rolq    $28, %rcx
    orq %r11, %r13
    rolq    $3, %r8
    movq    %r8, %rbx
    andq    %r8, %r9
    xorq    %r8, %r13
    orq %rax, %rbx
    xorq    %rax, %r9
    movq    32(%rsp), %r8
    xorq    %rcx, %rbx
    movq    %r9, 64(%rsp)
    andq    %rcx, %rax
    movq    160(%rsp), %r9
    xorq    %r15, %rax
    movq    %r13, 112(%rsp)
    movq    %rbx, 128(%rsp)
    movq    %r15, %rbx
    orq %rcx, %rbx
    movq    56(%rsp), %rcx
    xorq    %rsi, %r8
    xorq    %r11, %rbx
    rolq    %r8
    movq    %rax, 168(%rsp)
    movq    %rdx, %rax
    movq    %rbx, 192(%rsp)
    movq    120(%rsp), %rbx
    xorq    %rbp, %rcx
    xorq    %r10, %r9
    xorq    %rdi, %r14
    rolq    $25, %r9
    rolq    $6, %rcx
    xorq    %r12, %rax
    rolq    $18, %r14
    movq    %r9, %r11
    rolq    $8, %rax
    orq %rcx, %r11
    movq    %r14, %r13
    movq    %rax, %r12
    xorq    %r8, %r11
    orq %r8, %r13
    andq    %rcx, %r8
    xorq    %r14, %r8
    movq    %r11, 32(%rsp)
    andq    %r9, %r12
    movq    %r8, 200(%rsp)
    movq    24(%rsp), %r8
    xorq    %rcx, %r12
    movq    176(%rsp), %r11
    notq    %rax
    movq    %r12, 56(%rsp)
    xorq    %rax, %r13
    movq    %rax, %r12
    movq    96(%rsp), %rax
    andq    %r14, %r12
    movq    136(%rsp), %r14
    xorq    %rdi, %rbx
    xorq    128(%rsp), %r8
    rolq    $36, %rbx
    xorq    %r9, %r12
    xorq    %rsi, %r11
    movq    72(%rsp), %rcx
    movq    %r13, 160(%rsp)
    rolq    $10, %r11
    movq    48(%rsp), %r13
    xorq    %rdx, %rax
    movq    %r11, %r15
    rolq    $27, %rax
    xorq    %rbp, %r14
    xorq    32(%rsp), %r8
    andq    %rbx, %r15
    rolq    $15, %r14
    xorq    %rax, %r15
    xorq    %r10, %rcx
    movq    %r11, %r9
    rolq    $56, %rcx
    movq    %r15, 96(%rsp)
    xorq    %r15, %r8
    orq %r14, %r9
    xorq    64(%rsp), %r13
    notq    %r14
    xorq    %rbx, %r9
    movq    %r14, %r15
    orq %rax, %rbx
    orq %rcx, %r15
    xorq    %rcx, %rbx
    xorq    %r11, %r15
    xorq    56(%rsp), %r13
    movq    %rcx, %r11
    movq    %r15, 120(%rsp)
    xorq    152(%rsp), %r10
    andq    %rax, %r11
    xorq    40(%rsp), %rsi
    xorq    %r14, %r11
    movq    %rbx, 136(%rsp)
    xorq    80(%rsp), %rdi
    xorq    208(%rsp), %rdx
    xorq    %r9, %r13
    rolq    $55, %r10
    xorq    88(%rsp), %rbp
    rolq    $2, %rsi
    movq    %r10, %rax
    rolq    $41, %rdi
    notq    %rax
    movq    %rsi, %r15
    rolq    $39, %rdx
    andq    %rdi, %r15
    movq    %rax, %r14
    movq    %rdi, %rcx
    xorq    %rdx, %r15
    andq    %rdx, %r14
    orq %rdx, %rcx
    movq    112(%rsp), %rdx
    rolq    $62, %rbp
    xorq    %rax, %rcx
    xorq    %rbp, %r14
    movq    %rsi, %rbx
    xorq    %rcx, %r13
    xorq    %r14, %r8
    movq    192(%rsp), %rax
    movq    %rcx, 88(%rsp)
    xorq    %r12, %rdx
    xorq    104(%rsp), %rdx
    xorq    120(%rsp), %rdx
    xorq    %r15, %rdx
    orq %rbp, %rbx
    andq    %r10, %rbp
    movq    168(%rsp), %r10
    xorq    %rsi, %rbp
    xorq    %rdi, %rbx
    xorq    184(%rsp), %rax
    movq    %r13, %rdi
    movq    %rdx, %rcx
    rolq    %rdi
    movq    %rbx, 80(%rsp)
    movabsq $-9223372036854742912, %rsi
    rolq    %rcx
    xorq    %rbp, %r10
    xorq    %r8, %rcx
    rolq    %r8
    xorq    144(%rsp), %r10
    xorq    %r11, %rax
    xorq    160(%rsp), %rax
    xorq    200(%rsp), %r10
    xorq    %rbx, %rax
    xorq    %rax, %r8
    movq    %rax, %rbx
    movq    64(%rsp), %rax
    rolq    %rbx
    xorq    %r8, %rbp
    xorq    136(%rsp), %r10
    xorq    %r13, %rbx
    rolq    $14, %rbp
    xorq    %rbx, %r12
    rolq    $43, %r12
    xorq    %rcx, %rax
    rolq    $44, %rax
    movq    %r12, %r13
    xorq    %r10, %rdi
    rolq    %r10
    notq    %r13
    xorq    %rdx, %r10
    movq    24(%rsp), %rdx
    xorq    %r10, %r11
    rolq    $21, %r11
    xorq    %rdi, %rdx
    xorq    %rdx, %rsi
    movq    %rsi, 24(%rsp)
    movq    %r12, %rsi
    orq %rax, %rsi
    xorq    %rsi, 24(%rsp)
    orq %r11, %r13
    movq    %rbp, %rsi
    xorq    %rax, %r13
    andq    %rdx, %rax
    xorq    %rbp, %rax
    andq    %r11, %rsi
    xorq    %rbx, %r15
    xorq    %r12, %rsi
    movq    %rax, 176(%rsp)
    movq    %rbp, %r12
    movq    32(%rsp), %rax
    movq    %rsi, 64(%rsp)
    orq %rdx, %r12
    movq    168(%rsp), %rsi
    rolq    $61, %r15
    xorq    %rcx, %r9
    movq    184(%rsp), %rdx
    rolq    $45, %r9
    xorq    %r11, %r12
    movq    %r15, %r11
    movq    %r13, 40(%rsp)
    movq    %r9, %rbp
    xorq    %rdi, %rax
    movq    %r12, 152(%rsp)
    notq    %r11
    rolq    $3, %rax
    xorq    %r8, %rsi
    movq    %r15, %r12
    xorq    %r10, %rdx
    rolq    $20, %rsi
    movq    %rax, %r13
    rolq    $28, %rdx
    orq %rsi, %r13
    andq    %rax, %rbp
    orq %r9, %r11
    orq %rdx, %r12
    xorq    %rdx, %r13
    xorq    %r9, %r12
    xorq    %rsi, %rbp
    xorq    %rax, %r11
    movq    %r13, 184(%rsp)
    andq    %rdx, %rsi
    movq    %r12, %rax
    movq    %rbp, 32(%rsp)
    xorq    %r15, %rsi
    movq    %r11, 72(%rsp)
    movq    %r12, 168(%rsp)
    movq    48(%rsp), %rdx
    movq    136(%rsp), %r11
    movq    %rsi, 208(%rsp)
    movq    112(%rsp), %r9
    movq    160(%rsp), %rbp
    xorq    %rcx, %rdx
    xorq    152(%rsp), %rax
    rolq    %rdx
    xorq    176(%rsp), %rsi
    xorq    %rbx, %r9
    xorq    %r8, %r11
    rolq    $8, %r11
    xorq    %r10, %rbp
    xorq    %rdi, %r14
    movq    %r11, %r12
    rolq    $25, %rbp
    movq    %r11, %r15
    notq    %r12
    rolq    $18, %r14
    andq    %rbp, %r15
    rolq    $6, %r9
    movq    %rbp, %r13
    movq    %r12, %r11
    xorq    %r9, %r15
    orq %r9, %r13
    andq    %r14, %r11
    andq    %rdx, %r9
    xorq    %rbp, %r11
    movq    %r14, %rbp
    xorq    %r14, %r9
    xorq    %rdx, %r13
    orq %rdx, %rbp
    movq    %r9, 224(%rsp)
    xorq    %r9, %rsi
    movq    56(%rsp), %r9
    movq    184(%rsp), %rdx
    movq    %r13, 48(%rsp)
    xorq    %r12, %rbp
    movq    120(%rsp), %r14
    movq    %rbp, 160(%rsp)
    xorq    %rbp, %rax
    movq    128(%rsp), %r13
    movq    %r15, 136(%rsp)
    movq    144(%rsp), %r12
    xorq    %rcx, %r9
    movq    80(%rsp), %rbp
    rolq    $10, %r9
    xorq    24(%rsp), %rdx
    movq    %r9, %r15
    xorq    %rbx, %r14
    xorq    %rdi, %r13
    rolq    $15, %r14
    rolq    $36, %r13
    xorq    %r8, %r12
    rolq    $27, %r12
    xorq    %r10, %rbp
    andq    %r13, %r15
    rolq    $56, %rbp
    xorq    %r12, %r15
    xorq    48(%rsp), %rdx
    movq    %r15, 120(%rsp)
    xorq    192(%rsp), %r10
    xorq    200(%rsp), %r8
    xorq    104(%rsp), %rbx
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    orq %r9, %r15
    rolq    $55, %r10
    movq    %r14, 56(%rsp)
    orq %rbp, %r14
    xorq    %r13, %r15
    xorq    %r9, %r14
    movq    %rbp, %r9
    orq %r12, %r13
    andq    %r12, %r9
    movq    96(%rsp), %r12
    xorq    %rbp, %r13
    movq    88(%rsp), %rbp
    rolq    $39, %r8
    movq    %r14, 80(%rsp)
    xorq    56(%rsp), %r9
    movq    %r13, 144(%rsp)
    xorq    %r13, %rsi
    rolq    $62, %rbx
    xorq    %rdi, %r12
    rolq    $41, %r12
    xorq    %rcx, %rbp
    movq    %r10, %rcx
    movq    %r12, %rdi
    notq    %rcx
    rolq    $2, %rbp
    orq %r8, %rdi
    movq    %rcx, %r14
    movq    %rbp, %r13
    xorq    %rcx, %rdi
    movq    64(%rsp), %rcx
    andq    %r8, %r14
    movq    %rdi, 96(%rsp)
    movq    136(%rsp), %rdi
    andq    %r12, %r13
    xorq    %rbx, %r14
    xorq    %r8, %r13
    xorq    %r9, %rax
    xorq    %r14, %rdx
    movq    %rbp, %r8
    xorq    32(%rsp), %rdi
    xorq    %r15, %rdi
    xorq    96(%rsp), %rdi
    xorq    40(%rsp), %rdi
    xorq    %r13, %rcx
    orq %rbx, %r8
    xorq    72(%rsp), %rcx
    andq    %r10, %rbx
    xorq    %r12, %r8
    xorq    %rbp, %rbx
    xorq    %r8, %rax
    xorq    %rbx, %rsi
    movq    %rax, %rbp
    movq    %rsi, %r10
    movq    %rdi, %r12
    rolq    %rbp
    xorq    %r11, %rcx
    rolq    %r10
    xorq    %rdi, %rbp
    xorq    80(%rsp), %rcx
    movq    %r8, 104(%rsp)
    rolq    %r12
    xorq    %rsi, %r12
    xorq    %rbp, %r11
    rolq    $43, %r11
    movl    $2147483649, %esi
    movq    %r11, %rdi
    movq    %rcx, %r8
    xorq    %rcx, %r10
    movq    24(%rsp), %rcx
    rolq    %r8
    xorq    %r10, %r9
    notq    %rdi
    xorq    %rdx, %r8
    rolq    %rdx
    xorq    %rax, %rdx
    movq    32(%rsp), %rax
    rolq    $21, %r9
    xorq    %r12, %rcx
    xorq    %rdx, %rbx
    orq %r9, %rdi
    xorq    %rcx, %rsi
    rolq    $14, %rbx
    movq    %rsi, 112(%rsp)
    movq    %r11, %rsi
    xorq    %r8, %rax
    rolq    $44, %rax
    orq %rax, %rsi
    xorq    %rsi, 112(%rsp)
    xorq    %rax, %rdi
    movq    %rbx, %rsi
    movq    %rdi, 24(%rsp)
    andq    %r9, %rsi
    andq    %rcx, %rax
    movq    %rbx, %rdi
    xorq    %r11, %rsi
    xorq    %rbx, %rax
    orq %rcx, %rdi
    movq    48(%rsp), %rcx
    movq    %rsi, 192(%rsp)
    movq    %rax, 200(%rsp)
    xorq    %r8, %r15
    xorq    %rbp, %r13
    movq    152(%rsp), %rax
    rolq    $45, %r15
    rolq    $61, %r13
    movq    208(%rsp), %rsi
    movq    %r15, %r11
    xorq    %r9, %rdi
    xorq    %r12, %rcx
    movq    %r13, %rbx
    movq    %rdi, 32(%rsp)
    rolq    $3, %rcx
    notq    %rbx
    movq    %r13, %rdi
    xorq    %r10, %rax
    andq    %rcx, %r11
    movq    %rcx, %r9
    xorq    %rdx, %rsi
    rolq    $28, %rax
    orq %r15, %rbx
    rolq    $20, %rsi
    xorq    %rcx, %rbx
    orq %rax, %rdi
    xorq    %rsi, %r11
    orq %rsi, %r9
    andq    %rax, %rsi
    xorq    %r13, %rsi
    xorq    %rax, %r9
    movq    160(%rsp), %rcx
    movq    40(%rsp), %rax
    movq    %rsi, 56(%rsp)
    xorq    %r15, %rdi
    movq    72(%rsp), %rsi
    movq    %r11, 152(%rsp)
    movq    144(%rsp), %r13
    movq    %r9, 88(%rsp)
    xorq    %r10, %rcx
    movq    %rbx, 48(%rsp)
    movq    %rdi, %rbx
    xorq    %r8, %rax
    rolq    $25, %rcx
    movq    %rdi, 128(%rsp)
    xorq    %rbp, %rsi
    rolq    %rax
    movq    %rcx, %r9
    rolq    $6, %rsi
    xorq    %rdx, %r13
    xorq    %r12, %r14
    rolq    $8, %r13
    rolq    $18, %r14
    xorq    32(%rsp), %rbx
    movq    %r13, %r11
    notq    %r13
    movq    80(%rsp), %rdi
    andq    %rcx, %r11
    movq    %r13, %r15
    orq %rsi, %r9
    xorq    %rsi, %r11
    andq    %r14, %r15
    andq    %rax, %rsi
    xorq    %rcx, %r15
    xorq    %r14, %rsi
    movq    %r14, %rcx
    xorq    %rax, %r9
    orq %rax, %rcx
    movq    %rsi, 232(%rsp)
    movq    136(%rsp), %rsi
    xorq    %r13, %rcx
    xorq    %rbp, %rdi
    movq    %r9, 144(%rsp)
    xorq    %rcx, %rbx
    movq    104(%rsp), %rax
    movq    184(%rsp), %r9
    movq    %rcx, 40(%rsp)
    rolq    $15, %rdi
    movq    176(%rsp), %rcx
    movq    %r11, 160(%rsp)
    xorq    %r8, %rsi
    movq    %r15, 208(%rsp)
    movq    %rdi, %r15
    rolq    $10, %rsi
    xorq    %r10, %rax
    notq    %rdi
    xorq    %r12, %r9
    movq    %rsi, %r11
    rolq    $56, %rax
    rolq    $36, %r9
    xorq    %rdx, %rcx
    movq    %rdi, %r13
    rolq    $27, %rcx
    andq    %r9, %r11
    orq %rax, %r13
    xorq    %rcx, %r11
    xorq    %rsi, %r13
    orq %rsi, %r15
    movq    %r11, 136(%rsp)
    movq    %rax, %r11
    xorq    %r9, %r15
    andq    %rcx, %r11
    movq    %r13, 80(%rsp)
    movq    88(%rsp), %rsi
    xorq    %rdi, %r11
    xorq    120(%rsp), %r12
    orq %rcx, %r9
    xorq    168(%rsp), %r10
    xorq    %rax, %r9
    xorq    %r11, %rbx
    xorq    224(%rsp), %rdx
    movq    %r9, 72(%rsp)
    movq    96(%rsp), %rax
    rolq    $41, %r12
    movq    192(%rsp), %r9
    rolq    $55, %r10
    movq    %r12, %r14
    movq    64(%rsp), %rcx
    rolq    $39, %rdx
    movq    %r10, %rdi
    xorq    112(%rsp), %rsi
    xorq    %r8, %rax
    notq    %rdi
    rolq    $2, %rax
    orq %rdx, %r14
    movq    %rdi, %r8
    xorq    %rdi, %r14
    movq    %rax, %rdi
    xorq    %rbp, %rcx
    andq    %r12, %rdi
    movq    160(%rsp), %rbp
    rolq    $62, %rcx
    xorq    %rdx, %rdi
    xorq    144(%rsp), %rsi
    andq    %rdx, %r8
    xorq    %rdi, %r9
    movq    %r14, 120(%rsp)
    xorq    %rcx, %r8
    xorq    48(%rsp), %r9
    xorq    152(%rsp), %rbp
    xorq    136(%rsp), %rsi
    xorq    208(%rsp), %r9
    xorq    %r15, %rbp
    xorq    %r14, %rbp
    movq    %rax, %r14
    xorq    %r8, %rsi
    orq %rcx, %r14
    xorq    24(%rsp), %rbp
    xorq    %r13, %r9
    movq    56(%rsp), %r13
    xorq    %r12, %r14
    andq    %rcx, %r10
    xorq    %r14, %rbx
    movq    152(%rsp), %r12
    xorq    %rax, %r10
    movq    %r9, %rax
    movq    %rbx, %rdx
    rolq    %rax
    movq    %rbp, %rcx
    rolq    %rdx
    xorq    200(%rsp), %r13
    rolq    %rcx
    xorq    %rsi, %rax
    rolq    %rsi
    xorq    %rbp, %rdx
    movabsq $-9223372034707259384, %rbp
    xorq    %rbx, %rsi
    xorq    %rax, %r12
    movq    112(%rsp), %rbx
    rolq    $44, %r12
    xorq    232(%rsp), %r13
    xorq    72(%rsp), %r13
    xorq    %r10, %r13
    xorq    %rsi, %r10
    xorq    %r13, %rcx
    rolq    %r13
    xorq    %r9, %r13
    movq    32(%rsp), %r9
    xorq    %rcx, %rbx
    movq    %rbx, 112(%rsp)
    movq    208(%rsp), %rbx
    xorq    %r13, %r11
    rolq    $14, %r10
    rolq    $21, %r11
    xorq    112(%rsp), %rbp
    xorq    %r13, %r9
    rolq    $28, %r9
    xorq    %rdx, %rbx
    movq    %r9, 32(%rsp)
    movq    56(%rsp), %r9
    rolq    $43, %rbx
    xorq    %rsi, %r9
    rolq    $20, %r9
    movq    %r9, 56(%rsp)
    movq    144(%rsp), %r9
    xorq    %rcx, %r9
    rolq    $3, %r9
    xorq    %rax, %r15
    xorq    %rdx, %rdi
    rolq    $45, %r15
    rolq    $61, %rdi
    xorq    %rcx, %r8
    movq    %r15, 96(%rsp)
    movq    24(%rsp), %r15
    rolq    $18, %r8
    movq    %rdi, 64(%rsp)
    xorq    %rax, %r15
    rolq    %r15
    movq    %r15, 24(%rsp)
    movq    48(%rsp), %rdi
    movq    40(%rsp), %r15
    movq    %r8, 104(%rsp)
    movq    88(%rsp), %r8
    xorq    %rdx, %rdi
    rolq    $6, %rdi
    xorq    %r13, %r15
    movq    %rdi, 48(%rsp)
    movq    72(%rsp), %rdi
    rolq    $25, %r15
    movq    %r15, 40(%rsp)
    movq    200(%rsp), %r15
    xorq    %rcx, %r8
    rolq    $36, %r8
    xorq    136(%rsp), %rcx
    movq    %r8, 88(%rsp)
    movq    160(%rsp), %r8
    xorq    %rsi, %rdi
    rolq    $8, %rdi
    xorq    %rsi, %r15
    xorq    232(%rsp), %rsi
    movq    %rdi, 72(%rsp)
    notq    %rdi
    rolq    $27, %r15
    movq    %rdi, 184(%rsp)
    movq    80(%rsp), %rdi
    xorq    %rax, %r8
    rolq    $10, %r8
    rolq    $41, %rcx
    rolq    $39, %rsi
    xorq    %rdx, %rdi
    xorq    192(%rsp), %rdx
    rolq    $15, %rdi
    movq    %rdi, 80(%rsp)
    movq    %r13, %rdi
    xorq    128(%rsp), %r13
    xorq    %r14, %rdi
    movq    80(%rsp), %r14
    rolq    $56, %rdi
    rolq    $62, %rdx
    rolq    $55, %r13
    xorq    120(%rsp), %rax
    notq    %r14
    movq    %r13, 128(%rsp)
    notq    %r13
    movq    %r14, 144(%rsp)
    movq    %r12, %r14
    orq %rbx, %r14
    xorq    %r14, %rbp
    movq    216(%rsp), %r14
    rolq    $2, %rax
    movq    %rbp, (%r14)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r12, %rbp
    andq    112(%rsp), %r12
    movq    %rbp, 8(%r14)
    movq    %r10, %rbp
    andq    %r11, %rbp
    xorq    %rbx, %rbp
    movq    112(%rsp), %rbx
    xorq    %r10, %r12
    movq    %rbp, 16(%r14)
    movq    %r12, 32(%r14)
    orq %r10, %rbx
    movq    56(%rsp), %r10
    xorq    %r11, %rbx
    movq    %rbx, 24(%r14)
    orq %r9, %r10
    xorq    32(%rsp), %r10
    movq    %r10, 40(%r14)
    movq    96(%rsp), %r10
    andq    %r9, %r10
    xorq    56(%rsp), %r10
    movq    %r10, 48(%r14)
    movq    64(%rsp), %r10
    notq    %r10
    orq 96(%rsp), %r10
    xorq    %r9, %r10
    movq    64(%rsp), %r9
    movq    %r10, 56(%r14)
    orq 32(%rsp), %r9
    xorq    96(%rsp), %r9
    movq    %r9, 64(%r14)
    movq    56(%rsp), %r9
    andq    32(%rsp), %r9
    xorq    64(%rsp), %r9
    movq    %r9, 72(%r14)
    movq    40(%rsp), %r9
    orq 48(%rsp), %r9
    xorq    24(%rsp), %r9
    movq    %r9, 80(%r14)
    movq    72(%rsp), %r9
    andq    40(%rsp), %r9
    xorq    48(%rsp), %r9
    movq    %r9, 88(%r14)
    movq    184(%rsp), %r9
    andq    104(%rsp), %r9
    xorq    40(%rsp), %r9
    movq    %r9, 96(%r14)
    movq    104(%rsp), %r9
    orq 24(%rsp), %r9
    xorq    184(%rsp), %r9
    movq    %r9, 104(%r14)
    movq    24(%rsp), %r9
    andq    48(%rsp), %r9
    xorq    104(%rsp), %r9
    movq    %r9, 112(%r14)
    movq    88(%rsp), %r9
    andq    %r8, %r9
    xorq    %r15, %r9
    movq    %r9, 120(%r14)
    movq    80(%rsp), %r9
    orq %r8, %r9
    xorq    88(%rsp), %r9
    movq    %r9, 128(%r14)
    movq    144(%rsp), %r9
    orq %rdi, %r9
    xorq    %r8, %r9
    movq    %rdi, %r8
    andq    %r15, %r8
    movq    %r9, 136(%r14)
    xorq    144(%rsp), %r8
    movq    %r8, 144(%r14)
    movq    88(%rsp), %r8
    orq %r15, %r8
    xorq    %rdi, %r8
    movq    %r13, %rdi
    andq    %rsi, %rdi
    movq    %r8, 152(%r14)
    xorq    %rdx, %rdi
    movq    %rdi, 160(%r14)
    movq    %rcx, %rdi
    orq %rsi, %rdi
    xorq    %r13, %rdi
    movq    %rdi, 168(%r14)
    movq    %rax, %rdi
    andq    %rcx, %rdi
    xorq    %rsi, %rdi
    movq    %rax, %rsi
    orq %rdx, %rsi
    andq    128(%rsp), %rdx
    movq    %rdi, 176(%r14)
    xorq    %rcx, %rsi
    movq    %rsi, 184(%r14)
    xorq    %rax, %rdx
    movq    %rdx, 192(%r14)
    addq    $240, %rsp
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
    .cfi_endproc
.LFE31:
    .size   KeccakP1600_Permute_24rounds, .-KeccakP1600_Permute_24rounds
    .p2align 4,,15
    .globl  KeccakP1600_Permute_12rounds
    .type   KeccakP1600_Permute_12rounds, @function
KeccakP1600_Permute_12rounds:
.LFB32:
    .cfi_startproc
    pushq   %r15
    .cfi_def_cfa_offset 16
    .cfi_offset 15, -16
    pushq   %r14
    .cfi_def_cfa_offset 24
    .cfi_offset 14, -24
    pushq   %r13
    .cfi_def_cfa_offset 32
    .cfi_offset 13, -32
    pushq   %r12
    .cfi_def_cfa_offset 40
    .cfi_offset 12, -40
    pushq   %rbp
    .cfi_def_cfa_offset 48
    .cfi_offset 6, -48
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $88, %rsp
    .cfi_def_cfa_offset 144
    movq    48(%rdi), %r14
    movq    8(%rdi), %r15
    movq    56(%rdi), %rdx
    movq    (%rdi), %rbp
    movq    40(%rdi), %r9
    movq    72(%rdi), %r10
    xorq    %r14, %r15
    movq    96(%rdi), %r12
    xorq    16(%rdi), %rdx
    movq    24(%rdi), %rcx
    xorq    %rbp, %r9
    xorq    88(%rdi), %r15
    movq    64(%rdi), %rax
    xorq    32(%rdi), %r10
    xorq    %r12, %rdx
    xorq    80(%rdi), %r9
    xorq    128(%rdi), %r15
    xorq    136(%rdi), %rdx
    xorq    %rcx, %rax
    movq    144(%rdi), %rbx
    xorq    104(%rdi), %rax
    xorq    112(%rdi), %r10
    xorq    120(%rdi), %r9
    xorq    168(%rdi), %r15
    xorq    176(%rdi), %rdx
    xorq    %rbx, %rax
    movq    192(%rdi), %r13
    xorq    152(%rdi), %r10
    xorq    184(%rdi), %rax
    movq    %r15, %r8
    xorq    160(%rdi), %r9
    movq    %rdx, %rsi
    rolq    %r8
    rolq    %rsi
    xorq    %r13, %r10
    xorq    %r10, %r8
    movq    %rax, %r11
    rolq    %r10
    xorq    %r9, %rsi
    xorq    %rdx, %r10
    rolq    %r11
    movq    %rsi, %rdx
    xorq    %r15, %r11
    rolq    %r9
    xorq    %r14, %rdx
    xorq    %rax, %r9
    xorq    %r8, %rbp
    rolq    $44, %rdx
    xorq    %r11, %r12
    movq    %r9, %rax
    rolq    $43, %r12
    xorq    %r13, %rax
    movl    $2147516555, %r14d
    movq    %r12, %r13
    xorq    %rbp, %r14
    xorq    %r10, %rbx
    orq %rdx, %r13
    rolq    $21, %rbx
    xorq    %r10, %rcx
    rolq    $14, %rax
    xorq    %r13, %r14
    movq    %r12, %r13
    notq    %r13
    movq    %rax, %r15
    rolq    $28, %rcx
    orq %rbx, %r13
    andq    %rbx, %r15
    xorq    %rdx, %r13
    andq    %rbp, %rdx
    xorq    %r12, %r15
    xorq    %rax, %rdx
    movq    %r13, -120(%rsp)
    movq    %rax, %r12
    movq    %r15, -72(%rsp)
    orq %rbp, %r12
    movq    128(%rdi), %rax
    movq    %rdx, -32(%rsp)
    movq    176(%rdi), %rdx
    xorq    %rbx, %r12
    movq    80(%rdi), %rbp
    movq    %r12, -112(%rsp)
    movq    72(%rdi), %rbx
    xorq    %rsi, %rax
    xorq    %r11, %rdx
    rolq    $45, %rax
    rolq    $61, %rdx
    xorq    %r8, %rbp
    movq    %rax, %r15
    rolq    $3, %rbp
    xorq    %r9, %rbx
    movq    %rdx, %r12
    rolq    $20, %rbx
    movq    %rbp, %r13
    notq    %r12
    orq %rbx, %r13
    andq    %rbp, %r15
    orq %rax, %r12
    xorq    %rcx, %r13
    xorq    %rbx, %r15
    xorq    %rbp, %r12
    movq    %r13, -48(%rsp)
    movq    %rdx, %r13
    andq    %rcx, %rbx
    movq    %r12, -88(%rsp)
    movq    104(%rdi), %r12
    orq %rcx, %r13
    movq    56(%rdi), %rbp
    xorq    %rdx, %rbx
    xorq    %rax, %r13
    movq    8(%rdi), %rdx
    movq    %r13, -96(%rsp)
    movq    %r13, %rax
    movq    152(%rdi), %r13
    movq    %r15, -104(%rsp)
    movq    %rbx, %rcx
    xorq    %r10, %r12
    movq    %rbx, -56(%rsp)
    movq    160(%rdi), %rbx
    rolq    $25, %r12
    xorq    %r11, %rbp
    xorq    -112(%rsp), %rax
    rolq    $6, %rbp
    xorq    %rsi, %rdx
    xorq    -32(%rsp), %rcx
    movq    %r12, %r15
    rolq    %rdx
    xorq    %r9, %r13
    orq %rbp, %r15
    xorq    %r8, %rbx
    rolq    $8, %r13
    xorq    %rdx, %r15
    rolq    $18, %rbx
    movq    %r15, -24(%rsp)
    movq    %r13, %r15
    notq    %r13
    andq    %r12, %r15
    xorq    %rbp, %r15
    andq    %rdx, %rbp
    movq    %r15, -80(%rsp)
    movq    %r13, %r15
    xorq    %rbx, %rbp
    andq    %rbx, %r15
    movq    %rbp, -40(%rsp)
    xorq    %rbp, %rcx
    xorq    %r12, %r15
    movq    %rbx, %r12
    movq    32(%rdi), %rbp
    orq %rdx, %r12
    movq    %r15, -64(%rsp)
    movq    136(%rdi), %r15
    xorq    %r13, %r12
    movq    88(%rdi), %r13
    movq    %r12, -8(%rsp)
    xorq    %r12, %rax
    movq    40(%rdi), %r12
    xorq    %r9, %rbp
    movq    184(%rdi), %rbx
    rolq    $27, %rbp
    xorq    %r8, %r12
    rolq    $36, %r12
    xorq    %rsi, %r13
    xorq    %r11, %r15
    rolq    $10, %r13
    rolq    $15, %r15
    xorq    %r10, %rbx
    movq    %r13, %rdx
    movq    %r15, -16(%rsp)
    orq %r13, %r15
    andq    %r12, %rdx
    xorq    %r12, %r15
    rolq    $56, %rbx
    xorq    %rbp, %rdx
    orq %rbp, %r12
    movq    %rdx, 8(%rsp)
    movq    -48(%rsp), %rdx
    xorq    %rbx, %r12
    xorq    %r12, %rcx
    xorq    %r14, %rdx
    xorq    -24(%rsp), %rdx
    xorq    8(%rsp), %rdx
    movq    %r15, 16(%rsp)
    movq    -16(%rsp), %r15
    xorq    64(%rdi), %r10
    xorq    112(%rdi), %r9
    xorq    16(%rdi), %r11
    notq    %r15
    xorq    120(%rdi), %r8
    movq    %r15, (%rsp)
    orq %rbx, %r15
    rolq    $55, %r10
    xorq    %r13, %r15
    movq    %rbx, %r13
    movq    %r10, %rbx
    andq    %rbp, %r13
    xorq    168(%rdi), %rsi
    notq    %rbx
    xorq    (%rsp), %r13
    rolq    $39, %r9
    movq    %r15, -16(%rsp)
    movq    %rbx, %r15
    rolq    $41, %r8
    movq    %r12, (%rsp)
    rolq    $62, %r11
    andq    %r9, %r15
    movq    %r8, %rbp
    xorq    %r11, %r15
    rolq    $2, %rsi
    xorq    %r13, %rax
    xorq    %r15, %rdx
    orq %r9, %rbp
    xorq    %rbx, %rbp
    movq    -80(%rsp), %rbx
    movq    %rsi, %r12
    movq    %rbp, 64(%rsp)
    orq %r11, %r12
    andq    %r10, %r11
    xorq    %r8, %r12
    xorq    %rsi, %r11
    xorq    %r11, %rcx
    xorq    %r12, %rax
    xorq    -104(%rsp), %rbx
    movq    %rax, %r10
    rolq    %r10
    xorq    16(%rsp), %rbx
    xorq    %rbp, %rbx
    movq    %rsi, %rbp
    andq    %r8, %rbp
    xorq    -120(%rsp), %rbx
    xorq    %r9, %rbp
    movq    -72(%rsp), %r9
    movq    %rbx, %r8
    xorq    %rbx, %r10
    movabsq $-9223372036854775669, %rbx
    xorq    %rbp, %r9
    rolq    %r8
    xorq    -88(%rsp), %r9
    xorq    %rcx, %r8
    rolq    %rcx
    xorq    -64(%rsp), %r9
    xorq    -16(%rsp), %r9
    movq    %r9, %rsi
    xorq    %r9, %rcx
    movq    %r8, %r9
    rolq    %rsi
    xorq    %r14, %r9
    movq    -64(%rsp), %r14
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    -104(%rsp), %rax
    xorq    %r10, %r14
    xorq    %rsi, %rax
    rolq    $44, %rax
    rolq    $43, %r14
    xorq    %r9, %rbx
    movq    %rbx, -104(%rsp)
    movq    %r14, %rbx
    xorq    %rcx, %r13
    orq %rax, %rbx
    rolq    $21, %r13
    xorq    %rdx, %r11
    xorq    %rbx, -104(%rsp)
    movq    %r14, %rbx
    rolq    $14, %r11
    notq    %rbx
    xorq    %r10, %rbp
    orq %r13, %rbx
    rolq    $61, %rbp
    xorq    %rax, %rbx
    andq    %r9, %rax
    movq    %rbx, -64(%rsp)
    movq    %r11, %rbx
    xorq    %r11, %rax
    andq    %r13, %rbx
    movq    %rax, 40(%rsp)
    movq    -112(%rsp), %rax
    xorq    %r14, %rbx
    movq    %r11, %r14
    movq    16(%rsp), %r11
    orq %r9, %r14
    movq    -24(%rsp), %r9
    movq    %rbx, 24(%rsp)
    movq    -56(%rsp), %rbx
    xorq    %r13, %r14
    xorq    %rcx, %rax
    movq    %r14, 32(%rsp)
    xorq    %rsi, %r11
    rolq    $28, %rax
    xorq    %r8, %r9
    rolq    $45, %r11
    rolq    $3, %r9
    xorq    %rdx, %rbx
    movq    %r11, %r14
    rolq    $20, %rbx
    movq    %r9, %r13
    andq    %r9, %r14
    orq %rbx, %r13
    xorq    %rbx, %r14
    xorq    %rax, %r13
    movq    %r13, -56(%rsp)
    movq    %rbp, %r13
    movq    %r14, -24(%rsp)
    notq    %r13
    movq    %rbp, %r14
    orq %r11, %r13
    orq %rax, %r14
    xorq    %r9, %r13
    xorq    %r11, %r14
    andq    %rax, %rbx
    xorq    %rbp, %rbx
    movq    (%rsp), %rbp
    movq    %r13, -112(%rsp)
    movq    -8(%rsp), %rax
    xorq    %r8, %r15
    movq    %r14, 16(%rsp)
    movq    -88(%rsp), %r11
    rolq    $18, %r15
    movq    %rbx, 48(%rsp)
    movq    -120(%rsp), %r9
    movq    %r15, %r14
    xorq    %rdx, %rbp
    xorq    %rcx, %rax
    rolq    $8, %rbp
    rolq    $25, %rax
    xorq    %r10, %r11
    movq    %rbp, %r13
    rolq    $6, %r11
    andq    %rax, %r13
    notq    %rbp
    xorq    %r11, %r13
    movq    %rax, %rbx
    xorq    %rsi, %r9
    movq    %r13, (%rsp)
    movq    %rbp, %r13
    rolq    %r9
    andq    %r15, %r13
    orq %r11, %rbx
    orq %r9, %r14
    xorq    %rax, %r13
    movq    16(%rsp), %rax
    andq    %r9, %r11
    xorq    %rbp, %r14
    xorq    %r15, %r11
    movq    -80(%rsp), %rbp
    movq    %r11, 72(%rsp)
    movq    -48(%rsp), %r11
    xorq    %r9, %rbx
    movq    %r14, 56(%rsp)
    movq    -32(%rsp), %r9
    xorq    32(%rsp), %rax
    movq    %rbx, -8(%rsp)
    xorq    %rsi, %rbp
    rolq    $10, %rbp
    xorq    %r8, %r11
    rolq    $36, %r11
    xorq    %rdx, %r9
    movq    %rbp, %r15
    xorq    %r14, %rax
    movq    -16(%rsp), %r14
    rolq    $27, %r9
    xorq    %r10, %r14
    rolq    $15, %r14
    xorq    %rcx, %r12
    andq    %r11, %r15
    xorq    %r9, %r15
    rolq    $56, %r12
    movq    %r14, %rbx
    movq    %r15, -32(%rsp)
    movq    %r14, %r15
    orq %rbp, %rbx
    notq    %r15
    xorq    %r11, %rbx
    xorq    -96(%rsp), %rcx
    movq    %r15, %r14
    xorq    8(%rsp), %r8
    orq %r9, %r11
    orq %r12, %r14
    xorq    -40(%rsp), %rdx
    xorq    %r12, %r11
    xorq    %rbp, %r14
    movq    %r11, -80(%rsp)
    xorq    -72(%rsp), %r10
    movq    %r14, -48(%rsp)
    movq    %r12, %r14
    rolq    $55, %rcx
    andq    %r9, %r14
    movq    -56(%rsp), %r9
    rolq    $41, %r8
    rolq    $39, %rdx
    movq    %rcx, %r11
    movq    %r8, %rbp
    notq    %r11
    orq %rdx, %rbp
    xorq    %r15, %r14
    xorq    %r11, %rbp
    movq    %r11, %r15
    movq    (%rsp), %r11
    xorq    -104(%rsp), %r9
    rolq    $62, %r10
    andq    %rdx, %r15
    xorq    64(%rsp), %rsi
    xorq    %r10, %r15
    xorq    %r14, %rax
    movq    %rbp, -72(%rsp)
    xorq    -24(%rsp), %r11
    xorq    -8(%rsp), %r9
    rolq    $2, %rsi
    movq    %rsi, %r12
    xorq    %rbx, %r11
    xorq    -32(%rsp), %r9
    xorq    %r15, %r9
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    andq    %r8, %rbp
    orq %r10, %r12
    xorq    -64(%rsp), %r11
    xorq    %rdx, %rbp
    movq    24(%rsp), %rdx
    xorq    %r8, %r12
    andq    %r10, %rcx
    xorq    %r12, %rax
    xorq    %rsi, %rcx
    movq    %r11, %r8
    xorq    %rbp, %rdx
    rolq    %r8
    xorq    -112(%rsp), %rdx
    xorq    %r13, %rdx
    xorq    -48(%rsp), %rdx
    movq    %r12, -96(%rsp)
    movq    %rax, %r12
    movq    48(%rsp), %r10
    rolq    %r12
    xorq    %r11, %r12
    xorq    %r12, %r13
    movq    %rdx, %rsi
    rolq    $43, %r13
    xorq    40(%rsp), %r10
    rolq    %rsi
    xorq    %r9, %rsi
    xorq    72(%rsp), %r10
    xorq    -80(%rsp), %r10
    xorq    %rcx, %r10
    movq    %r10, %r11
    xorq    %r10, %r8
    movabsq $-9223372036854742903, %r10
    rolq    %r11
    xorq    %rdx, %r11
    movq    %r9, %rdx
    movq    -104(%rsp), %r9
    rolq    %rdx
    xorq    %r11, %r14
    xorq    %rax, %rdx
    movq    -24(%rsp), %rax
    xorq    %r8, %r9
    xorq    %rsi, %rax
    rolq    $44, %rax
    rolq    $21, %r14
    xorq    %r9, %r10
    movq    %r10, -120(%rsp)
    movq    %rax, %r10
    xorq    %rdx, %rcx
    orq %r13, %r10
    rolq    $14, %rcx
    xorq    %rsi, %rbx
    xorq    %r10, -120(%rsp)
    movq    %r13, %r10
    xorq    %r12, %rbp
    notq    %r10
    rolq    $45, %rbx
    orq %r14, %r10
    rolq    $61, %rbp
    xorq    %rax, %r10
    andq    %r9, %rax
    movq    %r10, -88(%rsp)
    movq    %rcx, %r10
    xorq    %rcx, %rax
    andq    %r14, %r10
    movq    %rax, 8(%rsp)
    movq    48(%rsp), %rax
    xorq    %r13, %r10
    movq    %rcx, %r13
    movq    32(%rsp), %rcx
    orq %r9, %r13
    movq    -8(%rsp), %r9
    movq    %r10, -24(%rsp)
    xorq    %r14, %r13
    movq    %rbx, %r10
    xorq    %rdx, %rax
    movq    %r13, -40(%rsp)
    movq    %rbp, %r13
    rolq    $20, %rax
    xorq    %r11, %rcx
    notq    %r13
    xorq    %r8, %r9
    rolq    $28, %rcx
    orq %rbx, %r13
    rolq    $3, %r9
    movq    %r9, %r14
    andq    %r9, %r10
    xorq    %r9, %r13
    orq %rax, %r14
    xorq    %rax, %r10
    andq    %rcx, %rax
    xorq    %rcx, %r14
    movq    -64(%rsp), %r9
    movq    %r10, -16(%rsp)
    movq    %r14, -8(%rsp)
    movq    %rbp, %r14
    movq    -80(%rsp), %r10
    orq %rcx, %r14
    movq    56(%rsp), %rcx
    movq    %r13, -104(%rsp)
    xorq    %rbx, %r14
    xorq    %rbp, %rax
    xorq    %r8, %r15
    movq    %rax, 32(%rsp)
    movq    -112(%rsp), %rax
    xorq    %rsi, %r9
    rolq    %r9
    xorq    %rdx, %r10
    rolq    $18, %r15
    xorq    %r11, %rcx
    rolq    $8, %r10
    movq    %r14, 64(%rsp)
    rolq    $25, %rcx
    movq    %r10, %rbp
    movq    -56(%rsp), %r14
    xorq    %r12, %rax
    movq    %rcx, %rbx
    andq    %rcx, %rbp
    rolq    $6, %rax
    movq    %r15, %r13
    notq    %r10
    orq %rax, %rbx
    xorq    %rax, %rbp
    orq %r9, %r13
    xorq    %r9, %rbx
    andq    %rax, %r9
    movq    40(%rsp), %rax
    movq    %rbx, -112(%rsp)
    movq    (%rsp), %rbx
    xorq    %r8, %r14
    movq    %rbp, -80(%rsp)
    movq    %r10, %rbp
    rolq    $36, %r14
    andq    %r15, %rbp
    xorq    %r10, %r13
    xorq    %r15, %r9
    xorq    %rcx, %rbp
    xorq    %rdx, %rax
    movq    -48(%rsp), %r15
    xorq    %rsi, %rbx
    rolq    $27, %rax
    movq    -96(%rsp), %r10
    rolq    $10, %rbx
    movq    %r13, 48(%rsp)
    movq    %rbx, %rcx
    movq    %r9, 56(%rsp)
    andq    %r14, %rcx
    xorq    %r12, %r15
    xorq    %rax, %rcx
    xorq    %r11, %r10
    rolq    $15, %r15
    movq    %rcx, -48(%rsp)
    movq    -120(%rsp), %r9
    rolq    $56, %r10
    movq    -88(%rsp), %r13
    xorq    -8(%rsp), %r9
    xorq    16(%rsp), %r11
    xorq    -72(%rsp), %rsi
    xorq    -32(%rsp), %r8
    xorq    -112(%rsp), %r9
    xorq    72(%rsp), %rdx
    rolq    $55, %r11
    xorq    -16(%rsp), %r13
    rolq    $2, %rsi
    rolq    $41, %r8
    xorq    24(%rsp), %r12
    xorq    %rcx, %r9
    movq    %rbx, %rcx
    orq %r15, %rcx
    notq    %r15
    rolq    $39, %rdx
    movq    %r15, -64(%rsp)
    orq %r10, %r15
    xorq    %r14, %rcx
    xorq    %rbx, %r15
    orq %rax, %r14
    movq    %r10, %rbx
    xorq    %r10, %r14
    andq    %rax, %rbx
    movq    %r11, %rax
    movq    %r14, -56(%rsp)
    notq    %rax
    movq    %rsi, %r14
    movq    %r15, -96(%rsp)
    andq    %r8, %r14
    movq    %rax, %r15
    movq    %r8, %r10
    xorq    -80(%rsp), %r13
    xorq    %rdx, %r14
    andq    %rdx, %r15
    orq %rdx, %r10
    movq    -104(%rsp), %rdx
    rolq    $62, %r12
    xorq    %rax, %r10
    movq    %rsi, %rax
    xorq    %r12, %r15
    xorq    -64(%rsp), %rbx
    movq    %r10, -72(%rsp)
    xorq    %rcx, %r13
    xorq    %r15, %r9
    xorq    %rbp, %rdx
    xorq    %r10, %r13
    movq    32(%rsp), %r10
    xorq    -24(%rsp), %rdx
    orq %r12, %rax
    andq    %r11, %r12
    xorq    %r8, %rax
    xorq    %rsi, %r12
    movq    %rax, -32(%rsp)
    movq    64(%rsp), %rax
    movq    %r13, %r8
    xorq    %r12, %r10
    rolq    %r8
    xorq    8(%rsp), %r10
    xorq    -96(%rsp), %rdx
    xorq    -40(%rsp), %rax
    xorq    56(%rsp), %r10
    xorq    %r14, %rdx
    xorq    %rbx, %rax
    movq    %rdx, %rsi
    xorq    48(%rsp), %rax
    rolq    %rsi
    xorq    -56(%rsp), %r10
    xorq    %r9, %rsi
    rolq    %r9
    xorq    -32(%rsp), %rax
    xorq    %r10, %r8
    rolq    %r10
    xorq    %rdx, %r10
    movq    -120(%rsp), %rdx
    xorq    %r10, %rbx
    xorq    %rax, %r9
    movq    %rax, %r11
    movq    -16(%rsp), %rax
    rolq    %r11
    xorq    %r9, %r12
    rolq    $21, %rbx
    xorq    %r13, %r11
    xorq    %r8, %rdx
    movabsq $-9223372036854743037, %r13
    xorq    %r11, %rbp
    xorq    %rsi, %rax
    rolq    $43, %rbp
    rolq    $44, %rax
    rolq    $14, %r12
    xorq    %rdx, %r13
    movq    %r13, -120(%rsp)
    movq    %rbp, %r13
    xorq    %rsi, %rcx
    orq %rax, %r13
    xorq    %r11, %r14
    rolq    $45, %rcx
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    rolq    $61, %r14
    notq    %r13
    orq %rbx, %r13
    xorq    %rax, %r13
    andq    %rdx, %rax
    movq    %r13, -64(%rsp)
    movq    %r12, %r13
    xorq    %r12, %rax
    andq    %rbx, %r13
    movq    %rax, (%rsp)
    movq    -112(%rsp), %rax
    xorq    %rbp, %r13
    movq    %r12, %rbp
    orq %rdx, %rbp
    movq    -40(%rsp), %rdx
    movq    %r13, 16(%rsp)
    xorq    %rbx, %rbp
    movq    32(%rsp), %rbx
    movq    %rcx, %r13
    xorq    %r8, %rax
    movq    %rbp, -16(%rsp)
    movq    %r14, %rbp
    rolq    $3, %rax
    notq    %rbp
    xorq    %r10, %rdx
    movq    %rax, %r12
    andq    %rax, %r13
    xorq    %r9, %rbx
    rolq    $28, %rdx
    orq %rcx, %rbp
    rolq    $20, %rbx
    xorq    %rax, %rbp
    orq %rbx, %r12
    xorq    %rbx, %r13
    andq    %rdx, %rbx
    xorq    %rdx, %r12
    xorq    %r14, %rbx
    movq    %rbp, -112(%rsp)
    movq    %r12, -40(%rsp)
    movq    %r14, %r12
    movq    -56(%rsp), %r14
    movq    48(%rsp), %rbp
    orq %rdx, %r12
    movq    %rbx, 40(%rsp)
    xorq    %rcx, %r12
    movq    %rbx, %rcx
    movq    -104(%rsp), %rbx
    movq    %r12, %rax
    movq    -88(%rsp), %rdx
    movq    %r12, 32(%rsp)
    xorq    -16(%rsp), %rax
    movq    %r13, 24(%rsp)
    xorq    (%rsp), %rcx
    xorq    %r9, %r14
    xorq    %r10, %rbp
    rolq    $8, %r14
    xorq    %r11, %rbx
    rolq    $25, %rbp
    movq    %r14, %r12
    rolq    $6, %rbx
    andq    %rbp, %r12
    xorq    %rsi, %rdx
    movq    %rbp, %r13
    xorq    %rbx, %r12
    rolq    %rdx
    orq %rbx, %r13
    movq    %r12, -56(%rsp)
    movq    %r14, %r12
    xorq    %rdx, %r13
    xorq    %r8, %r15
    notq    %r12
    movq    %r13, -104(%rsp)
    rolq    $18, %r15
    movq    %r12, %r13
    andq    %rdx, %rbx
    andq    %r15, %r13
    movq    %r15, %r14
    xorq    %r15, %rbx
    xorq    %rbp, %r13
    orq %rdx, %r14
    movq    -96(%rsp), %r15
    movq    %r13, -88(%rsp)
    movq    -80(%rsp), %r13
    xorq    %r12, %r14
    movq    -8(%rsp), %r12
    movq    %rbx, 72(%rsp)
    xorq    %rbx, %rcx
    movq    8(%rsp), %rbp
    xorq    %r14, %rax
    movq    %r14, 48(%rsp)
    movq    -32(%rsp), %rbx
    xorq    %r11, %r15
    xorq    %rsi, %r13
    rolq    $15, %r15
    rolq    $10, %r13
    xorq    %r8, %r12
    movq    %r15, %r14
    xorq    %r9, %rbp
    rolq    $36, %r12
    movq    %r13, %rdx
    xorq    %r10, %rbx
    rolq    $27, %rbp
    notq    %r15
    rolq    $56, %rbx
    andq    %r12, %rdx
    orq %r13, %r14
    xorq    %rbp, %rdx
    xorq    %r12, %r14
    orq %rbp, %r12
    movq    %rdx, -32(%rsp)
    movq    -40(%rsp), %rdx
    xorq    %rbx, %r12
    movq    %r14, -96(%rsp)
    movq    %r15, %r14
    xorq    %r12, %rcx
    orq %rbx, %r14
    xorq    %r13, %r14
    movq    %rbx, %r13
    xorq    -120(%rsp), %rdx
    andq    %rbp, %r13
    xorq    %r15, %r13
    xorq    %r13, %rax
    xorq    -104(%rsp), %rdx
    xorq    -32(%rsp), %rdx
    movq    %r14, -80(%rsp)
    xorq    64(%rsp), %r10
    movq    %r12, -8(%rsp)
    xorq    -48(%rsp), %r8
    xorq    56(%rsp), %r9
    xorq    -24(%rsp), %r11
    rolq    $55, %r10
    xorq    -72(%rsp), %rsi
    rolq    $41, %r8
    movq    %r10, %rbx
    rolq    $39, %r9
    movq    %r8, %rbp
    notq    %rbx
    orq %r9, %rbp
    movq    %rbx, %r15
    rolq    $62, %r11
    xorq    %rbx, %rbp
    movq    -56(%rsp), %rbx
    rolq    $2, %rsi
    andq    %r9, %r15
    movq    %rbp, -72(%rsp)
    movq    %rsi, %r12
    xorq    %r11, %r15
    xorq    %r15, %rdx
    xorq    24(%rsp), %rbx
    xorq    -96(%rsp), %rbx
    xorq    %rbp, %rbx
    movq    %rsi, %rbp
    xorq    -64(%rsp), %rbx
    andq    %r8, %rbp
    orq %r11, %r12
    xorq    %r9, %rbp
    movq    16(%rsp), %r9
    andq    %r10, %r11
    xorq    %r8, %r12
    xorq    %rsi, %r11
    xorq    %r11, %rcx
    xorq    %r12, %rax
    movq    %rbx, %r8
    movq    %rax, %r10
    rolq    %r8
    xorq    %rbp, %r9
    rolq    %r10
    xorq    -112(%rsp), %r9
    xorq    %rcx, %r8
    rolq    %rcx
    xorq    %rbx, %r10
    movabsq $-9223372036854743038, %rbx
    xorq    -88(%rsp), %r9
    xorq    %r14, %r9
    movq    -88(%rsp), %r14
    movq    %r9, %rsi
    xorq    %r9, %rcx
    movq    -120(%rsp), %r9
    rolq    %rsi
    xorq    %rcx, %r13
    xorq    %rdx, %rsi
    rolq    %rdx
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    xorq    %r10, %r14
    xorq    %r8, %r9
    rolq    $43, %r14
    xorq    %rdx, %r11
    rolq    $21, %r13
    xorq    %r9, %rbx
    rolq    $14, %r11
    movq    %rbx, -120(%rsp)
    movq    %r14, %rbx
    xorq    %rsi, %rax
    rolq    $44, %rax
    orq %rax, %rbx
    xorq    %r10, %rbp
    xorq    %rbx, -120(%rsp)
    movq    %r14, %rbx
    rolq    $61, %rbp
    notq    %rbx
    orq %r13, %rbx
    xorq    %rax, %rbx
    andq    %r9, %rax
    movq    %rbx, -88(%rsp)
    movq    %r11, %rbx
    xorq    %r11, %rax
    andq    %r13, %rbx
    movq    %rax, 8(%rsp)
    movq    -16(%rsp), %rax
    xorq    %r14, %rbx
    movq    %r11, %r14
    movq    -96(%rsp), %r11
    orq %r9, %r14
    movq    -104(%rsp), %r9
    movq    %rbx, -48(%rsp)
    movq    40(%rsp), %rbx
    xorq    %r13, %r14
    xorq    %rcx, %rax
    movq    %r14, -24(%rsp)
    xorq    %rsi, %r11
    rolq    $28, %rax
    xorq    %r8, %r9
    rolq    $45, %r11
    rolq    $3, %r9
    xorq    %rdx, %rbx
    movq    %r11, %r14
    rolq    $20, %rbx
    movq    %r9, %r13
    andq    %r9, %r14
    orq %rbx, %r13
    xorq    %rbx, %r14
    andq    %rax, %rbx
    xorq    %rax, %r13
    movq    %r14, -16(%rsp)
    movq    %rbp, %r14
    movq    %r13, -96(%rsp)
    movq    %rbp, %r13
    orq %rax, %r14
    notq    %r13
    xorq    %r11, %r14
    xorq    %rbp, %rbx
    orq %r11, %r13
    movq    -112(%rsp), %r11
    movq    %rbx, 24(%rsp)
    xorq    %r9, %r13
    movq    -64(%rsp), %r9
    movq    %r14, 64(%rsp)
    movq    48(%rsp), %rax
    movq    %r13, -104(%rsp)
    movq    -8(%rsp), %rbp
    xorq    %r10, %r11
    xorq    %rsi, %r9
    rolq    %r9
    rolq    $6, %r11
    xorq    %rcx, %rax
    xorq    %rdx, %rbp
    rolq    $25, %rax
    xorq    %r8, %r15
    rolq    $8, %rbp
    movq    %rax, %rbx
    rolq    $18, %r15
    movq    %rbp, %r13
    orq %r11, %rbx
    notq    %rbp
    andq    %rax, %r13
    xorq    %r9, %rbx
    movq    %r15, %r14
    xorq    %r11, %r13
    movq    %rbx, -112(%rsp)
    orq %r9, %r14
    movq    %r13, -8(%rsp)
    movq    %rbp, %r13
    andq    %r9, %r11
    andq    %r15, %r13
    xorq    %rbp, %r14
    xorq    %r15, %r11
    xorq    %rax, %r13
    movq    64(%rsp), %rax
    movq    %r11, 48(%rsp)
    movq    -56(%rsp), %rbp
    movq    %r14, 40(%rsp)
    xorq    %rcx, %r12
    movq    -40(%rsp), %r11
    rolq    $56, %r12
    movq    (%rsp), %r9
    xorq    -24(%rsp), %rax
    xorq    %rsi, %rbp
    rolq    $10, %rbp
    xorq    %r8, %r11
    rolq    $36, %r11
    xorq    %rdx, %r9
    movq    %rbp, %r15
    xorq    %r14, %rax
    movq    -80(%rsp), %r14
    rolq    $27, %r9
    andq    %r11, %r15
    xorq    %r9, %r15
    movq    %r15, -56(%rsp)
    xorq    %r10, %r14
    rolq    $15, %r14
    movq    %r14, %r15
    movq    %r14, %rbx
    notq    %r15
    orq %rbp, %rbx
    movq    %r15, %r14
    xorq    %r11, %rbx
    orq %r12, %r14
    xorq    -32(%rsp), %r8
    orq %r9, %r11
    xorq    32(%rsp), %rcx
    xorq    %rbp, %r14
    xorq    %r12, %r11
    xorq    72(%rsp), %rdx
    movq    %r14, -80(%rsp)
    movq    %r12, %r14
    movq    %r11, -40(%rsp)
    andq    %r9, %r14
    xorq    -72(%rsp), %rsi
    rolq    $41, %r8
    xorq    %r15, %r14
    movq    -96(%rsp), %r9
    rolq    $55, %rcx
    movq    %r8, %rbp
    xorq    16(%rsp), %r10
    rolq    $39, %rdx
    movq    %rcx, %r11
    notq    %r11
    orq %rdx, %rbp
    rolq    $2, %rsi
    xorq    %r11, %rbp
    movq    %r11, %r15
    movq    -8(%rsp), %r11
    xorq    -120(%rsp), %r9
    movq    %rbp, -72(%rsp)
    andq    %rdx, %r15
    rolq    $62, %r10
    movq    %rsi, %r12
    xorq    %r10, %r15
    xorq    %r14, %rax
    xorq    -16(%rsp), %r11
    xorq    -112(%rsp), %r9
    xorq    %rbx, %r11
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    xorq    -56(%rsp), %r9
    andq    %r8, %rbp
    xorq    -88(%rsp), %r11
    xorq    %rdx, %rbp
    movq    -48(%rsp), %rdx
    xorq    %r15, %r9
    xorq    %rbp, %rdx
    xorq    -104(%rsp), %rdx
    xorq    %r13, %rdx
    orq %r10, %r12
    andq    %r10, %rcx
    movq    24(%rsp), %r10
    xorq    %r8, %r12
    xorq    %rsi, %rcx
    xorq    %r12, %rax
    xorq    -80(%rsp), %rdx
    movq    %r12, -32(%rsp)
    movq    %rax, %r12
    movq    %r11, %r8
    rolq    %r12
    rolq    %r8
    xorq    8(%rsp), %r10
    xorq    %r11, %r12
    movq    %rdx, %rsi
    xorq    %r12, %r13
    rolq    %rsi
    rolq    $43, %r13
    xorq    %r9, %rsi
    xorq    48(%rsp), %r10
    xorq    -40(%rsp), %r10
    xorq    %rcx, %r10
    movq    %r10, %r11
    xorq    %r10, %r8
    movabsq $-9223372036854775680, %r10
    rolq    %r11
    xorq    %rdx, %r11
    movq    %r9, %rdx
    movq    -120(%rsp), %r9
    rolq    %rdx
    xorq    %r11, %r14
    xorq    %rax, %rdx
    movq    -16(%rsp), %rax
    rolq    $21, %r14
    xorq    %rdx, %rcx
    xorq    %r8, %r9
    rolq    $14, %rcx
    xorq    %r9, %r10
    xorq    %rsi, %rax
    movq    %r10, -120(%rsp)
    rolq    $44, %rax
    movq    %rax, %r10
    orq %r13, %r10
    xorq    %r10, -120(%rsp)
    movq    %r13, %r10
    notq    %r10
    orq %r14, %r10
    xorq    %rsi, %rbx
    xorq    %r12, %rbp
    rolq    $45, %rbx
    xorq    %rax, %r10
    andq    %r9, %rax
    movq    %r10, -64(%rsp)
    movq    %rcx, %r10
    xorq    %rcx, %rax
    andq    %r14, %r10
    movq    %rax, (%rsp)
    movq    24(%rsp), %rax
    xorq    %r13, %r10
    movq    %rcx, %r13
    movq    -24(%rsp), %rcx
    orq %r9, %r13
    movq    -112(%rsp), %r9
    rolq    $61, %rbp
    xorq    %r14, %r13
    movq    %r10, 16(%rsp)
    movq    %rbx, %r10
    xorq    %rdx, %rax
    movq    %r13, -16(%rsp)
    movq    %rbp, %r13
    rolq    $20, %rax
    xorq    %r11, %rcx
    notq    %r13
    xorq    %r8, %r9
    rolq    $28, %rcx
    orq %rbx, %r13
    rolq    $3, %r9
    movq    %r9, %r14
    andq    %r9, %r10
    xorq    %r9, %r13
    orq %rax, %r14
    xorq    %rax, %r10
    andq    %rcx, %rax
    xorq    %rcx, %r14
    movq    %r10, 24(%rsp)
    xorq    %rbp, %rax
    movq    %r14, -24(%rsp)
    movq    %rbp, %r14
    movq    -88(%rsp), %r9
    movq    40(%rsp), %r10
    orq %rcx, %r14
    movq    %rax, 56(%rsp)
    movq    -104(%rsp), %rcx
    xorq    %rbx, %r14
    movq    %r13, -112(%rsp)
    movq    -40(%rsp), %rax
    movq    %r14, 32(%rsp)
    xorq    %rsi, %r9
    movq    -96(%rsp), %r14
    xorq    %r11, %r10
    rolq    %r9
    xorq    %r12, %rcx
    rolq    $6, %rcx
    rolq    $25, %r10
    xorq    %rdx, %rax
    rolq    $8, %rax
    xorq    %r8, %r15
    movq    %r10, %rbx
    movq    %rax, %rbp
    notq    %rax
    rolq    $18, %r15
    andq    %r10, %rbp
    orq %rcx, %rbx
    xorq    %r8, %r14
    xorq    %rcx, %rbp
    xorq    %r9, %rbx
    rolq    $36, %r14
    movq    %rbp, -104(%rsp)
    movq    %rax, %rbp
    andq    %r15, %rbp
    movq    %rbx, -88(%rsp)
    movq    -8(%rsp), %rbx
    xorq    %r10, %rbp
    movq    %r15, %r10
    orq %r9, %r10
    andq    %rcx, %r9
    xorq    %r15, %r9
    xorq    %rax, %r10
    movq    8(%rsp), %rax
    movq    %r9, 40(%rsp)
    movq    -120(%rsp), %r9
    xorq    %rsi, %rbx
    rolq    $10, %rbx
    movq    %r10, -40(%rsp)
    movq    -32(%rsp), %r10
    movq    %rbx, %r13
    movq    -80(%rsp), %r15
    movq    %rbx, %rcx
    xorq    %rdx, %rax
    andq    %r14, %r13
    xorq    -24(%rsp), %r9
    rolq    $27, %rax
    xorq    %rax, %r13
    xorq    %r11, %r10
    movq    %r13, -32(%rsp)
    xorq    %r12, %r15
    rolq    $56, %r10
    rolq    $15, %r15
    xorq    -88(%rsp), %r9
    orq %r15, %rcx
    notq    %r15
    xorq    %r14, %rcx
    xorq    %r13, %r9
    movq    -64(%rsp), %r13
    xorq    24(%rsp), %r13
    xorq    -104(%rsp), %r13
    movq    %r15, -80(%rsp)
    orq %r10, %r15
    xorq    -56(%rsp), %r8
    xorq    %rbx, %r15
    orq %rax, %r14
    xorq    64(%rsp), %r11
    movq    %r10, %rbx
    xorq    %r10, %r14
    xorq    48(%rsp), %rdx
    andq    %rax, %rbx
    movq    %r15, -96(%rsp)
    xorq    -72(%rsp), %rsi
    xorq    %rcx, %r13
    rolq    $41, %r8
    xorq    -80(%rsp), %rbx
    movq    %r14, -80(%rsp)
    rolq    $55, %r11
    movq    %r8, %r14
    xorq    -48(%rsp), %r12
    rolq    $39, %rdx
    movq    %r11, %rax
    movq    56(%rsp), %r10
    notq    %rax
    orq %rdx, %r14
    rolq    $2, %rsi
    xorq    %rax, %r14
    movq    %rax, %r15
    movq    %rsi, %rax
    xorq    %r14, %r13
    movq    %r14, -72(%rsp)
    movq    %rsi, %r14
    andq    %r8, %r14
    andq    %rdx, %r15
    rolq    $62, %r12
    xorq    %rdx, %r14
    movq    -112(%rsp), %rdx
    orq %r12, %rax
    xorq    %r8, %rax
    xorq    %r12, %r15
    movq    %r13, %r8
    movq    %rax, -48(%rsp)
    movq    32(%rsp), %rax
    xorq    %r15, %r9
    xorq    %rbp, %rdx
    xorq    16(%rsp), %rdx
    xorq    -16(%rsp), %rax
    xorq    -96(%rsp), %rdx
    xorq    %r14, %rdx
    xorq    %rbx, %rax
    andq    %r11, %r12
    xorq    %rsi, %r12
    xorq    -40(%rsp), %rax
    rolq    %r8
    xorq    %r12, %r10
    movq    %rdx, %rsi
    xorq    (%rsp), %r10
    rolq    %rsi
    xorq    %r9, %rsi
    rolq    %r9
    xorq    -48(%rsp), %rax
    xorq    40(%rsp), %r10
    xorq    %rax, %r9
    movq    %rax, %r11
    movq    24(%rsp), %rax
    rolq    %r11
    xorq    %r9, %r12
    xorq    -80(%rsp), %r10
    xorq    %r13, %r11
    rolq    $14, %r12
    xorq    %r11, %rbp
    rolq    $43, %rbp
    xorq    %rsi, %rax
    rolq    $44, %rax
    xorq    %r10, %r8
    rolq    %r10
    xorq    %rdx, %r10
    movq    -120(%rsp), %rdx
    xorq    %r10, %rbx
    rolq    $21, %rbx
    xorq    %r8, %rdx
    movq    %rdx, %r13
    xorq    $32778, %r13
    movq    %r13, -56(%rsp)
    movq    %rbp, %r13
    orq %rax, %r13
    xorq    %r13, -56(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %rbx, %r13
    xorq    %rax, %r13
    movq    %r13, -120(%rsp)
    movq    %r12, %r13
    andq    %rbx, %r13
    andq    %rdx, %rax
    xorq    %rsi, %rcx
    xorq    %rbp, %r13
    xorq    %r12, %rax
    movq    %r12, %rbp
    orq %rdx, %rbp
    movq    %rax, 64(%rsp)
    movq    -88(%rsp), %rax
    xorq    %rbx, %rbp
    movq    56(%rsp), %rbx
    xorq    %r11, %r14
    movq    -16(%rsp), %rdx
    rolq    $45, %rcx
    rolq    $61, %r14
    movq    %r13, -8(%rsp)
    movq    %rcx, %r13
    xorq    %r8, %rax
    movq    %rbp, 8(%rsp)
    movq    %r14, %rbp
    rolq    $3, %rax
    xorq    %r9, %rbx
    notq    %rbp
    rolq    $20, %rbx
    xorq    %r10, %rdx
    movq    %rax, %r12
    rolq    $28, %rdx
    orq %rbx, %r12
    andq    %rax, %r13
    xorq    %rdx, %r12
    xorq    %rbx, %r13
    orq %rcx, %rbp
    movq    %r12, -16(%rsp)
    andq    %rdx, %rbx
    movq    %r14, %r12
    xorq    %r14, %rbx
    xorq    %rax, %rbp
    orq %rdx, %r12
    xorq    %rcx, %r12
    movq    -64(%rsp), %rdx
    movq    %rbp, -88(%rsp)
    movq    %rbx, 56(%rsp)
    movq    -40(%rsp), %rbp
    movq    %rbx, %rcx
    movq    -112(%rsp), %rbx
    movq    %r12, %rax
    movq    %r12, 48(%rsp)
    movq    -80(%rsp), %r14
    movq    %r13, 24(%rsp)
    xorq    %rsi, %rdx
    xorq    8(%rsp), %rax
    xorq    %r10, %rbp
    rolq    %rdx
    xorq    64(%rsp), %rcx
    xorq    %r11, %rbx
    rolq    $6, %rbx
    rolq    $25, %rbp
    xorq    %r9, %r14
    rolq    $8, %r14
    movq    %rbp, %r13
    xorq    %r8, %r15
    movq    %r14, %r12
    orq %rbx, %r13
    rolq    $18, %r15
    andq    %rbp, %r12
    xorq    %rdx, %r13
    xorq    %rbx, %r12
    movq    %r13, -112(%rsp)
    andq    %rdx, %rbx
    movq    %r12, -80(%rsp)
    movq    %r14, %r12
    movq    %r15, %r14
    notq    %r12
    orq %rdx, %r14
    xorq    %r15, %rbx
    movq    %r12, %r13
    xorq    %r12, %r14
    movq    -24(%rsp), %r12
    andq    %r15, %r13
    movq    %rbx, 72(%rsp)
    xorq    %rbx, %rcx
    xorq    %rbp, %r13
    movq    (%rsp), %rbp
    movq    %r14, -40(%rsp)
    movq    %r13, -64(%rsp)
    movq    -104(%rsp), %r13
    xorq    %r14, %rax
    xorq    %r8, %r12
    movq    -48(%rsp), %rbx
    rolq    $36, %r12
    movq    -96(%rsp), %r15
    xorq    %r9, %rbp
    xorq    %rsi, %r13
    rolq    $27, %rbp
    rolq    $10, %r13
    xorq    %r10, %rbx
    movq    %r13, %rdx
    xorq    %r11, %r15
    rolq    $56, %rbx
    andq    %r12, %rdx
    rolq    $15, %r15
    xorq    %rbp, %rdx
    movq    %r15, %r14
    notq    %r15
    movq    %rdx, -48(%rsp)
    movq    -16(%rsp), %rdx
    orq %r13, %r14
    xorq    -56(%rsp), %rdx
    xorq    -112(%rsp), %rdx
    xorq    -48(%rsp), %rdx
    xorq    %r12, %r14
    orq %rbp, %r12
    xorq    32(%rsp), %r10
    movq    %r14, -96(%rsp)
    movq    %r15, %r14
    xorq    -32(%rsp), %r8
    orq %rbx, %r14
    xorq    %rbx, %r12
    xorq    40(%rsp), %r9
    xorq    %r13, %r14
    movq    %rbx, %r13
    andq    %rbp, %r13
    xorq    16(%rsp), %r11
    movq    %r14, -24(%rsp)
    rolq    $55, %r10
    xorq    %r15, %r13
    xorq    -72(%rsp), %rsi
    rolq    $41, %r8
    movq    %r10, %rbx
    movq    %r12, (%rsp)
    rolq    $39, %r9
    movq    %r8, %rbp
    notq    %rbx
    orq %r9, %rbp
    movq    %rbx, %r15
    rolq    $62, %r11
    xorq    %rbx, %rbp
    rolq    $2, %rsi
    andq    %r9, %r15
    movq    %rbp, -72(%rsp)
    movq    -80(%rsp), %rbx
    xorq    %r11, %r15
    xorq    %r12, %rcx
    movq    %rsi, %r12
    xorq    %r15, %rdx
    xorq    %r13, %rax
    xorq    24(%rsp), %rbx
    xorq    -96(%rsp), %rbx
    xorq    %rbp, %rbx
    movq    %rsi, %rbp
    andq    %r8, %rbp
    xorq    -120(%rsp), %rbx
    xorq    %r9, %rbp
    movq    -8(%rsp), %r9
    xorq    %rbp, %r9
    xorq    -88(%rsp), %r9
    xorq    -64(%rsp), %r9
    xorq    %r14, %r9
    orq %r11, %r12
    andq    %r10, %r11
    xorq    %r8, %r12
    xorq    %rsi, %r11
    movq    %rbx, %r8
    xorq    %r11, %rcx
    rolq    %r8
    movq    %r9, %rsi
    xorq    %rcx, %r8
    rolq    %rsi
    xorq    %r12, %rax
    rolq    %rcx
    xorq    %rdx, %rsi
    movq    -64(%rsp), %r14
    xorq    %r9, %rcx
    rolq    %rdx
    movq    -56(%rsp), %r9
    xorq    %rax, %rdx
    movq    %rax, %r10
    movq    24(%rsp), %rax
    rolq    %r10
    xorq    %rcx, %r13
    xorq    %rdx, %r11
    rolq    $21, %r13
    xorq    %rbx, %r10
    rolq    $14, %r11
    movabsq $-9223372034707292150, %rbx
    xorq    %r8, %r9
    xorq    %r10, %r14
    rolq    $43, %r14
    xorq    %r9, %rbx
    xorq    %rsi, %rax
    rolq    $44, %rax
    movq    %rbx, -104(%rsp)
    movq    %r14, %rbx
    orq %rax, %rbx
    xorq    %rbx, -104(%rsp)
    movq    %r14, %rbx
    notq    %rbx
    orq %r13, %rbx
    xorq    %rax, %rbx
    movq    %rbx, -64(%rsp)
    movq    %r11, %rbx
    andq    %r13, %rbx
    xorq    %r14, %rbx
    movq    %r11, %r14
    orq %r9, %r14
    andq    %r9, %rax
    movq    -112(%rsp), %r9
    xorq    %r11, %rax
    movq    %rbx, -32(%rsp)
    movq    -96(%rsp), %r11
    movq    56(%rsp), %rbx
    movq    %rax, 16(%rsp)
    xorq    %r13, %r14
    movq    8(%rsp), %rax
    movq    %r14, -56(%rsp)
    xorq    %r10, %rbp
    xorq    %r8, %r9
    rolq    $61, %rbp
    xorq    %r8, %r15
    rolq    $3, %r9
    xorq    %rsi, %r11
    xorq    %rdx, %rbx
    rolq    $45, %r11
    movq    %r9, %r13
    rolq    $20, %rbx
    xorq    %rcx, %rax
    movq    %r11, %r14
    rolq    $28, %rax
    orq %rbx, %r13
    andq    %r9, %r14
    xorq    %rax, %r13
    xorq    %rbx, %r14
    andq    %rax, %rbx
    movq    %r13, -96(%rsp)
    movq    %rbp, %r13
    xorq    %rbp, %rbx
    movq    %r14, 8(%rsp)
    notq    %r13
    movq    %rbp, %r14
    movq    (%rsp), %rbp
    orq %r11, %r13
    orq %rax, %r14
    xorq    %r9, %r13
    xorq    %r11, %r14
    movq    -40(%rsp), %rax
    movq    -120(%rsp), %r9
    movq    %r13, -112(%rsp)
    movq    -88(%rsp), %r11
    movq    %r14, 24(%rsp)
    xorq    %rdx, %rbp
    movq    %rbx, 32(%rsp)
    rolq    $8, %rbp
    xorq    %rcx, %rax
    xorq    %rsi, %r9
    movq    %rbp, %r13
    notq    %rbp
    xorq    %r10, %r11
    rolq    %r9
    movq    %rbp, %r14
    rolq    $25, %rax
    rolq    $6, %r11
    rolq    $18, %r15
    movq    %rax, %rbx
    andq    %rax, %r13
    andq    %r15, %r14
    xorq    %r11, %r13
    orq %r11, %rbx
    xorq    %rax, %r14
    movq    %r15, %rax
    andq    %r9, %r11
    orq %r9, %rax
    xorq    %r9, %rbx
    xorq    %r15, %r11
    xorq    %rbp, %rax
    movq    %rbx, -40(%rsp)
    xorq    %rcx, %r12
    movq    %rax, 40(%rsp)
    movq    24(%rsp), %rax
    rolq    $56, %r12
    movq    %r13, (%rsp)
    xorq    -56(%rsp), %rax
    xorq    40(%rsp), %rax
    movq    %r11, 56(%rsp)
    movq    -80(%rsp), %rbp
    movq    -16(%rsp), %r11
    movq    64(%rsp), %r9
    movq    -24(%rsp), %r13
    xorq    %rsi, %rbp
    rolq    $10, %rbp
    xorq    %r8, %r11
    rolq    $36, %r11
    xorq    %rdx, %r9
    movq    %rbp, %rbx
    rolq    $27, %r9
    xorq    %r10, %r13
    andq    %r11, %rbx
    xorq    %r9, %rbx
    rolq    $15, %r13
    movq    %rbx, -24(%rsp)
    movq    %r13, %rbx
    notq    %r13
    movq    %r13, %r15
    orq %rbp, %rbx
    orq %r12, %r15
    xorq    %r11, %rbx
    orq %r9, %r11
    xorq    %rbp, %r15
    movq    %r12, %rbp
    andq    %r9, %rbp
    movq    -96(%rsp), %r9
    movq    %r15, -80(%rsp)
    xorq    %r13, %rbp
    xorq    %rbp, %rax
    xorq    %r12, %r11
    xorq    -48(%rsp), %r8
    xorq    48(%rsp), %rcx
    movq    %r11, -16(%rsp)
    xorq    72(%rsp), %rdx
    xorq    -72(%rsp), %rsi
    rolq    $41, %r8
    xorq    -104(%rsp), %r9
    rolq    $55, %rcx
    movq    %r8, %r12
    xorq    -8(%rsp), %r10
    rolq    $39, %rdx
    movq    %rcx, %r11
    notq    %r11
    orq %rdx, %r12
    rolq    $2, %rsi
    xorq    %r11, %r12
    movq    %r11, %r15
    movq    (%rsp), %r11
    movq    %rsi, %r13
    andq    %rdx, %r15
    xorq    -40(%rsp), %r9
    andq    %r8, %r13
    rolq    $62, %r10
    movq    %r12, -72(%rsp)
    xorq    %rdx, %r13
    movq    -32(%rsp), %rdx
    xorq    %r10, %r15
    xorq    8(%rsp), %r11
    andq    %r10, %rcx
    xorq    -24(%rsp), %r9
    xorq    %r13, %rdx
    xorq    %rbx, %r11
    xorq    -112(%rsp), %rdx
    xorq    %r12, %r11
    movq    %rsi, %r12
    orq %r10, %r12
    movq    32(%rsp), %r10
    xorq    %r15, %r9
    xorq    %r8, %r12
    xorq    -64(%rsp), %r11
    xorq    %r14, %rdx
    xorq    %r12, %rax
    movq    %r12, -48(%rsp)
    xorq    -80(%rsp), %rdx
    xorq    %rsi, %rcx
    movq    %rax, %r12
    xorq    16(%rsp), %r10
    rolq    %r12
    xorq    %r11, %r12
    movq    %r11, %r8
    rolq    %r8
    xorq    %r12, %r14
    movq    %rdx, %rsi
    rolq    $43, %r14
    xorq    56(%rsp), %r10
    rolq    %rsi
    xorq    %r9, %rsi
    xorq    -16(%rsp), %r10
    xorq    %rcx, %r10
    movq    %r10, %r11
    xorq    %r10, %r8
    movabsq $-9223372034707259263, %r10
    rolq    %r11
    xorq    %rdx, %r11
    movq    %r9, %rdx
    movq    -104(%rsp), %r9
    rolq    %rdx
    xorq    %r11, %rbp
    xorq    %rax, %rdx
    movq    8(%rsp), %rax
    rolq    $21, %rbp
    xorq    %rdx, %rcx
    xorq    %r8, %r9
    rolq    $14, %rcx
    xorq    %r9, %r10
    xorq    %rsi, %rax
    movq    %r10, -120(%rsp)
    rolq    $44, %rax
    movq    %rax, %r10
    orq %r14, %r10
    xorq    %r10, -120(%rsp)
    movq    %r14, %r10
    notq    %r10
    orq %rbp, %r10
    xorq    %rax, %r10
    movq    %r10, -88(%rsp)
    movq    %rcx, %r10
    andq    %rbp, %r10
    xorq    %r14, %r10
    movq    %rcx, %r14
    orq %r9, %r14
    movq    %r10, -8(%rsp)
    xorq    %rbp, %r14
    andq    %r9, %rax
    movq    -40(%rsp), %r9
    xorq    %rcx, %rax
    movq    -56(%rsp), %rcx
    xorq    %rsi, %rbx
    movq    %rax, 64(%rsp)
    movq    32(%rsp), %rax
    rolq    $45, %rbx
    xorq    %r12, %r13
    movq    %rbx, %r10
    movq    %r14, 8(%rsp)
    xorq    %r8, %r9
    rolq    $61, %r13
    xorq    %r8, %r15
    rolq    $3, %r9
    xorq    %r11, %rcx
    movq    %r13, %r14
    xorq    %rdx, %rax
    movq    %r9, %rbp
    rolq    $28, %rcx
    rolq    $20, %rax
    andq    %r9, %r10
    notq    %r14
    orq %rax, %rbp
    xorq    %rax, %r10
    andq    %rcx, %rax
    xorq    %rcx, %rbp
    movq    %r10, -40(%rsp)
    xorq    %r13, %rax
    movq    %rbp, -56(%rsp)
    movq    40(%rsp), %r10
    movq    %r13, %rbp
    orq %rbx, %r14
    orq %rcx, %rbp
    movq    %rax, 48(%rsp)
    movq    -112(%rsp), %rcx
    xorq    %r9, %r14
    rolq    $18, %r15
    movq    -16(%rsp), %rax
    movq    %r14, -104(%rsp)
    xorq    %rbx, %rbp
    movq    -64(%rsp), %r9
    xorq    %r11, %r10
    movq    %r15, %rbx
    rolq    $25, %r10
    movq    %rbp, 32(%rsp)
    movq    -80(%rsp), %rbp
    xorq    %r12, %rcx
    movq    %r10, %r13
    xorq    %rdx, %rax
    rolq    $6, %rcx
    xorq    %rsi, %r9
    rolq    $8, %rax
    orq %rcx, %r13
    rolq    %r9
    movq    %rax, %r14
    notq    %rax
    xorq    %r9, %r13
    andq    %r10, %r14
    orq %r9, %rbx
    xorq    %rcx, %r14
    movq    %r13, -64(%rsp)
    movq    (%rsp), %r13
    movq    %r14, -112(%rsp)
    movq    %rax, %r14
    xorq    %rax, %rbx
    andq    %r15, %r14
    movq    16(%rsp), %rax
    andq    %rcx, %r9
    xorq    %r10, %r14
    movq    -96(%rsp), %r10
    xorq    %r15, %r9
    xorq    %rsi, %r13
    movq    %r9, 40(%rsp)
    movq    -48(%rsp), %rcx
    rolq    $10, %r13
    movq    -88(%rsp), %r15
    xorq    %r12, %rbp
    xorq    %rdx, %rax
    movq    %r13, %r9
    movq    %rbx, -16(%rsp)
    xorq    %r8, %r10
    rolq    $27, %rax
    movq    %r13, %rbx
    rolq    $36, %r10
    rolq    $15, %rbp
    xorq    %r11, %rcx
    andq    %r10, %r9
    xorq    -40(%rsp), %r15
    rolq    $56, %rcx
    xorq    %rax, %r9
    orq %rbp, %rbx
    movq    %r9, -48(%rsp)
    movq    -120(%rsp), %r9
    notq    %rbp
    movq    %rbp, -80(%rsp)
    orq %rcx, %rbp
    xorq    %r10, %rbx
    xorq    -112(%rsp), %r15
    xorq    %r13, %rbp
    movq    %rcx, %r13
    andq    %rax, %r13
    xorq    -56(%rsp), %r9
    xorq    %rbx, %r15
    xorq    -64(%rsp), %r9
    xorq    -48(%rsp), %r9
    movq    %rbp, -96(%rsp)
    xorq    -80(%rsp), %r13
    orq %rax, %r10
    xorq    %rcx, %r10
    xorq    24(%rsp), %r11
    xorq    -24(%rsp), %r8
    movq    %r10, -80(%rsp)
    movq    56(%rsp), %rax
    movq    -72(%rsp), %r10
    movq    -32(%rsp), %rcx
    rolq    $55, %r11
    rolq    $41, %r8
    xorq    %rdx, %rax
    rolq    $39, %rax
    xorq    %rsi, %r10
    movq    %r11, %rsi
    xorq    %r12, %rcx
    movq    %r8, %r12
    rolq    $2, %r10
    notq    %rsi
    orq %rax, %r12
    rolq    $62, %rcx
    xorq    %rsi, %r12
    movq    %rsi, %rdx
    movq    %r10, %rsi
    andq    %r8, %rsi
    andq    %rax, %rdx
    xorq    %r12, %r15
    xorq    %rax, %rsi
    movq    %r10, %rax
    movq    %r12, -24(%rsp)
    orq %rcx, %rax
    movq    -104(%rsp), %r12
    andq    %rcx, %r11
    xorq    %r8, %rax
    xorq    %r10, %r11
    movq    48(%rsp), %r10
    movq    %rax, 16(%rsp)
    movq    32(%rsp), %rax
    xorq    %rcx, %rdx
    xorq    %rdx, %r9
    movq    %r15, %r8
    xorq    %r14, %r12
    xorq    -8(%rsp), %r12
    xorq    %r11, %r10
    xorq    8(%rsp), %rax
    xorq    %rbp, %r12
    xorq    %r13, %rax
    xorq    %rsi, %r12
    xorq    -16(%rsp), %rax
    movq    %r12, %rcx
    xorq    16(%rsp), %rax
    xorq    64(%rsp), %r10
    rolq    %r8
    rolq    %rcx
    xorq    %r9, %rcx
    rolq    %r9
    xorq    %rax, %r9
    movq    %rax, %rbp
    movq    -40(%rsp), %rax
    xorq    40(%rsp), %r10
    rolq    %rbp
    xorq    %r9, %r11
    xorq    %r15, %rbp
    rolq    $14, %r11
    movabsq $-9223372036854742912, %r15
    xorq    %rbp, %r14
    rolq    $43, %r14
    xorq    %rcx, %rax
    xorq    -80(%rsp), %r10
    rolq    $44, %rax
    xorq    %r10, %r8
    rolq    %r10
    xorq    %r12, %r10
    movq    -120(%rsp), %r12
    xorq    %r10, %r13
    rolq    $21, %r13
    xorq    %r8, %r12
    xorq    %r12, %r15
    movq    %r15, -120(%rsp)
    movq    %r14, %r15
    orq %rax, %r15
    xorq    %r15, -120(%rsp)
    movq    %r14, %r15
    notq    %r15
    orq %r13, %r15
    xorq    %rax, %r15
    andq    %r12, %rax
    movq    %r15, -72(%rsp)
    movq    %r11, %r15
    xorq    %r11, %rax
    andq    %r13, %r15
    movq    %rax, 24(%rsp)
    movq    -64(%rsp), %rax
    xorq    %r14, %r15
    movq    %r11, %r14
    movq    8(%rsp), %r11
    orq %r12, %r14
    movq    48(%rsp), %r12
    movq    %r15, -40(%rsp)
    xorq    %r13, %r14
    movq    %r14, (%rsp)
    xorq    %r10, %r11
    xorq    %rbp, %rsi
    xorq    %r8, %rax
    rolq    $61, %rsi
    xorq    %rcx, %rbx
    rolq    $3, %rax
    rolq    $45, %rbx
    xorq    %r9, %r12
    movq    %rsi, %r14
    rolq    $20, %r12
    movq    %rax, %r15
    movq    %rbx, %r13
    notq    %r14
    rolq    $28, %r11
    orq %r12, %r15
    andq    %rax, %r13
    orq %rbx, %r14
    xorq    %r11, %r15
    xorq    %r12, %r13
    xorq    %rax, %r14
    andq    %r11, %r12
    xorq    %rsi, %r12
    movq    %r15, 8(%rsp)
    movq    %rsi, %r15
    movq    %r13, -64(%rsp)
    movq    %r12, %rsi
    orq %r11, %r15
    movq    %r14, -32(%rsp)
    movq    -16(%rsp), %r13
    xorq    %rbx, %r15
    movq    %r12, 56(%rsp)
    movq    -104(%rsp), %r12
    xorq    %r8, %rdx
    movq    -88(%rsp), %r11
    movq    %r15, %rax
    rolq    $18, %rdx
    movq    -80(%rsp), %rbx
    movq    %r15, 48(%rsp)
    xorq    %r10, %r13
    xorq    (%rsp), %rax
    rolq    $25, %r13
    xorq    %rbp, %r12
    xorq    24(%rsp), %rsi
    rolq    $6, %r12
    xorq    %rcx, %r11
    movq    %r13, %r14
    rolq    %r11
    xorq    %r9, %rbx
    orq %r12, %r14
    rolq    $8, %rbx
    xorq    %r11, %r14
    movq    %rbx, %r15
    movq    %r14, -88(%rsp)
    movq    %rbx, %r14
    andq    %r13, %r15
    notq    %r14
    xorq    %r12, %r15
    andq    %r11, %r12
    movq    %r14, %rbx
    xorq    %rdx, %r12
    movq    %r15, -80(%rsp)
    andq    %rdx, %rbx
    movq    -96(%rsp), %r15
    movq    %r12, 72(%rsp)
    xorq    %r13, %rbx
    movq    %rdx, %r13
    xorq    %r12, %rsi
    orq %r11, %r13
    movq    -112(%rsp), %r11
    movq    %rbx, -104(%rsp)
    xorq    %r14, %r13
    movq    -56(%rsp), %r14
    movq    %r13, -16(%rsp)
    xorq    %r13, %rax
    movq    64(%rsp), %r13
    movq    16(%rsp), %r12
    xorq    %rbp, %r15
    xorq    %rcx, %r11
    rolq    $15, %r15
    rolq    $10, %r11
    xorq    %r8, %r14
    movq    %r15, %rbx
    xorq    %r9, %r13
    rolq    $36, %r14
    movq    %r11, %rdx
    rolq    $27, %r13
    andq    %r14, %rdx
    orq %r11, %rbx
    xorq    %r13, %rdx
    xorq    %r10, %r12
    xorq    %r14, %rbx
    movq    %rdx, 16(%rsp)
    movq    8(%rsp), %rdx
    notq    %r15
    rolq    $56, %r12
    movq    %rbx, -112(%rsp)
    movq    %r15, %rbx
    orq %r12, %rbx
    xorq    %r11, %rbx
    movq    %r12, %r11
    xorq    -120(%rsp), %rdx
    andq    %r13, %r11
    movq    %rbx, -96(%rsp)
    xorq    %r15, %r11
    xorq    %r11, %rax
    xorq    -88(%rsp), %rdx
    xorq    16(%rsp), %rdx
    orq %r13, %r14
    xorq    %r12, %r14
    xorq    32(%rsp), %r10
    movq    %r14, -56(%rsp)
    xorq    %r14, %rsi
    movq    -48(%rsp), %r14
    xorq    40(%rsp), %r9
    movq    -24(%rsp), %r13
    rolq    $55, %r10
    xorq    -8(%rsp), %rbp
    xorq    %r8, %r14
    rolq    $41, %r14
    rolq    $39, %r9
    xorq    %rcx, %r13
    movq    %r14, %r8
    movq    %r10, %rcx
    notq    %rcx
    orq %r9, %r8
    rolq    $2, %r13
    xorq    %rcx, %r8
    movq    %rcx, %r15
    movq    -40(%rsp), %rcx
    movq    %r8, -8(%rsp)
    movq    -80(%rsp), %r8
    movq    %r13, %r12
    andq    %r14, %r12
    rolq    $62, %rbp
    andq    %r9, %r15
    xorq    %r9, %r12
    movq    %r13, %r9
    xorq    %rbp, %r15
    xorq    %r12, %rcx
    orq %rbp, %r9
    andq    %r10, %rbp
    xorq    -64(%rsp), %r8
    xorq    %r14, %r9
    xorq    %r13, %rbp
    xorq    -32(%rsp), %rcx
    xorq    %r9, %rax
    xorq    %r15, %rdx
    movq    %rax, %r10
    movl    $2147483649, %r14d
    xorq    -112(%rsp), %r8
    xorq    -104(%rsp), %rcx
    xorq    -8(%rsp), %r8
    xorq    %rbx, %rcx
    xorq    -72(%rsp), %r8
    movq    %r9, 64(%rsp)
    movq    %rcx, %r9
    xorq    %rbp, %rsi
    rolq    %r9
    movq    -104(%rsp), %rbx
    xorq    %rdx, %r9
    rolq    %rdx
    xorq    %rax, %rdx
    movq    -64(%rsp), %rax
    rolq    %r10
    movq    %r8, %r13
    xorq    %r8, %r10
    xorq    %rdx, %rbp
    rolq    %r13
    xorq    %r10, %rbx
    rolq    $14, %rbp
    xorq    %rsi, %r13
    rolq    %rsi
    xorq    %rcx, %rsi
    movq    -120(%rsp), %rcx
    rolq    $43, %rbx
    xorq    %r9, %rax
    movq    %rbx, %r8
    xorq    %rsi, %r11
    rolq    $44, %rax
    rolq    $21, %r11
    orq %rax, %r8
    xorq    %r13, %rcx
    xorq    %rcx, %r14
    xorq    %r8, %r14
    movq    %rbx, %r8
    notq    %r8
    movq    %r14, -104(%rsp)
    movq    %rbp, %r14
    orq %r11, %r8
    andq    %r11, %r14
    xorq    %rax, %r8
    andq    %rcx, %rax
    xorq    %rbx, %r14
    xorq    %rbp, %rax
    movq    %rbp, %rbx
    movq    %r8, -120(%rsp)
    orq %rcx, %rbx
    movq    %rax, 40(%rsp)
    movq    (%rsp), %rax
    xorq    %r11, %rbx
    movq    -88(%rsp), %rcx
    movq    %r14, 32(%rsp)
    movq    56(%rsp), %r11
    movq    %rbx, -64(%rsp)
    movq    -112(%rsp), %r8
    xorq    %rsi, %rax
    rolq    $28, %rax
    xorq    %rdx, %r11
    xorq    %r13, %rcx
    xorq    %r10, %r12
    rolq    $3, %rcx
    xorq    %r9, %r8
    rolq    $20, %r11
    rolq    $45, %r8
    movq    %rcx, %rbp
    rolq    $61, %r12
    orq %r11, %rbp
    movq    %r8, %r14
    movq    %r12, %rbx
    xorq    %rax, %rbp
    andq    %rcx, %r14
    notq    %rbx
    xorq    %r11, %r14
    movq    %rbp, -48(%rsp)
    movq    %r12, %rbp
    movq    %r14, (%rsp)
    orq %rax, %rbp
    movq    -56(%rsp), %r14
    xorq    %r8, %rbp
    orq %r8, %rbx
    movq    -16(%rsp), %r8
    xorq    %rcx, %rbx
    movq    -32(%rsp), %rcx
    andq    %rax, %r11
    movq    -72(%rsp), %rax
    xorq    %r12, %r11
    xorq    %r13, %r15
    xorq    %rdx, %r14
    rolq    $18, %r15
    movq    %rbx, -88(%rsp)
    xorq    %rsi, %r8
    rolq    $8, %r14
    movq    %r11, -112(%rsp)
    rolq    $25, %r8
    xorq    %r10, %rcx
    movq    %r14, %r12
    notq    %r14
    rolq    $6, %rcx
    xorq    %r9, %rax
    movq    %r8, %r11
    movq    %r14, %rbx
    rolq    %rax
    orq %rcx, %r11
    andq    %r8, %r12
    andq    %r15, %rbx
    movq    %rbp, -24(%rsp)
    xorq    %rax, %r11
    xorq    %rcx, %r12
    xorq    %r8, %rbx
    movq    %r15, %rbp
    movq    %r11, -72(%rsp)
    movq    %rbx, 56(%rsp)
    movq    -24(%rsp), %rbx
    orq %rax, %rbp
    xorq    %r14, %rbp
    andq    %rax, %rcx
    movq    -80(%rsp), %r11
    movq    %rbp, -32(%rsp)
    xorq    %r15, %rcx
    movq    8(%rsp), %r8
    movq    64(%rsp), %rax
    movq    %rcx, 80(%rsp)
    xorq    -64(%rsp), %rbx
    movq    %r12, -16(%rsp)
    movq    24(%rsp), %rcx
    xorq    %r9, %r11
    rolq    $10, %r11
    xorq    %r13, %r8
    xorq    %rsi, %rax
    rolq    $36, %r8
    movq    %r11, %r12
    xorq    %rbp, %rbx
    movq    -96(%rsp), %rbp
    rolq    $56, %rax
    xorq    %rdx, %rcx
    andq    %r8, %r12
    rolq    $27, %rcx
    xorq    %rcx, %r12
    xorq    %r10, %rbp
    movq    %r12, 8(%rsp)
    xorq    72(%rsp), %rdx
    rolq    $15, %rbp
    movq    %rbp, %r14
    notq    %rbp
    movq    %rbp, %r15
    orq %r11, %r14
    orq %rax, %r15
    xorq    %r8, %r14
    orq %rcx, %r8
    xorq    %r11, %r15
    movq    %rax, %r11
    xorq    %rax, %r8
    andq    %rcx, %r11
    movq    -40(%rsp), %rcx
    movq    %r8, -96(%rsp)
    xorq    %rbp, %r11
    movq    -8(%rsp), %rax
    movq    %r14, -80(%rsp)
    xorq    %r11, %rbx
    movq    %r15, -56(%rsp)
    xorq    %r10, %rcx
    movq    48(%rsp), %r10
    rolq    $62, %rcx
    xorq    %rsi, %r10
    movq    -48(%rsp), %rsi
    rolq    $55, %r10
    rolq    $39, %rdx
    xorq    16(%rsp), %r13
    xorq    %r9, %rax
    movq    %r10, %r8
    rolq    $2, %rax
    notq    %r8
    andq    %rcx, %r10
    xorq    -104(%rsp), %rsi
    movq    %r8, %r9
    xorq    %rax, %r10
    rolq    $41, %r13
    andq    %rdx, %r9
    movq    %r13, %rbp
    xorq    %rcx, %r9
    orq %rdx, %rbp
    xorq    -72(%rsp), %rsi
    xorq    %r8, %rbp
    movq    %rax, %r8
    andq    %r13, %r8
    movq    %rbp, 16(%rsp)
    xorq    %rdx, %r8
    xorq    %r12, %rsi
    movq    -16(%rsp), %r12
    xorq    %r9, %rsi
    xorq    (%rsp), %r12
    xorq    %r14, %r12
    movq    %rax, %r14
    orq %rcx, %r14
    xorq    %rbp, %r12
    movq    32(%rsp), %rbp
    xorq    %r13, %r14
    movq    -112(%rsp), %r13
    xorq    -120(%rsp), %r12
    xorq    %r14, %rbx
    movq    %rbx, %rdx
    xorq    %r8, %rbp
    xorq    40(%rsp), %r13
    xorq    -88(%rsp), %rbp
    movq    %r12, %rcx
    rolq    %rcx
    xorq    80(%rsp), %r13
    xorq    56(%rsp), %rbp
    xorq    -96(%rsp), %r13
    xorq    %r15, %rbp
    movq    -64(%rsp), %r15
    movq    %rbp, %rax
    xorq    %r10, %r13
    xorq    %r13, %rcx
    rolq    %r13
    rolq    %rax
    xorq    %rbp, %r13
    rolq    %rdx
    xorq    %r13, %r15
    xorq    %rsi, %rax
    rolq    %rsi
    rolq    $28, %r15
    xorq    %rbx, %rsi
    xorq    %r12, %rdx
    movq    %r15, -64(%rsp)
    movq    -112(%rsp), %r15
    xorq    %rdx, %r8
    movq    -104(%rsp), %r12
    rolq    $61, %r8
    xorq    %r13, %r11
    movq    56(%rsp), %rbx
    xorq    %rsi, %r10
    rolq    $21, %r11
    movabsq $-9223372034707259384, %rbp
    rolq    $14, %r10
    xorq    %rsi, %r15
    rolq    $20, %r15
    xorq    %rcx, %r12
    movq    %r15, -112(%rsp)
    movq    -72(%rsp), %r15
    xorq    %rdx, %rbx
    movq    %r12, -104(%rsp)
    movq    (%rsp), %r12
    rolq    $43, %rbx
    xorq    -104(%rsp), %rbp
    xorq    %rcx, %r15
    rolq    $3, %r15
    xorq    %rax, %r12
    movq    %r15, -72(%rsp)
    movq    -80(%rsp), %r15
    rolq    $44, %r12
    xorq    %rax, %r15
    rolq    $45, %r15
    movq    %r15, -80(%rsp)
    movq    -88(%rsp), %r15
    movq    %r8, -8(%rsp)
    movq    -120(%rsp), %r8
    xorq    %rdx, %r15
    xorq    %rax, %r8
    rolq    $6, %r15
    rolq    %r8
    movq    %r15, -88(%rsp)
    movq    -96(%rsp), %r15
    movq    %r8, -120(%rsp)
    movq    -32(%rsp), %r8
    xorq    %r13, %r8
    xorq    %rcx, %r9
    xorq    %rsi, %r15
    rolq    $25, %r8
    rolq    $18, %r9
    movq    %r8, -32(%rsp)
    movq    -48(%rsp), %r8
    rolq    $8, %r15
    movq    %r9, -40(%rsp)
    movq    -16(%rsp), %r9
    movq    %r15, -96(%rsp)
    notq    %r15
    movq    %r15, (%rsp)
    movq    40(%rsp), %r15
    xorq    %rcx, %r8
    xorq    8(%rsp), %rcx
    rolq    $36, %r8
    xorq    %rax, %r9
    xorq    16(%rsp), %rax
    movq    %r8, -48(%rsp)
    movq    -56(%rsp), %r8
    rolq    $10, %r9
    xorq    %rsi, %r15
    xorq    80(%rsp), %rsi
    rolq    $27, %r15
    rolq    $41, %rcx
    rolq    $2, %rax
    xorq    %rdx, %r8
    xorq    32(%rsp), %rdx
    rolq    $15, %r8
    movq    %r8, -56(%rsp)
    movq    %r13, %r8
    xorq    -24(%rsp), %r13
    xorq    %r14, %r8
    movq    -56(%rsp), %r14
    rolq    $39, %rsi
    rolq    $56, %r8
    rolq    $62, %rdx
    rolq    $55, %r13
    notq    %r14
    movq    %r13, -24(%rsp)
    notq    %r13
    movq    %r14, -16(%rsp)
    movq    %r12, %r14
    orq %rbx, %r14
    xorq    %r14, %rbp
    movq    %rbp, (%rdi)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r12, %rbp
    movq    %rbp, 8(%rdi)
    movq    %r10, %rbp
    andq    %r11, %rbp
    xorq    %rbx, %rbp
    movq    -104(%rsp), %rbx
    movq    %rbp, 16(%rdi)
    orq %r10, %rbx
    xorq    %r11, %rbx
    andq    -104(%rsp), %r12
    movq    %rbx, 24(%rdi)
    xorq    %r10, %r12
    movq    -72(%rsp), %r10
    movq    %r12, 32(%rdi)
    orq -112(%rsp), %r10
    xorq    -64(%rsp), %r10
    movq    %r10, 40(%rdi)
    movq    -80(%rsp), %r10
    andq    -72(%rsp), %r10
    xorq    -112(%rsp), %r10
    movq    %r10, 48(%rdi)
    movq    -8(%rsp), %r10
    notq    %r10
    orq -80(%rsp), %r10
    xorq    -72(%rsp), %r10
    movq    %r10, 56(%rdi)
    movq    -8(%rsp), %r10
    orq -64(%rsp), %r10
    xorq    -80(%rsp), %r10
    movq    %r10, 64(%rdi)
    movq    -112(%rsp), %r10
    andq    -64(%rsp), %r10
    xorq    -8(%rsp), %r10
    movq    %r10, 72(%rdi)
    movq    -32(%rsp), %r10
    orq -88(%rsp), %r10
    xorq    -120(%rsp), %r10
    movq    %r10, 80(%rdi)
    movq    -96(%rsp), %r10
    andq    -32(%rsp), %r10
    xorq    -88(%rsp), %r10
    movq    %r10, 88(%rdi)
    movq    (%rsp), %r10
    andq    -40(%rsp), %r10
    xorq    -32(%rsp), %r10
    movq    %r10, 96(%rdi)
    movq    -40(%rsp), %r10
    orq -120(%rsp), %r10
    xorq    (%rsp), %r10
    movq    %r10, 104(%rdi)
    movq    -120(%rsp), %r10
    andq    -88(%rsp), %r10
    xorq    -40(%rsp), %r10
    movq    %r10, 112(%rdi)
    movq    -48(%rsp), %r10
    andq    %r9, %r10
    xorq    %r15, %r10
    movq    %r10, 120(%rdi)
    movq    -56(%rsp), %r10
    orq %r9, %r10
    xorq    -48(%rsp), %r10
    movq    %r10, 128(%rdi)
    movq    -16(%rsp), %r10
    orq %r8, %r10
    xorq    %r9, %r10
    movq    %r8, %r9
    andq    %r15, %r9
    movq    %r10, 136(%rdi)
    xorq    -16(%rsp), %r9
    movq    %r9, 144(%rdi)
    movq    -48(%rsp), %r9
    orq %r15, %r9
    xorq    %r8, %r9
    movq    %r13, %r8
    andq    %rsi, %r8
    movq    %r9, 152(%rdi)
    xorq    %rdx, %r8
    movq    %r8, 160(%rdi)
    movq    %rcx, %r8
    orq %rsi, %r8
    xorq    %r13, %r8
    movq    %r8, 168(%rdi)
    movq    %rax, %r8
    andq    %rcx, %r8
    xorq    %rsi, %r8
    movq    %rax, %rsi
    orq %rdx, %rsi
    andq    -24(%rsp), %rdx
    movq    %r8, 176(%rdi)
    xorq    %rcx, %rsi
    movq    %rsi, 184(%rdi)
    xorq    %rax, %rdx
    movq    %rdx, 192(%rdi)
    addq    $88, %rsp
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
    .cfi_endproc
.LFE32:
    .size   KeccakP1600_Permute_12rounds, .-KeccakP1600_Permute_12rounds
    .p2align 4,,15
    .globl  KeccakP1600_ExtractBytesInLane
    .type   KeccakP1600_ExtractBytesInLane, @function
KeccakP1600_ExtractBytesInLane:
.LFB33:
    .cfi_startproc
    movq    %rdx, %r9
    subq    $24, %rsp
    .cfi_def_cfa_offset 32
    movl    %esi, %eax
    leal    -1(%rsi), %edx
    movq    (%rdi,%rax,8), %rax
    cmpl    $1, %edx
    jbe .L161
    cmpl    $8, %esi
    jne .L164
.L161:
    notq    %rax
.L162:
    movl    %ecx, %ecx
    movl    %r8d, %edx
    movq    %r9, %rdi
    leaq    (%rsp,%rcx), %rsi
    movq    %rax, (%rsp)
    call    memcpy
    addq    $24, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 8
    ret
    .p2align 4,,10
    .p2align 3
.L164:
    .cfi_restore_state
    cmpl    $17, %esi
    je  .L161
    cmpl    $12, %esi
    .p2align 4,,2
    je  .L161
    cmpl    $20, %esi
    .p2align 4,,2
    jne .L162
    .p2align 4,,5
    jmp .L161
    .cfi_endproc
.LFE33:
    .size   KeccakP1600_ExtractBytesInLane, .-KeccakP1600_ExtractBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_ExtractLanes
    .type   KeccakP1600_ExtractLanes, @function
KeccakP1600_ExtractLanes:
.LFB34:
    .cfi_startproc
    movq    %rbx, -16(%rsp)
    .cfi_offset 3, -24
    movq    %rsi, %rbx
    movq    %rdi, %rsi
    movq    %rbp, -8(%rsp)
    movq    %rbx, %rdi
    subq    $24, %rsp
    .cfi_def_cfa_offset 32
    .cfi_offset 6, -16
    movl    %edx, %ebp
    leal    0(,%rdx,8), %edx
    call    memcpy
    cmpl    $1, %ebp
    jbe .L165
    notq    8(%rbx)
    cmpl    $2, %ebp
    je  .L165
    notq    16(%rbx)
    cmpl    $8, %ebp
    jbe .L165
    notq    64(%rbx)
    cmpl    $12, %ebp
    jbe .L165
    notq    96(%rbx)
    cmpl    $17, %ebp
    jbe .L165
    notq    136(%rbx)
    cmpl    $20, %ebp
    jbe .L165
    notq    160(%rbx)
    .p2align 4,,10
    .p2align 3
.L165:
    movq    8(%rsp), %rbx
    movq    16(%rsp), %rbp
    addq    $24, %rsp
    .cfi_def_cfa_offset 8
    ret
    .cfi_endproc
.LFE34:
    .size   KeccakP1600_ExtractLanes, .-KeccakP1600_ExtractLanes
    .p2align 4,,15
    .globl  KeccakP1600_ExtractBytes
    .type   KeccakP1600_ExtractBytes, @function
KeccakP1600_ExtractBytes:
.LFB35:
    .cfi_startproc
    pushq   %r15
    .cfi_def_cfa_offset 16
    .cfi_offset 15, -16
    movq    %rsi, %r15
    pushq   %r14
    .cfi_def_cfa_offset 24
    .cfi_offset 14, -24
    pushq   %r13
    .cfi_def_cfa_offset 32
    .cfi_offset 13, -32
    movq    %rdi, %r13
    pushq   %r12
    .cfi_def_cfa_offset 40
    .cfi_offset 12, -40
    pushq   %rbp
    .cfi_def_cfa_offset 48
    .cfi_offset 6, -48
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $40, %rsp
    .cfi_def_cfa_offset 96
    testl   %edx, %edx
    je  .L189
    movl    %edx, %ebx
    movl    %edx, %r8d
    shrl    $3, %ebx
    andl    $7, %r8d
    testl   %ecx, %ecx
    je  .L173
    movl    %ecx, %ebp
    jmp .L185
    .p2align 4,,10
    .p2align 3
.L190:
    cmpl    $8, %ebx
    je  .L183
    cmpl    $17, %ebx
    .p2align 4,,3
    je  .L183
    cmpl    $12, %ebx
    .p2align 4,,2
    je  .L183
    cmpl    $20, %ebx
    .p2align 4,,2
    je  .L183
    .p2align 4,,10
    .p2align 3
.L184:
    movl    %r12d, %r14d
    movq    %r15, %rdi
    movq    %rax, 16(%rsp)
    leaq    16(%rsp), %rsi
    movq    %r14, %rdx
    addl    $1, %ebx
    addq    %r8, %rsi
    addq    %r14, %r15
    call    memcpy
    xorl    %r8d, %r8d
    subl    %r12d, %ebp
    je  .L173
.L185:
    leal    -1(%rbx), %edx
    movl    $8, %r12d
    movl    %ebx, %eax
    subl    %r8d, %r12d
    movq    0(%r13,%rax,8), %rax
    cmpl    %ebp, %r12d
    cmova   %ebp, %r12d
    cmpl    $1, %edx
    ja  .L190
.L183:
    notq    %rax
    jmp .L184
.L177:
    movq    16(%r13), %rax
    leaq    (%r15,%r12), %rdi
    movl    %ecx, %ebp
    andl    $7, %ebp
.L179:
    notq    %rax
.L180:
    leaq    16(%rsp), %rsi
    movl    %ebp, %edx
    movq    %rax, 16(%rsp)
    call    memcpy
.L173:
    addq    $40, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
    .p2align 4,,10
    .p2align 3
.L189:
    .cfi_restore_state
    movl    %ecx, %ebx
    movq    %rdi, %rsi
    movq    %r15, %rdi
    shrl    $3, %ebx
    movl    %ecx, 8(%rsp)
    leal    0(,%rbx,8), %r12d
    movq    %r12, %rdx
    call    memcpy
    cmpl    $1, %ebx
    movl    8(%rsp), %ecx
    jbe .L176
    notq    8(%r15)
    cmpl    $2, %ebx
    je  .L177
    notq    16(%r15)
    cmpl    $8, %ebx
    jbe .L176
    notq    64(%r15)
    cmpl    $12, %ebx
    jbe .L176
    notq    96(%r15)
    cmpl    $17, %ebx
    jbe .L176
    notq    136(%r15)
    cmpl    $20, %ebx
    jbe .L176
    notq    160(%r15)
    .p2align 4,,10
    .p2align 3
.L176:
    leal    -1(%rbx), %edx
    movl    %ecx, %ebp
    movl    %ebx, %eax
    leaq    (%r15,%r12), %rdi
    andl    $7, %ebp
    cmpl    $1, %edx
    movq    0(%r13,%rax,8), %rax
    jbe .L179
    cmpl    $8, %ebx
    je  .L179
    cmpl    $17, %ebx
    je  .L179
    cmpl    $12, %ebx
    .p2align 4,,2
    je  .L179
    cmpl    $20, %ebx
    .p2align 4,,2
    jne .L180
    .p2align 4,,5
    jmp .L179
    .cfi_endproc
.LFE35:
    .size   KeccakP1600_ExtractBytes, .-KeccakP1600_ExtractBytes
    .p2align 4,,15
    .globl  KeccakP1600_ExtractAndAddBytesInLane
    .type   KeccakP1600_ExtractAndAddBytesInLane, @function
KeccakP1600_ExtractAndAddBytesInLane:
.LFB36:
    .cfi_startproc
    movl    %esi, %eax
    movq    (%rdi,%rax,8), %rax
    leal    -1(%rsi), %edi
    cmpl    $1, %edi
    jbe .L192
    cmpl    $8, %esi
    jne .L199
.L192:
    notq    %rax
.L193:
    movq    %rax, -24(%rsp)
    xorl    %eax, %eax
    testl   %r9d, %r9d
    je  .L191
    .p2align 4,,10
    .p2align 3
.L197:
    leal    (%r8,%rax), %esi
    movzbl  -24(%rsp,%rsi), %esi
    xorb    (%rdx,%rax), %sil
    movb    %sil, (%rcx,%rax)
    addq    $1, %rax
    cmpl    %eax, %r9d
    ja  .L197
.L191:
    rep
    ret
    .p2align 4,,10
    .p2align 3
.L199:
    cmpl    $17, %esi
    je  .L192
    cmpl    $12, %esi
    .p2align 4,,5
    je  .L192
    cmpl    $20, %esi
    .p2align 4,,2
    jne .L193
    .p2align 4,,5
    jmp .L192
    .cfi_endproc
.LFE36:
    .size   KeccakP1600_ExtractAndAddBytesInLane, .-KeccakP1600_ExtractAndAddBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_ExtractAndAddLanes
    .type   KeccakP1600_ExtractAndAddLanes, @function
KeccakP1600_ExtractAndAddLanes:
.LFB37:
    .cfi_startproc
    testl   %ecx, %ecx
    je  .L200
    leaq    16(%rdx), %r9
    movl    %ecx, %r10d
    leaq    16(%rsi), %r11
    shrl    %r10d
    cmpq    %r9, %rsi
    leal    (%r10,%r10), %r8d
    setae   %al
    cmpq    %r11, %rdx
    setae   %r11b
    orl %r11d, %eax
    cmpq    %r9, %rdi
    leaq    16(%rdi), %r11
    setae   %r9b
    cmpq    %r11, %rdx
    setae   %r11b
    orl %r11d, %r9d
    andl    %r9d, %eax
    cmpl    $7, %ecx
    seta    %r9b
    testb   %r9b, %al
    je  .L208
    testl   %r8d, %r8d
    je  .L208
    xorl    %eax, %eax
    xorl    %r9d, %r9d
    .p2align 4,,10
    .p2align 3
.L204:
    movdqu  (%rdi,%rax), %xmm1
    addl    $1, %r9d
    movdqu  (%rsi,%rax), %xmm0
    pxor    %xmm1, %xmm0
    movdqu  %xmm0, (%rdx,%rax)
    addq    $16, %rax
    cmpl    %r9d, %r10d
    ja  .L204
    cmpl    %r8d, %ecx
    je  .L205
    .p2align 4,,10
    .p2align 3
.L211:
    movl    %r8d, %eax
    addl    $1, %r8d
    movq    (%rdi,%rax,8), %r9
    xorq    (%rsi,%rax,8), %r9
    cmpl    %r8d, %ecx
    movq    %r9, (%rdx,%rax,8)
    ja  .L211
    cmpl    $1, %ecx
    jbe .L200
.L205:
    notq    8(%rdx)
    cmpl    $2, %ecx
    je  .L200
    notq    16(%rdx)
    cmpl    $8, %ecx
    jbe .L200
    notq    64(%rdx)
    cmpl    $12, %ecx
    jbe .L200
    notq    96(%rdx)
    cmpl    $17, %ecx
    jbe .L200
    notq    136(%rdx)
    cmpl    $20, %ecx
    jbe .L200
    notq    160(%rdx)
    .p2align 4,,10
    .p2align 3
.L200:
    rep
    ret
.L208:
    xorl    %r8d, %r8d
    jmp .L211
    .cfi_endproc
.LFE37:
    .size   KeccakP1600_ExtractAndAddLanes, .-KeccakP1600_ExtractAndAddLanes
    .p2align 4,,15
    .globl  KeccakP1600_ExtractAndAddBytes
    .type   KeccakP1600_ExtractAndAddBytes, @function
KeccakP1600_ExtractAndAddBytes:
.LFB38:
    .cfi_startproc
    pushq   %r12
    .cfi_def_cfa_offset 16
    .cfi_offset 12, -16
    testl   %ecx, %ecx
    pushq   %rbp
    .cfi_def_cfa_offset 24
    .cfi_offset 6, -24
    pushq   %rbx
    .cfi_def_cfa_offset 32
    .cfi_offset 3, -32
    je  .L250
    movl    %ecx, %r11d
    movl    %ecx, %r10d
    movl    $8, %ebx
    shrl    $3, %r11d
    andl    $7, %r10d
    testl   %r8d, %r8d
    je  .L215
    .p2align 4,,10
    .p2align 3
.L242:
    leal    -1(%r11), %ecx
    movl    %ebx, %r9d
    movl    %r11d, %eax
    subl    %r10d, %r9d
    movq    (%rdi,%rax,8), %rax
    cmpl    %r8d, %r9d
    cmova   %r8d, %r9d
    cmpl    $1, %ecx
    jbe .L231
    cmpl    $8, %r11d
    je  .L231
    cmpl    $17, %r11d
    je  .L231
    cmpl    $12, %r11d
    je  .L231
    cmpl    $20, %r11d
    je  .L231
    .p2align 4,,10
    .p2align 3
.L232:
    movq    %rax, -16(%rsp)
    xorl    %eax, %eax
    .p2align 4,,10
    .p2align 3
.L233:
    leal    (%r10,%rax), %ecx
    movzbl  -16(%rsp,%rcx), %ecx
    xorb    (%rsi,%rax), %cl
    movb    %cl, (%rdx,%rax)
    addq    $1, %rax
    cmpl    %eax, %r9d
    ja  .L233
    movl    %r9d, %eax
    addl    $1, %r11d
    xorl    %r10d, %r10d
    addq    %rax, %rsi
    addq    %rax, %rdx
    subl    %r9d, %r8d
    jne .L242
.L215:
    popq    %rbx
    .cfi_remember_state
    .cfi_def_cfa_offset 24
    popq    %rbp
    .cfi_def_cfa_offset 16
    popq    %r12
    .cfi_def_cfa_offset 8
    ret
    .p2align 4,,10
    .p2align 3
.L231:
    .cfi_restore_state
    notq    %rax
    jmp .L232
.L250:
    movl    %r8d, %r10d
    shrl    $3, %r10d
    testl   %r10d, %r10d
    je  .L218
    leaq    16(%rdx), %r11
    movl    %r8d, %ebp
    leaq    16(%rsi), %rbx
    shrl    $4, %ebp
    cmpq    %r11, %rsi
    leaq    16(%rdi), %r12
    setae   %al
    cmpq    %rbx, %rdx
    leal    (%rbp,%rbp), %ecx
    setae   %r9b
    orl %r9d, %eax
    cmpl    $7, %r10d
    seta    %r9b
    andl    %r9d, %eax
    cmpq    %r11, %rdi
    setae   %r9b
    cmpq    %r12, %rdx
    setae   %r12b
    orl %r12d, %r9d
    testb   %r9b, %al
    je  .L236
    testl   %ecx, %ecx
    je  .L236
    xorl    %eax, %eax
    xorl    %r9d, %r9d
    .p2align 4,,10
    .p2align 3
.L220:
    movdqu  (%rdi,%rax), %xmm1
    addl    $1, %r9d
    movdqu  (%rsi,%rax), %xmm0
    pxor    %xmm1, %xmm0
    movdqu  %xmm0, (%rdx,%rax)
    addq    $16, %rax
    cmpl    %ebp, %r9d
    jb  .L220
    cmpl    %ecx, %r10d
    je  .L221
    .p2align 4,,10
    .p2align 3
.L241:
    movl    %ecx, %eax
    addl    $1, %ecx
    movq    (%rdi,%rax,8), %r9
    xorq    (%rsi,%rax,8), %r9
    cmpl    %ecx, %r10d
    movq    %r9, (%rdx,%rax,8)
    ja  .L241
    cmpl    $1, %r10d
    je  .L218
.L221:
    notq    8(%rdx)
    cmpl    $2, %r10d
    je  .L224
    notq    16(%rdx)
    cmpl    $8, %r10d
    jbe .L218
    notq    64(%rdx)
    cmpl    $12, %r10d
    jbe .L218
    notq    96(%rdx)
    cmpl    $17, %r10d
    jbe .L218
    notq    136(%rdx)
    cmpl    $20, %r10d
    jbe .L218
    notq    160(%rdx)
.L218:
    leal    0(,%r10,8), %ebx
    andl    $7, %r8d
    movl    %r10d, %eax
    leaq    (%rdx,%rbx), %r11
    addq    %rsi, %rbx
    movq    (%rdi,%rax,8), %rax
    leal    -1(%r10), %edx
    cmpl    $1, %edx
    jbe .L225
    cmpl    $8, %r10d
    je  .L225
    cmpl    $17, %r10d
    je  .L225
    cmpl    $12, %r10d
    jne .L251
.L225:
    notq    %rax
.L226:
    movq    %rax, -16(%rsp)
    xorl    %eax, %eax
    testl   %r8d, %r8d
    je  .L215
    .p2align 4,,10
    .p2align 3
.L240:
    movl    %eax, %edx
    addl    $1, %eax
    movzbl  -16(%rsp,%rdx), %ecx
    xorb    (%rbx,%rdx), %cl
    cmpl    %eax, %r8d
    movb    %cl, (%r11,%rdx)
    ja  .L240
    popq    %rbx
    .cfi_remember_state
    .cfi_def_cfa_offset 24
    popq    %rbp
    .cfi_def_cfa_offset 16
    popq    %r12
    .cfi_def_cfa_offset 8
    ret
.L251:
    .cfi_restore_state
    cmpl    $20, %r10d
    jne .L226
    .p2align 4,,4
    jmp .L225
.L224:
    andl    $7, %r8d
    movq    16(%rdi), %rax
    jmp .L225
.L236:
    xorl    %ecx, %ecx
    jmp .L241
    .cfi_endproc
.LFE38:
    .size   KeccakP1600_ExtractAndAddBytes, .-KeccakP1600_ExtractAndAddBytes
    .p2align 4,,15
    .globl  KeccakF1600_FastLoop_Absorb
    .type   KeccakF1600_FastLoop_Absorb, @function
KeccakF1600_FastLoop_Absorb:
.LFB39:
    .cfi_startproc
    pushq   %r15
    .cfi_def_cfa_offset 16
    .cfi_offset 15, -16
    pushq   %r14
    .cfi_def_cfa_offset 24
    .cfi_offset 14, -24
    pushq   %r13
    .cfi_def_cfa_offset 32
    .cfi_offset 13, -32
    pushq   %r12
    .cfi_def_cfa_offset 40
    .cfi_offset 12, -40
    pushq   %rbp
    .cfi_def_cfa_offset 48
    .cfi_offset 6, -48
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $144, %rsp
    .cfi_def_cfa_offset 200
    movq    8(%rdi), %rax
    movq    %rdi, 88(%rsp)
    movq    32(%rdi), %rbx
    movl    %esi, 108(%rsp)
    movq    %rcx, 136(%rsp)
    movq    40(%rdi), %rsi
    movq    16(%rdi), %rcx
    movq    (%rdi), %r11
    movq    %rax, -40(%rsp)
    movq    24(%rdi), %r8
    movq    %rbx, -96(%rsp)
    movq    48(%rdi), %rdi
    movq    %rsi, -80(%rsp)
    movq    %rcx, -112(%rsp)
    movq    %rdi, -120(%rsp)
    movq    88(%rsp), %rdi
    movq    80(%rdi), %r12
    movq    88(%rdi), %r14
    movq    64(%rdi), %rbp
    movq    72(%rdi), %r10
    movq    104(%rdi), %r15
    movq    %r12, -104(%rsp)
    movq    56(%rdi), %r13
    movq    %r14, -24(%rsp)
    movq    96(%rdi), %rsi
    movq    %rbp, -72(%rsp)
    movq    %r10, -32(%rsp)
    movq    %r15, -48(%rsp)
    movq    120(%rdi), %rcx
    movq    112(%rdi), %rax
    movq    128(%rdi), %rbx
    movq    136(%rdi), %r15
    movq    %rcx, -16(%rsp)
    movq    144(%rdi), %r9
    movq    152(%rdi), %r14
    movq    %rax, -64(%rsp)
    movq    160(%rdi), %rcx
    movq    %rbx, -56(%rsp)
    movq    168(%rdi), %rdi
    movl    108(%rsp), %eax
    movq    %rdi, 24(%rsp)
    movq    88(%rsp), %rdi
    sall    $3, %eax
    cmpq    %rax, 136(%rsp)
    movq    %rax, 120(%rsp)
    movq    176(%rdi), %rdi
    movq    %rdi, (%rsp)
    movq    88(%rsp), %rdi
    movq    184(%rdi), %rdi
    movq    %rdi, -8(%rsp)
    movq    88(%rsp), %rdi
    movq    192(%rdi), %r12
    jb  .L269
    movl    108(%rsp), %eax
    movq    %r15, 32(%rsp)
    movq    %r12, %rdi
    movq    136(%rsp), %rbp
    movq    %rdx, 80(%rsp)
    movq    %rcx, %r15
    movq    %r11, -88(%rsp)
    movq    %r13, %r12
    salq    $3, %rax
    subq    120(%rsp), %rbp
    movq    %rax, 128(%rsp)
    movq    %r9, %rax
    cmpl    $21, 108(%rsp)
    movq    %rbp, 112(%rsp)
    movq    %r8, %rbp
    je  .L311
    .p2align 4,,10
    .p2align 3
.L254:
    cmpl    $15, 108(%rsp)
    ja  .L256
    cmpl    $7, 108(%rsp)
    ja  .L257
    cmpl    $3, 108(%rsp)
    ja  .L258
    cmpl    $1, 108(%rsp)
    jbe .L312
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    (%r10), %r10
    movq    8(%r11), %r11
    xorq    %r10, -88(%rsp)
    xorq    %r11, -40(%rsp)
    cmpl    $2, 108(%rsp)
    jne .L313
    .p2align 4,,10
    .p2align 3
.L255:
    movq    -120(%rsp), %rbx
    movq    -80(%rsp), %rcx
    movq    -112(%rsp), %r11
    movq    -32(%rsp), %r13
    xorq    -40(%rsp), %rbx
    xorq    -88(%rsp), %rcx
    movq    -72(%rsp), %rdx
    xorq    %r12, %r11
    xorq    %rsi, %r11
    xorq    -96(%rsp), %r13
    xorq    -24(%rsp), %rbx
    xorq    32(%rsp), %r11
    xorq    -104(%rsp), %rcx
    xorq    %rbp, %rdx
    xorq    -48(%rsp), %rdx
    xorq    -56(%rsp), %rbx
    xorq    (%rsp), %r11
    xorq    -64(%rsp), %r13
    xorq    -16(%rsp), %rcx
    xorq    %rax, %rdx
    xorq    24(%rsp), %rbx
    xorq    -8(%rsp), %rdx
    movq    %r11, %r8
    xorq    %r14, %r13
    rolq    %r8
    xorq    %r15, %rcx
    xorq    %rdi, %r13
    movq    %rbx, %r10
    xorq    %rcx, %r8
    rolq    %rcx
    rolq    %r10
    xorq    %rdx, %rcx
    movq    %rdx, %r9
    xorq    %r13, %r10
    rolq    %r13
    movq    -120(%rsp), %rdx
    xorq    %r11, %r13
    movq    -88(%rsp), %r11
    rolq    %r9
    xorq    %rbx, %r9
    xorq    %r8, %rdx
    xorq    %r10, %r11
    rolq    $44, %rdx
    xorq    %r9, %rsi
    movq    %r11, %rbx
    rolq    $43, %rsi
    xorq    %r13, %rax
    xorq    $1, %rbx
    rolq    $21, %rax
    xorq    %rcx, %rdi
    movq    %rbx, 8(%rsp)
    movq    %rsi, %rbx
    rolq    $14, %rdi
    orq %rdx, %rbx
    xorq    %rbx, 8(%rsp)
    movq    %rsi, %rbx
    notq    %rbx
    orq %rax, %rbx
    xorq    %rdx, %rbx
    andq    %r11, %rdx
    movq    %rbx, -88(%rsp)
    movq    %rdi, %rbx
    xorq    %rdi, %rdx
    andq    %rax, %rbx
    movq    %rdx, 56(%rsp)
    movq    -32(%rsp), %rdx
    xorq    %rsi, %rbx
    movq    %rdi, %rsi
    movq    %r13, %rdi
    orq %r11, %rsi
    movq    -104(%rsp), %r11
    xorq    %rbp, %rdi
    xorq    %rax, %rsi
    movq    -56(%rsp), %rax
    rolq    $28, %rdi
    movq    %rsi, 40(%rsp)
    movq    (%rsp), %rsi
    xorq    %rcx, %rdx
    rolq    $20, %rdx
    movq    %rbx, 16(%rsp)
    xorq    %r10, %r11
    rolq    $3, %r11
    xorq    %r8, %rax
    movq    %r11, %rbp
    xorq    %r9, %rsi
    rolq    $45, %rax
    orq %rdx, %rbp
    rolq    $61, %rsi
    movq    %rax, %rbx
    xorq    %rdi, %rbp
    andq    %r11, %rbx
    movq    %rbp, -104(%rsp)
    movq    %rsi, %rbp
    xorq    %rdx, %rbx
    notq    %rbp
    movq    %rbx, -32(%rsp)
    orq %rax, %rbp
    xorq    %r11, %rbp
    movq    %rsi, %r11
    movq    %rbp, -120(%rsp)
    movq    -48(%rsp), %rbx
    orq %rdi, %r11
    andq    %rdi, %rdx
    xorq    %rcx, %r14
    xorq    %rax, %r11
    xorq    %rsi, %rdx
    rolq    $8, %r14
    movq    %r10, %rdi
    movq    %rdx, (%rsp)
    movq    %rdx, %rsi
    movq    %r9, %rdx
    xorq    %r13, %rbx
    xorq    %r12, %rdx
    xorq    %r15, %rdi
    rolq    $25, %rbx
    movq    %r11, -56(%rsp)
    movq    %r11, %rax
    movq    %r14, %r15
    movq    -40(%rsp), %r11
    rolq    $6, %rdx
    andq    %rbx, %r15
    notq    %r14
    rolq    $18, %rdi
    xorq    %rdx, %r15
    xorq    40(%rsp), %rax
    movq    %rbx, %r12
    movq    %r15, 48(%rsp)
    movq    %r14, %r15
    xorq    56(%rsp), %rsi
    xorq    %r8, %r11
    andq    %rdi, %r15
    movq    32(%rsp), %rbp
    rolq    %r11
    xorq    %rbx, %r15
    movq    %rdi, %rbx
    orq %r11, %rbx
    orq %rdx, %r12
    andq    %r11, %rdx
    xorq    %r14, %rbx
    xorq    %rdi, %rdx
    xorq    %r11, %r12
    xorq    %rbx, %rax
    movq    -96(%rsp), %r11
    movq    %rbx, 64(%rsp)
    movq    -24(%rsp), %r14
    xorq    %rdx, %rsi
    movq    %rdx, 72(%rsp)
    movq    -80(%rsp), %rbx
    movq    %r12, -48(%rsp)
    movq    -104(%rsp), %rdx
    xorq    %rcx, %r11
    movq    -8(%rsp), %rdi
    xorq    %r8, %r14
    rolq    $27, %r11
    xorq    %r10, %rbx
    rolq    $36, %rbx
    rolq    $10, %r14
    xorq    8(%rsp), %rdx
    movq    %r14, %r12
    xorq    %r9, %rbp
    xorq    -64(%rsp), %rcx
    xorq    %r13, %rdi
    andq    %rbx, %r12
    xorq    -72(%rsp), %r13
    xorq    %r11, %r12
    rolq    $15, %rbp
    xorq    -112(%rsp), %r9
    xorq    -48(%rsp), %rdx
    rolq    $56, %rdi
    movq    %r12, -96(%rsp)
    xorq    -16(%rsp), %r10
    rolq    $39, %rcx
    rolq    $55, %r13
    xorq    24(%rsp), %r8
    rolq    $62, %r9
    xorq    %r12, %rdx
    movq    %rbp, %r12
    notq    %rbp
    orq %r14, %r12
    movq    %rbp, -40(%rsp)
    orq %rdi, %rbp
    xorq    %rbx, %r12
    xorq    %r14, %rbp
    orq %r11, %rbx
    movq    %rdi, %r14
    xorq    %rdi, %rbx
    movq    %r13, %rdi
    andq    %r11, %r14
    notq    %rdi
    xorq    %rbx, %rsi
    xorq    -40(%rsp), %r14
    movq    %rbx, -24(%rsp)
    movq    %rdi, %rbx
    rolq    $41, %r10
    andq    %rcx, %rbx
    movq    %rbp, -80(%rsp)
    xorq    %r9, %rbx
    movq    %r10, %rbp
    rolq    $2, %r8
    xorq    %rbx, %rdx
    movq    %r8, %r11
    xorq    %r14, %rax
    orq %rcx, %rbp
    andq    %r9, %r13
    xorq    %rdi, %rbp
    movq    48(%rsp), %rdi
    xorq    %r8, %r13
    movq    %rbp, -112(%rsp)
    orq %r9, %r11
    xorq    %r13, %rsi
    xorq    %r10, %r11
    xorq    %r11, %rax
    xorq    -32(%rsp), %rdi
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    andq    %r10, %rbp
    xorq    -88(%rsp), %rdi
    movq    %rax, %r10
    xorq    %rcx, %rbp
    movq    16(%rsp), %rcx
    rolq    %r10
    movq    %rdi, %r9
    xorq    %rdi, %r10
    xorq    %rbp, %rcx
    rolq    %r9
    xorq    -120(%rsp), %rcx
    xorq    %rsi, %r9
    rolq    %rsi
    xorq    %r15, %rcx
    xorq    %r10, %r15
    xorq    -80(%rsp), %rcx
    rolq    $43, %r15
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    8(%rsp), %rcx
    rolq    %r8
    xorq    %rdx, %r8
    rolq    %rdx
    xorq    %rax, %rdx
    movq    -32(%rsp), %rax
    xorq    %r9, %rcx
    movq    %rcx, %rdi
    xorq    %r8, %rax
    rolq    $44, %rax
    xorq    %rsi, %r14
    xorq    $32898, %rdi
    movq    %rdi, -72(%rsp)
    movq    %r15, %rdi
    rolq    $21, %r14
    orq %rax, %rdi
    xorq    %rdx, %r13
    xorq    %r10, %rbp
    xorq    %rdi, -72(%rsp)
    movq    %r15, %rdi
    rolq    $14, %r13
    notq    %rdi
    rolq    $61, %rbp
    orq %r14, %rdi
    xorq    %r8, %r12
    xorq    %rax, %rdi
    andq    %rcx, %rax
    rolq    $45, %r12
    movq    %rdi, -40(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rax
    andq    %r14, %rdi
    movq    %rax, -16(%rsp)
    movq    40(%rsp), %rax
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, -32(%rsp)
    movq    -48(%rsp), %rdi
    orq %rcx, %r15
    movq    (%rsp), %rcx
    xorq    %r14, %r15
    movq    %r12, %r14
    movq    %r15, -64(%rsp)
    movq    %rbp, %r15
    xorq    %rsi, %rax
    notq    %r15
    rolq    $28, %rax
    xorq    %r9, %rdi
    orq %r12, %r15
    rolq    $3, %rdi
    xorq    %rdx, %rcx
    xorq    %rdi, %r15
    rolq    $20, %rcx
    andq    %rdi, %r14
    movq    %rdi, %r13
    movq    %rbp, %rdi
    xorq    %rcx, %r14
    orq %rax, %rdi
    orq %rcx, %r13
    movq    %r14, 32(%rsp)
    xorq    %rax, %r13
    xorq    %r12, %rdi
    andq    %rax, %rcx
    movq    64(%rsp), %rax
    xorq    %rbp, %rcx
    movq    %rdi, 24(%rsp)
    movq    -120(%rsp), %rdi
    movq    %rcx, (%rsp)
    xorq    %r9, %rbx
    movq    -88(%rsp), %rcx
    rolq    $18, %rbx
    movq    %r13, -48(%rsp)
    movq    -24(%rsp), %rbp
    movq    %rbx, %r14
    xorq    %rsi, %r11
    xorq    %rsi, %rax
    rolq    $25, %rax
    xorq    %r10, %rdi
    rolq    $6, %rdi
    xorq    %r8, %rcx
    movq    %rax, %r12
    rolq    %rcx
    xorq    %rdx, %rbp
    orq %rdi, %r12
    rolq    $8, %rbp
    xorq    %rcx, %r12
    orq %rcx, %r14
    movq    %r12, -120(%rsp)
    movq    %rbp, %r12
    movq    %rbp, %r13
    notq    %r12
    andq    %rax, %r13
    movq    %r12, %rbp
    xorq    %rdi, %r13
    andq    %rcx, %rdi
    andq    %rbx, %rbp
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    xorq    %rax, %rbp
    movq    24(%rsp), %rax
    movq    %r13, -24(%rsp)
    movq    48(%rsp), %r12
    movq    %rdi, 8(%rsp)
    movq    -80(%rsp), %r13
    movq    %r14, -8(%rsp)
    movq    -104(%rsp), %rdi
    movq    56(%rsp), %rcx
    xorq    -64(%rsp), %rax
    xorq    %r8, %r12
    rolq    $10, %r12
    xorq    %r10, %r13
    xorq    %r9, %rdi
    rolq    $15, %r13
    movq    %r12, %rbx
    rolq    $36, %rdi
    xorq    %rdx, %rcx
    rolq    $27, %rcx
    xorq    %r14, %rax
    rolq    $56, %r11
    movq    %r13, %r14
    andq    %rdi, %rbx
    xorq    -56(%rsp), %rsi
    xorq    %rcx, %rbx
    notq    %r14
    xorq    72(%rsp), %rdx
    movq    %rbx, -104(%rsp)
    movq    %r13, %rbx
    movq    %r14, %r13
    orq %r12, %rbx
    orq %r11, %r13
    xorq    -112(%rsp), %r8
    xorq    %rdi, %rbx
    xorq    %r12, %r13
    movq    -96(%rsp), %r12
    orq %rcx, %rdi
    rolq    $55, %rsi
    movq    %r13, -80(%rsp)
    xorq    %r11, %rdi
    movq    %r11, %r13
    rolq    $39, %rdx
    movq    %rdi, 40(%rsp)
    movq    16(%rsp), %rdi
    andq    %rcx, %r13
    xorq    %r9, %r12
    movq    %rsi, %r9
    xorq    %r14, %r13
    notq    %r9
    rolq    $41, %r12
    xorq    %r13, %rax
    movq    %r9, %r14
    rolq    $2, %r8
    xorq    %r10, %rdi
    andq    %rdx, %r14
    movq    %r12, %r10
    rolq    $62, %rdi
    orq %rdx, %r10
    xorq    %rdi, %r14
    xorq    %r9, %r10
    movq    %r14, -96(%rsp)
    movq    -48(%rsp), %rcx
    movq    -24(%rsp), %r9
    movq    %r10, -56(%rsp)
    movq    -32(%rsp), %r11
    xorq    -72(%rsp), %rcx
    xorq    32(%rsp), %r9
    xorq    -120(%rsp), %rcx
    xorq    %rbx, %r9
    xorq    %r10, %r9
    movq    %r8, %r10
    xorq    -104(%rsp), %rcx
    xorq    %r14, %rcx
    movq    %r8, %r14
    xorq    -40(%rsp), %r9
    orq %rdi, %r14
    andq    %r12, %r10
    xorq    %r12, %r14
    movq    (%rsp), %r12
    xorq    %rdx, %r10
    xorq    %r10, %r11
    xorq    %r14, %rax
    andq    %rdi, %rsi
    xorq    %r15, %r11
    xorq    %r8, %rsi
    movq    %rax, %r8
    xorq    %rbp, %r11
    movq    %r9, %rdi
    rolq    %r8
    xorq    -16(%rsp), %r12
    xorq    %r9, %r8
    rolq    %rdi
    xorq    -80(%rsp), %r11
    movq    %rcx, %r9
    xorq    %r8, %rbp
    rolq    %r9
    xorq    %rax, %r9
    movq    32(%rsp), %rax
    rolq    $43, %rbp
    xorq    8(%rsp), %r12
    movq    %r11, %rdx
    rolq    %rdx
    xorq    %rcx, %rdx
    movabsq $-9223372036854742902, %rcx
    xorq    40(%rsp), %r12
    xorq    %rdx, %rax
    rolq    $44, %rax
    xorq    %rsi, %r12
    xorq    %r12, %rdi
    rolq    %r12
    xorq    %r11, %r12
    movq    -72(%rsp), %r11
    xorq    %r12, %r13
    rolq    $21, %r13
    xorq    %rdi, %r11
    xorq    %r9, %rsi
    xorq    %rdx, %rbx
    xorq    %r11, %rcx
    rolq    $14, %rsi
    xorq    %r8, %r10
    movq    %rcx, -72(%rsp)
    movq    %rax, %rcx
    rolq    $45, %rbx
    orq %rbp, %rcx
    rolq    $61, %r10
    xorq    %rcx, -72(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    orq %r13, %rcx
    xorq    %rax, %rcx
    andq    %r11, %rax
    movq    %rcx, -88(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rax
    andq    %r13, %rcx
    movq    %rax, 56(%rsp)
    movq    (%rsp), %rax
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    -64(%rsp), %rsi
    movq    %rcx, 32(%rsp)
    movq    -120(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r13, %rbp
    movq    %rbx, %r13
    xorq    %r9, %rax
    movq    %rbp, 16(%rsp)
    movq    %r10, %rbp
    rolq    $20, %rax
    xorq    %r12, %rsi
    notq    %rbp
    xorq    %rdi, %rcx
    rolq    $28, %rsi
    orq %rbx, %rbp
    rolq    $3, %rcx
    movq    %rcx, %r11
    andq    %rcx, %r13
    xorq    %rcx, %rbp
    orq %rax, %r11
    xorq    %rax, %r13
    andq    %rsi, %rax
    xorq    %rsi, %r11
    xorq    %r10, %rax
    movq    -40(%rsp), %rcx
    movq    %r11, -64(%rsp)
    movq    %r10, %r11
    movq    -96(%rsp), %r10
    orq %rsi, %r11
    movq    -8(%rsp), %rsi
    movq    %rax, 48(%rsp)
    xorq    %rbx, %r11
    movq    %r8, %rax
    movq    %r13, -120(%rsp)
    movq    %r11, (%rsp)
    movq    40(%rsp), %r11
    xorq    %rdx, %rcx
    xorq    %rdi, %r10
    xorq    %r15, %rax
    rolq    %rcx
    xorq    %r12, %rsi
    rolq    $18, %r10
    movq    %rbp, -112(%rsp)
    rolq    $25, %rsi
    rolq    $6, %rax
    movq    %r10, %rbx
    xorq    %r9, %r11
    movq    %rsi, %r13
    orq %rcx, %rbx
    rolq    $8, %r11
    orq %rax, %r13
    movq    %r11, %r15
    notq    %r11
    xorq    %rcx, %r13
    xorq    %r11, %rbx
    andq    %rsi, %r15
    movq    %r13, -96(%rsp)
    xorq    %rax, %r15
    movq    %rbx, 40(%rsp)
    movq    -24(%rsp), %rbx
    movq    %r15, -8(%rsp)
    movq    %r11, %r15
    movq    -48(%rsp), %r11
    andq    %rax, %rcx
    movq    -16(%rsp), %rax
    andq    %r10, %r15
    xorq    %rsi, %r15
    movq    -80(%rsp), %r13
    xorq    %r10, %rcx
    xorq    %rdx, %rbx
    movq    %r12, %r10
    movq    %rcx, 64(%rsp)
    rolq    $10, %rbx
    xorq    %rdi, %r11
    xorq    %r14, %r10
    rolq    $36, %r11
    xorq    %r9, %rax
    movq    %rbx, %rsi
    rolq    $27, %rax
    andq    %r11, %rsi
    xorq    %r8, %r13
    xorq    %rax, %rsi
    rolq    $15, %r13
    movq    %rbx, %rbp
    movq    %rsi, -80(%rsp)
    movq    -72(%rsp), %rsi
    rolq    $56, %r10
    movq    -88(%rsp), %rcx
    xorq    -64(%rsp), %rsi
    xorq    -96(%rsp), %rsi
    orq %r13, %rbp
    notq    %r13
    movq    %r13, %r14
    xorq    -120(%rsp), %rcx
    xorq    %r11, %rbp
    orq %r10, %r14
    xorq    24(%rsp), %r12
    xorq    %rbx, %r14
    xorq    8(%rsp), %r9
    movq    %r14, -24(%rsp)
    movq    %r10, %r14
    xorq    -32(%rsp), %r8
    andq    %rax, %r14
    orq %r11, %rax
    xorq    -8(%rsp), %rcx
    xorq    %r13, %r14
    movq    -104(%rsp), %r13
    xorq    %r10, %rax
    movq    -56(%rsp), %r10
    rolq    $55, %r12
    rolq    $39, %r9
    xorq    -80(%rsp), %rsi
    rolq    $62, %r8
    xorq    %rbp, %rcx
    xorq    %rdi, %r13
    movq    -112(%rsp), %rdi
    rolq    $41, %r13
    xorq    %rdx, %r10
    movq    %r12, %rdx
    movq    %r13, %rbx
    notq    %rdx
    rolq    $2, %r10
    orq %r9, %rbx
    movq    %rdx, %r11
    xorq    %r15, %rdi
    xorq    %rdx, %rbx
    andq    %r9, %r11
    xorq    32(%rsp), %rdi
    xorq    %rbx, %rcx
    movq    %rbx, -104(%rsp)
    movq    %r10, %rbx
    xorq    %r8, %r11
    andq    %r13, %rbx
    movq    %r10, %rdx
    xorq    %r11, %rsi
    xorq    %r9, %rbx
    xorq    -24(%rsp), %rdi
    orq %r8, %rdx
    andq    %r8, %r12
    xorq    %r13, %rdx
    movq    48(%rsp), %r13
    xorq    %r10, %r12
    movq    %rdx, -32(%rsp)
    movq    (%rsp), %rdx
    movq    %rcx, %r10
    rolq    %r10
    xorq    %rbx, %rdi
    xorq    %r12, %r13
    movq    %rdi, %r8
    xorq    16(%rsp), %rdx
    rolq    %r8
    xorq    56(%rsp), %r13
    xorq    %rsi, %r8
    xorq    %r14, %rdx
    xorq    40(%rsp), %rdx
    xorq    64(%rsp), %r13
    xorq    -32(%rsp), %rdx
    xorq    %rax, %r13
    xorq    %r13, %r10
    rolq    %r13
    xorq    %rdi, %r13
    movabsq $-9223372034707259392, %rdi
    movq    %rdx, %r9
    xorq    %r13, %r14
    rolq    %r9
    rolq    $21, %r14
    xorq    %rcx, %r9
    movq    %rsi, %rcx
    movq    -72(%rsp), %rsi
    rolq    %rcx
    xorq    %r9, %r15
    xorq    %rdx, %rcx
    movq    -120(%rsp), %rdx
    rolq    $43, %r15
    xorq    %rcx, %r12
    xorq    %r10, %rsi
    rolq    $14, %r12
    xorq    %r8, %rdx
    rolq    $44, %rdx
    xorq    %rsi, %rdi
    xorq    %r9, %rbx
    movq    %rdi, -72(%rsp)
    movq    %r15, %rdi
    rolq    $61, %rbx
    orq %rdx, %rdi
    xorq    %r8, %rbp
    xorq    %rdi, -72(%rsp)
    movq    %r15, %rdi
    rolq    $45, %rbp
    notq    %rdi
    orq %r14, %rdi
    xorq    %rdx, %rdi
    andq    %rsi, %rdx
    movq    %rdi, -40(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rdx
    andq    %r14, %rdi
    movq    %rdx, -56(%rsp)
    movq    -96(%rsp), %rdx
    xorq    %r15, %rdi
    movq    %r12, %r15
    orq %rsi, %r15
    movq    48(%rsp), %rsi
    movq    %rdi, -48(%rsp)
    movq    16(%rsp), %rdi
    xorq    %r14, %r15
    movq    %rbp, %r14
    movq    %r15, -16(%rsp)
    xorq    %r10, %rdx
    movq    %rbx, %r15
    rolq    $3, %rdx
    notq    %r15
    xorq    %rcx, %rsi
    orq %rbp, %r15
    movq    %rdx, %r12
    rolq    $20, %rsi
    xorq    %r13, %rdi
    xorq    %rdx, %r15
    rolq    $28, %rdi
    orq %rsi, %r12
    andq    %rdx, %r14
    movq    %rbx, %rdx
    xorq    %rdi, %r12
    xorq    %rsi, %r14
    orq %rdi, %rdx
    andq    %rdi, %rsi
    movq    -88(%rsp), %rdi
    xorq    %rbp, %rdx
    movq    40(%rsp), %rbp
    xorq    %rbx, %rsi
    movq    -112(%rsp), %rbx
    movq    %rdx, 8(%rsp)
    movq    %rsi, 16(%rsp)
    xorq    -16(%rsp), %rdx
    xorq    -56(%rsp), %rsi
    xorq    %r8, %rdi
    xorq    %rcx, %rax
    xorq    %r13, %rbp
    rolq    $8, %rax
    movq    %r14, 24(%rsp)
    rolq    $25, %rbp
    xorq    %r9, %rbx
    xorq    %r10, %r11
    movq    %rax, %r14
    notq    %rax
    rolq    %rdi
    rolq    $18, %r11
    rolq    $6, %rbx
    movq    %r12, -96(%rsp)
    andq    %rbp, %r14
    movq    %r15, -120(%rsp)
    movq    %rbp, %r12
    movq    %rax, %r15
    xorq    %rbx, %r14
    orq %rbx, %r12
    andq    %r11, %r15
    andq    %rdi, %rbx
    movq    %r14, 40(%rsp)
    xorq    %r11, %rbx
    xorq    %rbp, %r15
    movq    -8(%rsp), %r14
    movq    %r11, %rbp
    xorq    %rbx, %rsi
    movq    %rbx, 72(%rsp)
    orq %rdi, %rbp
    movq    -64(%rsp), %rbx
    xorq    %rdi, %r12
    xorq    %rax, %rbp
    movq    56(%rsp), %r11
    movq    %r12, -112(%rsp)
    xorq    %rbp, %rdx
    movq    -32(%rsp), %rdi
    movq    %rbp, 48(%rsp)
    movq    -24(%rsp), %rbp
    xorq    %r8, %r14
    movq    -96(%rsp), %rax
    rolq    $10, %r14
    xorq    %r10, %rbx
    rolq    $36, %rbx
    xorq    %rcx, %r11
    movq    %r14, %r12
    rolq    $27, %r11
    xorq    %r13, %rdi
    andq    %rbx, %r12
    xorq    %r9, %rbp
    rolq    $56, %rdi
    rolq    $15, %rbp
    xorq    %r11, %r12
    xorq    -72(%rsp), %rax
    movq    %r12, -32(%rsp)
    xorq    -112(%rsp), %rax
    xorq    %r12, %rax
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, -88(%rsp)
    orq %rdi, %rbp
    orq %r14, %r12
    xorq    %r14, %rbp
    xorq    %rbx, %r12
    orq %r11, %rbx
    movq    %rbp, -24(%rsp)
    xorq    -80(%rsp), %r10
    xorq    %rdi, %rbx
    xorq    (%rsp), %r13
    movq    %rdi, %r14
    movq    %rbx, -64(%rsp)
    xorq    64(%rsp), %rcx
    xorq    %rbx, %rsi
    andq    %r11, %r14
    xorq    32(%rsp), %r9
    rolq    $41, %r10
    xorq    -104(%rsp), %r8
    rolq    $55, %r13
    movq    %r10, %rbp
    xorq    -88(%rsp), %r14
    rolq    $39, %rcx
    movq    %r13, %rdi
    notq    %rdi
    orq %rcx, %rbp
    rolq    $62, %r9
    xorq    %rdi, %rbp
    movq    %rdi, %rbx
    movq    40(%rsp), %rdi
    rolq    $2, %r8
    andq    %rcx, %rbx
    movq    %rbp, -104(%rsp)
    xorq    %r9, %rbx
    xorq    %r14, %rdx
    movq    %r8, %r11
    xorq    %rbx, %rax
    xorq    24(%rsp), %rdi
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    -40(%rsp), %rdi
    andq    %r10, %rbp
    andq    %r9, %r13
    xorq    %rcx, %rbp
    movq    -48(%rsp), %rcx
    xorq    %r8, %r13
    orq %r9, %r11
    xorq    %r13, %rsi
    xorq    %r10, %r11
    movq    %rdi, %r9
    xorq    %r11, %rdx
    xorq    %rbp, %rcx
    rolq    %r9
    movq    %rdx, %r10
    xorq    -120(%rsp), %rcx
    xorq    %rsi, %r9
    rolq    %rsi
    rolq    %r10
    xorq    %rdi, %r10
    xorq    %r15, %rcx
    xorq    %r10, %r15
    xorq    -24(%rsp), %rcx
    rolq    $43, %r15
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    -72(%rsp), %rcx
    rolq    %r8
    xorq    %rsi, %r14
    xorq    %rax, %r8
    rolq    %rax
    xorq    %rdx, %rax
    movq    24(%rsp), %rdx
    rolq    $21, %r14
    xorq    %r9, %rcx
    xorq    %rax, %r13
    movq    %rcx, %rdi
    rolq    $14, %r13
    xorq    $32907, %rdi
    xorq    %r8, %rdx
    movq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    rolq    $44, %rdx
    orq %rdx, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    xorq    %r10, %rbp
    notq    %rdi
    rolq    $61, %rbp
    orq %r14, %rdi
    xorq    %r8, %r12
    xorq    %rdx, %rdi
    andq    %rcx, %rdx
    rolq    $45, %r12
    movq    %rdi, -88(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rdx
    andq    %r14, %rdi
    movq    %rdx, 24(%rsp)
    movq    -16(%rsp), %rdx
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, -72(%rsp)
    movq    -112(%rsp), %rdi
    orq %rcx, %r15
    xorq    %r14, %r15
    movq    16(%rsp), %rcx
    movq    %r12, %r14
    movq    %r15, 32(%rsp)
    movq    %rbp, %r15
    xorq    %rsi, %rdx
    notq    %r15
    rolq    $28, %rdx
    xorq    %r9, %rdi
    orq %r12, %r15
    rolq    $3, %rdi
    xorq    %rax, %rcx
    xorq    %rdi, %r15
    andq    %rdi, %r14
    movq    %rdi, %r13
    movq    %rbp, %rdi
    rolq    $20, %rcx
    orq %rdx, %rdi
    xorq    %rcx, %r14
    orq %rcx, %r13
    xorq    %r12, %rdi
    andq    %rdx, %rcx
    xorq    %rdx, %r13
    xorq    %rbp, %rcx
    movq    %rdi, (%rsp)
    movq    -40(%rsp), %rdx
    movq    -120(%rsp), %rdi
    movq    %rcx, -8(%rsp)
    movq    48(%rsp), %rcx
    movq    %r13, -16(%rsp)
    movq    -64(%rsp), %rbp
    movq    %r14, -112(%rsp)
    xorq    %r8, %rdx
    xorq    %r10, %rdi
    rolq    %rdx
    rolq    $6, %rdi
    xorq    %rsi, %rcx
    xorq    %r9, %rbx
    rolq    $25, %rcx
    xorq    %rax, %rbp
    rolq    $18, %rbx
    movq    %rcx, %r12
    rolq    $8, %rbp
    movq    %rbx, %r14
    orq %rdi, %r12
    movq    %rbp, %r13
    orq %rdx, %r14
    xorq    %rdx, %r12
    andq    %rcx, %r13
    xorq    %rsi, %r11
    movq    %r12, -120(%rsp)
    movq    %rbp, %r12
    xorq    %rdi, %r13
    notq    %r12
    andq    %rdx, %rdi
    movq    %r13, -64(%rsp)
    movq    %r12, %rbp
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    andq    %rbx, %rbp
    movq    40(%rsp), %r12
    movq    %rdi, 56(%rsp)
    xorq    %rcx, %rbp
    movq    (%rsp), %rcx
    movq    %r14, 16(%rsp)
    movq    -96(%rsp), %rdi
    rolq    $56, %r11
    movq    -24(%rsp), %r13
    movq    -56(%rsp), %rdx
    xorq    %r8, %r12
    xorq    32(%rsp), %rcx
    rolq    $10, %r12
    xorq    %r9, %rdi
    movq    %r12, %rbx
    xorq    %r10, %r13
    rolq    $36, %rdi
    rolq    $15, %r13
    xorq    %rax, %rdx
    andq    %rdi, %rbx
    rolq    $27, %rdx
    xorq    %r14, %rcx
    movq    %r13, %r14
    xorq    %rdx, %rbx
    notq    %r14
    movq    %rbx, -96(%rsp)
    movq    %r13, %rbx
    movq    %r14, %r13
    orq %r12, %rbx
    orq %r11, %r13
    xorq    %rdi, %rbx
    xorq    %r12, %r13
    orq %rdx, %rdi
    movq    -32(%rsp), %r12
    xorq    %r11, %rdi
    movq    %r13, -24(%rsp)
    xorq    8(%rsp), %rsi
    movq    %rdi, -56(%rsp)
    movq    %r11, %r13
    xorq    72(%rsp), %rax
    andq    %rdx, %r13
    movq    -48(%rsp), %rdi
    xorq    %r14, %r13
    xorq    %r9, %r12
    movq    -16(%rsp), %rdx
    xorq    %r13, %rcx
    rolq    $41, %r12
    rolq    $55, %rsi
    xorq    -104(%rsp), %r8
    rolq    $39, %rax
    movq    %rsi, %r9
    movq    -72(%rsp), %r11
    xorq    %r10, %rdi
    movq    %r12, %r10
    notq    %r9
    orq %rax, %r10
    movq    %r9, %r14
    xorq    -80(%rsp), %rdx
    xorq    %r9, %r10
    movq    -64(%rsp), %r9
    rolq    $2, %r8
    movq    %r10, -32(%rsp)
    rolq    $62, %rdi
    andq    %rax, %r14
    xorq    %rdi, %r14
    xorq    -120(%rsp), %rdx
    movq    %r14, -104(%rsp)
    xorq    -112(%rsp), %r9
    xorq    -96(%rsp), %rdx
    xorq    %rbx, %r9
    xorq    %r10, %r9
    movq    %r8, %r10
    andq    %r12, %r10
    xorq    -88(%rsp), %r9
    xorq    %rax, %r10
    xorq    %r14, %rdx
    xorq    %r10, %r11
    movq    %r8, %r14
    xorq    %r15, %r11
    xorq    %rbp, %r11
    xorq    -24(%rsp), %r11
    orq %rdi, %r14
    andq    %rdi, %rsi
    xorq    %r12, %r14
    movq    -8(%rsp), %r12
    xorq    %r8, %rsi
    xorq    %r14, %rcx
    movq    %r9, %rdi
    rolq    %rdi
    movq    %rcx, %r8
    movq    %r11, %rax
    rolq    %r8
    xorq    24(%rsp), %r12
    rolq    %rax
    xorq    %r9, %r8
    xorq    %rdx, %rax
    movq    %rdx, %r9
    movq    -112(%rsp), %rdx
    rolq    %r9
    xorq    %r8, %rbp
    xorq    %rcx, %r9
    movl    $2147483649, %ecx
    rolq    $43, %rbp
    xorq    56(%rsp), %r12
    xorq    %rax, %rdx
    rolq    $44, %rdx
    xorq    -56(%rsp), %r12
    xorq    %rsi, %r12
    xorq    %r9, %rsi
    xorq    %r12, %rdi
    rolq    %r12
    xorq    %r11, %r12
    movq    -80(%rsp), %r11
    rolq    $14, %rsi
    xorq    %r12, %r13
    rolq    $21, %r13
    xorq    %rdi, %r11
    xorq    %r11, %rcx
    movq    %rcx, -80(%rsp)
    movq    %rdx, %rcx
    orq %rbp, %rcx
    xorq    %rcx, -80(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    orq %r13, %rcx
    xorq    %rdx, %rcx
    andq    %r11, %rdx
    xorq    %rax, %rbx
    movq    %rcx, -40(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rdx
    andq    %r13, %rcx
    movq    %rdx, 40(%rsp)
    movq    -8(%rsp), %rdx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    32(%rsp), %rsi
    movq    %rcx, -48(%rsp)
    movq    -120(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r8, %r10
    rolq    $45, %rbx
    xorq    %r13, %rbp
    xorq    %r9, %rdx
    rolq    $61, %r10
    movq    %rbx, %r13
    rolq    $20, %rdx
    xorq    %r12, %rsi
    movq    %rbp, 8(%rsp)
    xorq    %rdi, %rcx
    rolq    $28, %rsi
    movq    %r10, %rbp
    rolq    $3, %rcx
    notq    %rbp
    movq    %rcx, %r11
    andq    %rcx, %r13
    orq %rbx, %rbp
    orq %rdx, %r11
    xorq    %rdx, %r13
    andq    %rsi, %rdx
    xorq    %rsi, %r11
    xorq    %r10, %rdx
    xorq    %rcx, %rbp
    movq    %r11, 32(%rsp)
    movq    %r10, %r11
    movq    -88(%rsp), %r10
    orq %rsi, %r11
    movq    %rdx, 48(%rsp)
    movq    -56(%rsp), %rdx
    xorq    %rbx, %r11
    movq    -104(%rsp), %rbx
    movq    %r8, %rcx
    movq    %r11, -8(%rsp)
    movq    16(%rsp), %r11
    xorq    %r15, %rcx
    xorq    %rax, %r10
    rolq    $6, %rcx
    movq    %r13, -120(%rsp)
    rolq    %r10
    movq    %rbp, -112(%rsp)
    xorq    %r12, %r11
    rolq    $25, %r11
    xorq    %r9, %rdx
    xorq    %rdi, %rbx
    rolq    $18, %rbx
    rolq    $8, %rdx
    movq    %r11, %r13
    movq    %rbx, %rsi
    movq    %rdx, %r15
    notq    %rdx
    orq %r10, %rsi
    andq    %r11, %r15
    orq %rcx, %r13
    xorq    %rdx, %rsi
    xorq    %rcx, %r15
    xorq    %r10, %r13
    movq    %rsi, 16(%rsp)
    movq    -80(%rsp), %rsi
    andq    %rcx, %r10
    movq    %r15, -56(%rsp)
    movq    %rdx, %r15
    xorq    %rbx, %r10
    andq    %rbx, %r15
    movq    -40(%rsp), %rcx
    movq    %r13, -104(%rsp)
    movq    -64(%rsp), %rbx
    xorq    %r11, %r15
    movq    %r10, 64(%rsp)
    xorq    32(%rsp), %rsi
    movq    %r12, %r10
    movq    -16(%rsp), %r11
    xorq    %r14, %r10
    movq    24(%rsp), %rdx
    rolq    $56, %r10
    xorq    %rax, %rbx
    movq    -24(%rsp), %r13
    xorq    -120(%rsp), %rcx
    rolq    $10, %rbx
    xorq    %rdi, %r11
    xorq    -104(%rsp), %rsi
    movq    %rbx, %rbp
    rolq    $36, %r11
    xorq    %r9, %rdx
    rolq    $27, %rdx
    andq    %r11, %rbp
    xorq    %r8, %r13
    xorq    %rdx, %rbp
    xorq    -56(%rsp), %rcx
    rolq    $15, %r13
    xorq    %rbp, %rsi
    movq    %rbp, -24(%rsp)
    movq    %rbx, %rbp
    orq %r13, %rbp
    notq    %r13
    xorq    %r11, %rbp
    movq    %r13, %r14
    xorq    %rbp, %rcx
    orq %r10, %r14
    xorq    %rbx, %r14
    movq    %r14, -64(%rsp)
    movq    %r10, %r14
    xorq    (%rsp), %r12
    andq    %rdx, %r14
    orq %r11, %rdx
    xorq    56(%rsp), %r9
    xorq    %r13, %r14
    movq    -96(%rsp), %r13
    xorq    %r10, %rdx
    movq    -32(%rsp), %r10
    xorq    -72(%rsp), %r8
    rolq    $55, %r12
    rolq    $39, %r9
    xorq    %rdi, %r13
    movq    -112(%rsp), %rdi
    rolq    $41, %r13
    xorq    %rax, %r10
    movq    %r12, %rax
    movq    %r13, %rbx
    rolq    $2, %r10
    notq    %rax
    orq %r9, %rbx
    rolq    $62, %r8
    movq    %rax, %r11
    xorq    %rax, %rbx
    movq    %r10, %rax
    xorq    %r15, %rdi
    orq %r8, %rax
    xorq    -48(%rsp), %rdi
    xorq    %rbx, %rcx
    xorq    %r13, %rax
    movq    %rbx, -96(%rsp)
    movq    %r10, %rbx
    movq    %rax, -72(%rsp)
    movq    -8(%rsp), %rax
    andq    %r9, %r11
    andq    %r13, %rbx
    xorq    %r8, %r11
    movq    48(%rsp), %r13
    xorq    -64(%rsp), %rdi
    xorq    %r9, %rbx
    xorq    %r11, %rsi
    xorq    8(%rsp), %rax
    xorq    %rbx, %rdi
    xorq    %r14, %rax
    xorq    16(%rsp), %rax
    andq    %r8, %r12
    movq    %rdi, %r8
    xorq    %r10, %r12
    rolq    %r8
    xorq    %r12, %r13
    movq    %rcx, %r10
    xorq    %rsi, %r8
    xorq    40(%rsp), %r13
    rolq    %r10
    xorq    -72(%rsp), %rax
    xorq    64(%rsp), %r13
    movq    %rax, %r9
    rolq    %r9
    xorq    %rcx, %r9
    movq    %rsi, %rcx
    movq    -80(%rsp), %rsi
    rolq    %rcx
    xorq    %rdx, %r13
    xorq    %r9, %r15
    xorq    %rax, %rcx
    movq    -120(%rsp), %rax
    xorq    %r13, %r10
    rolq    %r13
    rolq    $43, %r15
    xorq    %rcx, %r12
    xorq    %rdi, %r13
    xorq    %r10, %rsi
    rolq    $14, %r12
    movabsq $-9223372034707259263, %rdi
    xorq    %r13, %r14
    xorq    %rsi, %rdi
    xorq    %r8, %rax
    rolq    $21, %r14
    rolq    $44, %rax
    movq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    orq %rax, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rax, %rdi
    movq    %rdi, -88(%rsp)
    movq    %r12, %rdi
    andq    %r14, %rdi
    xorq    %r15, %rdi
    andq    %rsi, %rax
    movq    %r12, %r15
    xorq    %r12, %rax
    orq %rsi, %r15
    xorq    %r9, %rbx
    movq    %rax, 24(%rsp)
    movq    -104(%rsp), %rax
    rolq    $61, %rbx
    movq    %rdi, -32(%rsp)
    movq    48(%rsp), %rsi
    xorq    %r14, %r15
    movq    8(%rsp), %rdi
    movq    %r15, -16(%rsp)
    xorq    %r8, %rbp
    movq    %rbx, %r15
    rolq    $45, %rbp
    xorq    %r10, %rax
    notq    %r15
    movq    %rbp, %r14
    rolq    $3, %rax
    xorq    %rcx, %rsi
    orq %rbp, %r15
    xorq    %r13, %rdi
    rolq    $20, %rsi
    xorq    %rax, %r15
    rolq    $28, %rdi
    andq    %rax, %r14
    movq    %rax, %r12
    movq    %rbx, %rax
    xorq    %rsi, %r14
    orq %rsi, %r12
    orq %rdi, %rax
    andq    %rdi, %rsi
    xorq    %rdi, %r12
    xorq    %rbp, %rax
    xorq    %rbx, %rsi
    movq    %r12, -104(%rsp)
    movq    %r14, (%rsp)
    movq    %r15, -120(%rsp)
    movq    %rax, 8(%rsp)
    xorq    -16(%rsp), %rax
    movq    %rsi, 56(%rsp)
    movq    -112(%rsp), %rbx
    movq    16(%rsp), %rbp
    movq    -40(%rsp), %rdi
    xorq    24(%rsp), %rsi
    xorq    %r9, %rbx
    xorq    %r13, %rbp
    rolq    $6, %rbx
    xorq    %r8, %rdi
    rolq    $25, %rbp
    rolq    %rdi
    xorq    %rcx, %rdx
    xorq    %r10, %r11
    rolq    $8, %rdx
    rolq    $18, %r11
    movq    %rbp, %r12
    movq    %rdx, %r14
    notq    %rdx
    orq %rbx, %r12
    movq    %rdx, %r15
    andq    %rbp, %r14
    xorq    %rdi, %r12
    andq    %r11, %r15
    xorq    %rbx, %r14
    andq    %rdi, %rbx
    xorq    %rbp, %r15
    movq    %r11, %rbp
    xorq    %r11, %rbx
    orq %rdi, %rbp
    movq    %r14, 16(%rsp)
    movq    -56(%rsp), %r14
    xorq    %rdx, %rbp
    movq    -104(%rsp), %rdx
    xorq    %rbx, %rsi
    movq    %rbx, 72(%rsp)
    movq    32(%rsp), %rbx
    xorq    %rbp, %rax
    movq    40(%rsp), %r11
    movq    %rbp, 48(%rsp)
    movq    -64(%rsp), %rbp
    movq    %r12, -112(%rsp)
    xorq    %r8, %r14
    xorq    -80(%rsp), %rdx
    rolq    $10, %r14
    xorq    %r10, %rbx
    movq    -72(%rsp), %rdi
    movq    %r14, %r12
    rolq    $36, %rbx
    xorq    %rcx, %r11
    rolq    $27, %r11
    xorq    %r9, %rbp
    andq    %rbx, %r12
    xorq    -112(%rsp), %rdx
    xorq    %r11, %r12
    rolq    $15, %rbp
    xorq    %r13, %rdi
    movq    %r12, -72(%rsp)
    rolq    $56, %rdi
    xorq    %r12, %rdx
    movq    %rbp, %r12
    notq    %rbp
    orq %r14, %r12
    movq    %rbp, -56(%rsp)
    xorq    %rbx, %r12
    orq %rdi, %rbp
    xorq    -8(%rsp), %r13
    xorq    -24(%rsp), %r10
    xorq    %r14, %rbp
    orq %r11, %rbx
    xorq    64(%rsp), %rcx
    xorq    %rdi, %rbx
    movq    %rbp, -64(%rsp)
    movq    %rdi, %r14
    xorq    %rbx, %rsi
    xorq    -96(%rsp), %r8
    rolq    $55, %r13
    andq    %r11, %r14
    xorq    -48(%rsp), %r9
    rolq    $41, %r10
    movq    %r13, %rdi
    xorq    -56(%rsp), %r14
    rolq    $39, %rcx
    movq    %r10, %rbp
    movq    %rbx, -56(%rsp)
    notq    %rdi
    orq %rcx, %rbp
    rolq    $2, %r8
    xorq    %rdi, %rbp
    movq    %rdi, %rbx
    movq    16(%rsp), %rdi
    movq    %rbp, -96(%rsp)
    andq    %rcx, %rbx
    rolq    $62, %r9
    xorq    %r9, %rbx
    movq    %r8, %r11
    xorq    %r14, %rax
    xorq    %rbx, %rdx
    xorq    (%rsp), %rdi
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    andq    %r10, %rbp
    xorq    -88(%rsp), %rdi
    xorq    %rcx, %rbp
    movq    -32(%rsp), %rcx
    xorq    %rbp, %rcx
    xorq    -120(%rsp), %rcx
    xorq    %r15, %rcx
    xorq    -64(%rsp), %rcx
    orq %r9, %r11
    andq    %r9, %r13
    xorq    %r8, %r13
    movq    %rdi, %r9
    xorq    %r13, %rsi
    rolq    %r9
    xorq    %r10, %r11
    xorq    %rsi, %r9
    rolq    %rsi
    xorq    %r11, %rax
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    -80(%rsp), %rcx
    rolq    %r8
    movq    %rax, %r10
    xorq    %rsi, %r14
    rolq    %r10
    xorq    %rdx, %r8
    rolq    %rdx
    xorq    %rax, %rdx
    movq    (%rsp), %rax
    xorq    %rdi, %r10
    xorq    %r9, %rcx
    xorq    %r10, %r15
    rolq    $21, %r14
    movabsq $-9223372036854743031, %rdi
    rolq    $43, %r15
    xorq    %rdx, %r13
    xorq    %rcx, %rdi
    rolq    $14, %r13
    xorq    %r8, %rax
    movq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    rolq    $44, %rax
    orq %rax, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rax, %rdi
    movq    %rdi, -40(%rsp)
    movq    %r13, %rdi
    andq    %r14, %rdi
    xorq    %r15, %rdi
    movq    %r13, %r15
    orq %rcx, %r15
    movq    %rdi, -24(%rsp)
    movq    -112(%rsp), %rdi
    xorq    %r14, %r15
    andq    %rcx, %rax
    movq    56(%rsp), %rcx
    xorq    %r13, %rax
    xorq    %r10, %rbp
    xorq    %r8, %r12
    movq    %rax, 32(%rsp)
    movq    -16(%rsp), %rax
    rolq    $61, %rbp
    rolq    $45, %r12
    movq    %r15, -48(%rsp)
    xorq    %r9, %rdi
    movq    %rbp, %r15
    rolq    $3, %rdi
    xorq    %rdx, %rcx
    movq    %r12, %r14
    notq    %r15
    rolq    $20, %rcx
    xorq    %rsi, %rax
    andq    %rdi, %r14
    orq %r12, %r15
    rolq    $28, %rax
    movq    %rdi, %r13
    xorq    %rdi, %r15
    xorq    %rcx, %r14
    orq %rcx, %r13
    movq    %rbp, %rdi
    andq    %rax, %rcx
    orq %rax, %rdi
    xorq    %rax, %r13
    xorq    %rbp, %rcx
    xorq    %r12, %rdi
    movq    -56(%rsp), %rbp
    movq    %rcx, -8(%rsp)
    movq    48(%rsp), %rcx
    xorq    %r9, %rbx
    movq    %rdi, (%rsp)
    movq    -120(%rsp), %rdi
    rolq    $18, %rbx
    movq    -88(%rsp), %rax
    movq    %r13, -16(%rsp)
    xorq    %rdx, %rbp
    movq    %r14, -112(%rsp)
    movq    %rbx, %r14
    xorq    %rsi, %rcx
    rolq    $8, %rbp
    rolq    $25, %rcx
    xorq    %r10, %rdi
    movq    %rbp, %r13
    xorq    %r8, %rax
    rolq    $6, %rdi
    movq    %rcx, %r12
    rolq    %rax
    orq %rdi, %r12
    andq    %rcx, %r13
    xorq    %rax, %r12
    xorq    %rdi, %r13
    orq %rax, %r14
    movq    %r12, -120(%rsp)
    movq    %rbp, %r12
    andq    %rax, %rdi
    notq    %r12
    xorq    %rbx, %rdi
    movq    24(%rsp), %rax
    movq    %r12, %rbp
    xorq    %r12, %r14
    movq    16(%rsp), %r12
    andq    %rbx, %rbp
    movq    %rdi, 56(%rsp)
    movq    -104(%rsp), %rdi
    xorq    %rcx, %rbp
    movq    (%rsp), %rcx
    movq    %r13, -56(%rsp)
    movq    -64(%rsp), %r13
    xorq    %rdx, %rax
    movq    %r14, 40(%rsp)
    xorq    %r8, %r12
    rolq    $27, %rax
    xorq    %rsi, %r11
    rolq    $10, %r12
    xorq    %r9, %rdi
    rolq    $56, %r11
    xorq    -48(%rsp), %rcx
    rolq    $36, %rdi
    movq    %r12, %rbx
    xorq    %r10, %r13
    andq    %rdi, %rbx
    rolq    $15, %r13
    xorq    %rax, %rbx
    movq    %rbx, -104(%rsp)
    movq    %r13, %rbx
    xorq    %r14, %rcx
    movq    %r13, %r14
    orq %r12, %rbx
    notq    %r14
    xorq    %rdi, %rbx
    orq %rax, %rdi
    movq    %r14, %r13
    xorq    %r11, %rdi
    orq %r11, %r13
    movq    %rdi, 24(%rsp)
    movq    -32(%rsp), %rdi
    xorq    %r12, %r13
    movq    -72(%rsp), %r12
    movq    %r13, -64(%rsp)
    movq    %r11, %r13
    andq    %rax, %r13
    xorq    %r14, %r13
    xorq    %r13, %rcx
    xorq    %r10, %rdi
    xorq    8(%rsp), %rsi
    xorq    72(%rsp), %rdx
    xorq    %r9, %r12
    rolq    $62, %rdi
    xorq    -96(%rsp), %r8
    rolq    $41, %r12
    movq    %r12, %r10
    rolq    $55, %rsi
    movq    %rsi, %r9
    rolq    $39, %rdx
    andq    %rdi, %rsi
    notq    %r9
    orq %rdx, %r10
    rolq    $2, %r8
    movq    %r9, %r14
    xorq    %r9, %r10
    xorq    %r8, %rsi
    andq    %rdx, %r14
    xorq    %rdi, %r14
    movq    %r14, -96(%rsp)
    movq    -16(%rsp), %rax
    movq    -56(%rsp), %r9
    movq    %r10, -72(%rsp)
    movq    -24(%rsp), %r11
    xorq    -80(%rsp), %rax
    xorq    -112(%rsp), %r9
    xorq    -120(%rsp), %rax
    xorq    %rbx, %r9
    xorq    %r10, %r9
    movq    %r8, %r10
    andq    %r12, %r10
    xorq    -40(%rsp), %r9
    xorq    -104(%rsp), %rax
    xorq    %rdx, %r10
    xorq    %r10, %r11
    xorq    %r15, %r11
    xorq    %rbp, %r11
    xorq    %r14, %rax
    movq    %r8, %r14
    xorq    -64(%rsp), %r11
    orq %rdi, %r14
    movq    %r9, %rdi
    xorq    %r12, %r14
    movq    -8(%rsp), %r12
    xorq    %r14, %rcx
    movq    %r11, %rdx
    movq    %rcx, %r8
    xorq    32(%rsp), %r12
    rolq    %rdi
    rolq    %rdx
    rolq    %r8
    xorq    %rax, %rdx
    xorq    %r9, %r8
    movq    %rax, %r9
    movq    -112(%rsp), %rax
    rolq    %r9
    xorq    %r8, %rbp
    xorq    56(%rsp), %r12
    xorq    %rcx, %r9
    rolq    $43, %rbp
    xorq    %rdx, %rax
    rolq    $44, %rax
    xorq    24(%rsp), %r12
    xorq    %rsi, %r12
    xorq    %r9, %rsi
    xorq    %r12, %rdi
    rolq    %r12
    xorq    %r11, %r12
    movq    -80(%rsp), %r11
    rolq    $14, %rsi
    xorq    %r12, %r13
    rolq    $21, %r13
    xorq    %rdi, %r11
    movq    %r11, %rcx
    xorb    $-118, %cl
    movq    %rcx, -80(%rsp)
    movq    %rax, %rcx
    orq %rbp, %rcx
    xorq    %rcx, -80(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    orq %r13, %rcx
    xorq    %rax, %rcx
    andq    %r11, %rax
    movq    %rcx, -88(%rsp)
    movq    %rsi, %rcx
    andq    %r13, %rcx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    orq %r11, %rbp
    movq    %rcx, -32(%rsp)
    movq    -120(%rsp), %rcx
    xorq    %r13, %rbp
    xorq    %rsi, %rax
    movq    -48(%rsp), %rsi
    movq    %rax, 16(%rsp)
    movq    -8(%rsp), %rax
    xorq    %rdx, %rbx
    xorq    %r8, %r10
    rolq    $45, %rbx
    movq    %rbp, 8(%rsp)
    xorq    %rdi, %rcx
    rolq    $61, %r10
    movq    %rbx, %r13
    rolq    $3, %rcx
    xorq    %r12, %rsi
    movq    %r10, %rbp
    xorq    %r9, %rax
    movq    %rcx, %r11
    rolq    $28, %rsi
    rolq    $20, %rax
    andq    %rcx, %r13
    notq    %rbp
    orq %rax, %r11
    xorq    %rax, %r13
    andq    %rsi, %rax
    xorq    %rsi, %r11
    xorq    %r10, %rax
    orq %rbx, %rbp
    movq    %r11, -48(%rsp)
    movq    %r10, %r11
    movq    -40(%rsp), %r10
    orq %rsi, %r11
    movq    %rax, 48(%rsp)
    movq    24(%rsp), %rax
    xorq    %rbx, %r11
    movq    -96(%rsp), %rbx
    xorq    %rcx, %rbp
    movq    %r11, -8(%rsp)
    movq    40(%rsp), %r11
    movq    %r8, %rcx
    xorq    %r15, %rcx
    xorq    %rdx, %r10
    movq    %r13, -120(%rsp)
    xorq    %r9, %rax
    rolq    $6, %rcx
    movq    %rbp, -112(%rsp)
    rolq    $8, %rax
    xorq    %rdi, %rbx
    rolq    %r10
    xorq    %r12, %r11
    rolq    $18, %rbx
    movq    %rax, %r15
    rolq    $25, %r11
    movq    %rbx, %rsi
    notq    %rax
    movq    %r11, %r13
    orq %rcx, %r13
    xorq    %r10, %r13
    andq    %r11, %r15
    orq %r10, %rsi
    xorq    %rax, %rsi
    movq    %r13, -96(%rsp)
    xorq    %rcx, %r15
    movq    %rsi, 40(%rsp)
    movq    -80(%rsp), %rsi
    andq    %rcx, %r10
    movq    %r15, 24(%rsp)
    movq    %rax, %r15
    xorq    %rbx, %r10
    andq    %rbx, %r15
    movq    -56(%rsp), %rbx
    movq    %r10, 64(%rsp)
    xorq    %r11, %r15
    movq    -16(%rsp), %r11
    movq    %r12, %r10
    xorq    -48(%rsp), %rsi
    xorq    %r14, %r10
    movq    32(%rsp), %rax
    rolq    $56, %r10
    xorq    %rdx, %rbx
    movq    -64(%rsp), %r13
    rolq    $10, %rbx
    xorq    %rdi, %r11
    movq    -88(%rsp), %rcx
    xorq    -96(%rsp), %rsi
    rolq    $36, %r11
    movq    %rbx, %rbp
    xorq    %r9, %rax
    andq    %r11, %rbp
    rolq    $27, %rax
    xorq    %r8, %r13
    xorq    %rax, %rbp
    rolq    $15, %r13
    xorq    -120(%rsp), %rcx
    xorq    %rbp, %rsi
    movq    %rbp, -64(%rsp)
    movq    %rbx, %rbp
    orq %r13, %rbp
    notq    %r13
    movq    %r13, %r14
    xorq    %r11, %rbp
    orq %r10, %r14
    xorq    24(%rsp), %rcx
    xorq    %rbx, %r14
    movq    %r14, -16(%rsp)
    movq    %r10, %r14
    andq    %rax, %r14
    orq %r11, %rax
    xorq    %r13, %r14
    movq    -104(%rsp), %r13
    xorq    %rbp, %rcx
    xorq    %r10, %rax
    xorq    (%rsp), %r12
    xorq    56(%rsp), %r9
    movq    -72(%rsp), %r10
    xorq    -24(%rsp), %r8
    xorq    %rdi, %r13
    rolq    $41, %r13
    movq    -112(%rsp), %rdi
    rolq    $55, %r12
    rolq    $39, %r9
    movq    %r13, %rbx
    xorq    %rdx, %r10
    movq    %r12, %rdx
    orq %r9, %rbx
    rolq    $2, %r10
    notq    %rdx
    rolq    $62, %r8
    xorq    %rdx, %rbx
    movq    %rdx, %r11
    movq    %r10, %rdx
    orq %r8, %rdx
    xorq    %rbx, %rcx
    xorq    %r15, %rdi
    xorq    %r13, %rdx
    movq    %rbx, -104(%rsp)
    movq    %r10, %rbx
    movq    %rdx, -72(%rsp)
    movq    -8(%rsp), %rdx
    andq    %r13, %rbx
    xorq    -32(%rsp), %rdi
    andq    %r8, %r12
    xorq    %r9, %rbx
    movq    48(%rsp), %r13
    xorq    %r10, %r12
    andq    %r9, %r11
    xorq    %r8, %r11
    movq    %rcx, %r10
    xorq    8(%rsp), %rdx
    xorq    %r11, %rsi
    xorq    -16(%rsp), %rdi
    xorq    %r12, %r13
    xorq    16(%rsp), %r13
    xorq    %r14, %rdx
    xorq    40(%rsp), %rdx
    xorq    %rbx, %rdi
    movq    %rdi, %r8
    xorq    -72(%rsp), %rdx
    xorq    64(%rsp), %r13
    rolq    %r8
    xorq    %rsi, %r8
    rolq    %r10
    movq    %rdx, %r9
    rolq    %r9
    xorq    %rax, %r13
    xorq    %rcx, %r9
    movq    %rsi, %rcx
    movq    -80(%rsp), %rsi
    rolq    %rcx
    xorq    %r13, %r10
    rolq    %r13
    xorq    %rdx, %rcx
    movq    -120(%rsp), %rdx
    xorq    %rdi, %r13
    xorq    %r9, %r15
    xorq    %r13, %r14
    xorq    %rcx, %r12
    rolq    $43, %r15
    xorq    %r10, %rsi
    rolq    $21, %r14
    movq    %rsi, %rdi
    rolq    $14, %r12
    xorq    %r8, %rdx
    xorb    $-120, %dil
    movq    %rdi, -80(%rsp)
    rolq    $44, %rdx
    movq    %r15, %rdi
    orq %rdx, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rdx, %rdi
    andq    %rsi, %rdx
    movq    %rdi, -40(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rdx
    andq    %r14, %rdi
    movq    %rdx, 32(%rsp)
    movq    -96(%rsp), %rdx
    xorq    %r15, %rdi
    movq    %r12, %r15
    movq    %rdi, -24(%rsp)
    movq    8(%rsp), %rdi
    orq %rsi, %r15
    xorq    %r14, %r15
    movq    48(%rsp), %rsi
    movq    %r15, -56(%rsp)
    xorq    %r13, %rdi
    rolq    $28, %rdi
    xorq    %r9, %rbx
    xorq    %r8, %rbp
    rolq    $61, %rbx
    rolq    $45, %rbp
    xorq    %r10, %rdx
    movq    %rbx, %r15
    rolq    $3, %rdx
    movq    %rbp, %r14
    notq    %r15
    xorq    %rcx, %rsi
    andq    %rdx, %r14
    orq %rbp, %r15
    movq    %rdx, %r12
    rolq    $20, %rsi
    xorq    %rdx, %r15
    movq    %rbx, %rdx
    xorq    %rsi, %r14
    orq %rdi, %rdx
    orq %rsi, %r12
    andq    %rdi, %rsi
    xorq    %rbp, %rdx
    movq    40(%rsp), %rbp
    xorq    %rbx, %rsi
    movq    -112(%rsp), %rbx
    xorq    %rdi, %r12
    xorq    %rcx, %rax
    movq    -88(%rsp), %rdi
    rolq    $8, %rax
    movq    %r12, -96(%rsp)
    xorq    %r10, %r11
    movq    %r14, (%rsp)
    movq    %rax, %r14
    xorq    %r13, %rbp
    notq    %rax
    rolq    $18, %r11
    rolq    $25, %rbp
    xorq    %r9, %rbx
    movq    %r15, -120(%rsp)
    rolq    $6, %rbx
    xorq    %r8, %rdi
    movq    %rbp, %r12
    rolq    %rdi
    orq %rbx, %r12
    movq    %rax, %r15
    xorq    %rdi, %r12
    movq    %rdx, 8(%rsp)
    xorq    -56(%rsp), %rdx
    movq    %rsi, 56(%rsp)
    xorq    32(%rsp), %rsi
    andq    %rbp, %r14
    andq    %r11, %r15
    xorq    %rbx, %r14
    movq    %r12, -112(%rsp)
    xorq    %rbp, %r15
    movq    %r11, %rbp
    andq    %rdi, %rbx
    orq %rdi, %rbp
    xorq    %r11, %rbx
    movq    %r14, 40(%rsp)
    xorq    %rax, %rbp
    movq    -96(%rsp), %rax
    xorq    %rbx, %rsi
    movq    24(%rsp), %r14
    movq    %rbx, 72(%rsp)
    xorq    %rbp, %rdx
    movq    -48(%rsp), %rbx
    movq    %rbp, 48(%rsp)
    movq    16(%rsp), %r11
    xorq    -80(%rsp), %rax
    movq    -16(%rsp), %rbp
    xorq    %r8, %r14
    movq    -72(%rsp), %rdi
    rolq    $10, %r14
    xorq    %r10, %rbx
    rolq    $36, %rbx
    xorq    %rcx, %r11
    movq    %r14, %r12
    xorq    -112(%rsp), %rax
    rolq    $27, %r11
    andq    %rbx, %r12
    xorq    %r9, %rbp
    xorq    %r11, %r12
    rolq    $15, %rbp
    xorq    %r13, %rdi
    movq    %r12, -72(%rsp)
    rolq    $56, %rdi
    xorq    %r12, %rax
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, -88(%rsp)
    orq %rdi, %rbp
    orq %r14, %r12
    xorq    %r14, %rbp
    movq    %rdi, %r14
    xorq    %rbx, %r12
    andq    %r11, %r14
    movq    %rbp, -48(%rsp)
    orq %r11, %rbx
    xorq    -88(%rsp), %r14
    xorq    %r14, %rdx
    xorq    %rdi, %rbx
    xorq    -8(%rsp), %r13
    xorq    -64(%rsp), %r10
    movq    %rbx, -16(%rsp)
    xorq    %rbx, %rsi
    xorq    64(%rsp), %rcx
    xorq    -104(%rsp), %r8
    rolq    $55, %r13
    xorq    -32(%rsp), %r9
    rolq    $41, %r10
    movq    %r13, %rdi
    rolq    $39, %rcx
    movq    %r10, %rbp
    notq    %rdi
    orq %rcx, %rbp
    movq    %rdi, %rbx
    rolq    $2, %r8
    xorq    %rdi, %rbp
    movq    40(%rsp), %rdi
    andq    %rcx, %rbx
    movq    %rbp, -104(%rsp)
    rolq    $62, %r9
    movq    %r8, %r11
    orq %r9, %r11
    andq    %r9, %r13
    xorq    %r9, %rbx
    xorq    %r10, %r11
    xorq    %r8, %r13
    xorq    %rbx, %rax
    xorq    (%rsp), %rdi
    xorq    %r11, %rdx
    xorq    %r13, %rsi
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    andq    %r10, %rbp
    xorq    -40(%rsp), %rdi
    movq    %rdx, %r10
    xorq    %rcx, %rbp
    movq    -24(%rsp), %rcx
    movq    %rdi, %r9
    xorq    %rbp, %rcx
    xorq    -120(%rsp), %rcx
    xorq    %r15, %rcx
    xorq    -48(%rsp), %rcx
    rolq    %r9
    rolq    %r10
    xorq    %rsi, %r9
    rolq    %rsi
    xorq    %rdi, %r10
    movl    $2147516425, %edi
    xorq    %r10, %r15
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    -80(%rsp), %rcx
    rolq    %r8
    rolq    $43, %r15
    xorq    %rsi, %r14
    xorq    %rax, %r8
    rolq    %rax
    xorq    %rdx, %rax
    movq    (%rsp), %rdx
    rolq    $21, %r14
    xorq    %r9, %rcx
    xorq    %rax, %r13
    xorq    %rcx, %rdi
    rolq    $14, %r13
    movq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    xorq    %r8, %rdx
    rolq    $44, %rdx
    orq %rdx, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rdx, %rdi
    andq    %rcx, %rdx
    movq    %rdi, -88(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rdx
    andq    %r14, %rdi
    movq    %rdx, 24(%rsp)
    movq    -56(%rsp), %rdx
    xorq    %r15, %rdi
    movq    %r13, %r15
    orq %rcx, %r15
    movq    56(%rsp), %rcx
    movq    %rdi, -32(%rsp)
    movq    -112(%rsp), %rdi
    xorq    %r14, %r15
    xorq    %rsi, %rdx
    movq    %r15, -64(%rsp)
    rolq    $28, %rdx
    xorq    %rax, %rcx
    rolq    $20, %rcx
    xorq    %r10, %rbp
    xorq    %r8, %r12
    rolq    $61, %rbp
    rolq    $45, %r12
    xorq    %r9, %rdi
    movq    %rbp, %r15
    rolq    $3, %rdi
    movq    %r12, %r14
    notq    %r15
    andq    %rdi, %r14
    movq    %rdi, %r13
    orq %r12, %r15
    xorq    %rcx, %r14
    orq %rcx, %r13
    xorq    %rdi, %r15
    andq    %rdx, %rcx
    movq    %rbp, %rdi
    xorq    %rbp, %rcx
    orq %rdx, %rdi
    xorq    %rdx, %r13
    xorq    %r12, %rdi
    movq    %rcx, -8(%rsp)
    movq    48(%rsp), %rcx
    movq    %rdi, (%rsp)
    movq    -120(%rsp), %rdi
    xorq    %r9, %rbx
    movq    -40(%rsp), %rdx
    movq    %r13, -56(%rsp)
    rolq    $18, %rbx
    movq    -16(%rsp), %rbp
    movq    %r14, -112(%rsp)
    movq    %rbx, %r14
    xorq    %rsi, %rcx
    rolq    $25, %rcx
    xorq    %r10, %rdi
    rolq    $6, %rdi
    xorq    %r8, %rdx
    movq    %rcx, %r12
    rolq    %rdx
    xorq    %rax, %rbp
    orq %rdi, %r12
    rolq    $8, %rbp
    xorq    %rdx, %r12
    movq    %r12, -120(%rsp)
    movq    %rbp, %r12
    movq    %rbp, %r13
    notq    %r12
    andq    %rcx, %r13
    movq    %r12, %rbp
    xorq    %rdi, %r13
    andq    %rbx, %rbp
    movq    %r13, -16(%rsp)
    movq    -48(%rsp), %r13
    xorq    %rcx, %rbp
    orq %rdx, %r14
    andq    %rdx, %rdi
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    movq    (%rsp), %rcx
    movq    40(%rsp), %r12
    movq    %rdi, 56(%rsp)
    xorq    %rsi, %r11
    movq    -96(%rsp), %rdi
    xorq    %r10, %r13
    movq    %r14, 16(%rsp)
    movq    32(%rsp), %rdx
    rolq    $15, %r13
    rolq    $56, %r11
    xorq    -64(%rsp), %rcx
    xorq    %r8, %r12
    xorq    8(%rsp), %rsi
    rolq    $10, %r12
    xorq    %r9, %rdi
    rolq    $36, %rdi
    xorq    %rax, %rdx
    movq    %r12, %rbx
    rolq    $27, %rdx
    andq    %rdi, %rbx
    xorq    %r14, %rcx
    xorq    %rdx, %rbx
    movq    %r13, %r14
    xorq    72(%rsp), %rax
    movq    %rbx, -96(%rsp)
    notq    %r14
    movq    %r13, %rbx
    orq %r12, %rbx
    movq    %r14, %r13
    rolq    $55, %rsi
    xorq    %rdi, %rbx
    orq %r11, %r13
    orq %rdx, %rdi
    xorq    %r12, %r13
    xorq    %r11, %rdi
    movq    -72(%rsp), %r12
    movq    %r13, -48(%rsp)
    movq    %r11, %r13
    rolq    $39, %rax
    movq    %rdi, 32(%rsp)
    movq    -24(%rsp), %rdi
    andq    %rdx, %r13
    movq    -56(%rsp), %rdx
    xorq    %r14, %r13
    xorq    %r13, %rcx
    movq    -32(%rsp), %r11
    xorq    %r10, %rdi
    rolq    $62, %rdi
    xorq    %r9, %r12
    xorq    -80(%rsp), %rdx
    rolq    $41, %r12
    movq    %rsi, %r9
    xorq    -104(%rsp), %r8
    movq    %r12, %r10
    notq    %r9
    orq %rax, %r10
    movq    %r9, %r14
    andq    %rdi, %rsi
    xorq    %r9, %r10
    xorq    -120(%rsp), %rdx
    andq    %rax, %r14
    movq    -16(%rsp), %r9
    rolq    $2, %r8
    xorq    %rdi, %r14
    movq    %r14, -104(%rsp)
    xorq    %r8, %rsi
    movq    %r10, -72(%rsp)
    xorq    -96(%rsp), %rdx
    xorq    -112(%rsp), %r9
    xorq    %r14, %rdx
    movq    %r8, %r14
    xorq    %rbx, %r9
    orq %rdi, %r14
    xorq    %r10, %r9
    movq    %r8, %r10
    xorq    %r12, %r14
    andq    %r12, %r10
    movq    -8(%rsp), %r12
    xorq    %r14, %rcx
    xorq    %rax, %r10
    xorq    -88(%rsp), %r9
    movq    %rcx, %r8
    xorq    %r10, %r11
    xorq    %r15, %r11
    xorq    24(%rsp), %r12
    xorq    %rbp, %r11
    xorq    -48(%rsp), %r11
    movq    %r9, %rdi
    rolq    %rdi
    xorq    56(%rsp), %r12
    movq    %r11, %rax
    xorq    32(%rsp), %r12
    xorq    %rsi, %r12
    xorq    %r12, %rdi
    rolq    %rax
    rolq    %r8
    rolq    %r12
    xorq    %rdx, %rax
    xorq    %r9, %r8
    xorq    %r11, %r12
    movq    %rdx, %r9
    movq    -80(%rsp), %r11
    rolq    %r9
    xorq    %r8, %rbp
    movq    -112(%rsp), %rdx
    xorq    %rcx, %r9
    movl    $2147483658, %ecx
    rolq    $43, %rbp
    xorq    %r12, %r13
    xorq    %r9, %rsi
    rolq    $21, %r13
    rolq    $14, %rsi
    xorq    %rdi, %r11
    xorq    %rax, %rdx
    xorq    %r11, %rcx
    rolq    $44, %rdx
    movq    %rcx, -80(%rsp)
    movq    %rdx, %rcx
    orq %rbp, %rcx
    xorq    %rcx, -80(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    orq %r13, %rcx
    xorq    %rdx, %rcx
    andq    %r11, %rdx
    movq    %rcx, -40(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rdx
    andq    %r13, %rcx
    movq    %rdx, 40(%rsp)
    movq    -8(%rsp), %rdx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    -64(%rsp), %rsi
    movq    %rcx, -24(%rsp)
    movq    -120(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r13, %rbp
    xorq    %r9, %rdx
    movq    %rbp, 8(%rsp)
    xorq    %r12, %rsi
    rolq    $20, %rdx
    xorq    %rdi, %rcx
    rolq    $28, %rsi
    rolq    $3, %rcx
    xorq    %rax, %rbx
    xorq    %r8, %r10
    rolq    $45, %rbx
    movq    %rcx, %r11
    rolq    $61, %r10
    orq %rdx, %r11
    movq    %rbx, %r13
    movq    %r10, %rbp
    xorq    %rsi, %r11
    andq    %rcx, %r13
    notq    %rbp
    xorq    %rdx, %r13
    movq    %r11, -64(%rsp)
    andq    %rsi, %rdx
    movq    %r10, %r11
    xorq    %r10, %rdx
    orq %rbx, %rbp
    orq %rsi, %r11
    movq    %rdx, 48(%rsp)
    movq    32(%rsp), %rdx
    xorq    %rbx, %r11
    movq    -104(%rsp), %rbx
    xorq    %rcx, %rbp
    movq    %r11, -8(%rsp)
    movq    -88(%rsp), %r10
    movq    %r8, %rcx
    movq    16(%rsp), %r11
    xorq    %r15, %rcx
    movq    %r13, -120(%rsp)
    xorq    %r9, %rdx
    rolq    $6, %rcx
    movq    %rbp, -112(%rsp)
    xorq    %rdi, %rbx
    rolq    $8, %rdx
    rolq    $18, %rbx
    xorq    %rax, %r10
    movq    %rdx, %r15
    xorq    %r12, %r11
    rolq    %r10
    movq    %rbx, %rsi
    rolq    $25, %r11
    notq    %rdx
    orq %r10, %rsi
    andq    %r11, %r15
    xorq    %rdx, %rsi
    movq    %r11, %r13
    xorq    %rcx, %r15
    movq    %rsi, 16(%rsp)
    movq    -80(%rsp), %rsi
    movq    %r15, 32(%rsp)
    movq    %rdx, %r15
    orq %rcx, %r13
    andq    %rbx, %r15
    xorq    %r10, %r13
    movq    24(%rsp), %rdx
    xorq    %r11, %r15
    andq    %rcx, %r10
    movq    -56(%rsp), %r11
    xorq    %rbx, %r10
    movq    -16(%rsp), %rbx
    movq    %r13, -104(%rsp)
    xorq    -64(%rsp), %rsi
    movq    %r10, 64(%rsp)
    movq    %r12, %r10
    movq    -48(%rsp), %r13
    xorq    %r9, %rdx
    xorq    %r14, %r10
    xorq    %rdi, %r11
    movq    -40(%rsp), %rcx
    rolq    $27, %rdx
    rolq    $36, %r11
    xorq    %rax, %rbx
    rolq    $56, %r10
    rolq    $10, %rbx
    xorq    -104(%rsp), %rsi
    movq    %rbx, %rbp
    xorq    %r8, %r13
    andq    %r11, %rbp
    rolq    $15, %r13
    xorq    -120(%rsp), %rcx
    xorq    %rdx, %rbp
    xorq    %rbp, %rsi
    movq    %rbp, -48(%rsp)
    movq    %rbx, %rbp
    orq %r13, %rbp
    notq    %r13
    movq    %r13, %r14
    xorq    32(%rsp), %rcx
    xorq    %r11, %rbp
    orq %r10, %r14
    xorq    %rbx, %r14
    movq    %r14, -16(%rsp)
    movq    %r10, %r14
    xorq    -32(%rsp), %r8
    andq    %rdx, %r14
    xorq    (%rsp), %r12
    orq %r11, %rdx
    xorq    %r13, %r14
    xorq    56(%rsp), %r9
    xorq    %r10, %rdx
    movq    -96(%rsp), %r13
    xorq    %rbp, %rcx
    movq    -72(%rsp), %r10
    rolq    $62, %r8
    rolq    $55, %r12
    rolq    $39, %r9
    xorq    %rdi, %r13
    movq    -112(%rsp), %rdi
    rolq    $41, %r13
    xorq    %rax, %r10
    movq    %r12, %rax
    movq    %r13, %rbx
    rolq    $2, %r10
    notq    %rax
    orq %r9, %rbx
    movq    %rax, %r11
    andq    %r8, %r12
    xorq    %rax, %rbx
    movq    %r10, %rax
    xorq    %r15, %rdi
    orq %r8, %rax
    xorq    %rbx, %rcx
    movq    %rbx, -96(%rsp)
    xorq    %r13, %rax
    movq    %r10, %rbx
    xorq    %r10, %r12
    movq    %rax, -72(%rsp)
    movq    -8(%rsp), %rax
    andq    %r13, %rbx
    movq    48(%rsp), %r13
    xorq    %r9, %rbx
    andq    %r9, %r11
    xorq    -24(%rsp), %rdi
    xorq    %r8, %r11
    movq    %rcx, %r10
    xorq    %r11, %rsi
    rolq    %r10
    xorq    8(%rsp), %rax
    xorq    %r12, %r13
    xorq    40(%rsp), %r13
    xorq    -16(%rsp), %rdi
    xorq    %r14, %rax
    xorq    16(%rsp), %rax
    xorq    64(%rsp), %r13
    xorq    %rbx, %rdi
    movq    %rdi, %r8
    xorq    -72(%rsp), %rax
    rolq    %r8
    xorq    %rdx, %r13
    xorq    %rsi, %r8
    xorq    %r13, %r10
    movq    %rax, %r9
    rolq    %r9
    rolq    %r13
    xorq    %r8, %rbp
    xorq    %rcx, %r9
    movq    %rsi, %rcx
    movq    -80(%rsp), %rsi
    rolq    %rcx
    xorq    %rdi, %r13
    xorq    %r9, %r15
    xorq    %rax, %rcx
    movq    -120(%rsp), %rax
    movl    $2147516555, %edi
    rolq    $43, %r15
    xorq    %r13, %r14
    xorq    %rcx, %r12
    rolq    $21, %r14
    xorq    %r10, %rsi
    rolq    $14, %r12
    xorq    %rsi, %rdi
    xorq    %r8, %rax
    movq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    rolq    $44, %rax
    orq %rax, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rax, %rdi
    andq    %rsi, %rax
    movq    %rdi, -88(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rax
    andq    %r14, %rdi
    movq    %rax, 24(%rsp)
    movq    -104(%rsp), %rax
    xorq    %r15, %rdi
    movq    %r12, %r15
    orq %rsi, %r15
    movq    %rdi, -32(%rsp)
    movq    48(%rsp), %rsi
    movq    8(%rsp), %rdi
    xorq    %r14, %r15
    xorq    %r10, %rax
    movq    %r15, -56(%rsp)
    rolq    $3, %rax
    xorq    %rcx, %rsi
    movq    %rax, %r12
    xorq    %r13, %rdi
    rolq    $20, %rsi
    rolq    $28, %rdi
    rolq    $45, %rbp
    xorq    %r9, %rbx
    rolq    $61, %rbx
    movq    %rbp, %r14
    orq %rsi, %r12
    movq    %rbx, %r15
    andq    %rax, %r14
    xorq    %rdi, %r12
    notq    %r15
    xorq    %rsi, %r14
    andq    %rdi, %rsi
    orq %rbp, %r15
    xorq    %rbx, %rsi
    movq    %r12, -104(%rsp)
    xorq    %rax, %r15
    movq    %rbx, %rax
    movq    %r14, (%rsp)
    orq %rdi, %rax
    movq    %r15, -120(%rsp)
    xorq    %rcx, %rdx
    xorq    %rbp, %rax
    rolq    $8, %rdx
    xorq    %r10, %r11
    movq    %rax, 8(%rsp)
    xorq    -56(%rsp), %rax
    movq    %rdx, %r14
    movq    %rsi, 56(%rsp)
    movq    16(%rsp), %rbp
    notq    %rdx
    movq    -40(%rsp), %rdi
    rolq    $18, %r11
    movq    %rdx, %r15
    movq    -112(%rsp), %rbx
    andq    %r11, %r15
    xorq    24(%rsp), %rsi
    xorq    %r13, %rbp
    rolq    $25, %rbp
    xorq    %r8, %rdi
    rolq    %rdi
    xorq    %rbp, %r15
    andq    %rbp, %r14
    movq    %rbp, %r12
    movq    %r11, %rbp
    xorq    %r9, %rbx
    orq %rdi, %rbp
    rolq    $6, %rbx
    xorq    %rdx, %rbp
    movq    -104(%rsp), %rdx
    orq %rbx, %r12
    xorq    %rdi, %r12
    xorq    %rbx, %r14
    xorq    %rbp, %rax
    andq    %rdi, %rbx
    movq    %r14, 16(%rsp)
    movq    32(%rsp), %r14
    xorq    %r11, %rbx
    movq    40(%rsp), %r11
    movq    %rbp, 48(%rsp)
    xorq    %rbx, %rsi
    xorq    -80(%rsp), %rdx
    movq    %rbx, 72(%rsp)
    movq    -64(%rsp), %rbx
    movq    %r12, -112(%rsp)
    movq    -16(%rsp), %rbp
    xorq    %r8, %r14
    movq    -72(%rsp), %rdi
    rolq    $10, %r14
    xorq    %rcx, %r11
    xorq    -112(%rsp), %rdx
    movq    %r14, %r12
    rolq    $27, %r11
    xorq    %r10, %rbx
    rolq    $36, %rbx
    xorq    %r9, %rbp
    xorq    -24(%rsp), %r9
    andq    %rbx, %r12
    rolq    $15, %rbp
    xorq    %r11, %r12
    xorq    %r13, %rdi
    xorq    -8(%rsp), %r13
    rolq    $56, %rdi
    xorq    %r12, %rdx
    movq    %r12, -72(%rsp)
    movq    %rbp, %r12
    notq    %rbp
    rolq    $62, %r9
    movq    %rbp, -40(%rsp)
    orq %rdi, %rbp
    orq %r14, %r12
    xorq    %r14, %rbp
    movq    %rdi, %r14
    xorq    %rbx, %r12
    andq    %r11, %r14
    orq %r11, %rbx
    rolq    $55, %r13
    xorq    -40(%rsp), %r14
    xorq    %rdi, %rbx
    movq    %rbp, -64(%rsp)
    xorq    %rbx, %rsi
    movq    %r13, %rdi
    movq    %rbx, -16(%rsp)
    notq    %rdi
    movq    %rdi, %rbx
    xorq    %r14, %rax
    xorq    64(%rsp), %rcx
    andq    %r9, %r13
    xorq    -48(%rsp), %r10
    xorq    -96(%rsp), %r8
    rolq    $39, %rcx
    rolq    $41, %r10
    andq    %rcx, %rbx
    movq    %r10, %rbp
    rolq    $2, %r8
    xorq    %r9, %rbx
    orq %rcx, %rbp
    movq    %r8, %r11
    xorq    %r8, %r13
    xorq    %rdi, %rbp
    movq    16(%rsp), %rdi
    orq %r9, %r11
    movq    %rbp, -96(%rsp)
    xorq    %r10, %r11
    xorq    %rbx, %rdx
    xorq    %r11, %rax
    xorq    %r13, %rsi
    xorq    (%rsp), %rdi
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    andq    %r10, %rbp
    xorq    -88(%rsp), %rdi
    movq    %rax, %r10
    xorq    %rcx, %rbp
    movq    -32(%rsp), %rcx
    rolq    %r10
    movq    %rdi, %r9
    xorq    %rdi, %r10
    movabsq $-9223372036854775669, %rdi
    xorq    %rbp, %rcx
    rolq    %r9
    xorq    -120(%rsp), %rcx
    xorq    %rsi, %r9
    xorq    %r15, %rcx
    xorq    -64(%rsp), %rcx
    movq    %rcx, %r8
    rolq    %r8
    xorq    %rdx, %r8
    rolq    %rsi
    xorq    %r10, %r15
    xorq    %rcx, %rsi
    rolq    %rdx
    movq    -80(%rsp), %rcx
    xorq    %rax, %rdx
    movq    (%rsp), %rax
    rolq    $43, %r15
    xorq    %rsi, %r14
    xorq    %rdx, %r13
    xorq    %r8, %r12
    rolq    $21, %r14
    rolq    $14, %r13
    xorq    %r10, %rbp
    xorq    %r9, %rcx
    rolq    $45, %r12
    xorq    %rcx, %rdi
    xorq    %r8, %rax
    movq    %rdi, -80(%rsp)
    rolq    $44, %rax
    movq    %r15, %rdi
    orq %rax, %rdi
    xorq    %rdi, -80(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rax, %rdi
    andq    %rcx, %rax
    movq    %rdi, -40(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rax
    andq    %r14, %rdi
    movq    %rax, 32(%rsp)
    movq    -56(%rsp), %rax
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, -24(%rsp)
    movq    -112(%rsp), %rdi
    orq %rcx, %r15
    movq    56(%rsp), %rcx
    xorq    %r14, %r15
    movq    %r12, %r14
    xorq    %rsi, %rax
    movq    %r15, -48(%rsp)
    rolq    $28, %rax
    xorq    %r9, %rdi
    rolq    $3, %rdi
    xorq    %rdx, %rcx
    rolq    $20, %rcx
    rolq    $61, %rbp
    andq    %rdi, %r14
    xorq    %rcx, %r14
    movq    %rdi, %r13
    movq    %rbp, %r15
    orq %rcx, %r13
    movq    %r14, -112(%rsp)
    andq    %rax, %rcx
    movq    %rbp, %r14
    xorq    %rbp, %rcx
    xorq    %rax, %r13
    notq    %r14
    movq    %rcx, -8(%rsp)
    movq    48(%rsp), %rcx
    orq %r12, %r14
    orq %rax, %r15
    movq    -88(%rsp), %rax
    xorq    %rdi, %r14
    movq    -120(%rsp), %rdi
    xorq    %r12, %r15
    movq    -16(%rsp), %rbp
    xorq    %r9, %rbx
    movq    %r13, -56(%rsp)
    xorq    %rsi, %rcx
    rolq    $18, %rbx
    movq    %r15, (%rsp)
    rolq    $25, %rcx
    xorq    %r8, %rax
    movq    %rbx, %r15
    xorq    %r10, %rdi
    movq    %rcx, %r12
    rolq    %rax
    rolq    $6, %rdi
    xorq    %rdx, %rbp
    orq %rax, %r15
    orq %rdi, %r12
    rolq    $8, %rbp
    xorq    %rax, %r12
    movq    %rbp, %r13
    movq    %r12, -120(%rsp)
    movq    %rbp, %r12
    andq    %rcx, %r13
    notq    %r12
    xorq    %rdi, %r13
    andq    %rax, %rdi
    movq    %r12, %rbp
    xorq    %r12, %r15
    xorq    %rbx, %rdi
    andq    %rbx, %rbp
    movq    24(%rsp), %rax
    movq    %rdi, 56(%rsp)
    xorq    %rcx, %rbp
    movq    (%rsp), %rcx
    movq    %r13, -16(%rsp)
    movq    16(%rsp), %r12
    movq    %r15, 40(%rsp)
    movq    -104(%rsp), %rdi
    movq    -64(%rsp), %r13
    xorq    -48(%rsp), %rcx
    xorq    %r15, %rcx
    xorq    %rdx, %rax
    xorq    %r8, %r12
    rolq    $10, %r12
    xorq    %r9, %rdi
    rolq    $27, %rax
    rolq    $36, %rdi
    movq    %r12, %rbx
    xorq    %r10, %r13
    andq    %rdi, %rbx
    rolq    $15, %r13
    xorq    %rsi, %r11
    xorq    %rax, %rbx
    rolq    $56, %r11
    xorq    8(%rsp), %rsi
    movq    %rbx, -104(%rsp)
    movq    %r13, %rbx
    notq    %r13
    orq %r12, %rbx
    movq    %r13, %r15
    xorq    72(%rsp), %rdx
    xorq    %rdi, %rbx
    orq %rax, %rdi
    xorq    -72(%rsp), %r9
    xorq    %r11, %rdi
    orq %r11, %r15
    xorq    -96(%rsp), %r8
    movq    %rdi, 24(%rsp)
    movq    -32(%rsp), %rdi
    xorq    %r12, %r15
    rolq    $55, %rsi
    movq    %r11, %r12
    rolq    $39, %rdx
    andq    %rax, %r12
    movq    -56(%rsp), %rax
    movq    %r15, -64(%rsp)
    rolq    $41, %r9
    xorq    %r13, %r12
    rolq    $2, %r8
    xorq    %r10, %rdi
    movq    %rsi, %r10
    xorq    %r12, %rcx
    notq    %r10
    rolq    $62, %rdi
    movq    %r9, %r11
    movq    %r10, %r15
    movq    %r8, %r13
    andq    %rdx, %r15
    xorq    %rdi, %r15
    xorq    -80(%rsp), %rax
    orq %rdx, %r11
    xorq    %r10, %r11
    orq %rdi, %r13
    andq    %rdi, %rsi
    movq    %r8, %r10
    xorq    %r9, %r13
    andq    %r9, %r10
    xorq    %r13, %rcx
    xorq    %r8, %rsi
    xorq    -120(%rsp), %rax
    xorq    %rdx, %r10
    xorq    -104(%rsp), %rax
    movq    %r11, -96(%rsp)
    movq    -16(%rsp), %r11
    movq    %r13, -72(%rsp)
    movq    %rcx, %r13
    movq    -8(%rsp), %rdi
    rolq    %r13
    movq    -24(%rsp), %rdx
    xorq    %r15, %rax
    xorq    -112(%rsp), %r11
    xorq    32(%rsp), %rdi
    xorq    %r10, %rdx
    xorq    %r14, %rdx
    xorq    %rbx, %r11
    xorq    %rbp, %rdx
    xorq    -96(%rsp), %r11
    xorq    56(%rsp), %rdi
    xorq    -64(%rsp), %rdx
    xorq    -40(%rsp), %r11
    xorq    24(%rsp), %rdi
    movq    %rdx, %r8
    rolq    %r8
    movq    %r11, %r9
    xorq    %r11, %r13
    xorq    %rax, %r8
    movabsq $-9223372036854742903, %r11
    xorq    %rsi, %rdi
    rolq    %r9
    xorq    %rdi, %r9
    rolq    %rdi
    xorq    %rdx, %rdi
    rolq    %rax
    movq    -112(%rsp), %rdx
    xorq    %rcx, %rax
    movq    -80(%rsp), %rcx
    xorq    %r13, %rbp
    rolq    $43, %rbp
    xorq    %rdi, %r12
    xorq    %rax, %rsi
    rolq    $21, %r12
    rolq    $14, %rsi
    xorq    %r8, %rbx
    xorq    %r8, %rdx
    xorq    %r13, %r10
    rolq    $45, %rbx
    rolq    $44, %rdx
    xorq    %r9, %rcx
    rolq    $61, %r10
    xorq    %rcx, %r11
    movq    %r11, -80(%rsp)
    movq    %rdx, %r11
    orq %rbp, %r11
    xorq    %r11, -80(%rsp)
    movq    %rbp, %r11
    notq    %r11
    orq %r12, %r11
    xorq    %rdx, %r11
    andq    %rcx, %rdx
    movq    %r11, -88(%rsp)
    movq    %rsi, %r11
    xorq    %rsi, %rdx
    andq    %r12, %r11
    movq    %rdx, 16(%rsp)
    movq    -48(%rsp), %rdx
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    movq    -120(%rsp), %rsi
    orq %rcx, %rbp
    movq    -8(%rsp), %rcx
    movq    %r11, -32(%rsp)
    xorq    %r12, %rbp
    movq    %rbx, %r12
    xorq    %rdi, %rdx
    movq    %rbp, 8(%rsp)
    movq    %r10, %rbp
    xorq    %r9, %rsi
    rolq    $28, %rdx
    notq    %rbp
    rolq    $3, %rsi
    xorq    %rax, %rcx
    rolq    $20, %rcx
    movq    %rsi, %r11
    orq %rcx, %r11
    xorq    %rdx, %r11
    andq    %rsi, %r12
    orq %rbx, %rbp
    xorq    %rcx, %r12
    andq    %rdx, %rcx
    xorq    %rsi, %rbp
    xorq    %r10, %rcx
    movq    24(%rsp), %rsi
    movq    %r11, -48(%rsp)
    movq    %r10, %r11
    movq    40(%rsp), %r10
    movq    %rcx, 48(%rsp)
    movq    -40(%rsp), %rcx
    orq %rdx, %r11
    xorq    %r9, %r15
    movq    %r13, %rdx
    rolq    $18, %r15
    movq    %r12, -120(%rsp)
    xorq    %rax, %rsi
    xorq    %r14, %rdx
    movq    %rbp, -112(%rsp)
    xorq    %rdi, %r10
    rolq    $8, %rsi
    movq    %r15, %rbp
    rolq    $25, %r10
    xorq    %r8, %rcx
    rolq    $6, %rdx
    rolq    %rcx
    movq    %r10, %r12
    movq    %rsi, %r14
    orq %rdx, %r12
    andq    %r10, %r14
    notq    %rsi
    orq %rcx, %rbp
    xorq    %rbx, %r11
    xorq    %rcx, %r12
    xorq    %rdx, %r14
    xorq    %rsi, %rbp
    andq    %rdx, %rcx
    movq    %rsi, %rbx
    xorq    %r15, %rcx
    movq    %r11, -8(%rsp)
    andq    %r15, %rbx
    movq    %r12, 24(%rsp)
    movq    %r14, 40(%rsp)
    xorq    %r10, %rbx
    movq    %rbp, 64(%rsp)
    movq    -56(%rsp), %r11
    movq    32(%rsp), %rdx
    movq    %rcx, 72(%rsp)
    movq    -16(%rsp), %r10
    movq    -80(%rsp), %rcx
    xorq    %r9, %r11
    movq    -64(%rsp), %rbp
    xorq    %rax, %rdx
    rolq    $36, %r11
    movq    -72(%rsp), %r14
    rolq    $27, %rdx
    xorq    %r8, %r10
    xorq    -104(%rsp), %r9
    xorq    -48(%rsp), %rcx
    rolq    $10, %r10
    movq    %r10, %r12
    xorq    %r13, %rbp
    xorq    56(%rsp), %rax
    andq    %r11, %r12
    rolq    $15, %rbp
    xorq    -24(%rsp), %r13
    xorq    %rdx, %r12
    movq    %rbp, %r15
    xorq    -96(%rsp), %r8
    xorq    24(%rsp), %rcx
    movq    %r12, -72(%rsp)
    xorq    %rdi, %r14
    notq    %r15
    xorq    (%rsp), %rdi
    movq    %r10, %rsi
    rolq    $56, %r14
    orq %rbp, %rsi
    movq    %r15, %rbp
    xorq    %r11, %rsi
    orq %rdx, %r11
    xorq    %r12, %rcx
    movq    -88(%rsp), %r12
    orq %r14, %rbp
    xorq    %r10, %rbp
    rolq    $55, %rdi
    xorq    %r14, %r11
    movq    %rbp, -64(%rsp)
    movq    %r14, %rbp
    rolq    $39, %rax
    rolq    $41, %r9
    andq    %rdx, %rbp
    movq    %rdi, %rdx
    xorq    -120(%rsp), %r12
    notq    %rdx
    rolq    $62, %r13
    movq    %rdx, %r14
    movq    %r9, %r10
    andq    %rax, %r14
    rolq    $2, %r8
    xorq    %r15, %rbp
    movq    %r8, %r15
    xorq    40(%rsp), %r12
    xorq    %rsi, %r12
    xorq    %r13, %r14
    orq %rax, %r10
    xorq    %rdx, %r10
    movq    %r8, %rdx
    andq    %r9, %r15
    orq %r13, %rdx
    xorq    %rax, %r15
    movq    -112(%rsp), %rax
    xorq    %r9, %rdx
    andq    %r13, %rdi
    movq    48(%rsp), %r13
    movq    %rdx, -96(%rsp)
    movq    -8(%rsp), %rdx
    xorq    %r8, %rdi
    xorq    %r10, %r12
    movq    %r10, -104(%rsp)
    xorq    %r14, %rcx
    xorq    %rbx, %rax
    movq    %r12, %r10
    xorq    -32(%rsp), %rax
    xorq    %rdi, %r13
    rolq    %r10
    xorq    8(%rsp), %rdx
    xorq    16(%rsp), %r13
    xorq    -64(%rsp), %rax
    xorq    %rbp, %rdx
    xorq    64(%rsp), %rdx
    xorq    72(%rsp), %r13
    xorq    %r15, %rax
    movq    %rax, %r8
    xorq    -96(%rsp), %rdx
    rolq    %r8
    xorq    %r11, %r13
    xorq    %rcx, %r8
    xorq    %r13, %r10
    rolq    %r13
    xorq    %rax, %r13
    rolq    %rcx
    movq    -120(%rsp), %rax
    movq    %rdx, %r9
    xorq    %rdx, %rcx
    rolq    %r9
    xorq    %r12, %r9
    movq    -80(%rsp), %r12
    xorq    %r10, %r12
    xorq    %r9, %rbx
    xorq    %r8, %rax
    movq    %r12, -56(%rsp)
    rolq    $43, %rbx
    rolq    $44, %rax
    movabsq $-9223372036854743037, %r12
    movq    %rbx, %rdx
    xorq    %r13, %rbp
    xorq    -56(%rsp), %r12
    orq %rax, %rdx
    rolq    $21, %rbp
    xorq    %rcx, %rdi
    xorq    %r8, %rsi
    rolq    $14, %rdi
    xorq    %r9, %r15
    rolq    $45, %rsi
    rolq    $61, %r15
    xorq    %rdx, %r12
    movq    %rbx, %rdx
    notq    %rdx
    orq %rbp, %rdx
    xorq    %rax, %rdx
    movq    %rdx, -40(%rsp)
    andq    -56(%rsp), %rax
    movq    %rdi, %rdx
    andq    %rbp, %rdx
    xorq    %rbx, %rdx
    movq    -56(%rsp), %rbx
    movq    %rdx, -80(%rsp)
    movq    24(%rsp), %rdx
    xorq    %rdi, %rax
    movq    %rax, -16(%rsp)
    movq    48(%rsp), %rax
    orq %rdi, %rbx
    movq    8(%rsp), %rdi
    xorq    %r10, %rdx
    xorq    %rbp, %rbx
    rolq    $3, %rdx
    movq    %rbx, -24(%rsp)
    movq    %rsi, %rbx
    xorq    %rcx, %rax
    movq    %rdx, %rbp
    andq    %rdx, %rbx
    xorq    %r13, %rdi
    rolq    $20, %rax
    rolq    $28, %rdi
    orq %rax, %rbp
    xorq    %rdi, %rbp
    xorq    %rax, %rbx
    andq    %rdi, %rax
    movq    %rbp, -56(%rsp)
    movq    %r15, %rbp
    xorq    %r15, %rax
    notq    %rbp
    movq    %rbx, 32(%rsp)
    movq    64(%rsp), %rbx
    orq %rsi, %rbp
    movq    %rax, (%rsp)
    xorq    %rcx, %r11
    xorq    %rdx, %rbp
    movq    %r15, %rdx
    rolq    $8, %r11
    orq %rdi, %rdx
    movq    -88(%rsp), %rdi
    movq    %r11, %r15
    xorq    %rsi, %rdx
    movq    %rax, %rsi
    movq    -112(%rsp), %rax
    xorq    %r13, %rbx
    xorq    %r10, %r14
    movq    %rbp, -120(%rsp)
    rolq    $25, %rbx
    rolq    $18, %r14
    notq    %r11
    xorq    %r8, %rdi
    andq    %rbx, %r15
    movq    %rbx, %rbp
    xorq    %r9, %rax
    rolq    %rdi
    xorq    -16(%rsp), %rsi
    rolq    $6, %rax
    movq    %rdx, 24(%rsp)
    xorq    -24(%rsp), %rdx
    xorq    %rax, %r15
    orq %rax, %rbp
    xorq    %rdi, %rbp
    movq    %r15, 56(%rsp)
    movq    %r14, %r15
    movq    %rbp, 8(%rsp)
    orq %rdi, %r15
    movq    %r11, %rbp
    xorq    %r11, %r15
    andq    %r14, %rbp
    andq    %rdi, %rax
    movq    16(%rsp), %r11
    xorq    %r14, %rax
    xorq    %rbx, %rbp
    movq    40(%rsp), %r14
    xorq    %r15, %rdx
    xorq    %rax, %rsi
    movq    -48(%rsp), %rbx
    movq    %rbp, -112(%rsp)
    movq    %rax, 64(%rsp)
    movq    -64(%rsp), %rbp
    xorq    %rcx, %r11
    movq    -96(%rsp), %rdi
    movq    %r15, 48(%rsp)
    rolq    $27, %r11
    xorq    %r10, %rbx
    xorq    %r8, %r14
    rolq    $10, %r14
    rolq    $36, %rbx
    xorq    %r9, %rbp
    movq    %r14, %rax
    rolq    $15, %rbp
    xorq    %r13, %rdi
    andq    %rbx, %rax
    movq    %rbp, %r15
    notq    %rbp
    xorq    %r11, %rax
    orq %r14, %r15
    rolq    $56, %rdi
    movq    %rax, -96(%rsp)
    movq    -56(%rsp), %rax
    xorq    %rbx, %r15
    movq    %r15, -48(%rsp)
    movq    %rbp, %r15
    orq %r11, %rbx
    orq %rdi, %r15
    xorq    %rdi, %rbx
    xorq    %r14, %r15
    movq    %rdi, %r14
    xorq    %rbx, %rsi
    xorq    %r12, %rax
    andq    %r11, %r14
    xorq    8(%rsp), %rax
    xorq    %rbp, %r14
    xorq    %r14, %rdx
    xorq    -96(%rsp), %rax
    movq    %r15, -64(%rsp)
    xorq    -8(%rsp), %r13
    movq    %rbx, 16(%rsp)
    xorq    -72(%rsp), %r10
    xorq    72(%rsp), %rcx
    xorq    -32(%rsp), %r9
    xorq    -104(%rsp), %r8
    rolq    $55, %r13
    movq    %r13, %rdi
    rolq    $41, %r10
    notq    %rdi
    rolq    $39, %rcx
    movq    %r10, %rbp
    movq    %rdi, %rbx
    rolq    $62, %r9
    rolq    $2, %r8
    andq    %rcx, %rbx
    orq %rcx, %rbp
    xorq    %rdi, %rbp
    movq    56(%rsp), %rdi
    andq    %r9, %r13
    movq    %rbp, -104(%rsp)
    movq    %r8, %r11
    xorq    %r9, %rbx
    xorq    %r8, %r13
    orq %r9, %r11
    xorq    %rbx, %rax
    xorq    %r10, %r11
    xorq    %r13, %rsi
    xorq    32(%rsp), %rdi
    xorq    %r11, %rdx
    xorq    -48(%rsp), %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    andq    %r10, %rbp
    xorq    -40(%rsp), %rdi
    movq    %rdx, %r10
    xorq    %rcx, %rbp
    movq    -80(%rsp), %rcx
    rolq    %r10
    movq    %rdi, %r9
    xorq    %rdi, %r10
    xorq    %rbp, %rcx
    rolq    %r9
    xorq    -120(%rsp), %rcx
    xorq    %rsi, %r9
    rolq    %rsi
    xorq    -112(%rsp), %rcx
    xorq    %r15, %rcx
    movq    -112(%rsp), %r15
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    %r9, %rcx
    rolq    %r8
    xorq    %r12, %rcx
    movabsq $-9223372036854743038, %r12
    xorq    %rax, %r8
    rolq    %rax
    xorq    %rdx, %rax
    movq    32(%rsp), %rdx
    xorq    %r8, %rdx
    rolq    $44, %rdx
    xorq    %r10, %r15
    xorq    %rax, %r13
    rolq    $43, %r15
    xorq    %rcx, %r12
    rolq    $14, %r13
    movq    %r15, %rdi
    xorq    %rsi, %r14
    xorq    %r10, %rbp
    rolq    $21, %r14
    orq %rdx, %rdi
    rolq    $61, %rbp
    xorq    %rdi, %r12
    movq    %r15, %rdi
    movq    %r12, -88(%rsp)
    notq    %rdi
    movq    %r13, %r12
    orq %r14, %rdi
    andq    %r14, %r12
    xorq    %rdx, %rdi
    xorq    %r15, %r12
    andq    %rcx, %rdx
    movq    %r13, %r15
    xorq    %r13, %rdx
    movq    %r12, -72(%rsp)
    orq %rcx, %r15
    movq    -48(%rsp), %r12
    movq    %rdx, 32(%rsp)
    movq    8(%rsp), %rcx
    movq    %rdi, -112(%rsp)
    xorq    %r14, %r15
    movq    (%rsp), %rdx
    movq    %r15, -32(%rsp)
    movq    %rbp, %r15
    movq    -24(%rsp), %rdi
    notq    %r15
    xorq    %r8, %r12
    xorq    %r9, %rcx
    rolq    $45, %r12
    rolq    $3, %rcx
    xorq    %rax, %rdx
    movq    %r12, %r14
    rolq    $20, %rdx
    xorq    %rsi, %rdi
    movq    %rcx, %r13
    rolq    $28, %rdi
    orq %rdx, %r13
    andq    %rcx, %r14
    orq %r12, %r15
    xorq    %rdi, %r13
    xorq    %rdx, %r14
    xorq    %rcx, %r15
    movq    %rbp, %rcx
    andq    %rdi, %rdx
    orq %rdi, %rcx
    xorq    %rbp, %rdx
    movq    16(%rsp), %rbp
    xorq    %r12, %rcx
    movq    -120(%rsp), %rdi
    movq    %rdx, -8(%rsp)
    movq    %rcx, (%rsp)
    movq    48(%rsp), %rcx
    xorq    %r9, %rbx
    movq    -40(%rsp), %rdx
    movq    %r13, -24(%rsp)
    rolq    $18, %rbx
    xorq    %rax, %rbp
    movq    %r14, -48(%rsp)
    movq    %rbx, %r14
    xorq    %r10, %rdi
    rolq    $8, %rbp
    xorq    %rsi, %rcx
    rolq    $6, %rdi
    movq    %rbp, %r13
    rolq    $25, %rcx
    xorq    %r8, %rdx
    notq    %rbp
    movq    %rcx, %r12
    rolq    %rdx
    andq    %rcx, %r13
    orq %rdi, %r12
    xorq    %rdi, %r13
    andq    %rdx, %rdi
    xorq    %rdx, %r12
    xorq    %rbx, %rdi
    orq %rdx, %r14
    movq    %r12, -120(%rsp)
    movq    -16(%rsp), %rdx
    xorq    %rbp, %r14
    movq    %r13, 8(%rsp)
    movq    %rbp, %r13
    movq    -64(%rsp), %r12
    andq    %rbx, %r13
    movq    56(%rsp), %rbx
    movq    %rdi, 40(%rsp)
    xorq    %rcx, %r13
    movq    (%rsp), %rcx
    movq    %r14, 16(%rsp)
    movq    -56(%rsp), %rdi
    xorq    %rax, %rdx
    rolq    $27, %rdx
    xorq    %r8, %rbx
    xorq    -32(%rsp), %rcx
    rolq    $10, %rbx
    xorq    %r9, %rdi
    movq    %rbx, %rbp
    rolq    $36, %rdi
    xorq    %r14, %rcx
    xorq    %r10, %r12
    andq    %rdi, %rbp
    xorq    %rdx, %rbp
    rolq    $15, %r12
    xorq    %rsi, %r11
    movq    %rbp, -64(%rsp)
    movq    %r12, %rbp
    rolq    $56, %r11
    orq %rbx, %rbp
    xorq    24(%rsp), %rsi
    notq    %r12
    xorq    %rdi, %rbp
    orq %rdx, %rdi
    xorq    64(%rsp), %rax
    xorq    %r11, %rdi
    movq    %r12, %r14
    xorq    -104(%rsp), %r8
    movq    %rdi, -56(%rsp)
    movq    -80(%rsp), %rdi
    orq %r11, %r14
    rolq    $55, %rsi
    xorq    %rbx, %r14
    xorq    -96(%rsp), %r9
    movq    %r14, -16(%rsp)
    movq    %r11, %r14
    rolq    $39, %rax
    andq    %rdx, %r14
    rolq    $2, %r8
    xorq    %r10, %rdi
    movq    %rsi, %r10
    xorq    %r12, %r14
    notq    %r10
    rolq    $62, %rdi
    movq    -8(%rsp), %r12
    movq    %r10, %rdx
    rolq    $41, %r9
    xorq    %r14, %rcx
    andq    %rax, %rdx
    movq    %r9, %rbx
    movq    %r8, %r11
    xorq    %rdi, %rdx
    orq %rax, %rbx
    movq    %rdx, -104(%rsp)
    movq    -24(%rsp), %rdx
    xorq    %r10, %rbx
    movq    %rbx, -96(%rsp)
    movq    8(%rsp), %rbx
    movq    %r8, %r10
    xorq    -88(%rsp), %rdx
    xorq    -120(%rsp), %rdx
    xorq    -64(%rsp), %rdx
    xorq    -104(%rsp), %rdx
    xorq    -48(%rsp), %rbx
    andq    %r9, %r10
    orq %rdi, %r11
    xorq    32(%rsp), %r12
    xorq    %rax, %r10
    xorq    %r9, %r11
    movq    -72(%rsp), %rax
    andq    %rdi, %rsi
    xorq    %r11, %rcx
    xorq    %r8, %rsi
    movq    %rcx, %r9
    xorq    %rbp, %rbx
    rolq    %r9
    xorq    -96(%rsp), %rbx
    xorq    40(%rsp), %r12
    xorq    %r10, %rax
    xorq    %r15, %rax
    xorq    %r13, %rax
    xorq    -112(%rsp), %rbx
    xorq    -16(%rsp), %rax
    xorq    -56(%rsp), %r12
    movq    %rbx, %r8
    xorq    %rbx, %r9
    movq    %rdx, %rbx
    rolq    %r8
    movq    %rax, %rdi
    rolq    %rbx
    xorq    %rsi, %r12
    rolq    %rdi
    xorq    %rcx, %rbx
    xorq    %r12, %r8
    rolq    %r12
    xorq    %rdx, %rdi
    xorq    %rax, %r12
    movq    -88(%rsp), %rdx
    xorq    %r9, %r13
    movabsq $-9223372036854775680, %rcx
    movq    -48(%rsp), %rax
    xorq    %r8, %rdx
    xorq    %rdi, %rax
    rolq    $44, %rax
    rolq    $43, %r13
    xorq    %rdx, %rcx
    movq    %rcx, -88(%rsp)
    movq    %rax, %rcx
    xorq    %r12, %r14
    orq %r13, %rcx
    rolq    $21, %r14
    xorq    %rbx, %rsi
    xorq    %rcx, -88(%rsp)
    movq    %r13, %rcx
    rolq    $14, %rsi
    notq    %rcx
    xorq    %r9, %r10
    orq %r14, %rcx
    rolq    $61, %r10
    xorq    %rdi, %rbp
    xorq    %rax, %rcx
    andq    %rdx, %rax
    rolq    $45, %rbp
    movq    %rcx, -40(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rax
    andq    %r14, %rcx
    movq    %rax, 24(%rsp)
    movq    -32(%rsp), %rax
    xorq    %r13, %rcx
    movq    %rsi, %r13
    movq    %rbp, %rsi
    movq    %rcx, -80(%rsp)
    movq    -120(%rsp), %rcx
    orq %rdx, %r13
    movq    -8(%rsp), %rdx
    xorq    %r14, %r13
    xorq    %r12, %rax
    movq    %r13, -48(%rsp)
    movq    %r10, %r13
    rolq    $28, %rax
    notq    %r13
    xorq    %r8, %rcx
    orq %rbp, %r13
    rolq    $3, %rcx
    xorq    %rbx, %rdx
    rolq    $20, %rdx
    movq    %rcx, %r14
    andq    %rcx, %rsi
    orq %rdx, %r14
    xorq    %rdx, %rsi
    xorq    %rcx, %r13
    xorq    %rax, %r14
    movq    %rsi, -8(%rsp)
    movq    -112(%rsp), %rsi
    movq    %r14, -32(%rsp)
    movq    %r10, %r14
    movq    %r9, %rcx
    orq %rax, %r14
    movq    %r13, -120(%rsp)
    movq    -16(%rsp), %r13
    xorq    %rbp, %r14
    andq    %rax, %rdx
    movq    -56(%rsp), %rax
    xorq    %r10, %rdx
    movq    16(%rsp), %r10
    xorq    %r15, %rcx
    movq    -104(%rsp), %rbp
    rolq    $6, %rcx
    xorq    %rdi, %rsi
    rolq    %rsi
    movq    %rdx, 48(%rsp)
    xorq    %r9, %r13
    xorq    %rbx, %rax
    xorq    %r12, %r11
    movq    %r14, 56(%rsp)
    rolq    $8, %rax
    xorq    %r12, %r10
    rolq    $56, %r11
    rolq    $25, %r10
    xorq    %r8, %rbp
    movq    %rax, %rdx
    movq    %r10, %r15
    notq    %rax
    rolq    $18, %rbp
    orq %rcx, %r15
    andq    %r10, %rdx
    rolq    $15, %r13
    xorq    %rsi, %r15
    xorq    %rcx, %rdx
    movq    %r15, -112(%rsp)
    movq    %rax, %r15
    andq    %rbp, %r15
    movq    %rdx, -104(%rsp)
    movq    32(%rsp), %rdx
    xorq    %r10, %r15
    movq    %rbp, %r10
    orq %rsi, %r10
    andq    %rcx, %rsi
    movq    -40(%rsp), %rcx
    xorq    %rax, %r10
    movq    -24(%rsp), %rax
    xorq    %rbp, %rsi
    movq    %r10, -56(%rsp)
    movq    8(%rsp), %r10
    xorq    %rbx, %rdx
    movq    %rsi, 16(%rsp)
    movq    -88(%rsp), %rsi
    rolq    $27, %rdx
    xorq    %r8, %rax
    xorq    %rdi, %r10
    rolq    $36, %rax
    rolq    $10, %r10
    movq    %r10, %r14
    movq    %r10, %rbp
    andq    %rax, %r14
    xorq    -32(%rsp), %rsi
    orq %r13, %rbp
    xorq    %rdx, %r14
    xorq    -8(%rsp), %rcx
    notq    %r13
    movq    %r14, -24(%rsp)
    xorq    %rax, %rbp
    xorq    -112(%rsp), %rsi
    xorq    -104(%rsp), %rcx
    xorq    %r14, %rsi
    movq    %r13, %r14
    orq %r11, %r14
    xorq    %rbp, %rcx
    xorq    %r10, %r14
    movq    %r14, -16(%rsp)
    movq    %r11, %r14
    xorq    -64(%rsp), %r8
    xorq    (%rsp), %r12
    andq    %rdx, %r14
    orq %rax, %rdx
    movq    40(%rsp), %rax
    xorq    %r11, %rdx
    xorq    %r13, %r14
    movq    -96(%rsp), %r10
    rolq    $41, %r8
    xorq    -72(%rsp), %r9
    rolq    $55, %r12
    xorq    %rbx, %rax
    movq    %r8, %rbx
    rolq    $39, %rax
    xorq    %rdi, %r10
    movq    %r12, %rdi
    notq    %rdi
    orq %rax, %rbx
    rolq    $2, %r10
    xorq    %rdi, %rbx
    movq    %rdi, %r11
    rolq    $62, %r9
    xorq    %rbx, %rcx
    andq    %rax, %r11
    movq    %rbx, -96(%rsp)
    movq    %r10, %rbx
    xorq    %r9, %r11
    movq    -120(%rsp), %rdi
    andq    %r8, %rbx
    xorq    %r11, %rsi
    movq    %r10, %r13
    xorq    %rax, %rbx
    movq    56(%rsp), %rax
    orq %r9, %r13
    xorq    %r8, %r13
    andq    %r9, %r12
    xorq    %r15, %rdi
    movq    %r13, -72(%rsp)
    xorq    %r10, %r12
    xorq    -80(%rsp), %rdi
    movq    %rcx, %r10
    xorq    -48(%rsp), %rax
    rolq    %r10
    xorq    -16(%rsp), %rdi
    xorq    %r14, %rax
    xorq    -56(%rsp), %rax
    xorq    %rbx, %rdi
    movq    %rdi, %r8
    xorq    %r13, %rax
    movq    48(%rsp), %r13
    rolq    %r8
    movq    %rax, %r9
    xorq    %rsi, %r8
    rolq    %r9
    xorq    %rcx, %r9
    movq    %rsi, %rcx
    movq    -88(%rsp), %rsi
    xorq    %r12, %r13
    rolq    %rcx
    xorq    %r9, %r15
    xorq    24(%rsp), %r13
    xorq    %rax, %rcx
    rolq    $43, %r15
    movq    -8(%rsp), %rax
    xorq    16(%rsp), %r13
    xorq    %r8, %rax
    rolq    $44, %rax
    xorq    %rdx, %r13
    xorq    %r13, %r10
    rolq    %r13
    xorq    %rdi, %r13
    xorq    %r10, %rsi
    xorq    %r13, %r14
    movq    %rsi, %rdi
    rolq    $21, %r14
    xorq    $32778, %rdi
    xorq    %rcx, %r12
    movq    %rdi, -64(%rsp)
    movq    %r15, %rdi
    rolq    $14, %r12
    orq %rax, %rdi
    xorq    %r9, %rbx
    xorq    %r8, %rbp
    xorq    %rdi, -64(%rsp)
    movq    %r15, %rdi
    rolq    $61, %rbx
    notq    %rdi
    rolq    $45, %rbp
    orq %r14, %rdi
    xorq    %rax, %rdi
    andq    %rsi, %rax
    movq    %rdi, -88(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rax
    andq    %r14, %rdi
    movq    %rax, -8(%rsp)
    movq    -112(%rsp), %rax
    xorq    %r15, %rdi
    movq    %r12, %r15
    orq %rsi, %r15
    movq    -48(%rsp), %rsi
    movq    %rdi, 32(%rsp)
    xorq    %r14, %r15
    movq    48(%rsp), %rdi
    movq    %rbp, %r14
    movq    %r15, (%rsp)
    movq    %rbx, %r15
    xorq    %r10, %rax
    notq    %r15
    rolq    $3, %rax
    xorq    %r13, %rsi
    orq %rbp, %r15
    andq    %rax, %r14
    rolq    $28, %rsi
    xorq    %rax, %r15
    xorq    %rcx, %rdi
    movq    %rax, %r12
    movq    %rbx, %rax
    rolq    $20, %rdi
    orq %rsi, %rax
    orq %rdi, %r12
    xorq    %rdi, %r14
    xorq    %rbp, %rax
    xorq    %rsi, %r12
    movq    %r14, 8(%rsp)
    movq    %rax, 40(%rsp)
    xorq    (%rsp), %rax
    andq    %rsi, %rdi
    xorq    %rbx, %rdi
    movq    %r12, -48(%rsp)
    xorq    %rcx, %rdx
    movq    %r15, -112(%rsp)
    movq    %rdi, %rsi
    rolq    $8, %rdx
    movq    %rdi, 48(%rsp)
    movq    -56(%rsp), %rbp
    xorq    %r10, %r11
    movq    -120(%rsp), %rbx
    movq    %rdx, %r14
    notq    %rdx
    movq    -40(%rsp), %rdi
    rolq    $18, %r11
    movq    %rdx, %r15
    xorq    -8(%rsp), %rsi
    andq    %r11, %r15
    xorq    %r13, %rbp
    rolq    $25, %rbp
    xorq    %r9, %rbx
    xorq    %r8, %rdi
    rolq    $6, %rbx
    xorq    %rbp, %r15
    rolq    %rdi
    andq    %rbp, %r14
    movq    %rbp, %r12
    movq    %r11, %rbp
    xorq    %rbx, %r14
    orq %rbx, %r12
    orq %rdi, %rbp
    andq    %rdi, %rbx
    movq    %r14, 64(%rsp)
    xorq    %rdx, %rbp
    xorq    %r11, %rbx
    movq    -104(%rsp), %r14
    xorq    %rbp, %rax
    xorq    %rbx, %rsi
    movq    %rbp, 72(%rsp)
    movq    %rbx, 96(%rsp)
    movq    24(%rsp), %r11
    xorq    %rdi, %r12
    movq    -32(%rsp), %rbx
    movq    %r12, -56(%rsp)
    movq    -16(%rsp), %rbp
    xorq    %r8, %r14
    movq    -72(%rsp), %rdi
    rolq    $10, %r14
    movq    -48(%rsp), %rdx
    xorq    %rcx, %r11
    movq    %r14, %r12
    xorq    %r10, %rbx
    rolq    $27, %r11
    xorq    %r9, %rbp
    rolq    $36, %rbx
    rolq    $15, %rbp
    xorq    %r13, %rdi
    andq    %rbx, %r12
    xorq    -64(%rsp), %rdx
    xorq    %r11, %r12
    rolq    $56, %rdi
    xorq    56(%rsp), %r13
    movq    %r12, -104(%rsp)
    xorq    -24(%rsp), %r10
    xorq    16(%rsp), %rcx
    xorq    -56(%rsp), %rdx
    rolq    $55, %r13
    xorq    -80(%rsp), %r9
    rolq    $41, %r10
    xorq    -96(%rsp), %r8
    rolq    $39, %rcx
    xorq    %r12, %rdx
    movq    %rbp, %r12
    notq    %rbp
    orq %r14, %r12
    movq    %rbp, -120(%rsp)
    orq %rdi, %rbp
    xorq    %r14, %rbp
    xorq    %rbx, %r12
    orq %r11, %rbx
    xorq    %rdi, %rbx
    movq    %rbp, -72(%rsp)
    movq    %rdi, %r14
    movq    %r10, %rbp
    movq    %r13, %rdi
    xorq    %rbx, %rsi
    notq    %rdi
    orq %rcx, %rbp
    movq    %rbx, -32(%rsp)
    xorq    %rdi, %rbp
    movq    %rdi, %rbx
    movq    64(%rsp), %rdi
    andq    %r11, %r14
    rolq    $62, %r9
    andq    %rcx, %rbx
    xorq    -120(%rsp), %r14
    xorq    %r9, %rbx
    rolq    $2, %r8
    xorq    %rbx, %rdx
    movq    %rbp, -96(%rsp)
    movq    %r8, %r11
    xorq    8(%rsp), %rdi
    xorq    %r14, %rax
    xorq    %r12, %rdi
    andq    %r9, %r13
    orq %r9, %r11
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    %r8, %r13
    andq    %r10, %rbp
    xorq    -88(%rsp), %rdi
    xorq    %r10, %r11
    xorq    %rcx, %rbp
    movq    32(%rsp), %rcx
    xorq    %r13, %rsi
    xorq    %r11, %rax
    movq    %rax, %r10
    movq    %rdi, %r9
    rolq    %r10
    xorq    %rbp, %rcx
    rolq    %r9
    xorq    %rdi, %r10
    xorq    -112(%rsp), %rcx
    xorq    %rsi, %r9
    rolq    %rsi
    movabsq $-9223372034707292150, %rdi
    xorq    %r15, %rcx
    xorq    %r10, %r15
    xorq    -72(%rsp), %rcx
    rolq    $43, %r15
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    -64(%rsp), %rcx
    rolq    %r8
    xorq    %rsi, %r14
    xorq    %rdx, %r8
    rolq    %rdx
    xorq    %rax, %rdx
    movq    8(%rsp), %rax
    rolq    $21, %r14
    xorq    %r9, %rcx
    xorq    %rdx, %r13
    xorq    %r8, %rax
    rolq    $44, %rax
    rolq    $14, %r13
    xorq    %rcx, %rdi
    movq    %rdi, -40(%rsp)
    movq    %r15, %rdi
    xorq    %r8, %r12
    orq %rax, %rdi
    rolq    $45, %r12
    xorq    %r10, %rbp
    xorq    %rdi, -40(%rsp)
    movq    %r15, %rdi
    rolq    $61, %rbp
    notq    %rdi
    orq %r14, %rdi
    xorq    %rax, %rdi
    andq    %rcx, %rax
    movq    %rdi, -120(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rax
    andq    %r14, %rdi
    movq    %rax, -64(%rsp)
    movq    (%rsp), %rax
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, -80(%rsp)
    movq    -56(%rsp), %rdi
    orq %rcx, %r15
    movq    48(%rsp), %rcx
    xorq    %r14, %r15
    movq    %r12, %r14
    xorq    %rsi, %rax
    movq    %r15, -24(%rsp)
    movq    %rbp, %r15
    rolq    $28, %rax
    xorq    %r9, %rdi
    orq %rax, %r15
    rolq    $3, %rdi
    xorq    %rdx, %rcx
    xorq    %r12, %r15
    rolq    $20, %rcx
    andq    %rdi, %r14
    movq    %rdi, %r13
    xorq    %rcx, %r14
    orq %rcx, %r13
    andq    %rax, %rcx
    movq    %r14, -56(%rsp)
    movq    %rbp, %r14
    xorq    %rax, %r13
    xorq    %rbp, %rcx
    movq    -88(%rsp), %rax
    notq    %r14
    orq %r12, %r14
    movq    %rcx, (%rsp)
    movq    72(%rsp), %rcx
    xorq    %rdi, %r14
    movq    -112(%rsp), %rdi
    movq    %r13, -16(%rsp)
    movq    -32(%rsp), %rbp
    movq    %r15, 24(%rsp)
    xorq    %r8, %rax
    rolq    %rax
    xorq    %rsi, %rcx
    xorq    %r9, %rbx
    rolq    $25, %rcx
    xorq    %r10, %rdi
    rolq    $18, %rbx
    rolq    $6, %rdi
    xorq    %rdx, %rbp
    movq    %rcx, %r12
    rolq    $8, %rbp
    orq %rdi, %r12
    movq    %rbx, %r15
    xorq    %rax, %r12
    movq    %rbp, %r13
    orq %rax, %r15
    movq    %r12, -112(%rsp)
    andq    %rcx, %r13
    movq    %rbp, %r12
    xorq    %rdi, %r13
    notq    %r12
    andq    %rax, %rdi
    xorq    %r12, %r15
    xorq    %rbx, %rdi
    movq    %r12, %rbp
    movq    64(%rsp), %r12
    andq    %rbx, %rbp
    movq    %rdi, 16(%rsp)
    movq    -48(%rsp), %rdi
    xorq    %rcx, %rbp
    movq    %r13, -32(%rsp)
    movq    -8(%rsp), %rax
    xorq    %rsi, %r11
    movq    %r15, 8(%rsp)
    movq    24(%rsp), %rcx
    rolq    $56, %r11
    movq    -72(%rsp), %r13
    xorq    %r8, %r12
    rolq    $10, %r12
    xorq    %r9, %rdi
    rolq    $36, %rdi
    xorq    %rdx, %rax
    movq    %r12, %rbx
    xorq    -24(%rsp), %rcx
    rolq    $27, %rax
    andq    %rdi, %rbx
    xorq    %r10, %r13
    xorq    %rax, %rbx
    rolq    $15, %r13
    movq    %rbx, -72(%rsp)
    movq    %r13, %rbx
    notq    %r13
    xorq    %r15, %rcx
    movq    %r13, %r15
    orq %r12, %rbx
    orq %r11, %r15
    xorq    %rdi, %rbx
    orq %rax, %rdi
    xorq    %r12, %r15
    movq    %r11, %r12
    xorq    %r11, %rdi
    andq    %rax, %r12
    movq    -16(%rsp), %rax
    movq    %rdi, -8(%rsp)
    xorq    40(%rsp), %rsi
    movq    %r15, -48(%rsp)
    xorq    %r13, %r12
    xorq    -104(%rsp), %r9
    xorq    %r12, %rcx
    xorq    96(%rsp), %rdx
    xorq    -40(%rsp), %rax
    movq    32(%rsp), %rdi
    rolq    $55, %rsi
    rolq    $41, %r9
    xorq    -96(%rsp), %r8
    rolq    $39, %rdx
    movq    %r9, %r11
    xorq    -112(%rsp), %rax
    orq %rdx, %r11
    xorq    %r10, %rdi
    movq    %rsi, %r10
    rolq    $2, %r8
    notq    %r10
    rolq    $62, %rdi
    xorq    %r10, %r11
    movq    %r10, %r15
    movq    %r8, %r10
    xorq    -72(%rsp), %rax
    movq    %r11, -104(%rsp)
    andq    %r9, %r10
    movq    -32(%rsp), %r11
    andq    %rdx, %r15
    xorq    %rdx, %r10
    movq    -80(%rsp), %rdx
    xorq    %rdi, %r15
    movq    %r8, %r13
    xorq    %r15, %rax
    xorq    -56(%rsp), %r11
    xorq    %rbx, %r11
    xorq    -104(%rsp), %r11
    xorq    -120(%rsp), %r11
    xorq    %r10, %rdx
    orq %rdi, %r13
    andq    %rdi, %rsi
    movq    (%rsp), %rdi
    xorq    %r14, %rdx
    xorq    %rbp, %rdx
    xorq    %r9, %r13
    xorq    %r8, %rsi
    xorq    -48(%rsp), %rdx
    xorq    %r13, %rcx
    movq    %r13, -96(%rsp)
    movq    %r11, %r9
    movq    %rcx, %r13
    xorq    -64(%rsp), %rdi
    rolq    %r9
    rolq    %r13
    xorq    %r11, %r13
    movabsq $-9223372034707259263, %r11
    movq    %rdx, %r8
    xorq    %r13, %rbp
    rolq    %r8
    rolq    $43, %rbp
    xorq    16(%rsp), %rdi
    xorq    %rax, %r8
    rolq    %rax
    xorq    %rcx, %rax
    movq    -40(%rsp), %rcx
    xorq    -8(%rsp), %rdi
    xorq    %rsi, %rdi
    xorq    %rax, %rsi
    xorq    %rdi, %r9
    rolq    %rdi
    xorq    %rdx, %rdi
    movq    -56(%rsp), %rdx
    xorq    %r9, %rcx
    xorq    %rcx, %r11
    xorq    %rdi, %r12
    rolq    $14, %rsi
    movq    %r11, -56(%rsp)
    rolq    $21, %r12
    xorq    %r8, %rdx
    rolq    $44, %rdx
    movq    %rdx, %r11
    orq %rbp, %r11
    xorq    %r8, %rbx
    xorq    %r13, %r10
    xorq    %r11, -56(%rsp)
    movq    %rbp, %r11
    rolq    $45, %rbx
    notq    %r11
    rolq    $61, %r10
    orq %r12, %r11
    xorq    %rdx, %r11
    andq    %rcx, %rdx
    movq    %r11, -88(%rsp)
    movq    %rsi, %r11
    xorq    %rsi, %rdx
    andq    %r12, %r11
    movq    %rdx, 56(%rsp)
    movq    -24(%rsp), %rdx
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    movq    -112(%rsp), %rsi
    orq %rcx, %rbp
    movq    (%rsp), %rcx
    movq    %r11, 32(%rsp)
    xorq    %r12, %rbp
    movq    %rbx, %r12
    xorq    %rdi, %rdx
    movq    %rbp, 40(%rsp)
    movq    %r10, %rbp
    xorq    %r9, %rsi
    rolq    $28, %rdx
    notq    %rbp
    rolq    $3, %rsi
    xorq    %rax, %rcx
    orq %rbx, %rbp
    rolq    $20, %rcx
    andq    %rsi, %r12
    movq    %rsi, %r11
    xorq    %rcx, %r12
    orq %rcx, %r11
    andq    %rdx, %rcx
    xorq    %r10, %rcx
    xorq    %rdx, %r11
    xorq    %rsi, %rbp
    movq    %rcx, 48(%rsp)
    movq    -120(%rsp), %rcx
    movq    %r11, -24(%rsp)
    movq    %r10, %r11
    movq    8(%rsp), %r10
    movq    -8(%rsp), %rsi
    orq %rdx, %r11
    movq    %r13, %rdx
    xorq    %r14, %rdx
    xorq    %rbx, %r11
    movq    %r12, -112(%rsp)
    xorq    %r8, %rcx
    movq    %rbp, -40(%rsp)
    rolq    %rcx
    rolq    $6, %rdx
    xorq    %rdi, %r10
    rolq    $25, %r10
    xorq    %rax, %rsi
    xorq    %r9, %r15
    rolq    $18, %r15
    rolq    $8, %rsi
    movq    %r10, %r12
    orq %rdx, %r12
    movq    %rsi, %r14
    movq    %r15, %rbp
    xorq    %rcx, %r12
    andq    %r10, %r14
    notq    %rsi
    orq %rcx, %rbp
    andq    %rdx, %rcx
    xorq    %rdx, %r14
    xorq    %rsi, %rbp
    xorq    %r15, %rcx
    movq    %r11, (%rsp)
    movq    %r12, -120(%rsp)
    movq    %rsi, %rbx
    movq    %r14, -8(%rsp)
    andq    %r15, %rbx
    movq    %rbp, 8(%rsp)
    xorq    %r10, %rbx
    movq    -32(%rsp), %r10
    movq    %rcx, 64(%rsp)
    movq    -56(%rsp), %rcx
    movq    -16(%rsp), %r11
    movq    -64(%rsp), %rdx
    movq    -48(%rsp), %rbp
    xorq    %r8, %r10
    xorq    -24(%rsp), %rcx
    rolq    $10, %r10
    movq    -96(%rsp), %r14
    xorq    %r9, %r11
    movq    %r10, %r12
    xorq    %rax, %rdx
    rolq    $36, %r11
    movq    %r10, %rsi
    xorq    %r13, %rbp
    rolq    $27, %rdx
    andq    %r11, %r12
    xorq    -120(%rsp), %rcx
    rolq    $15, %rbp
    xorq    %rdx, %r12
    xorq    %rdi, %r14
    orq %rbp, %rsi
    movq    %r12, -96(%rsp)
    movq    %rbp, %r15
    rolq    $56, %r14
    notq    %r15
    xorq    %r12, %rcx
    xorq    %r11, %rsi
    xorq    24(%rsp), %rdi
    xorq    -104(%rsp), %r8
    movq    %r15, %rbp
    orq %rdx, %r11
    orq %r14, %rbp
    xorq    -72(%rsp), %r9
    xorq    %r14, %r11
    xorq    %r10, %rbp
    xorq    16(%rsp), %rax
    movq    %rbp, -32(%rsp)
    rolq    $55, %rdi
    movq    %r14, %rbp
    rolq    $2, %r8
    andq    %rdx, %rbp
    movq    %rdi, %rdx
    xorq    %r15, %rbp
    rolq    $41, %r9
    notq    %rdx
    movq    %r8, %r15
    rolq    $39, %rax
    movq    %rdx, %r14
    andq    %r9, %r15
    movq    %r9, %r10
    andq    %rax, %r14
    xorq    %rax, %r15
    orq %rax, %r10
    movq    -88(%rsp), %r12
    movq    -40(%rsp), %rax
    xorq    %rdx, %r10
    movq    %r8, %rdx
    xorq    -80(%rsp), %r13
    movq    %r10, -104(%rsp)
    xorq    -112(%rsp), %r12
    xorq    %rbx, %rax
    xorq    32(%rsp), %rax
    rolq    $62, %r13
    xorq    %r13, %r14
    orq %r13, %rdx
    xorq    -8(%rsp), %r12
    xorq    %r14, %rcx
    xorq    -32(%rsp), %rax
    xorq    %rsi, %r12
    xorq    %r10, %r12
    xorq    %r15, %rax
    xorq    %r9, %rdx
    andq    %r13, %rdi
    movq    %rdx, -80(%rsp)
    movq    (%rsp), %rdx
    xorq    %r8, %rdi
    movq    48(%rsp), %r13
    movq    %r12, %r10
    movq    %rax, %r8
    rolq    %r10
    rolq    %r8
    xorq    %rcx, %r8
    rolq    %rcx
    xorq    40(%rsp), %rdx
    xorq    %rdi, %r13
    xorq    56(%rsp), %r13
    xorq    %rbp, %rdx
    xorq    8(%rsp), %rdx
    xorq    64(%rsp), %r13
    xorq    -80(%rsp), %rdx
    xorq    %r11, %r13
    xorq    %r13, %r10
    rolq    %r13
    xorq    %rax, %r13
    movq    -112(%rsp), %rax
    movq    %rdx, %r9
    xorq    %rdx, %rcx
    xorq    %r13, %rbp
    rolq    %r9
    rolq    $21, %rbp
    xorq    %rcx, %rdi
    xorq    %r12, %r9
    movq    -56(%rsp), %r12
    rolq    $14, %rdi
    xorq    %r9, %rbx
    xorq    %r8, %rax
    rolq    $43, %rbx
    rolq    $44, %rax
    movq    %rbx, %rdx
    xorq    %r10, %r12
    orq %rax, %rdx
    movq    %r12, -64(%rsp)
    movabsq $-9223372036854742912, %r12
    xorq    -64(%rsp), %r12
    xorq    %rdx, %r12
    movq    %rbx, %rdx
    notq    %rdx
    orq %rbp, %rdx
    xorq    %r9, %r15
    xorq    %r8, %rsi
    rolq    $61, %r15
    xorq    %rax, %rdx
    rolq    $45, %rsi
    movq    %rdx, -112(%rsp)
    andq    -64(%rsp), %rax
    movq    %rdi, %rdx
    andq    %rbp, %rdx
    xorq    %rbx, %rdx
    movq    -64(%rsp), %rbx
    movq    %rdx, -72(%rsp)
    movq    -120(%rsp), %rdx
    xorq    %rdi, %rax
    movq    %rax, -64(%rsp)
    movq    48(%rsp), %rax
    orq %rdi, %rbx
    movq    40(%rsp), %rdi
    xorq    %r10, %rdx
    xorq    %rbp, %rbx
    rolq    $3, %rdx
    movq    %rbx, -48(%rsp)
    movq    %rsi, %rbx
    xorq    %rcx, %rax
    movq    %rdx, %rbp
    andq    %rdx, %rbx
    rolq    $20, %rax
    xorq    %r13, %rdi
    rolq    $28, %rdi
    orq %rax, %rbp
    xorq    %rax, %rbx
    xorq    %rdi, %rbp
    andq    %rdi, %rax
    movq    %rbx, -56(%rsp)
    movq    %rbp, -16(%rsp)
    movq    %r15, %rbp
    xorq    %r15, %rax
    notq    %rbp
    movq    %rax, 16(%rsp)
    movq    8(%rsp), %rbx
    orq %rsi, %rbp
    xorq    %rdx, %rbp
    movq    %r15, %rdx
    orq %rdi, %rdx
    movq    -88(%rsp), %rdi
    movq    %rbp, -120(%rsp)
    xorq    %rsi, %rdx
    movq    %rax, %rsi
    movq    -40(%rsp), %rax
    xorq    -64(%rsp), %rsi
    movq    %rdx, 24(%rsp)
    xorq    -48(%rsp), %rdx
    xorq    %r8, %rdi
    xorq    %r9, %rax
    rolq    %rdi
    rolq    $6, %rax
    xorq    %r13, %rbx
    xorq    %rcx, %r11
    rolq    $25, %rbx
    rolq    $8, %r11
    xorq    %r10, %r14
    movq    %rbx, %rbp
    movq    %r11, %r15
    notq    %r11
    orq %rax, %rbp
    andq    %rbx, %r15
    rolq    $18, %r14
    xorq    %rdi, %rbp
    xorq    %rax, %r15
    andq    %rdi, %rax
    movq    %rbp, -88(%rsp)
    movq    %r11, %rbp
    xorq    %r14, %rax
    andq    %r14, %rbp
    movq    %r15, -40(%rsp)
    movq    %r14, %r15
    movq    -8(%rsp), %r14
    xorq    %rbx, %rbp
    orq %rdi, %r15
    movq    -24(%rsp), %rbx
    xorq    %r11, %r15
    xorq    %rax, %rsi
    movq    56(%rsp), %r11
    movq    %rax, 40(%rsp)
    xorq    %r15, %rdx
    movq    -80(%rsp), %rdi
    movq    %rbp, 48(%rsp)
    xorq    %r8, %r14
    movq    -32(%rsp), %rbp
    movq    %r15, 8(%rsp)
    rolq    $10, %r14
    xorq    %r10, %rbx
    rolq    $36, %rbx
    xorq    %rcx, %r11
    movq    %r14, %rax
    rolq    $27, %r11
    andq    %rbx, %rax
    xorq    %r13, %rdi
    xorq    %r11, %rax
    xorq    %r9, %rbp
    rolq    $56, %rdi
    movq    %rax, -80(%rsp)
    movq    -16(%rsp), %rax
    rolq    $15, %rbp
    movq    %rbp, %r15
    notq    %rbp
    xorq    %r12, %rax
    xorq    -88(%rsp), %rax
    xorq    -80(%rsp), %rax
    orq %r14, %r15
    xorq    %rbx, %r15
    orq %r11, %rbx
    movq    %r15, -32(%rsp)
    movq    %rbp, %r15
    xorq    %rdi, %rbx
    orq %rdi, %r15
    xorq    %rbx, %rsi
    xorq    %r14, %r15
    movq    %rdi, %r14
    movq    %r15, -24(%rsp)
    xorq    (%rsp), %r13
    andq    %r11, %r14
    xorq    -96(%rsp), %r10
    xorq    %rbp, %r14
    movq    %rbx, -8(%rsp)
    xorq    64(%rsp), %rcx
    xorq    %r14, %rdx
    xorq    -104(%rsp), %r8
    rolq    $55, %r13
    xorq    32(%rsp), %r9
    rolq    $41, %r10
    movq    %r13, %rdi
    rolq    $39, %rcx
    movq    %r10, %rbp
    notq    %rdi
    orq %rcx, %rbp
    movq    %rdi, %rbx
    rolq    $2, %r8
    xorq    %rdi, %rbp
    movq    -40(%rsp), %rdi
    andq    %rcx, %rbx
    movq    %rbp, -96(%rsp)
    rolq    $62, %r9
    movq    %r8, %r11
    xorq    %r9, %rbx
    xorq    %rbx, %rax
    xorq    -56(%rsp), %rdi
    xorq    -32(%rsp), %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    andq    %r10, %rbp
    xorq    -112(%rsp), %rdi
    xorq    %rcx, %rbp
    movq    -72(%rsp), %rcx
    xorq    %rbp, %rcx
    xorq    -120(%rsp), %rcx
    andq    %r9, %r13
    orq %r9, %r11
    xorq    %r8, %r13
    xorq    %r10, %r11
    xorq    %r11, %rdx
    movq    %rdi, %r9
    xorq    %r13, %rsi
    movq    %rdx, %r10
    rolq    %r9
    xorq    48(%rsp), %rcx
    rolq    %r10
    xorq    %rsi, %r9
    xorq    %rdi, %r10
    rolq    %rsi
    xorq    %r15, %rcx
    movq    48(%rsp), %r15
    movq    %rcx, %r8
    xorq    %rcx, %rsi
    movq    %r9, %rcx
    rolq    %r8
    xorq    %r12, %rcx
    movl    $2147483649, %r12d
    xorq    %rax, %r8
    rolq    %rax
    xorq    %rcx, %r12
    xorq    %rdx, %rax
    movq    -56(%rsp), %rdx
    xorq    %r10, %r15
    rolq    $43, %r15
    xorq    %rsi, %r14
    xorq    %rax, %r13
    rolq    $14, %r13
    movq    %r15, %rdi
    rolq    $21, %r14
    xorq    %r8, %rdx
    rolq    $44, %rdx
    orq %rdx, %rdi
    xorq    %rdi, %r12
    movq    %r15, %rdi
    notq    %rdi
    movq    %r12, -56(%rsp)
    movq    %r13, %r12
    orq %r14, %rdi
    xorq    %rdx, %rdi
    andq    %r14, %r12
    andq    %rcx, %rdx
    xorq    %r15, %r12
    movq    %rdi, -104(%rsp)
    movq    %r13, %r15
    movq    %r12, (%rsp)
    movq    -32(%rsp), %r12
    xorq    %r13, %rdx
    movq    -88(%rsp), %rdi
    orq %rcx, %r15
    movq    %rdx, 56(%rsp)
    movq    16(%rsp), %rcx
    xorq    %r14, %r15
    xorq    %r10, %rbp
    movq    -48(%rsp), %rdx
    rolq    $61, %rbp
    movq    %r15, 32(%rsp)
    xorq    %r8, %r12
    movq    %rbp, %r15
    rolq    $45, %r12
    xorq    %r9, %rdi
    rolq    $3, %rdi
    xorq    %rax, %rcx
    movq    %r12, %r14
    rolq    $20, %rcx
    xorq    %rsi, %rdx
    andq    %rdi, %r14
    rolq    $28, %rdx
    xorq    %rcx, %r14
    movq    %rdi, %r13
    orq %rcx, %r13
    movq    %r14, -88(%rsp)
    andq    %rdx, %rcx
    movq    %rbp, %r14
    xorq    %rbp, %rcx
    xorq    %rdx, %r13
    notq    %r14
    movq    %rcx, -32(%rsp)
    movq    8(%rsp), %rcx
    orq %r12, %r14
    orq %rdx, %r15
    movq    -8(%rsp), %rbp
    xorq    %rdi, %r14
    movq    -120(%rsp), %rdi
    xorq    %r12, %r15
    movq    -112(%rsp), %rdx
    movq    %r13, 16(%rsp)
    xorq    %rsi, %rcx
    movq    %r15, 48(%rsp)
    rolq    $25, %rcx
    xorq    %rax, %rbp
    xorq    %r10, %rdi
    movq    %rcx, %r12
    rolq    $6, %rdi
    xorq    %r8, %rdx
    rolq    %rdx
    rolq    $8, %rbp
    xorq    %r9, %rbx
    orq %rdi, %r12
    rolq    $18, %rbx
    movq    %rbp, %r13
    xorq    %rdx, %r12
    andq    %rcx, %r13
    movq    %rbx, %r15
    movq    %r12, -120(%rsp)
    movq    %rbp, %r12
    xorq    %rdi, %r13
    notq    %r12
    orq %rdx, %r15
    andq    %rdx, %rdi
    xorq    %r12, %r15
    xorq    %rbx, %rdi
    movq    %r12, %rbp
    movq    -40(%rsp), %r12
    andq    %rbx, %rbp
    movq    %rdi, 8(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %rcx, %rbp
    movq    %r13, -8(%rsp)
    movq    -64(%rsp), %rdx
    xorq    %rsi, %r11
    movq    %r15, -48(%rsp)
    movq    48(%rsp), %rcx
    rolq    $56, %r11
    movq    -24(%rsp), %r13
    xorq    %r8, %r12
    rolq    $10, %r12
    xorq    %r9, %rdi
    rolq    $36, %rdi
    xorq    %rax, %rdx
    movq    %r12, %rbx
    xorq    32(%rsp), %rcx
    rolq    $27, %rdx
    andq    %rdi, %rbx
    xorq    %r10, %r13
    xorq    %rdx, %rbx
    rolq    $15, %r13
    movq    %rbx, 64(%rsp)
    movq    %r13, %rbx
    notq    %r13
    xorq    %r15, %rcx
    movq    %r13, %r15
    orq %r12, %rbx
    orq %r11, %r15
    xorq    %rdi, %rbx
    xorq    %r12, %r15
    movq    %r11, %r12
    andq    %rdx, %r12
    movq    %r15, -16(%rsp)
    xorq    %r13, %r12
    xorq    %r12, %rcx
    orq %rdx, %rdi
    xorq    24(%rsp), %rsi
    xorq    %r11, %rdi
    xorq    40(%rsp), %rax
    movq    %rdi, -24(%rsp)
    movq    -72(%rsp), %rdi
    xorq    -80(%rsp), %r9
    rolq    $55, %rsi
    xorq    -96(%rsp), %r8
    rolq    $39, %rax
    xorq    %r10, %rdi
    movq    %rsi, %r10
    rolq    $62, %rdi
    notq    %r10
    rolq    $41, %r9
    movq    %r10, %rdx
    movq    %r9, %r11
    rolq    $2, %r8
    andq    %rax, %rdx
    orq %rax, %r11
    xorq    %rdi, %rdx
    xorq    %r10, %r11
    movq    %r8, %r10
    movq    %rdx, -64(%rsp)
    movq    16(%rsp), %rdx
    andq    %r9, %r10
    movq    %r11, 24(%rsp)
    movq    -8(%rsp), %r11
    xorq    %rax, %r10
    movq    (%rsp), %rax
    xorq    -56(%rsp), %rdx
    xorq    -88(%rsp), %r11
    xorq    %r10, %rax
    xorq    %r14, %rax
    xorq    -120(%rsp), %rdx
    xorq    %rbp, %rax
    xorq    %rbx, %r11
    xorq    %r15, %rax
    xorq    24(%rsp), %r11
    movq    %r8, %r15
    orq %rdi, %r15
    xorq    64(%rsp), %rdx
    xorq    %r9, %r15
    xorq    -104(%rsp), %r11
    xorq    -64(%rsp), %rdx
    xorq    %r15, %rcx
    andq    %rdi, %rsi
    movq    -32(%rsp), %rdi
    xorq    %r8, %rsi
    movq    %rax, %r8
    movq    %r11, %r9
    rolq    %r8
    movq    %rcx, %r13
    rolq    %r9
    rolq    %r13
    xorq    %rdx, %r8
    rolq    %rdx
    xorq    %r11, %r13
    xorq    56(%rsp), %rdi
    xorq    %rcx, %rdx
    xorq    %r13, %rbp
    movq    -56(%rsp), %rcx
    rolq    $43, %rbp
    movabsq $-9223372034707259384, %r11
    xorq    8(%rsp), %rdi
    xorq    -24(%rsp), %rdi
    xorq    %rsi, %rdi
    xorq    %rdx, %rsi
    xorq    %rdi, %r9
    rolq    %rdi
    xorq    %rax, %rdi
    movq    -88(%rsp), %rax
    xorq    %r9, %rcx
    xorq    %rcx, %r11
    xorq    %rdi, %r12
    rolq    $14, %rsi
    movq    %r11, -88(%rsp)
    rolq    $21, %r12
    xorq    %r8, %rax
    rolq    $44, %rax
    movq    %rax, %r11
    orq %rbp, %r11
    xorq    %r11, -88(%rsp)
    movq    %rbp, %r11
    notq    %r11
    orq %r12, %r11
    xorq    %rax, %r11
    movq    %r11, -40(%rsp)
    movq    %rsi, %r11
    andq    %r12, %r11
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    orq %rcx, %rbp
    andq    %rcx, %rax
    movq    -32(%rsp), %rcx
    xorq    %rsi, %rax
    movq    -120(%rsp), %rsi
    xorq    %r12, %rbp
    movq    %rax, -96(%rsp)
    movq    32(%rsp), %rax
    xorq    %r13, %r10
    rolq    $61, %r10
    xorq    %r8, %rbx
    movq    %r11, -112(%rsp)
    xorq    %rdx, %rcx
    rolq    $45, %rbx
    xorq    %r9, %rsi
    rolq    $20, %rcx
    movq    %rbx, %r11
    rolq    $3, %rsi
    xorq    %rdi, %rax
    movq    %rsi, %r12
    rolq    $28, %rax
    andq    %rsi, %r11
    orq %rcx, %r12
    xorq    %rcx, %r11
    andq    %rax, %rcx
    xorq    %rax, %r12
    xorq    %r10, %rcx
    movq    %r11, -120(%rsp)
    movq    %r12, -80(%rsp)
    movq    %r10, %r12
    movq    -48(%rsp), %r11
    notq    %r12
    movq    %rcx, -32(%rsp)
    movq    %r13, %rcx
    orq %rbx, %r12
    xorq    %r14, %rcx
    movq    -64(%rsp), %r14
    xorq    %rsi, %r12
    movq    %r10, %rsi
    movq    -104(%rsp), %r10
    orq %rax, %rsi
    movq    -24(%rsp), %rax
    xorq    %rdi, %r11
    rolq    $25, %r11
    xorq    %rbx, %rsi
    rolq    $6, %rcx
    movq    %rsi, -72(%rsp)
    xorq    %r9, %r14
    movq    %r11, %rbx
    xorq    %r8, %r10
    xorq    %rdx, %rax
    rolq    %r10
    rolq    $8, %rax
    rolq    $18, %r14
    orq %rcx, %rbx
    movq    %rax, %rsi
    notq    %rax
    xorq    %r10, %rbx
    andq    %r11, %rsi
    movq    %rbx, -104(%rsp)
    xorq    %rdi, %r15
    xorq    %rcx, %rsi
    rolq    $56, %r15
    movq    %rsi, -24(%rsp)
    movq    %rax, %rsi
    andq    %r14, %rsi
    xorq    %r11, %rsi
    movq    %r14, %r11
    orq %r10, %r11
    andq    %rcx, %r10
    movq    56(%rsp), %rcx
    xorq    %rax, %r11
    movq    -8(%rsp), %rax
    xorq    %r14, %r10
    movq    16(%rsp), %r14
    movq    %r10, -64(%rsp)
    movq    -16(%rsp), %r10
    movq    %r11, -48(%rsp)
    xorq    %rdx, %rcx
    xorq    %r8, %rax
    rolq    $27, %rcx
    rolq    $10, %rax
    xorq    %r9, %r14
    rolq    $36, %r14
    xorq    %r13, %r10
    movq    %rax, %rbx
    rolq    $15, %r10
    andq    %r14, %rbx
    movq    %rax, %r11
    xorq    %rcx, %rbx
    orq %r10, %r11
    notq    %r10
    xorq    (%rsp), %r13
    movq    %rbx, -16(%rsp)
    movq    %r10, %rbx
    orq %r15, %rbx
    xorq    %r14, %r11
    xorq    %rax, %rbx
    movq    %r15, %rax
    orq %rcx, %r14
    andq    %rcx, %rax
    xorq    %r15, %r14
    movq    %r11, -56(%rsp)
    rolq    $62, %r13
    xorq    %r10, %rax
    xorq    48(%rsp), %rdi
    xorq    24(%rsp), %r8
    movq    %rbx, 32(%rsp)
    xorq    64(%rsp), %r9
    xorq    8(%rsp), %rdx
    rolq    $55, %rdi
    rolq    $2, %r8
    movq    %rdi, %rcx
    andq    %r13, %rdi
    rolq    $41, %r9
    notq    %rcx
    movq    %r8, %r11
    rolq    $39, %rdx
    movq    %rcx, %r15
    andq    %r9, %r11
    movq    %r9, %r10
    andq    %rdx, %r15
    xorq    %rdx, %r11
    orq %rdx, %r10
    movq    %r8, %rdx
    xorq    %r13, %r15
    orq %r13, %rdx
    xorq    %rcx, %r10
    xorq    %r8, %rdi
    xorq    %r9, %rdx
    movq    %r10, 24(%rsp)
    movq    %r11, (%rsp)
    movq    128(%rsp), %rcx
    movq    %rdx, -8(%rsp)
    movq    112(%rsp), %rbx
    movq    112(%rsp), %rdx
    addq    %rcx, 80(%rsp)
    subq    120(%rsp), %rdx
    cmpq    %rbx, 120(%rsp)
    ja  .L314
    cmpl    $21, 108(%rsp)
    movq    %rdx, 112(%rsp)
    jne .L254
.L311:
    movq    80(%rsp), %rdx
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    80(%rsp), %r13
    movq    32(%rdx), %rcx
    movq    (%r10), %r10
    movq    8(%r11), %r11
    movq    16(%r13), %r13
    xorq    %rcx, -96(%rsp)
    movq    80(%rsp), %rcx
    movq    40(%rdx), %rbx
    xorq    %r10, -88(%rsp)
    xorq    %r11, -40(%rsp)
    movq    48(%rdx), %r10
    movq    64(%rdx), %r11
    xorq    %r13, -112(%rsp)
    xorq    24(%rdx), %rbp
    movq    72(%rdx), %r13
    xorq    56(%rdx), %r12
    movq    88(%rcx), %rcx
    movq    80(%rdx), %rdx
    xorq    %rbx, -80(%rsp)
    xorq    %r10, -120(%rsp)
    xorq    %r11, -72(%rsp)
    xorq    %r13, -32(%rsp)
    xorq    %rdx, -104(%rsp)
    xorq    %rcx, -24(%rsp)
    movq    80(%rsp), %rbx
    movq    104(%rbx), %r10
    movq    112(%rbx), %r11
    movq    120(%rbx), %r13
    movq    128(%rbx), %rdx
    movq    136(%rbx), %rcx
    xorq    96(%rbx), %rsi
    xorq    %r10, -48(%rsp)
    xorq    %r11, -64(%rsp)
    xorq    %r13, -16(%rsp)
    xorq    %rdx, -56(%rsp)
    xorq    %rcx, 32(%rsp)
    xorq    144(%rbx), %rax
    xorq    152(%rbx), %r14
    xorq    160(%rbx), %r15
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L256:
    movq    80(%rsp), %rdx
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    80(%rsp), %r13
    movq    32(%rdx), %rcx
    movq    (%r10), %r10
    movq    8(%r11), %r11
    movq    16(%r13), %r13
    xorq    %rcx, -96(%rsp)
    movq    80(%rsp), %rcx
    movq    40(%rdx), %rbx
    xorq    %r10, -88(%rsp)
    xorq    %r11, -40(%rsp)
    movq    48(%rdx), %r10
    movq    64(%rdx), %r11
    xorq    %r13, -112(%rsp)
    xorq    24(%rdx), %rbp
    movq    72(%rdx), %r13
    xorq    56(%rdx), %r12
    movq    88(%rcx), %rcx
    movq    80(%rdx), %rdx
    xorq    %rbx, -80(%rsp)
    xorq    %r10, -120(%rsp)
    xorq    %r11, -72(%rsp)
    xorq    %r13, -32(%rsp)
    xorq    %rdx, -104(%rsp)
    xorq    %rcx, -24(%rsp)
    movq    80(%rsp), %rbx
    movq    104(%rbx), %r10
    movq    112(%rbx), %r11
    movq    120(%rbx), %r13
    xorq    96(%rbx), %rsi
    xorq    %r10, -48(%rsp)
    xorq    %r11, -64(%rsp)
    xorq    %r13, -16(%rsp)
    cmpl    $23, 108(%rsp)
    ja  .L264
    cmpl    $19, 108(%rsp)
    ja  .L265
    cmpl    $17, 108(%rsp)
    ja  .L266
    cmpl    $16, 108(%rsp)
    je  .L255
    movq    128(%rbx), %rdx
    xorq    %rdx, -56(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L257:
    movq    80(%rsp), %rbx
    movq    80(%rsp), %r13
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    (%rbx), %rbx
    movq    32(%r13), %rdx
    movq    8(%r10), %r10
    movq    16(%r11), %r11
    xorq    %rbx, -88(%rsp)
    movq    40(%r13), %rcx
    movq    48(%r13), %rbx
    xorq    %r10, -40(%rsp)
    xorq    %r11, -112(%rsp)
    xorq    24(%r13), %rbp
    xorq    %rdx, -96(%rsp)
    xorq    %rcx, -80(%rsp)
    xorq    %rbx, -120(%rsp)
    xorq    56(%r13), %r12
    cmpl    $11, 108(%rsp)
    ja  .L261
    cmpl    $9, 108(%rsp)
    ja  .L262
    cmpl    $8, 108(%rsp)
    je  .L255
    movq    64(%r13), %r10
    xorq    %r10, -72(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L264:
    movq    %rbx, %rdx
    movq    128(%rbx), %r11
    movq    136(%rbx), %r13
    xorq    144(%rbx), %rax
    xorq    152(%rbx), %r14
    xorq    160(%rbx), %r15
    movq    168(%rbx), %rcx
    movq    184(%rdx), %r10
    movq    176(%rbx), %rbx
    xorq    %r11, -56(%rsp)
    xorq    %r13, 32(%rsp)
    xorq    %rcx, 24(%rsp)
    xorq    %rbx, (%rsp)
    xorq    %r10, -8(%rsp)
    cmpl    $24, 108(%rsp)
    je  .L255
    xorq    192(%rdx), %rdi
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L314:
    movq    %rax, %r9
    movq    136(%rsp), %rax
    movq    %r15, %rcx
    movq    -88(%rsp), %r11
    movq    %r12, %r13
    movq    %rbp, %r8
    movq    32(%rsp), %r15
    movq    %rdi, %r12
    subq    %rbx, %rax
.L253:
    movq    88(%rsp), %rdi
    movq    -40(%rsp), %rbp
    movq    -112(%rsp), %r10
    movq    -80(%rsp), %rdx
    movq    -120(%rsp), %rbx
    movq    %r11, (%rdi)
    movq    %rbp, 8(%rdi)
    movq    -96(%rsp), %r11
    movq    -72(%rsp), %rbp
    movq    %r10, 16(%rdi)
    movq    %rdx, 40(%rdi)
    movq    -32(%rsp), %r10
    movq    -48(%rsp), %rdx
    movq    %rbx, 48(%rdi)
    movq    %r11, 32(%rdi)
    movq    -64(%rsp), %rbx
    movq    -104(%rsp), %r11
    movq    %r13, 56(%rdi)
    movq    %rbp, 64(%rdi)
    movq    -24(%rsp), %r13
    movq    -56(%rsp), %rbp
    movq    %rsi, 96(%rdi)
    movq    -16(%rsp), %rsi
    movq    %r10, 72(%rdi)
    movq    %r11, 80(%rdi)
    movq    %r13, 88(%rdi)
    movq    %rbx, 112(%rdi)
    movq    %rbp, 128(%rdi)
    movq    %r15, 136(%rdi)
    movq    %r8, 24(%rdi)
    movq    %rdx, 104(%rdi)
    movq    %rsi, 120(%rdi)
    movq    %r9, 144(%rdi)
    movq    %r14, 152(%rdi)
    movq    -8(%rsp), %r13
    movq    24(%rsp), %r10
    movq    %rcx, 160(%rdi)
    movq    (%rsp), %r11
    movq    %r12, 192(%rdi)
    movq    %r13, 184(%rdi)
    movq    %r10, 168(%rdi)
    movq    %r11, 176(%rdi)
    addq    $144, %rsp
    .cfi_remember_state
    .cfi_def_cfa_offset 56
    popq    %rbx
    .cfi_def_cfa_offset 48
    popq    %rbp
    .cfi_def_cfa_offset 40
    popq    %r12
    .cfi_def_cfa_offset 32
    popq    %r13
    .cfi_def_cfa_offset 24
    popq    %r14
    .cfi_def_cfa_offset 16
    popq    %r15
    .cfi_def_cfa_offset 8
    ret
    .p2align 4,,10
    .p2align 3
.L258:
    .cfi_restore_state
    movq    80(%rsp), %rdx
    movq    80(%rsp), %rcx
    movq    80(%rsp), %rbx
    movq    80(%rsp), %r10
    movq    (%rdx), %rdx
    movq    8(%rcx), %rcx
    movq    16(%rbx), %rbx
    xorq    24(%r10), %rbp
    xorq    %rdx, -88(%rsp)
    xorq    %rcx, -40(%rsp)
    xorq    %rbx, -112(%rsp)
    cmpl    $5, 108(%rsp)
    ja  .L260
    cmpl    $4, 108(%rsp)
    je  .L255
    movq    32(%r10), %r11
    xorq    %r11, -96(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L265:
    movq    80(%rsp), %r11
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    128(%r11), %r11
    movq    136(%r13), %r13
    xorq    144(%rdx), %rax
    xorq    152(%rdx), %r14
    xorq    %r11, -56(%rsp)
    xorq    %r13, 32(%rsp)
    cmpl    $21, 108(%rsp)
    ja  .L267
    cmpl    $20, 108(%rsp)
    je  .L255
    xorq    160(%rdx), %r15
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L261:
    movq    64(%r13), %rcx
    movq    72(%r13), %rbx
    movq    80(%r13), %r10
    movq    88(%r13), %r11
    xorq    %rcx, -72(%rsp)
    xorq    %rbx, -32(%rsp)
    xorq    %r10, -104(%rsp)
    xorq    %r11, -24(%rsp)
    cmpl    $13, 108(%rsp)
    ja  .L263
    cmpl    $12, 108(%rsp)
    je  .L255
    movq    80(%rsp), %r13
    xorq    96(%r13), %rsi
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L263:
    movq    80(%rsp), %rdx
    movq    104(%rdx), %rcx
    xorq    96(%rdx), %rsi
    xorq    %rcx, -48(%rsp)
    cmpl    $14, 108(%rsp)
    je  .L255
    movq    112(%rdx), %rbx
    xorq    %rbx, -64(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L267:
    movq    168(%rdx), %rbx
    xorq    160(%rdx), %r15
    xorq    %rbx, 24(%rsp)
    cmpl    $22, 108(%rsp)
    je  .L255
    movq    176(%rdx), %r10
    xorq    %r10, (%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L260:
    movq    32(%r10), %r13
    movq    40(%r10), %rdx
    xorq    %r13, -96(%rsp)
    xorq    %rdx, -80(%rsp)
    cmpl    $6, 108(%rsp)
    je  .L255
    movq    80(%rsp), %rcx
    movq    48(%rcx), %rcx
    xorq    %rcx, -120(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L262:
    movq    80(%rsp), %r11
    movq    80(%rsp), %r13
    movq    64(%r11), %r11
    movq    72(%r13), %r13
    xorq    %r11, -72(%rsp)
    xorq    %r13, -32(%rsp)
    cmpl    $10, 108(%rsp)
    je  .L255
    movq    80(%rsp), %rdx
    movq    80(%rdx), %rdx
    xorq    %rdx, -104(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L266:
    movq    80(%rsp), %rcx
    movq    80(%rsp), %rbx
    movq    128(%rcx), %rcx
    movq    136(%rbx), %rbx
    xorq    %rcx, -56(%rsp)
    xorq    %rbx, 32(%rsp)
    cmpl    $18, 108(%rsp)
    je  .L255
    movq    80(%rsp), %r10
    xorq    144(%r10), %rax
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L313:
    movq    80(%rsp), %r13
    movq    16(%r13), %r13
    xorq    %r13, -112(%rsp)
    jmp .L255
    .p2align 4,,10
    .p2align 3
.L312:
    movl    108(%rsp), %edx
    testl   %edx, %edx
    je  .L255
    movq    80(%rsp), %rbx
    movq    (%rbx), %rbx
    xorq    %rbx, -88(%rsp)
    jmp .L255
.L269:
    xorl    %eax, %eax
    jmp .L253
    .cfi_endproc
.LFE39:
    .size   KeccakF1600_FastLoop_Absorb, .-KeccakF1600_FastLoop_Absorb
    .ident  "GCC: (SUSE Linux) 4.7.4"
    .section    .note.GNU-stack,"",@progbits
