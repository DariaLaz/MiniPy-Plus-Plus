#include "Builtins/RangeBuiltin.h"

std::string RangeBuiltin::name() const
{
    return "range";
}

Value RangeBuiltin::func(const std::vector<Value> &args) const
{
    if (args.empty() || args.size() > 3)
    {
        throw std::runtime_error("range() expects 1 to 3 arguments");
    }

    for (const Value &arg : args)
    {
        validate_alternative<int>(arg, "range() arguments must be integers");
    }

    int start = 0;
    int stop = 0;
    int step = 1;

    if (args.size() == 1)
    {
        stop = std::get<int>(args[0]);
    }
    else if (args.size() == 2)
    {
        start = std::get<int>(args[0]);
        stop = std::get<int>(args[1]);
    }
    else
    {
        start = std::get<int>(args[0]);
        stop = std::get<int>(args[1]);
        step = std::get<int>(args[2]);
    }

    if (step == 0)
    {
        throw std::runtime_error("range() step cannot be zero");
    }

    auto result = std::make_shared<ListValue>();

    if (step > 0)
    {
        for (int i = start; i < stop; i += step)
        {
            result->elements.push_back(i);
        }
    }
    else
    {
        for (int i = start; i > stop; i += step)
        {
            result->elements.push_back(i);
        }
    }

    return result;
}
