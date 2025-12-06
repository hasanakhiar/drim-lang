#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

enum TokenType
{
    TOKEN_EOF = 0,
    TOKEN_INT,
    TOKEN_ID, // a, x, myVar
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DRIM,
    TOKEN_WAKE
};

// Helper for debugging
std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TOKEN_INT:
        return "INTEGER";
    case TOKEN_ID:
        return "IDENTIFIER";
    case TOKEN_ASSIGN:
        return "ASSIGN";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_DRIM:
        return "KEYWORD_DRIM";
    case TOKEN_WAKE:
        return "KEYWORD_WAKE";
    case TOKEN_EOF:
        return "EOF";
    default:
        return "UNKNOWN";
    }
}

// Struct to hold token data
struct Token
{
    TokenType type;
    std::string value;
};
