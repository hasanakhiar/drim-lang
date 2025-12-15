#include "include/parser.h"
#include <iostream>

Parser::Parser(Lexer l) : lexer(l)
{
    currentToken = lexer.getNextToken();
}

void Parser::consume_token(TokenType type)
{
    if (currentToken.type == type)
    {
        currentToken = lexer.getNextToken();
    }
    else
    {
        std::cerr << "Syntax Error: At " << currentToken.line_number << ":"
                  << currentToken.char_position_in_line
                  << " Expected " << tokenTypeToString(type)
                  << " but got " << tokenTypeToString(currentToken.type) << std::endl;
        exit(1);
    }
}

// Main entry point
std::unique_ptr<Program> Parser::parse()
{
    auto program = std::make_unique<Program>();

    while (currentToken.type != TOKEN_EOF)
    {
        program->statements.push_back(parseStatement());
    }
    return program;
}

std::unique_ptr<Statement> Parser::parseStatement()
{
    if (currentToken.type == TOKEN_ID)
    {
        std::string varName = currentToken.value;
        consume_token(TOKEN_ID);
        consume_token(TOKEN_ASSIGN);

        // Parse the right side as an expression
        auto value = parseExpression();

        return std::make_unique<Assignment>(varName, std::move(value));
    }
    else if (currentToken.type == TOKEN_DRIM)
    {
        consume_token(TOKEN_DRIM);
        consume_token(TOKEN_LPAREN);

        std::string varName = currentToken.value;
        consume_token(TOKEN_ID);

        consume_token(TOKEN_RPAREN);

        return std::make_unique<DrimStatement>(varName);
    }
    else if (currentToken.type == TOKEN_WAKE)
    {
        consume_token(TOKEN_WAKE);
        consume_token(TOKEN_LPAREN);

        // The inside of wake is an expression
        auto expr = parseExpression();

        consume_token(TOKEN_RPAREN);

        return std::make_unique<WakeStatement>(std::move(expr));
    }
    else
    {
        std::cerr << "Unexpected token: " << tokenTypeToString(currentToken.type) << std::endl;
        exit(1);
    }
}

std::unique_ptr<Expression> Parser::parseExpression()
{
    if (currentToken.type == TOKEN_INT)
    {
        int val = std::stoi(currentToken.value);
        consume_token(TOKEN_INT);
        return std::make_unique<Number>(val);
    }
    else if (currentToken.type == TOKEN_ID)
    {
        std::string name = currentToken.value;
        consume_token(TOKEN_ID);
        return std::make_unique<Variable>(name);
    }
    else
    {
        std::cerr << "Expected Integer or Identifier at " << currentToken.line_number << std::endl;
        exit(1);
    }
}