#include "Lexer.h"
#include <stdexcept>

Lexer::Lexer(const std::string &source) : source(source) {}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (curr < source.size())
    {
        char curr_char = static_cast<unsigned char>(source[curr]);

        if (std::isspace(curr_char))
        {
            curr++;
            continue;
        }

        if (std::isdigit(curr_char))
        {
            tokens.push_back(tokenize_number());
            continue;
        }

        if (is_identifier())
        {
            tokens.push_back(tokenize_identifier());
            continue;
        }

        tokens.push_back(tokenize_symbol());
        curr++;
    }

    tokens.push_back({TokenType::End, ""});

    return tokens;
}

Token Lexer::tokenize_number()
{
    std::string number = "";
    while (curr < source.size() && is_digit())
    {
        number += source[curr];
        curr++;
    }

    return {TokenType::Number, number};
}

Token Lexer::tokenize_symbol()
{
    switch (static_cast<unsigned char>(source[curr]))
    {
    case '+':
        return {TokenType::Plus, "+"};
    case '-':
        return {TokenType::Minus, "-"};
    case '*':
        return {TokenType::Star, "*"};
    case '/':
        return {TokenType::Slash, "/"};
    case '(':
        return {TokenType::LeftParen, "("};
    case ')':
        return {TokenType::RightParen, ")"};
    case '=':
        if (check_next('='))
        {
            curr++;
            return {TokenType::EqualEqual, "=="};
        }
        return {TokenType::Equal, "="};

    case '<':
        if (check_next('='))
        {
            curr++;
            return {TokenType::LessEqual, "<="};
        }
        return {TokenType::Less, "<"};

    case '>':
        if (check_next('='))
        {
            curr++;
            return {TokenType::GreaterEqual, ">="};
        }
        return {TokenType::Greater, ">"};

    case '!':
        if (check_next('='))
        {
            curr++;
            return {TokenType::NotEqual, "!="};
        }

    default:
        throw std::runtime_error(
            "Unexpected character: " + std::string(1, source[curr]));
    }
}

Token Lexer::tokenize_identifier()
{
    std::string identifier;

    while (curr < source.size() && is_identifier())
    {
        identifier += source[curr];
        curr++;
    }

    return {TokenType::Identifier, identifier};
}

bool Lexer::is_identifier() const
{
    return std::isalpha(static_cast<unsigned char>(source[curr])) ||
           source[curr] == '_';
}

bool Lexer::is_digit() const
{
    return std::isdigit(static_cast<unsigned char>(source[curr]));
}

bool Lexer::check_next(char ch) const
{
    if (curr == source.size() - 1)
    {
        return false;
    }
    return source[curr + 1] == ch;
}
