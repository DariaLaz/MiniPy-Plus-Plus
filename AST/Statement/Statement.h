#pragma once

#include <optional>

#include "AST/Environment.h"

class Statement
{
public:
    virtual ~Statement() = default;

    virtual std::optional<Value> execute(Environment &env) const = 0;
};