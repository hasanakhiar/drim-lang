//
// Created by SMTEmon on 12/14/2025.
//

#ifndef DRIM_LANG_INTERPRETER_H
#define DRIM_LANG_INTERPRETER_H

#include "ast.h"
#include <map>
#include <string>
#include <iostream>

class Interpreter : public Visitor {
private:
    std::map<std::string, int> memory;
    int lastEvaluationResult; // Stores result of Expressions, since we have void return type on nodes

public:
    void visit(Number* node) override;
    void visit(Variable* node) override;
    void visit(Assignment* node) override;
    void visit(DrimStatement* node) override;
    void visit(WakeStatement* node) override;
    void visit(Program* node) override;
};

#endif //DRIM_LANG_INTERPRETER_H