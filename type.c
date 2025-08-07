#include "ncc.h"

Type *int_type() {
    Type *type = calloc(1, sizeof(Type));
    type->kind = TY_INT;
    type->size = 8;
    return type;
}

Type *pointer_to(Type *base) {
    if (!base) {
        error("ポインタのベース型がNULLです");
    }
    Type *type = calloc(1, sizeof(Type));
    type->kind = TY_PTR;
    type->ptr_to = base;
    type->size = 8;
    return type;
}

int size_of(Type *type) {
    if (!type) {
        error("型がNULLです");
    }
    return type->size;
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
