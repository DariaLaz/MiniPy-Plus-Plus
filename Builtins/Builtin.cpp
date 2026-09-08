#include "Builtins/Builtin.h"

std::shared_ptr<BuildinFunctionValue> Builtin::make()
{
    auto native = std::make_shared<BuildinFunctionValue>();

    native->name = name();

    auto self = shared_from_this();
    native->function = [self](const std::vector<Value> &args) -> Value
    {
        return self->func(args);
    };

    return native;
}