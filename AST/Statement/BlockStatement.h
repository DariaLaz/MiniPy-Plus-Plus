#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "AST/Statement/Statement.h"

class BlockStatement : public Statement
{
public:
    BlockStatement(
        std::vector<std::unique_ptr<Statement>> statements)
        : statements(std::move(statements))
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

private:
    std::vector<std::unique_ptr<Statement>> statements;
};