#pragma once
#include <vector>

#include "TokenStream.h"

class ExpressionParser
{
public:
    ExpressionParser(TokenStream &tokens) : tokens(tokens) {};
    std::unique_ptr<Expression> parse();

private:
    std::unique_ptr<Expression> expression();
    std::unique_ptr<Expression> equality();
    std::unique_ptr<Expression> comparison();
    std::unique_ptr<Expression> term();
    std::unique_ptr<Expression> factor();
    std::unique_ptr<Expression> unary();
    std::unique_ptr<Expression> postfix();
    std::unique_ptr<Expression> list();
    std::unique_ptr<Expression> primary();

    std::unique_ptr<Expression> binary_expr(
        std::unique_ptr<Expression> (ExpressionParser::*expr)(),
        std::initializer_list<TokenType> types);

    TokenStream &tokens;
};