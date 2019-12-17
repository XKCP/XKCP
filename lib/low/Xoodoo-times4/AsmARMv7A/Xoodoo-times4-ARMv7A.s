@
@ Implementation by Conno Boel, hereby denoted as "the implementer".
@
@ *NOT* an official member of the Keccak Team (https://keccak.team/)
@
@ To the extent possible under law, the implementer has waived all copyright
@ and related or neighboring rights to the source code in this file.
@ http://creativecommons.org/publicdomain/zero/1.0/
@

@ WARNING: These functions work only on little endian CPU with@ ARMv7A + NEON architecture (Cortex-A8, ...).

.text

@ Xoodootimes4_InitializeAll: void * states -> void
.align 8
.global Xoodootimes4_InitializeAll
.type Xoodootimes4_InitializeAll, %function
Xoodootimes4_InitializeAll:
  vmov.i32  q0, #0
  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}

  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}

  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}

  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}
  vstm      r0!, {d0-d1}
  bx        lr


@ Xoodootimes4_AddByte: void * states -> uint instanceIndex -> const uchar byte -> uint offset -> void
.align 8
.global Xoodootimes4_AddByte
.type Xoodootimes4_AddByte, %function
Xoodootimes4_AddByte:
  add       r1, r1, r3
  and       r3, r3, #3
  sub       r1, r1, r3
  add       r0, r0, r1, lsl #2 @ states+(WORD instanceIndex)
  add       r0, r0, r3
  ldrb      r3, [r0]
  eor       r3, r3, r2
  strb      r3, [r0]
  bx        lr

@ Xoodootimes4_AddBytes: void * states -> uint instanceIndex -> const uchar * data -> uint offset -> uint length -> void
.align 8
.global Xoodootimes4_AddBytes
.type Xoodootimes4_AddBytes, %function
Xoodootimes4_AddBytes:
  add       r1, r1, r3
  and       r3, r3, #3
  sub       r1, r1, r3
  add       r0, r0, r1, lsl #2 @ states+(WORD instanceIndex)
  add       r0, r0, r3

  ldr       r1, [sp]
  subs      r1, r1, #1
  bxcc      lr

  @ r0 start
  @ r1 lenght > 0
  @ r2 data
  @ r3 byte offset {0,1,2,3}

  push      {r4, r5, lr}
Xt4_AddBytes_Loop:
  ldrb      r4, [r0]
  ldrb      r5, [r2], #1
  eor       r4, r4, r5
  strb      r4, [r0], #1
  and       r5, r0, #3
  cmp       r5, #0
  addeq     r0, r0, #12 @ Skip state
  subs      r1, r1, #1
  bcs       Xt4_AddBytes_Loop
  pop       {r4, r5, pc}

@ Xoodootimes4_AddLanesAll: void * states -> const uchar * data -> uint laneCount -> uint laneOffset -> void
.align 8
.global Xoodootimes4_AddLanesAll
.type Xoodootimes4_AddLanesAll, %function
Xoodootimes4_AddLanesAll:
  cmp       r2, r3
  cmpeq     r2, #12
  tsteq     r1, #3
  moveq     r3, lr
  beq       Xt4_AddLanesAll_Full

  push      {r4-r7,lr}

  add       r4, r1, r3, lsl #2
  add       r5, r4, r3, lsl #2
  add       r6, r5, r3, lsl #2

  subs      r2, r2, #1
  popcc     {r4-r7,pc}

  and       r3, r1, #3
  cmp       r3, #0

  bne       Xt4_AddLanesAll_Unaligned_Loop
Xt4_AddLanesAll_Aligned_Loop:
  vldm      r0, {d0, d1}
  ldr       r3, [r1], #4
  vmov      s4, r3
  ldr       r3, [r4], #4
  vmov      s5, r3
  ldr       r3, [r5], #4
  vmov      s6, r3
  ldr       r3, [r6], #4
  vmov      s7, r3
  veor      q0, q0, q1
  vstm      r0!, {d0, d1}
  subs      r2, r2, #1
  bcs       Xt4_AddLanesAll_Aligned_Loop
  pop       {r4-r7,pc}
