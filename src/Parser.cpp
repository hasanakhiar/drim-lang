//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//
#include "../include/Parser.h"
#include <iostream>

// Constructor
Parser::Parser(std::vector<Token> t) : tokens(t) {}

// Helpers
Token Parser::peek() {
    return tokens[current];
}

Token Parser::advance() {
    if (current < tokens.size()) current++;
    return tokens[current - 1];
}

Token Parser::consume(TokenType type, std::string message) {
    if (peek().type == type) return advance();
    std::cerr << "Error: " << message << " on line " << peek().line << "\n";
    exit(1);
}

// === MAIN PARSE LOGIC ===
std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> commands;

    while (peek().type != TOKEN_EOF) {

        // 1. Handle Input: drim(var)
        if (peek().type == KW_DRIM) {
            advance(); // Eat 'drim'
            consume(TOKEN_LPAREN, "Expect '(' after drim");
            Token varName = consume(TOKEN_IDENTIFIER, "Expect variable name");
            consume(TOKEN_RPAREN, "Expect ')' after variable");

            commands.push_back(std::make_shared<InputStmt>(varName));
        }

        // 2. Handle Output: wake(value)
        else if (peek().type == KW_WAKE) {
            advance(); // Eat 'wake'
            consume(TOKEN_LPAREN, "Expect '(' after wake");

            std::shared_ptr<Expr> valueToPrint;

            // Is it a String "hello"?
            if (peek().type == TOKEN_STRING) {
                valueToPrint = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }
            // Is it a Variable name?
            else if (peek().type == TOKEN_IDENTIFIER) {
                valueToPrint = std::make_shared<VariableExpr>(peek());
                advance();
            }
            // Is it a Number? (Treat as text for now)
            else if (peek().type == TOKEN_INT) {
                valueToPrint = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }

            consume(TOKEN_RPAREN, "Expect ')' after value");
            commands.push_back(std::make_shared<PrintStmt>(valueToPrint));
        }

        // 3. Handle Assignment: var = value
        // We look ahead: Is it "NAME =" ?
        else if (peek().type == TOKEN_IDENTIFIER &&
                 (current + 1 < tokens.size() && tokens[current + 1].type == TOKEN_ASSIGN)) {
            Token varName = advance(); // Eat name
            advance(); // Eat '='

            std::shared_ptr<Expr> valueToSave;

            if (peek().type == TOKEN_STRING) {
                valueToSave = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }
            else if (peek().type == TOKEN_INT) {
                valueToSave = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }
            else if (peek().type == TOKEN_IDENTIFIER) {
                valueToSave = std::make_shared<VariableExpr>(peek());
                advance();
            }

            commands.push_back(std::make_shared<AssignStmt>(varName, valueToSave));
        }
        // Skip unknown tokens for now
        else {
            advance();
        }
    }
    return commands;
}
