#include "ncc.h"

char *user_input;
Token *token;

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char *op) {
    if (!token || token->kind != TK_RESERVED || strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

Token *consume_ident() {
    if (!token || token->kind != TK_IDENT) return NULL;
    Token *tok = token;
    token = token->next;
    return tok;
}

void expect(char *op) {
    if (!token || token->kind != TK_RESERVED || strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        error_at(token ? token->str : user_input, "'%s'ではありません", op);
    }
    token = token->next;
}

int expect_number() {
    if (!token || token->kind != TK_NUM)
        error_at(token ? token->str : user_input, "数ではありません");
    int value = token->val;
    token = token->next;
    return value;
}

bool at_eof() { return !token || token->kind == TK_EOF; }

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    tok->len = len;
    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (!strncmp(p, "return", 6) && !isalnum(p[6]) && p[6] != '_') {
            cur = new_token(TK_RESERVED, cur, p, 6);
            p += 6;
            continue;
        }

        if (!strncmp(p, "==", 2) || !strncmp(p, "!=", 2) ||
            !strncmp(p, "<=", 2) || !strncmp(p, ">=", 2)) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            char *start = p;
            while (('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z') ||
                   ('0' <= *p && *p <= '9') || *p == '_') {
                p++;
            }
            cur = new_token(TK_IDENT, cur, start, p - start);
            continue;
        }

        if (strchr("+-*/()<>;=", *p)) {
            cur = new_token(TK_RESERVED, cur, p, 1);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            char *q = p;
            cur = new_token(TK_NUM, cur, p, 0);
            cur->val = strtol(p, &q, 10);
            cur->len = q - p;
            p = q;
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}
