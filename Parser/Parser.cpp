#include <stdexcept>
#include <string>
#include <utility>

#include "AST/Expression/BinaryExpression.h"
#include "AST/Expression/BoolExpression.h"
#include "AST/Expression/NumberExpression.h"
#include "AST/Expression/UnaryExpression.h"
#include "AST/Expression/VariableExpression.h"
#include "AST/Statement/AssignmentStatement.h"
#include "AST/Statement/BlockStatement.h"
#include "AST/Statement/ExpressionStatement.h"
#include "AST/Statement/IfStatement.h"
#include "AST/Statement/WhileStatement.h"
#include "Parser.h"

Parser::Parser(
    const std::vector<Token> &input)
    : tokens(input),
      expression_parser(tokens),
      statement_parser(
          tokens,
          expression_parser)
{
}

std::unique_ptr<Statement> Parser::parse()
{
    return statement_parser.parse_program();
}