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
#include "AST/Expression/ListExpression.h"
#include "AST/Expression/IndexExpression.h"
#include "AST/Expression/CallExpression.h"
#include "AST/Expression/LogicalExpression.h"
#include "AST/Expression/NoneExpression.h"

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

    if (tokens.match(TokenType::LeftBracket))
    {
        return list();
    }

    if (tokens.match(TokenType::String))
    {
        return std::make_unique<StringExpression>(tokens.prev().text);
    }

    if (tokens.match(TokenType::None))
    {
        return std::make_unique<NoneExpression>();
    }

    throw std::runtime_error("Expected expression");
}

std::unique_ptr<Expression> ExpressionParser::list()
{
    std::vector<std::unique_ptr<Expression>> elements;

    if (!tokens.check(TokenType::RightBracket))
    {
        do
        {
            elements.push_back(expression());
        } while (tokens.match(TokenType::Comma));
    }

    if (!tokens.match(TokenType::RightBracket))
    {
        throw std::runtime_error("Expected ']' after list");
    }

    return std::make_unique<ListExpression>(std::move(elements));
}

std::unique_ptr<Expression>
ExpressionParser::postfix()
{
    auto expr = primary();

    while (true)
    {
        if (tokens.match(TokenType::LeftBracket))
        {
            auto index = expression();

            if (!tokens.match(TokenType::RightBracket))
            {
                throw std::runtime_error("Expected ']' after index");
            }

            expr = std::make_unique<IndexExpression>(std::move(expr), std::move(index));
        }
        else if (tokens.match(TokenType::LeftParen))
        {
            std::vector<std::unique_ptr<Expression>> arguments;

            if (!tokens.check(TokenType::RightParen))
            {
                do
                {
                    arguments.push_back(expression());
                } while (tokens.match(TokenType::Comma));
            }

            if (!tokens.match(TokenType::RightParen))
            {
                throw std::runtime_error("Expected ')' after arguments");
            }

            expr = std::make_unique<CallExpression>(std::move(expr), std::move(arguments));
        }
        else
        {
            break;
        }
    }

    return expr;
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

    return postfix();
}

std::unique_ptr<Expression> ExpressionParser::factor()
{
    return binary_expr<BinaryExpression>(&ExpressionParser::unary, {TokenType::Star, TokenType::Slash});
}

std::unique_ptr<Expression> ExpressionParser::term()
{
    return binary_expr<BinaryExpression>(&ExpressionParser::factor, {TokenType::Plus, TokenType::Minus});
}

std::unique_ptr<Expression> ExpressionParser::comparison()
{
    return binary_expr<BinaryExpression>(&ExpressionParser::term, {TokenType::Less, TokenType::LessEqual, TokenType::Greater, TokenType::GreaterEqual});
}

std::unique_ptr<Expression> ExpressionParser::equality()
{
    return binary_expr<BinaryExpression>(&ExpressionParser::comparison, {TokenType::EqualEqual, TokenType::NotEqual});
}

std::unique_ptr<Expression> ExpressionParser::not_expression()
{
    if (tokens.check(TokenType::Not))
    {
        Token op = tokens.current();
        tokens.increment();

        auto right = not_expression();

        return std::make_unique<UnaryExpression>(
            op,
            std::move(right));
    }

    return equality();
}

std::unique_ptr<Expression>
ExpressionParser::and_expression()
{
    return binary_expr<LogicalExpression>(&ExpressionParser::not_expression, {TokenType::And});
}

std::unique_ptr<Expression>
ExpressionParser::or_expression()
{
    return binary_expr<LogicalExpression>(&ExpressionParser::and_expression, {TokenType::Or});
}

std::unique_ptr<Expression> ExpressionParser::expression()
{
    return or_expression();
}

template <typename T>
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

        left = std::make_unique<T>(
            std::move(left),
            op,
            std::move(right));
    }

    return left;
}