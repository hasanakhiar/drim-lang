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
#include <map>

// calling this when we hitting return
struct ReturnValue {
    Value value;
    ReturnValue(Value v) : value(v) {}
};

class Interpreter {
    std::shared_ptr<Scope> scope;

    // map to store all the user defined function
    std::map<std::string, std::shared_ptr<FunctionStmt>> userFunctions;

public:
    Interpreter(); 
    void interpret(std::vector<std::shared_ptr<Stmt>> commands);
    Value evaluate(std::shared_ptr<Expr> expr);
};

#endif