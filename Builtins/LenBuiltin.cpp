
#include "Builtins/LenBuiltin.h"

#include "Builtins/Builtin.h"
#include "Errors/TypeError.h"

std::string LenBuiltin::name() const
{
    return "len";
}

Value LenBuiltin::func(const std::vector<Value> &args, const SourceLocation &location) const
{
    if (args.size() != 1)
    {
        throw TypeError("len() expects 1 argument", location);
    }

    const Value &value = args[0];

    if (std::holds_alternative<std::string>(value))
    {
        return static_cast<int>(std::get<std::string>(value).size());
    }

    if (std::holds_alternative<std::shared_ptr<ListValue>>(value))
    {
        auto list = std::get<std::shared_ptr<ListValue>>(value);
        return static_cast<int>(list->elements.size());
    }

    if (std::holds_alternative<std::shared_ptr<DictValue>>(value))
    {
        auto dict = std::get<std::shared_ptr<DictValue>>(value);
        return static_cast<int>(dict->elements.size());
    }

    throw TypeError("Object has no len()", location);
}
