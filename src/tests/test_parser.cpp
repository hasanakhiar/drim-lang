//
// Created by SMTEmon on 12/6/2025.
//

#include "include/lexer.h"
#include "include/parser.h"
#include "include/interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string readFileContent(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    const std::string filename = "../test.drim";
    const std::string sourceCode = readFileContent(filename);

    std::cout << "--- STARTING INTERPRETER ---\n";

    // 1. Lexer
    Lexer lexer(sourceCode);

    // 2. Parser
    Parser parser(lexer);
    std::unique_ptr<Program> ast = parser.parse();

    // 3. Interpreter
    Interpreter interpreter;
    ast->accept(interpreter);

    std::cout << "--- END OF PROGRAM ---\n";
    return 0;
}