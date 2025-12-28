//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "AST.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

class Interpreter {
    // Stores variable name "a" -> value "10"
    std::map<std::string, std::string> memory;

public:
    // Function for executing commands
    void interpret(std::vector<std::shared_ptr<Stmt>> commands);

private:
    std::string evaluate(std::shared_ptr<Expr> expr);
};

#endif