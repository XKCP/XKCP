    .file   "KeccakP-1600-opt64.c"
    .text
    .p2align 4,,15
    .globl  KeccakP1600_StaticInitialize
    .type   KeccakP1600_StaticInitialize, @function
KeccakP1600_StaticInitialize:
.LFB22:
    .cfi_startproc
    rep
    ret
    .cfi_endproc
.LFE22:
    .size   KeccakP1600_StaticInitialize, .-KeccakP1600_StaticInitialize
    .p2align 4,,15
    .globl  KeccakP1600_Initialize
    .type   KeccakP1600_Initialize, @function
KeccakP1600_Initialize:
.LFB23:
    .cfi_startproc
    movq    %rdi, %rsi
    movl    $200, %r8d
    testb   $1, %sil
    jne .L28
    testb   $2, %dil
    jne .L29
.L4:
    testb   $4, %dil
    jne .L30
.L5:
    movl    %r8d, %ecx
    xorl    %eax, %eax
    shrl    $3, %ecx
    testb   $4, %r8b
    rep stosq
    je  .L6
    movl    $0, (%rdi)
    addq    $4, %rdi
.L6:
    testb   $2, %r8b
    je  .L7
    movw    $0, (%rdi)
    addq    $2, %rdi
.L7:
    andl    $1, %r8d
    je  .L8
    movb    $0, (%rdi)
.L8:
    movq    $-1, 8(%rsi)
    movq    $-1, 16(%rsi)
    movq    $-1, 64(%rsi)
    movq    $-1, 96(%rsi)
    movq    $-1, 136(%rsi)
    movq    $-1, 160(%rsi)
    ret
    .p2align 4,,10
    .p2align 3
.L28:
    leaq    1(%rsi), %rdi
    movb    $0, (%rsi)
    movb    $-57, %r8b
    testb   $2, %dil
    je  .L4
    .p2align 4,,10
    .p2align 3
.L29:
    movw    $0, (%rdi)
    addq    $2, %rdi
    subl    $2, %r8d
    testb   $4, %dil
    je  .L5
    .p2align 4,,10
    .p2align 3
.L30:
    movl    $0, (%rdi)
    subl    $4, %r8d
    addq    $4, %rdi
    jmp .L5
    .cfi_endproc
.LFE23:
    .size   KeccakP1600_Initialize, .-KeccakP1600_Initialize
    .p2align 4,,15
    .globl  KeccakP1600_AddBytesInLane
    .type   KeccakP1600_AddBytesInLane, @function
KeccakP1600_AddBytesInLane:
.LFB24:
    .cfi_startproc
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movq    %rsp, %rbp
    .cfi_def_cfa_register 6
    movq    %r12, -8(%rbp)
    .cfi_offset 12, -24
    movl    %esi, %r12d
    movq    %rbx, -16(%rbp)
    subq    $16, %rsp
    andq    $-32, %rsp
    subq    $64, %rsp
    .cfi_offset 3, -32
    testl   %r8d, %r8d
    je  .L31
    cmpl    $1, %r8d
    movq    %rdi, %rbx
    movq    %rdx, %rsi
    je  .L39
    leaq    32(%rsp), %rdi
    movl    %r8d, %edx
    movl    %ecx, 24(%rsp)
    movq    $0, 32(%rsp)
    call    memcpy
    movq    32(%rsp), %rax
    movl    24(%rsp), %ecx
.L34:
    sall    $3, %ecx
    salq    %cl, %rax
    xorq    %rax, (%rbx,%r12,8)
.L31:
    movq    -16(%rbp), %rbx
    movq    -8(%rbp), %r12
    leave
    .cfi_remember_state
    .cfi_def_cfa 7, 8
    ret
    .p2align 4,,10
    .p2align 3
.L39:
    .cfi_restore_state
    movzbl  (%rdx), %eax
    jmp .L34
    .cfi_endproc
.LFE24:
    .size   KeccakP1600_AddBytesInLane, .-KeccakP1600_AddBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_AddLanes
    .type   KeccakP1600_AddLanes, @function
KeccakP1600_AddLanes:
.LFB25:
    .cfi_startproc
    cmpl    $7, %edx
    jbe .L48
    movl    $8, %ecx
    xorl    %eax, %eax
    jmp .L42
    .p2align 4,,10
    .p2align 3
.L49:
    movl    %r8d, %ecx
.L42:
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
    jbe .L49
    leal    4(%rcx), %eax
    leal    2(%rcx), %r8d
.L41:
    cmpl    %eax, %edx
    jae .L54
    jmp .L58
    .p2align 4,,10
    .p2align 3
.L51:
    movl    %r8d, %eax
.L54:
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
    jae .L51
    leal    2(%rax), %r8d
.L43:
    cmpl    %r8d, %edx
    jae .L55
    jmp .L59
    .p2align 4,,10
    .p2align 3
.L53:
    movl    %ecx, %r8d
.L55:
    movl    %eax, %ecx
    addl    $1, %eax
    movq    (%rsi,%rcx,8), %r9
    xorq    %r9, (%rdi,%rcx,8)
    movq    (%rsi,%rax,8), %rcx
    xorq    %rcx, (%rdi,%rax,8)
    leal    2(%r8), %ecx
    movl    %r8d, %eax
    cmpl    %ecx, %edx
    jae .L53
.L45:
    cmpl    %r8d, %edx
    jbe .L40
    movq    (%rsi,%r8,8), %rax
    xorq    %rax, (%rdi,%r8,8)
.L40:
    rep
    ret
.L48:
    movl    $2, %r8d
    movl    $4, %eax
    xorl    %ecx, %ecx
    jmp .L41
.L58:
    movl    %ecx, %eax
    jmp .L43
.L59:
    movl    %eax, %r8d
    jmp .L45
    .cfi_endproc
.LFE25:
    .size   KeccakP1600_AddLanes, .-KeccakP1600_AddLanes
    .p2align 4,,15
    .globl  KeccakP1600_AddByte
    .type   KeccakP1600_AddByte, @function
KeccakP1600_AddByte:
.LFB26:
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
.LFE26:
    .size   KeccakP1600_AddByte, .-KeccakP1600_AddByte
    .p2align 4,,15
    .globl  KeccakP1600_AddBytes
    .type   KeccakP1600_AddBytes, @function
KeccakP1600_AddBytes:
.LFB27:
    .cfi_startproc
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movq    %rdi, %r9
    movq    %rsp, %rbp
    .cfi_def_cfa_register 6
    pushq   %r15
    pushq   %r14
    .cfi_offset 15, -24
    .cfi_offset 14, -32
    movq    %rsi, %r14
    pushq   %r13
    pushq   %r12
    pushq   %rbx
    andq    $-32, %rsp
    subq    $64, %rsp
    .cfi_offset 13, -40
    .cfi_offset 12, -48
    .cfi_offset 3, -56
    testl   %edx, %edx
    je  .L76
    movl    %edx, %r15d
    movl    %edx, %r13d
    shrl    $3, %r15d
    andl    $7, %r13d
    testl   %ecx, %ecx
    je  .L61
    movl    %ecx, %ebx
.L70:
    movl    $8, %eax
    movl    %ebx, %r12d
    subl    %r13d, %eax
    cmpl    %ebx, %eax
    cmovbe  %eax, %r12d
    cmpl    $1, %r12d
    jne .L68
    movzbl  (%r14), %edx
    movl    $1, %r8d
.L69:
    leal    0(,%r13,8), %ecx
    movq    %rdx, %r13
    movl    %r15d, %eax
    salq    %cl, %r13
    addl    $1, %r15d
    addq    %r8, %r14
    xorq    %r13, (%r9,%rax,8)
    xorl    %r13d, %r13d
    subl    %r12d, %ebx
    jne .L70
.L61:
    leaq    -40(%rbp), %rsp
    popq    %rbx
    popq    %r12
    popq    %r13
    popq    %r14
    popq    %r15
    popq    %rbp
    .cfi_remember_state
    .cfi_def_cfa 7, 8
    ret
    .p2align 4,,10
    .p2align 3
.L68:
    .cfi_restore_state
    movl    %r12d, %r8d
    movq    %r14, %rsi
    movq    %r9, 16(%rsp)
    leaq    32(%rsp), %rdi
    movq    %r8, %rdx
    movq    %r8, 24(%rsp)
    movq    $0, 32(%rsp)
    call    memcpy
    movq    32(%rsp), %rdx
    movq    16(%rsp), %r9
    movq    24(%rsp), %r8
    jmp .L69
    .p2align 4,,10
    .p2align 3
.L76:
    movl    %ecx, %r12d
    movl    %ecx, 24(%rsp)
    shrl    $3, %r12d
    movq    %r9, 16(%rsp)
    movl    %r12d, %edx
    call    KeccakP1600_AddLanes
    movl    24(%rsp), %ecx
    movq    16(%rsp), %r9
    movl    %ecx, %ebx
    andl    $7, %ebx
    je  .L61
    leal    0(,%r12,8), %esi
    addq    %r14, %rsi
    cmpl    $1, %ebx
    jne .L65
    movzbl  (%rsi), %eax
.L66:
    xorq    %rax, (%r9,%r12,8)
    jmp .L61
.L65:
    leaq    32(%rsp), %rdi
    movl    %ebx, %edx
    movq    %r9, 16(%rsp)
    movq    $0, 32(%rsp)
    call    memcpy
    movq    32(%rsp), %rax
    movq    16(%rsp), %r9
    jmp .L66
    .cfi_endproc
.LFE27:
    .size   KeccakP1600_AddBytes, .-KeccakP1600_AddBytes
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteBytesInLane
    .type   KeccakP1600_OverwriteBytesInLane, @function
KeccakP1600_OverwriteBytesInLane:
.LFB28:
    .cfi_startproc
    leal    -1(%rsi), %eax
    movq    %rdx, %r9
    cmpl    $1, %eax
    jbe .L82
    cmpl    $8, %esi
    jne .L78
.L82:
    testl   %r8d, %r8d
    je  .L91
    leal    (%rcx,%rsi,8), %eax
    movq    %r9, %rdx
    addl    %eax, %r8d
    .p2align 4,,10
    .p2align 3
.L84:
    movzbl  (%rdx), %ecx
    movl    %eax, %esi
    addl    $1, %eax
    addq    $1, %rdx
    cmpl    %r8d, %eax
    notl    %ecx
    movb    %cl, (%rdi,%rsi)
    jne .L84
.L91:
    rep
    ret
    .p2align 4,,10
    .p2align 3
.L78:
    cmpl    $17, %esi
    je  .L82
    cmpl    $12, %esi
    .p2align 4,,5
    je  .L82
    cmpl    $20, %esi
    .p2align 4,,2
    je  .L82
    leal    0(,%rsi,8), %eax
    movl    %ecx, %ecx
    movl    %r8d, %edx
    addq    %rcx, %rax
    movq    %r9, %rsi
    addq    %rax, %rdi
    jmp memcpy
    .cfi_endproc
.LFE28:
    .size   KeccakP1600_OverwriteBytesInLane, .-KeccakP1600_OverwriteBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteLanes
    .type   KeccakP1600_OverwriteLanes, @function
KeccakP1600_OverwriteLanes:
.LFB29:
    .cfi_startproc
    xorl    %eax, %eax
    testl   %edx, %edx
    jne .L99
    jmp .L92
    .p2align 4,,10
    .p2align 3
.L101:
    cmpl    $8, %eax
    je  .L94
    cmpl    $17, %eax
    .p2align 4,,5
    je  .L94
    cmpl    $12, %eax
    .p2align 4,,2
    je  .L94
    cmpl    $20, %eax
    .p2align 4,,2
    je  .L94
    movq    (%rsi,%rax,8), %rcx
    movq    %rcx, (%rdi,%rax,8)
    addq    $1, %rax
    cmpl    %eax, %edx
    jbe .L92
    .p2align 4,,10
    .p2align 3
.L99:
    leal    -1(%rax), %r8d
    cmpl    $1, %r8d
    ja  .L101
.L94:
    movq    (%rsi,%rax,8), %rcx
    notq    %rcx
    movq    %rcx, (%rdi,%rax,8)
    addq    $1, %rax
    cmpl    %eax, %edx
    ja  .L99
.L92:
    rep
    ret
    .cfi_endproc
.LFE29:
    .size   KeccakP1600_OverwriteLanes, .-KeccakP1600_OverwriteLanes
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteBytes
    .type   KeccakP1600_OverwriteBytes, @function
KeccakP1600_OverwriteBytes:
.LFB30:
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
    je  .L135
    movl    %edx, %r14d
    movl    %edx, %eax
    shrl    $3, %r14d
    andl    $7, %eax
    testl   %ecx, %ecx
    je  .L102
    leal    0(,%r14,8), %r15d
    movl    %ecx, %r13d
    movq    %rsi, %rbp
    movl    $8, %ecx
    jmp .L121
    .p2align 4,,10
    .p2align 3
.L136:
    cmpl    $8, %r14d
    je  .L118
    cmpl    $17, %r14d
    je  .L118
    cmpl    $12, %r14d
    je  .L118
    cmpl    $20, %r14d
    je  .L118
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
.L120:
    addl    $1, %r14d
    addq    %r8, %rbp
    addl    $8, %r15d
    xorl    %eax, %eax
    subl    %ebx, %r13d
    je  .L102
.L121:
    leal    -1(%r14), %edx
    movl    %ecx, %ebx
    subl    %eax, %ebx
    cmpl    %r13d, %ebx
    cmova   %r13d, %ebx
    cmpl    $1, %edx
    ja  .L136
.L118:
    leal    (%rax,%r15), %esi
    xorl    %eax, %eax
    .p2align 4,,10
    .p2align 3
.L117:
    movzbl  0(%rbp,%rax), %edx
    leal    (%rsi,%rax), %r8d
    addq    $1, %rax
    cmpl    %eax, %ebx
    notl    %edx
    movb    %dl, (%r12,%r8)
    ja  .L117
    movl    %ebx, %r8d
    jmp .L120
.L139:
    leal    0(,%r8,8), %eax
    movl    %ecx, %r13d
    leal    -1(%r8), %edi
    movl    %eax, %ecx
    andl    $7, %r13d
    addq    %rcx, %rsi
    cmpl    $1, %edi
    movq    %rcx, %rdx
    ja  .L137
.L109:
    testl   %r13d, %r13d
    movq    %rsi, %rbp
    leal    0(%r13,%rax), %esi
    jne .L130
    jmp .L102
    .p2align 4,,10
    .p2align 3
.L138:
    movl    %eax, %ecx
.L130:
    movzbl  0(%rbp), %edx
    addl    $1, %eax
    addq    $1, %rbp
    cmpl    %esi, %eax
    notl    %edx
    movb    %dl, (%r12,%rcx)
    jne .L138
    .p2align 4,,10
    .p2align 3
.L102:
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
.L135:
    .cfi_restore_state
    movl    %ecx, %r8d
    shrl    $3, %r8d
    testl   %r8d, %r8d
    je  .L104
    xorl    %eax, %eax
    jmp .L108
    .p2align 4,,10
    .p2align 3
.L140:
    cmpl    $8, %eax
    je  .L105
    cmpl    $17, %eax
    .p2align 4,,3
    je  .L105
    cmpl    $12, %eax
    .p2align 4,,2
    je  .L105
    cmpl    $20, %eax
    .p2align 4,,2
    je  .L105
    movq    (%rsi,%rax,8), %rdx
    movq    %rdx, (%r12,%rax,8)
    .p2align 4,,10
    .p2align 3
.L107:
    addq    $1, %rax
    cmpl    %eax, %r8d
    jbe .L139
.L108:
    leal    -1(%rax), %edi
    cmpl    $1, %edi
    ja  .L140
.L105:
    movq    (%rsi,%rax,8), %rdx
    notq    %rdx
    movq    %rdx, (%r12,%rax,8)
    jmp .L107
.L137:
    cmpl    $8, %r8d
    je  .L109
    cmpl    $17, %r8d
    je  .L109
    cmpl    $12, %r8d
    je  .L109
    cmpl    $20, %r8d
    je  .L109
.L110:
    leaq    (%r12,%rdx), %rdi
    movl    %r13d, %edx
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
    jmp memcpy
.L104:
    .cfi_restore_state
    movl    %ecx, %r13d
    xorl    %edx, %edx
    andl    $7, %r13d
    jmp .L110
    .cfi_endproc
.LFE30:
    .size   KeccakP1600_OverwriteBytes, .-KeccakP1600_OverwriteBytes
    .p2align 4,,15
    .globl  KeccakP1600_OverwriteWithZeroes
    .type   KeccakP1600_OverwriteWithZeroes, @function
KeccakP1600_OverwriteWithZeroes:
.LFB31:
    .cfi_startproc
    movl    %esi, %r8d
    xorl    %eax, %eax
    movq    %rdi, %rdx
    shrl    $3, %r8d
    testl   %r8d, %r8d
    jne .L152
    jmp .L149
    .p2align 4,,10
    .p2align 3
.L155:
    cmpl    $8, %eax
    je  .L145
    cmpl    $17, %eax
    .p2align 4,,5
    je  .L145
    cmpl    $12, %eax
    .p2align 4,,2
    je  .L145
    cmpl    $20, %eax
    .p2align 4,,2
    je  .L145
    addl    $1, %eax
    movq    $0, (%rdx)
    addq    $8, %rdx
    cmpl    %r8d, %eax
    je  .L149
    .p2align 4,,10
    .p2align 3
.L152:
    leal    -1(%rax), %ecx
    cmpl    $1, %ecx
    ja  .L155
.L145:
    addl    $1, %eax
    movq    $-1, (%rdx)
    addq    $8, %rdx
    cmpl    %r8d, %eax
    jne .L152
.L149:
    andl    $7, %esi
    je  .L156
    leal    -1(%r8), %eax
    cmpl    $1, %eax
    jbe .L150
    cmpl    $8, %r8d
    je  .L150
    cmpl    $17, %r8d
    je  .L150
    cmpl    $12, %r8d
    je  .L150
    cmpl    $20, %r8d
    je  .L150
    sall    $3, %r8d
    movl    %esi, %edx
    xorl    %esi, %esi
    addq    %r8, %rdi
    jmp memset
    .p2align 4,,10
    .p2align 3
.L150:
    sall    $3, %r8d
    movl    %esi, %edx
    movl    $255, %esi
    addq    %r8, %rdi
    jmp memset
    .p2align 4,,10
    .p2align 3
.L156:
    rep
    ret
    .cfi_endproc
.LFE31:
    .size   KeccakP1600_OverwriteWithZeroes, .-KeccakP1600_OverwriteWithZeroes
    .p2align 4,,15
    .globl  KeccakP1600_Permute_24rounds
    .type   KeccakP1600_Permute_24rounds, @function
KeccakP1600_Permute_24rounds:
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
    subq    $232, %rsp
    .cfi_def_cfa_offset 288
    movq    %rdi, 208(%rsp)
    movq    48(%rdi), %r13
    movq    (%rdi), %r11
    movq    8(%rdi), %rax
    movq    16(%rdi), %rcx
    movq    24(%rdi), %r8
    movq    32(%rdi), %rbx
    movq    40(%rdi), %rsi
    movq    %rax, 16(%rsp)
    movq    56(%rdi), %rdi
    movq    %rcx, 8(%rsp)
    movq    %rbx, (%rsp)
    movq    %rsi, -8(%rsp)
    movq    %rdi, -16(%rsp)
    movq    208(%rsp), %rdi
    movq    64(%rdi), %rdi
    movq    %rdi, -24(%rsp)
    movq    208(%rsp), %rdi
    movq    88(%rdi), %r9
    movq    80(%rdi), %rbp
    movq    104(%rdi), %r10
    movq    112(%rdi), %r12
    movq    72(%rdi), %r15
    movq    %r9, -40(%rsp)
    movq    96(%rdi), %rbx
    movq    %rbp, -32(%rsp)
    movq    %r10, -48(%rsp)
    movq    %r12, -56(%rsp)
    movq    120(%rdi), %r14
    movq    128(%rdi), %rax
    movq    136(%rdi), %rcx
    movq    144(%rdi), %r10
    movq    152(%rdi), %rsi
    movq    %r14, -64(%rsp)
    movq    160(%rdi), %rdi
    movq    %rax, -72(%rsp)
    movq    (%rsp), %r12
    movq    %rcx, -80(%rsp)
    movq    %rsi, -88(%rsp)
    movq    -8(%rsp), %rsi
    movq    %rdi, -96(%rsp)
    movq    208(%rsp), %rdi
    xorq    %r15, %r12
    xorq    -56(%rsp), %r12
    xorq    %r11, %rsi
    movq    168(%rdi), %rdi
    xorq    -32(%rsp), %rsi
    xorq    -88(%rsp), %r12
    movq    %rdi, -104(%rsp)
    movq    208(%rsp), %rdi
    xorq    %r14, %rsi
    movq    16(%rsp), %r14
    xorq    -96(%rsp), %rsi
    movq    176(%rdi), %rdi
    xorq    %r13, %r14
    xorq    %r9, %r14
    movq    -16(%rsp), %r9
    xorq    %rax, %r14
    movq    %rdi, -112(%rsp)
    movq    208(%rsp), %rdi
    xorq    -104(%rsp), %r14
    xorq    8(%rsp), %r9
    movq    184(%rdi), %rdi
    xorq    %rbx, %r9
    xorq    %rcx, %r9
    movq    %r14, %rcx
    movq    %rdi, -120(%rsp)
    movq    208(%rsp), %rdi
    xorq    -112(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    192(%rdi), %rbp
    movq    -24(%rsp), %rdi
    movq    %r9, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rax
    xorq    %rbp, %r12
    xorq    %r8, %rdi
    xorq    %r12, %rcx
    xorq    -48(%rsp), %rdi
    xorq    %rcx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %r9, %r12
    movq    %rax, %r9
    xorq    %r10, %rdi
    xorq    %r13, %r9
    xorq    -120(%rsp), %rdi
    xorq    %rdi, %rsi
    movq    %rdi, %rdx
    movq    %rsi, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r14, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdx, %rbx
    xorq    %rbp, %rdi
    xorq    %r12, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    movq    %rbx, %r13
    xorq    %r12, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %r9, %rbp
    movq    %rdi, %r14
    notq    %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    $1, %rbp
    orq %r10, %r13
    andq    %r10, %r14
    xorq    %r11, %rbp
    xorq    %r9, %r13
    xorq    %rbx, %r14
    movq    %r11, %rbx
    andq    %r9, %r11
    movq    %r13, 24(%rsp)
    xorq    %rdi, %r11
    orq %rdi, %rbx
    movq    -112(%rsp), %rdi
    xorq    %r10, %rbx
    movq    %r11, 80(%rsp)
    movq    -72(%rsp), %r10
    movq    -32(%rsp), %r11
    movq    %rsi, %r9
    movq    %r14, 48(%rsp)
    xorq    %r15, %r9
    movq    %rbx, 40(%rsp)
    xorq    %rdx, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r15
    xorq    %rcx, %r11
    xorq    %rax, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    movq    %r10, %r14
    notq    %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r11, %r14
    orq %r9, %r13
    orq %r10, %r15
    movq    %rdi, %rbx
    xorq    %r9, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    xorq    %r11, %r15
    orq %r8, %rbx
    xorq    %r10, %rbx
    andq    %r8, %r9
    movq    -88(%rsp), %r10
    xorq    %rdi, %r9
    movq    -48(%rsp), %r11
    movq    %r14, 112(%rsp)
    movq    %r9, 104(%rsp)
    movq    -16(%rsp), %r9
    movq    16(%rsp), %rdi
    movq    %r13, 56(%rsp)
    xorq    %rsi, %r10
    movq    -96(%rsp), %r8
    movq    %rbx, 32(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r14
    xorq    %r12, %r11
    xorq    %rdx, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %r14
    notq    %r10
    movq    %r11, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r14
    xorq    %rax, %rdi
    xorq    %rcx, %r8
    movq    %r14, 72(%rsp)
    movq    %r10, %r14
    orq %r9, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %rdi, %r9
    andq    %r8, %r14
    movq    %r8, %rbx
    xorq    %r11, %r14
    xorq    %r8, %r9
    orq %rdi, %rbx
    movq    -40(%rsp), %r11
    xorq    %rdi, %r13
    xorq    %r10, %rbx
    movq    %r9, 168(%rsp)
    movq    (%rsp), %r8
    movq    -8(%rsp), %r9
    movq    %r13, 64(%rsp)
    movq    -80(%rsp), %r10
    movq    %rbx, 128(%rsp)
    movq    -120(%rsp), %rdi
    xorq    %rax, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    xorq    %rsi, %r8
    xorq    %rcx, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdx, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r12, %rdi
    andq    %r9, %r13
    movq    %r10, %rbx
    xorq    %r8, %r13
    notq    %r10
    orq %r11, %rbx
    movq    %r13, 136(%rsp)
    movq    %r10, %r13
    xorq    %r9, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r13
    orq %r8, %r9
    xorq    -24(%rsp), %r12
    xorq    %r11, %r13
    xorq    %rdi, %r9
    xorq    -64(%rsp), %rcx
    movq    %r13, 152(%rsp)
    movq    %rdi, %r13
    movq    64(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %rbx, 88(%rsp)
    andq    %r8, %r13
    movq    %rcx, %rbx
    xorq    56(%rsp), %rdi
    movq    %r12, %r8
    xorq    %r10, %r13
    xorq    -56(%rsp), %rsi
    notq    %r8
    movq    %r9, 160(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbx
    xorq    8(%rsp), %rdx
    movq    %r8, %r11
    xorq    %r8, %rbx
    xorq    -104(%rsp), %rax
    andq    %rsi, %r11
    xorq    136(%rsp), %rdi
    movq    %rbx, 184(%rsp)
    movq    72(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rdx, %r11
    xorq    %rbp, %rdi
    movq    168(%rsp), %r9
    xorq    112(%rsp), %r10
    xorq    %r11, %rdi
    movq    48(%rsp), %r8
    xorq    88(%rsp), %r10
    xorq    %rbx, %r10
    movq    %rax, %rbx
    xorq    24(%rsp), %r10
    andq    %rcx, %rbx
    andq    %rdx, %r12
    xorq    %rsi, %rbx
    movq    %rax, %rsi
    xorq    104(%rsp), %r9
    orq %rdx, %rsi
    xorq    %rbx, %r8
    xorq    %rcx, %rsi
    xorq    %rax, %r12
    xorq    %r15, %r8
    movq    %rsi, 176(%rsp)
    xorq    %r14, %r8
    movq    %r10, %rdx
    movq    128(%rsp), %rsi
    xorq    160(%rsp), %r9
    xorq    152(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    32(%rsp), %rsi
    xorq    %r12, %r9
    xorq    80(%rsp), %r9
    movq    %r8, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %rdi, %rax
    xorq    176(%rsp), %rsi
    xorq    %r9, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    %rdx, %r8
    xorq    40(%rsp), %rsi
    xorq    %rbp, %r8
    movq    %rsi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rdi
    movq    112(%rsp), %rsi
    xorq    %r10, %rcx
    xorq    %rcx, %r14
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %rbp
    movq    %r14, %r10
    xorq    %r9, %r13
    orq %rsi, %rbp
    notq    %r10
    xorq    %rdi, %r12
    xorq    $32898, %rbp
    xorq    %rcx, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    orq %r13, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rsi, %r10
    movq    %rbp, 112(%rsp)
    movq    %r12, %rbp
    movq    %r10, 120(%rsp)
    andq    %r13, %rbp
    movq    %r8, %r10
    xorq    %r14, %rbp
    orq %r12, %r10
    andq    %rsi, %r8
    xorq    %r13, %r10
    movq    %rbp, 192(%rsp)
    movq    88(%rsp), %rbp
    xorq    %r12, %r8
    movq    %r10, 200(%rsp)
    movq    64(%rsp), %r10
    movq    %r8, 96(%rsp)
    movq    104(%rsp), %r8
    movq    40(%rsp), %rsi
    xorq    %rax, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rdx, %r10
    xorq    %rdi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r10, %r13
    movq    %r10, %r12
    xorq    %r9, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    orq %r8, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r13, 104(%rsp)
    movq    %rbx, %r13
    movq    %rbx, %r14
    notq    %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rbp, %r13
    orq %rsi, %r14
    xorq    %rsi, %r12
    xorq    %r10, %r13
    xorq    %rbp, %r14
    andq    %rsi, %r8
    xorq    %rbx, %r8
    movq    128(%rsp), %r10
    movq    %r12, 40(%rsp)
    movq    160(%rsp), %rbx
    movq    %r8, 88(%rsp)
    movq    %rcx, %r8
    movq    24(%rsp), %rsi
    xorq    %r15, %r8
    movq    %r14, 64(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r9, %r10
    xorq    %rdx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rdi, %rbx
    movq    %r10, %r15
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    orq %r8, %r15
    andq    %r10, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    xorq    %rsi, %r15
    andq    %rsi, %r8
    movq    %r15, 128(%rsp)
    movq    %rbp, 160(%rsp)
    movq    %rbx, %rbp
    movq    176(%rsp), %r15
    notq    %rbp
    movq    %rbp, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %rbx
    xorq    %r11, %r8
    xorq    %r9, %r15
    xorq    %r10, %rbx
    movq    %r11, %r10
    movq    %r8, 216(%rsp)
    orq %rsi, %r10
    movq    56(%rsp), %r8
    xorq    %rbp, %r10
    movq    80(%rsp), %rsi
    movq    %r10, 144(%rsp)
    movq    72(%rsp), %r10
    movq    152(%rsp), %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %rdi, %rsi
    xorq    %rdx, %r8
    xorq    %rax, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r11
    xorq    %rcx, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r11
    movq    %rbp, %r12
    xorq    32(%rsp), %r9
    xorq    %rsi, %r11
    notq    %r12
    xorq    168(%rsp), %rdi
    movq    %r11, 80(%rsp)
    movq    %rbp, %r11
    movq    %r15, %rbp
    orq %r10, %r11
    andq    %rsi, %rbp
    movq    %r12, %r14
    xorq    %r8, %r11
    orq %rsi, %r8
    movq    128(%rsp), %rsi
    xorq    %r15, %r8
    orq %r15, %r14
    xorq    48(%rsp), %rcx
    movq    %r8, 72(%rsp)
    xorq    136(%rsp), %rdx
    xorq    %r10, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    40(%rsp), %rsi
    movq    %r9, %r8
    movq    %r14, 56(%rsp)
    notq    %r8
    xorq    184(%rsp), %rax
    xorq    %r12, %rbp
    movq    %r8, %r15
    movq    192(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    80(%rsp), %rsi
    andq    %rdi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    movq    %r15, 32(%rsp)
    xorq    %r15, %rsi
    movq    160(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    xorq    112(%rsp), %rsi
    orq %rdi, %r10
    xorq    104(%rsp), %r15
    xorq    %r8, %r10
    movq    %r10, 168(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    xorq    %r11, %r15
    andq    %rdx, %r14
    xorq    %r10, %r15
    movq    144(%rsp), %r10
    xorq    120(%rsp), %r15
    xorq    %rdi, %r14
    movq    %rax, %rdi
    orq %rcx, %rdi
    xorq    %r14, %r12
    xorq    %rdx, %rdi
    xorq    %r13, %r12
    andq    %rcx, %r9
    xorq    64(%rsp), %r10
    movq    %rdi, 136(%rsp)
    xorq    %rbx, %r12
    xorq    56(%rsp), %r12
    xorq    %rax, %r9
    movq    %r15, %rdx
    movq    104(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rbp, %r10
    xorq    %rdi, %r10
    movq    216(%rsp), %rdi
    movq    %r12, %rax
    xorq    200(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rax
    xorq    88(%rsp), %rdi
    xorq    %rax, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r10, %r8
    xorq    %r10, %rsi
    movq    112(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    72(%rsp), %rdi
    xorq    %r15, %r8
    xorq    %r8, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %rdi
    movq    %rbx, %r15
    xorq    96(%rsp), %rdi
    notq    %r15
    xorq    %rdi, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r12, %rdi
    movq    %rbx, %r12
    xorq    %rdx, %r10
    xorq    %rdi, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rcx, %r12
    orq %rbp, %r15
    xorq    %rsi, %r9
    movq    %r12, 24(%rsp)
    xorq    %rcx, %r15
    xorq    %r8, %r14
    movabsq $-9223372036854742902, %r12
    movq    %r15, 112(%rsp)
    movq    %r10, %r15
    xorq    %r12, 24(%rsp)
    xorq    %rax, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    orq %r9, %r15
    andq    %rbp, %r12
    xorq    %rbp, %r15
    xorq    %r10, 24(%rsp)
    andq    %rcx, %r10
    xorq    %rbx, %r12
    xorq    %r9, %r10
    movq    %r12, 104(%rsp)
    movq    88(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r10, 184(%rsp)
    movq    128(%rsp), %r10
    movq    %r14, %r12
    movq    200(%rsp), %rcx
    notq    %r12
    movq    %r15, 152(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r11, %r12
    movq    %r11, %rbp
    xorq    %rsi, %r9
    xorq    %rdx, %r10
    movq    %r14, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r12
    andq    %r10, %rbp
    movq    %r10, %rbx
    movq    144(%rsp), %r10
    xorq    %rdi, %rcx
    movq    %r12, 48(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    xorq    %r9, %rbp
    movq    56(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbx
    orq %rcx, %r15
    andq    %rcx, %r9
    movq    120(%rsp), %rcx
    xorq    %r11, %r15
    xorq    %r14, %r9
    movq    72(%rsp), %r11
    movq    %rbx, 128(%rsp)
    xorq    %rdi, %r10
    movq    %r9, 200(%rsp)
    movq    %r8, %r9
    xorq    %r8, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %r13, %r9
    xorq    %rax, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    movq    32(%rsp), %r13
    xorq    %rsi, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbx
    movq    %rbp, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %rbx, 32(%rsp)
    movq    %r11, %rbx
    movq    %r11, %rbp
    notq    %rbx
    xorq    %rdx, %r13
    andq    %r10, %rbp
    movq    %rbx, %r11
    xorq    %r9, %rbp
    andq    %rcx, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r13, %r11
    xorq    %r13, %r9
    movq    %rbp, 72(%rsp)
    xorq    %r10, %r11
    movq    %r13, %r10
    movq    40(%rsp), %rbp
    orq %rcx, %r10
    movq    %r9, 224(%rsp)
    movq    136(%rsp), %rcx
    xorq    %rbx, %r10
    movq    96(%rsp), %r9
    movq    %r15, 176(%rsp)
    movq    160(%rsp), %rbx
    movq    %r10, 144(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rdx, %rbp
    movq    %r12, %r10
    xorq    %rdi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rax, %rbx
    xorq    %rsi, %r9
    notq    %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    movq    %r12, %r14
    andq    %rbp, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %r13
    orq %rbx, %r10
    orq %rcx, %r14
    xorq    %rbp, %r10
    orq %r9, %rbp
    movq    %r13, 40(%rsp)
    xorq    %rcx, %rbp
    xorq    %rbx, %r14
    xorq    64(%rsp), %rdi
    movq    %rbp, 136(%rsp)
    movq    32(%rsp), %rbp
    movq    %rcx, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %rcx
    xorq    216(%rsp), %rsi
    movq    %r14, 56(%rsp)
    notq    %rcx
    xorq    80(%rsp), %rdx
    andq    %r9, %rbx
    xorq    128(%rsp), %rbp
    movq    %rcx, %r13
    xorq    %r12, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %rsi, %r13
    movq    %rdx, %r15
    xorq    192(%rsp), %r8
    xorq    40(%rsp), %rbp
    orq %rsi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    xorq    %rcx, %r15
    xorq    168(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %rbp
    movq    %rax, %r14
    movq    %rax, %rcx
    xorq    24(%rsp), %rbp
    movq    %r15, 80(%rsp)
    andq    %rdx, %r14
    movq    72(%rsp), %r15
    xorq    %rsi, %r14
    movq    104(%rsp), %rsi
    movq    224(%rsp), %r12
    movq    144(%rsp), %r9
    xorq    88(%rsp), %r15
    xorq    %r14, %rsi
    xorq    %r10, %r15
    xorq    80(%rsp), %r15
    xorq    112(%rsp), %r15
    xorq    48(%rsp), %rsi
    orq %r8, %rcx
    andq    %r8, %rdi
    xorq    200(%rsp), %r12
    xorq    %rdx, %rcx
    xorq    %rax, %rdi
    xorq    176(%rsp), %r9
    movq    %rcx, 64(%rsp)
    movq    88(%rsp), %r8
    movq    %r15, %rdx
    xorq    %r11, %rsi
    xorq    136(%rsp), %r12
    xorq    %rbx, %r9
    xorq    56(%rsp), %rsi
    xorq    %rcx, %r9
    xorq    152(%rsp), %r9
    xorq    %rdi, %r12
    xorq    184(%rsp), %r12
    movq    %rsi, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r9, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r12, %rdx
    xorq    %r15, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
    movq    %rbp, %rsi
    xorq    %rcx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %rsi
    movq    24(%rsp), %r9
    xorq    %r12, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rbp, %rax
    movq    %r11, %rbp
    xorq    %rsi, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rdx, %r9
    xorq    %rax, %r8
    movq    %r11, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %r8, %rbp
    notq    %r15
    xorq    %rax, %r10
    movq    %rbp, 24(%rsp)
    orq %rbx, %r15
    xorq    %rcx, %r14
    movabsq $-9223372034707259392, %rbp
    xorq    %r8, %r15
    xorq    %rbp, 24(%rsp)
    movq    %rdi, %rbp
    movq    %r15, 120(%rsp)
    andq    %rbx, %rbp
    xorq    %r11, %rbp
    movq    %r9, %r11
    orq %rdi, %r11
    movq    %rbp, 168(%rsp)
    xorq    %r9, 24(%rsp)
    andq    %r8, %r9
    xorq    %rbx, %r11
    xorq    %rdi, %r9
    movq    32(%rsp), %r8
    movq    %r11, 88(%rsp)
    movq    152(%rsp), %rdi
    movq    %r9, 160(%rsp)
    movq    200(%rsp), %r9
    xorq    %rdx, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r15
    xorq    %rsi, %r9
    xorq    %r12, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r15, 32(%rsp)
    movq    %r10, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %r15
    andq    %r8, %rbx
    movq    %r14, %rbp
    notq    %r15
    xorq    %r9, %rbx
    orq %rdi, %rbp
    orq %r10, %r15
    andq    %rdi, %r9
    movq    112(%rsp), %rdi
    xorq    %r8, %r15
    movq    136(%rsp), %r8
    xorq    %r10, %rbp
    xorq    %r14, %r9
    movq    144(%rsp), %r10
    movq    %rbx, 152(%rsp)
    movq    %r9, 200(%rsp)
    movq    48(%rsp), %r9
    xorq    %rax, %rdi
    movq    %rbp, 192(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r8
    xorq    %r12, %r10
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r14
    xorq    %rcx, %r9
    notq    %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r10, %r14
    movq    %r10, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r14
    orq %r9, %r11
    andq    %rdi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rdi, %r11
    xorq    %r13, %r9
    movq    %r13, %rbx
    movq    %r11, 48(%rsp)
    orq %rdi, %rbx
    movq    56(%rsp), %r11
    movq    %r14, 136(%rsp)
    movq    %r8, %r14
    xorq    %r8, %rbx
    movq    %r9, 144(%rsp)
    movq    72(%rsp), %r9
    andq    %r13, %r14
    movq    128(%rsp), %r8
    xorq    %r10, %r14
    movq    %rbx, 96(%rsp)
    movq    184(%rsp), %r10
    xorq    %rcx, %r11
    movq    64(%rsp), %rdi
    xorq    %rax, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rbp
    xorq    %rdx, %r8
    xorq    %rsi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %rbp
    xorq    %r12, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %rbp, 56(%rsp)
    movq    %r11, %rbp
    notq    %r11
    orq %r9, %rbp
    movq    %r11, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    orq %rdi, %r13
    orq %r10, %r8
    xorq    %r9, %r13
    xorq    %rdi, %r8
    xorq    40(%rsp), %rdx
    movq    %r13, 64(%rsp)
    movq    %rdi, %r13
    movq    48(%rsp), %rdi
    andq    %r10, %r13
    movq    136(%rsp), %r10
    movq    %r8, 72(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    176(%rsp), %r12
    movq    %rdx, %rbx
    xorq    %r11, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    152(%rsp), %r10
    movq    %r12, %r8
    xorq    32(%rsp), %rdi
    notq    %r8
    xorq    224(%rsp), %rsi
    movq    %r8, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbx
    xorq    %rbp, %r10
    xorq    80(%rsp), %rax
    xorq    %r8, %rbx
    movq    168(%rsp), %r8
    andq    %rsi, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rbx, 40(%rsp)
    xorq    %rbx, %r10
    movq    %rax, %rbx
    xorq    56(%rsp), %rdi
    andq    %rdx, %rbx
    xorq    %rsi, %rbx
    xorq    104(%rsp), %rcx
    movq    %rax, %rsi
    xorq    %rbx, %r8
    xorq    120(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r15, %r8
    xorq    %rcx, %r11
    movq    144(%rsp), %r9
    xorq    %r11, %rdi
    xorq    %r14, %r8
    xorq    24(%rsp), %rdi
    xorq    64(%rsp), %r8
    orq %rcx, %rsi
    andq    %rcx, %r12
    xorq    %rdx, %rsi
    xorq    200(%rsp), %r9
    xorq    %rax, %r12
    movq    %rsi, 80(%rsp)
    movq    96(%rsp), %rsi
    movq    %r10, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    xorq    72(%rsp), %r9
    xorq    192(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rdi, %rax
    xorq    %r12, %r9
    xorq    %r13, %rsi
    xorq    160(%rsp), %r9
    xorq    80(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r9, %rdx
    xorq    88(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    24(%rsp), %r8
    xorq    %r9, %r13
    xorq    %rsi, %rdi
    movq    %rsi, %rcx
    movq    152(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r10, %rcx
    xorq    %rdx, %r8
    xorq    %rdi, %r12
    xorq    %rcx, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %r10
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    $32907, %r10
    xorq    %rax, %rbp
    xorq    %rcx, %rbx
    xorq    %r8, %r10
    movq    %r10, 24(%rsp)
    movq    %r14, %r10
    notq    %r10
    orq %r13, %r10
    xorq    %rsi, %r10
    movq    %r10, 112(%rsp)
    movq    %r12, %r10
    andq    %r13, %r10
    xorq    %r14, %r10
    movq    %r8, %r14
    andq    %rsi, %r8
    xorq    %r12, %r8
    movq    %r10, 104(%rsp)
    movq    48(%rsp), %r10
    movq    %r8, 152(%rsp)
    orq %r12, %r14
    movq    88(%rsp), %rsi
    movq    200(%rsp), %r8
    xorq    %r13, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rdx, %r10
    movq    %r14, 128(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r10, %r13
    xorq    %rdi, %r8
    movq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    xorq    %r9, %rsi
    orq %r8, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r13, 48(%rsp)
    movq    %rbx, %r13
    movq    %rbx, %r14
    notq    %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rbp, %r13
    orq %rsi, %r14
    xorq    %rsi, %r12
    xorq    %r10, %r13
    xorq    %rbp, %r14
    movq    72(%rsp), %r10
    movq    96(%rsp), %rbp
    andq    %rsi, %r8
    movq    %r12, 88(%rsp)
    movq    120(%rsp), %rsi
    xorq    %rbx, %r8
    movq    %r14, 184(%rsp)
    movq    %r8, 176(%rsp)
    movq    %rcx, %r8
    xorq    %r15, %r8
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r9, %rbp
    xorq    %rdi, %r10
    xorq    %rdx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbp,%rbp
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    notq    %r10
    movq    %rbp, %r15
    andq    %rbp, %rbx
    orq %r8, %r15
    xorq    %r8, %rbx
    andq    %rsi, %r8
    xorq    %rsi, %r15
    movq    %rbx, 200(%rsp)
    movq    %r10, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %rbx
    xorq    %r11, %r8
    movq    %r15, 72(%rsp)
    xorq    %rbp, %rbx
    movq    %r11, %rbp
    movq    %r8, 216(%rsp)
    orq %rsi, %rbp
    movq    32(%rsp), %rsi
    xorq    %r10, %rbp
    movq    136(%rsp), %r10
    movq    %rbp, 96(%rsp)
    movq    64(%rsp), %rbp
    movq    160(%rsp), %r8
    movq    80(%rsp), %r15
    xorq    %rdx, %rsi
    xorq    %rax, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r11
    xorq    %rcx, %rbp
    xorq    %rdi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
#NO_APP
    andq    %rsi, %r11
    movq    %rbp, %r12
    xorq    %r9, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
#NO_APP
    notq    %r12
    xorq    %r8, %r11
    movq    %r11, 80(%rsp)
    movq    %r12, %r14
    movq    %rbp, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r15,%r15
# 0 "" 2
#NO_APP
    orq %r10, %r11
    orq %r15, %r14
    movq    %r15, %rbp
    xorq    %rsi, %r11
    xorq    %r10, %r14
    andq    %r8, %rbp
    orq %r8, %rsi
    xorq    192(%rsp), %r9
    movq    %r14, 32(%rsp)
    xorq    %r15, %rsi
    xorq    144(%rsp), %rdi
    xorq    %r12, %rbp
    movq    %rsi, 64(%rsp)
    movq    72(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r8
    xorq    56(%rsp), %rdx
    notq    %r8
    xorq    168(%rsp), %rcx
    xorq    88(%rsp), %rsi
    movq    %r8, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %rdi, %r15
    movq    %rdx, %r10
    xorq    40(%rsp), %rax
    xorq    80(%rsp), %rsi
    orq %rdi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    xorq    %r8, %r10
    movq    %r15, 40(%rsp)
    xorq    %r15, %rsi
    xorq    24(%rsp), %rsi
    movq    %r10, 56(%rsp)
    movq    200(%rsp), %r15
    movq    104(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    movq    96(%rsp), %r8
    xorq    48(%rsp), %r15
    andq    %rdx, %r14
    xorq    %rdi, %r14
    movq    %rax, %rdi
    xorq    %r14, %r12
    orq %rcx, %rdi
    xorq    %r13, %r12
    xorq    %r11, %r15
    xorq    %rbx, %r12
    xorq    %r10, %r15
    xorq    32(%rsp), %r12
    xorq    112(%rsp), %r15
    xorq    %rdx, %rdi
    andq    %rcx, %r9
    movq    %rdi, 168(%rsp)
    xorq    %rax, %r9
    movq    48(%rsp), %rcx
    xorq    184(%rsp), %r8
    movq    %r12, %rax
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rbp, %r8
    xorq    %rsi, %rax
    xorq    %rdi, %r8
    movq    216(%rsp), %rdi
    xorq    %rax, %rcx
    xorq    128(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    176(%rsp), %rdi
    movq    %r8, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r15, %r10
    xorq    64(%rsp), %rdi
    xorq    %r10, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rsi
    movq    24(%rsp), %r8
    movq    %rbx, %r15
    xorq    %r9, %rdi
    notq    %r15
    xorq    %rsi, %r9
    xorq    152(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdi, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r12, %rdi
    movq    %rbx, %r12
    xorq    %rdx, %r8
    orq %rcx, %r12
    xorq    %rdi, %rbp
    movq    %r12, 24(%rsp)
    movl    $2147483649, %r12d
    xorq    %r12, 24(%rsp)
    movq    %r9, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %r8, 24(%rsp)
    orq %rbp, %r15
    andq    %rbp, %r12
    xorq    %rcx, %r15
    xorq    %rax, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r15, 120(%rsp)
    movq    %r8, %r15
    andq    %rcx, %r8
    xorq    %r9, %r8
    orq %r9, %r15
    movq    72(%rsp), %r9
    movq    128(%rsp), %rcx
    xorq    %rbp, %r15
    movq    %r11, %rbp
    movq    %r8, 192(%rsp)
    xorq    %rbx, %r12
    xorq    %r10, %r14
    movq    176(%rsp), %r8
    movq    %r12, 136(%rsp)
    xorq    %rdx, %r9
    movq    %r15, 160(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %rbp
    movq    %r9, %rbx
    xorq    %rdi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %r8
    movq    %r14, %r12
    movq    %r14, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    orq %r8, %rbx
    andq    %rcx, %r8
    xorq    %r14, %r8
    orq %rcx, %r15
    notq    %r12
    xorq    %rcx, %rbx
    xorq    %r11, %r15
    movq    %rbp, 128(%rsp)
    orq %r11, %r12
    movq    %r8, 144(%rsp)
    movq    112(%rsp), %rcx
    movq    96(%rsp), %rbp
    movq    %r10, %r11
    xorq    %r9, %r12
    movq    64(%rsp), %r8
    xorq    %r13, %r11
    movq    %rbx, 72(%rsp)
    movq    40(%rsp), %r13
    movq    %r12, 48(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rax, %rcx
    xorq    %rdi, %rbp
    movq    %r15, 176(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rsi, %r8
    xorq    %rdx, %r13
    movq    %rbp, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r9
    orq %r11, %rbx
    notq    %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %rbp, %r9
    movq    %r13, %r12
    xorq    %rcx, %rbx
    xorq    %r11, %r9
    orq %rcx, %r12
    andq    %rcx, %r11
    xorq    %r8, %r12
    xorq    %r13, %r11
    movq    %rbx, 40(%rsp)
    movq    %r9, 64(%rsp)
    movq    152(%rsp), %r9
    movq    %r8, %rbx
    movq    %r12, 96(%rsp)
    movq    32(%rsp), %r12
    andq    %r13, %rbx
    movq    %r11, 224(%rsp)
    movq    88(%rsp), %r11
    xorq    %rbp, %rbx
    movq    168(%rsp), %rcx
    movq    200(%rsp), %rbp
    xorq    %rsi, %r9
    xorq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdx, %r11
    xorq    %rdi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r8
    notq    %r12
    xorq    %rax, %rbp
    movq    %r12, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rbp, %r14
    orq %rbp, %r8
    movq    %rbp, %r13
    movq    %rcx, %rbp
    andq    %r11, %r13
    xorq    %r11, %r8
    andq    %r9, %rbp
    xorq    %r9, %r13
    movq    %r14, 168(%rsp)
    xorq    %r12, %rbp
    orq %r9, %r11
    xorq    80(%rsp), %rdx
    xorq    %rcx, %r11
    xorq    184(%rsp), %rdi
    movq    %r13, 32(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdx, %r15
    movq    %rdi, %rcx
    xorq    216(%rsp), %rsi
    notq    %rcx
    movq    40(%rsp), %r12
    movq    %r11, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r15
    xorq    56(%rsp), %rax
    movq    %rcx, %r13
    xorq    %rcx, %r15
    xorq    104(%rsp), %r10
    andq    %rsi, %r13
    movq    %r15, 80(%rsp)
    movq    64(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    movq    %rax, %rcx
    xorq    72(%rsp), %r12
    andq    %rdx, %r14
    movq    224(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rsi, %r14
    orq %r10, %rcx
    movq    136(%rsp), %rsi
    xorq    128(%rsp), %r15
    xorq    %rdx, %rcx
    xorq    %r10, %r13
    movq    %rcx, 56(%rsp)
    movq    96(%rsp), %rcx
    xorq    32(%rsp), %r12
    xorq    %r14, %rsi
    xorq    48(%rsp), %rsi
    xorq    %r8, %r15
    xorq    80(%rsp), %r15
    xorq    176(%rsp), %rcx
    xorq    %r13, %r12
    xorq    24(%rsp), %r12
    xorq    %rbx, %rsi
    xorq    120(%rsp), %r15
    xorq    168(%rsp), %rsi
    xorq    %rbp, %rcx
    andq    %r10, %rdi
    xorq    144(%rsp), %r9
    xorq    %rax, %rdi
    xorq    56(%rsp), %rcx
    movq    %r15, %rdx
    movq    24(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r11, %r9
    movq    %rsi, %rax
    xorq    %rdi, %r9
    xorq    160(%rsp), %rcx
    xorq    192(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r12, %rax
    movq    %rcx, %r11
    xorq    %r9, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
    movq    %r12, %rsi
    xorq    %rdx, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rsi
    movq    128(%rsp), %rcx
    xorq    %r9, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r15, %r11
    xorq    %rsi, %rdi
    xorq    %r11, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r12
    xorq    %rax, %rcx
    movq    %rbx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r12
    notq    %r15
    movq    %r12, 24(%rsp)
    movabsq $-9223372034707259263, %r12
    xorq    %r12, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rbp, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r10, 24(%rsp)
    xorq    %rcx, %r15
    movq    %rdi, %r12
    andq    %rbp, %r12
    movq    %r15, 112(%rsp)
    movq    %r10, %r15
    andq    %rcx, %r10
    orq %rdi, %r15
    movq    144(%rsp), %rcx
    xorq    %rdi, %r10
    movq    160(%rsp), %rdi
    xorq    %rbx, %r12
    movq    %r10, 152(%rsp)
    movq    40(%rsp), %r10
    xorq    %rbp, %r15
    movq    %r12, 104(%rsp)
    xorq    %rax, %r8
    xorq    %r11, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r8, %rbp
    movq    %r14, %r12
    xorq    %rdx, %r10
    notq    %r12
    movq    %r15, 128(%rsp)
    xorq    %r9, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r10, %rbp
    movq    %r10, %rbx
    xorq    %rsi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %r8, %r12
    xorq    %rcx, %rbp
    orq %rcx, %rbx
    movq    %r14, %r15
    andq    %rdi, %rcx
    xorq    %r10, %r12
    xorq    %r14, %rcx
    movq    96(%rsp), %r10
    orq %rdi, %r15
    xorq    %rdi, %rbx
    xorq    %r8, %r15
    movq    48(%rsp), %rdi
    movq    88(%rsp), %r8
    movq    %rcx, 144(%rsp)
    movq    120(%rsp), %rcx
    movq    %rbx, 160(%rsp)
    xorq    %r9, %r10
    movq    %rbp, 184(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %r11, %rdi
    xorq    %rsi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rax, %rcx
    movq    %r12, 40(%rsp)
    movq    168(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    orq %rdi, %rbx
    movq    %r8, %rbp
    notq    %r8
    xorq    %rcx, %rbx
    xorq    %rdx, %r13
    andq    %r10, %rbp
    movq    %rbx, 48(%rsp)
    movq    %r8, %rbx
    xorq    %rdi, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r13, %rbx
    movq    %rbp, 88(%rsp)
    andq    %rcx, %rdi
    xorq    %r10, %rbx
    movq    %r13, %r10
    movq    64(%rsp), %rbp
    orq %rcx, %r10
    xorq    %r13, %rdi
    movq    192(%rsp), %rcx
    xorq    %r8, %r10
    movq    72(%rsp), %r8
    movq    %rdi, 216(%rsp)
    movq    56(%rsp), %rdi
    movq    %r10, 96(%rsp)
    xorq    %r11, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r10
    xorq    %rax, %rbp
    xorq    %rsi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rbp, %r10
    movq    %rbp, %r13
    xorq    %rdx, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    andq    %r8, %r13
    orq %rcx, %r8
    xorq    %r9, %rdi
    notq    %r12
    xorq    %rcx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    %r12, %r14
    movq    %r13, 56(%rsp)
    movq    %r8, 72(%rsp)
    movq    48(%rsp), %r8
    orq %rdi, %r14
    xorq    %rbp, %r14
    movq    %rdi, %rbp
    movq    %r15, 200(%rsp)
    andq    %rcx, %rbp
    movq    %r14, 64(%rsp)
    xorq    %r12, %rbp
    xorq    136(%rsp), %r11
    xorq    160(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    176(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rcx
    xorq    56(%rsp), %r8
    notq    %rcx
    xorq    224(%rsp), %rsi
    movq    %rcx, %r13
    xorq    32(%rsp), %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %rsi, %r13
    movq    %rdx, %r15
    xorq    80(%rsp), %rax
    xorq    %r11, %r13
    orq %rsi, %r15
    xorq    %rcx, %r15
    xorq    %r13, %r8
    xorq    24(%rsp), %r8
    movq    %r15, 80(%rsp)
    movq    88(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    movq    96(%rsp), %rdi
    movq    %rax, %rcx
    andq    %rdx, %r14
    movq    216(%rsp), %r12
    orq %r11, %rcx
    xorq    %rsi, %r14
    movq    104(%rsp), %rsi
    xorq    %rdx, %rcx
    xorq    184(%rsp), %r15
    movq    %rcx, 32(%rsp)
    xorq    200(%rsp), %rdi
    xorq    %r14, %rsi
    xorq    40(%rsp), %rsi
    xorq    %r10, %r15
    xorq    80(%rsp), %r15
    xorq    %rbp, %rdi
    xorq    %rcx, %rdi
    xorq    %rbx, %rsi
    xorq    112(%rsp), %r15
    xorq    64(%rsp), %rsi
    xorq    128(%rsp), %rdi
    andq    %r11, %r9
    xorq    144(%rsp), %r12
    xorq    %rax, %r9
    movq    %r15, %rdx
    movq    %rsi, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    72(%rsp), %r12
    movq    %rdi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r15, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %r12
    xorq    %r8, %rax
    xorq    %rcx, %rbx
    xorq    152(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r11
    movq    %rbx, %r15
    notq    %r15
    xorq    %r12, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
    movq    %r8, %rsi
    movq    24(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdi, %rsi
    movq    184(%rsp), %rdi
    xorq    %r12, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rbp, %r15
    xorq    %rdx, %r8
    xorq    %rsi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rax, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r11
    xorq    %rdi, %r15
    movq    %r11, 24(%rsp)
    movabsq $-9223372036854743031, %r11
    xorq    %r11, 24(%rsp)
    movq    %r9, %r11
    movq    %r15, 120(%rsp)
    andq    %rbp, %r11
    movq    %r8, %r15
    xorq    %rbx, %r11
    movq    %r11, 168(%rsp)
    xorq    %r8, 24(%rsp)
    orq %r9, %r15
    andq    %rdi, %r8
    xorq    %r9, %r8
    xorq    %rbp, %r15
    movq    128(%rsp), %r9
    xorq    %rcx, %r14
    movq    144(%rsp), %rdi
    xorq    %rax, %r10
    movq    %r8, 184(%rsp)
    movq    48(%rsp), %r8
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r15, 136(%rsp)
    movq    %r14, %r15
    xorq    %r12, %r9
    notq    %r15
    xorq    %rsi, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r15
    movq    %r10, %rbp
    xorq    %rdx, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r15
    andq    %r8, %rbp
    movq    %r8, %rbx
    movq    %r14, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r10, %r8
    xorq    %rdi, %rbp
    orq %rdi, %rbx
    andq    %r9, %rdi
    movq    %r8, 176(%rsp)
    movq    72(%rsp), %r8
    xorq    %r14, %rdi
    xorq    %r9, %rbx
    movq    96(%rsp), %r10
    movq    %rdi, 192(%rsp)
    movq    40(%rsp), %r9
    movq    112(%rsp), %rdi
    movq    %rbx, 48(%rsp)
    xorq    %rsi, %r8
    movq    %rbp, 128(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r14
    xorq    %rcx, %r9
    xorq    %r12, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rax, %rdi
    movq    %r10, %r11
    notq    %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r10, %r14
    orq %r9, %r11
    movq    %r13, %rbx
    xorq    %r9, %r14
    andq    %rdi, %r9
    xorq    %rdi, %r11
    xorq    %r13, %r9
    movq    %r11, 40(%rsp)
    orq %rdi, %rbx
    movq    %r14, 72(%rsp)
    movq    %r8, %r14
    xorq    %r8, %rbx
    movq    %r9, 144(%rsp)
    movq    88(%rsp), %r9
    andq    %r13, %r14
    movq    160(%rsp), %r8
    xorq    %r10, %r14
    movq    %rbx, 96(%rsp)
    movq    152(%rsp), %r10
    movq    64(%rsp), %r11
    xorq    %rax, %r9
    movq    32(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rbp
    xorq    %rdx, %r8
    xorq    %rsi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %rbp
    xorq    %rcx, %r11
    xorq    %r12, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
    xorq    104(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %rbp, 32(%rsp)
    movq    %r11, %rbp
    notq    %r11
    movq    %r11, %r13
    orq %r9, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r13
    xorq    %r8, %rbp
    orq %r10, %r8
    xorq    %r9, %r13
    xorq    %rdi, %r8
    movq    144(%rsp), %r9
    movq    %r13, 64(%rsp)
    movq    %rdi, %r13
    movq    40(%rsp), %rdi
    andq    %r10, %r13
    movq    72(%rsp), %r10
    movq    %r8, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r11, %r13
    xorq    200(%rsp), %r12
    xorq    56(%rsp), %rdx
    xorq    128(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %rdx, %rbx
    movq    %r12, %r8
    xorq    216(%rsp), %rsi
    notq    %r8
    xorq    %rbp, %r10
    xorq    80(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbx
    movq    %r8, %r11
    xorq    48(%rsp), %rdi
    xorq    %r8, %rbx
    andq    %rsi, %r11
    movq    168(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rbx, 80(%rsp)
    xorq    %rbx, %r10
    movq    %rax, %rbx
    andq    %rdx, %rbx
    xorq    32(%rsp), %rdi
    xorq    %rcx, %r11
    xorq    %rsi, %rbx
    movq    %rax, %rsi
    xorq    120(%rsp), %r10
    orq %rcx, %rsi
    xorq    %rbx, %r8
    xorq    %rdx, %rsi
    xorq    %r15, %r8
    andq    %rcx, %r12
    movq    %rsi, 56(%rsp)
    movq    96(%rsp), %rsi
    xorq    %r11, %rdi
    xorq    %r14, %r8
    xorq    24(%rsp), %rdi
    movq    %r10, %rdx
    xorq    64(%rsp), %r8
    xorq    176(%rsp), %rsi
    xorq    %r13, %rsi
    xorq    56(%rsp), %rsi
    xorq    136(%rsp), %rsi
    xorq    %rax, %r12
    movq    %r8, %rax
    xorq    192(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rdi, %rax
    movq    %rsi, %rcx
    xorq    88(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rdi
    movq    128(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r12, %r9
    xorq    %rdi, %r12
    xorq    184(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r10, %rcx
    xorq    %rax, %rsi
    xorq    %rcx, %r14
    xorq    %r9, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    24(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %r10
    xorq    %r9, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r10
    xorq    %rdx, %r8
    xorb    $-118, %r10b
    xorq    %r8, %r10
    movq    %r10, 24(%rsp)
    movq    %r14, %r10
    notq    %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    orq %r13, %r10
    xorq    %rsi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r10, 112(%rsp)
    movq    %r12, %r10
    andq    %r13, %r10
    xorq    %r14, %r10
    movq    %r8, %r14
    orq %r12, %r14
    movq    %r10, 104(%rsp)
    movq    40(%rsp), %r10
    xorq    %r13, %r14
    andq    %rsi, %r8
    movq    136(%rsp), %rsi
    movq    %r14, 128(%rsp)
    xorq    %rcx, %rbx
    xorq    %r12, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r14
    movq    %r8, 152(%rsp)
    xorq    %rax, %rbp
    notq    %r14
    movq    192(%rsp), %r8
    xorq    %rdx, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rbp, %r14
    movq    %rbp, %r13
    xorq    %r9, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r14
    andq    %r10, %r13
    movq    %r10, %r12
    movq    %rbx, %r10
    xorq    %rdi, %r8
    xorq    %rdx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r10
    xorq    %rbp, %r10
    movq    96(%rsp), %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r10, 160(%rsp)
    orq %r8, %r12
    movq    88(%rsp), %r10
    xorq    %rsi, %r12
    xorq    %r8, %r13
    andq    %rsi, %r8
    movq    120(%rsp), %rsi
    xorq    %rbx, %r8
    movq    %r12, 40(%rsp)
    movq    %r8, 192(%rsp)
    xorq    %r9, %rbp
    movq    %rcx, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r12
    movq    %r13, 136(%rsp)
    xorq    %r15, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r10
    orq %r8, %r12
    movq    %r11, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %rax, %rsi
    notq    %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
    andq    %rbp, %r13
    orq %rsi, %r15
    xorq    %r8, %r13
    xorq    %r10, %r15
    andq    %rsi, %r8
    movq    %r10, %rbx
    movq    72(%rsp), %r10
    xorq    %r11, %r8
    movq    48(%rsp), %rsi
    andq    %r11, %rbx
    movq    %r8, 216(%rsp)
    movq    184(%rsp), %r8
    xorq    %rbp, %rbx
    movq    %r15, 96(%rsp)
    movq    64(%rsp), %rbp
    movq    %r12, 88(%rsp)
    movq    56(%rsp), %r15
    xorq    %rax, %r10
    movq    %r13, 200(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r11
    xorq    %rdx, %rsi
    xorq    %rdi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %rsi, %r11
    xorq    %rcx, %rbp
    xorq    %r9, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r11
    xorq    168(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r11, 48(%rsp)
    movq    %rbp, %r11
    movq    %rbp, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r15,%r15
# 0 "" 2
#NO_APP
    orq %r10, %r11
    movq    %r15, %rbp
    notq    %r12
    xorq    %rsi, %r11
    andq    %r8, %rbp
    orq %r8, %rsi
    movq    88(%rsp), %r8
    movq    %r12, %r13
    xorq    %r12, %rbp
    orq %r15, %r13
    xorq    %r15, %rsi
    xorq    176(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r10, %r13
    xorq    144(%rsp), %rdi
    movq    %rsi, 64(%rsp)
    xorq    40(%rsp), %r8
    movq    %r9, %rsi
    movq    %r13, 56(%rsp)
    notq    %rsi
    xorq    32(%rsp), %rdx
    movq    %rsi, %r15
    xorq    80(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    48(%rsp), %r8
    andq    %rdi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    movq    %rdx, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %r15, 80(%rsp)
    xorq    %r15, %r8
    orq %rdi, %r10
    movq    %rax, %r15
    xorq    %rsi, %r10
    xorq    24(%rsp), %r8
    andq    %rdx, %r15
    movq    %r10, 32(%rsp)
    movq    200(%rsp), %r13
    xorq    %rdi, %r15
    movq    96(%rsp), %rdi
    movq    %rax, %r12
    orq %rcx, %r12
    andq    %r9, %rcx
    xorq    %rdx, %r12
    xorq    %rax, %rcx
    xorq    136(%rsp), %r13
    movq    %r12, 72(%rsp)
    xorq    160(%rsp), %rdi
    xorq    %r11, %r13
    xorq    %rbp, %rdi
    xorq    %r10, %r13
    movq    104(%rsp), %r10
    xorq    %r12, %rdi
    movq    216(%rsp), %r12
    xorq    128(%rsp), %rdi
    xorq    112(%rsp), %r13
    xorq    %r15, %r10
    xorq    192(%rsp), %r12
    xorq    %r14, %r10
    xorq    %rbx, %r10
    movq    %rdi, %rsi
    xorq    56(%rsp), %r10
    movq    %r13, %rdx
    xorq    64(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r10, %rax
    xorq    %r13, %rsi
    xorq    %rcx, %r12
    xorq    %rsi, %rbx
    xorq    152(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
    xorq    %r12, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    136(%rsp), %rdi
    xorq    %r10, %r12
    movq    24(%rsp), %r10
    xorq    %r8, %rcx
    xorq    %r12, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    movq    %rbx, %r9
    xorq    %rax, %rdi
    notq    %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r13
    xorq    %rdx, %r10
    xorb    $-120, %r13b
    xorq    %r10, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r13, 24(%rsp)
    movq    %rcx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    andq    %rbp, %r13
    orq %rbp, %r9
    xorq    %rbx, %r13
    movq    %r10, %rbx
    andq    %rdi, %r10
    xorq    %rdi, %r9
    xorq    %rcx, %r10
    orq %rcx, %rbx
    movq    128(%rsp), %rcx
    movq    %r9, 120(%rsp)
    xorq    %rbp, %rbx
    movq    88(%rsp), %r9
    movq    %r10, 184(%rsp)
    movq    192(%rsp), %rdi
    movq    %r13, 168(%rsp)
    movq    %rbx, 136(%rsp)
    xorq    %r12, %rcx
    xorq    %rax, %r11
    xorq    %rsi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r10
    xorq    %rdx, %r9
    xorq    %r8, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    andq    %r9, %r10
    movq    %r9, %rbp
    movq    %r15, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    notq    %r13
    xorq    %rdi, %r10
    orq %rdi, %rbp
    andq    %rcx, %rdi
    orq %r11, %r13
    movq    %r10, 88(%rsp)
    xorq    %r15, %rdi
    xorq    %r9, %r13
    movq    96(%rsp), %r10
    movq    %rdi, 192(%rsp)
    movq    %rsi, %r9
    movq    64(%rsp), %rdi
    xorq    %r14, %r9
    movq    %r15, %rbx
    movq    80(%rsp), %r14
    xorq    %rcx, %rbp
    orq %rcx, %rbx
    movq    112(%rsp), %rcx
    xorq    %r11, %rbx
    movq    %rbp, 128(%rsp)
    xorq    %r12, %r10
    xorq    %r8, %rdi
    movq    %rbx, 176(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    movq    %rdi, %r11
    xorq    %rdx, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r9, %rbp
    andq    %r10, %r11
    movq    %r14, %r15
    xorq    %rax, %rcx
    notq    %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbp
    xorq    %r9, %r11
    orq %rcx, %r15
    xorq    %rdi, %r15
    andq    %rcx, %r9
    movq    %rbp, 80(%rsp)
    xorq    %r14, %r9
    movq    %r11, 64(%rsp)
    movq    %rdi, %r11
    movq    %r15, 96(%rsp)
    movq    200(%rsp), %rbx
    andq    %r14, %r11
    movq    %r9, 144(%rsp)
    movq    40(%rsp), %r9
    xorq    %r10, %r11
    movq    152(%rsp), %rdi
    movq    56(%rsp), %rbp
    xorq    %rax, %rbx
    movq    72(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r10
    xorq    %rdx, %r9
    xorq    %r8, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r10
    xorq    %rsi, %rbp
    xorq    %r12, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r10
    xorq    104(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r10, 40(%rsp)
    movq    %rbp, %r10
    notq    %rbp
    orq %rbx, %r10
    movq    %rbp, %r14
    xorq    160(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %r10
    orq %rcx, %r14
    orq %rdi, %r9
    xorq    %rbx, %r14
    xorq    %rcx, %r9
    movq    %rcx, %rbx
    movq    80(%rsp), %rcx
    andq    %rdi, %rbx
    movq    %r9, 72(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rbp, %rbx
    xorq    216(%rsp), %r8
    movq    %r14, 56(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    48(%rsp), %rdx
    movq    %r12, %rdi
    andq    %rsi, %r12
    xorq    128(%rsp), %rcx
    notq    %rdi
    movq    %rdi, %r15
    xorq    32(%rsp), %rax
    andq    %r8, %r15
    movq    96(%rsp), %r9
    xorq    %rsi, %r15
    xorq    40(%rsp), %rcx
    movq    %r15, 32(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %rbp
    xorq    176(%rsp), %r9
    orq %r8, %rbp
    xorq    %r15, %rcx
    movq    64(%rsp), %r15
    xorq    %rdi, %rbp
    movq    144(%rsp), %rdi
    movq    %rbp, 104(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    xorq    %rbx, %r9
    xorq    %rax, %r12
    xorq    88(%rsp), %r15
    andq    %rdx, %r14
    xorq    192(%rsp), %rdi
    xorq    %r8, %r14
    movq    %rax, %r8
    orq %rsi, %r8
    xorq    24(%rsp), %rcx
    xorq    %rdx, %r8
    movq    88(%rsp), %rsi
    xorq    %r10, %r15
    xorq    %r8, %r9
    movq    %r8, 152(%rsp)
    xorq    %rbp, %r15
    movq    168(%rsp), %rbp
    xorq    72(%rsp), %rdi
    xorq    120(%rsp), %r15
    xorq    136(%rsp), %r9
    xorq    %r14, %rbp
    xorq    %r13, %rbp
    xorq    %r12, %rdi
    xorq    %r11, %rbp
    movq    %r15, %rdx
    xorq    56(%rsp), %rbp
    movq    %r9, %r8
    xorq    184(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r15, %r8
    xorq    %r8, %r11
    movq    %rbp, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %r11, %r15
    xorq    %rcx, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %rcx
    movq    24(%rsp), %r9
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdi, %rdx
    orq %rsi, %r15
    xorq    %rcx, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rbp, %rdi
    movl    $2147516425, %ebp
    xorq    %rdx, %r9
    xorq    %rbp, %r15
    movq    %r11, %rbp
    xorq    %rdi, %rbx
    notq    %rbp
    xorq    %r9, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    movq    %r15, 24(%rsp)
    xorq    %rsi, %rbp
    movq    %rbp, 112(%rsp)
    movq    %r9, %rbp
    andq    %rsi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %r12, %r9
    movq    %r12, %r15
    movq    192(%rsp), %rsi
    andq    %rbx, %r15
    movq    %r9, 200(%rsp)
    orq %r12, %rbp
    movq    136(%rsp), %r9
    xorq    %r11, %r15
    xorq    %rbx, %rbp
    movq    80(%rsp), %r11
    movq    %r15, 88(%rsp)
    movq    %rbp, 160(%rsp)
    xorq    %rdi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rcx, %rsi
    xorq    %rax, %r10
    xorq    %rdx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r15
    xorq    %r8, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %r15
    movq    %r11, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r14, %rbx
    xorq    %rsi, %r15
    orq %rsi, %r12
    andq    %r9, %rsi
    movq    %r14, %rbp
    notq    %rbx
    xorq    %r14, %rsi
    orq %r10, %rbx
    orq %r9, %rbp
    movq    %rsi, 216(%rsp)
    movq    72(%rsp), %rsi
    xorq    %r11, %rbx
    xorq    %r10, %rbp
    movq    96(%rsp), %r11
    movq    %r8, %r10
    xorq    %r13, %r10
    movq    32(%rsp), %r13
    xorq    %r9, %r12
    movq    120(%rsp), %r9
    movq    %r12, 80(%rsp)
    xorq    %rcx, %rsi
    movq    %rbx, 48(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %r14
    notq    %rsi
    xorq    %rdi, %r11
    movq    %rsi, %rbx
    xorq    %rdx, %r13
    movq    %rbp, 192(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r12
    andq    %r11, %r14
    movq    64(%rsp), %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r13, %rbx
    orq %r10, %r12
    xorq    %r10, %r14
    xorq    %rax, %r9
    movq    %r15, 136(%rsp)
    movq    %r13, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r12
    xorq    %r11, %rbx
    andq    %r9, %r10
    xorq    %r13, %r10
    movq    128(%rsp), %r11
    movq    %r12, 32(%rsp)
    movq    %r10, 224(%rsp)
    movq    56(%rsp), %r12
    orq %r9, %r15
    movq    184(%rsp), %r10
    xorq    %rax, %rbp
    xorq    %rsi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r9
    xorq    %rdx, %r11
    movq    152(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %r9
    xorq    %rcx, %r10
    xorq    %r8, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r9
    xorq    %rdi, %rsi
    xorq    168(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r9, 56(%rsp)
    movq    %r12, %r9
    notq    %r12
    movq    %r12, %r13
    orq %rbp, %r9
    xorq    176(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r13
    xorq    %r11, %r9
    orq %r10, %r11
    xorq    %rbp, %r13
    movq    %rsi, %rbp
    xorq    %rsi, %r11
    andq    %r10, %rbp
    xorq    144(%rsp), %rcx
    movq    %r13, 64(%rsp)
    xorq    %r12, %rbp
    movq    32(%rsp), %r12
    movq    %r14, 72(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    40(%rsp), %rdx
    movq    %rdi, %rsi
    movq    %r15, 96(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    104(%rsp), %rax
    notq    %rsi
    movq    %rdx, %r14
    xorq    80(%rsp), %r12
    movq    %rsi, %r13
    orq %rcx, %r14
    andq    %rcx, %r13
    xorq    %rsi, %r14
    movq    %r11, 128(%rsp)
    xorq    %r8, %r13
    andq    %r8, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    56(%rsp), %r12
    movq    %rax, %rsi
    xorq    %rax, %rdi
    orq %r8, %rsi
    xorq    %rdx, %rsi
    xorq    %r13, %r12
    xorq    24(%rsp), %r12
    movq    %r14, 104(%rsp)
    movq    72(%rsp), %r15
    movq    %rsi, 168(%rsp)
    movq    96(%rsp), %rsi
    movq    224(%rsp), %r10
    movq    24(%rsp), %r8
    xorq    136(%rsp), %r15
    xorq    192(%rsp), %rsi
    xorq    216(%rsp), %r10
    xorq    %r9, %r15
    xorq    %r14, %r15
    movq    %rax, %r14
    xorq    %rbp, %rsi
    andq    %rdx, %r14
    xorq    168(%rsp), %rsi
    xorq    %r11, %r10
    xorq    %rcx, %r14
    movq    88(%rsp), %rcx
    xorq    %rdi, %r10
    xorq    112(%rsp), %r15
    xorq    200(%rsp), %r10
    xorq    160(%rsp), %rsi
    xorq    %r14, %rcx
    xorq    48(%rsp), %rcx
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rsi, %r11
    xorq    %rbx, %rcx
    xorq    64(%rsp), %rcx
    xorq    %r10, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r15, %r11
    xorq    %rdx, %r8
    xorq    %rcx, %r10
    movq    %rcx, %rax
    movq    %r12, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    136(%rsp), %rsi
    xorq    %r11, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r12, %rax
    movq    %rbx, %r12
    movq    %rbx, %r15
    xorq    %rax, %rsi
    notq    %r15
    xorq    %r10, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r12
    xorq    %rcx, %rdi
    movq    %r12, 24(%rsp)
    movl    $2147483658, %r12d
    xorq    %r12, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rbp, %r15
    xorq    %rsi, %r15
    xorq    %r8, 24(%rsp)
    movq    %r15, 120(%rsp)
    movq    %r8, %r15
    andq    %rsi, %r8
    movq    216(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    orq %rdi, %r15
    movq    %rdi, %r12
    movq    %r8, 184(%rsp)
    movq    32(%rsp), %rdi
    andq    %rbp, %r12
    movq    160(%rsp), %r8
    xorq    %rbx, %r12
    xorq    %rbp, %r15
    movq    %r12, 136(%rsp)
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r15, 152(%rsp)
    xorq    %r10, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdx, %rdi
    xorq    %r11, %r14
    xorq    %rax, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %r12
    movq    %r14, %r15
    movq    %rdi, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r9,%r9
# 0 "" 2
#NO_APP
    notq    %r12
    movq    %r9, %rbp
    orq %r8, %r15
    andq    %rdi, %rbp
    orq %r9, %r12
    xorq    %r9, %r15
    xorq    %rsi, %rbp
    xorq    %rdi, %r12
    orq %rsi, %rbx
    movq    96(%rsp), %r9
    andq    %r8, %rsi
    xorq    %r8, %rbx
    movq    128(%rsp), %rdi
    xorq    %r14, %rsi
    movq    %rbx, 32(%rsp)
    movq    48(%rsp), %r8
    movq    %rsi, 144(%rsp)
    xorq    %rdx, %r13
    movq    112(%rsp), %rsi
    movq    %rbp, 160(%rsp)
    xorq    %r10, %r9
    movq    %r12, 40(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rcx, %rdi
    movq    %r9, %rbx
    xorq    %r11, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %rbp
    xorq    %rax, %rsi
    notq    %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r9, %rbp
    orq %r8, %rbx
    movq    %r15, 176(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    xorq    %rsi, %rbx
    movq    %rbx, 48(%rsp)
    movq    72(%rsp), %rbx
    movq    %rbp, 128(%rsp)
    movq    %rdi, %rbp
    movq    64(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r13, %rbp
    xorq    %r9, %rbp
    movq    %r13, %r9
    orq %rsi, %r9
    xorq    %rdi, %r9
    andq    %rsi, %r8
    movq    200(%rsp), %rsi
    movq    %r9, 96(%rsp)
    movq    80(%rsp), %r9
    xorq    %r13, %r8
    movq    168(%rsp), %rdi
    xorq    %rax, %rbx
    xorq    %r11, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    movq    %r8, 216(%rsp)
    xorq    %rcx, %rsi
    xorq    %rdx, %r9
    xorq    56(%rsp), %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    xorq    %r10, %rdi
    xorq    192(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rsi, %r13
    movq    %r12, %r8
    xorq    104(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %r13, 80(%rsp)
    orq %rbx, %r8
    movq    %r12, %r13
    movq    %rdi, %r12
    xorq    %r9, %r8
    notq    %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %rsi, %r12
    orq %rsi, %r9
    movq    %r10, %rsi
    notq    %rsi
    movq    %r13, %r14
    xorq    %r13, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    orq %rdi, %r14
    movq    %rsi, %r13
    movq    %rdx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rbx, %r14
    xorq    %rdi, %r9
    xorq    88(%rsp), %r11
    xorq    224(%rsp), %rcx
    movq    %r9, 72(%rsp)
    movq    %rax, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %rcx, %r13
    orq %rcx, %r15
    movq    48(%rsp), %r9
    xorq    %rsi, %r15
    movq    216(%rsp), %rdi
    movq    %r14, 64(%rsp)
    movq    %r15, 56(%rsp)
    movq    128(%rsp), %r15
    movq    %rax, %r14
    andq    %rdx, %r14
    movq    96(%rsp), %rsi
    orq %r11, %rbx
    xorq    %rcx, %r14
    movq    136(%rsp), %rcx
    andq    %r11, %r10
    xorq    32(%rsp), %r9
    xorq    %rdx, %rbx
    xorq    %r11, %r13
    xorq    160(%rsp), %r15
    xorq    %rax, %r10
    movq    %rbx, 104(%rsp)
    xorq    144(%rsp), %rdi
    xorq    176(%rsp), %rsi
    xorq    %r14, %rcx
    xorq    80(%rsp), %r9
    xorq    40(%rsp), %rcx
    xorq    %r8, %r15
    xorq    72(%rsp), %rdi
    xorq    56(%rsp), %r15
    xorq    %r12, %rsi
    xorq    %r13, %r9
    xorq    %rbx, %rsi
    xorq    %rbp, %rcx
    xorq    152(%rsp), %rsi
    xorq    %r10, %rdi
    xorq    64(%rsp), %rcx
    xorq    120(%rsp), %r15
    xorq    24(%rsp), %r9
    xorq    184(%rsp), %rdi
    movq    %rsi, %rbx
    movq    %rcx, %rax
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rdi, %rdx
    xorq    %r9, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rdi
    movq    %r9, %rcx
    movq    24(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    160(%rsp), %rsi
    xorq    %rdi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r15, %rbx
    xorq    %rdx, %r9
    xorq    %rcx, %r10
    xorq    %rbx, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r11
    xorq    %rax, %rsi
    movq    %rbp, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r11
    notq    %r15
    movq    %r11, 24(%rsp)
    movl    $2147516555, %r11d
    xorq    %r11, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r15
    xorq    %rsi, %r15
    xorq    %r9, 24(%rsp)
    movq    %r15, 112(%rsp)
    movq    %r9, %r15
    andq    %rsi, %r9
    movq    144(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r9
    orq %r10, %r15
    movq    %r10, %r11
    movq    %r9, 160(%rsp)
    movq    48(%rsp), %r9
    andq    %r12, %r11
    movq    152(%rsp), %r10
    xorq    %rbp, %r11
    xorq    %r12, %r15
    movq    %r11, 168(%rsp)
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdx, %r9
    movq    %r15, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdi, %r10
    movq    %r9, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
    xorq    %rbx, %r14
    orq %rsi, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r11
    xorq    %r10, %rbp
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    andq    %r9, %r11
    movq    %r14, %r12
    movq    %r14, %r15
    xorq    %rsi, %r11
    orq %r10, %r15
    notq    %r12
    xorq    %r8, %r15
    movq    %r11, 192(%rsp)
    orq %r8, %r12
    movq    96(%rsp), %r11
    andq    %r10, %rsi
    movq    %rbp, 152(%rsp)
    movq    40(%rsp), %r10
    xorq    %r14, %rsi
    xorq    %r9, %r12
    movq    120(%rsp), %r8
    movq    %rsi, 144(%rsp)
    movq    72(%rsp), %rsi
    movq    %r12, 48(%rsp)
    xorq    %rdi, %r11
    movq    64(%rsp), %r12
    movq    %r15, 200(%rsp)
    xorq    %rbx, %r10
    xorq    %rax, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    xorq    %rcx, %rsi
    orq %r10, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    movq    %rsi, %r9
    notq    %rsi
    movq    %rbp, 40(%rsp)
    movq    %rsi, %rbp
    andq    %r11, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r13, %rbp
    xorq    %r10, %r9
    andq    %r8, %r10
    xorq    %r11, %rbp
    movq    %r13, %r11
    movq    %r9, 72(%rsp)
    orq %r8, %r11
    movq    32(%rsp), %r8
    xorq    %rsi, %r11
    xorq    %r13, %r10
    movq    184(%rsp), %rsi
    movq    %r11, 96(%rsp)
    movq    128(%rsp), %r11
    xorq    %rbx, %r12
    movq    %r10, 224(%rsp)
    movq    104(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r9
    xorq    %rdx, %r8
    xorq    %rcx, %rsi
    xorq    %rax, %r11
    xorq    216(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    orq %r11, %r9
    xorq    %rdi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r13
    xorq    %r8, %r9
    xorq    176(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r13
    orq %rsi, %r8
    xorq    136(%rsp), %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r13, 32(%rsp)
    xorq    %r10, %r8
    movq    %r12, %r13
    movq    %r10, %r12
    movq    %r8, 64(%rsp)
    movq    40(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rsi, %r12
    movq    %rdi, %rsi
    notq    %r13
    notq    %rsi
    xorq    %r13, %r12
    movq    %r13, %r14
    movq    %rsi, %r13
    orq %r10, %r14
    xorq    80(%rsp), %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %rcx, %r13
    xorq    %r11, %r14
    xorq    56(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rbx,%rbx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rbx, %r13
    xorq    152(%rsp), %r8
    movq    %rdx, %r15
    orq %rcx, %r15
    movq    %r14, 104(%rsp)
    movq    %rax, %r14
    xorq    %rsi, %r15
    movq    %rax, %rsi
    andq    %rdx, %r14
    orq %rbx, %rsi
    xorq    %rcx, %r14
    andq    %rbx, %rdi
    xorq    32(%rsp), %r8
    xorq    %rdx, %rsi
    xorq    %rax, %rdi
    movabsq $-9223372036854775669, %rbx
    xorq    %r13, %r8
    xorq    24(%rsp), %r8
    movq    %r15, 80(%rsp)
    movq    %rsi, 56(%rsp)
    movq    72(%rsp), %r15
    movq    96(%rsp), %rsi
    movq    224(%rsp), %r10
    movq    168(%rsp), %rcx
    xorq    192(%rsp), %r15
    xorq    200(%rsp), %rsi
    xorq    144(%rsp), %r10
    xorq    %r14, %rcx
    xorq    48(%rsp), %rcx
    xorq    %r9, %r15
    xorq    %r12, %rsi
    xorq    80(%rsp), %r15
    xorq    64(%rsp), %r10
    xorq    56(%rsp), %rsi
    xorq    %rbp, %rcx
    xorq    104(%rsp), %rcx
    xorq    %rdi, %r10
    xorq    112(%rsp), %r15
    xorq    88(%rsp), %rsi
    xorq    160(%rsp), %r10
    movq    %rcx, %rax
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
    xorq    %r10, %rdx
    movq    %rsi, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r15, %r11
    xorq    %rax, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rcx, %r10
    movq    %r8, %rcx
    xorq    %r11, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    192(%rsp), %rsi
    xorq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r15
    movq    24(%rsp), %r8
    xorq    %rcx, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r15
    xorq    %rdx, %r8
    xorq    %rbx, %r15
    movq    %rbp, %rbx
    notq    %rbx
    xorq    %r8, %r15
    orq %r12, %rbx
    movq    %r15, 24(%rsp)
    xorq    %rsi, %rbx
    movq    %rbx, 120(%rsp)
    movq    %r8, %rbx
    andq    %rsi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    144(%rsp), %rsi
    orq %rdi, %rbx
    movq    %r8, 184(%rsp)
    movq    %rdi, %r15
    movq    40(%rsp), %r8
    movq    88(%rsp), %rdi
    xorq    %r12, %rbx
    andq    %r12, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rbp, %r15
    movq    %rbx, 136(%rsp)
    xorq    %rcx, %rsi
    xorq    %rdx, %r8
    movq    %r9, %r12
    movq    %r15, 128(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %rdi
    movq    %r8, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r11, %r14
    andq    %r8, %r12
    orq %rsi, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %rbx
    movq    %r14, %r15
    xorq    %rsi, %r12
    orq %rdi, %rbx
    andq    %rdi, %rsi
    notq    %r15
    xorq    %r9, %rbx
    xorq    %r14, %rsi
    orq %r9, %r15
    movq    %rbx, 192(%rsp)
    movq    96(%rsp), %rbx
    xorq    %rdi, %rbp
    movq    48(%rsp), %r9
    movq    %rsi, 144(%rsp)
    xorq    %rdx, %r13
    movq    112(%rsp), %rdi
    movq    %rbp, 88(%rsp)
    xorq    %r8, %r15
    movq    64(%rsp), %rsi
    movq    %r12, 176(%rsp)
    xorq    %r10, %rbx
    movq    %r15, 40(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    xorq    %r11, %r9
    xorq    %rax, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r13, %r12
    orq %r9, %rbp
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdi, %rbp
    orq %rdi, %r12
    movq    %rsi, %r8
    notq    %rsi
    movq    %rbp, 48(%rsp)
    andq    %rbx, %r8
    xorq    %rsi, %r12
    movq    %rsi, %rbp
    xorq    %r9, %r8
    movq    %r12, 96(%rsp)
    andq    %r13, %rbp
    movq    104(%rsp), %r12
    andq    %rdi, %r9
    movq    160(%rsp), %rsi
    xorq    %rbx, %rbp
    xorq    %r13, %r9
    movq    72(%rsp), %rbx
    movq    %r8, 64(%rsp)
    movq    %r9, 216(%rsp)
    movq    152(%rsp), %r9
    movq    56(%rsp), %rdi
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r11, %r12
    xorq    %rax, %rbx
    xorq    %rdx, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r8
    xorq    %r10, %rdi
    xorq    200(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %r8
    movq    %rbx, %r13
    xorq    224(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r8
    andq    %r9, %r13
    orq %rsi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    xorq    %rsi, %r13
    xorq    32(%rsp), %rdx
    movq    %r9, 72(%rsp)
    movq    48(%rsp), %r9
    movq    %r13, 56(%rsp)
    movq    %r12, %r13
    movq    %rdi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %rsi, %r12
    movq    %r10, %rsi
    notq    %r13
    xorq    88(%rsp), %r9
    notq    %rsi
    xorq    %r13, %r12
    movq    %r13, %r14
    movq    %rsi, %r13
    xorq    168(%rsp), %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %rcx, %r13
    orq %rdi, %r14
    xorq    80(%rsp), %rax
    xorq    56(%rsp), %r9
    xorq    %rbx, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r11, %r13
    movq    %rdx, %r15
    movq    216(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %r9
    orq %rcx, %r15
    movq    %r14, 104(%rsp)
    xorq    %rsi, %r15
    movq    %rax, %r14
    movq    96(%rsp), %rsi
    movq    %r15, 80(%rsp)
    movq    64(%rsp), %r15
    andq    %rdx, %r14
    xorq    %rcx, %r14
    movq    128(%rsp), %rcx
    movq    %rax, %rbx
    xorq    144(%rsp), %rdi
    orq %r11, %rbx
    andq    %r11, %r10
    xorq    192(%rsp), %rsi
    xorq    %rdx, %rbx
    xorq    %rax, %r10
    xorq    176(%rsp), %r15
    movq    %rbx, 32(%rsp)
    xorq    %r14, %rcx
    xorq    24(%rsp), %r9
    xorq    40(%rsp), %rcx
    xorq    72(%rsp), %rdi
    xorq    %r12, %rsi
    xorq    %r8, %r15
    xorq    %rbx, %rsi
    xorq    80(%rsp), %r15
    xorq    %rbp, %rcx
    xorq    136(%rsp), %rsi
    xorq    %r10, %rdi
    xorq    104(%rsp), %rcx
    xorq    184(%rsp), %rdi
    xorq    120(%rsp), %r15
    movq    %rsi, %rbx
    movq    %rcx, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbx,%rbx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r15, %rbx
    xorq    %r9, %rax
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rdi, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rdi
    movq    %r9, %rcx
    xorq    %rbx, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    176(%rsp), %rsi
    xorq    %rdi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r11
    movq    24(%rsp), %r9
    movq    %rbp, %r15
    notq    %r15
    xorq    %rcx, %r10
    xorq    %rax, %r8
    xorq    %rax, %rsi
    xorq    %rbx, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r11
    xorq    %rdx, %r9
    movq    %r11, 24(%rsp)
    movabsq $-9223372036854742903, %r11
    xorq    %r11, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r15
    xorq    %rsi, %r15
    xorq    %r9, 24(%rsp)
    movq    %r15, 112(%rsp)
    movq    %r9, %r15
    andq    %rsi, %r9
    movq    144(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r9
    orq %r10, %r15
    movq    %r10, %r11
    movq    48(%rsp), %r10
    movq    %r9, 160(%rsp)
    andq    %r12, %r11
    movq    136(%rsp), %r9
    xorq    %rbp, %r11
    xorq    %r12, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r11, 168(%rsp)
    movq    %r8, %r11
    xorq    %rcx, %rsi
    xorq    %rdx, %r10
    movq    %r15, 152(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    xorq    %rdi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r14, %r12
    orq %rsi, %rbp
    andq    %r10, %r11
    xorq    %rsi, %r11
    notq    %r12
    andq    %r9, %rsi
    movq    %r11, 176(%rsp)
    orq %r8, %r12
    movq    96(%rsp), %r11
    movq    %r14, %r15
    xorq    %r10, %r12
    xorq    %r14, %rsi
    movq    40(%rsp), %r10
    orq %r9, %r15
    movq    %rsi, 144(%rsp)
    xorq    %r8, %r15
    movq    72(%rsp), %rsi
    xorq    %r9, %rbp
    movq    120(%rsp), %r8
    movq    %rbp, 136(%rsp)
    xorq    %rdi, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    xorq    %rbx, %r10
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %rbp
    xorq    %rax, %r8
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    movq    %rsi, %r9
    notq    %rsi
    movq    %rbp, 40(%rsp)
    movq    %rsi, %rbp
    andq    %r11, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r13, %rbp
    xorq    %r10, %r9
    movq    %r12, 48(%rsp)
    xorq    %r11, %rbp
    movq    %r13, %r11
    movq    104(%rsp), %r12
    orq %r8, %r11
    andq    %r8, %r10
    movq    88(%rsp), %r8
    xorq    %rsi, %r11
    movq    184(%rsp), %rsi
    xorq    %r13, %r10
    movq    %r11, 96(%rsp)
    movq    64(%rsp), %r11
    movq    %r10, 224(%rsp)
    movq    32(%rsp), %r10
    movq    %r9, 72(%rsp)
    xorq    %rdx, %r8
    xorq    %rcx, %rsi
    movq    %r15, 200(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rbx, %r12
    xorq    %rax, %r11
    xorq    %rdi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r9
    xorq    192(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r11, %r9
    movq    %r11, %r13
    xorq    216(%rsp), %rcx
    xorq    %r8, %r9
    andq    %r8, %r13
    xorq    128(%rsp), %rbx
    orq %rsi, %r8
    xorq    %rsi, %r13
    xorq    56(%rsp), %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r8
    movq    %r13, 32(%rsp)
    movq    %r12, %r13
    movq    %r8, 64(%rsp)
    movq    40(%rsp), %r8
    movq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rsi, %r12
    movq    %rdi, %rsi
    notq    %r13
    notq    %rsi
    xorq    %r13, %r12
    movq    %r13, %r14
    xorq    136(%rsp), %r8
    movq    %rsi, %r13
    orq %r10, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %rcx, %r13
    xorq    %r11, %r14
    xorq    80(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    32(%rsp), %r8
    xorq    %rbx, %r13
    movq    %r14, 104(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %r8
    movq    %rax, %r14
    xorq    24(%rsp), %r8
    orq %rcx, %r15
    andq    %rdx, %r14
    xorq    %rsi, %r15
    movq    %rax, %rsi
    orq %rbx, %rsi
    movq    %r15, 80(%rsp)
    movq    72(%rsp), %r15
    xorq    %rdx, %rsi
    movq    224(%rsp), %r10
    xorq    %rcx, %r14
    movq    %rsi, 56(%rsp)
    movq    96(%rsp), %rsi
    andq    %rbx, %rdi
    movq    168(%rsp), %rcx
    xorq    %rax, %rdi
    movabsq $-9223372036854743037, %rbx
    xorq    176(%rsp), %r15
    xorq    144(%rsp), %r10
    xorq    200(%rsp), %rsi
    xorq    %r14, %rcx
    xorq    48(%rsp), %rcx
    xorq    %r9, %r15
    xorq    64(%rsp), %r10
    xorq    %r12, %rsi
    xorq    80(%rsp), %r15
    xorq    56(%rsp), %rsi
    xorq    %rbp, %rcx
    xorq    %rdi, %r10
    xorq    104(%rsp), %rcx
    xorq    112(%rsp), %r15
    xorq    152(%rsp), %rsi
    xorq    160(%rsp), %r10
    movq    %rcx, %rax
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
    xorq    %r10, %rdx
    movq    %rsi, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rcx, %r10
    xorq    %r15, %r11
    movq    %r8, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    176(%rsp), %rsi
    xorq    %r11, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r15
    movq    24(%rsp), %r8
    xorq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rax, %rsi
    xorq    %rcx, %rdi
    xorq    %rax, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r15
    xorq    %rdx, %r8
    xorq    %r11, %r14
    xorq    %rbx, %r15
    movq    %rbp, %rbx
    notq    %rbx
    xorq    %r8, %r15
    orq %r12, %rbx
    movq    %r15, 24(%rsp)
    xorq    %rsi, %rbx
    movq    %rbx, 120(%rsp)
    movq    %r8, %rbx
    andq    %rsi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    orq %rdi, %rbx
    movq    %rdi, %r15
    movq    40(%rsp), %rdi
    movq    %r8, 184(%rsp)
    andq    %r12, %r15
    movq    144(%rsp), %rsi
    xorq    %rbp, %r15
    xorq    %r12, %rbx
    movq    152(%rsp), %r8
    movq    %rbx, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdx, %rdi
    movq    %r9, %r12
    movq    %r15, 128(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %rbp
    xorq    %rcx, %rsi
    xorq    %r10, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r14, %rbx
    orq %rsi, %rbp
    movq    %r14, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    andq    %rdi, %r12
    orq %r8, %rbx
    xorq    %rsi, %r12
    xorq    %r9, %rbx
    andq    %r8, %rsi
    xorq    %r14, %rsi
    movq    %rbx, 192(%rsp)
    notq    %r15
    movq    96(%rsp), %rbx
    orq %r9, %r15
    movq    %rsi, 144(%rsp)
    movq    48(%rsp), %r9
    xorq    %rdi, %r15
    movq    %rbp, 152(%rsp)
    movq    64(%rsp), %rsi
    movq    %r12, 176(%rsp)
    xorq    %rdx, %r13
    movq    112(%rsp), %rdi
    movq    %r15, 40(%rsp)
    xorq    %r10, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    xorq    %r11, %r9
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %r9, %rbp
    movq    %rsi, %r8
    xorq    %rax, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rbx, %r8
    xorq    %rdi, %rbp
    notq    %rsi
    movq    %r13, %r12
    movq    %rbp, 48(%rsp)
    xorq    %r9, %r8
    orq %rdi, %r12
    andq    %rdi, %r9
    movq    %rsi, %rbp
    xorq    %rsi, %r12
    xorq    %r13, %r9
    andq    %r13, %rbp
    xorq    %rbx, %rbp
    movq    %r12, 96(%rsp)
    movq    72(%rsp), %rbx
    movq    %r9, 216(%rsp)
    movq    104(%rsp), %r12
    movq    160(%rsp), %rsi
    movq    %r8, 64(%rsp)
    movq    136(%rsp), %r9
    movq    56(%rsp), %rdi
    xorq    %rax, %rbx
    xorq    %rcx, %rsi
    xorq    %rdx, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r11, %r12
    movq    %rbx, %r13
    xorq    %r10, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r8
    andq    %r9, %r13
    xorq    32(%rsp), %rdx
    orq %rbx, %r8
    xorq    %rsi, %r13
    xorq    168(%rsp), %r11
    xorq    %r9, %r8
    orq %rsi, %r9
    movq    %r13, 56(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    movq    %r12, %r13
    movq    %rdi, %r12
    movq    %r9, 72(%rsp)
    movq    48(%rsp), %r9
    andq    %rsi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    200(%rsp), %r10
    notq    %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    152(%rsp), %r9
    movq    %r10, %rsi
    xorq    %r13, %r12
    xorq    224(%rsp), %rcx
    notq    %rsi
    movq    %r13, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r15
    xorq    56(%rsp), %r9
    movq    %rsi, %r13
    orq %rcx, %r15
    xorq    80(%rsp), %rax
    orq %rdi, %r14
    xorq    %rsi, %r15
    andq    %rcx, %r13
    movq    216(%rsp), %rdi
    movq    %r15, 80(%rsp)
    movq    64(%rsp), %r15
    xorq    %rbx, %r14
    xorq    %r11, %r13
    movq    %r14, 104(%rsp)
    movq    96(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %r9
    movq    %rax, %r14
    movq    %rax, %rbx
    xorq    24(%rsp), %r9
    xorq    176(%rsp), %r15
    andq    %rdx, %r14
    orq %r11, %rbx
    xorq    %rcx, %r14
    movq    128(%rsp), %rcx
    andq    %r11, %r10
    xorq    144(%rsp), %rdi
    xorq    %rdx, %rbx
    xorq    %rax, %r10
    xorq    192(%rsp), %rsi
    movq    %rbx, 32(%rsp)
    xorq    %r8, %r15
    xorq    %r14, %rcx
    xorq    80(%rsp), %r15
    xorq    40(%rsp), %rcx
    xorq    72(%rsp), %rdi
    xorq    %r12, %rsi
    xorq    %rbx, %rsi
    xorq    120(%rsp), %r15
    xorq    %rbp, %rcx
    xorq    88(%rsp), %rsi
    xorq    %r10, %rdi
    xorq    104(%rsp), %rcx
    xorq    184(%rsp), %rdi
    movq    %r15, %rdx
    movq    %rsi, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rcx, %rax
    xorq    %rdi, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
    xorq    %rcx, %rdi
    movq    %r9, %rcx
    movq    24(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    176(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r15, %rbx
    xorq    %rdx, %r9
    xorq    %rax, %rsi
    xorq    %rbx, %rbp
    xorq    %rdi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r11
    movq    %rbp, %r15
    xorq    %rcx, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r11
    notq    %r15
    xorq    %rax, %r8
    movq    %r11, 24(%rsp)
    xorq    %rbx, %r14
    movabsq $-9223372036854743038, %r11
    xorq    %r11, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r15
    xorq    %rsi, %r15
    xorq    %r9, 24(%rsp)
    movq    %r15, 112(%rsp)
    movq    %r9, %r15
    andq    %rsi, %r9
    movq    144(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r9
    movq    %r10, %r11
    orq %r10, %r15
    movq    %r9, 160(%rsp)
    movq    48(%rsp), %r9
    andq    %r12, %r11
    movq    88(%rsp), %r10
    xorq    %rbp, %r11
    xorq    %r12, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r11, 168(%rsp)
    xorq    %rdx, %r9
    movq    %r8, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r14, %r12
    movq    %r9, %rbp
    xorq    %rcx, %rsi
    andq    %r9, %r11
    notq    %r12
    xorq    %rdi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbp
    xorq    %rsi, %r11
    movq    %r15, 136(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
    orq %r8, %r12
    movq    %r14, %r15
    xorq    %r9, %r12
    andq    %r10, %rsi
    movq    72(%rsp), %r9
    xorq    %r14, %rsi
    orq %r10, %r15
    movq    96(%rsp), %r10
    xorq    %r8, %r15
    movq    %rsi, 144(%rsp)
    movq    120(%rsp), %r8
    movq    40(%rsp), %rsi
    movq    %r11, 176(%rsp)
    xorq    %rdx, %r13
    xorq    %rcx, %r9
    movq    %rbp, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r11
    movq    %r12, 48(%rsp)
    xorq    %rdi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r13, %r12
    andq    %r10, %r11
    movq    %r10, %rbp
    xorq    %rax, %r8
    xorq    %rbx, %rsi
    notq    %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %r8, %r12
    xorq    %rsi, %r11
    orq %rsi, %rbp
    andq    %r8, %rsi
    xorq    %r8, %rbp
    xorq    %r9, %r12
    xorq    %r13, %rsi
    movq    %rbp, 40(%rsp)
    movq    152(%rsp), %r8
    movq    64(%rsp), %rbp
    movq    %r11, 72(%rsp)
    movq    %r9, %r11
    movq    %r12, 96(%rsp)
    movq    104(%rsp), %r12
    andq    %r13, %r11
    movq    %rsi, 224(%rsp)
    movq    184(%rsp), %rsi
    xorq    %r10, %r11
    movq    32(%rsp), %r10
    xorq    %rdx, %r8
    movq    %r15, 200(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rax, %rbp
    xorq    %rbx, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rcx, %rsi
    movq    %rbp, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r9
    notq    %r12
    xorq    %rdi, %r10
    orq %rbp, %r9
    movq    %r12, %r14
    andq    %r8, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    orq %r10, %r14
    orq %rsi, %r8
    xorq    %rbp, %r14
    xorq    %r10, %r8
    movq    %r10, %rbp
    movq    40(%rsp), %r10
    xorq    %rsi, %r13
    andq    %rsi, %rbp
    movq    %r13, 32(%rsp)
    xorq    192(%rsp), %rdi
    xorq    %r12, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %rsi
    xorq    216(%rsp), %rcx
    movq    %r14, 104(%rsp)
    xorq    88(%rsp), %r10
    notq    %rsi
    movq    %r8, 64(%rsp)
    movq    %rsi, %r13
    xorq    56(%rsp), %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    32(%rsp), %r10
    andq    %rcx, %r13
    movq    %rdx, %r15
    xorq    128(%rsp), %rbx
    orq %rcx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rbx, %r13
    xorq    %rsi, %r15
    xorq    80(%rsp), %rax
    xorq    %r13, %r10
    xorq    24(%rsp), %r10
    movq    %r15, 80(%rsp)
    movq    72(%rsp), %r15
    movq    224(%rsp), %r12
    movq    168(%rsp), %r8
    movq    96(%rsp), %rsi
    xorq    176(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    xorq    %r9, %r15
    xorq    80(%rsp), %r15
    andq    %rdx, %r14
    andq    %rbx, %rdi
    xorq    144(%rsp), %r12
    xorq    %rcx, %r14
    movq    %rax, %rcx
    xorq    200(%rsp), %rsi
    xorq    %r14, %r8
    orq %rbx, %rcx
    xorq    48(%rsp), %r8
    xorq    %rdx, %rcx
    xorq    %rax, %rdi
    xorq    112(%rsp), %r15
    movq    %rcx, 56(%rsp)
    xorq    64(%rsp), %r12
    xorq    %rbp, %rsi
    xorq    %rcx, %rsi
    xorq    %r11, %r8
    xorq    136(%rsp), %rsi
    movq    %r15, %rdx
    xorq    %rdi, %r12
    xorq    104(%rsp), %r8
    xorq    160(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rsi, %rcx
    movq    %r8, %rax
    xorq    %r12, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r10, %rax
    xorq    %r15, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r12
    xorq    %rsi, %r10
    movq    24(%rsp), %r8
    movq    176(%rsp), %rsi
    xorq    %rdx, %r8
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    xorq    %r10, %rdi
    xorq    %r12, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    movq    %r11, %r15
    xorq    %rcx, %r14
    orq %rsi, %rbx
    notq    %r15
    xorq    %rax, %r9
    movq    %rbx, 24(%rsp)
    movabsq $-9223372036854775680, %rbx
    xorq    %rbx, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %r8, 24(%rsp)
    andq    %rbp, %rbx
    orq %rbp, %r15
    xorq    %r11, %rbx
    movq    %r8, %r11
    andq    %rsi, %r8
    xorq    %rsi, %r15
    movq    144(%rsp), %rsi
    xorq    %rdi, %r8
    orq %rdi, %r11
    movq    136(%rsp), %rdi
    movq    %r8, 184(%rsp)
    movq    40(%rsp), %r8
    xorq    %rbp, %r11
    movq    %r15, 120(%rsp)
    xorq    %r10, %rsi
    movq    %rbx, 128(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdx, %r8
    xorq    %r12, %rdi
    movq    %r11, 152(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r15
    movq    96(%rsp), %r11
    orq %rsi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r15, 40(%rsp)
    movq    %r14, %r15
    movq    %r14, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r9,%r9
# 0 "" 2
#NO_APP
    notq    %r15
    movq    %r9, %rbx
    andq    %r8, %rbx
    orq %r9, %r15
    xorq    %r8, %r15
    xorq    %rsi, %rbx
    orq %rdi, %rbp
    andq    %rdi, %rsi
    xorq    %r9, %rbp
    movq    48(%rsp), %r9
    xorq    %r14, %rsi
    movq    112(%rsp), %rdi
    xorq    %r12, %r11
    movq    %rsi, 192(%rsp)
    movq    64(%rsp), %rsi
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r8
    xorq    %rcx, %r9
    movq    %rbx, 136(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r10, %rsi
    orq %r9, %r8
    xorq    %rax, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %r14
    notq    %rsi
    movq    %rbp, 176(%rsp)
    andq    %r11, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r9, %r14
    xorq    %rdi, %r8
    andq    %rdi, %r9
    movq    %r8, 48(%rsp)
    movq    72(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r14, 64(%rsp)
    movq    %r13, %rbx
    movq    %rsi, %r14
    xorq    %r13, %r9
    andq    %r13, %r14
    orq %rdi, %rbx
    xorq    %r11, %r14
    xorq    %rsi, %rbx
    movq    %r9, 144(%rsp)
    movq    104(%rsp), %r11
    xorq    %rax, %r8
    movq    %rbx, 96(%rsp)
    movq    56(%rsp), %rdi
    movq    160(%rsp), %rsi
    movq    88(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %rbp
    xorq    %rcx, %r11
    xorq    %r12, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r10, %rsi
    xorq    %rdx, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %r9, %rbp
    xorq    200(%rsp), %r12
    xorq    %rsi, %rbp
    xorq    32(%rsp), %rdx
    movq    %rbp, 56(%rsp)
    movq    %r11, %rbp
    notq    %r11
    movq    %r11, %r13
    orq %r8, %rbp
    xorq    168(%rsp), %rcx
    orq %rdi, %r13
    xorq    %r9, %rbp
    xorq    80(%rsp), %rax
    xorq    %r8, %r13
    orq %rsi, %r9
    movq    %r13, 104(%rsp)
    movq    %rdi, %r13
    xorq    %rdi, %r9
    andq    %rsi, %r13
    movq    224(%rsp), %rsi
    movq    %r9, 72(%rsp)
    movq    48(%rsp), %rdi
    xorq    %r11, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r8
    movq    144(%rsp), %r9
    xorq    %r10, %rsi
    movq    64(%rsp), %r10
    notq    %r8
    xorq    40(%rsp), %rdi
    movq    %r8, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %rbx
    xorq    136(%rsp), %r10
    xorq    56(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %rsi, %r11
    orq %rsi, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r8, %rbx
    xorq    %rcx, %r11
    xorq    %rbp, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r11, %rdi
    movq    %rbx, 80(%rsp)
    xorq    %rbx, %r10
    movq    %rax, %rbx
    xorq    120(%rsp), %r10
    xorq    24(%rsp), %rdi
    andq    %rdx, %rbx
    andq    %rcx, %r12
    xorq    %rsi, %rbx
    movq    %rax, %rsi
    xorq    192(%rsp), %r9
    orq %rcx, %rsi
    movq    128(%rsp), %r8
    xorq    %rax, %r12
    xorq    %rdx, %rsi
    movq    %r10, %rdx
    movq    %rsi, 32(%rsp)
    movq    96(%rsp), %rsi
    xorq    72(%rsp), %r9
    xorq    %rbx, %r8
    xorq    %r15, %r8
    xorq    176(%rsp), %rsi
    xorq    %r14, %r8
    xorq    104(%rsp), %r8
    xorq    %r12, %r9
    xorq    184(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    32(%rsp), %rsi
    movq    %r8, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rdi, %rax
    xorq    %r9, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    152(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    24(%rsp), %r8
    xorq    %rsi, %rdi
    movq    %rsi, %rcx
    movq    136(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r10, %rcx
    xorq    %rdx, %r8
    xorq    %rcx, %r14
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %r10
    xorq    %r9, %r13
    xorq    %rdi, %r12
    orq %rsi, %r10
    xorq    %rax, %rbp
    xorq    %rcx, %rbx
    xorq    $32778, %r10
    xorq    %r8, %r10
    movq    %r10, 24(%rsp)
    movq    %r14, %r10
    notq    %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    orq %r13, %r10
    xorq    %rsi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r10, 112(%rsp)
    movq    %r12, %r10
    andq    %r13, %r10
    xorq    %r14, %r10
    movq    %r8, %r14
    andq    %rsi, %r8
    movq    %r10, 168(%rsp)
    movq    48(%rsp), %r10
    orq %r12, %r14
    movq    192(%rsp), %rsi
    xorq    %r13, %r14
    xorq    %r12, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    movq    %r8, 88(%rsp)
    movq    152(%rsp), %r8
    xorq    %rdx, %r10
    movq    %r14, 136(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r10, %r13
    xorq    %rdi, %rsi
    movq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r13
    xorq    %r9, %r8
    orq %rsi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r13, 48(%rsp)
    movq    %rbx, %r13
    movq    %rbx, %r14
    notq    %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
#NO_APP
    orq %rbp, %r13
    orq %r8, %r14
    xorq    %r8, %r12
    xorq    %r10, %r13
    xorq    %rbp, %r14
    andq    %r8, %rsi
    xorq    %rbx, %rsi
    movq    96(%rsp), %rbx
    movq    %rcx, %r10
    movq    %rsi, 192(%rsp)
    movq    72(%rsp), %rsi
    xorq    %r15, %r10
    movq    120(%rsp), %r8
    xorq    %rdx, %r11
    movq    %r12, 152(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r9, %rbx
    movq    %r14, 160(%rsp)
    xorq    %rdi, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rbp
    xorq    %rax, %r8
    notq    %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
#NO_APP
    andq    %rbx, %rbp
    movq    %rbx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
    orq %r10, %r15
    andq    %r8, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r11, %r10
    movq    %rbp, 200(%rsp)
    movq    %r11, %r12
    movq    %r10, 216(%rsp)
    movq    %rsi, %rbp
    movq    64(%rsp), %r10
    xorq    %r8, %r15
    andq    %r11, %rbp
    orq %r8, %r12
    movq    104(%rsp), %r11
    movq    %r15, 72(%rsp)
    xorq    %rsi, %r12
    movq    40(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r12, 96(%rsp)
    movq    32(%rsp), %r15
    xorq    %rax, %r10
    movq    184(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r14
    xorq    %rdx, %r8
    xorq    %rcx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r8, %r14
    movq    %r11, %rbx
    xorq    %rdi, %rsi
    xorq    %r9, %r15
    notq    %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r15,%r15
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r14
    orq %r10, %rbx
    movq    %r11, %r12
    xorq    %r8, %rbx
    orq %rsi, %r8
    orq %r15, %r12
    xorq    %r15, %r8
    xorq    %r10, %r12
    movq    %r14, 40(%rsp)
    movq    %r8, 104(%rsp)
    movq    72(%rsp), %r8
    movq    %r12, 32(%rsp)
    movq    %r15, %r12
    xorq    56(%rsp), %rdx
    xorq    176(%rsp), %r9
    andq    %rsi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    152(%rsp), %r8
    movq    %r9, %rsi
    xorq    %r11, %r12
    notq    %rsi
    xorq    144(%rsp), %rdi
    movq    %rsi, %r14
    xorq    128(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    40(%rsp), %r8
    andq    %rdi, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r15
    xorq    80(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rdi, %r15
    xorq    %rcx, %r14
    xorq    %rsi, %r15
    xorq    %r14, %r8
    movq    %r14, 80(%rsp)
    xorq    24(%rsp), %r8
    movq    %r15, 56(%rsp)
    movq    200(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    movq    %rax, %rsi
    movq    216(%rsp), %r10
    andq    %rdx, %r14
    xorq    48(%rsp), %r15
    xorq    %rdi, %r14
    movq    168(%rsp), %rdi
    xorq    %rbx, %r15
    xorq    56(%rsp), %r15
    xorq    112(%rsp), %r15
    xorq    %r14, %rdi
    orq %rcx, %rsi
    xorq    %rdx, %rsi
    xorq    192(%rsp), %r10
    andq    %rcx, %r9
    movq    %rsi, 64(%rsp)
    movq    96(%rsp), %rsi
    xorq    %r13, %rdi
    xorq    %rax, %r9
    xorq    %rbp, %rdi
    movq    48(%rsp), %rcx
    xorq    32(%rsp), %rdi
    movq    %r15, %rdx
    xorq    104(%rsp), %r10
    xorq    160(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdi, %rax
    xorq    %r9, %r10
    xorq    %r12, %rsi
    xorq    88(%rsp), %r10
    xorq    64(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
    xorq    %r10, %rdx
    xorq    %rax, %rcx
    xorq    136(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rdi, %r10
    movq    %r8, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rsi, %r11
    xorq    %rsi, %rdi
    movq    24(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r15, %r11
    xorq    %r10, %r12
    xorq    %r11, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r8
    xorq    %rdx, %rsi
    movq    %rbp, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    orq %rcx, %r8
    notq    %r15
    movq    %r8, 24(%rsp)
    orq %r12, %r15
    xorq    %rax, %rbx
    movabsq $-9223372034707292150, %r8
    xorq    %rcx, %r15
    xorq    %r11, %r14
    xorq    %r8, 24(%rsp)
    movq    %r15, 120(%rsp)
    movq    %rsi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r8
    orq %r9, %r15
    andq    %r12, %r8
    xorq    %r12, %r15
    xorq    %rsi, 24(%rsp)
    andq    %rcx, %rsi
    xorq    %rbp, %r8
    xorq    %r9, %rsi
    movq    136(%rsp), %rcx
    movq    %r8, 128(%rsp)
    movq    72(%rsp), %r8
    movq    %rsi, 176(%rsp)
    movq    192(%rsp), %rsi
    movq    %r15, 184(%rsp)
    xorq    %r10, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdi, %rsi
    xorq    %rdx, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %rbx, %r9
    movq    %r14, %r12
    movq    %r14, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    notq    %r12
    andq    %r8, %r9
    movq    %r8, %rbp
    xorq    %rsi, %r9
    orq %rsi, %rbp
    orq %rbx, %r12
    orq %rcx, %r15
    andq    %rcx, %rsi
    xorq    %rcx, %rbp
    xorq    %r8, %r12
    xorq    %rbx, %r15
    xorq    %r14, %rsi
    movq    %rbp, 72(%rsp)
    movq    %r9, 136(%rsp)
    movq    %r11, %r9
    movq    %r12, 48(%rsp)
    movq    %r15, 192(%rsp)
    movq    %rsi, 144(%rsp)
    movq    112(%rsp), %rcx
    movq    96(%rsp), %rbp
    movq    104(%rsp), %rsi
    xorq    %rax, %rcx
    xorq    %r13, %r9
    movq    80(%rsp), %r13
    xorq    %r10, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %rbx
    xorq    %rdi, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbx
    movq    %rsi, %r8
    movq    %r13, %r12
    notq    %rsi
    movq    %rbx, 80(%rsp)
    andq    %rbp, %r8
    orq %rcx, %r12
    movq    %rsi, %rbx
    xorq    %r9, %r8
    xorq    %rsi, %r12
    andq    %r13, %rbx
    andq    %rcx, %r9
    xorq    %rbp, %rbx
    xorq    %r13, %r9
    movq    %r12, 96(%rsp)
    movq    200(%rsp), %rbp
    movq    %r9, 224(%rsp)
    movq    32(%rsp), %r12
    movq    %r8, 104(%rsp)
    movq    152(%rsp), %r9
    movq    88(%rsp), %r8
    movq    64(%rsp), %rcx
    xorq    %rax, %rbp
    xorq    %r11, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbp,%rbp
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    movq    %r12, %rsi
    notq    %r12
    xorq    %rdx, %r9
    orq %rbp, %rsi
    movq    %r12, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    xorq    %rdi, %r8
    xorq    %r10, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    xorq    %r9, %rsi
    orq %rcx, %r14
    orq %r8, %r9
    xorq    %rbp, %r14
    xorq    160(%rsp), %r10
    xorq    %rcx, %r9
    xorq    40(%rsp), %rdx
    movq    %rcx, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %rdx, %r15
    movq    %r10, %rcx
    xorq    216(%rsp), %rdi
    notq    %rcx
    andq    %r8, %rbp
    xorq    56(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r15
    xorq    %r12, %rbp
    movq    80(%rsp), %r12
    xorq    %rcx, %r15
    movq    %r14, 64(%rsp)
    xorq    168(%rsp), %r11
    movq    %r15, 56(%rsp)
    movq    104(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    movq    %r13, 32(%rsp)
    movq    %rcx, %r13
    andq    %rdx, %r14
    xorq    72(%rsp), %r12
    andq    %rdi, %r13
    xorq    136(%rsp), %r15
    xorq    %rdi, %r14
    movq    %rax, %rcx
    movq    128(%rsp), %rdi
    movq    %r9, 88(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    32(%rsp), %r12
    xorq    %r11, %r13
    xorq    %rsi, %r15
    xorq    %r14, %rdi
    xorq    56(%rsp), %r15
    xorq    48(%rsp), %rdi
    xorq    %r13, %r12
    xorq    24(%rsp), %r12
    xorq    120(%rsp), %r15
    xorq    %rbx, %rdi
    orq %r11, %rcx
    xorq    %rdx, %rcx
    xorq    64(%rsp), %rdi
    andq    %r11, %r10
    movq    %rcx, 168(%rsp)
    movq    96(%rsp), %rcx
    xorq    %rax, %r10
    movq    224(%rsp), %r8
    movq    %r15, %rdx
    movq    24(%rsp), %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    192(%rsp), %rcx
    movq    %rdi, %rax
    xorq    144(%rsp), %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r12, %rax
    xorq    %rbp, %rcx
    xorq    168(%rsp), %rcx
    xorq    %r9, %r8
    xorq    %r10, %r8
    xorq    176(%rsp), %r8
    xorq    184(%rsp), %rcx
    xorq    %r8, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %rcx, %r9
    xorq    %rdi, %r8
    movq    %r12, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rdi
    movq    136(%rsp), %rcx
    xorq    %rdx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r15, %r9
    xorq    %r8, %rbp
    xorq    %rdi, %r10
    xorq    %r9, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r12
    xorq    %rax, %rcx
    movq    %rbx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r12
    notq    %r15
    xorq    %rax, %rsi
    movq    %r12, 24(%rsp)
    orq %rbp, %r15
    xorq    %r9, %r14
    movabsq $-9223372034707259263, %r12
    xorq    %rcx, %r15
    xorq    %r12, 24(%rsp)
    movq    %r15, 112(%rsp)
    movq    %r11, %r15
    orq %r10, %r15
    movq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rbp, %r15
    andq    %rbp, %r12
    movq    %rsi, %rbp
    xorq    %r11, 24(%rsp)
    andq    %rcx, %r11
    xorq    %rbx, %r12
    xorq    %r10, %r11
    movq    144(%rsp), %rcx
    movq    %r12, 136(%rsp)
    movq    184(%rsp), %r10
    movq    %r11, 160(%rsp)
    movq    80(%rsp), %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r15, 152(%rsp)
    xorq    %rdi, %rcx
    movq    %r14, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdx, %r11
    xorq    %r8, %r10
    movq    %r14, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %rbp
    movq    %r11, %rbx
    notq    %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbp
    orq %rcx, %rbx
    andq    %r10, %rcx
    xorq    %r14, %rcx
    orq %r10, %r15
    xorq    %r10, %rbx
    xorq    %rsi, %r15
    movq    48(%rsp), %r10
    orq %rsi, %r12
    movq    %rcx, 144(%rsp)
    movq    88(%rsp), %rsi
    xorq    %r11, %r12
    movq    120(%rsp), %rcx
    movq    %rbp, 184(%rsp)
    movq    96(%rsp), %r11
    movq    %rbx, 80(%rsp)
    movq    %r12, 40(%rsp)
    movq    %r15, 200(%rsp)
    xorq    %rax, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %r10
    xorq    %rdi, %rsi
    xorq    %r8, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rbp
    notq    %rsi
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %rbp
    movq    %r11, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
    orq %r10, %rbx
    andq    %rcx, %r10
    movq    %rbp, 48(%rsp)
    movq    %rsi, %rbp
    xorq    %rcx, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r13, %rbp
    xorq    %r13, %r10
    movq    %rbx, 120(%rsp)
    xorq    %r11, %rbp
    movq    %r13, %r11
    orq %rcx, %r11
    xorq    %rsi, %r11
    movq    %r11, 88(%rsp)
    movq    %r10, 96(%rsp)
    movq    104(%rsp), %rbx
    movq    72(%rsp), %rsi
    movq    176(%rsp), %rcx
    movq    64(%rsp), %r12
    xorq    %rax, %rbx
    movq    168(%rsp), %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    xorq    %rdx, %rsi
    xorq    %rdi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %rsi, %r13
    xorq    %r9, %r12
    xorq    %r8, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r13, 104(%rsp)
    movq    %r12, %r13
    movq    %r12, %r11
    notq    %r13
    orq %rbx, %r11
    movq    %r13, %r14
    xorq    %rsi, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r14
    movq    %r10, %r12
    xorq    %rbx, %r14
    xorq    32(%rsp), %rdx
    andq    %rcx, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    192(%rsp), %r8
    orq %rcx, %rsi
    movq    %rdx, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %rcx
    xorq    224(%rsp), %rdi
    xorq    %r10, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    notq    %rcx
    orq %rdi, %r15
    movq    %rsi, 72(%rsp)
    xorq    %rcx, %r15
    movq    120(%rsp), %rsi
    movq    %r14, 64(%rsp)
    movq    %r15, 32(%rsp)
    movq    48(%rsp), %r15
    xorq    %r13, %r12
    xorq    56(%rsp), %rax
    movq    %rcx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    andq    %rdi, %r13
    xorq    80(%rsp), %rsi
    andq    %rdx, %r14
    xorq    184(%rsp), %r15
    movq    %rax, %rcx
    xorq    %rdi, %r14
    movq    136(%rsp), %rdi
    xorq    128(%rsp), %r9
    xorq    104(%rsp), %rsi
    xorq    %r11, %r15
    movq    96(%rsp), %r10
    xorq    %r14, %rdi
    xorq    32(%rsp), %r15
    xorq    40(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r13
    xorq    %r13, %rsi
    xorq    112(%rsp), %r15
    xorq    %rbp, %rdi
    xorq    24(%rsp), %rsi
    xorq    64(%rsp), %rdi
    orq %r9, %rcx
    andq    %r9, %r8
    xorq    %rdx, %rcx
    xorq    144(%rsp), %r10
    xorq    %rax, %r8
    movq    %rcx, 128(%rsp)
    movq    88(%rsp), %rcx
    movq    %r15, %rdx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdi, %rax
    xorq    72(%rsp), %r10
    xorq    200(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rax
    xorq    %r8, %r10
    xorq    %r12, %rcx
    xorq    160(%rsp), %r10
    xorq    128(%rsp), %rcx
    xorq    %r10, %rdx
    xorq    152(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rdi, %r10
    movq    %rsi, %rdi
    movq    24(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rcx, %rbx
    xorq    %rcx, %rdi
    movq    184(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r15, %rbx
    xorq    %rdx, %rsi
    xorq    %r10, %r12
    xorq    %rbx, %rbp
    xorq    %rdi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r9
    xorq    %rax, %rcx
    movq    %rbp, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r9
    notq    %r15
    movq    %r9, 24(%rsp)
    movabsq $-9223372036854742912, %r9
    xorq    %r9, 24(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r9
    xorq    %rsi, 24(%rsp)
    orq %r12, %r15
    andq    %r12, %r9
    xorq    %rcx, %r15
    xorq    %rbp, %r9
    xorq    %rax, %r11
    xorq    %rbx, %r14
    movq    %r15, 56(%rsp)
    movq    %rsi, %r15
    andq    %rcx, %rsi
    xorq    %r8, %rsi
    movq    144(%rsp), %rcx
    orq %r8, %r15
    movq    %rsi, 176(%rsp)
    movq    120(%rsp), %rsi
    xorq    %r12, %r15
    movq    152(%rsp), %r8
    movq    %r9, 168(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r9
    movq    %r15, 184(%rsp)
    xorq    %rdi, %rcx
    xorq    %rdx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %rsi, %r9
    movq    %rsi, %rbp
    xorq    %r10, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r14, %r15
    xorq    %rcx, %r9
    orq %rcx, %rbp
    movq    %r14, %r12
    andq    %r8, %rcx
    notq    %r15
    xorq    %r14, %rcx
    orq %r8, %r12
    xorq    %r8, %rbp
    xorq    %r11, %r12
    movq    %r9, 120(%rsp)
    orq %r11, %r15
    movq    40(%rsp), %r9
    movq    %rcx, 144(%rsp)
    xorq    %rsi, %r15
    movq    88(%rsp), %r11
    movq    %rbp, 152(%rsp)
    movq    72(%rsp), %rcx
    movq    %r12, 192(%rsp)
    movq    112(%rsp), %r8
    xorq    %rbx, %r9
    movq    64(%rsp), %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r10, %r11
    xorq    %rdi, %rcx
    xorq    %rdx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %rsi
    notq    %rcx
    xorq    %rbx, %r12
    movq    %rcx, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    andq    %r11, %rsi
    andq    %r13, %rbp
    movq    %r11, %r14
    xorq    %r9, %rsi
    xorq    %r11, %rbp
    orq %r9, %r14
    movq    %r13, %r11
    andq    %r8, %r9
    movq    %rsi, 72(%rsp)
    xorq    %r13, %r9
    orq %r8, %r11
    movq    128(%rsp), %rsi
    xorq    %rcx, %r11
    movq    %r9, 216(%rsp)
    movq    48(%rsp), %r9
    movq    %r11, 88(%rsp)
    movq    80(%rsp), %r11
    xorq    %r8, %r14
    movq    160(%rsp), %rcx
    movq    %r14, 40(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rax, %r9
    xorq    %r10, %rsi
    movq    %r12, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r13
    xorq    %rdx, %r11
    xorq    %rdi, %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r11,%r11
# 0 "" 2
#NO_APP
    andq    %r11, %r13
    orq %r9, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r13
    xorq    %r11, %r8
    movq    %r13, 64(%rsp)
    movq    %r12, %r13
    notq    %r13
    movq    %r13, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r14
    movq    %rsi, %r12
    xorq    %r9, %r14
    movq    40(%rsp), %r9
    andq    %rcx, %r12
    xorq    %r13, %r12
    xorq    200(%rsp), %r10
    orq %rcx, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rcx
    xorq    %rsi, %r11
    xorq    96(%rsp), %rdi
    notq    %rcx
    xorq    104(%rsp), %rdx
    movq    %r14, 128(%rsp)
    movq    %rcx, %r13
    xorq    32(%rsp), %rax
    movq    %r11, 160(%rsp)
    xorq    152(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %rdi, %r13
    movq    %rdx, %rsi
    xorq    136(%rsp), %rbx
    xorq    64(%rsp), %r9
    orq %rdi, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rbx, %r13
    xorq    %rcx, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %r9
    movq    %rax, %r11
    movq    %rax, %rcx
    xorq    24(%rsp), %r9
    movq    %rsi, 104(%rsp)
    andq    %rdx, %r11
    movq    72(%rsp), %r14
    xorq    %rdi, %r11
    orq %rbx, %rcx
    movq    168(%rsp), %rdi
    xorq    %rdx, %rcx
    movq    %rcx, 32(%rsp)
    xorq    120(%rsp), %r14
    xorq    %r11, %rdi
    xorq    %r15, %rdi
    xorq    %rbp, %rdi
    xorq    %r8, %r14
    xorq    128(%rsp), %rdi
    xorq    %rsi, %r14
    movq    88(%rsp), %rsi
    xorq    56(%rsp), %r14
    xorq    192(%rsp), %rsi
    andq    %r10, %rbx
    movq    216(%rsp), %r10
    xorq    %rax, %rbx
    movq    %rdi, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
    xorq    %r12, %rsi
    xorq    144(%rsp), %r10
    xorq    %rcx, %rsi
    movq    %r14, %rcx
    xorq    184(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    160(%rsp), %r10
    xorq    %rsi, %r9
    movq    %rsi, %rdx
    movq    120(%rsp), %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r14, %rdx
    xorq    %rbx, %r10
    xorq    %rdx, %rbp
    xorq    %r9, %rbx
    xorq    176(%rsp), %r10
    xorq    %rax, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %r14
    xorq    %r10, %rcx
    movq    %r14, 112(%rsp)
    movl    $2147483649, %r14d
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r14, 112(%rsp)
    xorq    %rdi, %r10
    movq    %rbp, %r14
    movq    24(%rsp), %rdi
    notq    %r14
    xorq    %r10, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rcx, %rdi
    xorq    %rdi, 112(%rsp)
    xorq    %rsi, %r14
    xorq    %rax, %r8
    movq    %r14, 24(%rsp)
    movq    %rbx, %r14
    xorq    %rdx, %r11
    andq    %r12, %r14
    xorq    %rbp, %r14
    movq    %rdi, %rbp
    andq    %rsi, %rdi
    xorq    %rbx, %rdi
    orq %rbx, %rbp
    movq    40(%rsp), %rbx
    movq    %rdi, 200(%rsp)
    movq    144(%rsp), %rdi
    xorq    %r12, %rbp
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r14, 136(%rsp)
    movq    %r8, %r14
    movq    184(%rsp), %rsi
    xorq    %rcx, %rbx
    movq    %rbp, 120(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rbx,%rbx
# 0 "" 2
#NO_APP
    andq    %rbx, %r14
    xorq    %r9, %rdi
    movq    %rbx, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r14
    xorq    %r10, %rsi
    orq %rdi, %r12
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r14, 184(%rsp)
    movq    %r11, %r14
    notq    %r14
    orq %r8, %r14
    xorq    %rbx, %r14
    movq    %r11, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbx
    andq    %rsi, %rdi
    xorq    %rsi, %r12
    xorq    %r8, %rbx
    xorq    %r11, %rdi
    movq    56(%rsp), %rsi
    movq    88(%rsp), %r8
    movq    %rdi, 48(%rsp)
    movq    %rdx, %rdi
    movq    160(%rsp), %r11
    movq    %r12, 80(%rsp)
    xorq    %r15, %rdi
    movq    %rbx, 96(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rax, %rsi
    xorq    %r10, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    %r8, %rbp
    xorq    %rcx, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r11, %r12
    movq    %r13, %rbx
    notq    %r11
    orq %rdi, %rbp
    orq %rsi, %rbx
    movq    %r11, %r15
    xorq    %rsi, %rbp
    xorq    %r11, %rbx
    andq    %r8, %r12
    andq    %r13, %r15
    movq    %rbp, 88(%rsp)
    xorq    %rdi, %r12
    xorq    %r8, %r15
    movq    %rbx, 40(%rsp)
    movq    152(%rsp), %r8
    andq    %rsi, %rdi
    movq    72(%rsp), %r11
    movq    %r15, 144(%rsp)
    movq    176(%rsp), %rsi
    xorq    %r13, %rdi
    movq    %r12, 160(%rsp)
    movq    128(%rsp), %rbx
    movq    %rdi, 224(%rsp)
    movq    32(%rsp), %rdi
    xorq    %rcx, %r8
    xorq    %rax, %r11
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r9, %rsi
    xorq    %rdx, %rbx
    xorq    %r10, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r15
    notq    %rbx
    movq    %r11, %rbp
    movq    %rbx, %r12
    andq    %r8, %rbp
    orq %r11, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r12
    xorq    %rsi, %rbp
    xorq    %r8, %r15
    xorq    %r11, %r12
    movq    %rdi, %r11
    orq %rsi, %r8
    andq    %rsi, %r11
    movq    88(%rsp), %rsi
    movq    %r12, 32(%rsp)
    xorq    %rbx, %r11
    xorq    %rdi, %r8
    xorq    192(%rsp), %r10
    xorq    64(%rsp), %rcx
    movq    %r8, 56(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rcx,%rcx
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %rcx, %r13
    movq    %r10, %rdi
    xorq    216(%rsp), %r9
    notq    %rdi
    xorq    104(%rsp), %rax
    movq    %rbp, 152(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
    xorq    80(%rsp), %rsi
    movq    %rdi, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rdi, %r13
    movq    136(%rsp), %rbx
    movq    %rax, %rdi
    andq    %rcx, %rdi
    andq    %r9, %r8
    xorq    168(%rsp), %rdx
    xorq    %r9, %rdi
    movq    %rax, %r9
    movq    %r13, 168(%rsp)
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
#NO_APP
    orq %rdx, %r9
    xorq    %rbp, %rsi
    xorq    %rdi, %rbx
    xorq    %rcx, %r9
    movq    160(%rsp), %rbp
    xorq    %r14, %rbx
    movq    %r9, 176(%rsp)
    movq    40(%rsp), %r9
    xorq    %rdx, %r8
    xorq    144(%rsp), %rbx
    xorq    %r8, %rsi
    xorq    112(%rsp), %rsi
    xorq    184(%rsp), %rbp
    xorq    96(%rsp), %r9
    xorq    %r12, %rbx
    movq    224(%rsp), %r12
    xorq    %r15, %rbp
    xorq    %r13, %rbp
    xorq    %r11, %r9
    movq    112(%rsp), %r13
    xorq    24(%rsp), %rbp
    xorq    176(%rsp), %r9
    andq    %rdx, %r10
    xorq    48(%rsp), %r12
    xorq    %rax, %r10
    movq    %rbx, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rbp, %rcx
    xorq    %rsi, %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    56(%rsp), %r12
    xorq    120(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %r12
    xorq    200(%rsp), %r12
    movq    %r9, %rdx
    xorq    %r9, %rsi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movabsq $-9223372034707259384, %r9
    xorq    %rbp, %rdx
    xorq    %rsi, %r10
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r12, %rcx
    xorq    %rcx, %r13
    movq    %r13, 112(%rsp)
    movq    184(%rsp), %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rbx, %r12
    movq    144(%rsp), %rbx
    xorq    %r12, %r11
    xorq    %rax, %r13
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rdx, %rbx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    orq %r13, %rbp
    xorq    %r9, %rbp
    movq    120(%rsp), %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r12, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, 120(%rsp)
    movq    48(%rsp), %r9
    xorq    %rsi, %r9
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, 48(%rsp)
    movq    88(%rsp), %r9
    xorq    %rcx, %r9
    xorq    %rdx, %rdi
    xorq    %rdx, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, 64(%rsp)
    movq    40(%rsp), %rdi
    xorq    %rax, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, 72(%rsp)
    movq    56(%rsp), %r14
    xorq    %rcx, %r8
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %r12, %rdi
    movq    %r15, 104(%rsp)
    movq    24(%rsp), %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, 40(%rsp)
    movq    80(%rsp), %rdi
    xorq    %rsi, %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, 56(%rsp)
    notq    %r14
    xorq    %rax, %r15
    movq    %r14, 88(%rsp)
    movq    32(%rsp), %r14
    xorq    %rcx, %rdi
    xorq    152(%rsp), %rcx
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, 80(%rsp)
    movq    176(%rsp), %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r15,%r15
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r15, 24(%rsp)
    movq    200(%rsp), %r15
    xorq    %rdx, %r14
    movq    %r8, 128(%rsp)
    movq    160(%rsp), %r8
    xorq    %r12, %rdi
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, 32(%rsp)
    notq    %r14
    xorq    96(%rsp), %r12
    movq    %r14, 160(%rsp)
    xorq    136(%rsp), %rdx
    xorq    %rsi, %r15
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r14
    xorq    224(%rsp), %rsi
    xorq    %rax, %r8
    notq    %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r15,%r15
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rdi,%rdi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r14, 96(%rsp)
    movq    208(%rsp), %r14
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    112(%rsp), %rbp
    xorq    168(%rsp), %rax
#APP
# 283 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rbp, (%r14)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r13, %rbp
    andq    112(%rsp), %r13
    movq    %rbp, 8(%r14)
    movq    %r10, %rbp
    andq    %r11, %rbp
    xorq    %rbx, %rbp
    movq    112(%rsp), %rbx
    xorq    %r10, %r13
    movq    %rbp, 16(%r14)
    movq    %r13, 32(%r14)
    orq %r10, %rbx
    movq    48(%rsp), %r10
    xorq    %r11, %rbx
    movq    %rbx, 24(%r14)
    orq %r9, %r10
    xorq    120(%rsp), %r10
    movq    %r10, 40(%r14)
    movq    104(%rsp), %r10
    andq    %r9, %r10
    xorq    48(%rsp), %r10
    movq    %r10, 48(%r14)
    movq    64(%rsp), %r10
    notq    %r10
    orq 104(%rsp), %r10
    xorq    %r9, %r10
    movq    64(%rsp), %r9
    movq    %r10, 56(%r14)
    orq 120(%rsp), %r9
    xorq    104(%rsp), %r9
    movq    %r9, 64(%r14)
    movq    48(%rsp), %r9
    andq    120(%rsp), %r9
    xorq    64(%rsp), %r9
    movq    %r9, 72(%r14)
    movq    40(%rsp), %r9
    orq 72(%rsp), %r9
    xorq    24(%rsp), %r9
    movq    %r9, 80(%r14)
    movq    56(%rsp), %r9
    andq    40(%rsp), %r9
    xorq    72(%rsp), %r9
    movq    %r9, 88(%r14)
    movq    88(%rsp), %r9
    andq    128(%rsp), %r9
    xorq    40(%rsp), %r9
    movq    %r9, 96(%r14)
    movq    128(%rsp), %r9
    orq 24(%rsp), %r9
    xorq    88(%rsp), %r9
    movq    %r9, 104(%r14)
    movq    72(%rsp), %r9
    andq    24(%rsp), %r9
    xorq    128(%rsp), %r9
    movq    %r9, 112(%r14)
    movq    80(%rsp), %r9
    andq    %r8, %r9
    xorq    %r15, %r9
    movq    %r9, 120(%r14)
    movq    32(%rsp), %r9
    orq %r8, %r9
    xorq    80(%rsp), %r9
    movq    %r9, 128(%r14)
    movq    160(%rsp), %r9
    orq %rdi, %r9
    xorq    %r8, %r9
    movq    %rdi, %r8
    andq    %r15, %r8
    movq    %r9, 136(%r14)
    xorq    160(%rsp), %r8
    movq    %r8, 144(%r14)
    movq    80(%rsp), %r8
    orq %r15, %r8
    xorq    %rdi, %r8
    movq    96(%rsp), %rdi
    movq    %r8, 152(%r14)
    andq    %rsi, %rdi
    xorq    %rdx, %rdi
    movq    %rdi, 160(%r14)
    movq    %rcx, %rdi
    orq %rsi, %rdi
    xorq    96(%rsp), %rdi
    movq    %rdi, 168(%r14)
    movq    %rax, %rdi
    andq    %rcx, %rdi
    xorq    %rsi, %rdi
    movq    %rax, %rsi
    orq %rdx, %rsi
    andq    %r12, %rdx
    movq    %rdi, 176(%r14)
    xorq    %rcx, %rsi
    xorq    %rax, %rdx
    movq    %rsi, 184(%r14)
    movq    %rdx, 192(%r14)
    addq    $232, %rsp
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
    .size   KeccakP1600_Permute_24rounds, .-KeccakP1600_Permute_24rounds
    .p2align 4,,15
    .globl  KeccakP1600_Permute_12rounds
    .type   KeccakP1600_Permute_12rounds, @function
KeccakP1600_Permute_12rounds:
.LFB33:
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
    subq    $80, %rsp
    .cfi_def_cfa_offset 136
    movq    (%rdi), %rbp
    movq    40(%rdi), %r8
    movq    56(%rdi), %r10
    movq    48(%rdi), %r14
    movq    8(%rdi), %r15
    movq    72(%rdi), %r12
    xorq    %rbp, %r8
    movq    24(%rdi), %r9
    movq    96(%rdi), %r11
    xorq    16(%rdi), %r10
    xorq    %r14, %r15
    movq    64(%rdi), %rcx
    xorq    80(%rdi), %r8
    xorq    32(%rdi), %r12
    xorq    88(%rdi), %r15
    xorq    %r11, %r10
    xorq    %r9, %rcx
    movq    144(%rdi), %rbx
    xorq    120(%rdi), %r8
    xorq    136(%rdi), %r10
    xorq    104(%rdi), %rcx
    xorq    112(%rdi), %r12
    xorq    128(%rdi), %r15
    xorq    176(%rdi), %r10
    movq    192(%rdi), %r13
    xorq    %rbx, %rcx
    xorq    160(%rdi), %r8
    xorq    152(%rdi), %r12
    xorq    168(%rdi), %r15
    movq    %r10, %rax
    xorq    184(%rdi), %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %r12
    xorq    %r8, %rax
    movq    %r15, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r8
    xorq    %r12, %rsi
    movq    %rcx, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r10, %r12
    xorq    %r15, %rdx
    movq    %rax, %r10
    xorq    %rsi, %rbp
    xorq    %r14, %r10
    movq    %r8, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rdx, %r11
    xorq    %r13, %rcx
    movl    $2147516555, %r13d
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r14
    xorq    %r12, %rbx
    xorq    %r12, %r9
    orq %r10, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %r14
    movq    %rcx, %r15
    movq    %r11, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %r13
    andq    %rbx, %r15
    xorq    %rbp, %r14
    xorq    %r11, %r15
    orq %rbx, %r13
    movq    %rbp, %r11
    xorq    %r10, %r13
    orq %rcx, %r11
    andq    %r10, %rbp
    xorq    %rbx, %r11
    xorq    %rcx, %rbp
    movq    %r13, -120(%rsp)
    movq    %r15, -104(%rsp)
    movq    80(%rdi), %rbx
    movq    176(%rdi), %rcx
    movq    %r11, -64(%rsp)
    movq    72(%rdi), %r10
    movq    %rbp, -56(%rsp)
    movq    128(%rdi), %r11
    xorq    %rsi, %rbx
    xorq    %rdx, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rbx,%rbx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    movq    %rcx, %rbp
    xorq    %r8, %r10
    xorq    %rax, %r11
    notq    %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r11,%r11
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r11, %r15
    orq %r10, %r13
    orq %r11, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %rbx, %r15
    xorq    %r9, %r13
    movq    %r13, -96(%rsp)
    xorq    %r10, %r15
    movq    %rcx, %r13
    xorq    %rbx, %rbp
    andq    %r9, %r10
    orq %r9, %r13
    xorq    %rcx, %r10
    movq    104(%rdi), %rbx
    xorq    %r11, %r13
    movq    %r10, 8(%rsp)
    movq    152(%rdi), %r11
    movq    56(%rdi), %r10
    movq    %r15, -112(%rsp)
    movq    8(%rdi), %rcx
    movq    %rbp, -32(%rsp)
    movq    160(%rdi), %r9
    xorq    %r12, %rbx
    movq    %r13, -88(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r15
    xorq    %rdx, %r10
    xorq    %r8, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r10, %r15
    movq    %r11, %rbp
    notq    %r11
    xorq    %rax, %rcx
    andq    %rbx, %rbp
    movq    %r11, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    xorq    %r10, %rbp
    xorq    %rsi, %r9
    movq    %r15, -24(%rsp)
    andq    %rcx, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    movq    %r9, %r15
    xorq    %r9, %r10
    xorq    %rbx, %r13
    orq %rcx, %r15
    movq    88(%rdi), %rbx
    xorq    %r11, %r15
    movq    %r10, -72(%rsp)
    movq    32(%rdi), %r9
    movq    40(%rdi), %r10
    movq    %rbp, -80(%rsp)
    movq    136(%rdi), %r11
    movq    %r13, -48(%rsp)
    movq    184(%rdi), %rcx
    xorq    %rax, %rbx
    movq    %r15, -40(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    xorq    %r8, %r9
    xorq    %rsi, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rdx, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r12, %rcx
    andq    %r10, %rbp
    xorq    64(%rdi), %r12
    xorq    %r9, %rbp
    xorq    120(%rdi), %rsi
    movq    %rbp, 16(%rsp)
    movq    %r11, %rbp
    notq    %r11
    movq    %r11, %r13
    orq %rbx, %rbp
    xorq    112(%rdi), %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r13
    xorq    %r10, %rbp
    orq %r9, %r10
    xorq    %rbx, %r13
    xorq    %rcx, %r10
    xorq    16(%rdi), %rdx
    movq    %r13, (%rsp)
    movq    %rcx, %r13
    movq    -24(%rsp), %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    movq    %rsi, %r15
    movq    %r12, %r9
    xorq    -96(%rsp), %rcx
    notq    %r9
    movq    %r10, 24(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r15
    xorq    168(%rdi), %rax
    xorq    %r11, %r13
    xorq    %r9, %r15
    movq    %r9, %r11
    xorq    16(%rsp), %rcx
    movq    %r15, 40(%rsp)
    andq    %r8, %r11
    movq    -80(%rsp), %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rdx, %r11
    xorq    %r14, %rcx
    movq    %rax, %rbx
    xorq    -112(%rsp), %r15
    xorq    %r11, %rcx
    movq    -104(%rsp), %r10
    movq    -40(%rsp), %r9
    xorq    %rbp, %r15
    xorq    40(%rsp), %r15
    xorq    -120(%rsp), %r15
    andq    %rsi, %rbx
    andq    %rdx, %r12
    xorq    %r8, %rbx
    movq    %rax, %r8
    xorq    -88(%rsp), %r9
    orq %rdx, %r8
    xorq    %rbx, %r10
    xorq    %rsi, %r8
    movq    -72(%rsp), %rsi
    xorq    %rax, %r12
    xorq    -32(%rsp), %r10
    movq    %r15, %rdx
    movq    %r8, -16(%rsp)
    xorq    %r13, %r9
    xorq    %r8, %r9
    xorq    8(%rsp), %rsi
    xorq    -48(%rsp), %r10
    xorq    -64(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    24(%rsp), %rsi
    xorq    (%rsp), %r10
    movq    %r9, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r12, %rsi
    xorq    %r15, %r8
    xorq    -56(%rsp), %rsi
    movq    %r10, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rcx, %rax
    xorq    %rsi, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %rsi
    movq    %rdx, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r14, %r10
    xorq    %r9, %rcx
    movq    -48(%rsp), %r14
    movq    -112(%rsp), %r9
    xorq    %r8, %r14
    xorq    %rax, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r14, %r15
    xorq    %rsi, %r13
    xorq    %rcx, %r12
    orq %r9, %r15
    xorq    %r8, %rbx
    xorq    %rax, %rbp
    movq    %r15, -112(%rsp)
    movabsq $-9223372036854775669, %r15
    xorq    %r15, -112(%rsp)
    movq    %r14, %r15
    notq    %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    orq %r13, %r15
    xorq    %r10, -112(%rsp)
    xorq    %r9, %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r12,%r12
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r15, -48(%rsp)
    movq    %r12, %r15
    andq    %r13, %r15
    xorq    %r14, %r15
    movq    %r10, %r14
    andq    %r9, %r10
    xorq    %r12, %r10
    orq %r12, %r14
    movq    -24(%rsp), %r12
    movq    %r10, 32(%rsp)
    movq    8(%rsp), %r10
    xorq    %r13, %r14
    movq    -64(%rsp), %r9
    movq    %r15, 48(%rsp)
    movq    %r14, 56(%rsp)
    movq    %rbx, %r14
    xorq    %rdx, %r12
    notq    %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r15
    xorq    %rcx, %r10
    xorq    %rsi, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r15, -64(%rsp)
    movq    %rbp, %r13
    movq    %rbx, %r15
    andq    %r12, %r13
    orq %rbp, %r14
    orq %r9, %r15
    xorq    %r12, %r14
    xorq    %r10, %r13
    xorq    %rbp, %r15
    andq    %r9, %r10
    movq    %r13, 8(%rsp)
    xorq    %rdx, %r11
    xorq    %rbx, %r10
    movq    %r15, -24(%rsp)
    movq    %r10, -8(%rsp)
    movq    -40(%rsp), %rbx
    movq    -32(%rsp), %r10
    movq    -120(%rsp), %r9
    movq    24(%rsp), %rbp
    xorq    %rsi, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r12
    xorq    %r8, %r10
    xorq    %rax, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r12
    xorq    %rcx, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r12, -40(%rsp)
    movq    %rbp, %r12
    movq    %rbp, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r11,%r11
# 0 "" 2
#NO_APP
    notq    %r12
    movq    %r11, %r15
    andq    %rbx, %r13
    orq %r9, %r15
    movq    %r12, %rbp
    xorq    %r10, %r13
    xorq    %r12, %r15
    andq    %r11, %rbp
    movq    -80(%rsp), %r12
    andq    %r9, %r10
    movq    %r13, 24(%rsp)
    xorq    %rbx, %rbp
    xorq    %r11, %r10
    movq    -96(%rsp), %rbx
    movq    %r15, 64(%rsp)
    movq    -56(%rsp), %r11
    movq    %r10, 72(%rsp)
    movq    (%rsp), %r13
    xorq    %rax, %r12
    movq    -16(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r10
    xorq    %rcx, %r11
    xorq    %rdx, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r11,%r11
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    xorq    %rsi, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %rbx, %r10
    xorq    -88(%rsp), %rsi
    xorq    %r11, %r10
    xorq    16(%rsp), %rdx
    movq    %r10, -56(%rsp)
    movq    %r13, %r10
    notq    %r13
    movq    %r13, %r15
    orq %r12, %r10
    xorq    -72(%rsp), %rcx
    orq %r9, %r15
    xorq    %rbx, %r10
    xorq    40(%rsp), %rax
    xorq    %r12, %r15
    movq    %r9, %r12
    andq    %r11, %r12
    orq %r11, %rbx
    movq    -104(%rsp), %r11
    xorq    %r13, %r12
    movq    -40(%rsp), %r13
    xorq    %r9, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %r11
    movq    %rdx, %r9
    movq    %rsi, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    notq    %r8
    orq %rcx, %r9
    movq    %rbx, -80(%rsp)
    xorq    %r8, %r9
    movq    %r8, %rbx
    movq    24(%rsp), %r8
    xorq    -64(%rsp), %r13
    andq    %rcx, %rbx
    movq    %r15, -96(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    8(%rsp), %r8
    xorq    %r11, %rbx
    movq    %rax, %r15
    xorq    -56(%rsp), %r13
    movq    %rbx, -104(%rsp)
    andq    %rdx, %r15
    movq    %r9, -88(%rsp)
    xorq    %r10, %r8
    xorq    %rbx, %r13
    xorq    %r9, %r8
    movq    %rax, %rbx
    xorq    -112(%rsp), %r13
    xorq    -48(%rsp), %r8
    xorq    %rcx, %r15
    orq %r11, %rbx
    andq    %r11, %rsi
    movq    72(%rsp), %r11
    xorq    %rdx, %rbx
    movq    64(%rsp), %r9
    xorq    %rax, %rsi
    movq    %rbx, -72(%rsp)
    movq    48(%rsp), %rcx
    movq    %r8, %rdx
    xorq    -8(%rsp), %r11
    xorq    -24(%rsp), %r9
    xorq    %r15, %rcx
    xorq    %r14, %rcx
    xorq    -80(%rsp), %r11
    xorq    %rbp, %rcx
    xorq    %r12, %r9
    xorq    -96(%rsp), %rcx
    xorq    %rbx, %r9
    xorq    56(%rsp), %r9
    xorq    %rsi, %r11
    xorq    32(%rsp), %r11
    movq    %rcx, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r9, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
    xorq    %r8, %rbx
    movq    8(%rsp), %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    movq    %r13, %rcx
    xorq    %rbx, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %rcx
    movq    -112(%rsp), %r9
    xorq    %r11, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r13, %rax
    movq    %rbp, %r13
    xorq    %rdx, %r9
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r8, %r13
    xorq    %rcx, %rsi
    xorq    %rbx, %r15
    movq    %r13, -120(%rsp)
    xorq    %rax, %r10
    movabsq $-9223372036854742903, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r8, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, -120(%rsp)
    movq    %r13, -32(%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %r9, %rbp
    andq    %r8, %r9
    orq %rsi, %rbp
    xorq    %rsi, %r9
    movq    -40(%rsp), %r8
    xorq    %r12, %rbp
    movq    56(%rsp), %rsi
    movq    %r9, (%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    movq    -8(%rsp), %r9
    movq    %rbp, 16(%rsp)
    movq    %r15, %rbp
    notq    %rbp
    movq    %r13, 8(%rsp)
    xorq    %rdx, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %rbp
    movq    %r10, %r13
    xorq    %r11, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    andq    %r8, %r13
    movq    %r8, %r12
    movq    %r15, %r8
    xorq    %rcx, %r9
    movq    %rbp, -112(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rsi, %r8
    xorq    %r9, %r13
    orq %r9, %r12
    andq    %rsi, %r9
    xorq    %rsi, %r12
    xorq    %r10, %r8
    xorq    %r15, %r9
    movq    -80(%rsp), %r10
    movq    %r8, -16(%rsp)
    movq    %r9, 56(%rsp)
    movq    64(%rsp), %r9
    movq    %rbx, %r8
    movq    -48(%rsp), %rsi
    xorq    %r14, %r8
    movq    %r13, 40(%rsp)
    movq    -104(%rsp), %r14
    movq    %r12, -40(%rsp)
    xorq    %rcx, %r10
    movq    -96(%rsp), %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %r11, %r9
    xorq    %rax, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    movq    %r9, %r12
    xorq    %rdx, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    orq %r8, %r12
    andq    %rsi, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
#NO_APP
    notq    %r10
    xorq    %r14, %r8
    movq    %r14, %r15
    movq    %r13, -80(%rsp)
    orq %rsi, %r15
    movq    %r10, %r13
    movq    %r8, 64(%rsp)
    movq    24(%rsp), %r8
    xorq    %r10, %r15
    andq    %r14, %r13
    movq    -64(%rsp), %r10
    xorq    %rsi, %r12
    xorq    %r9, %r13
    movq    32(%rsp), %r9
    movq    %r12, -104(%rsp)
    movq    -72(%rsp), %rsi
    xorq    %rbx, %rbp
    movq    %r15, -8(%rsp)
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r12
    xorq    %rdx, %r10
    xorq    %rcx, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r10, %r12
    xorq    %r11, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r12, -64(%rsp)
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %r8, %r12
    orq %rsi, %r14
    xorq    -24(%rsp), %r11
    xorq    %r8, %r14
    xorq    %r10, %r12
    xorq    72(%rsp), %rcx
    movq    %r14, -96(%rsp)
    movq    %rsi, %r14
    orq %r9, %r10
    andq    %r9, %r14
    movq    -104(%rsp), %r9
    xorq    %rsi, %r10
    movq    48(%rsp), %rsi
    movq    %r10, -72(%rsp)
    xorq    %rbp, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r8
    xorq    -56(%rsp), %rdx
    xorq    -40(%rsp), %r9
    notq    %r8
    movq    %r8, %r15
    xorq    %rbx, %rsi
    xorq    -88(%rsp), %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    -64(%rsp), %r9
    andq    %rcx, %r15
    movq    %rdx, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rcx, %rbx
    xorq    %rsi, %r15
    xorq    %r8, %rbx
    xorq    %r15, %r9
    xorq    -120(%rsp), %r9
    movq    %rbx, -56(%rsp)
    movq    -80(%rsp), %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r10
    movq    %rax, %rbp
    movq    -8(%rsp), %r8
    andq    %rdx, %r10
    xorq    40(%rsp), %rbx
    xorq    %rcx, %r10
    movq    8(%rsp), %rcx
    xorq    %r12, %rbx
    xorq    -56(%rsp), %rbx
    xorq    %r10, %rcx
    xorq    -32(%rsp), %rbx
    xorq    -112(%rsp), %rcx
    orq %rsi, %rbp
    andq    %rsi, %r11
    movq    64(%rsp), %rsi
    xorq    %rdx, %rbp
    xorq    %rax, %r11
    xorq    -16(%rsp), %r8
    movq    %rbp, -88(%rsp)
    movq    %rbx, %rdx
    xorq    %r13, %rcx
    xorq    56(%rsp), %rsi
    xorq    %r14, %r8
    xorq    -96(%rsp), %rcx
    xorq    %rbp, %r8
    xorq    16(%rsp), %r8
    xorq    -72(%rsp), %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rcx, %rax
    movq    %r8, %rbp
    xorq    %r11, %rsi
    xorq    (%rsp), %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rsi, %rdx
    xorq    %rbx, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rsi
    movq    %r9, %rcx
    movq    -120(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r8, %rcx
    movq    40(%rsp), %r8
    xorq    %rbp, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, %rbx
    xorq    %rdx, %r9
    xorq    %rsi, %r14
    xorq    %rcx, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r8, %rbx
    xorq    %rax, %r12
    xorq    %rbp, %r10
    movq    %rbx, -120(%rsp)
    movabsq $-9223372036854743037, %rbx
    xorq    %rbx, -120(%rsp)
    movq    %r13, %rbx
    notq    %rbx
    orq %r14, %rbx
    xorq    %r8, %rbx
    xorq    %r9, -120(%rsp)
    movq    %rbx, -48(%rsp)
    movq    %r11, %rbx
    andq    %r14, %rbx
    xorq    %r13, %rbx
    movq    %r9, %r13
    andq    %r8, %r9
    xorq    %r11, %r9
    orq %r11, %r13
    movq    -104(%rsp), %r11
    movq    %r9, 40(%rsp)
    movq    56(%rsp), %r9
    xorq    %r14, %r13
    movq    16(%rsp), %r8
    movq    %rbx, -24(%rsp)
    movq    %r13, 24(%rsp)
    xorq    %rdx, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r14
    xorq    %rcx, %r9
    xorq    %rsi, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %r8, %r14
    movq    %r12, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    movq    %r14, 16(%rsp)
    andq    %r11, %rbx
    movq    %r10, %r14
    notq    %r13
    xorq    %r9, %rbx
    orq %r12, %r13
    orq %r8, %r14
    andq    %r8, %r9
    xorq    %r11, %r13
    xorq    %r12, %r14
    xorq    %r10, %r9
    movq    %rbx, 48(%rsp)
    movq    %r13, -104(%rsp)
    movq    %r14, 56(%rsp)
    movq    %r9, 32(%rsp)
    movq    -72(%rsp), %r9
    movq    -32(%rsp), %r8
    movq    -8(%rsp), %rbx
    movq    -112(%rsp), %r11
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rcx, %r9
    xorq    %rsi, %rbx
    xorq    %rbp, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %rdx, %r15
    notq    %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
#NO_APP
    andq    %rbx, %r12
    movq    %rbx, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r11, %r12
    orq %r11, %r10
    andq    %r8, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %r15, %r11
    movq    %r15, %r13
    movq    %r12, -72(%rsp)
    orq %r8, %r13
    movq    %r11, 72(%rsp)
    movq    -80(%rsp), %r11
    xorq    %r9, %r13
    movq    %r9, %r12
    movq    -40(%rsp), %r9
    xorq    %r8, %r10
    movq    (%rsp), %r8
    movq    %r13, -8(%rsp)
    movq    -96(%rsp), %r13
    movq    %r10, -112(%rsp)
    andq    %r15, %r12
    xorq    %rax, %r11
    movq    -88(%rsp), %r10
    xorq    %rbx, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r14
    xorq    %rdx, %r9
    xorq    %rcx, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r14
    xorq    %rbp, %r13
    xorq    %rsi, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r14, -96(%rsp)
    movq    %r13, %r14
    movq    %r13, %rbx
    notq    %r14
    orq %r11, %rbx
    movq    %r14, %r15
    xorq    %r9, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r15
    orq %r8, %r9
    movq    %r10, %r13
    xorq    %r10, %r9
    movq    8(%rsp), %r10
    andq    %r8, %r13
    xorq    -64(%rsp), %rdx
    xorq    %r11, %r15
    movq    %r9, -80(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    -16(%rsp), %rsi
    movq    %r15, -88(%rsp)
    xorq    %r14, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rbp, %r10
    movq    %rsi, %r8
    movq    %rdx, %rbp
    xorq    64(%rsp), %rcx
    notq    %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %rbp
    movq    -112(%rsp), %r9
    movq    %r8, %r14
    xorq    %r8, %rbp
    xorq    -56(%rsp), %rax
    andq    %rcx, %r14
    movq    %rbp, -64(%rsp)
    movq    -72(%rsp), %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r15
    xorq    16(%rsp), %r9
    movq    %rax, %r8
    andq    %rdx, %r15
    xorq    %rcx, %r15
    movq    -24(%rsp), %rcx
    xorq    48(%rsp), %rbp
    xorq    -96(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r15, %rcx
    xorq    %r10, %r14
    xorq    -104(%rsp), %rcx
    xorq    %rbx, %rbp
    xorq    -64(%rsp), %rbp
    xorq    %r14, %r9
    xorq    -120(%rsp), %r9
    xorq    %r12, %rcx
    xorq    -48(%rsp), %rbp
    xorq    -88(%rsp), %rcx
    orq %r10, %r8
    andq    %r10, %rsi
    xorq    %rdx, %r8
    xorq    %rax, %rsi
    movq    %r8, -56(%rsp)
    movq    -8(%rsp), %r8
    movq    72(%rsp), %r10
    movq    %rbp, %rdx
    movq    %rcx, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    56(%rsp), %r8
    xorq    32(%rsp), %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
    xorq    %r13, %r8
    xorq    -80(%rsp), %r10
    xorq    -56(%rsp), %r8
    xorq    %rsi, %r10
    xorq    24(%rsp), %r8
    xorq    40(%rsp), %r10
    movq    %r8, %r11
    xorq    %r10, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rcx, %r10
    movq    %r9, %rcx
    movq    -120(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r8, %rcx
    movq    48(%rsp), %r8
    xorq    %r10, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rbp, %r11
    xorq    %rdx, %r9
    xorq    %rcx, %rsi
    xorq    %r11, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %rbp
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %rbp
    movq    %rbp, -120(%rsp)
    movabsq $-9223372036854743038, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rbp, -120(%rsp)
    movq    %r12, %rbp
    xorq    %r11, %r15
    notq    %rbp
    xorq    %rax, %rbx
    orq %r13, %rbp
    xorq    %r8, %rbp
    xorq    %r9, -120(%rsp)
    movq    %rbp, -32(%rsp)
    movq    %rsi, %rbp
    andq    %r13, %rbp
    xorq    %r12, %rbp
    movq    %r9, %r12
    andq    %r8, %r9
    xorq    %rsi, %r9
    orq %rsi, %r12
    movq    -112(%rsp), %rsi
    movq    %r9, (%rsp)
    movq    32(%rsp), %r9
    xorq    %r13, %r12
    movq    %rbp, 8(%rsp)
    movq    24(%rsp), %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %rdx, %rsi
    movq    %r12, -40(%rsp)
    movq    %r15, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %r13
    xorq    %rcx, %r9
    xorq    %r10, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
    notq    %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rbp,%rbp
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rbp, %r13
    movq    %rbx, %r8
    orq %rbx, %r12
    movq    %r13, 24(%rsp)
    andq    %rsi, %r8
    movq    %r15, %r13
    xorq    %r9, %r8
    xorq    %rsi, %r12
    orq %rbp, %r13
    xorq    %rbx, %r13
    movq    %r8, -16(%rsp)
    movq    -104(%rsp), %rbx
    movq    -80(%rsp), %r8
    movq    %r12, -112(%rsp)
    andq    %rbp, %r9
    movq    -48(%rsp), %rsi
    xorq    %r15, %r9
    movq    %r13, 48(%rsp)
    movq    -8(%rsp), %r12
    movq    %r9, 32(%rsp)
    xorq    %r11, %rbx
    xorq    %rax, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r10, %r12
    xorq    %rcx, %r8
    xorq    %rdx, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r12,%r12
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r12, %r15
    movq    %r8, %rbp
    movq    %r14, %r9
    orq %rbx, %r15
    andq    %r12, %rbp
    notq    %r8
    orq %rsi, %r9
    xorq    %rsi, %r15
    xorq    %rbx, %rbp
    xorq    %r8, %r9
    movq    %r15, -104(%rsp)
    andq    %rsi, %rbx
    movq    %rbp, -80(%rsp)
    movq    %r8, %rbp
    xorq    %r14, %rbx
    movq    %r9, -8(%rsp)
    movq    -88(%rsp), %r13
    andq    %r14, %rbp
    xorq    %r12, %rbp
    movq    -56(%rsp), %rsi
    movq    %rbx, 64(%rsp)
    movq    -72(%rsp), %r12
    movq    16(%rsp), %r9
    movq    40(%rsp), %r8
    xorq    %r11, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, %rbx
    notq    %r13
    xorq    %rax, %r12
    movq    %r13, %r15
    xorq    %r10, %rsi
    xorq    %rdx, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r12,%r12
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r12, %r14
    orq %rsi, %r15
    orq %r12, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r12, %r15
    andq    %r9, %r14
    movq    %rsi, %r12
    xorq    %r9, %rbx
    xorq    %rcx, %r8
    movq    %r15, -88(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r14
    andq    %r8, %r12
    orq %r8, %r9
    xorq    %r13, %r12
    movq    -24(%rsp), %r13
    xorq    %rsi, %r9
    xorq    -96(%rsp), %rdx
    movq    %r9, -72(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    56(%rsp), %r10
    movq    %rdx, %r9
    movq    %r14, -56(%rsp)
    xorq    %r11, %r13
    movq    -80(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    movq    -104(%rsp), %rsi
    movq    %r10, %r8
    xorq    72(%rsp), %rcx
    notq    %r8
    xorq    -16(%rsp), %r11
    movq    %r8, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r9
    xorq    -64(%rsp), %rax
    andq    %rcx, %r14
    xorq    %r8, %r9
    xorq    24(%rsp), %rsi
    xorq    %rbx, %r11
    movq    %r9, -64(%rsp)
    movq    -8(%rsp), %r8
    xorq    %r9, %r11
    movq    8(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r15
    xorq    -56(%rsp), %rsi
    andq    %rdx, %r15
    xorq    -32(%rsp), %r11
    xorq    %rcx, %r15
    movq    %rax, %rcx
    xorq    %r15, %r9
    xorq    -112(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %r13, %r14
    orq %r13, %rcx
    xorq    %r14, %rsi
    xorq    %rbp, %r9
    xorq    -120(%rsp), %rsi
    xorq    -88(%rsp), %r9
    xorq    %rdx, %rcx
    andq    %r13, %r10
    movq    64(%rsp), %r13
    xorq    %rax, %r10
    movq    %rcx, -96(%rsp)
    xorq    48(%rsp), %r8
    movq    %r11, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r9, %rax
    xorq    32(%rsp), %r13
    xorq    %r12, %r8
    xorq    %rcx, %r8
    xorq    -40(%rsp), %r8
    xorq    -72(%rsp), %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rax
    movq    %r8, %rcx
    xorq    %r10, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rsi
    movq    -16(%rsp), %r8
    xorq    (%rsp), %r13
    xorq    %rsi, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r11, %rcx
    xorq    %rcx, %rbp
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r11
    xorq    %r13, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %r9, %r13
    movq    -120(%rsp), %r9
    movq    %r11, -120(%rsp)
    movabsq $-9223372036854775680, %r11
    xorq    %r13, %r12
    xorq    %r11, -120(%rsp)
    movq    %rbp, %r11
    notq    %r11
    xorq    %rdx, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %r9, -120(%rsp)
    orq %r12, %r11
    xorq    %rcx, %r15
    xorq    %r8, %r11
    xorq    %rax, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r11, -48(%rsp)
    movq    %r10, %r11
    andq    %r12, %r11
    xorq    %rbp, %r11
    movq    %r9, %rbp
    andq    %r8, %r9
    orq %r10, %rbp
    xorq    %r10, %r9
    movq    -104(%rsp), %r8
    xorq    %r12, %rbp
    movq    -40(%rsp), %r10
    movq    %r9, 40(%rsp)
    movq    %rbp, 16(%rsp)
    movq    32(%rsp), %r9
    movq    %r15, %rbp
    notq    %rbp
    movq    %r11, -24(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    movq    %rbx, %r12
    xorq    %rdx, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    andq    %r8, %r12
    movq    %r8, %r11
    movq    %r15, %r8
    xorq    %r13, %r10
    xorq    %rsi, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r10,%r10
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r10, %r8
    xorq    %r9, %r12
    orq %r9, %r11
    andq    %r10, %r9
    xorq    %r10, %r11
    xorq    %rbx, %r8
    xorq    %r15, %r9
    movq    %r11, -40(%rsp)
    movq    -32(%rsp), %r10
    movq    -8(%rsp), %r11
    movq    %r8, 56(%rsp)
    movq    %r9, 32(%rsp)
    movq    -112(%rsp), %r8
    movq    -72(%rsp), %r9
    movq    %r12, -16(%rsp)
    xorq    %rax, %r10
    movq    %rbp, -104(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rcx, %r8
    xorq    %r13, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
    xorq    %rdx, %r14
    movq    %r11, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r15
    orq %r8, %r12
    notq    %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
#NO_APP
    andq    %r11, %r15
    movq    %r14, %rbx
    xorq    %r10, %r12
    xorq    %r8, %r15
    orq %r10, %rbx
    andq    %r10, %r8
    xorq    %r9, %rbx
    xorq    %r14, %r8
    movq    %r15, -72(%rsp)
    movq    %rbx, -8(%rsp)
    movq    %r9, %r15
    movq    -88(%rsp), %rbx
    movq    %r8, 72(%rsp)
    movq    24(%rsp), %r9
    andq    %r14, %r15
    movq    (%rsp), %r8
    xorq    %r11, %r15
    movq    %r12, -112(%rsp)
    movq    -96(%rsp), %r10
    movq    -80(%rsp), %r11
    xorq    %rcx, %rbx
    xorq    %rdx, %r9
    xorq    %rsi, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r12
    notq    %rbx
    xorq    %r13, %r10
    movq    %rbx, %r14
    xorq    %rax, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r11, %r14
    movq    %r11, %rbp
    orq %r11, %r12
    movq    %r14, -88(%rsp)
    movq    %r10, %r14
    andq    %r9, %rbp
    andq    %r8, %r14
    xorq    %r8, %rbp
    xorq    %r9, %r12
    xorq    %rbx, %r14
    orq %r8, %r9
    movq    -112(%rsp), %r8
    xorq    %r10, %r9
    xorq    48(%rsp), %r13
    movq    %rbp, -96(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r9, -80(%rsp)
    movq    %r13, %r9
    xorq    64(%rsp), %rsi
    xorq    -40(%rsp), %r8
    notq    %r9
    movq    %r9, %rbx
    xorq    -56(%rsp), %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %rsi, %rbx
    xorq    %rbp, %r8
    movq    %rdx, %rbp
    xorq    8(%rsp), %rcx
    orq %rsi, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbx
    xorq    %r9, %rbp
    xorq    -64(%rsp), %rax
    xorq    %rbx, %r8
    xorq    -120(%rsp), %r8
    movq    %rbp, -64(%rsp)
    movq    -72(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    -24(%rsp), %r9
    movq    %rax, %r10
    orq %rcx, %r10
    xorq    -16(%rsp), %r11
    xorq    %rdx, %r10
    movq    %r10, -56(%rsp)
    xorq    %r12, %r11
    xorq    %rbp, %r11
    movq    %rax, %rbp
    andq    %rdx, %rbp
    xorq    -48(%rsp), %r11
    xorq    %rsi, %rbp
    movq    -8(%rsp), %rsi
    xorq    %rbp, %r9
    xorq    -104(%rsp), %r9
    movq    %r11, %rdx
    xorq    56(%rsp), %rsi
    xorq    %r15, %r9
    xorq    -88(%rsp), %r9
    xorq    %r14, %rsi
    andq    %rcx, %r13
    xorq    %r10, %rsi
    movq    72(%rsp), %r10
    xorq    %rax, %r13
    xorq    16(%rsp), %rsi
    movq    %r9, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    32(%rsp), %r10
    xorq    %r8, %rax
    movq    %rsi, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rsi, %r8
    movq    -16(%rsp), %rsi
    xorq    -80(%rsp), %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r11, %rcx
    xorq    %rax, %rsi
    xorq    %r13, %r10
    xorq    %rcx, %r15
    xorq    %r8, %r13
    xorq    40(%rsp), %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %rdx
    orq %rsi, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r9, %r10
    movq    -120(%rsp), %r9
    xorq    $32778, %r11
    xorq    %r10, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r14,%r14
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rdx, %r9
    xorq    %r9, %r11
    movq    %r11, -120(%rsp)
    movq    %r15, %r11
    notq    %r11
    orq %r14, %r11
    xorq    %rsi, %r11
    movq    %r11, -32(%rsp)
    movq    %r13, %r11
    andq    %r14, %r11
    xorq    %rcx, %rbp
    xorq    %rax, %r12
    xorq    %r15, %r11
    movq    %r9, %r15
    andq    %rsi, %r9
    orq %r13, %r15
    xorq    %r13, %r9
    movq    %r11, 8(%rsp)
    xorq    %r14, %r15
    movq    -112(%rsp), %r11
    movq    %r9, 24(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    16(%rsp), %rsi
    movq    %r15, (%rsp)
    movq    %rbp, %r15
    movq    32(%rsp), %r9
    notq    %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r15
    movq    %r12, %r14
    xorq    %rdx, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r11, %r15
    andq    %r11, %r14
    movq    %r11, %r13
    movq    %rbp, %r11
    xorq    %r10, %rsi
    xorq    %r8, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rsi, %r11
    xorq    %r9, %r14
    orq %r9, %r13
    andq    %rsi, %r9
    xorq    %r12, %r11
    xorq    %rsi, %r13
    xorq    %rbp, %r9
    movq    %r11, 48(%rsp)
    movq    -48(%rsp), %rsi
    movq    %r9, 32(%rsp)
    movq    -104(%rsp), %r11
    movq    -8(%rsp), %rbp
    movq    %r13, 16(%rsp)
    movq    -80(%rsp), %r9
    movq    %r14, -16(%rsp)
    xorq    %rax, %rsi
    movq    %r15, -112(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    xorq    %r10, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r11,%r11
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    %rbp, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r13
    orq %r11, %r12
    notq    %r9
    andq    %rbp, %r13
    xorq    %rsi, %r12
    xorq    %rdx, %rbx
    xorq    %r11, %r13
    movq    %r12, -104(%rsp)
    andq    %rsi, %r11
    movq    %r13, -80(%rsp)
    movq    -88(%rsp), %r13
    movq    %r9, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r14
    xorq    %rbx, %r11
    andq    %rbx, %r12
    orq %rsi, %r14
    movq    -72(%rsp), %rbx
    movq    %r11, 64(%rsp)
    xorq    %r9, %r14
    movq    -40(%rsp), %r11
    xorq    %rcx, %r13
    movq    -56(%rsp), %rsi
    movq    %r14, -8(%rsp)
    xorq    %rbp, %r12
    movq    40(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, %r14
    movq    %r13, %rbp
    xorq    %rax, %rbx
    notq    %r14
    xorq    %rdx, %r11
    xorq    %r10, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r14, %r13
    orq %rbx, %rbp
    movq    %rbx, %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    xorq    %r11, %rbp
    andq    %r11, %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rsi, %r13
    orq %r9, %r11
    xorq    %r9, %r15
    xorq    %rbx, %r13
    xorq    %rsi, %r11
    movq    %r15, -56(%rsp)
    movq    %r13, -88(%rsp)
    movq    %rsi, %r13
    movq    -104(%rsp), %rsi
    movq    %r11, -40(%rsp)
    movq    -24(%rsp), %r11
    andq    %r9, %r13
    xorq    %r14, %r13
    movq    8(%rsp), %r9
    xorq    %rcx, %r11
    xorq    16(%rsp), %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    -96(%rsp), %rdx
    xorq    56(%rsp), %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r15, %rsi
    movq    %r10, %rcx
    movq    %rdx, %r15
    xorq    72(%rsp), %r8
    notq    %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r15
    movq    %rcx, %r14
    xorq    -64(%rsp), %rax
    xorq    %rcx, %r15
    movq    -80(%rsp), %rcx
    andq    %r8, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %r15, -64(%rsp)
    movq    %rax, %rbx
    xorq    %r11, %r14
    orq %r11, %rbx
    xorq    %r14, %rsi
    xorq    -16(%rsp), %rcx
    xorq    %rdx, %rbx
    xorq    -120(%rsp), %rsi
    movq    %rbx, -96(%rsp)
    xorq    %rbp, %rcx
    xorq    %r15, %rcx
    movq    %rax, %r15
    andq    %rdx, %r15
    xorq    -32(%rsp), %rcx
    xorq    %r8, %r15
    movq    -8(%rsp), %r8
    xorq    %r15, %r9
    xorq    -112(%rsp), %r9
    movq    %rcx, %rdx
    xorq    48(%rsp), %r8
    xorq    %r12, %r9
    xorq    -88(%rsp), %r9
    xorq    %r13, %r8
    xorq    %rbx, %r8
    xorq    (%rsp), %r8
    andq    %r11, %r10
    movq    64(%rsp), %r11
    xorq    %rax, %r10
    movq    %r9, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %r8, %rbx
    xorq    %rsi, %rax
    xorq    32(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbx
    movq    -16(%rsp), %rcx
    xorq    %rbx, %r12
    xorq    -40(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rax, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r10, %r11
    xorq    24(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rsi
    movq    -120(%rsp), %r8
    xorq    %rsi, %r10
    xorq    %r11, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    %r12, %r9
    xorq    %rdx, %r8
    orq %rcx, %r9
    xorq    %r11, %r13
    movq    %r9, -120(%rsp)
    movabsq $-9223372034707292150, %r9
    xorq    %r9, -120(%rsp)
    movq    %r12, %r9
    notq    %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    orq %r13, %r9
    xorq    %r8, -120(%rsp)
    xorq    %rcx, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r9, -48(%rsp)
    movq    %r10, %r9
    andq    %r13, %r9
    xorq    %r12, %r9
    movq    %r8, %r12
    orq %r10, %r12
    andq    %rcx, %r8
    movq    %r9, -24(%rsp)
    xorq    %r10, %r8
    movq    32(%rsp), %r9
    xorq    %r13, %r12
    movq    %r8, 40(%rsp)
    movq    -104(%rsp), %r8
    xorq    %rbx, %r15
    movq    (%rsp), %rcx
    movq    %r12, -72(%rsp)
    xorq    %rax, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %r12
    xorq    %rsi, %r9
    xorq    %rdx, %r14
    xorq    %rdx, %r8
    notq    %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r13
    xorq    %r11, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbp,%rbp
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %rbp, %r10
    orq %r9, %r13
    orq %rbp, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %r8, %r10
    xorq    %rcx, %r13
    xorq    %r8, %r12
    movq    %r13, (%rsp)
    xorq    %r9, %r10
    movq    -40(%rsp), %r8
    movq    %r15, %r13
    movq    %r10, -16(%rsp)
    andq    %rcx, %r9
    movq    -112(%rsp), %r10
    movq    %r12, -104(%rsp)
    orq %rcx, %r13
    movq    -8(%rsp), %r12
    xorq    %rbp, %r13
    xorq    %r15, %r9
    movq    -32(%rsp), %rcx
    xorq    %rsi, %r8
    movq    %r13, 56(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %rbp
    xorq    %rbx, %r10
    movq    -88(%rsp), %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rax, %rcx
    xorq    %r11, %r12
    notq    %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r12,%r12
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
#NO_APP
    andq    %r12, %rbp
    movq    %r12, %r15
    movq    %r9, 32(%rsp)
    xorq    %r10, %rbp
    orq %r10, %r15
    movq    %r14, %r9
    movq    %rbp, -40(%rsp)
    movq    %r8, %rbp
    xorq    %rcx, %r15
    andq    %r14, %rbp
    orq %rcx, %r9
    andq    %rcx, %r10
    movq    -96(%rsp), %rcx
    xorq    %r12, %rbp
    xorq    %r8, %r9
    xorq    %r14, %r10
    movq    -80(%rsp), %r12
    movq    %r9, -8(%rsp)
    movq    %r10, 72(%rsp)
    movq    24(%rsp), %r9
    xorq    %rbx, %r13
    movq    16(%rsp), %r10
    movq    %r15, -112(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, %r8
    notq    %r13
    xorq    %r11, %rcx
    movq    %r13, %r15
    xorq    %rax, %r12
    xorq    %rsi, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r15
    xorq    %rdx, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %r12, %r15
    orq %r12, %r8
    movq    %r12, %r14
    movq    %rcx, %r12
    movq    %r15, -88(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r10,%r10
# 0 "" 2
#NO_APP
    andq    %r9, %r12
    andq    %r10, %r14
    xorq    %r10, %r8
    xorq    %r9, %r14
    xorq    %r13, %r12
    orq %r9, %r10
    movq    -112(%rsp), %r13
    xorq    %rcx, %r10
    movq    %r14, -96(%rsp)
    movq    8(%rsp), %r9
    movq    %r10, -80(%rsp)
    xorq    %rbx, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    48(%rsp), %r11
    xorq    (%rsp), %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rcx
    xorq    64(%rsp), %rsi
    andq    %r9, %r11
    notq    %rcx
    xorq    -56(%rsp), %rdx
    xorq    -96(%rsp), %r13
    movq    %rcx, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %rsi, %r14
    movq    %rdx, %rbx
    xorq    -64(%rsp), %rax
    xorq    %r9, %r14
    orq %rsi, %rbx
    xorq    %rcx, %rbx
    xorq    %r14, %r13
    xorq    -120(%rsp), %r13
    movq    %rbx, -64(%rsp)
    movq    -40(%rsp), %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r15
    movq    -8(%rsp), %rcx
    movq    %rax, %r10
    andq    %rdx, %r15
    orq %r9, %r10
    movq    72(%rsp), %r9
    xorq    %rsi, %r15
    movq    -24(%rsp), %rsi
    xorq    %rdx, %r10
    xorq    -16(%rsp), %rbx
    movq    %r10, -56(%rsp)
    xorq    56(%rsp), %rcx
    xorq    %r15, %rsi
    xorq    -104(%rsp), %rsi
    xorq    %r8, %rbx
    xorq    -64(%rsp), %rbx
    xorq    %r12, %rcx
    xorq    %r10, %rcx
    xorq    -72(%rsp), %rcx
    xorq    %rbp, %rsi
    xorq    -48(%rsp), %rbx
    xorq    -88(%rsp), %rsi
    xorq    %rax, %r11
    xorq    32(%rsp), %r9
    movq    %rcx, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %rbx, %rdx
    xorq    %rbx, %r10
    movq    -120(%rsp), %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    -80(%rsp), %r9
    movq    %rsi, %rax
    xorq    %r10, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %r13, %rax
    xorq    %r11, %r9
    xorq    40(%rsp), %r9
    xorq    %r9, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
    movq    %r13, %rsi
    movq    %rbp, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rsi
    movq    -16(%rsp), %rcx
    xorq    %r9, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rsi, %r11
    xorq    %rdx, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rax, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r13
    movq    %r13, -120(%rsp)
    movabsq $-9223372034707259263, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %rcx, %r13
    xorq    %rbx, -120(%rsp)
    movq    %r13, -32(%rsp)
    movq    %r11, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rbx, %rbp
    orq %r11, %rbp
    movq    %r13, 8(%rsp)
    xorq    %r12, %rbp
    andq    %rcx, %rbx
    movq    32(%rsp), %rcx
    xorq    %r11, %rbx
    movq    -72(%rsp), %r11
    movq    %rbp, 16(%rsp)
    movq    %rbx, 24(%rsp)
    movq    -112(%rsp), %rbx
    xorq    %rax, %r8
    xorq    %r10, %r15
    xorq    %rdx, %r14
    xorq    %rsi, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdx, %rbx
    xorq    %r9, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r12
    orq %rcx, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r11,%r11
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %r11, %r12
    movq    %r8, %r13
    movq    %r15, %rbp
    movq    %r12, -72(%rsp)
    andq    %rbx, %r13
    movq    %r15, %r12
    notq    %rbp
    xorq    %rcx, %r13
    orq %r11, %r12
    orq %r8, %rbp
    andq    %r11, %rcx
    xorq    %r8, %r12
    xorq    %rbx, %rbp
    xorq    %r15, %rcx
    movq    %r13, -16(%rsp)
    movq    %rbp, -112(%rsp)
    movq    %r12, 48(%rsp)
    movq    %rcx, 32(%rsp)
    movq    -8(%rsp), %r11
    movq    -104(%rsp), %r8
    movq    -80(%rsp), %rbx
    movq    -48(%rsp), %rcx
    xorq    %r9, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    xorq    %r10, %r8
    xorq    %rsi, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %r8, %r13
    movq    %rbx, %r15
    movq    %r14, %rbp
    xorq    %rax, %rcx
    notq    %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r13
    andq    %r11, %r15
    orq %rcx, %rbp
    xorq    %r8, %r15
    xorq    %rbx, %rbp
    andq    %rcx, %r8
    movq    %rbp, -80(%rsp)
    xorq    %r14, %r8
    movq    -40(%rsp), %rbp
    movq    %r8, -8(%rsp)
    movq    (%rsp), %r8
    movq    %rbx, %r12
    movq    40(%rsp), %rcx
    movq    %r13, -48(%rsp)
    andq    %r14, %r12
    movq    -88(%rsp), %r13
    xorq    %r11, %r12
    movq    %r15, -104(%rsp)
    xorq    %rax, %rbp
    movq    -56(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r14
    xorq    %rdx, %r8
    xorq    %rsi, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r14
    xorq    %r10, %r13
    xorq    %r9, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r14
    xorq    56(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r14, -88(%rsp)
    movq    %r13, %rbx
    movq    %r13, %r14
    orq %rbp, %rbx
    notq    %r14
    xorq    %r8, %rbx
    movq    %r14, %r15
    orq %rcx, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r11, %r8
    orq %r11, %r15
    movq    %r11, %r13
    movq    -24(%rsp), %r11
    andq    %rcx, %r13
    xorq    %rbp, %r15
    xorq    %r14, %r13
    movq    %r8, (%rsp)
    movq    -48(%rsp), %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rcx
    movq    %r15, -40(%rsp)
    xorq    %r10, %r11
    notq    %rcx
    movq    -104(%rsp), %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    72(%rsp), %rsi
    movq    %rcx, %r14
    andq    %r11, %r9
    xorq    -96(%rsp), %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %rbp
    xorq    -64(%rsp), %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbp
    andq    %rsi, %r14
    xorq    -72(%rsp), %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbp
    movq    %rax, %rcx
    movq    %rax, %r15
    orq %r11, %rcx
    andq    %rdx, %r15
    xorq    %r11, %r14
    xorq    %rdx, %rcx
    xorq    %rsi, %r15
    movq    8(%rsp), %rsi
    movq    %rcx, -24(%rsp)
    movq    -80(%rsp), %rcx
    xorq    %rax, %r9
    movq    -8(%rsp), %r11
    movq    %rbp, -96(%rsp)
    xorq    -16(%rsp), %r10
    xorq    %r15, %rsi
    xorq    -88(%rsp), %r8
    xorq    48(%rsp), %rcx
    xorq    -112(%rsp), %rsi
    xorq    32(%rsp), %r11
    xorq    %rbx, %r10
    xorq    %r14, %r8
    xorq    %rbp, %r10
    xorq    %r13, %rcx
    xorq    -120(%rsp), %r8
    xorq    -24(%rsp), %rcx
    xorq    %r12, %rsi
    xorq    -32(%rsp), %r10
    xorq    -40(%rsp), %rsi
    xorq    16(%rsp), %rcx
    xorq    (%rsp), %r11
    movq    %r10, %rdx
    movq    %rsi, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    %rcx, %rbp
    xorq    %r8, %rax
    xorq    24(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
    xorq    %rbp, %r12
    xorq    %r11, %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rsi, %r11
    movq    %r8, %rsi
    movq    -120(%rsp), %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rsi
    movq    -16(%rsp), %rcx
    xorq    %r11, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %r10
    xorq    %rsi, %r9
    xorq    %rdx, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rax, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %rcx, %r10
    movq    %r10, -120(%rsp)
    movabsq $-9223372036854742912, %r10
    xorq    %r10, -120(%rsp)
    movq    %r12, %r10
    notq    %r10
    orq %r13, %r10
    xorq    %rcx, %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, -120(%rsp)
    movq    %r10, -64(%rsp)
    movq    %r9, %r10
    andq    %r13, %r10
    xorq    %r12, %r10
    movq    %r8, %r12
    andq    %rcx, %r8
    xorq    %r9, %r8
    orq %r9, %r12
    movq    -48(%rsp), %r9
    movq    %r8, 56(%rsp)
    movq    16(%rsp), %r8
    xorq    %r13, %r12
    movq    32(%rsp), %rcx
    movq    %r10, 40(%rsp)
    movq    %r12, -16(%rsp)
    xorq    %r11, %r8
    xorq    %rdx, %r9
    xorq    %rax, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r13
    xorq    %rsi, %rcx
    xorq    %rbp, %r15
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rbx, %r10
    orq %rcx, %r13
    xorq    %rdx, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r9, %r10
    xorq    %r8, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r13, 16(%rsp)
    xorq    %rcx, %r10
    movq    %r15, %r12
    andq    %r8, %rcx
    movq    %r15, %r13
    notq    %r12
    xorq    %r15, %rcx
    orq %r8, %r13
    orq %rbx, %r12
    xorq    %rbx, %r13
    movq    %rcx, 64(%rsp)
    movq    -80(%rsp), %rbx
    movq    (%rsp), %rcx
    movq    %r10, -48(%rsp)
    xorq    %r9, %r12
    movq    -112(%rsp), %r10
    movq    %r12, -56(%rsp)
    movq    -32(%rsp), %r9
    movq    %r13, 32(%rsp)
    xorq    %r11, %rbx
    xorq    %rsi, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rbx,%rbx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rbx, %r15
    movq    %rcx, %r8
    xorq    %rbp, %r10
    andq    %rbx, %r8
    xorq    %rax, %r9
    notq    %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r10, %r15
    movq    %rcx, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r9, %r15
    xorq    %r10, %r8
    andq    %r9, %r10
    xorq    %r14, %r10
    andq    %r14, %r12
    movq    %r14, %r13
    movq    %r10, 72(%rsp)
    movq    -104(%rsp), %r10
    xorq    %rbx, %r12
    orq %r9, %r13
    movq    -72(%rsp), %rbx
    movq    %r8, -80(%rsp)
    xorq    %rcx, %r13
    movq    24(%rsp), %rcx
    movq    %r15, -112(%rsp)
    movq    %r13, (%rsp)
    movq    -40(%rsp), %r13
    xorq    %rax, %r10
    movq    -24(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r14
    xorq    %rdx, %rbx
    xorq    %rsi, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rbx,%rbx
# 0 "" 2
#NO_APP
    andq    %rbx, %r14
    xorq    %rbp, %r13
    xorq    %r11, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r14
    xorq    48(%rsp), %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r14, -40(%rsp)
    movq    %r13, %r8
    movq    %r13, %r14
    orq %r10, %r8
    notq    %r14
    xorq    -8(%rsp), %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rbx, %r8
    movq    %r14, %r15
    orq %rcx, %rbx
    movq    %r9, %r13
    xorq    %r9, %rbx
    orq %r9, %r15
    andq    %rcx, %r13
    movq    -112(%rsp), %r9
    xorq    %r10, %r15
    movq    8(%rsp), %rcx
    xorq    %r14, %r13
    movq    %rbx, -72(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r10
    movq    %r15, -24(%rsp)
    xorq    %rbp, %rcx
    notq    %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    -88(%rsp), %rdx
    movq    %r10, %r14
    andq    %rcx, %r11
    xorq    16(%rsp), %r9
    andq    %rsi, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %rbx
    xorq    %rcx, %r14
    xorq    -96(%rsp), %rax
    orq %rsi, %rbx
    xorq    -40(%rsp), %r9
    xorq    %r10, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r15
    xorq    %rax, %r11
    andq    %rdx, %r15
    xorq    %r14, %r9
    xorq    %rsi, %r15
    movq    %rax, %rsi
    xorq    -120(%rsp), %r9
    movq    %rbx, 24(%rsp)
    orq %rcx, %rsi
    movq    -80(%rsp), %rbp
    xorq    %rdx, %rsi
    movq    %rsi, 8(%rsp)
    movq    (%rsp), %rsi
    movq    72(%rsp), %r10
    xorq    -48(%rsp), %rbp
    xorq    32(%rsp), %rsi
    xorq    64(%rsp), %r10
    xorq    %r8, %rbp
    xorq    %rbx, %rbp
    movq    40(%rsp), %rbx
    xorq    %r13, %rsi
    xorq    8(%rsp), %rsi
    xorq    -72(%rsp), %r10
    xorq    -64(%rsp), %rbp
    xorq    %r15, %rbx
    xorq    -56(%rsp), %rbx
    xorq    -16(%rsp), %rsi
    xorq    %r11, %r10
    movq    %rbp, %rcx
    xorq    %r12, %rbx
    xorq    -24(%rsp), %rbx
    movq    %rsi, %rdx
    xorq    56(%rsp), %r10
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rbp, %rdx
    movq    %rbx, %rax
    xorq    %rdx, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
    movq    -48(%rsp), %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r12, %rbp
    xorq    %r9, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rax, %rsi
    xorq    %r10, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %rsi, %rbp
    movq    %rbp, -32(%rsp)
    movl    $2147483649, %ebp
    xorq    %rbp, -32(%rsp)
    movq    %r12, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    notq    %rbp
    xorq    %rbx, %r10
    movq    -120(%rsp), %rbx
    xorq    %r10, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r13,%r13
# 0 "" 2
#NO_APP
    orq %r13, %rbp
    xorq    %rsi, %rbp
    xorq    %rcx, %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rbx, -32(%rsp)
    movq    %rbp, -120(%rsp)
    movq    %r11, %rbp
    andq    %r13, %rbp
    xorq    %r12, %rbp
    movq    %rbx, %r12
    andq    %rsi, %rbx
    xorq    %r11, %rbx
    movq    -16(%rsp), %rsi
    orq %r11, %r12
    movq    %rbx, -8(%rsp)
    movq    64(%rsp), %r11
    xorq    %r13, %r12
    movq    -112(%rsp), %rbx
    movq    %rbp, 48(%rsp)
    movq    %r12, -48(%rsp)
    xorq    %r10, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    xorq    %rcx, %rbx
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    xorq    %rdx, %r15
    xorq    %rcx, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r11, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rsi, %r13
    movq    %r8, %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r13, -88(%rsp)
    andq    %rbx, %rbp
    movq    %r15, %r13
    xorq    %r11, %rbp
    notq    %r13
    andq    %rsi, %r11
    xorq    %r15, %r11
    orq %r8, %r13
    movq    %r15, %r12
    xorq    %rbx, %r13
    orq %rsi, %r12
    movq    %r11, -104(%rsp)
    movq    -72(%rsp), %rbx
    xorq    %r8, %r12
    movq    %rbp, -112(%rsp)
    movq    (%rsp), %r11
    movq    %r12, -16(%rsp)
    movq    -56(%rsp), %r8
    movq    -64(%rsp), %rsi
    xorq    %r9, %rbx
    xorq    %r10, %r11
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbx,%rbx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %rbx, %rbp
    movq    %r11, %r15
    notq    %rbx
    xorq    %rdx, %r8
    andq    %r11, %rbp
    movq    %rbx, %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r15
    xorq    %r8, %rbp
    xorq    %rax, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %r14, %r12
    xorq    %rsi, %r15
    movq    %r15, -64(%rsp)
    xorq    %r11, %r12
    movq    %r14, %r15
    movq    %rbp, (%rsp)
    andq    %rsi, %r8
    movq    -24(%rsp), %rbp
    xorq    %r14, %r8
    orq %rsi, %r15
    movq    -80(%rsp), %r11
    xorq    %rbx, %r15
    movq    %r8, 64(%rsp)
    movq    16(%rsp), %rbx
    movq    8(%rsp), %rsi
    movq    %r12, -72(%rsp)
    movq    56(%rsp), %r8
    xorq    %rdx, %rbp
    movq    %r15, -96(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r14
    notq    %rbp
    xorq    %rax, %r11
    movq    %rbp, %r15
    xorq    %rcx, %rbx
    xorq    %r10, %rsi
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r11, %r14
    movq    %r11, %r12
    xorq    %r9, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rsi, %r15
    xorq    %rbx, %r14
    andq    %rbx, %r12
    orq %r8, %rbx
    xorq    %r11, %r15
    movq    %rsi, %r11
    xorq    %rsi, %rbx
    movq    -64(%rsp), %rsi
    andq    %r8, %r11
    xorq    %r8, %r12
    xorq    %rbp, %r11
    xorq    40(%rsp), %rdx
    xorq    32(%rsp), %r10
    movq    %rbx, 8(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    72(%rsp), %r9
    movq    %r10, %r8
    movq    %r12, 16(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    -40(%rsp), %rcx
    notq    %r8
    movq    %r15, -24(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    24(%rsp), %rax
    movq    %rcx, %rbx
    movq    %r8, %r15
    xorq    -88(%rsp), %rsi
    orq %r9, %rbx
    andq    %r9, %r15
    movq    (%rsp), %rbp
    xorq    %r8, %rbx
    movq    %r14, -56(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r8
    movq    %rbx, 24(%rsp)
    andq    %rdx, %r10
    xorq    %r12, %rsi
    movq    64(%rsp), %r12
    andq    %rcx, %r8
    xorq    -112(%rsp), %rbp
    xorq    %r9, %r8
    xorq    %rax, %r10
    movq    -96(%rsp), %r9
    xorq    %rdx, %r15
    xorq    %r15, %rsi
    xorq    -104(%rsp), %r12
    xorq    %r14, %rbp
    movq    %rax, %r14
    xorq    -32(%rsp), %rsi
    xorq    -16(%rsp), %r9
    xorq    %rbx, %rbp
    orq %rdx, %r14
    movq    48(%rsp), %rbx
    xorq    %rcx, %r14
    xorq    8(%rsp), %r12
    xorq    -120(%rsp), %rbp
    xorq    %r11, %r9
    xorq    %r8, %rbx
    xorq    %r14, %r9
    xorq    %r13, %rbx
    xorq    %r10, %r12
    xorq    -48(%rsp), %r9
    xorq    -72(%rsp), %rbx
    movq    %rbp, %rcx
    xorq    -8(%rsp), %r12
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r9, %rdx
    xorq    -24(%rsp), %rbx
    xorq    %r12, %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rbp, %rdx
    movq    -32(%rsp), %rbp
    movq    %rbx, %rax
    xorq    %rdx, %r8
    xorq    %rdx, %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rax
    xorq    %rcx, %rbp
    movq    %rbp, -32(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %rsi
    movq    -112(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rbx, %r12
    movq    -72(%rsp), %rbx
    xorq    %rsi, %r10
    xorq    %r12, %r11
    xorq    %rax, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rbp
    movq    %r9, -112(%rsp)
    xorq    %rdx, %rbx
    movabsq $-9223372034707259384, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    xorq    %r9, %rbp
    movq    -48(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r11,%r11
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r12, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, -48(%rsp)
    movq    -104(%rsp), %r9
    xorq    %rsi, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, -104(%rsp)
    movq    -64(%rsp), %r9
    xorq    %rcx, %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r9, -64(%rsp)
    movq    -56(%rsp), %r9
    movq    %r8, -80(%rsp)
    movq    -120(%rsp), %r8
    xorq    %rax, %r9
    xorq    %rax, %r8
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, -40(%rsp)
    movq    8(%rsp), %r13
    xorq    %rcx, %r15
    movq    %r9, -56(%rsp)
    movq    -96(%rsp), %r9
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    -32(%rsp), %rbp
    movq    %r8, -120(%rsp)
    xorq    %rsi, %r13
    movq    -88(%rsp), %r8
    movq    %r15, -72(%rsp)
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, 8(%rsp)
    notq    %r13
    xorq    %r12, %r9
    movq    %r13, 40(%rsp)
    movq    -24(%rsp), %r13
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r9,%r9
# 0 "" 2
#NO_APP
    movq    -8(%rsp), %r15
    movq    %r9, -96(%rsp)
    xorq    %rcx, %r8
    movq    (%rsp), %r9
    xorq    %rdx, %r13
    xorq    16(%rsp), %rcx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, -24(%rsp)
    notq    %r13
    xorq    48(%rsp), %rdx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rsi, %r15
    movq    %r8, -88(%rsp)
    xorq    64(%rsp), %rsi
    xorq    %rax, %r9
    movq    %r13, (%rsp)
    xorq    24(%rsp), %rax
    movq    %r12, %r8
    xorq    -16(%rsp), %r12
    movq    %rbp, (%rdi)
    movq    %rbx, %rbp
    movq    -32(%rsp), %r13
    xorq    %r14, %r8
    notq    %rbp
    orq %r11, %rbp
    xorq    -112(%rsp), %rbp
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%r15,%r15
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %rbp, 8(%rdi)
    movq    %r10, %rbp
    andq    %r11, %rbp
    xorq    %rbx, %rbp
    movq    -32(%rsp), %rbx
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r10, %rbx
    movq    %rbp, 16(%rdi)
    movq    %r12, %r14
#APP
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rcx,%rcx
# 0 "" 2
# 298 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r11, %rbx
    andq    -112(%rsp), %r13
    notq    %r14
    movq    %rbx, 24(%rdi)
    xorq    %r10, %r13
    movq    -64(%rsp), %r10
    movq    %r13, 32(%rdi)
    orq -104(%rsp), %r10
    xorq    -48(%rsp), %r10
    movq    %r10, 40(%rdi)
    movq    -56(%rsp), %r10
    andq    -64(%rsp), %r10
    xorq    -104(%rsp), %r10
    movq    %r10, 48(%rdi)
    movq    -80(%rsp), %r10
    notq    %r10
    orq -56(%rsp), %r10
    xorq    -64(%rsp), %r10
    movq    %r10, 56(%rdi)
    movq    -80(%rsp), %r10
    orq -48(%rsp), %r10
    xorq    -56(%rsp), %r10
    movq    %r10, 64(%rdi)
    movq    -104(%rsp), %r10
    andq    -48(%rsp), %r10
    xorq    -80(%rsp), %r10
    movq    %r10, 72(%rdi)
    movq    -96(%rsp), %r10
    orq -40(%rsp), %r10
    xorq    -120(%rsp), %r10
    movq    %r10, 80(%rdi)
    movq    8(%rsp), %r10
    andq    -96(%rsp), %r10
    xorq    -40(%rsp), %r10
    movq    %r10, 88(%rdi)
    movq    40(%rsp), %r10
    andq    -72(%rsp), %r10
    xorq    -96(%rsp), %r10
    movq    %r10, 96(%rdi)
    movq    -72(%rsp), %r10
    orq -120(%rsp), %r10
    xorq    40(%rsp), %r10
    movq    %r10, 104(%rdi)
    movq    -40(%rsp), %r10
    andq    -120(%rsp), %r10
    xorq    -72(%rsp), %r10
    movq    %r10, 112(%rdi)
    movq    -88(%rsp), %r10
    andq    %r9, %r10
    xorq    %r15, %r10
    movq    %r10, 120(%rdi)
    movq    -24(%rsp), %r10
    orq %r9, %r10
    xorq    -88(%rsp), %r10
    movq    %r10, 128(%rdi)
    movq    (%rsp), %r10
    orq %r8, %r10
    xorq    %r9, %r10
    movq    %r8, %r9
    andq    %r15, %r9
    movq    %r10, 136(%rdi)
    xorq    (%rsp), %r9
    movq    %r9, 144(%rdi)
    movq    -88(%rsp), %r9
    orq %r15, %r9
    xorq    %r8, %r9
    movq    %r14, %r8
    andq    %rsi, %r8
    movq    %r9, 152(%rdi)
    xorq    %rdx, %r8
    movq    %r8, 160(%rdi)
    movq    %rcx, %r8
    orq %rsi, %r8
    xorq    %r14, %r8
    movq    %r8, 168(%rdi)
    movq    %rax, %r8
    andq    %rcx, %r8
    xorq    %rsi, %r8
    movq    %rax, %rsi
    orq %rdx, %rsi
    andq    %r12, %rdx
    movq    %r8, 176(%rdi)
    xorq    %rcx, %rsi
    xorq    %rax, %rdx
    movq    %rsi, 184(%rdi)
    movq    %rdx, 192(%rdi)
    addq    $80, %rsp
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
.LFE33:
    .size   KeccakP1600_Permute_12rounds, .-KeccakP1600_Permute_12rounds
    .p2align 4,,15
    .globl  KeccakP1600_ExtractBytesInLane
    .type   KeccakP1600_ExtractBytesInLane, @function
KeccakP1600_ExtractBytesInLane:
.LFB34:
    .cfi_startproc
    movq    %rdx, %r9
    subq    $24, %rsp
    .cfi_def_cfa_offset 32
    movl    %esi, %eax
    leal    -1(%rsi), %edx
    movq    (%rdi,%rax,8), %rax
    cmpl    $1, %edx
    jbe .L162
    cmpl    $8, %esi
    jne .L165
.L162:
    notq    %rax
.L163:
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
.L165:
    .cfi_restore_state
    cmpl    $17, %esi
    je  .L162
    cmpl    $12, %esi
    .p2align 4,,2
    je  .L162
    cmpl    $20, %esi
    .p2align 4,,2
    jne .L163
    .p2align 4,,5
    jmp .L162
    .cfi_endproc
.LFE34:
    .size   KeccakP1600_ExtractBytesInLane, .-KeccakP1600_ExtractBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_ExtractLanes
    .type   KeccakP1600_ExtractLanes, @function
KeccakP1600_ExtractLanes:
.LFB35:
    .cfi_startproc
    movq    %rbx, -16(%rsp)
    .cfi_offset 3, -24
    movq    %rsi, %rbx
    movq    %rdi, %rsi
    movq    %rbp, -8(%rsp)
    movq    %rbx, %rdi
    .cfi_offset 6, -16
    movl    %edx, %ebp
    leal    0(,%rdx,8), %edx
    subq    $24, %rsp
    .cfi_def_cfa_offset 32
    call    memcpy
    cmpl    $1, %ebp
    jbe .L166
    notq    8(%rbx)
    cmpl    $2, %ebp
    je  .L166
    notq    16(%rbx)
    cmpl    $8, %ebp
    jbe .L166
    notq    64(%rbx)
    cmpl    $12, %ebp
    jbe .L166
    notq    96(%rbx)
    cmpl    $17, %ebp
    jbe .L166
    notq    136(%rbx)
    cmpl    $20, %ebp
    jbe .L166
    notq    160(%rbx)
    .p2align 4,,10
    .p2align 3
.L166:
    movq    8(%rsp), %rbx
    movq    16(%rsp), %rbp
    addq    $24, %rsp
    .cfi_def_cfa_offset 8
    ret
    .cfi_endproc
.LFE35:
    .size   KeccakP1600_ExtractLanes, .-KeccakP1600_ExtractLanes
    .p2align 4,,15
    .globl  KeccakP1600_ExtractBytes
    .type   KeccakP1600_ExtractBytes, @function
KeccakP1600_ExtractBytes:
.LFB36:
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
    je  .L190
    movl    %edx, %ebx
    movl    %edx, %r8d
    shrl    $3, %ebx
    andl    $7, %r8d
    testl   %ecx, %ecx
    je  .L174
    movl    %ecx, %ebp
    jmp .L186
    .p2align 4,,10
    .p2align 3
.L191:
    cmpl    $8, %ebx
    je  .L184
    cmpl    $17, %ebx
    .p2align 4,,3
    je  .L184
    cmpl    $12, %ebx
    .p2align 4,,2
    je  .L184
    cmpl    $20, %ebx
    .p2align 4,,2
    je  .L184
    .p2align 4,,10
    .p2align 3
.L185:
    movl    %r12d, %r14d
    movq    %r15, %rdi
    movq    %rax, 16(%rsp)
    leaq    16(%rsp), %rsi
    movq    %r14, %rdx
    addq    %r8, %rsi
    call    memcpy
    addl    $1, %ebx
    addq    %r14, %r15
    xorl    %r8d, %r8d
    subl    %r12d, %ebp
    je  .L174
.L186:
    leal    -1(%rbx), %edx
    movl    $8, %r12d
    movl    %ebx, %eax
    subl    %r8d, %r12d
    movq    0(%r13,%rax,8), %rax
    cmpl    %ebp, %r12d
    cmova   %ebp, %r12d
    cmpl    $1, %edx
    ja  .L191
.L184:
    notq    %rax
    jmp .L185
.L178:
    movq    16(%r13), %rax
    leaq    (%r15,%r12), %rdi
    movl    %ecx, %ebp
    andl    $7, %ebp
.L180:
    notq    %rax
.L181:
    leaq    16(%rsp), %rsi
    movl    %ebp, %edx
    movq    %rax, 16(%rsp)
    call    memcpy
.L174:
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
.L190:
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
    jbe .L177
    notq    8(%r15)
    cmpl    $2, %ebx
    je  .L178
    notq    16(%r15)
    cmpl    $8, %ebx
    jbe .L177
    notq    64(%r15)
    cmpl    $12, %ebx
    jbe .L177
    notq    96(%r15)
    cmpl    $17, %ebx
    jbe .L177
    notq    136(%r15)
    cmpl    $20, %ebx
    jbe .L177
    notq    160(%r15)
    .p2align 4,,10
    .p2align 3
.L177:
    leal    -1(%rbx), %edx
    movl    %ecx, %ebp
    movl    %ebx, %eax
    leaq    (%r15,%r12), %rdi
    andl    $7, %ebp
    cmpl    $1, %edx
    movq    0(%r13,%rax,8), %rax
    jbe .L180
    cmpl    $8, %ebx
    je  .L180
    cmpl    $17, %ebx
    je  .L180
    cmpl    $12, %ebx
    .p2align 4,,2
    je  .L180
    cmpl    $20, %ebx
    .p2align 4,,2
    jne .L181
    .p2align 4,,5
    jmp .L180
    .cfi_endproc
.LFE36:
    .size   KeccakP1600_ExtractBytes, .-KeccakP1600_ExtractBytes
    .p2align 4,,15
    .globl  KeccakP1600_ExtractAndAddBytesInLane
    .type   KeccakP1600_ExtractAndAddBytesInLane, @function
KeccakP1600_ExtractAndAddBytesInLane:
.LFB37:
    .cfi_startproc
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movl    %esi, %eax
    movq    %rsp, %rbp
    .cfi_def_cfa_register 6
    andq    $-32, %rsp
    addq    $16, %rsp
    movq    (%rdi,%rax,8), %rax
    leal    -1(%rsi), %edi
    cmpl    $1, %edi
    jbe .L193
    cmpl    $8, %esi
    jne .L201
.L193:
    notq    %rax
.L194:
    movq    %rax, -48(%rsp)
    xorl    %eax, %eax
    testl   %r9d, %r9d
    je  .L192
    .p2align 4,,10
    .p2align 3
.L198:
    leal    (%r8,%rax), %esi
    movzbl  -48(%rsp,%rsi), %esi
    xorb    (%rdx,%rax), %sil
    movb    %sil, (%rcx,%rax)
    addq    $1, %rax
    cmpl    %eax, %r9d
    ja  .L198
.L192:
    leave
    .cfi_remember_state
    .cfi_def_cfa 7, 8
    ret
    .p2align 4,,10
    .p2align 3
.L201:
    .cfi_restore_state
    cmpl    $17, %esi
    je  .L193
    cmpl    $12, %esi
    .p2align 4,,4
    je  .L193
    cmpl    $20, %esi
    .p2align 4,,2
    jne .L194
    .p2align 4,,5
    jmp .L193
    .cfi_endproc
.LFE37:
    .size   KeccakP1600_ExtractAndAddBytesInLane, .-KeccakP1600_ExtractAndAddBytesInLane
    .p2align 4,,15
    .globl  KeccakP1600_ExtractAndAddLanes
    .type   KeccakP1600_ExtractAndAddLanes, @function
KeccakP1600_ExtractAndAddLanes:
.LFB38:
    .cfi_startproc
    testl   %ecx, %ecx
    je  .L202
    leaq    32(%rdx), %r9
    movl    %ecx, %r10d
    leaq    32(%rsi), %r11
    shrl    $2, %r10d
    cmpq    %r9, %rsi
    leal    0(,%r10,4), %r8d
    setae   %al
    cmpq    %r11, %rdx
    setae   %r11b
    orl %r11d, %eax
    cmpq    %r9, %rdi
    leaq    32(%rdi), %r11
    setae   %r9b
    cmpq    %r11, %rdx
    setae   %r11b
    orl %r11d, %r9d
    andl    %r9d, %eax
    cmpl    $6, %ecx
    seta    %r9b
    testb   %r9b, %al
    je  .L210
    testl   %r8d, %r8d
    je  .L210
    xorl    %eax, %eax
    xorl    %r9d, %r9d
    .p2align 4,,10
    .p2align 3
.L206:
    vmovdqu (%rdi,%rax), %xmm1
    addl    $1, %r9d
    vmovdqu (%rsi,%rax), %xmm0
    vinsertf128 $0x1, 16(%rdi,%rax), %ymm1, %ymm1
    vinsertf128 $0x1, 16(%rsi,%rax), %ymm0, %ymm0
    vxorps  %ymm0, %ymm1, %ymm0
    vmovdqu %xmm0, (%rdx,%rax)
    vextractf128    $0x1, %ymm0, 16(%rdx,%rax)
    addq    $32, %rax
    cmpl    %r9d, %r10d
    ja  .L206
    cmpl    %r8d, %ecx
    je  .L207
    .p2align 4,,10
    .p2align 3
.L213:
    movl    %r8d, %eax
    addl    $1, %r8d
    movq    (%rdi,%rax,8), %r9
    xorq    (%rsi,%rax,8), %r9
    cmpl    %r8d, %ecx
    movq    %r9, (%rdx,%rax,8)
    ja  .L213
    cmpl    $1, %ecx
    jbe .L202
.L207:
    notq    8(%rdx)
    cmpl    $2, %ecx
    je  .L202
    notq    16(%rdx)
    cmpl    $8, %ecx
    jbe .L202
    notq    64(%rdx)
    cmpl    $12, %ecx
    jbe .L202
    notq    96(%rdx)
    cmpl    $17, %ecx
    jbe .L202
    notq    136(%rdx)
    cmpl    $20, %ecx
    jbe .L202
    notq    160(%rdx)
    .p2align 4,,10
    .p2align 3
.L202:
    vzeroupper
    ret
.L210:
    xorl    %r8d, %r8d
    jmp .L213
    .cfi_endproc
.LFE38:
    .size   KeccakP1600_ExtractAndAddLanes, .-KeccakP1600_ExtractAndAddLanes
    .p2align 4,,15
    .globl  KeccakP1600_ExtractAndAddBytes
    .type   KeccakP1600_ExtractAndAddBytes, @function
KeccakP1600_ExtractAndAddBytes:
.LFB39:
    .cfi_startproc
    pushq   %rbp
    .cfi_def_cfa_offset 16
    .cfi_offset 6, -16
    movq    %rsp, %rbp
    .cfi_def_cfa_register 6
    pushq   %rbx
    andq    $-32, %rsp
    addq    $8, %rsp
    testl   %ecx, %ecx
    .cfi_offset 3, -24
    je  .L252
    movl    %ecx, %r11d
    movl    %ecx, %r10d
    movl    $8, %ebx
    shrl    $3, %r11d
    andl    $7, %r10d
    testl   %r8d, %r8d
    je  .L217
    .p2align 4,,10
    .p2align 3
.L244:
    leal    -1(%r11), %ecx
    movl    %ebx, %r9d
    movl    %r11d, %eax
    subl    %r10d, %r9d
    movq    (%rdi,%rax,8), %rax
    cmpl    %r8d, %r9d
    cmova   %r8d, %r9d
    cmpl    $1, %ecx
    jbe .L233
    cmpl    $8, %r11d
    je  .L233
    cmpl    $17, %r11d
    je  .L233
    cmpl    $12, %r11d
    je  .L233
    cmpl    $20, %r11d
    je  .L233
    .p2align 4,,10
    .p2align 3
.L234:
    movq    %rax, -40(%rsp)
    xorl    %eax, %eax
    .p2align 4,,10
    .p2align 3
.L235:
    leal    (%r10,%rax), %ecx
    movzbl  -40(%rsp,%rcx), %ecx
    xorb    (%rsi,%rax), %cl
    movb    %cl, (%rdx,%rax)
    addq    $1, %rax
    cmpl    %eax, %r9d
    ja  .L235
    movl    %r9d, %eax
    addl    $1, %r11d
    xorl    %r10d, %r10d
    addq    %rax, %rsi
    addq    %rax, %rdx
    subl    %r9d, %r8d
    jne .L244
.L217:
    movq    -8(%rbp), %rbx
    leave
    .cfi_remember_state
    .cfi_def_cfa 7, 8
    vzeroupper
    ret
    .p2align 4,,10
    .p2align 3
.L233:
    .cfi_restore_state
    notq    %rax
    jmp .L234
.L252:
    movl    %r8d, %r10d
    shrl    $3, %r10d
    testl   %r10d, %r10d
    je  .L220
    leaq    32(%rdx), %r9
    movl    %r8d, %r11d
    leaq    32(%rsi), %rbx
    shrl    $5, %r11d
    cmpq    %r9, %rsi
    leal    0(,%r11,4), %ecx
    setae   %al
    cmpq    %rbx, %rdx
    setae   %bl
    orl %ebx, %eax
    cmpl    $6, %r10d
    seta    %bl
    andl    %ebx, %eax
    cmpq    %r9, %rdi
    leaq    32(%rdi), %rbx
    setae   %r9b
    cmpq    %rbx, %rdx
    setae   %bl
    orl %ebx, %r9d
    testb   %r9b, %al
    je  .L238
    testl   %ecx, %ecx
    je  .L238
    xorl    %eax, %eax
    xorl    %r9d, %r9d
    .p2align 4,,10
    .p2align 3
.L222:
    vmovdqu (%rdi,%rax), %xmm1
    addl    $1, %r9d
    vmovdqu (%rsi,%rax), %xmm0
    vinsertf128 $0x1, 16(%rdi,%rax), %ymm1, %ymm1
    vinsertf128 $0x1, 16(%rsi,%rax), %ymm0, %ymm0
    vxorps  %ymm0, %ymm1, %ymm0
    vmovdqu %xmm0, (%rdx,%rax)
    vextractf128    $0x1, %ymm0, 16(%rdx,%rax)
    addq    $32, %rax
    cmpl    %r11d, %r9d
    jb  .L222
    cmpl    %ecx, %r10d
    je  .L223
    .p2align 4,,10
    .p2align 3
.L243:
    movl    %ecx, %eax
    addl    $1, %ecx
    movq    (%rdi,%rax,8), %r9
    xorq    (%rsi,%rax,8), %r9
    cmpl    %ecx, %r10d
    movq    %r9, (%rdx,%rax,8)
    ja  .L243
    cmpl    $1, %r10d
    je  .L220
.L223:
    notq    8(%rdx)
    cmpl    $2, %r10d
    je  .L226
    notq    16(%rdx)
    cmpl    $8, %r10d
    jbe .L220
    notq    64(%rdx)
    cmpl    $12, %r10d
    jbe .L220
    notq    96(%rdx)
    cmpl    $17, %r10d
    jbe .L220
    notq    136(%rdx)
    cmpl    $20, %r10d
    jbe .L220
    notq    160(%rdx)
.L220:
    leal    0(,%r10,8), %eax
    andl    $7, %r8d
    leal    -1(%r10), %ecx
    addq    %rax, %rdx
    addq    %rax, %rsi
    movl    %r10d, %eax
    cmpl    $1, %ecx
    movq    (%rdi,%rax,8), %rax
    jbe .L227
    cmpl    $8, %r10d
    je  .L227
    cmpl    $17, %r10d
    je  .L227
    cmpl    $12, %r10d
    jne .L253
.L227:
    notq    %rax
.L228:
    movq    %rax, -40(%rsp)
    xorl    %eax, %eax
    testl   %r8d, %r8d
    je  .L217
    .p2align 4,,10
    .p2align 3
.L242:
    movl    %eax, %ecx
    addl    $1, %eax
    movzbl  -40(%rsp,%rcx), %edi
    xorb    (%rsi,%rcx), %dil
    cmpl    %eax, %r8d
    movb    %dil, (%rdx,%rcx)
    ja  .L242
    movq    -8(%rbp), %rbx
    leave
    .cfi_remember_state
    .cfi_def_cfa 7, 8
    vzeroupper
    ret
.L253:
    .cfi_restore_state
    cmpl    $20, %r10d
    jne .L228
    jmp .L227
.L226:
    andl    $7, %r8d
    addq    $16, %rdx
    addq    $16, %rsi
    movq    16(%rdi), %rax
    jmp .L227
.L238:
    xorl    %ecx, %ecx
    jmp .L243
    .cfi_endproc
.LFE39:
    .size   KeccakP1600_ExtractAndAddBytes, .-KeccakP1600_ExtractAndAddBytes
    .p2align 4,,15
    .globl  KeccakF1600_FastLoop_Absorb
    .type   KeccakF1600_FastLoop_Absorb, @function
KeccakF1600_FastLoop_Absorb:
.LFB40:
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
    movq    %rdi, %rbp
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $136, %rsp
    .cfi_def_cfa_offset 192
    movq    8(%rdi), %rax
    movl    %esi, 100(%rsp)
    movq    32(%rdi), %rbx
    movq    %rcx, 128(%rsp)
    movq    40(%rdi), %rsi
    movq    %rdi, 88(%rsp)
    movq    16(%rdi), %rcx
    movq    56(%rbp), %r8
    movq    %rax, (%rsp)
    movq    64(%rbp), %r9
    movq    %rbx, 8(%rsp)
    movq    80(%rbp), %r10
    movq    %rsi, 40(%rsp)
    movq    88(%rbp), %r11
    movq    %rcx, -104(%rsp)
    movq    96(%rbp), %r15
    movq    %r8, -48(%rsp)
    movq    104(%rbp), %rax
    movq    %r9, -8(%rsp)
    movq    (%rdi), %r12
    movq    %r10, -40(%rsp)
    movq    24(%rdi), %r14
    movq    %r11, -24(%rsp)
    movq    72(%rbp), %r13
    movq    %r15, -112(%rsp)
    movq    48(%rdi), %rdi
    movq    %rax, -64(%rsp)
    movq    88(%rsp), %r8
    movq    112(%rbp), %rcx
    movq    120(%rbp), %rbx
    movq    128(%rbp), %rsi
    movq    184(%r8), %rax
    movq    168(%r8), %r9
    movq    %rcx, -80(%rsp)
    movq    136(%rbp), %rbp
    movq    %rbx, -88(%rsp)
    movq    176(%r8), %r11
    movq    %rax, -32(%rsp)
    movl    100(%rsp), %eax
    movq    %r9, 24(%rsp)
    movq    144(%r8), %r10
    movq    %rbp, -56(%rsp)
    movq    152(%r8), %r15
    movq    160(%r8), %rcx
    movq    %r11, 32(%rsp)
    sall    $3, %eax
    movq    192(%r8), %r9
    cmpq    %rax, 128(%rsp)
    movq    %rax, 112(%rsp)
    jb  .L271
    movl    100(%rsp), %eax
    movq    %r12, -120(%rsp)
    movq    %r9, %r11
    movq    128(%rsp), %rbx
    movq    %r14, -96(%rsp)
    movq    %rdi, %rbp
    movq    %rdx, 72(%rsp)
    movq    %r10, %r12
    movq    %rcx, %r14
    movq    %rsi, %r9
    salq    $3, %rax
    subq    112(%rsp), %rbx
    movq    %rax, 120(%rsp)
    cmpl    $21, 100(%rsp)
    movq    %rbx, 104(%rsp)
    movq    %r13, %rbx
    je  .L313
    .p2align 4,,10
    .p2align 3
.L256:
    cmpl    $15, 100(%rsp)
    ja  .L258
    cmpl    $7, 100(%rsp)
    ja  .L259
    cmpl    $3, 100(%rsp)
    ja  .L260
    cmpl    $1, 100(%rsp)
    jbe .L314
    movq    72(%rsp), %r8
    movq    72(%rsp), %r10
    movq    (%r8), %r8
    movq    8(%r10), %r10
    xorq    %r8, -120(%rsp)
    xorq    %r10, (%rsp)
    cmpl    $2, 100(%rsp)
    jne .L315
    .p2align 4,,10
    .p2align 3
.L257:
    movq    -48(%rsp), %r10
    movq    40(%rsp), %rsi
    movq    -8(%rsp), %rdi
    movq    (%rsp), %r8
    xorq    -104(%rsp), %r10
    xorq    -120(%rsp), %rsi
    xorq    -96(%rsp), %rdi
    movq    8(%rsp), %r13
    xorq    %rbp, %r8
    xorq    -112(%rsp), %r10
    xorq    -40(%rsp), %rsi
    xorq    -64(%rsp), %rdi
    xorq    -24(%rsp), %r8
    xorq    %rbx, %r13
    xorq    -56(%rsp), %r10
    xorq    -88(%rsp), %rsi
    xorq    %r12, %rdi
    xorq    -80(%rsp), %r13
    xorq    -32(%rsp), %rdi
    xorq    %r9, %r8
    xorq    32(%rsp), %r10
    xorq    24(%rsp), %r8
    xorq    %r14, %rsi
    xorq    %r15, %r13
    xorq    %r11, %r13
    movq    %rdi, %rdx
    movq    %r10, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rax
    movq    %r8, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdi, %rsi
    movq    %rax, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rbp, %rdi
    xorq    %r13, %rcx
    movq    -112(%rsp), %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %r10, %r13
    movq    -120(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r8, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r10
    xorq    %rdx, %rbp
    xorq    %r13, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r8
    xorq    %rsi, %r11
    xorq    %rsi, %rbx
    orq %rdi, %r8
    xorq    $1, %r8
    xorq    %r10, %r8
    movq    %r8, -120(%rsp)
    movq    %rbp, %r8
    notq    %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r8
    xorq    %rdi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r8, -112(%rsp)
    movq    %r11, %r8
    andq    %r12, %r8
    xorq    %rbp, %r8
    movq    %r10, %rbp
    andq    %rdi, %r10
    xorq    %r11, %r10
    orq %r11, %rbp
    movq    %r8, 16(%rsp)
    movq    %r10, -72(%rsp)
    movq    %rax, %r10
    movq    -40(%rsp), %r8
    xorq    %r9, %r10
    movq    32(%rsp), %r9
    xorq    %r12, %rbp
    movq    -96(%rsp), %rdi
    movq    %rbp, 48(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rcx, %r8
    movq    %r10, %r12
    xorq    %rdx, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rbp
    xorq    %r13, %rdi
    notq    %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r10, %rbp
    movq    %r8, %r11
    andq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    orq %rbx, %r11
    movq    %r9, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rbx, %r12
    xorq    %rdi, %r11
    orq %rdi, %r8
    xorq    %r10, %r8
    andq    %rdi, %rbx
    movq    -64(%rsp), %r10
    xorq    %r9, %rbx
    movq    -48(%rsp), %r9
    xorq    %rsi, %r15
    movq    (%rsp), %rdi
    movq    %r8, -16(%rsp)
    xorq    %rcx, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %r8
    xorq    %r13, %r10
    movq    %r11, -40(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    notq    %r8
    movq    %r12, 32(%rsp)
    movq    %r10, %r11
    movq    %r15, %r12
    xorq    %rdx, %r9
    movq    %r8, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r10, %r12
    orq %r9, %r11
    xorq    %rax, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r9, %r12
    xorq    %rdi, %r11
    andq    %r14, %r15
    andq    %rdi, %r9
    movq    %rbx, 56(%rsp)
    movq    %r14, %rbx
    xorq    %r10, %r15
    xorq    %r14, %r9
    orq %rdi, %rbx
    movq    %rbp, -96(%rsp)
    xorq    %r8, %rbx
    movq    %r11, -48(%rsp)
    movq    -24(%rsp), %r11
    movq    -56(%rsp), %r10
    movq    %r9, 80(%rsp)
    movq    8(%rsp), %rdi
    movq    %r12, -64(%rsp)
    movq    40(%rsp), %r9
    movq    %rbx, 64(%rsp)
    movq    -32(%rsp), %r8
    xorq    %rax, %r11
    xorq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r11,%r11
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    movq    %r10, %r12
    xorq    %rsi, %rdi
    xorq    %rcx, %r9
    xorq    %r13, %r8
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r9, %rbp
    orq %r11, %r12
    movq    %r8, %r14
    xorq    %rdi, %rbp
    xorq    %r9, %r12
    andq    %rdi, %r14
    orq %rdi, %r9
    movq    -48(%rsp), %rdi
    xorq    %r10, %r14
    movq    %r10, %rbx
    movq    -64(%rsp), %r10
    xorq    %r8, %r9
    orq %r8, %rbx
    xorq    -8(%rsp), %r13
    movq    %rbp, 8(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    -40(%rsp), %rdi
    movq    %r13, %r8
    xorq    %r11, %rbx
    xorq    32(%rsp), %r10
    notq    %r8
    movq    %rbx, 40(%rsp)
    xorq    -88(%rsp), %rcx
    movq    %r8, %rbx
    movq    %r9, -24(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rbp, %rdi
    movq    %rcx, %rbp
    xorq    -80(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %rsi, %rbx
    orq %rsi, %rbp
    xorq    -104(%rsp), %rdx
    xorq    %r8, %rbp
    xorq    %r12, %r10
    movq    80(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rdx, %rbx
    xorq    24(%rsp), %rax
    xorq    %rbp, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rbx, %rdi
    movq    %rbp, -104(%rsp)
    movq    %rax, %rbp
    xorq    -112(%rsp), %r10
    movq    %rax, %r11
    xorq    -120(%rsp), %rdi
    andq    %rcx, %rbp
    orq %rdx, %r11
    xorq    %rsi, %rbp
    movq    16(%rsp), %r8
    andq    %rdx, %r13
    movq    64(%rsp), %rsi
    xorq    %rcx, %r11
    xorq    %rax, %r13
    xorq    56(%rsp), %r9
    movq    %r10, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rbp, %r8
    xorq    -16(%rsp), %rsi
    xorq    -96(%rsp), %r8
    xorq    -24(%rsp), %r9
    xorq    %r14, %rsi
    xorq    %r15, %r8
    xorq    %r11, %rsi
    xorq    %r13, %r9
    xorq    40(%rsp), %r8
    xorq    48(%rsp), %rsi
    xorq    -72(%rsp), %r9
    movq    %r8, %rax
    movq    %rsi, %rcx
    xorq    %r9, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdi, %rax
    xorq    %r10, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    xorq    %rsi, %rdi
    movq    32(%rsp), %r8
    movq    -120(%rsp), %rsi
    xorq    %rcx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %r10
    xorq    %rax, %r8
    xorq    %rdx, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r9, %r14
    orq %r8, %r10
    xorq    %rdi, %r13
    xorq    $32898, %r10
    xorq    %rcx, %rbp
    xorq    %rax, %r12
    xorq    %rsi, %r10
    movq    %r10, -120(%rsp)
    movq    %r15, %r10
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r14, %r10
    xorq    %r8, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r10, (%rsp)
    movq    %r13, %r10
    andq    %r14, %r10
    xorq    %r15, %r10
    movq    %rsi, %r15
    andq    %r8, %rsi
    xorq    %r13, %rsi
    orq %r13, %r15
    movq    %r10, -8(%rsp)
    movq    -48(%rsp), %r10
    xorq    %r14, %r15
    movq    %rsi, -88(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    48(%rsp), %rsi
    movq    %rbp, %r14
    movq    %r15, -80(%rsp)
    movq    56(%rsp), %r8
    notq    %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r14
    movq    %r12, %r15
    xorq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %r14
    andq    %r10, %r15
    movq    %r10, %r13
    movq    %rbp, %r10
    xorq    %r9, %rsi
    xorq    %rdi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    orq %rsi, %r10
    orq %r8, %r13
    xorq    %r8, %r15
    xorq    %rsi, %r13
    xorq    %r12, %r10
    andq    %rsi, %r8
    xorq    %rbp, %r8
    movq    %r10, 24(%rsp)
    movq    64(%rsp), %r10
    movq    %r8, 32(%rsp)
    movq    -96(%rsp), %r8
    xorq    %rdx, %rbx
    movq    -112(%rsp), %rsi
    movq    %r13, -56(%rsp)
    xorq    %r9, %r11
    movq    -24(%rsp), %rbp
    movq    %r15, -48(%rsp)
    xorq    %r9, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r12
    xorq    %rcx, %r8
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r12
    xorq    %rdi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r12, -112(%rsp)
    movq    %rbp, %r12
    movq    %rbp, %r13
    notq    %r12
    andq    %r10, %r13
    movq    %r12, %rbp
    xorq    %r8, %r13
    andq    %rsi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%rbx,%rbx
# 0 "" 2
#NO_APP
    andq    %rbx, %rbp
    xorq    %rbx, %r8
    movq    %rbx, %r15
    xorq    %r10, %rbp
    movq    -64(%rsp), %r10
    orq %rsi, %r15
    movq    %r8, -32(%rsp)
    movq    -40(%rsp), %r8
    xorq    %r12, %r15
    movq    -72(%rsp), %rsi
    movq    %r13, -96(%rsp)
    movq    40(%rsp), %r12
    movq    %r15, -24(%rsp)
    xorq    %rax, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %rdx, %r8
    xorq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %rbx
    xorq    %rcx, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbx
    movq    %r12, %r13
    xorq    80(%rsp), %rdi
    movq    %rbx, 40(%rsp)
    movq    %r12, %rbx
    notq    %r13
    orq %r10, %rbx
    movq    %r13, %r12
    xorq    8(%rsp), %rdx
    xorq    %r8, %rbx
    orq %rsi, %r8
    xorq    16(%rsp), %rcx
    xorq    %r11, %r8
    orq %r11, %r12
    xorq    -104(%rsp), %rax
    movq    %r8, -64(%rsp)
    movq    -112(%rsp), %r8
    xorq    %r10, %r12
    movq    %r12, -40(%rsp)
    movq    %r11, %r12
    andq    %rsi, %r12
    movq    -16(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    -56(%rsp), %r8
    xorq    %r13, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r9, %rsi
    movq    %rdx, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %r9
    xorq    40(%rsp), %r8
    orq %rdi, %r13
    notq    %r9
    movq    %r9, %r15
    xorq    %r9, %r13
    andq    %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    xorq    %r15, %r8
    xorq    -120(%rsp), %r8
    movq    %r13, 8(%rsp)
    movq    -96(%rsp), %r13
    movq    -8(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r9
    movq    -32(%rsp), %r11
    xorq    -48(%rsp), %r13
    andq    %rdx, %r9
    xorq    %rdi, %r9
    movq    %rax, %rdi
    xorq    %rbx, %r13
    xorq    8(%rsp), %r13
    xorq    (%rsp), %r13
    xorq    %r9, %r10
    orq %rcx, %rdi
    xorq    %rdx, %rdi
    xorq    32(%rsp), %r11
    andq    %rsi, %rcx
    movq    %rdi, 16(%rsp)
    movq    -24(%rsp), %rdi
    xorq    %r14, %r10
    xorq    %rax, %rcx
    xorq    %rbp, %r10
    xorq    -40(%rsp), %r10
    movq    %r13, %rdx
    xorq    -64(%rsp), %r11
    xorq    24(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r10, %rax
    xorq    %rcx, %r11
    xorq    %r12, %rdi
    xorq    -88(%rsp), %r11
    xorq    16(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
    xorq    %r11, %rdx
    xorq    -80(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r10, %r11
    movq    -48(%rsp), %r10
    xorq    %rdi, %r8
    movq    %rdi, %rsi
    movq    -120(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %r11, %r12
    xorq    %rsi, %rbp
    xorq    %rax, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rdx, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r10,%r10
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %r8, %rcx
    orq %r10, %r13
    xorq    %rsi, %r9
    movq    %r13, -120(%rsp)
    xorq    %rax, %rbx
    movabsq $-9223372036854742902, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r10, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdi, -120(%rsp)
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r10, %rdi
    orq %rcx, %rbp
    xorq    %rcx, %rdi
    movq    -112(%rsp), %r10
    xorq    %r12, %rbp
    movq    -80(%rsp), %rcx
    movq    %rdi, -16(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r9,%r9
# 0 "" 2
#NO_APP
    movq    32(%rsp), %rdi
    movq    %rbp, -72(%rsp)
    movq    %r9, %rbp
    notq    %rbp
    movq    %r13, 48(%rsp)
    xorq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    movq    %rbx, %r13
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r10, %rbp
    andq    %r10, %r13
    movq    %r10, %r12
    movq    %r9, %r10
    xorq    %r8, %rdi
    movq    %rbp, -104(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rcx, %r10
    xorq    %rdi, %r13
    orq %rdi, %r12
    andq    %rcx, %rdi
    xorq    %rcx, %r12
    xorq    %rbx, %r10
    xorq    %r9, %rdi
    movq    %r12, -80(%rsp)
    movq    %rsi, %r9
    movq    %r13, -112(%rsp)
    movq    %r10, 32(%rsp)
    movq    %rdi, 56(%rsp)
    movq    -64(%rsp), %r10
    movq    (%rsp), %rcx
    movq    -24(%rsp), %rdi
    xorq    %rax, %rcx
    xorq    %r8, %r10
    xorq    %r14, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %r11, %rdi
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rdi, %r13
    movq    %rdi, %r12
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %r13
    orq %r9, %r12
    andq    %rcx, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    xorq    %r15, %r9
    movq    %r15, %rbx
    andq    %r15, %rbp
    movq    %r9, 80(%rsp)
    movq    -96(%rsp), %r9
    xorq    %rdi, %rbp
    orq %rcx, %rbx
    movq    -56(%rsp), %rdi
    xorq    %rcx, %r12
    xorq    %r10, %rbx
    movq    -88(%rsp), %rcx
    movq    -40(%rsp), %r10
    movq    %r12, -24(%rsp)
    movq    16(%rsp), %r14
    xorq    %rax, %r9
    movq    %rbx, 64(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %rdx, %rdi
    xorq    %r8, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rdi, %r12
    xorq    %rsi, %r10
    xorq    %r11, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rcx, %r12
    movq    %r10, %rbx
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r12, -96(%rsp)
    orq %r9, %rbx
    movq    %r14, %r12
    xorq    %rdi, %rbx
    movq    %r13, -64(%rsp)
    andq    %rcx, %r12
    orq %rcx, %rdi
    movq    %r10, %r13
    movq    24(%rsp), %rcx
    orq %r14, %r13
    xorq    %r14, %rdi
    xorq    8(%rsp), %rax
    xorq    %r9, %r13
    movq    -24(%rsp), %r9
    movq    %rdi, -88(%rsp)
    movq    %r13, -40(%rsp)
    movq    -64(%rsp), %r13
    xorq    %r10, %r12
    xorq    %r11, %rcx
    xorq    40(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %rdi
    xorq    -32(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    notq    %rdi
    movq    %rax, %r11
    xorq    -80(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdi, %r15
    andq    %rdx, %r11
    movq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    -112(%rsp), %r13
    xorq    %r8, %r11
    andq    %r8, %r15
    orq %r8, %r10
    movq    48(%rsp), %r8
    movq    %rax, %r14
    xorq    %rdi, %r10
    xorq    -96(%rsp), %r9
    movq    %r10, 8(%rsp)
    xorq    -8(%rsp), %rsi
    xorq    %rbx, %r13
    movq    64(%rsp), %rdi
    xorq    %r11, %r8
    xorq    %r10, %r13
    movq    80(%rsp), %r10
    xorq    -104(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r15
    xorq    -48(%rsp), %r13
    xorq    %r15, %r9
    xorq    -120(%rsp), %r9
    xorq    %rbp, %r8
    orq %rsi, %r14
    xorq    56(%rsp), %r10
    andq    %rcx, %rsi
    xorq    %rdx, %r14
    xorq    32(%rsp), %rdi
    xorq    %rax, %rsi
    movq    %r13, %rdx
    xorq    -40(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    -88(%rsp), %r10
    xorq    %r12, %rdi
    xorq    %r14, %rdi
    movq    %r8, %rax
    xorq    -72(%rsp), %rdi
    xorq    %rsi, %r10
    xorq    -16(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rdi, %rcx
    xorq    %r9, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r10, %rdx
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    -112(%rsp), %r8
    xorq    %r9, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %rcx
    xorq    %rdx, %rdi
    xorq    %r10, %r12
    xorq    %rcx, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    xorq    %rax, %rbx
    xorq    %rcx, %r11
    movq    %r13, -120(%rsp)
    movabsq $-9223372034707259392, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r8, %r13
    xorq    %rdi, -120(%rsp)
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r8, %rdi
    xorq    %rsi, %rdi
    orq %rsi, %rbp
    movq    -24(%rsp), %r8
    movq    -72(%rsp), %rsi
    movq    %rdi, -56(%rsp)
    xorq    %r12, %rbp
    movq    56(%rsp), %rdi
    movq    %rbp, -8(%rsp)
    movq    %r13, 40(%rsp)
    xorq    %rdx, %r8
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %rdi
    movq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    movq    %rbx, %r13
    orq %rdi, %r12
    notq    %rbp
    andq    %r8, %r13
    xorq    %rsi, %r12
    orq %rbx, %rbp
    xorq    %rdi, %r13
    andq    %rsi, %rdi
    xorq    %r8, %rbp
    movq    %r11, %r8
    xorq    %r11, %rdi
    orq %rsi, %r8
    movq    -88(%rsp), %r11
    movq    %rdi, 16(%rsp)
    xorq    %rbx, %r8
    movq    -48(%rsp), %rsi
    movq    %r13, 24(%rsp)
    movq    %r8, -32(%rsp)
    movq    -104(%rsp), %r8
    movq    64(%rsp), %rdi
    movq    %r12, -24(%rsp)
    movq    %rbp, -112(%rsp)
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r8
    xorq    %r9, %r11
    xorq    %r10, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    xorq    %rdx, %r15
    notq    %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    andq    %rdi, %r13
    movq    %rdi, %r12
    movq    %r15, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    orq %rsi, %rbx
    xorq    %r8, %r13
    orq %r8, %r12
    andq    %rsi, %r8
    xorq    %r11, %rbx
    movq    %r11, %rbp
    xorq    %r15, %r8
    movq    -40(%rsp), %r11
    xorq    %rsi, %r12
    movq    %r8, 56(%rsp)
    movq    -64(%rsp), %r8
    andq    %r15, %rbp
    movq    -80(%rsp), %rsi
    xorq    %rdi, %rbp
    movq    %rbx, -72(%rsp)
    movq    -16(%rsp), %rdi
    movq    %r12, -104(%rsp)
    xorq    %r10, %r14
    xorq    %rcx, %r11
    movq    %r13, -88(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    notq    %r11
    xorq    %rax, %r8
    movq    %r11, %r13
    xorq    %rdx, %rsi
    xorq    %r9, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r12
    orq %r8, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    orq %r14, %r13
    andq    %rsi, %r12
    xorq    %rsi, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r12
    xorq    %r8, %r13
    movq    -104(%rsp), %r8
    orq %rdi, %rsi
    movq    %r12, -40(%rsp)
    movq    %r14, %r12
    xorq    %r14, %rsi
    andq    %rdi, %r12
    xorq    80(%rsp), %r9
    movq    %rsi, -80(%rsp)
    movq    32(%rsp), %rsi
    xorq    %r11, %r12
    xorq    -24(%rsp), %r8
    movq    %r13, -64(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    -96(%rsp), %rdx
    xorq    %r10, %rsi
    xorq    48(%rsp), %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rdi
    xorq    -40(%rsp), %r8
    notq    %rdi
    xorq    8(%rsp), %rax
    movq    %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %r9, %r15
    movq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %r9, %r10
    xorq    %rcx, %r15
    xorq    %rdi, %r10
    xorq    %r15, %r8
    xorq    -120(%rsp), %r8
    movq    %r10, -96(%rsp)
    movq    -88(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    56(%rsp), %r11
    movq    %rax, %r14
    movq    -72(%rsp), %rdi
    xorq    24(%rsp), %r13
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    %rax, %r10
    andq    %rdx, %r10
    xorq    (%rsp), %r13
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    xorq    %r10, %r9
    xorq    -112(%rsp), %r9
    xorq    %rbp, %r9
    xorq    -64(%rsp), %r9
    orq %rcx, %r14
    andq    %rsi, %rcx
    xorq    16(%rsp), %r11
    xorq    %rax, %rcx
    xorq    %rdx, %r14
    xorq    -32(%rsp), %rdi
    movq    %r13, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r9, %rax
    xorq    -80(%rsp), %r11
    xorq    %r12, %rdi
    xorq    %r14, %rdi
    xorq    -8(%rsp), %rdi
    xorq    %rcx, %r11
    xorq    -56(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rdi, %rsi
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
    xorq    %rdi, %r8
    movq    -120(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    24(%rsp), %r9
    xorq    %r8, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %rdx, %rdi
    xorq    %r11, %r12
    xorq    %rsi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
    xorq    $32907, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %rax, %rbx
    movq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r9, %r13
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    orq %rcx, %rbp
    xorq    %rcx, %rdi
    movq    -104(%rsp), %r9
    xorq    %r12, %rbp
    movq    -8(%rsp), %rcx
    movq    %rdi, 32(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    16(%rsp), %rdi
    movq    %rbp, 24(%rsp)
    movq    %r10, %rbp
    notq    %rbp
    movq    %r13, 8(%rsp)
    xorq    %rdx, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    movq    %rbx, %r13
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r9, %rbp
    andq    %r9, %r13
    movq    %r9, %r12
    movq    %r10, %r9
    xorq    %r8, %rdi
    movq    %rbp, -104(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rcx, %r9
    xorq    %rdi, %r13
    orq %rdi, %r12
    andq    %rcx, %rdi
    xorq    %rbx, %r9
    xorq    %rcx, %r12
    xorq    %r10, %rdi
    movq    %r9, 48(%rsp)
    movq    (%rsp), %rcx
    movq    -112(%rsp), %r9
    movq    %rdi, -16(%rsp)
    movq    -80(%rsp), %r10
    movq    %r13, 16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r12, -8(%rsp)
    xorq    %rax, %rcx
    xorq    %rsi, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r11, %rdi
    xorq    %r8, %r10
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    movq    %rdi, %r12
    notq    %r10
    andq    %rdi, %r13
    orq %r9, %r12
    movq    %r10, %rbp
    xorq    %r9, %r13
    andq    %rcx, %r9
    xorq    %rcx, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %r15, %r9
    movq    %r12, -112(%rsp)
    movq    %r15, %rbx
    movq    %r13, -80(%rsp)
    andq    %r15, %rbp
    orq %rcx, %rbx
    movq    %r9, 64(%rsp)
    movq    -88(%rsp), %r9
    xorq    %rdi, %rbp
    movq    -24(%rsp), %rcx
    xorq    %r10, %rbx
    xorq    %r11, %r14
    movq    -64(%rsp), %r10
    movq    %rbx, -72(%rsp)
    movq    -56(%rsp), %rdi
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %rdx, %rcx
    xorq    %rsi, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %rcx, %r12
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %rdi, %r12
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %r12, -24(%rsp)
    movq    %r10, %r13
    movq    %r14, %r12
    orq %r9, %rbx
    orq %r14, %r13
    andq    %rdi, %r12
    xorq    %rcx, %rbx
    xorq    %r9, %r13
    xorq    %r10, %r12
    orq %rdi, %rcx
    movq    %r13, -64(%rsp)
    movq    -112(%rsp), %r9
    xorq    %r14, %rcx
    movq    -80(%rsp), %r13
    movq    %rcx, -88(%rsp)
    movq    -32(%rsp), %rcx
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    -40(%rsp), %rdx
    movq    %rax, %r14
    xorq    %r11, %rcx
    movq    %rax, %r11
    xorq    56(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %rdi
    xorq    -8(%rsp), %r9
    notq    %rdi
    xorq    16(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdi, %r15
    andq    %rdx, %r11
    movq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r11
    andq    %r8, %r15
    orq %r8, %r10
    movq    8(%rsp), %r8
    xorq    %rdi, %r10
    xorq    %rbx, %r13
    xorq    -24(%rsp), %r9
    xorq    %r10, %r13
    movq    %r10, -96(%rsp)
    movq    -72(%rsp), %rdi
    movq    64(%rsp), %r10
    xorq    %r11, %r8
    xorq    40(%rsp), %rsi
    xorq    -104(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r15
    orq %rsi, %r14
    xorq    -48(%rsp), %r13
    xorq    %r15, %r9
    xorq    %rdx, %r14
    xorq    %rbp, %r8
    xorq    -120(%rsp), %r9
    xorq    -64(%rsp), %r8
    xorq    48(%rsp), %rdi
    andq    %rcx, %rsi
    movq    %r13, %rdx
    xorq    -16(%rsp), %r10
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    xorq    %r12, %rdi
    xorq    -88(%rsp), %r10
    xorq    %r14, %rdi
    xorq    24(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
    xorq    %rsi, %r10
    xorq    32(%rsp), %r10
    movq    %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %rcx
    xorq    %rcx, %rbp
    xorq    %r10, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    16(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %r10, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    xorq    %r9, %rsi
    movq    %r13, -120(%rsp)
    movl    $2147483649, %r13d
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    xorq    %rdx, %rdi
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rdi, -120(%rsp)
    xorq    %r8, %r13
    xorq    %rcx, %r11
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    xorq    %rax, %rbx
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r8, %rdi
    orq %rsi, %rbp
    xorq    %rsi, %rdi
    movq    -112(%rsp), %r8
    xorq    %r12, %rbp
    movq    24(%rsp), %rsi
    movq    %rdi, -56(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    -16(%rsp), %rdi
    movq    %rbp, -40(%rsp)
    movq    %r11, %rbp
    notq    %rbp
    movq    %r13, 40(%rsp)
    xorq    %rdx, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    movq    %rbx, %r13
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    andq    %r8, %r13
    movq    %r8, %r12
    movq    %r11, %r8
    xorq    %r9, %rdi
    movq    %rbp, -112(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rsi, %r8
    xorq    %rdi, %r13
    orq %rdi, %r12
    andq    %rsi, %rdi
    xorq    %rbx, %r8
    xorq    %rsi, %r12
    xorq    %r11, %rdi
    movq    %r8, 16(%rsp)
    movq    -88(%rsp), %r11
    movq    -104(%rsp), %r8
    movq    %rdi, -16(%rsp)
    movq    -48(%rsp), %rsi
    movq    %r13, -32(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r12, 24(%rsp)
    xorq    %rcx, %r8
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    %rdi, %r12
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    notq    %r11
    orq %r8, %r12
    andq    %rdi, %r13
    movq    %r11, %rbp
    xorq    %rsi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    movq    %r15, %rbx
    andq    %rsi, %r8
    xorq    %r15, %r8
    andq    %r15, %rbp
    orq %rsi, %rbx
    xorq    %rdi, %rbp
    xorq    %r11, %rbx
    movq    -8(%rsp), %rsi
    movq    -64(%rsp), %r11
    movq    %r8, 56(%rsp)
    xorq    %r10, %r14
    movq    -80(%rsp), %r8
    movq    %r12, -104(%rsp)
    movq    32(%rsp), %rdi
    movq    %rbx, -72(%rsp)
    xorq    %rdx, %rsi
    movq    %r13, -88(%rsp)
    xorq    %rcx, %r11
    xorq    %rax, %r8
    xorq    %r9, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r8, %r12
    movq    %r11, %rbx
    notq    %r11
    andq    %rsi, %r12
    movq    %r11, %r13
    orq %r8, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %rdi, %r12
    orq %r14, %r13
    xorq    %rsi, %rbx
    xorq    %r8, %r13
    movq    %r12, -8(%rsp)
    movq    -104(%rsp), %r8
    movq    %r14, %r12
    orq %rdi, %rsi
    movq    %r13, -64(%rsp)
    andq    %rdi, %r12
    xorq    %r11, %r12
    xorq    %r14, %rsi
    xorq    64(%rsp), %r9
    movq    %rsi, -80(%rsp)
    movq    48(%rsp), %rsi
    xorq    24(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    -24(%rsp), %rdx
    xorq    %r10, %rsi
    xorq    8(%rsp), %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rdi
    xorq    -8(%rsp), %r8
    notq    %rdi
    xorq    -96(%rsp), %rax
    movq    %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    andq    %r9, %r15
    movq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %r9, %r10
    xorq    %rcx, %r15
    xorq    %rdi, %r10
    xorq    %r15, %r8
    xorq    -120(%rsp), %r8
    movq    %r10, -96(%rsp)
    movq    -88(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    -72(%rsp), %rdi
    movq    %rax, %r14
    movq    56(%rsp), %r11
    orq %rcx, %r14
    xorq    -32(%rsp), %r13
    xorq    %rdx, %r14
    xorq    16(%rsp), %rdi
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    %rax, %r10
    andq    %rdx, %r10
    xorq    %r12, %rdi
    xorq    (%rsp), %r13
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    movq    %r13, %rdx
    xorq    %r10, %r9
    xorq    -112(%rsp), %r9
    xorq    %rbp, %r9
    xorq    -64(%rsp), %r9
    xorq    %r14, %rdi
    andq    %rsi, %rcx
    xorq    -16(%rsp), %r11
    xorq    %rax, %rcx
    xorq    -40(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r9, %rax
    xorq    -80(%rsp), %r11
    movq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %rcx, %r11
    xorq    %rsi, %rbp
    xorq    -56(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %r8, %rax
    xorq    %r11, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    -32(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    -120(%rsp), %rdi
    xorq    %r11, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rax, %r9
    xorq    %r8, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
    xorq    %rdx, %rdi
    movq    %r13, -120(%rsp)
    movabsq $-9223372034707259263, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rdi, -120(%rsp)
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %rcx, %rdi
    orq %rcx, %rbp
    movq    -104(%rsp), %r9
    xorq    %r12, %rbp
    movq    -40(%rsp), %rcx
    movq    %rdi, 32(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %rsi, %r10
    movq    %rbp, -24(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    xorq    %rax, %rbx
    movq    %r13, 8(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %rbp
    movq    %rbx, %r13
    xorq    %rdx, %r9
    orq %rbx, %rbp
    xorq    %r11, %rcx
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    movq    %r9, %r12
    xorq    %r9, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %r10, %r9
    xorq    %rdi, %r13
    orq %rdi, %r12
    andq    %rcx, %rdi
    orq %rcx, %r9
    xorq    %rcx, %r12
    xorq    %r10, %rdi
    xorq    %rbx, %r9
    movq    -80(%rsp), %r10
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r9, 48(%rsp)
    movq    (%rsp), %rcx
    movq    -112(%rsp), %r9
    movq    %r12, -40(%rsp)
    xorq    %r8, %r10
    movq    %r13, -32(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r11, %rdi
    movq    %r10, %r13
    xorq    %rax, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r12
    xorq    %rsi, %r9
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdx, %r15
    orq %r9, %r12
    andq    %rdi, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %rbx
    xorq    %rcx, %r12
    xorq    %r9, %r13
    orq %rcx, %rbx
    andq    %rcx, %r9
    movq    %rbp, -104(%rsp)
    xorq    %r10, %rbx
    movq    %r12, -112(%rsp)
    xorq    %r15, %r9
    movq    %r10, %rbp
    movq    -64(%rsp), %r10
    movq    %r9, 64(%rsp)
    andq    %r15, %rbp
    movq    -88(%rsp), %r9
    movq    %rbx, -72(%rsp)
    xorq    %rdi, %rbp
    movq    24(%rsp), %rcx
    xorq    %r11, %r14
    movq    -56(%rsp), %rdi
    movq    %r13, -80(%rsp)
    xorq    %rsi, %r10
    xorq    40(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %rax, %r9
    xorq    %rdx, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    movq    %r9, %r12
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbx
    andq    %rcx, %r12
    orq %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r14, %rcx
    notq    %r10
    xorq    %rdi, %r12
    movq    %rcx, -56(%rsp)
    movq    16(%rsp), %rcx
    movq    %r10, %r13
    movq    %r12, -64(%rsp)
    orq %r14, %r13
    movq    %r14, %r12
    xorq    %r9, %r13
    andq    %rdi, %r12
    movq    -112(%rsp), %r9
    xorq    %r10, %r12
    movq    %r13, -88(%rsp)
    movq    -80(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r11, %rcx
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %rdi
    xorq    -8(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    notq    %rdi
    movq    %rdx, %r10
    movq    %rax, %r11
    xorq    56(%rsp), %r8
    andq    %rdx, %r11
    movq    %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r10
    xorq    %r8, %r11
    andq    %r8, %r15
    xorq    %rdi, %r10
    movq    8(%rsp), %r8
    movq    %rax, %r14
    movq    -72(%rsp), %rdi
    movq    %r10, -96(%rsp)
    orq %rsi, %r14
    xorq    -40(%rsp), %r9
    xorq    %rdx, %r14
    xorq    %rsi, %r15
    xorq    -32(%rsp), %r13
    xorq    %r11, %r8
    xorq    48(%rsp), %rdi
    xorq    -64(%rsp), %r9
    xorq    %rbx, %r13
    xorq    -104(%rsp), %r8
    xorq    %r10, %r13
    movq    64(%rsp), %r10
    xorq    %r12, %rdi
    xorq    -48(%rsp), %r13
    xorq    %r15, %r9
    xorq    %r14, %rdi
    xorq    %rbp, %r8
    xorq    -24(%rsp), %rdi
    xorq    -88(%rsp), %r8
    xorq    -120(%rsp), %r9
    andq    %rcx, %rsi
    movq    %r13, %rdx
    xorq    -16(%rsp), %r10
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    movq    %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    -56(%rsp), %r10
    xorq    %r13, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rcx, %rbp
    xorq    %r9, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rsi, %r10
    movq    %rbp, %r13
    xorq    32(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
    xorq    %r9, %rsi
    xorq    %r10, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    -32(%rsp), %r8
    xorq    %rdx, %rdi
    xorq    %r10, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    movq    %r13, -120(%rsp)
    movabsq $-9223372036854743031, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r8, %r13
    xorq    %rdi, -120(%rsp)
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    orq %rsi, %rbp
    movq    %r13, 40(%rsp)
    xorq    %r12, %rbp
    andq    %r8, %rdi
    movq    -112(%rsp), %r8
    xorq    %rsi, %rdi
    movq    -24(%rsp), %rsi
    xorq    %rcx, %r11
    movq    %rdi, 24(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %rax, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %rbp, -8(%rsp)
    movq    %r11, %rbp
    xorq    %rdx, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %rbp
    movq    %rbx, %r13
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    andq    %r8, %r13
    movq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    xorq    %r9, %rdi
    movq    %r11, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r13
    orq %rdi, %r12
    andq    %rsi, %rdi
    xorq    %r11, %rdi
    orq %rsi, %r8
    movq    -56(%rsp), %r11
    xorq    %rbx, %r8
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    xorq    %rsi, %r12
    movq    %r8, 16(%rsp)
    movq    -48(%rsp), %rsi
    movq    -104(%rsp), %r8
    movq    %r12, -24(%rsp)
    xorq    %rdx, %r15
    xorq    %r9, %r11
    movq    %r13, -32(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r10, %rdi
    movq    %r11, %r13
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r12
    xorq    %rcx, %r8
    notq    %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r15, %rbx
    orq %r8, %r12
    andq    %rdi, %r13
    xorq    %r8, %r13
    orq %rsi, %rbx
    andq    %rsi, %r8
    xorq    %r11, %rbx
    xorq    %r15, %r8
    movq    %rbp, -112(%rsp)
    movq    %r11, %rbp
    movq    -88(%rsp), %r11
    movq    %r8, 56(%rsp)
    andq    %r15, %rbp
    movq    -80(%rsp), %r8
    xorq    %rsi, %r12
    xorq    %rdi, %rbp
    movq    -40(%rsp), %rsi
    movq    %rbx, -72(%rsp)
    movq    32(%rsp), %rdi
    movq    %r12, -104(%rsp)
    xorq    %r10, %r14
    xorq    %rcx, %r11
    movq    %r13, -56(%rsp)
    xorq    8(%rsp), %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    xorq    %rax, %r8
    notq    %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %rbx
    movq    %r8, %r12
    xorq    %r9, %rdi
    xorq    %rdx, %rsi
    movq    %r11, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbx
    andq    %rsi, %r12
    orq %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r14, %rsi
    orq %r14, %r13
    xorq    %rdi, %r12
    xorq    %r8, %r13
    movq    %rsi, -88(%rsp)
    movq    -104(%rsp), %r8
    movq    48(%rsp), %rsi
    movq    %r12, -40(%rsp)
    movq    %r14, %r12
    andq    %rdi, %r12
    movq    %r13, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r11, %r12
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    -24(%rsp), %r8
    movq    %rsi, %rdi
    notq    %rdi
    xorq    64(%rsp), %r9
    movq    %rdi, %r15
    xorq    -64(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    -40(%rsp), %r8
    andq    %r9, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    xorq    %rcx, %r15
    xorq    -96(%rsp), %rax
    orq %r9, %r10
    xorq    %rdi, %r10
    xorq    %r15, %r8
    xorq    -120(%rsp), %r8
    movq    %r10, -96(%rsp)
    movq    -56(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    -72(%rsp), %rdi
    movq    %rax, %r14
    movq    56(%rsp), %r11
    orq %rcx, %r14
    andq    %rsi, %rcx
    xorq    -32(%rsp), %r13
    xorq    %rdx, %r14
    xorq    %rax, %rcx
    xorq    16(%rsp), %rdi
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    %rax, %r10
    andq    %rdx, %r10
    xorq    %r12, %rdi
    xorq    (%rsp), %r13
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    xorq    %r14, %rdi
    xorq    -8(%rsp), %rdi
    movq    %r13, %rdx
    xorq    %r10, %r9
    xorq    -112(%rsp), %r9
    movq    %rdi, %rsi
    xorq    %rbp, %r9
    xorq    -80(%rsp), %r9
    xorq    -16(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r9, %rax
    xorq    %r13, %rsi
    xorq    -88(%rsp), %r11
    xorq    %rsi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    xorq    %rdi, %r8
    movq    -120(%rsp), %rdi
    xorq    24(%rsp), %r11
    xorq    %r8, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    -32(%rsp), %r9
    xorq    %rdx, %rdi
    xorq    %r11, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
    xorb    $-118, %r13b
    xorq    %rdi, %r13
    movq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %r9, %r13
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    orq %rcx, %rbp
    movq    -104(%rsp), %r9
    movq    %r13, 8(%rsp)
    xorq    %r12, %rbp
    xorq    %rcx, %rdi
    movq    -8(%rsp), %rcx
    movq    %rdi, 32(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %rsi, %r10
    movq    %rbp, -64(%rsp)
    xorq    %rax, %rbx
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    xorq    %rdx, %r9
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %rbp
    movq    %rbx, %r13
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    andq    %r9, %r13
    movq    %r9, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r9, %rbp
    xorq    %rdi, %r13
    orq %rdi, %r12
    movq    %r10, %r9
    andq    %rcx, %rdi
    xorq    %rcx, %r12
    xorq    %r10, %rdi
    orq %rcx, %r9
    movq    -88(%rsp), %r10
    xorq    %rbx, %r9
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r9, 48(%rsp)
    movq    -112(%rsp), %r9
    movq    (%rsp), %rcx
    movq    %r12, -8(%rsp)
    xorq    %r8, %r10
    movq    %r13, -32(%rsp)
    xorq    %r11, %rdi
    movq    %rbp, -104(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r12
    xorq    %rsi, %r9
    xorq    %rax, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    orq %r9, %r12
    movq    %r10, %r13
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %rcx, %r12
    movq    %r15, %rbx
    andq    %rdi, %r13
    xorq    %r9, %r13
    orq %rcx, %rbx
    andq    %rcx, %r9
    xorq    %r10, %rbx
    xorq    %r15, %r9
    movq    %r12, -112(%rsp)
    movq    %r10, %rbp
    movq    %r13, -88(%rsp)
    movq    -80(%rsp), %r10
    andq    %r15, %rbp
    movq    %r9, 64(%rsp)
    movq    -56(%rsp), %r9
    xorq    %rdi, %rbp
    movq    -24(%rsp), %rcx
    movq    %rbx, -72(%rsp)
    movq    24(%rsp), %rdi
    xorq    %r11, %r14
    xorq    %rsi, %r10
    xorq    40(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    notq    %r10
    xorq    %rax, %r9
    movq    %r10, %r13
    xorq    %r8, %rdi
    xorq    %rdx, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    movq    %r9, %r12
    xorq    56(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %r14, %r13
    xorq    %rcx, %rbx
    andq    %rcx, %r12
    orq %rdi, %rcx
    xorq    %r9, %r13
    xorq    %rdi, %r12
    xorq    %r14, %rcx
    movq    %r13, -80(%rsp)
    movq    -88(%rsp), %r13
    movq    %rcx, -56(%rsp)
    movq    16(%rsp), %rcx
    movq    %r12, -24(%rsp)
    movq    %r14, %r12
    movq    -112(%rsp), %r9
    andq    %rdi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %r12
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    -32(%rsp), %r13
    movq    %rcx, %rdi
    xorq    -40(%rsp), %rdx
    notq    %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    xorq    -96(%rsp), %rax
    movq    %rdi, %r15
    orq %r8, %r10
    xorq    %rbx, %r13
    xorq    -8(%rsp), %r9
    xorq    %rdi, %r10
    andq    %r8, %r15
    movq    -72(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r10, %r13
    movq    %r10, -96(%rsp)
    movq    %rax, %r11
    movq    64(%rsp), %r10
    andq    %rdx, %r11
    movq    %rax, %r14
    xorq    %r8, %r11
    movq    8(%rsp), %r8
    orq %rsi, %r14
    xorq    48(%rsp), %rdi
    xorq    %rsi, %r15
    xorq    %rdx, %r14
    xorq    -24(%rsp), %r9
    andq    %rcx, %rsi
    xorq    -16(%rsp), %r10
    xorq    %rax, %rsi
    xorq    %r11, %r8
    xorq    -48(%rsp), %r13
    xorq    -104(%rsp), %r8
    xorq    %r12, %rdi
    xorq    %r15, %r9
    xorq    %r14, %rdi
    xorq    -56(%rsp), %r10
    xorq    -64(%rsp), %rdi
    movq    %r13, %rdx
    xorq    %rbp, %r8
    xorq    -120(%rsp), %r9
    xorq    -80(%rsp), %r8
    xorq    %rsi, %r10
    xorq    32(%rsp), %r10
    movq    %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    xorq    %r13, %rcx
    xorq    %r10, %rdx
    xorq    %rcx, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    -32(%rsp), %r8
    xorq    %r9, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
    xorq    %r10, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r8
    xorq    %r9, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    xorq    %rdx, %rdi
    xorb    $-120, %r13b
    xorq    %rdi, %r13
    movq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r8, %rdi
    xorq    %rsi, %rdi
    orq %rsi, %rbp
    movq    -64(%rsp), %rsi
    movq    -112(%rsp), %r8
    xorq    %r12, %rbp
    movq    %rdi, 24(%rsp)
    movq    -16(%rsp), %rdi
    movq    %rbp, -40(%rsp)
    movq    %r13, 40(%rsp)
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    xorq    %rax, %rbx
    xorq    %rdx, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    xorq    %r9, %rdi
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %rbp
    movq    %rbx, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    andq    %r8, %r13
    movq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r8, %rbp
    xorq    %rdi, %r13
    orq %rdi, %r12
    movq    %r11, %r8
    andq    %rsi, %rdi
    xorq    %rsi, %r12
    xorq    %r11, %rdi
    orq %rsi, %r8
    movq    -56(%rsp), %r11
    xorq    %rbx, %r8
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r8, 16(%rsp)
    movq    -104(%rsp), %r8
    movq    -48(%rsp), %rsi
    movq    %r12, -64(%rsp)
    xorq    %r9, %r11
    movq    %r13, -32(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r10, %rdi
    movq    %r11, %r13
    notq    %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r12
    xorq    %rcx, %r8
    andq    %rdi, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    orq %r8, %r12
    movq    %rbp, -112(%rsp)
    movq    %r15, %rbx
    movq    %r11, %rbp
    xorq    %r8, %r13
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
    andq    %r15, %rbp
    orq %rsi, %rbx
    andq    %rsi, %r8
    xorq    %r11, %rbx
    movq    -80(%rsp), %r11
    xorq    %r15, %r8
    xorq    %rdi, %rbp
    movq    -8(%rsp), %rsi
    movq    %r8, 56(%rsp)
    movq    -88(%rsp), %r8
    xorq    %r10, %r14
    movq    32(%rsp), %rdi
    movq    %rbx, -72(%rsp)
    xorq    %rcx, %r11
    movq    %r12, -104(%rsp)
    xorq    8(%rsp), %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    xorq    %rax, %r8
    notq    %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %rbx
    movq    %r8, %r12
    xorq    %r9, %rdi
    xorq    %rdx, %rsi
    movq    %r13, -56(%rsp)
    movq    %r11, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbx
    andq    %rsi, %r12
    orq %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r14, %rsi
    orq %r14, %r13
    xorq    %rdi, %r12
    xorq    %r8, %r13
    movq    %rsi, -88(%rsp)
    movq    -104(%rsp), %r8
    movq    48(%rsp), %rsi
    movq    %r12, -8(%rsp)
    movq    %r14, %r12
    andq    %rdi, %r12
    xorq    64(%rsp), %r9
    movq    %r13, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r11, %r12
    xorq    %r10, %rsi
    xorq    -24(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    -64(%rsp), %r8
    movq    %rsi, %rdi
    movq    %rdx, %r10
    notq    %rdi
    orq %r9, %r10
    xorq    -96(%rsp), %rax
    movq    %rdi, %r15
    xorq    %rdi, %r10
    andq    %r9, %r15
    xorq    -8(%rsp), %r8
    xorq    %rcx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    orq %rcx, %r14
    andq    %rsi, %rcx
    xorq    %r15, %r8
    xorq    %rax, %rcx
    xorq    %rdx, %r14
    xorq    -120(%rsp), %r8
    movq    %r10, -96(%rsp)
    movq    -56(%rsp), %r13
    movq    56(%rsp), %r11
    movq    -72(%rsp), %rdi
    xorq    -32(%rsp), %r13
    xorq    -16(%rsp), %r11
    xorq    16(%rsp), %rdi
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    %rax, %r10
    xorq    -88(%rsp), %r11
    andq    %rdx, %r10
    xorq    (%rsp), %r13
    xorq    %r12, %rdi
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    xorq    %r14, %rdi
    xorq    -40(%rsp), %rdi
    xorq    %rcx, %r11
    xorq    24(%rsp), %r11
    movq    %r13, %rdx
    xorq    %r10, %r9
    xorq    -112(%rsp), %r9
    movq    %rdi, %rsi
    xorq    %rbp, %r9
    xorq    -80(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r9, %rax
    xorq    %r9, %r11
    movq    -32(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %r11, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbp
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    -120(%rsp), %rdi
    movq    %r13, -120(%rsp)
    movl    $2147516425, %r13d
    xorq    %r8, %rcx
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdx, %rdi
    xorq    %rdi, -120(%rsp)
    xorq    %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %rcx, %rdi
    orq %rcx, %rbp
    movq    -40(%rsp), %rcx
    movq    %rdi, 32(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %r12, %rbp
    movq    -104(%rsp), %r9
    movq    %rbp, -24(%rsp)
    movq    %r13, 8(%rsp)
    xorq    %r11, %rcx
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r10
    xorq    %rax, %rbx
    xorq    %rdx, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %rbp
    movq    %rbx, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rbx, %rbp
    andq    %r9, %r13
    movq    %r9, %r12
    xorq    %rdi, %r13
    xorq    %r9, %rbp
    orq %rdi, %r12
    movq    %r10, %r9
    andq    %rcx, %rdi
    xorq    %rcx, %r12
    xorq    %r10, %rdi
    orq %rcx, %r9
    movq    -88(%rsp), %r10
    xorq    %rbx, %r9
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r9, 48(%rsp)
    movq    -112(%rsp), %r9
    movq    (%rsp), %rcx
    movq    %r13, -32(%rsp)
    xorq    %r8, %r10
    movq    %r12, -40(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    notq    %r10
    xorq    %r11, %rdi
    movq    %rbp, -104(%rsp)
    movq    %r10, %rbp
    xorq    %rsi, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r12
    andq    %rdi, %r13
    xorq    %rax, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r15, %rbp
    orq %r9, %r12
    movq    %r15, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %r13
    xorq    %rcx, %r12
    xorq    %rdi, %rbp
    orq %rcx, %rbx
    andq    %rcx, %r9
    movq    %r12, -112(%rsp)
    xorq    %r10, %rbx
    xorq    %r15, %r9
    movq    -80(%rsp), %r10
    movq    %r9, 64(%rsp)
    movq    -56(%rsp), %r9
    xorq    %r11, %r14
    movq    -64(%rsp), %rcx
    movq    %rbx, -72(%rsp)
    movq    24(%rsp), %rdi
    movq    %r13, -88(%rsp)
    xorq    %rsi, %r10
    xorq    40(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    notq    %r10
    xorq    %rax, %r9
    movq    %r10, %r13
    xorq    %r8, %rdi
    xorq    %rdx, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    movq    %r9, %r12
    xorq    56(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
#NO_APP
    orq %r14, %r13
    xorq    %rcx, %rbx
    andq    %rcx, %r12
    orq %rdi, %rcx
    xorq    %r9, %r13
    xorq    %rdi, %r12
    xorq    %r14, %rcx
    movq    %r13, -80(%rsp)
    movq    -88(%rsp), %r13
    movq    %rcx, -56(%rsp)
    movq    16(%rsp), %rcx
    movq    %r12, -64(%rsp)
    movq    %r14, %r12
    xorq    -8(%rsp), %rdx
    andq    %rdi, %r12
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r10, %r12
    xorq    %r11, %rcx
    movq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    -32(%rsp), %r13
    movq    %rcx, %rdi
    orq %r8, %r10
    notq    %rdi
    movq    %rax, %r11
    movq    -112(%rsp), %r9
    xorq    %rdi, %r10
    andq    %rdx, %r11
    movq    %rdi, %r15
    movq    %r10, -96(%rsp)
    xorq    %r8, %r11
    andq    %r8, %r15
    xorq    %rbx, %r13
    movq    8(%rsp), %r8
    movq    %rax, %r14
    xorq    %r10, %r13
    movq    64(%rsp), %r10
    xorq    %rsi, %r15
    movq    -72(%rsp), %rdi
    orq %rsi, %r14
    andq    %rcx, %rsi
    xorq    -40(%rsp), %r9
    xorq    %rax, %rsi
    xorq    %rdx, %r14
    xorq    %r11, %r8
    xorq    -48(%rsp), %r13
    xorq    -16(%rsp), %r10
    xorq    -104(%rsp), %r8
    xorq    48(%rsp), %rdi
    xorq    -64(%rsp), %r9
    movq    %r13, %rdx
    xorq    -56(%rsp), %r10
    xorq    %rbp, %r8
    xorq    -80(%rsp), %r8
    xorq    %r12, %rdi
    xorq    %r14, %rdi
    xorq    %r15, %r9
    xorq    %rsi, %r10
    xorq    -24(%rsp), %rdi
    xorq    32(%rsp), %r10
    xorq    -120(%rsp), %r9
    movq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdi, %rcx
    xorq    %r10, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    -32(%rsp), %r8
    xorq    %r9, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %rcx
    xorq    %r10, %r12
    xorq    %rcx, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
    movq    %r13, -120(%rsp)
    movl    $2147483658, %r13d
    xorq    %r9, %rsi
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdx, %rdi
    xorq    %rdi, -120(%rsp)
    xorq    %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r8, %rdi
    xorq    %rsi, %rdi
    orq %rsi, %rbp
    movq    -112(%rsp), %r8
    movq    -24(%rsp), %rsi
    movq    %rdi, 24(%rsp)
    xorq    %r12, %rbp
    movq    -16(%rsp), %rdi
    movq    %rbp, -8(%rsp)
    movq    %r13, 40(%rsp)
    xorq    %rdx, %r8
    xorq    %r10, %rsi
    xorq    %r9, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    xorq    %rax, %rbx
    movq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    orq %rdi, %r12
    xorq    %rdx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    notq    %rbp
    movq    %rbx, %r13
    xorq    %rsi, %r12
    andq    %r8, %r13
    orq %rbx, %rbp
    movq    %r12, -24(%rsp)
    xorq    %rdi, %r13
    xorq    %r8, %rbp
    andq    %rsi, %rdi
    movq    %r11, %r8
    xorq    %r11, %rdi
    movq    -56(%rsp), %r11
    orq %rsi, %r8
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    xorq    %rbx, %r8
    movq    -48(%rsp), %rsi
    movq    %r13, -32(%rsp)
    movq    %r8, 16(%rsp)
    movq    -104(%rsp), %r8
    xorq    %r9, %r11
    movq    %rbp, -112(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    notq    %r11
    xorq    %r10, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    movq    %rdi, %r12
    movq    %r15, %rbx
    xorq    %rax, %rsi
    xorq    %rcx, %r8
    andq    %rdi, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r15, %rbp
    orq %rsi, %rbx
    orq %r8, %r12
    xorq    %rsi, %r12
    xorq    %rdi, %rbp
    xorq    %r11, %rbx
    xorq    %r8, %r13
    andq    %rsi, %r8
    movq    -80(%rsp), %r11
    xorq    %r15, %r8
    movq    -40(%rsp), %rsi
    movq    %rbx, -72(%rsp)
    movq    %r8, 56(%rsp)
    movq    -88(%rsp), %r8
    xorq    %r10, %r14
    movq    32(%rsp), %rdi
    movq    %r12, -104(%rsp)
    xorq    %rcx, %r11
    movq    %r13, -56(%rsp)
    xorq    8(%rsp), %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    xorq    %rax, %r8
    xorq    %rdx, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %rbx
    movq    %r8, %r12
    xorq    %r9, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbx
    andq    %rsi, %r12
    orq %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r14, %rsi
    notq    %r11
    xorq    %rdi, %r12
    movq    %rsi, -88(%rsp)
    movq    48(%rsp), %rsi
    movq    %r11, %r13
    movq    %r12, -40(%rsp)
    orq %r14, %r13
    movq    %r14, %r12
    xorq    %r8, %r13
    andq    %rdi, %r12
    movq    -104(%rsp), %r8
    xorq    64(%rsp), %r9
    xorq    %r11, %r12
    movq    %r13, -80(%rsp)
    xorq    %r10, %rsi
    xorq    -64(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rdi
    xorq    -96(%rsp), %rax
    notq    %rdi
    movq    %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    xorq    -24(%rsp), %r8
    movq    %rdx, %r10
    orq %r9, %r10
    movq    %rax, %r14
    xorq    %rdi, %r10
    orq %rcx, %r14
    andq    %rsi, %rcx
    xorq    %rdx, %r14
    xorq    %rax, %rcx
    xorq    -40(%rsp), %r8
    xorq    %r15, %r8
    xorq    -120(%rsp), %r8
    movq    %r10, -96(%rsp)
    movq    -56(%rsp), %r13
    movq    56(%rsp), %r11
    movq    -72(%rsp), %rdi
    xorq    -32(%rsp), %r13
    xorq    -16(%rsp), %r11
    xorq    16(%rsp), %rdi
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    %rax, %r10
    xorq    -88(%rsp), %r11
    andq    %rdx, %r10
    xorq    %r12, %rdi
    xorq    (%rsp), %r13
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    xorq    %r14, %rdi
    xorq    -8(%rsp), %rdi
    xorq    %rcx, %r11
    xorq    24(%rsp), %r11
    movq    %r13, %rdx
    xorq    %r10, %r9
    xorq    -112(%rsp), %r9
    movq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
    xorq    %rbp, %r9
    xorq    -80(%rsp), %r9
    movq    %r9, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r9, %r11
    movq    -32(%rsp), %r9
    xorq    %r13, %rsi
    xorq    %rsi, %rbp
    xorq    %r11, %r12
    xorq    %rax, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    -120(%rsp), %rdi
    movq    %r13, -120(%rsp)
    movl    $2147516555, %r13d
    xorq    %r8, %rcx
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdx, %rdi
    xorq    %rdi, -120(%rsp)
    xorq    %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %rcx, %rdi
    orq %rcx, %rbp
    movq    -104(%rsp), %r9
    movq    -8(%rsp), %rcx
    movq    %rdi, 32(%rsp)
    xorq    %r12, %rbp
    movq    -16(%rsp), %rdi
    movq    %rbp, -64(%rsp)
    movq    %r13, 8(%rsp)
    xorq    %rdx, %r9
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r8, %rdi
    movq    %r9, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rsi, %r10
    movq    %rbx, %r13
    orq %rdi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    andq    %r9, %r13
    xorq    %rcx, %r12
    notq    %rbp
    xorq    %rdi, %r13
    andq    %rcx, %rdi
    orq %rbx, %rbp
    xorq    %r10, %rdi
    movq    %r12, -8(%rsp)
    xorq    %r9, %rbp
    movq    %r10, %r9
    movq    %rdi, -16(%rsp)
    orq %rcx, %r9
    movq    -72(%rsp), %rdi
    movq    %r13, -32(%rsp)
    xorq    %rbx, %r9
    movq    (%rsp), %rcx
    xorq    %rdx, %r15
    movq    %r9, 48(%rsp)
    movq    -88(%rsp), %r10
    movq    -112(%rsp), %r9
    movq    %rbp, -104(%rsp)
    xorq    %r11, %rdi
    xorq    %rax, %rcx
    xorq    %r8, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %rdi, %r12
    movq    %r10, %r13
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    orq %r9, %r12
    andq    %rdi, %r13
    movq    %r10, %rbp
    movq    %r15, %rbx
    xorq    %rcx, %r12
    xorq    %r9, %r13
    andq    %r15, %rbp
    orq %rcx, %rbx
    andq    %rcx, %r9
    movq    %r12, -112(%rsp)
    xorq    %rdi, %rbp
    xorq    %r10, %rbx
    xorq    %r15, %r9
    movq    -80(%rsp), %r10
    movq    %rbx, -72(%rsp)
    movq    24(%rsp), %rdi
    movq    %r9, 64(%rsp)
    movq    -56(%rsp), %r9
    movq    %r13, -88(%rsp)
    movq    -24(%rsp), %rcx
    xorq    %r8, %rdi
    xorq    %rsi, %r10
    xorq    %r11, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %rax, %r9
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %rbx
    movq    %r9, %r12
    xorq    %rdx, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %rcx, %rbx
    andq    %rcx, %r12
    orq %rdi, %rcx
    xorq    %rdi, %r12
    xorq    56(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r14, %rcx
    orq %r14, %r13
    movq    %r12, -24(%rsp)
    xorq    %r9, %r13
    movq    %rcx, -56(%rsp)
    movq    -112(%rsp), %r9
    movq    16(%rsp), %rcx
    movq    %r14, %r12
    movq    %r13, -80(%rsp)
    andq    %rdi, %r12
    movq    -88(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    -8(%rsp), %r9
    xorq    %r10, %r12
    xorq    %r11, %rcx
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %rdi
    xorq    40(%rsp), %rsi
    notq    %rdi
    xorq    -40(%rsp), %rdx
    movq    %rdi, %r15
    andq    %r8, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r15
    xorq    -24(%rsp), %r9
    movq    %rdx, %r10
    xorq    -32(%rsp), %r13
    orq %r8, %r10
    movq    %rax, %r11
    xorq    %rdi, %r10
    andq    %rdx, %r11
    movq    -72(%rsp), %rdi
    movq    %r10, -96(%rsp)
    xorq    %r8, %r11
    movq    8(%rsp), %r8
    movq    %rax, %r14
    xorq    %r15, %r9
    xorq    %rbx, %r13
    orq %rsi, %r14
    andq    %rcx, %rsi
    xorq    %r10, %r13
    movq    64(%rsp), %r10
    xorq    %rdx, %r14
    xorq    %r11, %r8
    xorq    48(%rsp), %rdi
    xorq    %rax, %rsi
    xorq    -104(%rsp), %r8
    xorq    -48(%rsp), %r13
    xorq    -16(%rsp), %r10
    xorq    %r12, %rdi
    xorq    -120(%rsp), %r9
    xorq    %rbp, %r8
    xorq    %r14, %rdi
    xorq    -80(%rsp), %r8
    movq    %r13, %rdx
    xorq    -56(%rsp), %r10
    xorq    -64(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    xorq    %rsi, %r10
    xorq    32(%rsp), %r10
    movq    %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
    xorq    %r13, %rcx
    xorq    %r10, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    -32(%rsp), %r8
    xorq    %rcx, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %r10, %r12
    xorq    %rax, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    xorq    %r9, %rsi
    xorq    %rcx, %r11
    movq    %r13, -120(%rsp)
    movabsq $-9223372036854775669, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    xorq    %rdx, %rdi
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdi, -120(%rsp)
    xorq    %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r8, %rdi
    xorq    %rsi, %rdi
    orq %rsi, %rbp
    movq    -112(%rsp), %r8
    movq    -64(%rsp), %rsi
    movq    %rdi, 24(%rsp)
    xorq    %r12, %rbp
    movq    -16(%rsp), %rdi
    movq    %rbp, -40(%rsp)
    movq    %r13, 40(%rsp)
    movq    %rbx, %r13
    xorq    %rdx, %r8
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %rdi
    movq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    andq    %r8, %r13
    orq %rdi, %r12
    notq    %rbp
    xorq    %rdi, %r13
    andq    %rsi, %rdi
    orq %rbx, %rbp
    xorq    %r11, %rdi
    xorq    %rsi, %r12
    xorq    %r8, %rbp
    movq    %r11, %r8
    movq    -56(%rsp), %r11
    orq %rsi, %r8
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    xorq    %rbx, %r8
    movq    -48(%rsp), %rsi
    movq    %r13, -32(%rsp)
    movq    %r8, 16(%rsp)
    movq    -104(%rsp), %r8
    xorq    %rdx, %r15
    xorq    %r9, %r11
    movq    %r12, -64(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    notq    %r11
    xorq    %r10, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rbp, -112(%rsp)
    movq    %rdi, %r12
    movq    %r11, %rbp
    movq    %r15, %rbx
    xorq    %rax, %rsi
    xorq    %rcx, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %rdi, %r13
    orq %r8, %r12
    andq    %r15, %rbp
    orq %rsi, %rbx
    xorq    %rsi, %r12
    xorq    %r8, %r13
    xorq    %rdi, %rbp
    xorq    %r11, %rbx
    andq    %rsi, %r8
    movq    -80(%rsp), %r11
    xorq    %r15, %r8
    movq    %rbx, -72(%rsp)
    movq    -8(%rsp), %rsi
    movq    %r8, 56(%rsp)
    movq    32(%rsp), %rdi
    movq    %r13, -56(%rsp)
    movq    -88(%rsp), %r8
    movq    %r12, -104(%rsp)
    xorq    %r9, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdx, %rsi
    xorq    %rcx, %r11
    xorq    %r10, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    notq    %r11
    xorq    %rax, %r8
    movq    %r11, %r13
    xorq    64(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r14, %r13
    xorq    8(%rsp), %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
    orq %r8, %rbx
    movq    %r8, %r12
    movq    -104(%rsp), %r8
    movq    %r13, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbx
    andq    %rsi, %r12
    orq %rdi, %rsi
    xorq    %r14, %rsi
    xorq    %rdi, %r12
    xorq    -24(%rsp), %rdx
    movq    %rsi, -88(%rsp)
    movq    48(%rsp), %rsi
    xorq    -64(%rsp), %r8
    movq    %r12, -8(%rsp)
    movq    %r14, %r12
    andq    %rdi, %r12
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r10, %rsi
    xorq    %r11, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rdi
    xorq    -8(%rsp), %r8
    notq    %rdi
    movq    %rdi, %r15
    andq    %r9, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r15, %r8
    movq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    -120(%rsp), %r8
    orq %r9, %r10
    movq    %rax, %r14
    xorq    %rdi, %r10
    orq %rcx, %r14
    movq    %r10, -96(%rsp)
    movq    -56(%rsp), %r13
    andq    %rsi, %rcx
    movq    56(%rsp), %r11
    xorq    %rax, %rcx
    xorq    %rdx, %r14
    movq    -72(%rsp), %rdi
    xorq    -32(%rsp), %r13
    xorq    -16(%rsp), %r11
    xorq    16(%rsp), %rdi
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    %rax, %r10
    xorq    -88(%rsp), %r11
    andq    %rdx, %r10
    xorq    (%rsp), %r13
    xorq    %r12, %rdi
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    xorq    %r14, %rdi
    xorq    -40(%rsp), %rdi
    xorq    %rcx, %r11
    xorq    24(%rsp), %r11
    movq    %r13, %rdx
    xorq    %r10, %r9
    xorq    -112(%rsp), %r9
    movq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rbp, %r9
    xorq    -80(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %r9, %r11
    movq    %r9, %rax
    movq    -32(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbp
    xorq    %rax, %r9
    xorq    %rdi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    movq    -120(%rsp), %rdi
    xorq    %r11, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    orq %r9, %r13
    xorq    %r8, %rcx
    xorq    %rsi, %r10
    movq    %r13, -120(%rsp)
    xorq    %rax, %rbx
    movabsq $-9223372036854742903, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    xorq    %rdx, %rdi
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdi, -120(%rsp)
    xorq    %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %rcx, %rdi
    movq    -104(%rsp), %r9
    orq %rcx, %rbp
    movq    %rdi, 32(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %r12, %rbp
    movq    -40(%rsp), %rcx
    movq    %rbp, -24(%rsp)
    movq    %r10, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rdx, %r9
    movq    %r13, 8(%rsp)
    notq    %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %r8, %rdi
    movq    %rbx, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r12
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r12
    andq    %r9, %r13
    orq %rbx, %rbp
    xorq    %rdi, %r13
    xorq    %r9, %rbp
    andq    %rcx, %rdi
    movq    %r10, %r9
    xorq    %r10, %rdi
    movq    -88(%rsp), %r10
    orq %rcx, %r9
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    xorq    %rbx, %r9
    movq    (%rsp), %rcx
    movq    %r13, -32(%rsp)
    movq    %r9, 48(%rsp)
    movq    -112(%rsp), %r9
    xorq    %rdx, %r15
    xorq    %r8, %r10
    movq    %r12, -40(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %r11, %rdi
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rdi, %r13
    movq    %rdi, %r12
    xorq    %rax, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
    movq    %rbp, -104(%rsp)
    movq    %r15, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    xorq    %r9, %r13
    orq %r9, %r12
    andq    %rcx, %r9
    xorq    %rcx, %r12
    andq    %r15, %rbp
    xorq    %r15, %r9
    orq %rcx, %rbx
    movq    %r12, -112(%rsp)
    xorq    %rdi, %rbp
    xorq    %r10, %rbx
    movq    %r9, 64(%rsp)
    movq    -64(%rsp), %rcx
    movq    %rbx, -72(%rsp)
    movq    -56(%rsp), %r9
    movq    %r13, -88(%rsp)
    movq    -80(%rsp), %r10
    movq    24(%rsp), %rdi
    xorq    %rdx, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rax, %r9
    xorq    %rsi, %r10
    xorq    %r11, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    notq    %r10
    xorq    56(%rsp), %r8
    movq    %r10, %r13
    xorq    40(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r14, %r13
    orq %r9, %rbx
    movq    %r9, %r12
    xorq    %r9, %r13
    movq    -112(%rsp), %r9
    xorq    %rcx, %rbx
    andq    %rcx, %r12
    orq %rdi, %rcx
    movq    %r13, -80(%rsp)
    xorq    %r14, %rcx
    xorq    %rdi, %r12
    movq    -88(%rsp), %r13
    movq    %rcx, -56(%rsp)
    movq    16(%rsp), %rcx
    xorq    -40(%rsp), %r9
    movq    %r12, -64(%rsp)
    movq    %r14, %r12
    andq    %rdi, %r12
    xorq    -8(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r11, %rcx
    xorq    %r10, %r12
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %rdi
    xorq    -64(%rsp), %r9
    notq    %rdi
    movq    %rdi, %r15
    andq    %r8, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    xorq    %rsi, %r15
    xorq    %r15, %r9
    orq %r8, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    -120(%rsp), %r9
    xorq    %rdi, %r10
    movq    %rax, %r11
    xorq    -32(%rsp), %r13
    movq    %r10, -96(%rsp)
    andq    %rdx, %r11
    xorq    %r8, %r11
    movq    -72(%rsp), %rdi
    movq    %rax, %r14
    movq    8(%rsp), %r8
    orq %rsi, %r14
    andq    %rcx, %rsi
    xorq    %rdx, %r14
    xorq    %rax, %rsi
    xorq    %rbx, %r13
    xorq    %r10, %r13
    movq    64(%rsp), %r10
    xorq    %r11, %r8
    xorq    48(%rsp), %rdi
    xorq    -104(%rsp), %r8
    xorq    -48(%rsp), %r13
    xorq    -16(%rsp), %r10
    xorq    %r12, %rdi
    xorq    %rbp, %r8
    xorq    %r14, %rdi
    xorq    -80(%rsp), %r8
    movq    %r13, %rdx
    xorq    -56(%rsp), %r10
    xorq    -24(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    xorq    %rsi, %r10
    xorq    32(%rsp), %r10
    movq    %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rdi, %r9
    movq    -120(%rsp), %rdi
    xorq    %r10, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r8, %r10
    movq    -32(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %rcx
    xorq    %rdx, %rdi
    xorq    %r10, %r12
    xorq    %rcx, %rbp
    xorq    %r9, %rsi
    xorq    %rax, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rax, %r8
    xorq    %rcx, %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    movq    %r13, -120(%rsp)
    movabsq $-9223372036854743037, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdi, -120(%rsp)
    xorq    %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r13, (%rsp)
    movq    %rsi, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r8, %rdi
    xorq    %rsi, %rdi
    movq    -112(%rsp), %r8
    orq %rsi, %rbp
    movq    %rdi, 24(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %r12, %rbp
    movq    -24(%rsp), %rsi
    movq    %r13, 40(%rsp)
    movq    %rbx, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rdx, %r8
    movq    %rbp, -8(%rsp)
    movq    %r11, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r12
    xorq    %r9, %rdi
    andq    %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    notq    %rbp
    orq %rdi, %r12
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
    xorq    %rdi, %r13
    orq %rbx, %rbp
    xorq    %r8, %rbp
    andq    %rsi, %rdi
    movq    %r11, %r8
    xorq    %r11, %rdi
    orq %rsi, %r8
    movq    -56(%rsp), %r11
    xorq    %rbx, %r8
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r8, 16(%rsp)
    movq    -48(%rsp), %rsi
    xorq    %rdx, %r15
    movq    -104(%rsp), %r8
    movq    %r13, -32(%rsp)
    xorq    %r9, %r11
    movq    %r12, -24(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %r13
    xorq    %r10, %rdi
    notq    %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rdi, %r13
    movq    %rdi, %r12
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r8
    movq    %rbp, -112(%rsp)
    movq    %r15, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r11, %rbp
    xorq    %r8, %r13
    orq %r8, %r12
    andq    %rsi, %r8
    andq    %r15, %rbp
    orq %rsi, %rbx
    xorq    %r15, %r8
    xorq    %rsi, %r12
    xorq    %rdi, %rbp
    xorq    %r11, %rbx
    movq    -40(%rsp), %rsi
    movq    %r8, 56(%rsp)
    movq    -80(%rsp), %r11
    movq    %rbx, -72(%rsp)
    movq    -88(%rsp), %r8
    movq    %r13, -56(%rsp)
    movq    32(%rsp), %rdi
    movq    %r12, -104(%rsp)
    xorq    %rdx, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
    xorq    %r9, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rcx, %r11
    xorq    %r10, %r14
    movq    %r8, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rbx
    notq    %r11
    andq    %rsi, %r12
    movq    %r11, %r13
    orq %r8, %rbx
    xorq    %rdi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r14, %r13
    xorq    %rsi, %rbx
    orq %rdi, %rsi
    xorq    %r8, %r13
    movq    -104(%rsp), %r8
    xorq    %r14, %rsi
    movq    %rsi, -88(%rsp)
    movq    48(%rsp), %rsi
    movq    %r12, -40(%rsp)
    movq    %r14, %r12
    xorq    8(%rsp), %rcx
    andq    %rdi, %r12
    xorq    64(%rsp), %r9
    movq    %r13, -80(%rsp)
    xorq    -24(%rsp), %r8
    xorq    %r11, %r12
    xorq    %r10, %rsi
    xorq    -64(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %rsi, %rdi
    xorq    -96(%rsp), %rax
    notq    %rdi
    xorq    -40(%rsp), %r8
    movq    %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    andq    %r9, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    xorq    %rcx, %r15
    orq %r9, %r10
    xorq    %r15, %r8
    xorq    %rdi, %r10
    xorq    -120(%rsp), %r8
    movq    %r10, -96(%rsp)
    movq    -56(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    56(%rsp), %r11
    movq    %rax, %r14
    movq    -72(%rsp), %rdi
    xorq    -32(%rsp), %r13
    xorq    %rbx, %r13
    xorq    -16(%rsp), %r11
    orq %rcx, %r14
    xorq    %r10, %r13
    movq    %rax, %r10
    xorq    16(%rsp), %rdi
    andq    %rdx, %r10
    andq    %rsi, %rcx
    xorq    (%rsp), %r13
    xorq    %r9, %r10
    movq    40(%rsp), %r9
    xorq    %rdx, %r14
    xorq    -88(%rsp), %r11
    xorq    %rax, %rcx
    xorq    %r12, %rdi
    xorq    %r14, %rdi
    movq    %r13, %rdx
    xorq    %r10, %r9
    xorq    -8(%rsp), %rdi
    xorq    -112(%rsp), %r9
    xorq    %rcx, %r11
    xorq    24(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdi, %rsi
    xorq    %rbp, %r9
    xorq    -80(%rsp), %r9
    xorq    %r11, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r13, %rsi
    xorq    %r9, %r11
    movq    %r9, %rax
    movq    -32(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r8
    movq    -120(%rsp), %rdi
    xorq    %rax, %r9
    xorq    %rdx, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbp
    xorq    %r11, %r12
    xorq    %r8, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    xorq    %rsi, %r10
    xorq    %rax, %rbx
    orq %r9, %r13
    movq    %r13, -120(%rsp)
    movabsq $-9223372036854743038, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
#NO_APP
    orq %r12, %r13
    xorq    %rdi, -120(%rsp)
    xorq    %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r13, -48(%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %rcx, %rdi
    movq    -104(%rsp), %r9
    orq %rcx, %rbp
    movq    %rdi, 32(%rsp)
    movq    -16(%rsp), %rdi
    xorq    %r12, %rbp
    movq    -8(%rsp), %rcx
    movq    %rbp, -64(%rsp)
    movq    %r10, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rdx, %r9
    movq    %r13, 8(%rsp)
    notq    %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %rbx, %r13
    movq    %r9, %r12
    xorq    %r8, %rdi
    andq    %r9, %r13
    orq %rbx, %rbp
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r12
    xorq    %rdi, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r12
    xorq    %r9, %rbp
    movq    %r10, %r9
    orq %rcx, %r9
    andq    %rcx, %rdi
    movq    (%rsp), %rcx
    xorq    %rbx, %r9
    xorq    %r10, %rdi
    movq    -112(%rsp), %r10
    movq    %r9, 48(%rsp)
    movq    -88(%rsp), %r9
    xorq    %rdx, %r15
    movq    %rdi, -16(%rsp)
    movq    -72(%rsp), %rdi
    movq    %r13, -32(%rsp)
    xorq    %rax, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    %r12, -8(%rsp)
    xorq    %rsi, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r13
    notq    %r9
    xorq    %r11, %rdi
    movq    %r9, %rbx
    movq    %rbp, -104(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %r15, %rbx
    andq    %rdi, %r13
    movq    %rdi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %rdi, %rbx
    xorq    %r10, %r13
    orq %r10, %r12
    movq    %r15, %rdi
    andq    %rcx, %r10
    xorq    %rcx, %r12
    xorq    %r15, %r10
    orq %rcx, %rdi
    movq    %r12, -112(%rsp)
    xorq    %r9, %rdi
    movq    %r10, 64(%rsp)
    movq    -56(%rsp), %r10
    movq    %rdi, -72(%rsp)
    movq    -24(%rsp), %rcx
    movq    -80(%rsp), %rbp
    movq    %r13, -88(%rsp)
    movq    24(%rsp), %rdi
    xorq    %rax, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r9
    xorq    %rdx, %rcx
    xorq    %rsi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %r8, %rdi
    movq    %rbp, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r11, %r14
    andq    %rcx, %r9
    notq    %r12
    xorq    %rdi, %r9
    movq    %r12, %r13
    xorq    16(%rsp), %r11
    movq    %r9, -24(%rsp)
    movq    %rbp, %r9
    xorq    -40(%rsp), %rdx
    orq %r10, %r9
    xorq    56(%rsp), %r8
    xorq    %rcx, %r9
    orq %rdi, %rcx
    xorq    -96(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %r14, %rcx
    orq %r14, %r13
    movq    %r14, %rbp
    movq    %rcx, -56(%rsp)
    movq    40(%rsp), %rcx
    xorq    %r10, %r13
    movq    -112(%rsp), %r10
    andq    %rdi, %rbp
    movq    %r13, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rsi, %rcx
    movq    %rdx, %r15
    movq    %r11, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    notq    %rsi
    orq %r8, %r15
    xorq    -8(%rsp), %r10
    xorq    %rsi, %r15
    movq    %rsi, %r14
    movq    -88(%rsp), %rsi
    andq    %r8, %r14
    movq    %r15, -96(%rsp)
    xorq    %r12, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    -24(%rsp), %r10
    xorq    %rcx, %r14
    xorq    -32(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    -72(%rsp), %rdi
    movq    %rax, %r12
    xorq    %r14, %r10
    xorq    %r9, %rsi
    xorq    -120(%rsp), %r10
    xorq    %r15, %rsi
    movq    %rax, %r15
    xorq    -48(%rsp), %rsi
    andq    %rdx, %r15
    orq %rcx, %r12
    xorq    %r8, %r15
    movq    8(%rsp), %r8
    xorq    %rdx, %r12
    xorq    48(%rsp), %rdi
    andq    %rcx, %r11
    xorq    %rax, %r11
    movq    %rsi, %rdx
    xorq    %r15, %r8
    xorq    -104(%rsp), %r8
    xorq    %rbp, %rdi
    xorq    %r12, %rdi
    xorq    -64(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    %rbx, %r8
    xorq    %r13, %r8
    movq    64(%rsp), %r13
    movq    %r8, %rax
    movq    %rdi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    -16(%rsp), %r13
    xorq    %r10, %rax
    xorq    %rsi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    movq    -120(%rsp), %rsi
    xorq    %rdi, %r10
    xorq    %rcx, %rbx
    movq    -32(%rsp), %rdi
    xorq    -56(%rsp), %r13
    xorq    %rax, %rdi
    xorq    %r11, %r13
    xorq    32(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r13, %rdx
    xorq    %rdx, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r8
    xorq    %r13, %rbp
    xorq    %r10, %r11
    orq %rdi, %r8
    xorq    %rcx, %r15
    xorq    %rax, %r9
    movq    %r8, -120(%rsp)
    movabsq $-9223372036854775680, %r8
    xorq    %r8, -120(%rsp)
    movq    %rbx, %r8
    notq    %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %rbp, %r8
    xorq    %rsi, -120(%rsp)
    xorq    %rdi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r11,%r11
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r8, (%rsp)
    movq    %r11, %r8
    andq    %rbp, %r8
    xorq    %rbx, %r8
    movq    %rsi, %rbx
    andq    %rdi, %rsi
    movq    %r8, 40(%rsp)
    movq    -112(%rsp), %r8
    xorq    %r11, %rsi
    movq    -16(%rsp), %rdi
    movq    %rsi, 24(%rsp)
    orq %r11, %rbx
    movq    -64(%rsp), %rsi
    xorq    %rbp, %rbx
    movq    %rbx, -40(%rsp)
    movq    %r15, %rbx
    xorq    %rdx, %r8
    notq    %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %rbp
    xorq    %r10, %rdi
    xorq    %r13, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %rbp, -112(%rsp)
    movq    %r9, %r11
    movq    %r15, %rbp
    andq    %r8, %r11
    orq %r9, %rbx
    orq %rsi, %rbp
    xorq    %r8, %rbx
    xorq    %rdi, %r11
    movq    -56(%rsp), %r8
    xorq    %r9, %rbp
    andq    %rsi, %rdi
    movq    -72(%rsp), %r9
    xorq    %r15, %rdi
    movq    -48(%rsp), %rsi
    movq    %r11, -64(%rsp)
    movq    %rdi, 16(%rsp)
    movq    -104(%rsp), %rdi
    xorq    %rdx, %r14
    xorq    %r10, %r8
    movq    %rbp, -32(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r15
    xorq    %r13, %r9
    notq    %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %r15
    xorq    %rcx, %rdi
    movq    %r9, %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %r15
    xorq    %rax, %rsi
    orq %rdi, %r11
    movq    %r15, -56(%rsp)
    movq    %r8, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r14,%r14
# 0 "" 2
#NO_APP
    andq    %rsi, %rdi
    andq    %r14, %r15
    movq    %r14, %rbp
    xorq    %rsi, %r11
    xorq    %r9, %r15
    xorq    %r14, %rdi
    orq %rsi, %rbp
    movq    -88(%rsp), %r9
    movq    %r11, -104(%rsp)
    xorq    %r8, %rbp
    movq    -80(%rsp), %r11
    movq    %rdi, -16(%rsp)
    movq    -8(%rsp), %r8
    movq    %r13, %rsi
    movq    %rbp, -72(%rsp)
    movq    32(%rsp), %rdi
    xorq    %r12, %rsi
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %rdx, %r8
    xorq    %rcx, %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r10, %rdi
    movq    %rsi, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %r8, %r12
    xorq    48(%rsp), %r13
    andq    %rdi, %r14
    xorq    %rdi, %r12
    xorq    8(%rsp), %rcx
    movq    %r12, -8(%rsp)
    movq    %r11, %r12
    notq    %r11
    orq %r9, %r12
    movq    %r11, %rbp
    xorq    -24(%rsp), %rdx
    xorq    %r8, %r12
    orq %rdi, %r8
    xorq    -96(%rsp), %rax
    xorq    %rsi, %r8
    orq %rsi, %rbp
    movq    64(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r8, -88(%rsp)
    movq    %r13, %r8
    xorq    %r9, %rbp
    notq    %r8
    movq    %rbp, -80(%rsp)
    xorq    %r11, %r14
    movq    %r8, %r9
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rsi,%rsi
# 0 "" 2
#NO_APP
    andq    %rsi, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r9
    movq    %r9, -96(%rsp)
    movq    -104(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    orq %rsi, %r10
    xorq    %r8, %r10
    xorq    -112(%rsp), %rdi
    movq    %r10, 8(%rsp)
    movq    -56(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %rbp
    movq    40(%rsp), %r8
    movq    %rax, %r11
    xorq    -8(%rsp), %rdi
    andq    %rdx, %rbp
    xorq    -64(%rsp), %r10
    xorq    %r9, %rdi
    movq    -16(%rsp), %r9
    xorq    %r12, %r10
    xorq    -120(%rsp), %rdi
    xorq    8(%rsp), %r10
    xorq    (%rsp), %r10
    xorq    %rsi, %rbp
    andq    %rcx, %r13
    xorq    16(%rsp), %r9
    xorq    %rbp, %r8
    xorq    %rax, %r13
    movq    -72(%rsp), %rsi
    xorq    %rbx, %r8
    orq %rcx, %r11
    xorq    %rdx, %r11
    xorq    %r15, %r8
    xorq    -80(%rsp), %r8
    movq    %r10, %rdx
    xorq    -88(%rsp), %r9
    xorq    -32(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rax
    xorq    %r13, %r9
    xorq    %r14, %rsi
    xorq    24(%rsp), %r9
    xorq    %r11, %rsi
    xorq    -40(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    %r9, %rdx
    xorq    %rdi, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    -64(%rsp), %r8
    movq    %rsi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rdi
    movq    -120(%rsp), %rsi
    xorq    %r9, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r10, %rcx
    xorq    %rax, %r8
    xorq    %rcx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %r10
    xorq    %rdx, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r8, %r10
    xorq    %rdi, %r13
    xorq    %rcx, %rbp
    xorq    $32778, %r10
    xorq    %rax, %r12
    xorq    %rsi, %r10
    movq    %r10, -120(%rsp)
    movq    %r15, %r10
    notq    %r10
    orq %r14, %r10
    xorq    %r8, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r10, -48(%rsp)
    movq    %r13, %r10
    andq    %r14, %r10
    xorq    %r15, %r10
    movq    %rsi, %r15
    andq    %r8, %rsi
    movq    %r10, -24(%rsp)
    movq    -104(%rsp), %r10
    xorq    %r13, %rsi
    movq    16(%rsp), %r8
    movq    %rsi, 32(%rsp)
    orq %r13, %r15
    movq    -40(%rsp), %rsi
    xorq    %r14, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbp,%rbp
# 0 "" 2
#NO_APP
    xorq    %rdx, %r10
    movq    %r15, -64(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %rdi, %r8
    xorq    %r9, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r13
    movq    %r13, -40(%rsp)
    movq    %rbp, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r12,%r12
# 0 "" 2
#NO_APP
    notq    %r13
    movq    %r12, %r15
    andq    %r10, %r15
    orq %r12, %r13
    xorq    %r8, %r15
    xorq    %r10, %r13
    movq    -72(%rsp), %r10
    movq    %r15, 16(%rsp)
    movq    %rbp, %r15
    andq    %rsi, %r8
    orq %rsi, %r15
    movq    (%rsp), %rsi
    movq    %r13, -104(%rsp)
    xorq    %r12, %r15
    xorq    %rbp, %r8
    movq    -88(%rsp), %rbp
    xorq    %r9, %r10
    movq    %r8, 56(%rsp)
    movq    %rcx, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r12
    xorq    %rbx, %r8
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r12
    movq    -96(%rsp), %rbx
    xorq    %rdi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
    movq    %r15, 48(%rsp)
    xorq    %r9, %r11
    movq    %r12, -96(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
    movq    %rbp, %r12
    xorq    %rdx, %rbx
    andq    %r10, %r13
    notq    %r12
    xorq    %r8, %r13
    movq    %r12, %rbp
    andq    %rsi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rbx, %r8
    andq    %rbx, %rbp
    movq    %rbx, %r15
    movq    -56(%rsp), %rbx
    movq    %r8, 64(%rsp)
    orq %rsi, %r15
    movq    -112(%rsp), %r8
    xorq    %r12, %r15
    xorq    %r10, %rbp
    movq    24(%rsp), %rsi
    movq    %r13, -88(%rsp)
    movq    -80(%rsp), %r12
    movq    %r15, -72(%rsp)
    xorq    %rax, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r10
    xorq    %rdx, %r8
    xorq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r10
    xorq    %rcx, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r10, -112(%rsp)
    movq    %r12, %r10
    notq    %r12
    movq    %r12, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
#NO_APP
    orq %rbx, %r10
    orq %r11, %r13
    xorq    -32(%rsp), %r9
    xorq    %rbx, %r13
    movq    %r11, %rbx
    xorq    -8(%rsp), %rdx
    xorq    %r8, %r10
    andq    %rsi, %rbx
    xorq    -16(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rsi, %r8
    movq    %rdx, %r15
    movq    %r9, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    notq    %rsi
    orq %rdi, %r15
    xorq    %r11, %r8
    xorq    %rsi, %r15
    xorq    8(%rsp), %rax
    movq    %r8, -56(%rsp)
    movq    %r15, 8(%rsp)
    movq    -96(%rsp), %r8
    xorq    %r12, %rbx
    movq    -88(%rsp), %r15
    movq    %rsi, %r12
    movq    %r13, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r14
    andq    %rdi, %r12
    xorq    40(%rsp), %rcx
    xorq    -40(%rsp), %r8
    andq    %rdx, %r14
    movq    %rax, %r11
    xorq    16(%rsp), %r15
    xorq    %rdi, %r14
    movq    -24(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    -112(%rsp), %r8
    xorq    %rcx, %r12
    xorq    %r10, %r15
    movq    -72(%rsp), %rsi
    xorq    8(%rsp), %r15
    xorq    %r14, %rdi
    xorq    %r12, %r8
    xorq    -120(%rsp), %r8
    xorq    -48(%rsp), %r15
    xorq    -104(%rsp), %rdi
    orq %rcx, %r11
    andq    %rcx, %r9
    xorq    48(%rsp), %rsi
    xorq    %rdx, %r11
    xorq    %rax, %r9
    movq    %r15, %rdx
    xorq    %rbp, %rdi
    xorq    %r13, %rdi
    movq    64(%rsp), %r13
    xorq    %rbx, %rsi
    xorq    %r11, %rsi
    movq    %rdi, %rax
    xorq    -64(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    xorq    56(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rsi, %rcx
    xorq    %r8, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    -56(%rsp), %r13
    xorq    %rsi, %r8
    movq    -120(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r15, %rcx
    xorq    %r9, %r13
    xorq    %rcx, %rbp
    xorq    %r8, %r9
    xorq    32(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r15
    xorq    %r13, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rdi, %r13
    movq    16(%rsp), %rdi
    xorq    %rdx, %rsi
    xorq    %r13, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rax, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rdi, %r15
    xorq    %rax, %r10
    xorq    %rcx, %r14
    movq    %r15, -120(%rsp)
    movabsq $-9223372034707292150, %r15
    xorq    %r15, -120(%rsp)
    movq    %rbp, %r15
    notq    %r15
    orq %rbx, %r15
    xorq    %rdi, %r15
    xorq    %rsi, -120(%rsp)
    movq    %r15, (%rsp)
    movq    %r9, %r15
    andq    %rbx, %r15
    xorq    %rbp, %r15
    movq    %rsi, %rbp
    andq    %rdi, %rsi
    xorq    %r9, %rsi
    movq    56(%rsp), %rdi
    orq %r9, %rbp
    movq    %rsi, 24(%rsp)
    movq    -64(%rsp), %rsi
    xorq    %rbx, %rbp
    movq    -96(%rsp), %r9
    movq    %r15, 40(%rsp)
    movq    %rbp, -8(%rsp)
    xorq    %r8, %rdi
    xorq    %r13, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %rdx, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %rbx
    movq    %r9, %r15
    xorq    %rdi, %rbx
    orq %rdi, %r15
    andq    %rsi, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    movq    %rbx, -64(%rsp)
    movq    %r14, %rbx
    movq    %r14, %rbp
    notq    %rbx
    orq %rsi, %rbp
    xorq    %r14, %rdi
    orq %r10, %rbx
    xorq    %r10, %rbp
    movq    -72(%rsp), %r10
    xorq    %r9, %rbx
    movq    -48(%rsp), %r9
    movq    %rdi, 16(%rsp)
    movq    -104(%rsp), %rdi
    xorq    %rsi, %r15
    movq    %rbp, -32(%rsp)
    movq    -56(%rsp), %rsi
    movq    %r15, -96(%rsp)
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r13, %r10
    xorq    %rcx, %rdi
    xorq    %r8, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r15
    xorq    %rdx, %r12
    xorq    %r13, %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %r15
    movq    %rsi, %rbp
    notq    %rsi
    movq    %r15, -104(%rsp)
    movq    %rsi, %r15
    andq    %r10, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r12,%r12
# 0 "" 2
#NO_APP
    andq    %r12, %r15
    xorq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %r10, %r15
    movq    %r12, %r10
    xorq    %r12, %rdi
    orq %r9, %r10
    movq    -88(%rsp), %r9
    movq    %rdi, -16(%rsp)
    movq    -40(%rsp), %rdi
    xorq    %rsi, %r10
    movq    %rbp, -56(%rsp)
    movq    32(%rsp), %rsi
    movq    %r10, -72(%rsp)
    movq    -80(%rsp), %r10
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %rdx, %rdi
    xorq    %r8, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rdi, %r12
    xorq    %rcx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r12, -40(%rsp)
    movq    %r10, %r12
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r9, %r12
    movq    %r10, %rbp
    movq    %r11, %r14
    xorq    %rdi, %r12
    orq %r11, %rbp
    andq    %rsi, %r14
    orq %rsi, %rdi
    xorq    48(%rsp), %r13
    xorq    %r9, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, %rsi
    xorq    64(%rsp), %r8
    xorq    %r11, %rdi
    notq    %rsi
    xorq    -24(%rsp), %rcx
    xorq    %r10, %r14
    movq    %rsi, %r9
    xorq    -112(%rsp), %rdx
    movq    %rbp, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r9
    xorq    8(%rsp), %rax
    movq    %rdi, -88(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r9
    movq    %r9, -112(%rsp)
    movq    -104(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    orq %r8, %r10
    xorq    %rsi, %r10
    xorq    -96(%rsp), %rdi
    movq    %r10, 8(%rsp)
    movq    -56(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %rbp
    movq    -72(%rsp), %rsi
    movq    %rax, %r11
    xorq    -40(%rsp), %rdi
    andq    %rdx, %rbp
    xorq    -64(%rsp), %r10
    xorq    %r8, %rbp
    movq    40(%rsp), %r8
    xorq    %r9, %rdi
    movq    -16(%rsp), %r9
    xorq    %r12, %r10
    xorq    -120(%rsp), %rdi
    xorq    8(%rsp), %r10
    xorq    %rbp, %r8
    xorq    %rbx, %r8
    xorq    %r15, %r8
    xorq    (%rsp), %r10
    xorq    -80(%rsp), %r8
    andq    %rcx, %r13
    orq %rcx, %r11
    xorq    16(%rsp), %r9
    xorq    %rax, %r13
    xorq    %rdx, %r11
    xorq    -32(%rsp), %rsi
    movq    %r10, %rdx
    movq    %r8, %rax
    xorq    -88(%rsp), %r9
    xorq    %r14, %rsi
    xorq    %r11, %rsi
    xorq    -8(%rsp), %rsi
    xorq    %r13, %r9
    xorq    24(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rsi, %rcx
    xorq    %rdi, %rax
    xorq    %r9, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    -64(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %r10, %rcx
    xorq    %rsi, %rdi
    movq    -120(%rsp), %rsi
    xorq    %rcx, %r15
    xorq    %rax, %r8
    xorq    %r9, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r15,%r15
# 0 "" 2
#NO_APP
    movq    %r15, %r10
    xorq    %rdi, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r10
    xorq    %rdx, %rsi
    movq    %r10, -120(%rsp)
    movabsq $-9223372034707259263, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r14,%r14
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %r10, -120(%rsp)
    movq    %r15, %r10
    xorq    %rax, %r12
    notq    %r10
    xorq    %rcx, %rbp
    orq %r14, %r10
    xorq    %r8, %r10
    xorq    %rsi, -120(%rsp)
    movq    %r10, -48(%rsp)
    movq    %r13, %r10
    andq    %r14, %r10
    xorq    %r15, %r10
    movq    %rsi, %r15
    andq    %r8, %rsi
    movq    %r10, -24(%rsp)
    movq    -104(%rsp), %r10
    xorq    %r13, %rsi
    movq    16(%rsp), %r8
    movq    %rsi, 32(%rsp)
    orq %r13, %r15
    movq    -8(%rsp), %rsi
    xorq    %r14, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r12,%r12
# 0 "" 2
#NO_APP
    xorq    %rdx, %r10
    movq    %r15, -64(%rsp)
    movq    %r12, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r13
    xorq    %rdi, %r8
    xorq    %r9, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r13
    andq    %r10, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r13
    xorq    %r8, %r15
    andq    %rsi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r13, -8(%rsp)
    movq    %rbp, %r13
    xorq    %rbp, %r8
    notq    %r13
    movq    %r15, 16(%rsp)
    movq    %rbp, %r15
    orq %r12, %r13
    orq %rsi, %r15
    movq    (%rsp), %rsi
    xorq    %r10, %r13
    movq    -72(%rsp), %r10
    movq    %r8, 56(%rsp)
    movq    -88(%rsp), %rbp
    movq    %rcx, %r8
    xorq    %r12, %r15
    xorq    %rbx, %r8
    movq    -112(%rsp), %rbx
    movq    %r13, -104(%rsp)
    xorq    %rax, %rsi
    movq    %r15, 48(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r9, %r10
    xorq    %rdi, %rbp
    xorq    %rdx, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r12
    xorq    %r9, %r11
    orq %r8, %r12
    xorq    %rsi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %r12, -112(%rsp)
    movq    %rbp, %r13
    movq    %rbp, %r12
    andq    %r10, %r13
    notq    %r12
    xorq    %r8, %r13
    movq    %r12, %rbp
    andq    %rsi, %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rbx, %r8
    andq    %rbx, %rbp
    movq    %rbx, %r15
    movq    -56(%rsp), %rbx
    movq    %r8, 64(%rsp)
    orq %rsi, %r15
    movq    -96(%rsp), %r8
    xorq    %r12, %r15
    xorq    %r10, %rbp
    movq    24(%rsp), %rsi
    movq    %r13, -88(%rsp)
    movq    -80(%rsp), %r12
    movq    %r15, -72(%rsp)
    xorq    %rax, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbx, %r10
    xorq    %rdx, %r8
    xorq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r10
    xorq    %rcx, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r10, -96(%rsp)
    movq    %r12, %r10
    notq    %r12
    movq    %r12, %r13
    orq %rbx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
#NO_APP
    orq %r11, %r13
    xorq    %r8, %r10
    xorq    %rbx, %r13
    movq    %r11, %rbx
    andq    %rsi, %rbx
    orq %rsi, %r8
    xorq    -32(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %rsi
    xorq    8(%rsp), %rax
    xorq    %r12, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    xorq    -40(%rsp), %rdx
    notq    %rsi
    movq    %rax, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r15
    xorq    -16(%rsp), %rdi
    andq    %rdx, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rsi, %r12
    orq %rdi, %r15
    xorq    %r11, %r8
    xorq    %rsi, %r15
    xorq    %rdi, %r14
    andq    %rdi, %r12
    movq    -24(%rsp), %rdi
    movq    %r8, -56(%rsp)
    movq    %rax, %r11
    movq    %r15, 8(%rsp)
    movq    -112(%rsp), %r8
    movq    -88(%rsp), %r15
    movq    %r13, -80(%rsp)
    xorq    40(%rsp), %rcx
    xorq    %r14, %rdi
    movq    -72(%rsp), %rsi
    xorq    -8(%rsp), %r8
    xorq    16(%rsp), %r15
    xorq    -104(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    -96(%rsp), %r8
    xorq    %rcx, %r12
    orq %rcx, %r11
    xorq    %r10, %r15
    xorq    %rbp, %rdi
    xorq    8(%rsp), %r15
    xorq    %r13, %rdi
    movq    64(%rsp), %r13
    xorq    %r12, %r8
    xorq    -120(%rsp), %r8
    xorq    -48(%rsp), %r15
    xorq    %rdx, %r11
    andq    %rcx, %r9
    xorq    56(%rsp), %r13
    xorq    %rax, %r9
    movq    %rdi, %rax
    xorq    48(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %r15, %rdx
    xorq    %r8, %rax
    xorq    -56(%rsp), %r13
    xorq    %rbx, %rsi
    xorq    %r11, %rsi
    xorq    -64(%rsp), %rsi
    xorq    %r9, %r13
    xorq    32(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rsi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %rdx
    xorq    %r15, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rdi, %r13
    movq    16(%rsp), %rdi
    xorq    %rcx, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r15
    xorq    %r13, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rax, %rdi
    xorq    %rsi, %r8
    movq    -120(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r15
    xorq    %r8, %r9
    movq    %r15, -120(%rsp)
    movabsq $-9223372036854742912, %r15
    xorq    %r15, -120(%rsp)
    movq    %rbp, %r15
    xorq    %rdx, %rsi
    notq    %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%rbx,%rbx
# 0 "" 2
#NO_APP
    xorq    %rsi, -120(%rsp)
    orq %rbx, %r15
    xorq    %rax, %r10
    xorq    %rdi, %r15
    xorq    %rcx, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r15, (%rsp)
    movq    %r9, %r15
    andq    %rbx, %r15
    xorq    %rbp, %r15
    movq    %rsi, %rbp
    andq    %rdi, %rsi
    xorq    %r9, %rsi
    orq %r9, %rbp
    movq    -112(%rsp), %r9
    movq    56(%rsp), %rdi
    xorq    %rbx, %rbp
    movq    %rsi, 24(%rsp)
    movq    %r10, %rbx
    movq    -64(%rsp), %rsi
    movq    %r15, 40(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %rdx, %r9
    movq    %rbp, -40(%rsp)
    movq    %r14, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r9,%r9
# 0 "" 2
#NO_APP
    andq    %r9, %rbx
    xorq    %r8, %rdi
    movq    %r9, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rdi, %rbx
    xorq    %r13, %rsi
    orq %rdi, %r15
    movq    %rbx, -112(%rsp)
    movq    %r14, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
#NO_APP
    notq    %rbx
    andq    %rsi, %rdi
    orq %rsi, %rbp
    xorq    %r14, %rdi
    orq %r10, %rbx
    xorq    %r10, %rbp
    xorq    %r9, %rbx
    movq    -72(%rsp), %r10
    movq    %rdi, 16(%rsp)
    movq    -48(%rsp), %r9
    xorq    %rsi, %r15
    movq    %rbp, -32(%rsp)
    movq    -104(%rsp), %rdi
    movq    %r15, -64(%rsp)
    movq    -56(%rsp), %rsi
    xorq    %r13, %r10
    xorq    %rax, %r9
    xorq    %rcx, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rdi,%rdi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %r15
    xorq    %r8, %rsi
    xorq    %rdx, %r12
    orq %rdi, %r15
    xorq    %r13, %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %r9, %r15
    movq    %rsi, %rbp
    notq    %rsi
    movq    %r15, -48(%rsp)
    movq    %rsi, %r15
    andq    %r10, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r12,%r12
# 0 "" 2
#NO_APP
    andq    %r12, %r15
    xorq    %rdi, %rbp
    andq    %r9, %rdi
    xorq    %r10, %r15
    movq    %r12, %r10
    xorq    %r12, %rdi
    orq %r9, %r10
    movq    -88(%rsp), %r9
    movq    %rdi, -72(%rsp)
    movq    -8(%rsp), %rdi
    xorq    %rsi, %r10
    movq    %rbp, -104(%rsp)
    movq    32(%rsp), %rsi
    movq    %r10, -56(%rsp)
    movq    -80(%rsp), %r10
    xorq    %rax, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r9,%r9
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    xorq    %rdx, %rdi
    xorq    %r8, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%rdi,%rdi
# 0 "" 2
#NO_APP
    andq    %rdi, %r12
    xorq    %rcx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r12, -8(%rsp)
    movq    %r10, %r12
    notq    %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r10, %rbp
    movq    %r11, %r14
    orq %r9, %r12
    orq %r11, %rbp
    andq    %rsi, %r14
    xorq    %rdi, %r12
    xorq    %r9, %rbp
    xorq    %r10, %r14
    orq %rsi, %rdi
    xorq    48(%rsp), %r13
    xorq    %r11, %rdi
    movq    %rbp, -80(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r13,%r13
# 0 "" 2
#NO_APP
    movq    %r13, %rsi
    xorq    64(%rsp), %r8
    movq    %rdi, -88(%rsp)
    notq    %rsi
    xorq    -24(%rsp), %rcx
    movq    %rsi, %r9
    xorq    -96(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %r9
    xorq    8(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r9
    movq    %r9, -96(%rsp)
    movq    -48(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r10
    orq %r8, %r10
    xorq    %rsi, %r10
    xorq    -64(%rsp), %rdi
    movq    %r10, 8(%rsp)
    movq    -104(%rsp), %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %rbp
    movq    -56(%rsp), %rsi
    movq    %rax, %r11
    xorq    -8(%rsp), %rdi
    andq    %rdx, %rbp
    orq %rcx, %r11
    xorq    -112(%rsp), %r10
    xorq    %r8, %rbp
    xorq    %rdx, %r11
    movq    40(%rsp), %r8
    xorq    -32(%rsp), %rsi
    xorq    %r9, %rdi
    movq    -72(%rsp), %r9
    xorq    %r12, %r10
    xorq    -120(%rsp), %rdi
    xorq    8(%rsp), %r10
    xorq    %rbp, %r8
    xorq    %rbx, %r8
    xorq    %r15, %r8
    xorq    -80(%rsp), %r8
    xorq    (%rsp), %r10
    xorq    %r14, %rsi
    andq    %rcx, %r13
    xorq    16(%rsp), %r9
    xorq    %rax, %r13
    xorq    %r11, %rsi
    xorq    -40(%rsp), %rsi
    movq    %r8, %rax
    movq    %r10, %rdx
    xorq    -88(%rsp), %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rsi, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %r13, %r9
    xorq    %r10, %rcx
    xorq    24(%rsp), %r9
    xorq    %rcx, %r15
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%r15,%r15
# 0 "" 2
#NO_APP
    xorq    %rdi, %rax
    movq    %r15, %r10
    xorq    %r9, %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r9,%r9
# 0 "" 2
#NO_APP
    xorq    %r8, %r9
    movq    -112(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdi,%rdi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rdi
    movq    -120(%rsp), %rsi
    xorq    %r9, %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r14,%r14
# 0 "" 2
#NO_APP
    xorq    %rax, %r8
    xorq    %rdi, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%r8,%r8
# 0 "" 2
#NO_APP
    orq %r8, %r10
    xorq    %rdx, %rsi
    movq    %r10, -120(%rsp)
    movl    $2147483649, %r10d
    xorq    %r10, -120(%rsp)
    movq    %r15, %r10
    notq    %r10
    orq %r14, %r10
    xorq    %r8, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%r13,%r13
# 0 "" 2
#NO_APP
    xorq    %rsi, -120(%rsp)
    movq    %r10, -112(%rsp)
    movq    %r13, %r10
    andq    %r14, %r10
    xorq    %rax, %r12
    xorq    %r15, %r10
    movq    %rsi, %r15
    andq    %r8, %rsi
    xorq    %r13, %rsi
    movq    %r10, 32(%rsp)
    movq    -48(%rsp), %r10
    orq %r13, %r15
    movq    16(%rsp), %r8
    movq    %rsi, 48(%rsp)
    movq    -40(%rsp), %rsi
    xorq    %r14, %r15
    xorq    %rcx, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %r15, -24(%rsp)
    movq    %r12, %r15
    xorq    %rdx, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%rbp,%rbp
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %rbp, %r14
    andq    %r10, %r15
    movq    %r10, %r13
    xorq    %r9, %rsi
    xorq    %rdi, %r8
    notq    %r14
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r8, %r15
    orq %r8, %r13
    andq    %rsi, %r8
    xorq    %rsi, %r13
    xorq    %rbp, %r8
    orq %r12, %r14
    xorq    %r10, %r14
    movq    %r13, 16(%rsp)
    movq    %rbp, %r10
    movq    %r15, -48(%rsp)
    orq %rsi, %r10
    movq    -88(%rsp), %rbp
    movq    %r8, -40(%rsp)
    movq    -56(%rsp), %r8
    xorq    %r12, %r10
    movq    (%rsp), %rsi
    movq    %r10, -16(%rsp)
    movq    %rcx, %r10
    xorq    %rbx, %r10
    movq    -96(%rsp), %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r9, %r8
    xorq    %rdi, %rbp
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r8,%r8
# 0 "" 2
#NO_APP
    movq    %r8, %r12
    xorq    %rax, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rbp,%rbp
# 0 "" 2
#NO_APP
    orq %r10, %r12
    movq    %rbp, %r13
    xorq    %rdx, %rbx
    xorq    %rsi, %r12
    andq    %r8, %r13
    xorq    %r9, %r11
    movq    %r12, -88(%rsp)
    movq    %rbp, %r12
    xorq    %r10, %r13
    notq    %r12
    andq    %rsi, %r10
    movq    %r13, -56(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %r12, %rbp
    xorq    %rbx, %r10
    movq    %rbx, %r15
    andq    %rbx, %rbp
    movq    %r10, 64(%rsp)
    movq    -104(%rsp), %r10
    xorq    %r8, %rbp
    movq    -64(%rsp), %r8
    orq %rsi, %r15
    movq    24(%rsp), %rsi
    xorq    %r12, %r15
    movq    -80(%rsp), %r12
    movq    %r15, 56(%rsp)
    xorq    %rax, %r10
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%r10,%r10
# 0 "" 2
#NO_APP
    movq    %r10, %rbx
    xorq    %rdx, %r8
    xorq    %rdi, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r8,%r8
# 0 "" 2
#NO_APP
    andq    %r8, %rbx
    xorq    %rcx, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %rbx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%r12,%r12
# 0 "" 2
#NO_APP
    movq    %rbx, 24(%rsp)
    movq    %r12, %rbx
    movq    %r12, %r13
    orq %r10, %rbx
    notq    %r13
    xorq    %r8, %rbx
    orq %rsi, %r8
    movq    %r13, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %r11, %r8
    orq %r11, %r12
    movq    %r8, -64(%rsp)
    movq    -32(%rsp), %r8
    xorq    %r10, %r12
    movq    %r12, 80(%rsp)
    movq    %r11, %r12
    movq    -88(%rsp), %r10
    andq    %rsi, %r12
    xorq    %r13, %r12
    xorq    40(%rsp), %rcx
    xorq    %r9, %r8
    xorq    -8(%rsp), %rdx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r8,%r8
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %r8, %rsi
    movq    %rdx, %r13
    xorq    -72(%rsp), %rdi
    notq    %rsi
    xorq    8(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r13
    xorq    16(%rsp), %r10
    movq    %rsi, %r15
    xorq    %rsi, %r13
    andq    %rdi, %r15
    movq    %r13, -32(%rsp)
    movq    -56(%rsp), %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rax, %r9
    xorq    24(%rsp), %r10
    movq    %rax, %rsi
    andq    %rdx, %r9
    xorq    -48(%rsp), %r13
    xorq    %rdi, %r9
    movq    32(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rcx, %r15
    orq %rcx, %rsi
    xorq    %rbx, %r13
    xorq    %rdx, %rsi
    xorq    %r15, %r10
    xorq    -32(%rsp), %r13
    xorq    %r9, %rdi
    xorq    %r14, %rdi
    xorq    -120(%rsp), %r10
    xorq    %rbp, %rdi
    xorq    80(%rsp), %rdi
    xorq    -112(%rsp), %r13
    movq    %rsi, -72(%rsp)
    movq    56(%rsp), %rsi
    movq    64(%rsp), %r11
    movq    %r13, %rdx
    xorq    -16(%rsp), %rsi
    xorq    %r12, %rsi
    xorq    -72(%rsp), %rsi
    xorq    -24(%rsp), %rsi
    andq    %r8, %rcx
    xorq    -40(%rsp), %r11
    xorq    %rax, %rcx
    movq    %rdi, %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rdx,%rdx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rax,%rax
# 0 "" 2
#NO_APP
    movq    %rsi, %r8
    xorq    %r10, %rax
    xorq    -64(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %r13, %r8
    xorq    %r8, %rbp
    xorq    %rcx, %r11
    xorq    48(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $43,%rbp,%rbp
# 0 "" 2
#NO_APP
    movq    %rbp, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r10,%r10
# 0 "" 2
#NO_APP
    xorq    %r11, %rdx
    xorq    %rsi, %r10
    movq    -120(%rsp), %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%r11,%r11
# 0 "" 2
#NO_APP
    xorq    %rdi, %r11
    movq    -48(%rsp), %rdi
    xorq    %r10, %rcx
    xorq    %r11, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $21,%r12,%r12
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $14,%rcx,%rcx
# 0 "" 2
#NO_APP
    xorq    %rax, %rdi
    xorq    %rdx, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $44,%rdi,%rdi
# 0 "" 2
#NO_APP
    orq %rdi, %r13
    movq    %r13, -120(%rsp)
    movabsq $-9223372034707259384, %r13
    xorq    %r13, -120(%rsp)
    movq    %rbp, %r13
    notq    %r13
    orq %r12, %r13
    xorq    %rdi, %r13
    xorq    %rsi, -120(%rsp)
    movq    %r13, (%rsp)
    movq    %rcx, %r13
    andq    %r12, %r13
    xorq    %rbp, %r13
    movq    %r13, -104(%rsp)
    movq    %rsi, %r13
    orq %rcx, %r13
    andq    %rdi, %rsi
    movq    -88(%rsp), %rdi
    xorq    %rcx, %rsi
    movq    -24(%rsp), %rcx
    xorq    %r12, %r13
    movq    %rsi, 8(%rsp)
    movq    -40(%rsp), %rsi
    xorq    %r8, %r9
    movq    %r13, -96(%rsp)
    xorq    %rax, %rbx
    xorq    %rdx, %r15
    xorq    %rdx, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $3,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %rbp
    xorq    %r10, %rsi
    xorq    %r11, %rcx
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $20,%rsi,%rsi
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $61,%r9,%r9
# 0 "" 2
#NO_APP
    orq %rsi, %rbp
    movq    %r9, %r12
    movq    %r9, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $28,%rcx,%rcx
# 0 "" 2
#NO_APP
    notq    %r12
    xorq    %rcx, %rbp
    orq %rcx, %r13
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $45,%rbx,%rbx
# 0 "" 2
#NO_APP
    movq    %rbp, 40(%rsp)
    xorq    %rbx, %r13
    orq %rbx, %r12
    movq    %rbx, %rbp
    movq    %rsi, %rbx
    xorq    %rdi, %r12
    andq    %rdi, %rbp
    andq    %rcx, %rbx
    movq    -64(%rsp), %rdi
    movq    -112(%rsp), %rcx
    xorq    %r9, %rbx
    xorq    %rsi, %rbp
    movq    56(%rsp), %r9
    movq    %r13, -8(%rsp)
    movq    %r8, %rsi
    xorq    %r14, %rsi
    movq    %r12, -48(%rsp)
    movq    -72(%rsp), %r14
    xorq    %r10, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $8,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r13
    xorq    %rax, %rcx
    xorq    %r11, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $1,%rcx,%rcx
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $25,%r9,%r9
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $6,%rsi,%rsi
# 0 "" 2
#NO_APP
    movq    %r9, %r12
    notq    %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $18,%r15,%r15
# 0 "" 2
#NO_APP
    andq    %r9, %r13
    orq %rsi, %r12
    xorq    %r11, %r14
    xorq    %rsi, %r13
    xorq    %rcx, %r12
    andq    %rcx, %rsi
    movq    %r13, -24(%rsp)
    movq    %r15, %r13
    xorq    %r15, %rsi
    orq %rcx, %r13
    movq    %r12, -40(%rsp)
    movq    -56(%rsp), %rcx
    movq    %rdi, %r12
    movq    %rsi, -80(%rsp)
    movq    48(%rsp), %rsi
    andq    %r15, %r12
    movq    16(%rsp), %r15
    xorq    %rdi, %r13
    movq    80(%rsp), %rdi
    xorq    %r9, %r12
    movq    %r13, -64(%rsp)
    xorq    %rax, %rcx
    movq    %r12, -112(%rsp)
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $10,%rcx,%rcx
# 0 "" 2
#NO_APP
    movq    %rcx, %r9
    xorq    %rdx, %r15
    xorq    %r10, %rsi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $36,%r15,%r15
# 0 "" 2
#NO_APP
    andq    %r15, %r9
    xorq    %r8, %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $27,%rsi,%rsi
# 0 "" 2
#NO_APP
    xorq    %rsi, %r9
    movq    %r9, -88(%rsp)
    xorq    32(%rsp), %r8
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $15,%rdi,%rdi
# 0 "" 2
#NO_APP
    movq    %rdi, %r9
    notq    %rdi
    movq    %rdi, %r12
    orq %rcx, %r9
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $56,%r14,%r14
# 0 "" 2
#NO_APP
    orq %r14, %r12
    xorq    %r15, %r9
    orq %rsi, %r15
    xorq    %rcx, %r12
    xorq    %r14, %r15
    movq    %r12, -56(%rsp)
    movq    %r14, %r12
    andq    %rsi, %r12
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $62,%r8,%r8
# 0 "" 2
#NO_APP
    xorq    %rdi, %r12
    xorq    -16(%rsp), %r11
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $55,%r11,%r11
# 0 "" 2
#NO_APP
    movq    %r11, %rcx
    xorq    24(%rsp), %rdx
    andq    %r8, %r11
    notq    %rcx
    xorq    64(%rsp), %r10
    movq    %rcx, %r14
    xorq    -32(%rsp), %rax
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $39,%r10,%r10
# 0 "" 2
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $41,%rdx,%rdx
# 0 "" 2
#NO_APP
    movq    %rdx, %r13
    andq    %r10, %r14
    movq    120(%rsp), %rdi
#APP
# 468 "SnP/KeccakP-1600/Optimized64/KeccakP-1600-opt64.c" 1
    shld $2,%rax,%rax
# 0 "" 2
#NO_APP
    orq %r10, %r13
    movq    %rax, %rsi
    xorq    %r8, %r14
    xorq    %rcx, %r13
    orq %r8, %rsi
    movq    %rax, %rcx
    xorq    %rax, %r11
    movq    104(%rsp), %r8
    andq    %rdx, %rcx
    movq    104(%rsp), %rax
    xorq    %r10, %rcx
    xorq    %rdx, %rsi
    addq    %rdi, 72(%rsp)
    movq    %r13, 24(%rsp)
    movq    %rcx, 32(%rsp)
    subq    112(%rsp), %rax
    movq    %rsi, -32(%rsp)
    cmpq    %r8, 112(%rsp)
    ja  .L316
    cmpl    $21, 100(%rsp)
    movq    %rax, 104(%rsp)
    jne .L256
.L313:
    movq    72(%rsp), %rsi
    movq    72(%rsp), %rdi
    movq    72(%rsp), %rdx
    movq    72(%rsp), %r8
    movq    (%rsi), %rsi
    movq    8(%rdi), %rdi
    movq    72(%rsp), %r10
    movq    72(%rsp), %r13
    movq    72(%rsp), %rax
    movq    56(%rdx), %rcx
    movq    16(%r8), %r8
    movq    24(%r10), %r10
    movq    32(%r13), %r13
    movq    40(%rax), %rax
    xorq    %rsi, -120(%rsp)
    xorq    %rdi, (%rsp)
    movq    64(%rdx), %rsi
    movq    80(%rdx), %rdi
    xorq    %r8, -104(%rsp)
    xorq    %r10, -96(%rsp)
    xorq    %r13, 8(%rsp)
    xorq    %rax, 40(%rsp)
    xorq    48(%rdx), %rbp
    xorq    %rcx, -48(%rsp)
    xorq    %rsi, -8(%rsp)
    xorq    72(%rdx), %rbx
    xorq    %rdi, -40(%rsp)
    movq    72(%rsp), %rcx
    movq    88(%rdx), %r8
    movq    96(%rdx), %r10
    movq    104(%rdx), %r13
    movq    112(%rdx), %rax
    movq    136(%rcx), %rsi
    movq    120(%rdx), %rdx
    xorq    %r8, -24(%rsp)
    xorq    %r10, -112(%rsp)
    xorq    %r13, -64(%rsp)
    xorq    %rax, -80(%rsp)
    xorq    %rdx, -88(%rsp)
    xorq    128(%rcx), %r9
    xorq    %rsi, -56(%rsp)
    xorq    144(%rcx), %r12
    xorq    152(%rcx), %r15
    xorq    160(%rcx), %r14
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L258:
    movq    72(%rsp), %rdi
    movq    72(%rsp), %r8
    movq    72(%rsp), %rcx
    movq    72(%rsp), %r10
    movq    (%rdi), %rdi
    movq    8(%r8), %r8
    movq    72(%rsp), %r13
    movq    72(%rsp), %rax
    movq    72(%rsp), %rdx
    xorq    %rdi, -120(%rsp)
    xorq    %r8, (%rsp)
    movq    16(%r10), %r10
    movq    24(%r13), %r13
    movq    32(%rax), %rax
    movq    40(%rdx), %rdx
    movq    56(%rcx), %rsi
    movq    64(%rcx), %rdi
    movq    80(%rcx), %r8
    xorq    %r10, -104(%rsp)
    xorq    %r13, -96(%rsp)
    xorq    %rax, 8(%rsp)
    xorq    %rdx, 40(%rsp)
    xorq    48(%rcx), %rbp
    xorq    72(%rcx), %rbx
    xorq    %rsi, -48(%rsp)
    xorq    %rdi, -8(%rsp)
    xorq    %r8, -40(%rsp)
    movq    88(%rcx), %r10
    movq    96(%rcx), %r13
    movq    104(%rcx), %rax
    movq    112(%rcx), %rdx
    movq    120(%rcx), %rcx
    xorq    %r10, -24(%rsp)
    xorq    %r13, -112(%rsp)
    xorq    %rax, -64(%rsp)
    xorq    %rdx, -80(%rsp)
    xorq    %rcx, -88(%rsp)
    cmpl    $23, 100(%rsp)
    ja  .L266
    cmpl    $19, 100(%rsp)
    ja  .L267
    cmpl    $17, 100(%rsp)
    ja  .L268
    cmpl    $16, 100(%rsp)
    je  .L257
    movq    72(%rsp), %rsi
    xorq    128(%rsi), %r9
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L259:
    movq    72(%rsp), %r10
    movq    72(%rsp), %rax
    movq    72(%rsp), %rdx
    movq    72(%rsp), %rcx
    movq    72(%rsp), %rsi
    movq    72(%rsp), %rdi
    movq    72(%rsp), %r8
    movq    (%rax), %rax
    movq    8(%rdx), %rdx
    movq    16(%rcx), %rcx
    movq    24(%rsi), %rsi
    movq    32(%rdi), %rdi
    movq    40(%r8), %r8
    movq    56(%r10), %r13
    xorq    %rax, -120(%rsp)
    xorq    %rdx, (%rsp)
    xorq    %rcx, -104(%rsp)
    xorq    %rsi, -96(%rsp)
    xorq    %rdi, 8(%rsp)
    xorq    %r8, 40(%rsp)
    xorq    48(%r10), %rbp
    xorq    %r13, -48(%rsp)
    cmpl    $11, 100(%rsp)
    ja  .L263
    cmpl    $9, 100(%rsp)
    ja  .L264
    cmpl    $8, 100(%rsp)
    je  .L257
    movq    64(%r10), %rax
    xorq    %rax, -8(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L266:
    movq    72(%rsp), %rsi
    movq    136(%rsi), %rdi
    movq    168(%rsi), %r8
    movq    176(%rsi), %r10
    movq    184(%rsi), %r13
    xorq    128(%rsi), %r9
    xorq    %rdi, -56(%rsp)
    xorq    144(%rsi), %r12
    xorq    152(%rsi), %r15
    xorq    160(%rsi), %r14
    xorq    %r8, 24(%rsp)
    xorq    %r10, 32(%rsp)
    xorq    %r13, -32(%rsp)
    cmpl    $24, 100(%rsp)
    je  .L257
    xorq    192(%rsi), %r11
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L316:
    movq    128(%rsp), %rax
    movq    %r12, %r10
    movq    %r14, %rcx
    movq    -120(%rsp), %r12
    movq    %r9, %rsi
    movq    %rbp, %rdi
    movq    -96(%rsp), %r14
    movq    %rbx, %r13
    movq    %r11, %r9
    subq    %r8, %rax
.L255:
    movq    88(%rsp), %r11
    movq    -104(%rsp), %rdx
    movq    8(%rsp), %rbx
    movq    40(%rsp), %rbp
    movq    %r12, (%r11)
    movq    (%rsp), %r12
    movq    -48(%rsp), %r8
    movq    %rdx, 16(%r11)
    movq    %rbx, 32(%r11)
    movq    -24(%rsp), %rdx
    movq    -112(%rsp), %rbx
    movq    %rbp, 40(%r11)
    movq    %r12, 8(%r11)
    movq    -8(%rsp), %r12
    movq    -80(%rsp), %rbp
    movq    %rdi, 48(%r11)
    movq    %r8, 56(%r11)
    movq    -64(%rsp), %rdi
    movq    -88(%rsp), %r8
    movq    %r13, 72(%r11)
    movq    %r12, 64(%r11)
    movq    -40(%rsp), %r13
    movq    -56(%rsp), %r12
    movq    %r14, 24(%r11)
    movq    %rdx, 88(%r11)
    movq    %rbx, 96(%r11)
    movq    %r13, 80(%r11)
    movq    %rbp, 112(%r11)
    movq    %r12, 136(%r11)
    movq    %rdi, 104(%r11)
    movq    %r8, 120(%r11)
    movq    %rsi, 128(%r11)
    movq    %r10, 144(%r11)
    movq    %r15, 152(%r11)
    movq    24(%rsp), %r13
    movq    32(%rsp), %r15
    movq    %rcx, 160(%r11)
    movq    -32(%rsp), %rdx
    movq    %r9, 192(%r11)
    movq    %r13, 168(%r11)
    movq    %r15, 176(%r11)
    movq    %rdx, 184(%r11)
    addq    $136, %rsp
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
.L260:
    .cfi_restore_state
    movq    72(%rsp), %rax
    movq    72(%rsp), %rdx
    movq    72(%rsp), %rcx
    movq    72(%rsp), %rsi
    movq    (%rax), %rax
    movq    8(%rdx), %rdx
    movq    16(%rcx), %rcx
    movq    24(%rsi), %rsi
    xorq    %rax, -120(%rsp)
    xorq    %rdx, (%rsp)
    xorq    %rcx, -104(%rsp)
    xorq    %rsi, -96(%rsp)
    cmpl    $5, 100(%rsp)
    ja  .L262
    cmpl    $4, 100(%rsp)
    je  .L257
    movq    72(%rsp), %rdi
    movq    32(%rdi), %rdi
    xorq    %rdi, 8(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L267:
    movq    72(%rsp), %r10
    movq    136(%r10), %r13
    xorq    128(%r10), %r9
    xorq    144(%r10), %r12
    xorq    152(%r10), %r15
    xorq    %r13, -56(%rsp)
    cmpl    $21, 100(%rsp)
    ja  .L269
    cmpl    $20, 100(%rsp)
    je  .L257
    xorq    160(%r10), %r14
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L263:
    movq    %r10, %r8
    movq    64(%r10), %rdi
    xorq    72(%r10), %rbx
    movq    88(%r8), %r13
    movq    80(%r10), %r10
    xorq    %rdi, -8(%rsp)
    xorq    %r13, -24(%rsp)
    xorq    %r10, -40(%rsp)
    cmpl    $13, 100(%rsp)
    ja  .L265
    cmpl    $12, 100(%rsp)
    je  .L257
    movq    96(%r8), %rax
    xorq    %rax, -112(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L265:
    movq    96(%r8), %rdx
    movq    104(%r8), %rcx
    xorq    %rdx, -112(%rsp)
    xorq    %rcx, -64(%rsp)
    cmpl    $14, 100(%rsp)
    je  .L257
    movq    72(%rsp), %rsi
    movq    112(%rsi), %rsi
    xorq    %rsi, -80(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L269:
    movq    168(%r10), %rdx
    xorq    160(%r10), %r14
    xorq    %rdx, 24(%rsp)
    cmpl    $22, 100(%rsp)
    je  .L257
    movq    176(%r10), %rcx
    xorq    %rcx, 32(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L262:
    movq    72(%rsp), %r8
    movq    72(%rsp), %r10
    movq    32(%r8), %r8
    movq    40(%r10), %r10
    xorq    %r8, 8(%rsp)
    xorq    %r10, 40(%rsp)
    cmpl    $6, 100(%rsp)
    je  .L257
    movq    72(%rsp), %r13
    xorq    48(%r13), %rbp
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L264:
    movq    72(%rsp), %rdx
    movq    72(%rsp), %rcx
    movq    64(%rdx), %rdx
    xorq    72(%rcx), %rbx
    xorq    %rdx, -8(%rsp)
    cmpl    $10, 100(%rsp)
    je  .L257
    movq    80(%rcx), %rsi
    xorq    %rsi, -40(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L268:
    movq    72(%rsp), %rdi
    movq    136(%rdi), %r8
    xorq    128(%rdi), %r9
    xorq    %r8, -56(%rsp)
    cmpl    $18, 100(%rsp)
    je  .L257
    xorq    144(%rdi), %r12
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L315:
    movq    72(%rsp), %r13
    movq    16(%r13), %r13
    xorq    %r13, -104(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L314:
    movl    100(%rsp), %eax
    testl   %eax, %eax
    je  .L257
    movq    72(%rsp), %rdi
    movq    (%rdi), %rdi
    xorq    %rdi, -120(%rsp)
    jmp .L257
.L271:
    xorl    %eax, %eax
    jmp .L255
    .cfi_endproc
.LFE40:
    .size   KeccakF1600_FastLoop_Absorb, .-KeccakF1600_FastLoop_Absorb
    .ident  "GCC: (SUSE Linux) 4.7.4"
    .section    .note.GNU-stack,"",@progbits
