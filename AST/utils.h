#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <stdexcept>

#include "Value.h"

int normalize_int_index(Value index, int size);

inline void validate_index(std::size_t min, std::size_t max, int val, std::string message);