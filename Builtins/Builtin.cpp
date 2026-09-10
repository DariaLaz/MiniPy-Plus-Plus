#include "Builtins/Builtin.h"

std::shared_ptr<BuiltinFunctionValue> Builtin::make()
{
    auto builtin = std::make_shared<BuiltinFunctionValue>();

    builtin->name = name();

    auto self = shared_from_this();
    builtin->function = [self](const std::vector<Value> &args, SourceLocation location)
    { return self->func(args, location); };

    return builtin;
}