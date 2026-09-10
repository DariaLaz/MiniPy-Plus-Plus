#pragma once

#include "Lexer/Token.h"
#include "Utils/SourceLocation.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Lexer
{
public:
  Lexer(const std::string &source);

  std::vector<Token> tokenize();
  Token make_token(TokenType type, const std::string &text) const;

private:
  std::string source;
  std::size_t curr = 0;
  int grouping_depth = 0;

  int line = 1;
  int column = 1;

  int token_start_line = 1;
  int token_start_column = 1;

  std::vector<int> indent_levels = {0};
  bool line_start = true;

  Token tokenize_symbol();
  Token tokenize_number();
  Token tokenize_identifier_or_keyword();
  Token tokenize_string();

  bool is_identifier_part() const;
  bool is_identifier_start() const;
  bool is_digit() const;

  bool check_next(char ch) const;

  void handle_indentation(std::vector<Token> &tokens);

  char consume();

  std::optional<Token> tokenize_simple_symbol();
  std::optional<Token> tokenize_grouping_symbol();
  std::optional<Token> tokenize_comparing_symbol();

  SourceLocation get_location() const;
};
