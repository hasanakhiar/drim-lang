//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "AST.h"
#include "Value.h"
#include "Scope.h"
#include <vector>
#include <string>
#include <memory>

class Interpreter {
    std::shared_ptr<Scope> scope;

public:
    Interpreter(); 
    void interpret(std::vector<std::shared_ptr<Stmt>> commands);
    Value evaluate(std::shared_ptr<Expr> expr);
};

#endif