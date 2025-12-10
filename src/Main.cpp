#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Interpreter.h"

int main(int argc, char* argv[]) {
    // 1. Setup
    if (argc != 2) {
        std::cout << "Usage: drim <script.drim>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // 2. Lexer
    Lexer lexer(source);
    lexer.scanTokens();

    // 3. Parser
    Parser parser(lexer.tokens);
    //std::vector<std::shared_ptr<Stmt>> = parser.parse();

    auto commands = parser.parse();

    // 4. Interpreter
    Interpreter interpreter;
    interpreter.interpret(commands);

    return 0;
}