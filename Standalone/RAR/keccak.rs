/* -*- mode: asm; fill-column: 75; comment-column: 50 -*- */

;;  Implementation of the contemporary FIPS 202 Standard by the Zephyr Pellerin
;;  hereby denoted as "the implementer".

;; For more information, feedback or questions, please refer to our websites:
;; http://keccak.noekeon.org/
;; http://keyak.noekeon.org/
;; http://ketje.noekeon.org/

;; To the extent possible under law, the implementer has waived all copyright
;; and related or neighboring rights to the source code in this file.
;; http://creativecommons.org/publicdomain/zero/1.0/

;; INFO: Tested on unrar 4.35, later versions do not include the RAR virtual machine.

#include <constants.rh>
#include <crctools.rh>
#include <math.rh>
#include <util.rh>

;; Magic memory pointers to store important
;; keccak specification constants
#define RC_BASE         #0x00004096
#define ROT_OFFSETS     #0x00002800
#define TRIANGLR_NUMS   #0x00002048
#define INT_BC          #0x00003000 ; used internally

;; Implementation constants
#define TEST_VECTOR     #0x00004000
#define TEST_VECTOR_LEN #28
#define ROW_STATE       #0x00005000

;; This number is not magic
;; it is derived from 200 - (2 * Message Digest Length)
;; where mdlen = 32, the mdlen of SHA-256
#define RSIZ #72
#define RSIZW #9 ; RSIZ / 8

;; Keccak permutations are designated by keccak-f[b] where b defines the width of the
;; permutation, the number of rounds depends on the width (in our case 1600, the highest)
;; and is given by nr = 12 + 2l where 2^l = b / 25. This gives 24 rounds
#define KECCAK_ROUNDS #24

