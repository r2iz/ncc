#include "ncc.h"

static int label_count = 0;
static bool in_function = false;

static const char *arg_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

static void gen_control_flow(Node *node);

static void emit_prologue(int stack_size) {
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    if (stack_size > 0) {
        printf("  sub rsp, %d\n", stack_size);
    }
}

static void emit_epilogue() {
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}

static void emit_function_args(int argc) {
    for (int i = 0; i < argc; i++) {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", 8 * (i + 1));
        printf("  mov [rax], %s\n", arg_regs[i]);
    }
}

static void emit_function_call(char *func_name, int argc) {
    for (int i = 0; i < argc; i++) {
        printf("  pop %s\n", arg_regs[i]);
    }
    printf("  call %s\n", func_name);
    printf("  push rax\n");
}

static void emit_binary_op(NodeKind kind) {
    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (kind) {
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
            error("サポートされていない二項演算子です");
    }

    printf("  push rax\n");
}

void gen_addr(Node *node) {
    if (node->kind == ND_LVAR) {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", node->offset);
        printf("  push rax\n");
        return;
    }
    if (node->kind == ND_DEREF) {
        gen(node->lhs);
        return;
    }

    error("変数ではありません");
}

void gen(Node *node) {
    if (!node) return;

    switch (node->kind) {
        case ND_FUNCDEF: {
            printf(".global %s\n", node->func_name);
            printf("%s:\n", node->func_name);
            emit_prologue(node->offset);
            emit_function_args(node->paramc);

            bool prev_in_function = in_function;
            in_function = true;

            for (Node *n = node->func_body; n; n = n->next) {
                gen(n);
            }

            in_function = prev_in_function;
            emit_epilogue();
            return;
        }
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
            if (in_function) {
                emit_epilogue();
            } else {
                printf("  jmp .Lreturn\n");
            }
            return;
        case ND_ADDR:
            gen_addr(node->lhs);
            return;
        case ND_DEREF:
            gen(node->lhs);
            printf("  pop rax\n");
            printf("  mov rax, [rax]\n");
            printf("  push rax\n");
            return;
        case ND_VAR_DECL:
            return;
        case ND_EXPR_STMT:
            gen(node->lhs);
            printf("  pop rax\n");
            return;
        case ND_BLOCK:
            for (Node *n = node->body; n; n = n->next) {
                gen(n);
            }
            return;
        case ND_FUNCALL:
            for (int i = node->argc - 1; i >= 0; i--) {
                gen(node->args[i]);
            }
            emit_function_call(node->func_name, node->argc);
            return;
        case ND_IF:
        case ND_WHILE:
        case ND_FOR:
            gen_control_flow(node);
            return;
        default:
            break;
    }

    gen(node->lhs);
    gen(node->rhs);
    emit_binary_op(node->kind);
}

static void gen_control_flow(Node *node) {
    switch (node->kind) {
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
            break;
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
            break;
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
            break;
        }
        default:
            error("何かがおかしいです");
    }
}

void codegen(Node *node) {
    printf(".intel_syntax noprefix\n");

    Node *main_statements = NULL;

    for (Node *n = node; n; n = n->next) {
        if (n->kind == ND_FUNCDEF) {
            gen(n);
        } else {
            if (!main_statements) {
                main_statements = n;
            }
        }
    }

    if (main_statements) {
        printf(".global main\n");
        printf("main:\n");

        emit_prologue(get_current_offset());

        for (Node *n = main_statements; n; n = n->next) {
            if (n->kind != ND_FUNCDEF) {
                gen(n);
            }
        }

        printf(".Lreturn:\n");
        emit_epilogue();
    }
}
