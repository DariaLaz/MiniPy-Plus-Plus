#pragma once

#include "Expression.h"

class BoolExpression : public Expression
{
public:
  BoolExpression(bool value, const SourceLocation &location) : value(value), Expression(location)
  {
  }

  Value evaluate(Environment &env) const override
  {
    return value;
  }

private:
  bool value;
};