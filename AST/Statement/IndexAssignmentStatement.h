#pragma once

#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "AST/utils.h"
#include "Errors/TypeError.h"
#include "Runtime/Environment.h"
#include "Runtime/Value.h"

#include <memory>
#include <stdexcept>
#include <string>

class IndexAssignmentStatement : public Statement
{
public:
    IndexAssignmentStatement(std::string object_name, std::unique_ptr<Expression> index,
                             std::unique_ptr<Expression> value, SourceLocation location)
        : object_name(std::move(object_name)), index(std::move(index)), value(std::move(value)),
          Statement(location)
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        Value object_value = env.get(object_name, index->get_location());
        Value index_value = index->evaluate(env);

        if (std::holds_alternative<std::shared_ptr<ListValue>>(object_value))
        {

            auto &list = std::get<std::shared_ptr<ListValue>>(object_value);

            validate_alternative<int>(index_value, index->get_location(),
                                      "Index must be an integer");

            int i = normalize_int_index(index_value, list->elements.size(), get_location());

            if (i < 0 || i >= static_cast<int>(list->elements.size()))
            {
                throw IndexError("List index out of range", index->get_location());
            }

            list->elements[i] = value->evaluate(env);

            return std::nullopt;
        }

        if (std::holds_alternative<std::shared_ptr<DictValue>>(object_value))
        {
            validate_alternative<std::string>(index_value, index->get_location(),
                                              "Dict key must be a string");

            auto dict = std::get<std::shared_ptr<DictValue>>(object_value);

            std::string key = std::get<std::string>(index_value);

            dict->elements[key] = value->evaluate(env);

            return std::nullopt;
        }

        throw TypeError("Object should be indexable (list, dict)", get_location());
    }

private:
    std::string object_name;

    std::unique_ptr<Expression> index;
    std::unique_ptr<Expression> value;
};