//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#include "../include/Interpreter.h"
#include <iostream>
#include <string>

std::string Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        return lit->value;
    }
    else if (auto var = std::dynamic_pointer_cast<VariableExpr>(expr)) {
        if (memory.count(var->name.lexeme)) {
            return memory[var->name.lexeme];
        }
        return "null";
    }
    else if (auto call = std::dynamic_pointer_cast<CallExpr>(expr)) {
        std::string funcName = call->callee.lexeme;
        std::string argVal = evaluate(call->argument);
        
        try {
            double val = std::stof(argVal);
            
            // Mass
            if (funcName == "kgToLbs") return std::to_string(val * 2.20462);
            if (funcName == "lbsToKg") return std::to_string(val / 2.20462);
            
            // Distance
            if (funcName == "kmToMiles") return std::to_string(val * 0.621371);
            if (funcName == "milesToKm") return std::to_string(val / 0.621371);
            
            // Energy
            if (funcName == "joulesToCal") return std::to_string(val * 0.239006);
            if (funcName == "calToJoules") return std::to_string(val / 0.239006);

            // Temperature
            if (funcName == "cToF") return std::to_string((val * 9.0/5.0) + 32.0);
            if (funcName == "fToC") return std::to_string((val - 32.0) * 5.0/9.0);

        } catch (...) {
            return "error: invalid argument for " + funcName;
        }
        return "error: unknown function " + funcName;
    }
    return "null";
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {

        // --- EXECUTE INPUT (drim) ---
        if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            std::cout << "drim input " << input->name.lexeme << ": "; // Optional
            if (std::getline(std::cin, userText)) {
                memory[input->name.lexeme] = userText;
            }
        }

        // --- EXECUTE ASSIGNMENT (var = val) ---
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            memory[assign->name.lexeme] = evaluate(assign->value);
        }

        // --- EXECUTE PRINT (wake) ---
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            std::cout << evaluate(print->expression) << "\n";
        }
    }
}
