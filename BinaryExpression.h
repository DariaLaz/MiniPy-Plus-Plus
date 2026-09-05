#pragma once

#include <memory>

#include "Expression.h"
#include "Token.h"

class BinaryExpression : public Expression
{
public:
    BinaryExpression(
        std::unique_ptr<Expression> left,
        Token op,
        std::unique_ptr<Expression> right)
        : left(std::move(left)),
          op(std::move(op)),
          right(std::move(right))
    {
    }

    std::unique_ptr<Expression> left;
    Token op;
    std::unique_ptr<Expression> right;
};