#pragma once
#include <string>

enum class TokenType
{
    Number,
    Plus,
    Minus,
    Star,
    Slash,
    LeftParen,
    RightParen,
    End
};

struct Token
{
    TokenType type;
    std::string text;
};