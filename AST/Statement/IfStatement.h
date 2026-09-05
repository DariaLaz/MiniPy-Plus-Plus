#pragma once

#include <memory>
#include <optional>

#include "Statement.h"
#include "AST/Expression/Expression.h"
#include "Value.h"

class IfStatement : public Statement
{
public:
    IfStatement(
        std::unique_ptr<Expression> condition,
        std::unique_ptr<Statement> then,
        std::unique_ptr<Statement> else_branch = nullptr)
        : condition(std::move(condition)),
          then(std::move(then)),
          else_branch(std::move(else_branch))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value result = condition->evaluate(env);

        if (is_truthy(result))
        {
            return then->execute(env);
        }

        if (else_branch)
        {
            return else_branch->execute(env);
        }

        return std::nullopt;
    }

private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> then;
    std::vector<std::unique_ptr<Statement>> elifs;
    std::unique_ptr<Statement> else_branch;
};