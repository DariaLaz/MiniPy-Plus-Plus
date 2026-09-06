#pragma once

#include <iostream>
#include <memory>
#include <optional>

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"

class PrintStatement : public Statement
{
public:
    PrintStatement(std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value value = expression->evaluate(env);

        if (std::holds_alternative<int>(value))
        {
            std::cout << std::get<int>(value);
        }
        else if (std::holds_alternative<bool>(value))
        {
            std::cout << (std::get<bool>(value) ? "True" : "False");
        }
        else if (std::holds_alternative<std::string>(value))
        {
            std::cout << std::get<std::string>(value);
        }

        std::cout << '\n';

        return std::nullopt;
    }

private:
    std::unique_ptr<Expression> expression;
};