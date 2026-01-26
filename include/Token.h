//
// Created by Muntahi Hasan Akhiar on 4/12/25.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum TokenType {

    KW_DRIM,    // Input
    KW_WAKE,    // Output
    KW_TYPE,    // Type Function
    KW_CONVERT, // Conversion Function
    KW_IF, KW_ELSE,   //
    KW_AND, KW_OR,    // (Logic)

    // Comparison Operators
    TOKEN_LESS,          // <
    TOKEN_GREATER,       // >
    TOKEN_LESS_EQUAL,    // <=
    TOKEN_GREATER_EQUAL, // >=
    TOKEN_EQUAL_EQUAL,   // ==
    TOKEN_BANG_EQUAL,    // != (Not Equal)
    
    // Math Operators
    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_STAR,     // *
    TOKEN_SLASH,    // /
    TOKEN_POW,      // ^

    // Bitwise Operators
    TOKEN_BIT_AND,    // &
    TOKEN_BIT_OR,     // |
    TOKEN_BIT_NOT,    // ~
    TOKEN_LSHIFT,     // <<
    TOKEN_RSHIFT,     // >>

    TOKEN_IDENTIFIER, // Variable
    TOKEN_STRING,     // "hello"
    TOKEN_INT,        // 123
    TOKEN_DOUBLE,

    // Syntax
    TOKEN_LPAREN,     // (
    TOKEN_RPAREN,     // )
    TOKEN_ASSIGN,     // =
    TOKEN_COMMA,      // ,
    TOKEN_LBRACE,     // {
    TOKEN_RBRACE,     // }

    TOKEN_EOF,
    TOKEN_ERROR
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

#endif