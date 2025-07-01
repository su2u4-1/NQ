# instruction set A
`store {register} {address}`: store register value at address  
`load {register} {address}`: load value from address into register  
`{operator} {register1} {register2} {register3}`: perform operation on register1 and register2, store result in register3  
`{operator} {register1} {register2}`: perform operation on register1, store result in register2  
`jump {address} if {register}`: jump to the address if register is TRUE (not zero)  
`input {value} {register}`: store value into register  
> note: unconditional jump equal `jump {address} if $one`
# instruction set B
`pop {register}`: pop value from stack into register  
`push {register}`: push value from register onto stack  
`{operator} {register1} {register2} {register3}`: perform operation on register1 and register2, store result in register3  
`{operator} {register1} {register2}`: perform operation on register1, store result in register2  
`{operator}`: pop some values​ ​(usually one or two) from stack, perform operation, and push result onto stack  
`jump {label} if {register}`: jump to label if register is TRUE (not zero)  
`jump {label} if not {register}`: jump to label if register is FALSE (zero)  
`jump {label}`: jump to label unconditionally  
`label {label}`: define a label for jumping  
`input {value}`: push value into stack  
`call {function} {n}`: call a function, n is the number of arguments  
`return`: return from a function, jump to return address  
`function {name} {n}`: define a function with name and n arguments  
# registers
> note: `$hs`, `$he`, `$cs`, `$ce`, `$ds`, `$de`, `$hs`, `$he`, `$ss`, `$se`, `$zero`, `$one`, `$mem` and `$s0` ~ `$s53` are not actually registers but memory maps  
> note: `$mem` accesses the memory address specified by `$addr`  
## read-only registers
`$zero`: always zero, used for comparisons  
`$one`: always one, used for constant values  
`$cs`: code section start address  
`$ce`: code section end address  
`$ds`: data section start address  
`$de`: data section end address  
`$hs`: heap section start address  
`$he`: heap section end address  
`$ss`: stack section start address  
`$se`: stack section end address  
`$mem`: memory register, used for memory operations  
## writeable registers
`$addr`: address register, used for addressing  
`$hn`: heap next, points to the next available address in the heap  
`$sp`: stack pointer, points to the top of the stack, used for implementation `push` and `pop` instructions  
`$fp`: frame pointer, points to the current function's frame  
`$pc`: program counter, points to the current instruction, used for implementation `jump` instruction  
`$a` ~ `$z`: general-purpose registers  
`$s0` ~ `$s53`: saved registers, used for saving values across function calls
# operators
## two operands
### arithmetic
`add`: add two values  
`sub`: subtract second value from first  
`mul`: multiply two values  
`div`: divide first value by second  
`mod`: modulo operation  
### comparison
`eq`: check if two values are equal  
`ne`: check if two values are not equal  
`lt`: check if first value is less than second  
`gt`: check if first value is greater than second  
`le`: check if first value is less than or equal to second  
`ge`: check if first value is greater than or equal to second  
### logical
`and`: logical AND operation  
`or`: logical OR operation  
`xor`: logical XOR operation  
### shift
`shr`: arithmetic right shift  
`lrs`: logical right shift  
`shl`: shift left  
## one operand
`neg`: negate value  
`not`: logical negation  
# translate B to A
> TODO
# some design decisions
- the stack is downwards growing
- heap is upwards growing, `$hs` pointer points to the start of the heap, `$he` points to the now end of the heap
- objects are stored in the heap, and the heap is managed by the VM
- the stack is used for function calls, local variables, and temporary values
- the frame pointer `$fp` points to the current function's frame, which is used for accessing local variables and function arguments
- arguments must be pushed before calling a function
- if need alloc memory or free object, use `push{size}\ncall sys.alloc 1` and `push{pointer}\ncall sys.free 1` respectively  
## memory design
|address|value|pointer|describe|
|-------|-----|-------|--------|
|0x0|0| <- `$zero`|always zero, used for comparisons|
|0x4|1| <- `$one`|always one, used for constant values|
|0x8|0x100 (memory start)| <- `$cs`|code section start address|
|0xc|`$ds` - 1| <- `$ce`|code section end address|
|0x10|`$ce` + 1| <- `$ds`|data section start address|
|0x14|`$hs` - 1| <- `$de`|data section end address|
|0x18|`$de` + 1| <- `$hs`|heap start address|
|0x1c|`$se` - 1| <- `$he`|heap end address|
|0x20|`$he` + 1| <- `$se`|stack end address|
|0x24|0xffffffff (memory max)| <- `$ss`|stack start address|
|0x28|`$s0`| -> some address|saved register s0|
|0x2c|`$s1`| -> some address|saved register s1|
......
|0xfc|`$s53`| -> some address|saved register s53|
## stack design
|stack|
|-----|
|frame0 (system)|
|frame1 (main)|
|frame2 (...)|
......
### frame structure
> note: n is the number of arguments, m is the number of local variables
|describe|pointer|
|--------|-------|
|arg0| <- ($fp - n)|
|arg1| <- ($fp - n + 1)|
......
|arg(n-1)| <- ($fp - 1)|
|return address| <- $fp|
|previous frame| <- ($fp + 1)|
|local variable 0| <- ($fp + 2)|
|local variable 1| <- ($fp + 3)|
......
|local variable (m-1)| <- ($fp + m + 1)|
|stack top| <- $sp|
## heap design
|object|pointer|
|------|-------|
|object0| <- $hs|
|object1| <- ($hs + object0 size)|
|object2| <- ($hs + object0 size + object1 size)|
......
### object structure
> note: take object0 as an example
|describe|pointer|
|--------|-------|
|size| <- $hs|
|pointer to previous object| <- ($hs + 1)|
|attribute count| <- ($hs + 2)|
|pointer to attribute0| <- ($hs + 3)|
|pointer to attribute1| <- ($hs + 4)|
......
|pointer to attribute(n-1)| <- ($hs + 3 + attribute count)|