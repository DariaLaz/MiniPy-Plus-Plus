#pragma once

#include "AST/Expression/Expression.h"
#include "Errors/TypeError.h"
#include "Errors/ZeroDivisionError.h"
#include "Lexer/Token.h"

#include <memory>

class BinaryExpression : public Expression
{
public:
    BinaryExpression(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right)
        : Expression({op.line, op.column}), left(std::move(left)), op(std::move(op)),
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
            return add(left_value, right_value, get_location());

        case TokenType::Minus:
            validate_operands<int>(left_value, right_value);
            return std::get<int>(left_value) - std::get<int>(right_value);

        case TokenType::Star:
            validate_operands<int>(left_value, right_value);
            return std::get<int>(left_value) * std::get<int>(right_value);

        case TokenType::Slash:
            validate_operands<int>(left_value, right_value);
            if (std::get<int>(right_value) == 0)
                throw ZeroDivisionError("Division by zero", get_location());

            return std::get<int>(left_value) / std::get<int>(right_value);

        case TokenType::EqualEqual:
            return left_value == right_value;

        case TokenType::NotEqual:
            return left_value != right_value;

        case TokenType::Less:
            validate_operands<int>(left_value, right_value);
            return std::get<int>(left_value) < std::get<int>(right_value);

        case TokenType::LessEqual:
            validate_operands<int>(left_value, right_value);
            return std::get<int>(left_value) <= std::get<int>(right_value);

        case TokenType::Greater:
            validate_operands<int>(left_value, right_value);
            return std::get<int>(left_value) > std::get<int>(right_value);

        case TokenType::GreaterEqual:
            validate_operands<int>(left_value, right_value);
            return std::get<int>(left_value) >= std::get<int>(right_value);

        default:
            throw std::logic_error("Invalid operator");
        }
    }

private:
    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;

    template <typename T>
    void validate_operands(const Value &left, const Value &right) const
    {
        if (!std::holds_alternative<T>(left) || !std::holds_alternative<T>(right))
        {
            throw TypeError("Unsupported operand types for '" + op.text + "'", get_location());
        }
    }
};