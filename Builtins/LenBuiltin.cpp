
#include "Builtins/Builtin.h"
#include "Builtins/LenBuiltin.h"

std::string LenBuiltin::name() const
{
    return "len";
}

Value LenBuiltin::func(const std::vector<Value> &args) const
{
    if (args.size() != 1)
    {
        throw std::runtime_error("len() expects 1 argument");
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

    if (std::holds_alternative<
            std::shared_ptr<DictValue>>(value))
    {
        auto dict = std::get<std::shared_ptr<DictValue>>(value);
        return static_cast<int>(dict->elements.size());
    }

    throw std::runtime_error("Object has no len()");
}
