#pragma once

#include "Expression.h"

class BoolExpression : public Expression
{
public:
    BoolExpression(bool value) : value(value)
    {
    }

    Value evaluate(Environment &env) const override
    {
        return value;
    }

private:
    bool value;
};