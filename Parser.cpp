#include <stdexcept>
#include <string>
#include <utility>

#include "AST/Expression/NumberExpression.h"
#include "AST/Expression/BinaryExpression.h"
#include "Parser.h"
#include "AST/Expression/UnaryExpression.h"
#include "AST/Expression/VariableExpression.h"
#include "AST/Statement/AssignmentStatement .h"
#include "AST/Statement/ExpressionStatement.h"

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

bool Parser::check(std::initializer_list<TokenType> types) const
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

bool Parser::check_next(TokenType type) const
{
    if (curr + 1 >= tokens.size())
        return false;

    return tokens[curr + 1].type == type;
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

    if (match(TokenType::Identifier))
    {
        return std::make_unique<VariableExpression>(prev().text);
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

std::unique_ptr<Expression> Parser::unary()
{
    if (check({TokenType::Minus, TokenType::Plus}))
    {
        Token t = current();
        curr++;

        auto val = unary();
        return std::make_unique<UnaryExpression>(t, std::move(val));
    }

    return primary();
}

std::unique_ptr<Expression> Parser::factor()
{
    return binary_expr(&Parser::unary, {TokenType::Slash, TokenType::Slash});
}

std::unique_ptr<Expression> Parser::term()
{
    return binary_expr(&Parser::factor, {TokenType::Plus, TokenType::Minus});
}

std::unique_ptr<Expression> Parser::comparison()
{
    return binary_expr(&Parser::term, {TokenType::Less, TokenType::LessEqual, TokenType::Greater, TokenType::GreaterEqual});
}

std::unique_ptr<Expression> Parser::equality()
{
    return binary_expr(&Parser::comparison, {TokenType::EqualEqual, TokenType::NotEqual});
}

std::unique_ptr<Expression> Parser::expression()
{
    return equality();
}

std::unique_ptr<Statement> Parser::statement()
{
    if (check(TokenType::Identifier) && check_next(TokenType::Equal))
    {
        std::string name = current().text;

        curr += 2; // identifier + '='

        auto value = expression();

        return std::make_unique<AssignmentStatement>(name, std::move(value));
    }

    return std::make_unique<ExpressionStatement>(expression());
}

std::unique_ptr<Expression> Parser::binary_expr(
    std::unique_ptr<Expression> (Parser::*expr)(),
    std::initializer_list<TokenType> types)
{
    auto left = (this->*expr)();

    while (check(types))
    {
        Token t = current();
        curr++;

        auto right = (this->*expr)();

        left = std::make_unique<BinaryExpression>(
            std::move(left),
            t,
            std::move(right));
    }

    return left;
}

std::unique_ptr<Statement> Parser::parse()
{
    auto st = statement();

    if (!check(TokenType::End))
    {
        throw std::runtime_error(
            "Unexpected token: " + current().text);
    }

    return st;
}