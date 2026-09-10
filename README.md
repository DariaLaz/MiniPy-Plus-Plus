# MiniPy++

MiniPy-Plus-Plus is a small Python-like interpreter written in C++. The project implements a lexer, recursive-descent parser, Abstract Syntax Tree,
semantic analysis, runtime environment, built-in functions, error handling..

## Building

MiniPy++ requires a C++17-compatible compiler.
The project can be built using CMake or the included build.sh.

### CMake

Configure the project: `cmake -S . -B build`
Build: `cmake --build build`

#### Run tests:

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### build.sh

Build running `build.sh`

## Interpreter Pipeline

- The source code is first tokenized by the lexer.
- The parser then converts the token stream into an Abstract Syntax Tree.
- The semantic analyzer performs checks that cannot be handled purely by the grammar.
- Finally, the AST is executed using the runtime environment.

```text
Source Code
    |
    v
  Lexer
    |
    v
  Tokens
    |
    v
  Parser
    |
    v
   AST
    |
    v
Semantic Analyzer
    |
    v
 Runtime
    |
    v
  Output
```

## Lexer

- The lexer converts source code into a sequence of tokens.

Example:

```
x = 10 + 20
```

is converted conceptually into:

```
IDENTIFIER(x)
EQUAL
NUMBER(10)
PLUS
NUMBER(20)
NEWLINE
END
```

### Tokens

## Parser

The parser receives the tokens produced by the lexer and builds the Abstract Syntax Tree.

MiniPy++ uses a recursive-descent parser.

### Grammar

#### Expression Grammar

dictionary ->
"{"
(
expression ":" expression
("," expression ":" expression)\*
)?
"}"

list ->
"["
(expression ("," expression)*)?
"]"

primary ->
NUMBER
| STRING
| IDENTIFIER
| TRUE
| FALSE
| NONE
| list
| dictionary
| "(" expression ")"

postfix ->
primary
(
"[" expression "]"
| "[" expression? ":" expression? "]"
| "(" (expression ("," expression)_)? ")"
)_

unary ->
("+" | "-") unary
| postfix

factor ->
unary (("_" | "/") unary)_

term ->
factor (("+" | "-") factor)\*

comparison ->
term ((">" | ">=" | "<" | "<=") term)\*

equality ->
comparison (("==" | "!=") comparison)\*

not_expression ->
"not" not_expression
| equality

and_expression ->
not_expression ("and" not_expression)\*

or_expression ->
and_expression ("or" and_expression)\*

expression ->
or_expression

#### Statement Grammar

assignment ->
IDENTIFIER "=" expression

index_assignment ->
IDENTIFIER "[" expression "]" "=" expression

if_statement ->
"if" expression ":" block
("elif" expression ":" block)\*
("else" ":" block)?

while_statement ->
"while" expression ":" block

for_statement ->
"for" IDENTIFIER "in" expression ":" block

parameters ->
IDENTIFIER ("," IDENTIFIER)\*

function_statement ->
"def" IDENTIFIER "(" parameters? ")" ":" block

return_statement ->
"return" expression? NEWLINE

break_statement ->
"break" NEWLINE

continue_statement ->
"continue" NEWLINE

simple_statement ->
assignment
| index_assignment
| expression

statement ->
if_statement
| while_statement
| for_statement
| function_statement
| return_statement
| break_statement
| continue_statement
| simple_statement NEWLINE

block ->
NEWLINE INDENT statement+ DEDENT

## Semantic Analysis

- Some errors are syntactically valid but semantically invalid:
  - `break` or `continue` outside a loop
  - `return` outside of a function
  - duplicate parameter names in functions

## Runtime

- Runtime values are represented using std::variant
- The runtime currently supports values corresponding to:

```
int
bool
string
list
dictionary
function
built-in function
None
```

### Environment

Variables are stored inside an Environment.
Environments can have parent environments => allows to support lexical scoping and closures.

## Supported constructions

### Operations

#### Arithmetic

Supported arithmetic operators:

```
+
-
*
/
```

#### Comparison

Supported comparison operators:

```
==
!=
<
<=
>
>=
```

#### Logical Operators

Supported logical operators:

```
and
or
not
```

### Lists

- Lists can contain runtime values.
- Supports
  - Indexing and Negative Indexing
    ```
    numbers = [10, 20, 30]
    ```

print(numbers[0])
print(numbers[-1])

```

- Indexed Assignment
```

numbers = [10, 20, 30]

numbers[1] = 100

```

- Slicing
```

numbers = [1, 2, 3, 4, 5]

print(numbers[1:4])
print(numbers[:3])
print(numbers[2:])

```

### Strings

- Supports
  - Concatenation
    `name = "Mini" + "Py"
   `
  - Indexing
    `text = "hello"
print(text[0])
   `
  - Slicing
    `text = "hello"
print(text[1:4])
   `

### Dictionaries

- Supports dictionaries with string keys

```

person = {
"name": "Alice",
"age": 25
}

person["age"] = 26
person["city"] = "Sofia"
print(person["name"])

```

### Control Flow

#### `if / elif / else`

```

x = 10

if x > 10:
print("greater")
elif x == 10:
print("equal")
else:
print("smaller")

```

#### `while` loop

```

i = 0

while i < 5:
print(i)
i = i + 1

```

#### `for` loop

```

for i in range(5):
print(i)

```

#### `break`

#### `continue`

### Functions

- Functions are declared using `def`.
- If a function finishes without an explicit `return`, it returns `None`.
- Recursive functions are supported.
- Closures: functions remember the environment in which they were created.
- Several built-in functions:

```

print
len
range
append
str
int

```

## Error Handling

- Provides Python-like error classes.
- Currently supported errors include:

```

SyntaxError
IndentationError
TypeError
NameError
IndexError
KeyError
ValueError
ZeroDivisionError

```

- Errors contain source locations.
  Example:

```

print(missing)

```

may produce:

```

NameError at 1:7: Undefined variable: missing

```

```
