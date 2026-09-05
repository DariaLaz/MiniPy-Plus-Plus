#pragma once

#include <memory>
#include <stdexcept>

#include "Expression.h"
#include "Token.h"

class UnaryExpression : public Expression
{
public:
    Token op;
    std::unique_ptr<Expression> expr;

    UnaryExpression(
        Token op,
        std::unique_ptr<Expression> expr)
        : op(std::move(op)),
          expr(std::move(expr))
    {
    }

    int evaluate() const override
    {
        int value = expr->evaluate();

        switch (op.type)
        {
        case TokenType::Minus:
            return -value;

        case TokenType::Plus:
            return value;

        default:
            throw std::runtime_error("Invalid unary operator");
        }
    }
};