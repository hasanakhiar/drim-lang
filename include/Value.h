#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <string>
#include <iostream>

// Container for long long, long double, string, and bool
using Value = std::variant<long long, long double, std::string, bool>;

// Printer Helper
inline void printValue(const Value& v) {
    if (std::holds_alternative<long long>(v)) 
        std::cout << std::get<long long>(v);
    else if (std::holds_alternative<long double>(v)) 
        std::cout << std::get<long double>(v);
    else if (std::holds_alternative<std::string>(v)) 
        std::cout << std::get<std::string>(v);
    else if (std::holds_alternative<bool>(v))
        std::cout << (std::get<bool>(v) ? "true" : "false");
}

#endif