#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AST/Statement/Statement.h"
#include "AST/Environment.h"
#include "Value.h"

class FunctionStatement : public Statement
{
public:
    FunctionStatement(std::string name, std::vector<std::string> parameters, std::unique_ptr<Statement> body, SourceLocation location)
        : name(std::move(name)), parameters(std::move(parameters)), body(std::move(body)), Statement(location)
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        auto function = std::make_shared<FunctionValue>();

        function->parameters = parameters;
        function->body = body.get();
        function->closure = env.shared_from_this();

        env.set(name, function);

        return std::nullopt;
    }

    const Statement &get_body() const
    {
        return *body;
    }

    const std::vector<std::string> &get_parameters() const
    {
        return parameters;
    }

private:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<Statement> body;
};