Xt4_AddLanesAll_Unaligned_Loop:
  vldm      r0, {d0, d1}

  ldrb      r3, [r1], #1
  ldrb      r7, [r1], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r1], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r1], #1
  eor       r3, r3, r7, lsl #24
  vmov      s4, r3

  ldrb      r3, [r4], #1
  ldrb      r7, [r4], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r4], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r4], #1
  eor       r3, r3, r7, lsl #24
  vmov      s5, r3

  ldrb      r3, [r5], #1
  ldrb      r7, [r5], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r5], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r5], #1
  eor       r3, r3, r7, lsl #24
  vmov      s6, r3

  ldrb      r3, [r6], #1
  ldrb      r7, [r6], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r6], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r6], #1
  eor       r3, r3, r7, lsl #24
  vmov      s7, r3

  veor      q0, q0, q1
  vstm      r0!, {d0, d1}
  subs      r2, r2, #1
  bcs       Xt4_AddLanesAll_Unaligned_Loop
  pop       {r4-r7,pc}
Xt4_AddLanesAll_Full:
  vldm      r1!, {d0-d15}
  vuzp.32   q0, q6
  vldm      r1, {d16-d23}
  vuzp.32   q3, q9
  vtrn.32   q0, q3
  vtrn.32   q6, q9

  vuzp.32   q1, q7
  vuzp.32   q4, q10
  vtrn.32   q1, q4
  vtrn.32   q7, q10

  vuzp.32   q2, q8
  vuzp.32   q5, q11
  vtrn.32   q2, q5
  vtrn.32   q8, q11

  vldm      r0, {d24-d31}
  veor      q12, q0, q12
  veor      q13, q6, q13
  veor      q14, q3, q14
  veor      q15, q9, q15
  vstm      r0!, {d24-d31}
  vldm      r0, {d24-d31}
  veor      q12, q1, q12
  veor      q13, q7, q13
  veor      q14, q4, q14
  veor      q15, q10, q15
  vstm      r0!, {d24-d31}
  vldm      r0, {d24-d31}
  veor      q12, q2, q12
  veor      q13, q8, q13
  veor      q14, q5, q14
  veor      q15, q11, q15
  vstm      r0, {d24-d31}
  mov       pc, r3

