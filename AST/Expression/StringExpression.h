#pragma once

#include "AST/Expression/Expression.h"

#include <string>

class StringExpression : public Expression
{
public:
  StringExpression(std::string value, const SourceLocation &location)
      : value(std::move(value)), Expression(location)
  {
  }

  Value evaluate(Environment &) const override
  {
    return value;
  }

private:
  std::string value;
};