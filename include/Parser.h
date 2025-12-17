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
    // Constructor
    Parser(std::vector<Token> t);
    
    // convert Tokens -> Commands (AST)
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    bool isAtEnd();
    // Helper to check token type without consuming
    bool check(TokenType type);

    // Hierarchy of expression parsing
    std::shared_ptr<Expr> expression(); // Handles + and -
    std::shared_ptr<Expr> term();       // Handles * and /
    std::shared_ptr<Expr> primary();    // Handles numbers, vars, ( )


    // Check current token
    Token peek();

    // Consume current and move forward
    Token advance();

    //Check specific token
    Token consume(TokenType type, std::string message);
};

#endif