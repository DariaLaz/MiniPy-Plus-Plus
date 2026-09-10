#pragma once

#include "AST/Statement/Statement.h"
#include "Signals/BreakSignal.h"

class BreakStatement : public Statement
{
public:
  BreakStatement(SourceLocation location) : Statement(location) {}

  std::optional<Value> execute(Environment &) const override
  {
    throw BreakSignal{};
  }
};