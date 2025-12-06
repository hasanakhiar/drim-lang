//
// Created by SMTEmon on 12/6/2025.
//

#include "include/parser.h"
#include "include/lib_calls.h"


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

    std::cout << "--- STARTING INTERPRETER ---\n";

    Lexer lexer(sourceCode);
    Parser parser(lexer);

    parser.parse();

    std::cout << "--- END OF PROGRAM ---\n";
    return 0;
}