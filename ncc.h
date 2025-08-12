#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Type
typedef enum {
    TY_CHAR,   // char
    TY_INT,    // int
    TY_PTR,    // pointer
    TY_ARRAY,  // array
} TypeKind;

#define SIZE_CHAR 1
#define SIZE_INT 4
#define SIZE_PTR 8

typedef struct Type Type;

struct Type {
    TypeKind kind;
    Type *ptr_to;
    Type *array_of;
    int array_len;
    int size;
};

Type *char_type();
Type *int_type();
Type *pointer_to(Type *base);
Type *array_of(Type *base, int len);
int size_of(Type *type);
Type *parse_type();

// Global variable management
typedef struct GVar GVar;

struct GVar {
    char *name;
    int len;
    Type *type;
    GVar *next;
};

GVar *create_gvar(char *name, int len, Type *type);
GVar *find_gvar(char *name, int len);
extern GVar *globals;

// Local variable management
typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
    Type *type;
};

LVar *create_lvar(char *name, int len, Type *type);
LVar *find_lvar(char *name, int len);
void clear_locals();
int get_current_offset();
void set_current_offset(int offset);

// tokenize

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_CHAR,  // character literal
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
Token *consume_char();
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
Token *tokenize(char *p);

// parse
typedef enum {
    ND_ADD,         // +
    ND_SUB,         // -
    ND_MUL,         // *
    ND_DIV,         // /
    ND_EQ,          // ==
    ND_NE,          // !=
    ND_LT,          // <
    ND_LE,          // <=
    ND_ASSIGN,      // =
    ND_RETURN,      // return
    ND_IF,          // if
    ND_WHILE,       // while
    ND_FOR,         // for
    ND_BLOCK,       // { ... }
    ND_FUNCALL,     // function call
    ND_FUNCDEF,     // function definition
    ND_EXPR_STMT,   // expression statement
    ND_LVAR,        // Local variable
    ND_GLOBAL_VAR,  // Global variable
    ND_ADDR,        // &
    ND_DEREF,       // *
    ND_INDEX,       // array[index]
    ND_VAR_DECL,    // variable declaration
    ND_SIZEOF,      // sizeof
    ND_NUM,
} NodeKind;

typedef struct Node Node;

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

    char *var_name;

    char *func_name;
    Node *args[6];
    int argc;

    int paramc;
    Node *func_body;

    Type *type;
    int val;
    int offset;
};

// Node utility functions
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_unary(NodeKind kind, Node *expr);
Node *new_num(int val);
Node *new_lvar_node(LVar *lvar);
char *strndup_safe(char *src, int len);

Type *get_type_from_node(Node *node);

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
