#include "Value.h"
#include "Errors/TypeError.h"
#include "Utils/SourceLocation.h"

#include <iostream>
#include <variant>
#include <vector>
#include <memory>

std::string value_to_string(const Value &value, const SourceLocation &location)
{
    if (std::holds_alternative<std::monostate>(value))
    {
        return "None";
    }

    if (std::holds_alternative<int>(value))
    {
        return std::to_string(std::get<int>(value));
    }

    if (std::holds_alternative<bool>(value))
    {
        return std::get<bool>(value) ? "True" : "False";
    }

    if (std::holds_alternative<std::string>(value))
    {
        return std::get<std::string>(value);
    }

    if (std::holds_alternative<std::shared_ptr<ListValue>>(value))
    {
        auto list = std::get<std::shared_ptr<ListValue>>(value);

        std::string result = "[";

        for (std::size_t i = 0; i < list->elements.size(); ++i)
        {
            result += value_to_string(list->elements[i], location);

            if (i + 1 < list->elements.size())
            {
                result += ", ";
            }
        }

        result += "]";

        return result;
    }

    if (std::holds_alternative<std::shared_ptr<FunctionValue>>(value))
    {
        return "<function>";
    }

    if (std::holds_alternative<std::shared_ptr<BuildinFunctionValue>>(value))
    {
        return "<built-in function>";
    }

    if (std::holds_alternative<std::shared_ptr<DictValue>>(value))
    {
        auto dict = std::get<std::shared_ptr<DictValue>>(value);

        std::string result = "{";

        std::size_t i = 0;

        for (const auto &[key, val] : dict->elements)
        {
            result += key;
            result += ": ";
            result += value_to_string(val, location);

            if (++i < dict->elements.size())
            {
                result += ", ";
            }
        }

        result += "}";

        return result;
    }

    throw TypeError("Cannot convert value to string", location);
}

bool is_truthy(const Value &value)
{
    if (std::holds_alternative<bool>(value))
    {
        return std::get<bool>(value);
    }

    if (std::holds_alternative<int>(value))
    {
        return std::get<int>(value) != 0;
    }

    if (std::holds_alternative<std::string>(value))
    {
        return !std::get<std::string>(value).empty();
    }

    if (std::holds_alternative<std::monostate>(value))
    {
        return false;
    }

    if (std::holds_alternative<std::shared_ptr<DictValue>>(value))
    {
        auto dict = std::get<std::shared_ptr<DictValue>>(value);
        return !dict->elements.empty();
    }

    return false;
}

Value add(const Value &left, const Value &right, const SourceLocation &location)
{
    if (std::holds_alternative<int>(left) && std::holds_alternative<int>(right))
    {
        return std::get<int>(left) + std::get<int>(right);
    }
    else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
    {
        return std::get<std::string>(left) + std::get<std::string>(right);
    }
    else
    {
        throw TypeError("Invalid operands for addition", location);
    }
}
