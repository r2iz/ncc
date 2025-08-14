.intel_syntax noprefix
.data
palette:
  .zero 13
.text
.global m
m:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov rax, rbp
  sub rax, 8
  mov [rax], rdi
  mov rax, rbp
  sub rax, 16
  mov [rax], rsi
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
# emit_binary_op
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  push 5000
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
  push 10000
# emit_binary_op
  pop rdi
  pop rax
  cqo
  idiv rdi
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  mov rsp, rbp
  pop rbp
  ret
.text
.global a
a:
  push rbp
  mov rbp, rsp
  sub rsp, 64
  mov rax, rbp
  sub rax, 8
  mov [rax], rdi
  mov rax, rbp
  sub rax, 16
  mov [rax], rsi
  mov rax, rbp
  sub rax, 24
  mov [rax], rdx
  mov rax, rbp
  sub rax, 32
  mov [rax], rcx
  mov rax, rbp
  sub rax, 56
  push rax
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  movsxd rax, dword ptr [rax]
  push rax
  mov rax, rbp
  sub rax, 24
  push rax
  pop rax
  mov rax, [rax]
  push rax
# emit_binary_op
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  push 5000
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
  push 10000
# emit_binary_op
  pop rdi
  pop rax
  cqo
  idiv rdi
  push rax
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  movsxd rax, dword ptr [rax]
  push rax
  mov rax, rbp
  sub rax, 32
  push rax
  pop rax
  mov rax, [rax]
  push rax
# emit_binary_op
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  push 5000
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
  push 10000
# emit_binary_op
  pop rdi
  pop rax
  cqo
  idiv rdi
  push rax
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 40
  push rax
  mov rax, rbp
  sub rax, 24
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  movsxd rax, dword ptr [rax]
  push rax
# emit_function_call: m, 2
  pop rdi
# emit_function_call: m, 2
  pop rsi
  mov rax, 0
  call m
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 48
  push rax
  mov rax, rbp
  sub rax, 32
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  movsxd rax, dword ptr [rax]
  push rax
# emit_function_call: m, 2
  pop rdi
# emit_function_call: m, 2
  pop rsi
  mov rax, 0
  call m
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 56
  push rax
  mov rax, rbp
  sub rax, 40
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 48
  push rax
  pop rax
  mov rax, [rax]
  push rax
# emit_binary_op
  pop rdi
  pop rax
  sub rax, rdi
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 64
  push rax
  mov rax, rbp
  sub rax, 24
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  movsxd rax, dword ptr [rax]
  push rax
# emit_function_call: m, 2
  pop rdi
# emit_function_call: m, 2
  pop rsi
  mov rax, 0
  call m
  push rax
  mov rax, rbp
  sub rax, 32
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rax, [rax]
  push rax
  pop rax
  movsxd rax, dword ptr [rax]
  push rax
# emit_function_call: m, 2
  pop rdi
# emit_function_call: m, 2
  pop rsi
  mov rax, 0
  call m
  push rax
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 56
  push rax
  pop rax
  mov rax, [rax]
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov dword ptr [rax], edi
  push rdi
  mov rax, rbp
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
  mov rax, rbp
  sub rax, 64
  push rax
  pop rax
  mov rax, [rax]
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov dword ptr [rax], edi
  push rdi
  mov rsp, rbp
  pop rbp
  ret
.text
.global main
main:
  push rbp
  mov rbp, rsp
  sub rsp, 8896
  lea rax, palette[rip]
  push rax
  push 0
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 46
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 1
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 44
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 2
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 45
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 3
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 126
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 4
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 58
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 5
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 59
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 6
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 61
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 7
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 33
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 8
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 42
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 9
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 35
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 10
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 36
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 11
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 64
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
  lea rax, palette[rip]
  push rax
  push 12
# ND_INDEX
  pop rdi
  pop rax
  imul rdi, 1
  add rax, rdi
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], dil
  push rdi
.data
.L.str.0:
  .byte 0, 51, 51, 91, 50, 74, 0
.text
  lea rax, .L.str.0[rip]
  push rax
# emit_function_call: printf, 1
  pop rdi
  mov rax, 0
  call printf
  push rax
  mov rax, rbp
  sub rax, 8808
  push rax
  push 10000
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8816
  push rax
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8824
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8832
  push rax
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8840
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
.data
.L.str.1:
  .byte 98, 101, 102, 111, 114, 101, 32, 102, 111, 114, 10, 0
