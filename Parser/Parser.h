#pragma once
#include "Lexer/Token.h"
#include "Parser/ExpressionParser.h"
#include "Parser/StatementParser.h"
#include "Parser/TokenStream.h"

#include <memory>
#include <vector>

class Parser
{
public:
  Parser(const std::vector<Token> &tokens);

  std::unique_ptr<Statement> parse();

private:
  TokenStream tokens;
  ExpressionParser expression_parser;
  StatementParser statement_parser;
};