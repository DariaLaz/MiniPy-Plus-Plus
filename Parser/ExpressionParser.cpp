#include "Parser/ExpressionParser.h"

#include "AST/Expression/BinaryExpression.h"
#include "AST/Expression/BoolExpression.h"
#include "AST/Expression/CallExpression.h"
#include "AST/Expression/DictExpression.h"
#include "AST/Expression/IndexExpression.h"
#include "AST/Expression/ListExpression.h"
#include "AST/Expression/LogicalExpression.h"
#include "AST/Expression/NoneExpression.h"
#include "AST/Expression/NumberExpression.h"
#include "AST/Expression/SliceExpression.h"
#include "AST/Expression/StringExpression.h"
#include "AST/Expression/UnaryExpression.h"
#include "AST/Expression/VariableExpression.h"
#include "Errors/SyntaxError.h"

#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<Expression> ExpressionParser::parse()
{
    return expression();
}

std::unique_ptr<Expression> ExpressionParser::primary()
{
    if (tokens.match(TokenType::Number))
    {
        return std::make_unique<NumberExpression>(std::stoi(tokens.prev().text),
                                                  get_prev_location());
    }

    if (tokens.match(TokenType::Identifier))
    {
        return std::make_unique<VariableExpression>(tokens.prev().text, get_prev_location());
    }

    if (tokens.match(TokenType::LeftParen))
    {
        auto expr = expression();

        if (!tokens.match(TokenType::RightParen))
        {
            throw SyntaxError("Expected ')'", get_location());
        }

        return expr;
    }

    if (tokens.match(TokenType::True))
    {
        return std::make_unique<BoolExpression>(true, get_prev_location());
    }

    if (tokens.match(TokenType::False))
    {
        return std::make_unique<BoolExpression>(false, get_prev_location());
    }

    if (tokens.match(TokenType::LeftBracket))
    {
        return list();
    }

    if (tokens.match(TokenType::String))
    {
        return std::make_unique<StringExpression>(tokens.prev().text, get_prev_location());
    }

    if (tokens.match(TokenType::None))
    {
        return std::make_unique<NoneExpression>(get_prev_location());
    }

    if (tokens.match(TokenType::LeftBrace))
    {
        return dictionary();
    }

    throw SyntaxError("Expected expression", get_location());
}

std::unique_ptr<Expression> ExpressionParser::dictionary()
{
    std::vector<DictExpression::Entry> entries;

    if (!tokens.check(TokenType::RightBrace))
    {
        while (true)
        {
            auto key = expression();

            if (!tokens.match(TokenType::Colon))
            {
                throw SyntaxError("Expected ':' after dictionary key", get_location());
            }

            auto value = expression();

            entries.emplace_back(std::move(key), std::move(value));

            if (!tokens.match(TokenType::Comma))
            {
                break;
            }
        }
    }

    if (!tokens.match(TokenType::RightBrace))
    {
        throw SyntaxError("Expected '}' after dictionary", get_location());
    }

    return std::make_unique<DictExpression>(std::move(entries), get_location());
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
        throw SyntaxError("Expected ']' after list", get_location());
    }

    return std::make_unique<ListExpression>(std::move(elements), get_prev_location());
}

std::unique_ptr<Expression> ExpressionParser::postfix()
{
    auto expr = primary();

    while (true)
    {
        if (tokens.match(TokenType::LeftBracket))
        {
            SourceLocation location = get_prev_location();

            std::unique_ptr<Expression> first = nullptr;

            // x[:3]
            if (!tokens.check(TokenType::Colon))
            {
                first = expression();
            }

            if (tokens.match(TokenType::Colon))
            {
                std::unique_ptr<Expression> end = nullptr;

                // x[1:]
                if (!tokens.check(TokenType::RightBracket))
                {
                    end = expression();
                }

                if (!tokens.match(TokenType::RightBracket))
                {
                    throw SyntaxError("Expected ']' after slice", get_location());
                }

                expr = std::make_unique<SliceExpression>(std::move(expr), std::move(first),
                                                         std::move(end), location);
            }

            else
            {
                if (!first)
                {
                    throw SyntaxError("Expected index", get_location());
                }

                if (!tokens.match(TokenType::RightBracket))
                {
                    throw SyntaxError("Expected ']' after index", get_location());
                }

                expr =
                    std::make_unique<IndexExpression>(std::move(expr), std::move(first), location);
            }
        }
        else if (tokens.match(TokenType::LeftParen))
        {
            SourceLocation location = get_prev_location();

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
                throw SyntaxError("Expected ')' after arguments", get_location());
            }

            expr =
                std::make_unique<CallExpression>(std::move(expr), std::move(arguments), location);
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
    return binary_expr<BinaryExpression>(&ExpressionParser::unary,
                                         {TokenType::Star, TokenType::Slash});
}

std::unique_ptr<Expression> ExpressionParser::term()
{
    return binary_expr<BinaryExpression>(&ExpressionParser::factor,
                                         {TokenType::Plus, TokenType::Minus});
}

std::unique_ptr<Expression> ExpressionParser::comparison()
{
    return binary_expr<BinaryExpression>(
        &ExpressionParser::term,
        {TokenType::Less, TokenType::LessEqual, TokenType::Greater, TokenType::GreaterEqual});
}

std::unique_ptr<Expression> ExpressionParser::equality()
{
    return binary_expr<BinaryExpression>(&ExpressionParser::comparison,
                                         {TokenType::EqualEqual, TokenType::NotEqual});
}

std::unique_ptr<Expression> ExpressionParser::not_expression()
{
    if (tokens.check(TokenType::Not))
    {
        Token op = tokens.current();
        tokens.increment();

        auto right = not_expression();

        return std::make_unique<UnaryExpression>(op, std::move(right));
    }

    return equality();
}

std::unique_ptr<Expression> ExpressionParser::and_expression()
{
    return binary_expr<LogicalExpression>(&ExpressionParser::not_expression, {TokenType::And});
}

std::unique_ptr<Expression> ExpressionParser::or_expression()
{
    return binary_expr<LogicalExpression>(&ExpressionParser::and_expression, {TokenType::Or});
}

std::unique_ptr<Expression> ExpressionParser::expression()
{
    return or_expression();
}

template <typename T>
std::unique_ptr<Expression>
ExpressionParser::binary_expr(std::unique_ptr<Expression> (ExpressionParser::*expr)(),
                              std::initializer_list<TokenType> types)
{
    auto left = (this->*expr)();

    while (tokens.check(types))
    {
        Token op = tokens.current();
        tokens.increment();

        auto right = (this->*expr)();

        left = std::make_unique<T>(std::move(left), op, std::move(right));
    }

    return left;
}

SourceLocation ExpressionParser::get_location() const
{
    return SourceLocation{tokens.current().line, tokens.current().column};
}

SourceLocation ExpressionParser::get_prev_location() const
{
    return SourceLocation{tokens.prev().line, tokens.prev().column};
}