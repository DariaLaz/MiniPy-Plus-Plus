#include <stdexcept>
#include <string>
#include <utility>

#include "Parser/ExpressionParser.h"
#include "Parser/StatementParser.h"
#include "AST/Statement/AssignmentStatement.h"
#include "AST/Statement/BlockStatement.h"
#include "AST/Statement/ExpressionStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/PrintStatement.h"
#include "AST/Statement/WhileStatement.h"

StatementParser::StatementParser(
    TokenStream &tokens,
    ExpressionParser &expressions)
    : tokens(tokens),
      expressions(expressions)
{
}

std::unique_ptr<Statement> StatementParser::parse_program()
{
    std::vector<std::unique_ptr<Statement>> statements;

    while (!tokens.check(TokenType::End))
    {
        if (tokens.match(TokenType::Newline))
        {
            continue;
        }

        statements.push_back(statement());
    }

    return std::make_unique<BlockStatement>(std::move(statements));
}

std::unique_ptr<Statement> StatementParser::if_statement()
{
    auto condition = expressions.parse();

    if (!tokens.match(TokenType::Colon))
    {
        throw std::runtime_error("Expected ':' after 'if' condition");
    }

    auto then = block();

    std::unique_ptr<Statement> else_branch = nullptr;

    if (tokens.match(TokenType::Elif))
    {
        else_branch = if_statement();
    }
    else if (tokens.match(TokenType::Else))
    {
        if (!tokens.match(TokenType::Colon))
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

std::unique_ptr<Statement> StatementParser::print_statement()
{
    if (!tokens.match(TokenType::LeftParen))
    {
        throw std::runtime_error("Expected '(' after print");
    }

    auto value = expressions.parse();

    if (!tokens.match(TokenType::RightParen))
    {
        throw std::runtime_error("Expected ')' after print expression");
    }

    return std::make_unique<PrintStatement>(std::move(value));
}

std::unique_ptr<Statement> StatementParser::while_statement()
{
    auto condition = expressions.parse();

    if (!tokens.match(TokenType::Colon))
    {
        throw std::runtime_error("Expected ':' after 'while' condition");
    }

    auto body = block();

    return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
}

std::unique_ptr<Statement> StatementParser::simple_statement()
{
    if (tokens.check(TokenType::Identifier) && tokens.check_next(TokenType::Equal))
    {
        std::string name = tokens.current().text;

        tokens.increment(2);

        auto value = expressions.parse();

        return std::make_unique<AssignmentStatement>(name, std::move(value));
    }

    return std::make_unique<ExpressionStatement>(expressions.parse());
}

std::unique_ptr<Statement> StatementParser::statement()
{
    if (tokens.match(TokenType::If))
    {
        return if_statement();
    }

    if (tokens.match(TokenType::While))
    {
        return while_statement();
    }

    if (tokens.match(TokenType::Print))
    {
        return print_statement();
    }

    auto stmt = simple_statement();

    if (!tokens.match(TokenType::Newline))
    {
        throw std::runtime_error("Expected newline after statement");
    }

    return stmt;
}

std::unique_ptr<Statement> StatementParser::block()
{
    if (!tokens.match(TokenType::Newline))
    {
        throw std::runtime_error("Expected newline before block");
    }

    if (!tokens.match(TokenType::Indent))
    {
        throw std::runtime_error("Expected indented block");
    }

    std::vector<std::unique_ptr<Statement>> statements;

    while (!tokens.check(TokenType::Dedent) && !tokens.check(TokenType::End))
    {
        if (tokens.match(TokenType::Newline))
        {
            continue;
        }

        statements.push_back(statement());
    }

    if (!tokens.match(TokenType::Dedent))
    {
        throw std::runtime_error("Expected dedent after block");
    }

    return std::make_unique<BlockStatement>(std::move(statements));
}