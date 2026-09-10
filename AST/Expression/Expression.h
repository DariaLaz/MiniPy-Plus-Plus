#pragma once

#include "Runtime/Environment.h"
#include "Runtime/Value.h"

class Expression
{
public:
    Expression(const SourceLocation &location) : location(location) {}
    virtual ~Expression() noexcept = default;
    virtual Value evaluate(Environment &) const = 0;

    SourceLocation get_location() const
    {
        return location;
    }

private:
    SourceLocation location;
};