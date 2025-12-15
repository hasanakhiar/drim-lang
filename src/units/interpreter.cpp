//
// Created by SMTEmon on 12/14/2025.
//

#include "include/interpreter.h"

// --- Implement AST Node accept methods ---
void Number::accept(Visitor& visitor) { visitor.visit(this); }
void Variable::accept(Visitor& visitor) { visitor.visit(this); }
void Assignment::accept(Visitor& visitor) { visitor.visit(this); }
void DrimStatement::accept(Visitor& visitor) { visitor.visit(this); }
void WakeStatement::accept(Visitor& visitor) { visitor.visit(this); }
void Program::accept(Visitor& visitor) { visitor.visit(this); }

// --- Implement Interpreter Logic ---

void Interpreter::visit(Number* node) {
    lastEvaluationResult = node->value;
}

void Interpreter::visit(Variable* node) {
    if (memory.find(node->name) != memory.end()) {
        lastEvaluationResult = memory[node->name];
    } else {
        std::cerr << "Runtime Error: Variable '" << node->name << "' is undefined." << std::endl;
        exit(1);
    }
}

void Interpreter::visit(Assignment* node) {
    // 1. Evaluate the right-hand side
    node->value->accept(*this);
    int result = lastEvaluationResult;

    // 2. Assign to memory
    memory[node->variableName] = result;
}

void Interpreter::visit(DrimStatement* node) {
    int inputVal;
    std::cout << "drim > ";
    std::cin >> inputVal;

    memory[node->variableName] = inputVal;
}

void Interpreter::visit(WakeStatement* node) {
    // 1. Evaluate inside parens
    node->expression->accept(*this);

    // 2. Print result
    std::cout << lastEvaluationResult << std::endl;
}

void Interpreter::visit(Program* node) {
    // Execute statements one by one
    for (const auto& stmt : node->statements) {
        stmt->accept(*this);
    }
}