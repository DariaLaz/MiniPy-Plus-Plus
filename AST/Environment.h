#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

#include "Value.h"

class Environment
{
public:
    void set(const std::string &name, Value value)
    {
        values[name] = value;
    }

    Value get(const std::string &name) const
    {
        auto val = values.find(name);

        if (val == values.end())
        {
            throw std::runtime_error("Undefined variable: " + name);
        }

        return val->second;
    }

private:
    std::unordered_map<std::string, Value> values;
};