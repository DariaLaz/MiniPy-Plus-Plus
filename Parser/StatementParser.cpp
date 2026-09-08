#include <stdexcept>
#include <string>
#include <utility>

#include "Parser/ExpressionParser.h"
#include "Parser/StatementParser.h"
#include "AST/Statement/AssignmentStatement.h"
#include "AST/Statement/BlockStatement.h"
#include "AST/Statement/ExpressionStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "AST/Statement/ForStatement.h"
#include "AST/Statement/FunctionStatement.h"
#include "AST/Statement/ReturnStatement.h"
#include "AST/Statement/BreakStatement.h"
#include "AST/Statement/ContinueStatement.h"
#include "AST/Statement/IndexAssignmentStatement.h"

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

std::unique_ptr<Statement> StatementParser::for_statement()
{
    if (!tokens.check(TokenType::Identifier))
    {
        throw std::runtime_error("Expected variable after 'for'");
    }

    std::string variable = tokens.current().text;

    tokens.match(TokenType::Identifier);

    if (!tokens.match(TokenType::In))
    {
        throw std::runtime_error("Expected 'in' after for variable");
    }

    auto iterable = expressions.parse();

    if (!tokens.match(TokenType::Colon))
    {
        throw std::runtime_error("Expected ':' after for statement");
    }

    auto body = block();

    return std::make_unique<ForStatement>(
        variable,
        std::move(iterable),
        std::move(body));
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

std::unique_ptr<Statement> StatementParser::return_statement()
{
    std::unique_ptr<Expression> value = nullptr;

    if (!tokens.check(TokenType::Newline))
    {
        value = expressions.parse();
    }

    if (!tokens.match(TokenType::Newline))
    {
        throw std::runtime_error("Expected newline after return");
    }

    return std::make_unique<ReturnStatement>(std::move(value));
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

    // list[index] = expression
    if (tokens.check(TokenType::Identifier) && tokens.check_next(TokenType::LeftBracket))
    {
        int saved_position = tokens.position();

        auto assignment = index_assignment();

        if (assignment)
        {
            return assignment;
        }
        tokens.set_position(saved_position);
    }

    return std::make_unique<ExpressionStatement>(expressions.parse());
}

std::unique_ptr<Statement> StatementParser::function_statement()
{
    if (!tokens.check(TokenType::Identifier))
    {
        throw std::runtime_error("Expected function name");
    }

    std::string name = tokens.current().text;

    tokens.match(TokenType::Identifier);

    if (!tokens.match(TokenType::LeftParen))
    {
        throw std::runtime_error("Expected '(' after function name");
    }

    std::vector<std::string> parameters;

    if (!tokens.check(TokenType::RightParen))
    {
        do
        {
            if (!tokens.check(TokenType::Identifier))
            {
                throw std::runtime_error("Expected parameter name");
            }

            parameters.push_back(tokens.current().text);

            tokens.match(TokenType::Identifier);
        } while (tokens.match(TokenType::Comma));
    }

    if (!tokens.match(TokenType::RightParen))
    {
        throw std::runtime_error("Expected ')' after parameters");
    }

    if (!tokens.match(TokenType::Colon))
    {
        throw std::runtime_error("Expected ':' after function declaration");
    }

    auto body = block();

    return std::make_unique<FunctionStatement>(
        name,
        std::move(parameters),
        std::move(body));
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

    if (tokens.match(TokenType::For))
    {
        return for_statement();
    }

    if (tokens.match(TokenType::Def))
    {
        return function_statement();
    }

    if (tokens.match(TokenType::Return))
    {
        return return_statement();
    }

    if (tokens.match(TokenType::Break))
    {
        if (!tokens.match(TokenType::Newline))
        {
            throw std::runtime_error("Expected newline after break");
        }

        return std::make_unique<BreakStatement>();
    }

    if (tokens.match(TokenType::Continue))
    {
        if (!tokens.match(TokenType::Newline))
        {
            throw std::runtime_error("Expected newline after continue");
        }

        return std::make_unique<ContinueStatement>();
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

std::unique_ptr<Statement> StatementParser::index_assignment()
{
    std::string name = tokens.current().text;

    tokens.match(TokenType::Identifier);

    if (!tokens.match(TokenType::LeftBracket))
    {
        return nullptr;
    }

    auto index = expressions.parse();

    if (!tokens.match(TokenType::RightBracket))
    {
        throw std::runtime_error("Expected ']' after index");
    }

    if (!tokens.match(TokenType::Equal))
    {
        return nullptr;
    }

    auto value = expressions.parse();

    return std::make_unique<IndexAssignmentStatement>(name, std::move(index), std::move(value));
}