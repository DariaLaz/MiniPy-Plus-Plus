#pragma once

#include "Errors/IndexError.h"
#include "Runtime/Value.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

int normalize_int_index(Value index, std::size_t size, const SourceLocation& location);
