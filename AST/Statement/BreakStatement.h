#pragma once

#include "AST/Statement/Statement.h"
#include "signals/BreakSignal.h"

class BreakStatement : public Statement
{
public:
    BreakStatement(SourceLocation location) : Statement(location) {}

    std::optional<Value> execute(Environment &env) const override
    {
        throw BreakSignal{};
    }
};