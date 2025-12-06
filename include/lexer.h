
#ifndef DRIM_LANG_LEXER_H
#define DRIM_LANG_LEXER_H

#include "lib_calls.h"

enum TokenType
{
    TOKEN_EOF = 0,
    TOKEN_INT,
    TOKEN_ID,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DRIM,
    TOKEN_WAKE
};

std::string tokenTypeToString(const TokenType type);

struct Token
{
    TokenType type;
    std::string value;
    int line_number;
    int char_position_in_line;
};

class Lexer
{
private:
    std::string src;
    size_t pos;
    int line_number;
    char current_char;
    int char_position_in_line;

    void advance();
    void skipWhitespace();
    void skipComment();
    Token collectIdentifier();
    Token collectNumber();

public:
    Lexer(std::string source);
    Token getNextToken();
    int getLineNumber() const;
    int getCharPositionInLine() const;
};

#endif //DRIM_LANG_LEXER_H