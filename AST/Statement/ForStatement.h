#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "AST/Environment.h"
#include "Value.h"

class ForStatement : public Statement
{
public:
    ForStatement(std::string variable, std::unique_ptr<Expression> iterable, std::unique_ptr<Statement> body)
        : variable(std::move(variable)), iterable(std::move(iterable)), body(std::move(body))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value value = iterable->evaluate(env);

        if (!std::holds_alternative<std::shared_ptr<ListValue>>(value))
        {
            throw std::runtime_error("Object is not iterable");
        }

        auto list = std::get<std::shared_ptr<ListValue>>(value);

        for (const Value &element : list->elements)
        {
            env.set(variable, element);
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

private:
    std::string variable;
    std::unique_ptr<Expression> iterable;
    std::unique_ptr<Statement> body;
};