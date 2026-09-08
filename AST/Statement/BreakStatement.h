#pragma once

#include "AST/Statement/Statement.h"
#include "signals/BreakSignal.h"

class BreakStatement : public Statement
{
public:
    std::optional<Value> execute(Environment &env) const override
    {
        throw BreakSignal{};
    }
};