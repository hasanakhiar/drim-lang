#include "../include/Interpreter.h"
#include "../include/Utils.h"
#include "../include/Physics.h"
#include "../include/DS.h"
#include <iostream>
#include <string>
#include <cmath>
#include <variant>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool isTruthy(const Value& v) {
    if (auto b = std::get_if<bool>(&v.data)) return *b;
    if (auto i = std::get_if<long long>(&v.data)) return *i != 0; 
    if (auto d = std::get_if<long double>(&v.data)) return *d != 0.0;
    if (auto s = std::get_if<std::string>(&v.data)) return !s->empty();
    return false;
}

Value parseInput(std::string text) {
    if (text.empty()) return text;

    bool isNumber = true;
    bool hasDot = false;

    for (char c : text) {
        if (!isdigit(c) && c != '-') {
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
            if (hasDot) return std::stold(text);
            return std::stoll(text);
        } catch (...) {
            return text;
        }
    }
    return text;
}

long double getLongDouble(const Value& v) {
    if (auto i = std::get_if<long long>(&v.data)) return (long double)*i;
    if (auto d = std::get_if<long double>(&v.data)) return *d;
    return 0.0;
}

Interpreter::Interpreter(){}

Value Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    
    // FUNCTION CALLS (Physics & DS)
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
        
        // Check if it's a Stack/Queue function
        if (funcName.find("stack_") == 0 || funcName.find("queue_") == 0) {
            return execDS(funcName, args, count);
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

        if (auto r = std::get_if<long long>(&rightVal.data)) {
            if (una->op.type == TOKEN_BIT_NOT) return ~(*r);
        }
        std::cerr << "Runtime Error: Invalid unary operation\n";
        exit(1);
    }

    // CONVERSIONS (conv_dist)
    if (auto conv = std::dynamic_pointer_cast<ConvertExpr>(expr)) {
        Value val = evaluate(conv->value);
        Value modeVal = evaluate(conv->mode);

        auto modePtr = std::get_if<std::string>(&modeVal.data);
        if (!modePtr) {
            std::cerr << "Runtime Error: Conversion mode must be a string\n";
            exit(1);
        }

        std::string mode = *modePtr;
        long double num = 0.0;

        // Get the number as long double
        if (auto i = std::get_if<long long>(&val.data)) num = (long double)*i;
        else if (auto d = std::get_if<long double>(&val.data)) num = *d;
        else {
             std::cerr << "Runtime Error: Cannot convert non-number\n";
             exit(1);
        }

        // --- CONVERSION LOGIC ---
        if (mode == "in_cm") return num * 2.54L;
        if (mode == "cm_in") return num / 2.54L;
        if (mode == "hp_kw") return num * 0.7457L;
        if (mode == "kw_hp") return num / 0.7457L;
        if (mode == "f_c") return (num - 32.0L) * 5.0L / 9.0L;
        if (mode == "c_f") return (num * 9.0L / 5.0L) + 32.0L;
        if (mode == "psi_bar") return num * 0.0689476L;
        if (mode == "bar_psi") return num / 0.0689476L;
        if (mode == "mb_gb") return num / 1024.0L;
        if (mode == "gb_mb") return num * 1024.0L;
        if (mode == "j_cal") return num / 4184.0L;
        if (mode == "cal_j") return num * 4184.0L;
        if (mode == "deg_rad") return num * (M_PI / 180.0L);
        if (mode == "rad_deg") return num * (180.0L / M_PI);
        if (mode == "lb_kg") return num * 0.453592L;
        if (mode == "kg_lb") return num / 0.453592L;
        if (mode == "usd_bdt") return num * 122.0L;
        if (mode == "bdt_usd") return num / 122.0L;
        if (mode == "usd_eur") return num * 0.92L;
        if (mode == "eur_usd") return num / 0.92L;
        if (mode == "mph_kmph") return num * 1.60934L;
        if (mode == "kmph_mph") return num / 1.60934L;
        if (mode == "nm_ftlb") return num * 0.737562L;
        if (mode == "ftlb_nm") return num / 0.737562L;
        if (mode == "g_ms2") return num * 9.80665L;
        if (mode == "ms2_g") return num / 9.80665L;

        std::cerr << "Runtime Error: Unknown conversion mode '" << mode << "'\n";
        exit(1);
    }

    // BINARY OPERATIONS (Math + Logic)
    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        Value leftVal = evaluate(bin->left);
        Value rightVal = evaluate(bin->right);

        if (bin->op.type == KW_AND) return (isTruthy(leftVal) && isTruthy(rightVal));
        if (bin->op.type == KW_OR) return (isTruthy(leftVal) || isTruthy(rightVal));

        bool leftIsNum = std::holds_alternative<long long>(leftVal.data) || std::holds_alternative<long double>(leftVal.data); 
        bool rightIsNum = std::holds_alternative<long long>(rightVal.data) || std::holds_alternative<long double>(rightVal.data);
        
        long double l = 0.0, r = 0.0;
        if (leftIsNum) l = getLongDouble(leftVal);
        if (rightIsNum) r = getLongDouble(rightVal);

        if (leftIsNum && rightIsNum) {
            switch (bin->op.type) {
                case TOKEN_LESS:          return (l < r);
                case TOKEN_GREATER:       return (l > r);
                case TOKEN_LESS_EQUAL:    return (l <= r);
                case TOKEN_GREATER_EQUAL: return (l >= r);
                case TOKEN_EQUAL_EQUAL:   return (l == r);
                case TOKEN_BANG_EQUAL:    return (l != r);
            }
        }

        if (bin->op.type == TOKEN_EQUAL_EQUAL) return leftVal == rightVal;
        if (bin->op.type == TOKEN_BANG_EQUAL) return leftVal != rightVal;

        if (leftIsNum && rightIsNum) {
            bool useDouble = std::holds_alternative<long double>(leftVal.data) || std::holds_alternative<long double>(rightVal.data);
            if (bin->op.type == TOKEN_PLUS) {
                if(useDouble) return l + r;
                return (long long)l + (long long)r;
            }
            if (bin->op.type == TOKEN_MINUS) {
                if(useDouble) return l - r;
                return (long long)l - (long long)r;
            }
            if (bin->op.type == TOKEN_STAR) {
                if(useDouble) return l * r;
                return (long long)l * (long long)r;
            }
            if (bin->op.type == TOKEN_SLASH) {
                if (r == 0) { std::cerr << "Runtime Error: Division by zero\n"; exit(1); }
                if(useDouble) return l / r;
                return (long long)(l / r);
            }
            if (bin->op.type == TOKEN_POW) return powl(l, r);

            if (!useDouble) {
                long long li = (long long)l;
                long long ri = (long long)r;
                if (bin->op.type == TOKEN_BIT_AND) return li & ri;
                if (bin->op.type == TOKEN_BIT_OR) return li | ri;
                if (bin->op.type == TOKEN_LSHIFT) return li << ri;
                if (bin->op.type == TOKEN_RSHIFT) return li >> ri;
            }
        }

        if (bin->op.type == TOKEN_PLUS) {
            auto toStr = [](const Value& v) -> std::string {
                if (auto i = std::get_if<long long>(&v.data)) return std::to_string(*i);
                if (auto d = std::get_if<long double>(&v.data)) return std::to_string(*d);
                if (auto b = std::get_if<bool>(&v.data)) return *b ? "true" : "false";        
                if (auto s = std::get_if<std::string>(&v.data)) return *s;
                return "<collection>";
            };
            return toStr(leftVal) + toStr(rightVal);
        }

        std::cerr << "Runtime Error: Invalid operation\n";
        exit(1);
    }

    return 0LL; 
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {
        if (!cmd) continue;
        if (auto ifStmt = std::dynamic_pointer_cast<IfStmt>(cmd)) {
            Value cond = evaluate(ifStmt->condition);
            if (isTruthy(cond)) {
                interpret({ ifStmt->thenBranch });
            } else if (ifStmt->elseBranch != nullptr) {
                interpret({ ifStmt->elseBranch });
            }
        }
        else if (auto block = std::dynamic_pointer_cast<BlockStmt>(cmd)) {
            interpret(block->statements);
        }
        else if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            if (std::getline(std::cin, userText)) memory[input->name.lexeme] = parseInput(userText);
        }
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            memory[assign->name.lexeme] = evaluate(assign->value);
        }
        else if (auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(cmd)) {
            evaluate(exprStmt->expression);
        }
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            printValue(evaluate(print->expression));
            std::cout << "\n";
        }
        else if (auto typeStmt = std::dynamic_pointer_cast<TypeStmt>(cmd)) {
            Value valToCheck = evaluate(typeStmt->expression);
            if (std::holds_alternative<long long>(valToCheck.data)) std::cout << "<type 'int'>\n";
            else if (std::holds_alternative<long double>(valToCheck.data)) std::cout << "<type 'float'>\n";
            else if (std::holds_alternative<std::string>(valToCheck.data)) std::cout << "<type 'string'>\n";   
            else if (std::holds_alternative<bool>(valToCheck.data)) std::cout << "<type 'bool'>\n";
            else std::cout << "<type 'collection'>\n";
        }
    }
}