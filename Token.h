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
    Colon,

    If,
    Elif,
    Else,
    True,
    False,

    End
};

struct Token
{
    TokenType type;
    std::string text;
};