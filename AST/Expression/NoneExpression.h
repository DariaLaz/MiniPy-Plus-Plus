#pragma once

#include "AST/Expression/Expression.h"
#include "Runtime/Value.h"

class NoneExpression : public Expression
{
public:
  NoneExpression(const SourceLocation &location) : Expression(location) {}

  Value evaluate(Environment &) const override
  {
    return std::monostate{};
  }
};