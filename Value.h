#pragma once

#include <variant>

using Value = std::variant<int, bool>;

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

    return false;
}