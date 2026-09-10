#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"

#include <memory>
#include <optional>
#include <string>

class ExpressionStatement : public Statement
{
public:
  ExpressionStatement(std::unique_ptr<Expression> expression, SourceLocation location)
      : expression(std::move(expression)), Statement(location)
  {
  }

  std::optional<Value> execute(Environment &env) const override
  {
    return expression->evaluate(env);
  }

private:
  std::unique_ptr<Expression> expression;
};