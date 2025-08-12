#include "ncc.h"

Type *int_type() {
    Type *type = calloc(1, sizeof(Type));
    if (!type) {
        error("メモリの確保に失敗しました");
    }
    type->kind = TY_INT;
    type->size = SIZE_INT;  // intは4バイト
    return type;
}

Type *pointer_to(Type *base) {
    if (!base) {
        error("ポインタのベース型がNULLです");
    }
    Type *type = calloc(1, sizeof(Type));
    if (!type) {
        error("メモリの確保に失敗しました");
    }
    type->kind = TY_PTR;
    type->ptr_to = base;
    type->size = SIZE_PTR;  // ポインタは8バイト
    return type;
}

Type *array_of(Type *base, int len) {
    if (!base) {
        error("配列のベース型がNULLです");
    }
    Type *type = calloc(1, sizeof(Type));
    type->kind = TY_ARRAY;
    type->array_of = base;
    type->array_len = len;
    type->size = base->size * len;
    return type;
}

int size_of(Type *type) {
    if (!type) {
        error("型がNULLです");
    }
    return type->size;
}

Type *get_type_from_node(Node *node) {
    if (!node) {
        return NULL;
    }

    switch (node->kind) {
        case ND_NUM:
            return int_type();
        case ND_LVAR:
            return node->type;
        case ND_DEREF:
            if (node->lhs && node->lhs->type &&
                node->lhs->type->kind == TY_PTR) {
                return node->lhs->type->ptr_to;
            }
            break;
        case ND_ADDR:
            if (node->lhs && node->lhs->type) {
                return pointer_to(node->lhs->type);
            }
            break;
        case ND_INDEX:
            if (node->lhs && node->lhs->type) {
                if (node->lhs->type->kind == TY_ARRAY) {
                    return node->lhs->type->array_of;
                } else if (node->lhs->type->kind == TY_PTR) {
                    return node->lhs->type->ptr_to;
                }
            }
            break;
        case ND_ADD:
        case ND_SUB:
        case ND_MUL:
        case ND_DIV:
        case ND_EQ:
        case ND_NE:
        case ND_LT:
        case ND_LE:
            return int_type();
        case ND_GLOBAL_VAR:
            if (node->var_name) {
                GVar *gvar = find_gvar(node->var_name, strlen(node->var_name));
                if (gvar) {
                    return gvar->type;
                }
            }
            break;
        default:
            break;
    }
    return int_type();
}

Type *parse_type() {
    if (!consume("int")) {
        error_at(token->str, "型が必要です");
    }
    Type *type = int_type();

    while (consume("*")) {
        type = pointer_to(type);
    }

    return type;
}
