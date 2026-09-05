#pragma once

#include "AST/Environment.h"
#include <Value.h>

class Expression
{
public:
    virtual ~Expression() noexcept = default;
    virtual Value evaluate(Environment &env) const = 0;
};