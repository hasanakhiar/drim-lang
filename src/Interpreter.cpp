#include "../include/Interpreter.h"
#include "../include/Utils.h"
#include "../include/Physics.h"
#include <iostream>
#include <string>
#include <cmath>
#include <variant>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool isTruthy(const Value& v) {
    if (std::holds_alternative<int>(v)) return std::get<int>(v) != 0;
    if (std::holds_alternative<double>(v)) return std::get<double>(v) != 0.0;
    if (std::holds_alternative<std::string>(v)) return !std::get<std::string>(v).empty();
    return false; // Should not happen for these types
}
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

// Helper to extract double from Value
double getDouble(const Value& v) {
    if (std::holds_alternative<int>(v)) return (double)std::get<int>(v);
    if (std::holds_alternative<double>(v)) return std::get<double>(v);
    return 0.0;
}

Interpreter::Interpreter(){}

Value Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    
    // FUNCTION CALLS (Physics)
    if (auto call = std::dynamic_pointer_cast<CallExpr>(expr)) {
        std::string funcName;
        if (auto var = std::dynamic_pointer_cast<VariableExpr>(call->callee)) {
            funcName = var->name.lexeme;
        } else {
             std::cerr << "Runtime Error: Can only call identifiers.\n";
             exit(1);
        }

        Value args[255];
        size_t count = 0;
        for (auto arg : call->arguments) {
            if (count >= 255) {
                std::cerr << "Runtime Error: Too many arguments.\n";
                exit(1);
            }
            args[count++] = evaluate(arg);
        }
        return execPhysics(funcName, args, count);
    }

    // Literals
    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        return lit->value;
    }

    // VARIABLES
    if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        if (memory.count(var->name.lexeme)) {
            return memory[var->name.lexeme];
        }
        std::cerr << "Runtime Error: Undefined variable '" << var->name.lexeme << "'\n";
        exit(1);
    }

    // UNARY OPERATIONS (Bitwise NOT)
    if (auto una = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        Value rightVal = evaluate(una->right);

        if (std::holds_alternative<int>(rightVal)) {
            int r = std::get<int>(rightVal);
            if (una->op.type == TOKEN_BIT_NOT) return ~r;
        }
        std::cerr << "Runtime Error: Invalid unary operation\n";
        exit(1);
    }

    // CONVERSIONS (conv_dist)
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

        // --- CONVERSION LOGIC ---
        // Length
        if (mode == "in_cm") return num * 2.54;
        if (mode == "cm_in") return num / 2.54;

        // Power
        if (mode == "hp_kw") return num * 0.7457;
        if (mode == "kw_hp") return num / 0.7457;

        // Temperature
        if (mode == "f_c") return (num - 32.0) * 5.0 / 9.0;
        if (mode == "c_f") return (num * 9.0 / 5.0) + 32.0;

        // Pressure
        if (mode == "psi_bar") return num * 0.0689476;
        if (mode == "bar_psi") return num / 0.0689476;

        // Digital Storage
        if (mode == "mb_gb") return num / 1024.0;
        if (mode == "gb_mb") return num * 1024.0;

        // Energy
        if (mode == "j_cal") return num / 4184.0;
        if (mode == "cal_j") return num * 4184.0;

        // Angles
        if (mode == "deg_rad") return num * (M_PI / 180.0);
        if (mode == "rad_deg") return num * (180.0 / M_PI);

        // Mass
        if (mode == "lb_kg") return num * 0.453592;
        if (mode == "kg_lb") return num / 0.453592;

        // Currency
        if (mode == "usd_bdt") return num * 122;
        if (mode == "bdt_usd") return num / 122;

        if (mode == "usd_eur") return num * 0.92;
        if (mode == "eur_usd") return num / 0.92;

        // Speed
        if (mode == "mph_kmph") return num * 1.60934;
        if (mode == "kmph_mph") return num / 1.60934;

        // Torque
        if (mode == "nm_ftlb") return num * 0.737562;
        if (mode == "ftlb_nm") return num / 0.737562;

        // G-Force
        if (mode == "g_ms2") return num * 9.80665;
        if (mode == "ms2_g") return num / 9.80665;

        std::cerr << "Runtime Error: Unknown conversion mode '" << mode << "'\n";
        exit(1);
    }

    // BINARY OPERATIONS (Math + Logic)
    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        Value leftVal = evaluate(bin->left);
        Value rightVal = evaluate(bin->right);

        // --- A. LOGIC OPERATORS (and, or) ---
        if (bin->op.type == KW_AND) {
            return (isTruthy(leftVal) && isTruthy(rightVal)) ? 1 : 0;
        }
        if (bin->op.type == KW_OR) {
            return (isTruthy(leftVal) || isTruthy(rightVal)) ? 1 : 0;
        }

        // Numeric operations
        bool leftIsNum = std::holds_alternative<int>(leftVal) || std::holds_alternative<double>(leftVal); 
        bool rightIsNum = std::holds_alternative<int>(rightVal) || std::holds_alternative<double>(rightVal);
        
        // Convert to doubles for easy comparison
        double l = 0.0, r = 0.0;
        if (leftIsNum) l = std::holds_alternative<int>(leftVal) ? std::get<int>(leftVal) : std::get<double>(leftVal);
        if (rightIsNum) r = std::holds_alternative<int>(rightVal) ? std::get<int>(rightVal) : std::get<double>(rightVal);

        // --- B. COMPARISONS (<, >, ==, !=) ---
        if (leftIsNum && rightIsNum) {
            switch (bin->op.type) {
                case TOKEN_LESS:          return (l < r) ? 1 : 0;
                case TOKEN_GREATER:       return (l > r) ? 1 : 0;
                case TOKEN_LESS_EQUAL:    return (l <= r) ? 1 : 0;
                case TOKEN_GREATER_EQUAL: return (l >= r) ? 1 : 0;
                case TOKEN_EQUAL_EQUAL:   return (l == r) ? 1 : 0;
                case TOKEN_BANG_EQUAL:    return (l != r) ? 1 : 0;
            }
        }

        // MATH OPERATIONS (+, -, *, /) ---
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
        if (!cmd) continue;

        // IF STATEMENT
        if (auto ifStmt = std::dynamic_pointer_cast<IfStmt>(cmd)) {
            Value cond = evaluate(ifStmt->condition);
            if (isTruthy(cond)) {
                // Execute 'Then' (It's usually a BlockStmt)
                std::vector<std::shared_ptr<Stmt>> wrapper = { ifStmt->thenBranch };
                interpret(wrapper); // Recursive call
            } else if (ifStmt->elseBranch != nullptr) {
                // Execute 'Else'
                std::vector<std::shared_ptr<Stmt>> wrapper = { ifStmt->elseBranch };
                interpret(wrapper);
            }
        }
        // BLOCK STATEMENT { ... }
        else if (auto block = std::dynamic_pointer_cast<BlockStmt>(cmd)) {
            interpret(block->statements); // Recursively execute the list inside
        }
        // Input
        else if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            //std::cout << "drim input " << input->name.lexeme << ": ";
            if (std::getline(std::cin, userText)) {
                memory[input->name.lexeme] = parseInput(userText);
            }
        }
        // Assignment
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            Value val = evaluate(assign->value);
            memory[assign->name.lexeme] = val;
        }
        // PRINT (wake)
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            Value output = evaluate(print->expression);
            printValue(output);
            std::cout << "\n";
        }
        //TYPE CHECK
        else if (auto typeStmt = std::dynamic_pointer_cast<TypeStmt>(cmd)) {
            Value valToCheck = evaluate(typeStmt->expression);

            if (std::holds_alternative<int>(valToCheck)) std::cout << "<type 'int'>\n";
            else if (std::holds_alternative<double>(valToCheck)) std::cout << "<type 'float'>\n";
            else if (std::holds_alternative<std::string>(valToCheck)) std::cout << "<type 'string'>\n";   
        }
    }
}
