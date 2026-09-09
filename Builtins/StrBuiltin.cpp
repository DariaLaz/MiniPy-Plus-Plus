#include "Builtins/StrBuiltin.h"
#include "Errors/ValueError.h"

#include <stdexcept>

std::string StrBuiltin::name() const
{
    return "str";
}

Value StrBuiltin::func(const std::vector<Value> &args, const SourceLocation &location) const
{
    if (args.size() != 1)
    {
        throw ValueError("str() expects 1 argument", location);
    }

    return value_to_string(args[0], location);
}