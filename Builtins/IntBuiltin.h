#pragma once

#include "Builtins/Builtin.h"

class IntBuiltin : public Builtin
{
public:
    std::string name() const override;

    Value func(const std::vector<Value> &args, const SourceLocation &location) const override;
};