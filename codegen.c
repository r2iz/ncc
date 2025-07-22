#include "ncc.h"

static int label_count = 0;

void gen_addr(Node *node) {
    if (node->kind == ND_LVAR) {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", node->offset);
        printf("  push rax\n");
        return;
    }

    error("変数ではありません");
}

void gen(Node *node) {
    switch (node->kind) {
        case ND_NUM:
            printf("  push %d\n", node->val);
            return;
        case ND_LVAR:
            gen_addr(node);
            printf("  pop rax\n");
            printf("  mov rax, [rax]\n");
            printf("  push rax\n");
            return;
        case ND_ASSIGN:
            gen_addr(node->lhs);
            gen(node->rhs);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  mov [rax], rdi\n");
            printf("  push rdi\n");
            return;
        case ND_RETURN:
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  jmp .Lreturn\n");
            return;
        case ND_EXPR_STMT:
            gen(node->lhs);
            printf("  pop rax\n");
            return;
        case ND_BLOCK: {
            for (Node *n = node->body; n; n = n->next) {
                gen(n);
            }
            return;
        }
        case ND_FUNCALL: {
            printf("  call %s\n", node->func_name);
            printf("  push rax\n");
            return;
        }
        case ND_IF: {
            int l_else = label_count++;
            int l_end = label_count++;
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lelse%d\n", l_else);
            if (node->then) {
                gen(node->then);
            }
            printf("  jmp .Lend%d\n", l_end);
            printf(".Lelse%d:\n", l_else);
            if (node->els) {
                gen(node->els);
            }
            printf(".Lend%d:\n", l_end);
        }
        case ND_WHILE: {
            int l_begin = label_count++;
            int l_end = label_count++;
            printf(".Lbegin%d:\n", l_begin);
            gen(node->cond);
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lend%d\n", l_end);
            if (node->then) {
                gen(node->then);
            }
            printf("  jmp .Lbegin%d\n", l_begin);
            printf(".Lend%d:\n", l_end);
            return;
        }
        case ND_FOR: {
            int l_begin = label_count++;
            int l_end = label_count++;
            if (node->init) {
                gen(node->init);
            }
            printf(".Lbegin%d:\n", l_begin);
            if (node->cond) {
                gen(node->cond);
                printf("  pop rax\n");
                printf("  cmp rax, 0\n");
                printf("  je .Lend%d\n", l_end);
            }
            if (node->then) {
                gen(node->then);
            }
            if (node->inc) {
                gen(node->inc);
            }
            printf("  jmp .Lbegin%d\n", l_begin);
            printf(".Lend%d:\n", l_end);
            return;
        }
        default:
            break;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        case ND_EQ:
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_NE:
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LT:
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LE:
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzb rax, al\n");
            break;
        default:
            error("コードを生成できません");
    }

    printf("  push rax\n");
}

void codegen(Node *node) {
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // prologue
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", current_offset);

    for (Node *n = node; n; n = n->next) {
        gen(n);
    }

    // epilogue
    printf(".Lreturn:\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");

    printf("  ret\n");
}
