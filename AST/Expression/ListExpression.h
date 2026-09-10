#pragma once

#include "AST/Expression/Expression.h"

#include <memory>
#include <utility>
#include <vector>

class ListExpression : public Expression
{
public:
  ListExpression(std::vector<std::unique_ptr<Expression>> elements,
                 const SourceLocation &location)
      : Expression(location), elements(std::move(elements))
  {
  }

  Value evaluate(Environment &env) const override
  {
    auto list = std::make_shared<ListValue>();

    for (const auto &element : elements)
    {
      list->elements.push_back(element->evaluate(env));
    }

    return list;
  }

private:
  std::vector<std::unique_ptr<Expression>> elements;
};