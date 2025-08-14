.intel_syntax noprefix
.text
.global f
f:
  push rbp
  mov rbp, rsp
  sub rsp, 8
  mov rax, rbp
  sub rax, 8
  mov [rax], rdi
  mov rsp, rbp
  pop rbp
  ret
.text
.global g
g:
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
  push 999
# ND_ASSIGN
  pop rdi
  pop rax
  mov [rax], rdi
  push rdi
  mov rax, rbp
  sub rax, 16
  push rax
  push 888
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
  sub rax, 16
  push rax
  pop rax
  mov rax, [rax]
  push rax
# emit_binary_op
  pop rdi
  pop rax
  add rax, rdi
  push rax
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  mov rsp, rbp
  pop rbp
  ret
.text
.global main
main:
  push rbp
  mov rbp, rsp
  push 0
  pop rax
  mov rsp, rbp
  pop rbp
  ret
  mov rsp, rbp
  pop rbp
  ret
