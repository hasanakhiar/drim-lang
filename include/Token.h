//
// Created by Muntahi Hasan Akhiar on 4/12/25.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {

    KW_DRIM,    // Input
    KW_WAKE,    // Output


    // Math Operators
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_STAR,     // *
    TOKEN_SLASH,    // /
    TOKEN_POW,      // ^

    TOKEN_IDENTIFIER, // Variable
    TOKEN_STRING,     // "hello"
    TOKEN_INT,        // 123


    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_ASSIGN,     // =

    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

#endif