#ifndef PHYSICS_H
#define PHYSICS_H

#include "Value.h"
#include <string>

// Execute a physics function
Value execPhysics(const std::string& name, const Value* args, size_t count);

#endif
