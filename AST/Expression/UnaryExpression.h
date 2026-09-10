#pragma once

#include "AST/Expression/Expression.h"
#include "Lexer/Token.h"

#include <memory>
#include <stdexcept>

class UnaryExpression : public Expression
{
public:
    Token op;
    std::unique_ptr<Expression> expr;

    UnaryExpression(Token op, std::unique_ptr<Expression> expr)
        : op(std::move(op)), expr(std::move(expr)), Expression({op.line, op.column})
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value value = expr->evaluate(env);

        switch (op.type)
        {
        case TokenType::Minus:
            validate_alternative<int>(value, get_location(),
                                      "Bad operand type for unary for '" + op.text + "'");
            return -std::get<int>(value);

        case TokenType::Plus:
            return value;

        case TokenType::Not:
            return !is_truthy(value);

        default:
            throw TypeError("Invalid unary operator", get_location());
        }
    }
};