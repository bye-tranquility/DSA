  .text
  .global _longest_inc_subseq_in_asm

; x0 ~ int64_t* array
; x1 ~ uint64_t* help_array
; x2 ~ uint64_t size

_longest_inc_subseq_in_asm:
  mov x3, #1  ; x3 ~ 1
  mov x4, #0  ; x4 ~ i

init_loop:  
  cmp x4, x2  ; i < size?
  bge init_done  ; i >= size, ending loop
  str x3, [x1, x4, lsl 3]  ; help_array[i] = 1
  add x4, x4, 1  ; i++
  b init_loop

init_done:
  sub x4, x2, 1  ; i = size - 1

outer_loop:
  cmp x4, -1  ; i <= -1?
  ble find_max  ; i < 0, moving on to finding max

  mov x5, x4
  add x5, x5, 1  ; j = i + 1

inner_loop:
  cmp x5, x2  ; j < size?
  bge end_inner_loop  ; j >= size, ending loop

  ldr x6, [x0, x4, lsl 3]  ; x6 ~ array[i]
  ldr x7, [x0, x5, lsl 3]  ; x7 ~ array[j]
  cmp x6, x7
  bge next_j_iter  ; array[i] >= array[j], moving on to next j

  ldr x8, [x1, x4, lsl 3]  ; x8 ~ help_array[i]
  ldr x9, [x1, x5, lsl 3]  ; x9 ~ help_array[j]
  add x9, x9, 1   ; x9 = help_array[j] + 1
  cmp x8, x9
  bge next_j_iter  ; help_array[i] >= help_array[j] + 1
  mov x8, x9
  str x8, [x1, x4, lsl #3]

next_j_iter:
  add x5, x5, 1  ; ++j
  b inner_loop

end_inner_loop:
  sub x4, x4, 1  ; --i
  b outer_loop

find_max:
  mov x4, 0  ; i = 0
  mov x10, 0  ; max_length = 0

max_loop:
  cmp x4, x2  ; i < size?
  bge end_max_loop

  ldr x8, [x1, x4, lsl 3]
  cmp x8, x10  ; help_array[i] <= max_length?
  ble skip_update_max

update_max:
  mov x10, x8  ; max_length = help_array[i]

skip_update_max:
  add x4, x4, 1  ; ++i
  b max_loop

end_max_loop:
  mov x0, x10  
  ret
