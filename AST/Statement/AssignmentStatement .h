#include <memory>
#include <optional>
#include <string>

#include "AST/Statement/Statement.h"
#include "AST/Expression/Expression.h"

class AssignmentStatement : public Statement
{
public:
    AssignmentStatement(
        std::string name,
        std::unique_ptr<Expression> value)
        : name(std::move(name)), value(std::move(value))
    {
    }

    std::optional<int> execute(Environment &env) const override
    {
        env.set(name, value->evaluate(env));

        return std::nullopt;
    }

private:
    std::string name;
    std::unique_ptr<Expression> value;
};