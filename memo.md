```
program      = { function_definition | stmt } ;

function_definition = "int" ident "(" [ function_params ] ")" "{" { stmt } "}" ;
function_params     = type ident { "," type ident } ;

stmt         = variable_declaration
             | "return" expr ";"
             | "{" { stmt } "}"
             | "if" "(" expr ")" stmt [ "else" stmt ]
             | "while" "(" expr ")" stmt
             | "for" "(" [ expr ] ";" [ expr ] ";" [ expr ] ")" stmt
             | expr ";" ;

variable_declaration = type ident [ "[" number "]" ] ";" ;

expr         = assign ;
assign       = equality [ "=" assign ] ;
equality     = relational { ("==" | "!=") relational } ;
relational   = add { ("<" | "<=" | ">" | ">=") add } ;
add          = mul { ("+" | "-") mul } ;
mul          = unary { ("*" | "/") unary } ;
unary        = ( "+" | "-" | "&" | "*" | "sizeof" ) unary | primary ;
primary      = "(" expr ")"
             | ident ( "(" [ function_args ] ")" | "[" expr "]" )?
             | number ;

function_args = expr { "," expr } ;

type         = "int" ;  // 現状intのみ

ident        = 識別子 ;
number       = 数値リテラル ;
```