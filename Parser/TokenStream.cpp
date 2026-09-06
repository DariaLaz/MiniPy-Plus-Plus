#include <stdexcept>
#include <string>
#include <utility>

#include "Parser/TokenStream.h"

TokenStream::TokenStream(const std::vector<Token> &tokens) : tokens(tokens)
{
}

const Token &TokenStream::current() const
{
    return tokens[curr];
}

const Token &TokenStream::prev() const
{
    return tokens[curr - 1];
}

bool TokenStream::check(TokenType type) const
{
    return current().type == type;
}

bool TokenStream::check(std::initializer_list<TokenType> types) const
{
    for (TokenType type : types)
    {
        if (check(type))
        {
            return true;
        }
    }

    return false;
}

bool TokenStream::check_next(TokenType type) const
{
    if (curr + 1 >= tokens.size())
        return false;

    return tokens[curr + 1].type == type;
}

bool TokenStream::match(TokenType type)
{
    if (!check(type))
    {
        return false;
    }

    curr++;
    return true;
}