#pragma once
#include "Expression.h"

class NumberExpression : public Expression
{
public:
    NumberExpression(int value)
        : value(value) {}

    int value;
};