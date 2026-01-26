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
    std::shared_ptr<Expr> expression(); // Entry point
    std::shared_ptr<Expr> logicOr();    
    std::shared_ptr<Expr> logicAnd();   
    std::shared_ptr<Expr> equality();   
    std::shared_ptr<Expr> comparison(); 


    std::shared_ptr<Expr> bitwiseOr();
    std::shared_ptr<Expr> bitwiseAnd();
    std::shared_ptr<Expr> shift();
    std::shared_ptr<Expr> additive();   // Handles + and -
    std::shared_ptr<Expr> term();       // Handles * and /
    std::shared_ptr<Expr> power();      // Handles ^
    std::shared_ptr<Expr> unary();      // Handles ~
    std::shared_ptr<Expr> primary();    // Handles numbers, vars, ( )


    // --- Statement Parsing ---
    std::shared_ptr<Stmt> statement();     // Decides if it's IF, PRINT, or ASSIGN
    std::shared_ptr<Stmt> ifStatement();   // Parses if-else
    std::vector<std::shared_ptr<Stmt>> block(); // Parses { ... }

    // Check current token
    Token peek();
    Token peekNext(); 
    
    // Consume current and move forward
    Token advance();

    //Check specific token
    Token consume(TokenType type, std::string message);
};

#endif