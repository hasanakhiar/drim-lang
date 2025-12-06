//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    Lexer(std::string s) : source(s) {}
    void scanTokens();

private:
    void scanToken();
    void identifier();
    void string();
    void number();
    void addToken(TokenType type);

    char advance();
    char peek();
    bool isAtEnd();
};

#endif