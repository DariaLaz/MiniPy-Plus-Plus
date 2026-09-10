#pragma once

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "Runtime/Value.h"

#include <memory>
#include <optional>
#include <signals/BreakSignal.h>
#include <signals/ContinueSignal.h>

class WhileStatement : public Statement
{
public:
    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body,
                   SourceLocation location)
        : Statement(location), condition(std::move(condition)), body(std::move(body))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        while (is_truthy(condition->evaluate(env)))
        {
            try
            {
                body->execute(env);
            }
            catch (const ContinueSignal &)
            {
                continue;
            }
            catch (const BreakSignal &)
            {
                break;
            }
        }

        return std::nullopt;
    }

    const Statement &get_body() const
    {
        return *body;
    }

private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;
};