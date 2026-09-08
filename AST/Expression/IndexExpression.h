#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "AST/Expression/Expression.h"
#include "AST/utils.h"

class IndexExpression : public Expression
{
public:
    IndexExpression(std::unique_ptr<Expression> object, std::unique_ptr<Expression> index)
        : object(std::move(object)), index(std::move(index))
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value object_value = object->evaluate(env);
        Value index_value = index->evaluate(env);

        if (std::holds_alternative<std::shared_ptr<DictValue>>(object_value))
        {
            validate_alternative<std::string>(index_value, "Dict key must be a string");

            auto dict = std::get<std::shared_ptr<DictValue>>(object_value);
            auto &key = std::get<std::string>(index_value);

            auto val = dict->elements.find(key);
            if (val == dict->elements.end())
            {
                throw std::runtime_error("Dictionary key not found: " + key);
            }

            return val->second;
        }

        if (std::holds_alternative<std::shared_ptr<ListValue>>(object_value))
        {
            const auto &list = std::get<std::shared_ptr<ListValue>>(object_value);

            validate_alternative<int>(index_value, "Index must be an integer");

            int i = normalize_int_index(index_value, list->elements.size());

            return list->elements[i];
        }

        if (std::holds_alternative<std::string>(object_value))
        {
            const auto &str = std::get<std::string>(object_value);

            validate_alternative<int>(index_value, "Index must be an integer");

            int i = normalize_int_index(index_value, str.size());

            return std::string(1, str[i]);
        }

        throw std::runtime_error("Object is not indexable");
    }

private:
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> index;
};