.text
  lea rax, .L.str.1[rip]
  push rax
# emit_function_call: printf, 1
  pop rdi
  mov rax, 0
  call printf
  push rax
.Lbegin0:
.data
.L.str.2:
  .byte 97, 102, 116, 101, 114, 32, 102, 111, 114, 10, 0
.text
  lea rax, .L.str.2[rip]
  push rax
# emit_function_call: printf, 1
  pop rdi
  mov rax, 0
  call printf
  push rax
  push 1760
  push 32
  mov rax, rbp
  sub rax, 8800
  push rax
# emit_function_call: memset, 3
  pop rdi
# emit_function_call: memset, 3
  pop rsi
# emit_function_call: memset, 3
  pop rdx
  mov rax, 0
  call memset
  push rax
  push 1760
  push 4
# emit_binary_op
  pop rdi
  pop rax
  imul rax, rdi
  push rax
  push 0
  mov rax, rbp
  sub rax, 7040
  push rax
# emit_function_call: memset, 3
  pop rdi
# emit_function_call: memset, 3
  pop rsi
# emit_function_call: memset, 3
  pop rdx
  mov rax, 0
  call memset
  push rax
  mov rax, rbp
  sub rax, 8848
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8856
  push rax
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8864
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
.Lbegin2:
  mov rax, rbp
  sub rax, 8864
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 88
# emit_binary_op
  pop rdi
  pop rax
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  pop rax
  cmp rax, 0
  je .Lend3
  mov rax, rbp
  sub rax, 8872
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8880
  push rax
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 8888
  push rax
  push 0
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
.Lbegin4:
  mov rax, rbp
  sub rax, 8888
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 314
# emit_binary_op
  pop rdi
  pop rax
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  pop rax
  cmp rax, 0
  je .Lend5
  mov rax, rbp
  sub rax, 8888
  push rax
  mov rax, rbp
  sub rax, 8888
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 1
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  pop rax
  push 200
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 2
# emit_binary_op
  pop rdi
  pop rax
  sub rax, rdi
  push rax
  mov rax, rbp
  sub rax, 8872
  push rax
  mov rax, rbp
  sub rax, 8880
  push rax
# emit_function_call: a, 4
  pop rdi
# emit_function_call: a, 4
  pop rsi
# emit_function_call: a, 4
  pop rdx
# emit_function_call: a, 4
  pop rcx
  mov rax, 0
  call a
  push rax
  jmp .Lbegin4
.Lend5:
  mov rax, rbp
  sub rax, 8864
  push rax
  mov rax, rbp
  sub rax, 8864
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 1
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  push 714
  push 9974
  mov rax, rbp
  sub rax, 8864
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 2
# emit_binary_op
  pop rdi
  pop rax
  cqo
  idiv rdi
  mov rax, rdx
  push rax
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
  mov rax, rbp
  sub rax, 8848
  push rax
  mov rax, rbp
  sub rax, 8856
  push rax
# emit_function_call: a, 4
  pop rdi
# emit_function_call: a, 4
  pop rsi
# emit_function_call: a, 4
  pop rdx
# emit_function_call: a, 4
  pop rcx
  mov rax, 0
  call a
  push rax
  jmp .Lbegin2
.Lend3:
  push 400
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 8
# emit_binary_op
  pop rdi
  pop rax
  sub rax, rdi
  push rax
  mov rax, rbp
  sub rax, 8824
  push rax
  mov rax, rbp
  sub rax, 8816
  push rax
# emit_function_call: a, 4
  pop rdi
# emit_function_call: a, 4
  pop rsi
# emit_function_call: a, 4
  pop rdx
# emit_function_call: a, 4
  pop rcx
  mov rax, 0
  call a
  push rax
  push 200
  mov rax, rbp
  sub rax, 8808
  push rax
  pop rax
  mov rax, [rax]
  push rax
  push 2
# emit_binary_op
  pop rdi
  pop rax
  sub rax, rdi
  push rax
  mov rax, rbp
  sub rax, 8840
  push rax
  mov rax, rbp
  sub rax, 8832
  push rax
# emit_function_call: a, 4
  pop rdi
# emit_function_call: a, 4
  pop rsi
# emit_function_call: a, 4
  pop rdx
# emit_function_call: a, 4
  pop rcx
  mov rax, 0
  call a
  push rax
  jmp .Lbegin0
.Lend1:
  push 0
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  mov rsp, rbp
  pop rbp
  ret
