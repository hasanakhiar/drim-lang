#include "../include/Interpreter.h"
#include "../include/Physics.h"
#include "../include/DS.h"
#include "../include/Signal.h"
#include <iostream>
#include <string>
#include <cmath>
#include <variant>

#ifndef M_PI
#define M_PI 3.14159265358979323846L
#endif

bool isTruthy(const Value& v) {
    if (auto b = std::get_if<bool>(&v.data)) return *b;
    if (auto i = std::get_if<long long>(&v.data)) return *i != 0;
    if (auto d = std::get_if<long double>(&v.data)) return *d != 0.0L;
    if (auto s = std::get_if<std::string>(&v.data)) return !s->empty();
    return false;
}

// Stricter version of parseInput as discussed
Value parseInput(std::string text) {
    if (text.empty()) return text;

    size_t i = 0;
    // Check for a leading negative sign, but ensure there's a character after it
    if (text[0] == '-' && text.size() > 1) i = 1;

    bool isNumber = true;
    bool hasDot = false;
    bool hasDigit = false;

    for (; i < text.size(); ++i) {
        if (isdigit(text[i])) {
            hasDigit = true;
        } else if (text[i] == '.' && !hasDot) {
            hasDot = true;
        } else {
            isNumber = false;
            break;
        }
    }

    if (isNumber && hasDigit) {
        try {
            if (hasDot) return std::stold(text); // 64-bit float
            return std::stoll(text);            // 64-bit integer
        } catch (...) {
            return text;
        }
    }
    return text;
}

long double getLongDouble(const Value& v) {
    if (auto i = std::get_if<long long>(&v.data)) return (long double)*i;
    if (auto d = std::get_if<long double>(&v.data)) return *d;
    return 0.0L;
}

// Helper to convert Value to String
std::string valToString(const Value& v) {
    if (auto i = std::get_if<long long>(&v.data)) return std::to_string(*i);
    if (auto d = std::get_if<long double>(&v.data)) return std::to_string(*d);
    if (auto b = std::get_if<bool>(&v.data)) return *b ? "true" : "false";
    if (auto s = std::get_if<std::string>(&v.data)) return *s;
    return "<collection>";
}

Interpreter::Interpreter() {
    scope = std::make_shared<Scope>();
}

