
#ifndef DRIM_LANG_PARSER_H
#define DRIM_LANG_PARSER_H

#include "lexer.h"
#include "ast.h"
#include <memory>

class Parser
{
private:
    Lexer lexer;
    Token currentToken;

    void consume_token(TokenType type);

    // Helper methods to build specific parts of the tree
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Expression> parseExpression();

public:
    Parser(Lexer l);
    std::unique_ptr<Program> parse();
};

#endif //DRIM_LANG_PARSER_H