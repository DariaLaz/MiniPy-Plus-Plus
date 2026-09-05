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

        tokens.push_back(tokenize_symbol());
        curr++;
    }

    tokens.push_back({TokenType::End, ""});

    return tokens;
}

Token Lexer::tokenize_number()
{
    std::string number = "";
    while (
        curr < source.size() &&
        std::isdigit(static_cast<unsigned char>(source[curr])))
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

    default:
        throw std::runtime_error(
            "Unexpected character: " + std::string(1, source[curr]));
    }
}
