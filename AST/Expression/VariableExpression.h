#pragma once

#include "AST/Expression/Expression.h"

class VariableExpression : public Expression
{
public:
    std::string name;

    VariableExpression(std::string name)
        : name(std::move(name))
    {
    }

    Value evaluate(Environment &env) const override
    {
        return env.get(name);
    }
};