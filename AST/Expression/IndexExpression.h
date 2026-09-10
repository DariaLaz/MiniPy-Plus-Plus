#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "AST/Expression/Expression.h"
#include "AST/utils.h"
#include "Errors/TypeError.h"
#include "Errors/KeyError.h"

class IndexExpression : public Expression
{
public:
    IndexExpression(std::unique_ptr<Expression> object, std::unique_ptr<Expression> index, SourceLocation location)
        : Expression(location), object(std::move(object)), index(std::move(index))
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value object_value = object->evaluate(env);
        Value index_value = index->evaluate(env);

        if (std::holds_alternative<std::shared_ptr<DictValue>>(object_value))
        {
            validate_alternative<std::string>(index_value, index->get_location(), "Dict key must be a string");

            auto dict = std::get<std::shared_ptr<DictValue>>(object_value);
            auto &key = std::get<std::string>(index_value);

            auto val = dict->elements.find(key);
            if (val == dict->elements.end())
            {
                throw KeyError("Dictionary key not found: " + key, index->get_location());
            }

            return val->second;
        }

        if (std::holds_alternative<std::shared_ptr<ListValue>>(object_value))
        {
            const auto &list = std::get<std::shared_ptr<ListValue>>(object_value);

            validate_alternative<int>(index_value, index->get_location(), "Index must be an integer");

            int i = normalize_int_index(index_value, list->elements.size(), get_location());

            return list->elements[i];
        }

        if (std::holds_alternative<std::string>(object_value))
        {
            const auto &str = std::get<std::string>(object_value);

            validate_alternative<int>(index_value, index->get_location(), "Index must be an integer");

            int i = normalize_int_index(index_value, str.size(), get_location());

            return std::string(1, str[i]);
        }

        throw TypeError("Object is not indexable", get_location());
    }

private:
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> index;
};