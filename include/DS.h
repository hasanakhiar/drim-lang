#ifndef DS_H
#define DS_H

#include "Value.h"
#include <string>

// Execute a Data Structure function
Value execDS(const std::string& name, const Value* args, size_t count);

#endif