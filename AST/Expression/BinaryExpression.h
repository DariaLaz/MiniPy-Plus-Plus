#pragma once

#include <memory>

#include "AST/Expression/Expression.h"
#include "Token.h"

class BinaryExpression : public Expression
{
public:
    BinaryExpression(
        std::unique_ptr<Expression> left,
        Token op,
        std::unique_ptr<Expression> right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right))
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value left_value = left->evaluate(env);
        Value right_value = right->evaluate(env);

        switch (op.type)
        {
        case TokenType::Plus:
            return add(left_value, right_value);

        case TokenType::Minus:
            return std::get<int>(left_value) - std::get<int>(right_value);

        case TokenType::Star:
            return std::get<int>(left_value) * std::get<int>(right_value);

        case TokenType::Slash:
            if (std::get<int>(right_value) == 0)
                throw std::runtime_error("Division by zero");

            return std::get<int>(left_value) / std::get<int>(right_value);

        case TokenType::EqualEqual:
            return left_value == right_value;

        case TokenType::NotEqual:
            return left_value != right_value;

        case TokenType::Less:
            return std::get<int>(left_value) < std::get<int>(right_value);

        case TokenType::LessEqual:
            return std::get<int>(left_value) <= std::get<int>(right_value);

        case TokenType::Greater:
            return std::get<int>(left_value) > std::get<int>(right_value);

        case TokenType::GreaterEqual:
            return std::get<int>(left_value) >= std::get<int>(right_value);

        default:
            throw std::runtime_error("Invalid operator");
        }
    }

private:
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};