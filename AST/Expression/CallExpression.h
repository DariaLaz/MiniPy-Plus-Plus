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

        std::vector<Value> argument_values;

        for (const auto &argument : arguments)
        {
            argument_values.push_back(argument->evaluate(env));
        }

        if (std::holds_alternative<std::shared_ptr<FunctionValue>>(callee_value))
        {
            auto function = std::get<std::shared_ptr<FunctionValue>>(callee_value);

            if (arguments.size() != function->parameters.size())
            {
                throw std::runtime_error("Wrong number of arguments");
            }

            Environment local_env(function->closure);

            for (std::size_t i = 0; i < argument_values.size(); ++i)
            {
                local_env.set(function->parameters[i], argument_values[i]);
            }

            try
            {
                function->body->execute(local_env);
            }
            catch (const ReturnSignal &signal)
            {
                return signal.value;
            }

            return std::monostate{};
        }

        if (std::holds_alternative<std::shared_ptr<BuildinFunctionValue>>(callee_value))
        {
            auto function = std::get<std::shared_ptr<BuildinFunctionValue>>(callee_value);
            return function->function(argument_values);
        }

        throw std::runtime_error("Object is not callable");
    }

private:
    std::unique_ptr<Expression> callee;

    std::vector<std::unique_ptr<Expression>> arguments;
};