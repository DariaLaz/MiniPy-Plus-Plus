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
#include "Errors/SyntaxError.h"

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

    return std::make_unique<BlockStatement>(std::move(statements), get_location());
}

std::unique_ptr<Statement> StatementParser::if_statement()
{
    auto condition = expressions.parse();

    if (!tokens.match(TokenType::Colon))
    {
        throw SyntaxError("Expected ':' after 'if' condition", get_location());
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
            throw SyntaxError("Expected ':' after else", get_location());
        }

        else_branch = block();
    }

    return std::make_unique<IfStatement>(
        std::move(condition),
        std::move(then),
        get_location(),
        std::move(else_branch));
}

std::unique_ptr<Statement> StatementParser::for_statement()
{
    if (!tokens.check(TokenType::Identifier))
    {
        throw SyntaxError("Expected variable after 'for'", get_location());
    }

    std::string variable = tokens.current().text;

    tokens.match(TokenType::Identifier);

    if (!tokens.match(TokenType::In))
    {
        throw SyntaxError("Expected 'in' after for variable", get_location());
    }

    auto iterable = expressions.parse();

    if (!tokens.match(TokenType::Colon))
    {
        throw SyntaxError("Expected ':' after for statement", get_location());
    }

    auto body = block();

    return std::make_unique<ForStatement>(
        variable,
        std::move(iterable),
        std::move(body),
        get_location());
}

std::unique_ptr<Statement> StatementParser::while_statement()
{
    auto condition = expressions.parse();

    if (!tokens.match(TokenType::Colon))
    {
        throw SyntaxError("Expected ':' after 'while' condition", get_location());
    }

    auto body = block();

    return std::make_unique<WhileStatement>(std::move(condition), std::move(body), get_location());
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
        throw SyntaxError("Expected newline after return", get_location());
    }

    return std::make_unique<ReturnStatement>(std::move(value), get_location());
}

std::unique_ptr<Statement> StatementParser::simple_statement()
{
    // x = expression
    if (tokens.check(TokenType::Identifier) && tokens.check_next(TokenType::Equal))
    {
        std::string name = tokens.current().text;

        tokens.increment(2);

        auto value = expressions.parse();

        return std::make_unique<AssignmentStatement>(name, std::move(value), get_location());
    }

    // x[index] = expression
    if (is_index_assignment())
    {
        return index_assignment();
    }

    return std::make_unique<ExpressionStatement>(expressions.parse(), get_location());
}

std::unique_ptr<Statement> StatementParser::function_statement()
{
    if (!tokens.check(TokenType::Identifier))
    {
        throw SyntaxError("Expected function name", get_location());
    }

    std::string name = tokens.current().text;

    tokens.match(TokenType::Identifier);

    if (!tokens.match(TokenType::LeftParen))
    {
        throw SyntaxError("Expected '(' after function name", get_location());
    }

    std::vector<std::string> parameters;

    if (!tokens.check(TokenType::RightParen))
    {
        do
        {
            if (!tokens.check(TokenType::Identifier))
            {
                throw SyntaxError("Expected parameter name", get_location());
            }

            parameters.push_back(tokens.current().text);

            tokens.match(TokenType::Identifier);
        } while (tokens.match(TokenType::Comma));
    }

    if (!tokens.match(TokenType::RightParen))
    {
        throw SyntaxError("Expected ')' after parameters", get_location());
    }

    if (!tokens.match(TokenType::Colon))
    {
        throw SyntaxError("Expected ':' after function declaration", get_location());
    }

    auto body = block();

    return std::make_unique<FunctionStatement>(
        name,
        std::move(parameters),
        std::move(body),
        get_location());
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
        SourceLocation location = get_prev_location();
        if (!tokens.match(TokenType::Newline))
        {
            throw SyntaxError("Expected newline after break", get_location());
        }

        return std::make_unique<BreakStatement>(location);
    }

    if (tokens.match(TokenType::Continue))
    {
        SourceLocation location = get_prev_location();

        if (!tokens.match(TokenType::Newline))
        {
            throw SyntaxError("Expected newline after continue", get_location());
        }

        return std::make_unique<ContinueStatement>(location);
    }

    auto stmt = simple_statement();

    if (!tokens.match(TokenType::Newline))
    {
        throw SyntaxError("Expected newline after statement", get_location());
    }

    return stmt;
}

std::unique_ptr<Statement> StatementParser::block()
{
    if (!tokens.match(TokenType::Newline))
    {
        throw SyntaxError("Expected newline before block", get_location());
    }

    while (tokens.match(TokenType::Newline))
    {
        // Skip empty lines
    }

    if (!tokens.match(TokenType::Indent))
    {
        throw SyntaxError("Expected indented block", get_location());
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
        throw SyntaxError("Expected dedent after block", get_location());
    }

    return std::make_unique<BlockStatement>(std::move(statements), get_location());
}

bool StatementParser::is_index_assignment()
{
    if (!tokens.check(TokenType::Identifier) || !tokens.check_next(TokenType::LeftBracket))
    {
        return false;
    }

    int saved_position = tokens.position();

    tokens.increment(2);

    int bracket_depth = 1;

    while (bracket_depth > 0 && !tokens.check(TokenType::End))
    {
        if (tokens.check(TokenType::LeftBracket))
        {
            bracket_depth++;
        }
        else if (tokens.check(TokenType::RightBracket))
        {
            bracket_depth--;
        }

        tokens.increment();
    }

    bool result = bracket_depth == 0 && tokens.check(TokenType::Equal);

    tokens.set_position(saved_position);

    return result;
}

std::unique_ptr<Statement> StatementParser::index_assignment()
{
    std::string name = tokens.current().text;

    tokens.match(TokenType::Identifier);

    if (!tokens.match(TokenType::LeftBracket))
    {
        throw SyntaxError("Expected '[' after identifier", get_location());
    }

    auto index = expressions.parse();

    if (!tokens.match(TokenType::RightBracket))
    {
        throw SyntaxError("Expected ']' after index", get_location());
    }

    if (!tokens.match(TokenType::Equal))
    {
        throw SyntaxError("Expected '=' after index", get_location());
    }

    auto value = expressions.parse();

    return std::make_unique<IndexAssignmentStatement>(name, std::move(index), std::move(value), get_location());
}

SourceLocation StatementParser::get_location() const
{
    return SourceLocation{tokens.current().line, tokens.current().column};
}

SourceLocation StatementParser::get_prev_location() const
{
    return SourceLocation{tokens.prev().line, tokens.prev().column};
}
