#pragma once
#include <vector>
#include <memory>

#include "Token.h"
#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"

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
    int increment(int i = 1) { return curr += i; }

private:
    int curr = 0;
    const std::vector<Token> &tokens;
};