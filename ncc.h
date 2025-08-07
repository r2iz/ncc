#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type
typedef enum {
    TY_INT,  // int
    TY_PTR,  // pointer
} TypeKind;

typedef struct Type Type;

struct Type {
    TypeKind kind;
    Type *ptr_to;  // ポインタが指す型
    int size;      // 型のサイズ
};

Type *int_type();
Type *pointer_to(Type *base);
int size_of(Type *type);

// tokenize

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

extern char *user_input;
extern Token *token;

void error(char *fmt, ...);
void error_at(char *cur, char *fmt, ...);

bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

// parse
typedef enum {
    ND_ADD,        // +
    ND_SUB,        // -
    ND_MUL,        // *
    ND_DIV,        // /
    ND_EQ,         // ==
    ND_NE,         // !=
    ND_LT,         // <
    ND_LE,         // <=
    ND_ASSIGN,     // =
    ND_RETURN,     // return
    ND_IF,         // if
    ND_WHILE,      // while
    ND_FOR,        // for
    ND_BLOCK,      // { ... }
    ND_FUNCALL,    // function call
    ND_FUNCDEF,    // function definition
    ND_EXPR_STMT,  // expression statement
    ND_LVAR,       // Local variable
    ND_ADDR,       // &
    ND_DEREF,      // *
    ND_VAR_DECL,   // variable declaration
    ND_NUM,
} NodeKind;

typedef struct Node Node;

extern int current_offset;

struct Node {
    NodeKind kind;
    Node *next;
    Node *lhs;
    Node *rhs;

    Node *cond;
    Node *then;
    Node *els;
    Node *init;
    Node *inc;

    Node *body;

    char *func_name;
    Node *args[6];
    int argc;
    // 関数定義用
    int paramc;
    Node *func_body;

    Type *type;  // ノードの型情報
    int val;
    int offset;
};

Node *program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

// codegen

void gen(Node *node);
void codegen(Node *node);