@ Xoodootimes4_OverwriteBytes: void * states -> uint instanceIndex -> const uchar * data -> uint offset -> uint length -> void
.align 8
.global Xoodootimes4_OverwriteBytes
.type Xoodootimes4_OverwriteBytes, %function
Xoodootimes4_OverwriteBytes:
  push      {r4, lr}
  ldr       r4, [sp, #8]
  cmp       r4, #48
  tsteq     r2, #3
  beq       Xt4_OverwriteBytes_Full

  add       r1, r1, r3
  and       r3, r3, #3
  sub       r1, r1, r3
  add       r0, r0, r1, lsl #2 @ states+(WORD instanceIndex)
  add       r0, r0, r3

  subs      r1, r4, #1
  popcc     {r4, pc}

  @ r0 start
  @ r1 lenght > 0
  @ r2 data
  @ r3 byte offset {0,1,2,3}

Xt4_OverwriteBytes_Loop:
  ldrb      r4, [r2], #1
  strb      r4, [r0], #1
  and       r4, r0, #3
  cmp       r4, #0
  addeq     r0, r0, #12 @ Skip state
  subs      r1, r1, #1
  bcs       Xt4_OverwriteBytes_Loop
  pop       {r4, pc}
Xt4_OverwriteBytes_Full:
  add       r0, r0, r1, lsl #2
  ldmia     r2!, {r1, r3, r4, r14}
  str       r1, [r0], #16
  str       r3, [r0], #16
  str       r4, [r0], #16
  str       r14, [r0], #16
  ldmia     r2!, {r1, r3, r4, r14}
  str       r1, [r0], #16
  str       r3, [r0], #16
  str       r4, [r0], #16
  str       r14, [r0], #16
  ldmia     r2, {r1, r3, r4, r14}
  str       r1, [r0], #16
  str       r3, [r0], #16
  str       r4, [r0], #16
  str       r14, [r0], #16
  pop       {r4, pc}

@ Xoodootimes4_OverwriteLanesAll: void * states -> uchar * data -> uint lanecount -> uint laneOffset -> void
.align 8
.global Xoodootimes4_OverwriteLanesAll
.type Xoodootimes4_OverwriteLanesAll, %function
Xoodootimes4_OverwriteLanesAll:
  push      {r4-r6,lr}

  add       r4, r1, r3, lsl #2
  add       r5, r4, r3, lsl #2
  add       r6, r5, r3, lsl #2

  subs      r2, r2, #1
  popcc     {r4-r6,pc}

  and       r3, r1, #3
  cmp       r3, #0
  bne       Xt4_OverwriteLanesAll_Unaligned_Loop

Xt4_OverwriteLanesAll_Aligned_Loop:
  ldr       r3, [r1], #4
  vmov      s0, r3
  ldr       r3, [r4], #4
  vmov      s1, r3
  ldr       r3, [r5], #4
  vmov      s2, r3
  ldr       r3, [r6], #4
  vmov      s3, r3
  vstm      r0!, {d0-d1}
  subs      r2, r2, #1
  bcs       Xt4_OverwriteLanesAll_Aligned_Loop
  pop       {r4-r6,pc}
Xt4_OverwriteLanesAll_Unaligned_Loop:
  push      {r7}

  ldrb      r3, [r1], #1
  ldrb      r7, [r1], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r1], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r1], #1
  eor       r3, r3, r7, lsl #24
  vmov      s0, r3

  ldrb      r3, [r4], #1
  ldrb      r7, [r4], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r4], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r4], #1
  eor       r3, r3, r7, lsl #24
  vmov      s1, r3

  ldrb      r3, [r5], #1
  ldrb      r7, [r5], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r5], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r5], #1
  eor       r3, r3, r7, lsl #24
  vmov      s2, r3

  ldrb      r3, [r6], #1
  ldrb      r7, [r6], #1
  eor       r3, r3, r7, lsl #8
  ldrb      r7, [r6], #1
  eor       r3, r3, r7, lsl #16
  ldrb      r7, [r6], #1
  eor       r3, r3, r7, lsl #24
  vmov      s3, r3

  vstm      r0!, {d0-d1}
  pop       {r7}
  subs      r2, r2, #1
  bcs       Xt4_OverwriteLanesAll_Unaligned_Loop
  pop       {r4-r6,pc}


@ Xoodootimes4_OverwriteWithZeroes: void * states -> uint instanceIndex -> uint byteCount -> void
.align 8
.global Xoodootimes4_OverwriteWithZeroes
.type Xoodootimes4_OverwriteWithZeroes, %function
Xoodootimes4_OverwriteWithZeroes:
  add       r0, r0, r1, lsl #2  @ states + 4*instance = state start
  mov       r1, #0
  mov       r3, lr
Xt4_OverwriteWithZeroes_Aligned:
  subs      r2, r2, #4
  strcs     r1, [r0], #16
  bhi       Xt4_OverwriteWithZeroes_Aligned
  moveq     pc, r3
  add       r2, r2, #4
Xt4_OverwriteWithZeroes_Leftovers:
  subs      r2, r2, #1
  movcc     pc, r3
  strb      r1, [r0], #1
  bhi       Xt4_OverwriteWithZeroes_Leftovers
  mov       pc, r3


@ Xoodootimes4_ExtractBytes: void * states -> uint instanceIndex -> const uchar * data -> uint offset -> uint length -> void
.align 8
.global Xoodootimes4_ExtractBytes
.type Xoodootimes4_ExtractBytes, %function
Xoodootimes4_ExtractBytes:
  add       r1, r1, r3
  and       r3, r3, #3
  sub       r1, r1, r3
  add       r0, r0, r1, lsl #2 @ states+(WORD instanceIndex)
  add       r0, r0, r3

  ldr       r1, [sp]
  subs      r1, r1, #1
  bxcc      lr

  push      {r4, lr}
Xt4_ExtractBytes_Loop:
  ldrb      r4, [r0], #1
  strb      r4, [r2], #1
  and       r4, r0, #3
  cmp       r4, #0
  addeq     r0, r0, #12 @ Skip state
  subs      r1, r1, #1
  bcs       Xt4_ExtractBytes_Loop
  pop       {r4, pc}

