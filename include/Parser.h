//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "AST.h"
#include <vector>
#include <memory>
#include <string>

class Parser {
    std::vector<Token> tokens;
    int current = 0;

public:
    // Constructor takes the list of tokens from the Lexer
    Parser(std::vector<Token> t);

    // Main function to convert Tokens -> Commands (AST)
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    // Helper to see the current token without moving
    Token peek();

    // Helper to consume the current token and move forward
    Token advance();

    // Helper to require a specific token (like closing parenthesis)
    Token consume(TokenType type, std::string message);
};

#endif