#pragma once

#include "Expression.h"
#include "signals/ReturnSignal.h"

class CallExpression : public Expression
{
public:
    CallExpression(std::unique_ptr<Expression> callee, std::vector<std::unique_ptr<Expression>> arguments)
        : callee(std::move(callee)), arguments(std::move(arguments))
    {
    }

    Value evaluate(Environment &env) const override
    {
        Value callee_value = callee->evaluate(env);

        if (!std::holds_alternative<std::shared_ptr<FunctionValue>>(callee_value))
        {
            throw std::runtime_error("Object is not callable");
        }

        auto function = std::get<std::shared_ptr<FunctionValue>>(callee_value);

        if (arguments.size() != function->parameters.size())
        {
            throw std::runtime_error("Wrong number of arguments");
        }

        Environment local_env(function->closure);

        for (std::size_t i = 0; i < arguments.size(); ++i)
        {
            Value argument = arguments[i]->evaluate(env);

            local_env.set(function->parameters[i], argument);
        }

        try
        {
            function->body->execute(local_env);
        }
        catch (const ReturnSignal &signal)
        {
            return signal.value;
        }

        return Value{std::monostate{}};
    }

private:
    std::unique_ptr<Expression> callee;

    std::vector<std::unique_ptr<Expression>> arguments;
};