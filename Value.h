#pragma once

#include <variant>

using Value = std::variant<int, bool, std::string>;

inline bool is_truthy(const Value &value)
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

    return false;
}

inline Value add(const Value &left, const Value &right)
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
        throw std::runtime_error("Invalid operands for addition");
    }
}