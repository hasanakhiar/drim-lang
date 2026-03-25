#include "../include/Physics.h"
#include <iostream>
#include <cmath>
#include <variant>

// Helper to get long double args
long double getNum(const Value* args, size_t count, size_t index) {
    if (index >= count) return 0.0;
    const Value& v = args[index];
    if (auto i = std::get_if<long long>(&v.data)) return (long double)*i;
    if (auto d = std::get_if<long double>(&v.data)) return *d;
    return 0.0;
}

Value execPhysics(const std::string& name, const Value* args, size_t count) {
    // 1. Motion (Kinematics)
    if (name == "speed") {
        if (count != 2) { 
            std::cerr << "Error: speed(distance, time) expects 2 arguments.\n"; 
            exit(1); 
        }
        long double d = getNum(args, count, 0);
        long double t = getNum(args, count, 1);
        if (t == 0) return 0.0L;
        return d / t;
    }

    if (name == "velocity") {
        if (count != 2) { 
            std::cerr << "Error: velocity(displacement, time) expects 2 arguments.\n"; 
            exit(1); 
        }
        long double d = getNum(args, count, 0);
        long double t = getNum(args, count, 1);
        if (t == 0) return 0.0L;
        return d / t;
    }

    if (name == "acceleration") {
        if (count != 3) { 
            std::cerr << "Error: acceleration(vf, vi, t) expects 3 arguments.\n"; 
            exit(1); 
        }
        long double vf = getNum(args, count, 0);
        long double vi = getNum(args, count, 1);
        long double t = getNum(args, count, 2);
        if (t == 0) return 0.0L;
        return (vf - vi) / t;
    }

    if (name == "distance") {
        if (count != 2) { 
             std::cerr << "Error: distance(speed, time) expects 2 arguments.\n"; 
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "final_velocity") {
        if (count != 3) {
            std::cerr << "Error: final_velocity(u, a, t) expects 3 arguments.\n";
            exit(1);
        }
        long double u = getNum(args, count, 0);
        long double a = getNum(args, count, 1);
        long double t = getNum(args, count, 2);
        return u + (a * t);
    }

    // 2. Force & Mechanics
    if (name == "force") {
        if (count != 2) {
             std::cerr << "Error: force(m, a) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "weight") {
        if (count != 2) {
             std::cerr << "Error: weight(m, g) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "pressure") {
        if (count != 2) {
             std::cerr << "Error: pressure(F, A) expects 2 arguments.\n";
             exit(1);
        }
        long double F = getNum(args, count, 0);
        long double A = getNum(args, count, 1);
        if (A == 0) return 0.0L;
        return F / A;
    }

    if (name == "momentum") {
        if (count != 2) {
             std::cerr << "Error: momentum(m, v) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "impulse") {
        if (count != 2) {
             std::cerr << "Error: impulse(F, t) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    // 3. Work, Energy & Power
    if (name == "work") {
        if (count != 2) {
             std::cerr << "Error: work(F, d) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "kinetic_energy") {
        if (count != 2) {
             std::cerr << "Error: kinetic_energy(m, v) expects 2 arguments.\n";
             exit(1);
        }
        long double m = getNum(args, count, 0);
        long double v = getNum(args, count, 1);
        return 0.5L * m * v * v;
    }

    if (name == "potential_energy") {
        if (count != 3) {
             std::cerr << "Error: potential_energy(m, g, h) expects 3 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1) * getNum(args, count, 2);
    }

    if (name == "power") {
        if (count != 2) {
             std::cerr << "Error: power(W, t) expects 2 arguments.\n";
             exit(1);
        }
        long double W = getNum(args, count, 0);
        long double t = getNum(args, count, 1);
        if (t == 0) return 0.0L;
        return W / t;
    }

    // 4. Circular Motion
    if (name == "centripetal_force") {
        if (count != 3) {
             std::cerr << "Error: centripetal_force(m, v, r) expects 3 arguments.\n";
             exit(1);
        }
        long double m = getNum(args, count, 0);
        long double v = getNum(args, count, 1);
        long double r = getNum(args, count, 2);
        if (r == 0) return 0.0L;
        return (m * v * v) / r;
    }

    if (name == "angular_speed") {
        if (count != 1) {
             std::cerr << "Error: angular_speed(T) expects 1 argument.\n";
             exit(1);
        }
        long double T = getNum(args, count, 0);
        if (T == 0) return 0.0L;
        return (2.0L * 3.14159265358979323846L) / T;
    }

    // 5. Electricity
    if (name == "voltage") {
        if (count != 2) {
             std::cerr << "Error: voltage(I, R) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "current") {
        if (count != 2) {
             std::cerr << "Error: current(V, R) expects 2 arguments.\n";
             exit(1);
        }
        long double V = getNum(args, count, 0);
        long double R = getNum(args, count, 1);
        if (R == 0) return 0.0L;
        return V / R;
    }

    if (name == "electrical_power") {
        if (count != 2) {
             std::cerr << "Error: electrical_power(V, I) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "electrical_energy") {
        if (count != 2) {
             std::cerr << "Error: electrical_energy(P, t) expects 2 arguments.\n";
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    // 6. Waves & Optics
    if (name == "wave_speed") {
        if (count != 2) {
             std::cerr << "Error: wave_speed(f, lambda) expects 2 arguments.\n"; 
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1);
    }

    if (name == "frequency") {
        if (count != 1) {
             std::cerr << "Error: frequency(T) expects 1 argument.\n"; 
             exit(1);
        }
        long double T = getNum(args, count, 0);
        if (T == 0) return 0.0L;
        return 1.0L / T;
    }

    // 7. Heat & Thermodynamics
    if (name == "heat_energy") {
        if (count != 3) {
             std::cerr << "Error: heat_energy(m, c, deltaT) expects 3 arguments.\n"; 
             exit(1);
        }
        return getNum(args, count, 0) * getNum(args, count, 1) * getNum(args, count, 2);
    }

    if (name == "to_kelvin") {
        if (count != 1) {
             std::cerr << "Error: to_kelvin(c) expects 1 argument.\n"; 
             exit(1);
        }
        return getNum(args, count, 0) + 273.15L;
    }

    if (name == "to_fahrenheit") {
        if (count != 1) {
             std::cerr << "Error: to_fahrenheit(c) expects 1 argument.\n"; 
             exit(1);
        }
        return (getNum(args, count, 0) * 1.8L) + 32.0L;
    }

    if (name == "mass_energy") {
        if (count != 1) {
             std::cerr << "Error: mass_energy(m) expects 1 argument.\n"; 
             exit(1);
        }
        long double m = getNum(args, count, 0);
        long double c = 299792458.0L; 
        return m * c * c;
    }

    std::cerr << "Runtime Error: Unknown function '" << name << "'\n";
    exit(1);
}