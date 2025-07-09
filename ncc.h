#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

extern char *user_input;
extern Token *token;

void error(char *fmt, ...);
void error_at(char *cur, char *fmt, ...);

bool consume(char op);
void expect(char op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str);
Token *tokenize(char *p);