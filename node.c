#include "ncc.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *new_unary(NodeKind kind, Node *expr) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = expr;
    return node;
}

Node *new_num(int val) { return new_node_num(val); }

Node *new_lvar_node(LVar *lvar) {
    Node *node = new_node(ND_LVAR, NULL, NULL);
    node->type = lvar->type;
    node->offset = lvar->offset;
    return node;
}

char *strndup_safe(char *src, int len) {
    if (!src || len <= 0) {
        error("無効な文字列です");
    }
    char *dst = malloc(len + 1);
    memcpy(dst, src, len);
    dst[len] = '\0';
    return dst;
}
