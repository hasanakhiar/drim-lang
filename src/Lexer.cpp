//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#include "../include/Lexer.h"
#include "../include/Utils.h"

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
        case '(': addToken(TOKEN_LPAREN); break;
        case ')': addToken(TOKEN_RPAREN); break;
        case '=': addToken(TOKEN_ASSIGN); break;
        case ' ':
        case '\r':
        case '\t': break;
        case '\n': line++; break;
        case '"': string(); break;
        default:
            if (isDigit(c)) number();
            else if (isAlpha(c)) identifier();
            else addToken(TOKEN_ERROR);
            break;
    }
}

void Lexer::identifier() {
    while (isAlphaNumeric(peek())) advance();
    std::string text = source.substr(start, current - start);

    TokenType type = TOKEN_IDENTIFIER;
    if (text == "drim") type = KW_DRIM;
    if (text == "wake") type = KW_WAKE;

    addToken(type);
}