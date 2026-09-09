#include "Builtins/AppendBuiltin.h"
#include "Errors/TypeError.h"

#include <stdexcept>

std::string AppendBuiltin::name() const
{
    return "append";
}

Value AppendBuiltin::func(const std::vector<Value> &args, const SourceLocation &location) const
{
    if (args.size() != 2)
    {
        throw TypeError("append() expects 2 arguments", location);
    }

    validate_alternative<std::shared_ptr<ListValue>>(args[0], location, "append() first argument must be a list");

    auto list = std::get<std::shared_ptr<ListValue>>(args[0]);

    list->elements.push_back(args[1]);

    return std::monostate{};
}