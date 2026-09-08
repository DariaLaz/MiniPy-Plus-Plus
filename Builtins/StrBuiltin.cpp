#include "Builtins/StrBuiltin.h"

#include <stdexcept>

std::string StrBuiltin::name() const
{
    return "str";
}

Value StrBuiltin::func(const std::vector<Value> &args) const
{
    if (args.size() != 1)
    {
        throw std::runtime_error("str() expects 1 argument");
    }

    return value_to_string(args[0]);
}