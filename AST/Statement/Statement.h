#pragma once

#include <optional>

#include "AST/Environment.h"

class Statement
{
public:
    virtual ~Statement() = default;

    virtual std::optional<int> execute(Environment &env) const = 0;
};