#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "AST/Expression/Expression.h"

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

        if (!std::holds_alternative<int>(index_value))
        {
            throw std::runtime_error("List index must be an integer");
        }

        int i = std::get<int>(index_value);

        if (std::holds_alternative<std::shared_ptr<ListValue>>(object_value))
        {

            auto list = std::get<std::shared_ptr<ListValue>>(object_value);

            i = normalize_index(i, list->elements.size());

            if (i < 0 || i >= list->elements.size())
            {
                throw std::runtime_error("List index out of range");
            }

            return list->elements[i];
        }

        if (std::holds_alternative<std::string>(object_value))
        {
            const std::string &str = std::get<std::string>(object_value);

            i = normalize_index(i, str.size());

            if (i < 0 || i >= str.size())
            {
                throw std::runtime_error("String index out of range");
            }

            return std::string(1, str[i]);
        }

        throw std::runtime_error("Object is not indexable");
    }

private:
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> index;

    int normalize_index(int index, int size) const
    {
        if (index < 0)
        {
            index += size;
        }

        return index;
    }
};