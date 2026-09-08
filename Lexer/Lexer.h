#pragma once

#include <string>
#include <vector>

#include "Token.h"
#include <unordered_map>

inline const std::unordered_map<std::string, TokenType> keyword_map = {
    {"if", TokenType::If},
    {"elif", TokenType::Elif},
    {"else", TokenType::Else},
    {"True", TokenType::True},
    {"False", TokenType::False},
    {"while", TokenType::While},
    {"print", TokenType::Print},
    {"for", TokenType::For},
    {"in", TokenType::In},
    {"def", TokenType::Def},
    {"return", TokenType::Return},
    {"break", TokenType::Break},
    {"continue", TokenType::Continue},
};

class Lexer
{
public:
    Lexer(const std::string &source);

    std::vector<Token> tokenize();

private:
    std::string source;
    int curr = 0;

    std::vector<int> indent_levels = {0};
    bool line_start = true;

    Token tokenize_symbol();
    Token tokenize_number();
    Token tokenize_identifier_or_keyword();
    Token tokenize_string();

    bool is_identifier() const;
    bool is_digit() const;

    bool check_next(char ch) const;

    void handle_indentation(std::vector<Token> &tokens);
};