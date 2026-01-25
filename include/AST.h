//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#ifndef AST_H
#define AST_H

#include "Token.h"
#include "Value.h"
#include <memory>  
#include <vector>
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

// Represents Math: 1 + 2, a * b
struct BinaryExpr : Expr {
    std::shared_ptr<Expr> left;
    Token op; // The operator (+, -, *, /)
    std::shared_ptr<Expr> right;

    BinaryExpr(std::shared_ptr<Expr> l, Token o, std::shared_ptr<Expr> r)
        : left(l), op(o), right(r) {}
};

struct UnaryExpr : Expr {
    Token op;
    std::shared_ptr<Expr> right;
    UnaryExpr(Token o, std::shared_ptr<Expr> r) : op(o), right(r) {}
};

struct CallExpr : Expr {
    std::shared_ptr<Expr> callee; // The function being called (usually a VariableExpr)
    Token paren; // The closing parenthesis ')' (for error reporting)
    std::vector<std::shared_ptr<Expr>> arguments;

    CallExpr(std::shared_ptr<Expr> c, Token p, std::vector<std::shared_ptr<Expr>> args)
        : callee(c), paren(p), arguments(args) {}
};

struct ConvertExpr : Expr {
    std::shared_ptr<Expr> value;
    std::shared_ptr<Expr> mode;
    ConvertExpr(std::shared_ptr<Expr> v, std::shared_ptr<Expr> m) : value(v), mode(m) {}
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