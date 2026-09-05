#include <stdexcept>
#include <string>
#include <utility>

#include "NumberExpression.h"
#include "BinaryExpression.h"
#include "Parser.h"

Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens)
{
}

const Token &Parser::current() const
{
    return tokens[curr];
}

const Token &Parser::prev() const
{
    return tokens[curr - 1];
}

bool Parser::check(TokenType type) const
{
    return current().type == type;
}

bool Parser::match(TokenType type)
{
    if (!check(type))
    {
        return false;
    }

    curr++;
    return true;
}

std::unique_ptr<Expression> Parser::primary()
{
    if (match(TokenType::Number))
    {
        return std::make_unique<NumberExpression>(std::stoi(prev().text));
    }

    if (match(TokenType::LeftParen))
    {
        auto expr = expression();

        if (!match(TokenType::RightParen))
        {
            throw std::runtime_error("Expected ')'");
        }

        return expr;
    }

    throw std::runtime_error("Expected expression");
}

std::unique_ptr<Expression> Parser::factor()
{
    auto left = primary();

    while (check(TokenType::Slash) || check(TokenType::Slash))
    {
        Token t = current();
        curr++;

        auto right = primary();

        left = std::make_unique<BinaryExpression>(
            std::move(left),
            t,
            std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::term()
{
    auto left = factor();

    while (check(TokenType::Plus) || check(TokenType::Minus))
    {
        Token t = current();
        curr++;

        auto right = factor();

        left = std::make_unique<BinaryExpression>(
            std::move(left),
            t,
            std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::expression()
{
    return term();
}

std::unique_ptr<Expression> Parser::parse()
{
    auto expr = expression();

    if (!check(TokenType::End))
    {
        throw std::runtime_error(
            "Unexpected token: " + current().text);
    }

    return expr;
}