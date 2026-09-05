#pragma once

#include <memory>

#include "AST/Expression/Expression.h"
#include "Token.h"

class BinaryExpression : public Expression
{
public:
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;

    BinaryExpression(
        std::unique_ptr<Expression> left,
        Token op,
        std::unique_ptr<Expression> right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right))
    {
    }

    int evaluate(Environment &env) const override
    {
        int left_value = left->evaluate(env);
        int right_value = right->evaluate(env);

        switch (op.type)
        {
        case TokenType::Plus:
            return left_value + right_value;

        case TokenType::Minus:
            return left_value - right_value;

        case TokenType::Star:
            return left_value * right_value;

        case TokenType::Slash:
            if (right_value == 0)
                throw std::runtime_error("Division by zero");

            return left_value / right_value;

        default:
            throw std::runtime_error("Invalid operator");
        }
    }
};