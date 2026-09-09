#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <stdexcept>

#include "Value.h"
#include "Errors/IndexError.h"

int normalize_int_index(Value index, int size);
