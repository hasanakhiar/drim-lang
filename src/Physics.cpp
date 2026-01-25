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

    // 4. Circular Motion

    // centripetal_force(mass, velocity, radius) -> F = mv^2/r
    functions["centripetal_force"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) {
             std::cout << "Error: centripetal_force(m, v, r) expects 3 arguments.\n";
             return 0.0;
        }
        double m = getNum(args, 0);
        double v = getNum(args, 1);
        double r = getNum(args, 2);
        if (r == 0) return 0.0;
        return (m * v * v) / r;
    };

    // angular_speed(period) -> w = 2pi / T
    functions["angular_speed"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
             std::cout << "Error: angular_speed(T) expects 1 argument.\n";
             return 0.0;
        }
        double T = getNum(args, 0);
        if (T == 0) return 0.0;
        return (2.0 * 3.1415926535) / T;
    };

    // 5. Electricity

    // voltage(current, resistance) -> V = IR
    functions["voltage"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: voltage(I, R) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // current(voltage, resistance) -> I = V/R
    functions["current"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: current(V, R) expects 2 arguments.\n";
             return 0.0;
        }
        double V = getNum(args, 0);
        double R = getNum(args, 1);
        if (R == 0) return 0.0;
        return V / R;
    };

    // electrical_power(voltage, current) -> P = VI
    functions["electrical_power"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: electrical_power(V, I) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // electrical_energy(power, time) -> E = Pt
    functions["electrical_energy"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: electrical_energy(P, t) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // 6. Waves & Optics

    // wave_speed(frequency, wavelength) -> v = f * lambda
    functions["wave_speed"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) {
             std::cout << "Error: wave_speed(f, lambda) expects 2 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1);
    };

    // frequency(period) -> f = 1/T
    functions["frequency"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
             std::cout << "Error: frequency(T) expects 1 argument.\n";
             return 0.0;
        }
        double T = getNum(args, 0);
        if (T == 0) return 0.0;
        return 1.0 / T;
    };

    // 7. Heat & Thermodynamics

    // heat_energy(mass, specific_heat, delta_temp) -> Q = mcT
    functions["heat_energy"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) {
             std::cout << "Error: heat_energy(m, c, deltaT) expects 3 arguments.\n";
             return 0.0;
        }
        return getNum(args, 0) * getNum(args, 1) * getNum(args, 2);
    };

    // to_kelvin(celsius) -> K = C + 273.15
    functions["to_kelvin"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
             std::cout << "Error: to_kelvin(c) expects 1 argument.\n";
             return 0.0;
        }
        return getNum(args, 0) + 273.15;
    };

    // to_fahrenheit(celsius) -> F = (9/5)C + 32
    functions["to_fahrenheit"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
             std::cout << "Error: to_fahrenheit(c) expects 1 argument.\n";
             return 0.0;
        }
        return (getNum(args, 0) * 1.8) + 32.0;
    };

    // Relativity
    // mass_energy(mass) -> E = mc^2
    functions["mass_energy"] = [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) {
             std::cout << "Error: mass_energy(m) expects 1 argument.\n";
             return 0.0;
        }
        double m = getNum(args, 0);
        double c = 299792458.0; 
        return m * c * c;
    };
}
