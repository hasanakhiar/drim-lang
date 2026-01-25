#ifndef PHYSICS_H
#define PHYSICS_H

#include "Value.h"
#include <map>
#include <string>
#include <vector>
#include <functional>

using NativeFn = std::function<Value(const std::vector<Value>&)>;

void registerPhysicsFunctions(std::map<std::string, NativeFn>& functions);

#endif