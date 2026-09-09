#pragma once

#include <stdexcept>
#include <string>
#include "Utils/SourceLocation.h"

class Error : public std::runtime_error
{
public:
    Error(const std::string &type, const std::string &message, const SourceLocation &location)
        : std::runtime_error(type + " at " + std::to_string(location.line) + ":" + std::to_string(location.column) + ": " + message)
    {
    }
};