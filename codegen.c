#include "ncc.h"

static int label_count = 0;
static bool in_function = false;
static int str_label_count = 0;

static const char *arg_regs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};

static void gen_control_flow(Node *node);

static void emit_prologue(int stack_size) {
    printf("# emit_prologue: stack_size=%d\n", stack_size);
    int aligned_size = (stack_size + 15) & ~15;
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    if (aligned_size > 0) {
        printf("  sub rsp, %d\n", aligned_size);
    }
}

static void emit_epilogue() {
    printf("# emit_epilogue\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}

static void emit_function_args(int argc) {
    printf("# emit_function_args: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("# emit_function_args: %s\n", arg_regs[i]);
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", 8 * (i + 1));
        printf("  mov [rax], %s\n", arg_regs[i]);
    }
}

static void emit_function_call(char *func_name, int argc) {
    printf("# emit_function_call");
    for (int i = 0; i < argc; i++) {
        printf("# emit_function_call: %s, %d\n", func_name, argc);
        printf("  pop %s\n", arg_regs[i]);
    }
    printf("# emit_function_call: return address\n");
    printf("  mov rax, 0\n");
    printf("  call %s\n", func_name);
    printf("  push rax\n");
}

static void emit_binary_op(NodeKind kind) {
    printf("# emit_binary_op\n");
    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (kind) {
        case ND_ADD:
            printf("# ND_ADD\n");
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("# ND_SUB\n");
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("# ND_MUL\n");
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("# ND_DIV\n");
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        case ND_PERCENT:
            printf("# ND_PERCENT\n");
            printf("  cqo\n");
            printf("  idiv rdi\n");
            printf("  mov rax, rdx\n");
            break;
        case ND_EQ:
            printf("# ND_EQ\n");
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_NE:
            printf("# ND_NE\n");
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LT:
            printf("# ND_LT\n");
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LE:
            printf("# ND_LE\n");
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
    printf("# gen_addr: node kind=%d\n", node->kind);
    if (node->kind == ND_LVAR) {
        printf("# ND_LVAR\n");
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", node->offset);
        printf("  push rax\n");
        return;
    }
    if (node->kind == ND_STR) {
        printf("# ND_STR\n");
        if (!node->str_label) {
            char buf[32];
            snprintf(buf, sizeof(buf), ".L.str.%d", str_label_count++);
            node->str_label = strndup_safe(buf, strlen(buf));

            printf(".data\n");
            printf("%s:\n", node->str_label);
            if (node->str_len > 0) {
                printf("  .byte ");
                for (int i = 0; i < node->str_len; i++) {
                    if (i) printf(", ");
                    printf("%d", (unsigned char)node->str_lit[i]);
                }
                printf(", 0\n");
            } else {
                printf("  .byte 0\n");
            }
            printf(".text\n");
        }
        printf("  lea rax, %s[rip]\n", node->str_label);
        printf("  push rax\n");
        return;
    }
    if (node->kind == ND_DEREF) {
        printf("# ND_DEREF\n");
        gen(node->lhs);
        return;
    }
    if (node->kind == ND_INDEX) {
        printf("# ND_INDEX\n");
        // base address
        printf("# ND_INDEX: gen_addr lhs\n");
        gen_addr(node->lhs);
        // calculate index
        printf("# ND_INDEX: gen_addr rhs\n");
        gen(node->rhs);
        printf("  pop rdi\n");
        printf("  pop rax\n");
        Type *base_type = node->lhs->type;
        Type *elem_type = (base_type->kind == TY_ARRAY) ? base_type->array_of
                                                        : base_type->ptr_to;

        int elem_size = size_of(elem_type);

        // calculate index * elem_size
        printf("# ND_INDEX: calculating address\n");
        printf("  imul rdi, %d\n", elem_size);
        printf("  add rax, rdi\n");
        printf("  push rax\n");
        return;
    }
    if (node->kind == ND_GLOBAL_VAR) {
        printf("# ND_GLOBAL_VAR\n");
        printf("  lea rax, %s[rip]\n", node->var_name);
        printf("  push rax\n");
        return;
    }

    error("変数ではありません");
}

void gen(Node *node) {
    if (!node) return;

    switch (node->kind) {
        case ND_FUNCDEF: {
            printf("# ND_FUNCDEF: %s\n", node->func_name);
            printf(".text\n");
            printf(".global %s\n", node->func_name);
            printf("%s:\n", node->func_name);
            emit_prologue(node->offset);
            emit_function_args(node->paramc);

            bool prev_in_function = in_function;
            in_function = true;

            for (Node *n = node->func_body; n; n = n->next) {
                printf("# ND_FUNCDEF: gen body\n");
                gen(n);
            }

            in_function = prev_in_function;

            emit_epilogue();
            return;
        }
        case ND_NUM:
            printf("# ND_NUM: %d\n", node->val);
            printf("  push %d\n", node->val);
            return;
        case ND_STR: {
            printf("# ND_STR: %s\n", node->str_lit);
            if (!node->str_label) {
                printf("# ND_STR: generating label\n");
                char buf[32];
                snprintf(buf, sizeof(buf), ".L.str.%d", str_label_count++);
                node->str_label = strndup_safe(buf, strlen(buf));

                printf(".data\n");
                printf("%s:\n", node->str_label);
                if (node->str_len > 0) {
                    printf("  .byte ");
                    for (int i = 0; i < node->str_len; i++) {
                        if (i) printf(", ");
                        printf("%d", (unsigned char)node->str_lit[i]);
                    }
                    printf(", 0\n");
                } else {
                    printf("  .byte 0\n");
                }
                printf(".text\n");
            }
            printf("  lea rax, %s[rip]\n", node->str_label);
            printf("  push rax\n");
            return;
        }
        case ND_LVAR:
            printf("# ND_LVAR: %s\n", node->var_name);
            if (node->type->kind == TY_ARRAY) {
                printf("# ND_LVAR: array\n");
                gen_addr(node);
            } else {
                printf("# ND_LVAR: scalar\n");
                gen_addr(node);
                printf("  pop rax\n");
                if (node->type->kind == TY_CHAR) {
                    printf("# ND_LVAR: char\n");
                    printf("  movsx rax, byte ptr [rax]\n");
                } else if (node->type->kind == TY_INT) {
                    printf("# ND_LVAR: int\n");
                    printf("  movsxd rax, dword ptr [rax]\n");
                } else {
                    printf("# ND_LVAR: pointer\n");
                    printf("  mov rax, [rax]\n");
                }
                printf("  push rax\n");
            }
            return;
        case ND_GLOBAL_VAR:
            printf("# ND_GLOBAL_VAR: %s\n", node->var_name);
            gen_addr(node);
            printf("  pop rax\n");
            if (node->type && node->type->kind == TY_CHAR) {
                printf("# ND_GLOBAL_VAR: char\n");
                printf("  movsx rax, byte ptr [rax]\n");
            } else if (node->type && node->type->kind == TY_INT) {
                printf("# ND_GLOBAL_VAR: int\n");
                printf("  movsxd rax, dword ptr [rax]\n");
            } else {
                printf("# ND_GLOBAL_VAR: pointer\n");
                printf("  mov rax, [rax]\n");
            }
            printf("  push rax\n");
            return;
        case ND_INDEX:
            printf("# ND_INDEX\n");
            gen_addr(node);
            printf("  pop rax\n");
            {
                Type *type = get_type_from_node(node);
                if (type && type->kind == TY_CHAR) {
                    printf("# ND_INDEX: char\n");
                    printf("  movsx rax, byte ptr [rax]\n");
                } else if (type && type->kind == TY_INT) {
                    printf("# ND_INDEX: int\n");
                    printf("  movsxd rax, dword ptr [rax]\n");
                } else {
                    printf("# ND_INDEX: pointer\n");
                    printf("  mov rax, [rax]\n");
                }
            }
            printf("  push rax\n");
            return;
        case ND_COMMA:
            printf("# ND_COMMA\n");
            gen(node->lhs);
            printf("  pop rax\n");
            gen(node->rhs);
            return;
        case ND_ASSIGN:
            printf("# ND_ASSIGN: %s\n", node->lhs->var_name);
            gen_addr(node->lhs);
            gen(node->rhs);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            if (node->lhs->kind == ND_LVAR) {
                if (node->lhs->type && node->lhs->type->kind == TY_CHAR) {
                    printf("# ND_ASSIGN: lvar char\n");
                    printf("  mov [rax], dil\n");
                } else if (node->lhs->type && node->lhs->type->kind == TY_INT) {
                    printf("# ND_ASSIGN: lvar int\n");
                    printf("  mov dword ptr [rax], edi\n");
                } else {
                    printf("# ND_ASSIGN: lvar pointer\n");
                    printf("  mov [rax], rdi\n");
                }
            } else if (node->lhs->kind == ND_GLOBAL_VAR) {
                if (node->lhs->type && node->lhs->type->kind == TY_CHAR) {
                    printf("# ND_ASSIGN: global char\n");
                    printf("  mov [rax], dil\n");
                } else if (node->lhs->type && node->lhs->type->kind == TY_INT) {
                    printf("# ND_ASSIGN: global int\n");
                    printf("  mov dword ptr [rax], edi\n");
                } else {
                    printf("# ND_ASSIGN: global pointer\n");
                    printf("  mov [rax], rdi\n");
                }
            } else {
                Type *type = get_type_from_node(node->lhs);
                if (type && type->kind == TY_CHAR) {
                    printf("# ND_ASSIGN: deref char\n");
                    printf("  mov [rax], dil\n");
                } else if (type && type->kind == TY_INT) {
                    printf("# ND_ASSIGN: deref int\n");
                    printf("  mov dword ptr [rax], edi\n");
                } else {
                    printf("# ND_ASSIGN: deref pointer\n");
                    printf("  mov [rax], rdi\n");
                }
            }
            printf("  push rdi\n");
            return;
        case ND_RETURN:
            printf("# ND_RETURN\n");
            if (node->lhs) {
                printf("# ND_RETURN: gen lhs\n");
                gen(node->lhs);
                printf("  pop rax\n");
            }
            if (in_function) {
                printf("# ND_RETURN: in function\n");
                emit_epilogue();
            } else {
                printf("# ND_RETURN: not in function\n");
                printf("  jmp .Lreturn\n");
            }
            return;
        case ND_ADDR:
            printf("# ND_ADDR\n");
            gen_addr(node->lhs);
            return;
        case ND_DEREF:
            printf("# ND_DEREF\n");
            gen(node->lhs);
            printf("  pop rax\n");
            {
                Type *type = get_type_from_node(node);
                if (type && type->kind == TY_CHAR) {
                    printf("# ND_DEREF: char\n");
                    printf("  movsx rax, byte ptr [rax]\n");
                } else if (type && type->kind == TY_INT) {
                    printf("# ND_DEREF: int\n");
                    printf("  movsxd rax, dword ptr [rax]\n");
                } else {
                    printf("# ND_DEREF: pointer\n");
                    printf("  mov rax, [rax]\n");
                }
            }
            printf("  push rax\n");
            return;
        case ND_VAR_DECL:
            return;
        case ND_SIZEOF: {
            printf("# ND_SIZEOF\n");
            Type *type;
            if (node->type) {
                type = node->type;
            } else {
                type = get_type_from_node(node->lhs);
            }
            int size = size_of(type);
            printf("  push %d\n", size);
            return;
        }
        case ND_EXPR_STMT:
            printf("# ND_EXPR_STMT\n");
            gen(node->lhs);
            printf("  pop rax\n");
            return;
        case ND_BLOCK:
            printf("# ND_BLOCK\n");
            for (Node *n = node->body; n; n = n->next) {
                gen(n);
            }
            return;
        case ND_FUNCALL:
            printf("# ND_FUNCALL: %s\n", node->func_name);
            for (int i = node->argc - 1; i >= 0; i--) {
                gen(node->args[i]);
            }
            emit_function_call(node->func_name, node->argc);
            return;
        case ND_IF:
        case ND_WHILE:
        case ND_FOR:
            printf("# ND_CONTROL_FLOW: %d\n", node->kind);
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
            printf("# ND_IF\n");
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
            printf("# ND_WHILE\n");
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
            printf("# ND_FOR\n");
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
                printf("  pop rax\n");
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

    for (GVar *gvar = globals; gvar; gvar = gvar->next) {
        printf(".data\n");
        printf("%.*s:\n", gvar->len, gvar->name);
        printf("  .zero %d\n", size_of(gvar->type));
    }

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
        printf(".text\n");
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