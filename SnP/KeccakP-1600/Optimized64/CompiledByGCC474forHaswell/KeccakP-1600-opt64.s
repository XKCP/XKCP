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
    movq    %rbx, -16(%rbp)
    subq    $16, %rsp
    andq    $-32, %rsp
    .cfi_offset 12, -24
    .cfi_offset 3, -32
    movl    %esi, %r12d
    subq    $64, %rsp
    testl   %r8d, %r8d
    je  .L31
    cmpl    $1, %r8d
    movq    %rdi, %rbx
    movq    %rdx, %rsi
    je  .L39
    leaq    32(%rsp), %rdi
    movl    %r8d, %edx
    movq    $0, 32(%rsp)
    movl    %ecx, 24(%rsp)
    call    memcpy
    movq    32(%rsp), %rax
    movl    24(%rsp), %ecx
.L34:
    sall    $3, %ecx
    shlx    %rcx, %rax, %rcx
    xorq    %rcx, (%rbx,%r12,8)
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
    shrl    $3, %eax
    sall    $3, %edx
    shlx    %rdx, %rsi, %rsi
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
    movq    %rdi, %r8
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
    movl    $1, %ecx
.L69:
    movl    %r15d, %eax
    sall    $3, %r13d
    addl    $1, %r15d
    shlx    %r13, %rdx, %r13
    addq    %rcx, %r14
    xorq    %r13, (%r8,%rax,8)
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
    movl    %r12d, %ecx
    leaq    32(%rsp), %rdi
    movq    %r14, %rsi
    movq    %rcx, %rdx
    movq    $0, 32(%rsp)
    movq    %rcx, 24(%rsp)
    movq    %r8, 16(%rsp)
    call    memcpy
    movq    32(%rsp), %rdx
    movq    16(%rsp), %r8
    movq    24(%rsp), %rcx
    jmp .L69
    .p2align 4,,10
    .p2align 3
.L76:
    movl    %ecx, %r12d
    movl    %ecx, 24(%rsp)
    movq    %r8, 16(%rsp)
    shrl    $3, %r12d
    movl    %r12d, %edx
    call    KeccakP1600_AddLanes
    movl    24(%rsp), %ecx
    movq    16(%rsp), %r8
    movl    %ecx, %ebx
    andl    $7, %ebx
    je  .L61
    leal    0(,%r12,8), %esi
    addq    %r14, %rsi
    cmpl    $1, %ebx
    jne .L65
    movzbl  (%rsi), %eax
.L66:
    xorq    %rax, (%r8,%r12,8)
    jmp .L61
.L65:
    leaq    32(%rsp), %rdi
    movl    %ebx, %edx
    movq    $0, 32(%rsp)
    movq    %r8, 16(%rsp)
    call    memcpy
    movq    32(%rsp), %rax
    movq    16(%rsp), %r8
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
    movq    %r9, %rsi
    addq    %rcx, %rax
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
    movl    %ecx, %r13d
    leal    0(,%r14,8), %r15d
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
    movl    %ecx, %ebx
    leal    -1(%r14), %edx
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
    leal    -1(%r8), %edi
    movl    %ecx, %r13d
    andl    $7, %r13d
    movl    %eax, %ecx
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
    subq    $240, %rsp
    .cfi_def_cfa_offset 296
    movq    (%rdi), %r13
    movq    8(%rdi), %rax
    movq    %rdi, 216(%rsp)
    movq    16(%rdi), %rdx
    movq    24(%rdi), %rcx
    movq    32(%rdi), %rbx
    movq    40(%rdi), %rsi
    movq    48(%rdi), %r11
    movq    56(%rdi), %rdi
    movq    %rax, 16(%rsp)
    movq    %rdx, 8(%rsp)
    movq    %rbx, (%rsp)
    movq    %rsi, -8(%rsp)
    movq    %rdi, -16(%rsp)
    movq    216(%rsp), %rdi
    movq    64(%rdi), %rdi
    movq    %rdi, -24(%rsp)
    movq    216(%rsp), %rdi
    movq    72(%rdi), %rdi
    movq    %rdi, -32(%rsp)
    movq    216(%rsp), %rdi
    movq    80(%rdi), %rdi
    movq    %rdi, -40(%rsp)
    movq    216(%rsp), %rdi
    movq    88(%rdi), %rdi
    movq    %rdi, -48(%rsp)
    movq    216(%rsp), %rdi
    movq    104(%rdi), %r8
    movq    96(%rdi), %rbp
    movq    %r8, -56(%rsp)
    movq    136(%rdi), %r14
    movq    128(%rdi), %r12
    movq    -8(%rsp), %r8
    movq    120(%rdi), %r10
    movq    168(%rdi), %rdx
    movq    %r14, -88(%rsp)
    movq    16(%rsp), %r14
    movq    %r12, -80(%rsp)
    xorq    %r13, %r8
    xorq    -40(%rsp), %r8
    movq    160(%rdi), %rax
    movq    112(%rdi), %r9
    xorq    %r11, %r14
    xorq    -48(%rsp), %r14
    movq    %rdx, -112(%rsp)
    xorq    -80(%rsp), %r14
    movq    152(%rdi), %r15
    xorq    %r10, %r8
    movq    %rax, -104(%rsp)
    movq    %r9, -64(%rsp)
    xorq    %rax, %r8
    movq    -24(%rsp), %rax
    movq    -32(%rsp), %r9
    xorq    (%rsp), %r9
    movq    144(%rdi), %rbx
    xorq    %rdx, %r14
    movq    -16(%rsp), %rdx
    xorq    8(%rsp), %rdx
    xorq    -64(%rsp), %r9
    xorq    %rcx, %rax
    xorq    -56(%rsp), %rax
    movq    %r15, -96(%rsp)
    movq    176(%rdi), %r15
    xorq    -96(%rsp), %r9
    movq    184(%rdi), %rsi
    xorq    %rbp, %rdx
    xorq    -88(%rsp), %rdx
    movq    192(%rdi), %r12
    xorq    %rbx, %rax
    rorx    $63, %r14, %rdi
    movq    %r10, -72(%rsp)
    xorq    %rsi, %rax
    movq    %rsi, -120(%rsp)
    xorq    %r12, %r9
    rorx    $63, %rax, %r10
    xorq    %r15, %rdx
    xorq    %r9, %rdi
    rorx    $63, %r9, %r9
    rorx    $63, %rdx, %rsi
    xorq    %rdx, %r9
    movq    %rdi, %rdx
    xorq    %r8, %rsi
    rorx    $63, %r8, %r8
    xorq    %r14, %r10
    xorq    %rax, %r8
    xorq    %r13, %rdx
    xorq    %r10, %rbp
    xorq    %r8, %r12
    rorx    $21, %rbp, %rbp
    xorq    %rsi, %r11
    rorx    $50, %r12, %rax
    movq    %rdx, %r12
    xorq    %r9, %rbx
    xorq    $1, %r12
    rorx    $43, %rbx, %rbx
    rorx    $20, %r11, %r11
    movq    %r12, 72(%rsp)
    movq    %rbp, %r13
    movq    %rbp, %r12
    movq    %rax, %r14
    orq %r11, %r12
    notq    %r13
    xorq    %r12, 72(%rsp)
    andq    %rbx, %r14
    orq %rbx, %r13
    xorq    %rbp, %r14
    movq    %rax, %rbp
    xorq    %r11, %r13
    orq %rdx, %rbp
    andq    %rdx, %r11
    movq    %r10, %rdx
    xorq    %rbx, %rbp
    xorq    %rax, %r11
    movq    -40(%rsp), %rbx
    movq    %r11, 40(%rsp)
    xorq    %r15, %rdx
    xorq    %r9, %rcx
    movq    -32(%rsp), %r11
    movq    -80(%rsp), %rax
    rorx    $3, %rdx, %rdx
    xorq    %rdi, %rbx
    rorx    $36, %rcx, %rcx
    movq    %r13, 24(%rsp)
    rorx    $61, %rbx, %rbx
    movq    %rdx, %r15
    movq    %r14, 48(%rsp)
    xorq    %r8, %r11
    xorq    %rsi, %rax
    movq    %rbx, %r12
    rorx    $19, %rax, %rax
    rorx    $44, %r11, %r11
    orq %rcx, %r15
    orq %r11, %r12
    movq    %rax, %r13
    movq    %rdx, %r14
    xorq    %rcx, %r12
    andq    %rbx, %r13
    xorq    %rax, %r15
    movq    %r12, 96(%rsp)
    xorq    %r11, %r13
    movq    -56(%rsp), %r12
    andq    %rcx, %r11
    notq    %r14
    movq    %r13, 80(%rsp)
    xorq    %rdx, %r11
    orq %rax, %r14
    movq    -16(%rsp), %rdx
    movq    -96(%rsp), %r13
    movq    %r15, %rax
    movq    %rbp, 32(%rsp)
    xorq    %rbp, %rax
    movq    16(%rsp), %rbp
    movq    %r11, 120(%rsp)
    xorq    %r9, %r12
    movq    %r11, %rcx
    movq    -104(%rsp), %r11
    rorx    $39, %r12, %r12
    xorq    %r10, %rdx
    xorq    %r8, %r13
    xorq    %rbx, %r14
    xorq    %rsi, %rbp
    rorx    $58, %rdx, %rdx
    rorx    $56, %r13, %r13
    movq    %r12, %rbx
    rorx    $63, %rbp, %rbp
    movq    %r14, 64(%rsp)
    xorq    %rdi, %r11
    orq %rdx, %rbx
    movq    %r13, %r14
    xorq    40(%rsp), %rcx
    rorx    $46, %r11, %r11
    xorq    %rbp, %rbx
    notq    %r13
    andq    %r12, %r14
    movq    %rbx, 88(%rsp)
    xorq    %rdx, %r14
    movq    %r15, 128(%rsp)
    andq    %rbp, %rdx
    movq    %r13, %rbx
    movq    %r11, %r15
    xorq    %r11, %rdx
    andq    %r11, %rbx
    orq %rbp, %r15
    movq    -48(%rsp), %r11
    xorq    %r13, %r15
    movq    -8(%rsp), %r13
    xorq    %r12, %rbx
    movq    (%rsp), %r12
    movq    %r14, 56(%rsp)
    movq    -88(%rsp), %r14
    movq    %rdx, 104(%rsp)
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    xorq    %rdi, %r13
    movq    -120(%rsp), %rbp
    rorx    $54, %r11, %r11
    xorq    %r8, %r12
    rorx    $28, %r13, %r13
    movq    %r11, %rdx
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    andq    %r13, %rdx
    xorq    %r9, %rbp
    rorx    $49, %r14, %r14
    xorq    %r12, %rdx
    rorx    $8, %rbp, %rbp
    movq    %r15, 112(%rsp)
    movq    %rdx, 184(%rsp)
    movq    96(%rsp), %rdx
    xorq    %r15, %rax
    xorq    72(%rsp), %rdx
    movq    %r14, %r15
    notq    %r14
    xorq    88(%rsp), %rdx
    orq %r11, %r15
    xorq    184(%rsp), %rdx
    movq    %r14, 136(%rsp)
    orq %rbp, %r14
    xorq    %r11, %r14
    movq    %rbp, %r11
    xorq    %r13, %r15
    andq    %r12, %r11
    xorq    136(%rsp), %r11
    orq %r12, %r13
    xorq    %rbp, %r13
    xorq    8(%rsp), %r10
    xorq    -24(%rsp), %r9
    xorq    -64(%rsp), %r8
    xorq    %r13, %rcx
    movq    %r14, 192(%rsp)
    movq    %r13, 152(%rsp)
    xorq    %r11, %rax
    xorq    -72(%rsp), %rdi
    xorq    -112(%rsp), %rsi
    rorx    $9, %r9, %r9
    rorx    $2, %r10, %r10
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    notq    %rbp
    rorx    $23, %rdi, %rdi
    movq    %rbp, %r14
    rorx    $62, %rsi, %rsi
    movq    %rdi, %r12
    andq    %r8, %r14
    movq    %rsi, %r13
    orq %r8, %r12
    xorq    %r10, %r14
    orq %r10, %r13
    xorq    %rbp, %r12
    movq    56(%rsp), %rbp
    xorq    80(%rsp), %rbp
    movq    %r12, 136(%rsp)
    andq    %r9, %r10
    xorq    %r14, %rdx
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r13, %rax
    xorq    %r10, %rcx
    xorq    %r15, %rbp
    rorx    $63, %rax, %r9
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    24(%rsp), %rbp
    andq    %rdi, %r12
    xorq    %r8, %r12
    movq    48(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %rbp, %r9
    xorq    %r12, %r8
    xorq    64(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbx, %r8
    xorq    192(%rsp), %r8
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    80(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rcx, %r11
    xorq    %rax, %rdx
    movq    72(%rsp), %rax
    rorx    $43, %r11, %r11
    xorq    %rsi, %r8
    xorq    %rdx, %r10
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    xorq    %rdi, %rax
    movq    %rax, %rbp
    xorq    $32898, %rbp
    xorq    %rsi, %r15
    xorq    %r9, %r12
    movq    %rbp, 80(%rsp)
    movq    %rbx, %rbp
    rorx    $19, %r15, %r15
    orq %r8, %rbp
    xorq    %rbp, 80(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $3, %r12, %r12
    xorq    %rdi, %r14
    orq %r11, %rbp
    rorx    $46, %r14, %r14
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 72(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 144(%rsp)
    movq    120(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    88(%rsp), %r10
    orq %rax, %rbx
    movq    32(%rsp), %rax
    movq    %rbp, 200(%rsp)
    xorq    %rdx, %r8
    xorq    %r11, %rbx
    movq    %r15, %r11
    xorq    %rdi, %r10
    rorx    $44, %r8, %r8
    movq    %rbx, 160(%rsp)
    rorx    $61, %r10, %r10
    xorq    %rcx, %rax
    movq    %r12, %rbx
    movq    %r10, %rbp
    rorx    $36, %rax, %rax
    andq    %r10, %r11
    orq %r8, %rbp
    xorq    %r8, %r11
    notq    %rbx
    xorq    %rax, %rbp
    andq    %rax, %r8
    orq %r15, %rbx
    movq    %rbp, 120(%rsp)
    movq    %r12, %rbp
    xorq    %r12, %r8
    orq %rax, %rbp
    movq    152(%rsp), %r12
    movq    112(%rsp), %rax
    xorq    %r10, %rbx
    movq    64(%rsp), %r10
    movq    %r8, 176(%rsp)
    movq    24(%rsp), %r8
    xorq    %r15, %rbp
    movq    %r11, 88(%rsp)
    xorq    %rcx, %rax
    xorq    %rdx, %r12
    movq    %rbx, 32(%rsp)
    rorx    $39, %rax, %rax
    xorq    %r9, %r10
    rorx    $56, %r12, %r12
    xorq    %rsi, %r8
    rorx    $58, %r10, %r10
    movq    %rax, %r11
    movq    %r12, %r15
    notq    %r12
    rorx    $63, %r8, %r8
    orq %r10, %r11
    movq    %r12, %rbx
    movq    %rbp, 168(%rsp)
    xorq    %r8, %r11
    andq    %rax, %r15
    andq    %r14, %rbx
    xorq    %rax, %rbx
    movq    %r14, %rax
    xorq    %r10, %r15
    orq %r8, %rax
    andq    %r8, %r10
    movq    %r11, 112(%rsp)
    xorq    %r12, %rax
    xorq    %r14, %r10
    movq    40(%rsp), %r8
    movq    %rax, 208(%rsp)
    movq    %rbp, %rax
    movq    56(%rsp), %rbp
    movq    %r10, 224(%rsp)
    movq    96(%rsp), %r10
    xorq    %rcx, %r13
    movq    192(%rsp), %r12
    xorq    %rdx, %r8
    xorq    128(%rsp), %rcx
    xorq    %rsi, %rbp
    rorx    $37, %r8, %r8
    rorx    $8, %r13, %r13
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    104(%rsp), %rdx
    rorx    $28, %r10, %r10
    movq    %rbp, %r11
    xorq    %r9, %r12
    andq    %r10, %r11
    rorx    $49, %r12, %r12
    xorq    184(%rsp), %rdi
    xorq    %r8, %r11
    xorq    160(%rsp), %rax
    xorq    48(%rsp), %r9
    movq    %r11, 40(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    rorx    $9, %rcx, %rcx
    orq %r13, %r14
    xorq    208(%rsp), %rax
    xorq    %r10, %r11
    xorq    %rbp, %r14
    movq    %r13, %rbp
    rorx    $25, %rdx, %rdx
    andq    %r8, %rbp
    movq    %r14, 96(%rsp)
    rorx    $23, %rdi, %rdi
    xorq    %r12, %rbp
    movq    %r10, %r12
    movq    %rcx, %r10
    notq    %r10
    movq    %r15, 152(%rsp)
    orq %r8, %r12
    movq    %r10, %r14
    rorx    $2, %r9, %r9
    movq    %rdi, %r15
    andq    %rdx, %r14
    xorq    %r13, %r12
    xorq    136(%rsp), %rsi
    xorq    %r9, %r14
    xorq    %rbp, %rax
    movq    120(%rsp), %r8
    xorq    80(%rsp), %r8
    orq %rdx, %r15
    xorq    %r10, %r15
    movq    152(%rsp), %r10
    xorq    88(%rsp), %r10
    rorx    $62, %rsi, %rsi
    movq    %r15, 128(%rsp)
    xorq    112(%rsp), %r8
    movq    %rsi, %r13
    xorq    40(%rsp), %r8
    orq %r9, %r13
    andq    %rcx, %r9
    movq    176(%rsp), %rcx
    xorq    %r11, %r10
    xorq    144(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    224(%rsp), %rcx
    andq    %rdi, %r15
    xorq    72(%rsp), %r10
    xorq    %r13, %rax
    xorq    %rdx, %r15
    movq    200(%rsp), %rdx
    xorq    %rsi, %r9
    movq    %r13, 56(%rsp)
    rorx    $63, %rax, %r13
    xorq    %r14, %r8
    xorq    %r12, %rcx
    xorq    %r15, %rdx
    xorq    32(%rsp), %rdx
    xorq    %r9, %rcx
    xorq    %r10, %r13
    rorx    $63, %r10, %rdi
    rorx    $63, %rcx, %r10
    xorq    %rcx, %rdi
    movabsq $-9223372036854742902, %rcx
    xorq    %rbx, %rdx
    xorq    96(%rsp), %rdx
    xorq    %r13, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rsi
    rorx    $63, %r8, %rdx
    xorq    %r8, %rsi
    xorq    %rax, %rdx
    movq    80(%rsp), %r8
    movq    88(%rsp), %rax
    xorq    %rdi, %r8
    xorq    %rsi, %rax
    xorq    %r10, %rbp
    xorq    %r8, %rcx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    rorx    $43, %rbp, %rbp
    movq    %rax, %rcx
    xorq    %rdx, %r9
    xorq    %rsi, %r11
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    rorx    $50, %r9, %r9
    rorx    $19, %r11, %r11
    orq %rbp, %rcx
    xorq    %r13, %r15
    xorq    %rdi, %r14
    xorq    %rax, %rcx
    andq    %r8, %rax
    rorx    $3, %r15, %r15
    movq    %rcx, 64(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 104(%rsp)
    movq    176(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    112(%rsp), %r8
    movq    %rcx, 80(%rsp)
    movq    160(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 88(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $46, %r14, %r14
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    orq %r11, %rbx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rax, %r9
    andq    %rcx, %rax
    xorq    %rcx, %rbp
    xorq    %r15, %rax
    xorq    %r8, %rbx
    movq    %rbp, 112(%rsp)
    movq    %r15, %rbp
    movq    %rax, 136(%rsp)
    orq %rcx, %rbp
    movq    208(%rsp), %rcx
    movq    32(%rsp), %rax
    movq    72(%rsp), %r8
    movq    %r9, 184(%rsp)
    movq    %rdx, %r9
    xorq    %r12, %r9
    xorq    %r11, %rbp
    movq    %r14, %r15
    xorq    %r10, %rcx
    xorq    %r13, %rax
    rorx    $56, %r9, %r9
    rorx    $39, %rcx, %rcx
    rorx    $58, %rax, %rax
    xorq    %rsi, %r8
    movq    %rcx, %r11
    rorx    $63, %r8, %r8
    movq    %r9, %r12
    orq %rax, %r11
    andq    %rcx, %r12
    orq %r8, %r15
    xorq    %r8, %r11
    xorq    %rax, %r12
    andq    %rax, %r8
    movq    %r11, 32(%rsp)
    movq    152(%rsp), %r11
    xorq    %r14, %r8
    movq    %r12, 160(%rsp)
    movq    120(%rsp), %r12
    notq    %r9
    movq    144(%rsp), %rax
    movq    %rbx, 48(%rsp)
    movq    %r9, %rbx
    movq    %r8, 208(%rsp)
    xorq    %rsi, %r11
    movq    24(%rsp), %r8
    xorq    112(%rsp), %r8
    rorx    $54, %r11, %r11
    andq    %r14, %rbx
    xorq    32(%rsp), %r8
    movq    96(%rsp), %r14
    xorq    %rdi, %r12
    xorq    %rcx, %rbx
    rorx    $28, %r12, %r12
    xorq    %rdx, %rax
    movq    %r11, %rcx
    xorq    %r9, %r15
    rorx    $37, %rax, %rax
    movq    56(%rsp), %r9
    andq    %r12, %rcx
    xorq    %r13, %r14
    xorq    %rax, %rcx
    rorx    $49, %r14, %r14
    movq    %rbp, 192(%rsp)
    movq    %rcx, 96(%rsp)
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    %r10, %r9
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r15, 176(%rsp)
    movq    64(%rsp), %rbp
    movq    %r14, %r15
    xorq    184(%rsp), %rbp
    xorq    %r12, %rcx
    xorq    160(%rsp), %rbp
    orq %r9, %r15
    xorq    %r11, %r15
    movq    %r9, %r11
    movq    %r15, 120(%rsp)
    movq    %r12, %r15
    andq    %rax, %r11
    orq %rax, %r15
    xorq    200(%rsp), %r13
    xorq    168(%rsp), %r10
    xorq    %r14, %r11
    xorq    %r9, %r15
    xorq    %rcx, %rbp
    xorq    224(%rsp), %rdx
    xorq    40(%rsp), %rdi
    xorq    128(%rsp), %rsi
    rorx    $2, %r13, %r9
    rorx    $9, %r10, %r13
    movq    %r13, %rax
    andq    %r9, %r13
    rorx    $23, %rdi, %rdi
    rorx    $25, %rdx, %rdx
    notq    %rax
    rorx    $62, %rsi, %rsi
    movq    %rdi, %r10
    movq    %rax, %r14
    orq %rdx, %r10
    movq    %rsi, %r12
    andq    %rdx, %r14
    xorq    %rax, %r10
    andq    %rdi, %r12
    movq    %rsi, %rax
    xorq    %rdx, %r12
    orq %r9, %rax
    movq    48(%rsp), %rdx
    xorq    %r9, %r14
    xorq    %rdi, %rax
    movq    136(%rsp), %r9
    movq    %rax, 128(%rsp)
    movq    192(%rsp), %rax
    xorq    %rsi, %r13
    xorq    88(%rsp), %rax
    xorq    %rbx, %rdx
    xorq    80(%rsp), %rdx
    xorq    120(%rsp), %rdx
    xorq    %r13, %r9
    xorq    104(%rsp), %r9
    xorq    208(%rsp), %r9
    xorq    %r10, %rbp
    xorq    %r14, %r8
    rorx    $63, %rbp, %rdi
    movq    %r10, 40(%rsp)
    xorq    %r11, %rax
    xorq    176(%rsp), %rax
    xorq    128(%rsp), %rax
    xorq    %r12, %rdx
    xorq    %r15, %r9
    rorx    $63, %rdx, %rsi
    xorq    %r9, %rdi
    xorq    %r8, %rsi
    rorx    $63, %r9, %r9
    rorx    $63, %r8, %r8
    xorq    %rdx, %r9
    movq    184(%rsp), %rdx
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    movq    24(%rsp), %rax
    xorq    %rbp, %r10
    movabsq $-9223372034707259392, %rbp
    xorq    %rdi, %rax
    xorq    %rsi, %rdx
    xorq    %r10, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rax, %rbp
    rorx    $20, %rdx, %rdx
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    xorq    %r9, %r11
    orq %rdx, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    rorx    $43, %r11, %r11
    notq    %rbp
    xorq    %r8, %r13
    orq %r11, %rbp
    rorx    $50, %r13, %r13
    xorq    %rsi, %rcx
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    xorq    %r10, %r12
    movq    %rbp, 72(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 152(%rsp)
    movq    88(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    rorx    $19, %rcx, %rcx
    orq %rax, %rbx
    movq    136(%rsp), %rax
    movq    %rbp, 56(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    rorx    $3, %r12, %r12
    rorx    $36, %rdx, %rdx
    movq    %rbx, 184(%rsp)
    movq    %rcx, %r13
    xorq    %r8, %rax
    movq    %r12, %rbx
    rorx    $44, %rax, %r11
    movq    32(%rsp), %rax
    notq    %rbx
    orq %rcx, %rbx
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r12, %r11
    movq    %r13, 136(%rsp)
    movq    %rbp, 88(%rsp)
    movq    %r12, %rbp
    movq    %rbx, 32(%rsp)
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    movq    %r11, %rcx
    movq    %rbp, 200(%rsp)
    movq    %rbp, %rax
    xorq    184(%rsp), %rax
    movq    %r11, 144(%rsp)
    movq    64(%rsp), %r11
    movq    48(%rsp), %rdx
    movq    176(%rsp), %rbp
    xorq    152(%rsp), %rcx
    xorq    %rsi, %r11
    xorq    %r10, %rdx
    xorq    %r8, %r15
    xorq    %r9, %rbp
    rorx    $56, %r15, %r15
    xorq    %rdi, %r14
    rorx    $39, %rbp, %rbp
    movq    %r15, %r13
    notq    %r15
    rorx    $46, %r14, %r14
    movq    %r15, %rbx
    rorx    $58, %rdx, %rdx
    movq    %rbp, %r12
    andq    %r14, %rbx
    rorx    $63, %r11, %r11
    orq %rdx, %r12
    xorq    %rbp, %rbx
    andq    %rbp, %r13
    movq    %r14, %rbp
    xorq    %r11, %r12
    xorq    %rdx, %r13
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    160(%rsp), %r11
    movq    %r13, 48(%rsp)
    movq    112(%rsp), %r13
    xorq    %r14, %rdx
    movq    %r12, 64(%rsp)
    movq    104(%rsp), %r12
    xorq    %r15, %rbp
    movq    120(%rsp), %r14
    movq    %rdx, 176(%rsp)
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    movq    88(%rsp), %rdx
    xorq    24(%rsp), %rdx
    rorx    $54, %r11, %r11
    xorq    64(%rsp), %rdx
    movq    %rbp, 168(%rsp)
    xorq    %rbp, %rax
    xorq    %rdi, %r13
    movq    128(%rsp), %rbp
    xorq    %r8, %r12
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    andq    %r13, %r15
    xorq    %r12, %r15
    xorq    %r9, %rbp
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %r15, 128(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 104(%rsp)
    orq %rbp, %r14
    orq %r11, %r15
    xorq    %r11, %r14
    movq    %rbp, %r11
    xorq    %r13, %r15
    andq    %r12, %r11
    xorq    104(%rsp), %r11
    movq    %r14, 120(%rsp)
    xorq    %r11, %rax
    orq %r12, %r13
    xorq    192(%rsp), %r9
    xorq    96(%rsp), %rdi
    xorq    208(%rsp), %r8
    xorq    %rbp, %r13
    xorq    40(%rsp), %rsi
    xorq    80(%rsp), %r10
    xorq    %r13, %rcx
    movq    %r13, 112(%rsp)
    rorx    $9, %r9, %r9
    rorx    $23, %rdi, %rdi
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    movq    %rdi, %r12
    notq    %rbp
    rorx    $62, %rsi, %rsi
    orq %r8, %r12
    movq    %rbp, %r14
    rorx    $2, %r10, %r10
    xorq    %rbp, %r12
    movq    48(%rsp), %rbp
    xorq    136(%rsp), %rbp
    movq    %r12, 40(%rsp)
    andq    %r8, %r14
    movq    %rsi, %r13
    xorq    %r10, %r14
    orq %r10, %r13
    andq    %r9, %r10
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r14, %rdx
    xorq    %r15, %rbp
    xorq    %r13, %rax
    xorq    %r10, %rcx
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    72(%rsp), %rbp
    andq    %rdi, %r12
    rorx    $63, %rax, %r9
    xorq    %r8, %r12
    movq    56(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %rbp, %r9
    xorq    %r12, %r8
    xorq    32(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbx, %r8
    xorq    120(%rsp), %r8
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    136(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rsi, %r8
    xorq    %rcx, %r11
    rorx    $20, %r8, %r8
    rorx    $43, %r11, %r11
    xorq    %rdx, %r10
    xorq    %rdi, %rax
    rorx    $50, %r10, %r10
    xorq    %rsi, %r15
    movq    %rax, %rbp
    xorq    %r9, %r12
    rorx    $19, %r15, %r15
    xorq    $32907, %rbp
    rorx    $3, %r12, %r12
    movq    %rbp, 96(%rsp)
    movq    %rbx, %rbp
    orq %r8, %rbp
    xorq    %rbp, 96(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 24(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 192(%rsp)
    movq    64(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    184(%rsp), %rax
    movq    %rbp, 80(%rsp)
    xorq    %r11, %rbx
    movq    144(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    movq    %rbx, 104(%rsp)
    movq    %r12, %rbx
    xorq    %rcx, %rax
    movq    %r8, %rbp
    andq    %r8, %r10
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    notq    %rbx
    rorx    $44, %r11, %r11
    orq %r15, %rbx
    orq %r11, %rbp
    xorq    %r11, %r10
    andq    %rax, %r11
    xorq    %rax, %rbp
    xorq    %r8, %rbx
    xorq    %r12, %r11
    movq    %rbp, 184(%rsp)
    movq    %r10, 136(%rsp)
    movq    %r12, %rbp
    movq    72(%rsp), %r8
    movq    112(%rsp), %r12
    orq %rax, %rbp
    movq    32(%rsp), %r10
    xorq    %r15, %rbp
    movq    168(%rsp), %rax
    movq    %rbx, 64(%rsp)
    movq    %r11, 144(%rsp)
    xorq    %rsi, %r8
    movq    %rbp, 160(%rsp)
    xorq    %r9, %r10
    xorq    %rdx, %r12
    xorq    %rdi, %r14
    rorx    $56, %r12, %r12
    rorx    $46, %r14, %r14
    xorq    %rcx, %rax
    movq    %r12, %r15
    notq    %r12
    rorx    $39, %rax, %rax
    movq    %r12, %rbx
    rorx    $63, %r8, %r8
    andq    %rax, %r15
    andq    %r14, %rbx
    movq    %rax, %r11
    rorx    $58, %r10, %r10
    xorq    %rax, %rbx
    movq    %r14, %rax
    xorq    %r10, %r15
    orq %r8, %rax
    orq %r10, %r11
    andq    %r8, %r10
    xorq    %r12, %rax
    xorq    %r14, %r10
    xorq    %r8, %r11
    movq    %rax, 208(%rsp)
    movq    %rbp, %rax
    movq    48(%rsp), %rbp
    movq    %r10, 224(%rsp)
    movq    88(%rsp), %r10
    xorq    %rcx, %r13
    movq    152(%rsp), %r8
    movq    120(%rsp), %r12
    rorx    $8, %r13, %r13
    xorq    %rsi, %rbp
    movq    %r11, 112(%rsp)
    xorq    104(%rsp), %rax
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    208(%rsp), %rax
    rorx    $28, %r10, %r10
    xorq    %rdx, %r8
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    %r8, %r11
    rorx    $49, %r12, %r12
    xorq    56(%rsp), %r9
    movq    %r11, 120(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    movq    %r15, 168(%rsp)
    orq %r13, %r14
    xorq    %r10, %r11
    xorq    %rbp, %r14
    movq    %r13, %rbp
    rorx    $2, %r9, %r9
    andq    %r8, %rbp
    movq    %r14, 88(%rsp)
    xorq    %r12, %rbp
    movq    %r10, %r12
    orq %r8, %r12
    xorq    %rbp, %rax
    movq    184(%rsp), %r8
    xorq    %r13, %r12
    xorq    200(%rsp), %rcx
    xorq    128(%rsp), %rdi
    xorq    176(%rsp), %rdx
    xorq    96(%rsp), %r8
    xorq    112(%rsp), %r8
    xorq    40(%rsp), %rsi
    xorq    120(%rsp), %r8
    rorx    $23, %rdi, %rdi
    rorx    $9, %rcx, %rcx
    rorx    $25, %rdx, %rdx
    movq    %rcx, %r10
    movq    %rdi, %r15
    notq    %r10
    orq %rdx, %r15
    rorx    $62, %rsi, %rsi
    xorq    %r10, %r15
    movq    %r10, %r14
    movq    %rsi, %r13
    movq    %r15, 40(%rsp)
    movq    168(%rsp), %r10
    andq    %rdx, %r14
    xorq    136(%rsp), %r10
    xorq    %r9, %r14
    orq %r9, %r13
    andq    %rcx, %r9
    movq    144(%rsp), %rcx
    xorq    192(%rsp), %rcx
    xorq    224(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %rsi, %r9
    xorq    %r13, %rax
    xorq    %r14, %r8
    movq    %r13, 128(%rsp)
    xorq    %r11, %r10
    rorx    $63, %rax, %r13
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    24(%rsp), %r10
    andq    %rdi, %r15
    xorq    %r12, %rcx
    xorq    %rdx, %r15
    movq    80(%rsp), %rdx
    xorq    %r9, %rcx
    rorx    $63, %r10, %rdi
    xorq    %r15, %rdx
    xorq    64(%rsp), %rdx
    xorq    %rcx, %rdi
    xorq    %rbx, %rdx
    xorq    88(%rsp), %rdx
    rorx    $63, %rdx, %rsi
    xorq    %r8, %rsi
    xorq    %r10, %r13
    rorx    $63, %rcx, %r10
    xorq    %rdx, %r10
    rorx    $63, %r8, %rdx
    movq    96(%rsp), %r8
    xorq    %rax, %rdx
    movq    136(%rsp), %rax
    movl    $2147483649, %ecx
    xorq    %r13, %rbx
    xorq    %r10, %rbp
    xorq    %rdx, %r9
    xorq    %rdi, %r8
    rorx    $21, %rbx, %rbx
    rorx    $43, %rbp, %rbp
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $50, %r9, %r9
    rorx    $20, %rax, %rax
    movq    %rcx, 72(%rsp)
    xorq    %rsi, %r11
    movq    %rax, %rcx
    rorx    $19, %r11, %r11
    xorq    %r13, %r15
    orq %rbx, %rcx
    xorq    %rcx, 72(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    rorx    $3, %r15, %r15
    orq %rbp, %rcx
    xorq    %rax, %rcx
    andq    %r8, %rax
    movq    %rcx, 32(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 152(%rsp)
    movq    144(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    112(%rsp), %r8
    movq    %rcx, 96(%rsp)
    movq    104(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 56(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    orq %r11, %rbx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rax, %r9
    andq    %rcx, %rax
    xorq    %rcx, %rbp
    movq    %r9, 104(%rsp)
    movq    208(%rsp), %r9
    movq    %rbp, 112(%rsp)
    movq    %r15, %rbp
    xorq    %r8, %rbx
    orq %rcx, %rbp
    movq    64(%rsp), %rcx
    movq    24(%rsp), %r8
    xorq    %r11, %rbp
    xorq    %r15, %rax
    xorq    %r10, %r9
    movq    %rax, 200(%rsp)
    movq    %rdx, %rax
    rorx    $39, %r9, %r9
    xorq    %r13, %rcx
    xorq    %r12, %rax
    xorq    %rdi, %r14
    rorx    $58, %rcx, %rcx
    rorx    $46, %r14, %r14
    xorq    %rsi, %r8
    rorx    $56, %rax, %rax
    movq    %r9, %r11
    rorx    $63, %r8, %r8
    orq %rcx, %r11
    movq    %rax, %r12
    movq    %r14, %r15
    xorq    %r8, %r11
    andq    %r9, %r12
    notq    %rax
    orq %r8, %r15
    andq    %rcx, %r8
    xorq    %rcx, %r12
    xorq    %rax, %r15
    xorq    %r14, %r8
    movq    %rbx, 48(%rsp)
    movq    %rbp, 136(%rsp)
    movq    %r11, 144(%rsp)
    movq    %rax, %rbx
    movq    %r12, 176(%rsp)
    movq    %r15, 208(%rsp)
    andq    %r14, %rbx
    movq    %r8, 232(%rsp)
    movq    168(%rsp), %r11
    xorq    %r9, %rbx
    movq    184(%rsp), %r12
    movq    192(%rsp), %rax
    movq    72(%rsp), %r8
    xorq    112(%rsp), %r8
    xorq    %rsi, %r11
    movq    88(%rsp), %r14
    xorq    144(%rsp), %r8
    rorx    $54, %r11, %r11
    xorq    %rdi, %r12
    xorq    %rdx, %rax
    rorx    $28, %r12, %r12
    movq    %r11, %rcx
    rorx    $37, %rax, %rax
    movq    128(%rsp), %r9
    andq    %r12, %rcx
    xorq    %r13, %r14
    xorq    %rax, %rcx
    movq    32(%rsp), %rbp
    rorx    $49, %r14, %r14
    xorq    104(%rsp), %rbp
    movq    %rcx, 128(%rsp)
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    176(%rsp), %rbp
    xorq    %r10, %r9
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r14, %r15
    xorq    %r12, %rcx
    orq %r9, %r15
    xorq    %r11, %r15
    movq    %r9, %r11
    xorq    %rcx, %rbp
    andq    %rax, %r11
    xorq    80(%rsp), %r13
    xorq    160(%rsp), %r10
    xorq    120(%rsp), %rdi
    xorq    40(%rsp), %rsi
    xorq    %r14, %r11
    xorq    224(%rsp), %rdx
    movq    %r15, 88(%rsp)
    movq    %r12, %r15
    orq %rax, %r15
    xorq    %r9, %r15
    rorx    $2, %r13, %r9
    rorx    $9, %r10, %r13
    rorx    $23, %rdi, %rdi
    rorx    $62, %rsi, %rsi
    movq    %r13, %rax
    rorx    $25, %rdx, %rdx
    movq    %rdi, %r10
    notq    %rax
    orq %rdx, %r10
    movq    %rsi, %r12
    movq    %rax, %r14
    xorq    %rax, %r10
    andq    %rdi, %r12
    movq    %rsi, %rax
    xorq    %rdx, %r12
    andq    %rdx, %r14
    orq %r9, %rax
    movq    48(%rsp), %rdx
    xorq    %r9, %r14
    xorq    %rdi, %rax
    andq    %r9, %r13
    movq    200(%rsp), %r9
    movq    %rax, 80(%rsp)
    movq    136(%rsp), %rax
    xorq    56(%rsp), %rax
    xorq    %rsi, %r13
    xorq    %rbx, %rdx
    xorq    96(%rsp), %rdx
    xorq    %r10, %rbp
    xorq    88(%rsp), %rdx
    xorq    %r13, %r9
    xorq    152(%rsp), %r9
    xorq    232(%rsp), %r9
    xorq    %r14, %r8
    rorx    $63, %rbp, %rdi
    xorq    %r11, %rax
    xorq    208(%rsp), %rax
    movq    %r10, 40(%rsp)
    xorq    80(%rsp), %rax
    xorq    %r12, %rdx
    xorq    %r15, %r9
    rorx    $63, %rdx, %rsi
    xorq    %r9, %rdi
    xorq    %r8, %rsi
    rorx    $63, %r8, %r8
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    rorx    $63, %r9, %r9
    movq    72(%rsp), %rax
    xorq    %rdx, %r9
    movq    104(%rsp), %rdx
    xorq    %rbp, %r10
    movabsq $-9223372034707259263, %rbp
    xorq    %r9, %r11
    xorq    %r10, %rbx
    rorx    $43, %r11, %r11
    xorq    %r8, %r13
    xorq    %rdi, %rax
    rorx    $21, %rbx, %rbx
    xorq    %rsi, %rdx
    xorq    %rax, %rbp
    rorx    $20, %rdx, %rdx
    rorx    $50, %r13, %r13
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    xorq    %r10, %r12
    orq %rdx, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $3, %r12, %r12
    xorq    %rsi, %rcx
    orq %r11, %rbp
    rorx    $19, %rcx, %rcx
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    movq    %rbp, 64(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 184(%rsp)
    movq    56(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    movq    %rcx, %r13
    orq %rax, %rbx
    movq    200(%rsp), %rax
    movq    %rbp, 120(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    rorx    $36, %rdx, %rdx
    movq    %rbx, 104(%rsp)
    movq    %r12, %rbx
    xorq    %r8, %rax
    notq    %rbx
    rorx    $44, %rax, %r11
    movq    144(%rsp), %rax
    orq %rcx, %rbx
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    movq    %rbx, 72(%rsp)
    xorq    %rdx, %rbp
    movq    %r13, 192(%rsp)
    movq    %rbp, 56(%rsp)
    movq    %r12, %rbp
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    andq    %rdx, %r11
    xorq    %r8, %r15
    movq    %rbp, 200(%rsp)
    movq    %rbp, %rax
    movq    208(%rsp), %rbp
    xorq    %r12, %r11
    movq    48(%rsp), %rdx
    rorx    $56, %r15, %r15
    movq    %r11, 160(%rsp)
    movq    %r11, %rcx
    movq    32(%rsp), %r11
    xorq    %r9, %rbp
    xorq    %rdi, %r14
    movq    %r15, %r13
    notq    %r15
    rorx    $39, %rbp, %rbp
    rorx    $46, %r14, %r14
    xorq    %r10, %rdx
    movq    %r15, %rbx
    xorq    %rsi, %r11
    rorx    $58, %rdx, %rdx
    andq    %r14, %rbx
    movq    %rbp, %r12
    xorq    184(%rsp), %rcx
    rorx    $63, %r11, %r11
    xorq    %rbp, %rbx
    orq %rdx, %r12
    andq    %rbp, %r13
    movq    %r14, %rbp
    xorq    %r11, %r12
    xorq    %rdx, %r13
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    176(%rsp), %r11
    xorq    104(%rsp), %rax
    movq    %r13, 144(%rsp)
    movq    112(%rsp), %r13
    xorq    %r14, %rdx
    movq    %r12, 48(%rsp)
    movq    152(%rsp), %r12
    xorq    %rdx, %rcx
    movq    88(%rsp), %r14
    movq    %rdx, 208(%rsp)
    xorq    %rsi, %r11
    movq    56(%rsp), %rdx
    xorq    24(%rsp), %rdx
    xorq    %r15, %rbp
    rorx    $54, %r11, %r11
    xorq    48(%rsp), %rdx
    xorq    %rdi, %r13
    movq    %rbp, 168(%rsp)
    xorq    %rbp, %rax
    xorq    %r8, %r12
    movq    80(%rsp), %rbp
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    andq    %r13, %r15
    xorq    %r12, %r15
    rorx    $49, %r14, %r14
    xorq    %r9, %rbp
    movq    %r15, 80(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    rorx    $8, %rbp, %rbp
    notq    %r14
    orq %r11, %r15
    movq    %r14, 32(%rsp)
    xorq    %r13, %r15
    orq %rbp, %r14
    orq %r12, %r13
    xorq    %rbp, %r13
    xorq    %r11, %r14
    movq    %rbp, %r11
    movq    %r14, 88(%rsp)
    andq    %r12, %r11
    xorq    32(%rsp), %r11
    movq    %r13, 112(%rsp)
    xorq    136(%rsp), %r9
    xorq    %r13, %rcx
    xorq    128(%rsp), %rdi
    xorq    232(%rsp), %r8
    xorq    40(%rsp), %rsi
    xorq    96(%rsp), %r10
    xorq    %r11, %rax
    rorx    $9, %r9, %r9
    rorx    $23, %rdi, %rdi
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    movq    %rdi, %r12
    notq    %rbp
    rorx    $62, %rsi, %rsi
    orq %r8, %r12
    movq    %rbp, %r14
    rorx    $2, %r10, %r10
    xorq    %rbp, %r12
    movq    144(%rsp), %rbp
    xorq    192(%rsp), %rbp
    movq    %r12, 40(%rsp)
    andq    %r8, %r14
    movq    %rsi, %r13
    orq %r10, %r13
    xorq    %r10, %r14
    andq    %r9, %r10
    xorq    %rdi, %r13
    xorq    %r14, %rdx
    xorq    %r15, %rbp
    xorq    %r13, %rax
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    64(%rsp), %rbp
    andq    %rdi, %r12
    rorx    $63, %rax, %r9
    xorq    %r8, %r12
    movq    120(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %r12, %r8
    xorq    72(%rsp), %r8
    xorq    %rbx, %r8
    xorq    88(%rsp), %r8
    xorq    %rsi, %r10
    xorq    %r10, %rcx
    xorq    %rbp, %r9
    movabsq $-9223372036854743031, %rbp
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %r9, %r12
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    192(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rcx, %r11
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    rorx    $43, %r11, %r11
    xorq    %rsi, %r8
    xorq    %rdx, %r10
    rorx    $3, %r12, %r12
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    xorq    %rsi, %r15
    xorq    %rdi, %rax
    rorx    $19, %r15, %r15
    xorq    %rax, %rbp
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    orq %r8, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 32(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 192(%rsp)
    movq    48(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    104(%rsp), %rax
    movq    %rbp, 96(%rsp)
    xorq    %r11, %rbx
    movq    160(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    movq    %rbx, 128(%rsp)
    movq    %r12, %rbx
    xorq    %rcx, %rax
    movq    %r8, %rbp
    notq    %rbx
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    andq    %r8, %r10
    rorx    $44, %r11, %r11
    orq %r15, %rbx
    orq %r11, %rbp
    xorq    %r11, %r10
    xorq    %rax, %rbp
    xorq    %r8, %rbx
    andq    %rax, %r11
    xorq    %r12, %r11
    movq    %rbp, 104(%rsp)
    movq    %r12, %rbp
    movq    112(%rsp), %r12
    orq %rax, %rbp
    movq    168(%rsp), %rax
    movq    %r10, 152(%rsp)
    movq    64(%rsp), %r8
    xorq    %r15, %rbp
    movq    72(%rsp), %r10
    xorq    %rdi, %r14
    movq    %rbx, 48(%rsp)
    xorq    %rdx, %r12
    rorx    $46, %r14, %r14
    xorq    %rcx, %rax
    rorx    $56, %r12, %r12
    rorx    $39, %rax, %rax
    xorq    %rsi, %r8
    movq    %r12, %r15
    notq    %r12
    xorq    %r9, %r10
    movq    %r12, %rbx
    rorx    $63, %r8, %r8
    movq    %r11, 160(%rsp)
    andq    %r14, %rbx
    rorx    $58, %r10, %r10
    andq    %rax, %r15
    xorq    %rax, %rbx
    movq    %rax, %r11
    movq    %r14, %rax
    orq %r10, %r11
    orq %r8, %rax
    xorq    %r10, %r15
    xorq    %r8, %r11
    xorq    %r12, %rax
    andq    %r8, %r10
    xorq    %r14, %r10
    movq    %rbp, 136(%rsp)
    movq    %r11, 64(%rsp)
    movq    %rax, 112(%rsp)
    movq    %rbp, %rax
    movq    144(%rsp), %rbp
    movq    %r10, 168(%rsp)
    movq    56(%rsp), %r10
    xorq    %rcx, %r13
    movq    184(%rsp), %r8
    movq    88(%rsp), %r12
    rorx    $8, %r13, %r13
    xorq    %rsi, %rbp
    xorq    128(%rsp), %rax
    movq    %r15, 72(%rsp)
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    112(%rsp), %rax
    rorx    $28, %r10, %r10
    xorq    %rdx, %r8
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    %r8, %r11
    rorx    $49, %r12, %r12
    movq    %r11, 88(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    orq %r13, %r14
    xorq    %r10, %r11
    xorq    %rbp, %r14
    movq    %r13, %rbp
    andq    %r8, %rbp
    xorq    80(%rsp), %rdi
    xorq    200(%rsp), %rcx
    xorq    208(%rsp), %rdx
    xorq    %r12, %rbp
    movq    %r10, %r12
    xorq    40(%rsp), %rsi
    movq    %r14, 56(%rsp)
    orq %r8, %r12
    xorq    120(%rsp), %r9
    movq    104(%rsp), %r8
    xorq    %r13, %r12
    rorx    $23, %rdi, %rdi
    rorx    $9, %rcx, %rcx
    xorq    24(%rsp), %r8
    rorx    $25, %rdx, %rdx
    movq    %rcx, %r10
    movq    %rdi, %r15
    notq    %r10
    orq %rdx, %r15
    rorx    $62, %rsi, %rsi
    xorq    %r10, %r15
    movq    %r10, %r14
    movq    72(%rsp), %r10
    xorq    152(%rsp), %r10
    movq    %r15, 40(%rsp)
    andq    %rdx, %r14
    xorq    64(%rsp), %r8
    rorx    $2, %r9, %r9
    movq    %rsi, %r13
    xorq    88(%rsp), %r8
    orq %r9, %r13
    xorq    %r9, %r14
    xorq    %rdi, %r13
    xorq    %rbp, %rax
    andq    %rcx, %r9
    xorq    %r11, %r10
    xorq    %r13, %rax
    xorq    %rsi, %r9
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    32(%rsp), %r10
    andq    %rdi, %r15
    xorq    %r14, %r8
    movq    160(%rsp), %rcx
    xorq    %rdx, %r15
    movq    96(%rsp), %rdx
    movq    %r13, 80(%rsp)
    rorx    $63, %rax, %r13
    rorx    $63, %r10, %rdi
    xorq    %r15, %rdx
    xorq    48(%rsp), %rdx
    xorq    %rbx, %rdx
    xorq    56(%rsp), %rdx
    xorq    192(%rsp), %rcx
    xorq    168(%rsp), %rcx
    xorq    %r10, %r13
    xorq    %r13, %rbx
    xorq    %r13, %r15
    rorx    $21, %rbx, %rbx
    rorx    $3, %r15, %r15
    rorx    $63, %rdx, %rsi
    xorq    %r12, %rcx
    xorq    %r8, %rsi
    xorq    %r9, %rcx
    xorq    %rsi, %r11
    rorx    $63, %rcx, %r10
    xorq    %rcx, %rdi
    rorx    $19, %r11, %r11
    xorq    %rdx, %r10
    rorx    $63, %r8, %rdx
    movq    24(%rsp), %r8
    xorq    %rax, %rdx
    movq    152(%rsp), %rax
    xorq    %r10, %rbp
    rorx    $43, %rbp, %rbp
    xorq    %rdx, %r9
    xorq    %rdi, %r8
    rorx    $50, %r9, %r9
    xorq    %rsi, %rax
    movq    %r8, %rcx
    rorx    $20, %rax, %rax
    xorb    $-118, %cl
    movq    %rcx, 120(%rsp)
    movq    %rax, %rcx
    orq %rbx, %rcx
    xorq    %rcx, 120(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    orq %rbp, %rcx
    xorq    %rax, %rcx
    andq    %r8, %rax
    movq    %rcx, 24(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 200(%rsp)
    movq    160(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    64(%rsp), %r8
    movq    %rcx, 184(%rsp)
    movq    128(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 152(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rcx, %rbp
    xorq    %rax, %r9
    orq %r11, %rbx
    movq    %rbp, 128(%rsp)
    movq    %r9, 160(%rsp)
    movq    %r15, %rbp
    movq    112(%rsp), %r9
    orq %rcx, %rbp
    andq    %rcx, %rax
    movq    48(%rsp), %rcx
    xorq    %r8, %rbx
    xorq    %r15, %rax
    movq    32(%rsp), %r8
    movq    %rax, 176(%rsp)
    movq    %rdx, %rax
    xorq    %r10, %r9
    xorq    %r12, %rax
    xorq    %r11, %rbp
    rorx    $39, %r9, %r9
    xorq    %r13, %rcx
    rorx    $56, %rax, %rax
    rorx    $58, %rcx, %rcx
    xorq    %rsi, %r8
    movq    %r9, %r11
    rorx    $63, %r8, %r8
    xorq    %rdi, %r14
    orq %rcx, %r11
    movq    %rax, %r12
    rorx    $46, %r14, %r14
    xorq    %r8, %r11
    andq    %r9, %r12
    movq    %r11, 32(%rsp)
    movq    %r14, %r15
    xorq    %rcx, %r12
    movq    72(%rsp), %r11
    notq    %rax
    movq    %r12, 48(%rsp)
    orq %r8, %r15
    movq    104(%rsp), %r12
    andq    %rcx, %r8
    xorq    %rax, %r15
    movq    %rbx, 64(%rsp)
    xorq    %r14, %r8
    movq    %rax, %rbx
    movq    192(%rsp), %rax
    movq    %r8, 208(%rsp)
    xorq    %rsi, %r11
    movq    120(%rsp), %r8
    xorq    128(%rsp), %r8
    rorx    $54, %r11, %r11
    andq    %r14, %rbx
    xorq    32(%rsp), %r8
    xorq    %rdi, %r12
    movq    56(%rsp), %r14
    rorx    $28, %r12, %r12
    xorq    %rdx, %rax
    movq    %r11, %rcx
    xorq    %r9, %rbx
    rorx    $37, %rax, %rax
    movq    80(%rsp), %r9
    andq    %r12, %rcx
    xorq    %r13, %r14
    movq    %r15, 112(%rsp)
    xorq    %rax, %rcx
    rorx    $49, %r14, %r14
    movq    %rbp, 144(%rsp)
    movq    %rcx, 80(%rsp)
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    %r10, %r9
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r14, %r15
    xorq    %r12, %rcx
    orq %r9, %r15
    movq    24(%rsp), %rbp
    xorq    160(%rsp), %rbp
    xorq    %r11, %r15
    xorq    48(%rsp), %rbp
    movq    %r9, %r11
    movq    %r15, 56(%rsp)
    xorq    96(%rsp), %r13
    movq    %r12, %r15
    xorq    136(%rsp), %r10
    xorq    88(%rsp), %rdi
    orq %rax, %r15
    xorq    40(%rsp), %rsi
    xorq    168(%rsp), %rdx
    xorq    %r9, %r15
    andq    %rax, %r11
    xorq    %rcx, %rbp
    rorx    $2, %r13, %r9
    xorq    %r14, %r11
    rorx    $9, %r10, %r13
    rorx    $23, %rdi, %rdi
    rorx    $62, %rsi, %rsi
    movq    %r13, %rax
    rorx    $25, %rdx, %rdx
    notq    %rax
    movq    %rdi, %r10
    movq    %rsi, %r12
    orq %rdx, %r10
    andq    %rdi, %r12
    movq    %rax, %r14
    xorq    %rax, %r10
    xorq    %rdx, %r12
    andq    %rdx, %r14
    movq    %rsi, %rax
    movq    64(%rsp), %rdx
    xorq    %r9, %r14
    orq %r9, %rax
    xorq    %r10, %rbp
    xorq    %r14, %r8
    xorq    %rdi, %rax
    movq    %r10, 40(%rsp)
    rorx    $63, %rbp, %rdi
    movq    %rax, 96(%rsp)
    xorq    %rbx, %rdx
    movq    144(%rsp), %rax
    xorq    184(%rsp), %rdx
    xorq    152(%rsp), %rax
    xorq    56(%rsp), %rdx
    xorq    %r11, %rax
    xorq    112(%rsp), %rax
    xorq    96(%rsp), %rax
    xorq    %r12, %rdx
    andq    %r9, %r13
    movq    176(%rsp), %r9
    xorq    %rsi, %r13
    rorx    $63, %rdx, %rsi
    xorq    %r8, %rsi
    rorx    $63, %r8, %r8
    xorq    %rsi, %rcx
    xorq    %r13, %r9
    xorq    200(%rsp), %r9
    xorq    %rax, %r8
    xorq    208(%rsp), %r9
    rorx    $63, %rax, %r10
    movq    120(%rsp), %rax
    xorq    %rbp, %r10
    xorq    %r8, %r13
    rorx    $19, %rcx, %rcx
    xorq    %r10, %rbx
    rorx    $50, %r13, %r13
    xorq    %r10, %r12
    rorx    $21, %rbx, %rbx
    rorx    $3, %r12, %r12
    xorq    %r15, %r9
    xorq    %r9, %rdi
    rorx    $63, %r9, %r9
    xorq    %rdx, %r9
    movq    160(%rsp), %rdx
    xorq    %rdi, %rax
    movq    %rax, %rbp
    xorq    %r9, %r11
    xorb    $-120, %bpl
    rorx    $43, %r11, %r11
    xorq    %rsi, %rdx
    movq    %rbp, 120(%rsp)
    movq    %rbx, %rbp
    rorx    $20, %rdx, %rdx
    orq %rdx, %rbp
    xorq    %rbp, 120(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    movq    %rbp, 72(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 192(%rsp)
    movq    152(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    movq    %rcx, %r13
    orq %rax, %rbx
    movq    176(%rsp), %rax
    movq    %rbp, 88(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    rorx    $36, %rdx, %rdx
    movq    %rbx, 104(%rsp)
    movq    %r12, %rbx
    xorq    %r8, %rax
    notq    %rbx
    rorx    $44, %rax, %r11
    movq    32(%rsp), %rax
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    orq %r11, %rbp
    xorq    %rdx, %rbp
    andq    %rax, %r13
    orq %rcx, %rbx
    movq    %rbp, 152(%rsp)
    movq    %r12, %rbp
    xorq    %r11, %r13
    orq %rdx, %rbp
    andq    %rdx, %r11
    xorq    %rax, %rbx
    xorq    %rcx, %rbp
    xorq    %r12, %r11
    movq    %r13, 136(%rsp)
    movq    %rbx, 32(%rsp)
    movq    %rbp, 160(%rsp)
    movq    %rbp, %rax
    xorq    104(%rsp), %rax
    movq    %r11, 168(%rsp)
    xorq    %r8, %r15
    movq    112(%rsp), %rbp
    movq    64(%rsp), %rdx
    movq    %r11, %rcx
    rorx    $56, %r15, %r15
    movq    24(%rsp), %r11
    xorq    %rdi, %r14
    movq    %r15, %r13
    notq    %r15
    rorx    $46, %r14, %r14
    xorq    %r9, %rbp
    xorq    %r10, %rdx
    movq    %r15, %rbx
    rorx    $39, %rbp, %rbp
    xorq    %rsi, %r11
    rorx    $58, %rdx, %rdx
    andq    %r14, %rbx
    movq    %rbp, %r12
    rorx    $63, %r11, %r11
    xorq    %rbp, %rbx
    orq %rdx, %r12
    andq    %rbp, %r13
    movq    %r14, %rbp
    xorq    %r11, %r12
    xorq    %rdx, %r13
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    48(%rsp), %r11
    movq    %r13, 176(%rsp)
    movq    128(%rsp), %r13
    xorq    %r15, %rbp
    xorq    192(%rsp), %rcx
    xorq    %r14, %rdx
    movq    %r12, 112(%rsp)
    movq    56(%rsp), %r14
    movq    %rbp, 224(%rsp)
    xorq    %rbp, %rax
    movq    200(%rsp), %r12
    movq    96(%rsp), %rbp
    xorq    %rsi, %r11
    rorx    $54, %r11, %r11
    xorq    %rdi, %r13
    movq    %rdx, 232(%rsp)
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    xorq    %rdx, %rcx
    xorq    %r8, %r12
    xorq    %r10, %r14
    xorq    %r9, %rbp
    movq    152(%rsp), %rdx
    andq    %r13, %r15
    xorq    120(%rsp), %rdx
    xorq    112(%rsp), %rdx
    rorx    $37, %r12, %r12
    xorq    144(%rsp), %r9
    xorq    80(%rsp), %rdi
    xorq    %r12, %r15
    xorq    208(%rsp), %r8
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %r15, 96(%rsp)
    xorq    40(%rsp), %rsi
    xorq    184(%rsp), %r10
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 24(%rsp)
    orq %r11, %r15
    orq %rbp, %r14
    xorq    %r13, %r15
    xorq    %r11, %r14
    rorx    $23, %rdi, %rdi
    orq %r12, %r13
    rorx    $9, %r9, %r9
    movq    %rbp, %r11
    xorq    %rbp, %r13
    rorx    $25, %r8, %r8
    andq    %r12, %r11
    movq    %r9, %rbp
    movq    %rdi, %r12
    movq    %r14, 128(%rsp)
    notq    %rbp
    orq %r8, %r12
    rorx    $62, %rsi, %rsi
    xorq    %rbp, %r12
    movq    %rbp, %r14
    movq    176(%rsp), %rbp
    xorq    136(%rsp), %rbp
    movq    %r12, 40(%rsp)
    andq    %r8, %r14
    xorq    24(%rsp), %r11
    rorx    $2, %r10, %r10
    xorq    %r13, %rcx
    xorq    %r10, %r14
    movq    %r13, 56(%rsp)
    movq    %rsi, %r13
    xorq    %r14, %rdx
    xorq    %r15, %rbp
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    72(%rsp), %rbp
    andq    %rdi, %r12
    xorq    %r11, %rax
    xorq    %r8, %r12
    movq    88(%rsp), %r8
    xorq    %r12, %r8
    xorq    32(%rsp), %r8
    xorq    %rbx, %r8
    xorq    128(%rsp), %r8
    orq %r10, %r13
    andq    %r9, %r10
    xorq    %rdi, %r13
    rorx    $63, %rbp, %rdi
    xorq    %rsi, %r10
    xorq    %r13, %rax
    xorq    %r10, %rcx
    rorx    $63, %rax, %r9
    rorx    $63, %r8, %rsi
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %r8, %rcx
    xorq    %rax, %rdx
    movq    120(%rsp), %rax
    movq    136(%rsp), %r8
    xorq    %rbp, %r9
    movl    $2147516425, %ebp
    xorq    %rcx, %r11
    xorq    %r9, %rbx
    rorx    $43, %r11, %r11
    xorq    %rdx, %r10
    xorq    %rdi, %rax
    rorx    $21, %rbx, %rbx
    xorq    %rsi, %r8
    xorq    %rax, %rbp
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    xorq    %rsi, %r15
    orq %r8, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $19, %r15, %r15
    orq %r11, %rbp
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 64(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 184(%rsp)
    movq    112(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    104(%rsp), %rax
    movq    %rbp, 80(%rsp)
    xorq    %r11, %rbx
    movq    168(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    movq    %rbx, 120(%rsp)
    xorq    %rcx, %rax
    movq    %r8, %rbp
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    xorq    %r9, %r12
    rorx    $44, %r11, %r11
    andq    %r8, %r10
    rorx    $3, %r12, %r12
    orq %r11, %rbp
    xorq    %r11, %r10
    andq    %rax, %r11
    xorq    %rax, %rbp
    xorq    %r12, %r11
    movq    %r12, %rbx
    movq    %rbp, 112(%rsp)
    movq    %r12, %rbp
    movq    56(%rsp), %r12
    notq    %rbx
    orq %rax, %rbp
    movq    224(%rsp), %rax
    orq %r15, %rbx
    xorq    %r15, %rbp
    movq    %r10, 104(%rsp)
    xorq    %rdx, %r12
    xorq    %r8, %rbx
    movq    72(%rsp), %r8
    rorx    $56, %r12, %r12
    movq    32(%rsp), %r10
    xorq    %rdi, %r14
    movq    %r12, %r15
    notq    %r12
    rorx    $46, %r14, %r14
    movq    %rbx, 48(%rsp)
    xorq    %rcx, %rax
    movq    %r12, %rbx
    rorx    $39, %rax, %rax
    xorq    %rsi, %r8
    andq    %r14, %rbx
    rorx    $63, %r8, %r8
    xorq    %rax, %rbx
    movq    %r11, 200(%rsp)
    xorq    %r9, %r10
    andq    %rax, %r15
    movq    %rax, %r11
    movq    %r14, %rax
    rorx    $58, %r10, %r10
    movq    %rbp, 136(%rsp)
    orq %r8, %rax
    xorq    %r10, %r15
    orq %r10, %r11
    xorq    %r12, %rax
    andq    %r8, %r10
    xorq    %r8, %r11
    movq    %rax, 144(%rsp)
    xorq    %r14, %r10
    movq    %rbp, %rax
    movq    176(%rsp), %rbp
    movq    %r10, 168(%rsp)
    xorq    %rcx, %r13
    movq    152(%rsp), %r10
    movq    192(%rsp), %r8
    rorx    $8, %r13, %r13
    movq    128(%rsp), %r12
    movq    %r11, 32(%rsp)
    xorq    %rsi, %rbp
    xorq    120(%rsp), %rax
    movq    %r15, 56(%rsp)
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    %rdx, %r8
    rorx    $28, %r10, %r10
    movq    %rbp, %r11
    xorq    144(%rsp), %rax
    rorx    $37, %r8, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    %r8, %r11
    rorx    $49, %r12, %r12
    xorq    160(%rsp), %rcx
    movq    %r11, 128(%rsp)
    xorq    96(%rsp), %rdi
    movq    %r12, %r11
    notq    %r12
    xorq    232(%rsp), %rdx
    orq %rbp, %r11
    movq    %r12, %r14
    xorq    %r10, %r11
    xorq    88(%rsp), %r9
    orq %r13, %r14
    rorx    $9, %rcx, %rcx
    xorq    40(%rsp), %rsi
    xorq    %rbp, %r14
    movq    %r13, %rbp
    rorx    $23, %rdi, %rdi
    andq    %r8, %rbp
    rorx    $25, %rdx, %rdx
    movq    %rdi, %r15
    xorq    %r12, %rbp
    movq    %r10, %r12
    movq    %rcx, %r10
    orq %r8, %r12
    notq    %r10
    movq    112(%rsp), %r8
    orq %rdx, %r15
    xorq    24(%rsp), %r8
    movq    %r14, 192(%rsp)
    xorq    %r10, %r15
    xorq    32(%rsp), %r8
    movq    %r10, %r14
    xorq    128(%rsp), %r8
    movq    %r15, 40(%rsp)
    rorx    $62, %rsi, %rsi
    movq    56(%rsp), %r10
    xorq    104(%rsp), %r10
    andq    %rdx, %r14
    rorx    $2, %r9, %r9
    xorq    %r13, %r12
    movq    %rsi, %r13
    xorq    %r9, %r14
    xorq    %rbp, %rax
    xorq    %r14, %r8
    xorq    %r11, %r10
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    64(%rsp), %r10
    andq    %rdi, %r15
    xorq    %rdx, %r15
    movq    80(%rsp), %rdx
    xorq    %r15, %rdx
    xorq    48(%rsp), %rdx
    xorq    %rbx, %rdx
    xorq    192(%rsp), %rdx
    orq %r9, %r13
    andq    %rcx, %r9
    movq    200(%rsp), %rcx
    xorq    184(%rsp), %rcx
    xorq    168(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %rsi, %r9
    xorq    %r13, %rax
    movq    %r13, 96(%rsp)
    rorx    $63, %r10, %rdi
    rorx    $63, %rax, %r13
    rorx    $63, %rdx, %rsi
    xorq    %r10, %r13
    xorq    %r8, %rsi
    xorq    %r12, %rcx
    xorq    %r13, %rbx
    xorq    %r9, %rcx
    rorx    $21, %rbx, %rbx
    rorx    $63, %rcx, %r10
    xorq    %rcx, %rdi
    movl    $2147483658, %ecx
    xorq    %rdx, %r10
    rorx    $63, %r8, %rdx
    movq    24(%rsp), %r8
    xorq    %rax, %rdx
    movq    104(%rsp), %rax
    xorq    %r10, %rbp
    rorx    $43, %rbp, %rbp
    xorq    %rdx, %r9
    xorq    %rdi, %r8
    rorx    $50, %r9, %r9
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    movq    %rax, %rcx
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    orq %rbp, %rcx
    xorq    %rax, %rcx
    andq    %r8, %rax
    movq    %rcx, 72(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 152(%rsp)
    movq    200(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %rcx, 88(%rsp)
    orq %r8, %rbx
    movq    120(%rsp), %rcx
    movq    32(%rsp), %r8
    xorq    %rbp, %rbx
    xorq    %rdx, %rax
    rorx    $44, %rax, %rax
    movq    %rbx, 104(%rsp)
    xorq    %r10, %rcx
    xorq    %rdi, %r8
    xorq    %rsi, %r11
    xorq    %r13, %r15
    rorx    $61, %r8, %r8
    rorx    $19, %r11, %r11
    rorx    $36, %rcx, %rcx
    movq    %r8, %rbp
    movq    %r11, %r9
    rorx    $3, %r15, %r15
    orq %rax, %rbp
    andq    %r8, %r9
    movq    %r15, %rbx
    xorq    %rcx, %rbp
    xorq    %rax, %r9
    notq    %rbx
    movq    %rbp, 120(%rsp)
    movq    %r9, 200(%rsp)
    movq    %r15, %rbp
    movq    144(%rsp), %r9
    orq %r11, %rbx
    orq %rcx, %rbp
    andq    %rcx, %rax
    movq    48(%rsp), %rcx
    xorq    %r8, %rbx
    xorq    %r15, %rax
    movq    64(%rsp), %r8
    xorq    %rdi, %r14
    movq    %rax, 176(%rsp)
    xorq    %r10, %r9
    movq    %rdx, %rax
    rorx    $39, %r9, %r9
    xorq    %r13, %rcx
    xorq    %r12, %rax
    xorq    %r11, %rbp
    rorx    $58, %rcx, %rcx
    rorx    $46, %r14, %r14
    xorq    %rsi, %r8
    rorx    $56, %rax, %rax
    movq    %r9, %r11
    rorx    $63, %r8, %r8
    orq %rcx, %r11
    movq    %rax, %r12
    movq    %r14, %r15
    xorq    %r8, %r11
    andq    %r9, %r12
    notq    %rax
    orq %r8, %r15
    andq    %rcx, %r8
    xorq    %rcx, %r12
    xorq    %rax, %r15
    xorq    %r14, %r8
    movq    %rbx, 32(%rsp)
    movq    %rbp, 160(%rsp)
    movq    %rax, %rbx
    movq    %r11, 48(%rsp)
    movq    %r12, 144(%rsp)
    andq    %r14, %rbx
    movq    %r15, 208(%rsp)
    movq    %r8, 224(%rsp)
    xorq    %r9, %rbx
    movq    56(%rsp), %r11
    movq    112(%rsp), %r12
    movq    96(%rsp), %r9
    movq    184(%rsp), %rax
    movq    192(%rsp), %r14
    movq    24(%rsp), %r8
    xorq    %rsi, %r11
    xorq    %rdi, %r12
    movq    72(%rsp), %rbp
    rorx    $54, %r11, %r11
    rorx    $28, %r12, %r12
    xorq    %rdx, %rax
    movq    %r11, %rcx
    xorq    %r13, %r14
    xorq    %r10, %r9
    andq    %r12, %rcx
    xorq    120(%rsp), %r8
    rorx    $37, %rax, %rax
    xorq    48(%rsp), %r8
    xorq    %rax, %rcx
    rorx    $49, %r14, %r14
    movq    %rcx, 96(%rsp)
    rorx    $8, %r9, %r9
    xorq    80(%rsp), %r13
    xorq    136(%rsp), %r10
    xorq    40(%rsp), %rsi
    xorq    128(%rsp), %rdi
    xorq    168(%rsp), %rdx
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    200(%rsp), %rbp
    orq %r14, %rcx
    notq    %r14
    xorq    144(%rsp), %rbp
    movq    %r14, %r15
    rorx    $62, %rsi, %rsi
    xorq    %r12, %rcx
    orq %r9, %r15
    rorx    $23, %rdi, %rdi
    rorx    $25, %rdx, %rdx
    xorq    %r11, %r15
    movq    %r9, %r11
    movq    %r15, 56(%rsp)
    movq    %r12, %r15
    andq    %rax, %r11
    orq %rax, %r15
    movq    %rsi, %r12
    xorq    %r14, %r11
    xorq    %r9, %r15
    rorx    $2, %r13, %r9
    rorx    $9, %r10, %r13
    movq    %r13, %rax
    andq    %rdi, %r12
    movq    %rdi, %r10
    notq    %rax
    xorq    %rdx, %r12
    orq %rdx, %r10
    movq    %rax, %r14
    xorq    %rax, %r10
    xorq    %rcx, %rbp
    andq    %rdx, %r14
    movq    32(%rsp), %rdx
    movq    %rsi, %rax
    xorq    %r9, %r14
    xorq    %r10, %rbp
    movq    %r10, 40(%rsp)
    xorq    %r14, %r8
    xorq    %rbx, %rdx
    xorq    88(%rsp), %rdx
    xorq    56(%rsp), %rdx
    xorq    %r12, %rdx
    orq %r9, %rax
    andq    %r9, %r13
    xorq    %rdi, %rax
    movq    176(%rsp), %r9
    xorq    %rsi, %r13
    movq    %rax, 80(%rsp)
    movq    160(%rsp), %rax
    rorx    $63, %rdx, %rsi
    xorq    104(%rsp), %rax
    xorq    %r8, %rsi
    rorx    $63, %rbp, %rdi
    xorq    %r13, %r9
    xorq    152(%rsp), %r9
    rorx    $63, %r8, %r8
    xorq    224(%rsp), %r9
    xorq    %r11, %rax
    xorq    208(%rsp), %rax
    xorq    80(%rsp), %rax
    xorq    %r15, %r9
    xorq    %r9, %rdi
    rorx    $63, %r9, %r9
    xorq    %rdx, %r9
    movq    200(%rsp), %rdx
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    movq    24(%rsp), %rax
    xorq    %rbp, %r10
    movl    $2147516555, %ebp
    xorq    %r9, %r11
    xorq    %r10, %rbx
    xorq    %rsi, %rdx
    rorx    $43, %r11, %r11
    xorq    %rdi, %rax
    rorx    $21, %rbx, %rbx
    rorx    $20, %rdx, %rdx
    xorq    %rax, %rbp
    xorq    %r8, %r13
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    rorx    $50, %r13, %r13
    orq %rdx, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    movq    %rbp, 64(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 184(%rsp)
    movq    104(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    orq %rax, %rbx
    movq    176(%rsp), %rax
    movq    %rbp, 128(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    xorq    %r10, %r12
    rorx    $36, %rdx, %rdx
    rorx    $3, %r12, %r12
    xorq    %rsi, %rcx
    xorq    %r8, %rax
    movq    %rbx, 112(%rsp)
    rorx    $19, %rcx, %rcx
    rorx    $44, %rax, %r11
    movq    48(%rsp), %rax
    movq    %r12, %rbx
    movq    %rcx, %r13
    notq    %rbx
    xorq    %r8, %r15
    orq %rcx, %rbx
    rorx    $56, %r15, %r15
    xorq    %rdi, %r14
    xorq    %rdi, %rax
    rorx    $46, %r14, %r14
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r12, %r11
    movq    %r13, 192(%rsp)
    movq    %rbp, 104(%rsp)
    movq    %r12, %rbp
    movq    %r11, 200(%rsp)
    orq %rdx, %rbp
    movq    32(%rsp), %rdx
    movq    %r15, %r13
    xorq    %rcx, %rbp
    movq    %r11, %rcx
    movq    72(%rsp), %r11
    movq    %rbp, 136(%rsp)
    movq    %rbp, %rax
    movq    208(%rsp), %rbp
    notq    %r15
    movq    %rbx, 48(%rsp)
    xorq    %r10, %rdx
    movq    %r15, %rbx
    xorq    %rsi, %r11
    rorx    $58, %rdx, %rdx
    xorq    %r9, %rbp
    andq    %r14, %rbx
    xorq    112(%rsp), %rax
    rorx    $39, %rbp, %rbp
    rorx    $63, %r11, %r11
    xorq    184(%rsp), %rcx
    movq    %rbp, %r12
    xorq    %rbp, %rbx
    andq    %rbp, %r13
    orq %rdx, %r12
    movq    %r14, %rbp
    xorq    %rdx, %r13
    xorq    %r11, %r12
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    144(%rsp), %r11
    movq    %r13, 168(%rsp)
    xorq    %r15, %rbp
    movq    120(%rsp), %r13
    movq    %r12, 32(%rsp)
    xorq    %r14, %rdx
    movq    152(%rsp), %r12
    movq    56(%rsp), %r14
    xorq    %rbp, %rax
    movq    %rdx, 208(%rsp)
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    movq    104(%rsp), %rdx
    xorq    24(%rsp), %rdx
    rorx    $54, %r11, %r11
    xorq    32(%rsp), %rdx
    xorq    %rdi, %r13
    movq    %rbp, 176(%rsp)
    xorq    %r8, %r12
    movq    80(%rsp), %rbp
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    andq    %r13, %r15
    rorx    $49, %r14, %r14
    xorq    %r12, %r15
    xorq    %r9, %rbp
    movq    %r15, 80(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    rorx    $8, %rbp, %rbp
    orq %r11, %r15
    notq    %r14
    xorq    %r13, %r15
    movq    %r14, 56(%rsp)
    orq %r12, %r13
    orq %rbp, %r14
    xorq    %rbp, %r13
    xorq    %r11, %r14
    movq    %rbp, %r11
    xorq    %r13, %rcx
    movq    %r14, 120(%rsp)
    andq    %r12, %r11
    xorq    56(%rsp), %r11
    movq    %r13, 56(%rsp)
    xorq    160(%rsp), %r9
    xorq    96(%rsp), %rdi
    xorq    224(%rsp), %r8
    xorq    88(%rsp), %r10
    xorq    40(%rsp), %rsi
    xorq    %r11, %rax
    rorx    $9, %r9, %r9
    rorx    $23, %rdi, %rdi
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    movq    %rdi, %r12
    notq    %rbp
    rorx    $2, %r10, %r10
    orq %r8, %r12
    movq    %rbp, %r14
    rorx    $62, %rsi, %rsi
    xorq    %rbp, %r12
    movq    168(%rsp), %rbp
    xorq    192(%rsp), %rbp
    andq    %r8, %r14
    movq    %r12, 40(%rsp)
    movq    %rsi, %r13
    xorq    %r10, %r14
    xorq    %r14, %rdx
    xorq    %r15, %rbp
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    64(%rsp), %rbp
    andq    %rdi, %r12
    orq %r10, %r13
    andq    %r9, %r10
    xorq    %r8, %r12
    movq    128(%rsp), %r8
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r13, %rax
    xorq    %r10, %rcx
    rorx    $63, %rbp, %rdi
    rorx    $63, %rax, %r9
    xorq    %r12, %r8
    xorq    48(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbp, %r9
    movabsq $-9223372036854775669, %rbp
    xorq    %rbx, %r8
    xorq    120(%rsp), %r8
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    192(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rcx, %r11
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    rorx    $43, %r11, %r11
    xorq    %rsi, %r8
    xorq    %rdx, %r10
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    xorq    %rdi, %rax
    xorq    %rax, %rbp
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    orq %r8, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r8, %rbp
    movq    %rbp, 72(%rsp)
    movq    %r10, %rbp
    andq    %r11, %rbp
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    orq %rax, %rbx
    movq    %rbp, 96(%rsp)
    xorq    %r11, %rbx
    andq    %rax, %r8
    movq    200(%rsp), %r11
    xorq    %r10, %r8
    movq    112(%rsp), %rax
    xorq    %rsi, %r15
    movq    %r8, 192(%rsp)
    movq    32(%rsp), %r8
    rorx    $19, %r15, %r15
    xorq    %rdx, %r11
    movq    %r15, %r10
    xorq    %r9, %r12
    xorq    %rcx, %rax
    rorx    $44, %r11, %r11
    rorx    $3, %r12, %r12
    xorq    %rdi, %r8
    rorx    $36, %rax, %rax
    movq    %rbx, 88(%rsp)
    rorx    $61, %r8, %r8
    movq    %r12, %rbx
    xorq    %rdi, %r14
    movq    %r8, %rbp
    andq    %r8, %r10
    notq    %rbx
    orq %r11, %rbp
    xorq    %r11, %r10
    andq    %rax, %r11
    xorq    %rax, %rbp
    xorq    %r12, %r11
    orq %r15, %rbx
    movq    %rbp, 112(%rsp)
    movq    %r12, %rbp
    movq    56(%rsp), %r12
    orq %rax, %rbp
    movq    176(%rsp), %rax
    xorq    %r8, %rbx
    movq    %r10, 152(%rsp)
    movq    64(%rsp), %r8
    xorq    %r15, %rbp
    xorq    %rdx, %r12
    movq    48(%rsp), %r10
    rorx    $46, %r14, %r14
    rorx    $56, %r12, %r12
    movq    %rbx, 32(%rsp)
    xorq    %rcx, %rax
    movq    %r12, %r15
    notq    %r12
    rorx    $39, %rax, %rax
    movq    %r12, %rbx
    xorq    %rsi, %r8
    xorq    %r9, %r10
    andq    %r14, %rbx
    rorx    $63, %r8, %r8
    movq    %r11, 160(%rsp)
    xorq    %rax, %rbx
    rorx    $58, %r10, %r10
    andq    %rax, %r15
    movq    %rax, %r11
    movq    %r14, %rax
    xorq    %r10, %r15
    orq %r10, %r11
    orq %r8, %rax
    movq    %rbp, 200(%rsp)
    xorq    %r8, %r11
    xorq    %r12, %rax
    andq    %r8, %r10
    movq    %r11, 48(%rsp)
    movq    %rax, 144(%rsp)
    movq    %rbp, %rax
    xorq    88(%rsp), %rax
    movq    168(%rsp), %rbp
    xorq    144(%rsp), %rax
    xorq    %r14, %r10
    movq    184(%rsp), %r8
    movq    %r10, 176(%rsp)
    movq    104(%rsp), %r10
    xorq    %rcx, %r13
    movq    120(%rsp), %r12
    xorq    %rsi, %rbp
    xorq    136(%rsp), %rcx
    rorx    $54, %rbp, %rbp
    xorq    %rdx, %r8
    rorx    $8, %r13, %r13
    xorq    %rdi, %r10
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    rorx    $28, %r10, %r10
    xorq    %r9, %r12
    xorq    80(%rsp), %rdi
    andq    %r10, %r11
    rorx    $49, %r12, %r12
    xorq    208(%rsp), %rdx
    xorq    %r8, %r11
    rorx    $9, %rcx, %rcx
    movq    %r15, 56(%rsp)
    movq    %r11, 120(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    rorx    $23, %rdi, %rdi
    orq %r13, %r14
    xorq    %r10, %r11
    rorx    $25, %rdx, %rdx
    xorq    %rbp, %r14
    movq    %r13, %rbp
    movq    %rdi, %r15
    andq    %r8, %rbp
    orq %rdx, %r15
    xorq    40(%rsp), %rsi
    xorq    %r12, %rbp
    movq    %r10, %r12
    movq    %rcx, %r10
    notq    %r10
    movq    %r14, 104(%rsp)
    xorq    128(%rsp), %r9
    xorq    %r10, %r15
    orq %r8, %r12
    movq    %r10, %r14
    movq    112(%rsp), %r8
    movq    56(%rsp), %r10
    rorx    $62, %rsi, %rsi
    xorq    24(%rsp), %r8
    xorq    152(%rsp), %r10
    andq    %rdx, %r14
    xorq    48(%rsp), %r8
    rorx    $2, %r9, %r9
    movq    %r15, 40(%rsp)
    xorq    120(%rsp), %r8
    xorq    %r9, %r14
    xorq    %r13, %r12
    xorq    %rbp, %rax
    movq    %rsi, %r13
    xorq    %r11, %r10
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    %r14, %r8
    xorq    72(%rsp), %r10
    andq    %rdi, %r15
    xorq    %rdx, %r15
    movq    96(%rsp), %rdx
    orq %r9, %r13
    andq    %rcx, %r9
    movq    160(%rsp), %rcx
    xorq    192(%rsp), %rcx
    xorq    176(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %rsi, %r9
    xorq    %r15, %rdx
    xorq    32(%rsp), %rdx
    xorq    %r13, %rax
    movq    %r13, 80(%rsp)
    rorx    $63, %rax, %r13
    rorx    $63, %r10, %rdi
    xorq    %r10, %r13
    xorq    %r12, %rcx
    xorq    %rbx, %rdx
    xorq    104(%rsp), %rdx
    xorq    %r9, %rcx
    rorx    $63, %rcx, %r10
    xorq    %rcx, %rdi
    movabsq $-9223372036854742903, %rcx
    xorq    %r13, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rsi
    rorx    $63, %r8, %rdx
    xorq    %r8, %rsi
    xorq    %rax, %rdx
    movq    24(%rsp), %r8
    movq    152(%rsp), %rax
    xorq    %r10, %rbp
    xorq    %rdx, %r9
    rorx    $43, %rbp, %rbp
    rorx    $50, %r9, %r9
    xorq    %rdi, %r8
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    movq    %rax, %rcx
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    orq %rbp, %rcx
    xorq    %rax, %rcx
    movq    %rcx, 64(%rsp)
    movq    %r9, %rcx
    andq    %rbp, %rcx
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    orq %r8, %rbx
    andq    %r8, %rax
    movq    48(%rsp), %r8
    xorq    %r9, %rax
    movq    %rcx, 128(%rsp)
    movq    88(%rsp), %rcx
    movq    %rax, 152(%rsp)
    movq    160(%rsp), %rax
    xorq    %rsi, %r11
    xorq    %rdi, %r8
    rorx    $19, %r11, %r11
    xorq    %rbp, %rbx
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    xorq    %r13, %r15
    xorq    %rdx, %rax
    movq    %r8, %rbp
    movq    %r11, %r9
    rorx    $44, %rax, %rax
    rorx    $36, %rcx, %rcx
    rorx    $3, %r15, %r15
    orq %rax, %rbp
    andq    %r8, %r9
    movq    %rbx, 184(%rsp)
    xorq    %rcx, %rbp
    xorq    %rax, %r9
    movq    %r15, %rbx
    movq    %rbp, 88(%rsp)
    movq    %r9, 136(%rsp)
    movq    %r15, %rbp
    movq    144(%rsp), %r9
    notq    %rbx
    orq %rcx, %rbp
    orq %r11, %rbx
    andq    %rcx, %rax
    movq    32(%rsp), %rcx
    xorq    %r8, %rbx
    xorq    %r15, %rax
    movq    72(%rsp), %r8
    movq    %rax, 168(%rsp)
    xorq    %r10, %r9
    movq    %rdx, %rax
    rorx    $39, %r9, %r9
    xorq    %r13, %rcx
    xorq    %r12, %rax
    xorq    %r11, %rbp
    rorx    $58, %rcx, %rcx
    xorq    %rsi, %r8
    rorx    $56, %rax, %rax
    movq    %r9, %r11
    rorx    $63, %r8, %r8
    xorq    %rdi, %r14
    orq %rcx, %r11
    movq    %rax, %r12
    rorx    $46, %r14, %r14
    xorq    %r8, %r11
    andq    %r9, %r12
    notq    %rax
    movq    %r11, 32(%rsp)
    xorq    %rcx, %r12
    movq    %rbx, 48(%rsp)
    movq    56(%rsp), %r11
    movq    %rax, %rbx
    movq    %r14, %r15
    movq    %r12, 144(%rsp)
    andq    %r14, %rbx
    orq %r8, %r15
    movq    112(%rsp), %r12
    andq    %rcx, %r8
    xorq    %r9, %rbx
    xorq    %rax, %r15
    xorq    %r14, %r8
    movq    192(%rsp), %rax
    movq    %r8, 224(%rsp)
    xorq    %rsi, %r11
    movq    24(%rsp), %r8
    xorq    88(%rsp), %r8
    rorx    $54, %r11, %r11
    xorq    32(%rsp), %r8
    xorq    %rdi, %r12
    movq    104(%rsp), %r14
    rorx    $28, %r12, %r12
    xorq    %rdx, %rax
    movq    %r11, %rcx
    rorx    $37, %rax, %rax
    movq    80(%rsp), %r9
    andq    %r12, %rcx
    xorq    %rax, %rcx
    xorq    %r13, %r14
    movq    %r15, 208(%rsp)
    movq    %rcx, 80(%rsp)
    xorq    %rcx, %r8
    rorx    $49, %r14, %r14
    movq    %r11, %rcx
    xorq    %r10, %r9
    movq    %rbp, 160(%rsp)
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r14, %r15
    movq    64(%rsp), %rbp
    xorq    136(%rsp), %rbp
    orq %r9, %r15
    xorq    144(%rsp), %rbp
    xorq    %r12, %rcx
    xorq    %r11, %r15
    movq    %r9, %r11
    movq    %r15, 56(%rsp)
    xorq    96(%rsp), %r13
    movq    %r12, %r15
    xorq    200(%rsp), %r10
    xorq    120(%rsp), %rdi
    orq %rax, %r15
    xorq    176(%rsp), %rdx
    xorq    40(%rsp), %rsi
    xorq    %r9, %r15
    andq    %rax, %r11
    xorq    %rcx, %rbp
    rorx    $2, %r13, %r9
    xorq    %r14, %r11
    rorx    $9, %r10, %r13
    rorx    $23, %rdi, %rdi
    movq    %r13, %rax
    rorx    $25, %rdx, %rdx
    movq    %rdi, %r10
    notq    %rax
    rorx    $62, %rsi, %rsi
    orq %rdx, %r10
    movq    %rax, %r14
    xorq    %rax, %r10
    movq    %rsi, %r12
    andq    %rdx, %r14
    xorq    %r10, %rbp
    movq    %rsi, %rax
    xorq    %r9, %r14
    movq    %r10, 40(%rsp)
    xorq    %r14, %r8
    andq    %rdi, %r12
    orq %r9, %rax
    xorq    %rdx, %r12
    movq    48(%rsp), %rdx
    xorq    %rdi, %rax
    movq    %rax, 96(%rsp)
    andq    %r9, %r13
    movq    160(%rsp), %rax
    movq    168(%rsp), %r9
    xorq    184(%rsp), %rax
    xorq    %rsi, %r13
    xorq    %rbx, %rdx
    xorq    128(%rsp), %rdx
    rorx    $63, %rbp, %rdi
    xorq    56(%rsp), %rdx
    xorq    %r13, %r9
    xorq    152(%rsp), %r9
    xorq    224(%rsp), %r9
    xorq    %r11, %rax
    xorq    208(%rsp), %rax
    xorq    96(%rsp), %rax
    xorq    %r12, %rdx
    rorx    $63, %rdx, %rsi
    xorq    %r8, %rsi
    xorq    %r15, %r9
    rorx    $63, %r8, %r8
    xorq    %r9, %rdi
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    rorx    $63, %r9, %r9
    movq    24(%rsp), %rax
    xorq    %rbp, %r10
    xorq    %rdx, %r9
    movq    136(%rsp), %rdx
    xorq    %r10, %rbx
    movabsq $-9223372036854743037, %rbp
    rorx    $21, %rbx, %rbx
    xorq    %r9, %r11
    xorq    %rdi, %rax
    rorx    $43, %r11, %r11
    xorq    %r8, %r13
    xorq    %rax, %rbp
    xorq    %rsi, %rdx
    rorx    $50, %r13, %r13
    movq    %rbp, 24(%rsp)
    rorx    $20, %rdx, %rdx
    movq    %rbx, %rbp
    orq %rdx, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %rdx, %rbp
    movq    %rbp, 72(%rsp)
    movq    %r13, %rbp
    andq    %r11, %rbp
    andq    %rax, %rdx
    xorq    %rsi, %rcx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    xorq    %r13, %rdx
    orq %rax, %rbx
    movq    168(%rsp), %rax
    movq    %rdx, 104(%rsp)
    xorq    %r11, %rbx
    movq    184(%rsp), %rdx
    movq    %rbp, 120(%rsp)
    xorq    %r10, %r12
    rorx    $19, %rcx, %rcx
    movq    %rbx, 112(%rsp)
    xorq    %r8, %rax
    rorx    $3, %r12, %r12
    movq    %rcx, %r13
    rorx    $44, %rax, %r11
    movq    32(%rsp), %rax
    xorq    %r9, %rdx
    rorx    $36, %rdx, %rdx
    movq    %r12, %rbx
    xorq    %r8, %r15
    notq    %rbx
    rorx    $56, %r15, %r15
    xorq    %rdi, %r14
    xorq    %rdi, %rax
    orq %rcx, %rbx
    rorx    $46, %r14, %r14
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r12, %r11
    movq    %r13, 192(%rsp)
    movq    %rbp, 184(%rsp)
    movq    %r12, %rbp
    movq    %rbx, 32(%rsp)
    orq %rdx, %rbp
    movq    %r15, %r13
    notq    %r15
    xorq    %rcx, %rbp
    movq    %r11, %rcx
    movq    %r15, %rbx
    movq    %rbp, 136(%rsp)
    movq    %rbp, %rax
    xorq    112(%rsp), %rax
    movq    %r11, 200(%rsp)
    movq    208(%rsp), %rbp
    andq    %r14, %rbx
    movq    48(%rsp), %rdx
    movq    64(%rsp), %r11
    xorq    104(%rsp), %rcx
    xorq    %r9, %rbp
    rorx    $39, %rbp, %rbp
    xorq    %r10, %rdx
    xorq    %rsi, %r11
    rorx    $58, %rdx, %rdx
    movq    %rbp, %r12
    rorx    $63, %r11, %r11
    orq %rdx, %r12
    andq    %rbp, %r13
    xorq    %r11, %r12
    xorq    %rdx, %r13
    xorq    %rbp, %rbx
    movq    %r14, %rbp
    andq    %r11, %rdx
    movq    %r13, 168(%rsp)
    orq %r11, %rbp
    movq    144(%rsp), %r11
    movq    88(%rsp), %r13
    xorq    %r15, %rbp
    xorq    %r14, %rdx
    movq    %r12, 48(%rsp)
    movq    152(%rsp), %r12
    movq    56(%rsp), %r14
    xorq    %rbp, %rax
    movq    %rbp, 176(%rsp)
    movq    %rdx, 208(%rsp)
    xorq    %rdx, %rcx
    movq    96(%rsp), %rbp
    xorq    %rsi, %r11
    movq    184(%rsp), %rdx
    xorq    24(%rsp), %rdx
    rorx    $54, %r11, %r11
    xorq    %rdi, %r13
    xorq    48(%rsp), %rdx
    xorq    %r8, %r12
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    xorq    %r9, %rbp
    andq    %r13, %r15
    xorq    160(%rsp), %r9
    xorq    %r12, %r15
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %r15, 96(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    xorq    224(%rsp), %r8
    xorq    80(%rsp), %rdi
    movq    %r14, 56(%rsp)
    orq %r11, %r15
    orq %rbp, %r14
    xorq    %r13, %r15
    xorq    %r11, %r14
    xorq    128(%rsp), %r10
    orq %r12, %r13
    rorx    $9, %r9, %r9
    movq    %rbp, %r11
    xorq    %rbp, %r13
    andq    %r12, %r11
    movq    %r9, %rbp
    xorq    56(%rsp), %r11
    notq    %rbp
    movq    %r14, 88(%rsp)
    rorx    $25, %r8, %r8
    rorx    $23, %rdi, %rdi
    movq    %rbp, %r14
    rorx    $2, %r10, %r10
    andq    %r8, %r14
    movq    %rdi, %r12
    xorq    40(%rsp), %rsi
    xorq    %r13, %rcx
    movq    %r13, 56(%rsp)
    xorq    %r11, %rax
    xorq    %r10, %r14
    orq %r8, %r12
    xorq    %rbp, %r12
    movq    168(%rsp), %rbp
    xorq    192(%rsp), %rbp
    movq    %r12, 40(%rsp)
    xorq    %r14, %rdx
    rorx    $62, %rsi, %rsi
    movq    %rsi, %r13
    xorq    %r15, %rbp
    orq %r10, %r13
    andq    %r9, %r10
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    72(%rsp), %rbp
    andq    %rdi, %r12
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r8, %r12
    movq    120(%rsp), %r8
    xorq    %r13, %rax
    xorq    %r10, %rcx
    rorx    $63, %rax, %r9
    rorx    $63, %rbp, %rdi
    xorq    %rbp, %r9
    movabsq $-9223372036854743038, %rbp
    xorq    %r12, %r8
    xorq    32(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbx, %r8
    xorq    88(%rsp), %r8
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    192(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rcx, %r11
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    rorx    $43, %r11, %r11
    xorq    %rsi, %r8
    xorq    %rdx, %r10
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    xorq    %rdi, %rax
    xorq    %rax, %rbp
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    orq %r8, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %rsi, %r15
    xorq    %r9, %r12
    xorq    %r8, %rbp
    andq    %rax, %r8
    rorx    $19, %r15, %r15
    movq    %rbp, 64(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 192(%rsp)
    movq    48(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    112(%rsp), %rax
    movq    %rbp, 80(%rsp)
    xorq    %r11, %rbx
    movq    200(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    rorx    $3, %r12, %r12
    movq    %rbx, 128(%rsp)
    xorq    %rcx, %rax
    movq    %r8, %rbp
    andq    %r8, %r10
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    movq    %r12, %rbx
    rorx    $44, %r11, %r11
    notq    %rbx
    xorq    %rdi, %r14
    orq %r11, %rbp
    xorq    %r11, %r10
    andq    %rax, %r11
    xorq    %rax, %rbp
    xorq    %r12, %r11
    movq    %r10, 152(%rsp)
    movq    %rbp, 112(%rsp)
    movq    %r12, %rbp
    movq    56(%rsp), %r12
    orq %rax, %rbp
    movq    176(%rsp), %rax
    movq    32(%rsp), %r10
    orq %r15, %rbx
    xorq    %r15, %rbp
    rorx    $46, %r14, %r14
    xorq    %r8, %rbx
    xorq    %rdx, %r12
    movq    72(%rsp), %r8
    xorq    %rcx, %rax
    rorx    $56, %r12, %r12
    xorq    %r9, %r10
    rorx    $39, %rax, %rax
    movq    %r12, %r15
    notq    %r12
    movq    %rbx, 48(%rsp)
    movq    %r11, 160(%rsp)
    xorq    %rsi, %r8
    rorx    $58, %r10, %r10
    movq    %rax, %r11
    movq    %r12, %rbx
    rorx    $63, %r8, %r8
    orq %r10, %r11
    andq    %rax, %r15
    andq    %r14, %rbx
    xorq    %r8, %r11
    xorq    %r10, %r15
    xorq    %rax, %rbx
    movq    %r14, %rax
    andq    %r8, %r10
    orq %r8, %rax
    xorq    %r14, %r10
    movq    %rbp, 200(%rsp)
    xorq    %r12, %rax
    movq    %r10, 176(%rsp)
    movq    184(%rsp), %r10
    movq    %rax, 144(%rsp)
    movq    %rbp, %rax
    movq    168(%rsp), %rbp
    movq    104(%rsp), %r8
    movq    88(%rsp), %r12
    xorq    %rcx, %r13
    xorq    %rdi, %r10
    movq    %r11, 32(%rsp)
    rorx    $8, %r13, %r13
    xorq    %rsi, %rbp
    rorx    $28, %r10, %r10
    xorq    136(%rsp), %rcx
    rorx    $54, %rbp, %rbp
    xorq    %rdx, %r8
    xorq    %r9, %r12
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    rorx    $49, %r12, %r12
    andq    %r10, %r11
    xorq    208(%rsp), %rdx
    xorq    120(%rsp), %r9
    xorq    %r8, %r11
    xorq    96(%rsp), %rdi
    rorx    $9, %rcx, %rcx
    movq    %r11, 88(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    xorq    128(%rsp), %rax
    orq %r13, %r14
    xorq    %r10, %r11
    xorq    144(%rsp), %rax
    xorq    %rbp, %r14
    movq    %r13, %rbp
    rorx    $25, %rdx, %rdx
    andq    %r8, %rbp
    movq    %r14, 104(%rsp)
    rorx    $23, %rdi, %rdi
    xorq    %r12, %rbp
    movq    %r10, %r12
    movq    %rcx, %r10
    orq %r8, %r12
    movq    112(%rsp), %r8
    xorq    24(%rsp), %r8
    xorq    32(%rsp), %r8
    notq    %r10
    rorx    $2, %r9, %r9
    xorq    88(%rsp), %r8
    movq    %r10, %r14
    movq    %r15, 56(%rsp)
    andq    %rdx, %r14
    movq    %rdi, %r15
    xorq    %r13, %r12
    xorq    %r9, %r14
    xorq    40(%rsp), %rsi
    xorq    %rbp, %rax
    xorq    %r14, %r8
    orq %rdx, %r15
    xorq    %r10, %r15
    movq    %r15, 40(%rsp)
    movq    56(%rsp), %r10
    rorx    $62, %rsi, %rsi
    xorq    152(%rsp), %r10
    movq    %rsi, %r13
    orq %r9, %r13
    andq    %rcx, %r9
    movq    160(%rsp), %rcx
    xorq    192(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %rsi, %r9
    xorq    176(%rsp), %rcx
    xorq    %r13, %rax
    movq    %r13, 96(%rsp)
    xorq    %r11, %r10
    rorx    $63, %rax, %r13
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    64(%rsp), %r10
    andq    %rdi, %r15
    xorq    %rdx, %r15
    movq    80(%rsp), %rdx
    xorq    %r12, %rcx
    xorq    %r9, %rcx
    xorq    %r10, %r13
    rorx    $63, %r10, %rdi
    rorx    $63, %rcx, %r10
    xorq    %r15, %rdx
    xorq    48(%rsp), %rdx
    xorq    %rcx, %rdi
    movabsq $-9223372036854775680, %rcx
    xorq    %rbx, %rdx
    xorq    104(%rsp), %rdx
    xorq    %r13, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rsi
    rorx    $63, %r8, %rdx
    xorq    %r8, %rsi
    xorq    %rax, %rdx
    movq    24(%rsp), %r8
    movq    152(%rsp), %rax
    xorq    %r10, %rbp
    xorq    %rdx, %r9
    rorx    $43, %rbp, %rbp
    rorx    $50, %r9, %r9
    xorq    %rdi, %r8
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    movq    %rax, %rcx
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    xorq    %rsi, %r11
    xorq    %r13, %r15
    orq %rbp, %rcx
    rorx    $19, %r11, %r11
    rorx    $3, %r15, %r15
    xorq    %rax, %rcx
    andq    %r8, %rax
    xorq    %rdi, %r14
    movq    %rcx, 72(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 152(%rsp)
    movq    160(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    32(%rsp), %r8
    movq    %rcx, 120(%rsp)
    movq    128(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 184(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $46, %r14, %r14
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    orq %r11, %rbx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rax, %r9
    andq    %rcx, %rax
    xorq    %rcx, %rbp
    movq    %r9, 136(%rsp)
    movq    144(%rsp), %r9
    movq    %rbp, 128(%rsp)
    movq    %r15, %rbp
    xorq    %r15, %rax
    orq %rcx, %rbp
    movq    48(%rsp), %rcx
    xorq    %r8, %rbx
    movq    %rax, 168(%rsp)
    movq    64(%rsp), %r8
    movq    %rdx, %rax
    xorq    %r10, %r9
    xorq    %r12, %rax
    xorq    %r11, %rbp
    rorx    $39, %r9, %r9
    xorq    %r13, %rcx
    rorx    $56, %rax, %rax
    rorx    $58, %rcx, %rcx
    xorq    %rsi, %r8
    movq    %r9, %r11
    movq    %rax, %r12
    rorx    $63, %r8, %r8
    orq %rcx, %r11
    andq    %r9, %r12
    movq    %r14, %r15
    xorq    %r8, %r11
    notq    %rax
    xorq    %rcx, %r12
    orq %r8, %r15
    andq    %rcx, %r8
    xorq    %rax, %r15
    movq    %rbx, 32(%rsp)
    xorq    %r14, %r8
    movq    %rbp, 160(%rsp)
    movq    %r11, 64(%rsp)
    movq    %r12, 48(%rsp)
    movq    %r15, 144(%rsp)
    movq    %rax, %rbx
    movq    %r8, 208(%rsp)
    movq    56(%rsp), %r11
    andq    %r14, %rbx
    movq    112(%rsp), %r12
    movq    192(%rsp), %rax
    xorq    %r9, %rbx
    movq    24(%rsp), %r8
    xorq    128(%rsp), %r8
    xorq    %rsi, %r11
    xorq    64(%rsp), %r8
    movq    104(%rsp), %r14
    rorx    $54, %r11, %r11
    xorq    %rdi, %r12
    xorq    %rdx, %rax
    rorx    $28, %r12, %r12
    movq    %r11, %rcx
    rorx    $37, %rax, %rax
    movq    96(%rsp), %r9
    andq    %r12, %rcx
    xorq    %r13, %r14
    xorq    %rax, %rcx
    rorx    $49, %r14, %r14
    xorq    80(%rsp), %r13
    movq    %rcx, 96(%rsp)
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    %r10, %r9
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r14, %r15
    xorq    200(%rsp), %r10
    orq %r9, %r15
    xorq    88(%rsp), %rdi
    xorq    40(%rsp), %rsi
    xorq    %r11, %r15
    xorq    176(%rsp), %rdx
    movq    72(%rsp), %rbp
    movq    %r15, 56(%rsp)
    movq    %r12, %r15
    xorq    136(%rsp), %rbp
    orq %rax, %r15
    xorq    48(%rsp), %rbp
    movq    %r9, %r11
    xorq    %r9, %r15
    rorx    $2, %r13, %r9
    rorx    $9, %r10, %r13
    andq    %rax, %r11
    movq    %r13, %rax
    rorx    $23, %rdi, %rdi
    notq    %rax
    xorq    %r12, %rcx
    xorq    %r14, %r11
    rorx    $25, %rdx, %rdx
    rorx    $62, %rsi, %rsi
    movq    %rax, %r14
    movq    %rdi, %r10
    xorq    %rcx, %rbp
    movq    %rsi, %r12
    andq    %rdx, %r14
    orq %rdx, %r10
    andq    %rdi, %r12
    xorq    %rax, %r10
    movq    %rsi, %rax
    xorq    %rdx, %r12
    orq %r9, %rax
    movq    32(%rsp), %rdx
    xorq    %r9, %r14
    xorq    %rdi, %rax
    andq    %r9, %r13
    movq    168(%rsp), %r9
    movq    %rax, 80(%rsp)
    movq    160(%rsp), %rax
    xorq    %rsi, %r13
    xorq    184(%rsp), %rax
    xorq    %rbx, %rdx
    xorq    120(%rsp), %rdx
    xorq    56(%rsp), %rdx
    xorq    %r13, %r9
    xorq    152(%rsp), %r9
    xorq    208(%rsp), %r9
    xorq    %r14, %r8
    xorq    %r10, %rbp
    movq    %r10, 40(%rsp)
    rorx    $63, %rbp, %rdi
    xorq    %r11, %rax
    xorq    144(%rsp), %rax
    xorq    80(%rsp), %rax
    xorq    %r12, %rdx
    rorx    $63, %rdx, %rsi
    xorq    %r15, %r9
    xorq    %r8, %rsi
    rorx    $63, %r8, %r8
    xorq    %r9, %rdi
    rorx    $63, %r9, %r9
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    movq    24(%rsp), %rax
    xorq    %rdx, %r9
    movq    136(%rsp), %rdx
    xorq    %rbp, %r10
    xorq    %r10, %rbx
    xorq    %r9, %r11
    xorq    %rdi, %rax
    rorx    $21, %rbx, %rbx
    rorx    $43, %r11, %r11
    movq    %rax, %rbp
    xorq    %rsi, %rdx
    xorq    %r8, %r13
    xorq    $32778, %rbp
    rorx    $20, %rdx, %rdx
    rorx    $50, %r13, %r13
    movq    %rbp, 88(%rsp)
    movq    %rbx, %rbp
    xorq    %rsi, %rcx
    orq %rdx, %rbp
    xorq    %rbp, 88(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $19, %rcx, %rcx
    xorq    %r10, %r12
    orq %r11, %rbp
    rorx    $3, %r12, %r12
    xorq    %r8, %r15
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    rorx    $56, %r15, %r15
    movq    %rbp, 24(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 192(%rsp)
    movq    184(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    movq    %rcx, %r13
    orq %rax, %rbx
    movq    168(%rsp), %rax
    movq    %rbp, 112(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    rorx    $36, %rdx, %rdx
    movq    %rbx, 104(%rsp)
    movq    %r12, %rbx
    xorq    %r8, %rax
    notq    %rbx
    rorx    $44, %rax, %r11
    movq    64(%rsp), %rax
    orq %rcx, %rbx
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r12, %r11
    movq    %r13, 136(%rsp)
    movq    %rbp, 184(%rsp)
    movq    %r12, %rbp
    movq    %r11, 168(%rsp)
    orq %rdx, %rbp
    movq    32(%rsp), %rdx
    movq    %r15, %r13
    xorq    %rcx, %rbp
    movq    %r11, %rcx
    movq    72(%rsp), %r11
    movq    %rbp, 200(%rsp)
    movq    %rbp, %rax
    movq    144(%rsp), %rbp
    xorq    104(%rsp), %rax
    xorq    192(%rsp), %rcx
    xorq    %r10, %rdx
    xorq    %rsi, %r11
    notq    %r15
    movq    %rbx, 64(%rsp)
    xorq    %r9, %rbp
    xorq    %rdi, %r14
    movq    %r15, %rbx
    rorx    $39, %rbp, %rbp
    rorx    $46, %r14, %r14
    rorx    $58, %rdx, %rdx
    andq    %r14, %rbx
    movq    %rbp, %r12
    rorx    $63, %r11, %r11
    xorq    %rbp, %rbx
    orq %rdx, %r12
    andq    %rbp, %r13
    movq    %r14, %rbp
    xorq    %r11, %r12
    xorq    %rdx, %r13
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    48(%rsp), %r11
    movq    %r13, 176(%rsp)
    movq    128(%rsp), %r13
    xorq    %r14, %rdx
    movq    %r12, 144(%rsp)
    movq    152(%rsp), %r12
    xorq    %r15, %rbp
    movq    %rdx, 232(%rsp)
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    movq    56(%rsp), %r14
    movq    184(%rsp), %rdx
    rorx    $54, %r11, %r11
    xorq    88(%rsp), %rdx
    movq    %rbp, 224(%rsp)
    xorq    %rbp, %rax
    xorq    144(%rsp), %rdx
    movq    80(%rsp), %rbp
    xorq    %rdi, %r13
    xorq    %r8, %r12
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    andq    %r13, %r15
    xorq    %r10, %r14
    xorq    %r12, %r15
    xorq    %r9, %rbp
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %r15, 80(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 32(%rsp)
    orq %rbp, %r14
    orq %r11, %r15
    xorq    %r11, %r14
    movq    %rbp, %r11
    xorq    %r13, %r15
    andq    %r12, %r11
    xorq    32(%rsp), %r11
    orq %r12, %r13
    xorq    %rbp, %r13
    movq    %r14, 128(%rsp)
    xorq    %r13, %rcx
    movq    %r13, 56(%rsp)
    xorq    %r11, %rax
    xorq    120(%rsp), %r10
    xorq    160(%rsp), %r9
    xorq    96(%rsp), %rdi
    xorq    208(%rsp), %r8
    xorq    40(%rsp), %rsi
    rorx    $9, %r9, %r9
    rorx    $2, %r10, %r10
    rorx    $23, %rdi, %rdi
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    movq    %rdi, %r12
    notq    %rbp
    rorx    $62, %rsi, %rsi
    orq %r8, %r12
    movq    %rbp, %r14
    movq    %rsi, %r13
    xorq    %rbp, %r12
    movq    176(%rsp), %rbp
    xorq    136(%rsp), %rbp
    movq    %r12, 40(%rsp)
    andq    %r8, %r14
    orq %r10, %r13
    xorq    %r10, %r14
    andq    %r9, %r10
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r14, %rdx
    xorq    %r13, %rax
    xorq    %r15, %rbp
    xorq    %r10, %rcx
    rorx    $63, %rax, %r9
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    24(%rsp), %rbp
    andq    %rdi, %r12
    xorq    %r8, %r12
    movq    112(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %rbp, %r9
    movabsq $-9223372034707292150, %rbp
    xorq    %r12, %r8
    xorq    64(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbx, %r8
    xorq    128(%rsp), %r8
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    136(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rax, %rdx
    movq    88(%rsp), %rax
    xorq    %rsi, %r8
    rorx    $20, %r8, %r8
    xorq    %rdi, %rax
    xorq    %r9, %rbx
    xorq    %rcx, %r11
    rorx    $21, %rbx, %rbx
    xorq    %rax, %rbp
    rorx    $43, %r11, %r11
    movq    %rbp, 72(%rsp)
    movq    %rbx, %rbp
    xorq    %rdx, %r10
    orq %r8, %rbp
    xorq    %rbp, 72(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $50, %r10, %r10
    xorq    %rsi, %r15
    orq %r11, %rbp
    rorx    $19, %r15, %r15
    xorq    %r9, %r12
    xorq    %r8, %rbp
    andq    %rax, %r8
    rorx    $3, %r12, %r12
    movq    %rbp, 32(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 88(%rsp)
    movq    144(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    104(%rsp), %rax
    movq    %rbp, 96(%rsp)
    xorq    %r11, %rbx
    movq    168(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    movq    %rbx, 120(%rsp)
    movq    %r12, %rbx
    xorq    %rcx, %rax
    movq    %r8, %rbp
    andq    %r8, %r10
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    notq    %rbx
    rorx    $44, %r11, %r11
    orq %r15, %rbx
    orq %r11, %rbp
    xorq    %r11, %r10
    andq    %rax, %r11
    xorq    %rax, %rbp
    xorq    %r12, %r11
    xorq    %r8, %rbx
    movq    %rbp, 104(%rsp)
    movq    %r12, %rbp
    movq    56(%rsp), %r12
    movq    %r10, 152(%rsp)
    orq %rax, %rbp
    movq    24(%rsp), %r8
    movq    64(%rsp), %r10
    movq    224(%rsp), %rax
    xorq    %r15, %rbp
    xorq    %rdx, %r12
    movq    %rbx, 48(%rsp)
    movq    %r11, 160(%rsp)
    rorx    $56, %r12, %r12
    xorq    %rsi, %r8
    movq    %rbp, 136(%rsp)
    xorq    %r9, %r10
    xorq    %rcx, %rax
    movq    %r12, %r15
    xorq    %rdi, %r14
    notq    %r12
    rorx    $39, %rax, %rax
    rorx    $46, %r14, %r14
    movq    %r12, %rbx
    rorx    $63, %r8, %r8
    andq    %r14, %rbx
    rorx    $58, %r10, %r10
    andq    %rax, %r15
    xorq    %rax, %rbx
    movq    %rax, %r11
    movq    %r14, %rax
    orq %r10, %r11
    orq %r8, %rax
    xorq    %r10, %r15
    xorq    %r8, %r11
    xorq    %r12, %rax
    andq    %r8, %r10
    movq    %r11, 56(%rsp)
    xorq    %r14, %r10
    movq    %rax, 168(%rsp)
    movq    %rbp, %rax
    movq    176(%rsp), %rbp
    movq    %r10, 208(%rsp)
    movq    184(%rsp), %r10
    movq    192(%rsp), %r8
    xorq    %rcx, %r13
    movq    128(%rsp), %r12
    rorx    $8, %r13, %r13
    xorq    200(%rsp), %rcx
    xorq    %rsi, %rbp
    xorq    120(%rsp), %rax
    movq    %r15, 144(%rsp)
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    %rdx, %r8
    rorx    $28, %r10, %r10
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    80(%rsp), %rdi
    xorq    %r8, %r11
    rorx    $49, %r12, %r12
    xorq    232(%rsp), %rdx
    movq    %r11, 128(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    xorq    168(%rsp), %rax
    orq %rbp, %r11
    orq %r13, %r14
    rorx    $23, %rdi, %rdi
    rorx    $9, %rcx, %rcx
    xorq    %rbp, %r14
    movq    %r13, %rbp
    xorq    %r10, %r11
    andq    %r8, %rbp
    rorx    $25, %rdx, %rdx
    movq    %rdi, %r15
    xorq    %r12, %rbp
    movq    %r10, %r12
    movq    %rcx, %r10
    orq %r8, %r12
    xorq    112(%rsp), %r9
    xorq    %rbp, %rax
    xorq    %r13, %r12
    notq    %r10
    xorq    40(%rsp), %rsi
    orq %rdx, %r15
    movq    %r14, 184(%rsp)
    movq    %r10, %r14
    xorq    %r10, %r15
    movq    144(%rsp), %r10
    xorq    152(%rsp), %r10
    movq    %r15, 40(%rsp)
    andq    %rdx, %r14
    rorx    $2, %r9, %r9
    rorx    $62, %rsi, %rsi
    xorq    %r9, %r14
    movq    104(%rsp), %r8
    movq    %rsi, %r13
    xorq    72(%rsp), %r8
    xorq    %r11, %r10
    orq %r9, %r13
    andq    %rcx, %r9
    xorq    %r15, %r10
    movq    %rsi, %r15
    movq    160(%rsp), %rcx
    andq    %rdi, %r15
    xorq    88(%rsp), %rcx
    xorq    56(%rsp), %r8
    xorq    %rdx, %r15
    movq    96(%rsp), %rdx
    xorq    208(%rsp), %rcx
    xorq    32(%rsp), %r10
    xorq    128(%rsp), %r8
    xorq    %rdi, %r13
    xorq    %r13, %rax
    xorq    %rsi, %r9
    movq    %r13, 80(%rsp)
    xorq    %r15, %rdx
    xorq    48(%rsp), %rdx
    rorx    $63, %rax, %r13
    xorq    %r12, %rcx
    xorq    %r9, %rcx
    xorq    %r10, %r13
    xorq    %r14, %r8
    rorx    $63, %r10, %rdi
    rorx    $63, %rcx, %r10
    xorq    %rbx, %rdx
    xorq    184(%rsp), %rdx
    xorq    %rcx, %rdi
    movabsq $-9223372034707259263, %rcx
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rsi
    rorx    $63, %r8, %rdx
    xorq    %r8, %rsi
    xorq    %rax, %rdx
    movq    72(%rsp), %r8
    movq    152(%rsp), %rax
    xorq    %rdi, %r8
    xorq    %r13, %rbx
    xorq    %r10, %rbp
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $21, %rbx, %rbx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    rorx    $43, %rbp, %rbp
    movq    %rax, %rcx
    xorq    %rdx, %r9
    xorq    %rsi, %r11
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    rorx    $50, %r9, %r9
    xorq    %r13, %r15
    orq %rbp, %rcx
    rorx    $19, %r11, %r11
    rorx    $3, %r15, %r15
    xorq    %rax, %rcx
    andq    %r8, %rax
    movq    %rcx, 64(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 152(%rsp)
    movq    160(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    56(%rsp), %r8
    movq    %rcx, 112(%rsp)
    movq    120(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 192(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    orq %r11, %rbx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rax, %r9
    xorq    %r8, %rbx
    xorq    %rcx, %rbp
    movq    %r9, 56(%rsp)
    movq    168(%rsp), %r8
    movq    %rbp, 120(%rsp)
    movq    %r15, %rbp
    andq    %rcx, %rax
    orq %rcx, %rbp
    movq    32(%rsp), %r9
    movq    48(%rsp), %rcx
    xorq    %r15, %rax
    xorq    %r11, %rbp
    movq    %rbx, 72(%rsp)
    movq    %rax, 160(%rsp)
    movq    %rdx, %rax
    movq    %rbp, 200(%rsp)
    xorq    %rsi, %r9
    xorq    %r13, %rcx
    xorq    %r10, %r8
    xorq    %r12, %rax
    rorx    $39, %r8, %r8
    rorx    $58, %rcx, %rcx
    rorx    $56, %rax, %rax
    xorq    %rdi, %r14
    rorx    $63, %r9, %r9
    movq    %rax, %r12
    notq    %rax
    rorx    $46, %r14, %r14
    andq    %r8, %r12
    movq    144(%rsp), %rbp
    movq    %r14, %r15
    xorq    %rcx, %r12
    orq %r9, %r15
    movq    %r8, %r11
    movq    %r12, 168(%rsp)
    movq    %rax, %r12
    xorq    %rax, %r15
    andq    %r14, %r12
    orq %rcx, %r11
    movq    88(%rsp), %rax
    xorq    %r8, %r12
    movq    104(%rsp), %r8
    xorq    %r9, %r11
    xorq    %rsi, %rbp
    andq    %rcx, %r9
    movq    184(%rsp), %rbx
    xorq    %r14, %r9
    rorx    $54, %rbp, %rbp
    xorq    %rdx, %rax
    xorq    %rdi, %r8
    movq    %r9, 224(%rsp)
    movq    %rbp, %r9
    rorx    $28, %r8, %r8
    rorx    $37, %rax, %rax
    movq    80(%rsp), %rcx
    andq    %r8, %r9
    xorq    %r13, %rbx
    movq    %r11, 48(%rsp)
    xorq    %rax, %r9
    rorx    $49, %rbx, %rbx
    movq    64(%rsp), %r14
    movq    %r9, 80(%rsp)
    movq    24(%rsp), %r9
    xorq    %r10, %rcx
    xorq    120(%rsp), %r9
    xorq    56(%rsp), %r14
    rorx    $8, %rcx, %rcx
    xorq    168(%rsp), %r14
    movq    %r15, 176(%rsp)
    xorq    %r11, %r9
    movq    %rbp, %r11
    xorq    80(%rsp), %r9
    orq %rbx, %r11
    notq    %rbx
    movq    %rbx, %r15
    xorq    %r8, %r11
    orq %rax, %r8
    orq %rcx, %r15
    xorq    %r11, %r14
    xorq    %rbp, %r15
    movq    %rcx, %rbp
    andq    %rax, %rbp
    movq    %r15, 88(%rsp)
    xorq    %rbx, %rbp
    xorq    %rcx, %r8
    xorq    96(%rsp), %r13
    xorq    136(%rsp), %r10
    xorq    128(%rsp), %rdi
    xorq    208(%rsp), %rdx
    xorq    40(%rsp), %rsi
    movq    %r8, 104(%rsp)
    rorx    $2, %r13, %rcx
    rorx    $23, %rdi, %rdi
    rorx    $9, %r10, %r13
    rorx    $25, %rdx, %rdx
    movq    %r13, %rax
    movq    %rdi, %rbx
    notq    %rax
    orq %rdx, %rbx
    rorx    $62, %rsi, %rsi
    xorq    %rax, %rbx
    movq    %rax, %r15
    movq    200(%rsp), %rax
    xorq    192(%rsp), %rax
    movq    %rsi, %r10
    movq    %rsi, %r8
    orq %rcx, %r10
    andq    %rdi, %r8
    andq    %rdx, %r15
    xorq    %rdi, %r10
    xorq    %rdx, %r8
    movq    72(%rsp), %rdx
    movq    %r10, 96(%rsp)
    andq    %rcx, %r13
    xorq    %rbx, %r14
    xorq    %rbp, %rax
    xorq    176(%rsp), %rax
    xorq    %rsi, %r13
    xorq    %r12, %rdx
    xorq    112(%rsp), %rdx
    xorq    %rcx, %r15
    xorq    88(%rsp), %rdx
    rorx    $63, %r14, %rdi
    xorq    %r15, %r9
    movq    %rbx, 40(%rsp)
    xorq    %r10, %rax
    movq    160(%rsp), %r10
    rorx    $63, %rax, %rbx
    xorq    %r8, %rdx
    xorq    %r14, %rbx
    movabsq $-9223372036854742912, %r14
    xorq    %r13, %r10
    xorq    152(%rsp), %r10
    rorx    $63, %rdx, %rcx
    xorq    224(%rsp), %r10
    xorq    %r9, %rcx
    rorx    $63, %r9, %r9
    xorq    104(%rsp), %r10
    xorq    %r10, %rdi
    rorx    $63, %r10, %r10
    xorq    %rdx, %r10
    movq    56(%rsp), %rdx
    xorq    %rax, %r9
    movq    24(%rsp), %rax
    xorq    %rbx, %r12
    xorq    %r10, %rbp
    rorx    $21, %r12, %r12
    rorx    $43, %rbp, %rbp
    xorq    %r9, %r13
    xorq    %rcx, %rdx
    movq    %r12, %rsi
    rorx    $50, %r13, %r13
    xorq    %rdi, %rax
    rorx    $20, %rdx, %rdx
    xorq    %rcx, %r11
    orq %rdx, %rsi
    xorq    %rax, %r14
    xorq    %rbx, %r8
    xorq    %rsi, %r14
    movq    %r12, %rsi
    rorx    $19, %r11, %r11
    notq    %rsi
    movq    %r14, 24(%rsp)
    movq    %r13, %r14
    orq %rbp, %rsi
    andq    %rbp, %r14
    rorx    $3, %r8, %r8
    xorq    %rdx, %rsi
    andq    %rax, %rdx
    xorq    %r12, %r14
    movq    %rsi, 32(%rsp)
    movq    %r13, %rsi
    xorq    %r13, %rdx
    orq %rax, %rsi
    movq    160(%rsp), %rax
    movq    %rdx, 184(%rsp)
    xorq    %rbp, %rsi
    movq    192(%rsp), %rdx
    movq    %r14, 128(%rsp)
    movq    %rsi, 56(%rsp)
    movq    %r11, %r12
    movq    %r8, %r13
    xorq    %r9, %rax
    movq    %r8, %r14
    notq    %r13
    rorx    $44, %rax, %rsi
    movq    48(%rsp), %rax
    xorq    %r10, %rdx
    rorx    $36, %rdx, %rdx
    orq %r11, %r13
    orq %rdx, %r14
    xorq    %rdi, %rax
    xorq    %r11, %r14
    rorx    $61, %rax, %rax
    movq    %r14, 160(%rsp)
    andq    %rax, %r12
    movq    %rax, %rbp
    xorq    %rax, %r13
    xorq    %rsi, %r12
    orq %rsi, %rbp
    andq    %rdx, %rsi
    xorq    %rdx, %rbp
    movq    %r14, %rax
    xorq    56(%rsp), %rax
    xorq    %r8, %rsi
    movq    %rbp, 192(%rsp)
    movq    %r12, 136(%rsp)
    movq    %r13, 48(%rsp)
    movq    %rsi, 144(%rsp)
    xorq    %rdi, %r15
    movq    176(%rsp), %r12
    movq    72(%rsp), %rdx
    rorx    $46, %r15, %r15
    movq    104(%rsp), %r8
    movq    64(%rsp), %rbp
    movq    %r15, %r14
    xorq    184(%rsp), %rsi
    xorq    %r10, %r12
    xorq    %rbx, %rdx
    rorx    $39, %r12, %r12
    xorq    %r9, %r8
    xorq    %rcx, %rbp
    rorx    $58, %rdx, %rdx
    rorx    $56, %r8, %r8
    movq    %r12, %r11
    rorx    $63, %rbp, %rbp
    orq %rdx, %r11
    movq    %r8, %r13
    xorq    %rbp, %r11
    andq    %r12, %r13
    notq    %r8
    orq %rbp, %r14
    movq    %r11, 72(%rsp)
    xorq    %rdx, %r13
    xorq    %r8, %r14
    movq    %r8, %r11
    movq    168(%rsp), %r8
    movq    %r13, 104(%rsp)
    andq    %r15, %r11
    movq    120(%rsp), %r13
    andq    %rbp, %rdx
    xorq    %r12, %r11
    movq    152(%rsp), %r12
    xorq    %r15, %rdx
    movq    %r14, 176(%rsp)
    xorq    %r14, %rax
    movq    %rdx, 208(%rsp)
    xorq    %rdx, %rsi
    xorq    %rcx, %r8
    movq    88(%rsp), %r14
    movq    192(%rsp), %rdx
    rorx    $54, %r8, %r8
    xorq    24(%rsp), %rdx
    movq    96(%rsp), %rbp
    xorq    %rdi, %r13
    xorq    72(%rsp), %rdx
    rorx    $28, %r13, %r13
    xorq    %r9, %r12
    movq    %r8, %r15
    rorx    $37, %r12, %r12
    xorq    %rbx, %r14
    andq    %r13, %r15
    xorq    %r10, %rbp
    rorx    $49, %r14, %r14
    xorq    %r12, %r15
    rorx    $8, %rbp, %rbp
    movq    %r15, 96(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    orq %r8, %r15
    movq    %r14, 64(%rsp)
    orq %rbp, %r14
    xorq    %r13, %r15
    xorq    %r8, %r14
    movq    %rbp, %r8
    andq    %r12, %r8
    xorq    200(%rsp), %r10
    xorq    40(%rsp), %rcx
    xorq    80(%rsp), %rdi
    xorq    224(%rsp), %r9
    orq %r13, %r12
    xorq    112(%rsp), %rbx
    movq    %r14, 120(%rsp)
    xorq    %rbp, %r12
    xorq    64(%rsp), %r8
    xorq    %r12, %rsi
    rorx    $9, %r10, %r10
    rorx    $62, %rcx, %r14
    movq    %r10, %rcx
    rorx    $23, %rdi, %r13
    rorx    $25, %r9, %r9
    notq    %rcx
    rorx    $2, %rbx, %rbx
    movq    %r13, %rbp
    movq    %rcx, %rdi
    orq %r9, %rbp
    xorq    %r8, %rax
    andq    %r9, %rdi
    xorq    %rcx, %rbp
    movq    128(%rsp), %rcx
    xorq    %rbx, %rdi
    movq    %rbp, 80(%rsp)
    movq    %rdi, 40(%rsp)
    xorq    %rdi, %rdx
    movq    104(%rsp), %rdi
    xorq    136(%rsp), %rdi
    xorq    %r15, %rdi
    xorq    %rbp, %rdi
    movq    %r14, %rbp
    xorq    32(%rsp), %rdi
    andq    %r13, %rbp
    xorq    %r9, %rbp
    movq    %r14, %r9
    xorq    %rbp, %rcx
    xorq    48(%rsp), %rcx
    orq %rbx, %r9
    andq    %r10, %rbx
    xorq    %r13, %r9
    xorq    %r14, %rbx
    rorx    $63, %rdi, %r14
    xorq    %r9, %rax
    xorq    %rbx, %rsi
    rorx    $63, %rax, %r13
    xorq    %r11, %rcx
    xorq    120(%rsp), %rcx
    xorq    %rsi, %r14
    movq    %r9, 88(%rsp)
    movq    24(%rsp), %r9
    rorx    $63, %rsi, %rsi
    rorx    $63, %rcx, %r10
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rdx
    xorq    %rdi, %r13
    xorq    %rax, %rdx
    movq    136(%rsp), %rax
    xorq    %rcx, %rsi
    xorq    %r14, %r9
    xorq    %r13, %r11
    movl    $2147483649, %ecx
    rorx    $21, %r11, %r11
    xorq    %r9, %rcx
    xorq    %rdx, %rbx
    xorq    %r10, %rax
    rorx    $50, %rbx, %rbx
    movq    %rcx, 24(%rsp)
    rorx    $20, %rax, %rax
    xorq    %rsi, %r8
    movq    %r11, %rcx
    movq    %r11, %rdi
    rorx    $43, %r8, %r8
    orq %rax, %rcx
    notq    %rdi
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    orq %r8, %rdi
    andq    %r8, %rcx
    xorq    %r13, %rbp
    xorq    %rax, %rdi
    xorq    %r11, %rcx
    andq    %r9, %rax
    xorq    %rbx, %rax
    movq    %rdi, 64(%rsp)
    movq    %rbx, %rdi
    movq    %rcx, 112(%rsp)
    orq %r9, %rdi
    rorx    $3, %rbp, %rbp
    movq    72(%rsp), %rcx
    movq    144(%rsp), %r9
    xorq    %r10, %r15
    movq    %rax, 136(%rsp)
    movq    56(%rsp), %rax
    xorq    %r8, %rdi
    rorx    $19, %r15, %r15
    movq    %rbp, %rbx
    movq    %rdi, 152(%rsp)
    xorq    %r14, %rcx
    xorq    %rdx, %r9
    movq    %r15, %r11
    rorx    $61, %rcx, %rcx
    xorq    %rsi, %rax
    rorx    $44, %r9, %r9
    rorx    $36, %rax, %rax
    movq    %rcx, %r8
    movq    %rbp, %rdi
    notq    %rbx
    andq    %rcx, %r11
    orq %rax, %rdi
    orq %r15, %rbx
    orq %r9, %r8
    xorq    %r15, %rdi
    xorq    %rax, %r8
    xorq    %rcx, %rbx
    xorq    %r9, %r11
    andq    %rax, %r9
    movq    176(%rsp), %rax
    movq    %rdi, 144(%rsp)
    movq    48(%rsp), %rdi
    movq    32(%rsp), %rcx
    xorq    %rdx, %r12
    movq    %r8, 56(%rsp)
    movq    40(%rsp), %r8
    xorq    %rbp, %r9
    xorq    %rsi, %rax
    rorx    $56, %r12, %r12
    movq    %r9, 168(%rsp)
    rorx    $39, %rax, %rax
    xorq    %r13, %rdi
    movq    %r12, %r9
    xorq    %r10, %rcx
    rorx    $58, %rdi, %rdi
    xorq    %r14, %r8
    andq    %rax, %r9
    rorx    $63, %rcx, %rcx
    rorx    $46, %r8, %r8
    xorq    %rdi, %r9
    movq    %rbx, 72(%rsp)
    notq    %r12
    movq    120(%rsp), %rbx
    movq    %rax, %rbp
    movq    %r11, 200(%rsp)
    orq %rdi, %rbp
    movq    %r9, 48(%rsp)
    andq    %rcx, %rdi
    movq    104(%rsp), %r9
    movq    %r12, %r11
    movq    %r8, %r15
    xorq    %rcx, %rbp
    xorq    %r8, %rdi
    andq    %r8, %r11
    orq %rcx, %r15
    movq    192(%rsp), %r8
    movq    88(%rsp), %rcx
    movq    %rdi, 224(%rsp)
    xorq    %r13, %rbx
    movq    184(%rsp), %rdi
    xorq    %rax, %r11
    xorq    %r10, %r9
    rorx    $49, %rbx, %rbx
    rorx    $54, %r9, %r9
    movq    %r11, 40(%rsp)
    xorq    %r14, %r8
    xorq    %rsi, %rcx
    movq    %rbx, %r11
    notq    %rbx
    xorq    %r12, %r15
    rorx    $8, %rcx, %rcx
    movq    %rbp, 32(%rsp)
    xorq    %rdx, %rdi
    rorx    $28, %r8, %r8
    movq    %r9, %rbp
    movq    %rbx, %r12
    rorx    $37, %rdi, %rdi
    movq    144(%rsp), %rax
    orq %rcx, %r12
    xorq    152(%rsp), %rax
    andq    %r8, %rbp
    xorq    %rdi, %rbp
    xorq    %r9, %r12
    movq    %r15, 176(%rsp)
    movq    %rbp, 120(%rsp)
    movq    %r12, 88(%rsp)
    movq    %rcx, %r12
    andq    %rdi, %r12
    orq %r9, %r11
    movq    112(%rsp), %r9
    xorq    %rbx, %r12
    xorq    %r15, %rax
    xorq    %r8, %r11
    xorq    %r12, %rax
    orq %rdi, %r8
    xorq    160(%rsp), %rsi
    xorq    96(%rsp), %r14
    xorq    208(%rsp), %rdx
    xorq    %rcx, %r8
    xorq    80(%rsp), %r10
    movq    48(%rsp), %rbx
    xorq    200(%rsp), %rbx
    xorq    128(%rsp), %r13
    rorx    $9, %rsi, %rbp
    movq    %r8, 104(%rsp)
    movq    56(%rsp), %rdi
    rorx    $23, %r14, %r14
    rorx    $25, %rdx, %rdx
    movq    %rbp, %rsi
    movq    %r14, %r15
    notq    %rsi
    rorx    $62, %r10, %rcx
    orq %rdx, %r15
    xorq    %r11, %rbx
    rorx    $2, %r13, %r13
    xorq    %rsi, %r15
    movq    %rcx, %r8
    xorq    24(%rsp), %rdi
    movq    %r15, 96(%rsp)
    xorq    %r15, %rbx
    movq    %rcx, %r15
    andq    %r14, %r8
    orq %r13, %r15
    xorq    32(%rsp), %rdi
    xorq    %r14, %r15
    xorq    %rdx, %r8
    movq    168(%rsp), %r14
    xorq    136(%rsp), %r14
    xorq    %r8, %r9
    xorq    72(%rsp), %r9
    xorq    224(%rsp), %r14
    xorq    64(%rsp), %rbx
    movq    %rsi, %r10
    xorq    40(%rsp), %r9
    xorq    120(%rsp), %rdi
    andq    %rdx, %r10
    xorq    104(%rsp), %r14
    xorq    88(%rsp), %r9
    andq    %r13, %rbp
    xorq    %r13, %r10
    xorq    %rcx, %rbp
    xorq    %r15, %rax
    rorx    $63, %rbx, %rsi
    rorx    $63, %rax, %rcx
    movq    40(%rsp), %r13
    xorq    %r10, %rdi
    xorq    %rbp, %r14
    rorx    $63, %r9, %rdx
    xorq    %rbx, %rcx
    xorq    %r14, %rsi
    rorx    $63, %r14, %r14
    xorq    %rdi, %rdx
    xorq    %r9, %r14
    rorx    $63, %rdi, %rdi
    movq    152(%rsp), %r9
    xorq    %rax, %rdi
    movq    24(%rsp), %rax
    xorq    %rcx, %r8
    rorx    $3, %r8, %r8
    movq    32(%rsp), %rbx
    xorq    %rcx, %r13
    xorq    %r14, %r9
    rorx    $21, %r13, %r13
    xorq    %r14, %r12
    xorq    %rsi, %rax
    rorx    $36, %r9, %r9
    xorq    %rdi, %rbp
    movq    %rax, 24(%rsp)
    movq    %r9, 40(%rsp)
    xorq    %rsi, %rbx
    movq    200(%rsp), %rax
    movq    168(%rsp), %r9
    xorq    %rdx, %r11
    rorx    $43, %r12, %r12
    rorx    $50, %rbp, %rbp
    rorx    $61, %rbx, %rbx
    rorx    $19, %r11, %r11
    xorq    %rdx, %rax
    xorq    %rdi, %r9
    rorx    $20, %rax, %rax
    rorx    $44, %r9, %r9
    movq    %rax, 80(%rsp)
    movq    %r9, 128(%rsp)
    movabsq $-9223372034707259384, %rax
    xorq    24(%rsp), %rax
    movq    %r8, 32(%rsp)
    movq    64(%rsp), %r8
    movq    136(%rsp), %r9
    xorq    %rdx, %r8
    xorq    %rdi, %r9
    rorx    $63, %r8, %r8
    rorx    $37, %r9, %r9
    movq    %r8, 64(%rsp)
    movq    72(%rsp), %r8
    movq    %r9, 192(%rsp)
    movq    56(%rsp), %r9
    xorq    %rcx, %r8
    rorx    $58, %r8, %r8
    xorq    %rsi, %r9
    movq    %r8, 72(%rsp)
    movq    176(%rsp), %r8
    rorx    $28, %r9, %r9
    xorq    %r14, %r8
    rorx    $39, %r8, %r8
    movq    %r8, 184(%rsp)
    movq    104(%rsp), %r8
    xorq    %rdi, %r8
    xorq    224(%rsp), %rdi
    rorx    $56, %r8, %r8
    movq    %r8, 152(%rsp)
    movq    %rsi, %r8
    xorq    120(%rsp), %rsi
    xorq    %r10, %r8
    movq    48(%rsp), %r10
    rorx    $46, %r8, %r8
    rorx    $25, %rdi, %rdi
    movq    %r8, 104(%rsp)
    movq    152(%rsp), %r8
    xorq    %rdx, %r10
    xorq    96(%rsp), %rdx
    rorx    $23, %rsi, %rsi
    rorx    $54, %r10, %r10
    notq    %r8
    movq    %r8, 200(%rsp)
    movq    88(%rsp), %r8
    rorx    $62, %rdx, %rdx
    xorq    %rcx, %r8
    xorq    112(%rsp), %rcx
    rorx    $49, %r8, %r8
    movq    %r8, 48(%rsp)
    movq    %r14, %r8
    xorq    144(%rsp), %r14
    xorq    %r15, %r8
    movq    48(%rsp), %r15
    rorx    $8, %r8, %r8
    rorx    $2, %rcx, %rcx
    notq    %r15
    rorx    $9, %r14, %r14
    movq    %r15, 88(%rsp)
    movq    80(%rsp), %r15
    movq    %r14, 56(%rsp)
    notq    %r14
    orq %r13, %r15
    xorq    %r15, %rax
    movq    216(%rsp), %r15
    movq    %rax, (%r15)
    movq    %r13, %rax
    notq    %rax
    orq %r12, %rax
    xorq    80(%rsp), %rax
    movq    %rax, 8(%r15)
    movq    %rbp, %rax
    andq    %r12, %rax
    xorq    %r13, %rax
    movq    %rax, 16(%r15)
    movq    24(%rsp), %rax
    orq %rbp, %rax
    xorq    %r12, %rax
    movq    %rax, 24(%r15)
    movq    80(%rsp), %rax
    andq    24(%rsp), %rax
    xorq    %rbp, %rax
    movq    %rax, 32(%r15)
    movq    128(%rsp), %rax
    orq %rbx, %rax
    xorq    40(%rsp), %rax
    movq    %rax, 40(%r15)
    movq    %r11, %rax
    andq    %rbx, %rax
    xorq    128(%rsp), %rax
    movq    %rax, 48(%r15)
    movq    32(%rsp), %rax
    notq    %rax
    orq %r11, %rax
    xorq    %rbx, %rax
    movq    %rax, 56(%r15)
    movq    32(%rsp), %rax
    orq 40(%rsp), %rax
    xorq    %r11, %rax
    movq    %rax, 64(%r15)
    movq    128(%rsp), %rax
    andq    40(%rsp), %rax
    xorq    32(%rsp), %rax
    movq    %rax, 72(%r15)
    movq    184(%rsp), %rax
    orq 72(%rsp), %rax
    xorq    64(%rsp), %rax
    movq    %rax, 80(%r15)
    movq    152(%rsp), %rax
    andq    184(%rsp), %rax
    xorq    72(%rsp), %rax
    movq    %rax, 88(%r15)
    movq    200(%rsp), %rax
    andq    104(%rsp), %rax
    xorq    184(%rsp), %rax
    movq    %rax, 96(%r15)
    movq    104(%rsp), %rax
    orq 64(%rsp), %rax
    xorq    200(%rsp), %rax
    movq    %rax, 104(%r15)
    movq    64(%rsp), %rax
    andq    72(%rsp), %rax
    xorq    104(%rsp), %rax
    movq    %rax, 112(%r15)
    movq    %r10, %rax
    andq    %r9, %rax
    xorq    192(%rsp), %rax
    movq    %rax, 120(%r15)
    movq    48(%rsp), %rax
    orq %r10, %rax
    xorq    %r9, %rax
    movq    %rax, 128(%r15)
    movq    88(%rsp), %rax
    orq %r8, %rax
    orq 192(%rsp), %r9
    xorq    %r10, %rax
    movq    %rax, 136(%r15)
    movq    192(%rsp), %rax
    xorq    %r8, %r9
    andq    %r8, %rax
    xorq    88(%rsp), %rax
    movq    %r9, 152(%r15)
    movq    %rax, 144(%r15)
    movq    %r14, %rax
    andq    %rdi, %rax
    xorq    %rcx, %rax
    movq    %rax, 160(%r15)
    movq    %rsi, %rax
    orq %rdi, %rax
    xorq    %r14, %rax
    movq    %rax, 168(%r15)
    movq    %rdx, %rax
    andq    %rsi, %rax
    xorq    %rdi, %rax
    movq    %rax, 176(%r15)
    movq    %rdx, %rax
    orq %rcx, %rax
    andq    56(%rsp), %rcx
    xorq    %rsi, %rax
    movq    %rax, 184(%r15)
    xorq    %rdx, %rcx
    movq    %rcx, 192(%r15)
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
    subq    $232, %rsp
    .cfi_def_cfa_offset 288
    movq    (%rdi), %r13
    movq    8(%rdi), %rax
    movq    %rdi, 96(%rsp)
    movq    16(%rdi), %rdx
    movq    24(%rdi), %rcx
    movq    32(%rdi), %rbx
    movq    40(%rdi), %rsi
    movq    48(%rdi), %r11
    movq    56(%rdi), %rdi
    movq    %rax, 16(%rsp)
    movq    %rdx, 8(%rsp)
    movq    %rbx, (%rsp)
    movq    %rsi, -8(%rsp)
    movq    %rdi, -16(%rsp)
    movq    96(%rsp), %rdi
    movq    64(%rdi), %rdi
    movq    %rdi, -24(%rsp)
    movq    96(%rsp), %rdi
    movq    104(%rdi), %r9
    movq    80(%rdi), %rbp
    movq    88(%rdi), %r8
    movq    112(%rdi), %r10
    movq    72(%rdi), %r15
    movq    %rbp, -32(%rsp)
    movq    %r9, -48(%rsp)
    movq    96(%rdi), %rbp
    movq    %r8, -40(%rsp)
    movq    %r10, -56(%rsp)
    movq    136(%rdi), %rax
    movq    160(%rdi), %rsi
    movq    120(%rdi), %r12
    movq    128(%rdi), %r14
    movq    144(%rdi), %rbx
    movq    152(%rdi), %rdx
    movq    168(%rdi), %rdi
    movq    %rax, -80(%rsp)
    movq    -8(%rsp), %r8
    movq    %r14, -72(%rsp)
    movq    16(%rsp), %r14
    movq    %rdi, -104(%rsp)
    movq    96(%rsp), %rdi
    movq    %rdx, -88(%rsp)
    movq    -16(%rsp), %rdx
    xorq    %r13, %r8
    xorq    8(%rsp), %rdx
    xorq    -32(%rsp), %r8
    xorq    %r11, %r14
    movq    176(%rdi), %rdi
    xorq    -40(%rsp), %r14
    movq    %r12, -64(%rsp)
    xorq    -72(%rsp), %r14
    xorq    -64(%rsp), %r8
    xorq    -104(%rsp), %r14
    xorq    %rbp, %rdx
    movq    %rdi, -112(%rsp)
    movq    96(%rsp), %rdi
    xorq    %rax, %rdx
    movq    -24(%rsp), %rax
    xorq    -112(%rsp), %rdx
    movq    %rsi, -96(%rsp)
    movq    184(%rdi), %rdi
    xorq    %rsi, %r8
    xorq    %rcx, %rax
    xorq    %r9, %rax
    movq    (%rsp), %r9
    rorx    $63, %rdx, %rsi
    movq    %rdi, -120(%rsp)
    movq    96(%rsp), %rdi
    xorq    %rbx, %rax
    xorq    -120(%rsp), %rax
    xorq    %r8, %rsi
    rorx    $63, %r8, %r8
    xorq    %r15, %r9
    xorq    %rsi, %r11
    xorq    %r10, %r9
    xorq    -88(%rsp), %r9
    movq    192(%rdi), %r12
    rorx    $63, %r14, %rdi
    rorx    $20, %r11, %r11
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    xorq    %r14, %r10
    xorq    %r12, %r9
    xorq    %r8, %r12
    xorq    %r10, %rbp
    xorq    %r9, %rdi
    rorx    $63, %r9, %r9
    rorx    $50, %r12, %rax
    xorq    %rdx, %r9
    movq    %rdi, %rdx
    movl    $2147516555, %r12d
    xorq    %r13, %rdx
    rorx    $21, %rbp, %rbp
    xorq    %r9, %rbx
    xorq    %rdx, %r12
    rorx    $43, %rbx, %rbx
    movq    %rax, %r14
    movq    %r12, 24(%rsp)
    movq    %rbp, %r12
    movq    %rbp, %r13
    orq %r11, %r12
    xorq    %r12, 24(%rsp)
    andq    %rbx, %r14
    xorq    %rbp, %r14
    notq    %r13
    movq    %rax, %rbp
    orq %rbx, %r13
    orq %rdx, %rbp
    xorq    %r8, %r15
    xorq    %rbx, %rbp
    xorq    %r11, %r13
    movq    -32(%rsp), %rbx
    andq    %rdx, %r11
    movq    -112(%rsp), %rdx
    xorq    %r9, %rcx
    xorq    %rax, %r11
    movq    -72(%rsp), %rax
    rorx    $36, %rcx, %rcx
    xorq    %rdi, %rbx
    movq    %r11, 72(%rsp)
    rorx    $44, %r15, %r11
    rorx    $61, %rbx, %rbx
    xorq    %r10, %rdx
    movq    %r13, 104(%rsp)
    xorq    %rsi, %rax
    rorx    $3, %rdx, %rdx
    movq    %rbx, %r12
    rorx    $19, %rax, %rax
    orq %r11, %r12
    movq    %rdx, %r15
    movq    %rax, %r13
    xorq    %rcx, %r12
    movq    %r14, 48(%rsp)
    andq    %rbx, %r13
    orq %rcx, %r15
    movq    %rdx, %r14
    movq    %r12, 80(%rsp)
    xorq    %r11, %r13
    xorq    %rax, %r15
    andq    %rcx, %r11
    movq    -48(%rsp), %r12
    notq    %r14
    xorq    %rdx, %r11
    movq    %r13, 64(%rsp)
    orq %rax, %r14
    movq    -16(%rsp), %rdx
    movq    -88(%rsp), %r13
    movq    %r15, %rax
    movq    %rbp, 40(%rsp)
    xorq    %rbp, %rax
    movq    16(%rsp), %rbp
    movq    %r11, 144(%rsp)
    xorq    %r9, %r12
    movq    %r11, %rcx
    movq    -96(%rsp), %r11
    rorx    $39, %r12, %r12
    xorq    %r10, %rdx
    xorq    %r8, %r13
    xorq    %rbx, %r14
    xorq    %rsi, %rbp
    rorx    $58, %rdx, %rdx
    rorx    $56, %r13, %r13
    movq    %r12, %rbx
    rorx    $63, %rbp, %rbp
    movq    %r14, 32(%rsp)
    xorq    %rdi, %r11
    orq %rdx, %rbx
    movq    %r13, %r14
    xorq    72(%rsp), %rcx
    rorx    $46, %r11, %r11
    xorq    %rbp, %rbx
    notq    %r13
    andq    %r12, %r14
    movq    %rbx, 128(%rsp)
    movq    %r15, 112(%rsp)
    xorq    %rdx, %r14
    movq    %r13, %rbx
    andq    %rbp, %rdx
    movq    %r11, %r15
    xorq    %r11, %rdx
    andq    %r11, %rbx
    orq %rbp, %r15
    movq    -40(%rsp), %r11
    xorq    %r12, %rbx
    xorq    %r13, %r15
    movq    -8(%rsp), %r13
    movq    (%rsp), %r12
    movq    %r14, 56(%rsp)
    movq    -80(%rsp), %r14
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    movq    %rdx, 88(%rsp)
    movq    -120(%rsp), %rbp
    rorx    $54, %r11, %r11
    xorq    %rdi, %r13
    xorq    %r8, %r12
    rorx    $28, %r13, %r13
    movq    %r11, %rdx
    rorx    $37, %r12, %r12
    andq    %r13, %rdx
    xorq    %r10, %r14
    xorq    %r9, %rbp
    xorq    %r12, %rdx
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %rdx, 208(%rsp)
    movq    80(%rsp), %rdx
    xorq    %r15, %rax
    xorq    24(%rsp), %rdx
    movq    %r15, 168(%rsp)
    movq    %r14, %r15
    xorq    128(%rsp), %rdx
    notq    %r14
    orq %r11, %r15
    xorq    208(%rsp), %rdx
    movq    %r14, 120(%rsp)
    orq %rbp, %r14
    xorq    %r11, %r14
    movq    %rbp, %r11
    xorq    %r13, %r15
    andq    %r12, %r11
    xorq    120(%rsp), %r11
    orq %r12, %r13
    xorq    %rbp, %r13
    xorq    8(%rsp), %r10
    xorq    -24(%rsp), %r9
    xorq    -56(%rsp), %r8
    xorq    %r13, %rcx
    movq    %r14, 152(%rsp)
    movq    %r13, 136(%rsp)
    xorq    %r11, %rax
    xorq    -64(%rsp), %rdi
    xorq    -104(%rsp), %rsi
    rorx    $9, %r9, %r9
    rorx    $2, %r10, %r10
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    notq    %rbp
    rorx    $23, %rdi, %rdi
    movq    %rbp, %r14
    rorx    $62, %rsi, %rsi
    movq    %rdi, %r12
    andq    %r8, %r14
    movq    %rsi, %r13
    orq %r8, %r12
    xorq    %r10, %r14
    orq %r10, %r13
    xorq    %rbp, %r12
    movq    56(%rsp), %rbp
    xorq    64(%rsp), %rbp
    movq    %r12, 176(%rsp)
    andq    %r9, %r10
    xorq    %r14, %rdx
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r13, %rax
    xorq    %r10, %rcx
    xorq    %r15, %rbp
    rorx    $63, %rax, %r9
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    104(%rsp), %rbp
    andq    %rdi, %r12
    xorq    %r8, %r12
    movq    48(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %rbp, %r9
    movabsq $-9223372036854775669, %rbp
    xorq    %r12, %r8
    xorq    32(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbx, %r8
    xorq    152(%rsp), %r8
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    64(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rcx, %r11
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    rorx    $43, %r11, %r11
    xorq    %rsi, %r8
    xorq    %rdx, %r10
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    xorq    %rdi, %rax
    xorq    %rax, %rbp
    xorq    %rsi, %r15
    xorq    %r9, %r12
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    rorx    $19, %r15, %r15
    orq %r8, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $3, %r12, %r12
    xorq    %rdi, %r14
    orq %r11, %rbp
    rorx    $46, %r14, %r14
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 64(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 192(%rsp)
    movq    144(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    128(%rsp), %r10
    orq %rax, %rbx
    movq    40(%rsp), %rax
    movq    %rbp, 160(%rsp)
    xorq    %rdx, %r8
    xorq    %r11, %rbx
    movq    %r15, %r11
    xorq    %rdi, %r10
    rorx    $44, %r8, %r8
    movq    %rbx, 184(%rsp)
    rorx    $61, %r10, %r10
    xorq    %rcx, %rax
    movq    %r12, %rbx
    movq    %r10, %rbp
    rorx    $36, %rax, %rax
    andq    %r10, %r11
    orq %r8, %rbp
    xorq    %r8, %r11
    notq    %rbx
    xorq    %rax, %rbp
    andq    %rax, %r8
    orq %r15, %rbx
    movq    %rbp, 144(%rsp)
    movq    %r12, %rbp
    xorq    %r12, %r8
    orq %rax, %rbp
    movq    136(%rsp), %r12
    movq    168(%rsp), %rax
    xorq    %r10, %rbx
    movq    32(%rsp), %r10
    movq    %r8, 120(%rsp)
    movq    104(%rsp), %r8
    xorq    %r15, %rbp
    movq    %r11, 128(%rsp)
    xorq    %rcx, %rax
    xorq    %rdx, %r12
    movq    %rbx, 40(%rsp)
    rorx    $39, %rax, %rax
    xorq    %r9, %r10
    rorx    $56, %r12, %r12
    xorq    %rsi, %r8
    rorx    $58, %r10, %r10
    movq    %rax, %r11
    movq    %r12, %r15
    notq    %r12
    rorx    $63, %r8, %r8
    orq %r10, %r11
    movq    %r12, %rbx
    movq    %rbp, 200(%rsp)
    xorq    %r8, %r11
    andq    %rax, %r15
    andq    %r14, %rbx
    xorq    %rax, %rbx
    movq    %r14, %rax
    xorq    %r10, %r15
    orq %r8, %rax
    andq    %r8, %r10
    movq    %r11, 32(%rsp)
    xorq    %r12, %rax
    xorq    %r14, %r10
    movq    72(%rsp), %r8
    movq    %rax, 136(%rsp)
    movq    %rbp, %rax
    movq    56(%rsp), %rbp
    movq    %r10, 216(%rsp)
    movq    80(%rsp), %r10
    xorq    %rcx, %r13
    movq    152(%rsp), %r12
    xorq    %rdx, %r8
    xorq    112(%rsp), %rcx
    xorq    %rsi, %rbp
    rorx    $37, %r8, %r8
    rorx    $8, %r13, %r13
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    88(%rsp), %rdx
    rorx    $28, %r10, %r10
    movq    %rbp, %r11
    xorq    %r9, %r12
    andq    %r10, %r11
    rorx    $49, %r12, %r12
    xorq    208(%rsp), %rdi
    xorq    %r8, %r11
    xorq    48(%rsp), %r9
    xorq    184(%rsp), %rax
    movq    %r11, 72(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    rorx    $9, %rcx, %rcx
    orq %r13, %r14
    xorq    136(%rsp), %rax
    xorq    %r10, %r11
    xorq    %rbp, %r14
    movq    %r13, %rbp
    rorx    $25, %rdx, %rdx
    andq    %r8, %rbp
    movq    %r14, 80(%rsp)
    rorx    $23, %rdi, %rdi
    xorq    %r12, %rbp
    movq    %r10, %r12
    movq    %rcx, %r10
    notq    %r10
    movq    %r15, 168(%rsp)
    orq %r8, %r12
    movq    %r10, %r14
    rorx    $2, %r9, %r9
    movq    %rdi, %r15
    andq    %rdx, %r14
    xorq    %r13, %r12
    xorq    176(%rsp), %rsi
    xorq    %r9, %r14
    xorq    %rbp, %rax
    movq    144(%rsp), %r8
    xorq    24(%rsp), %r8
    orq %rdx, %r15
    xorq    %r10, %r15
    movq    168(%rsp), %r10
    xorq    128(%rsp), %r10
    rorx    $62, %rsi, %rsi
    movq    %r15, 48(%rsp)
    xorq    32(%rsp), %r8
    movq    %rsi, %r13
    xorq    72(%rsp), %r8
    orq %r9, %r13
    andq    %rcx, %r9
    movq    120(%rsp), %rcx
    xorq    %r11, %r10
    xorq    192(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    216(%rsp), %rcx
    andq    %rdi, %r15
    xorq    64(%rsp), %r10
    xorq    %r13, %rax
    xorq    %rdx, %r15
    movq    160(%rsp), %rdx
    xorq    %rsi, %r9
    movq    %r13, 112(%rsp)
    rorx    $63, %rax, %r13
    xorq    %r14, %r8
    xorq    %r12, %rcx
    xorq    %r15, %rdx
    xorq    40(%rsp), %rdx
    xorq    %r9, %rcx
    xorq    %r10, %r13
    rorx    $63, %r10, %rdi
    rorx    $63, %rcx, %r10
    xorq    %rcx, %rdi
    movabsq $-9223372036854742903, %rcx
    xorq    %rbx, %rdx
    xorq    80(%rsp), %rdx
    xorq    %r13, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rsi
    rorx    $63, %r8, %rdx
    xorq    %r8, %rsi
    xorq    %rax, %rdx
    movq    24(%rsp), %r8
    movq    128(%rsp), %rax
    xorq    %rdi, %r8
    xorq    %rsi, %rax
    xorq    %r10, %rbp
    xorq    %r8, %rcx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    rorx    $43, %rbp, %rbp
    movq    %rax, %rcx
    xorq    %rdx, %r9
    xorq    %rsi, %r11
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    rorx    $50, %r9, %r9
    rorx    $19, %r11, %r11
    orq %rbp, %rcx
    xorq    %r13, %r15
    xorq    %rdi, %r14
    xorq    %rax, %rcx
    andq    %r8, %rax
    rorx    $3, %r15, %r15
    movq    %rcx, 104(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 88(%rsp)
    movq    120(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    32(%rsp), %r8
    movq    %rcx, 128(%rsp)
    movq    184(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 56(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $46, %r14, %r14
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    orq %r11, %rbx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rax, %r9
    andq    %rcx, %rax
    xorq    %rcx, %rbp
    xorq    %r15, %rax
    xorq    %r8, %rbx
    movq    %rbp, 208(%rsp)
    movq    %r15, %rbp
    movq    %rax, 184(%rsp)
    orq %rcx, %rbp
    movq    136(%rsp), %rcx
    movq    40(%rsp), %rax
    movq    64(%rsp), %r8
    movq    %r9, 152(%rsp)
    movq    %rdx, %r9
    xorq    %r12, %r9
    xorq    %r11, %rbp
    movq    %r14, %r15
    xorq    %r10, %rcx
    xorq    %r13, %rax
    rorx    $56, %r9, %r9
    rorx    $39, %rcx, %rcx
    rorx    $58, %rax, %rax
    xorq    %rsi, %r8
    movq    %rcx, %r11
    rorx    $63, %r8, %r8
    movq    %r9, %r12
    orq %rax, %r11
    andq    %rcx, %r12
    orq %r8, %r15
    xorq    %r8, %r11
    xorq    %rax, %r12
    andq    %rax, %r8
    movq    %r11, 40(%rsp)
    movq    168(%rsp), %r11
    xorq    %r14, %r8
    movq    %r12, 136(%rsp)
    movq    144(%rsp), %r12
    notq    %r9
    movq    192(%rsp), %rax
    movq    %rbx, 32(%rsp)
    movq    %r9, %rbx
    movq    %r8, 224(%rsp)
    xorq    %rsi, %r11
    movq    24(%rsp), %r8
    xorq    208(%rsp), %r8
    rorx    $54, %r11, %r11
    andq    %r14, %rbx
    xorq    40(%rsp), %r8
    movq    80(%rsp), %r14
    xorq    %rdi, %r12
    xorq    %rcx, %rbx
    rorx    $28, %r12, %r12
    xorq    %rdx, %rax
    movq    %r11, %rcx
    xorq    %r9, %r15
    rorx    $37, %rax, %rax
    movq    112(%rsp), %r9
    andq    %r12, %rcx
    xorq    %r13, %r14
    xorq    %rax, %rcx
    rorx    $49, %r14, %r14
    movq    %rbp, 176(%rsp)
    movq    %rcx, 80(%rsp)
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    %r10, %r9
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r15, 120(%rsp)
    movq    104(%rsp), %rbp
    movq    %r14, %r15
    xorq    152(%rsp), %rbp
    xorq    %r12, %rcx
    xorq    136(%rsp), %rbp
    orq %r9, %r15
    xorq    %r11, %r15
    movq    %r9, %r11
    movq    %r15, 112(%rsp)
    movq    %r12, %r15
    andq    %rax, %r11
    orq %rax, %r15
    xorq    160(%rsp), %r13
    xorq    200(%rsp), %r10
    xorq    %r14, %r11
    xorq    %r9, %r15
    xorq    %rcx, %rbp
    xorq    216(%rsp), %rdx
    xorq    72(%rsp), %rdi
    xorq    48(%rsp), %rsi
    rorx    $2, %r13, %r9
    rorx    $9, %r10, %r13
    movq    %r13, %rax
    andq    %r9, %r13
    rorx    $23, %rdi, %rdi
    rorx    $25, %rdx, %rdx
    notq    %rax
    rorx    $62, %rsi, %rsi
    movq    %rdi, %r10
    movq    %rax, %r14
    orq %rdx, %r10
    movq    %rsi, %r12
    andq    %rdx, %r14
    xorq    %rax, %r10
    andq    %rdi, %r12
    movq    %rsi, %rax
    xorq    %rdx, %r12
    orq %r9, %rax
    movq    32(%rsp), %rdx
    xorq    %r9, %r14
    xorq    %rdi, %rax
    movq    184(%rsp), %r9
    movq    %rax, 72(%rsp)
    movq    176(%rsp), %rax
    xorq    %rsi, %r13
    xorq    56(%rsp), %rax
    xorq    %rbx, %rdx
    xorq    128(%rsp), %rdx
    xorq    112(%rsp), %rdx
    xorq    %r13, %r9
    xorq    88(%rsp), %r9
    xorq    224(%rsp), %r9
    xorq    %r10, %rbp
    xorq    %r14, %r8
    rorx    $63, %rbp, %rdi
    movq    %r10, 48(%rsp)
    xorq    %r11, %rax
    xorq    120(%rsp), %rax
    xorq    72(%rsp), %rax
    xorq    %r12, %rdx
    xorq    %r15, %r9
    rorx    $63, %rdx, %rsi
    xorq    %r9, %rdi
    xorq    %r8, %rsi
    rorx    $63, %r9, %r9
    rorx    $63, %r8, %r8
    xorq    %rdx, %r9
    movq    152(%rsp), %rdx
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    movq    24(%rsp), %rax
    xorq    %rbp, %r10
    movabsq $-9223372036854743037, %rbp
    xorq    %rdi, %rax
    xorq    %rsi, %rdx
    xorq    %r10, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rax, %rbp
    rorx    $20, %rdx, %rdx
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    xorq    %r9, %r11
    orq %rdx, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    rorx    $43, %r11, %r11
    notq    %rbp
    xorq    %r8, %r13
    orq %r11, %rbp
    rorx    $50, %r13, %r13
    xorq    %rsi, %rcx
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    xorq    %r10, %r12
    movq    %rbp, 64(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 152(%rsp)
    movq    56(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    rorx    $19, %rcx, %rcx
    orq %rax, %rbx
    movq    184(%rsp), %rax
    movq    %rbp, 144(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    rorx    $3, %r12, %r12
    rorx    $36, %rdx, %rdx
    movq    %rbx, 168(%rsp)
    movq    %rcx, %r13
    xorq    %r8, %rax
    movq    %r12, %rbx
    rorx    $44, %rax, %r11
    movq    40(%rsp), %rax
    notq    %rbx
    orq %rcx, %rbx
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    andq    %rdx, %r11
    xorq    %rdx, %rbp
    xorq    %r12, %r11
    movq    %r13, 160(%rsp)
    movq    %rbp, 56(%rsp)
    movq    %r12, %rbp
    movq    %rbx, 40(%rsp)
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    movq    %r11, %rcx
    movq    %rbp, 184(%rsp)
    movq    %rbp, %rax
    xorq    168(%rsp), %rax
    movq    %r11, 192(%rsp)
    movq    32(%rsp), %rdx
    movq    104(%rsp), %r11
    movq    120(%rsp), %rbp
    xorq    152(%rsp), %rcx
    xorq    %rsi, %r11
    xorq    %r10, %rdx
    xorq    %r8, %r15
    rorx    $56, %r15, %r15
    xorq    %r9, %rbp
    xorq    %rdi, %r14
    movq    %r15, %r13
    notq    %r15
    rorx    $39, %rbp, %rbp
    rorx    $46, %r14, %r14
    movq    %r15, %rbx
    rorx    $58, %rdx, %rdx
    andq    %r14, %rbx
    movq    %rbp, %r12
    rorx    $63, %r11, %r11
    xorq    %rbp, %rbx
    orq %rdx, %r12
    andq    %rbp, %r13
    movq    %r14, %rbp
    xorq    %r11, %r12
    xorq    %rdx, %r13
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    136(%rsp), %r11
    movq    %r13, 200(%rsp)
    movq    208(%rsp), %r13
    xorq    %r14, %rdx
    movq    %r12, 32(%rsp)
    movq    88(%rsp), %r12
    xorq    %r15, %rbp
    movq    112(%rsp), %r14
    movq    %rdx, 216(%rsp)
    xorq    %rdx, %rcx
    xorq    %rsi, %r11
    movq    56(%rsp), %rdx
    xorq    24(%rsp), %rdx
    rorx    $54, %r11, %r11
    xorq    32(%rsp), %rdx
    movq    %rbp, 120(%rsp)
    xorq    %rbp, %rax
    xorq    %rdi, %r13
    movq    72(%rsp), %rbp
    xorq    %r8, %r12
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    andq    %r13, %r15
    xorq    %r12, %r15
    xorq    %r9, %rbp
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %r15, 72(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 88(%rsp)
    orq %rbp, %r14
    orq %r11, %r15
    xorq    %r11, %r14
    movq    %rbp, %r11
    xorq    %r13, %r15
    andq    %r12, %r11
    xorq    88(%rsp), %r11
    movq    %r14, 112(%rsp)
    xorq    %r11, %rax
    orq %r12, %r13
    xorq    176(%rsp), %r9
    xorq    80(%rsp), %rdi
    xorq    224(%rsp), %r8
    xorq    %rbp, %r13
    xorq    48(%rsp), %rsi
    xorq    128(%rsp), %r10
    xorq    %r13, %rcx
    movq    %r13, 88(%rsp)
    rorx    $9, %r9, %r9
    rorx    $23, %rdi, %rdi
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    movq    %rdi, %r12
    notq    %rbp
    rorx    $62, %rsi, %rsi
    orq %r8, %r12
    movq    %rbp, %r14
    rorx    $2, %r10, %r10
    xorq    %rbp, %r12
    movq    200(%rsp), %rbp
    xorq    160(%rsp), %rbp
    movq    %r12, 48(%rsp)
    andq    %r8, %r14
    movq    %rsi, %r13
    xorq    %r10, %r14
    orq %r10, %r13
    andq    %r9, %r10
    xorq    %rdi, %r13
    xorq    %rsi, %r10
    xorq    %r14, %rdx
    xorq    %r15, %rbp
    xorq    %r13, %rax
    xorq    %r10, %rcx
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    64(%rsp), %rbp
    andq    %rdi, %r12
    rorx    $63, %rax, %r9
    xorq    %r8, %r12
    movq    144(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %rbp, %r9
    movabsq $-9223372036854743038, %rbp
    xorq    %r12, %r8
    xorq    40(%rsp), %r8
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %rbx, %r8
    xorq    112(%rsp), %r8
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    160(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rax, %rdx
    movq    24(%rsp), %rax
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %rsi, %r8
    xorq    %rcx, %r11
    rorx    $20, %r8, %r8
    rorx    $43, %r11, %r11
    xorq    %rdx, %r10
    xorq    %rdi, %rax
    rorx    $50, %r10, %r10
    xorq    %rsi, %r15
    xorq    %rax, %rbp
    xorq    %r9, %r12
    rorx    $19, %r15, %r15
    movq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    rorx    $3, %r12, %r12
    orq %r8, %rbp
    xorq    %rbp, 24(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 104(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 208(%rsp)
    movq    32(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    168(%rsp), %rax
    movq    %rbp, 80(%rsp)
    xorq    %r11, %rbx
    movq    192(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    movq    %rbx, 128(%rsp)
    movq    %r12, %rbx
    xorq    %rcx, %rax
    movq    %r8, %rbp
    andq    %r8, %r10
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    notq    %rbx
    rorx    $44, %r11, %r11
    orq %r15, %rbx
    orq %r11, %rbp
    xorq    %r11, %r10
    andq    %rax, %r11
    xorq    %rax, %rbp
    xorq    %r8, %rbx
    xorq    %r12, %r11
    movq    %rbp, 168(%rsp)
    movq    %r10, 136(%rsp)
    movq    %r12, %rbp
    movq    40(%rsp), %r10
    movq    88(%rsp), %r12
    orq %rax, %rbp
    movq    64(%rsp), %r8
    xorq    %r15, %rbp
    movq    120(%rsp), %rax
    movq    %rbx, 32(%rsp)
    movq    %r11, 160(%rsp)
    movq    %rbp, 176(%rsp)
    xorq    %rsi, %r8
    xorq    %r9, %r10
    xorq    %rdx, %r12
    rorx    $56, %r12, %r12
    xorq    %rdi, %r14
    xorq    %rcx, %rax
    movq    %r12, %r15
    notq    %r12
    rorx    $46, %r14, %r14
    movq    %r12, %rbx
    rorx    $39, %rax, %rax
    rorx    $63, %r8, %r8
    andq    %r14, %rbx
    andq    %rax, %r15
    movq    %rax, %r11
    xorq    %rax, %rbx
    movq    %r14, %rax
    rorx    $58, %r10, %r10
    orq %r8, %rax
    xorq    %r10, %r15
    orq %r10, %r11
    xorq    %r12, %rax
    andq    %r8, %r10
    xorq    %r8, %r11
    movq    %rax, 192(%rsp)
    xorq    %r14, %r10
    movq    %rbp, %rax
    movq    200(%rsp), %rbp
    movq    %r10, 120(%rsp)
    xorq    %rcx, %r13
    movq    56(%rsp), %r10
    movq    152(%rsp), %r8
    rorx    $8, %r13, %r13
    movq    112(%rsp), %r12
    movq    %r11, 40(%rsp)
    xorq    %rsi, %rbp
    xorq    128(%rsp), %rax
    movq    %r15, 88(%rsp)
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    %rdx, %r8
    rorx    $28, %r10, %r10
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    192(%rsp), %rax
    xorq    %r8, %r11
    rorx    $49, %r12, %r12
    xorq    144(%rsp), %r9
    movq    %r11, 112(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    orq %r13, %r14
    xorq    %r10, %r11
    xorq    %rbp, %r14
    movq    %r13, %rbp
    rorx    $2, %r9, %r9
    andq    %r8, %rbp
    movq    %r14, 56(%rsp)
    xorq    %r12, %rbp
    movq    %r10, %r12
    orq %r8, %r12
    xorq    %rbp, %rax
    movq    168(%rsp), %r8
    xorq    %r13, %r12
    xorq    184(%rsp), %rcx
    xorq    72(%rsp), %rdi
    xorq    216(%rsp), %rdx
    xorq    24(%rsp), %r8
    xorq    40(%rsp), %r8
    xorq    48(%rsp), %rsi
    xorq    112(%rsp), %r8
    rorx    $23, %rdi, %rdi
    rorx    $9, %rcx, %rcx
    rorx    $25, %rdx, %rdx
    movq    %rcx, %r10
    movq    %rdi, %r15
    notq    %r10
    orq %rdx, %r15
    rorx    $62, %rsi, %rsi
    xorq    %r10, %r15
    movq    %r10, %r14
    movq    %rsi, %r13
    movq    %r15, 48(%rsp)
    movq    88(%rsp), %r10
    andq    %rdx, %r14
    xorq    136(%rsp), %r10
    xorq    %r9, %r14
    orq %r9, %r13
    andq    %rcx, %r9
    movq    160(%rsp), %rcx
    xorq    208(%rsp), %rcx
    xorq    120(%rsp), %rcx
    xorq    %rdi, %r13
    xorq    %rsi, %r9
    xorq    %r13, %rax
    xorq    %r14, %r8
    movq    %r13, 72(%rsp)
    xorq    %r11, %r10
    rorx    $63, %rax, %r13
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    104(%rsp), %r10
    andq    %rdi, %r15
    xorq    %r12, %rcx
    xorq    %rdx, %r15
    movq    80(%rsp), %rdx
    xorq    %r9, %rcx
    rorx    $63, %r10, %rdi
    xorq    %r15, %rdx
    xorq    32(%rsp), %rdx
    xorq    %rcx, %rdi
    xorq    %rbx, %rdx
    xorq    56(%rsp), %rdx
    rorx    $63, %rdx, %rsi
    xorq    %r8, %rsi
    xorq    %r10, %r13
    rorx    $63, %rcx, %r10
    xorq    %rdx, %r10
    rorx    $63, %r8, %rdx
    movq    24(%rsp), %r8
    xorq    %rax, %rdx
    movq    136(%rsp), %rax
    movabsq $-9223372036854775680, %rcx
    xorq    %r13, %rbx
    xorq    %r10, %rbp
    xorq    %rdx, %r9
    xorq    %rdi, %r8
    rorx    $21, %rbx, %rbx
    rorx    $43, %rbp, %rbp
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $50, %r9, %r9
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    xorq    %rsi, %r11
    movq    %rax, %rcx
    rorx    $19, %r11, %r11
    xorq    %r13, %r15
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    rorx    $3, %r15, %r15
    orq %rbp, %rcx
    xorq    %rax, %rcx
    andq    %r8, %rax
    movq    %rcx, 64(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 136(%rsp)
    movq    160(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    40(%rsp), %r8
    movq    %rcx, 144(%rsp)
    movq    128(%rsp), %rcx
    xorq    %rdx, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 152(%rsp)
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    orq %r11, %rbx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rax, %r9
    andq    %rcx, %rax
    xorq    %rcx, %rbp
    movq    %r9, 160(%rsp)
    movq    192(%rsp), %r9
    movq    %rbp, 128(%rsp)
    movq    %r15, %rbp
    xorq    %r8, %rbx
    orq %rcx, %rbp
    movq    32(%rsp), %rcx
    movq    104(%rsp), %r8
    xorq    %r11, %rbp
    xorq    %r15, %rax
    xorq    %r10, %r9
    movq    %rax, 200(%rsp)
    movq    %rdx, %rax
    rorx    $39, %r9, %r9
    xorq    %r13, %rcx
    xorq    %r12, %rax
    xorq    %rdi, %r14
    rorx    $58, %rcx, %rcx
    rorx    $46, %r14, %r14
    xorq    %rsi, %r8
    rorx    $56, %rax, %rax
    movq    %r9, %r11
    rorx    $63, %r8, %r8
    orq %rcx, %r11
    movq    %rax, %r12
    movq    %r14, %r15
    xorq    %r8, %r11
    andq    %r9, %r12
    notq    %rax
    orq %r8, %r15
    andq    %rcx, %r8
    xorq    %rcx, %r12
    xorq    %rax, %r15
    xorq    %r14, %r8
    movq    %rbx, 40(%rsp)
    movq    %rbp, 184(%rsp)
    movq    %r11, 104(%rsp)
    movq    %rax, %rbx
    movq    %r12, 32(%rsp)
    movq    %r15, 192(%rsp)
    andq    %r14, %rbx
    movq    %r8, 216(%rsp)
    movq    88(%rsp), %r11
    xorq    %r9, %rbx
    movq    168(%rsp), %r12
    movq    208(%rsp), %rax
    movq    24(%rsp), %r8
    xorq    128(%rsp), %r8
    xorq    %rsi, %r11
    movq    56(%rsp), %r14
    xorq    104(%rsp), %r8
    rorx    $54, %r11, %r11
    xorq    %rdi, %r12
    xorq    %rdx, %rax
    rorx    $28, %r12, %r12
    movq    %r11, %rcx
    rorx    $37, %rax, %rax
    movq    72(%rsp), %r9
    andq    %r12, %rcx
    xorq    %r13, %r14
    xorq    %rax, %rcx
    movq    64(%rsp), %rbp
    rorx    $49, %r14, %r14
    xorq    160(%rsp), %rbp
    movq    %rcx, 72(%rsp)
    xorq    %rcx, %r8
    movq    %r11, %rcx
    xorq    32(%rsp), %rbp
    xorq    %r10, %r9
    orq %r14, %rcx
    notq    %r14
    rorx    $8, %r9, %r9
    movq    %r14, %r15
    xorq    %r12, %rcx
    orq %r9, %r15
    xorq    %r11, %r15
    movq    %r9, %r11
    xorq    %rcx, %rbp
    andq    %rax, %r11
    xorq    80(%rsp), %r13
    xorq    176(%rsp), %r10
    xorq    112(%rsp), %rdi
    xorq    48(%rsp), %rsi
    xorq    %r14, %r11
    xorq    120(%rsp), %rdx
    movq    %r15, 56(%rsp)
    movq    %r12, %r15
    orq %rax, %r15
    xorq    %r9, %r15
    rorx    $2, %r13, %r9
    rorx    $9, %r10, %r13
    rorx    $23, %rdi, %rdi
    rorx    $62, %rsi, %rsi
    movq    %r13, %rax
    rorx    $25, %rdx, %rdx
    movq    %rdi, %r10
    notq    %rax
    orq %rdx, %r10
    movq    %rsi, %r12
    movq    %rax, %r14
    xorq    %rax, %r10
    andq    %rdi, %r12
    movq    %rsi, %rax
    xorq    %rdx, %r12
    andq    %rdx, %r14
    orq %r9, %rax
    movq    40(%rsp), %rdx
    xorq    %r9, %r14
    xorq    %rdi, %rax
    andq    %r9, %r13
    movq    200(%rsp), %r9
    movq    %rax, 80(%rsp)
    movq    184(%rsp), %rax
    xorq    152(%rsp), %rax
    xorq    %rsi, %r13
    xorq    %rbx, %rdx
    xorq    144(%rsp), %rdx
    xorq    %r10, %rbp
    xorq    56(%rsp), %rdx
    xorq    %r13, %r9
    xorq    136(%rsp), %r9
    xorq    216(%rsp), %r9
    xorq    %r14, %r8
    rorx    $63, %rbp, %rdi
    xorq    %r11, %rax
    xorq    192(%rsp), %rax
    movq    %r10, 48(%rsp)
    xorq    80(%rsp), %rax
    xorq    %r12, %rdx
    xorq    %r15, %r9
    rorx    $63, %rdx, %rsi
    xorq    %r9, %rdi
    xorq    %r8, %rsi
    rorx    $63, %r8, %r8
    xorq    %rax, %r8
    rorx    $63, %rax, %r10
    movq    24(%rsp), %rax
    rorx    $63, %r9, %r9
    xorq    %rbp, %r10
    xorq    %r8, %r13
    xorq    %rdx, %r9
    movq    160(%rsp), %rdx
    xorq    %r10, %rbx
    xorq    %rdi, %rax
    rorx    $21, %rbx, %rbx
    xorq    %r9, %r11
    movq    %rax, %rbp
    rorx    $43, %r11, %r11
    rorx    $50, %r13, %r13
    xorq    %rsi, %rdx
    xorq    $32778, %rbp
    xorq    %r10, %r12
    rorx    $20, %rdx, %rdx
    movq    %rbp, 112(%rsp)
    movq    %rbx, %rbp
    orq %rdx, %rbp
    xorq    %rbp, 112(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    rorx    $3, %r12, %r12
    xorq    %rsi, %rcx
    orq %r11, %rbp
    rorx    $19, %rcx, %rcx
    xorq    %rdx, %rbp
    andq    %rax, %rdx
    movq    %rbp, 24(%rsp)
    movq    %r13, %rbp
    xorq    %r13, %rdx
    andq    %r11, %rbp
    movq    %rdx, 208(%rsp)
    movq    152(%rsp), %rdx
    xorq    %rbx, %rbp
    movq    %r13, %rbx
    movq    %rcx, %r13
    orq %rax, %rbx
    movq    200(%rsp), %rax
    movq    %rbp, 168(%rsp)
    xorq    %r11, %rbx
    xorq    %r9, %rdx
    rorx    $36, %rdx, %rdx
    movq    %rbx, 88(%rsp)
    movq    %r12, %rbx
    xorq    %r8, %rax
    notq    %rbx
    rorx    $44, %rax, %r11
    movq    104(%rsp), %rax
    orq %rcx, %rbx
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    andq    %rax, %r13
    xorq    %rax, %rbx
    orq %r11, %rbp
    xorq    %r11, %r13
    movq    %rbx, 104(%rsp)
    xorq    %rdx, %rbp
    movq    %r13, 176(%rsp)
    movq    %rbp, 152(%rsp)
    movq    %r12, %rbp
    orq %rdx, %rbp
    xorq    %rcx, %rbp
    andq    %rdx, %r11
    xorq    %r8, %r15
    movq    %rbp, 160(%rsp)
    movq    %rbp, %rax
    movq    192(%rsp), %rbp
    xorq    %r12, %r11
    movq    40(%rsp), %rdx
    rorx    $56, %r15, %r15
    movq    %r11, 200(%rsp)
    movq    %r11, %rcx
    movq    64(%rsp), %r11
    xorq    %r9, %rbp
    xorq    %rdi, %r14
    movq    %r15, %r13
    notq    %r15
    rorx    $39, %rbp, %rbp
    rorx    $46, %r14, %r14
    xorq    %r10, %rdx
    movq    %r15, %rbx
    xorq    %rsi, %r11
    rorx    $58, %rdx, %rdx
    andq    %r14, %rbx
    movq    %rbp, %r12
    xorq    208(%rsp), %rcx
    rorx    $63, %r11, %r11
    xorq    %rbp, %rbx
    orq %rdx, %r12
    andq    %rbp, %r13
    movq    %r14, %rbp
    xorq    %r11, %r12
    xorq    %rdx, %r13
    orq %r11, %rbp
    andq    %r11, %rdx
    movq    32(%rsp), %r11
    xorq    88(%rsp), %rax
    movq    %r13, 192(%rsp)
    movq    128(%rsp), %r13
    xorq    %r14, %rdx
    movq    %r12, 40(%rsp)
    movq    136(%rsp), %r12
    xorq    %rdx, %rcx
    movq    %rdx, 224(%rsp)
    xorq    %rsi, %r11
    movq    56(%rsp), %r14
    movq    152(%rsp), %rdx
    xorq    112(%rsp), %rdx
    xorq    %r15, %rbp
    rorx    $54, %r11, %r11
    xorq    40(%rsp), %rdx
    xorq    %rdi, %r13
    movq    %rbp, 120(%rsp)
    xorq    %rbp, %rax
    xorq    %r8, %r12
    movq    80(%rsp), %rbp
    rorx    $28, %r13, %r13
    movq    %r11, %r15
    rorx    $37, %r12, %r12
    xorq    %r10, %r14
    andq    %r13, %r15
    xorq    %r12, %r15
    rorx    $49, %r14, %r14
    xorq    %r9, %rbp
    movq    %r15, 80(%rsp)
    xorq    %r15, %rdx
    movq    %r14, %r15
    rorx    $8, %rbp, %rbp
    notq    %r14
    orq %r11, %r15
    movq    %r14, 32(%rsp)
    xorq    %r13, %r15
    orq %rbp, %r14
    orq %r12, %r13
    xorq    %rbp, %r13
    xorq    %r11, %r14
    movq    %rbp, %r11
    movq    %r14, 128(%rsp)
    andq    %r12, %r11
    xorq    32(%rsp), %r11
    movq    %r13, 56(%rsp)
    xorq    184(%rsp), %r9
    xorq    %r13, %rcx
    xorq    72(%rsp), %rdi
    xorq    216(%rsp), %r8
    xorq    48(%rsp), %rsi
    xorq    144(%rsp), %r10
    xorq    %r11, %rax
    rorx    $9, %r9, %r9
    rorx    $23, %rdi, %rdi
    rorx    $25, %r8, %r8
    movq    %r9, %rbp
    movq    %rdi, %r12
    notq    %rbp
    rorx    $62, %rsi, %rsi
    orq %r8, %r12
    movq    %rbp, %r14
    rorx    $2, %r10, %r10
    xorq    %rbp, %r12
    movq    192(%rsp), %rbp
    xorq    176(%rsp), %rbp
    movq    %r12, 48(%rsp)
    andq    %r8, %r14
    movq    %rsi, %r13
    orq %r10, %r13
    xorq    %r10, %r14
    andq    %r9, %r10
    xorq    %rdi, %r13
    xorq    %r14, %rdx
    xorq    %r15, %rbp
    xorq    %r13, %rax
    xorq    %r12, %rbp
    movq    %rsi, %r12
    xorq    24(%rsp), %rbp
    andq    %rdi, %r12
    rorx    $63, %rax, %r9
    xorq    %r8, %r12
    movq    168(%rsp), %r8
    rorx    $63, %rbp, %rdi
    xorq    %r12, %r8
    xorq    104(%rsp), %r8
    xorq    %rbx, %r8
    xorq    128(%rsp), %r8
    xorq    %rsi, %r10
    xorq    %r10, %rcx
    xorq    %rbp, %r9
    movabsq $-9223372034707292150, %rbp
    xorq    %rcx, %rdi
    rorx    $63, %rcx, %rcx
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %r9, %r12
    rorx    $63, %r8, %rsi
    xorq    %r8, %rcx
    movq    176(%rsp), %r8
    xorq    %rdx, %rsi
    rorx    $63, %rdx, %rdx
    xorq    %rcx, %r11
    xorq    %rax, %rdx
    movq    112(%rsp), %rax
    rorx    $43, %r11, %r11
    xorq    %rsi, %r8
    xorq    %rdx, %r10
    rorx    $3, %r12, %r12
    rorx    $20, %r8, %r8
    rorx    $50, %r10, %r10
    xorq    %rsi, %r15
    xorq    %rdi, %rax
    rorx    $19, %r15, %r15
    xorq    %rax, %rbp
    movq    %rbp, 32(%rsp)
    movq    %rbx, %rbp
    orq %r8, %rbp
    xorq    %rbp, 32(%rsp)
    movq    %rbx, %rbp
    notq    %rbp
    orq %r11, %rbp
    xorq    %r8, %rbp
    andq    %rax, %r8
    movq    %rbp, 64(%rsp)
    movq    %r10, %rbp
    xorq    %r10, %r8
    andq    %r11, %rbp
    movq    %r8, 144(%rsp)
    movq    40(%rsp), %r8
    xorq    %rbx, %rbp
    movq    %r10, %rbx
    movq    %r15, %r10
    orq %rax, %rbx
    movq    88(%rsp), %rax
    movq    %rbp, 72(%rsp)
    xorq    %r11, %rbx
    movq    200(%rsp), %r11
    xorq    %rdi, %r8
    rorx    $61, %r8, %r8
    movq    %rbx, 112(%rsp)
    movq    %r12, %rbx
    xorq    %rcx, %rax
    movq    %r8, %rbp
    notq    %rbx
    xorq    %rdx, %r11
    rorx    $36, %rax, %rax
    andq    %r8, %r10
    rorx    $44, %r11, %r11
    orq %r15, %rbx
    orq %r11, %rbp
    xorq    %r11, %r10
    xorq    %rax, %rbp
    xorq    %r8, %rbx
    andq    %rax, %r11
    xorq    %r12, %r11
    movq    %rbp, 88(%rsp)
    movq    %r12, %rbp
    movq    56(%rsp), %r12
    orq %rax, %rbp
    movq    120(%rsp), %rax
    movq    %r10, 136(%rsp)
    movq    24(%rsp), %r8
    xorq    %r15, %rbp
    movq    104(%rsp), %r10
    xorq    %rdi, %r14
    movq    %rbx, 40(%rsp)
    xorq    %rdx, %r12
    rorx    $46, %r14, %r14
    xorq    %rcx, %rax
    rorx    $56, %r12, %r12
    rorx    $39, %rax, %rax
    xorq    %rsi, %r8
    movq    %r12, %r15
    notq    %r12
    xorq    %r9, %r10
    movq    %r12, %rbx
    rorx    $63, %r8, %r8
    movq    %r11, 184(%rsp)
    andq    %r14, %rbx
    rorx    $58, %r10, %r10
    andq    %rax, %r15
    xorq    %rax, %rbx
    movq    %rax, %r11
    movq    %r14, %rax
    orq %r10, %r11
    orq %r8, %rax
    xorq    %r10, %r15
    xorq    %r8, %r11
    xorq    %r12, %rax
    andq    %r8, %r10
    xorq    %r14, %r10
    movq    %rbp, 176(%rsp)
    movq    %r11, 56(%rsp)
    movq    %rax, 120(%rsp)
    movq    %rbp, %rax
    movq    192(%rsp), %rbp
    movq    %r10, 216(%rsp)
    movq    152(%rsp), %r10
    xorq    %rcx, %r13
    movq    208(%rsp), %r8
    movq    128(%rsp), %r12
    rorx    $8, %r13, %r13
    xorq    %rsi, %rbp
    xorq    112(%rsp), %rax
    movq    %r15, 200(%rsp)
    rorx    $54, %rbp, %rbp
    xorq    %rdi, %r10
    xorq    120(%rsp), %rax
    rorx    $28, %r10, %r10
    xorq    %rdx, %r8
    movq    %rbp, %r11
    rorx    $37, %r8, %r8
    xorq    %r9, %r12
    andq    %r10, %r11
    xorq    %r8, %r11
    rorx    $49, %r12, %r12
    movq    %r11, 128(%rsp)
    movq    %r12, %r11
    notq    %r12
    movq    %r12, %r14
    orq %rbp, %r11
    orq %r13, %r14
    xorq    %r10, %r11
    xorq    %rbp, %r14
    movq    %r13, %rbp
    andq    %r8, %rbp
    xorq    80(%rsp), %rdi
    xorq    160(%rsp), %rcx
    xorq    224(%rsp), %rdx
    xorq    %r12, %rbp
    movq    %r10, %r12
    xorq    48(%rsp), %rsi
    movq    %r14, 208(%rsp)
    orq %r8, %r12
    xorq    168(%rsp), %r9
    movq    88(%rsp), %r8
    xorq    %r13, %r12
    rorx    $23, %rdi, %rdi
    rorx    $9, %rcx, %rcx
    xorq    32(%rsp), %r8
    rorx    $25, %rdx, %rdx
    movq    %rcx, %r10
    movq    %rdi, %r15
    notq    %r10
    orq %rdx, %r15
    rorx    $62, %rsi, %rsi
    xorq    %r10, %r15
    movq    %r10, %r14
    movq    200(%rsp), %r10
    xorq    136(%rsp), %r10
    movq    %r15, 48(%rsp)
    andq    %rdx, %r14
    xorq    56(%rsp), %r8
    rorx    $2, %r9, %r9
    movq    %rsi, %r13
    xorq    128(%rsp), %r8
    orq %r9, %r13
    xorq    %r9, %r14
    xorq    %rdi, %r13
    xorq    %rbp, %rax
    andq    %rcx, %r9
    xorq    %r11, %r10
    xorq    %r13, %rax
    xorq    %rsi, %r9
    xorq    %r15, %r10
    movq    %rsi, %r15
    xorq    64(%rsp), %r10
    andq    %rdi, %r15
    xorq    %r14, %r8
    movq    184(%rsp), %rcx
    xorq    %rdx, %r15
    movq    72(%rsp), %rdx
    movq    %r13, 80(%rsp)
    rorx    $63, %rax, %r13
    rorx    $63, %r10, %rdi
    xorq    %r15, %rdx
    xorq    40(%rsp), %rdx
    xorq    %rbx, %rdx
    xorq    208(%rsp), %rdx
    xorq    144(%rsp), %rcx
    xorq    216(%rsp), %rcx
    xorq    %r10, %r13
    xorq    %r13, %rbx
    xorq    %r13, %r15
    rorx    $21, %rbx, %rbx
    rorx    $3, %r15, %r15
    rorx    $63, %rdx, %rsi
    xorq    %r12, %rcx
    xorq    %r8, %rsi
    xorq    %r9, %rcx
    xorq    %rsi, %r11
    rorx    $63, %rcx, %r10
    xorq    %rcx, %rdi
    movabsq $-9223372034707259263, %rcx
    xorq    %rdx, %r10
    rorx    $63, %r8, %rdx
    movq    32(%rsp), %r8
    xorq    %rax, %rdx
    movq    136(%rsp), %rax
    xorq    %r10, %rbp
    rorx    $43, %rbp, %rbp
    xorq    %rdx, %r9
    rorx    $19, %r11, %r11
    xorq    %rdi, %r8
    rorx    $50, %r9, %r9
    xorq    %rsi, %rax
    xorq    %r8, %rcx
    rorx    $20, %rax, %rax
    movq    %rcx, 24(%rsp)
    movq    %rax, %rcx
    orq %rbx, %rcx
    xorq    %rcx, 24(%rsp)
    movq    %rbx, %rcx
    notq    %rcx
    orq %rbp, %rcx
    xorq    %rax, %rcx
    andq    %r8, %rax
    movq    %rcx, 104(%rsp)
    movq    %r9, %rcx
    xorq    %r9, %rax
    andq    %rbp, %rcx
    movq    %rax, 136(%rsp)
    movq    184(%rsp), %rax
    xorq    %rbx, %rcx
    movq    %r9, %rbx
    movq    %r11, %r9
    orq %r8, %rbx
    movq    56(%rsp), %r8
    movq    %rcx, 168(%rsp)
    movq    112(%rsp), %rcx
    xorq    %rbp, %rbx
    xorq    %rdx, %rax
    movq    %rbx, 152(%rsp)
    rorx    $44, %rax, %rax
    movq    %r15, %rbx
    xorq    %rdi, %r8
    notq    %rbx
    rorx    $61, %r8, %r8
    xorq    %r10, %rcx
    movq    %r8, %rbp
    rorx    $36, %rcx, %rcx
    andq    %r8, %r9
    orq %rax, %rbp
    xorq    %rcx, %rbp
    xorq    %rax, %r9
    orq %r11, %rbx
    andq    %rcx, %rax
    movq    %rbp, 112(%rsp)
    xorq    %r8, %rbx
    xorq    %r15, %rax
    movq    120(%rsp), %r8
    movq    %r15, %rbp
    orq %rcx, %rbp
    movq    %rax, 184(%rsp)
    movq    40(%rsp), %rcx
    movq    %rdx, %rax
    movq    %r9, 56(%rsp)
    movq    64(%rsp), %r9
    xorq    %r12, %rax
    xorq    %r10, %r8
    xorq    %rdi, %r14
    rorx    $56, %rax, %rax
    rorx    $39, %r8, %r8
    xorq    %r13, %rcx
    movq    %rax, %r12
    rorx    $58, %rcx, %rcx
    notq    %rax
    andq    %r8, %r12
    xorq    %r11, %rbp
    rorx    $46, %r14, %r14
    xorq    %rcx, %r12
    xorq    %rsi, %r9
    movq    %rbp, 160(%rsp)
    movq    %r12, 192(%rsp)
    movq    %rax, %r12
    rorx    $63, %r9, %r9
    andq    %r14, %r12
    movq    200(%rsp), %rbp
    movq    %r14, %r15
    xorq    %r8, %r12
    orq %r9, %r15
    movq    %r8, %r11
    movq    88(%rsp), %r8
    xorq    %rax, %r15
    orq %rcx, %r11
    movq    144(%rsp), %rax
    xorq    %r9, %r11
    xorq    %rsi, %rbp
    andq    %rcx, %r9
    rorx    $54, %rbp, %rbp
    movq    80(%rsp), %rcx
    xorq    %r14, %r9
    xorq    %rdi, %r8
    movq    %rbx, 32(%rsp)
    movq    %r9, 224(%rsp)
    xorq    %rdx, %rax
    rorx    $28, %r8, %r8
    movq    %rbp, %r9
    rorx    $37, %rax, %rax
    movq    208(%rsp), %rbx
    andq    %r8, %r9
    movq    %r11, 40(%rsp)
    xorq    %r10, %rcx
    xorq    %rax, %r9
    rorx    $8, %rcx, %rcx
    movq    %r15, 120(%rsp)
    movq    %r9, 80(%rsp)
    movq    24(%rsp), %r9
    xorq    %r13, %rbx
    xorq    112(%rsp), %r9
    rorx    $49, %rbx, %rbx
    movq    104(%rsp), %r14
    xorq    %r11, %r9
    movq    %rbp, %r11
    xorq    80(%rsp), %r9
    orq %rbx, %r11
    notq    %rbx
    movq    %rbx, %r15
    xorq    %r8, %r11
    xorq    56(%rsp), %r14
    orq %rcx, %r15
    xorq    192(%rsp), %r14
    orq %rax, %r8
    xorq    %rbp, %r15
    movq    %rcx, %rbp
    xorq    %rcx, %r8
    movq    %r15, 144(%rsp)
    xorq    72(%rsp), %r13
    andq    %rax, %rbp
    xorq    176(%rsp), %r10
    xorq    128(%rsp), %rdi
    xorq    %rbx, %rbp
    xorq    48(%rsp), %rsi
    xorq    216(%rsp), %rdx
    xorq    %r11, %r14
    movq    %r8, 88(%rsp)
    rorx    $2, %r13, %rcx
    rorx    $23, %rdi, %rdi
    rorx    $9, %r10, %r13
    rorx    $25, %rdx, %rdx
    rorx    $62, %rsi, %rsi
    movq    %r13, %rax
    movq    %rdi, %rbx
    notq    %rax
    movq    %rsi, %r8
    orq %rdx, %rbx
    andq    %rdi, %r8
    movq    %rax, %r15
    xorq    %rax, %rbx
    movq    160(%rsp), %rax
    xorq    152(%rsp), %rax
    xorq    %rdx, %r8
    andq    %rdx, %r15
    movq    32(%rsp), %rdx
    movq    %rsi, %r10
    xorq    %rcx, %r15
    xorq    %rbx, %r14
    orq %rcx, %r10
    xorq    %r15, %r9
    movq    %rbx, 48(%rsp)
    xorq    %r12, %rdx
    xorq    %rbp, %rax
    xorq    168(%rsp), %rdx
    xorq    120(%rsp), %rax
    xorq    144(%rsp), %rdx
    xorq    %rdi, %r10
    movq    %r10, 72(%rsp)
    rorx    $63, %r14, %rdi
    xorq    %r10, %rax
    movq    184(%rsp), %r10
    xorq    %r8, %rdx
    andq    %rcx, %r13
    rorx    $63, %rdx, %rcx
    rorx    $63, %rax, %rbx
    xorq    %rsi, %r13
    xorq    %r9, %rcx
    rorx    $63, %r9, %r9
    xorq    %r13, %r10
    xorq    136(%rsp), %r10
    xorq    %r14, %rbx
    xorq    224(%rsp), %r10
    xorq    %rax, %r9
    movq    24(%rsp), %rax
    xorq    88(%rsp), %r10
    xorq    %rbx, %r12
    movabsq $-9223372036854742912, %r14
    rorx    $21, %r12, %r12
    xorq    %r9, %r13
    xorq    %rcx, %r11
    movq    %r12, %rsi
    rorx    $50, %r13, %r13
    xorq    %rbx, %r8
    rorx    $19, %r11, %r11
    rorx    $3, %r8, %r8
    xorq    %r10, %rdi
    rorx    $63, %r10, %r10
    xorq    %rdx, %r10
    movq    56(%rsp), %rdx
    xorq    %rdi, %rax
    xorq    %rax, %r14
    xorq    %r10, %rbp
    rorx    $43, %rbp, %rbp
    xorq    %rcx, %rdx
    rorx    $20, %rdx, %rdx
    orq %rdx, %rsi
    xorq    %rsi, %r14
    movq    %r12, %rsi
    notq    %rsi
    movq    %r14, 24(%rsp)
    movq    %r13, %r14
    orq %rbp, %rsi
    andq    %rbp, %r14
    xorq    %rdx, %rsi
    andq    %rax, %rdx
    xorq    %r12, %r14
    movq    %rsi, 64(%rsp)
    movq    %r13, %rsi
    xorq    %r13, %rdx
    orq %rax, %rsi
    movq    184(%rsp), %rax
    movq    %rdx, 208(%rsp)
    xorq    %rbp, %rsi
    movq    152(%rsp), %rdx
    movq    %r11, %r12
    movq    %rsi, 56(%rsp)
    movq    %r8, %r13
    movq    %r14, 128(%rsp)
    xorq    %r9, %rax
    movq    %r8, %r14
    notq    %r13
    rorx    $44, %rax, %rsi
    movq    40(%rsp), %rax
    xorq    %r10, %rdx
    rorx    $36, %rdx, %rdx
    xorq    %rdi, %rax
    rorx    $61, %rax, %rax
    movq    %rax, %rbp
    orq %rsi, %rbp
    xorq    %rdx, %rbp
    andq    %rax, %r12
    orq %r11, %r13
    xorq    %rsi, %r12
    orq %rdx, %r14
    andq    %rdx, %rsi
    xorq    %r11, %r14
    xorq    %rax, %r13
    xorq    %r8, %rsi
    movq    %rbp, 152(%rsp)
    movq    %r12, 176(%rsp)
    movq    %r14, %rax
    movq    %r13, 40(%rsp)
    xorq    56(%rsp), %rax
    xorq    %rdi, %r15
    movq    %r14, 184(%rsp)
    movq    %rsi, 200(%rsp)
    rorx    $46, %r15, %r15
    movq    120(%rsp), %r12
    movq    32(%rsp), %rdx
    movq    %r15, %r14
    movq    88(%rsp), %r8
    movq    104(%rsp), %rbp
    xorq    208(%rsp), %rsi
    xorq    %r10, %r12
    xorq    %rbx, %rdx
    rorx    $39, %r12, %r12
    xorq    %r9, %r8
    xorq    %rcx, %rbp
    rorx    $58, %rdx, %rdx
    rorx    $56, %r8, %r8
    movq    %r12, %r11
    rorx    $63, %rbp, %rbp
    orq %rdx, %r11
    movq    %r8, %r13
    xorq    %rbp, %r11
    andq    %r12, %r13
    notq    %r8
    orq %rbp, %r14
    movq    %r11, 32(%rsp)
    xorq    %rdx, %r13
    xorq    %r8, %r14
    movq    %r8, %r11
    movq    192(%rsp), %r8
    movq    %r13, 88(%rsp)
    movq    112(%rsp), %r13
    andq    %r15, %r11
    xorq    %r12, %r11
    movq    %r14, 120(%rsp)
    xorq    %r14, %rax
    andq    %rbp, %rdx
    movq    136(%rsp), %r12
    movq    72(%rsp), %rbp
    xorq    %rcx, %r8
    movq    144(%rsp), %r14
    xorq    %rdi, %r13
    rorx    $54, %r8, %r8
    xorq    %r15, %rdx
    rorx    $28, %r13, %r13
    movq    %r8, %r15
    movq    %rdx, 216(%rsp)
    xorq    %rdx, %rsi
    xorq    %r9, %r12
    xorq    %rbx, %r14
    xorq    %r10, %rbp
    movq    152(%rsp), %rdx
    andq    %r13, %r15
    xorq    24(%rsp), %rdx
    xorq    32(%rsp), %rdx
    xorq    160(%rsp), %r10
    rorx    $37, %r12, %r12
    xorq    48(%rsp), %rcx
    xorq    %r12, %r15
    rorx    $49, %r14, %r14
    rorx    $8, %rbp, %rbp
    movq    %r15, 72(%rsp)
    xorq    80(%rsp), %rdi
    xorq    224(%rsp), %r9
    xorq    168(%rsp), %rbx
    xorq    %r15, %rdx
    movq    %r14, %r15
    notq    %r14
    movq    %r14, 104(%rsp)
    orq %rbp, %r14
    rorx    $9, %r10, %r10
    xorq    %r8, %r14
    orq %r8, %r15
    movq    %rbp, %r8
    movq    %r14, 112(%rsp)
    rorx    $62, %rcx, %r14
    movq    %r10, %rcx
    notq    %rcx
    xorq    %r13, %r15
    rorx    $25, %r9, %r9
    andq    %r12, %r8
    orq %r13, %r12
    rorx    $23, %rdi, %r13
    movq    %rcx, %rdi
    rorx    $2, %rbx, %rbx
    xorq    %rbp, %r12
    andq    %r9, %rdi
    movq    %r13, %rbp
    xorq    104(%rsp), %r8
    xorq    %rbx, %rdi
    orq %r9, %rbp
    xorq    %r12, %rsi
    xorq    %rdi, %rdx
    movq    %rdi, 48(%rsp)
    movq    88(%rsp), %rdi
    xorq    176(%rsp), %rdi
    xorq    %rcx, %rbp
    movq    128(%rsp), %rcx
    movq    %rbp, 80(%rsp)
    xorq    %r8, %rax
    xorq    %r15, %rdi
    xorq    %rbp, %rdi
    movq    %r14, %rbp
    xorq    64(%rsp), %rdi
    andq    %r13, %rbp
    xorq    %r9, %rbp
    movq    %r14, %r9
    xorq    %rbp, %rcx
    xorq    40(%rsp), %rcx
    xorq    %r11, %rcx
    xorq    112(%rsp), %rcx
    orq %rbx, %r9
    xorq    %r13, %r9
    andq    %r10, %rbx
    xorq    %r9, %rax
    movq    %r9, 144(%rsp)
    xorq    %r14, %rbx
    movq    24(%rsp), %r9
    rorx    $63, %rax, %r13
    xorq    %rbx, %rsi
    rorx    $63, %rcx, %r10
    xorq    %rdi, %r13
    rorx    $63, %rdi, %r14
    xorq    %rdx, %r10
    rorx    $63, %rdx, %rdx
    xorq    %rsi, %r14
    xorq    %rax, %rdx
    movq    176(%rsp), %rax
    xorq    %r13, %r11
    rorx    $63, %rsi, %rsi
    xorq    %r14, %r9
    rorx    $21, %r11, %r11
    xorq    %rcx, %rsi
    movl    $2147483649, %ecx
    xorq    %rdx, %rbx
    xorq    %r9, %rcx
    xorq    %r10, %rax
    xorq    %rsi, %r8
    movq    %r11, %rdi
    rorx    $43, %r8, %r8
    rorx    $50, %rbx, %rbx
    movq    %rcx, 24(%rsp)
    rorx    $20, %rax, %rax
    movq    %r11, %rcx
    notq    %rdi
    orq %rax, %rcx
    xorq    %rcx, 24(%rsp)
    orq %r8, %rdi
    movq    %rbx, %rcx
    xorq    %r10, %r15
    xorq    %rax, %rdi
    andq    %r8, %rcx
    andq    %r9, %rax
    xorq    %r11, %rcx
    xorq    %rbx, %rax
    movq    %rdi, 104(%rsp)
    movq    %rbx, %rdi
    movq    %rcx, 168(%rsp)
    rorx    $19, %r15, %r15
    movq    32(%rsp), %rcx
    orq %r9, %rdi
    movq    %rax, 176(%rsp)
    movq    200(%rsp), %r9
    movq    56(%rsp), %rax
    xorq    %r8, %rdi
    movq    %rdi, 136(%rsp)
    movq    %r15, %r11
    xorq    %r14, %rcx
    xorq    %rsi, %rax
    xorq    %rdx, %r9
    xorq    %r13, %rbp
    rorx    $61, %rcx, %rcx
    rorx    $3, %rbp, %rbp
    rorx    $36, %rax, %rax
    rorx    $44, %r9, %r9
    movq    %rcx, %r8
    movq    %rbp, %rdi
    orq %r9, %r8
    andq    %rcx, %r11
    orq %rax, %rdi
    movq    %rbp, %rbx
    xorq    %rax, %r8
    xorq    %r9, %r11
    xorq    %r15, %rdi
    andq    %rax, %r9
    notq    %rbx
    movq    120(%rsp), %rax
    movq    %rdi, 192(%rsp)
    orq %r15, %rbx
    movq    40(%rsp), %rdi
    xorq    %rcx, %rbx
    xorq    %rdx, %r12
    movq    64(%rsp), %rcx
    xorq    %rbp, %r9
    movq    %r8, 56(%rsp)
    xorq    %rsi, %rax
    movq    48(%rsp), %r8
    rorx    $56, %r12, %r12
    rorx    $39, %rax, %rax
    movq    %r9, 200(%rsp)
    xorq    %r13, %rdi
    movq    %r12, %r9
    xorq    %r10, %rcx
    rorx    $58, %rdi, %rdi
    andq    %rax, %r9
    rorx    $63, %rcx, %rcx
    xorq    %r14, %r8
    xorq    %rdi, %r9
    notq    %r12
    movq    %rax, %rbp
    rorx    $46, %r8, %r8
    movq    %r11, 160(%rsp)
    orq %rdi, %rbp
    movq    %r9, 40(%rsp)
    andq    %rcx, %rdi
    movq    88(%rsp), %r9
    movq    %r12, %r11
    xorq    %r8, %rdi
    movq    %rbx, 32(%rsp)
    andq    %r8, %r11
    movq    112(%rsp), %rbx
    movq    %r8, %r15
    movq    152(%rsp), %r8
    xorq    %rax, %r11
    movq    %rdi, 224(%rsp)
    movq    192(%rsp), %rax
    movq    208(%rsp), %rdi
    xorq    %rcx, %rbp
    xorq    136(%rsp), %rax
    orq %rcx, %r15
    xorq    %r10, %r9
    movq    144(%rsp), %rcx
    rorx    $54, %r9, %r9
    xorq    %r14, %r8
    xorq    %r13, %rbx
    xorq    %r12, %r15
    movq    %rbp, 64(%rsp)
    xorq    %rdx, %rdi
    rorx    $28, %r8, %r8
    rorx    $49, %rbx, %rbx
    movq    %r9, %rbp
    rorx    $37, %rdi, %rdi
    movq    %r11, 48(%rsp)
    xorq    %r15, %rax
    xorq    %rsi, %rcx
    movq    %rbx, %r11
    andq    %r8, %rbp
    notq    %rbx
    rorx    $8, %rcx, %rcx
    xorq    %rdi, %rbp
    movq    %rbx, %r12
    movq    %r15, 120(%rsp)
    orq %rcx, %r12
    movq    %rbp, 112(%rsp)
    xorq    80(%rsp), %r10
    xorq    184(%rsp), %rsi
    xorq    72(%rsp), %r14
    xorq    %r9, %r12
    movq    %r12, 144(%rsp)
    xorq    216(%rsp), %rdx
    orq %r9, %r11
    movq    %rcx, %r12
    xorq    %r8, %r11
    orq %rdi, %r8
    andq    %rdi, %r12
    xorq    128(%rsp), %r13
    xorq    %rcx, %r8
    xorq    %rbx, %r12
    rorx    $62, %r10, %rcx
    movq    56(%rsp), %rdi
    movq    40(%rsp), %rbx
    xorq    24(%rsp), %rdi
    rorx    $9, %rsi, %rbp
    xorq    160(%rsp), %rbx
    movq    %r8, 88(%rsp)
    rorx    $23, %r14, %r14
    xorq    64(%rsp), %rdi
    movq    168(%rsp), %r9
    movq    %rbp, %rsi
    movq    %rcx, %r8
    rorx    $25, %rdx, %rdx
    xorq    112(%rsp), %rdi
    notq    %rsi
    andq    %r14, %r8
    movq    %r14, %r15
    xorq    %rdx, %r8
    orq %rdx, %r15
    movq    %rsi, %r10
    xorq    %rsi, %r15
    rorx    $2, %r13, %r13
    andq    %rdx, %r10
    xorq    %r11, %rbx
    xorq    %r8, %r9
    xorq    32(%rsp), %r9
    xorq    %r13, %r10
    xorq    48(%rsp), %r9
    movq    %r15, 72(%rsp)
    xorq    %r15, %rbx
    movq    %rcx, %r15
    xorq    104(%rsp), %rbx
    xorq    %r10, %rdi
    xorq    %r12, %rax
    xorq    144(%rsp), %r9
    orq %r13, %r15
    andq    %r13, %rbp
    movq    48(%rsp), %r13
    xorq    %r14, %r15
    movq    200(%rsp), %r14
    xorq    176(%rsp), %r14
    xorq    224(%rsp), %r14
    xorq    %rcx, %rbp
    rorx    $63, %rbx, %rsi
    xorq    88(%rsp), %r14
    rorx    $63, %r9, %rdx
    xorq    %r15, %rax
    xorq    %rdi, %rdx
    rorx    $63, %rdi, %rdi
    rorx    $63, %rax, %rcx
    xorq    %rax, %rdi
    movq    24(%rsp), %rax
    xorq    %rbx, %rcx
    xorq    %rcx, %r8
    movq    64(%rsp), %rbx
    xorq    %rcx, %r13
    xorq    %rbp, %r14
    rorx    $3, %r8, %r8
    xorq    %rdi, %rbp
    xorq    %r14, %rsi
    rorx    $63, %r14, %r14
    xorq    %rdx, %r11
    xorq    %r9, %r14
    movq    136(%rsp), %r9
    xorq    %rsi, %rax
    movq    %rax, 24(%rsp)
    movq    160(%rsp), %rax
    xorq    %r14, %r12
    xorq    %rsi, %rbx
    rorx    $21, %r13, %r13
    rorx    $43, %r12, %r12
    xorq    %r14, %r9
    rorx    $50, %rbp, %rbp
    rorx    $61, %rbx, %rbx
    rorx    $36, %r9, %r9
    xorq    %rdx, %rax
    rorx    $19, %r11, %r11
    movq    %r9, 48(%rsp)
    movq    200(%rsp), %r9
    rorx    $20, %rax, %rax
    movq    %rax, 80(%rsp)
    movabsq $-9223372034707259384, %rax
    xorq    24(%rsp), %rax
    xorq    %rdi, %r9
    rorx    $44, %r9, %r9
    movq    %r9, 128(%rsp)
    movq    %r8, 64(%rsp)
    movq    104(%rsp), %r8
    movq    176(%rsp), %r9
    xorq    %rdx, %r8
    xorq    %rdi, %r9
    rorx    $63, %r8, %r8
    rorx    $37, %r9, %r9
    movq    %r8, 104(%rsp)
    movq    32(%rsp), %r8
    movq    %r9, 152(%rsp)
    movq    56(%rsp), %r9
    xorq    %rcx, %r8
    rorx    $58, %r8, %r8
    xorq    %rsi, %r9
    movq    %r8, 32(%rsp)
    movq    120(%rsp), %r8
    rorx    $28, %r9, %r9
    xorq    %r14, %r8
    rorx    $39, %r8, %r8
    movq    %r8, 208(%rsp)
    movq    88(%rsp), %r8
    xorq    %rdi, %r8
    rorx    $56, %r8, %r8
    movq    %r8, 136(%rsp)
    movq    %rsi, %r8
    xorq    %r10, %r8
    movq    40(%rsp), %r10
    rorx    $46, %r8, %r8
    movq    %r8, 88(%rsp)
    movq    136(%rsp), %r8
    xorq    %rdx, %r10
    xorq    224(%rsp), %rdi
    xorq    112(%rsp), %rsi
    xorq    72(%rsp), %rdx
    rorx    $54, %r10, %r10
    notq    %r8
    movq    %r8, 160(%rsp)
    movq    144(%rsp), %r8
    rorx    $25, %rdi, %rdi
    rorx    $23, %rsi, %rsi
    rorx    $62, %rdx, %rdx
    xorq    %rcx, %r8
    xorq    168(%rsp), %rcx
    rorx    $49, %r8, %r8
    movq    %r8, 40(%rsp)
    movq    %r14, %r8
    xorq    192(%rsp), %r14
    xorq    %r15, %r8
    movq    40(%rsp), %r15
    rorx    $8, %r8, %r8
    rorx    $2, %rcx, %rcx
    notq    %r15
    rorx    $9, %r14, %r14
    movq    %r15, 144(%rsp)
    movq    80(%rsp), %r15
    movq    %r14, 56(%rsp)
    notq    %r14
    orq %r13, %r15
    xorq    %r15, %rax
    movq    96(%rsp), %r15
    movq    %rax, (%r15)
    movq    %r13, %rax
    notq    %rax
    orq %r12, %rax
    xorq    80(%rsp), %rax
    movq    %rax, 8(%r15)
    movq    %rbp, %rax
    andq    %r12, %rax
    xorq    %r13, %rax
    movq    %rax, 16(%r15)
    movq    24(%rsp), %rax
    orq %rbp, %rax
    xorq    %r12, %rax
    movq    %rax, 24(%r15)
    movq    80(%rsp), %rax
    andq    24(%rsp), %rax
    xorq    %rbp, %rax
    movq    %rax, 32(%r15)
    movq    128(%rsp), %rax
    orq %rbx, %rax
    xorq    48(%rsp), %rax
    movq    %rax, 40(%r15)
    movq    %r11, %rax
    andq    %rbx, %rax
    xorq    128(%rsp), %rax
    movq    %rax, 48(%r15)
    movq    64(%rsp), %rax
    notq    %rax
    orq %r11, %rax
    xorq    %rbx, %rax
    movq    %rax, 56(%r15)
    movq    64(%rsp), %rax
    orq 48(%rsp), %rax
    xorq    %r11, %rax
    movq    %rax, 64(%r15)
    movq    128(%rsp), %rax
    andq    48(%rsp), %rax
    xorq    64(%rsp), %rax
    movq    %rax, 72(%r15)
    movq    208(%rsp), %rax
    orq 32(%rsp), %rax
    xorq    104(%rsp), %rax
    movq    %rax, 80(%r15)
    movq    136(%rsp), %rax
    andq    208(%rsp), %rax
    xorq    32(%rsp), %rax
    movq    %rax, 88(%r15)
    movq    160(%rsp), %rax
    andq    88(%rsp), %rax
    xorq    208(%rsp), %rax
    movq    %rax, 96(%r15)
    movq    88(%rsp), %rax
    orq 104(%rsp), %rax
    xorq    160(%rsp), %rax
    movq    %rax, 104(%r15)
    movq    104(%rsp), %rax
    andq    32(%rsp), %rax
    xorq    88(%rsp), %rax
    movq    %rax, 112(%r15)
    movq    %r10, %rax
    andq    %r9, %rax
    xorq    152(%rsp), %rax
    movq    %rax, 120(%r15)
    movq    40(%rsp), %rax
    orq %r10, %rax
    xorq    %r9, %rax
    orq 152(%rsp), %r9
    movq    %rax, 128(%r15)
    movq    144(%rsp), %rax
    orq %r8, %rax
    xorq    %r8, %r9
    xorq    %r10, %rax
    movq    %r9, 152(%r15)
    movq    %rax, 136(%r15)
    movq    152(%rsp), %rax
    andq    %r8, %rax
    xorq    144(%rsp), %rax
    movq    %rax, 144(%r15)
    movq    %r14, %rax
    andq    %rdi, %rax
    xorq    %rcx, %rax
    movq    %rax, 160(%r15)
    movq    %rsi, %rax
    orq %rdi, %rax
    xorq    %r14, %rax
    movq    %rax, 168(%r15)
    movq    %rdx, %rax
    andq    %rsi, %rax
    xorq    %rdi, %rax
    movq    %rax, 176(%r15)
    movq    %rdx, %rax
    orq %rcx, %rax
    andq    56(%rsp), %rcx
    xorq    %rsi, %rax
    movq    %rax, 184(%r15)
    xorq    %rdx, %rcx
    movq    %rcx, 192(%r15)
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
.LFE33:
    .size   KeccakP1600_Permute_12rounds, .-KeccakP1600_Permute_12rounds
    .p2align 4,,15
    .globl  KeccakP1600_ExtractBytesInLane
    .type   KeccakP1600_ExtractBytesInLane, @function
KeccakP1600_ExtractBytesInLane:
.LFB34:
    .cfi_startproc
    movq    %rdx, %r9
    leal    -1(%rsi), %edx
    subq    $24, %rsp
    .cfi_def_cfa_offset 32
    movl    %esi, %eax
    cmpl    $1, %edx
    movq    (%rdi,%rax,8), %rax
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
    movq    %rbp, -8(%rsp)
    .cfi_offset 3, -24
    .cfi_offset 6, -16
    movq    %rsi, %rbx
    movl    %edx, %ebp
    leal    0(,%rdx,8), %edx
    movq    %rdi, %rsi
    subq    $24, %rsp
    .cfi_def_cfa_offset 32
    movq    %rbx, %rdi
    call    memcpy
    cmpl    $1, %ebp
    jbe .L166
    cmpl    $2, %ebp
    notq    8(%rbx)
    je  .L166
    cmpl    $8, %ebp
    notq    16(%rbx)
    jbe .L166
    cmpl    $12, %ebp
    notq    64(%rbx)
    jbe .L166
    cmpl    $17, %ebp
    notq    96(%rbx)
    jbe .L166
    cmpl    $20, %ebp
    notq    136(%rbx)
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
    leaq    16(%rsp), %rsi
    movl    %r12d, %r14d
    movq    %r15, %rdi
    movq    %r14, %rdx
    movq    %rax, 16(%rsp)
    addq    %r8, %rsi
    call    memcpy
    addl    $1, %ebx
    addq    %r14, %r15
    xorl    %r8d, %r8d
    subl    %r12d, %ebp
    je  .L174
.L186:
    movl    $8, %r12d
    leal    -1(%rbx), %edx
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
    movl    %ecx, %ebp
    leaq    (%r15,%r12), %rdi
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
    cmpl    $2, %ebx
    notq    8(%r15)
    je  .L178
    cmpl    $8, %ebx
    notq    16(%r15)
    jbe .L177
    cmpl    $12, %ebx
    notq    64(%r15)
    jbe .L177
    cmpl    $17, %ebx
    notq    96(%r15)
    jbe .L177
    cmpl    $20, %ebx
    notq    136(%r15)
    jbe .L177
    notq    160(%r15)
    .p2align 4,,10
    .p2align 3
.L177:
    leal    -1(%rbx), %edx
    movl    %ecx, %ebp
    movl    %ebx, %eax
    andl    $7, %ebp
    leaq    (%r15,%r12), %rdi
    movq    0(%r13,%rax,8), %rax
    cmpl    $1, %edx
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
    leaq    32(%rsi), %r11
    movl    %ecx, %r10d
    shrl    $2, %r10d
    cmpq    %r9, %rsi
    leal    0(,%r10,4), %r8d
    setae   %al
    cmpq    %r11, %rdx
    setae   %r11b
    orl %r11d, %eax
    leaq    32(%rdi), %r11
    cmpq    %r9, %rdi
    setae   %r9b
    cmpq    %r11, %rdx
    setae   %r11b
    orl %r11d, %r9d
    andl    %r9d, %eax
    cmpl    $5, %ecx
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
    vinserti128 $0x1, 16(%rdi,%rax), %ymm1, %ymm1
    vinserti128 $0x1, 16(%rsi,%rax), %ymm0, %ymm0
    vpxor   %ymm0, %ymm1, %ymm0
    vmovdqu %xmm0, (%rdx,%rax)
    vextracti128    $0x1, %ymm0, 16(%rdx,%rax)
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
    cmpl    $2, %ecx
    notq    8(%rdx)
    je  .L202
    cmpl    $8, %ecx
    notq    16(%rdx)
    jbe .L202
    cmpl    $12, %ecx
    notq    64(%rdx)
    jbe .L202
    cmpl    $17, %ecx
    notq    96(%rdx)
    jbe .L202
    cmpl    $20, %ecx
    notq    136(%rdx)
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
    movl    %ebx, %r9d
    leal    -1(%r11), %ecx
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
    leaq    32(%rsi), %rbx
    movl    %r8d, %r11d
    shrl    $5, %r11d
    cmpq    %r9, %rsi
    leal    0(,%r11,4), %ecx
    setae   %al
    cmpq    %rbx, %rdx
    setae   %bl
    orl %ebx, %eax
    cmpl    $5, %r10d
    seta    %bl
    andl    %ebx, %eax
    leaq    32(%rdi), %rbx
    cmpq    %r9, %rdi
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
    vinserti128 $0x1, 16(%rdi,%rax), %ymm1, %ymm1
    vinserti128 $0x1, 16(%rsi,%rax), %ymm0, %ymm0
    vpxor   %ymm0, %ymm1, %ymm0
    vmovdqu %xmm0, (%rdx,%rax)
    vextracti128    $0x1, %ymm0, 16(%rdx,%rax)
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
    cmpl    $2, %r10d
    notq    8(%rdx)
    je  .L226
    cmpl    $8, %r10d
    notq    16(%rdx)
    jbe .L220
    cmpl    $12, %r10d
    notq    64(%rdx)
    jbe .L220
    cmpl    $17, %r10d
    notq    96(%rdx)
    jbe .L220
    cmpl    $20, %r10d
    notq    136(%rdx)
    jbe .L220
    notq    160(%rdx)
.L220:
    leal    0(,%r10,8), %eax
    leal    -1(%r10), %ecx
    andl    $7, %r8d
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
    pushq   %rbx
    .cfi_def_cfa_offset 56
    .cfi_offset 3, -56
    subq    $144, %rsp
    .cfi_def_cfa_offset 200
    movq    8(%rdi), %rax
    movq    32(%rdi), %rbx
    movq    %rdi, 88(%rsp)
    movl    %esi, 108(%rsp)
    movq    %rdx, %rsi
    movq    %rcx, 136(%rsp)
    movq    16(%rdi), %rdx
    movq    24(%rdi), %rcx
    movq    (%rdi), %r11
    movq    40(%rdi), %rdi
    movq    %rax, -32(%rsp)
    movq    %rdx, -112(%rsp)
    movq    %rbx, -88(%rsp)
    movq    %rcx, -104(%rsp)
    movq    %rdi, -56(%rsp)
    movq    88(%rsp), %rdi
    movq    64(%rdi), %r10
    movq    72(%rdi), %r13
    movq    80(%rdi), %r14
    movq    88(%rdi), %r15
    movq    96(%rdi), %rax
    movq    48(%rdi), %r12
    movq    56(%rdi), %rbp
    movq    %r10, -16(%rsp)
    movq    %r13, 32(%rsp)
    movq    %r14, -96(%rsp)
    movq    %r15, -80(%rsp)
    movq    %rax, -72(%rsp)
    movq    104(%rdi), %rdx
    movq    120(%rdi), %rbx
    movq    112(%rdi), %rcx
    movq    128(%rdi), %r15
    movq    136(%rdi), %rdi
    movq    %rdx, -48(%rsp)
    movl    108(%rsp), %edx
    movq    %rbx, 40(%rsp)
    movq    %rcx, -24(%rsp)
    movq    %rdi, -8(%rsp)
    movq    88(%rsp), %rdi
    sall    $3, %edx
    cmpq    %rdx, 136(%rsp)
    movq    %rdx, 120(%rsp)
    movq    %rdi, %r10
    movq    144(%rdi), %rbx
    movq    152(%rdi), %r13
    movq    168(%r10), %r14
    movq    184(%r10), %rax
    movq    160(%rdi), %rdi
    movq    %r14, (%rsp)
    movq    %rax, -40(%rsp)
    movq    176(%r10), %r14
    movq    192(%r10), %rax
    jb  .L271
    movl    108(%rsp), %edx
    movq    136(%rsp), %rcx
    subq    120(%rsp), %rcx
    movq    %rsi, 80(%rsp)
    movq    %r12, %rsi
    movq    %rbp, -120(%rsp)
    movq    %r11, -64(%rsp)
    movq    %r13, %rbp
    salq    $3, %rdx
    cmpl    $21, 108(%rsp)
    movq    %rdx, 128(%rsp)
    movq    %rcx, 112(%rsp)
    je  .L313
    .p2align 4,,10
    .p2align 3
.L256:
    cmpl    $15, 108(%rsp)
    ja  .L258
    cmpl    $7, 108(%rsp)
    ja  .L259
    cmpl    $3, 108(%rsp)
    ja  .L260
    cmpl    $1, 108(%rsp)
    jbe .L314
    movq    80(%rsp), %r11
    movq    80(%rsp), %r12
    movq    (%r11), %r11
    movq    8(%r12), %r12
    xorq    %r11, -64(%rsp)
    xorq    %r12, -32(%rsp)
    cmpl    $2, 108(%rsp)
    jne .L315
    .p2align 4,,10
    .p2align 3
.L257:
    movq    -32(%rsp), %r12
    movq    -120(%rsp), %r11
    movq    -16(%rsp), %rdx
    xorq    -112(%rsp), %r11
    xorq    -104(%rsp), %rdx
    xorq    -72(%rsp), %r11
    xorq    %rsi, %r12
    xorq    -80(%rsp), %r12
    xorq    -48(%rsp), %rdx
    movq    -56(%rsp), %rcx
    xorq    -64(%rsp), %rcx
    xorq    -96(%rsp), %rcx
    xorq    -8(%rsp), %r11
    movq    32(%rsp), %r13
    xorq    -88(%rsp), %r13
    xorq    40(%rsp), %rcx
    xorq    -24(%rsp), %r13
    xorq    %r15, %r12
    xorq    (%rsp), %r12
    xorq    %rbx, %rdx
    xorq    -40(%rsp), %rdx
    xorq    %r14, %r11
    rorx    $63, %r11, %r8
    xorq    %rdi, %rcx
    xorq    %rbp, %r13
    xorq    %rcx, %r8
    xorq    %rax, %r13
    rorx    $63, %r12, %r10
    rorx    $63, %rcx, %rcx
    xorq    %r13, %r10
    rorx    $63, %rdx, %r9
    xorq    %rdx, %rcx
    rorx    $63, %r13, %r13
    movq    -64(%rsp), %rdx
    xorq    %r11, %r13
    movq    -72(%rsp), %r11
    xorq    %r12, %r9
    xorq    %r8, %rsi
    xorq    %r13, %rbx
    xorq    %rcx, %rax
    xorq    %r10, %rdx
    rorx    $20, %rsi, %rsi
    rorx    $43, %rbx, %rbx
    xorq    %r9, %r11
    movq    %rdx, %r12
    rorx    $50, %rax, %rax
    rorx    $21, %r11, %r11
    xorq    $1, %r12
    movq    %r12, -72(%rsp)
    movq    %r11, %r12
    orq %rsi, %r12
    xorq    %r12, -72(%rsp)
    movq    %r11, %r12
    notq    %r12
    orq %rbx, %r12
    xorq    %r9, %r14
    xorq    %rcx, %rbp
    xorq    %rsi, %r12
    andq    %rdx, %rsi
    rorx    $3, %r14, %r14
    movq    %r12, -64(%rsp)
    movq    %rax, %r12
    xorq    %rax, %rsi
    andq    %rbx, %r12
    movq    %rsi, 24(%rsp)
    movq    -104(%rsp), %rsi
    xorq    %r11, %r12
    movq    %rax, %r11
    movq    %r8, %rax
    orq %rdx, %r11
    movq    32(%rsp), %rdx
    xorq    %r15, %rax
    xorq    %rbx, %r11
    movq    %r14, %rbx
    rorx    $19, %rax, %rax
    movq    %r11, 16(%rsp)
    movq    -96(%rsp), %r11
    notq    %rbx
    xorq    %r13, %rsi
    xorq    %rcx, %rdx
    orq %rax, %rbx
    movq    %rax, %r15
    rorx    $36, %rsi, %rsi
    rorx    $44, %rdx, %rdx
    xorq    %r10, %r11
    movq    %r12, 8(%rsp)
    rorx    $56, %rbp, %rbp
    rorx    $61, %r11, %r11
    xorq    %r10, %rdi
    xorq    %r11, %rbx
    andq    %r11, %r15
    movq    %r11, %r12
    movq    %r14, %r11
    orq %rdx, %r12
    xorq    %rdx, %r15
    orq %rsi, %r11
    xorq    %rsi, %r12
    andq    %rsi, %rdx
    xorq    %rax, %r11
    xorq    %r14, %rdx
    movq    %r12, -96(%rsp)
    movq    %r15, 32(%rsp)
    movq    %rbx, -104(%rsp)
    movq    %rdx, %rsi
    movq    %r11, 56(%rsp)
    movq    -48(%rsp), %rbx
    movq    %r11, %rax
    movq    %rdx, 48(%rsp)
    movq    -120(%rsp), %rdx
    movq    %rbp, %r14
    movq    -32(%rsp), %r11
    xorq    24(%rsp), %rsi
    notq    %rbp
    xorq    %r13, %rbx
    rorx    $46, %rdi, %rdi
    movq    %rbp, %r15
    rorx    $39, %rbx, %rbx
    xorq    %r9, %rdx
    xorq    16(%rsp), %rax
    xorq    %r8, %r11
    rorx    $58, %rdx, %rdx
    movq    %rbx, %r12
    rorx    $63, %r11, %r11
    orq %rdx, %r12
    andq    %rbx, %r14
    xorq    %r11, %r12
    xorq    %rdx, %r14
    andq    %rdi, %r15
    andq    %r11, %rdx
    xorq    %rbx, %r15
    movq    %rdi, %rbx
    xorq    %rdi, %rdx
    orq %r11, %rbx
    movq    %r14, -48(%rsp)
    xorq    %rdx, %rsi
    movq    %rdx, 72(%rsp)
    movq    -88(%rsp), %rdx
    xorq    %rbp, %rbx
    movq    -80(%rsp), %r14
    movq    -8(%rsp), %rbp
    xorq    %rbx, %rax
    movq    %rbx, 64(%rsp)
    movq    -56(%rsp), %rbx
    xorq    %rcx, %rdx
    movq    %r12, -120(%rsp)
    movq    -40(%rsp), %rdi
    rorx    $37, %rdx, %r11
    xorq    %r8, %r14
    movq    -96(%rsp), %rdx
    xorq    -72(%rsp), %rdx
    rorx    $54, %r14, %r14
    xorq    %r10, %rbx
    xorq    -120(%rsp), %rdx
    rorx    $28, %rbx, %rbx
    movq    %r14, %r12
    xorq    %r9, %rbp
    andq    %rbx, %r12
    xorq    %r13, %rdi
    xorq    %r11, %r12
    rorx    $49, %rbp, %rbp
    rorx    $8, %rdi, %rdi
    movq    %r12, -88(%rsp)
    xorq    -16(%rsp), %r13
    xorq    %r12, %rdx
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, -80(%rsp)
    xorq    40(%rsp), %r10
    orq %rdi, %rbp
    xorq    %r14, %rbp
    orq %r14, %r12
    xorq    -24(%rsp), %rcx
    movq    %rdi, %r14
    xorq    %rbx, %r12
    rorx    $9, %r13, %r13
    andq    %r11, %r14
    xorq    -80(%rsp), %r14
    orq %r11, %rbx
    rorx    $23, %r10, %r10
    xorq    %rdi, %rbx
    movq    %rbp, -56(%rsp)
    rorx    $25, %rcx, %rcx
    movq    %r13, %rdi
    movq    %r10, %rbp
    xorq    -112(%rsp), %r9
    xorq    %rbx, %rsi
    notq    %rdi
    xorq    %r14, %rax
    xorq    (%rsp), %r8
    orq %rcx, %rbp
    xorq    %rdi, %rbp
    movq    %rbx, -80(%rsp)
    movq    %rdi, %rbx
    movq    -48(%rsp), %rdi
    xorq    32(%rsp), %rdi
    andq    %rcx, %rbx
    movq    %rbp, -112(%rsp)
    rorx    $2, %r9, %r9
    rorx    $62, %r8, %r8
    xorq    %r9, %rbx
    andq    %r9, %r13
    movq    %r8, %r11
    xorq    %rbx, %rdx
    xorq    %r8, %r13
    xorq    %r12, %rdi
    orq %r9, %r11
    xorq    %r13, %rsi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    -64(%rsp), %rdi
    andq    %r10, %rbp
    xorq    %r10, %r11
    xorq    %rcx, %rbp
    movq    8(%rsp), %rcx
    xorq    %r11, %rax
    rorx    $63, %rax, %r10
    rorx    $63, %rdi, %r9
    xorq    %rdi, %r10
    xorq    %rbp, %rcx
    xorq    -104(%rsp), %rcx
    xorq    %rsi, %r9
    rorx    $63, %rsi, %rsi
    xorq    %r15, %rcx
    xorq    -56(%rsp), %rcx
    xorq    %r10, %r15
    rorx    $21, %r15, %r15
    rorx    $63, %rcx, %r8
    xorq    %rcx, %rsi
    movq    32(%rsp), %rcx
    xorq    %rdx, %r8
    rorx    $63, %rdx, %rdx
    xorq    %rsi, %r14
    xorq    %rax, %rdx
    movq    -72(%rsp), %rax
    rorx    $43, %r14, %r14
    xorq    %r8, %rcx
    xorq    %rdx, %r13
    rorx    $20, %rcx, %rcx
    rorx    $50, %r13, %r13
    xorq    %r9, %rax
    movq    %rax, %rdi
    xorq    $32898, %rdi
    movq    %rdi, -16(%rsp)
    movq    %r15, %rdi
    orq %rcx, %rdi
    xorq    %rdi, -16(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    xorq    %r10, %rbp
    xorq    %r8, %r12
    orq %r14, %rdi
    rorx    $3, %rbp, %rbp
    rorx    $19, %r12, %r12
    xorq    %rcx, %rdi
    andq    %rax, %rcx
    xorq    %r9, %rbx
    movq    %rdi, -32(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rcx
    andq    %r14, %rdi
    movq    %rcx, -24(%rsp)
    movq    48(%rsp), %rcx
    xorq    %r15, %rdi
    movq    %r13, %r15
    rorx    $46, %rbx, %rbx
    orq %rax, %r15
    movq    %rdi, 32(%rsp)
    movq    -120(%rsp), %rdi
    xorq    %r14, %r15
    movq    16(%rsp), %rax
    xorq    %rdx, %rcx
    movq    %r15, -72(%rsp)
    movq    %rbp, %r15
    movq    %r12, %r14
    xorq    %r9, %rdi
    notq    %r15
    rorx    $44, %rcx, %rcx
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rax
    orq %r12, %r15
    xorq    %rdi, %r15
    rorx    $36, %rax, %rax
    andq    %rdi, %r14
    movq    %rdi, %r13
    movq    %rbp, %rdi
    xorq    %rcx, %r14
    orq %rcx, %r13
    orq %rax, %rdi
    andq    %rax, %rcx
    xorq    %rax, %r13
    xorq    %r12, %rdi
    movq    64(%rsp), %rax
    xorq    %rbp, %rcx
    movq    %rdi, -8(%rsp)
    movq    -104(%rsp), %rdi
    movq    -80(%rsp), %r12
    movq    %rcx, (%rsp)
    movq    -64(%rsp), %rcx
    xorq    %rsi, %rax
    movq    %r13, 40(%rsp)
    rorx    $39, %rax, %rax
    xorq    %r10, %rdi
    movq    %r14, -120(%rsp)
    rorx    $58, %rdi, %rdi
    xorq    %rdx, %r12
    movq    %rax, %rbp
    xorq    %r8, %rcx
    orq %rdi, %rbp
    rorx    $56, %r12, %r12
    rorx    $63, %rcx, %rcx
    movq    %r12, %r13
    notq    %r12
    xorq    %rcx, %rbp
    andq    %rax, %r13
    movq    %rbx, %r14
    movq    %rbp, -104(%rsp)
    movq    %r12, %rbp
    xorq    %rdi, %r13
    andq    %rbx, %rbp
    orq %rcx, %r14
    andq    %rcx, %rdi
    xorq    %rax, %rbp
    movq    -8(%rsp), %rax
    xorq    -72(%rsp), %rax
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    movq    -48(%rsp), %r12
    movq    %rdi, 16(%rsp)
    movq    -96(%rsp), %rdi
    xorq    %rsi, %r11
    movq    24(%rsp), %rcx
    movq    %r14, -40(%rsp)
    rorx    $8, %r11, %r11
    xorq    %r14, %rax
    xorq    %r8, %r12
    movq    -56(%rsp), %r14
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    xorq    56(%rsp), %rsi
    xorq    %rdx, %rcx
    rorx    $28, %rdi, %rdi
    movq    %r12, %rbx
    rorx    $37, %rcx, %rcx
    xorq    %r10, %r14
    andq    %rdi, %rbx
    xorq    %rcx, %rbx
    rorx    $49, %r14, %r14
    xorq    -88(%rsp), %r9
    movq    %rbx, -96(%rsp)
    movq    %r14, %rbx
    notq    %r14
    movq    %r13, -80(%rsp)
    movq    %r14, %r13
    xorq    72(%rsp), %rdx
    orq %r11, %r13
    xorq    8(%rsp), %r10
    rorx    $9, %rsi, %rsi
    xorq    %r12, %r13
    orq %r12, %rbx
    rorx    $23, %r9, %r12
    movq    %r13, -56(%rsp)
    movq    %rsi, %r9
    movq    %r11, %r13
    xorq    %rdi, %rbx
    andq    %rcx, %r13
    orq %rcx, %rdi
    notq    %r9
    xorq    %r14, %r13
    xorq    %r11, %rdi
    rorx    $25, %rdx, %rdx
    movq    %r9, %r14
    movq    %rdi, -48(%rsp)
    andq    %rdx, %r14
    rorx    $2, %r10, %rdi
    xorq    -112(%rsp), %r8
    xorq    %rdi, %r14
    movq    %r12, %r10
    xorq    %r13, %rax
    movq    %r14, -112(%rsp)
    movq    40(%rsp), %rcx
    xorq    -16(%rsp), %rcx
    movq    32(%rsp), %r11
    xorq    -104(%rsp), %rcx
    orq %rdx, %r10
    rorx    $62, %r8, %r8
    xorq    %r9, %r10
    xorq    -96(%rsp), %rcx
    movq    -80(%rsp), %r9
    xorq    -120(%rsp), %r9
    movq    %r10, -88(%rsp)
    andq    %rdi, %rsi
    xorq    %r8, %rsi
    xorq    %r14, %rcx
    movq    %r8, %r14
    xorq    %rbx, %r9
    orq %rdi, %r14
    xorq    %r10, %r9
    movq    %r8, %r10
    xorq    %r12, %r14
    andq    %r12, %r10
    movq    (%rsp), %r12
    xorq    -24(%rsp), %r12
    xorq    %rdx, %r10
    xorq    16(%rsp), %r12
    xorq    -32(%rsp), %r9
    xorq    %r10, %r11
    xorq    -48(%rsp), %r12
    xorq    %r14, %rax
    xorq    %r15, %r11
    rorx    $63, %rax, %r8
    xorq    %rbp, %r11
    xorq    -56(%rsp), %r11
    rorx    $63, %r9, %rdi
    xorq    %r9, %r8
    rorx    $63, %rcx, %r9
    xorq    %rsi, %r12
    xorq    %rax, %r9
    movq    -120(%rsp), %rax
    xorq    %r12, %rdi
    rorx    $63, %r12, %r12
    xorq    %r8, %rbp
    xorq    %r11, %r12
    rorx    $63, %r11, %rdx
    movq    -16(%rsp), %r11
    xorq    %rcx, %rdx
    movabsq $-9223372036854742902, %rcx
    xorq    %r12, %r13
    xorq    %rdx, %rax
    rorx    $21, %rbp, %rbp
    rorx    $43, %r13, %r13
    xorq    %rdi, %r11
    rorx    $20, %rax, %rax
    xorq    %r9, %rsi
    xorq    %r11, %rcx
    rorx    $50, %rsi, %rsi
    xorq    %rdx, %rbx
    movq    %rcx, -16(%rsp)
    movq    %rax, %rcx
    rorx    $19, %rbx, %rbx
    orq %rbp, %rcx
    xorq    %rcx, -16(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    xorq    %r8, %r10
    orq %r13, %rcx
    rorx    $3, %r10, %r10
    xorq    %rax, %rcx
    andq    %r11, %rax
    movq    %rcx, -64(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rax
    andq    %r13, %rcx
    movq    %rax, 56(%rsp)
    movq    (%rsp), %rax
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    -72(%rsp), %rsi
    movq    %rcx, 8(%rsp)
    movq    -104(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r9, %rax
    xorq    %r13, %rbp
    movq    %rbx, %r13
    xorq    %r12, %rsi
    rorx    $44, %rax, %rax
    movq    %rbp, 24(%rsp)
    xorq    %rdi, %rcx
    rorx    $36, %rsi, %rsi
    movq    %r10, %rbp
    rorx    $61, %rcx, %rcx
    notq    %rbp
    movq    %rcx, %r11
    andq    %rcx, %r13
    orq %rbx, %rbp
    orq %rax, %r11
    xorq    %rax, %r13
    andq    %rsi, %rax
    xorq    %rsi, %r11
    xorq    %r10, %rax
    xorq    %rcx, %rbp
    movq    %r11, -104(%rsp)
    movq    %r10, %r11
    movq    %rax, 48(%rsp)
    orq %rsi, %r11
    movq    -40(%rsp), %rsi
    movq    %r8, %rax
    xorq    %rbx, %r11
    xorq    %r15, %rax
    movq    -32(%rsp), %rcx
    movq    %r11, (%rsp)
    movq    -48(%rsp), %r11
    rorx    $58, %rax, %rax
    movq    -112(%rsp), %r15
    xorq    %r12, %rsi
    movq    %r13, -72(%rsp)
    rorx    $39, %rsi, %rsi
    xorq    %rdx, %rcx
    movq    %rbp, -120(%rsp)
    xorq    %r9, %r11
    movq    %rsi, %r13
    rorx    $63, %rcx, %rcx
    xorq    %rdi, %r15
    rorx    $56, %r11, %r11
    orq %rax, %r13
    rorx    $46, %r15, %r10
    movq    %r11, %r15
    xorq    %rcx, %r13
    movq    %r10, %rbx
    andq    %rsi, %r15
    notq    %r11
    xorq    %rax, %r15
    orq %rcx, %rbx
    movq    %r13, -112(%rsp)
    xorq    %r11, %rbx
    movq    %r15, -48(%rsp)
    movq    %r11, %r15
    movq    -80(%rsp), %r11
    movq    -56(%rsp), %r13
    andq    %rax, %rcx
    movq    %rbx, -40(%rsp)
    movq    40(%rsp), %rbx
    xorq    %r10, %rcx
    movq    -24(%rsp), %rax
    xorq    %r12, %r14
    andq    %r10, %r15
    xorq    %rdx, %r11
    xorq    %r8, %r13
    rorx    $8, %r14, %r10
    rorx    $54, %r11, %r11
    rorx    $49, %r13, %r13
    xorq    %rdi, %rbx
    movq    %r11, %rbp
    xorq    %rsi, %r15
    movq    %rcx, 64(%rsp)
    orq %r13, %rbp
    notq    %r13
    xorq    %r9, %rax
    rorx    $28, %rbx, %rbx
    movq    %r11, %rsi
    movq    -64(%rsp), %rcx
    movq    %r13, %r14
    xorq    -72(%rsp), %rcx
    rorx    $37, %rax, %rax
    andq    %rbx, %rsi
    xorq    -48(%rsp), %rcx
    orq %r10, %r14
    xorq    %rax, %rsi
    xorq    %r11, %r14
    xorq    %rbx, %rbp
    movq    %rsi, -56(%rsp)
    movq    %r14, -80(%rsp)
    movq    %r10, %r14
    movq    -16(%rsp), %rsi
    xorq    -104(%rsp), %rsi
    andq    %rax, %r14
    xorq    -112(%rsp), %rsi
    orq %rax, %rbx
    xorq    %r13, %r14
    xorq    %r10, %rbx
    xorq    -56(%rsp), %rsi
    xorq    %rbp, %rcx
    xorq    32(%rsp), %r8
    xorq    -8(%rsp), %r12
    xorq    16(%rsp), %r9
    xorq    -96(%rsp), %rdi
    xorq    -88(%rsp), %rdx
    rorx    $9, %r12, %r12
    rorx    $2, %r8, %r8
    rorx    $23, %rdi, %r13
    rorx    $25, %r9, %r9
    movq    %r12, %rax
    rorx    $62, %rdx, %r10
    movq    %r13, %rdx
    notq    %rax
    orq %r9, %rdx
    movq    -120(%rsp), %rdi
    movq    %rax, %r11
    xorq    %rax, %rdx
    movq    %r10, %rax
    andq    %r8, %r12
    xorq    %rdx, %rcx
    movq    %rdx, -96(%rsp)
    movq    %r10, %rdx
    orq %r8, %rdx
    andq    %r13, %rax
    xorq    %r15, %rdi
    xorq    %r13, %rdx
    xorq    8(%rsp), %rdi
    movq    48(%rsp), %r13
    movq    %rdx, -88(%rsp)
    movq    (%rsp), %rdx
    xorq    %r10, %r12
    xorq    24(%rsp), %rdx
    xorq    -80(%rsp), %rdi
    andq    %r9, %r11
    xorq    %r12, %r13
    xorq    56(%rsp), %r13
    xorq    %r8, %r11
    xorq    %r9, %rax
    xorq    64(%rsp), %r13
    xorq    %r11, %rsi
    rorx    $63, %rcx, %r10
    xorq    %r14, %rdx
    xorq    -40(%rsp), %rdx
    xorq    %rax, %rdi
    xorq    -88(%rsp), %rdx
    rorx    $63, %rdi, %r8
    xorq    %rsi, %r8
    xorq    %rbx, %r13
    xorq    %r13, %r10
    rorx    $63, %r13, %r13
    xorq    %rdi, %r13
    movabsq $-9223372034707259392, %rdi
    rorx    $63, %rdx, %r9
    xorq    %rcx, %r9
    rorx    $63, %rsi, %rcx
    movq    -72(%rsp), %rsi
    xorq    %rdx, %rcx
    movq    -16(%rsp), %rdx
    xorq    %r8, %rsi
    xorq    %r10, %rdx
    xorq    %r9, %r15
    rorx    $20, %rsi, %rsi
    rorx    $21, %r15, %r15
    xorq    %rdx, %rdi
    xorq    %r13, %r14
    movq    %rdi, -16(%rsp)
    movq    %r15, %rdi
    rorx    $43, %r14, %r14
    orq %rsi, %rdi
    xorq    %rdi, -16(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    xorq    %rcx, %r12
    xorq    %r9, %rax
    orq %r14, %rdi
    rorx    $50, %r12, %r12
    rorx    $3, %rax, %rax
    xorq    %rsi, %rdi
    andq    %rdx, %rsi
    xorq    %r8, %rbp
    movq    %rdi, -32(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rsi
    andq    %r14, %rdi
    movq    %rsi, -24(%rsp)
    movq    48(%rsp), %rsi
    xorq    %r15, %rdi
    movq    %r12, %r15
    rorx    $19, %rbp, %rbp
    orq %rdx, %r15
    movq    -112(%rsp), %rdx
    movq    %rdi, 32(%rsp)
    xorq    %r14, %r15
    movq    24(%rsp), %rdi
    xorq    %rcx, %rsi
    movq    %r15, -72(%rsp)
    movq    %rax, %r15
    movq    %rbp, %r14
    xorq    %r10, %rdx
    notq    %r15
    rorx    $44, %rsi, %rsi
    rorx    $61, %rdx, %rdx
    xorq    %r13, %rdi
    orq %rbp, %r15
    rorx    $36, %rdi, %rdi
    xorq    %rdx, %r15
    andq    %rdx, %r14
    movq    %rdx, %r12
    movq    %rax, %rdx
    xorq    %rsi, %r14
    orq %rsi, %r12
    orq %rdi, %rdx
    andq    %rdi, %rsi
    xorq    %rdi, %r12
    xorq    %rbp, %rdx
    xorq    %rax, %rsi
    movq    -40(%rsp), %rbp
    movq    -120(%rsp), %rax
    movq    -64(%rsp), %rdi
    movq    %rdx, 16(%rsp)
    movq    %rsi, 24(%rsp)
    xorq    -72(%rsp), %rdx
    xorq    -24(%rsp), %rsi
    xorq    %r9, %rax
    movq    %r14, -8(%rsp)
    xorq    %r8, %rdi
    xorq    %r13, %rbp
    xorq    %rcx, %rbx
    rorx    $56, %rbx, %rbx
    xorq    %r10, %r11
    rorx    $39, %rbp, %rbp
    movq    %rbx, %r14
    notq    %rbx
    rorx    $46, %r11, %r11
    movq    %r15, -112(%rsp)
    movq    %rbx, %r15
    rorx    $58, %rax, %rax
    andq    %rbp, %r14
    andq    %r11, %r15
    rorx    $63, %rdi, %rdi
    xorq    %rax, %r14
    xorq    %rbp, %r15
    movq    %r12, 40(%rsp)
    movq    %rbp, %r12
    movq    %r11, %rbp
    movq    %r14, -40(%rsp)
    orq %rdi, %rbp
    movq    -48(%rsp), %r14
    orq %rax, %r12
    xorq    %rbx, %rbp
    andq    %rdi, %rax
    movq    -104(%rsp), %rbx
    xorq    %r11, %rax
    movq    56(%rsp), %r11
    xorq    %rdi, %r12
    xorq    %rbp, %rdx
    xorq    %rax, %rsi
    xorq    %r8, %r14
    movq    %rbp, 48(%rsp)
    movq    %rax, 72(%rsp)
    rorx    $54, %r14, %r14
    movq    -80(%rsp), %rbp
    movq    40(%rsp), %rax
    xorq    %r10, %rbx
    xorq    -16(%rsp), %rax
    movq    %r12, -120(%rsp)
    xorq    %rcx, %r11
    xorq    -120(%rsp), %rax
    movq    -88(%rsp), %rdi
    rorx    $28, %rbx, %rbx
    movq    %r14, %r12
    rorx    $37, %r11, %r11
    xorq    %r9, %rbp
    andq    %rbx, %r12
    rorx    $49, %rbp, %rbp
    xorq    %r11, %r12
    xorq    %r13, %rdi
    rorx    $8, %rdi, %rdi
    xorq    %r12, %rax
    movq    %r12, -104(%rsp)
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, -80(%rsp)
    orq %rdi, %rbp
    orq %r14, %r12
    xorq    %r14, %rbp
    movq    %rdi, %r14
    xorq    %rbx, %r12
    movq    %rbp, -88(%rsp)
    andq    %r11, %r14
    xorq    -80(%rsp), %r14
    orq %r11, %rbx
    xorq    %r14, %rdx
    xorq    %rdi, %rbx
    xorq    -56(%rsp), %r10
    xorq    (%rsp), %r13
    xorq    64(%rsp), %rcx
    xorq    %rbx, %rsi
    xorq    -96(%rsp), %r8
    movq    %rbx, -80(%rsp)
    xorq    8(%rsp), %r9
    rorx    $23, %r10, %r10
    rorx    $9, %r13, %r13
    rorx    $25, %rcx, %rcx
    movq    %r10, %rbp
    movq    %r13, %rdi
    orq %rcx, %rbp
    rorx    $62, %r8, %r8
    notq    %rdi
    rorx    $2, %r9, %r9
    movq    %r8, %r11
    xorq    %rdi, %rbp
    movq    %rdi, %rbx
    movq    -40(%rsp), %rdi
    xorq    -8(%rsp), %rdi
    movq    %rbp, -96(%rsp)
    andq    %rcx, %rbx
    xorq    %r9, %rbx
    orq %r9, %r11
    andq    %r9, %r13
    xorq    %rbx, %rax
    xorq    %r10, %r11
    xorq    %r8, %r13
    xorq    %r11, %rdx
    xorq    %r13, %rsi
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    -32(%rsp), %rdi
    andq    %r10, %rbp
    rorx    $63, %rdx, %r10
    xorq    %rcx, %rbp
    movq    32(%rsp), %rcx
    rorx    $63, %rdi, %r9
    xorq    %rdi, %r10
    xorq    %rbp, %rcx
    xorq    -112(%rsp), %rcx
    xorq    %rsi, %r9
    rorx    $63, %rsi, %rsi
    xorq    %r15, %rcx
    xorq    -88(%rsp), %rcx
    rorx    $63, %rcx, %r8
    xorq    %rcx, %rsi
    movq    -8(%rsp), %rcx
    xorq    %rax, %r8
    rorx    $63, %rax, %rax
    xorq    %rdx, %rax
    movq    -16(%rsp), %rdx
    xorq    %r9, %rdx
    xorq    %r10, %r15
    xorq    %r8, %rcx
    movq    %rdx, %rdi
    rorx    $21, %r15, %r15
    rorx    $20, %rcx, %rcx
    xorq    $32907, %rdi
    xorq    %rsi, %r14
    xorq    %rax, %r13
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    rorx    $43, %r14, %r14
    orq %rcx, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    rorx    $50, %r13, %r13
    xorq    %r10, %rbp
    orq %r14, %rdi
    rorx    $3, %rbp, %rbp
    xorq    %r8, %r12
    xorq    %rcx, %rdi
    andq    %rdx, %rcx
    rorx    $19, %r12, %r12
    movq    %rdi, -64(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rcx
    andq    %r14, %rdi
    movq    %rcx, -8(%rsp)
    movq    24(%rsp), %rcx
    xorq    %r15, %rdi
    movq    %r13, %r15
    orq %rdx, %r15
    movq    %rdi, -16(%rsp)
    movq    -120(%rsp), %rdi
    xorq    %r14, %r15
    movq    -72(%rsp), %rdx
    xorq    %rax, %rcx
    movq    %r15, -48(%rsp)
    movq    %rbp, %r15
    movq    %r12, %r14
    xorq    %r9, %rdi
    notq    %r15
    rorx    $44, %rcx, %rcx
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rdx
    orq %r12, %r15
    xorq    %rdi, %r15
    rorx    $36, %rdx, %rdx
    andq    %rdi, %r14
    movq    %rdi, %r13
    movq    %rbp, %rdi
    xorq    %rcx, %r14
    orq %rcx, %r13
    orq %rdx, %rdi
    andq    %rdx, %rcx
    xorq    %r12, %rdi
    xorq    %rbp, %rcx
    xorq    %rdx, %r13
    movq    %rdi, (%rsp)
    movq    %rcx, 8(%rsp)
    movq    -32(%rsp), %rdx
    movq    -112(%rsp), %rdi
    movq    48(%rsp), %rcx
    movq    -80(%rsp), %r12
    movq    %r13, -72(%rsp)
    movq    %r14, -120(%rsp)
    xorq    %r8, %rdx
    xorq    %r10, %rdi
    xorq    %rsi, %rcx
    rorx    $58, %rdi, %rdi
    xorq    %rax, %r12
    rorx    $39, %rcx, %rcx
    rorx    $63, %rdx, %rdx
    rorx    $56, %r12, %r12
    movq    %rcx, %rbp
    xorq    %r9, %rbx
    movq    %r12, %r13
    orq %rdi, %rbp
    notq    %r12
    rorx    $46, %rbx, %rbx
    xorq    %rdx, %rbp
    andq    %rcx, %r13
    movq    %rbx, %r14
    movq    %rbp, -112(%rsp)
    movq    %r12, %rbp
    xorq    %rdi, %r13
    andq    %rbx, %rbp
    orq %rdx, %r14
    andq    %rdx, %rdi
    xorq    %rcx, %rbp
    movq    (%rsp), %rcx
    xorq    -48(%rsp), %rcx
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    movq    -40(%rsp), %r12
    movq    %rdi, 56(%rsp)
    movq    40(%rsp), %rdi
    xorq    %rsi, %r11
    movq    -24(%rsp), %rdx
    movq    %r14, 24(%rsp)
    rorx    $8, %r11, %r11
    xorq    %r14, %rcx
    xorq    %r8, %r12
    movq    -88(%rsp), %r14
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    movq    %r13, -80(%rsp)
    rorx    $28, %rdi, %rdi
    xorq    %rax, %rdx
    movq    %r12, %rbx
    rorx    $37, %rdx, %rdx
    xorq    %r10, %r14
    andq    %rdi, %rbx
    xorq    %rdx, %rbx
    rorx    $49, %r14, %r14
    xorq    32(%rsp), %r10
    movq    %rbx, -88(%rsp)
    movq    %r14, %rbx
    notq    %r14
    movq    %r14, %r13
    orq %r12, %rbx
    xorq    16(%rsp), %rsi
    orq %r11, %r13
    xorq    %rdi, %rbx
    orq %rdx, %rdi
    xorq    %r12, %r13
    xorq    %r11, %rdi
    movq    %r13, -24(%rsp)
    movq    %r11, %r13
    movq    %rdi, 40(%rsp)
    andq    %rdx, %r13
    rorx    $9, %rsi, %rsi
    rorx    $2, %r10, %rdi
    xorq    %r14, %r13
    movq    -72(%rsp), %rdx
    movq    -16(%rsp), %r11
    xorq    %r13, %rcx
    xorq    72(%rsp), %rax
    xorq    -104(%rsp), %r9
    xorq    -56(%rsp), %rdx
    xorq    -96(%rsp), %r8
    xorq    -112(%rsp), %rdx
    xorq    -88(%rsp), %rdx
    rorx    $23, %r9, %r12
    rorx    $25, %rax, %rax
    movq    %rsi, %r9
    movq    %r12, %r10
    notq    %r9
    rorx    $62, %r8, %r8
    orq %rax, %r10
    movq    %r9, %r14
    andq    %rdi, %rsi
    xorq    %r9, %r10
    movq    -80(%rsp), %r9
    xorq    -120(%rsp), %r9
    andq    %rax, %r14
    movq    %r10, -96(%rsp)
    xorq    %r8, %rsi
    xorq    %rdi, %r14
    xorq    %r14, %rdx
    movq    %r14, -104(%rsp)
    movq    %r8, %r14
    xorq    %rbx, %r9
    orq %rdi, %r14
    xorq    %r10, %r9
    movq    %r8, %r10
    xorq    %r12, %r14
    andq    %r12, %r10
    movq    8(%rsp), %r12
    xorq    -8(%rsp), %r12
    xorq    %rax, %r10
    xorq    56(%rsp), %r12
    xorq    -64(%rsp), %r9
    xorq    %r10, %r11
    xorq    40(%rsp), %r12
    xorq    %r14, %rcx
    xorq    %r15, %r11
    rorx    $63, %rcx, %r8
    xorq    %rbp, %r11
    xorq    -24(%rsp), %r11
    rorx    $63, %r9, %rdi
    xorq    %r9, %r8
    rorx    $63, %rdx, %r9
    xorq    %rsi, %r12
    xorq    %r12, %rdi
    rorx    $63, %r12, %r12
    rorx    $63, %r11, %rax
    xorq    %rdx, %rax
    xorq    %r11, %r12
    movq    -120(%rsp), %rdx
    movq    -56(%rsp), %r11
    xorq    %rcx, %r9
    movl    $2147483649, %ecx
    xorq    %r8, %rbp
    xorq    %r12, %r13
    xorq    %r9, %rsi
    xorq    %rax, %rdx
    rorx    $21, %rbp, %rbp
    rorx    $43, %r13, %r13
    xorq    %rdi, %r11
    rorx    $20, %rdx, %rdx
    rorx    $50, %rsi, %rsi
    xorq    %r11, %rcx
    xorq    %rax, %rbx
    xorq    %r8, %r10
    movq    %rcx, -56(%rsp)
    movq    %rdx, %rcx
    rorx    $19, %rbx, %rbx
    orq %rbp, %rcx
    xorq    %rcx, -56(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    rorx    $3, %r10, %r10
    orq %r13, %rcx
    xorq    %rdx, %rcx
    andq    %r11, %rdx
    movq    %rcx, -32(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rdx
    andq    %r13, %rcx
    movq    %rdx, 16(%rsp)
    movq    8(%rsp), %rdx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    -48(%rsp), %rsi
    movq    %rcx, 32(%rsp)
    movq    -112(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r9, %rdx
    xorq    %r13, %rbp
    movq    %rbx, %r13
    xorq    %r12, %rsi
    rorx    $44, %rdx, %rdx
    movq    %rbp, -40(%rsp)
    xorq    %rdi, %rcx
    rorx    $36, %rsi, %rsi
    movq    %r10, %rbp
    rorx    $61, %rcx, %rcx
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
    movq    %r11, -48(%rsp)
    movq    %r10, %r11
    movq    -64(%rsp), %r10
    orq %rsi, %r11
    movq    %r8, %rcx
    movq    40(%rsp), %rsi
    xorq    %rbx, %r11
    movq    %rdx, 48(%rsp)
    movq    %r13, -112(%rsp)
    xorq    %rax, %r10
    movq    %r11, 8(%rsp)
    xorq    %r15, %rcx
    movq    24(%rsp), %r11
    movq    -104(%rsp), %r15
    xorq    %r9, %rsi
    rorx    $56, %rsi, %rsi
    rorx    $58, %rcx, %rcx
    rorx    $63, %r10, %r10
    movq    %rbp, -120(%rsp)
    xorq    %r12, %r14
    xorq    %rdi, %r15
    xorq    %r12, %r11
    rorx    $46, %r15, %rdx
    rorx    $39, %r11, %r11
    movq    %rsi, %r15
    andq    %r11, %r15
    notq    %rsi
    movq    %r11, %r13
    xorq    %rcx, %r15
    movq    %rdx, %rbx
    orq %rcx, %r13
    movq    %r15, 40(%rsp)
    movq    %rsi, %r15
    orq %r10, %rbx
    andq    %rdx, %r15
    xorq    %r10, %r13
    xorq    %rsi, %rbx
    xorq    %r11, %r15
    movq    -80(%rsp), %r11
    movq    %r13, -104(%rsp)
    andq    %rcx, %r10
    movq    %rbx, 24(%rsp)
    movq    -24(%rsp), %r13
    movq    -72(%rsp), %rbx
    xorq    %rdx, %r10
    movq    -8(%rsp), %rdx
    xorq    %rax, %r11
    movq    %r10, 64(%rsp)
    movq    -32(%rsp), %rcx
    rorx    $54, %r11, %r11
    xorq    %r8, %r13
    rorx    $8, %r14, %r10
    xorq    %rdi, %rbx
    xorq    %r9, %rdx
    rorx    $49, %r13, %r13
    rorx    $28, %rbx, %rbx
    movq    %r11, %rsi
    movq    %r11, %rbp
    rorx    $37, %rdx, %rdx
    andq    %rbx, %rsi
    orq %r13, %rbp
    notq    %r13
    xorq    %rdx, %rsi
    xorq    -112(%rsp), %rcx
    movq    %r13, %r14
    movq    %rsi, -80(%rsp)
    xorq    40(%rsp), %rcx
    orq %r10, %r14
    movq    -56(%rsp), %rsi
    xorq    -48(%rsp), %rsi
    xorq    %r11, %r14
    xorq    -104(%rsp), %rsi
    xorq    %rbx, %rbp
    xorq    -80(%rsp), %rsi
    movq    %r14, -72(%rsp)
    movq    %r10, %r14
    andq    %rdx, %r14
    xorq    %rbp, %rcx
    xorq    %r13, %r14
    xorq    -88(%rsp), %rdi
    xorq    (%rsp), %r12
    xorq    -96(%rsp), %rax
    xorq    56(%rsp), %r9
    orq %rdx, %rbx
    xorq    -16(%rsp), %r8
    xorq    %r10, %rbx
    rorx    $23, %rdi, %r13
    rorx    $9, %r12, %r12
    movq    -120(%rsp), %rdi
    rorx    $62, %rax, %r10
    rorx    $25, %r9, %r9
    movq    %r12, %rax
    movq    %r13, %rdx
    notq    %rax
    rorx    $2, %r8, %r8
    orq %r9, %rdx
    movq    %rax, %r11
    andq    %r8, %r12
    xorq    %rax, %rdx
    movq    %r10, %rax
    xorq    %r10, %r12
    orq %r8, %rax
    xorq    %rdx, %rcx
    movq    %rdx, -96(%rsp)
    xorq    %r13, %rax
    movq    %r10, %rdx
    xorq    %r15, %rdi
    movq    %rax, -88(%rsp)
    movq    8(%rsp), %rax
    andq    %r13, %rdx
    xorq    -40(%rsp), %rax
    movq    48(%rsp), %r13
    xorq    %r9, %rdx
    xorq    32(%rsp), %rdi
    andq    %r9, %r11
    rorx    $63, %rcx, %r10
    xorq    -72(%rsp), %rdi
    xorq    %r8, %r11
    xorq    %r12, %r13
    xorq    16(%rsp), %r13
    xorq    %r11, %rsi
    xorq    %r14, %rax
    xorq    24(%rsp), %rax
    xorq    64(%rsp), %r13
    xorq    -88(%rsp), %rax
    xorq    %rdx, %rdi
    rorx    $63, %rdi, %r8
    xorq    %rbx, %r13
    rorx    $63, %rax, %r9
    xorq    %r13, %r10
    xorq    %rsi, %r8
    xorq    %rcx, %r9
    rorx    $63, %rsi, %rcx
    movq    -112(%rsp), %rsi
    xorq    %rax, %rcx
    movq    -56(%rsp), %rax
    rorx    $63, %r13, %r13
    xorq    %rdi, %r13
    xorq    %r9, %r15
    movabsq $-9223372034707259263, %rdi
    rorx    $21, %r15, %r15
    xorq    %r8, %rsi
    xorq    %r13, %r14
    xorq    %r10, %rax
    rorx    $20, %rsi, %rsi
    rorx    $43, %r14, %r14
    xorq    %rax, %rdi
    xorq    %rcx, %r12
    xorq    %r9, %rdx
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    rorx    $50, %r12, %r12
    orq %rsi, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    rorx    $3, %rdx, %rdx
    xorq    %r8, %rbp
    orq %r14, %rdi
    rorx    $19, %rbp, %rbp
    xorq    %rsi, %rdi
    andq    %rax, %rsi
    movq    %rdi, -64(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rsi
    andq    %r14, %rdi
    movq    %rsi, -8(%rsp)
    movq    48(%rsp), %rsi
    xorq    %r15, %rdi
    movq    %r12, %r15
    orq %rax, %r15
    movq    -104(%rsp), %rax
    movq    %rdi, -16(%rsp)
    xorq    %r14, %r15
    movq    -40(%rsp), %rdi
    movq    %rbp, %r14
    movq    %r15, -24(%rsp)
    movq    %rdx, %r15
    xorq    %rcx, %rsi
    xorq    %r10, %rax
    notq    %r15
    rorx    $44, %rsi, %rsi
    rorx    $61, %rax, %rax
    xorq    %r13, %rdi
    orq %rbp, %r15
    rorx    $36, %rdi, %rdi
    xorq    %rax, %r15
    andq    %rax, %r14
    movq    %rax, %r12
    movq    %rdx, %rax
    xorq    %rsi, %r14
    orq %rdi, %rax
    orq %rsi, %r12
    movq    %r14, (%rsp)
    xorq    %rbp, %rax
    xorq    %rdi, %r12
    movq    %r15, -112(%rsp)
    movq    %rax, -40(%rsp)
    xorq    -24(%rsp), %rax
    andq    %rdi, %rsi
    xorq    %rdx, %rsi
    movq    %r12, -104(%rsp)
    xorq    %rcx, %rbx
    movq    %rsi, 56(%rsp)
    movq    24(%rsp), %rbp
    rorx    $56, %rbx, %rbx
    movq    -120(%rsp), %rdx
    movq    -32(%rsp), %rdi
    xorq    %r10, %r11
    movq    %rbx, %r14
    notq    %rbx
    rorx    $46, %r11, %r11
    xorq    %r13, %rbp
    movq    %rbx, %r15
    xorq    -8(%rsp), %rsi
    rorx    $39, %rbp, %rbp
    xorq    %r9, %rdx
    xorq    %r8, %rdi
    rorx    $58, %rdx, %rdx
    andq    %rbp, %r14
    andq    %r11, %r15
    rorx    $63, %rdi, %rdi
    xorq    %rdx, %r14
    xorq    %rbp, %r15
    movq    %rbp, %r12
    movq    %r11, %rbp
    movq    %r14, 24(%rsp)
    orq %rdi, %rbp
    movq    40(%rsp), %r14
    orq %rdx, %r12
    xorq    %rbx, %rbp
    andq    %rdi, %rdx
    movq    -48(%rsp), %rbx
    xorq    %r11, %rdx
    movq    16(%rsp), %r11
    xorq    %rdi, %r12
    xorq    %rbp, %rax
    xorq    %rdx, %rsi
    xorq    %r8, %r14
    movq    %rbp, 48(%rsp)
    movq    %rdx, 72(%rsp)
    rorx    $54, %r14, %r14
    movq    -72(%rsp), %rbp
    movq    -104(%rsp), %rdx
    xorq    %r10, %rbx
    xorq    -56(%rsp), %rdx
    movq    %r12, -120(%rsp)
    xorq    %rcx, %r11
    xorq    -120(%rsp), %rdx
    rorx    $28, %rbx, %rbx
    movq    -88(%rsp), %rdi
    movq    %r14, %r12
    rorx    $37, %r11, %r11
    xorq    %r9, %rbp
    andq    %rbx, %r12
    rorx    $49, %rbp, %rbp
    xorq    %r11, %r12
    xorq    %r13, %rdi
    xorq    %r12, %rdx
    movq    %r12, -88(%rsp)
    movq    %rbp, %r12
    rorx    $8, %rdi, %rdi
    orq %r14, %r12
    notq    %rbp
    xorq    %rbx, %r12
    movq    %rbp, -48(%rsp)
    orq %rdi, %rbp
    xorq    8(%rsp), %r13
    xorq    -80(%rsp), %r10
    xorq    %r14, %rbp
    xorq    64(%rsp), %rcx
    orq %r11, %rbx
    movq    %rbp, -72(%rsp)
    xorq    %rdi, %rbx
    movq    %rdi, %r14
    xorq    -96(%rsp), %r8
    andq    %r11, %r14
    xorq    %rbx, %rsi
    xorq    -48(%rsp), %r14
    rorx    $23, %r10, %r10
    rorx    $9, %r13, %r13
    movq    %rbx, -48(%rsp)
    rorx    $25, %rcx, %rcx
    movq    %r13, %rdi
    movq    %r10, %rbp
    notq    %rdi
    orq %rcx, %rbp
    rorx    $62, %r8, %r8
    xorq    %rdi, %rbp
    movq    %rdi, %rbx
    movq    24(%rsp), %rdi
    xorq    (%rsp), %rdi
    movq    %rbp, -96(%rsp)
    andq    %rcx, %rbx
    xorq    32(%rsp), %r9
    movq    %r8, %r11
    xorq    %r14, %rax
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    -64(%rsp), %rdi
    andq    %r10, %rbp
    rorx    $2, %r9, %r9
    xorq    %rcx, %rbp
    movq    -16(%rsp), %rcx
    xorq    %r9, %rbx
    orq %r9, %r11
    andq    %r9, %r13
    xorq    %rbx, %rdx
    xorq    %r10, %r11
    xorq    %r8, %r13
    rorx    $63, %rdi, %r9
    xorq    %rbp, %rcx
    xorq    -112(%rsp), %rcx
    xorq    %r11, %rax
    rorx    $63, %rax, %r10
    xorq    %r15, %rcx
    xorq    -72(%rsp), %rcx
    xorq    %r13, %rsi
    xorq    %rsi, %r9
    rorx    $63, %rsi, %rsi
    xorq    %rdi, %r10
    xorq    %r10, %r15
    movabsq $-9223372036854743031, %rdi
    xorq    %r10, %rbp
    rorx    $21, %r15, %r15
    rorx    $3, %rbp, %rbp
    rorx    $63, %rcx, %r8
    xorq    %rcx, %rsi
    movq    (%rsp), %rcx
    xorq    %rdx, %r8
    rorx    $63, %rdx, %rdx
    xorq    %rsi, %r14
    xorq    %rax, %rdx
    movq    -56(%rsp), %rax
    rorx    $43, %r14, %r14
    xorq    %r8, %rcx
    xorq    %rdx, %r13
    xorq    %r8, %r12
    rorx    $20, %rcx, %rcx
    rorx    $50, %r13, %r13
    rorx    $19, %r12, %r12
    xorq    %r9, %rax
    xorq    %rax, %rdi
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    orq %rcx, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rcx, %rdi
    andq    %rax, %rcx
    movq    %rdi, -32(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rcx
    andq    %r14, %rdi
    movq    %rcx, 40(%rsp)
    movq    56(%rsp), %rcx
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, 32(%rsp)
    movq    -120(%rsp), %rdi
    orq %rax, %r15
    xorq    %r14, %r15
    movq    -24(%rsp), %rax
    xorq    %rdx, %rcx
    movq    %r15, -80(%rsp)
    movq    %rbp, %r15
    rorx    $44, %rcx, %rcx
    xorq    %r9, %rdi
    notq    %r15
    movq    %r12, %r14
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rax
    orq %r12, %r15
    movq    %rdi, %r13
    xorq    %rdi, %r15
    rorx    $36, %rax, %rax
    orq %rcx, %r13
    andq    %rdi, %r14
    movq    %rbp, %rdi
    xorq    %rax, %r13
    xorq    %rcx, %r14
    orq %rax, %rdi
    andq    %rax, %rcx
    xorq    %r12, %rdi
    movq    -64(%rsp), %rax
    xorq    %rbp, %rcx
    movq    %rdi, (%rsp)
    movq    -112(%rsp), %rdi
    movq    %rcx, 8(%rsp)
    movq    48(%rsp), %rcx
    xorq    %r9, %rbx
    movq    -48(%rsp), %r12
    xorq    %r8, %rax
    movq    %r13, -24(%rsp)
    xorq    %r10, %rdi
    rorx    $63, %rax, %rax
    rorx    $46, %rbx, %rbx
    xorq    %rsi, %rcx
    rorx    $58, %rdi, %rdi
    movq    %r14, -120(%rsp)
    rorx    $39, %rcx, %rcx
    xorq    %rdx, %r12
    movq    %rbx, %r14
    movq    %rcx, %rbp
    rorx    $56, %r12, %r12
    orq %rax, %r14
    orq %rdi, %rbp
    movq    %r12, %r13
    notq    %r12
    xorq    %rax, %rbp
    andq    %rcx, %r13
    xorq    %r12, %r14
    movq    %rbp, -112(%rsp)
    movq    %r12, %rbp
    xorq    %rdi, %r13
    andq    %rbx, %rbp
    andq    %rax, %rdi
    movq    24(%rsp), %r12
    xorq    %rcx, %rbp
    movq    (%rsp), %rcx
    xorq    -80(%rsp), %rcx
    xorq    %rbx, %rdi
    movq    -8(%rsp), %rax
    movq    %r14, 16(%rsp)
    movq    %rdi, 56(%rsp)
    movq    -104(%rsp), %rdi
    xorq    %r8, %r12
    rorx    $54, %r12, %r12
    xorq    %rsi, %r11
    movq    %r13, -48(%rsp)
    xorq    %r14, %rcx
    movq    -72(%rsp), %r14
    xorq    %rdx, %rax
    xorq    %r9, %rdi
    movq    %r12, %rbx
    rorx    $37, %rax, %rax
    rorx    $28, %rdi, %rdi
    rorx    $8, %r11, %r11
    xorq    %r10, %r14
    andq    %rdi, %rbx
    xorq    %rax, %rbx
    rorx    $49, %r14, %r14
    movq    %rbx, -104(%rsp)
    movq    %r14, %rbx
    notq    %r14
    movq    %r14, %r13
    orq %r12, %rbx
    orq %r11, %r13
    xorq    %rdi, %rbx
    xorq    %r12, %r13
    movq    %r13, -72(%rsp)
    movq    %r11, %r13
    andq    %rax, %r13
    xorq    %r14, %r13
    xorq    -40(%rsp), %rsi
    xorq    -88(%rsp), %r9
    xorq    72(%rsp), %rdx
    xorq    -16(%rsp), %r10
    orq %rax, %rdi
    xorq    %r11, %rdi
    xorq    -96(%rsp), %r8
    xorq    %r13, %rcx
    movq    %rdi, -8(%rsp)
    rorx    $9, %rsi, %rsi
    rorx    $23, %r9, %r12
    movq    %rsi, %r9
    rorx    $25, %rdx, %rdx
    rorx    $2, %r10, %rdi
    notq    %r9
    movq    %r12, %r10
    rorx    $62, %r8, %r8
    movq    %r9, %r14
    orq %rdx, %r10
    andq    %rdi, %rsi
    andq    %rdx, %r14
    xorq    %r9, %r10
    xorq    %r8, %rsi
    xorq    %rdi, %r14
    movq    %r14, -96(%rsp)
    movq    -48(%rsp), %r9
    xorq    -120(%rsp), %r9
    movq    -24(%rsp), %rax
    xorq    -56(%rsp), %rax
    movq    %r10, -88(%rsp)
    xorq    -112(%rsp), %rax
    movq    32(%rsp), %r11
    xorq    -104(%rsp), %rax
    xorq    %rbx, %r9
    xorq    %r10, %r9
    movq    %r8, %r10
    xorq    -32(%rsp), %r9
    andq    %r12, %r10
    xorq    %r14, %rax
    xorq    %rdx, %r10
    movq    %r8, %r14
    xorq    %r10, %r11
    orq %rdi, %r14
    xorq    %r12, %r14
    xorq    %r15, %r11
    movq    8(%rsp), %r12
    xorq    40(%rsp), %r12
    xorq    %r14, %rcx
    xorq    %rbp, %r11
    xorq    -72(%rsp), %r11
    xorq    56(%rsp), %r12
    rorx    $63, %r9, %rdi
    xorq    -8(%rsp), %r12
    rorx    $63, %rcx, %r8
    xorq    %r9, %r8
    rorx    $63, %rax, %r9
    xorq    %rcx, %r9
    xorq    %r8, %rbp
    xorq    %r8, %r10
    rorx    $63, %r11, %rdx
    rorx    $21, %rbp, %rbp
    rorx    $3, %r10, %r10
    xorq    %rsi, %r12
    xorq    %rax, %rdx
    movq    -120(%rsp), %rax
    xorq    %r12, %rdi
    rorx    $63, %r12, %r12
    xorq    %r9, %rsi
    xorq    %r11, %r12
    movq    -56(%rsp), %r11
    rorx    $50, %rsi, %rsi
    xorq    %rdx, %rax
    xorq    %r12, %r13
    xorq    %rdx, %rbx
    rorx    $20, %rax, %rax
    rorx    $43, %r13, %r13
    rorx    $19, %rbx, %rbx
    xorq    %rdi, %r11
    movq    %r11, %rcx
    xorb    $-118, %cl
    movq    %rcx, -56(%rsp)
    movq    %rax, %rcx
    orq %rbp, %rcx
    xorq    %rcx, -56(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    orq %r13, %rcx
    xorq    %rax, %rcx
    andq    %r11, %rax
    movq    %rcx, -64(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rax
    andq    %r13, %rcx
    movq    %rax, 24(%rsp)
    movq    8(%rsp), %rax
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    -80(%rsp), %rsi
    movq    %rcx, -16(%rsp)
    movq    -112(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r9, %rax
    xorq    %r13, %rbp
    movq    %rbx, %r13
    xorq    %r12, %rsi
    rorx    $44, %rax, %rax
    movq    %rbp, -40(%rsp)
    xorq    %rdi, %rcx
    rorx    $36, %rsi, %rsi
    movq    %r10, %rbp
    rorx    $61, %rcx, %rcx
    notq    %rbp
    movq    %rcx, %r11
    andq    %rcx, %r13
    orq %rax, %r11
    xorq    %rax, %r13
    xorq    %rsi, %r11
    orq %rbx, %rbp
    andq    %rsi, %rax
    movq    %r11, -80(%rsp)
    movq    %r10, %r11
    xorq    %rcx, %rbp
    orq %rsi, %r11
    movq    %r8, %rcx
    movq    -8(%rsp), %rsi
    xorq    %rbx, %r11
    xorq    %r15, %rcx
    movq    -96(%rsp), %r15
    movq    %r11, 8(%rsp)
    movq    16(%rsp), %r11
    xorq    %r10, %rax
    movq    -32(%rsp), %r10
    xorq    %r9, %rsi
    rorx    $58, %rcx, %rcx
    xorq    %rdi, %r15
    rorx    $56, %rsi, %rsi
    movq    %r13, -112(%rsp)
    xorq    %r12, %r11
    movq    %rax, 48(%rsp)
    rorx    $46, %r15, %rax
    rorx    $39, %r11, %r11
    xorq    %rdx, %r10
    movq    %rsi, %r15
    movq    %r11, %r13
    rorx    $63, %r10, %r10
    andq    %r11, %r15
    orq %rcx, %r13
    xorq    %rcx, %r15
    notq    %rsi
    xorq    %r10, %r13
    movq    %rbp, -120(%rsp)
    movq    %rax, %rbx
    movq    %r13, -96(%rsp)
    movq    %r15, -8(%rsp)
    movq    %rsi, %r15
    andq    %rax, %r15
    orq %r10, %rbx
    andq    %rcx, %r10
    xorq    %r11, %r15
    xorq    %rsi, %rbx
    movq    -48(%rsp), %r11
    movq    %rbx, 16(%rsp)
    movq    -24(%rsp), %rbx
    xorq    %rax, %r10
    movq    40(%rsp), %rax
    movq    -72(%rsp), %r13
    xorq    %r12, %r14
    xorq    %rdx, %r11
    movq    %r10, 64(%rsp)
    rorx    $8, %r14, %r10
    rorx    $54, %r11, %r11
    xorq    %rdi, %rbx
    movq    -64(%rsp), %rcx
    xorq    %r9, %rax
    rorx    $28, %rbx, %rbx
    movq    %r11, %rsi
    rorx    $37, %rax, %rax
    xorq    %r8, %r13
    andq    %rbx, %rsi
    xorq    %rax, %rsi
    rorx    $49, %r13, %r13
    movq    %r11, %rbp
    movq    %rsi, -72(%rsp)
    orq %r13, %rbp
    movq    -56(%rsp), %rsi
    notq    %r13
    xorq    -80(%rsp), %rsi
    xorq    %rbx, %rbp
    xorq    -96(%rsp), %rsi
    movq    %r13, %r14
    xorq    -72(%rsp), %rsi
    xorq    -112(%rsp), %rcx
    orq %r10, %r14
    xorq    -104(%rsp), %rdi
    xorq    (%rsp), %r12
    xorq    %r11, %r14
    xorq    -88(%rsp), %rdx
    xorq    56(%rsp), %r9
    orq %rax, %rbx
    movq    %r14, -48(%rsp)
    movq    %r10, %r14
    xorq    -8(%rsp), %rcx
    andq    %rax, %r14
    xorq    32(%rsp), %r8
    xorq    %r10, %rbx
    xorq    %r13, %r14
    rorx    $9, %r12, %r12
    rorx    $23, %rdi, %r13
    rorx    $62, %rdx, %r10
    rorx    $25, %r9, %r9
    movq    %r12, %rax
    movq    %r13, %rdx
    notq    %rax
    xorq    %rbp, %rcx
    orq %r9, %rdx
    rorx    $2, %r8, %r8
    movq    -120(%rsp), %rdi
    xorq    %rax, %rdx
    movq    %rax, %r11
    movq    %r10, %rax
    xorq    %rdx, %rcx
    movq    %rdx, -104(%rsp)
    movq    %r10, %rdx
    orq %r8, %rdx
    xorq    %r15, %rdi
    xorq    -16(%rsp), %rdi
    xorq    %r13, %rdx
    xorq    -48(%rsp), %rdi
    andq    %r9, %r11
    movq    %rdx, -88(%rsp)
    movq    8(%rsp), %rdx
    andq    %r13, %rax
    xorq    -40(%rsp), %rdx
    movq    48(%rsp), %r13
    xorq    %r8, %r11
    xorq    %r9, %rax
    andq    %r8, %r12
    xorq    %r11, %rsi
    xorq    %rax, %rdi
    rorx    $63, %rdi, %r8
    xorq    %r14, %rdx
    xorq    16(%rsp), %rdx
    xorq    -88(%rsp), %rdx
    xorq    %r10, %r12
    rorx    $63, %rcx, %r10
    xorq    %r12, %r13
    xorq    24(%rsp), %r13
    xorq    %rsi, %r8
    xorq    64(%rsp), %r13
    xorq    %r8, %rbp
    rorx    $19, %rbp, %rbp
    rorx    $63, %rdx, %r9
    xorq    %rcx, %r9
    rorx    $63, %rsi, %rcx
    movq    -112(%rsp), %rsi
    xorq    %rdx, %rcx
    movq    -56(%rsp), %rdx
    xorq    %rbx, %r13
    xorq    %r13, %r10
    rorx    $63, %r13, %r13
    xorq    %r9, %r15
    xorq    %rdi, %r13
    rorx    $21, %r15, %r15
    xorq    %r8, %rsi
    xorq    %r10, %rdx
    rorx    $20, %rsi, %rsi
    xorq    %r13, %r14
    movq    %rdx, %rdi
    rorx    $43, %r14, %r14
    xorq    %rcx, %r12
    xorb    $-120, %dil
    rorx    $50, %r12, %r12
    xorq    %r9, %rax
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    rorx    $3, %rax, %rax
    orq %rsi, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rsi, %rdi
    andq    %rdx, %rsi
    movq    %rdi, -32(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rsi
    andq    %r14, %rdi
    movq    %rsi, 40(%rsp)
    movq    48(%rsp), %rsi
    xorq    %r15, %rdi
    movq    %r12, %r15
    orq %rdx, %r15
    movq    -96(%rsp), %rdx
    movq    %rdi, 32(%rsp)
    movq    -40(%rsp), %rdi
    xorq    %r14, %r15
    xorq    %rcx, %rsi
    movq    %r15, -24(%rsp)
    rorx    $44, %rsi, %rsi
    movq    %rax, %r15
    xorq    %r10, %rdx
    movq    %rbp, %r14
    notq    %r15
    rorx    $61, %rdx, %rdx
    xorq    %r13, %rdi
    movq    %rdx, %r12
    rorx    $36, %rdi, %rdi
    orq %rsi, %r12
    xorq    %rdi, %r12
    andq    %rdx, %r14
    orq %rbp, %r15
    xorq    %rdx, %r15
    movq    %rax, %rdx
    xorq    %rsi, %r14
    orq %rdi, %rdx
    andq    %rdi, %rsi
    xorq    %rcx, %rbx
    xorq    %rbp, %rdx
    movq    16(%rsp), %rbp
    xorq    %rax, %rsi
    movq    -120(%rsp), %rax
    movq    -64(%rsp), %rdi
    rorx    $56, %rbx, %rbx
    xorq    %r10, %r11
    movq    %r14, (%rsp)
    movq    %rbx, %r14
    xorq    %r13, %rbp
    notq    %rbx
    rorx    $46, %r11, %r11
    rorx    $39, %rbp, %rbp
    xorq    %r9, %rax
    movq    %r15, -112(%rsp)
    movq    %rbx, %r15
    xorq    %r8, %rdi
    rorx    $58, %rax, %rax
    andq    %rbp, %r14
    andq    %r11, %r15
    rorx    $63, %rdi, %rdi
    xorq    %rax, %r14
    xorq    %rbp, %r15
    movq    %r12, -96(%rsp)
    movq    %rbp, %r12
    movq    %r11, %rbp
    movq    %rdx, -40(%rsp)
    orq %rdi, %rbp
    xorq    -24(%rsp), %rdx
    movq    %r14, 16(%rsp)
    movq    -8(%rsp), %r14
    xorq    %rbx, %rbp
    orq %rax, %r12
    movq    -80(%rsp), %rbx
    andq    %rdi, %rax
    movq    %rsi, 56(%rsp)
    xorq    %r11, %rax
    movq    24(%rsp), %r11
    xorq    40(%rsp), %rsi
    xorq    %r8, %r14
    xorq    %rdi, %r12
    xorq    %rbp, %rdx
    rorx    $54, %r14, %r14
    xorq    %r10, %rbx
    movq    -88(%rsp), %rdi
    movq    %rbp, 48(%rsp)
    movq    -48(%rsp), %rbp
    xorq    %rcx, %r11
    movq    %r12, -120(%rsp)
    rorx    $28, %rbx, %rbx
    movq    %r14, %r12
    rorx    $37, %r11, %r11
    andq    %rbx, %r12
    xorq    %rax, %rsi
    xorq    %r9, %rbp
    xorq    %r13, %rdi
    movq    %rax, 72(%rsp)
    xorq    %r11, %r12
    movq    -96(%rsp), %rax
    xorq    -56(%rsp), %rax
    xorq    -120(%rsp), %rax
    rorx    $49, %rbp, %rbp
    rorx    $8, %rdi, %rdi
    movq    %r12, -88(%rsp)
    xorq    %r12, %rax
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, -64(%rsp)
    orq %rdi, %rbp
    orq %r14, %r12
    xorq    %r14, %rbp
    xorq    %rbx, %r12
    orq %r11, %rbx
    movq    %rbp, -80(%rsp)
    xorq    8(%rsp), %r13
    xorq    %rdi, %rbx
    xorq    -72(%rsp), %r10
    xorq    64(%rsp), %rcx
    movq    %rdi, %r14
    xorq    -104(%rsp), %r8
    movq    %rbx, -48(%rsp)
    xorq    %rbx, %rsi
    xorq    -16(%rsp), %r9
    andq    %r11, %r14
    xorq    -64(%rsp), %r14
    rorx    $9, %r13, %r13
    rorx    $23, %r10, %r10
    rorx    $25, %rcx, %rcx
    movq    %r13, %rdi
    movq    %r10, %rbp
    notq    %rdi
    rorx    $62, %r8, %r8
    orq %rcx, %rbp
    movq    %rdi, %rbx
    rorx    $2, %r9, %r9
    xorq    %rdi, %rbp
    movq    16(%rsp), %rdi
    xorq    (%rsp), %rdi
    movq    %rbp, -104(%rsp)
    andq    %rcx, %rbx
    xorq    %r14, %rdx
    xorq    %r9, %rbx
    movq    %r8, %r11
    xorq    %rbx, %rax
    xorq    %r12, %rdi
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    -32(%rsp), %rdi
    andq    %r10, %rbp
    xorq    %rcx, %rbp
    movq    32(%rsp), %rcx
    xorq    %rbp, %rcx
    xorq    -112(%rsp), %rcx
    xorq    %r15, %rcx
    xorq    -80(%rsp), %rcx
    orq %r9, %r11
    andq    %r9, %r13
    xorq    %r10, %r11
    rorx    $63, %rdi, %r9
    xorq    %r8, %r13
    xorq    %r11, %rdx
    xorq    %r13, %rsi
    rorx    $63, %rdx, %r10
    rorx    $63, %rcx, %r8
    xorq    %rsi, %r9
    rorx    $63, %rsi, %rsi
    xorq    %rax, %r8
    rorx    $63, %rax, %rax
    xorq    %rcx, %rsi
    xorq    %rdx, %rax
    movq    -56(%rsp), %rdx
    movq    (%rsp), %rcx
    xorq    %rdi, %r10
    movl    $2147516425, %edi
    xorq    %rsi, %r14
    xorq    %r10, %r15
    rorx    $43, %r14, %r14
    xorq    %rax, %r13
    xorq    %r9, %rdx
    rorx    $21, %r15, %r15
    xorq    %r8, %rcx
    xorq    %rdx, %rdi
    rorx    $20, %rcx, %rcx
    rorx    $50, %r13, %r13
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    xorq    %r10, %rbp
    orq %rcx, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    rorx    $3, %rbp, %rbp
    xorq    %r8, %r12
    orq %r14, %rdi
    rorx    $19, %r12, %r12
    xorq    %rcx, %rdi
    andq    %rdx, %rcx
    movq    %rdi, -64(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rcx
    andq    %r14, %rdi
    movq    %rcx, -8(%rsp)
    movq    56(%rsp), %rcx
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, -16(%rsp)
    movq    -120(%rsp), %rdi
    orq %rdx, %r15
    movq    -24(%rsp), %rdx
    xorq    %r14, %r15
    xorq    %rax, %rcx
    movq    %r15, -72(%rsp)
    movq    %rbp, %r15
    rorx    $44, %rcx, %rcx
    xorq    %r9, %rdi
    movq    %r12, %r14
    notq    %r15
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rdx
    movq    %rdi, %r13
    rorx    $36, %rdx, %rdx
    orq %rcx, %r13
    andq    %rdi, %r14
    orq %r12, %r15
    xorq    %rdi, %r15
    xorq    %rcx, %r14
    movq    %rbp, %rdi
    andq    %rdx, %rcx
    orq %rdx, %rdi
    xorq    %rdx, %r13
    xorq    %rbp, %rcx
    xorq    %r12, %rdi
    movq    -32(%rsp), %rdx
    movq    %rcx, 8(%rsp)
    movq    48(%rsp), %rcx
    xorq    %r9, %rbx
    movq    %rdi, (%rsp)
    movq    -112(%rsp), %rdi
    rorx    $46, %rbx, %rbx
    movq    -48(%rsp), %r12
    xorq    %r8, %rdx
    movq    %r13, -24(%rsp)
    xorq    %rsi, %rcx
    rorx    $63, %rdx, %rdx
    movq    %r14, -120(%rsp)
    rorx    $39, %rcx, %rcx
    xorq    %r10, %rdi
    movq    %rbx, %r14
    rorx    $58, %rdi, %rdi
    xorq    %rax, %r12
    movq    %rcx, %rbp
    orq %rdi, %rbp
    rorx    $56, %r12, %r12
    orq %rdx, %r14
    xorq    %rdx, %rbp
    movq    %r12, %r13
    notq    %r12
    movq    %rbp, -112(%rsp)
    movq    %r12, %rbp
    andq    %rcx, %r13
    andq    %rbx, %rbp
    xorq    %rdi, %r13
    andq    %rdx, %rdi
    xorq    %rcx, %rbp
    movq    (%rsp), %rcx
    xorq    -72(%rsp), %rcx
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    movq    16(%rsp), %r12
    movq    %rdi, 56(%rsp)
    movq    -96(%rsp), %rdi
    xorq    %rsi, %r11
    movq    40(%rsp), %rdx
    movq    %r14, 24(%rsp)
    rorx    $8, %r11, %r11
    xorq    %r14, %rcx
    xorq    %r8, %r12
    movq    -80(%rsp), %r14
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    movq    %r13, -48(%rsp)
    xorq    %rax, %rdx
    rorx    $28, %rdi, %rdi
    movq    %r12, %rbx
    rorx    $37, %rdx, %rdx
    xorq    %r10, %r14
    andq    %rdi, %rbx
    xorq    %rdx, %rbx
    rorx    $49, %r14, %r14
    xorq    -88(%rsp), %r9
    xorq    -40(%rsp), %rsi
    xorq    32(%rsp), %r10
    movq    %rbx, -96(%rsp)
    xorq    72(%rsp), %rax
    movq    %r14, %rbx
    notq    %r14
    orq %r12, %rbx
    xorq    -104(%rsp), %r8
    movq    %r14, %r13
    xorq    %rdi, %rbx
    orq %rdx, %rdi
    orq %r11, %r13
    xorq    %r11, %rdi
    rorx    $9, %rsi, %rsi
    xorq    %r12, %r13
    rorx    $23, %r9, %r12
    rorx    $25, %rax, %rax
    movq    %r13, -80(%rsp)
    movq    %rdi, 40(%rsp)
    movq    %r11, %r13
    rorx    $2, %r10, %rdi
    movq    %rsi, %r9
    movq    %r12, %r10
    andq    %rdx, %r13
    notq    %r9
    orq %rax, %r10
    xorq    %r9, %r10
    xorq    %r14, %r13
    movq    %r9, %r14
    movq    -48(%rsp), %r9
    xorq    -120(%rsp), %r9
    rorx    $62, %r8, %r8
    movq    -24(%rsp), %rdx
    xorq    -56(%rsp), %rdx
    andq    %rax, %r14
    xorq    -112(%rsp), %rdx
    movq    %r10, -88(%rsp)
    xorq    %rdi, %r14
    xorq    -96(%rsp), %rdx
    movq    -16(%rsp), %r11
    xorq    %r13, %rcx
    xorq    %rbx, %r9
    movq    %r14, -104(%rsp)
    xorq    %r10, %r9
    movq    %r8, %r10
    xorq    -64(%rsp), %r9
    andq    %r12, %r10
    xorq    %rax, %r10
    xorq    %r14, %rdx
    movq    %r8, %r14
    xorq    %r10, %r11
    xorq    %r15, %r11
    xorq    %rbp, %r11
    xorq    -80(%rsp), %r11
    orq %rdi, %r14
    xorq    %r12, %r14
    movq    8(%rsp), %r12
    xorq    -8(%rsp), %r12
    xorq    56(%rsp), %r12
    andq    %rdi, %rsi
    xorq    %r14, %rcx
    xorq    40(%rsp), %r12
    xorq    %r8, %rsi
    rorx    $63, %r9, %rdi
    rorx    $63, %r11, %rax
    rorx    $63, %rcx, %r8
    xorq    %r9, %r8
    xorq    %rdx, %rax
    rorx    $63, %rdx, %r9
    movq    -120(%rsp), %rdx
    xorq    %rcx, %r9
    movl    $2147483658, %ecx
    xorq    %rsi, %r12
    xorq    %r8, %rbp
    xorq    %r9, %rsi
    xorq    %r12, %rdi
    rorx    $63, %r12, %r12
    rorx    $21, %rbp, %rbp
    xorq    %r11, %r12
    movq    -56(%rsp), %r11
    xorq    %rax, %rdx
    rorx    $20, %rdx, %rdx
    xorq    %r12, %r13
    rorx    $50, %rsi, %rsi
    rorx    $43, %r13, %r13
    xorq    %rdi, %r11
    xorq    %r11, %rcx
    movq    %rcx, -56(%rsp)
    movq    %rdx, %rcx
    orq %rbp, %rcx
    xorq    %rcx, -56(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    orq %r13, %rcx
    xorq    %rdx, %rcx
    andq    %r11, %rdx
    movq    %rcx, -32(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rdx
    andq    %r13, %rcx
    movq    %rdx, 16(%rsp)
    movq    8(%rsp), %rdx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    -72(%rsp), %rsi
    movq    %rcx, 32(%rsp)
    movq    -112(%rsp), %rcx
    orq %r11, %rbp
    xorq    %r9, %rdx
    xorq    %r13, %rbp
    xorq    %r12, %rsi
    rorx    $44, %rdx, %rdx
    movq    %rbp, -40(%rsp)
    xorq    %rdi, %rcx
    xorq    %rax, %rbx
    xorq    %r8, %r10
    rorx    $61, %rcx, %rcx
    rorx    $36, %rsi, %rsi
    rorx    $3, %r10, %r10
    movq    %rcx, %r11
    rorx    $19, %rbx, %rbx
    movq    %r10, %rbp
    orq %rdx, %r11
    movq    %rbx, %r13
    notq    %rbp
    xorq    %rsi, %r11
    andq    %rcx, %r13
    orq %rbx, %rbp
    movq    %r11, -72(%rsp)
    movq    %r10, %r11
    xorq    %rdx, %r13
    orq %rsi, %r11
    xorq    %rcx, %rbp
    andq    %rsi, %rdx
    xorq    %rbx, %r11
    movq    40(%rsp), %rsi
    movq    %r8, %rcx
    xorq    %r15, %rcx
    movq    %r11, 8(%rsp)
    movq    -104(%rsp), %r15
    movq    24(%rsp), %r11
    xorq    %r10, %rdx
    movq    -64(%rsp), %r10
    xorq    %r9, %rsi
    movq    %rdx, 48(%rsp)
    rorx    $58, %rcx, %rcx
    xorq    %rdi, %r15
    rorx    $56, %rsi, %rsi
    movq    %r13, -112(%rsp)
    xorq    %r12, %r11
    rorx    $46, %r15, %rdx
    movq    %rsi, %r15
    rorx    $39, %r11, %r11
    xorq    %rax, %r10
    notq    %rsi
    andq    %r11, %r15
    rorx    $63, %r10, %r10
    movq    %rdx, %rbx
    xorq    %rcx, %r15
    orq %r10, %rbx
    movq    %r11, %r13
    movq    %r15, 40(%rsp)
    movq    %rsi, %r15
    xorq    %rsi, %rbx
    andq    %rdx, %r15
    orq %rcx, %r13
    movq    %rbx, 24(%rsp)
    xorq    %r11, %r15
    movq    -48(%rsp), %r11
    xorq    %r10, %r13
    movq    -24(%rsp), %rbx
    andq    %rcx, %r10
    movq    %r13, -104(%rsp)
    xorq    %rdx, %r10
    movq    -80(%rsp), %r13
    movq    -8(%rsp), %rdx
    xorq    %rax, %r11
    movq    %rbp, -120(%rsp)
    xorq    %r12, %r14
    rorx    $54, %r11, %r11
    xorq    %rdi, %rbx
    movq    %r10, 64(%rsp)
    xorq    %r9, %rdx
    rorx    $28, %rbx, %rbx
    xorq    %r8, %r13
    movq    %r11, %rsi
    rorx    $37, %rdx, %rdx
    rorx    $49, %r13, %r13
    andq    %rbx, %rsi
    movq    %r11, %rbp
    rorx    $8, %r14, %r10
    xorq    %rdx, %rsi
    orq %r13, %rbp
    notq    %r13
    movq    %r13, %r14
    movq    %rsi, -80(%rsp)
    movq    -56(%rsp), %rsi
    orq %r10, %r14
    xorq    -72(%rsp), %rsi
    movq    -32(%rsp), %rcx
    xorq    %r11, %r14
    xorq    -104(%rsp), %rsi
    xorq    -112(%rsp), %rcx
    xorq    -80(%rsp), %rsi
    xorq    40(%rsp), %rcx
    xorq    %rbx, %rbp
    movq    %r14, -48(%rsp)
    xorq    -96(%rsp), %rdi
    movq    %r10, %r14
    xorq    (%rsp), %r12
    xorq    -88(%rsp), %rax
    andq    %rdx, %r14
    xorq    56(%rsp), %r9
    xorq    %r13, %r14
    orq %rdx, %rbx
    xorq    %r10, %rbx
    xorq    -16(%rsp), %r8
    xorq    %rbp, %rcx
    rorx    $23, %rdi, %r13
    movq    -120(%rsp), %rdi
    rorx    $9, %r12, %r12
    rorx    $62, %rax, %r10
    movq    %r13, %rdx
    rorx    $25, %r9, %r9
    movq    %r12, %rax
    notq    %rax
    orq %r9, %rdx
    xorq    %r15, %rdi
    xorq    32(%rsp), %rdi
    xorq    %rax, %rdx
    movq    %rax, %r11
    xorq    -48(%rsp), %rdi
    xorq    %rdx, %rcx
    movq    %rdx, -96(%rsp)
    movq    %r10, %rdx
    rorx    $2, %r8, %r8
    andq    %r9, %r11
    andq    %r13, %rdx
    movq    %r10, %rax
    xorq    %r8, %r11
    xorq    %r9, %rdx
    orq %r8, %rax
    xorq    %r11, %rsi
    xorq    %rdx, %rdi
    xorq    %r13, %rax
    movq    48(%rsp), %r13
    movq    %rax, -88(%rsp)
    movq    8(%rsp), %rax
    andq    %r8, %r12
    xorq    -40(%rsp), %rax
    xorq    %r10, %r12
    rorx    $63, %rcx, %r10
    xorq    %r12, %r13
    xorq    16(%rsp), %r13
    rorx    $63, %rdi, %r8
    xorq    64(%rsp), %r13
    xorq    %rsi, %r8
    xorq    %r14, %rax
    xorq    24(%rsp), %rax
    xorq    -88(%rsp), %rax
    xorq    %rbx, %r13
    xorq    %r13, %r10
    rorx    $63, %r13, %r13
    xorq    %rdi, %r13
    movl    $2147516555, %edi
    rorx    $63, %rax, %r9
    xorq    %r13, %r14
    xorq    %rcx, %r9
    rorx    $63, %rsi, %rcx
    movq    -112(%rsp), %rsi
    xorq    %rax, %rcx
    movq    -56(%rsp), %rax
    xorq    %r9, %r15
    rorx    $21, %r15, %r15
    rorx    $43, %r14, %r14
    xorq    %rcx, %r12
    xorq    %r8, %rsi
    rorx    $50, %r12, %r12
    xorq    %r10, %rax
    rorx    $20, %rsi, %rsi
    xorq    %rax, %rdi
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    orq %rsi, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rsi, %rdi
    andq    %rax, %rsi
    movq    %rdi, -64(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rsi
    andq    %r14, %rdi
    movq    %rsi, -8(%rsp)
    movq    48(%rsp), %rsi
    xorq    %r15, %rdi
    movq    %r12, %r15
    movq    %rdi, -16(%rsp)
    movq    -40(%rsp), %rdi
    orq %rax, %r15
    xorq    %r14, %r15
    movq    -104(%rsp), %rax
    movq    %r15, -24(%rsp)
    xorq    %r13, %rdi
    xorq    %rcx, %rsi
    xorq    %r9, %rdx
    rorx    $3, %rdx, %rdx
    xorq    %r8, %rbp
    xorq    %r10, %rax
    movq    %rdx, %r15
    rorx    $19, %rbp, %rbp
    rorx    $61, %rax, %rax
    notq    %r15
    movq    %rbp, %r14
    rorx    $36, %rdi, %rdi
    orq %rbp, %r15
    rorx    $44, %rsi, %rsi
    andq    %rax, %r14
    xorq    %rax, %r15
    movq    %rax, %r12
    movq    %rdx, %rax
    xorq    %rsi, %r14
    orq %rsi, %r12
    orq %rdi, %rax
    andq    %rdi, %rsi
    xorq    %rdi, %r12
    xorq    %rbp, %rax
    xorq    %rdx, %rsi
    movq    %r12, -104(%rsp)
    movq    %r14, (%rsp)
    movq    %r15, -112(%rsp)
    movq    %rax, -40(%rsp)
    xorq    %rcx, %rbx
    xorq    -24(%rsp), %rax
    movq    %rsi, 56(%rsp)
    rorx    $56, %rbx, %rbx
    movq    24(%rsp), %rbp
    movq    -120(%rsp), %rdx
    xorq    %r10, %r11
    movq    -32(%rsp), %rdi
    movq    %rbx, %r14
    notq    %rbx
    rorx    $46, %r11, %r11
    movq    %rbx, %r15
    xorq    -8(%rsp), %rsi
    xorq    %r13, %rbp
    xorq    %r9, %rdx
    andq    %r11, %r15
    rorx    $39, %rbp, %rbp
    xorq    %r8, %rdi
    rorx    $58, %rdx, %rdx
    rorx    $63, %rdi, %rdi
    andq    %rbp, %r14
    movq    %rbp, %r12
    xorq    %rdx, %r14
    xorq    %rbp, %r15
    orq %rdx, %r12
    movq    %r11, %rbp
    andq    %rdi, %rdx
    movq    %r14, 24(%rsp)
    xorq    %r11, %rdx
    orq %rdi, %rbp
    movq    16(%rsp), %r11
    movq    40(%rsp), %r14
    xorq    %rbx, %rbp
    movq    -72(%rsp), %rbx
    xorq    %rdi, %r12
    xorq    %rbp, %rax
    xorq    %rdx, %rsi
    movq    -88(%rsp), %rdi
    xorq    %rcx, %r11
    movq    %rbp, 48(%rsp)
    xorq    %r8, %r14
    movq    -48(%rsp), %rbp
    movq    %rdx, 72(%rsp)
    movq    -104(%rsp), %rdx
    xorq    -56(%rsp), %rdx
    rorx    $54, %r14, %r14
    movq    %r12, -120(%rsp)
    xorq    %r10, %rbx
    xorq    -120(%rsp), %rdx
    rorx    $28, %rbx, %rbx
    movq    %r14, %r12
    rorx    $37, %r11, %r11
    xorq    %r9, %rbp
    xorq    %r13, %rdi
    andq    %rbx, %r12
    xorq    8(%rsp), %r13
    xorq    -80(%rsp), %r10
    xorq    %r11, %r12
    xorq    64(%rsp), %rcx
    rorx    $49, %rbp, %rbp
    rorx    $8, %rdi, %rdi
    xorq    %r12, %rdx
    movq    %r12, -88(%rsp)
    movq    %rbp, %r12
    notq    %rbp
    orq %r14, %r12
    xorq    -96(%rsp), %r8
    movq    %rbp, -48(%rsp)
    orq %rdi, %rbp
    xorq    %rbx, %r12
    xorq    %r14, %rbp
    rorx    $23, %r10, %r10
    orq %r11, %rbx
    rorx    $9, %r13, %r13
    xorq    %rdi, %rbx
    rorx    $25, %rcx, %rcx
    movq    %rbp, -72(%rsp)
    movq    %rdi, %r14
    movq    %r10, %rbp
    movq    %r13, %rdi
    orq %rcx, %rbp
    andq    %r11, %r14
    notq    %rdi
    xorq    -48(%rsp), %r14
    xorq    %rbx, %rsi
    xorq    %rdi, %rbp
    xorq    32(%rsp), %r9
    movq    %rbx, -48(%rsp)
    movq    %rdi, %rbx
    movq    24(%rsp), %rdi
    xorq    (%rsp), %rdi
    rorx    $62, %r8, %r8
    andq    %rcx, %rbx
    movq    %rbp, -96(%rsp)
    xorq    %r14, %rax
    movq    %r8, %r11
    rorx    $2, %r9, %r9
    xorq    %r12, %rdi
    xorq    %r9, %rbx
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    %rbx, %rdx
    xorq    -64(%rsp), %rdi
    andq    %r10, %rbp
    orq %r9, %r11
    xorq    %rcx, %rbp
    movq    -16(%rsp), %rcx
    andq    %r9, %r13
    xorq    %r10, %r11
    xorq    %r8, %r13
    xorq    %r11, %rax
    xorq    %r13, %rsi
    xorq    %rbp, %rcx
    xorq    -112(%rsp), %rcx
    rorx    $63, %rdi, %r9
    xorq    %rsi, %r9
    rorx    $63, %rax, %r10
    rorx    $63, %rsi, %rsi
    xorq    %rdi, %r10
    movabsq $-9223372036854775669, %rdi
    xorq    %r15, %rcx
    xorq    -72(%rsp), %rcx
    xorq    %r10, %r15
    rorx    $21, %r15, %r15
    rorx    $63, %rcx, %r8
    xorq    %rcx, %rsi
    movq    (%rsp), %rcx
    xorq    %rdx, %r8
    rorx    $63, %rdx, %rdx
    xorq    %rsi, %r14
    xorq    %rax, %rdx
    movq    -56(%rsp), %rax
    rorx    $43, %r14, %r14
    xorq    %r8, %rcx
    xorq    %rdx, %r13
    rorx    $20, %rcx, %rcx
    rorx    $50, %r13, %r13
    xorq    %r9, %rax
    xorq    %rax, %rdi
    movq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    orq %rcx, %rdi
    xorq    %rdi, -56(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    orq %r14, %rdi
    xorq    %rcx, %rdi
    andq    %rax, %rcx
    movq    %rdi, -32(%rsp)
    movq    %r13, %rdi
    andq    %r14, %rdi
    xorq    %r15, %rdi
    movq    %r13, %r15
    orq %rax, %r15
    movq    %rdi, 32(%rsp)
    movq    -120(%rsp), %rdi
    xorq    %r14, %r15
    xorq    %r13, %rcx
    movq    -24(%rsp), %rax
    movq    %rcx, 40(%rsp)
    movq    56(%rsp), %rcx
    xorq    %r8, %r12
    rorx    $19, %r12, %r12
    xorq    %r9, %rdi
    xorq    %r10, %rbp
    rorx    $61, %rdi, %rdi
    movq    %r12, %r14
    xorq    %rsi, %rax
    xorq    %rdx, %rcx
    andq    %rdi, %r14
    rorx    $36, %rax, %rax
    rorx    $44, %rcx, %rcx
    rorx    $3, %rbp, %rbp
    movq    %rdi, %r13
    xorq    %rcx, %r14
    orq %rcx, %r13
    andq    %rax, %rcx
    movq    %r14, -120(%rsp)
    movq    %rbp, %r14
    xorq    %rbp, %rcx
    notq    %r14
    movq    %r15, -80(%rsp)
    movq    %rcx, 8(%rsp)
    orq %r12, %r14
    movq    %rbp, %r15
    movq    48(%rsp), %rcx
    xorq    %rdi, %r14
    orq %rax, %r15
    movq    -112(%rsp), %rdi
    xorq    %rax, %r13
    xorq    %r12, %r15
    movq    -64(%rsp), %rax
    movq    -48(%rsp), %r12
    xorq    %rsi, %rcx
    movq    %r13, -24(%rsp)
    rorx    $39, %rcx, %rcx
    xorq    %r10, %rdi
    xorq    %r9, %rbx
    xorq    %r8, %rax
    rorx    $58, %rdi, %rdi
    movq    %rcx, %rbp
    xorq    %rdx, %r12
    rorx    $63, %rax, %rax
    orq %rdi, %rbp
    rorx    $56, %r12, %r12
    xorq    %rax, %rbp
    rorx    $46, %rbx, %rbx
    movq    %r12, %r13
    notq    %r12
    movq    %rbp, -112(%rsp)
    movq    %r12, %rbp
    movq    %r15, (%rsp)
    andq    %rcx, %r13
    andq    %rbx, %rbp
    movq    %rbx, %r15
    xorq    %rdi, %r13
    xorq    %rcx, %rbp
    orq %rax, %r15
    andq    %rax, %rdi
    movq    (%rsp), %rcx
    xorq    -80(%rsp), %rcx
    xorq    %r12, %r15
    xorq    %rbx, %rdi
    movq    -8(%rsp), %rax
    movq    24(%rsp), %r12
    movq    %rdi, 56(%rsp)
    movq    -104(%rsp), %rdi
    movq    %r13, -48(%rsp)
    movq    -72(%rsp), %r13
    xorq    %r15, %rcx
    xorq    %rdx, %rax
    xorq    %r8, %r12
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    rorx    $37, %rax, %rax
    rorx    $28, %rdi, %rdi
    movq    %r12, %rbx
    xorq    %r10, %r13
    andq    %rdi, %rbx
    xorq    %rsi, %r11
    xorq    -88(%rsp), %r9
    xorq    %rax, %rbx
    xorq    -40(%rsp), %rsi
    rorx    $49, %r13, %r13
    xorq    -16(%rsp), %r10
    movq    %rbx, -104(%rsp)
    movq    %r13, %rbx
    xorq    72(%rsp), %rdx
    orq %r12, %rbx
    notq    %r13
    rorx    $8, %r11, %r11
    xorq    %rdi, %rbx
    movq    %r15, 16(%rsp)
    orq %rax, %rdi
    movq    %r13, %r15
    rorx    $23, %r9, %r9
    xorq    %r11, %rdi
    orq %r11, %r15
    rorx    $9, %rsi, %rsi
    xorq    %r12, %r15
    rorx    $25, %rdx, %rdx
    movq    %r11, %r12
    movq    %rdi, -8(%rsp)
    movq    %r9, %r11
    rorx    $2, %r10, %rdi
    movq    %rsi, %r10
    andq    %rax, %r12
    orq %rdx, %r11
    notq    %r10
    movq    -24(%rsp), %rax
    xorq    -56(%rsp), %rax
    xorq    %r10, %r11
    xorq    -96(%rsp), %r8
    xorq    -112(%rsp), %rax
    xorq    -104(%rsp), %rax
    movq    %r15, -72(%rsp)
    movq    %r10, %r15
    movq    %r11, -96(%rsp)
    movq    -48(%rsp), %r11
    andq    %rdx, %r15
    xorq    -120(%rsp), %r11
    xorq    %r13, %r12
    xorq    %rdi, %r15
    rorx    $62, %r8, %r8
    xorq    %r12, %rcx
    movq    %r8, %r10
    xorq    %r15, %rax
    movq    %r8, %r13
    xorq    %rbx, %r11
    xorq    -96(%rsp), %r11
    xorq    -32(%rsp), %r11
    andq    %r9, %r10
    orq %rdi, %r13
    xorq    %rdx, %r10
    movq    32(%rsp), %rdx
    andq    %rdi, %rsi
    movq    8(%rsp), %rdi
    xorq    40(%rsp), %rdi
    xorq    %r8, %rsi
    xorq    56(%rsp), %rdi
    xorq    %r9, %r13
    xorq    -8(%rsp), %rdi
    xorq    %r10, %rdx
    rorx    $63, %r11, %r9
    xorq    %r14, %rdx
    xorq    %r13, %rcx
    movq    %r13, -88(%rsp)
    xorq    %rbp, %rdx
    xorq    -72(%rsp), %rdx
    rorx    $63, %rcx, %r13
    xorq    %r11, %r13
    movabsq $-9223372036854742903, %r11
    xorq    %rsi, %rdi
    xorq    %r13, %rbp
    xorq    %rdi, %r9
    rorx    $63, %rdi, %rdi
    rorx    $21, %rbp, %rbp
    xorq    %rdx, %rdi
    rorx    $63, %rdx, %r8
    rorx    $63, %rax, %rdx
    xorq    %rax, %r8
    xorq    %rcx, %rdx
    movq    -56(%rsp), %rax
    movq    -120(%rsp), %rcx
    xorq    %rdi, %r12
    xorq    %rdx, %rsi
    rorx    $43, %r12, %r12
    rorx    $50, %rsi, %rsi
    xorq    %r9, %rax
    xorq    %r8, %rcx
    xorq    %rax, %r11
    rorx    $20, %rcx, %rcx
    movq    %r11, -56(%rsp)
    movq    %rcx, %r11
    orq %rbp, %r11
    xorq    %r11, -56(%rsp)
    movq    %rbp, %r11
    notq    %r11
    orq %r12, %r11
    xorq    %rcx, %r11
    movq    %r11, -64(%rsp)
    movq    %rsi, %r11
    andq    %r12, %r11
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    orq %rax, %rbp
    movq    %r11, -16(%rsp)
    xorq    %r12, %rbp
    andq    %rax, %rcx
    movq    8(%rsp), %rax
    xorq    %rsi, %rcx
    movq    -80(%rsp), %rsi
    xorq    %r8, %rbx
    movq    %rcx, 24(%rsp)
    movq    -112(%rsp), %rcx
    rorx    $19, %rbx, %rbx
    xorq    %rdx, %rax
    xorq    %r13, %r10
    movq    %rbx, %r12
    xorq    %rdi, %rsi
    rorx    $44, %rax, %rax
    rorx    $3, %r10, %r10
    xorq    %r9, %rcx
    rorx    $36, %rsi, %rsi
    movq    %rbp, -40(%rsp)
    rorx    $61, %rcx, %rcx
    movq    %r10, %rbp
    xorq    %r9, %r15
    movq    %rcx, %r11
    andq    %rcx, %r12
    notq    %rbp
    orq %rax, %r11
    xorq    %rax, %r12
    andq    %rsi, %rax
    xorq    %rsi, %r11
    xorq    %r10, %rax
    orq %rbx, %rbp
    movq    %r11, -80(%rsp)
    movq    %r10, %r11
    movq    %rax, 48(%rsp)
    orq %rsi, %r11
    movq    16(%rsp), %r10
    movq    -8(%rsp), %rax
    xorq    %rbx, %r11
    xorq    %rcx, %rbp
    movq    %r13, %rcx
    movq    %r11, 8(%rsp)
    movq    -32(%rsp), %r11
    rorx    $46, %r15, %r15
    xorq    %rdi, %r10
    xorq    %rdx, %rax
    xorq    %r14, %rcx
    rorx    $39, %r10, %r10
    rorx    $56, %rax, %rax
    rorx    $58, %rcx, %rcx
    xorq    %r8, %r11
    movq    %r12, -112(%rsp)
    movq    %rax, %r14
    rorx    $63, %r11, %r11
    notq    %rax
    movq    %r10, %r12
    movq    %r15, %rsi
    orq %rcx, %r12
    andq    %r10, %r14
    orq %r11, %rsi
    movq    %rax, %rbx
    xorq    %r11, %r12
    xorq    %rcx, %r14
    xorq    %rax, %rsi
    andq    %r15, %rbx
    xorq    %r10, %rbx
    movq    %rbp, -120(%rsp)
    movq    %r12, -8(%rsp)
    movq    %r14, 16(%rsp)
    movq    %rsi, 64(%rsp)
    andq    %rcx, %r11
    movq    -48(%rsp), %r10
    movq    40(%rsp), %rax
    xorq    %r15, %r11
    movq    -24(%rsp), %rcx
    movq    -72(%rsp), %r15
    movq    %r11, 72(%rsp)
    movq    -56(%rsp), %r11
    xorq    %rdx, %rax
    xorq    %r8, %r10
    xorq    -80(%rsp), %r11
    rorx    $54, %r10, %r10
    xorq    %r9, %rcx
    movq    -88(%rsp), %r14
    rorx    $28, %rcx, %rcx
    xorq    %r13, %r15
    movq    %r10, %rbp
    rorx    $37, %rax, %rax
    andq    %rcx, %rbp
    rorx    $49, %r15, %r15
    movq    %r10, %rsi
    xorq    %rax, %rbp
    xorq    %rdi, %r14
    xorq    %r12, %r11
    orq %r15, %rsi
    xorq    (%rsp), %rdi
    notq    %r15
    rorx    $8, %r14, %r14
    xorq    %rbp, %r11
    movq    %rbp, -88(%rsp)
    movq    %r15, %rbp
    xorq    -104(%rsp), %r9
    orq %r14, %rbp
    xorq    -96(%rsp), %r8
    xorq    56(%rsp), %rdx
    xorq    %r10, %rbp
    xorq    32(%rsp), %r13
    movq    -64(%rsp), %r12
    movq    %rbp, -72(%rsp)
    xorq    -112(%rsp), %r12
    movq    %r14, %rbp
    rorx    $9, %rdi, %rdi
    xorq    %rcx, %rsi
    xorq    16(%rsp), %r12
    andq    %rax, %rbp
    orq %rax, %rcx
    movq    %rdi, %rax
    rorx    $23, %r9, %r9
    notq    %rax
    xorq    %r14, %rcx
    rorx    $25, %rdx, %rdx
    rorx    $62, %r8, %r8
    movq    %rax, %r14
    movq    %r9, %r10
    xorq    %r15, %rbp
    rorx    $2, %r13, %r13
    andq    %rdx, %r14
    orq %rdx, %r10
    movq    %r8, %r15
    xorq    %rax, %r10
    xorq    %r13, %r14
    xorq    %rsi, %r12
    andq    %r9, %r15
    xorq    %r14, %r11
    xorq    %r10, %r12
    xorq    %rdx, %r15
    movq    %r8, %rdx
    movq    -120(%rsp), %rax
    orq %r13, %rdx
    andq    %r13, %rdi
    movq    48(%rsp), %r13
    xorq    %r9, %rdx
    xorq    %r8, %rdi
    movq    %r10, -104(%rsp)
    movq    %rdx, -96(%rsp)
    movq    8(%rsp), %rdx
    xorq    %rbx, %rax
    xorq    -40(%rsp), %rdx
    xorq    -16(%rsp), %rax
    xorq    %rdi, %r13
    xorq    -72(%rsp), %rax
    xorq    24(%rsp), %r13
    rorx    $63, %r12, %r10
    xorq    72(%rsp), %r13
    xorq    %rbp, %rdx
    xorq    64(%rsp), %rdx
    xorq    -96(%rsp), %rdx
    xorq    %r15, %rax
    rorx    $63, %rax, %r8
    xorq    %rcx, %r13
    xorq    %r11, %r8
    rorx    $63, %r11, %r11
    xorq    %r13, %r10
    rorx    $63, %r13, %r13
    rorx    $63, %rdx, %r9
    xorq    %rdx, %r11
    movq    -112(%rsp), %rdx
    xorq    %r12, %r9
    movq    -56(%rsp), %r12
    xorq    %rax, %r13
    xorq    %r9, %rbx
    xorq    %r13, %rbp
    xorq    %r11, %rdi
    rorx    $21, %rbx, %rbx
    xorq    %r8, %rdx
    rorx    $43, %rbp, %rbp
    xorq    %r10, %r12
    rorx    $20, %rdx, %rdx
    movq    %rbx, %rax
    movq    %r12, -48(%rsp)
    movabsq $-9223372036854743037, %r12
    xorq    -48(%rsp), %r12
    orq %rdx, %rax
    rorx    $50, %rdi, %rdi
    xorq    %rax, %r12
    movq    %rbx, %rax
    notq    %rax
    orq %rbp, %rax
    xorq    %rdx, %rax
    movq    %rax, -32(%rsp)
    movq    %rdi, %rax
    andq    %rbp, %rax
    xorq    %rbx, %rax
    andq    -48(%rsp), %rdx
    movq    -48(%rsp), %rbx
    movq    %rax, -56(%rsp)
    movq    48(%rsp), %rax
    xorq    %r9, %r15
    rorx    $3, %r15, %r15
    xorq    %r8, %rsi
    xorq    %r11, %rcx
    orq %rdi, %rbx
    rorx    $19, %rsi, %rsi
    rorx    $56, %rcx, %rcx
    xorq    %rdi, %rdx
    movq    -40(%rsp), %rdi
    xorq    %r11, %rax
    movq    %rdx, -48(%rsp)
    movq    -8(%rsp), %rdx
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, 32(%rsp)
    movq    %rsi, %rbx
    xorq    %r13, %rdi
    xorq    %r10, %r14
    xorq    %r10, %rdx
    rorx    $36, %rdi, %rdi
    rorx    $46, %r14, %r14
    rorx    $61, %rdx, %rdx
    movq    %rdx, %rbp
    andq    %rdx, %rbx
    orq %rax, %rbp
    xorq    %rax, %rbx
    andq    %rdi, %rax
    xorq    %rdi, %rbp
    xorq    %r15, %rax
    movq    %rbx, 40(%rsp)
    movq    %rbp, -24(%rsp)
    movq    %r15, %rbp
    movq    64(%rsp), %rbx
    notq    %rbp
    movq    %rax, (%rsp)
    orq %rsi, %rbp
    xorq    %rdx, %rbp
    movq    %r15, %rdx
    xorq    %r13, %rbx
    orq %rdi, %rdx
    rorx    $39, %rbx, %rbx
    movq    %rcx, %r15
    xorq    %rsi, %rdx
    movq    %rax, %rsi
    movq    -120(%rsp), %rax
    andq    %rbx, %r15
    movq    -64(%rsp), %rdi
    notq    %rcx
    xorq    -48(%rsp), %rsi
    movq    %rbp, -112(%rsp)
    movq    %rbx, %rbp
    xorq    %r9, %rax
    movq    %rdx, -8(%rsp)
    xorq    32(%rsp), %rdx
    rorx    $58, %rax, %rax
    xorq    %r8, %rdi
    xorq    %rax, %r15
    rorx    $63, %rdi, %rdi
    orq %rax, %rbp
    movq    %r15, 56(%rsp)
    movq    %rcx, %r15
    xorq    %rdi, %rbp
    andq    %r14, %r15
    movq    %rbp, -40(%rsp)
    movq    -72(%rsp), %rbp
    xorq    %rbx, %r15
    movq    %r14, %rbx
    orq %rdi, %rbx
    andq    %rdi, %rax
    movq    -96(%rsp), %rdi
    xorq    %rcx, %rbx
    xorq    %r14, %rax
    movq    16(%rsp), %r14
    xorq    %rbx, %rdx
    xorq    %rax, %rsi
    movq    %rbx, 48(%rsp)
    movq    %rax, 64(%rsp)
    movq    -80(%rsp), %rbx
    xorq    %r9, %rbp
    movq    -24(%rsp), %rax
    movq    24(%rsp), %rcx
    xorq    %r8, %r14
    rorx    $54, %r14, %r14
    rorx    $49, %rbp, %rbp
    xorq    %r13, %rdi
    xorq    %r10, %rbx
    movq    %rbp, -120(%rsp)
    movq    %r14, %rbp
    xorq    %r12, %rax
    xorq    -40(%rsp), %rax
    xorq    %r11, %rcx
    rorx    $28, %rbx, %rbx
    rorx    $37, %rcx, %rcx
    rorx    $8, %rdi, %rdi
    andq    %rbx, %rbp
    xorq    %rcx, %rbp
    movq    %rbp, -96(%rsp)
    xorq    %rbp, %rax
    movq    -120(%rsp), %rbp
    orq %r14, %rbp
    xorq    %rbx, %rbp
    orq %rcx, %rbx
    movq    %rbp, -80(%rsp)
    movq    -120(%rsp), %rbp
    xorq    %rdi, %rbx
    xorq    %rbx, %rsi
    notq    %rbp
    movq    %rbp, -120(%rsp)
    xorq    8(%rsp), %r13
    orq %rdi, %rbp
    xorq    72(%rsp), %r11
    xorq    -16(%rsp), %r9
    xorq    %r14, %rbp
    xorq    -88(%rsp), %r10
    xorq    -104(%rsp), %r8
    movq    %rdi, %r14
    andq    %rcx, %r14
    xorq    -120(%rsp), %r14
    movq    %rbx, 16(%rsp)
    rorx    $9, %r13, %r13
    movq    %rbp, -72(%rsp)
    movq    %r13, %rdi
    rorx    $25, %r11, %rcx
    rorx    $2, %r9, %r9
    notq    %rdi
    rorx    $23, %r10, %r10
    rorx    $62, %r8, %r8
    movq    %rdi, %rbx
    movq    %r10, %r11
    movq    %r8, %rbp
    andq    %rcx, %rbx
    xorq    %r14, %rdx
    xorq    %r9, %rbx
    xorq    %rbx, %rax
    orq %rcx, %r11
    andq    %r10, %rbp
    xorq    %rdi, %r11
    xorq    %rcx, %rbp
    movq    56(%rsp), %rdi
    movq    -56(%rsp), %rcx
    xorq    40(%rsp), %rdi
    andq    %r9, %r13
    xorq    -80(%rsp), %rdi
    movq    %r11, -104(%rsp)
    xorq    %r8, %r13
    xorq    %r13, %rsi
    xorq    %rbp, %rcx
    xorq    -112(%rsp), %rcx
    xorq    %r11, %rdi
    xorq    -32(%rsp), %rdi
    movq    %r8, %r11
    orq %r9, %r11
    xorq    %r15, %rcx
    xorq    -72(%rsp), %rcx
    xorq    %r10, %r11
    xorq    %r11, %rdx
    rorx    $63, %rdi, %r9
    rorx    $63, %rdx, %r10
    xorq    %rsi, %r9
    rorx    $63, %rsi, %rsi
    xorq    %rdi, %r10
    xorq    %rcx, %rsi
    rorx    $63, %rcx, %r8
    movq    40(%rsp), %rcx
    xorq    %rax, %r8
    xorq    %r10, %r15
    rorx    $63, %rax, %rax
    xorq    %rdx, %rax
    rorx    $21, %r15, %r15
    movq    %r9, %rdx
    xorq    %r8, %rcx
    xorq    %r12, %rdx
    movq    %r15, %rdi
    rorx    $20, %rcx, %rcx
    movabsq $-9223372036854743038, %r12
    xorq    %rsi, %r14
    orq %rcx, %rdi
    xorq    %rdx, %r12
    xorq    %rax, %r13
    xorq    %rdi, %r12
    movq    %r15, %rdi
    rorx    $43, %r14, %r14
    rorx    $50, %r13, %r13
    notq    %rdi
    movq    %r12, -64(%rsp)
    orq %r14, %rdi
    movq    %r13, %r12
    xorq    %rcx, %rdi
    andq    %r14, %r12
    andq    %rdx, %rcx
    xorq    %r15, %r12
    xorq    %r13, %rcx
    movq    %rdi, -120(%rsp)
    movq    %r12, -88(%rsp)
    movq    -80(%rsp), %r12
    movq    %r13, %r15
    movq    -40(%rsp), %rdi
    orq %rdx, %r15
    movq    %rcx, 40(%rsp)
    movq    32(%rsp), %rdx
    movq    (%rsp), %rcx
    xorq    %r10, %rbp
    xorq    %r8, %r12
    xorq    %r14, %r15
    rorx    $3, %rbp, %rbp
    rorx    $19, %r12, %r12
    xorq    %r9, %rdi
    movq    %r15, -16(%rsp)
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rdx
    xorq    %rax, %rcx
    movq    %r12, %r14
    movq    %rbp, %r15
    rorx    $36, %rdx, %rdx
    rorx    $44, %rcx, %rcx
    andq    %rdi, %r14
    movq    %rdi, %r13
    notq    %r15
    xorq    %rcx, %r14
    orq %rcx, %r13
    orq %r12, %r15
    andq    %rdx, %rcx
    xorq    %rdx, %r13
    xorq    %rdi, %r15
    xorq    %rbp, %rcx
    movq    %rbp, %rdi
    orq %rdx, %rdi
    movq    %rcx, -40(%rsp)
    movq    48(%rsp), %rcx
    xorq    %r12, %rdi
    movq    %r13, 32(%rsp)
    movq    -112(%rsp), %r13
    movq    %rdi, (%rsp)
    movq    -32(%rsp), %rdx
    xorq    %r9, %rbx
    movq    16(%rsp), %rdi
    xorq    %rsi, %rcx
    rorx    $46, %rbx, %rbx
    rorx    $39, %rcx, %rcx
    xorq    %r10, %r13
    movq    %r14, -80(%rsp)
    xorq    %r8, %rdx
    rorx    $58, %r13, %r13
    movq    %rcx, %rbp
    xorq    %rax, %rdi
    rorx    $63, %rdx, %rdx
    orq %r13, %rbp
    rorx    $56, %rdi, %rdi
    xorq    %rdx, %rbp
    movq    %rbx, %r14
    movq    %rdi, %r12
    notq    %rdi
    movq    %rbp, -112(%rsp)
    andq    %rcx, %r12
    movq    %rdi, %rbp
    xorq    %r13, %r12
    andq    %rbx, %rbp
    xorq    %rcx, %rbp
    movq    %r12, 8(%rsp)
    orq %rdx, %r14
    movq    56(%rsp), %r12
    xorq    %rdi, %r14
    andq    %rdx, %r13
    movq    -24(%rsp), %rdi
    xorq    %rbx, %r13
    movq    -48(%rsp), %rdx
    movq    %r13, 24(%rsp)
    movq    -72(%rsp), %r13
    xorq    %rsi, %r11
    xorq    %r8, %r12
    movq    (%rsp), %rcx
    xorq    -16(%rsp), %rcx
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    xorq    %rax, %rdx
    rorx    $28, %rdi, %rdi
    movq    %r12, %rbx
    rorx    $37, %rdx, %rdx
    xorq    %r10, %r13
    andq    %rdi, %rbx
    xorq    -8(%rsp), %rsi
    xorq    %rdx, %rbx
    rorx    $49, %r13, %r13
    xorq    -56(%rsp), %r10
    movq    %rbx, -72(%rsp)
    movq    %r13, %rbx
    notq    %r13
    orq %r12, %rbx
    rorx    $8, %r11, %r11
    xorq    %r14, %rcx
    xorq    %rdi, %rbx
    xorq    64(%rsp), %rax
    orq %rdx, %rdi
    movq    %r14, 16(%rsp)
    movq    %r13, %r14
    xorq    %r11, %rdi
    orq %r11, %r14
    rorx    $9, %rsi, %rsi
    movq    %rdi, -24(%rsp)
    xorq    %r12, %r14
    rorx    $2, %r10, %rdi
    movq    %rsi, %r10
    movq    %r14, -48(%rsp)
    notq    %r10
    movq    %r11, %r14
    rorx    $25, %rax, %rax
    xorq    -96(%rsp), %r9
    andq    %rdx, %r14
    movq    %r10, %rdx
    xorq    -104(%rsp), %r8
    xorq    %r13, %r14
    andq    %rax, %rdx
    xorq    %r14, %rcx
    movq    8(%rsp), %r13
    xorq    %rdi, %rdx
    movq    -40(%rsp), %r12
    movq    %rdx, -104(%rsp)
    movq    32(%rsp), %rdx
    rorx    $23, %r9, %r9
    xorq    -64(%rsp), %rdx
    movq    %r9, %r11
    rorx    $62, %r8, %r8
    xorq    -112(%rsp), %rdx
    xorq    -72(%rsp), %rdx
    xorq    -104(%rsp), %rdx
    orq %rax, %r11
    xorq    -80(%rsp), %r13
    xorq    %r10, %r11
    movq    %r8, %r10
    xorq    40(%rsp), %r12
    andq    %r9, %r10
    xorq    24(%rsp), %r12
    movq    %r11, -96(%rsp)
    xorq    %rax, %r10
    movq    -88(%rsp), %rax
    xorq    -24(%rsp), %r12
    xorq    %rbx, %r13
    andq    %rdi, %rsi
    xorq    %r11, %r13
    xorq    -120(%rsp), %r13
    movq    %r8, %r11
    xorq    %r10, %rax
    orq %rdi, %r11
    xorq    %r8, %rsi
    xorq    %r15, %rax
    xorq    %r9, %r11
    xorq    %rsi, %r12
    xorq    %rbp, %rax
    xorq    -48(%rsp), %rax
    xorq    %r11, %rcx
    rorx    $63, %r13, %r8
    rorx    $63, %rcx, %r9
    xorq    %r12, %r8
    rorx    $63, %r12, %r12
    xorq    %r13, %r9
    rorx    $63, %rdx, %r13
    xorq    %r9, %rbp
    rorx    $63, %rax, %rdi
    xorq    %rax, %r12
    movq    -64(%rsp), %rax
    xorq    %rdx, %rdi
    movq    -80(%rsp), %rdx
    xorq    %rcx, %r13
    movabsq $-9223372036854775680, %rcx
    rorx    $21, %rbp, %rbp
    xorq    %r12, %r14
    xorq    %r8, %rax
    xorq    %r13, %rsi
    rorx    $43, %r14, %r14
    xorq    %rdi, %rdx
    xorq    %rax, %rcx
    rorx    $50, %rsi, %rsi
    rorx    $20, %rdx, %rdx
    movq    %rcx, -64(%rsp)
    movq    %rdx, %rcx
    orq %rbp, %rcx
    xorq    %rcx, -64(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    xorq    %rdi, %rbx
    xorq    %r9, %r10
    orq %r14, %rcx
    rorx    $19, %rbx, %rbx
    rorx    $3, %r10, %r10
    xorq    %rdx, %rcx
    andq    %rax, %rdx
    movq    %rcx, -32(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rdx
    andq    %r14, %rcx
    movq    %rdx, -8(%rsp)
    movq    -40(%rsp), %rdx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    %rbx, %rsi
    movq    %rcx, -56(%rsp)
    movq    -112(%rsp), %rcx
    orq %rax, %rbp
    movq    -16(%rsp), %rax
    xorq    %r14, %rbp
    xorq    %r13, %rdx
    rorx    $44, %rdx, %rdx
    movq    %rbp, -80(%rsp)
    movq    %r10, %rbp
    xorq    %r8, %rcx
    notq    %rbp
    rorx    $61, %rcx, %rcx
    xorq    %r12, %rax
    orq %rbx, %rbp
    movq    %rcx, %r14
    rorx    $36, %rax, %rax
    andq    %rcx, %rsi
    orq %rdx, %r14
    xorq    %rdx, %rsi
    andq    %rax, %rdx
    xorq    %rax, %r14
    xorq    %rcx, %rbp
    xorq    %r10, %rdx
    movq    %r14, -16(%rsp)
    movq    %r9, %rcx
    movq    %r10, %r14
    movq    16(%rsp), %r10
    xorq    %r15, %rcx
    movq    -104(%rsp), %r15
    movq    %rsi, -40(%rsp)
    movq    %rdx, 48(%rsp)
    rorx    $58, %rcx, %rcx
    movq    -120(%rsp), %rsi
    movq    -24(%rsp), %rdx
    orq %rax, %r14
    xorq    %r12, %r10
    xorq    %r8, %r15
    xorq    %rbx, %r14
    rorx    $39, %r10, %r10
    rorx    $46, %r15, %rax
    movq    %rbp, -112(%rsp)
    xorq    %rdi, %rsi
    xorq    %r13, %rdx
    movq    %r10, %r15
    rorx    $63, %rsi, %rsi
    orq %rcx, %r15
    rorx    $56, %rdx, %rdx
    xorq    %rsi, %r15
    movq    %rdx, %rbx
    notq    %rdx
    movq    %r15, -120(%rsp)
    andq    %r10, %rbx
    movq    %rdx, %r15
    movq    %rax, %rbp
    xorq    %rcx, %rbx
    andq    %rax, %r15
    orq %rsi, %rbp
    andq    %rcx, %rsi
    movq    %rbx, -104(%rsp)
    xorq    %rax, %rsi
    movq    8(%rsp), %rax
    movq    32(%rsp), %rbx
    xorq    %r10, %r15
    xorq    %rdx, %rbp
    movq    -48(%rsp), %r10
    movq    40(%rsp), %rdx
    movq    %rsi, 16(%rsp)
    xorq    %r12, %r11
    xorq    %rdi, %rax
    movq    -64(%rsp), %rsi
    xorq    -16(%rsp), %rsi
    rorx    $54, %rax, %rax
    xorq    %r8, %rbx
    xorq    -120(%rsp), %rsi
    xorq    %r13, %rdx
    rorx    $28, %rbx, %rbx
    xorq    %r9, %r10
    movq    %r14, 56(%rsp)
    movq    %rax, %r14
    rorx    $37, %rdx, %rdx
    andq    %rbx, %r14
    movq    %rbp, -24(%rsp)
    rorx    $49, %r10, %r10
    movq    %rax, %rbp
    xorq    %rdx, %r14
    rorx    $8, %r11, %r11
    orq %r10, %rbp
    notq    %r10
    xorq    %r14, %rsi
    movq    %r14, 32(%rsp)
    movq    %r10, %r14
    movq    -32(%rsp), %rcx
    orq %r11, %r14
    xorq    -40(%rsp), %rcx
    xorq    %rbx, %rbp
    xorq    %rax, %r14
    xorq    -104(%rsp), %rcx
    orq %rdx, %rbx
    movq    %r14, -48(%rsp)
    xorq    (%rsp), %r12
    xorq    %r11, %rbx
    xorq    -72(%rsp), %r8
    xorq    24(%rsp), %r13
    movq    %r11, %r14
    xorq    -88(%rsp), %r9
    xorq    -96(%rsp), %rdi
    andq    %rdx, %r14
    xorq    %r10, %r14
    xorq    %rbp, %rcx
    rorx    $9, %r12, %r12
    movq    %r12, %rax
    rorx    $25, %r13, %r13
    rorx    $23, %r8, %r8
    notq    %rax
    rorx    $2, %r9, %r9
    movq    %r8, %rdx
    movq    %rax, %r11
    rorx    $62, %rdi, %r10
    movq    -112(%rsp), %rdi
    andq    %r13, %r11
    xorq    %r9, %r11
    orq %r13, %rdx
    andq    %r9, %r12
    xorq    %rax, %rdx
    movq    56(%rsp), %rax
    xorq    -80(%rsp), %rax
    xorq    %rdx, %rcx
    movq    %rdx, -96(%rsp)
    movq    %r10, %rdx
    andq    %r8, %rdx
    xorq    %r10, %r12
    xorq    %r15, %rdi
    xorq    %r13, %rdx
    movq    %r10, %r13
    xorq    -56(%rsp), %rdi
    xorq    %r14, %rax
    xorq    -24(%rsp), %rax
    orq %r9, %r13
    xorq    %r8, %r13
    xorq    -48(%rsp), %rdi
    xorq    %r11, %rsi
    movq    %r13, -88(%rsp)
    rorx    $63, %rcx, %r10
    xorq    %r13, %rax
    movq    48(%rsp), %r13
    rorx    $63, %rax, %r9
    xorq    %rdx, %rdi
    xorq    %rcx, %r9
    rorx    $63, %rsi, %rcx
    rorx    $63, %rdi, %r8
    xorq    %r12, %r13
    xorq    -8(%rsp), %r13
    xorq    %rax, %rcx
    xorq    16(%rsp), %r13
    movq    -64(%rsp), %rax
    xorq    %rsi, %r8
    movq    -40(%rsp), %rsi
    xorq    %r9, %r15
    xorq    %rcx, %r12
    rorx    $21, %r15, %r15
    rorx    $50, %r12, %r12
    xorq    %rbx, %r13
    xorq    %r8, %rsi
    xorq    %r13, %r10
    rorx    $63, %r13, %r13
    rorx    $20, %rsi, %rsi
    xorq    %r10, %rax
    xorq    %rdi, %r13
    movq    %rax, %rdi
    xorq    %r13, %r14
    xorq    $32778, %rdi
    rorx    $43, %r14, %r14
    xorq    %r9, %rdx
    movq    %rdi, -72(%rsp)
    movq    %r15, %rdi
    rorx    $3, %rdx, %rdx
    orq %rsi, %rdi
    xorq    %rdi, -72(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    xorq    %r8, %rbp
    xorq    %rcx, %rbx
    orq %r14, %rdi
    rorx    $19, %rbp, %rbp
    rorx    $56, %rbx, %rbx
    xorq    %rsi, %rdi
    andq    %rax, %rsi
    xorq    %r10, %r11
    movq    %rdi, -64(%rsp)
    movq    %r12, %rdi
    xorq    %r12, %rsi
    andq    %r14, %rdi
    movq    %rsi, -40(%rsp)
    movq    -80(%rsp), %rsi
    xorq    %r15, %rdi
    movq    %r12, %r15
    rorx    $46, %r11, %r11
    orq %rax, %r15
    movq    -120(%rsp), %rax
    movq    %rdi, 40(%rsp)
    xorq    %r14, %r15
    movq    48(%rsp), %rdi
    xorq    %r13, %rsi
    movq    %r15, (%rsp)
    movq    %rdx, %r15
    movq    %rbp, %r14
    xorq    %r10, %rax
    notq    %r15
    rorx    $36, %rsi, %rsi
    rorx    $61, %rax, %rax
    xorq    %rcx, %rdi
    orq %rbp, %r15
    xorq    %rax, %r15
    rorx    $44, %rdi, %rdi
    andq    %rax, %r14
    movq    %rax, %r12
    movq    %rdx, %rax
    xorq    %rdi, %r14
    orq %rdi, %r12
    orq %rsi, %rax
    andq    %rsi, %rdi
    xorq    %rdx, %rdi
    xorq    %rsi, %r12
    xorq    %rbp, %rax
    movq    %r12, -80(%rsp)
    movq    %r14, 8(%rsp)
    movq    %rdi, %rsi
    movq    %r15, -120(%rsp)
    movq    %rax, 24(%rsp)
    movq    %rbx, %r14
    xorq    (%rsp), %rax
    movq    %rdi, 48(%rsp)
    notq    %rbx
    movq    -24(%rsp), %rbp
    movq    -112(%rsp), %rdx
    movq    %rbx, %r15
    movq    -32(%rsp), %rdi
    xorq    -40(%rsp), %rsi
    xorq    %r13, %rbp
    xorq    %r9, %rdx
    rorx    $39, %rbp, %rbp
    xorq    %r8, %rdi
    rorx    $58, %rdx, %rdx
    movq    %rbp, %r12
    rorx    $63, %rdi, %rdi
    orq %rdx, %r12
    andq    %rbp, %r14
    andq    %r11, %r15
    xorq    %rdx, %r14
    andq    %rdi, %rdx
    xorq    %rbp, %r15
    xorq    %r11, %rdx
    movq    %r11, %rbp
    movq    %r14, 64(%rsp)
    xorq    %rdx, %rsi
    movq    %rdx, 96(%rsp)
    movq    -8(%rsp), %rdx
    orq %rdi, %rbp
    movq    -104(%rsp), %r14
    xorq    %rdi, %r12
    xorq    %rbx, %rbp
    movq    -16(%rsp), %rbx
    movq    %r12, -24(%rsp)
    xorq    %rcx, %rdx
    xorq    %rbp, %rax
    movq    %rbp, 72(%rsp)
    rorx    $37, %rdx, %r11
    xorq    %r8, %r14
    movq    -48(%rsp), %rbp
    movq    -80(%rsp), %rdx
    xorq    -72(%rsp), %rdx
    rorx    $54, %r14, %r14
    xorq    %r10, %rbx
    movq    -88(%rsp), %rdi
    xorq    -24(%rsp), %rdx
    rorx    $28, %rbx, %rbx
    movq    %r14, %r12
    xorq    %r9, %rbp
    andq    %rbx, %r12
    rorx    $49, %rbp, %rbp
    xorq    -56(%rsp), %r9
    xorq    %r11, %r12
    xorq    %r13, %rdi
    rorx    $8, %rdi, %rdi
    xorq    %r12, %rdx
    movq    %r12, -104(%rsp)
    movq    %rbp, %r12
    notq    %rbp
    movq    %rbp, -112(%rsp)
    orq %rdi, %rbp
    orq %r14, %r12
    xorq    %r14, %rbp
    movq    %rdi, %r14
    xorq    %rbx, %r12
    andq    %r11, %r14
    xorq    -112(%rsp), %r14
    orq %r11, %rbx
    xorq    %rdi, %rbx
    movq    %rbp, -88(%rsp)
    rorx    $2, %r9, %r9
    xorq    %rbx, %rsi
    movq    %rbx, -16(%rsp)
    xorq    %r14, %rax
    xorq    56(%rsp), %r13
    xorq    32(%rsp), %r10
    xorq    16(%rsp), %rcx
    xorq    -96(%rsp), %r8
    rorx    $23, %r10, %r10
    rorx    $9, %r13, %r13
    rorx    $25, %rcx, %rcx
    movq    %r13, %rdi
    movq    %r10, %rbp
    notq    %rdi
    orq %rcx, %rbp
    rorx    $62, %r8, %r8
    xorq    %rdi, %rbp
    movq    %rdi, %rbx
    movq    64(%rsp), %rdi
    xorq    8(%rsp), %rdi
    movq    %rbp, -96(%rsp)
    andq    %rcx, %rbx
    movq    %r8, %r11
    xorq    %r9, %rbx
    andq    %r9, %r13
    orq %r9, %r11
    xorq    %rbx, %rdx
    xorq    %r8, %r13
    xorq    %r10, %r11
    xorq    %r13, %rsi
    xorq    %r12, %rdi
    xorq    %r11, %rax
    xorq    %rbp, %rdi
    movq    %r8, %rbp
    xorq    -64(%rsp), %rdi
    andq    %r10, %rbp
    rorx    $63, %rax, %r10
    xorq    %rcx, %rbp
    movq    40(%rsp), %rcx
    rorx    $63, %rdi, %r9
    xorq    %rdi, %r10
    movabsq $-9223372034707292150, %rdi
    xorq    %rbp, %rcx
    xorq    -120(%rsp), %rcx
    xorq    %rsi, %r9
    rorx    $63, %rsi, %rsi
    xorq    %r15, %rcx
    xorq    -88(%rsp), %rcx
    xorq    %r10, %r15
    rorx    $21, %r15, %r15
    rorx    $63, %rcx, %r8
    xorq    %rcx, %rsi
    movq    8(%rsp), %rcx
    xorq    %rdx, %r8
    rorx    $63, %rdx, %rdx
    xorq    %rax, %rdx
    movq    -72(%rsp), %rax
    xorq    %r8, %rcx
    rorx    $20, %rcx, %rcx
    xorq    %r9, %rax
    xorq    %rsi, %r14
    xorq    %rdx, %r13
    xorq    %rax, %rdi
    rorx    $43, %r14, %r14
    rorx    $50, %r13, %r13
    movq    %rdi, -32(%rsp)
    movq    %r15, %rdi
    xorq    %r8, %r12
    orq %rcx, %rdi
    xorq    %rdi, -32(%rsp)
    movq    %r15, %rdi
    notq    %rdi
    rorx    $19, %r12, %r12
    xorq    %r10, %rbp
    orq %r14, %rdi
    rorx    $3, %rbp, %rbp
    xorq    %r9, %rbx
    xorq    %rcx, %rdi
    andq    %rax, %rcx
    rorx    $46, %rbx, %rbx
    movq    %rdi, -112(%rsp)
    movq    %r13, %rdi
    xorq    %r13, %rcx
    andq    %r14, %rdi
    movq    %rcx, -72(%rsp)
    movq    48(%rsp), %rcx
    xorq    %r15, %rdi
    movq    %r13, %r15
    movq    %rdi, -56(%rsp)
    movq    -24(%rsp), %rdi
    orq %rax, %r15
    movq    (%rsp), %rax
    xorq    %r14, %r15
    xorq    %rdx, %rcx
    movq    %r12, %r14
    rorx    $44, %rcx, %rcx
    movq    %r15, 32(%rsp)
    xorq    %r9, %rdi
    movq    %rbp, %r15
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rax
    andq    %rdi, %r14
    rorx    $36, %rax, %rax
    movq    %rdi, %r13
    xorq    %rcx, %r14
    orq %rcx, %r13
    andq    %rax, %rcx
    movq    %r14, -24(%rsp)
    movq    %rbp, %r14
    xorq    %rbp, %rcx
    orq %rax, %r15
    notq    %r14
    movq    %rcx, (%rsp)
    xorq    %r12, %r15
    orq %r12, %r14
    movq    72(%rsp), %rcx
    movq    -16(%rsp), %r12
    xorq    %rdi, %r14
    movq    -120(%rsp), %rdi
    xorq    %rax, %r13
    movq    -64(%rsp), %rax
    movq    %r15, -8(%rsp)
    xorq    %rsi, %rcx
    movq    %r13, -48(%rsp)
    movq    %rbx, %r15
    xorq    %rdx, %r12
    rorx    $39, %rcx, %rcx
    xorq    %r10, %rdi
    rorx    $56, %r12, %r12
    xorq    %r8, %rax
    rorx    $58, %rdi, %rdi
    movq    %rcx, %rbp
    movq    %r12, %r13
    rorx    $63, %rax, %rax
    orq %rdi, %rbp
    andq    %rcx, %r13
    notq    %r12
    xorq    %rax, %rbp
    xorq    %rdi, %r13
    orq %rax, %r15
    andq    %rax, %rdi
    xorq    %r12, %r15
    movq    %rbp, -120(%rsp)
    xorq    %rbx, %rdi
    movq    %r12, %rbp
    movq    64(%rsp), %r12
    movq    %rdi, 16(%rsp)
    movq    -80(%rsp), %rdi
    andq    %rbx, %rbp
    movq    -40(%rsp), %rax
    movq    %r13, -16(%rsp)
    xorq    %rcx, %rbp
    xorq    %r8, %r12
    movq    -88(%rsp), %r13
    movq    -8(%rsp), %rcx
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    xorq    32(%rsp), %rcx
    xorq    %rdx, %rax
    rorx    $28, %rdi, %rdi
    movq    %r12, %rbx
    rorx    $37, %rax, %rax
    xorq    %r10, %r13
    andq    %rdi, %rbx
    xorq    %rax, %rbx
    xorq    %rsi, %r11
    rorx    $49, %r13, %r13
    xorq    24(%rsp), %rsi
    xorq    40(%rsp), %r10
    rorx    $8, %r11, %r11
    movq    %rbx, -88(%rsp)
    movq    %r13, %rbx
    xorq    -104(%rsp), %r9
    orq %r12, %rbx
    notq    %r13
    xorq    %r15, %rcx
    xorq    %rdi, %rbx
    xorq    96(%rsp), %rdx
    orq %rax, %rdi
    movq    %r15, 8(%rsp)
    movq    %r13, %r15
    xorq    %r11, %rdi
    orq %r11, %r15
    rorx    $9, %rsi, %rsi
    xorq    -96(%rsp), %r8
    xorq    %r12, %r15
    movq    %rdi, -40(%rsp)
    movq    %r11, %r12
    rorx    $2, %r10, %rdi
    movq    %rsi, %r10
    rorx    $23, %r9, %r9
    andq    %rax, %r12
    notq    %r10
    rorx    $25, %rdx, %rdx
    xorq    %r13, %r12
    movq    %r15, -80(%rsp)
    movq    %r9, %r11
    movq    %r10, %r15
    xorq    %r12, %rcx
    movq    -48(%rsp), %rax
    andq    %rdx, %r15
    xorq    -32(%rsp), %rax
    orq %rdx, %r11
    rorx    $62, %r8, %r8
    xorq    %r10, %r11
    xorq    -120(%rsp), %rax
    xorq    -88(%rsp), %rax
    movq    %r8, %r10
    movq    %r11, -104(%rsp)
    movq    -16(%rsp), %r11
    xorq    -24(%rsp), %r11
    andq    %r9, %r10
    xorq    %rdx, %r10
    movq    %r8, %r13
    movq    -56(%rsp), %rdx
    xorq    %rdi, %r15
    orq %rdi, %r13
    andq    %rdi, %rsi
    movq    (%rsp), %rdi
    xorq    -72(%rsp), %rdi
    xorq    %r8, %rsi
    xorq    %rbx, %r11
    xorq    16(%rsp), %rdi
    xorq    -104(%rsp), %r11
    xorq    -112(%rsp), %r11
    xorq    -40(%rsp), %rdi
    xorq    %r10, %rdx
    xorq    %r14, %rdx
    xorq    %r9, %r13
    xorq    %r15, %rax
    xorq    %rbp, %rdx
    xorq    -80(%rsp), %rdx
    xorq    %r13, %rcx
    movq    %r13, -96(%rsp)
    rorx    $63, %rcx, %r13
    xorq    %rsi, %rdi
    rorx    $63, %r11, %r9
    xorq    %r11, %r13
    xorq    %rdi, %r9
    rorx    $63, %rdi, %rdi
    movabsq $-9223372034707259263, %r11
    xorq    %rdx, %rdi
    rorx    $63, %rdx, %r8
    rorx    $63, %rax, %rdx
    xorq    %rcx, %rdx
    xorq    %rax, %r8
    movq    -24(%rsp), %rcx
    movq    -32(%rsp), %rax
    xorq    %r9, %rax
    xorq    %r8, %rcx
    xorq    %r13, %rbp
    xorq    %rax, %r11
    rorx    $20, %rcx, %rcx
    rorx    $21, %rbp, %rbp
    movq    %r11, -24(%rsp)
    movq    %rcx, %r11
    xorq    %rdi, %r12
    orq %rbp, %r11
    xorq    %r11, -24(%rsp)
    movq    %rbp, %r11
    rorx    $43, %r12, %r12
    notq    %r11
    xorq    %rdx, %rsi
    orq %r12, %r11
    rorx    $50, %rsi, %rsi
    xorq    %r8, %rbx
    xorq    %rcx, %r11
    andq    %rax, %rcx
    rorx    $19, %rbx, %rbx
    movq    %r11, -64(%rsp)
    movq    %rsi, %r11
    xorq    %rsi, %rcx
    andq    %r12, %r11
    movq    %rcx, 56(%rsp)
    movq    -120(%rsp), %rcx
    xorq    %rbp, %r11
    movq    %rsi, %rbp
    movq    32(%rsp), %rsi
    orq %rax, %rbp
    movq    (%rsp), %rax
    movq    %r11, 40(%rsp)
    xorq    %r9, %rcx
    xorq    %r13, %r10
    xorq    %r12, %rbp
    rorx    $61, %rcx, %rcx
    xorq    %rdi, %rsi
    rorx    $3, %r10, %r10
    xorq    %rdx, %rax
    movq    %rcx, %r11
    rorx    $36, %rsi, %rsi
    rorx    $44, %rax, %rax
    movq    %rbx, %r12
    movq    %rbp, 24(%rsp)
    orq %rax, %r11
    andq    %rcx, %r12
    movq    %r10, %rbp
    xorq    %rsi, %r11
    xorq    %rax, %r12
    andq    %rsi, %rax
    movq    %r11, 32(%rsp)
    movq    %r10, %r11
    xorq    %r10, %rax
    orq %rsi, %r11
    movq    %rax, 48(%rsp)
    movq    8(%rsp), %r10
    xorq    %rbx, %r11
    movq    -40(%rsp), %rax
    notq    %rbp
    movq    %r11, (%rsp)
    movq    -112(%rsp), %r11
    orq %rbx, %rbp
    xorq    %rcx, %rbp
    movq    %r13, %rcx
    xorq    %rdi, %r10
    xorq    %r14, %rcx
    rorx    $39, %r10, %r10
    movq    %r12, -120(%rsp)
    xorq    %r8, %r11
    xorq    %rdx, %rax
    xorq    %r9, %r15
    rorx    $46, %r15, %r15
    rorx    $56, %rax, %rax
    rorx    $58, %rcx, %rcx
    rorx    $63, %r11, %r11
    movq    %r10, %r12
    movq    %rax, %r14
    movq    %r15, %rsi
    notq    %rax
    orq %rcx, %r12
    andq    %r10, %r14
    orq %r11, %rsi
    movq    %rax, %rbx
    xorq    %r11, %r12
    xorq    %rcx, %r14
    xorq    %rax, %rsi
    andq    %r15, %rbx
    movq    %rbp, -32(%rsp)
    movq    %r12, -112(%rsp)
    xorq    %r10, %rbx
    movq    %r14, -40(%rsp)
    movq    %rsi, 8(%rsp)
    movq    -16(%rsp), %r10
    andq    %rcx, %r11
    movq    -48(%rsp), %rcx
    xorq    %r15, %r11
    movq    -72(%rsp), %rax
    movq    -80(%rsp), %r15
    movq    %r11, 64(%rsp)
    movq    -24(%rsp), %r11
    xorq    %r8, %r10
    xorq    32(%rsp), %r11
    xorq    %r9, %rcx
    rorx    $54, %r10, %r10
    movq    -96(%rsp), %r14
    xorq    %rdx, %rax
    rorx    $28, %rcx, %rcx
    xorq    %r13, %r15
    movq    %r10, %rbp
    rorx    $37, %rax, %rax
    andq    %rcx, %rbp
    rorx    $49, %r15, %r15
    movq    %r10, %rsi
    xorq    %rax, %rbp
    xorq    %rdi, %r14
    xorq    %r12, %r11
    orq %r15, %rsi
    notq    %r15
    rorx    $8, %r14, %r14
    xorq    %rbp, %r11
    movq    %rbp, -96(%rsp)
    movq    -64(%rsp), %r12
    movq    %r15, %rbp
    xorq    -120(%rsp), %r12
    xorq    -40(%rsp), %r12
    orq %r14, %rbp
    xorq    %rcx, %rsi
    xorq    %r10, %rbp
    orq %rax, %rcx
    movq    %rbp, -16(%rsp)
    movq    %r14, %rbp
    xorq    %r14, %rcx
    andq    %rax, %rbp
    xorq    %r15, %rbp
    xorq    %rsi, %r12
    xorq    -56(%rsp), %r13
    xorq    -8(%rsp), %rdi
    xorq    -104(%rsp), %r8
    xorq    -88(%rsp), %r9
    xorq    16(%rsp), %rdx
    rorx    $2, %r13, %r13
    rorx    $9, %rdi, %rdi
    rorx    $62, %r8, %r8
    movq    %rdi, %rax
    rorx    $23, %r9, %r9
    movq    %r8, %r15
    notq    %rax
    rorx    $25, %rdx, %rdx
    andq    %r9, %r15
    movq    %rax, %r14
    movq    %r9, %r10
    xorq    %rdx, %r15
    andq    %rdx, %r14
    orq %rdx, %r10
    movq    %r8, %rdx
    orq %r13, %rdx
    xorq    %rax, %r10
    movq    -32(%rsp), %rax
    xorq    %r9, %rdx
    xorq    %r13, %r14
    andq    %r13, %rdi
    movq    %rdx, -88(%rsp)
    movq    (%rsp), %rdx
    xorq    %r8, %rdi
    xorq    24(%rsp), %rdx
    movq    48(%rsp), %r13
    xorq    %rbx, %rax
    xorq    40(%rsp), %rax
    xorq    %r10, %r12
    xorq    %r14, %r11
    xorq    -16(%rsp), %rax
    movq    %r10, -104(%rsp)
    rorx    $63, %r12, %r10
    xorq    %rdi, %r13
    xorq    56(%rsp), %r13
    xorq    %rbp, %rdx
    xorq    8(%rsp), %rdx
    xorq    64(%rsp), %r13
    xorq    -88(%rsp), %rdx
    xorq    %r15, %rax
    rorx    $63, %rax, %r8
    xorq    %rcx, %r13
    xorq    %r11, %r8
    rorx    $63, %r11, %r11
    rorx    $63, %rdx, %r9
    xorq    %r13, %r10
    rorx    $63, %r13, %r13
    xorq    %r12, %r9
    movq    -24(%rsp), %r12
    xorq    %rax, %r13
    xorq    %rdx, %r11
    movq    -120(%rsp), %rdx
    xorq    %r9, %rbx
    rorx    $21, %rbx, %rbx
    xorq    %r13, %rbp
    xorq    %r11, %rdi
    xorq    %r10, %r12
    movq    %rbx, %rax
    rorx    $43, %rbp, %rbp
    movq    %r12, -72(%rsp)
    movabsq $-9223372036854742912, %r12
    xorq    -72(%rsp), %r12
    xorq    %r8, %rdx
    rorx    $50, %rdi, %rdi
    xorq    %r9, %r15
    rorx    $20, %rdx, %rdx
    rorx    $3, %r15, %r15
    xorq    %r8, %rsi
    orq %rdx, %rax
    rorx    $19, %rsi, %rsi
    xorq    %rax, %r12
    movq    %rbx, %rax
    notq    %rax
    orq %rbp, %rax
    xorq    %rdx, %rax
    movq    %rax, -120(%rsp)
    andq    -72(%rsp), %rdx
    movq    %rdi, %rax
    andq    %rbp, %rax
    xorq    %rbx, %rax
    movq    -72(%rsp), %rbx
    movq    %rax, -56(%rsp)
    movq    48(%rsp), %rax
    xorq    %rdi, %rdx
    movq    %rdx, -72(%rsp)
    movq    -112(%rsp), %rdx
    orq %rdi, %rbx
    movq    24(%rsp), %rdi
    xorq    %r11, %rax
    xorq    %rbp, %rbx
    rorx    $44, %rax, %rax
    movq    %rbx, -80(%rsp)
    movq    %rsi, %rbx
    xorq    %r10, %rdx
    rorx    $61, %rdx, %rdx
    xorq    %r13, %rdi
    movq    %rdx, %rbp
    rorx    $36, %rdi, %rdi
    andq    %rdx, %rbx
    orq %rax, %rbp
    xorq    %rax, %rbx
    andq    %rdi, %rax
    xorq    %rdi, %rbp
    xorq    %r15, %rax
    movq    %rbx, -24(%rsp)
    movq    %rbp, -48(%rsp)
    movq    %r15, %rbp
    movq    8(%rsp), %rbx
    notq    %rbp
    movq    %rax, 16(%rsp)
    orq %rsi, %rbp
    xorq    %rdx, %rbp
    movq    %r15, %rdx
    orq %rdi, %rdx
    movq    -64(%rsp), %rdi
    movq    %rbp, -112(%rsp)
    xorq    %rsi, %rdx
    movq    %rax, %rsi
    movq    -32(%rsp), %rax
    movq    %rdx, -8(%rsp)
    xorq    -80(%rsp), %rdx
    xorq    -72(%rsp), %rsi
    xorq    %r11, %rcx
    xorq    %r13, %rbx
    rorx    $56, %rcx, %rcx
    rorx    $39, %rbx, %rbx
    xorq    %r9, %rax
    movq    %rcx, %r15
    rorx    $58, %rax, %rax
    xorq    %r10, %r14
    andq    %rbx, %r15
    notq    %rcx
    rorx    $46, %r14, %r14
    xorq    %rax, %r15
    xorq    %r8, %rdi
    movq    %rbx, %rbp
    movq    %r15, -32(%rsp)
    movq    %rcx, %r15
    rorx    $63, %rdi, %rdi
    andq    %r14, %r15
    orq %rax, %rbp
    andq    %rdi, %rax
    xorq    %rbx, %r15
    movq    %r14, %rbx
    xorq    %rdi, %rbp
    orq %rdi, %rbx
    xorq    %r14, %rax
    movq    %rbp, -64(%rsp)
    xorq    %rcx, %rbx
    movq    -40(%rsp), %r14
    movq    -16(%rsp), %rbp
    xorq    %rbx, %rdx
    xorq    %rax, %rsi
    movq    %rbx, 8(%rsp)
    movq    %rax, 24(%rsp)
    movq    32(%rsp), %rbx
    movq    -48(%rsp), %rax
    movq    56(%rsp), %rcx
    xorq    %r8, %r14
    xorq    %r9, %rbp
    rorx    $54, %r14, %r14
    movq    -88(%rsp), %rdi
    xorq    %r10, %rbx
    rorx    $49, %rbp, %rbp
    xorq    %r12, %rax
    xorq    -64(%rsp), %rax
    movq    %rbp, -40(%rsp)
    xorq    %r11, %rcx
    rorx    $28, %rbx, %rbx
    movq    %r14, %rbp
    rorx    $37, %rcx, %rcx
    andq    %rbx, %rbp
    xorq    %r13, %rdi
    xorq    %rcx, %rbp
    rorx    $8, %rdi, %rdi
    xorq    %rbp, %rax
    movq    %rbp, -88(%rsp)
    movq    -40(%rsp), %rbp
    orq %r14, %rbp
    xorq    %rbx, %rbp
    movq    %rbp, -16(%rsp)
    movq    -40(%rsp), %rbp
    notq    %rbp
    movq    %rbp, -40(%rsp)
    orq %rdi, %rbp
    xorq    %r14, %rbp
    movq    %rdi, %r14
    andq    %rcx, %r14
    xorq    -40(%rsp), %r14
    xorq    (%rsp), %r13
    xorq    -104(%rsp), %r8
    xorq    -96(%rsp), %r10
    orq %rcx, %rbx
    xorq    64(%rsp), %r11
    xorq    %rdi, %rbx
    movq    %rbp, 32(%rsp)
    movq    %rbx, -40(%rsp)
    xorq    %rbx, %rsi
    xorq    40(%rsp), %r9
    rorx    $9, %r13, %r13
    xorq    %r14, %rdx
    rorx    $62, %r8, %r8
    movq    %r13, %rdi
    rorx    $23, %r10, %r10
    notq    %rdi
    movq    %r8, %rbp
    rorx    $25, %r11, %rcx
    andq    %r10, %rbp
    movq    %rdi, %rbx
    movq    %r10, %r11
    xorq    %rcx, %rbp
    andq    %rcx, %rbx
    orq %rcx, %r11
    movq    -56(%rsp), %rcx
    xorq    %rdi, %r11
    movq    -32(%rsp), %rdi
    xorq    -24(%rsp), %rdi
    rorx    $2, %r9, %r9
    movq    %r11, -104(%rsp)
    xorq    -16(%rsp), %rdi
    andq    %r9, %r13
    xorq    %r9, %rbx
    xorq    %rbp, %rcx
    xorq    -112(%rsp), %rcx
    xorq    %r8, %r13
    xorq    %r13, %rsi
    xorq    %rbx, %rax
    xorq    %r11, %rdi
    xorq    -120(%rsp), %rdi
    movq    %r8, %r11
    xorq    %r15, %rcx
    xorq    32(%rsp), %rcx
    orq %r9, %r11
    xorq    %r10, %r11
    xorq    %r11, %rdx
    rorx    $63, %rdi, %r9
    rorx    $63, %rdx, %r10
    xorq    %rsi, %r9
    rorx    $63, %rcx, %r8
    rorx    $63, %rsi, %rsi
    xorq    %rax, %r8
    xorq    %rdi, %r10
    xorq    %rcx, %rsi
    movq    -24(%rsp), %rcx
    xorq    %r10, %r15
    rorx    $63, %rax, %rax
    xorq    %rdx, %rax
    rorx    $21, %r15, %r15
    movq    %r9, %rdx
    xorq    %r12, %rdx
    movq    %r15, %rdi
    movl    $2147483649, %r12d
    xorq    %r8, %rcx
    xorq    %rax, %r13
    xorq    %rdx, %r12
    rorx    $20, %rcx, %rcx
    rorx    $50, %r13, %r13
    xorq    %rsi, %r14
    orq %rcx, %rdi
    rorx    $43, %r14, %r14
    xorq    %r10, %rbp
    xorq    %rdi, %r12
    movq    %r15, %rdi
    rorx    $3, %rbp, %rbp
    movq    %r12, -24(%rsp)
    movq    %r13, %r12
    notq    %rdi
    andq    %r14, %r12
    orq %r14, %rdi
    xorq    %r15, %r12
    xorq    %rcx, %rdi
    movq    %r13, %r15
    andq    %rdx, %rcx
    movq    %r12, (%rsp)
    movq    -16(%rsp), %r12
    xorq    %r13, %rcx
    orq %rdx, %r15
    movq    %rdi, -96(%rsp)
    movq    -64(%rsp), %rdi
    xorq    %r14, %r15
    movq    %rcx, 56(%rsp)
    movq    -80(%rsp), %rdx
    movq    16(%rsp), %rcx
    xorq    %r8, %r12
    movq    %r15, 40(%rsp)
    movq    %rbp, %r15
    rorx    $19, %r12, %r12
    xorq    %r9, %rdi
    notq    %r15
    movq    %r12, %r14
    rorx    $61, %rdi, %rdi
    xorq    %rsi, %rdx
    xorq    %rax, %rcx
    orq %r12, %r15
    rorx    $36, %rdx, %rdx
    rorx    $44, %rcx, %rcx
    xorq    %rdi, %r15
    andq    %rdi, %r14
    movq    %rdi, %r13
    movq    %rbp, %rdi
    xorq    %rcx, %r14
    orq %rcx, %r13
    orq %rdx, %rdi
    andq    %rdx, %rcx
    xorq    %rdx, %r13
    xorq    %r12, %rdi
    xorq    %rbp, %rcx
    movq    -40(%rsp), %r12
    movq    %rcx, -16(%rsp)
    movq    8(%rsp), %rcx
    xorq    %r9, %rbx
    movq    %rdi, 48(%rsp)
    movq    -112(%rsp), %rdi
    rorx    $46, %rbx, %rbx
    movq    -120(%rsp), %rdx
    xorq    %rax, %r12
    movq    %r13, 16(%rsp)
    xorq    %rsi, %rcx
    rorx    $56, %r12, %r12
    movq    %r14, -64(%rsp)
    rorx    $39, %rcx, %rcx
    xorq    %r10, %rdi
    movq    %r12, %r13
    xorq    %r8, %rdx
    rorx    $58, %rdi, %rdi
    movq    %rcx, %rbp
    rorx    $63, %rdx, %rdx
    orq %rdi, %rbp
    andq    %rcx, %r13
    movq    %rbx, %r14
    xorq    %rdx, %rbp
    xorq    %rdi, %r13
    notq    %r12
    orq %rdx, %r14
    andq    %rdx, %rdi
    movq    %rbp, -120(%rsp)
    xorq    %r12, %r14
    xorq    %rbx, %rdi
    movq    %r12, %rbp
    movq    -32(%rsp), %r12
    movq    %rdi, 8(%rsp)
    movq    -48(%rsp), %rdi
    movq    -72(%rsp), %rdx
    andq    %rbx, %rbp
    movq    %r13, -40(%rsp)
    movq    32(%rsp), %r13
    xorq    %rcx, %rbp
    xorq    %r8, %r12
    movq    48(%rsp), %rcx
    xorq    40(%rsp), %rcx
    rorx    $54, %r12, %r12
    xorq    %r9, %rdi
    xorq    %rax, %rdx
    rorx    $28, %rdi, %rdi
    movq    %r12, %rbx
    rorx    $37, %rdx, %rdx
    xorq    %r10, %r13
    andq    %rdi, %rbx
    xorq    %rsi, %r11
    xorq    %rdx, %rbx
    rorx    $49, %r13, %r13
    rorx    $8, %r11, %r11
    movq    %rbx, 64(%rsp)
    movq    %r13, %rbx
    notq    %r13
    xorq    %r14, %rcx
    movq    %r14, -80(%rsp)
    movq    %r13, %r14
    orq %r11, %r14
    orq %r12, %rbx
    xorq    %r12, %r14
    movq    %r11, %r12
    xorq    %rdi, %rbx
    andq    %rdx, %r12
    orq %rdx, %rdi
    movq    %r14, 72(%rsp)
    xorq    %r13, %r12
    movq    16(%rsp), %rdx
    xorq    %r12, %rcx
    xorq    %r11, %rdi
    xorq    -8(%rsp), %rsi
    xorq    -88(%rsp), %r9
    xorq    -56(%rsp), %r10
    xorq    24(%rsp), %rax
    xorq    -104(%rsp), %r8
    movq    %rdi, -72(%rsp)
    xorq    -24(%rsp), %rdx
    rorx    $9, %rsi, %rsi
    xorq    -120(%rsp), %rdx
    rorx    $23, %r9, %r9
    rorx    $2, %r10, %rdi
    movq    %rsi, %r10
    rorx    $25, %rax, %rax
    movq    %r9, %r11
    rorx    $62, %r8, %r8
    notq    %r10
    orq %rax, %r11
    andq    %rdi, %rsi
    xorq    %r10, %r11
    movq    %r10, %r13
    movq    %r8, %r10
    andq    %r9, %r10
    andq    %rax, %r13
    movq    %r11, 24(%rsp)
    xorq    %rax, %r10
    movq    (%rsp), %rax
    movq    -40(%rsp), %r11
    xorq    -64(%rsp), %r11
    xorq    %rdi, %r13
    xorq    64(%rsp), %rdx
    xorq    %r8, %rsi
    xorq    %r10, %rax
    xorq    %r15, %rax
    xorq    %rbp, %rax
    xorq    %rbx, %r11
    xorq    24(%rsp), %r11
    xorq    %r14, %rax
    movq    %r8, %r14
    xorq    -96(%rsp), %r11
    orq %rdi, %r14
    movq    -16(%rsp), %rdi
    xorq    56(%rsp), %rdi
    xorq    8(%rsp), %rdi
    xorq    %r9, %r14
    xorq    %r13, %rdx
    xorq    -72(%rsp), %rdi
    xorq    %r14, %rcx
    movq    %r14, -8(%rsp)
    rorx    $63, %r11, %r9
    rorx    $63, %rax, %r8
    rorx    $63, %rcx, %r14
    xorq    %rsi, %rdi
    xorq    %rdi, %r9
    rorx    $63, %rdi, %rdi
    xorq    %rdx, %r8
    xorq    %r11, %r14
    xorq    %rax, %rdi
    rorx    $63, %rdx, %r11
    movq    -24(%rsp), %rax
    movq    -64(%rsp), %rdx
    xorq    %rcx, %r11
    movabsq $-9223372034707259384, %rcx
    xorq    %r14, %rbp
    xorq    %rdi, %r12
    rorx    $21, %rbp, %rbp
    rorx    $43, %r12, %r12
    xorq    %r11, %rsi
    xorq    %r9, %rax
    xorq    %r8, %rdx
    rorx    $50, %rsi, %rsi
    xorq    %rax, %rcx
    rorx    $20, %rdx, %rdx
    xorq    %r14, %r10
    movq    %rcx, -64(%rsp)
    movq    %rdx, %rcx
    rorx    $3, %r10, %r10
    orq %rbp, %rcx
    xorq    %rcx, -64(%rsp)
    movq    %rbp, %rcx
    notq    %rcx
    xorq    %r8, %rbx
    orq %r12, %rcx
    rorx    $19, %rbx, %rbx
    xorq    %rdx, %rcx
    andq    %rax, %rdx
    movq    %rcx, -32(%rsp)
    movq    %rsi, %rcx
    xorq    %rsi, %rdx
    andq    %r12, %rcx
    movq    %rdx, -88(%rsp)
    movq    -16(%rsp), %rdx
    xorq    %rbp, %rcx
    movq    %rsi, %rbp
    movq    %rbx, %rsi
    movq    %rcx, -112(%rsp)
    movq    -120(%rsp), %rcx
    orq %rax, %rbp
    movq    40(%rsp), %rax
    xorq    %r11, %rdx
    xorq    %r12, %rbp
    rorx    $44, %rdx, %rdx
    movq    %rbp, -104(%rsp)
    xorq    %r9, %rcx
    rorx    $61, %rcx, %rcx
    xorq    %rdi, %rax
    movq    %rcx, %r12
    rorx    $36, %rax, %rax
    andq    %rcx, %rsi
    orq %rdx, %r12
    xorq    %rdx, %rsi
    xorq    %rax, %r12
    movq    %r12, -56(%rsp)
    movq    %r10, %r12
    notq    %r12
    orq %rbx, %r12
    xorq    %rcx, %r12
    movq    %r10, %rcx
    orq %rax, %rcx
    movq    %r12, -120(%rsp)
    xorq    %rbx, %rcx
    andq    %rax, %rdx
    movq    -80(%rsp), %rbx
    movq    -72(%rsp), %rax
    xorq    %r10, %rdx
    movq    -96(%rsp), %r10
    xorq    %r9, %r13
    movq    %rcx, -16(%rsp)
    movq    %r14, %rcx
    xorq    %rdi, %rbx
    rorx    $46, %r13, %r13
    xorq    %r15, %rcx
    xorq    %r11, %rax
    rorx    $39, %rbx, %rbx
    xorq    %r8, %r10
    rorx    $56, %rax, %rax
    rorx    $58, %rcx, %rcx
    rorx    $63, %r10, %r10
    movq    %rdx, 32(%rsp)
    movq    %rbx, %rbp
    movq    %rax, %r12
    movq    %r13, %rdx
    notq    %rax
    orq %rcx, %rbp
    orq %r10, %rdx
    movq    %rax, %r15
    xorq    %r10, %rbp
    xorq    %rax, %rdx
    andq    %r13, %r15
    movq    -40(%rsp), %rax
    xorq    %rbx, %r15
    andq    %rbx, %r12
    movq    %rbp, -96(%rsp)
    movq    16(%rsp), %rbp
    xorq    %rcx, %r12
    andq    %rcx, %r10
    movq    %r15, -72(%rsp)
    movq    %rdx, -48(%rsp)
    xorq    %r8, %rax
    movq    -8(%rsp), %r15
    movq    56(%rsp), %rdx
    rorx    $54, %rax, %rax
    movq    72(%rsp), %rcx
    xorq    %r9, %rbp
    xorq    %r13, %r10
    rorx    $28, %rbp, %rbp
    movq    %rax, %rbx
    movq    %r10, -24(%rsp)
    xorq    %rdi, %r15
    xorq    %r11, %rdx
    andq    %rbp, %rbx
    xorq    %r14, %rcx
    rorx    $8, %r15, %r10
    rorx    $37, %rdx, %rdx
    rorx    $49, %rcx, %rcx
    movq    %rax, %r15
    xorq    %rdx, %rbx
    orq %rcx, %r15
    notq    %rcx
    movq    %r12, -80(%rsp)
    movq    %rbx, 40(%rsp)
    movq    %rcx, %r12
    movq    %r10, %rbx
    orq %r10, %r12
    andq    %rdx, %rbx
    xorq    %rbp, %r15
    orq %rdx, %rbp
    xorq    %rax, %r12
    xorq    %rcx, %rbx
    xorq    %r10, %rbp
    xorq    (%rsp), %r14
    xorq    48(%rsp), %rdi
    xorq    64(%rsp), %r9
    xorq    8(%rsp), %r11
    xorq    24(%rsp), %r8
    movq    %r12, -8(%rsp)
    rorx    $9, %rdi, %rax
    rorx    $2, %r14, %rcx
    rorx    $23, %r9, %r9
    rorx    $25, %r11, %r11
    movq    %rax, %rdx
    movq    %r9, %r13
    rorx    $62, %r8, %r8
    notq    %rdx
    orq %r11, %r13
    movq    %rdx, %rdi
    movq    %r8, %r14
    xorq    %rdx, %r13
    movq    %r8, %rdx
    andq    %rcx, %rax
    orq %rcx, %rdx
    andq    %r11, %rdi
    andq    %r9, %r14
    xorq    %r9, %rdx
    movq    %r13, (%rsp)
    xorq    %rcx, %rdi
    xorq    %r11, %r14
    movq    %rdx, -40(%rsp)
    xorq    %r8, %rax
    movq    128(%rsp), %rcx
    movq    112(%rsp), %rdx
    addq    %rcx, 80(%rsp)
    subq    120(%rsp), %rdx
    movq    112(%rsp), %r10
    cmpq    %r10, 120(%rsp)
    ja  .L316
    cmpl    $21, 108(%rsp)
    movq    %rdx, 112(%rsp)
    jne .L256
.L313:
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    80(%rsp), %r12
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    80(%rsp), %rcx
    movq    (%r10), %r10
    xorq    %r10, -64(%rsp)
    movq    80(%rsp), %r10
    movq    8(%r11), %r11
    movq    16(%r12), %r12
    movq    24(%r13), %r13
    movq    32(%rdx), %rdx
    movq    40(%rcx), %rcx
    xorq    %r11, -32(%rsp)
    xorq    %r12, -112(%rsp)
    movq    56(%r10), %r11
    movq    64(%r10), %r12
    xorq    %r13, -104(%rsp)
    xorq    %rdx, -88(%rsp)
    movq    72(%r10), %r13
    xorq    %rcx, -56(%rsp)
    xorq    48(%r10), %rsi
    xorq    %r11, -120(%rsp)
    xorq    %r12, -16(%rsp)
    xorq    %r13, 32(%rsp)
    movq    80(%r10), %rdx
    movq    80(%rsp), %r11
    xorq    %rdx, -96(%rsp)
    movq    80(%rsp), %r12
    movq    80(%rsp), %rdx
    movq    80(%rsp), %r13
    movq    88(%r10), %rcx
    movq    104(%r11), %r11
    xorq    %rcx, -80(%rsp)
    movq    96(%r10), %r10
    movq    112(%r12), %r12
    movq    120(%r13), %r13
    movq    136(%rdx), %rcx
    xorq    %r10, -72(%rsp)
    xorq    %r11, -48(%rsp)
    xorq    %r12, -24(%rsp)
    xorq    128(%rdx), %r15
    xorq    %r13, 40(%rsp)
    xorq    %rcx, -8(%rsp)
    xorq    144(%rdx), %rbx
    xorq    152(%rdx), %rbp
    xorq    160(%rdx), %rdi
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L258:
    movq    80(%rsp), %r12
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    80(%rsp), %rcx
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    (%r12), %r12
    xorq    %r12, -64(%rsp)
    movq    80(%rsp), %r12
    movq    8(%r13), %r13
    movq    16(%rdx), %rdx
    xorq    %r13, -32(%rsp)
    xorq    %rdx, -112(%rsp)
    movq    56(%r12), %r13
    movq    64(%r12), %rdx
    movq    24(%rcx), %rcx
    movq    32(%r10), %r10
    xorq    %rcx, -104(%rsp)
    xorq    %r10, -88(%rsp)
    movq    40(%r11), %r11
    xorq    48(%r12), %rsi
    xorq    %r11, -56(%rsp)
    xorq    %r13, -120(%rsp)
    xorq    %rdx, -16(%rsp)
    movq    72(%r12), %rcx
    xorq    %rcx, 32(%rsp)
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    80(%rsp), %rcx
    movq    80(%r12), %r10
    movq    88(%r12), %r11
    xorq    %r10, -96(%rsp)
    xorq    %r11, -80(%rsp)
    movq    96(%r12), %r12
    movq    104(%r13), %r13
    xorq    %r12, -72(%rsp)
    xorq    %r13, -48(%rsp)
    movq    112(%rdx), %rdx
    movq    120(%rcx), %rcx
    xorq    %rdx, -24(%rsp)
    xorq    %rcx, 40(%rsp)
    cmpl    $23, 108(%rsp)
    ja  .L266
    cmpl    $19, 108(%rsp)
    ja  .L267
    cmpl    $17, 108(%rsp)
    ja  .L268
    cmpl    $16, 108(%rsp)
    je  .L257
    movq    80(%rsp), %r10
    xorq    128(%r10), %r15
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L259:
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    80(%rsp), %r12
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    80(%rsp), %rcx
    movq    (%r10), %r10
    xorq    %r10, -64(%rsp)
    movq    80(%rsp), %r10
    movq    8(%r11), %r11
    movq    16(%r12), %r12
    xorq    %r11, -32(%rsp)
    xorq    %r12, -112(%rsp)
    movq    24(%r13), %r13
    movq    32(%rdx), %rdx
    xorq    %r13, -104(%rsp)
    xorq    %rdx, -88(%rsp)
    movq    40(%rcx), %rcx
    xorq    48(%r10), %rsi
    xorq    %rcx, -56(%rsp)
    movq    56(%r10), %r11
    xorq    %r11, -120(%rsp)
    cmpl    $11, 108(%rsp)
    ja  .L263
    cmpl    $9, 108(%rsp)
    ja  .L264
    cmpl    $8, 108(%rsp)
    je  .L257
    movq    64(%r10), %r12
    xorq    %r12, -16(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L266:
    movq    80(%rsp), %r11
    xorq    128(%r11), %r15
    movq    136(%r11), %r12
    xorq    144(%r11), %rbx
    xorq    %r12, -8(%rsp)
    xorq    152(%r11), %rbp
    xorq    160(%r11), %rdi
    movq    168(%r11), %r13
    xorq    176(%r11), %r14
    xorq    %r13, (%rsp)
    movq    184(%r11), %rdx
    xorq    %rdx, -40(%rsp)
    cmpl    $24, 108(%rsp)
    je  .L257
    xorq    192(%r11), %rax
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L316:
    movq    136(%rsp), %rdx
    movq    %rbp, %r13
    movq    -64(%rsp), %r11
    movq    -120(%rsp), %rbp
    movq    %rsi, %r12
    subq    %r10, %rdx
.L255:
    movq    88(%rsp), %rcx
    movq    -32(%rsp), %rsi
    movq    -112(%rsp), %r10
    movq    %r11, (%rcx)
    movq    %rsi, 8(%rcx)
    movq    -104(%rsp), %r11
    movq    -88(%rsp), %rsi
    movq    %r10, 16(%rcx)
    movq    %r12, 48(%rcx)
    movq    -56(%rsp), %r10
    movq    32(%rsp), %r12
    movq    %r11, 24(%rcx)
    movq    %rsi, 32(%rcx)
    movq    -16(%rsp), %r11
    movq    -96(%rsp), %rsi
    movq    %rbp, 56(%rcx)
    movq    -80(%rsp), %rbp
    movq    %r10, 40(%rcx)
    movq    %r12, 72(%rcx)
    movq    -72(%rsp), %r10
    movq    -24(%rsp), %r12
    movq    %r11, 64(%rcx)
    movq    %rsi, 80(%rcx)
    movq    -48(%rsp), %r11
    movq    40(%rsp), %rsi
    movq    %rbp, 88(%rcx)
    movq    -8(%rsp), %rbp
    movq    %r10, 96(%rcx)
    movq    %r12, 112(%rcx)
    movq    %r11, 104(%rcx)
    movq    %r15, 128(%rcx)
    movq    %rbp, 136(%rcx)
    movq    %rsi, 120(%rcx)
    movq    %rbx, 144(%rcx)
    movq    (%rsp), %r10
    movq    -40(%rsp), %r11
    movq    %rax, 192(%rcx)
    movq    %rdx, %rax
    movq    %r13, 152(%rcx)
    movq    %rdi, 160(%rcx)
    movq    %r10, 168(%rcx)
    movq    %r14, 176(%rcx)
    movq    %r11, 184(%rcx)
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
.L260:
    .cfi_restore_state
    movq    80(%rsp), %rdx
    movq    80(%rsp), %rcx
    movq    80(%rsp), %r10
    movq    80(%rsp), %r11
    movq    (%rdx), %rdx
    movq    8(%rcx), %rcx
    xorq    %rdx, -64(%rsp)
    xorq    %rcx, -32(%rsp)
    movq    16(%r10), %r10
    movq    24(%r11), %r11
    xorq    %r10, -112(%rsp)
    xorq    %r11, -104(%rsp)
    cmpl    $5, 108(%rsp)
    ja  .L262
    cmpl    $4, 108(%rsp)
    je  .L257
    movq    80(%rsp), %r12
    movq    32(%r12), %r12
    xorq    %r12, -88(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L267:
    movq    80(%rsp), %r13
    xorq    128(%r13), %r15
    movq    136(%r13), %rdx
    xorq    144(%r13), %rbx
    xorq    %rdx, -8(%rsp)
    xorq    152(%r13), %rbp
    cmpl    $21, 108(%rsp)
    ja  .L269
    cmpl    $20, 108(%rsp)
    je  .L257
    xorq    160(%r13), %rdi
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L263:
    movq    %r10, %r11
    movq    %r10, %r12
    movq    %r10, %r13
    movq    72(%r11), %r11
    movq    64(%r10), %r10
    xorq    %r11, 32(%rsp)
    xorq    %r10, -16(%rsp)
    movq    80(%r12), %r12
    movq    88(%r13), %r13
    xorq    %r12, -96(%rsp)
    xorq    %r13, -80(%rsp)
    cmpl    $13, 108(%rsp)
    ja  .L265
    cmpl    $12, 108(%rsp)
    je  .L257
    movq    80(%rsp), %rdx
    movq    96(%rdx), %rdx
    xorq    %rdx, -72(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L265:
    movq    80(%rsp), %rcx
    movq    80(%rsp), %r10
    movq    96(%rcx), %rcx
    movq    104(%r10), %r10
    xorq    %rcx, -72(%rsp)
    xorq    %r10, -48(%rsp)
    cmpl    $14, 108(%rsp)
    je  .L257
    movq    80(%rsp), %r11
    movq    112(%r11), %r11
    xorq    %r11, -24(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L269:
    xorq    160(%r13), %rdi
    movq    168(%r13), %r10
    xorq    %r10, (%rsp)
    cmpl    $22, 108(%rsp)
    je  .L257
    xorq    176(%r13), %r14
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L262:
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    32(%r13), %r13
    movq    40(%rdx), %rdx
    xorq    %r13, -88(%rsp)
    xorq    %rdx, -56(%rsp)
    cmpl    $6, 108(%rsp)
    je  .L257
    movq    80(%rsp), %rcx
    xorq    48(%rcx), %rsi
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L264:
    movq    80(%rsp), %r13
    movq    80(%rsp), %rdx
    movq    64(%r13), %r13
    movq    72(%rdx), %rdx
    xorq    %r13, -16(%rsp)
    xorq    %rdx, 32(%rsp)
    cmpl    $10, 108(%rsp)
    je  .L257
    movq    80(%rsp), %rcx
    movq    80(%rcx), %rcx
    xorq    %rcx, -96(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L268:
    movq    80(%rsp), %r11
    xorq    128(%r11), %r15
    movq    136(%r11), %r12
    xorq    %r12, -8(%rsp)
    cmpl    $18, 108(%rsp)
    je  .L257
    xorq    144(%r11), %rbx
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L315:
    movq    80(%rsp), %r13
    movq    16(%r13), %r13
    xorq    %r13, -112(%rsp)
    jmp .L257
    .p2align 4,,10
    .p2align 3
.L314:
    movl    108(%rsp), %edx
    testl   %edx, %edx
    je  .L257
    movq    80(%rsp), %r10
    movq    (%r10), %r10
    xorq    %r10, -64(%rsp)
    jmp .L257
.L271:
    xorl    %edx, %edx
    jmp .L255
    .cfi_endproc
.LFE40:
    .size   KeccakF1600_FastLoop_Absorb, .-KeccakF1600_FastLoop_Absorb
    .p2align 4,,15
    .ident  "GCC: (SUSE Linux) 4.7.4"
    .section    .note.GNU-stack,"",@progbits
