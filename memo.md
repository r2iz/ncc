program        = global_def* ;

global_def     = function_def | global_var_decl ;

function_def   = type ident "(" [ params ] ")" "{" stmt* "}" ;
params         = param { "," param } ;
param          = type ident ;

global_var_decl = type ident ( "[" num "]" )? ";" ;

stmt           = variable_decl
               | "return" expr ";"
               | "if" "(" expr ")" stmt ( "else" stmt )?
               | "while" "(" expr ")" stmt
               | "for" "(" expr? ";" expr? ";" expr? ")" stmt
               | "{" stmt* "}"
               | expr ";" ;

variable_decl  = type ident ( "[" num "]" )? ( "=" assign )? ";" ;

expr           = assign ;
assign         = equality ( "=" assign )? ;
equality       = relational ( ( "==" | "!=" ) relational )* ;
relational     = add ( ( "<" | "<=" | ">" | ">=" ) add )* ;
add            = mul ( ( "+" | "-" ) mul )* ;
mul            = unary ( ( "*" | "/" ) unary )* ;
unary          = ( "+" | "-" | "&" | "*" ) unary
               | "sizeof" unary
               | "sizeof" "(" type ")"
               | primary ;

primary        = num
               | char_literal
               | string_literal ( "[" expr "]" )*
               | ident ( func_args | ( "[" expr "]" )* )?
               | "(" expr ")" ;

func_args      = "(" [ expr ( "," expr )* ] ")" ;

type           = ( "char" | "int" ) "*"* ;

ident          = identifier ;
num            = number_literal ;
char_literal   = character_literal ;
string_literal = string_literal ;