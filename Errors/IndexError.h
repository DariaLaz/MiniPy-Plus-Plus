#pragma once

#include "Errors/Error.h"

class IndexError : public Error
{
public:
  IndexError(const std::string &message, const SourceLocation &location)
      : Error("IndexError", message, location)
  {
  }
};