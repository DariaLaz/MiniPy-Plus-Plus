#pragma once
#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"
#include "Lexer/Token.h"

#include <memory>
#include <vector>

class TokenStream
{
public:
  TokenStream(const std::vector<Token> &tokens);

  const Token &current() const;
  const Token &prev() const;
  bool check(TokenType type) const;
  bool check(std::initializer_list<TokenType> types) const;
  bool check_next(TokenType type) const;

  bool match(TokenType type);
  int increment(int i = 1);

  int position() const;
  void set_position(int pos);

private:
  int curr = 0;
  const std::vector<Token> &tokens;
};