#include "../include/Interpreter.h"
#include "../include/Utils.h"
#include <iostream>
#include <string>

// --- HELPER FUNCTION: Detects Int vs Double vs String ---
Value parseInput(std::string text) {
    if (text.empty()) return text;

    bool isNumber = true;
    bool hasDot = false;

    for (char c : text) {
        if (!isDigit(c)) {
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
            if (hasDot) return std::stod(text); // Return Double
            return std::stoi(text);            // Return Int
        } catch (...) {
            return text; // Conversion failed, return String
        }
    }
    return text; // Return String
}

// --- MAIN LOOP ---
void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> commands) {
    for (auto cmd : commands) {

        // --- EXECUTE INPUT ---
        if (auto input = std::dynamic_pointer_cast<InputStmt>(cmd)) {
            std::string userText;
            std::cout << "drim input " << input->name.lexeme << ": ";
            if (std::getline(std::cin, userText)) {
                // IMPORTANT: Use the helper to convert the type!
                memory[input->name.lexeme] = parseInput(userText);
            }
        }

        // --- EXECUTE ASSIGNMENT ---
        else if (auto assign = std::dynamic_pointer_cast<AssignStmt>(cmd)) {
            Value valToStore = ""; // Correct type (Value)

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

        // --- EXECUTE PRINT ---
        else if (auto print = std::dynamic_pointer_cast<PrintStmt>(cmd)) {
            Value output = "null"; // Correct type (Value)

            if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(print->expression)) {
                output = lit->value;
            }
            else if (auto var = std::dynamic_pointer_cast<VariableExpr>(print->expression)) {
                if (memory.count(var->name.lexeme)) {
                    output = memory[var->name.lexeme];
                }
            }

            printValue(output);
            std::cout << "\n";
        }
    }
}