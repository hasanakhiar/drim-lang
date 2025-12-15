#include "include/lexer.h"
#include <iostream>

std::string tokenTypeToString(const TokenType type)
{
    switch (type)
    {
    case TOKEN_INT: return "INTEGER";
    case TOKEN_ID: return "IDENTIFIER";
    case TOKEN_ASSIGN: return "=";
    case TOKEN_LPAREN: return "(";
    case TOKEN_RPAREN: return ")";
    case TOKEN_DRIM: return "KEYWORD_DRIM";
    case TOKEN_WAKE: return "KEYWORD_WAKE";
    case TOKEN_EOF: return "EOF";
    default: return "UNKNOWN";
    }
}

int Lexer::getLineNumber() const {
    return line_number;
}

int Lexer::getCharPositionInLine() const {
    return char_position_in_line;
}

Lexer::Lexer(std::string source) : src(source), pos(0), line_number(1), char_position_in_line(1)
{
    if (!src.empty())
        current_char = src[0];
    else
        current_char = '\0';
}


void Lexer::advance()
{
    pos++;
    char_position_in_line++;
    if (current_char == '\n') {
        line_number++;
        char_position_in_line = 1;
    }
    if (pos < src.length())
        current_char = src[pos];
    else
        current_char = '\0';
}

void Lexer::skipWhitespace()
{
    while (current_char != '\0' && isspace(current_char))
    {
        advance();
    }
}

void Lexer::skipComment()
{
    while (current_char != '\0' && current_char != '\n')
    {
        advance();
    }
}

Token Lexer::collectIdentifier()
{
    std::string res = "";
    while (current_char != '\0' && isalnum(current_char))
    {
        res += current_char;
        advance();
    }
    if (res == "drimIn") return {TOKEN_DRIM, res, getLineNumber(), getCharPositionInLine()};
    if (res == "wakeOut") return {TOKEN_WAKE, res, getLineNumber(), getCharPositionInLine()};
    return {TOKEN_ID, res};
}

Token Lexer::collectNumber()
{
    std::string res = "";
    while (current_char != '\0' && isdigit(current_char))
    {
        res += current_char;
        advance();
    }
    return {TOKEN_INT, res, getLineNumber(), getCharPositionInLine()};
}

Token Lexer::getNextToken()
{
    while (current_char != '\0')
    {
        if (isspace(current_char)) { skipWhitespace(); continue; }
        
        if (current_char == '/') {
            if (pos + 1 < src.length() && src[pos + 1] == '/') {
                skipComment();
                continue;
            }
        }

        if (isdigit(current_char)) return collectNumber();
        if (isalpha(current_char)) return collectIdentifier();

        if (current_char == '=') { advance(); return {TOKEN_ASSIGN, "=", getLineNumber(), getCharPositionInLine()}; }
        if (current_char == '(') { advance(); return {TOKEN_LPAREN, "(", getLineNumber(), getCharPositionInLine()}; }
        if (current_char == ')') { advance(); return {TOKEN_RPAREN, ")", getLineNumber(), getCharPositionInLine()}; }

        std::cerr << "Unexpected character: " << current_char  << " At: " << getLineNumber() << ":" << getCharPositionInLine() << std::endl;
        exit(1);
    }
    return {TOKEN_EOF, "", getLineNumber(), getCharPositionInLine()};
}