Value Interpreter::evaluate(std::shared_ptr<Expr> expr) {

    if (auto access = std::dynamic_pointer_cast<ArrayAccessExpr>(expr)) {
        Value indexVal = evaluate(access->index);
        long double idxRaw = getLongDouble(indexVal);
        int index = (int)idxRaw;
        return scope->getArrayElement(access->name, index);
    }

    if (auto arrLiteral = std::dynamic_pointer_cast<ArrayLiteralExpr>(expr)) {
        std::cerr << "Runtime Error: Array literal is only valid in assignment\n";
        exit(1);
    }

    // FUNCTION CALLS
    if (auto call = std::dynamic_pointer_cast<CallExpr>(expr)) {
        std::string funcName;
        if (auto var = std::dynamic_pointer_cast<VariableExpr>(call->callee)) {
            funcName = var->name.lexeme;
        } else {
             std::cerr << "Runtime Error: Can only call identifiers.\n";
             exit(1);
        }

        std::vector<Value> argsValues;
        for (auto arg : call->arguments) {
            argsValues.push_back(evaluate(arg));
        }

        std::shared_ptr<FunctionStmt> func = scope->getFunc(funcName);

        if (func) {
            if (argsValues.size() != func->params.size()) {
                std::cerr << "Runtime Error: Expected " << func->params.size() << " arguments but got " << argsValues.size() << ".\n";
                exit(1);
            }

            auto functionScope = std::make_shared<Scope>(scope);
            for (size_t i = 0; i < argsValues.size(); i++) {
                functionScope->define(func->params[i].lexeme, argsValues[i]);
            }

            auto previousScope = scope;
            this->scope = functionScope;

            Value result = 0LL;
            try {
                interpret(func->body);
            } catch (ReturnValue& rv) {
                result = rv.value;
            }
            this->scope = previousScope;
            return result;
        }

        Value args[255];
        size_t count = 0;
        for (auto arg : argsValues) {
            if (count >= 255) {
                std::cerr << "Runtime Error: Too many arguments.\n";
                exit(1);
            }
            args[count++] = arg;
        }

        if (funcName.find("stack_") == 0 || funcName.find("queue_") == 0) {
            return execDS(funcName, args, count);
        }

        return execPhysics(funcName, args, count);
    }

    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        if (auto s = std::get_if<std::string>(&lit->value.data)) {
            std::string text = *s;
            std::string result = "";
            size_t start = 0;

            while (true) {
                size_t openBrace = text.find('{', start);
                size_t backslash = text.find('\\', start);

                if (backslash != std::string::npos &&
                    (openBrace == std::string::npos || backslash < openBrace)) {
                    result += text.substr(start, backslash - start);

                    // Escape Seq
                    if (backslash + 1 < text.length()) {
                        char next = text[backslash + 1];
                        if (next == 'n') result += '\n';
                        else if (next == 't') result += '\t';
                        else if (next == '\\') result += '\\';
                        else if (next == '{') result += '{';
                        else if (next == '}') result += '}';
                        else result += next;

                        start = backslash + 2;
                        continue;
                    }
                }

                if (openBrace == std::string::npos) {
                    result += text.substr(start);
                    break;
                }

                result += text.substr(start, openBrace - start);
                size_t closeBrace = text.find('}', openBrace);

                if (closeBrace == std::string::npos) {
                    result += text.substr(openBrace);
                    break;
                }

                std::string varName = text.substr(openBrace + 1, closeBrace - openBrace - 1);
                Token dummyToken = {TOKEN_IDENTIFIER, varName, 0};
                Value varVal = scope->get(dummyToken);
                result += valToString(varVal);

                start = closeBrace + 1;
            }
            return Value(result);
        }
        return lit->value;
    }

    if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        return scope->get(var->name);
    }

    // UNARY OPERATIONS
    if (auto una = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        Value rightVal = evaluate(una->right);

        if (una->op.type == TOKEN_BIT_NOT) {
             if (auto r = std::get_if<long long>(&rightVal.data)) return Value((long long)(~(*r)));
        }
        if (una->op.type == TOKEN_MINUS) {
            if (auto r = std::get_if<long long>(&rightVal.data)) return Value((long long)(-(*r)));
            if (auto r = std::get_if<long double>(&rightVal.data)) return Value((long double)(-(*r)));
        }
        if (una->op.type == TOKEN_BANG) {
            return Value((bool)!isTruthy(rightVal));
        }

        std::cerr << "Runtime Error: Invalid unary operation\n";
        exit(1);
    }

    // CONVERSIONS
    if (auto conv = std::dynamic_pointer_cast<ConvertExpr>(expr)) {
        Value val = evaluate(conv->value);
        Value modeVal = evaluate(conv->mode);

        auto modePtr = std::get_if<std::string>(&modeVal.data);
        if (!modePtr) {
            std::cerr << "Runtime Error: Conversion mode must be a string\n";
            exit(1);
        }

        std::string mode = *modePtr;
        long double num = getLongDouble(val);

        if (mode == "in_cm") return Value((long double)(num * 2.54L));
        if (mode == "cm_in") return Value((long double)(num / 2.54L));
        if (mode == "hp_kw") return Value((long double)(num * 0.7457L));
        if (mode == "kw_hp") return Value((long double)(num / 0.7457L));
        if (mode == "f_c") return Value((long double)((num - 32.0L) * 5.0L / 9.0L));
        if (mode == "c_f") return Value((long double)((num * 9.0L / 5.0L) + 32.0L));
        if (mode == "psi_bar") return Value((long double)(num * 0.0689476L));
        if (mode == "bar_psi") return Value((long double)(num / 0.0689476L));
        if (mode == "mb_gb") return Value((long double)(num / 1024.0L));
        if (mode == "gb_mb") return Value((long double)(num * 1024.0L));
        if (mode == "j_cal") return Value((long double)(num / 4184.0L));
        if (mode == "cal_j") return Value((long double)(num * 4184.0L));
        if (mode == "deg_rad") return Value((long double)(num * (M_PI / 180.0L)));
        if (mode == "rad_deg") return Value((long double)(num * (180.0L / M_PI)));
        if (mode == "lb_kg") return Value((long double)(num * 0.453592L));
        if (mode == "kg_lb") return Value((long double)(num / 0.453592L));
        if (mode == "usd_bdt") return Value((long double)(num * 122.0L));
        if (mode == "bdt_usd") return Value((long double)(num / 122.0L));
        if (mode == "usd_eur") return Value((long double)(num * 0.92L));
        if (mode == "eur_usd") return Value((long double)(num / 0.92L));
        if (mode == "mph_kmph") return Value((long double)(num * 1.60934L));
        if (mode == "kmph_mph") return Value((long double)(num / 1.60934L));
        if (mode == "nm_ftlb") return Value((long double)(num * 0.737562L));
        if (mode == "ftlb_nm") return Value((long double)(num / 0.737562L));
        if (mode == "g_ms2") return Value((long double)(num * 9.80665L));
        if (mode == "ms2_g") return Value((long double)(num / 9.80665L));

        std::cerr << "Runtime Error: Unknown conversion mode '" << mode << "'\n";
        exit(1);
    }

    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        Value leftVal = evaluate(bin->left);
        Value rightVal = evaluate(bin->right);

        if (bin->op.type == KW_AND) return Value((bool)(isTruthy(leftVal) && isTruthy(rightVal)));
        if (bin->op.type == KW_OR) return Value((bool)(isTruthy(leftVal) || isTruthy(rightVal)));

        bool leftIsNum = std::holds_alternative<long long>(leftVal.data) || std::holds_alternative<long double>(leftVal.data);
        bool rightIsNum = std::holds_alternative<long long>(rightVal.data) || std::holds_alternative<long double>(rightVal.data);

        long double l = 0.0L, r = 0.0L;
        if (leftIsNum) l = getLongDouble(leftVal);
        if (rightIsNum) r = getLongDouble(rightVal);

        if (leftIsNum && rightIsNum) {
            switch (bin->op.type) {
                case TOKEN_LESS:          return Value((bool)(l < r));
                case TOKEN_GREATER:       return Value((bool)(l > r));
                case TOKEN_LESS_EQUAL:    return Value((bool)(l <= r));
                case TOKEN_GREATER_EQUAL: return Value((bool)(l >= r));
                case TOKEN_EQUAL_EQUAL:   return Value((bool)(l == r));
                case TOKEN_BANG_EQUAL:    return Value((bool)(l != r));
            }
        }

        if (bin->op.type == TOKEN_EQUAL_EQUAL) return Value((bool)(leftVal == rightVal));
        if (bin->op.type == TOKEN_BANG_EQUAL) return Value((bool)(leftVal != rightVal));

        if (leftIsNum && rightIsNum) {
            bool useDouble = std::holds_alternative<long double>(leftVal.data) || std::holds_alternative<long double>(rightVal.data);
            if (bin->op.type == TOKEN_PLUS) {
                if(useDouble) return Value((long double)(l + r));
                return Value((long long)((long long)l + (long long)r));
            }
            if (bin->op.type == TOKEN_MINUS) {
                if(useDouble) return Value((long double)(l - r));
                return Value((long long)((long long)l - (long long)r));
            }
            if (bin->op.type == TOKEN_STAR) {
                if(useDouble) return Value((long double)(l * r));
                return Value((long long)((long long)l * (long long)r));
            }
            if (bin->op.type == TOKEN_SLASH) {
                if (r == 0) { std::cerr << "Runtime Error: Division by zero\n"; exit(1); }
                if(useDouble) return Value((long double)(l / r));
                return Value((long long)((long long)l / (long long)r));
            }
            if (bin->op.type == TOKEN_POW) return Value((long double)powl(l, r));
            if (bin->op.type == TOKEN_MOD) {
                if (!useDouble) {
                    long long li = (long long)l;
                    long long ri = (long long)r;
                    if (ri == 0) { std::cerr << "Runtime Error: Modulo by zero\n"; exit(1); }
                    return Value((long long)(li % ri));
                }
                if (r == 0) { std::cerr << "Runtime Error: Modulo by zero\n"; exit(1); }
                return Value((long double)std::fmod(l, r));
            }

            if (!useDouble) {
                long long li = (long long)l;
                long long ri = (long long)r;
                if (bin->op.type == TOKEN_BIT_AND) return Value((long long)(li & ri));
                if (bin->op.type == TOKEN_BIT_OR) return Value((long long)(li | ri));
                if (bin->op.type == TOKEN_LSHIFT) return Value((long long)(li << ri));
                if (bin->op.type == TOKEN_RSHIFT) return Value((long long)(li >> ri));
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
            return Value(toStr(leftVal) + toStr(rightVal));
        }

        std::cerr << "Runtime Error: Invalid operation\n";
        exit(1);
    }

    return 0LL;
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {
        if (!cmd) continue;

        if (auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(cmd)) {
            try {
                while (true) {
                    Value cond = evaluate(whileStmt->condition);
                    if (!isTruthy(cond)) break;
                    try {
                        std::vector<std::shared_ptr<Stmt>> wrapper = { whileStmt->body };
                        interpret(wrapper);
                    } catch (ContinueSignal&) {
                        continue;
                    }
                }
            } catch (BreakSignal&) {
            }
            continue;
        }

        if (auto brk = std::dynamic_pointer_cast<BreakStmt>(cmd)) throw BreakSignal();
        if (auto cont = std::dynamic_pointer_cast<ContinueStmt>(cmd)) throw ContinueSignal();

        if (auto funcStmt = std::dynamic_pointer_cast<FunctionStmt>(cmd)) {
            scope->defineFunc(funcStmt->name.lexeme, funcStmt);
            continue;
        }

        if (auto returnStmt = std::dynamic_pointer_cast<ReturnStmt>(cmd)) {
            Value val = 0LL;
            if (returnStmt->value) val = evaluate(returnStmt->value);
            throw ReturnValue(val);
        }

        if (auto ifStmt = std::dynamic_pointer_cast<IfStmt>(cmd)) {
            Value cond = evaluate(ifStmt->condition);
            if (isTruthy(cond)) {
                interpret({ ifStmt->thenBranch });
            } else if (ifStmt->elseBranch != nullptr) {
                interpret({ ifStmt->elseBranch });
            }
        }
        else if (auto seq = std::dynamic_pointer_cast<SequenceStmt>(cmd)) {
            interpret(seq->statements);
        }
        else if (auto block = std::dynamic_pointer_cast<BlockStmt>(cmd)) {
            std::shared_ptr<Scope> previous = scope;
            scope = std::make_shared<Scope>(previous);
            interpret(block->statements);
            scope = previous;
        }
        else if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            if (std::getline(std::cin, userText)) {
                Value parsed = parseInput(userText);
                if (auto var = std::dynamic_pointer_cast<VariableExpr>(input->target)) {
                    scope->assign(var->name, parsed);
                } else if (auto arr = std::dynamic_pointer_cast<ArrayAccessExpr>(input->target)) {
                    Value indexVal = evaluate(arr->index);
                    int index = (int)getLongDouble(indexVal);
                    scope->assignArrayElement(arr->name, index, parsed);
                }
            }
        }
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            scope->assign(assign->name, evaluate(assign->value));
        }
        else if (auto arrDecl = std::dynamic_pointer_cast<ArrayDeclStmt>(cmd)) {
            scope->declareArray(arrDecl->name);
        }
        else if (auto arrAssign = std::dynamic_pointer_cast<ArrayAssignStmt>(cmd)) {
            std::vector<Value> elements;
            for (auto elementExpr : arrAssign->value->elements) {
                elements.push_back(evaluate(elementExpr));
            }
            scope->assignArray(arrAssign->name, elements);
        }
        else if (auto arrElemAssign = std::dynamic_pointer_cast<ArrayElementAssignStmt>(cmd)) {
            int index = (int)getLongDouble(evaluate(arrElemAssign->index));
            scope->assignArrayElement(arrElemAssign->name, index, evaluate(arrElemAssign->value));
        }
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            printValue(evaluate(print->expression));
            if (print->createNewLine) std::cout << "\n";
        }
        else if (auto typeStmt = std::dynamic_pointer_cast<TypeStmt>(cmd)) {
            Value valToCheck = evaluate(typeStmt->expression);
            if (std::holds_alternative<long long>(valToCheck.data)) std::cout << "<type 'int'>\n";
            else if (std::holds_alternative<long double>(valToCheck.data)) std::cout << "<type 'float'>\n";
            else if (std::holds_alternative<std::string>(valToCheck.data)) std::cout << "<type 'string'>\n";
            else if (std::holds_alternative<bool>(valToCheck.data)) std::cout << "<type 'bool'>\n";
            else std::cout << "<type 'collection'>\n";
        }
        else if (auto exprStmt = std::dynamic_pointer_cast<ExprStmt>(cmd)) {
            evaluate(exprStmt->expression);
        }
    }
}
