//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "AST.h"
#include "Value.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

class Interpreter {
    std::map<std::string, Value> memory;

public:
    void interpret(std::vector<std::shared_ptr<Stmt>> commands);
    Value evaluate(std::shared_ptr<Expr> expr);
};

#endif