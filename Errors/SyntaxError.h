#pragma once

#include <stdexcept>
#include <string>

#include "Token.h"
#include "Errors/Error.h"
#include "Utils/SourceLocation.h"

class SyntaxError : public Error
{
public:
    SyntaxError(const std::string &message, const SourceLocation &location)
        : Error("SyntaxError", message, location)
    {
    }
};