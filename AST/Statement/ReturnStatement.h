#pragma once

#include <memory>
#include <optional>

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "signals/ReturnSignal.h"
#include "Value.h"

class ReturnStatement : public Statement
{
public:
    ReturnStatement(std::unique_ptr<Expression> value, SourceLocation location)
        : value(std::move(value)), Statement(location)
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        if (value)
        {
            throw ReturnSignal{value->evaluate(env)};
        }

        throw ReturnSignal{Value{std::monostate{}}};
    }

private:
    std::unique_ptr<Expression> value;
};