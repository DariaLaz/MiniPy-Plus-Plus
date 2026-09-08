#include "Builtins/AppendBuiltin.h"

#include <stdexcept>

std::string AppendBuiltin::name() const
{
    return "append";
}

Value AppendBuiltin::func(
    const std::vector<Value> &args) const
{
    if (args.size() != 2)
    {
        throw std::runtime_error("append() expects 2 arguments");
    }

    validate_alternative<std::shared_ptr<ListValue>>(args[0], "append() first argument must be a list");

    auto list = std::get<std::shared_ptr<ListValue>>(args[0]);

    list->elements.push_back(args[1]);

    return std::monostate{};
}