# Simple Shell

A simple Unix shell implemented in C as a Systems Programming project.

## Features

- Command execution
- Command arguments
- Pipes (`|`)
- Input redirection (`<`)
- Output redirection (`>`)
- Read-write redirection (`<>`) 
- Output append redirection (`>>`)
- Heredocs (`<<`)
- File descriptor redirections
- Command parsing and tokenization
- Built-in command support
- Process management using `fork()` and `exec()`

## Project Structure

```text
simple-shell/
├── include/
│   ├── command.h
│   ├── execution.h
│   ├── linked_list.h
│   ├── parser.h
│   ├── redirection.h
│   ├── token.h
│   ├── tokenizer.h
│   └── utils.h
├── src/
│   ├── command.c
│   ├── execution.c
│   ├── linked_list.c
│   ├── main.c
│   ├── parser.c
│   ├── redirection.c
│   ├── token.c
│   ├── tokenizer.c
│   └── utils.c
├── Makefile
└── README.md
```
## Technologies
C
GCC
Make
Unix/Linux System calls API
