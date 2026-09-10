#pragma once

#include "Errors/Error.h"

class ZeroDivisionError : public Error
{
public:
  ZeroDivisionError(const std::string &message, const SourceLocation &location)
      : Error("ZeroDivisionError", message, location)
  {
  }
};