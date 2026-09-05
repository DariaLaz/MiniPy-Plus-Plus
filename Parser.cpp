#include <stdexcept>
#include <string>
#include <utility>

#include "AST/Expression/BinaryExpression.h"
#include "AST/Expression/BoolExpression.h"
#include "AST/Expression/NumberExpression.h"
#include "AST/Expression/UnaryExpression.h"
#include "AST/Expression/VariableExpression.h"
#include "AST/Statement/AssignmentStatement .h"
#include "AST/Statement/ExpressionStatement.h"
#include "AST/Statement/IfStatement.h"
#include "Parser.h"
#include "BlockStatement.h"

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

    if (match(TokenType::True))
    {
        return std::make_unique<BoolExpression>(true);
    }

    if (match(TokenType::False))
    {
        return std::make_unique<BoolExpression>(false);
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

std::unique_ptr<Statement> Parser::if_statement()
{
    auto condition = expression();

    if (!match(TokenType::Colon))
    {
        throw std::runtime_error("Expected ':' after 'if' condition");
    }

    auto then = block();

    std::unique_ptr<Statement> else_branch = nullptr;

    if (match(TokenType::Elif))
    {
        else_branch = if_statement();
    }
    else if (match(TokenType::Else))
    {
        if (!match(TokenType::Colon))
        {
            throw std::runtime_error("Expected ':' after else");
        }

        else_branch = block();
    }

    return std::make_unique<IfStatement>(
        std::move(condition),
        std::move(then),
        std::move(else_branch));
}

std::unique_ptr<Statement> Parser::simple_statement()
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

std::unique_ptr<Statement> Parser::statement()
{
    if (match(TokenType::If))
    {
        return if_statement();
    }

    auto stmt = simple_statement();

    if (!match(TokenType::Newline))
    {
        throw std::runtime_error("Expected newline after statement");
    }

    return stmt;
}

std::unique_ptr<Statement> Parser::block()
{
    if (!match(TokenType::Newline))
    {
        throw std::runtime_error("Expected newline before block");
    }

    if (!match(TokenType::Indent))
    {
        throw std::runtime_error("Expected indented block");
    }

    std::vector<std::unique_ptr<Statement>> statements;

    while (!check(TokenType::Dedent) && !check(TokenType::End))
    {
        if (match(TokenType::Newline))
        {
            continue;
        }

        statements.push_back(statement());
    }

    if (!match(TokenType::Dedent))
    {
        throw std::runtime_error("Expected dedent after block");
    }

    return std::make_unique<BlockStatement>(std::move(statements));
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
    std::vector<std::unique_ptr<Statement>> statements;

    while (!check(TokenType::End))
    {
        if (match(TokenType::Newline))
        {
            continue;
        }

        statements.push_back(statement());
    }

    return std::make_unique<BlockStatement>(std::move(statements));
}