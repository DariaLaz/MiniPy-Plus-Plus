#pragma once

#include "Errors/Error.h"

class KeyError : public Error
{
public:
    KeyError(const std::string &message, const SourceLocation &location)
        : Error("KeyError", message, location)
    {
    }
};