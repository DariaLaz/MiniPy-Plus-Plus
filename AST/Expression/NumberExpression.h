#pragma once
#include "AST/Expression/Expression.h"

class NumberExpression : public Expression
{
public:
    int value;

    NumberExpression(int value)
        : value(value) {}

    Value evaluate(Environment &env) const override
    {
        return value;
    }
};