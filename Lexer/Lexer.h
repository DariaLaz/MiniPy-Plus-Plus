#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "Token.h"
#include "Utils/SourceLocation.h"

static const std::unordered_map<char, TokenType> simple_symbols = {
    {'+', TokenType::Plus},
    {'-', TokenType::Minus},
    {'*', TokenType::Star},
    {'/', TokenType::Slash},
    {',', TokenType::Comma},
    {':', TokenType::Colon},
};

static const std::unordered_map<std::string, TokenType> keyword_map = {
    {"if", TokenType::If},
    {"elif", TokenType::Elif},
    {"else", TokenType::Else},
    {"True", TokenType::True},
    {"False", TokenType::False},
    {"while", TokenType::While},
    {"for", TokenType::For},
    {"in", TokenType::In},
    {"def", TokenType::Def},
    {"return", TokenType::Return},
    {"break", TokenType::Break},
    {"continue", TokenType::Continue},
    {"and", TokenType::And},
    {"or", TokenType::Or},
    {"not", TokenType::Not},
    {"None", TokenType::None},
};

class Lexer
{
public:
    Lexer(const std::string &source);

    std::vector<Token> tokenize();
    Token make_token(TokenType type, const std::string &text) const;

private:
    std::string source;
    int curr = 0;
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
