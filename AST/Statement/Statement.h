#pragma once

#include <optional>

#include "AST/Environment.h"

class Statement
{
public:
    Statement(SourceLocation location) : location(location)
    {
    }
    virtual ~Statement() = default;

    virtual std::optional<Value> execute(Environment &env) const = 0;

    SourceLocation get_location() const
    {
        return location;
    }

private:
    SourceLocation location;
};