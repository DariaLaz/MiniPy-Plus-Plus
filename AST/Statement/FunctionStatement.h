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
    FunctionStatement(std::string name, std::vector<std::string> parameters, std::unique_ptr<Statement> body)
        : name(std::move(name)), parameters(std::move(parameters)), body(std::move(body))
    {
    }

    std::optional<Value> execute(Environment &env) const override
    {
        auto function = std::make_shared<FunctionValue>();

        function->parameters = parameters;
        function->body = body.get();
        function->closure = &env;

        env.set(name, function);

        return std::nullopt;
    }

private:
    std::string name;
    std::vector<std::string> parameters;
    std::unique_ptr<Statement> body;
};