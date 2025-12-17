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

// NEW HELPER
bool Parser::isAtEnd() {
    return peek().type == TOKEN_EOF;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}


// === EXPRESSION PARSING (Lowest Priority: + -) ===
std::shared_ptr<Expr> Parser::expression() {
    std::shared_ptr<Expr> expr = term();

    while (check(TOKEN_PLUS) || check(TOKEN_MINUS)) {
        Token op = advance();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

// === TERM PARSING (Higher Priority: * /) ===
std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = primary();

    while (check(TOKEN_STAR) || check(TOKEN_SLASH)) {
        Token op = advance();
        std::shared_ptr<Expr> right = primary();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

// === PRIMARY PARSING (Highest Priority: literals, vars, parens) ===
std::shared_ptr<Expr> Parser::primary() {
    if (check(TOKEN_INT) || check(TOKEN_STRING)) {
        Token t = advance();
        return std::make_shared<LiteralExpr>(t.lexeme);
    }

    if (check(TOKEN_IDENTIFIER)) {
        return std::make_shared<VariableExpr>(advance());
    }

    if (check(TOKEN_LPAREN)) {
        advance();
        std::shared_ptr<Expr> expr = expression();
        consume(TOKEN_RPAREN, "Expect ')' after expression.");
        return expr;
    }

    std::cerr << "Error: Expect expression on line " << peek().line << "\n";
    exit(1);
}

// === MAIN PARSE LOGIC UPDATED ===
std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> commands;

    while (peek().type != TOKEN_EOF) {

        if (peek().type == KW_DRIM) {
            advance();
            consume(TOKEN_LPAREN, "Expect '('");
            Token varName = consume(TOKEN_IDENTIFIER, "Expect var name");
            consume(TOKEN_RPAREN, "Expect ')'");
            commands.push_back(std::make_shared<InputStmt>(varName));
        }
        else if (peek().type == KW_WAKE) {
            advance();
            consume(TOKEN_LPAREN, "Expect '('");

            // NOW WE PARSE A FULL EXPRESSION, NOT JUST A LITERAL
            std::shared_ptr<Expr> value = expression();

            consume(TOKEN_RPAREN, "Expect ')'");
            commands.push_back(std::make_shared<PrintStmt>(value));
        }
        else if (peek().type == TOKEN_IDENTIFIER &&
                (current + 1 < tokens.size() && tokens[current+1].type == TOKEN_ASSIGN)) {

            Token varName = advance();
            advance(); // Eat '='

            // NOW WE PARSE A FULL EXPRESSION
            std::shared_ptr<Expr> value = expression();

            commands.push_back(std::make_shared<AssignStmt>(varName, value));
        }
        else {
            advance();
        }
    }
    return commands;
}
