# Very Small C Compiler

I'm building a tiny C compiler from scratch in C++! It takes simple C programs and compiles them to x86-64 assembly, then runs them.

## What it can do

```c
int main() {
    return 5 + 3 * 2 - 1;  // Compiles to assembly and runs, exits with code 10
}

int main() {
    return (5 + 3) * 2;    // Parentheses for grouping, exits with code 16
}

int main() {
    return 24 / (3 + 1);   // Division and grouping, exits with code 6
}
```

Currently supports:
- `int main()` functions
- Integer numbers
- Arithmetic operators: `+`, `-`, `*`, `/`
- Parentheses `()` for expression grouping
- Proper operator precedence (multiplication/division before addition/subtraction)
- `return` statements

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
