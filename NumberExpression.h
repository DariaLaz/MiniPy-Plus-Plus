#pragma once
#include "Expression.h"

class NumberExpression : public Expression
{
public:
    int value;

    NumberExpression(int value)
        : value(value) {}

    int evaluate() const override
    {
        return value;
    }
};