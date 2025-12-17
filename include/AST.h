//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef AST_H
#define AST_H

#include "Token.h"
#include <memory>  
#include <string>

// 1. The Parents (Base Types)


// Everything that "Does something" is a Stmt (Statement)
struct Stmt {
    virtual ~Stmt() = default; // Virtual destructor is required for casting to work
};

// Everything that "Has a value" is an Expr (Expression)
struct Expr {
    virtual ~Expr() = default;
};

// ==========================================
// 2. The Data (Expressions)
// ==========================================

// Represents raw text like "hello" or "123"
struct LiteralExpr : Expr {
    std::string value;
    LiteralExpr(std::string v) : value(v) {}
};

// Represents a variable name like 'myVar'
struct VariableExpr : Expr {
    Token name;
    VariableExpr(Token n) : name(n) {}
};

// Represents Math: 1 + 2, a * b
struct BinaryExpr : Expr {
    std::shared_ptr<Expr> left;
    Token op; // The operator (+, -, *, /)
    std::shared_ptr<Expr> right;

    BinaryExpr(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r)
        : left(l), op(o), right(r) {}
};


// 3. The Commands (Statements)

// Command: drim(x)
struct InputStmt : Stmt {
    Token name; // Stores the variable name 'x'
    InputStmt(Token n) : name(n) {}
};

// Command: wake("hello")
struct PrintStmt : Stmt {
    std::shared_ptr<Expr> expression; // Points to the data "hello"
    PrintStmt(std::shared_ptr<Expr> e) : expression(e) {}
};

// Command: x = "value"
struct AssignStmt : Stmt {
    Token name; // 'x'
    std::shared_ptr<Expr> value; // Points to "value"
    AssignStmt(Token n, std::shared_ptr<Expr> v) : name(n), value(v) {}
};

#endif