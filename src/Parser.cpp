//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

// ----------------------------------------------------     TODO - Implement STOI, STOD  -------------------------------------------------------

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
            advance(); 
            consume(TOKEN_LPAREN, "Expect '(' after wake");

            std::shared_ptr<Expr> valueToPrint;

            if (peek().type == TOKEN_STRING) {
                valueToPrint = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }

            else if (peek().type == TOKEN_INT) {
                // Convert text "123" -> int 123
                int val = std::stoi(peek().lexeme);
                valueToPrint = std::make_shared<LiteralExpr>(val);
                advance();
            }

            else if (peek().type == TOKEN_DOUBLE) {
                // Convert text "123.45" -> double 123.45
                double val = std::stod(peek().lexeme);
                valueToPrint = std::make_shared<LiteralExpr>(val);
                advance();
            }

            // Checking for variable
            else if (peek().type == TOKEN_IDENTIFIER) {
                valueToPrint = std::make_shared<VariableExpr>(peek());
                advance();
            }

            consume(TOKEN_RPAREN, "Expect ')' after value");
            commands.push_back(std::make_shared<PrintStmt>(valueToPrint));
        }

        else if (peek().type == KW_TYPE) {
            advance();   
            consume(TOKEN_LPAREN, "Expect '(' after type");

            std::shared_ptr<Expr> valueToCheck;

            // Detect What's inside
            if (peek().type == TOKEN_STRING) {
                valueToCheck = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }
            else if (peek().type == TOKEN_INT) {
                int val = std::stoi(peek().lexeme);
                valueToCheck = std::make_shared<LiteralExpr>(val);
                advance();
            }
            else if (peek().type == TOKEN_DOUBLE) {
                double val = std::stod(peek().lexeme);
                valueToCheck = std::make_shared<LiteralExpr>(val);
                advance();
            }
            else if (peek().type == TOKEN_IDENTIFIER) {
                valueToCheck = std::make_shared<VariableExpr>(peek());
                advance();
            }

            consume(TOKEN_RPAREN, "Expect ')' after value");
            commands.push_back(std::make_shared<TypeStmt>(valueToCheck));
        }
        // Handle Assignment: var = value        --   Is it "NAME =" ?
        else if (peek().type == TOKEN_IDENTIFIER &&
                 (current + 1 < tokens.size() && tokens[current + 1].type == TOKEN_ASSIGN)) {
            Token varName = advance(); // consume name
            advance(); // consume '='

            std::shared_ptr<Expr> valueToSave;

            if (peek().type == TOKEN_STRING) {
                valueToSave = std::make_shared<LiteralExpr>(peek().lexeme);
                advance();
            }
            else if (peek().type == TOKEN_INT) {
                int val = std::stoi(peek().lexeme);
                valueToSave = std::make_shared<LiteralExpr>(val);
                advance();
            }
            else if (peek().type == TOKEN_DOUBLE) {
                double val = std::stod(peek().lexeme);
                valueToSave = std::make_shared<LiteralExpr>(val);
                advance();
            }
            else if (peek().type == TOKEN_IDENTIFIER) {
                valueToSave = std::make_shared<VariableExpr>(peek());
                advance();
            }

            commands.push_back(std::make_shared<AssignStmt>(varName, valueToSave));
        }
        // Skipping Unknown Tokens for now
        else {
            advance();
        }
    }
    return commands;
}
