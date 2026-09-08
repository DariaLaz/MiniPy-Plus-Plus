#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "AST/Statement/Statement.h"
#include "AST/Expression/Expression.h"
#include "AST/Environment.h"
#include "Value.h"

class IndexAssignmentStatement : public Statement
{
public:
    IndexAssignmentStatement(std::string list_name, std::unique_ptr<Expression> index, std::unique_ptr<Expression> value)
        : list_name(std::move(list_name)), index(std::move(index)), value(std::move(value))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value list_value = env.get(list_name);
        Value index_value = index->evaluate(env);

        if (!std::holds_alternative<std::shared_ptr<ListValue>>(list_value))
        {
            throw std::runtime_error("Object is not a list");
        }

        if (!std::holds_alternative<int>(index_value))
        {
            throw std::runtime_error("List index must be an integer");
        }

        auto list = std::get<std::shared_ptr<ListValue>>(list_value);

        int i = std::get<int>(index_value);

        if (i < 0 || i >= static_cast<int>(list->elements.size()))
        {
            throw std::runtime_error("List index out of range");
        }

        list->elements[i] = value->evaluate(env);

        return std::nullopt;
    }

private:
    std::string list_name;

    std::unique_ptr<Expression> index;
    std::unique_ptr<Expression> value;
};