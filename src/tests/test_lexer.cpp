
#include <include/lexer.h>

#include <include/lib_calls.h>


std::string readFileContent(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

int main()
{

    const std::string filename = "../test.drim";
    const std::string sourceCode = readFileContent(filename);

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