#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

#include "AST/Expression/Expression.h"
#include "Value.h"

class SliceExpression : public Expression
{
public:
    SliceExpression(std::unique_ptr<Expression> object, std::unique_ptr<Expression> start, std::unique_ptr<Expression> end, const SourceLocation &location)
        : object(std::move(object)), start(std::move(start)), end(std::move(end)), Expression(location)
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value object_value = object->evaluate(env);

        if (std::holds_alternative<std::shared_ptr<ListValue>>(object_value))
        {
            auto list = std::get<std::shared_ptr<ListValue>>(object_value);

            int size = static_cast<int>(list->elements.size());
            int start_index = get_index(start.get(), env, 0);
            int end_index = get_index(end.get(), env, size);

            normalize_bounds(start_index, end_index, size);

            auto result = std::make_shared<ListValue>();

            for (int i = start_index; i < end_index; i++)
            {
                result->elements.push_back(list->elements[i]);
            }

            return result;
        }

        if (std::holds_alternative<std::string>(object_value))
        {
            const std::string &str = std::get<std::string>(object_value);

            int size = static_cast<int>(str.size());
            int start_index = get_index(start.get(), env, 0);
            int end_index = get_index(end.get(), env, size);

            normalize_bounds(start_index, end_index, size);

            return str.substr(start_index, end_index - start_index);
        }

        throw TypeError("Object is not sliceable", get_location());
    }

private:
    std::unique_ptr<Expression> object;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;

    int get_index(const Expression *expression, Environment &env, int default_value) const
    {
        if (expression == nullptr)
        {
            return default_value;
        }

        Value value = expression->evaluate(env);

        validate_alternative<int>(value, expression->get_location(), "Slice indices must be integers");

        return std::get<int>(value);
    }

    void normalize_bounds(int &start, int &end, int size) const
    {
        if (start < 0)
            start += size;

        if (end < 0)
            end += size;

        start = std::clamp(start, 0, size);
        end = std::clamp(end, 0, size);

        if (end < start)
        {
            end = start;
        }
    }
};