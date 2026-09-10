#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"

#include <memory>
#include <optional>
#include <string>

class AssignmentStatement : public Statement
{
public:
  AssignmentStatement(std::string name, std::unique_ptr<Expression> value,
                      SourceLocation location)
      : Statement(location), name(std::move(name)), value(std::move(value))
  {
  }

  std::optional<Value> execute(Environment &env) const override
  {
    env.set(name, value->evaluate(env));

    return std::nullopt;
  }

private:
  std::string name;
  std::unique_ptr<Expression> value;
};