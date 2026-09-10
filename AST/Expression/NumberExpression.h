#pragma once
#include "AST/Expression/Expression.h"

class NumberExpression : public Expression
{
public:
  int value;

  NumberExpression(int value, const SourceLocation &location) : Expression(location), value(value)
  {
  }

  Value evaluate(Environment &) const override
  {
    return value;
  }
};