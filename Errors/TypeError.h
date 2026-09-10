#pragma once

#include "Errors/Error.h"

class TypeError : public Error
{
public:
  TypeError(const std::string &message, const SourceLocation &location)
      : Error("TypeError", message, location)
  {
  }
};