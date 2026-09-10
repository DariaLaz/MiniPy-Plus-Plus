#include <stdexcept>
#include <string>
#include <vector>

#include <stdexcept>
#include <string>

#include "Lexer/Lexer.h"
#include "Errors/SyntaxError.h"

namespace
{
    void expect_token(
        const Token &token,
        TokenType expected_type,
        const std::string &expected_text,
        int expected_line,
        int expected_column)
    {
        if (token.type != expected_type || token.text != expected_text || token.line != expected_line || token.column != expected_column)
        {
            throw std::runtime_error("Lexer token assertion failed for '" + expected_text + "'");
        }
    }

    void test_basic_tokens()
    {
        Lexer lexer("x = 10\n");

        auto tokens = lexer.tokenize();

        expect_token(tokens[0], TokenType::Identifier, "x", 1, 1);
        expect_token(tokens[1], TokenType::Equal, "=", 1, 3);
        expect_token(tokens[2], TokenType::Number, "10", 1, 5);
        expect_token(tokens[3], TokenType::Newline, "\\n", 1, 7);

        if (tokens[4].type != TokenType::End)
        {
            throw std::runtime_error("Lexer did not emit End token");
        }
    }

    void test_identifier_with_digits()
    {
        Lexer lexer("value123 = 5\n");

        auto tokens = lexer.tokenize();

        expect_token(tokens[0], TokenType::Identifier, "value123", 1, 1);
    }

    void test_keywords()
    {
        Lexer lexer("if else elif while for in def return break continue and or not True False None\n");

        auto tokens = lexer.tokenize();

        const std::vector<TokenType> expected = {
            TokenType::If,
            TokenType::Else,
            TokenType::Elif,
            TokenType::While,
            TokenType::For,
            TokenType::In,
            TokenType::Def,
            TokenType::Return,
            TokenType::Break,
            TokenType::Continue,
            TokenType::And,
            TokenType::Or,
            TokenType::Not,
            TokenType::True,
            TokenType::False,
            TokenType::None};

        for (std::size_t i = 0; i < expected.size(); ++i)
        {
            if (tokens[i].type != expected[i])
            {
                throw std::runtime_error("Lexer keyword test failed");
            }
        }
    }

    void test_comparison_operators()
    {
        Lexer lexer(
            "a == b\n"
            "a != b\n"
            "a <= b\n"
            "a >= b\n"
            "a < b\n"
            "a > b\n");

        auto tokens = lexer.tokenize();

        std::vector<TokenType> operators;

        for (const auto &token : tokens)
        {
            switch (token.type)
            {
            case TokenType::EqualEqual:
            case TokenType::NotEqual:
            case TokenType::LessEqual:
            case TokenType::GreaterEqual:
            case TokenType::Less:
            case TokenType::Greater:
                operators.push_back(token.type);
                break;

            default:
                break;
            }
        }

        const std::vector<TokenType> expected = {
            TokenType::EqualEqual,
            TokenType::NotEqual,
            TokenType::LessEqual,
            TokenType::GreaterEqual,
            TokenType::Less,
            TokenType::Greater};

        if (operators != expected)
        {
            throw std::runtime_error("Lexer comparison operator test failed");
        }
    }

    void test_string_escapes()
    {
        Lexer lexer(R"("hello\nworld\t\"test\"\\")");

        auto tokens = lexer.tokenize();

        if (tokens[0].type != TokenType::String)
        {
            throw std::runtime_error("Expected String token");
        }

        const std::string expected = "hello\nworld\t\"test\"\\";

        if (tokens[0].text != expected)
        {
            throw std::runtime_error("Lexer string escape test failed");
        }
    }

    void test_indentation()
    {
        Lexer lexer(
            "if True:\n"
            "    x = 1\n"
            "x = 2\n");

        auto tokens = lexer.tokenize();

        int indent_count = 0;
        int dedent_count = 0;

        for (const auto &token : tokens)
        {
            if (token.type == TokenType::Indent)
            {
                indent_count++;
            }

            if (token.type == TokenType::Dedent)
            {
                dedent_count++;
            }
        }

        if (indent_count != 1)
        {
            throw std::runtime_error("Expected exactly one Indent token");
        }

        if (dedent_count != 1)
        {
            throw std::runtime_error("Expected exactly one Dedent token");
        }
    }

    void test_multiline_grouping()
    {
        Lexer lexer(
            "x = [\n"
            "    1,\n"
            "    2,\n"
            "    3\n"
            "]\n");

        auto tokens = lexer.tokenize();

        int newline_count = 0;
        int indent_count = 0;

        for (const auto &token : tokens)
        {
            if (token.type == TokenType::Newline)
            {
                newline_count++;
            }

            if (token.type == TokenType::Indent)
            {
                indent_count++;
            }
        }

        if (newline_count != 1)
        {
            throw std::runtime_error("Lexer emitted newline inside grouping");
        }

        if (indent_count != 0)
        {
            throw std::runtime_error("Lexer emitted indentation inside grouping");
        }
    }

    void test_unexpected_character()
    {
        try
        {
            Lexer lexer("x = @\n");
            lexer.tokenize();
        }
        catch (const SyntaxError &)
        {
            return;
        }

        throw std::runtime_error("Expected SyntaxError for unexpected character");
    }

    void test_unterminated_string()
    {
        try
        {
            Lexer lexer("\"hello");
            lexer.tokenize();
        }
        catch (const SyntaxError &)
        {
            return;
        }

        throw std::runtime_error("Expected SyntaxError for unterminated string");
    }

    void test_slice_tokens()
    {
        Lexer lexer("x[1:3]\n");
        auto tokens = lexer.tokenize();

        if (tokens[0].type != TokenType::Identifier)
            throw std::runtime_error("Expected Identifier");

        if (tokens[1].type != TokenType::LeftBracket)
            throw std::runtime_error("Expected LeftBracket");

        if (tokens[2].type != TokenType::Number)
            throw std::runtime_error("Expected Number");

        if (tokens[3].type != TokenType::Colon)
            throw std::runtime_error("Expected Colon");

        if (tokens[4].type != TokenType::Number)
            throw std::runtime_error("Expected Number");

        if (tokens[5].type != TokenType::RightBracket)
            throw std::runtime_error("Expected RightBracket");
    }

}

void run_lexer_tests()
{
    test_slice_tokens();
    test_basic_tokens();
    test_identifier_with_digits();
    test_keywords();
    test_comparison_operators();
    test_string_escapes();
    test_indentation();
    test_multiline_grouping();
    test_unexpected_character();
    test_unterminated_string();
}