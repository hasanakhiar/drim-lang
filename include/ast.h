//
// Created by SMTEmon on 12/14/2025.
//

#ifndef DRIM_LANG_AST_H
#define DRIM_LANG_AST_H

#include <string>
#include <vector>
#include <memory>

class Visitor;

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(Visitor& visitor) = 0;
};

// Expressions that return a value
class Expression : public ASTNode {};

class Number : public Expression {
public:
    int value;
    Number(int v) : value(v) {}
    void accept(Visitor& visitor) override;
};

class Variable : public Expression {
public:
    std::string name;
    Variable(std::string n) : name(n) {}
    void accept(Visitor& visitor) override;
};

// Statements that perform an action
class Statement : public ASTNode {};

class Assignment : public Statement {
public:
    std::string variableName;
    std::unique_ptr<Expression> value; // The right hand side

    Assignment(std::string name, std::unique_ptr<Expression> val)
        : variableName(name), value(std::move(val)) {}
    void accept(Visitor& visitor) override;
};

class DrimStatement : public Statement {
public:
    std::string variableName;
    DrimStatement(std::string name) : variableName(name) {}
    void accept(Visitor& visitor) override;
};

class WakeStatement : public Statement {
public:
    std::unique_ptr<Expression> expression;
    WakeStatement(std::unique_ptr<Expression> expr) : expression(std::move(expr)) {}
    void accept(Visitor& visitor) override;
};

// Root
class Program : public ASTNode {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    void accept(Visitor& visitor) override;
};

// Interface
class Visitor {
public:
    virtual void visit(Number* node) = 0;
    virtual void visit(Variable* node) = 0;
    virtual void visit(Assignment* node) = 0;
    virtual void visit(DrimStatement* node) = 0;
    virtual void visit(WakeStatement* node) = 0;
    virtual void visit(Program* node) = 0;
};

#endif //DRIM_LANG_AST_H