#pragma once

#include <memory>
#include <stdexcept>

#include "AST/Expression/Expression.h"
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

    Value evaluate(Environment &env) const override
    {
        Value value = expr->evaluate(env);

        switch (op.type)
        {
        case TokenType::Minus:
            return -std::get<int>(value);

        case TokenType::Plus:
            return value;

        case TokenType::Not:
            return !is_truthy(value);

        default:
            throw std::runtime_error("Invalid unary operator");
        }
    }
};