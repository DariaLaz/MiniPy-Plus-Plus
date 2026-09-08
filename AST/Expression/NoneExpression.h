#pragma once

#include "AST/Expression/Expression.h"
#include "Value.h"

class NoneExpression : public Expression
{
public:
    Value evaluate(Environment &) const override
    {
        return std::monostate{};
    }
};