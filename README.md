# Drim Lang

Drim Lang is a custom interpreted programming language written in C++. It features a clean syntax for mathematical operations, logic control, and specialized built-in functions for physics calculations and unit conversions.

## Features

- **Variables & Dynamic Typing**: Supports Integers, Floating-point numbers, and Strings.
- **Input/Output**:
  - `wake(...)`: Output data to the console.
  - `drim(...)`: Take input from the user.
- **Control Flow**: `if`, `else if`, and `else` blocks.
- **Arithmetic Operations**: Addition, subtraction, multiplication, division, and power (`^`).
- **Bitwise Operations**: AND (`&`), OR (`|`), Left Shift (`<<`), Right Shift (`>>`).
- **Logical Operators**: `and`, `or`.
- **Built-in Physics Functions**: Calculate speed, force, final velocity, and temperature conversions.
- **Unit Conversions**: Easy conversion for length, mass, temperature, and currency.

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

### Hello World & Variables
```drim
msg = "Hello, Drim!"
wake(msg)

x = 10
y = 20
wake(x + y)
```

### Input
```drim
wake("Enter your name:")
drim(name)
wake("Hello " + name)
```

### Logic & Control Flow
```drim
score = 85
if score > 90 {
    wake("Grade: A")
} else if score > 80 {
    wake("Grade: B")
} else {
    wake("Keep trying!")
}
```

### Physics & Conversions
```drim
// Calculate Force: F = ma
f = force(10, 9.8)
wake("Force: ")
wake(f)

// Convert Celsius to Fahrenheit
tempF = convert(25, "c_f")
wake("25C in Fahrenheit")
wake(tempF)
```

## Project Structure

- `src/`: Source code for the interpreter (Lexer, Parser, Interpreter, etc.).
- `include/`: Header files.
- `testing_sources/`: Example `.drim` scripts to test language features.
- `CMakeLists.txt`: Build configuration.