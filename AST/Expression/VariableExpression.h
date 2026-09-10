#pragma once

#include "AST/Expression/Expression.h"

class VariableExpression : public Expression
{
public:
    std::string name;

    VariableExpression(std::string name, const SourceLocation &location)
        : Expression(location), name(std::move(name))
    {
    }

    Value evaluate(Environment &env) const override
    {
        return env.get(name, get_location());
    }
};