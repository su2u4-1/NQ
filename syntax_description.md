# operators
The processed of processed by variable is greater than processed by term.
The associativity of precedence 8 is right-to-left.
## basic operators
|Precedence|Operator|Description|
|-|-|-|
|1|a * b|multiplication|
|1|a / b|division|
|1|a % b|remainder|
|2|a + b|addition|
|2|a - b|subtraction|
|3|a << b|bitwise left shift|
|3|a >> b|bitwise right shift|
|4|a < b|less than|
|4|a <= b|less than or equal to|
|4|a > b|greater than|
|4|a >= b|greater than or equal to|
|5|a == b|equality|
|5|a != b|inequality|
|6|a & b|bitwise AND|
|6|a ^ b|bitwise XOR (exclusive or)|
|6|a \| b|bitwise OR (inclusive or)|
|7|a && b|logical AND|
|7|a \|\| b|logical OR|
|8|a = b|direct assignment (provided by default for C++ classes)|
|8|a += b|compound assignment by sum|
|8|a -= b|compound assignment by difference|
|8|a *= b|compound assignment by product|
|8|a /= b|compound assignment by quotient|
|8|a %= b|compound assignment by remainder|
|8|a <<= b|compound assignment by bitwise left shift|
|8|a >>= b|compound assignment by bitwise right shift|
|8|a &= b|compound assignment by bitwise AND|
|8|a ^= b|compound assignment by bitwise XOR|
|8|a \|= b|compound assignment by bitwise OR|
## unary operators
### processed by term
|Operator|Description|
|-|-|
|-a|unary minus|
|!a|logical NOT|
|~a|bitwise NOT|
|@a|indirection (dereference)|
|$a|address-of|
### processed by variable
|Operator|Description|
|-|-|
|a()|function call|
|a[]|subscript|
|a.b|member access|
