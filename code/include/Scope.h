#ifndef SCOPE_H
#define SCOPE_H

#include "Value.h"
#include "Token.h"
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <vector>

struct FunctionStmt;

//Manages variable storage and lookups for nested scopes.

class Scope {
    std::shared_ptr<Scope> enclosing; // Parent scope
    std::map<std::string, Value> values;
    std::map<std::string, std::vector<Value>> arrays;
    std::map<std::string, std::string> arrayElementTypes;
    // Map for user-defined func
    std::map<std::string, std::shared_ptr<FunctionStmt>> functions;

    std::string inferValueTypeName(const Value& value) {
        if (std::holds_alternative<long long>(value.data)) return "int";
        if (std::holds_alternative<long double>(value.data)) return "float";
        if (std::holds_alternative<std::string>(value.data)) return "string";
        if (std::holds_alternative<bool>(value.data)) return "bool";
        return "unknown";
    }

    Scope* findArrayOwner(const std::string& name) {
        if (arrays.count(name)) return this;
        if (enclosing) return enclosing->findArrayOwner(name);
        return nullptr;
    }

    Scope* findValueOwner(const std::string& name) {
        if (values.count(name)) return this;
        if (enclosing) return enclosing->findValueOwner(name);
        return nullptr;
    }

    Scope* rootScope() {
        Scope* current = this;
        while (current->enclosing) {
            current = current->enclosing.get();
        }
        return current;
    }

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
        if (hasArray(name.lexeme)) {
            std::cerr << "Runtime Error: '" << name.lexeme << "' is an array, cannot assign scalar value\n";
            exit(1);
        }
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

    bool hasArray(const std::string& name) {
        if (arrays.count(name)) return true;
        if (enclosing) return enclosing->hasArray(name);
        return false;
    }

    void declareArray(const Token& name) {
        if (values.count(name.lexeme)) {
            std::cerr << "Runtime Error: '" << name.lexeme << "' already exists as a variable in current scope\n";
            exit(1);
        }
        if (!arrays.count(name.lexeme)) {
            arrays[name.lexeme] = {};
            arrayElementTypes[name.lexeme] = "";
        }
    }

    void assignArray(const Token& name, const std::vector<Value>& elements) {
        if (findValueOwner(name.lexeme)) {
            std::cerr << "Runtime Error: '" << name.lexeme << "' already exists as a variable\n";
            exit(1);
        }

        std::string inferred = "";
        for (const auto& element : elements) {
            std::string currentType = inferValueTypeName(element);
            if (inferred.empty()) {
                inferred = currentType;
            } else if (currentType != inferred) {
                std::cerr << "Runtime Error: Mixed array literal types for '" << name.lexeme
                          << "'. Expected " << inferred << " but got " << currentType << "\n";
                exit(1);
            }
        }

        Scope* owner = findArrayOwner(name.lexeme);
        if (!owner) owner = this;
        owner->arrays[name.lexeme] = elements;
        owner->arrayElementTypes[name.lexeme] = inferred;
    }

    void assignArrayElement(const Token& name, int index, Value value) {
        if (index < 0) {
            std::cerr << "Runtime Error: Array index cannot be negative for '" << name.lexeme << "'\n";
            exit(1);
        }

        if (findValueOwner(name.lexeme)) {
            std::cerr << "Runtime Error: '" << name.lexeme << "' is a variable, not an array\n";
            exit(1);
        }

        Scope* owner = findArrayOwner(name.lexeme);
        if (!owner) {
            owner = rootScope();
            owner->arrays[name.lexeme] = {};
            owner->arrayElementTypes[name.lexeme] = "";
        }

        std::string currentType = inferValueTypeName(value);
        std::string& expectedType = owner->arrayElementTypes[name.lexeme];
        if (expectedType.empty()) {
            expectedType = currentType;
        } else if (expectedType != currentType) {
            std::cerr << "Runtime Error: Array value type is '" << currentType
                      << "', must be matched with '" << expectedType << "' for array '"
                      << name.lexeme << "'\n";
            exit(1);
        }

        std::vector<Value>& arr = owner->arrays[name.lexeme];
        if (index >= static_cast<int>(arr.size())) {
            arr.resize(index + 1, 0LL);
        }
        arr[index] = value;
    }

    Value getArrayElement(const Token& name, int index) {
        if (index < 0) {
            std::cerr << "Runtime Error: Array index cannot be negative for '" << name.lexeme << "'\n";
            exit(1);
        }

        Scope* owner = findArrayOwner(name.lexeme);
        if (!owner) {
            std::cerr << "Runtime Error: Undefined array '" << name.lexeme << "'\n";
            exit(1);
        }

        std::vector<Value>& arr = owner->arrays[name.lexeme];
        if (index >= static_cast<int>(arr.size())) {
            std::cerr << "Runtime Error: Array index out of bounds for '" << name.lexeme << "'\n";
            exit(1);
        }

        return arr[index];
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
