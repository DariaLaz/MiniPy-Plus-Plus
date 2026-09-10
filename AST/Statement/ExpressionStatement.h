#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"

#include <memory>
#include <optional>
#include <string>

class ExpressionStatement : public Statement
{
public:
  ExpressionStatement(std::unique_ptr<Expression> expression, SourceLocation location)
      : Statement(location), expression(std::move(expression))
  {
  }

  std::optional<Value> execute(Environment &env) const override
  {
    return expression->evaluate(env);
  }

private:
  std::unique_ptr<Expression> expression;
};