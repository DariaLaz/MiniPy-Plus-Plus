# MiniPy++

MiniPy-Plus-Plus is a small Python-like interpreter written in C++.

## Interpreter Pipeline

## REPL

## Lexer

### Tokens

## Parser

The parser receives tokens and builds the Abstract Syntax Tree

### Expression Grammar

list -> '[' (expression (',' expression)*)? ']'
primary -> NUMBER | STRING | IDENTIFIER | TRUE | FALSE | list | '(' expression ')'
postfix -> primary ("[" expression "]")\* | "(" (expression ("," expression)\*)? ")"
unary -> ("+" | "-") unary | postfix
factor -> unary (("\*" | "/") unary)\*
term -> factor (('+' | '-') factor)\*
comparison -> term ((">" | ">=" | "<" | "<=") term)\*
equality -> comparison (("==" | "!=") comparison)\*
expression -> equality
if_statement -> "if" expression ":" block
("elif" expression ":" block)\*
("else" ":" block)?
while_statement → "while" expression ":" block
simple_statement -> IDENTIFIER "=" expression | expression | IDENTIFIER "[" expression "]" "=" expression

print_statement -> "print" "(" expression ")"
for_statement → "for" IDENTIFIER "in" expression ":" block
parameters -> IDENTIFIER ("," IDENTIFIER)\*
function_statement -> "def" IDENTIFIER "(" parameters? ")" ":" block
return_statement -> "return" expression? NEWLINE
statement -> if_statement | while_statement | print_statement | simple_statement NEWLINE | for_statement | function_statement | return_statement
block -> NEWLINE INDENT statement+ DEDENT
