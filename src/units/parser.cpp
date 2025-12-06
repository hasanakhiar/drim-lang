
#include <include/lexer.h>
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
        std::cerr << "Syntax Error: At: " << currentToken.line_number << ":" << currentToken.char_position_in_line + 1 << " Expected token type " << tokenTypeToString(type)
                  << " but got type " << tokenTypeToString(currentToken.type) << std::endl;
        exit(1);
    }
}

void Parser::parse()
{
    while (currentToken.type != TOKEN_EOF)
    {
        if (currentToken.type == TOKEN_ID)
        {
            std::string varName = currentToken.value;
            consume_token(TOKEN_ID);
            consume_token(TOKEN_ASSIGN);

            int val = std::stoi(currentToken.value);
            consume_token(TOKEN_INT);

            memory[varName] = val;
        }
        else if (currentToken.type == TOKEN_DRIM)
        {
            consume_token(TOKEN_DRIM);
            consume_token(TOKEN_LPAREN);

            std::string varName = currentToken.value;
            consume_token(TOKEN_ID);

            consume_token(TOKEN_RPAREN);

            int inputVal;
            std::cout << "drim > ";
            std::cin >> inputVal;
            memory[varName] = inputVal;
        }
        else if (currentToken.type == TOKEN_WAKE)
        {
            consume_token(TOKEN_WAKE);
            consume_token(TOKEN_LPAREN);

            if (currentToken.type == TOKEN_INT) {
                std::string varVal = currentToken.value;
                std::cout << varVal << std::endl;
                consume_token(TOKEN_INT);
                consume_token(TOKEN_RPAREN);
                continue;
            }

            std::string varName = currentToken.value;
            consume_token(TOKEN_ID);

            consume_token(TOKEN_RPAREN);

            if (memory.find(varName) != memory.end()) {
                std::cout << memory[varName] << std::endl;
            } else {
                std::cerr << "Runtime Error: At: " << currentToken.line_number << ":" << currentToken.char_position_in_line + 1 << " Variable '" << varName << "' is undefined." << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cerr << "Syntax Error: Unexpected token At: " << currentToken.line_number << ":" << currentToken.char_position_in_line + 1 << " " << tokenTypeToString(currentToken.type) << std::endl;
            exit(1);
        }
    }
}