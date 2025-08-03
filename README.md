# Very Small C Compiler

I'm building a tiny C compiler from scratch in C++! It takes simple C programs and compiles them to x86-64 assembly, then runs them.

## What it can do

```c
// Basic arithmetic and variables
int main() {
    return 5 + 3 * 2 - 1;
}
```

```c
// Multiple functions with calls
int getAnswer() {
    return 42;
}

int main() {
    int x = getAnswer();
    return x + 8;
}
```

```c
// Functions in expressions and control flow
int getValue() {
    return 10;
}

int main() {
    if (getValue() > 5) {
        return getValue() * 2;
    } else {
        return 0;
    }
}
```

## C Language Subset Supported

**Data Types & Variables:**
- `int` variables only
- Integer literals
- Variable declarations: `int x = 5;` or `int x;`
- Variable assignment: `x = 10;`

**Operators:**
- Arithmetic: `+`, `-`, `*`, `/` (with proper precedence)
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Parentheses: `()` for grouping expressions

**Control Flow:**
- `if` statements: `if (condition) { ... }`
- `if-else` statements: `if (condition) { ... } else { ... }`
- `while` loops: `while (condition) { ... }`
- Arbitrary nesting of all control structures
- Block scoping with `{ }`

**Functions:**
- Multiple function definitions: `int functionName() { ... }`
- Function calls as expressions: `int x = getValue();`
- Functions in arithmetic: `return func1() + func2() * 3;`
- Functions in conditions: `if (checkValue() > 0) { ... }`
- `int main()` as program entry point
- `return` statements with expressions

**Other:**
- Single-line comments: `// comment text`
- Complex expressions mixing arithmetic and comparisons

## Try it out

```bash
make                           # Build the compiler
./bin/vscc examples/sample1.c  # Compile and run a program
make test                      # Run all tests
```

## What happens when you run it

1. **Tokenizes** the C code (splits into keywords, numbers, etc.)
2. **Parses** it into an Abstract Syntax Tree
3. **Generates** Intel syntax x86-64 assembly
4. **Assembles** and **executes** the program
5. Shows you the exit code

The output shows each step so you can see how a compiler works internally.

## Example output

```
=> FILE: examples/sample8.c
====== Emitting Assembly =====
.intel_syntax noprefix
.section .text
    .globl _start

_start:
    call main
    mov rdi, rax
    mov rax, 60
    syscall

main:
    mov rax, 2
    push rax
    mov rax, 3
    mov rbx, rax
    pop rax
    imul rax, rbx    ; multiplication instruction
    ret
====== End of Assembly ======

Program exited with code: 6
```

## Why I built this

Compiler hard but when compiler work good!           
Also good learning experience (looks good on resume maybe?).
