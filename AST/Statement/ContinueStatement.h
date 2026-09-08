#pragma once

#include "AST/Statement/Statement.h"
#include "signals/ContinueSignal.h"

class ContinueStatement : public Statement
{
public:
    std::optional<Value> execute(Environment &env) const override
    {
        throw ContinueSignal{};
    }
};