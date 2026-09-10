#pragma once

#include "AST/Expression/Expression.h"

#include <string>

class StringExpression : public Expression
{
public:
  StringExpression(std::string value, const SourceLocation &location)
      : Expression(location), value(std::move(value))
  {
  }

  Value evaluate(Environment &) const override
  {
    return value;
  }

private:
  std::string value;
};