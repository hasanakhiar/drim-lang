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

    // 2. Force & Mechanics

    // force(mass, acceleration) -> F = ma
    functions["force"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: force(m, a) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // weight(mass, gravity) -> W = mg
    functions["weight"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: weight(m, g) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // pressure(force, area) -> P = F/A
    functions["pressure"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: pressure(F, A) expects 2 arguments.\n";
             return 0.0;
        }
        double F = getNum(args, 0);
        double A = getNum(args, 1);
        if (A == 0) return 0.0;
        return F / A;
    };

    // momentum(mass, velocity) -> p = mv
    functions["momentum"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: momentum(m, v) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // impulse(force, time) -> J = Ft
    functions["impulse"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: impulse(F, t) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // 3. Work, Energy & Power

    // work(force, distance) -> W = Fd
    functions["work"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: work(F, d) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // kinetic_energy(mass, velocity) -> KE = 0.5 * m * v^2
    functions["kinetic_energy"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: kinetic_energy(m, v) expects 2 arguments.\n";
             return 0.0;
        }
        double m = getNum(args, 0);
        double v = getNum(args, 1);
        return 0.5 * m * v * v;
    };

    // potential_energy(mass, gravity, height) -> PE = mgh
    functions["potential_energy"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) {
             std::cout << "Error: potential_energy(m, g, h) expects 3 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1) * getNum(args, 2);
    };

    // power(work, time) -> P = W/t
    functions["power"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: power(W, t) expects 2 arguments.\n";
             return 0.0;
        }
        double W = getNum(args, 0);
        double t = getNum(args, 1);
        if (t == 0) return 0.0;
        return W / t;
    };
}
