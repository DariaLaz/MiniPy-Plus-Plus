#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

#include "Value.h"

class Environment
{
public:
    Environment(Environment *parent = nullptr) : parent(parent)
    {
    }

    void set(const std::string &name, Value value)
    {
        values[name] = std::move(value);
    }

    Value get(const std::string &name) const
    {
        auto val = values.find(name);

        if (val != values.end())
        {
            return val->second;
        }

        if (parent != nullptr)
        {
            return parent->get(name);
        }

        throw std::runtime_error("Undefined variable: " + name);
    }

private:
    std::unordered_map<std::string, Value> values;
    Environment *parent;
};