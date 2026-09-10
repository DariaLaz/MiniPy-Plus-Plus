#pragma once

#include "Runtime/Value.h"

#include <memory>
#include <string>
#include <vector>

class Builtin : public std::enable_shared_from_this<Builtin>
{
public:
  virtual ~Builtin() = default;

  virtual std::string name() const = 0;
  virtual Value func(const std::vector<Value> &args, const SourceLocation &location) const = 0;

  std::shared_ptr<BuiltinFunctionValue> make();
};