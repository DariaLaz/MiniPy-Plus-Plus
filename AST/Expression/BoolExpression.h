#pragma once

#include "Expression.h"

class BoolExpression : public Expression
{
public:
  BoolExpression(bool value, const SourceLocation &location) : Expression(location), value(value)
  {
  }

  Value evaluate(Environment &) const override
  {
    return value;
  }

private:
  bool value;
};