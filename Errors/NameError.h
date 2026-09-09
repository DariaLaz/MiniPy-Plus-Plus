#pragma once

#include "Errors/Error.h"

class NameError : public Error
{
public:
    NameError(const std::string &message, const SourceLocation &location)
        : Error("NameError", message, location)
    {
    }
};