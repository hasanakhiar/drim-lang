#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

enum TokenType
{
    TOKEN_EOF = 0,
    TOKEN_INT,
    TOKEN_ID, // a, x, myVar
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DRIM,
    TOKEN_WAKE
};

// Helper for debugging
std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TOKEN_INT:
        return "INTEGER";
    case TOKEN_ID:
        return "IDENTIFIER";
    case TOKEN_ASSIGN:
        return "ASSIGN";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_DRIM:
        return "KEYWORD_DRIM";
    case TOKEN_WAKE:
        return "KEYWORD_WAKE";
    case TOKEN_EOF:
        return "EOF";
    default:
        return "UNKNOWN";
    }
}

// Struct to hold token data
struct Token
{
    TokenType type;
    std::string value;
};

class Lexer
{
private:
    std::string src;
    size_t pos;
    char current_char;

    void advance()
    {
        pos++;
        if (pos < src.length())
        {
            current_char = src[pos];
        }
        else
        {
            current_char = '\0'; // End of file
        }
    }

    void skipWhitespace()
    {
        while (current_char != '\0' && isspace(current_char))
        {
            advance();
        }
    }

    void skipComment()
    {
        // Skip until the end of the line
        while (current_char != '\0' && current_char != '\n')
        {
            advance();
        }
    }

    // Parse keywords (drim, wake) and variable names
    Token collectIdentifier()
    {
        std::string res = "";
        while (current_char != '\0' && isalnum(current_char))
        {
            res += current_char;
            advance();
        }

        // Check for keyword
        if (res == "drim")
            return {TOKEN_DRIM, res};
        if (res == "wake")
            return {TOKEN_WAKE, res};

        // Otherwise res == variable name
        return {TOKEN_ID, res};
    }

    Token collectNumber()
    {
        std::string res = "";
        while (current_char != '\0' && isdigit(current_char))
        {
            res += current_char;
            advance();
        }
        return {TOKEN_INT, res};
    }

public:
    Lexer(std::string source) : src(source), pos(0)
    {
        if (!src.empty())
            current_char = src[0];
        else
            current_char = '\0';
    }

    Token getNextToken()
    {
        while (current_char != '\0')
        {

            // handle whitespace
            if (isspace(current_char))
            {
                skipWhitespace();
                continue;
            }

            // handle comments
            if (current_char == '/')
            {
                if (pos + 1 < src.length() && src[pos + 1] == '/')
                {
                    skipComment();
                    continue;
                }
            }

            // handle numbers
            if (isdigit(current_char))
            {
                return collectNumber();
            }

            // handle identifiers and keywords (can only have int and alpha rn)
            if (isalpha(current_char))
            {
                return collectIdentifier();
            }

            // handle symbols
            if (current_char == '=')
            {
                advance();
                return {TOKEN_ASSIGN, "="};
            }
            if (current_char == '(')
            {
                advance();
                return {TOKEN_LPAREN, "("};
            }
            if (current_char == ')')
            {
                advance();
                return {TOKEN_RPAREN, ")"};
            }

            // error handling for unexpected Characters
            std::cerr << "Unexpected character: " << current_char << std::endl;
            exit(1);
        }

        return {TOKEN_EOF, ""};
    }
};
