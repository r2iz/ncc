#include "ncc.h"

static Node *parse_function_definition(Token *name_token) {
    Type *ret_type = parse_type();
    Token *tok = consume_ident();
    if (!tok) {
        error_at(token->str, "関数名が必要です");
    }
    expect("(");

    Node *func = calloc(1, sizeof(Node));
    func->kind = ND_FUNCDEF;
    func->func_name = strndup_safe(tok->str, tok->len);
    func->paramc = 0;

    if (!consume(")")) {
        do {
            if (func->paramc >= 6) {
                error_at(tok->str, "引数は最大6つまでです");
            }
            Type *param_type = parse_type();
            Token *param_tok = consume_ident();
            if (!param_tok) {
                error_at(token->str, "引数名が必要です");
            }

            create_lvar(param_tok->str, param_tok->len, param_type);
            func->paramc++;
        } while (consume(","));
        expect(")");
    }

    expect("{");

    Node head;
    head.next = NULL;
    Node *cur = &head;

    while (!consume("}")) {
        cur->next = stmt();
        cur = cur->next;
    }
    func->func_body = head.next;
    func->offset = get_current_offset();

    return func;
}

static Node *parse_function_call(Token *name_token) {
    Node *node = new_node(ND_FUNCALL, NULL, NULL);
    node->func_name = strndup_safe(name_token->str, name_token->len);
    node->argc = 0;

    if (!consume(")")) {
        do {
            if (node->argc >= 6) {
                error_at(name_token->str, "引数は最大6つまでです");
            }
            node->args[node->argc++] = expr();
        } while (consume(","));
        expect(")");
    }
    return node;
}

static Node *parse_global_variable_declaration() {
    Type *type = parse_type();
    Token *tok = consume_ident();

    if (consume("[")) {
        int array_len = expect_number();
        expect("]");
        type = array_of(type, array_len);
    }
    create_gvar(tok->str, tok->len, type);

    expect(";");

    Node *node = new_node(ND_VAR_DECL, NULL, NULL);
    node->type = type;
    node->var_name = strndup_safe(tok->str, tok->len);
    return node;
}

static Node *parse_variable_declaration() {
    Type *type = parse_type();
    Token *tok = consume_ident();
    if (!tok) {
        error_at(token->str, "変数名が必要です");
    }

    if (consume("[")) {
        int array_len = expect_number();
        expect("]");
        type = array_of(type, array_len);
    }

    LVar *lvar = create_lvar(tok->str, tok->len, type);
    Node *decl = new_node(ND_VAR_DECL, NULL, NULL);
    decl->type = type;
    decl->var_name = strndup_safe(tok->str, tok->len);
    decl->offset = lvar->offset;

    // =があったら代入もくっつける
    if (consume("=")) {
        Node *lhs = new_lvar_node(lvar);
        Node *rhs = assign();
        Node *assign_node = new_node(ND_ASSIGN, lhs, rhs);
        expect(";");
        Node *block = new_node(ND_BLOCK, NULL, NULL);
        Node *head = calloc(1, sizeof(Node));
        head->next = decl;
        decl->next = assign_node;
        assign_node->next = NULL;
        block->body = head->next;
        return block;
    } else {
        expect(";");
        return decl;
    }
}

Node *program() {
    clear_locals();
    Node head;
    head.next = NULL;
    Node *cur = &head;

    while (!at_eof()) {
        if (token && token->kind == TK_RESERVED &&
            ((token->len == 3 && !strncmp(token->str, "int", 3)) ||
             (token->len == 4 && !strncmp(token->str, "char", 4)))) {
            Token *saved = token;
            token = token->next;
            if (token && token->kind == TK_IDENT && token->next &&
                token->next->str[0] != '(') {
                token = saved;
                parse_global_variable_declaration();
                continue;
            } else {
                token = saved;
                cur->next = parse_function_definition(saved);
                cur = cur->next;
                continue;
            }
            token = saved;
        }
        cur->next = stmt();
        cur = cur->next;
    }
    return head.next;
}

Node *stmt() {
    if (token && token->kind == TK_RESERVED &&
        ((token->len == 3 && !strncmp(token->str, "int", 3)) ||
         (token->len == 4 && !strncmp(token->str, "char", 4)))) {
        return parse_variable_declaration();
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

    Node *node = expr();
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
        } else if (consume("%")) {
            node = new_node(ND_PERCENT, node, unary());
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
    if (consume("sizeof")) {
        if (consume("(")) {
            if (token && token->kind == TK_RESERVED &&
                (strncmp(token->str, "char", 4) == 0 ||
                 strncmp(token->str, "int", 3) == 0)) {
                Type *type = parse_type();
                expect(")");
                Node *node = new_node(ND_SIZEOF, NULL, NULL);
                node->type = type;
                return node;
            } else {
                Node *expr_node = expr();
                expect(")");
                Node *node = new_unary(ND_SIZEOF, expr_node);
                return node;
            }
        } else {
            Node *node = new_unary(ND_SIZEOF, unary());
            return node;
        }
    }
    return primary();
}

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        while (consume("[")) {
            Node *index = expr();
            expect("]");
            Node *array_access = new_node(ND_INDEX, node, index);
            array_access->type = get_type_from_node(array_access);
            node = array_access;
        }
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        Node *node = NULL;

        // function call
        if (consume("(")) {
            node = parse_function_call(tok);
        } else {
            // variable
            LVar *lvar = find_lvar(tok->str, tok->len);
            if (!lvar) {
                GVar *gvar = find_gvar(tok->str, tok->len);
                if (!gvar) {
                    error_at(tok->str, "宣言されていない変数です");
                }
                // global variable
                node = new_node(ND_GLOBAL_VAR, NULL, NULL);
                node->var_name = strndup_safe(tok->str, tok->len);
                node->type = gvar->type;
            } else {
                node = new_lvar_node(lvar);
            }
        }

        while (consume("[")) {
            Node *index = expr();
            expect("]");
            Node *array_access = new_node(ND_INDEX, node, index);
            array_access->type = get_type_from_node(array_access);
            node = array_access;
        }

        return node;
    }

    Token *char_tok = consume_char();
    if (char_tok) {
        Node *node = new_node_num(char_tok->val);
        node->type = char_type();
        return node;
    }

    Token *str_tok = consume_string();
    if (str_tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_STR;
        node->str_lit = str_tok->string;
        node->str_len = str_tok->string_len;
        node->type = array_of(char_type(), node->str_len + 1);
        while (consume("[")) {
            Node *index = expr();
            expect("]");
            Node *array_access = new_node(ND_INDEX, node, index);
            array_access->type = get_type_from_node(array_access);
            node = array_access;
        }
        return node;
    }

    return new_node_num(expect_number());
}
