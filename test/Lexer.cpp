//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "../include/Lexer.h"

// Helper
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case KW_DRIM: return "KW_DRIM";
        case KW_WAKE: return "KW_WAKE";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_INT: return "INT";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

int main(int argc, char* argv[]) {

    std::string source = std::string(R"(
        wake("hello")
        wake(;)
    )");


    std::cout << "Scanning file...\n";
    std::cout << "--------------------------------\n";

    Lexer lexer(source);
    lexer.scanTokens();


    for (const Token& t : lexer.tokens) {
        std::cout << "Line " << t.line << " | "
                  << tokenTypeToString(t.type) << " \t | "
                  << t.lexeme << "\n";
    }
    std::cout << "--------------------------------\n";

    return 0;
}