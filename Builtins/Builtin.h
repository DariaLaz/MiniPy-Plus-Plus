#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Value.h"

class Builtin : public std::enable_shared_from_this<Builtin>
{
public:
    virtual ~Builtin() = default;

    virtual std::string name() const = 0;
    virtual Value func(const std::vector<Value> &args) const = 0;

    std::shared_ptr<BuildinFunctionValue> make();
};