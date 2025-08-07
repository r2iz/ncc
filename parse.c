#include "ncc.h"

typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

static LVar *locals;
int current_offset;

static LVar *find_lvar(char *name, int len) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == len && !strncmp(var->name, name, len)) {
            return var;
        }
    }
    return NULL;
}

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

Node *new_num(int val) {
    Node *node = new_node(ND_NUM, NULL, NULL);
    node->val = val;
    return node;
}

Node *program() {
    locals = NULL;
    current_offset = 0;
    Node head;
    head.next = NULL;
    Node *cur = &head;

    while (!at_eof()) {
        // lookahead
        Token *saved_token = token;
        Token *tok = consume_ident();
        if (tok && consume("(")) {
            // function definition
            Node *func = calloc(1, sizeof(Node));
            func->kind = ND_FUNCDEF;
            func->func_name = malloc(tok->len + 1);
            memcpy(func->func_name, tok->str, tok->len);
            func->func_name[tok->len] = '\0';
            func->paramc = 0;

            if (!consume(")")) {
                do {
                    if (func->paramc >= 6) {
                        error_at(tok->str, "引数は最大6つまでです");
                    }
                    if (!consume("int")) {
                        error_at(token->str, "引数の型が必要です");
                    }
                    Token *param_tok = consume_ident();
                    if (!param_tok) {
                        error_at(token->str, "引数名が必要です");
                    }

                    LVar *lvar = calloc(1, sizeof(LVar));
                    lvar->name = param_tok->str;
                    lvar->len = param_tok->len;
                    current_offset += 8;
                    lvar->offset = current_offset;
                    lvar->next = locals;
                    locals = lvar;

                    func->paramc++;
                } while (consume(","));
                expect(")");
            }

            expect("{");
            // 関数定義開始時のcurrent_offsetを保存
            int func_offset = current_offset;

            Node head2;
            head2.next = NULL;
            Node *cur2 = &head2;

            while (!consume("}")) {
                cur2->next = stmt();
                cur2 = cur2->next;
            }
            func->func_body = head2.next;

            // 関数のローカル変数領域サイズを保存
            func->offset = current_offset;

            cur->next = func;
            cur = cur->next;

            locals = NULL;
            current_offset = 0;
            continue;
        } else {
            token = saved_token;
        }

        cur->next = stmt();
        cur = cur->next;
    }
    return head.next;
}

Node *stmt() {
    if (consume("int")) {
        Token *tok = consume_ident();
        if (!tok) error_at(token->str, "変数名が必要です");

        LVar *lvar = calloc(1, sizeof(LVar));
        lvar->name = tok->str;
        lvar->len = tok->len;
        current_offset += 8;
        lvar->offset = current_offset;
        lvar->next = locals;
        locals = lvar;

        expect(";");
        return new_node(ND_VAR_DECL, NULL, NULL);
    }

    if (consume("return")) {
        Node *node = new_unary(ND_RETURN, expr());
        expect(";");
        return node;
    }

    if (consume("{")) {
        Node *node = new_node(ND_BLOCK, NULL, NULL);
        Node head;
        head.next = NULL;
        Node *cur = &head;

        while (!consume("}")) {
            cur->next = stmt();
            cur = cur->next;
        }
        node->body = head.next;
        return node;
    }

    if (consume("if")) {
        Node *node = new_node(ND_IF, NULL, NULL);
        expect("(");
        node->cond = expr();
        expect(")");
        node->then = stmt();
        if (consume("else")) {
            node->els = stmt();
        }
        return node;
    }

    if (consume("while")) {
        Node *node = new_node(ND_WHILE, NULL, NULL);
        expect("(");
        node->cond = expr();
        expect(")");
        node->then = stmt();
        return node;
    }

    if (consume("for")) {
        Node *node = new_node(ND_FOR, NULL, NULL);
        expect("(");
        if (!consume(";")) {
            node->init = expr();
            expect(";");
        }
        if (!consume(";")) {
            node->cond = expr();
            expect(";");
        }
        if (!consume(")")) {
            node->inc = expr();
            expect(")");
        }
        node->then = stmt();
        return node;
    }

    Node *node = new_unary(ND_EXPR_STMT, expr());
    expect(";");
    return node;
}

Node *expr() { return assign(); }

Node *assign() {
    Node *node = equality();

    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = new_node(ND_NE, node, relational());
        } else {
            return node;
        }
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<")) {
            node = new_node(ND_LT, node, add());
        } else if (consume("<=")) {
            node = new_node(ND_LE, node, add());
        } else if (consume(">")) {
            node = new_node(ND_LT, add(), node);
        } else if (consume(">=")) {
            node = new_node(ND_LE, add(), node);
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume("-")) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume("+")) {
        return unary();
    }
    if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), unary());
    }
    if (consume("&")) {
        return new_unary(ND_ADDR, unary());
    }
    if (consume("*")) {
        return new_unary(ND_DEREF, unary());
    }
    return primary();
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        // function call
        if (consume("(")) {
            Node *node = new_node(ND_FUNCALL, NULL, NULL);
            node->func_name = malloc(tok->len + 1);
            memcpy(node->func_name, tok->str, tok->len);
            node->func_name[tok->len] = '\0';
            node->argc = 0;
            if (!consume(")")) {
                do {
                    if (node->argc >= 6)
                        error_at(tok->str, "引数は最大6つまでです");
                    node->args[node->argc++] = expr();
                } while (consume(","));
                expect(")");
            }
            return node;
        }

        // variable
        LVar *lvar = find_lvar(tok->str, tok->len);
        if (!lvar) {
            error_at(tok->str, "宣言されていない変数です");
        }
        Node *node = new_node(ND_LVAR, NULL, NULL);
        node->offset = lvar->offset;
        return node;
    }

    return new_node_num(expect_number());
}
