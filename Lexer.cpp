#include "Lexer.h"
#include <stdexcept>

Lexer::Lexer(const std::string &source) : source(source) {}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;

    while (curr < source.size())
    {
        if (line_start)
        {
            handle_indentation(tokens);

            if (curr >= source.size())
            {
                break;
            }
        }
        char curr_char = static_cast<unsigned char>(source[curr]);

        if (curr_char == '\n')
        {
            tokens.push_back({TokenType::Newline, "\\n"});

            curr++;
            line_start = true;
            continue;
        }

        if (std::isspace(curr_char))
        {
            curr++;
            continue;
        }

        if (std::isdigit(curr_char))
        {
            tokens.push_back(tokenize_number());
            continue;
        }

        if (is_identifier())
        {
            tokens.push_back(tokenize_identifier_or_keyword());
            continue;
        }

        tokens.push_back(tokenize_symbol());
        curr++;
    }

    if (tokens.empty() || tokens.back().type != TokenType::Newline)
    {
        tokens.push_back({TokenType::Newline, "\\n"});
    }

    while (indent_levels.size() > 1)
    {
        indent_levels.pop_back();
        tokens.push_back({TokenType::Dedent, ""});
    }

    tokens.push_back({TokenType::End, ""});

    return tokens;
}

Token Lexer::tokenize_number()
{
    std::string number = "";
    while (curr < source.size() && is_digit())
    {
        number += source[curr];
        curr++;
    }

    return {TokenType::Number, number};
}

Token Lexer::tokenize_symbol()
{
    switch (static_cast<unsigned char>(source[curr]))
    {
    case '+':
        return {TokenType::Plus, "+"};
    case '-':
        return {TokenType::Minus, "-"};
    case '*':
        return {TokenType::Star, "*"};
    case '/':
        return {TokenType::Slash, "/"};
    case '(':
        return {TokenType::LeftParen, "("};
    case ')':
        return {TokenType::RightParen, ")"};
    case ':':
        return {TokenType::Colon, ":"};
    case '=':
        if (check_next('='))
        {
            curr++;
            return {TokenType::EqualEqual, "=="};
        }
        return {TokenType::Equal, "="};

    case '<':
        if (check_next('='))
        {
            curr++;
            return {TokenType::LessEqual, "<="};
        }
        return {TokenType::Less, "<"};

    case '>':
        if (check_next('='))
        {
            curr++;
            return {TokenType::GreaterEqual, ">="};
        }
        return {TokenType::Greater, ">"};

    case '!':
        if (check_next('='))
        {
            curr++;
            return {TokenType::NotEqual, "!="};
        }

    default:
        throw std::runtime_error(
            "Unexpected character: " + std::string(1, source[curr]));
    }
}

Token Lexer::tokenize_identifier_or_keyword()
{
    std::string identifier;

    while (curr < source.size() && is_identifier())
    {
        identifier += source[curr];
        curr++;
    }

    if (identifier == "if")
        return {TokenType::If, identifier};

    if (identifier == "elif")
        return {TokenType::Elif, identifier};

    if (identifier == "else")
        return {TokenType::Else, identifier};

    if (identifier == "True")
        return {TokenType::True, identifier};

    if (identifier == "False")
        return {TokenType::False, identifier};

    if (identifier == "while")
        return {TokenType::While, identifier};

    return {TokenType::Identifier, identifier};
}

bool Lexer::is_identifier() const
{
    return std::isalpha(static_cast<unsigned char>(source[curr])) ||
           source[curr] == '_';
}

bool Lexer::is_digit() const
{
    return std::isdigit(static_cast<unsigned char>(source[curr]));
}

bool Lexer::check_next(char ch) const
{
    if (curr == source.size() - 1)
    {
        return false;
    }
    return source[curr + 1] == ch;
}

void Lexer::handle_indentation(std::vector<Token> &tokens)
{
    int spaces = 0;

    while (curr < source.size() && source[curr] == ' ')
    {
        spaces++;
        curr++;
    }

    if (curr < source.size() && source[curr] == '\t')
    {
        throw std::runtime_error("Tabs are not supported for indentation");
    }

    if (curr >= source.size() || source[curr] == '\n')
    {
        return;
    }

    int current_indent = indent_levels.back();

    if (spaces > current_indent)
    {
        indent_levels.push_back(spaces);

        tokens.push_back({TokenType::Indent, ""});
    }
    else if (spaces < current_indent)
    {
        while (indent_levels.size() > 1 && spaces < indent_levels.back())
        {
            indent_levels.pop_back();
            tokens.push_back({TokenType::Dedent, ""});
        }

        if (spaces != indent_levels.back())
        {
            throw std::runtime_error("Inconsistent indentation");
        }
    }

    line_start = false;
}