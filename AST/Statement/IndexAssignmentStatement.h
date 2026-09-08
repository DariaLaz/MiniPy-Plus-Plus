#pragma once

#include <memory>
#include <stdexcept>
#include <string>

#include "AST/Statement/Statement.h"
#include "AST/Expression/Expression.h"
#include "AST/Environment.h"
#include "Value.h"
#include "AST/utils.h"

class IndexAssignmentStatement : public Statement
{
public:
    IndexAssignmentStatement(std::string object_name, std::unique_ptr<Expression> index, std::unique_ptr<Expression> value)
        : object_name(std::move(object_name)), index(std::move(index)), value(std::move(value))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value object_value = env.get(object_name);
        Value index_value = index->evaluate(env);

        if (std::holds_alternative<std::shared_ptr<ListValue>>(object_value))
        {

            auto &list = std::get<std::shared_ptr<ListValue>>(object_value);

            validate_alternative<int>(index_value, "Index must be an integer");

            int i = normalize_int_index(index_value, list->elements.size());

            if (i < 0 || i >= static_cast<int>(list->elements.size()))
            {
                throw std::runtime_error("List index out of range");
            }

            list->elements[i] = value->evaluate(env);

            return std::nullopt;
        }

        if (std::holds_alternative<std::shared_ptr<DictValue>>(object_value))
        {
            validate_alternative<std::string>(index_value, "Dict key must be a string");

            auto dict = std::get<std::shared_ptr<DictValue>>(object_value);

            std::string key = std::get<std::string>(index_value);

            dict->elements[key] = value->evaluate(env);

            return std::nullopt;
        }

        throw std::runtime_error("Object should be indexable (list, dict)");
    }

private:
    std::string object_name;

    std::unique_ptr<Expression> index;
    std::unique_ptr<Expression> value;
};