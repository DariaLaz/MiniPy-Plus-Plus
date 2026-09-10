#pragma once

#include "Errors/TypeError.h"
#include "Expression.h"
#include "signals/ReturnSignal.h"

class CallExpression : public Expression
{
public:
    CallExpression(std::unique_ptr<Expression> callee,
                   std::vector<std::unique_ptr<Expression>> arguments, SourceLocation location)
        : Expression(location), callee(std::move(callee)), arguments(std::move(arguments))
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
                throw TypeError("Wrong number of arguments", get_location());
            }

            auto local_env = std::make_shared<Environment>(function->closure);

            for (std::size_t i = 0; i < argument_values.size(); ++i)
            {
                local_env->set(function->parameters[i], argument_values[i]);
            }

            try
            {
                function->body->execute(*local_env);
            }
            catch (const ReturnSignal &signal)
            {
                return signal.value;
            }

            return std::monostate{};
        }

        if (std::holds_alternative<std::shared_ptr<BuiltinFunctionValue>>(callee_value))
        {
            auto function = std::get<std::shared_ptr<BuiltinFunctionValue>>(callee_value);
            return function->function(argument_values, get_location());
        }

        throw TypeError("Object is not callable", get_location());
    }

private:
    std::unique_ptr<Expression> callee;

    std::vector<std::unique_ptr<Expression>> arguments;
};