_start:
  ; set our Keccak spec defined rotation offsets
  mov r0, ROT_OFFSETS
  mov [r0+#4], #1
  mov [r0+#8], #3
  mov [r0+#12], #6
  mov [r0+#16], #10
  mov [r0+#20], #15
  mov [r0+#24], #21
  mov [r0+#28], #28
  mov [r0+#32], #36
  mov [r0+#36], #45
  mov [r0+#40], #55
  mov [r0+#44], #2
  mov [r0+#48], #14
  mov [r0+#52], #27
  mov [r0+#56], #41
  mov [r0+#60], #56
  mov [r0+#64], #8
  mov [r0+#68], #25
  mov [r0+#72], #43
  mov [r0+#76], #62
  mov [r0+#80], #18
  mov [r0+#84], #39
  mov [r0+#88], #61
  mov [r0+#92], #20
  mov [r0+#96], #44

  ; define some triangular numbers
  mov r0, TRIANGLR_NUMS
  mov [r0+#4], #10
  mov [r0+#8], #7
  mov [r0+#12], #11
  mov [r0+#16], #17
  mov [r0+#20], #18
  mov [r0+#24], #3
  mov [r0+#28], #5
  mov [r0+#32], #16
  mov [r0+#36], #8
  mov [r0+#40], #21
  mov [r0+#44], #24
  mov [r0+#48], #4
  mov [r0+#52], #15
  mov [r0+#56], #23
  mov [r0+#60], #19
  mov [r0+#64], #13
  mov [r0+#68], #12
  mov [r0+#72], #2
  mov [r0+#76], #20
  mov [r0+#80], #14
  mov [r0+#84], #22
  mov [r0+#88], #9
  mov [r0+#92], #6
  mov [r0+#96], #1

  ; define our round constants (64 w/ 32 bit words, hence the doubling)
  mov r0, RC_BASE
  mov [r0+#4], #0x00000001
  mov [r0+#8], #0x00000001
  mov [r0+#12], #0x00000000
  mov [r0+#16], #0x00008082
  mov [r0+#20], #0x80000000
  mov [r0+#24], #0x0000808a
  mov [r0+#28], #0x80000000
  mov [r0+#32], #0x80008000
  mov [r0+#36], #0x00000000
  mov [r0+#40], #0x0000808b
  mov [r0+#44], #0x00000000
  mov [r0+#48], #0x80000001
  mov [r0+#52], #0x80000000
  mov [r0+#56], #0x80008081
  mov [r0+#60], #0x80000000
  mov [r0+#64], #0x00008009
  mov [r0+#68], #0x00000000
  mov [r0+#72], #0x0000008a
  mov [r0+#76], #0x00000000
  mov [r0+#80], #0x00000088
  mov [r0+#84], #0x00000000
  mov [r0+#88], #0x80008009
  mov [r0+#92], #0x00000000
  mov [r0+#96], #0x8000000a
  mov [r0+#100], #0x00000000
  mov [r0+#104], #0x8000808b
  mov [r0+#108], #0x80000000
  mov [r0+#112], #0x0000008b
  mov [r0+#116], #0x80000000
  mov [r0+#120], #0x00008089
  mov [r0+#124], #0x80000000
  mov [r0+#128], #0x00008003
  mov [r0+#132], #0x80000000
  mov [r0+#136], #0x00008002
  mov [r0+#140], #0x80000000
  mov [r0+#144], #0x00000080
  mov [r0+#148], #0x00000000
  mov [r0+#152], #0x0000800a
  mov [r0+#156], #0x80000000
  mov [r0+#160], #0x8000000a
  mov [r0+#164], #0x80000000
  mov [r0+#168], #0x80008081
  mov [r0+#172], #0x80000000
  mov [r0+#176], #0x00008080
  mov [r0+#180], #0x00000000
  mov [r0+#184], #0x80000001
  mov [r0+#188], #0x80000000
  mov [r0+#192], #0x80008008

  ; our test vector for 24 round Keccak-256 "b0w.1z.1984&N0W"
  mov r0, TEST_VECTOR
  mov [r0+#4], #0x2e773062
  mov [r0+#8], #0x312e7a31
  mov [r0+#12], #0x26343839
  mov [r0+#16], #0x3b57304e


  call $keccak
  mov     [VMADDR_NEWBLOCKPOS], TEST_VECTOR
  mov     [VMADDR_NEWBLOCKSIZE], #100

  ; Compensate to required CRC
  push    RAR_FILECRC
  push    [VMADDR_NEWBLOCKSIZE]
  push    [VMADDR_NEWBLOCKPOS]
  call    $_compensate_crc
  test    r0, r0
  jz      $finished
  call    $_error

finished:
    call    $_success


;; this function does bitwise rotation on a 64 bit value
;; with 32 bits of precision
;; adapted from similar HACKMEM algorithm!
;; ( mad respect from the youth of today! )
rotate:
  push r6     ; save frame pointer
  mov r6, r7  ; create new frame
  pop r0 ; r0 contains the count
  pop r1 ; r1 contains the low value
  pop r2 ; r2 contains the high value
  and r0, #0x3F
  cmp r0, #0x1F
  jbe $inf32
  ; swap our values
  mov r3, r1
  mov r1, r2
  mov r2, r3
  and  r0, #0x1F
inf32:
  ; hakmem magic ahead
  mov r5, #32
  sub r5, r0
  mov r4, r2
  shr r4, r5
  mov r4, r4
  mov r4, r1
  shl r4, r0
  or r4, r4
  shl r2, r0
  shr r1, r5
  or r2, r1
  mov r1, r4
  mov r0, r2 ; our return value, the beginning of the 64 bit int
  mov     r7, r6                      ; clear frame
  pop     r6
  ret

keccak:
  ; Absorbing phase
  ; defined in case you need to change the size of your input vector
  ; forall block Pi in P
  ;   S[x,y] = S[x,y] xor Pi[x+5*y],          forall (x,y) such that x+5*y < r/w
  ;   S = Keccak-f[r+c](S)
  mov r0, RSIZ
  mov r1, TEST_VECTOR_LEN
  mov r3, INT_BC
  mov r4, TEST_VECTOR
  call $keccak_round
  ret

keccak_round:
  mov r2, #0
  call $xor_slice
  call $_keccak_round

  ; you can rewrite this logic if you'd like to test
  ; messages with a size greater than that of a single
  ; 5x5 (25 byte) slice
  sub r1, RSIZ
  add r1, RSIZ
  cmp TEST_VECTOR_LEN, r0; rounds
  push r0
  jmp $keccak_round
  ret


xor_slice:
  ; xor twice because weve only got 32 bits of precision here
  ; and we are operating on 64 bit values, keep this in mind
  xor [r3+r2], [r4+r2]
  xor [r3+r2+#4], [r4+r2+#4]
  cmp r2, RSIZW
  add r2, #1
  jbe $xor_slice
  ret

_keccak_round:
  call $theta
  call $rho_pi
  call $chi
  call $iota
  ret

theta:
  ; C[x] = ROW_STATE[x,0] ⊕ OW_STATE[x,1] ⊕ ROW_STATE[x,2] ⊕ ROW_STATE[x,3] ⊕ ROW_STATE[x,4], ∀ x in 0...4
  call $parity
  ; D[x] = C[x - 1] ⊕ ROT(C[x + 1], 1),  ∀ x in 0...4
  ; ROW_STATE[x,y] = ROW_STATE[x,y] ⊕ D[x],   ∀ (x, y) in (0...4, 0...4)
  mov r4, #0 ; i
  call $theta_assignment
  ret

;; heres a haiku that describes this function
;; 32 bit word here
;; standard calls for 64 bit
;; xor them seperately
parity:
  mov r0, #0
  ; xor the lower 32 bits
  mov r1, r0
  add r1, ROW_STATE
  mov r2, INT_BC

  mov [r2+r0], [r1]
  xor [r2+r0], [r1+#8]
  xor [r2+r0], [r1+#16]
  xor [r2+r0], [r1+#24]
  xor [r2+r0], [r1+#32]

  ; now xor the higher 32 bits
  mov [r2+r0+#4], [r1+#4]
  xor [r2+r0+#4], [r1+#12]
  xor [r2+r0+#4], [r1+#20]
  xor [r2+r0+#4], [r1+#28]

  ; loop
  add r0, #1
  cmp r0, #4
  ja $parity
  ret

theta_assignment:
  push r6
  mov r6, r7
  sub r7, #80
  mov r1, ROW_STATE
  mov r2, INT_BC
  mov r5, #0 ; j
  ; here we produce
  ; D[x] = C[x - 1] ⊕ ROT(C[x + 1], 1),  ∀ x in 0...4
  push [r1+#4]
  push #5
  call $_mod
  mov r3, r0 ; store our first INT_BC index in r3
  push [r1+#1]
  push #5
  call $_mod ; r0 now contains C[x + 1]
  push [r0+#4]
  push [r0]
  push #1
  xor r0, r3
  ; r0 is now D[x]
  call $inner_theta_loop
  add r1, #1
  cmp r1, #4
  ja $theta_assignment
  mov     r7, r6  ; clear frame
  pop     r6
  ret

inner_theta_loop:
  ; ROW_STATE[x,y] = ROW_STATE[x,y] ⊕ D[x],   ∀ (x, y)
  mov r2, r4 ; INT_BC is no longer needed
  add r2, r5 ; i + j or x + y in keccak spec nomenclature
  mov r2, [r1+r2]
  xor [r2], [r0]
  xor [r2+#4], [r0+#4] ; the final value to write back into the ROW_STATE
  mov [r1], [r2]
  mov [r1+4], [r2]
  add r5, #1
  cmp r5, #4
  jbe $inner_theta_loop
  ret

;; INT_BC[y; 2x + 3y] = ROT(ROW_STATE[x; y]; r[x; y]), 8(x; y) in (0 : : : 4; 0 : : : 4)
rho_pi:
  push r6     ; save frame pointer
  mov r6, r7  ; create new frame
  sub  r7, #4; allocation some variable space

  mov r1, #0
  mov r5, INT_BC
  mov r4, [r5+#8] ; 2nd item (dbl word precision)
  call $inner_pi
  mov     r7, r6
  pop     r6
  ret

inner_pi:
  push r6     ; save frame pointer
  mov r6, r7  ; create new frame
  sub  r7, #32; allocation some variable space

  mov r0, INT_BC
  mov [r0], #0x00000000
  ; iterate over the triangular numbers 0..24 by the
  ; specification defined rotational constants
  mov r0, TRIANGLR_NUMS ; address of beginning of our list of triangular numbers
  mov r2, [r0+r1]
  mov r0, INT_BC
  mov [r0], [r5]
  mov [r0+#4], [r5+r2]
  mov r4, [r5+r2]

  ; now begin to rotate our row state
  push #0x00000000
  push r4
  mov r0, ROT_OFFSETS
  mov r3, [r0+r1]
  push r3
  push r0
  call $rotate
  mov [r5+r2], r3
  add r2, #4
  mov [r5+r2], r0
  pop r0
  mov r0, INT_BC
  mov r4, [r0]
  mov [r4+#4], [r0+#4]
  add r1, #1
  cmp r1, #24
  jz $inner_pi
  mov     r7, r6
  pop     r6
  ret

;; a[i][j][k] ⊕ = ¬a[i][j+1][k] & a[i][j+2][k].
chi:
  pop r0 ; address of row state
  pop r1 ; bitwise combination pointer
  ; iterate over all our rows
  mov r2, #0
  mov r4, ROW_STATE
  mov r5, INT_BC
outer_chi_loop:
  mov r3, #0
row_assignment:
  mov [r5+r3], [r4+ r3 + r2]
  add r3, #1
  cmp r3, #5
  ja $row_assignment

  mov r3, #0
bitwise_combine_along_rows:
  ; st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
  cmp r3, #5
  ja $bitwise_combine_along_rows

  add r2, #5
  cmp r2, #25
  ja $outer_chi_loop
  ret

;;  a[0,0] = a[0,0] xor RC
iota:
  push r6     ; save frame pointer
  mov r6, r7  ; create new frame
  sub r7, #8
  pop r0 ; contains a pointer to the first value of our state
  pop r1 ; containts our round
  mov r2, #4
  mul r2, r1
  xor [r0], r2
  xor [r0+#4], [r2+#4] ; unlimited references, wuw.
  mov r6, r7
  pop r6
  ret
