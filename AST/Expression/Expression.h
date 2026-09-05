#pragma once

#include "AST/Environment.h"

class Expression
{
public:
    virtual ~Expression() noexcept = default;
    virtual int evaluate(Environment &env) const = 0;
};