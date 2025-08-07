#include "ncc.h"

// ローカル変数のリスト
static LVar *locals = NULL;
static int current_offset = 0;

LVar *create_lvar(char *name, int len, Type *type) {
    if (!name || len <= 0 || !type) {
        error("無効な変数情報です");
    }

    LVar *lvar = calloc(1, sizeof(LVar));

    lvar->name = name;
    lvar->len = len;
    lvar->type = type;
    current_offset += size_of(type);
    lvar->offset = current_offset;
    lvar->next = locals;
    locals = lvar;

    return lvar;
}

LVar *find_lvar(char *name, int len) {
    if (!name || len <= 0) {
        return NULL;
    }

    for (LVar *var = locals; var; var = var->next) {
        if (var->len == len && !strncmp(var->name, name, len)) {
            return var;
        }
    }
    return NULL;
}

void clear_locals() {
    locals = NULL;
    current_offset = 0;
}

int get_current_offset() { return current_offset; }

void set_current_offset(int offset) { current_offset = offset; }
