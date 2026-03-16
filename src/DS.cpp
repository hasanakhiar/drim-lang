#include "../include/DS.h"
#include <iostream>
#include <vector>
#include <memory>

Value execDS(const std::string& name, const Value* args, size_t count) {
    // 1. Create a new stack
    if (name == "stack_create") {
        auto stackPtr = std::make_shared<std::vector<Value>>();
        return Value(stackPtr);
    }

    // All other operations require at least 1 argument (the stack)
    if (count < 1) {
        std::cerr << "Runtime Error: '" << name << "' expects at least 1 argument (the stack).\n";
        exit(1);
    }

    auto stackVariant = std::get_if<std::shared_ptr<std::vector<Value>>>(&args[0].data);
    if (!stackVariant) {
        std::cerr << "Runtime Error: First argument of '" << name << "' must be a stack.\n";
        exit(1);
    }
    auto& stack = *stackVariant;

    if (name == "stack_push") {
        if (count != 2) {
            std::cerr << "Runtime Error: stack_push(s, val) expects 2 arguments.\n";
            exit(1);
        }
        stack->push_back(args[1]);
        return args[1]; // Return the pushed value
    }

    if (name == "stack_pop") {
        if (stack->empty()) {
            std::cerr << "Runtime Error: Cannot pop from an empty stack.\n";
            exit(1);
        }
        Value top = stack->back();
        stack->pop_back();
        return top;
    }

    if (name == "stack_peek") {
        if (stack->empty()) {
            std::cerr << "Runtime Error: Cannot peek at an empty stack.\n";
            exit(1);
        }
        return stack->back();
    }

    if (name == "stack_empty") {
        return (bool)stack->empty();
    }

    if (name == "stack_size") {
        return (long long)stack->size();
    }

    std::cerr << "Runtime Error: Unknown stack function '" << name << "'\n";
    exit(1);
}