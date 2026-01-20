//
// Created by Muntahi Hasan Akhiar on 4/12/25.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {

    KW_DRIM,    // Input
    KW_WAKE,    // Output


    TOKEN_IDENTIFIER, // Variable
    TOKEN_STRING,     // "hello"
    TOKEN_INT,        // 123
    TOKEN_DOUBLE,

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