#include "Builtins/IntBuiltin.h"

#include <stdexcept>

std::string IntBuiltin::name() const
{
    return "int";
}

Value IntBuiltin::func(
    const std::vector<Value> &args) const
{
    if (args.size() != 1)
    {
        throw std::runtime_error("int() expects 1 argument");
    }

    const Value &value = args[0];

    if (std::holds_alternative<int>(value))
    {
        return std::get<int>(value);
    }

    if (std::holds_alternative<bool>(value))
    {
        return std::get<bool>(value) ? 1 : 0;
    }

    if (std::holds_alternative<std::string>(value))
    {
        const std::string &text = std::get<std::string>(value);

        try
        {
            std::size_t pos = 0;

            int result = std::stoi(text, &pos);

            return result;
        }
        catch (...)
        {
            throw std::runtime_error("invalid literal for int(): " + text);
        }
    }

    throw std::runtime_error("int() unsupported type");
}