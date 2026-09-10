#pragma once

#include "AST/Statement/Statement.h"

#include <memory>
#include <optional>
#include <utility>
#include <vector>

class BlockStatement : public Statement
{
public:
    BlockStatement(std::vector<std::unique_ptr<Statement>> statements, SourceLocation location)
        : statements(std::move(statements)), Statement(location)
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        std::optional<Value> result = std::nullopt;

        for (const auto &statement : statements)
        {
            auto current_result = statement->execute(env);

            if (current_result.has_value())
            {
                result = current_result;
            }
        }

        return result;
    }

    const std::vector<std::unique_ptr<Statement>> &get_statements() const
    {
        return statements;
    }

private:
    std::vector<std::unique_ptr<Statement>> statements;
};