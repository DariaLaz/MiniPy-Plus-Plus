#pragma once

#include "Errors/Error.h"
#include "Lexer/Token.h"
#include "Utils/SourceLocation.h"

#include <stdexcept>
#include <string>

class SyntaxError : public Error
{
public:
  SyntaxError(const std::string &message, const SourceLocation &location)
      : Error("SyntaxError", message, location)
  {
  }
};