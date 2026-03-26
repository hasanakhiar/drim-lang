# Drim Lang

Drim Lang is a custom-interpreted programming language written in C++. It features a clean syntax for mathematical operations, logic control, and specialized built-in functions for physics calculations, unit conversions, and built-in data structures.

## Features

- **Variables & Dynamic Typing**: Supports Integers (with **int64** precision), Floating-point numbers, and Strings.
- **Type Checking**: Identify variable types at runtime using the `type()` keyword.
- **Input/Output**:
  - `wake(...)`: Output data to the console.
  - `drim(...)`: Take input from the user.
- **Control Flow**: `if`, `else if`, and `else` blocks.
- **Loops**:
  - `drimming condition { ... }`: A versatile loop (similar to `while`).
  - `stopdrim`: Break out of a loop.
  - `drimagain`: Skip to the next iteration of a loop.
- **Functions**: Define reusable code blocks with `func` and return values with `return`. Supports recursion.
- **Arrays**:
  - Dynamic arrays: `x = [1, 2, 3]`.
  - Type-safe input: `y[]` (automatically infers and enforces type based on the first input).
- **String Interpolation**: Easily embed variables in strings using `{variable_name}`.
- **Data Structures**: Built-in support for Queues and Stacks.
- **Multi-Assignment**: Assign values to multiple variables in a single line: `x = 10, y = 20`.
- **Arithmetic Operations**: Addition, subtraction, multiplication, division, modulo (`%`), and power (`^`).
- **Bitwise Operations**: AND (`&`), OR (`|`), NOT (`~`), Left Shift (`<<`), Right Shift (`>>`).
- **Logical Operators**: `and`, `or`.
- **Built-in Physics Functions**: Direct support for formulas like `force` ($F=ma$), `speed`, `final_velocity`, and mass-energy ($E=mc^2$).
- **Unit Conversions**: Built-in tools to convert between units for length and temperature (for example, inches to cm or Celsius to Fahrenheit) and more via `convert(val, "type")`.

## Getting Started

### Prerequisites

- C++17 compliant compiler (GCC, Clang, MSVC)
- CMake (Version 3.10 or higher)

### Build Instructions

1.  Clone the repository:

    ```bash
    git clone <repository_url>
    cd drim-lang
    ```

2.  Create a build directory:

    ```bash
    mkdir build
    cd build
    ```

3.  Generate build files with CMake:

    ```bash
    cmake ..
    ```

4.  Build the project:
    ```bash
    cmake --build .
    ```

### Running the Interpreter

Once built, you can run `.drim` scripts using the generated executable:

```bash
./drim ../testing_sources/testing_everything.drim
```

Or on Windows:

```powershell
.\drim.exe ..\testing_sources\testing_everything.drim
```

## Language Examples

### Hello World & String Interpolation

```drim
name = "Drimmer"
wake("Hello, {name}!")

score = 100
wake("Your score is: {score}")
```

### Functions & Recursion

```drim
func factorial(n) {
    if n <= 1 {
        return 1
    }
    return n * factorial(n - 1)
}

wake("Factorial of 5: " + factorial(5))
```

### Loops & Control Flow

```drim
i = 1
drimming i <= 10 {
    if i % 2 == 0 {
        wake("{i} is even")
    } else {
        wake("{i} is odd")
    }

    if i == 5 {
        wake("Found 5, skipping...")
        i = i + 1
        drimagain
    }

    i = i + 1
}
```

### Arrays

```drim
arr = [10, 20, 30, 40]
wake("Element at index 2: " + arr[2])

// Declaring a type-safe array for input
names[]
i = 0
drimming i < 3 {
    wake("Enter name {i}:")
    drim(names[i])
    i = i + 1
}
```

### Built-in Data Structures (Stack & Queue)

```drim
// Stack example (LIFO)
s = stack_create()
stack_push(s, 1)
stack_push(s, 2)
wake("Stack size: " + stack_size(s))
wake("Popped from stack: " + stack_pop(s)) // Returns 2

// Queue example (FIFO)
q = queue_create()
queue_enqueue(q, "first")
queue_enqueue(q, "second")
wake("Queue size: " + queue_size(q))
item = queue_dequeue(q)
wake("Dequeued: " + item) // Returns "first"
```

### Type Checking

```drim
x = 10
wake(type(x)) // Outputs: <type 'int'>

y = "Hello"
wake(type(y)) // Outputs: <type 'string'>

z = 3.14
wake(type(z)) // Outputs: <type 'float'>
```

### Physics & Conversions

```drim
// Calculate Force: F = ma
f = force(10, 9.8)
wake("Force: {f} N")

// Convert Celsius to Fahrenheit
tempF = convert(25, "c_f")
wake("25C in Fahrenheit: {tempF}")
```

## Project Structure

```text
drim-lang/
├── include/           # Header files
│   ├── AST.h          # Abstract Syntax Tree node definitions
│   ├── DS.h           # Data Structure definitions
│   ├── Interpreter.h  # Tree-walk interpreter logic
│   ├── Lexer.h        # Lexical analyzer (tokenizer)
│   ├── Parser.h       # Recursive descent parser
│   ├── Physics.h      # Physics engine & conversions
│   ├── Scope.h        # Variable scoping & environment
│   ├── Signal.h       # Functions for control flow of loops
│   ├── Token.h        # Token types and definitions
│   └── Value.h        # Dynamic value type (int, float, string, etc.)
├── src/               # Implementation files
│   ├── Main.cpp       # Entry point for the CLI
│   ├── DS.cpp
│   ├── Interpreter.cpp
│   ├── Lexer.cpp
│   ├── Parser.cpp
│   ├── Utils.cpp
│   └── Physics.cpp
├── testing_sources/   # Example .drim scripts and test cases
├── docs/              # Project documentation and reports
│   ├── final-report/  # Final LaTeX report and diagrams
│   └── proposal-report/
└── CMakeLists.txt     # Build configuration
```

- **`include/` & `src/`**: The core of the interpreter. The language follows a classic pipeline: Lexer → Parser (AST) → Interpreter.
- **`testing_sources/`**: Contains numerous `.drim` files demonstrating every feature from basic loops to complex recursion and data structures.
- **`docs/`**: Detailed technical documentation, including the final project report, class diagrams, and system architecture flowcharts.
- **`CMakeLists.txt`**: Cross-platform build instructions for the C++ compiler.
