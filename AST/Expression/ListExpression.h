#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "AST/Expression/Expression.h"

class ListExpression : public Expression
{
public:
    ListExpression(
        std::vector<std::unique_ptr<Expression>> elements)
        : elements(std::move(elements))
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