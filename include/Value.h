#ifndef VALUE_H
#define VALUE_H

#include <variant>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

// Forward declaration
struct AnyValue;

// A traditional way to handle recursive Value types (like stacks containing values)
struct AnyValue {
    std::variant<
        long long, 
        long double, 
        std::string, 
        bool, 
        std::shared_ptr<std::vector<AnyValue>> 
    > data;

    // Constructors for convenience
    AnyValue() : data(false) {}
    
    AnyValue(long long v) : data(v) {}
    AnyValue(long double v) : data(v) {}
    AnyValue(std::string v) : data(v) {}
    AnyValue(const char* v) : data(std::string(v)) {}
    
    // Use enable_if to ensure this only matches actual booleans, not pointers
    template<typename T, typename = std::enable_if_t<std::is_same_v<T, bool>>>
    AnyValue(T v) : data(v) {}

    AnyValue(std::shared_ptr<std::vector<AnyValue>> v) : data(v) {}

    // Equality operator for variant comparison
    bool operator==(const AnyValue& other) const { return data == other.data; }
    bool operator!=(const AnyValue& other) const { return data != other.data; }
};

// Redefine Value as AnyValue
using Value = AnyValue;

// Printer Helper
inline void printValue(const Value& v) {
    if (std::holds_alternative<long long>(v.data)) 
        std::cout << std::get<long long>(v.data);
    else if (std::holds_alternative<long double>(v.data)) 
        std::cout << std::get<long double>(v.data);
    else if (std::holds_alternative<std::string>(v.data)) 
        std::cout << std::get<std::string>(v.data);
    else if (std::holds_alternative<bool>(v.data))
        std::cout << (std::get<bool>(v.data) ? "true" : "false");
    else if (std::holds_alternative<std::shared_ptr<std::vector<AnyValue>>>(v.data))
        std::cout << "<stack size=" << std::get<std::shared_ptr<std::vector<AnyValue>>>(v.data)->size() << ">";
}

#endif