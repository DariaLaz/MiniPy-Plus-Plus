#pragma once
#include "Builtins/Builtin.h"
#include "Runtime/Value.h"

#include <stdexcept>

class RangeBuiltin : public Builtin
{
public:
  std::string name() const override;

  Value func(const std::vector<Value> &args, const SourceLocation &location) const override;
};