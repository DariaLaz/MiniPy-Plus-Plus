#pragma once

#include "AST/Statement/Statement.h"
#include "Signals/ContinueSignal.h"

class ContinueStatement : public Statement
{
public:
  ContinueStatement(SourceLocation location) : Statement(location) {}

  std::optional<Value> execute(Environment &) const override
  {
    throw ContinueSignal{};
  }
};