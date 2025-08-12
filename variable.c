#include "ncc.h"

// global variable list
GVar *globals = NULL;

GVar *create_gvar(char *name, int len, Type *type) {
    GVar *gvar = calloc(1, sizeof(GVar));
    gvar->name = name;
    gvar->len = len;
    gvar->type = type;
    gvar->next = globals;
    globals = gvar;

    return gvar;
}

GVar *find_gvar(char *name, int len) {
    for (GVar *gvar = globals; gvar; gvar = gvar->next) {
        if (gvar->len == len && !strncmp(gvar->name, name, len)) {
            return gvar;
        }
    }
    return NULL;
}

// local variable list
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

    // 8バイト境界でアライメント
    int type_size = size_of(type);
    int aligned_size = (type_size + 7) & ~7;
    current_offset += aligned_size;
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
