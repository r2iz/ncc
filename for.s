.intel_syntax noprefix
.text
.global main
main:
  push rbp
  mov rbp, rsp
.Lbegin0:
.data
.L.str.0:
  .byte 97, 10, 0
.text
  lea rax, .L.str.0[rip]
  push rax
# emit_function_call: printf, 1
  pop rdi
  mov rax, 0
  call printf
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