@ Xoodootimes4_ExtractLanesAll: void * states -> uchar * data -> uint lanecount -> uint laneoffset -> void
.align 8
.global Xoodootimes4_ExtractLanesAll
.type Xoodootimes4_ExtractLanesAll, %function
Xoodootimes4_ExtractLanesAll:
  push      {r4-r6,lr}

  add       r4, r1, r3, lsl #2
  add       r5, r4, r3, lsl #2
  add       r6, r5, r3, lsl #2

  subs      r2, r2, #1
  popcc     {r4-r6,pc}

  and       r3, r1, #3
  cmp       r3, #0
  bne       Xt4_ExtractLanesAll_Unaligned_Loop
Xt4_ExtractLanesAll_Aligned_Loop:
  vldm      r0!, {d0-d1}
  vmov      r3, s0
  str       r3, [r1], #4
  vmov      r3, s1
  str       r3, [r4], #4
  vmov      r3, s2
  str       r3, [r5], #4
  vmov      r3, s3
  str       r3, [r6], #4
  subs      r2, r2, #1
  bcs       Xt4_ExtractLanesAll_Aligned_Loop
  pop       {r4-r6,pc}
Xt4_ExtractLanesAll_Unaligned_Loop:
  push      {r7}
  vldm      r0!, {d0-d1}
  vmov      r3, s0
  strb      r3, [r1], #1
  lsr       r3, r3, #8
  strb      r3, [r1], #1
  lsr       r3, r3, #8
  strb      r3, [r1], #1
  lsr       r3, r3, #8
  strb      r3, [r1], #1

  vmov      r3, s1
  strb      r3, [r4], #1
  lsr       r3, r3, #8
  strb      r3, [r4], #1
  lsr       r3, r3, #8
  strb      r3, [r4], #1
  lsr       r3, r3, #8
  strb      r3, [r4], #1

  vmov      r3, s2
  strb      r3, [r5], #1
  lsr       r3, r3, #8
  strb      r3, [r5], #1
  lsr       r3, r3, #8
  strb      r3, [r5], #1
  lsr       r3, r3, #8
  strb      r3, [r5], #1

  vmov      r3, s3
  strb      r3, [r6], #1
  lsr       r3, r3, #8
  strb      r3, [r6], #1
  lsr       r3, r3, #8
  strb      r3, [r6], #1
  lsr       r3, r3, #8
  strb      r3, [r6], #1

  pop       {r7}
  subs      r2, r2, #1
  bcs       Xt4_ExtractLanesAll_Unaligned_Loop
  pop       {r4-r6,pc}

