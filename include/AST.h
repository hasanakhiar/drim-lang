//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef AST_H
#define AST_H

#include "Token.h"
#include "Value.h"
#include <memory>  
#include <string>

// Everything that "Does something" is a Stmt (Statement)
struct Stmt {
    virtual ~Stmt() = default; // Virtual destructor is required for casting to work
};

// Everything that "Has a value" is an Expr (Expression)
struct Expr {
    virtual ~Expr() = default;
};

// 2. The Data (Expressions) Represents raw text like "hello" or "123"
struct LiteralExpr : Expr {
    Value value; // Can now hold int, double, or string
    LiteralExpr(Value v) : value(v) {}
};

// Represents a variable name like 'myVar'
struct VariableExpr : Expr {
    Token name;
    VariableExpr(Token n) : name(n) {}
};


// 3. The Commands (Statements) -- Command: drim(x)
struct InputStmt : Stmt {
    Token name;
    InputStmt(Token n) : name(n) {}
};

// Command: wake("hello")
struct PrintStmt : Stmt {
    std::shared_ptr<Expr> expression;
    PrintStmt(std::shared_ptr<Expr> e) : expression(e) {}
};

// Command: x = "value"
struct AssignStmt : Stmt {
    Token name;
    std::shared_ptr<Expr> value;
    AssignStmt(Token n, std::shared_ptr<Expr> v) : name(n), value(v) {}
};
struct TypeStmt : Stmt {
    std::shared_ptr<Expr> expression; // (variable or literal currently being checked)
    TypeStmt(std::shared_ptr<Expr> e) : expression(e) {}
};

#endif