#include "Lexer/Lexer.h"

#include "Errors/IndentationError.h"
#include "Errors/SyntaxError.h"

#include <optional>
#include <stdexcept>

namespace
{
    const std::unordered_map<char, TokenType> simple_symbols = {
        {'+', TokenType::Plus},
        {'-', TokenType::Minus},
        {'*', TokenType::Star},
        {'/', TokenType::Slash},
        {',', TokenType::Comma},
        {':', TokenType::Colon},
    };

    const std::unordered_map<std::string, TokenType> keyword_map = {
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
} // namespace

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
        token_start_line = line;
        token_start_column = column;
        auto curr_char = static_cast<unsigned char>(source[curr]);

        if (curr_char == '\n')
        {
            consume();
            if (grouping_depth > 0)
            {
                // newline inside (), [] or {}
                continue;
            }

            tokens.push_back(make_token(TokenType::Newline, "\\n"));

            line_start = true;
            continue;
        }

        if (std::isspace(curr_char))
        {
            consume();
            continue;
        }

        if (std::isdigit(curr_char))
        {
            tokens.push_back(tokenize_number());
            continue;
        }

        if (is_identifier_start())
        {
            tokens.push_back(tokenize_identifier_or_keyword());
            continue;
        }

        if (curr_char == '"')
        {
            tokens.push_back(tokenize_string());
            continue;
        }

        tokens.push_back(tokenize_symbol());
    }

    if (tokens.empty() || tokens.back().type != TokenType::Newline)
    {
        tokens.push_back(make_token(TokenType::Newline, "\\n"));
    }

    while (indent_levels.size() > 1)
    {
        indent_levels.pop_back();
        tokens.push_back(make_token(TokenType::Dedent, ""));
    }

    tokens.push_back(make_token(TokenType::End, ""));

    return tokens;
}

Token Lexer::tokenize_number()
{
    std::string number = "";
    while (curr < source.size() && is_digit())
    {
        number += consume();
    }

    return make_token(TokenType::Number, number);
}

Token Lexer::tokenize_symbol()
{
    std::optional<Token> simple_symbol = tokenize_simple_symbol();
    if (simple_symbol.has_value())
    {
        return simple_symbol.value();
    }

    std::optional<Token> grouping_symbol = tokenize_grouping_symbol();
    if (grouping_symbol.has_value())
    {
        return grouping_symbol.value();
    }

    std::optional<Token> comparing_symbol = tokenize_comparing_symbol();
    if (comparing_symbol.has_value())
    {
        return comparing_symbol.value();
    }

    throw SyntaxError(std::string("Unexpected character: ") + source[curr], get_location());
}

std::optional<Token> Lexer::tokenize_grouping_symbol()
{
    switch (source[curr])
    {
    case '(':
        grouping_depth++;
        consume();
        return make_token(TokenType::LeftParen, "(");

    case ')':
        grouping_depth--;
        consume();
        return make_token(TokenType::RightParen, ")");

    case '[':
        grouping_depth++;
        consume();
        return make_token(TokenType::LeftBracket, "[");

    case ']':
        grouping_depth--;
        consume();
        return make_token(TokenType::RightBracket, "]");

    case '{':
        grouping_depth++;
        consume();
        return make_token(TokenType::LeftBrace, "{");

    case '}':
        grouping_depth--;
        consume();
        return make_token(TokenType::RightBrace, "}");
    }

    return std::nullopt;
}

std::optional<Token> Lexer::tokenize_comparing_symbol()
{

    switch (source[curr])
    {
    case '=':
        consume();

        if (curr < source.size() && source[curr] == '=')
        {
            consume();
            return make_token(TokenType::EqualEqual, "==");
        }

        return make_token(TokenType::Equal, "=");

    case '<':
        consume();

        if (curr < source.size() && source[curr] == '=')
        {
            consume();
            return make_token(TokenType::LessEqual, "<=");
        }

        return make_token(TokenType::Less, "<");

    case '>':
        consume();

        if (curr < source.size() && source[curr] == '=')
        {
            consume();
            return make_token(TokenType::GreaterEqual, ">=");
        }

        return make_token(TokenType::Greater, ">");

    case '!':
        consume();

        if (curr < source.size() && source[curr] == '=')
        {
            consume();
            return make_token(TokenType::NotEqual, "!=");
        }

        throw SyntaxError("Unexpected character: !", get_location());
    }

    return std::nullopt;
}

std::optional<Token> Lexer::tokenize_simple_symbol()
{
    char ch = source[curr];

    if (simple_symbols.find(ch) != simple_symbols.end())
    {
        consume();
        return make_token(simple_symbols.at(ch), std::string(1, ch));
    }

    return std::nullopt;
}

Token Lexer::tokenize_identifier_or_keyword()
{
    std::string identifier;

    while (curr < source.size() && is_identifier_part())
    {
        identifier += consume();
    }

    if (keyword_map.find(identifier) != keyword_map.end())
    {
        return make_token(keyword_map.at(identifier), identifier);
    }

    return make_token(TokenType::Identifier, identifier);
}

Token Lexer::tokenize_string()
{
    consume();
    std::string value;

    while (curr < source.size())
    {
        if (source[curr] == '"')
        {
            consume();
            return make_token(TokenType::String, value);
        }

        if (source[curr] == '\n')
        {
            throw SyntaxError("Unterminated string", get_location());
        }

        if (source[curr] == '\\')
        {
            consume();

            if (curr >= source.size())
            {
                throw SyntaxError("Unterminated escape sequence", get_location());
            }

            char escaped = source[curr];

            switch (escaped)
            {
            case 'n':
                value += '\n';
                break;

            case 't':
                value += '\t';
                break;

            case '"':
                value += '"';
                break;

            case '\\':
                value += '\\';
                break;

            default:
                throw SyntaxError("Unknown escape sequence", get_location());
            }

            consume();
            continue;
        }

        value += consume();
    }

    throw SyntaxError("Unterminated string", get_location());
}

bool Lexer::is_identifier_start() const
{
    return std::isalpha(source[curr]) || source[curr] == '_';
}

bool Lexer::is_identifier_part() const
{
    return std::isalnum(source[curr]) || source[curr] == '_';
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
        consume();
    }

    if (curr < source.size() && source[curr] == '\t')
    {
        throw SyntaxError("Tabs are not supported for indentation", get_location());
    }

    if (curr >= source.size() || source[curr] == '\n')
    {
        return;
    }

    int current_indent = indent_levels.back();

    if (spaces > current_indent)
    {
        indent_levels.push_back(spaces);

        tokens.push_back(make_token(TokenType::Indent, ""));
    }
    else if (spaces < current_indent)
    {
        while (indent_levels.size() > 1 && spaces < indent_levels.back())
        {
            indent_levels.pop_back();
            tokens.push_back(make_token(TokenType::Dedent, ""));
        }

        if (spaces != indent_levels.back())
        {
            throw IndentationError("Inconsistent indentation", get_location());
        }
    }

    line_start = false;
}

char Lexer::consume()
{
    char ch = source[curr];

    curr++;

    if (ch == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }

    return ch;
}

Token Lexer::make_token(TokenType type, const std::string &text) const
{
    return Token{type, text, token_start_line, token_start_column};
}

SourceLocation Lexer::get_location() const
{
    return SourceLocation{token_start_line, token_start_column};
}