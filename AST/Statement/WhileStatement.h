#pragma once

#include <memory>
#include <optional>

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "Value.h"

class WhileStatement : public Statement
{
public:
    WhileStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Statement> body)
        : condition(std::move(condition)),
          body(std::move(body))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        while (is_truthy(condition->evaluate(env)))
        {
            body->execute(env);
        }

        return std::nullopt;
    }

private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
};