#pragma once

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "Runtime/Environment.h"
#include "Runtime/Value.h"

#include <memory>
#include <stdexcept>
#include <string>

class ForStatement : public Statement
{
public:
    ForStatement(std::string variable, std::unique_ptr<Expression> iterable,
                 std::unique_ptr<Statement> body, SourceLocation location)
        : variable(std::move(variable)), iterable(std::move(iterable)), body(std::move(body)),
          Statement(location)
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value value = iterable->evaluate(env);

        validate_alternative<std::shared_ptr<ListValue>>(value, iterable->get_location(),
                                                         "Object is not iterable");

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

    const Statement &get_body() const
    {
        return *body;
    }

private:
    std::string variable;
    std::unique_ptr<Expression> iterable;
    std::unique_ptr<Statement> body;
};