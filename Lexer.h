#pragma once

#include <string>
#include <vector>

#include "Token.h"

class Lexer
{
public:
    Lexer(const std::string &source);

    std::vector<Token> tokenize();

private:
    std::string source;
    int curr = 0;

    Token tokenize_symbol();
    Token tokenize_number();
    Token tokenize_identifier();

    bool is_identifier() const;
    bool is_digit() const;

    bool check_next(char ch) const;
};