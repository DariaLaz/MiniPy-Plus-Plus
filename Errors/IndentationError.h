#pragma once

#include "Errors/Error.h"

class IndentationError : public Error
{
public:
  IndentationError(const std::string &message, const SourceLocation &location)
      : Error("IndentationError", message, location)
  {
  }
};