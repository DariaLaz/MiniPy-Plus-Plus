#include <memory>
#include <optional>
#include <string>

#include "AST/Statement/Statement.h"
#include "AST/Expression/Expression.h"

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(
        std::unique_ptr<Expression> expression)
        : expression(std::move(expression))
    {
    }

    std::optional<int> execute(Environment &env) const override
    {
        return expression->evaluate(env);
    }

private:
    std::unique_ptr<Expression> expression;
};