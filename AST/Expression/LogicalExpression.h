#pragma once

#include <memory>
#include <stdexcept>

#include "AST/Expression/Expression.h"
#include "AST/Environment.h"
#include "Token.h"
#include "Value.h"

class LogicalExpression : public Expression
{
public:
    LogicalExpression(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right))
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value left_value = left->evaluate(env);

        if (op.type == TokenType::Or)
        {
            if (is_truthy(left_value))
            {
                return left_value;
            }

            return right->evaluate(env);
        }

        if (op.type == TokenType::And)
        {
            if (!is_truthy(left_value))
            {
                return left_value;
            }

            return right->evaluate(env);
        }

        throw std::runtime_error("Invalid logical operator");
    }

private:
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};