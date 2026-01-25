#include "../include/Interpreter.h"
#include "../include/Utils.h"
#include <iostream>
#include <string>
#include <cmath>
#include <variant>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Parse string inputs into int or double if possible
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
            // fallback to string if conversion fails
            return text;
        }
    }
    return text;
}

Value Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        return lit->value;
    }

    if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        if (memory.count(var->name.lexeme)) {
            return memory[var->name.lexeme];
        }
        std::cerr << "Runtime Error: Undefined variable '" << var->name.lexeme << "'\n";
        exit(1);
    }

    if (auto una = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        Value rightVal = evaluate(una->right);

        if (std::holds_alternative<int>(rightVal)) {
            int r = std::get<int>(rightVal);
            if (una->op.type == TOKEN_BIT_NOT) return ~r;
        }
        std::cerr << "Runtime Error: Invalid unary operation\n";
        exit(1);
    }

    if (auto conv = std::dynamic_pointer_cast<ConvertExpr>(expr)) {
        Value val = evaluate(conv->value);
        Value modeVal = evaluate(conv->mode);

        if (!std::holds_alternative<std::string>(modeVal)) {
            std::cerr << "Runtime Error: Conversion mode must be a string\n";
            exit(1);
        }

        std::string mode = std::get<std::string>(modeVal);
        double num = 0.0;

        // Get the number as double
        if (std::holds_alternative<int>(val)) num = (double)std::get<int>(val);
        else if (std::holds_alternative<double>(val)) num = std::get<double>(val);
        else {
             std::cerr << "Runtime Error: Cannot convert non-number\n";
             exit(1);
        }

        // Length
        if (mode == "in_cm") return num * 2.54;
        if (mode == "cm_in") return num / 2.54;

        // Power
        if (mode == "hp_kw") return num * 0.7457;
        if (mode == "kw_hp") return num / 0.7457;

        // Temperature
        if (mode == "f_c") return (num - 32.0) * 5.0 / 9.0;
        if (mode == "c_f") return (num * 9.0 / 5.0) + 32.0;

        std::cerr << "Runtime Error: Unknown conversion mode '" << mode << "'\n";
        exit(1);
    }

    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        Value leftVal = evaluate(bin->left);
        Value rightVal = evaluate(bin->right);

        // Numeric operations
        bool leftIsNum = std::holds_alternative<int>(leftVal) || std::holds_alternative<double>(leftVal); 
        bool rightIsNum = std::holds_alternative<int>(rightVal) || std::holds_alternative<double>(rightVal);

        if (leftIsNum && rightIsNum) {
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
                return (int)(l / r);
            }
            if (bin->op.type == TOKEN_POW) {
                return pow(l, r);
            }

            // Bitwise operations (integers only)
            if (!useDouble) {
                int li = (int)l;
                int ri = (int)r;
                if (bin->op.type == TOKEN_BIT_AND) return li & ri;
                if (bin->op.type == TOKEN_BIT_OR) return li | ri;
                if (bin->op.type == TOKEN_LSHIFT) return li << ri;
                if (bin->op.type == TOKEN_RSHIFT) return li >> ri;
            }
        }

        // String concatenation
        if (bin->op.type == TOKEN_PLUS) {
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

    return 0;
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {

        if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            std::cout << "drim input " << input->name.lexeme << ": ";
            if (std::getline(std::cin, userText)) {
                memory[input->name.lexeme] = parseInput(userText);
            }
        }

        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            Value val = evaluate(assign->value);
            memory[assign->name.lexeme] = val;
        }

        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            Value output = evaluate(print->expression);
            printValue(output);
            std::cout << "\n";
        }

        else if (auto typeStmt = std::dynamic_pointer_cast<TypeStmt>(cmd)) {
            Value valToCheck = evaluate(typeStmt->expression);

            if (std::holds_alternative<int>(valToCheck)) std::cout << "<type 'int'>\n";
            else if (std::holds_alternative<double>(valToCheck)) std::cout << "<type 'float'>\n";
            else if (std::holds_alternative<std::string>(valToCheck)) std::cout << "<type 'string'>\n";   
        }
    }
}
