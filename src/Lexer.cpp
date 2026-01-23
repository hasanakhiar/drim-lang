//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#include "../include/Lexer.h"
#include "../include/Utils.h"
#include <iostream>
void Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.push_back({TOKEN_EOF, "", line});
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {

        case '(': addToken(TOKEN_LPAREN); break;
        case ')': addToken(TOKEN_RPAREN); break;
        case '=': addToken(TOKEN_ASSIGN); break;


        // NEW: Math Operators
        case '+': addToken(TOKEN_PLUS); break;
        case '-': addToken(TOKEN_MINUS); break;
        case '*': addToken(TOKEN_STAR); break;
        case '/': addToken(TOKEN_SLASH); break;
        case '^': addToken(TOKEN_POW); break;

        // Bitwise & Shift
        case '&': addToken(TOKEN_BIT_AND); break;
        case '|': addToken(TOKEN_BIT_OR); break;
        case '~': addToken(TOKEN_BIT_NOT); break;
        case '<': 
            if(peek() == '<') { advance(); addToken(TOKEN_LSHIFT); }
            break;
        case '>':
             if(peek() == '>') { advance(); addToken(TOKEN_RSHIFT); }
             break;


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
    if (text == "type") type = KW_TYPE; 

    addToken(type);
}

void Lexer::string() {
    while(peek() != '"' && !isAtEnd()) {
      if(peek() == '\n') line++ ;
      advance();
    }
    if(isAtEnd()) {
        std::cerr << "Unterminated string on line " << line << "\n";
        return ;
    }
    advance() ;

    std::string value = source.substr(start + 1, current - start - 2);
    
    tokens.push_back({TOKEN_STRING, value, line});
}
void Lexer::number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part
    if (peek() == '.' && isDigit(source[current + 1])) {
        // Consume the "."
        advance();

        // Consume the rest of the digits
        while (isDigit(peek())) advance();
        
        // It is a Double
        addToken(TOKEN_DOUBLE);
    } else {
        // It is an Integer
        addToken(TOKEN_INT);
    }
}

void Lexer::addToken(TokenType type){
    std::string text = source.substr(start, current - start);
    tokens.push_back({type, text, line});
}


char Lexer::advance() {
    return source[current++];
}

char Lexer::peek(){
    if(isAtEnd())
        return '\0';
    return source[current];
}

bool Lexer::isAtEnd(){
    return current >= source.length(); 
}
