#include "Value.h"

#include <iostream>
#include <variant>
#include <vector>
#include <memory>

void print_value(const Value &value)
{
    if (std::holds_alternative<std::monostate>(value))
    {
        std::cout << "None";
    }
    else if (std::holds_alternative<int>(value))
    {
        std::cout << std::get<int>(value);
    }
    else if (std::holds_alternative<bool>(value))
    {
        std::cout << (std::get<bool>(value) ? "True" : "False");
    }
    else if (std::holds_alternative<std::string>(value))
    {
        std::cout << std::get<std::string>(value);
    }
    else if (
        std::holds_alternative<std::shared_ptr<ListValue>>(value))
    {
        auto list = std::get<std::shared_ptr<ListValue>>(value);

        std::cout << "[";

        for (std::size_t i = 0; i < list->elements.size(); ++i)
        {
            print_value(list->elements[i]);

            if (i + 1 < list->elements.size())
            {
                std::cout << ", ";
            }
        }

        std::cout << "]";
    }
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

    return false;
}

Value add(const Value &left, const Value &right)
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