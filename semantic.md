# NQ Language - Formal Grammar & Semantic Notes

## Overview

NQ is a statically typed, C-style programming language designed with simplicity, consistency, and extensibility in mind. It supports generics, classes, methods, operator overloading, and labeled loops. However, it intentionally omits advanced features like inheritance, interfaces, type coercion, chained assignment, and exception handling.

This document contains the formal grammar (BNF-style) and additional semantic annotations for implementation clarity.

---

## Grammar Definition

### Top-Level Structure
```bnf
code ::= [{<import> | <declare_var'global'> | <function> | <class> | <single_line_comment> | <multi_line_comment>}]
```

###  Declarations
```bnf
declare_var ::= 'var' <type> <identifier'var_name'> ['=' <expression>] [{',' <identifier'var_name'> ['=' <expression>]}] ';'
declare_attr ::= ('attr' | 'static') ['public'] <type> <identifier'var_name'> ['=' <expression>] [{',' <identifier'var_name'> ['=' <expression>]}] ';'
declare_args ::= [<type> <identifier'var_name'> [{',' <type> <identifier'var_name'>}]] [[','] <type> <identifier'var_name'> '*']
declare_generic ::= '<' <identifier'type_name'> [{',' <identifier'type_name'>}] '>'
type ::= ['const'] (<builtin_type> | <identifier'class_name'>) [<use_generic>]
```

###  Expressions
```bnf
expression ::= <term> {<operator> <term>}
assign_exp ::= variable <assign_op> <expression> ';'
term ::= ('$' | '@' | '-' | '~' | '!') <term> | <call> | <variable> | <value> | '(' <expression> ')'
call ::= <variable> [<use_generic>] '(' [<expression> [{',' <expression>}]] ')'
use_generic ::= '<' <type> [{',' <type>}] '>'
variable ::= <identifier'var_name'> | ('(' <expression> ')' | <call> | <variable>) ['[' <expression> ']' | '.' <identifier'attr_name'>]
```

#### Literals
```bnf
value ::= <int> | <char> | <bool> | <null> | <string> | <float> | <list> | <pointer> | <dict> | <tuple>
pointer ::= <expression> | '$' <expression>
list ::= '[' <expression> [{',' <expression>}] ']'
tuple ::= '(' <expression> ({',' <expression>}, ',') ')'
dict ::= '{' [<expression> ':' <expression> [{',' <expression> ':' <expression>}]] '}'
```

###  Functions and Classes
```bnf
function ::= 'func' ['const'] <type> <identifier'func_name'> [<declare_generic>] '(' [<declare_args>] ')' '{' <statements> '}'
class ::= 'class' <identifier'class_name'> [<declare_generic>] '{' [{<declare_attr> | <method> | <function>}] '}'
method ::= 'method' [('public' | 'op')] <type> <identifier'method_name'> [<declare_generic>] '(' <identifier'self'> [',' <declare_args>] ')' '{' <statements> '}'
```
- `const func` indicates a pure function (no side effects, return value depends only on arguments).
- Function overloading is supported by parameter signature (similar to C++).
- `method` vs `func`: The only difference is the explicit `self` parameter.
- `attr` and `method` are private by default unless marked `public`.

###  Control Flow
```bnf
statements ::= [{<if> | <for> | <while> | <return> | <expression> ';' | <break> | <continue> | <declare_var'local'> | <assign_exp>}]

if ::= 'if' '(' <expression> ')' '{' <statements> '}' [{'elif' '(' <expression> ')' '{' <statements> '}'}] ['else' '{' <statements> '}']

for ::= 'for' [<identifier'label'>] '(' <for_loop> ')' '{' <statements> '}' ['else' '{' <statements> '}']
for_loop ::= <type> <identifier'var_name'> 'in' <container> | <expression> ';' <expression> ';' <expression>
container ::= <list> | <tuple> | <dict> | <str> | <expression>

while ::= 'while' [<identifier'label'>] '(' <expression> ')' '{' <statements> '}' ['else' '{' <statements> '}']

return ::= 'return' [<expression>] ';'
break ::= 'break' [<identifier'label'>] ';'
continue ::= 'continue' [<identifier'label'>] ';'
```

