//
// Created by Muntahi Hasan Akhiar on 11/12/25.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Interpreter.h"

int main(int argc, char* argv[]) {
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

        //  Lexer

        Lexer lexer(source);

        lexer.scanTokens();

    

        //  Parser

        Parser parser(lexer.tokens);

        auto commands = parser.parse();

    

        //  Interpreter

        Interpreter interpreter;

        interpreter.interpret(commands);

    


    return 0;
}
