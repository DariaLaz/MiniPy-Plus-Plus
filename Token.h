#pragma once
#include <string>

enum class TokenType
{
    Number,
    Identifier,

    Plus,
    Minus,
    Star,
    Slash,

    Equal,      // =
    EqualEqual, // ==
    NotEqual,   // !=

    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    LeftParen,
    RightParen,

    End
};

struct Token
{
    TokenType type;
    std::string text;
};