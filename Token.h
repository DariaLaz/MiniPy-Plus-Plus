#pragma once
#include <string>

enum class TokenType
{
    Number,
    String,
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

    While,

    Print,

    Newline,
    Indent,
    Dedent,

    LeftBracket,
    RightBracket,
    Comma,

    For,
    In,

    Def,
    Return,

    Break,
    Continue,

    And,
    Or,
    Not,

    End
};

struct Token
{
    TokenType type;
    std::string text;
};