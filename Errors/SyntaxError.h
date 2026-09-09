#pragma once

#include <stdexcept>
#include <string>

#include "Token.h"

class SyntaxError : public std::runtime_error
{
public:
    SyntaxError(const std::string &message, const Token &token)
        : std::runtime_error(
              "SyntaxError at " + std::to_string(token.line) + ":" + std::to_string(token.column) + ": " + message)
    {
    }
};