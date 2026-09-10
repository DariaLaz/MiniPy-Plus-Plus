#pragma once

#include "Errors/Error.h"

class ValueError : public Error
{
public:
  ValueError(const std::string &message, const SourceLocation &location)
      : Error("ValueError", message, location)
  {
  }
};