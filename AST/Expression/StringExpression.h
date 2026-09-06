#pragma once

#include <string>

#include "AST/Expression/Expression.h"

class StringExpression : public Expression
{
public:
    StringExpression(std::string value)
        : value(std::move(value))
    {
    }

    Value evaluate(Environment &) const override
    {
        return value;
    }

private:
    std::string value;
};