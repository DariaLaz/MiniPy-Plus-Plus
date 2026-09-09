#pragma once
#include <vector>
#include <memory>

#include "Token.h"
#include "AST/Statement/Statement.h"
#include "Parser/TokenStream.h"
#include "Parser/ExpressionParser.h"

class StatementParser
{
public:
    StatementParser(
        TokenStream &tokens,
        ExpressionParser &expressions);

    std::unique_ptr<Statement> parse_program();

private:
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> function_statement();
    std::unique_ptr<Statement> return_statement();
    std::unique_ptr<Statement> simple_statement();
    std::unique_ptr<Statement> for_statement();
    std::unique_ptr<Statement> if_statement();
    std::unique_ptr<Statement> while_statement();
    std::unique_ptr<Statement> block();

    std::unique_ptr<Statement> index_assignment();

    SourceLocation get_location() const;

    TokenStream &tokens;
    ExpressionParser &expressions;
};