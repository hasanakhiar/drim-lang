
#ifndef DRIM_LANG_PARSER_H
#define DRIM_LANG_PARSER_H

#include "lexer.h"
#include <map>
#include <string>

class Parser
{
private:
    Lexer lexer;
    Token currentToken;
    std::map<std::string, int> memory;

    void consume_token(TokenType type);

public:
    Parser(Lexer l);
    void parse();
};

#endif //DRIM_LANG_PARSER_H