#pragma once
#include <vector>
#include <memory>

#include "Token.h"
#include "AST/Expression/Expression.h"
#include "AST/Statement/Statement.h"

class Parser
{
public:
    Parser(const std::vector<Token> &tokens);
    std::unique_ptr<Statement> parse();

    int value;

private:
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> primary();

    const Token &current() const;
    const Token &prev() const;
    bool check(TokenType type) const;
    bool check_next(TokenType type) const;
    bool match(TokenType type);

    int curr = 0;
    const std::vector<Token> &tokens;
};