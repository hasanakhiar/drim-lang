#include "../include/Interpreter.h"
#include "../include/Utils.h"
#include <iostream>
#include <string>
#include <cmath>
#include <variant>

// --- HELPER: Try to parse string to Number ---
Value parseInput(std::string text) {
    if (text.empty()) return text;

    bool isNumber = true;
    bool hasDot = false;

    for (char c : text) {
        if (!isdigit(c)) {
            if (c == '.' && !hasDot) {
                hasDot = true;
            } else {
                isNumber = false;
                break;
            }
        }
    }

    if (isNumber) {
        try {
            if (hasDot) return std::stod(text);
            return std::stoi(text);
        } catch (...) {
            return text;
        }
    }
    return text;
}

// --- EVALUATE EXPRESSIONS ---
Value Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    // 1. Literal
    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        return lit->value;
    }

    // 2. Variable
    if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        if (memory.count(var->name.lexeme)) {
            return memory[var->name.lexeme];
        }
        std::cerr << "Runtime Error: Undefined variable '" << var->name.lexeme << "'\n";
        exit(1);
    }

    // 3. Unary Expr (~5)
    if (auto una = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        Value rightVal = evaluate(una->right);
        
        if (std::holds_alternative<int>(rightVal)) {
            int r = std::get<int>(rightVal);
            if (una->op.type == TOKEN_BIT_NOT) return ~r;
        }
        std::cerr << "Runtime Error: Invalid unary operation\n";
        exit(1);
    }

    // 4. Binary Math (1 + 2)
    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        Value leftVal = evaluate(bin->left);
        Value rightVal = evaluate(bin->right);

        // --- MATH OPERATIONS ---
        
        // CHECK: Are both numbers? (Int or Double)
        bool leftIsNum = std::holds_alternative<int>(leftVal) || std::holds_alternative<double>(leftVal);
        bool rightIsNum = std::holds_alternative<int>(rightVal) || std::holds_alternative<double>(rightVal);

        if (leftIsNum && rightIsNum) {
            // Promote to double if one is double
            bool useDouble = std::holds_alternative<double>(leftVal) || std::holds_alternative<double>(rightVal);
            
            double l = std::holds_alternative<int>(leftVal) ? std::get<int>(leftVal) : std::get<double>(leftVal);
            double r = std::holds_alternative<int>(rightVal) ? std::get<int>(rightVal) : std::get<double>(rightVal);

            if (bin->op.type == TOKEN_PLUS) {
                if(useDouble) return l + r;
                return (int)l + (int)r;
            }
            if (bin->op.type == TOKEN_MINUS) {
                if(useDouble) return l - r;
                return (int)l - (int)r;
            }
            if (bin->op.type == TOKEN_STAR) {
                if(useDouble) return l * r;
                return (int)l * (int)r;
            }
            if (bin->op.type == TOKEN_SLASH) {
                if (r == 0) { std::cerr << "Runtime Error: Division by zero\n"; exit(1); }
                if(useDouble) return l / r;
                return (int)(l / r); // Integer division
            }
            if (bin->op.type == TOKEN_POW) {
                return (int)pow(l, r); // Pow returns double usually, casting to int for now based on previous logic
            }

            // --- BITWISE (Must be Ints) ---
            if (!useDouble) {
                int li = (int)l;
                int ri = (int)r;
                if (bin->op.type == TOKEN_BIT_AND) return li & ri;
                if (bin->op.type == TOKEN_BIT_OR) return li | ri;
                if (bin->op.type == TOKEN_LSHIFT) return li << ri;
                if (bin->op.type == TOKEN_RSHIFT) return li >> ri;
            }
        }

        // --- STRING CONCATENATION ---
        if (bin->op.type == TOKEN_PLUS) {
            // Helper lambda to to_string
            auto toStr = [](const Value& v) -> std::string {
                if (std::holds_alternative<int>(v)) return std::to_string(std::get<int>(v));
                if (std::holds_alternative<double>(v)) return std::to_string(std::get<double>(v));
                return std::get<std::string>(v);
            };
            return toStr(leftVal) + toStr(rightVal);
        }

        std::cerr << "Runtime Error: Invalid operation\n";
        exit(1);
    }

    return 0; // Default
}


// --- MAIN LOOP ---
void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {

        // INPUT
        if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            std::cout << "drim input " << input->name.lexeme << ": ";
            if (std::getline(std::cin, userText)) {
                memory[input->name.lexeme] = parseInput(userText);
            }
        }

        // ASSIGNMENT
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            Value val = evaluate(assign->value);
            memory[assign->name.lexeme] = val;
        }

        // PRINT
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            Value output = evaluate(print->expression);
            printValue(output);
            std::cout << "\n";
        }

        // TYPE CHECK
        else if (auto typeStmt = std::dynamic_pointer_cast<TypeStmt>(cmd)) {
            Value valToCheck = evaluate(typeStmt->expression);

            if (std::holds_alternative<int>(valToCheck)) std::cout << "<type 'int'>\n";
            else if (std::holds_alternative<double>(valToCheck)) std::cout << "<type 'float'>\n";
            else if (std::holds_alternative<std::string>(valToCheck)) std::cout << "<type 'string'>\n";
        }
    }
}
