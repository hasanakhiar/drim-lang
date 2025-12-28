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

std::shared_ptr<Expr> Parser::parseExpression() {
    if (peek().type == TOKEN_STRING || peek().type == TOKEN_INT) {
        std::string val = peek().lexeme;
        advance();
        return std::make_shared<LiteralExpr>(val);
    }
    else if (peek().type == TOKEN_IDENTIFIER) {
        // Check if it's a function call: identifier(
        if (current + 1 < tokens.size() && tokens[current + 1].type == TOKEN_LPAREN) {
            Token name = advance(); // Function name
            consume(TOKEN_LPAREN, "Expect '(' after function name");
            auto arg = parseExpression(); // Argument
            consume(TOKEN_RPAREN, "Expect ')' after argument");
            return std::make_shared<CallExpr>(name, arg);
        }
        else {
            Token name = advance();
            return std::make_shared<VariableExpr>(name);
        }
    }
    return nullptr;
}

// === MAIN PARSE LOGIC ===
std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> commands;

    while (peek().type != TOKEN_EOF) {

        //  Handle Input: drim(var)
        if (peek().type == KW_DRIM) {
            advance(); // Eat 'drim'
            consume(TOKEN_LPAREN, "Expect '(' after drim");
            Token varName = consume(TOKEN_IDENTIFIER, "Expect variable name");
            consume(TOKEN_RPAREN, "Expect ')' after variable");

            commands.push_back(std::make_shared<InputStmt>(varName));
        }

        //  Handle Output: wake(value)
        else if (peek().type == KW_WAKE) {
            advance(); // Eat 'wake'
            consume(TOKEN_LPAREN, "Expect '(' after wake");

            std::shared_ptr<Expr> valueToPrint = parseExpression();

            consume(TOKEN_RPAREN, "Expect ')' after value");
            commands.push_back(std::make_shared<PrintStmt>(valueToPrint));
        }

        // Handle Assignment: var = value
        // Is it "NAME =" ?
        else if (peek().type == TOKEN_IDENTIFIER &&
                 (current + 1 < tokens.size() && tokens[current + 1].type == TOKEN_ASSIGN)) {
            Token varName = advance(); // consume name
            advance(); // consume '='

            std::shared_ptr<Expr> valueToSave = parseExpression();

            commands.push_back(std::make_shared<AssignStmt>(varName, valueToSave));
        }
        // Skip unknown tokens for now
        else {
            advance();
        }
    }
    return commands;
}