@ Xoodootimes4_ExtractAndAddBytes: void * states -> uint instanceIndex -> uchar * input -> uchar * output -> uint offset -> uint length -> void
.align 8
.global Xoodootimes4_ExtractAndAddBytes
.type Xoodootimes4_ExtractAndAddBytes, %function
Xoodootimes4_ExtractAndAddBytes:
  push      {r4, r5, lr}
  ldr       r4, [sp, #12]
  ldr       r5, [sp, #16]
  cmp       r5, #48
  tsteq     r2, #3
  tsteq     r3, #3
  beq       Xt4_ExtractAndAddBytes_Full

  add       r1, r1, r4
  and       r4, r4, #3
  sub       r1, r1, r4
  add       r0, r0, r1, lsl #2 @ states+(WORD instanceIndex)
  add       r0, r0, r4

  subs      r1, r5, #1
  popcc     {r4, r5, pc}

Xt4_ExtractAndAddBytes_Loop:
  ldrb      r4, [r0], #1
  ldrb      r5, [r2], #1
  eor       r4, r4, r5
  strb      r4, [r3], #1
  and       r4, r0, #3
  cmp       r4, #0
  addeq     r0, r0, #12 @ Skip state
  subs      r1, r1, #1
  bcs       Xt4_ExtractAndAddBytes_Loop
  pop       {r4, r5, pc}
Xt4_ExtractAndAddBytes_Full:
  add       r0, r0, r1, lsl #2
  ldmia     r2!, {r1, r4, r5}
  ldr       r14, [r0], #16
  eor       r1, r1, r14
  ldr       r14, [r0], #16
  eor       r4, r4, r14
  ldr       r14, [r0], #16
  eor       r5, r5, r14
  stmia     r3!, {r1, r4, r5}
  ldmia     r2!, {r1, r4, r5}
  ldr       r14, [r0], #16
  eor       r1, r1, r14
  ldr       r14, [r0], #16
  eor       r4, r4, r14
  ldr       r14, [r0], #16
  eor       r5, r5, r14
  stmia     r3!, {r1, r4, r5}
  ldmia     r2!, {r1, r4, r5}
  ldr       r14, [r0], #16
  eor       r1, r1, r14
  ldr       r14, [r0], #16
  eor       r4, r4, r14
  ldr       r14, [r0], #16
  eor       r5, r5, r14
  stmia     r3!, {r1, r4, r5}
  ldmia     r2, {r1, r4, r5}
  ldr       r14, [r0], #16
  eor       r1, r1, r14
  ldr       r14, [r0], #16
  eor       r4, r4, r14
  ldr       r14, [r0], #16
  eor       r5, r5, r14
  stmia     r3, {r1, r4, r5}
  pop       {r4, r5, pc}


@ Xoodootimes4_ExtractAndAddLanesAll: void * states -> uchar * input -> uchar * output -> uint laneCount -> uint laneOffset
.align 8
.global Xoodootimes4_ExtractAndAddLanesAll
.type Xoodootimes4_ExtractAndAddLanesAll, %function
Xoodootimes4_ExtractAndAddLanesAll:
  subs      r3, r3, #1
  bxcc      lr

  push      {r4-r11,lr}
  ldr       r9, [sp, #36]

  add       r4, r1, r9, lsl #2 @ r4 = r1 + 48
  add       r5, r4, r9, lsl #2 @ r5 = r1 + 96
  add       r6, r5, r9, lsl #2 @ r6 = r1 + 144

  add       r7, r2, r9, lsl #2 @ r7 = r2 + 48
  add       r8, r7, r9, lsl #2 @ r8 = r2 + 96
  add       r9, r8, r9, lsl #2 @ r9 = r2 + 144

Xt4_ExtractAndAddLanesAll_Unaligned_Loop:
  vldm      r0!, {d2-d3}

  ldrb      r11, [r1], #1
  ldrb      r10, [r1], #1
  eor       r11, r11, r10, lsl #8
  ldrb      r10, [r1], #1
  eor       r11, r11, r10, lsl #16
  ldrb      r10, [r1], #1
  eor       r11, r11, r10, lsl #24
  vmov      s0, r11

  ldrb      r11, [r4], #1
  ldrb      r10, [r4], #1
  eor       r11, r11, r10, lsl #8
  ldrb      r10, [r4], #1
  eor       r11, r11, r10, lsl #16
  ldrb      r10, [r4], #1
  eor       r11, r11, r10, lsl #24
  vmov      s1, r11

  ldrb      r11, [r5], #1
  ldrb      r10, [r5], #1
  eor       r11, r11, r10, lsl #8
  ldrb      r10, [r5], #1
  eor       r11, r11, r10, lsl #16
  ldrb      r10, [r5], #1
  eor       r11, r11, r10, lsl #24
  vmov      s2, r11

  ldrb      r11, [r6], #1
  ldrb      r10, [r6], #1
  eor       r11, r11, r10, lsl #8
  ldrb      r10, [r6], #1
  eor       r11, r11, r10, lsl #16
  ldrb      r10, [r6], #1
  eor       r11, r11, r10, lsl #24
  vmov      s3, r11
  veor      q0, q0, q1

  vmov      r10, s0
  strb      r10, [r2], #1
  lsr       r10, r10, #8
  strb      r10, [r2], #1
  lsr       r10, r10, #8
  strb      r10, [r2], #1
  lsr       r10, r10, #8
  strb      r10, [r2], #1

  vmov      r10, s1
  strb      r10, [r7], #1
  lsr       r10, r10, #8
  strb      r10, [r7], #1
  lsr       r10, r10, #8
  strb      r10, [r7], #1
  lsr       r10, r10, #8
  strb      r10, [r7], #1

  vmov      r10, s2
  strb      r10, [r8], #1
  lsr       r10, r10, #8
  strb      r10, [r8], #1
  lsr       r10, r10, #8
  strb      r10, [r8], #1
  lsr       r10, r10, #8
  strb      r10, [r8], #1

  vmov      r10, s3
  strb      r10, [r9], #1
  lsr       r10, r10, #8
  strb      r10, [r9], #1
  lsr       r10, r10, #8
  strb      r10, [r9], #1
  lsr       r10, r10, #8
  strb      r10, [r9], #1

  subs      r3, r3, #1
  bcs       Xt4_ExtractAndAddLanesAll_Unaligned_Loop
  pop       {r4-r11,pc}


@ Xoodootimes4_Permute_once:
@ q0: a00 -> q1: a01 -> q2:  a02 -> q3:  a03 ->
@ q4: a10 -> q5: a11 -> q6:  a12 -> q7:  a13 ->
@ q8: a20 -> q9: a21 -> q10: a22 -> q11: a23

.macro round
  theta
  rho_w
  chi
  rho_e
.endm

.macro theta
  veor      q15, q3, q7
  veor      q15, q15, q11

  vmov.32   r3, r4, d30
  vmov.32   r1, r2, d31
  ror       r3, r3, #27
  veor      q14, q0, q4
  ror       r4, r4, #27
  veor      q14, q14, q8
  ror       r1, r1, #27
  ror       r2, r2, #27
  eor       r3, r3, r3, ror #23
  eor       r4, r4, r4, ror #23
  eor       r1, r1, r1, ror #23
  vmov.32   d30, r3, r4
  eor       r2, r2, r2, ror #23
  vmov.32   d31, r1, r2

  vmov.32   r3, r4, d28
  vmov.32   r1, r2, d29
  ror       r3, r3, #27
  veor      q0, q0, q15
  ror       r4, r4, #27
  veor      q4, q4, q15
  ror       r1, r1, #27
  veor      q8, q8, q15
  ror       r2, r2, #27
  veor      q15, q1, q5
  eor       r3, r3, r3, ror #23
  veor      q15, q15, q9
  eor       r4, r4, r4, ror #23
  eor       r1, r1, r1, ror #23
  vmov.32   d28, r3, r4
  eor       r2, r2, r2, ror #23
  vmov.32   d29, r1, r2

  vmov.32   r3, r4, d30
  vmov.32   r1, r2, d31
  ror       r3, r3, #27
  veor      q1, q1, q14
  ror       r4, r4, #27
  veor      q5, q5, q14
  ror       r1, r1, #27
  veor      q9, q9, q14
  ror       r2, r2, #27
  veor      q14, q2, q6
  eor       r3, r3, r3, ror #23
  veor      q14, q14, q10
  eor       r4, r4, r4, ror #23
  eor       r1, r1, r1, ror #23
  vmov.32   d30, r3, r4
  eor       r2, r2, r2, ror #23
  vmov.32   d31, r1, r2

  vmov.32   r3, r4, d28
  vmov.32   r1, r2, d29
  ror       r3, r3, #27
  veor      q2, q2, q15
  ror       r4, r4, #27
  veor      q6, q6, q15
  ror       r1, r1, #27
  veor      q10, q10, q15
  ror       r2, r2, #27
  eor       r3, r3, r3, ror #23
  eor       r4, r4, r4, ror #23
  eor       r1, r1, r1, ror #23
  vmov.32   d28, r3, r4
  eor       r2, r2, r2, ror #23
  vmov.32   d29, r1, r2
  veor      q3, q3, q14
  veor      q7, q7, q14
  veor      q11, q11, q14
.endm

.macro rho_w
  @ vshl.U32  q12, q8, #11
  @ vsri.U32  q12, q8, #21
  vmov.32   r1, r2, d16
  vshl.U32  q13, q9, #11
  vmov.32   r3, r4, d17
  vsri.U32  q13, q9, #21
  ror       r1, r1, #21
  vshl.U32  q14, q10, #11
  ror       r2, r2, #21
  vsri.U32  q14, q10, #21
  ror       r3, r3, #21
  vshl.U32  q15, q11, #11
  ror       r4, r4, #21
  vsri.U32  q15, q11, #21
  vmov.32   d24, r1, r2
  vmov.32   d25, r3, r4
  @ NOTE: Here we are hiding in the shadows. What happens is that the ROR action is interleaved with the vector actions so that they get executed for free instead of a NOP .
.endm

.macro chi
  @ NOTE: Iota
  vdup.32   q8, r3
  veor      q0, q0, q8

  @ NOTE: Probably this is optimal.  (Prove?)
  vbic      q11, q12, q7
  vbic      q9, q0, q12
  vbic      q10, q7, q0
  veor      q8, q10, q12
  veor      q12, q7, q9
  veor      q0, q0, q11

  vbic      q7, q13, q4
  vbic      q10, q1, q13
  vbic      q11, q4, q1
  veor      q9, q11, q13
  veor      q13, q4, q10
  veor      q1, q1, q7

  vbic      q4, q14, q5
  vbic      q11, q2, q14
  vbic      q7, q5, q2
  veor      q10, q7, q14
  veor      q14, q5, q11
  veor      q2, q2, q4

  vbic      q5, q15, q6
  vbic      q7, q3, q15
  vbic      q4, q6, q3
  veor      q4, q4, q15
  veor      q15, q6, q7
  veor      q3, q3, q5
.endm

.macro rho_e
  vshl.U32  q11, q9, #8
  vsri.U32  q11, q9, #24

  vshl.U32  q9, q4, #8
  vsri.U32  q9, q4, #24

  vmov.32   r1, r2, d16
  vmov.32   r3, r4, d17
  ror       r1, r1, #24
  vshl.U32  q8, q10, #8
  ror       r2, r2, #24
  vsri.U32  q8, q10, #24
  ror       r3, r3, #24
  vmov.32   d20, r1, r2
  ror       r4, r4, #24
  vmov.32   d21, r3, r4

  vshl.U32  q4, q12, #1
  vsri.U32  q4, q12, #31

  vshl.U32  q5, q13, #1
  vsri.U32  q5, q13, #31

  vshl.U32  q6, q14, #1
  vsri.U32  q6, q14, #31

  vshl.U32  q7, q15, #1
  vsri.U32  q7, q15, #31
.endm

@ NOTE: The idea was to maybe merge rho_e and theta partially, however because P depends on the registers it also XORs into, we do not save cycles by stepping to core registers. Because at no point can we use the barrel shifter, which is the only reason we should want to choose the core registers over the vector registers.

@ Xoodootimes4_PermuteAll_6rounds: void * argStates -> void
.align 8
.global Xoodootimes4_PermuteAll_6rounds
.type Xoodootimes4_PermuteAll_6rounds, %function
Xoodootimes4_PermuteAll_6rounds:
  vpush     {d8-d15}
  push      {r4}
  vldm      r0!, {d0-d15}
  vldm      r0, {d16-d23}
  sub       r0, r0, #128 @ (16*64)/8

  theta
  rho_w
  mov       r3, #0x00000060
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x0000002C
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000380
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x000000F0
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x000001A0
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000012
  chi
  rho_e

  vstm      r0!, {d0-d15}
  vstm      r0, {d16-d23}
  pop       {r4}
  vpop      {d8-d15}
  bx        lr

@ Xoodootimes4_PermuteAll_12rounds:
.align 8
.global Xoodootimes4_PermuteAll_12rounds
.type Xoodootimes4_PermuteAll_12rounds, %function
Xoodootimes4_PermuteAll_12rounds:
  vpush     {d8-d15}
  push      {r4-r5}
  vldm      r0!, {d0-d15}
  vldm      r0, {d16-d23}
  sub       r0, r0, #128

  theta
  rho_w
  mov       r3, #0x00000058
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000038
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x000003C0
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x000000D0
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000120
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000014
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000060
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x0000002C
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000380
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x000000F0
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x000001A0
  chi
  rho_e

  theta
  rho_w
  mov       r3, #0x00000012
  chi
  rho_e
  vstm      r0!, {d0-d15}
  vstm      r0, {d16-d23}
  pop       {r4-r5}
  vpop      {d8-d15}
  bx        lr
