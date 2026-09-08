#pragma once

#include "Expression.h"

class CallExpression : public Expression
{
public:
    CallExpression(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression>> arguments)
        : callee(std::move(callee)), arguments(std::move(arguments))
    {
    }

    Value evaluate(Environment &env) const override;

private:
    std::unique_ptr<Expression> callee;

    std::vector<std::unique_ptr<Expression>> arguments;
};