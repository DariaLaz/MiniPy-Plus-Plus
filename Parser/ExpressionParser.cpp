#include <stdexcept>
#include <string>
#include <utility>

#include "Parser/ExpressionParser.h"
#include "AST/Expression/BinaryExpression.h"
#include "AST/Expression/BoolExpression.h"
#include "AST/Expression/NumberExpression.h"
#include "AST/Expression/UnaryExpression.h"
#include "AST/Expression/VariableExpression.h"
#include "AST/Expression/StringExpression.h"

std::unique_ptr<Expression> ExpressionParser::parse()
{
    return expression();
}

std::unique_ptr<Expression> ExpressionParser::primary()
{
    if (tokens.match(TokenType::Number))
    {
        return std::make_unique<NumberExpression>(std::stoi(tokens.prev().text));
    }

    if (tokens.match(TokenType::Identifier))
    {
        return std::make_unique<VariableExpression>(tokens.prev().text);
    }

    if (tokens.match(TokenType::LeftParen))
    {
        auto expr = expression();

        if (!tokens.match(TokenType::RightParen))
        {
            throw std::runtime_error("Expected ')'");
        }

        return expr;
    }

    if (tokens.match(TokenType::True))
    {
        return std::make_unique<BoolExpression>(true);
    }

    if (tokens.match(TokenType::False))
    {
        return std::make_unique<BoolExpression>(false);
    }

    if (tokens.match(TokenType::String))
    {
        return std::make_unique<StringExpression>(tokens.prev().text);
    }

    throw std::runtime_error("Expected expression");
}

std::unique_ptr<Expression> ExpressionParser::unary()
{
    if (tokens.check({TokenType::Minus, TokenType::Plus}))
    {
        Token t = tokens.current();
        tokens.increment();

        auto val = unary();
        return std::make_unique<UnaryExpression>(t, std::move(val));
    }

    return primary();
}

std::unique_ptr<Expression> ExpressionParser::factor()
{
    return binary_expr(&ExpressionParser::unary, {TokenType::Star, TokenType::Slash});
}

std::unique_ptr<Expression> ExpressionParser::term()
{
    return binary_expr(&ExpressionParser::factor, {TokenType::Plus, TokenType::Minus});
}

std::unique_ptr<Expression> ExpressionParser::comparison()
{
    return binary_expr(&ExpressionParser::term, {TokenType::Less, TokenType::LessEqual, TokenType::Greater, TokenType::GreaterEqual});
}

std::unique_ptr<Expression> ExpressionParser::equality()
{
    return binary_expr(&ExpressionParser::comparison, {TokenType::EqualEqual, TokenType::NotEqual});
}

std::unique_ptr<Expression> ExpressionParser::expression()
{
    return equality();
}

std::unique_ptr<Expression> ExpressionParser::binary_expr(
    std::unique_ptr<Expression> (ExpressionParser::*expr)(),
    std::initializer_list<TokenType> types)
{
    auto left = (this->*expr)();

    while (tokens.check(types))
    {
        Token op = tokens.current();
        tokens.increment();

        auto right = (this->*expr)();

        left = std::make_unique<BinaryExpression>(
            std::move(left),
            op,
            std::move(right));
    }

    return left;
}