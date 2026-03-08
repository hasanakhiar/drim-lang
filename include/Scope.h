#ifndef SCOPE_H
#define SCOPE_H

#include "Value.h"
#include "Token.h"
#include <map>
#include <string>
#include <memory>
#include <iostream>


//Manages variable storage and lookups for nested scopes.

class Scope {
    std::shared_ptr<Scope> enclosing; // Parent scope
    std::map<std::string, Value> values;
    // Map for user-defined func
    std::map<std::string, std::shared_ptr<FunctionStmt>> functions;

public:
    Scope() : enclosing(nullptr) {}
    Scope(std::shared_ptr<Scope> enclosing) : enclosing(enclosing) {}


    // Updates an existing variable or defines a new one in the current scope
    // if it doesn't exist in the chain.

    bool contains(const std::string& name) {
        if (values.count(name)) return true;
        if (enclosing) return enclosing->contains(name);
        return false;
    }

    void assign(const Token& name, Value value) {
        if (values.count(name.lexeme)) {
            values[name.lexeme] = value;
        } else if (enclosing && enclosing->contains(name.lexeme)) {
            enclosing->assign(name, value);
        } else {
            values[name.lexeme] = value;
        }
    }

    // Looks up a variable in the current scope or parent scopes.

    Value get(const Token& name) {
        if (values.count(name.lexeme)) {
            return values[name.lexeme];
        }

        if (enclosing) {
            return enclosing->get(name);
        }

        std::cerr << "Runtime Error: Undefined variable '" << name.lexeme << "'\n";
        exit(1);
    }

    //Define a variable strictly in the current scope (for the params)
    void define (const std::string& name, Value value) {
        values[name] = value;
    }

    void defineFunc(const std::string& name, std::shared_ptr<FunctionStmt> func) {
        functions[name] = func;
    }

    std::shared_ptr<FunctionStmt> getFunc(const std::string& name) {
        if (functions.count(name)) {
            return functions[name];
        }
        if (enclosing) {
            return enclosing->getFunc(name);
        }
        return nullptr;
    }

    std::shared_ptr<Scope> getEnclosing() { return enclosing; }
};

#endif
