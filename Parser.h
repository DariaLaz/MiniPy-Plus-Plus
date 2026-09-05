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
    std::unique_ptr<Statement> block();
    std::unique_ptr<Statement> statement();
    std::unique_ptr<Statement> simple_statement();
    std::unique_ptr<Statement> if_statement();
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> primary();

    std::unique_ptr<Expression> binary_expr(
        std::unique_ptr<Expression> (Parser::*expr)(),
        std::initializer_list<TokenType> types);

    const Token &current() const;
    const Token &prev() const;
    bool check(TokenType type) const;
    bool check(std::initializer_list<TokenType> types) const;
    bool check_next(TokenType type) const;
    bool match(TokenType type);

    int curr = 0;
    const std::vector<Token> &tokens;
};