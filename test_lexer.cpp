#include <bits/stdc++.h>

#include "lexer.cpp"

int main()
{
    // Your code snippet
    std::string sourceCode =
        "a12 = 10 // This is a comment\n"
        "drim(a)\n"
        "wake(a)";

    std::cout << "Raw Source Code:\n"
              << sourceCode << "\n\n";
    std::cout << "Lexer Output:\n----------------\n";

    Lexer lexer(sourceCode);
    Token token = lexer.getNextToken();

    while (token.type != TOKEN_EOF)
    {
        std::cout << "Type: " << tokenTypeToString(token.type)
                  << " \t| Value: " << token.value << std::endl;
        token = lexer.getNextToken();
    }

    return 0;
}