#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>

#include "Value.h"
#include "Errors/NameError.h"

class Environment : public std::enable_shared_from_this<Environment>
{
public:
    Environment(std::shared_ptr<Environment> parent = nullptr) : parent(std::move(parent))
    {
    }

    void set(const std::string &name, Value value)
    {
        values[name] = std::move(value);
    }

    Value get(const std::string &name, SourceLocation location) const
    {
        auto val = values.find(name);

        if (val != values.end())
        {
            return val->second;
        }

        if (parent != nullptr)
        {
            return parent->get(name, location);
        }

        throw NameError("Undefined variable: " + name, location);
    }

private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> parent;
};