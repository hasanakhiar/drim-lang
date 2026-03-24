#include "../include/DS.h"
#include <iostream>
#include <vector>
#include <memory>

Value execDS(const std::string& name, const Value* args, size_t count) {
    // === 1. CREATION ===
    if (name == "stack_create" || name == "queue_create") {
        auto collectionPtr = std::make_shared<std::vector<Value>>();
        return Value(collectionPtr);
    }

    // All other operations require at least 1 argument (the collection)
    if (count < 1) {
        std::cerr << "Runtime Error: '" << name << "' expects at least 1 argument.\n";
        exit(1);
    }

    auto variantPtr = std::get_if<std::shared_ptr<std::vector<Value>>>(&args[0].data);
    if (!variantPtr) {
        std::cerr << "Runtime Error: First argument of '" << name << "' must be a collection.\n";
        exit(1);
    }
    auto& list = *variantPtr;

    // === 2. STACK OPERATIONS (LIFO) ===
    if (name == "stack_push") {
        if (count != 2) { std::cerr << "Runtime Error: stack_push(s, val) expects 2 args.\n"; exit(1); }
        list->push_back(args[1]);
        return args[1];
    }
    if (name == "stack_pop") {
        if (list->empty()) { std::cerr << "Runtime Error: stack_pop from empty stack.\n"; exit(1); }
        Value top = list->back();
        list->pop_back();
        return top;
    }
    if (name == "stack_peek") {
        if (list->empty()) { std::cerr << "Runtime Error: stack_peek at empty stack.\n"; exit(1); }
        return list->back();
    }

    // === 3. QUEUE OPERATIONS (FIFO) ===
    if (name == "queue_enqueue") {
        if (count != 2) { std::cerr << "Runtime Error: queue_enqueue(q, val) expects 2 args.\n"; exit(1); }
        list->push_back(args[1]); // Enqueue at the end
        return args[1];
    }
    if (name == "queue_dequeue") {
        if (list->empty()) { std::cerr << "Runtime Error: queue_dequeue from empty queue.\n"; exit(1); }
        Value front = list->front();
        list->erase(list->begin()); // Traditional way to remove from front of a vector
        return front;
    }
    if (name == "queue_peek") {
        if (list->empty()) { std::cerr << "Runtime Error: queue_peek at empty queue.\n"; exit(1); }
        return list->front();
    }

    // === 4. COMMON OPERATIONS ===
    if (name == "stack_empty" || name == "queue_empty") {
        return (bool)list->empty();
    }
    if (name == "stack_size" || name == "queue_size") {
        return (long long)list->size();
    }

    std::cerr << "Runtime Error: Unknown DS function '" << name << "'\n";
    exit(1);
}