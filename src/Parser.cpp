#include "../include/Parser.h"
#include <iostream>
#include <string>

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


// === EXPRESSION PARSING (Entry Point) ===
std::shared_ptr<Expr> Parser::expression() {
    return bitwiseOr();
}

// === BITWISE OR (|) ===
std::shared_ptr<Expr> Parser::bitwiseOr() {
    std::shared_ptr<Expr> expr = bitwiseAnd();
    while (check(TOKEN_BIT_OR)) {
        Token op = advance();
        std::shared_ptr<Expr> right = bitwiseAnd();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// === BITWISE AND (&) ===
std::shared_ptr<Expr> Parser::bitwiseAnd() {
    std::shared_ptr<Expr> expr = shift();
    while (check(TOKEN_BIT_AND)) {
        Token op = advance();
        std::shared_ptr<Expr> right = shift();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// === SHIFT (<<, >>) ===
std::shared_ptr<Expr> Parser::shift() {
    std::shared_ptr<Expr> expr = additive();
    while (check(TOKEN_LSHIFT) || check(TOKEN_RSHIFT)) {
        Token op = advance();
        std::shared_ptr<Expr> right = additive();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// === ADDITIVE (+, -) ===
std::shared_ptr<Expr> Parser::additive() {
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
    std::shared_ptr<Expr> expr = power();

    while (check(TOKEN_STAR) || check(TOKEN_SLASH)) {
        Token op = advance();
        std::shared_ptr<Expr> right = power();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

// === POWER PARSING (Right-associative) ===
std::shared_ptr<Expr> Parser::power() {
    std::shared_ptr<Expr> expr = unary();

    while (check(TOKEN_POW)) {
        Token op = advance();
        std::shared_ptr<Expr> right = power();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }

    return expr;
}

// === UNARY PARSING (~, etc) ===
std::shared_ptr<Expr> Parser::unary() {
    if (check(TOKEN_BIT_NOT)) {
        Token op = advance();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<UnaryExpr>(op, right);
    }
    return primary();
}

// === PRIMARY PARSING (Highest Priority: literals, vars, parens) ===
std::shared_ptr<Expr> Parser::primary() {
    if (check(TOKEN_INT)) {
        int val = std::stoi(advance().lexeme);
        return std::make_shared<LiteralExpr>(val);
    }

    if (check(TOKEN_DOUBLE)) {
        double val = std::stod(advance().lexeme);
        return std::make_shared<LiteralExpr>(val);
    }

    if (check(TOKEN_STRING)) {
        return std::make_shared<LiteralExpr>(advance().lexeme);
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
            consume(TOKEN_LPAREN, "Expect '(' after wake");

            std::shared_ptr<Expr> value = expression();

            consume(TOKEN_RPAREN, "Expect ')' after value");
            commands.push_back(std::make_shared<PrintStmt>(value));
        }
        else if (peek().type == KW_TYPE) {
            advance();   
            consume(TOKEN_LPAREN, "Expect '(' after type");

            std::shared_ptr<Expr> valueToCheck = expression();

            consume(TOKEN_RPAREN, "Expect ')' after value");
            commands.push_back(std::make_shared<TypeStmt>(valueToCheck));
        }
        else if (peek().type == TOKEN_IDENTIFIER &&
                (current + 1 < tokens.size() && tokens[current+1].type == TOKEN_ASSIGN)) {

            Token varName = advance();
            advance(); // Eat '='

            std::shared_ptr<Expr> value = expression();

            commands.push_back(std::make_shared<AssignStmt>(varName, value));
        }
        else {
            advance();
        }
    }
    return commands;
}
