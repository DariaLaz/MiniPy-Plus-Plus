#pragma once
#include "AST/Expression/Expression.h"

class NumberExpression : public Expression
{
public:
    int value;

    NumberExpression(int value, const SourceLocation &location)
        : value(value), Expression(location) {}

    Value evaluate(Environment &env) const override
    {
        return value;
    }
};