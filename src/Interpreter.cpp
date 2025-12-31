#include "../include/Interpreter.h"
#include <iostream>
#include <string>
#include <cmath>


// Helper to check if a string is a number
bool isNumber(const std::string& s) {
    if(s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

std::string Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    // 1. Literal (10, "hello")
    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        return lit->value;
    }

    // 2. Variable (x)
    if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        if (memory.count(var->name.lexeme)) {
            return memory[var->name.lexeme];
        }
        std::cerr << "Runtime Error: Undefined variable '" << var->name.lexeme << "'\n";
        exit(1);
    }

    // 3. Unary Expr (~5)
    if (auto una = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        std::string rightVal = evaluate(una->right);
        if (isNumber(rightVal)) {
             int r = std::stoi(rightVal);
             if (una->op.type == TOKEN_BIT_NOT) return std::to_string(~r);
        }
        std::cerr << "Runtime Error: Invalid unary operation\n";
        exit(1);
    }

    // 4. Binary Math (1 + 2)
    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        std::string leftVal = evaluate(bin->left);
        std::string rightVal = evaluate(bin->right);

        // Check if both are numbers for Math
        if (isNumber(leftVal) && isNumber(rightVal)) {
            int l = std::stoi(leftVal);
            int r = std::stoi(rightVal);

            if (bin->op.type == TOKEN_PLUS) return std::to_string(l + r);
            if (bin->op.type == TOKEN_MINUS) return std::to_string(l - r);
            if (bin->op.type == TOKEN_STAR) return std::to_string(l * r);
            if (bin->op.type == TOKEN_SLASH) {
                if(r == 0) { std::cerr << "Runtime Error: Division by zero\n"; exit(1); }
                return std::to_string(l / r);
            }
            if (bin->op.type == TOKEN_POW) return std::to_string((int)pow(l, r));

            // Bitwise
            if (bin->op.type == TOKEN_BIT_AND) return std::to_string(l & r);
            if (bin->op.type == TOKEN_BIT_OR) return std::to_string(l | r);
            if (bin->op.type == TOKEN_LSHIFT) return std::to_string(l << r);
            if (bin->op.type == TOKEN_RSHIFT) return std::to_string(l >> r);
        }

        // String Concatenation (if using +)
        if (bin->op.type == TOKEN_PLUS) {
            return leftVal + rightVal;
        }

        std::cerr << "Runtime Error: Invalid operation on non-numbers\n";
        exit(1);
    }

    return "";
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {

        if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            // std::cout << "drim input " << input->name.lexeme << ": ";
            if (std::getline(std::cin, userText)) {
                memory[input->name.lexeme] = userText;
            }
        }
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            // Use the new evaluate function!
            std::string val = evaluate(assign->value);
            memory[assign->name.lexeme] = val;
        }
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            // Use the new evaluate function!
            std::string output = evaluate(print->expression);
            std::cout << output << "\n";
        }
    }
}