- Loops can be labeled for `break label;` or `continue label;`.
- Labeled names must be unique in the local scope.

###  Import
```bnf
import ::= 'import' (<identifier'stdlib_name'> | <str'userlib_path'> 'as' <identifier'alias_name'>) ';'
```

###  Operators & Precedence
```bnf
operator ::= '*' | '/' | '%' | '+' | '-' | '<<' | '>>' | '<' | '<=' | '>' | '>=' | '==' | '!=' |
             '&' | '^' | '|' | '&&' | '||' | <assign_op> | '!' | '~' | '@' | '$'
assign_op ::= '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '<<=' | '>>=' | '&=' | '^=' | '|='
```

#### Precedence Table (Higher = tighter binding)
| Precedence | Operators                     | Associativity       |
|------------|-------------------------------|---------------------|
| 1          | `*`, `/`, `%`                 | Left-to-right       |
| 2          | `+`, `-`                      | Left-to-right       |
| 3          | `<<`, `>>`                    | Left-to-right       |
| 4          | `<`, `<=`, `>`, `>=`          | Left-to-right       |
| 5          | `==`, `!=`                    | Left-to-right       |
| 6          | `&`, `^`, `|`                 | Left-to-right       |
| 7          | `&&`, `||`                    | Left-to-right       |
| 8          | Assignment Operators          | Right-to-left       |

#### Unary Operators
- `-a`: Negation
- `!a`: Logical NOT
- `~a`: Bitwise NOT
- `@a`: Dereference pointer
- `$a`: Address-of

#### Postfix Operators
- `a()`: Function call
- `a[]`: Subscript
- `a.b`: Member access

###  Syntax Tokens
```bnf
identifier ::= (<atoz> | <AtoZ> | '_') {<atoz> | <AtoZ> | <digit> | '_'}
int ::= ['-'] <digit> [{<digit>}]
char ::= '\'' <unicode_character> '\''
string ::= '"' [{<unicode_character>}] '"'
float ::= ['-'] <digit> [{<digit>}] '.' <digit> [{<digit>}]
bool ::= 'true' | 'false'
null ::= 'NULL'
void ::= 'void'
```

### Keywords
```bnf
keyword ::= <bool> | <void> | <null> | 'if' | 'elif' | 'else'
          | 'for' | 'in' | 'while' | 'return' | 'break' | 'continue' | 'import' | 'const'
          | 'public' | 'func' | 'method' | 'class' | 'var' | 'as' | 'op' | 'attr' | 'static'
```

### Built-in Types
```bnf
builtin_type ::= 'int' | 'char' | 'bool' | 'void' | 'str' | 'float' | 'list' | 'pointer' | 'range' | 'tuple' | 'dict'
```

### Comments
```bnf
single_line_comment ::= ('#' | "//") [{<unicode_character>}] '\n'
multi_line_comment ::= '/*' [{<unicode_character> | '\n'}] '*/'
```

###  Semantic Rules and Notes

- **Pure functions** use `const func` — must have no side effects and return only based on inputs.
- **Method vs Function**: `method` includes explicit `self` and is always bound to a class.
- **Overloading** is supported for functions (by parameter type).
- **No inheritance** or interface support currently.
- **Explicit type casts** are not supported; future support may come via standard functions like `int(x)`.
- **No implicit type conversion** — all types must match exactly.
- **Pointers** support arithmetic but do not scale by type size (e.g., `p + 1` adds 1, not `sizeof(*p)`).
- **`range(start, end)`** behaves like Python but returns a full array.
- **Truthiness**:
  - `0`, `0.0`, `NULL`, and `void` are treated as `false`.
  - All other values (including non-numeric) are treated as `true`.
- **Unsupported Features**:
  - f-strings and string escape sequences
  - `switch`, `try-catch`, and `throw`
  - chained assignment (e.g., `a = b = c`)
  - type aliases (planned but not implemented)
  - type inference
  - statement expressions
- **Standard Library**: Will be designed after core language completion.
