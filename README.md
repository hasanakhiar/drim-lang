# Drim Lang

A simple custom programming language interpreter written in C++.

## Features

- **Variables**: Supports integer variable assignments using the `=` operator.
- **Input (`drim`)**: Reads integer input from the console into a variable.
- **Output (`wake`)**: Prints variables or integer literals to the console.
- **Comments**: Supports single-line comments starting with `//`.
- **Error Handling**: Detailed syntax and runtime error reporting (e.g., undefined variables, unexpected tokens).

## Usage

### Syntax

```
// Initialize a variable
a = 10

// Print a value
wake(a)
wake(42)

// Get user input
drim(myVar)
wake(myVar)
```

### Keywords

- `drim`: Input command. Syntax: `drim(variable_name)`
- `wake`: Output command. Syntax: `wake(value)` or `wake(variable_name)`

## How It Works

The language processing is split into two main stages:

### 1. Lexer

Located in , the Lexer reads the source code character by character. `src/units/lexer.cpp`

- It skips whitespace and comments.
- It groups characters into **Tokens** (e.g., `TOKEN_INT`, `TOKEN_ID`, `TOKEN_DRIM`, `TOKEN_ASSIGN`).
- It tracks line numbers to provide helpful error messages.

### 2. Parser

Located in , the Parser acts as an interpreter. `src/units/parser.cpp`

- It requests tokens from the Lexer one by one.
- It uses a `std::map<std::string, int>` as memory to store variable values.
- It executes statements sequentially:
  - **Assignments**: Parses the variable name and value, then stores it in the map.
  - **`drim`**: Prompts the user (`drim > `), reads `std::cin`, and saves the input to the variable.
  - **`wake`**: Retrieves the value from the map (checking if it exists) or uses the literal integer, then prints to `std::cout`.
