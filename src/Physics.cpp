#include "../include/Physics.h"
#include <iostream>
#include <cmath>

// Helper to get double args safely
double getNum(const std::vector<Value>& args, size_t index) {
    if (index >= args.size()) return 0.0;
    const Value& v = args[index];
    if (std::holds_alternative<int>(v)) return (double)std::get<int>(v);
    if (std::holds_alternative<double>(v)) return std::get<double>(v);
    return 0.0;
}

void registerPhysicsFunctions(std::map<std::string, NativeFn>& functions) {
    // 1. Motion (Kinematics)
    
    // speed(distance, time) -> distance / time
    functions["speed"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) { 
            std::cout << "Error: speed(distance, time) expects 2 arguments.\n"; 
            return 0.0; 
        }
        double d = getNum(args, 0);
        double t = getNum(args, 1);
        if (t == 0) return 0.0; // Avoid div by zero
        return d / t;
    };

    // velocity(displacement, time)
    functions["velocity"] = [](const std::vector<Value>& args) -> Value {
         if (args.size() != 2) { 
            std::cout << "Error: velocity(displacement, time) expects 2 arguments.\n"; 
            return 0.0; 
        }
        double d = getNum(args, 0);
        double t = getNum(args, 1);
        if (t == 0) return 0.0;
        return d / t;
    };

    // acceleration(final_velocity, initial_velocity, time)
    functions["acceleration"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) { 
            std::cout << "Error: acceleration(vf, vi, t) expects 3 arguments.\n"; 
            return 0.0; 
        }
        double vf = getNum(args, 0);
        double vi = getNum(args, 1);
        double t = getNum(args, 2);
        if (t == 0) return 0.0;
        return (vf - vi) / t;
    };

    // distance(speed, time)
    functions["distance"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) { 
             std::cout << "Error: distance(speed, time) expects 2 arguments.\n"; 
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // final_velocity(u, a, t) -> v = u + at
    functions["final_velocity"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) {
            std::cout << "Error: final_velocity(u, a, t) expects 3 arguments.\n";
            return 0.0;
        }
        double u = getNum(args, 0);
        double a = getNum(args, 1);
        double t = getNum(args, 2);
        return u + (a * t);
    };
}
