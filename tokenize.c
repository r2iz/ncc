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

Token *consume_char() {
    if (!token || token->kind != TK_CHAR) return NULL;
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

static bool is_alnum(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') || c == '_';
}

static bool is_keyword(char *p, int *len) {
    static char *keywords[] = {"if",     "else", "while", "for",
                               "return", "char", "int",   "sizeof"};
    static int keyword_count = sizeof(keywords) / sizeof(*keywords);

    for (int i = 0; i < keyword_count; i++) {
        int kw_len = strlen(keywords[i]);
        if (!strncmp(p, keywords[i], kw_len) && !is_alnum(p[kw_len])) {
            *len = kw_len;
            return true;
        }
    }
    return false;
}
static bool is_two_char_op(char *p, int *len) {
    if (!strncmp(p, "==", 2) || !strncmp(p, "!=", 2) || !strncmp(p, "<=", 2) ||
        !strncmp(p, ">=", 2)) {
        *len = 2;
        return true;
    }
    return false;
}

static bool is_single_char_op(char c) {
    return strchr("+-*/()<>;={},&[]", c) != NULL;
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

        int len;
        if (is_keyword(p, &len)) {
            cur = new_token(TK_RESERVED, cur, p, len);
            p += len;
            continue;
        }

        if (is_two_char_op(p, &len)) {
            cur = new_token(TK_RESERVED, cur, p, len);
            p += len;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            char *start = p;
            while (is_alnum(*p)) {
                p++;
            }
            cur = new_token(TK_IDENT, cur, start, p - start);
            continue;
        }

        if (is_single_char_op(*p)) {
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

        if (*p == '\'') {
            char *start = p;
            p++;  // skip opening quote
            int val;
            if (*p == '\\') {
                p++;  // skip backslash
                switch (*p) {
                    case 'n':
                        val = '\n';
                        break;
                    case 't':
                        val = '\t';
                        break;
                    case 'r':
                        val = '\r';
                        break;
                    case '\\':
                        val = '\\';
                        break;
                    case '\'':
                        val = '\'';
                        break;
                    case '0':
                        val = '\0';
                        break;
                    default:
                        error_at(p, "未対応のエスケープシーケンスです");
                }
                p++;
            } else {
                val = *p;
                p++;
            }
            if (*p != '\'') {
                error_at(p, "文字リテラルが閉じられていません");
            }
            p++;  // skip closing quote
            cur = new_token(TK_CHAR, cur, start, p - start);
            cur->val = val;
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}
