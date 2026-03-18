#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

enum class ArrayType {
    UNKNOWN,
    INT,
    DOUBLE,
    STRING,
    BOOL
};

struct ArrayValue;
using Value = std::variant<int, double, std::string, bool, std::shared_ptr<ArrayValue>>;

struct ArrayValue {
    ArrayType elementType = ArrayType::UNKNOWN;
    std::vector<Value> elements;
};

// Printer Helper
inline void printValue(const Value& v) {
    if (std::holds_alternative<int>(v)) 
        std::cout << std::get<int>(v);
    else if (std::holds_alternative<double>(v)) 
        std::cout << std::get<double>(v);
    else if (std::holds_alternative<std::string>(v)) 
        std::cout << std::get<std::string>(v);
    else if (std::holds_alternative<bool>(v))
        std::cout << (std::get<bool>(v) ? "true" : "false");
    else if (std::holds_alternative<std::shared_ptr<ArrayValue>>(v)) {
        auto arr = std::get<std::shared_ptr<ArrayValue>>(v);
        std::cout << "[";
        for (size_t i = 0; i < arr->elements.size(); i++) {
            printValue(arr->elements[i]);
            if (i + 1 < arr->elements.size()) std::cout << ", ";
        }
        std::cout << "]";
    }
}

#endif