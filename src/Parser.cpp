#include "../include/Parser.h"
#include <iostream>
#include <string>

// Constructor
Parser::Parser(std::vector<Token> t) : tokens(t) {}

// Helpers
Token Parser::peek() {
    return tokens[current];
}

Token Parser::peekNext() {
    if (current + 1 >= tokens.size()) return tokens.back();
    return tokens[current + 1];
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
    return logicOr(); 
}
// 1. Logic OR (Lowest Priority among logic)
std::shared_ptr<Expr> Parser::logicOr() {
    std::shared_ptr<Expr> expr = logicAnd();
    while (check(KW_OR)) {
        Token op = advance();
        std::shared_ptr<Expr> right = logicAnd();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// 2. Logic AND
std::shared_ptr<Expr> Parser::logicAnd() {
    std::shared_ptr<Expr> expr = equality();
    while (check(KW_AND)) {
        Token op = advance();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// 3. Equality (==, !=)
std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    while (check(TOKEN_EQUAL_EQUAL) || check(TOKEN_BANG_EQUAL)) {
        Token op = advance();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
}

// 4. Comparison (<, >, <=, >=)
std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = bitwiseOr(); // Chains to your existing bitwise logic
    while (check(TOKEN_LESS) || check(TOKEN_GREATER) || 
           check(TOKEN_LESS_EQUAL) || check(TOKEN_GREATER_EQUAL)) {
        Token op = advance();
        std::shared_ptr<Expr> right = bitwiseOr();
        expr = std::make_shared<BinaryExpr>(expr, op, right);
    }
    return expr;
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

    if (check(TOKEN_TRUE)) {
        advance();
        return std::make_shared<LiteralExpr>(true);
    }
    if (check(TOKEN_FALSE)) {
        advance();
        return std::make_shared<LiteralExpr>(false);
    }

    if (check(TOKEN_IDENTIFIER)) {
        Token name = advance();
        
        // Check for Function Call: identifier followed by '('
        if (check(TOKEN_LPAREN)) {
            advance(); // Eat '('
            std::vector<std::shared_ptr<Expr>> args;
            if (!check(TOKEN_RPAREN)) {
                do {
                    args.push_back(expression());
                } while (check(TOKEN_COMMA) && advance().type == TOKEN_COMMA);
            }
            Token paren = consume(TOKEN_RPAREN, "Expect ')' after arguments.");
            return std::make_shared<CallExpr>(std::make_shared<VariableExpr>(name), paren, args);
        }

        return std::make_shared<VariableExpr>(name);
    }

    if (check(KW_CONVERT)) {
        advance(); // consume conv_dist
        consume(TOKEN_LPAREN, "Expect '(' after conv_dist");
        
        std::shared_ptr<Expr> val = expression();
        consume(TOKEN_COMMA, "Expect ',' after value");
        std::shared_ptr<Expr> mode = expression();
        
        consume(TOKEN_RPAREN, "Expect ')' after arguments");
        return std::make_shared<ConvertExpr>(val, mode);
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


std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> commands;
    while (!isAtEnd()) {
        auto stmt = statement();
        if (stmt) commands.push_back(stmt);
    }
    return commands;
}

// Decides what kind of statement we are looking at

std::shared_ptr<Stmt> Parser::statement() {

    // 1. IF Statement

    if (check(KW_IF)) {

        return ifStatement();

    }

    // 2. BLOCK Statement { ... }

    if (check(TOKEN_LBRACE)) {

        advance(); // Consume '{'

        return std::make_shared<BlockStmt>(block());

    }
    // 3. INPUT (drim)
    if (check(KW_DRIM)) {
        advance(); consume(TOKEN_LPAREN, "Expect '('");
        Token name = consume(TOKEN_IDENTIFIER, "Expect var name");
        consume(TOKEN_RPAREN, "Expect ')'");
        return std::make_shared<InputStmt>(name);
    }
    // 4. PRINT (wake)
    if (check(KW_WAKE)) {
        advance(); consume(TOKEN_LPAREN, "Expect '('");
        std::shared_ptr<Expr> val = expression();
        consume(TOKEN_RPAREN, "Expect ')'");
        return std::make_shared<PrintStmt>(val);
    }
    // 5. TYPE
    if (check(KW_TYPE)) {
         advance(); consume(TOKEN_LPAREN, "Expect '('");
         std::shared_ptr<Expr> val = expression();
         consume(TOKEN_RPAREN, "Expect ')'");
         return std::make_shared<TypeStmt>(val);
    }
    // 6. ASSIGNMENT (var = val)
    if (check(TOKEN_IDENTIFIER) && peekNext().type == TOKEN_ASSIGN) {
        Token name = advance();
        advance(); // Eat '='
        std::shared_ptr<Expr> value = expression();
        return std::make_shared<AssignStmt>(name, value);
    }

    // Fallback: Skip token to avoid infinite loop on error
    advance();
    return nullptr; 
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(KW_IF, "Expect 'if'.");
    
    // Parse Condition
    std::shared_ptr<Expr> condition = expression();

    // Parse 'Then' Branch
    consume(TOKEN_LBRACE, "Expect '{' after if condition."); 
    std::vector<std::shared_ptr<Stmt>> thenStmts = block();
    std::shared_ptr<Stmt> thenBranch = std::make_shared<BlockStmt>(thenStmts);

    // Parse 'Else' Branch (Optional)
    std::shared_ptr<Stmt> elseBranch = nullptr;
    // if (check(KW_ELSE)) {
    //     advance();
    //     consume(TOKEN_LBRACE, "Expect '{' after else.");
    //     std::vector<std::shared_ptr<Stmt>> elseStmts = block();
    //     elseBranch = std::make_shared<BlockStmt>(elseStmts);
    // }
    if (check(KW_ELSE)) {
        advance(); // Eat 'else'
        if (check(KW_IF)) {
            // Found "else if" -> Recursively parse the next IF statement
            elseBranch = ifStatement(); 
        } 
        else {
            // Found "else {" -> Parse the block normally
            consume(TOKEN_LBRACE, "Expect '{' after else.");
            std::vector<std::shared_ptr<Stmt>> elseStmts = block();
            elseBranch = std::make_shared<BlockStmt>(elseStmts);
        }
    }
    return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> stmts;
    // Keep parsing until we hit '}' or EOF
    while (!check(TOKEN_RBRACE) && !isAtEnd()) {
        auto stmt = statement();
        if (stmt) stmts.push_back(stmt);
    }
    consume(TOKEN_RBRACE, "Expect '}' after block.");
    return stmts;
}