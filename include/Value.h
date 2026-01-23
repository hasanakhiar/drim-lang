#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <string>
#include <iostream>

// Container
using Value = std::variant<int, double, std::string>;

// Printer Helper
inline void printValue(const Value& v) {
    if (std::holds_alternative<int>(v)) 
        std::cout << std::get<int>(v);
    else if (std::holds_alternative<double>(v)) 
        std::cout << std::get<double>(v);
    else if (std::holds_alternative<std::string>(v)) 
        std::cout << std::get<std::string>(v);
}

#endif