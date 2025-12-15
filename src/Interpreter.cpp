//
// Created by Muntahi Hasan Akhiar on 6/12/25.
//

#include "../include/Interpreter.h"
#include <iostream>

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {

        // --- EXECUTE INPUT (drim) ---
        if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            std::cout << "drim input " << input->name.lexeme << " "; // Optional: Remove if you want pure headless
            if (std::getline(std::cin, userText)) {
                memory[input->name.lexeme] = userText;
            }
        }

        // --- EXECUTE ASSIGNMENT (var = val) ---
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            std::string valToStore = "";

            if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(assign->value)) {
                valToStore = lit->value;
            }
            else if (auto var = std::dynamic_pointer_cast<VariableExpr>(assign->value)) {
                if (memory.count(var->name.lexeme)) {
                    valToStore = memory[var->name.lexeme];
                }
            }

            memory[assign->name.lexeme] = valToStore;
        }

        // --- EXECUTE PRINT (wake) ---
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            std::string output = "null";

            // If printing a literal string "hello"
            if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(print->expression)) {
                output = lit->value;
            }
            // If printing a variable
            else if (auto var = std::dynamic_pointer_cast<VariableExpr>(print->expression)) {
                if (memory.count(var->name.lexeme)) {
                    output = memory[var->name.lexeme];
                }
            }

            std::cout << output << "\n";
        }
    }
}